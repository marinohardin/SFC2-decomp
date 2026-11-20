//	tNotifierAgent.h
//
//	Name from the exe's RTTI. Constructor 0x005cb981, destructor 0x005cba09. Size 0xc from a
//	local in the map initialize procedure.
//
#if !defined( tNotifierAgent_h )
#define tNotifierAgent_h

#include "Common/BaseTypes.h"
#include "Common/Database/DatabaseID.h"

class tNotifierAgent
{
public:
	tNotifierAgent();
	virtual ~tNotifierAgent();

	// HYPOTHESIS: both take a value the map initialize procedure reads from its relay.
	void fn_005cc674( int32 param_1 );
	void fn_005cc6db( int32 param_1 );

	// HYPOTHESIS: 0x00568b2e hands it the ID of each ship it repairs; 0x005682b2 the ID of each
	// ship it kills (0x005cba6e).
	void fn_005cba1f( const nDatabase::tID& id );
	void fn_005cba6e( const nDatabase::tID& id );
	// HYPOTHESIS: 0x00569c83 hands it the ID of the character whose stores it updated.
	void fn_005cbc48( const nDatabase::tID& id );

protected:
	int8u field_04[0x8];
};

#endif
