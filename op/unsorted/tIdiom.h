//	tIdiom.h: tIdiom, a streamable holding up to two strings (RTTI .?AVtIdiom@@).

#if !defined( tIdiom_h )
#define tIdiom_h

#pragma once

#include <string>
#include <vector>

#include "Common/DataStore/Streamable.h"

// HYPOTHESIS: the strings are a std::vector<std::string> at +0xc (VC6 layout: a one-byte
// allocator padded to four, then first, last and end pointers, 16 bytes), so the class is
// 0x1c bytes. The destructor at 0x005d3817 destroys that vector (0x00402d42) and the
// constructors at 0x005d37d2 and 0x005d3852 zero the pointers; the class is embedded by value
// at +0x4c and +0x68 of another class (0x005469a4, 0x00546a54), 0x1c apart. The elements are
// 16 bytes (0x005d39b7 scales the index by 16) and are copied with the string assign at
// 0x00403b49. The member names are not known.
class tIdiom : public nDataStore::tStreamable
{
public:
	tIdiom();
	// HYPOTHESIS: 0x005d3852 passes its argument on to 0x005d3a6a, which stores it as the
	// first string through 0x005d39b7.
	tIdiom( const std::string& text );
	// HYPOTHESIS: 0x005d38a4 copies the base's two fields, then the vector out of line.
	tIdiom( const tIdiom& other );
	virtual ~tIdiom();

	// HYPOTHESIS: slots 1 and 2 of the vftable (0x005d3906, 0x005d3918) forward to the
	// vector at +0xc, so these are the streaming pair.
	virtual	void StreamOut( nDataStore::tOutput& output ) const;
	virtual	void StreamIn( nDataStore::tInput& input );

	// HYPOTHESIS: stores a string at an index below two, growing the vector to index + 1 when
	// it is short; an index of two or more asserts (Idiom.cpp line 0x5b) and is taken as one.
	void fn_005d39b7( int index, const std::string& text );

	std::vector< std::string >	field_0c;
};

typedef char tIdiom_size_is_0x1c[ sizeof( tIdiom ) == 0x1c ? 1 : -1 ];

#endif
