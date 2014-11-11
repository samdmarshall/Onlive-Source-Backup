/* index.c: module index file shared functions for modprobe and depmod
    Copyright (C) 2008  Alan Jenkins <alan-jenkins@tuffmail.co.uk>.

    These programs are free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with these programs.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fnmatch.h>

#include "logging.h"
#include "index.h"

/*
 * Index abstract data type (used only by depmod)
 */

struct index_node *index_create()
{
	struct index_node *node;

	node = NOFAIL(calloc(sizeof(struct index_node), 1));
	node->prefix = NOFAIL(strdup(""));
	node->first = INDEX_CHILDMAX;
	
	return node;
}

void index_destroy(struct index_node *node)
{
	int c;
	
	for (c = node->first; c <= node->last; c++) {
		struct index_node *child = node->children[c];
		
		if (child)
			index_destroy(child);
	}
	free(node->prefix);
	free(node);
}

static void index__checkstring(const char *str)
{
	int spaces = 0;
	int i;
	
	for (i = 0; str[i]; i++) {
		int ch = str[i];
		
		if (ch >= INDEX_CHILDMAX)
			fatal("Module index: bad character '%c'=0x%x - only 7-bit ASCII is supported:"
			      "\n%s\n", (char) ch, (int) ch, str);
		
		if (ch == ' ')
			spaces++;
	}
	
	if (!spaces)
		fatal("Module index: internal error - missing space (key/value separator)"
		      "\n%s\n", str);
}

int index_insert(struct index_node *node, const char *str)
{
	int i = 0; /* index within str */
	int ch;
	int duplicate = 0;
	
	index__checkstring(str);
	
	while(1) {
		int j; /* index within node->prefix */
	
		/* Ensure node->prefix is a prefix of &str[i].
		   If it is not already, then we must split node. */
		for (j = 0; node->prefix[j]; j++) {
			ch = node->prefix[j];
		
			if (ch != str[i+j]) {
				char *prefix = node->prefix;
				struct index_node *n;
				
				/* New child is copy of node with prefix[j+1..N] */
				n = NOFAIL(calloc(sizeof(struct index_node), 1));
				memcpy(n, node, sizeof(struct index_node));
				n->prefix = NOFAIL(strdup(&prefix[j+1]));
				
				/* Parent has prefix[0..j], child at prefix[j] */
				memset(node, 0, sizeof(struct index_node));
				prefix[j] = '\0';
				node->prefix = prefix;
				node->first = ch;
				node->last = ch;
				node->children[ch] = n;
				
				break;
			}
		}
		/* j is now length of node->prefix */
		i += j;
	
		ch = str[i];
		if(ch == '\0') {
			if (node->isendpoint)
				duplicate = 1;
			
			node->isendpoint = 1;
			return duplicate;
		}
		
		if (!node->children[ch]) {
			struct index_node *child;
		
			if (ch < node->first)
				node->first = ch;
			if (ch > node->last)
				node->last = ch;
			node->children[ch] = NOFAIL(calloc(sizeof(struct index_node), 1));
			
			child = node->children[ch];
			child->prefix = NOFAIL(strdup(&str[i+1]));
			child->isendpoint = 1;
			child->first = INDEX_CHILDMAX;
			
			return duplicate;
		}
		
		/* Descend into child node and continue */
		node = node->children[ch];
		i++;
	}
}

static int index__haschildren(const struct index_node *node)
{
	return node->first < INDEX_CHILDMAX;
}

/* Recursive post-order traversal

   Pre-order would make for better read-side buffering / readahead / caching.
   (post-order means you go backwards in the file as you descend the tree).
   However, index reading is already fast enough.
   Pre-order is simpler for writing, and depmod is already slow.
 */
