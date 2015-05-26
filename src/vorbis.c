/*
 * https://xiph.org/vorbis/doc/Vorbis_I_spec.html#x1-810005
 * https://wiki.xiph.org/VorbisComment
 */
#include "tagspriv.h"

#define leuint(d) (((uchar*)(d))[3]<<24 | ((uchar*)(d))[2]<<16 | ((uchar*)(d))[1]<<8 | ((uchar*)(d))[0]<<0)

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

		if(d[nsegs] == 1 && sz >= 16){ /* identification */
			if(ctx->read(ctx, d, 16) != 16)
				return -1;
			sz -= 16;
			ctx->channels = d[10];
			ctx->samplerate = leuint(&d[11]);
		}else if(d[nsegs] == 3){ /* comment */
			break;
		}
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

	/* calculate the duration */
	sz = ctx->bufsz <= 4096 ? ctx->bufsz : 4096;
	for(i = sz; i < 65536+16; i += sz - 16){
		if(ctx->seek(ctx, -i, 2) <= 0)
			break;
		v = ctx->buf;
		if(ctx->read(ctx, v, sz) != sz)
			break;
		for(; v != NULL && v < ctx->buf+sz;){
			v = memchr(v, 'O', ctx->buf+sz - v - 14);
			if(v != NULL && v[1] == 'g' && v[2] == 'g' && v[3] == 'S' && (v[5] & 4) == 4){ /* last page */
				uvlong g = leuint(v+6) | (uvlong)leuint(v+10)<<32;
				ctx->duration = g * 1000 / ctx->samplerate;
				return 0;
			}
			if(v != NULL)
				v++;
		}
	}

	return 0;
}
