#include "tagspriv.h"

typedef struct Getter Getter;

struct Getter
{
	int (*f)(Tagctx *ctx, int *num);
	int extlen;
	const char *ext;
};

extern int tagvorbis(Tagctx *ctx, int *num);
extern int tagflac(Tagctx *ctx, int *num);
extern int tagid3v2(Tagctx *ctx, int *num);
extern int tagid3v1(Tagctx *ctx, int *num);

static const Getter g[] =
{
	{tagid3v2, 4, ".mp3"},
	{tagid3v1, 4, ".mp3"},
	{tagvorbis, 4, ".ogg"},
	{tagflac, 5, ".flac"},
};

void
tagscallcb(Tagctx *ctx, int type, const char *s, int offset, int size)
{
	ctx->found |= 1<<type;
	ctx->tag(ctx, type, s, offset, size);
}

int
tagsget(Tagctx *ctx)
{
	int i, len, num, res;

	/* enough for having an extension */
	len = 0;
	if(ctx->filename != nil && (len = strlen(ctx->filename)) < 5)
		return -1;
	ctx->channels = 0;
	ctx->samplerate = 0;
	ctx->duration = 0;
	ctx->found = 0;
	res = -1;
	for(i = 0; i < (int)(sizeof(g)/sizeof(g[0])); i++){
		if(ctx->filename == nil || memcmp(&ctx->filename[len-g[i].extlen], g[i].ext, g[i].extlen) == 0){
			num = 0;
			if(g[i].f(ctx, &num) == 0 && num > 0)
				res = 0;
			ctx->seek(ctx, 0, 0);
		}
	}

	return res;
}
