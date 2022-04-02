
#include <cstdio>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cstdint>
#define snprint std::snprintf
#define nil nullptr

// In Unix systems, you might want to use strcasecmp, but it is not a Standard C function
// That is why I keep this implementation
inline int cistrcmp( const char * s1, const char * s2 )
{
    while ( *s1 != '\0' && ( std::toupper( *s1++ ) == toupper( *s2++ ) ) );
    const char su1 = static_cast<char>( std::toupper( *( ( unsigned char * )--s1 ) ) );
    const char su2 = static_cast<char>( std::toupper( *( ( unsigned char * )--s2 ) ) );
    return ( su1 < su2 ) ? -1 : ( su1 != su2 );
}

typedef uint8_t uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef uint16_t u16int;
typedef uint32_t u32int;
typedef uint64_t uvlong;

#include "tags.h"

enum
{
    Numgenre = 192,
};

template <typename T>
inline constexpr uint beuint( T * d ) noexcept
{
    return static_cast<uint>( d[0] << 24 | d[1] << 16 | d[2] << 8 | d[3] << 0 );
}

template <typename T>
inline constexpr uint leuint( T * d ) noexcept
{
    return static_cast<uint>( d[3] << 24 | d[2] << 16 | d[1] <<  8 | d[0] <<  0 );
}

extern const char * id3genres[Numgenre];

/*
 * Converts (to UTF-8) at most sz bytes of src and writes it to out buffer.
 * Returns the number of bytes converted.
 * You need sz*2+1 bytes for out buffer to be completely safe.
 */
int iso88591toutf8( uchar * out, int osz, const uchar * src, int sz );

/*
 * Converts (to UTF-8) at most sz bytes of src and writes it to out buffer.
 * Returns the number of bytes converted or < 0 in case of error.
 * You need sz*4+1 bytes for out buffer to be completely safe.
 * UTF-16 defaults to big endian if there is no BOM.
 */
int utf16to8( uchar * out, int osz, const uchar * src, int sz );

/*
 * This one is common for both vorbis.c and flac.c
 * It maps a string k to tag type and executes the callback from ctx.
 * Returns 1 if callback was called, 0 otherwise.
 */
void cbvorbiscomment( Tagctx * ctx, char * k, char * v );

void tagscallcb( Tagctx * ctx, int type, const char * k, const char * s, int offset, int size, Tagread f );

template <typename T>
inline void txtcb( Tagctx * ctx, int type, const char * k, T * s ) noexcept
{
    tagscallcb( ctx, type, k, reinterpret_cast<const char*>( s ), 0, 0, nil );
}