static uint32_t index_write__node(const struct index_node *node, FILE *out)
{
 	uint32_t *child_offs = NULL;
 	int child_count = 0;
	long offset;
	
	if (!node)
		return 0;
	
	/* Write children and save their offsets */
	if (index__haschildren(node)) {
		const struct index_node *child;
		int i;
		
		child_count = node->last - node->first + 1;
		child_offs = NOFAIL(malloc(child_count * sizeof(uint32_t)));
		
		for (i = 0; i < child_count; i++) {
			child = node->children[node->first + i];
			child_offs[i] = htonl(index_write__node(child, out));
		}
	}
		
	/* Now write this node */
	offset = ftell(out);
	
	if (node->prefix[0]) {
		fputs(node->prefix, out);
		fputc('\0', out);
		offset |= INDEX_NODE_PREFIX;
	}
		
	if (child_count) {
		fputc(node->first, out);
		fputc(node->last, out);
		fwrite(child_offs, sizeof(uint32_t), child_count, out);
		free(child_offs);
		offset |= INDEX_NODE_CHILDS;
	}
	
	if (node->isendpoint)
		offset |= INDEX_NODE_ENDPOINT;
	
	return offset;
}

void index_write(const struct index_node *node, FILE *out)
{
	long initial_offset, final_offset;
	uint32_t u;
	
	u = htonl(INDEX_MAGIC);
	fwrite(&u, sizeof(u), 1, out);
	
	/* First word is reserved for the offset of the root node */
	initial_offset = ftell(out);
	u = 0;
	fwrite(&u, sizeof(uint32_t), 1, out);
	
	/* Dump trie */
	u = htonl(index_write__node(node, out));
	
	/* Update first word */
	final_offset = ftell(out);
	fseek(out, initial_offset, SEEK_SET);
	fwrite(&u, sizeof(uint32_t), 1, out);
	fseek(out, final_offset, SEEK_SET);
}



static void read_error()
{
	fatal("Module index: unexpected error: %s\n"
			"Try re-running depmod\n", errno ? strerror(errno) : "EOF");
}

static int read_char(FILE *in)
{
	int ch;

	errno = 0;
	ch = getc_unlocked(in);
	if (ch == EOF)
		read_error();
	return ch;
}

static uint32_t read_long(FILE *in)
{
	uint32_t l;

	errno = 0;
	if (fread(&l, sizeof(uint32_t), 1, in) <= 0)
		read_error();
	return ntohl(l);
}

/*
 * Buffer abstract data type
 *
 * Used internally to store the current path during tree traversal.
 * They help build wildcard key strings to pass to fnmatch(),
 * as well as building values of matching keys.
 */

struct buffer {
	char *bytes;
	unsigned size;
	unsigned used;
};

static void buf__realloc(struct buffer *buf, unsigned size)
{
	if (size > buf->size) {
		buf->bytes = NOFAIL(realloc(buf->bytes, size));
		buf->size = size;
	}
}

static struct buffer *buf_create()
{
	struct buffer *buf;
	 
	buf = NOFAIL(calloc(sizeof(struct buffer), 1));
	buf__realloc(buf, 256);
	return buf;
}

static void buf_destroy(struct buffer *buf)
{
	free(buf->bytes);
	free(buf);
}

/* Destroy buffer and return a copy as a C string */
static char *buf_detach(struct buffer *buf)
{
	char *bytes;

	bytes = NOFAIL(realloc(buf->bytes, buf->used + 1));
	bytes[buf->used] = '\0';

	free(buf);
	return bytes;
}

/* Return a C string owned by the buffer
   (invalidated if the buffer is changed).
 */
static const char *buf_str(struct buffer *buf)
{
	buf__realloc(buf, buf->used + 1);
	buf->bytes[buf->used] = '\0';
	return buf->bytes;
}

static int buf_fwrite(struct buffer *buf, FILE *out)
{
	return fwrite(buf->bytes, 1, buf->used, out);
}

static struct index_value *add_value(struct buffer *buf,
				     struct index_value *values)
{
	struct index_value *n = malloc(sizeof(struct index_value) + buf->used + 1);

	memcpy(n->value, buf->bytes, buf->used);
	n->value[buf->used] = '\0';
	n->next = values;

