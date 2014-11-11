/*
 *  * (C) Copyright Marvell Semiconductors,Inc 2006 All rightes reserved
 *   Those algorithms is derived from lib/lzo/lzo1x_decompress.c and 
 *   lib/inflate.c. Added several pseudo functions for booting stage.
 *
 *   */
#include <linux/compiler.h>
#include <linux/lzo.h>
#include <linux/bitops.h>
#include <asm/byteorder.h>
#include <asm/elf.h>
#include <mach/unaligned.h>
#include "lzo/lzodefs.h"

unsigned int elf_hwcap;

unsigned int hweight32(unsigned int w)
{
	unsigned int res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	return (res + (res >> 16)) & 0x000000FF;
}

unsigned long wrong_size_cmpxchg(volatile void *ptr)
{
	return 0;
}

void __bad_xchg(volatile void *ptr, int size)
{
}

#define HAVE_IP(x, ip_end, ip) ((size_t)(ip_end - ip) < (x))
#define HAVE_OP(x, op_end, op) ((size_t)(op_end - op) < (x))
#define HAVE_LB(m_pos, out, op) (m_pos < out || m_pos >= op)

#define COPY4(dst, src)	\
		put_unaligned(get_unaligned((const u32 *)(src)), (u32 *)(dst))

int lzo1x_decompress_safe(const unsigned char *in, size_t in_len,
			unsigned char *out, size_t *out_len)
{
	const unsigned char * const ip_end = in + in_len;
	unsigned char * const op_end = out + *out_len;
	const unsigned char *ip = in, *m_pos;
	unsigned char *op = out;
	size_t t;

	*out_len = 0;

	if (*ip > 17) {
		t = *ip++ - 17;
		if (t < 4)
			goto match_next;
		if (HAVE_OP(t, op_end, op))
			goto output_overrun;
		if (HAVE_IP(t + 1, ip_end, ip))
			goto input_overrun;
		do {
			*op++ = *ip++;
		} while (--t > 0);
		goto first_literal_run;
	}

	while ((ip < ip_end)) {
		t = *ip++;
		if (t >= 16)
			goto match;
		if (t == 0) {
			if (HAVE_IP(1, ip_end, ip))
				goto input_overrun;

			while (*ip == 0) {
				t += 255;
				ip++;
				if (HAVE_IP(1, ip_end, ip))
					goto input_overrun;
			}
			t += 15 + *ip++;
		}
		if (HAVE_OP(t + 3, op_end, op))
			goto output_overrun;
		if (HAVE_IP(t + 4, ip_end, ip))
			goto input_overrun;

		COPY4(op, ip);
		op += 4;
		ip += 4;
		if (--t > 0) {
			if (t >= 4) {
				do {
					COPY4(op, ip);
					op += 4;
					ip += 4;
					t -= 4;
				} while (t >= 4);
				if (t > 0) {
					do {
						*op++ = *ip++;
					} while (--t > 0);
				}
			} else {
				do {
					*op++ = *ip++;
				} while (--t > 0);
			}
		}

first_literal_run:
		t = *ip++;
		if (t >= 16)
			goto match;
		m_pos = op - (1 + M2_MAX_OFFSET);
		m_pos -= t >> 2;
		m_pos -= *ip++ << 2;

		if (HAVE_LB(m_pos, out, op))
			goto lookbehind_overrun;

		if (HAVE_OP(3, op_end, op))
			goto output_overrun;
		*op++ = *m_pos++;
		*op++ = *m_pos++;
		*op++ = *m_pos;

		goto match_done;

		do {
match:
			if (t >= 64) {
				m_pos = op - 1;
				m_pos -= (t >> 2) & 7;
				m_pos -= *ip++ << 3;
				t = (t >> 5) - 1;
				if (HAVE_LB(m_pos, out, op))
					goto lookbehind_overrun;
				if (HAVE_OP(t + 3 - 1, op_end, op))
					goto output_overrun;
				goto copy_match;
			} else if (t >= 32) {
				t &= 31;
				if (t == 0) {
					if (HAVE_IP(1, ip_end, ip))
						goto input_overrun;
					while (*ip == 0) {
						t += 255;
						ip++;
						if (HAVE_IP(1, ip_end, ip))
							goto input_overrun;
					}
					t += 31 + *ip++;
				}
				m_pos = op - 1;
				m_pos -= get_unaligned_le16(ip) >> 2;
				ip += 2;
			} else if (t >= 16) {
				m_pos = op;
				m_pos -= (t & 8) << 11;

				t &= 7;
				if (t == 0) {
					if (HAVE_IP(1, ip_end, ip))
						goto input_overrun;
					while (*ip == 0) {
						t += 255;
						ip++;
						if (HAVE_IP(1, ip_end, ip))
							goto input_overrun;
					}
					t += 7 + *ip++;
				}
				m_pos -= get_unaligned_le16(ip) >> 2;
				ip += 2;
				if (m_pos == op)
					goto eof_found;
				m_pos -= 0x4000;
			} else {
				m_pos = op - 1;
				m_pos -= t >> 2;
				m_pos -= *ip++ << 2;

				if (HAVE_LB(m_pos, out, op))
					goto lookbehind_overrun;
				if (HAVE_OP(2, op_end, op))
					goto output_overrun;

				*op++ = *m_pos++;
				*op++ = *m_pos;
				goto match_done;
			}

			if (HAVE_LB(m_pos, out, op))
				goto lookbehind_overrun;
			if (HAVE_OP(t + 3 - 1, op_end, op))
				goto output_overrun;

			if (t >= 2 * 4 - (3 - 1) && (op - m_pos) >= 4) {
				COPY4(op, m_pos);
				op += 4;
				m_pos += 4;
				t -= 4 - (3 - 1);
				do {
					COPY4(op, m_pos);
					op += 4;
					m_pos += 4;
					t -= 4;
				} while (t >= 4);
				if (t > 0)
					do {
						*op++ = *m_pos++;
					} while (--t > 0);
			} else {
copy_match:
				*op++ = *m_pos++;
				*op++ = *m_pos++;
				do {
					*op++ = *m_pos++;
				} while (--t > 0);
			}
match_done:
			t = ip[-2] & 3;
			if (t == 0)
				break;
match_next:
			if (HAVE_OP(t, op_end, op))
				goto output_overrun;
			if (HAVE_IP(t + 1, ip_end, ip))
				goto input_overrun;

			*op++ = *ip++;
			if (t > 1) {
				*op++ = *ip++;
				if (t > 2)
					*op++ = *ip++;
			}

			t = *ip++;
		} while (ip < ip_end);
	}

	*out_len = op - out;
	return LZO_E_EOF_NOT_FOUND;

eof_found:
	*out_len = op - out;
	return (ip == ip_end ? LZO_E_OK :
		(ip < ip_end ? LZO_E_INPUT_NOT_CONSUMED : LZO_E_INPUT_OVERRUN));
input_overrun:
	*out_len = op - out;
	return LZO_E_INPUT_OVERRUN;

output_overrun:
	*out_len = op - out;
	return LZO_E_OUTPUT_OVERRUN;

lookbehind_overrun:
	*out_len = op - out;
	return LZO_E_LOOKBEHIND_OVERRUN;
}

