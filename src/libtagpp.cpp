
#include <libtagpp.hpp>
#include <tags.h>

#include <cstdio>

namespace
{

struct Aux
{
	FILE * f;
    libtagpp::Tag& tag;
};

int ctxread(Tagctx *ctx, void *buf, int cnt)
{
	Aux *aux = (Aux *) ctx->aux;
	return fread(buf,1,cnt,aux->f);
}

int ctxseek(Tagctx *ctx, int offset, int whence)
{
	Aux *aux = (Aux *) ctx->aux;
    fseek(aux->f, offset, whence);
	return ftell(aux->f);
}

};

namespace libtagpp
{

/* Properties */

Properties::Properties()
: _channels(0), _samplerate(0), _bitrate(0), _duration(0), _format(0) {}

Properties::~Properties() {}


int Properties::channels() const
{
    return _channels;
}

int Properties::samplerate() const
{
    return _samplerate;
}

int Properties::bitrate() const
{
    return _bitrate;
}

const char * Properties::duration() const
{
    return "";
}

int Properties::format() const
{
    return _format;
}


/* Tag */

Tag::Tag() {}

void ctxtag(Tagctx *ctx, int t, const char *v, int offset, int size, Tagread f)
{
    Aux *aux = (Aux *) ctx->aux;

    switch(t)
    {
        case Tartist: aux->tag._artist = v; break;
        case Talbum: aux->tag._album = v; break;
        case Ttitle: aux->tag._title = v; break;
        case Tdate: aux->tag._year = v; break;
        case Ttrack: aux->tag._track = v; break;
        case Tgenre: aux->tag._genre = v; break;
        case Talbumgain: aux->tag._albumgain = v; break;
        case Talbumpeak: aux->tag._albumpeak = v; break;
        case Ttrackgain: aux->tag._trackgain = v; break;
        default: break;
    }
}


bool Tag::readTag(const std::string& filename)
{
    const char * f = filename.c_str();
    char buf[256];
	Aux aux = { NULL, *this };
	Tagctx ctx = { NULL, ctxread, ctxseek, ctxtag, &aux, buf, sizeof(buf) };

    if((aux.f = fopen(f, "rb")) == NULL)
    {
        fprintf(stderr, "failed to open: %s does not exist\n",f);
        return false;
    }

    bool success = tagsget(&ctx) == 0;
    fclose(aux.f);

    if(success)
    {
        _properties._channels   = ctx.channels;
        _properties._samplerate = ctx.samplerate;
        _properties._bitrate    = ctx.bitrate;
        _properties._duration   = ctx.duration;
        _properties._format     = ctx.format;
        return true;
    }

    return false;
}


const char * Tag::title() const
{
    return _title.c_str();
}

const char * Tag::artist() const
{
    return _artist.c_str();
}

const char * Tag::album() const
{
    return _album.c_str();
}

const char * Tag::year() const
{
    return _year.c_str();
}

const char * Tag::track() const
{
    return _track.c_str();
}

const char * Tag::genre() const
{
    return _genre.c_str();
}

const char * Tag::albumgain() const
{
    return _albumgain.c_str();
}

const char * Tag::albumpeak() const
{
    return _albumpeak.c_str();
}

const char * Tag::trackgain() const
{
    return _trackgain.c_str();
}

const char * Tag::trackpeak() const
{
    return _trackpeak.c_str();
}

const Properties& Tag::properties() const
{
    return _properties;
}


Tag::~Tag() {}

};
