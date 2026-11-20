//	tNotifierAgent.h
//
//	Only what the rebuilt functions need is declared yet.
//
#if !defined( tNotifierAgent_h )
#define tNotifierAgent_h

#pragma once

#include "Common/Database/DatabaseID.h"

// HYPOTHESIS: 0x005682b2 holds one in 0xc bytes, built by 0x005cb981 and destroyed by
// 0x005cba09, and hands it the ID of each ship it kills (0x005cba6e).
class tNotifierAgent
{
public:
	tNotifierAgent();
	~tNotifierAgent();

	// HYPOTHESIS: 0x00568b2e hands it the ID of each ship it repairs.
	void fn_005cba1f( const nDatabase::tID& id );
	void fn_005cba6e( const nDatabase::tID& id );
	// HYPOTHESIS: 0x00569c83 hands it the ID of the character whose stores it updated.
	void fn_005cbc48( const nDatabase::tID& id );

	char m_00[0xc];
};

#endif
