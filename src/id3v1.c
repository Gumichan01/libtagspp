/*
 * http://en.wikipedia.org/wiki/ID3
 * Space-padded strings are mentioned there. This is wrong and is a lie.
 */
#include "tagspriv.h"

enum
{
	Insz = 128,
	Outsz = 61,
};

int
tagid3v1(Tagctx *ctx, int *num)
{
	uchar *in, *out;

	if(ctx->bufsz < Insz+Outsz)
		return -1;
	in = (uchar*)ctx->buf;
	out = in + Insz;

	if(ctx->seek(ctx, -Insz, 2) < 0)
		return -1;
	if(ctx->read(ctx, in, Insz) != Insz || memcmp(in, "TAG", 3) != 0)
		return -1;

	if(iso88591toutf8(out, Outsz, &in[3], 30) > 0){
		ctx->tag(ctx, Ttitle, (char*)out, 0, 0);
		*num += 1;
	}
	if(iso88591toutf8(out, Outsz, &in[33], 30) > 0){
		ctx->tag(ctx, Tartist, (char*)out, 0, 0);
		*num += 1;
	}
	if(iso88591toutf8(out, Outsz, &in[63], 30) > 0){
		ctx->tag(ctx, Talbum, (char*)out, 0, 0);
		*num += 1;
	}

	in[93+4] = 0;
	if(in[93] != 0){
		ctx->tag(ctx, Tdate, (char*)&in[93], 0, 0);
		*num += 1;
	}

	if(in[125] == 0 && in[126] > 0){
		snprint((char*)out, Outsz, "%d", in[126]);
		ctx->tag(ctx, Ttrack, (char*)out, 0, 0);
		*num += 1;
	}

	if(in[127] < Numgenre){
		ctx->tag(ctx, Tgenre, id3genres[in[127]], 0, 0);
		*num += 1;
	}

	return 0;
}
