/*
 * https://xiph.org/vorbis/doc/Vorbis_I_spec.html#x1-810005
 * https://wiki.xiph.org/VorbisComment
 */
#include "tagspriv.h"

#define leuint(d) ((d)[3]<<24 | (d)[2]<<16 | (d)[1]<<8 | (d)[0]<<0)

int
cbvorbiscomment(Tagctx *ctx, char *k, char *v){
	if(cistrcmp(k, "album") == 0)
		ctx->tag(ctx, Talbum, v, 0, 0);
	else if(cistrcmp(k, "title") == 0)
		ctx->tag(ctx, Ttitle, v, 0, 0);
	else if(cistrcmp(k, "artist") == 0 || cistrcmp(k, "performer") == 0)
		ctx->tag(ctx, Tartist, v, 0, 0);
	else if(cistrcmp(k, "tracknumber") == 0)
		ctx->tag(ctx, Ttrack, v, 0, 0);
	else if(cistrcmp(k, "date") == 0)
		ctx->tag(ctx, Tdate, v, 0, 0);
	else if(cistrcmp(k, "replaygain_track_peak") == 0)
		ctx->tag(ctx, Ttrackpeak, v, 0, 0);
	else if(cistrcmp(k, "replaygain_track_gain") == 0)
		ctx->tag(ctx, Ttrackgain, v, 0, 0);
	else if(cistrcmp(k, "replaygain_album_peak") == 0)
		ctx->tag(ctx, Talbumpeak, v, 0, 0);
	else if(cistrcmp(k, "replaygain_album_gain") == 0)
		ctx->tag(ctx, Talbumgain, v, 0, 0);
	else if(cistrcmp(k, "genre") == 0)
		ctx->tag(ctx, Tgenre, v, 0, 0);
	else
		return 0;
	return 1;
}

int
tagvorbis(Tagctx *ctx, int *num)
{
	char *v;
	uchar *d, h[4];
	int sz, numtags, i, npages;

	d = (uchar*)ctx->buf;
	/* need to find vorbis frame with type=3 */
	for(npages = 0; npages < 2; npages++){ /* vorbis comment is the second header */
		int nsegs;
		if(ctx->read(ctx, d, 27) != 27)
			return -1;
		if(memcmp(d, "OggS", 4) != 0)
			return -1;

		/* calculate the size of the packet */
		nsegs = d[26];
		if(ctx->read(ctx, d, nsegs+1) != nsegs+1)
			return -1;
		for(sz = i = 0; i < nsegs; sz += d[i++]);

		if(d[nsegs] == 3)
			break;
		ctx->seek(ctx, sz-1, 1);
	}

	if(ctx->read(ctx, &d[1], 10) != 10 || memcmp(&d[1], "vorbis", 6) != 0)
		return -1;
	sz = leuint(&d[7]);
	if(ctx->seek(ctx, sz, 1) < 0 || ctx->read(ctx, h, 4) != 4)
		return -1;
	numtags = leuint(h);

	for(i = 0; i < numtags; i++){
		if(ctx->read(ctx, h, 4) != 4)
			return -1;
		if((sz = leuint(h)) < 0)
			return -1;

		if(ctx->bufsz < sz+1){
			if(ctx->seek(ctx, sz, 1) < 0)
				return -1;
			continue;
		}
		if(ctx->read(ctx, ctx->buf, sz) != sz)
			return -1;
		ctx->buf[sz] = 0;

		if((v = strchr(ctx->buf, '=')) == nil)
			return -1;
		*v++ = 0;
		*num += cbvorbiscomment(ctx, ctx->buf, v);
	}

	return 0;
}
