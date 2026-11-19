// Placeholder written by tools/units.py. Delete this block when the unit's first
// function lands; the generator never touches a file with content.
// Unit:      unsorted/tAsset.cpp
// Functions: 0x00675b80 to 0x00675cd0, 4 in op/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   sdk-release (harness/units.json)

#include "unsorted/tAsset.h"
#include "Tiger/tStream.h"

extern short g_00882e16;

// FUNCTION: STARFLEETOP 0x00675b80
tAsset::tAsset()
{
	field_30 = 0;
	field_04 = 0;
	field_5A = g_00882e16;
}

// HYPOTHESIS: field_30 is a kind code; 3, 0x11 and 0x16 are the ones this test accepts.
// FUNCTION: STARFLEETOP 0x00675bf0
short tAsset::fn_00675bf0()
{
	int kind = (unsigned short)field_30;
	if (kind == 3 || kind == 0x11 || kind == 0x16)
		return 1;
	return 0;
}

// FUNCTION: STARFLEETOP 0x00675c10
tStream& tAsset::vfunction5( tStream& s )
{
	// HYPOTHESIS: field_04 is written as an unsigned long; the overload taking it is not known.
	s << (unsigned short)field_30 << (unsigned long)field_04 << (unsigned short)field_5A;
	if ( (unsigned short)field_5A >= 0x99 )
		s << field_08 << field_32;
	return s;
}

// FUNCTION: STARFLEETOP 0x00675cd0
tStream& tAsset::vfunction6( tStream& s )
{
	// HYPOTHESIS: field_30 is not read back; the original reads only field_04 and field_5A.
	unsigned short* p = (unsigned short*)&field_04;
	s.fn_slot2( p, 4 );
	if ( s.fSwap )
	{
		unsigned short hi = p[1];
		unsigned short lo = p[0];
		p[0] = (unsigned short)( ( hi >> 8 ) | ( hi << 8 ) );
		p[1] = (unsigned short)( ( lo >> 8 ) | ( lo << 8 ) );
	}
	unsigned short* q = (unsigned short*)&field_5A;
	s.fn_slot2( q, 2 );
	if ( s.fSwap )
		*q = (unsigned short)( ( *q >> 8 ) | ( *q << 8 ) );
	if ( *q >= 0x99 )
		s >> field_08 >> field_32;
	return s;
}
