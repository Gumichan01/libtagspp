
#include <libtagpp.hpp>
#include <tags.h>


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
    return NULL;
}

int Properties::format() const
{
    return _format;
}


/* Tag */

Tag::Tag() {}

Tag::~Tag() {}

};
