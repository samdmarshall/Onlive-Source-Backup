#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>

#include "logging.h"
#include "index.h"

static char *getline_wrapped(FILE *file, unsigned int *linenum)
{
	int size = 256;
	int i = 0;
	char *buf = NOFAIL(malloc(size));
	for(;;) {
		int ch = getc_unlocked(file);
		
		switch(ch) {
			case EOF:
				if (i == 0) {
					free(buf);
					return NULL;
				}
				/* else fall through */
			
			case '\n':
				if (linenum)
					(*linenum)++;
				if (i == size)
					buf = NOFAIL(realloc(buf, size + 1));
				buf[i] = '\0';
				return buf;
			
			case '\\':
				ch = getc_unlocked(file);
			
				if (ch == '\n') {
					if (linenum)
						(*linenum)++;
					continue;
				}
				/* else fall through */
		
			default:
				buf[i++] = ch;
	
				if (i == size) {
					size *= 2;
					buf = NOFAIL(realloc(buf, size));
				}
		}
	}
}

static void write_index(const char *filename)
{
	struct index_node *index;
	char *line;
	FILE *cfile;
	
	cfile = fopen(filename, "w");
	if (!cfile)
		fatal("Could not open %s for writing: %s\n",
		      filename, strerror(errno));
	
	index = index_create();
	
	while((line = getline_wrapped(stdin, NULL))) {
		index_insert(index, line);
		free(line);
	}
	
	index_write(index, cfile);
	index_destroy(index);
	fclose(cfile);
}

static void dump_index(const char *filename)
{
	FILE *cfile;
	
	cfile = fopen(filename, "r");
	if (!cfile)
		fatal("Could not open %s for reading: %s\n",
		      filename, strerror(errno));
	
	index_dump(cfile, stdout, "");
	
	fclose(cfile);
}

static void search_index(const char *filename, char *key)
{
	char *value;
	FILE *cfile;
	
	cfile = fopen(filename, "r");
	if (!cfile)
		fatal("Could not open %s for reading: %s\n",
		      filename, strerror(errno));
	
	value = index_search(cfile, key);
	if (value)
		printf("Found value:\n%s\n", value);
	else
		printf("Not found.\n");
	
	free(value);
	fclose(cfile);
}

static void searchwild_index(const char *filename, char *key)
{
	struct index_value *values;
	FILE *cfile;
	
	cfile = fopen(filename, "r");
	if (!cfile)
		fatal("Could not open %s for reading: %s\n",
		      filename, strerror(errno));
	
	values = index_searchwild(cfile, key);
	if (values)
		printf("Found value(s):\n");
	else
		printf("Not found.\n");
	
	while(values) {
		struct index_value *next = values->next;
		
		printf("%s\n", values->value);
		free(values);
		values = next;
	}
	
	fclose(cfile);
}

static void print_usage(const char *progname)
{
	fprintf(stderr,
		"Usage: %s [MODE] [FILE] ...\n"
		" -o, --output <outfile>\n"
		" -d, --dump <infile>\n"
		" -s, --search <infile> <key>\n"
		" -w, --searchwild <infile> <key>\n"
		,progname);
	exit(1);
}

static struct option options[] = {
	{ "output", 0, NULL, 'o' },
	{ "dump", 0, NULL, 'd' },
	{ "search", 0, NULL, 's' },
	{ "searchwild", 0, NULL, 'w' },
};

int main(int argc, char *argv[])
{
	char opt;
	char mode = 0;
	char *filename = NULL;
	char *key = NULL;
	
	while ((opt = getopt_long(argc, argv, "odsw", options, NULL))
		       != -1) {
		switch (opt) {
			case 'o':
				mode = 'o';
				break;
			case 'd':
				mode = 'd';
				break;
			case 's':
				mode = 's';
				break;
			case 'w':
				mode = 'w';
				break;
			default:
				print_usage(argv[0]);
		}
	}
	if (!mode)
		print_usage(argv[0]);
	
	if (optind >= argc)
		print_usage(argv[0]);
	filename = argv[optind];
	
	if (mode == 's' || mode == 'w') {
		if (optind+1 >= argc)
			print_usage(argv[0]);
		key = argv[optind+1];
	}
	
	switch(mode) {
		case 'o':
			write_index(filename);
			break;
		case 'd':
			dump_index(filename);
			break;
		case 's':
			search_index(filename, key);
			break;
		case 'w':
			searchwild_index(filename, key);
			break;
	}
	
	return 0;
}
