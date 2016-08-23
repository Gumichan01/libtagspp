
#include <libtagpp.hpp>
#include <iostream>

using namespace std;


int main(int argc, char **argv)
{

	if(argc < 2)
    {
		cout << "usage: readtags FILE..." << endl;;
		return -1;
	}

	for(int i = 1; i < argc; i++)
    {
        cout << "*** " << argv[i] << endl;;

        libtagpp::Tag tag;
        if(tag.readTag(argv[i]))
        {
            cout << "Title - " << tag.title() << endl
                 << "Artist - " << tag.artist() << endl
                 << "Album - " << tag.album() << endl
                 << "Year - " << tag.year() << endl
                 << "Track - " << tag.track() << endl
                 << "Genre - " << tag.genre() << endl
                 << " ===============================" << endl
                 << "Duration - " << tag.properties().duration() << endl;
        }
        else
            cerr << "Cannot read the tag of the following file: " << endl
                 << "--- " << argv[1] << endl;

	}
	return 0;
}
