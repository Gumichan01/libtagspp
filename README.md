libtags++
=======

A cross-platform library for reading tags, based on [libtags](https://github.com/ftrvxmtrx/libtags).

Comparison to id3lib and taglib:

|                | libtags++       | id3lib           | taglib                      |
|:---------------|:----------------|:-----------------|:----------------------------|
| ID3v2.4        | yes             | no               | yes                         |
| Ogg/Vorbis     | yes             | no               | yes                         |
| FLAC           | yes             | no               | yes                         |
| m4a            | yes             | no               | yes                         |
| replay gain    | yes             | no               | ???                         |
| size           | tiny (~60 KB)   | bloated?         | more bloated (~2 MB, v1.11) |
| license        | MIT             | LGPL             | LGPL/MPL                    |
| written in     | C++             | C++              | C++                         |
| memory         | no allocations  | allocates memory | allocates memory            |
| thread safe    | yes             | ???              | ???                         |
| tag writing    | no, not a goal  | yes              | yes                         |


## Example program


    libtagpp::Tag tag;
    if(tag.readTag("Z-Bombs.mp3")
    {
        cout << "Title - " << tag.title() << endl                       // Z-Bombs.mp3
             << "Artist - " << tag.artist() << endl                     // Comptroller
             << "Album - " << tag.album() << endl                       // Baddies
             << " ===============================" << endl
             << "Duration - " << tag.properties().duration() << endl    // Electronic
    }
    else
        cerr << "Cannot read the tag" << endl;

## Build

### Windows

**TODO**

### Linux/Mac OSX

The library can be compiled using any C++ compiler (g++, clang++).
You just need to use the makefile.
