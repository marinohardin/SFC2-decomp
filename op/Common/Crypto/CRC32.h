//	CRC32.h
//
//	A table-driven 32-bit CRC.
//	Declarations follow the SDK's header.
//
#if !defined( CRC32_h )
#define CRC32_h

#include "Common/BaseTypes.h"
#include <vector>
#include <iosfwd>	// std::string, without the locale statics <string> brings in

class tCRC32
{
public:
// Types
	enum eDefs
	{
		kTableSize		= 256,
		kCRCBufferSize	= 32768,
		kDefaultSeed	= 0xFFFFFFFF
	};

// Setup
	tCRC32();
	virtual ~tCRC32();

// Operations
	int32u CalcFileCRC( const std::string& fileName, int32u nSeed = kDefaultSeed );
	int32u UpdateCRC( int32u crc_accum, const char *data_blk_ptr, int32 data_blk_size );

protected:
// Operations
	void GenerateCRCTable();

// Fields
	int32u fTable[ kTableSize ];
	static const int32 fPolynomial;
};

#endif