#ifndef INIT
#define INIT
#endif

//static const unsigned char magic[7] =  
//   { 0x00, 0xe9, 0x4c, 0x5a, 0x4f, 0xff, 0x1a };
static const unsigned char lzop_magic[9] =
   { 0x89, 0x4c, 0x5a, 0x4f, 0x00, 0x0d, 0x0a, 0x1a, 0x0a };

#define FN_MAX ( 256 )
#define NEXTBYTE()  ({ int v = get_byte(); if (v < 0) goto underrun; (uch)v; })
#define __get32() ({ unsigned long v2 = 0; v2 |= (ulg) NEXTBYTE() << 24; v2 |= (ulg) NEXTBYTE() << 16; v2 |= (ulg) NEXTBYTE() << 8; v2 |= (ulg) NEXTBYTE();(ulg) v2; })

static int INIT lzo_decompress(void)
{
	int r = 0;
	int i = 0;
	int fn_len = 0;
	int __m_loop = sizeof(lzop_magic);
	unsigned char m[sizeof(lzop_magic)] ;
	int method ;
	int level;
	size_t buf_len;
	ulg  crc_code = 0;
	ulg  flags = 0;
	char f_name[FN_MAX];
	//size_t total_in = total_out = 0;

	while( __m_loop) 
		m[__m_loop--] = NEXTBYTE();

	for ( i = 6  ; i ; i-- ) 
		(void) NEXTBYTE(); /* Get version related 6 bytes */

	method = NEXTBYTE();
	level = NEXTBYTE();  /* Level must be 1 or 9 */

	if(method != 3 )    /* It's 1 first, but lzop changed it to 3*/
	{
		putstr ("\n method : ");
		puthex (method);
		putstr ("\n level : ");
		puthex (level);
		error("invalid method or level");
	}
	
	flags = __get32(); 

#define F_H_FILTER (0x00000800L)

	if( flags & F_H_FILTER)
		(void) __get32(); // Ignore the filter information.

	for ( i = 12 ; i ; i-- ) 
		(void) NEXTBYTE(); /* Get mode, mod_time and gmtdiff relate three words */

	fn_len = NEXTBYTE(); /* If this byte is not null, then it will be the length of the compressed file name */
	
	if ( fn_len ) 
	{
		putstr ( "\n fn_len : ");
		puthex (fn_len);

		for ( i = fn_len; i ; i-- )
			f_name[fn_len - i] = NEXTBYTE();

		f_name[fn_len] = 0;

		putstr(f_name);
	}

	crc_code = __get32(); /* Get crc32 checksum code */

	while( inptr < insize )
	{
		uch *in;
		uch *out;
		uch *feed;
		size_t in_len;
		size_t out_len;
		size_t new_len = 0;

		out_len = __get32(); /* First word of each block is output length */
		if(out_len == 0)
			break;

		buf_len = out_len + out_len / 16 + 64 + 3; /* It's lzo's window size, it depends on output buffer size */

		if ( buf_len > WSIZE ) /* If windows size too small , enlarge it */
		{
			putstr ("\n buf_len : ");
			puthex (buf_len);
			putstr (" > WSIZE : ");
			puthex (WSIZE );
		}

		in_len = __get32(); /* Second word is input length stored in block */
		(void)	__get32(); /* Ignore 32 bit checksum, third word */

		if (in_len == 0)
			putstr("\nin_len == 0");
		else if (in_len > out_len)   /* In_len cannot be less than output length */
			putstr("\nin_len > out_len");

		if(in_len == 0 || in_len > out_len)
		{
			putstr("\n buf_len: 0x");
			puthex( buf_len);
			putstr("\n insize: 0x");
			puthex( insize);

			putstr("\n in_len: 0x");
			puthex( in_len);
			putstr("\n out_len: 0x");
			puthex( out_len);		

			error("block_size error - data corrupted. ");
		}

		feed = in = window + buf_len - in_len ; /* Pointer feed to fill the input window buffer. */
		out = window;
		__memzero( window, WSIZE);

		if (in_len < out_len ) /* Input length smaller than output length, means this block is compressed */
		{
			in = &inbuf[inptr];

			new_len = out_len;

			r = lzo1x_decompress_safe(in, in_len, out, &new_len);

			if( new_len  != out_len) /* Check decompress status */
			{
				putstr ("\n new_len : ");
				puthex (new_len );
				putstr ("\n out_len : ");
				puthex (out_len);
				putstr ("\n Error : out_len != new_len, see below");
			} 

			if( r != LZO_E_OK )
			{
				putstr("\n Error code: ");
				putc ('0' - r);
				error("Error decompress");
			}
			inptr += in_len;
			outcnt = new_len;
			flush_window(); /* Flush each output window */
		}
		else  /* Input length == output length, just copy to output  */ 
		{
			feed = window;
			while(feed < (window + in_len))
				*feed++ = NEXTBYTE();

			outcnt = in_len;
			flush_window();
		}
	}

		return 0;
underrun:
		error("out of input data");
		return -1;
}
