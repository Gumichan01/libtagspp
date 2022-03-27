
#include <libtagspp.hpp>
#include <iostream>

using namespace std;


int main( int argc, char ** argv )
{
    if ( argc < 2 )
    {
        cout << "usage: rtags FILE..." << endl;;
        return -1;
    }

    for ( int i = 1; i < argc; i++ )
    {
        libtagpp::Tag tag;
        if ( tag.readTag( argv[i] ) )
        {
            cout << "*** " << argv[i] << endl
                 << "Title - " << tag.title() << endl
                 << "Artist - " << tag.artist() << endl
                 << "Album - " << tag.album() << endl
                 << "Year - " << tag.year() << endl
                 << "Track - " << tag.track() << endl
                 << "Genre - " << tag.genre() << endl
                 << "Image metadata - offset: " << tag.getImageMetaData()._img_offset
                 << "; size: " << tag.getImageMetaData()._img_size << endl
                 << " ===============================" << endl
                 << "Duration - " << tag.properties().duration << endl
                 << "Channels - " << tag.properties().channels << endl
                 << "Sample rate - " << tag.properties().samplerate << endl
                 << "Bitrate - " << tag.properties().bitrate << endl
                 << "Format - " << tag.properties().format << endl << endl;
        }
        else
        {
            cerr << "Cannot read the tag of the following file: " << endl
                 << "--- " << argv[1] << "--- " << endl;
            return -1;
        }
    }
    return 0;
}
