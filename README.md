libtags++
=======

[![Build Status](https://travis-ci.org/Gumichan01/libtagspp.svg?branch=master)](https://travis-ci.org/Gumichan01/libtagspp)

A cross-platform library for reading tags, based on [libtags](https://github.com/ftrvxmtrx/libtags).

Comparison to id3lib and taglib:

|                | libtags++       | id3lib           | taglib                      |
|:---------------|:----------------|:-----------------|:----------------------------|
| ID3v2.4        | yes             | no               | yes                         |
| Ogg/Vorbis     | yes             | no               | yes                         |
| FLAC           | yes             | no               | yes                         |
| m4a            | yes             | no               | yes                         |
| opus           | yes             | no               | yes                         |
| WAV            | yes             | no               | yes                         |
| replay gain    | yes             | no               | ???                         |
| size           | tiny (~40 KB)   | bloated?         | more bloated (~2 MB, v1.11) |
| license        | MIT             | LGPL             | LGPL/MPL                    |
| written in     | C++             | C++              | C++                         |
| memory         | no allocations  | allocates memory | allocates memory            |
| thread safe    | yes             | ???              | ???                         |
| tag writing    | no, not a goal  | yes              | yes                         |


## Example program


    libtagpp::Tag tag;
    if(tag.readTag("data/gumichan01-eastern_wind.ogg")
    {
        cout << "Title - " << tag.title() << endl                       // Eastern Wind
             << "Artist - " << tag.artist() << endl                     // Gumichan01
             << "Album - " << tag.album() << endl                       // QD-A9
             << " ===============================" << endl
             << "Duration - " << tag.properties().duration() << endl    // 1:41
    }
    else
        cerr << "Cannot read the tag" << endl;

## Build

### Windows

You can just use the codeblocks project file to generate the library and test the program

### Linux/Mac OSX

The library can be compiled using any C++ compiler (g++, clang++).
You just need to use the makefile.
