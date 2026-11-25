#ifndef TMETACLIENTPLAYERLISTPANEL_H
#define TMETACLIENTPLAYERLISTPANEL_H

#include "Switch/Switch.h"

// HYPOTHESIS: the player list panel. The name is the exe's: RTTI names the nested
// tMetaClientPlayerListPanel::tAddItem and the creator and tRemoveItem name the outer class.
// Only the members the units in this lane use are declared; the other fields are filler.
class fn_00602e18_message;

class tMetaClientPlayerListPanel
{
public:
	void fn_00602e18( fn_00602e18_message* message );
	void fn_00603aac( int32 param_1 );
	void fn_00603bfb( int32u id );

protected:
	int8u field_00[0xac];
	tSmartPtr<nSwitch::tAddress> field_ac;
	int8u field_b0[0x1c];
	nSwitch::tSwitch* field_cc;
};

// The extent the declared members reach (field_cc ends at 0xd0); not the original's full size.
typedef char tMetaClientPlayerListPanel_size_is_0xd0[ sizeof( tMetaClientPlayerListPanel ) == 0xd0 ? 1 : -1 ];

#endif
