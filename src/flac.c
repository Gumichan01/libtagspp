#include "tagspriv.h"

#define beu3(d)   ((d)[0]<<16 | (d)[1]<<8  | (d)[2]<<0)
#define leuint(d) ((d)[3]<<24 | (d)[2]<<16 | (d)[1]<<8 | (d)[0]<<0)

int
tagflac(Tagctx *ctx, int *num)
{
	uchar d[8];
	int sz, last;

	if(ctx->read(ctx, d, sizeof(d)) != sizeof(d) || memcmp(d, "fLaC\x00", 5) != 0)
		return -1;

	/* skip streaminfo */
	sz = beu3(&d[5]);
	if(ctx->seek(ctx, sz, 1) != sz+8)
		return -1;

	for(last = 0; !last;){
		if(ctx->read(ctx, d, 4) != 4)
			return -1;

		sz = beu3(&d[1]);
		if((d[0] & 0x80) != 0)
			last = 1;

		if((d[0] & 0x7f) == 4){ /* 4 = vorbis comment */
			int i, numtags, tagsz, vensz;
			char *k, *v;

			if(sz < 12 || ctx->read(ctx, d, 4) != 4)
				return -1;

			sz -= 4;
			vensz = leuint(d);
			if(vensz < 0 || vensz > sz-8)
				return -1;
			/* skip vendor, read the number of tags */
			if(ctx->seek(ctx, vensz, 1) < 0 || ctx->read(ctx, d, 4) != 4)
				return -1;
			sz -= 4;
			numtags = leuint(d);

			for(i = 0; i < numtags && sz > 4; i++){
				if(ctx->read(ctx, d, 4) != 4)
					return -1;
				tagsz = leuint(d);
				sz -= 4;
				if(tagsz > sz)
					return -1;

				/* if it doesn't fit, ignore it */
				if(tagsz+1 > ctx->bufsz){
					if(ctx->seek(ctx, tagsz, 1) < 0)
						return -1;
					continue;
				}

				if(ctx->read(ctx, ctx->buf, tagsz) != tagsz)
					return -1;

				k = ctx->buf;
				k[tagsz] = 0;
				/* some tags have a stupid '\r'; ignore */
				if(k[tagsz-1] == '\r')
					k[tagsz-1] = 0;

				if((v = strchr(k, '=')) != nil){
					*v++ = 0;
					*num += cbvorbiscomment(ctx, k, v);
				}
			}
		}else if(ctx->seek(ctx, sz, 1) <= 0)
			return -1;
	}

	return 0;
}
