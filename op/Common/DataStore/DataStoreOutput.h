//	DataStoreOutput.h
//	Writes values into a data store buffer. Declarations follow the SDK's header; of its template
//	stream operators for the standard containers only the vector one is used by matched code. The
//	original defines the inline functions in DataStoreOutput.cpp, which the header includes; here
//	they follow the class, and only the ones matched code uses are defined yet.
//	MATCH-ONLY: operator<<( const std::string& ), defined inline here and inlined by
//	FileDatabaseMgr.cpp's StreamOut, is left out where DATASTORE_OUT_OF_LINE_WRITE is defined:
//	tClientCharacter.cpp holds the out-of-line copy (0x0042a116), claimable only from a .cpp.
//	MATCH-ONLY: the int32u, int32s and double writes are defined only where
//	DATASTORE_INLINE_NUMBER_WRITE is defined (tMetaMapHex.cpp, whose StreamOut inlines them);
//	tAuctionItem.cpp's and tMetaMapDesc.cpp's StreamOut call the exe's out-of-line copies.
#if !defined( DataStoreOutput_h )
#define DataStoreOutput_h

#pragma once

#include "Common/BaseTypes.h"
#include "Common/DataStore/DataStoreBuffer.h"

#include <string>

namespace nDataStore
{

class tOutput
{
public:
	typedef int32u iOutputFlags;

	enum
	{
		kNone			= 0,
		kOutputVersion	= 1 << 0,
		kOutputCRC		= 1 << 1,
	};

	inline tOutput( int32u initSize = 0 );
	inline tOutput( tBuffer* buffer );
	inline tOutput( const tOutput& copy );
	inline tOutput& operator=( const tOutput& copy );
	inline ~tOutput();

	inline static void	SetDefaultFlags( iOutputFlags newDefaultFlags );

	inline void			AddFlags( iOutputFlags newFlags, bool replaceNewFlags = false );
	inline void			RemoveFlags( iOutputFlags flagsToRemove );
	inline iOutputFlags	GetCurFlags() const;
	inline bool			CheckFlags( iOutputFlags flags ) const;

	inline const tBuffer* GetBuffer() const;
	inline tBuffer* Close( bool cleanUpBuffer = false );

	inline void Put( byte data );
	inline void Write( const byte* data, int32 size );

	template< class tType >
	friend tOutput& operator<<( tOutput& output, const std::vector< tType >& value );

	inline tOutput& operator<<( tOutput& (*func)( tOutput& input ) );
	inline tOutput& operator<<( const tBuffer& buffer );
	inline tOutput& operator<<( const std::string& string );
	inline tOutput& operator<<( const char* string );
	inline tOutput& operator<<( bool value );
	inline tOutput& operator<<( char value );
	inline tOutput& operator<<( int8s value );
	inline tOutput& operator<<( int8u value );
	inline tOutput& operator<<( int16u value );
	inline tOutput& operator<<( int16s value );
	inline tOutput& operator<<( int32u value );
	inline tOutput& operator<<( int32s value );
	inline tOutput& operator<<( float value );
	inline tOutput& operator<<( double value );
	inline tOutput& operator<<( long double value );

	friend std::ostream& operator<<( std::ostream& outputStream, const tOutput& output );

protected:
	iOutputFlags		fOutputFlags;
	static iOutputFlags fDefaultOutputFlags;
	bool fCleanUpBuffer;
	tBuffer* fBuffer;
};

void tOutput::Put( byte data )
{
	fBuffer->Put( data );
}

void tOutput::Write( const byte* data, int32 size )
{
	fBuffer->Write( data, size );
}
#ifndef DATASTORE_OUT_OF_LINE_WRITE
tOutput& tOutput::operator<<( const std::string& string )
{
	int32u size = string.size();
	Write( (const byte*)&size, sizeof( size ) );

	Write( (const byte*)string.data(), size );

	return *this;
}
#endif
tOutput& tOutput::operator<<( bool value )
{
	Put( (byte)value );

	return *this;
}

#ifdef DATASTORE_INLINE_NUMBER_WRITE
tOutput& tOutput::operator<<( int32u value )
{
	Write( (const byte*)&value, sizeof( value ) );

	return *this;
}

tOutput& tOutput::operator<<( int32s value )
{
	Write( (const byte*)&value, sizeof( value ) );

	return *this;
}

tOutput& tOutput::operator<<( double value )
{
	Write( (const byte*)&value, sizeof( value ) );

	return *this;
}
#endif

template< class tType >
tOutput& operator<<( tOutput& output, const std::vector< tType >& value )
{
	int32u size = value.size();
	output.Write( (const byte*)&size, sizeof( size ) );

	for( std::vector< tType >::const_iterator i = value.begin();
		 i != value.end();
		 i++ )
	{
		output << *i;
	}

	return output;
}

};

#endif
