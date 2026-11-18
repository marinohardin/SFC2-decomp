// tMissionCompleteResults: declarations of this class (original file unknown).
// Only what the rebuilt functions need is declared yet.
#pragma once

#if !defined( tMissionCompleteResults_h )
#define tMissionCompleteResults_h

#include "port/types.h"
#include "Common/DataStore/Streamable.h"
#include <vector>
#include "unsorted/tHexPoint.h"

// HYPOTHESIS: the element of the vector at 0x20 of tMissionCompleteResults: 0x58 bytes with a
// virtual destructor, an out-of-line copy constructor (reached through std::_Construct at
// 0x0053d1bd) and an out-of-line operator= at 0x0053d20b. Its real type is not known.
class tObject_0053d20b
{
public:
	tObject_0053d20b( const tObject_0053d20b& other );
	virtual ~tObject_0053d20b();
	tObject_0053d20b& operator=( const tObject_0053d20b& other );

	int32u fPad_04[0x15];
};

// HYPOTHESIS: a tStreamable at offset 0; tBattles::StreamOut passes it to the tStreamable
// output operator unadjusted, and the dword and byte at 0x04 and 0x08 that the assignment
// copies are tStreamable's fObjectID and fObjectVersion.
class tMissionCompleteResults : public nDataStore::tStreamable
{
public:
	tMissionCompleteResults();
	virtual ~tMissionCompleteResults();
	// The original's assignment is compiler-generated (memberwise, no self test); tBattles's
	// unit holds its copy at 0x00547d3b.
	tMissionCompleteResults& operator=( const tMissionCompleteResults& other );

	virtual	void StreamOut( nDataStore::tOutput& output ) const;
	virtual	void StreamIn( nDataStore::tInput& input );

	// Assigned by tHexPoint's out-of-line operator= (0x005b69d0).
	tHexPoint m_0c;
	// The exe's assignment calls std::vector's operator= (0x005481bd) for it.
	std::vector< tObject_0053d20b > m_20;
};

// MATCH-ONLY: the original's vector assignment is the library template's, instantiated for
// tMissionCompleteResults::operator=; tBattles's unit writes it out as a specialization so its
// lines are in that unit. The code is the library's.
template<>
std::vector< tObject_0053d20b >& std::vector< tObject_0053d20b >::operator=( const std::vector< tObject_0053d20b >& _X );

#endif
