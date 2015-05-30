#ifdef __unix__
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#define snprint snprintf
#define cistrcmp strcasecmp
#define nil NULL
typedef unsigned char uchar;
typedef unsigned long long uvlong;
#else
#include <u.h>
#include <libc.h>
#endif
#include "tags.h"

enum
{
	Numgenre = 192,
};

extern const char *id3genres[Numgenre];

/*
 * Converts (to UTF-8) at most sz bytes of src and writes it to out buffer.
 * Returns the number of bytes converted.
 * You need sz*2+1 bytes for out buffer to be completely safe.
 */
int iso88591toutf8(uchar *out, int osz, const uchar *src, int sz);

/*
 * Converts (to UTF-8) at most sz bytes of src and writes it to out buffer.
 * Returns the number of bytes converted or < 0 in case of error.
 * You need sz*4+1 bytes for out buffer to be completely safe.
 * UTF-16 defaults to big endian if there is no BOM.
 */
int utf16to8(uchar *out, int osz, const uchar *src, int sz);

/*
 * This one is common for both tagvorbis.c and tagflac.c
 * It maps a string k to Tagtype and executes cb.
 * Returns 1 if cb was called, 0 otherwise.
 */
int cbvorbiscomment(Tagctx *ctx, char *k, char *v);