	return n;
}

static void buf_popchars(struct buffer *buf, unsigned n)
{
	buf->used -= n;
}

static void buf_pushchar(struct buffer *buf, char ch)
{
	buf__realloc(buf, buf->used + 1);
	buf->bytes[buf->used] = ch;
	buf->used++;
}

static unsigned buf_pushchars(struct buffer *buf, const char *str)
{
	unsigned i = 0;
	int ch;

	while ((ch = str[i])) {
		buf_pushchar(buf, ch);
		i++;
	}
	return i;
}

/* like buf_pushchars(), but the string comes from a file */
static unsigned buf_freadchars(struct buffer *buf, FILE *in)
{
	unsigned i = 0;
	int ch;

	while ((ch = read_char(in))) {
		buf_pushchar(buf, ch);
		i++;
	}

	return i;
}

static void buf_popchar(struct buffer *buf)
{
	buf->used--;
}

/*
 * Index file searching (used only by modprobe)
 */

struct index_node_f {
	FILE *file;
	char *prefix;		/* path compression */
	char isendpoint;	/* does this node represent a string? */
	unsigned char first;	/* range of child nodes */
	unsigned char last;
	uint32_t children[0];
};

static void index_fatal(const char *why)
{
	fatal("Module index corrupt: %s\n"
	      "Try re-running depmod\n", why);
}

static struct index_node_f *index_read(FILE *in, uint32_t offset)
{
	struct index_node_f *node;
	char *prefix;
	int i, child_count = 0;

	if ((offset & INDEX_NODE_MASK) == 0)
		return NULL;

	fseek(in, offset & INDEX_NODE_MASK, SEEK_SET);
	
	if (offset & INDEX_NODE_PREFIX) {
		struct buffer *buf = buf_create();
		buf_freadchars(buf, in);
		prefix = buf_detach(buf);
	} else
		prefix = NOFAIL(strdup(""));
		
	if (offset & INDEX_NODE_CHILDS) {
		char first = read_char(in);
		char last = read_char(in);
		child_count = last - first + 1;
		
		node = NOFAIL(malloc(sizeof(struct index_node_f) +
				     sizeof(uint32_t) * child_count));
		
		node->first = first;
		node->last = last;

		for (i = 0; i < child_count; i++)
			node->children[i] = read_long(in);
	} else {
		node = NOFAIL(malloc(sizeof(struct index_node_f)));
		node->first = INDEX_CHILDMAX;
		node->last = 0;
	}
	
	node->prefix = prefix;
	node->isendpoint = !!(offset & INDEX_NODE_ENDPOINT);
	node->file = in;
	return node;
}

static void index_close(struct index_node_f *node)
{
	free(node->prefix);
	free(node);
}

static struct index_node_f *index_readchild(const struct index_node_f *parent,
					    int ch)
{
	if (parent->first <= ch && ch <= parent->last)
		return index_read(parent->file,
		                  parent->children[ch - parent->first]);
	else
		return NULL;
}

static struct index_node_f *index_readroot(FILE *in)
{
	uint32_t offset;

	fseek(in, 0, SEEK_SET);
	
	if (read_long(in) != INDEX_MAGIC)
		index_fatal("Bad magic number");

	offset = read_long(in);
	return index_read(in, offset);
}

/*
 * Dump all strings as lines in a plain text file.
 */

static void index_dump_node(struct index_node_f *node,
			    struct buffer *buf,
			    FILE *out,
			    const char *prefix)
{
	int ch, pushed;
	
	pushed = buf_pushchars(buf, node->prefix);
	
	if (node->isendpoint) {
		fputs(prefix, out);
		buf_fwrite(buf, out);
		fputc('\n', out);
	}
	
	for (ch = node->first; ch <= node->last; ch++) {
		struct index_node_f *child = index_readchild(node, ch);
		
		if (!child)
			continue;
			
		buf_pushchar(buf, ch);
		index_dump_node(child, buf, out, prefix);
		buf_popchar(buf);
	}
	
	buf_popchars(buf, pushed);
	index_close(node);
}

