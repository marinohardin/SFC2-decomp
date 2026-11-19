//	DataStoreBuffer.h
//	The byte store behind a data store's input and output. Declarations follow the SDK's header,
//	without its operator const byte*(), which tools/manifest.py cannot read yet. The original
//	defines the inline functions in DataStoreBuffer.cpp, which the header includes; here they
//	follow the class, and only the ones matched code uses are defined yet.
//	MATCH-ONLY: tBuffer::Write, defined inline here and inlined by FileDatabaseMgr.cpp's StreamOut,
//	is left out where DATASTORE_OUT_OF_LINE_WRITE is defined: tClientCharacter.cpp holds the copy
//	the compiler emitted out of line (0x0042a0fc), which the harness can claim only from a .cpp.
//	MATCH-ONLY: tBuffer::Get is left out where DATASTORE_OUT_OF_LINE_BUFFER_GET is defined, for
//	the same reason: unsorted/fn_0042A000.cpp holds its out-of-line copy (0x0042a31c).
#if !defined( DataStoreBuffer_h )
#define DataStoreBuffer_h

#pragma once

#include "Common/BaseTypes.h"
#include "Common/Object.h"
#include "Common/Exceptions/OutOfBounds.h"

#include <vector>
#include <iosfwd>

namespace nDataStore
{

typedef int32u iPos;

class tBuffer : public tObject
{
public:
	typedef int32u iBufferSize;

	inline tBuffer( iBufferSize initSize = 0 );
	inline ~tBuffer();

	inline void Clear();

	inline void Put( byte data );
	inline void Write( const byte* data, int32u size );

	inline byte Get( iPos getPos ) const;
	inline void Read( byte* data, int32 size, iPos getPos ) const;

	inline iBufferSize Size() const;

protected:
	typedef std::vector<byte> iBuffer;

	iBuffer fBuffer;
};

std::istream& operator>>( std::istream& inputStream, tBuffer& input );
std::ostream& operator<<( std::ostream& outputStream, const tBuffer& output );

void tBuffer::Put( byte data )
{
	fBuffer.push_back( data );
}
#ifndef DATASTORE_OUT_OF_LINE_WRITE
void tBuffer::Write( const byte* data, int32u size )
{
	fBuffer.insert( fBuffer.end(), data, data + size );
}
#endif
#ifndef DATASTORE_OUT_OF_LINE_BUFFER_GET
byte tBuffer::Get( iPos getPos ) const
{
	if( getPos >= Size() )
	{
		throw except::tOutOfBounds( "tBuffer", 0, Size(), getPos, __FILE__, __LINE__ );
	}

	return fBuffer[getPos];
}
#endif

void tBuffer::Read( byte* data, int32 size, iPos getPos ) const
{
	if( getPos + size > Size() )
	{
		throw except::tOutOfBounds( "tBuffer", 0, Size(), getPos + size, __FILE__, __LINE__ );
	}

	iBuffer::const_iterator start = fBuffer.begin() + getPos;
	iBuffer::const_iterator end = start + size;

	std::copy( start, end, data );
}

tBuffer::iBufferSize tBuffer::Size() const
{
	return fBuffer.size();
}

};

#endif
