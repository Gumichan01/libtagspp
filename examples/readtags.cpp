
#include <tags.h>
#include <stdio.h>

typedef struct Aux Aux;

struct Aux
{
	FILE * fd;
};

static const char *t2s[] =
{
	"Artist",
	"Album",
	"Title",
	"Date",
	"Track",
	"Albumgain",
	"Albumpeak",
	"Trackgain",
	"Trackpeak",
	"Genre",
	"Image",
};

static void
cb(Tagctx *ctx, int t, const char *v, int offset, int size, Tagread f)
{
	if(t == Timage)
		printf("%-12s %s %d %d\n", t2s[t], v, offset, size);
	else
		printf("%-12s %s\n", t2s[t], v);
}

static int
ctxread(Tagctx *ctx, void *buf, int cnt)
{
	Aux *aux = (Aux *) ctx->aux;
	return fread(buf,1,cnt,aux->fd);
}

static int
ctxseek(Tagctx *ctx, int offset, int whence)
{
	Aux *aux = (Aux *) ctx->aux;
    fseek(aux->fd, offset, whence);
	return ftell(aux->fd);
}

int
main(int argc, char **argv)
{
	int i;
	char buf[256];
	Aux aux;
	Tagctx ctx =
	{
        NULL,
		ctxread,
		ctxseek,
		cb,
        &aux,
		buf,
		sizeof(buf)
	};

	if(argc < 2){
		printf("usage: readtags FILE...\n");
		return -1;
	}

	for(i = 1; i < argc; i++){
		printf("*** %s\n", argv[i]);
		if((aux.fd = fopen(argv[i], "rb")) < 0)
			printf("failed to open\n");
		else{
			ctx.filename = argv[i];
			if(tagsget(&ctx) != 0)
				printf("no tags or failed to read tags\n");
			else{
				if(ctx.duration > 0)
					printf("%-12s %d ms\n", "duration", ctx.duration);
				if(ctx.samplerate > 0)
					printf("%-12s %d\n", "samplerate", ctx.samplerate);
				if(ctx.channels > 0)
					printf("%-12s %d\n", "channels", ctx.channels);
				if(ctx.bitrate > 0)
					printf("%-12s %d\n", "bitrate", ctx.bitrate);
			}
			fclose(aux.fd);
		}
		printf("\n");
	}
	return 0;
}
