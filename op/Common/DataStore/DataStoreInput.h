//	DataStoreInput.h
//
//	Reads values out of a data store buffer.
//	Declarations follow the SDK's header, without its template stream operators for the standard
//	containers and tInterlockedScalar, which no matched code uses yet. The original defines the
//	inline functions in DataStoreInput.cpp, which the header includes; here they follow the class,
//	and only the ones matched code uses are defined yet.
//
#if !defined( DataStoreInput_h )
#define DataStoreInput_h

#pragma once

#include "Common/BaseTypes.h"
#include "Common/DataStore/DataStoreBuffer.h"

#include <string>

namespace nDataStore
{

class tInput
{
public:
	typedef int32u iInputFlags;

	enum
	{
		kNone			= 0,
		kReadCRC		= 1 << 0,
		kValidateCRC	= 1 << 1,
		kReadVersion	= 1 << 2,
		kDefaultVersion = 1 << 3,
	};

	inline tInput( int32u initSize = 0 );
	inline tInput( const tBuffer* buffer );
	inline tInput( const tInput& copy );
	inline tInput& operator=( const tInput& copy );
	inline ~tInput();

	inline static void	SetDefaultFlags( iInputFlags newDefaultFlags );

	inline void			AddFlags( iInputFlags newFlags, bool replaceNewFlags = false );
	inline void			RemoveFlags( iInputFlags flagsToRemove );
	inline iInputFlags	GetCurFlags() const;
	inline bool			CheckFlags( iInputFlags flags ) const;

	inline const tBuffer* GetBuffer() const;
	inline tBuffer* Close( bool cleanUpBuffer = false );

	inline void SetGetPos( int32 pos );

	inline byte Get();
	inline void Read( byte* data, int32 size );

	inline void Rewind();

	inline tInput& operator>>( tInput& (*func)( tInput& input ) );
	inline tInput& operator>>( tBuffer& buffer );
	inline tInput& operator>>( std::string& string );
	inline tInput& operator>>( bool& value );
	inline tInput& operator>>( char& value );
	inline tInput& operator>>( int8s& value );
	inline tInput& operator>>( int8u& value );
	inline tInput& operator>>( int16u& value );
	inline tInput& operator>>( int16s& value );
	inline tInput& operator>>( int32u& value );
	inline tInput& operator>>( int32s& value );
	inline tInput& operator>>( float& value );
	inline tInput& operator>>( double& value );
	inline tInput& operator>>( long double& value );

	friend std::istream& operator>>( std::istream& inputStream, tInput& input );

protected:
	iInputFlags			fInputFlags;
	static iInputFlags	fDefaultInputFlags;
	bool fCleanUpBuffer;
	iPos fGetPos;
	const tBuffer* fBuffer;
};

byte tInput::Get()
{
	return fBuffer->Get( fGetPos++ );
}

void tInput::Read( byte* data, int32 size )
{
	fBuffer->Read( data, size, fGetPos );

	fGetPos += size;
}

tInput& tInput::operator>>( std::string& string )
{
	string.erase();

	int32u size = 0;
	Read( (byte*)&size, sizeof( size ) );

	for( int32u i = 0; i < size; i++ )
	{
		string += Get();
	}

	return *this;
}

};

#endif