void index_dump(FILE *in, FILE *out, const char *prefix)
{
	struct index_node_f *root;
	struct buffer *buf;
	
	buf = buf_create();
	root = index_readroot(in);
	index_dump_node(root, buf, out, prefix);
	buf_destroy(buf);
}

/*
 * Search the index for a key
 *
 * Returns the value of the first match
 *
 * The calling convention for the inner functions
 * is for the callee to free the node argument (using index_close).
 */

/* Level 1: interface function */
char *index_search(FILE *in, const char *key);

/* Level 2: descend the tree */
static char *index_search__node(struct index_node_f *node, const char *key, int i);

/* Level 3: return the first value in the subtree.
   The first character of the value is node->prefix[j] */
static char *index_search__firstvalue(struct index_node_f *node, int j);

char *index_search(FILE *in, const char *key)
{
	struct index_node_f *root;
	char *value;
	
	root = index_readroot(in);
	value = index_search__node(root, key, 0);
	
	return value;
}

static char *index_search__node(struct index_node_f *node, const char *key, int i)
{
	struct index_node_f *child;
	int ch;
	int j;

	while(node) {
		for (j = 0; node->prefix[j]; j++) {
			ch = node->prefix[j];
			
			if (ch == ' ' && key[i+j] == '\0')
				return index_search__firstvalue(node, j+1);
			
			if (ch != key[i+j]) {
				index_close(node);
				return NULL;
			}
		}
		i += j;
		
		if (key[i] == '\0') {
			child = index_readchild(node, ' ');
			index_close(node);
			if (child)
				return index_search__firstvalue(child, 0);
			else
				return NULL;
		}
		
		child = index_readchild(node, key[i]);
		index_close(node);
		node = child;
		i++;
	}
	
	return NULL;
}

static char *index_search__firstvalue(struct index_node_f *node, int j)
{
	struct index_node_f *child;
	struct buffer *buf;
	 
	buf = buf_create();
	buf_pushchars(buf, &node->prefix[j]);

	while (!node->isendpoint) {
		if (node->first == INDEX_CHILDMAX)
			index_fatal("Index node is neither parent nor endpoint");
		
		buf_pushchar(buf, node->first);
		child = index_readchild(node, node->first);
		index_close(node);
		node = child;
		if (!node)
			index_fatal("Index node has non-existent first child");
 	
		buf_pushchars(buf, node->prefix);
	}
	
	index_close(node);
	return buf_detach(buf);
}

/*
 * Search the index for a key.  The index may contain wildcards.
 *
 * Returns a list of all the values of matching keys.
 */

/* Level 1: interface function */
struct index_value *index_searchwild(FILE *in, const char *key);

/* Level 2: descend the tree (until we hit a wildcard) */
static void index_searchwild__node(struct index_node_f *node,
				   struct buffer *buf,
				   const char *key, int i,
				   struct index_value **out);

/* Level 3: traverse a sub-keyspace which starts with a wildcard,
            generating all subkeys.
*/
static void index_searchwild__all(struct index_node_f *node, int j,
				  struct buffer *buf,
				  const char *subkey,
				  struct index_value **out);

/* Level 4: check for a subkey match */
static void index_searchwild__match(struct index_node_f *node, int j,
				    struct buffer *buf,
				    const char *subkey,
				    struct index_value **out);

/* Level 5: add all values for a matching subkey */
static void index_searchwild__allvalues(struct index_node_f *node, int j,
					struct buffer *value_buf,
					struct index_value **out);


struct index_value *index_searchwild(FILE *in, const char *key)
{
	struct index_node_f *root = index_readroot(in);
	struct buffer *buf = buf_create();
	struct index_value *out = NULL;
	
	index_searchwild__node(root, buf, key, 0, &out);
 	buf_destroy(buf);
	return out;
}

