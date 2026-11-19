//	tStream.h: tStream, the byte sink assets and strings are written to.
// Evidence:  tStream::operator<<(tString const &) symbol at 0x00758cd0; tAsset::vfunction5
// (0x00675c10) chains the writes through the stream slot 3 returns.

#ifndef tStream_h
#define tStream_h 1

#include "Tiger/tstring.h"

// HYPOTHESIS: tStream is an abstract byte sink; slot 3 (+0x0c) writes a buffer and returns the
// stream, the byte at +4 asks for the values to be written byte-swapped.
class tStream
{
public:
	virtual void	fn_slot0( void );
	virtual void	fn_slot1( void );
	virtual void	fn_slot2( void* buffer, long size );
	virtual tStream&	fn_slot3( void* buffer, long size );
	unsigned char	fSwap;
	tStream&	operator<<( unsigned long v );
	tStream&	operator<<( unsigned short v );
	tStream&	operator<<( char c );
	tStream&	operator<<( const tString& s );
	tStream&	operator>>( const tString& s );
};

// HYPOTHESIS: the long, short and char writers were inline overloads; the exe has no copy of
// any of them.
inline tStream& tStream::operator<<( unsigned long v )
{
	unsigned long out = v;
	if ( fSwap )
	{
		unsigned short hi = ( (unsigned short*)&out )[1];
		unsigned short lo = ( (unsigned short*)&out )[0];
		( (unsigned short*)&out )[0] = (unsigned short)( ( hi >> 8 ) | ( hi << 8 ) );
		( (unsigned short*)&out )[1] = (unsigned short)( ( lo >> 8 ) | ( lo << 8 ) );
	}
	return fn_slot3( &out, 4 );
}

inline tStream& tStream::operator<<( unsigned short v )
{
	unsigned short out = v;
	if ( fSwap )
		out = (unsigned short)( ( v >> 8 ) | ( v << 8 ) );
	return fn_slot3( &out, 2 );
}

inline tStream& tStream::operator<<( char c )
{
	fn_slot3( &c, 1 );
	return *this;
}

#endif	// tStream_h
