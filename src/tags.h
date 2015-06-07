#ifndef __unix__
#pragma lib "libtags.a"
#endif

typedef struct Tagctx Tagctx;

enum
{
	Tartist,
	Talbum,
	Ttitle,
	Tdate, /* "2014", "2015/02/01", but the year goes first */
	Ttrack, /* "1", "01", "1/4", but the track number goes first */
	Talbumgain,
	Talbumpeak,
	Ttrackgain,
	Ttrackpeak,
	Tgenre,
	Timage,
};

enum
{
	Fmp3,
	Fogg,
	Fflac,
};

struct Tagctx
{
	const char *filename;
	int (*read)(Tagctx *ctx, void *buf, int cnt);
	int (*seek)(Tagctx *ctx, int offset, int whence);
	void (*tag)(Tagctx *ctx, int type, const char *s, int offset, int size);
	void *aux;
	char *buf;
	int bufsz;

	int channels;
	int samplerate;
	int duration; /* ms */
	int format;

	/* private, don't touch */
	int found;
	int num;
};

extern int tagsget(Tagctx *ctx);
