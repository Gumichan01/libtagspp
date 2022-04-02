#include "tagspriv.h"

#define le16u(d) (u16int)((d)[0] | (d)[1]<<8)
#define N 8

static struct
{
    const char * s;
    int type;
} t[N] =
{
    {"IART", Tartist},
    {"ICRD", Tdate},
    {"IGNR", Tgenre},
    {"INAM", Ttitle},
    {"IPRD", Talbum},
    {"ITRK", Ttrack},
    {"ICMT", Tunknown},
    {"ISFT", Tunknown}
};

int extractmdata( Tagctx * ctx, uchar * buffer );

int tagwav( Tagctx * ctx )
{
    uchar * d;
    int i, n, info;
    u32int csz, x;
    uvlong sz;

    d = ( uchar * )ctx->buf;

    sz = 1;
    info = 0;
    for ( i = 0; i < 8 && sz > 0; i++ )
    {
        if ( ctx->read( ctx, d, 4 + 4 + ( i ? 0 : 4 ) ) != 4 + 4 + ( i ? 0 : 4 ) )
            return -1;
        if ( i == 0 )
        {
            if ( memcmp( d, "RIFF", 4 ) != 0 || memcmp( d + 8, "WAVE", 4 ) != 0 )
                return -1;
            sz = leuint( d + 4 );
            if ( sz < 4 )
                return -1;
            sz -= 4;
            continue;
        }
        else if ( memcmp( d, "INFO", 4 ) == 0 )
        {
            info = 1;
            ctx->seek( ctx, -4, 1 );
            continue;
        }

        if ( sz <= 8 )
            break;
        sz -= 4 + 4;
        csz = leuint( d + 4 );
        if ( sz < static_cast<uvlong>(csz) )
            break;
        sz -= csz;

        if ( i == 1 )
        {
            if ( memcmp( d, "fmt ", 4 ) != 0 || csz < 16 )
                return -1;
            if ( ctx->read( ctx, d, 16 ) != 16 )
                return -1;
            csz -= 16;
            ctx->channels = le16u( d + 2 );
            ctx->samplerate = leuint( d + 4 );
            x = leuint( d + 8 );
            if ( ctx->channels < 1 || ctx->samplerate <1 || x < 1)
            {
                return -1;
            }
            ctx->duration = sz * 1000 / x;
        }
        else if ( memcmp( d, "LIST", 4 ) == 0 )
        {
            sz = csz - 4;
            continue;
        }
        else if ( info )
        {
            for ( n = 0; n < N; n++ )
            {
                if ( memcmp( d, t[n].s, 4 ) == 0 )
                {
                    if ( static_cast<u32int>( ctx->read( ctx, d, csz ) ) != csz )
                        return -1;
                    d[csz - 1] = 0;
                    txtcb( ctx, t[n].type, "", d );
                    csz = 0;
                    break;
                }
            }
            return extractmdata( ctx, d );
        }

        if ( ctx->seek( ctx, csz, 1 ) < 0 )
            return -1;
    }

    return i > 0 ? 0 : -1;
}

int
extractmdata( Tagctx * ctx, uchar * buffer )
{
    const u32int FOUR_CC = 4;

    memset( buffer, 0, ctx->bufsz );
    while ( ctx->read( ctx, buffer, FOUR_CC ) == FOUR_CC && memcmp( buffer, "data", FOUR_CC ) != 0 )
    {
        int n = 0;
        int csz = 0;
        while ( n < N &&  csz == 0 )
        {
            if ( memcmp( buffer, t[n].s, FOUR_CC ) == 0 )
            {
                if ( ctx->read( ctx, &csz, sizeof( u32int ) ) != sizeof( u32int ) )
                    return -1;

                if ( ctx->read( ctx, buffer, csz ) != csz )
                    return -1;

                buffer[csz - 1] = 0;
                if ( t[n].type != Tunknown )
                    txtcb( ctx, t[n].type, "", buffer );
            }
            n++;
        }
        memset( buffer, 0, csz );
        csz = 0;
    }
    return 0;
}