static void index_searchwild__node(struct index_node_f *node,
				   struct buffer *buf,
				   const char *key, int i,
				   struct index_value **out)
{
	struct index_node_f *child;
	int j;
	int ch;

	while(node) {
		for (j = 0; node->prefix[j]; j++) {
			ch = node->prefix[j];
			
			if (ch == '*' || ch == '?' || ch == '[') {
				index_searchwild__all(node, j, buf,
						      &key[i+j], out);
				return;
			}
			
			if (ch == ' ' && key[i+j] == '\0') {
				/* Note: buf matches &key[i+j] - both are empty */
				index_searchwild__match(node, j+1, buf,
						        &key[i+j], out);
				return;
			}
				
			if (ch != key[i+j]) {
				index_close(node);
				return;
			}
		}
		i += j;
		
		child = index_readchild(node, '*');
		if (child) {
			buf_pushchar(buf, '*');
			index_searchwild__all(child, 0, buf, &key[i], out);
			buf_popchar(buf);
		}
		
		child = index_readchild(node, '?');
		if (child) {
			buf_pushchar(buf, '*');
			index_searchwild__all(child, 0, buf, &key[i], out);
			buf_popchar(buf);
		}
		
		child = index_readchild(node, '[');
		if (child) {
			buf_pushchar(buf, '[');
			index_searchwild__all(child, 0, buf, &key[i], out);
			buf_popchar(buf);
		}
		
		if (key[i] == '\0') {
			child = index_readchild(node, ' ');
			index_close(node);
			/* Note: buf matches &key[i] - both are empty */
			if(child)
				index_searchwild__match(child, 0,
					                buf, &key[i], out);
			return;
		}
		
		child = index_readchild(node, key[i]);
		index_close(node);
		node = child;
		i++;
	}
}

static void index_searchwild__all(struct index_node_f *node, int j,
				  struct buffer *buf,
				  const char *subkey,
				  struct index_value **out)
{
	int pushed = 0;
	int ch;
	
	while (node->prefix[j]) {
		ch = node->prefix[j];
		
		if (ch == ' ') {
			index_searchwild__match(node, j+1, buf, subkey, out);
			goto out_popchars;
		}
		buf_pushchar(buf, ch);
		pushed++;
		j++;
	}

	for (ch = node->first; ch <= node->last; ch++) {
		struct index_node_f *child = index_readchild(node, ch);
		
		if (!child)
			continue;
			
		if (ch == ' ') {
			index_searchwild__match(child, 0, buf, subkey, out);
		} else {
			buf_pushchar(buf, ch);
			index_searchwild__all(child, 0, buf, subkey, out);
			buf_popchar(buf);
		}
	}
	
	index_close(node);
	
out_popchars:
	buf_popchars(buf, pushed);
}

static void index_searchwild__match(struct index_node_f *node, int j,
				    struct buffer *buf,
				    const char *subkey,
				    struct index_value **out)
{
	struct buffer *value_buf;
	const char *pattern;
	
	pattern = buf_str(buf);
	if (fnmatch(pattern, subkey, 0) == 0) {
		value_buf = buf_create();
		index_searchwild__allvalues(node, j, value_buf, out);
		buf_destroy(value_buf);
	} else
		index_close(node);
}

static void index_searchwild__allvalues(struct index_node_f *node, int j,
					struct buffer *value_buf,
					struct index_value **out)
{
	int pushed = 0;
	int ch;

	pushed = buf_pushchars(value_buf, &node->prefix[j]);

	if (node->isendpoint)
		*out = add_value(value_buf, *out);
	
	for (ch = node->first; ch <= node->last; ch++) {
		struct index_node_f *child = index_readchild(node, ch);
		
		if (!child)
			continue;
		
		buf_pushchar(value_buf, ch);
		index_searchwild__allvalues(child, 0, value_buf, out);
		buf_popchar(value_buf);
	}

	index_close(node);
	buf_popchars(value_buf, pushed);
}
