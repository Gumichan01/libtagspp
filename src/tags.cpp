#include "tagspriv.h"

typedef struct Getter Getter;

struct Getter
{
    int ( *f )( Tagctx * ctx );
    int format;
};

extern int tagvorbis( Tagctx * ctx );
extern int tagflac( Tagctx * ctx );
extern int tagid3v2( Tagctx * ctx );
extern int tagid3v1( Tagctx * ctx );
extern int tagit( Tagctx * ctx );
extern int tagm4a( Tagctx * ctx );
extern int tagopus( Tagctx * ctx );
extern int tagwav( Tagctx * ctx );

static const Getter g[] =
{
    {tagid3v2, Fmp3},
    {tagid3v1, Fmp3},
    {tagvorbis, Fogg},
    {tagflac, Fflac},
    {tagm4a, Fm4a},
    {tagopus, Fopus},
    {tagwav, Fwav},
    {tagit, Fit},
};

void tagscallcb( Tagctx * ctx, int type, const char * k, const char * s, int offset, int size, Tagread f )
{
    if ( type != Tunknown )
    {
        ctx->found |= 1 << type;
        ctx->num++;
    }
    ctx->tag( ctx, type, k, s, offset, size, f );
}

int tagsget( Tagctx * ctx )
{
    int i, res;

    ctx->channels = ctx->samplerate = ctx->bitrate = ctx->duration = 0;
    ctx->found = 0;
    ctx->format = Funknown;
    ctx->restart = 0;
    res = -1;
    for ( i = 0; i < ( int )( sizeof( g ) / sizeof( g[0] ) ); i++ )
    {
        ctx->num = 0;
        if ( g[i].f( ctx ) == 0 )
        {
            if ( ctx->num > 0 )
                res = 0;
            ctx->format = g[i].format;
        }
        ctx->seek( ctx, ctx->restart, 0 );
    }

    return res;
}
