//	tShipSystemManager.h: tShipSystemManager (RTTI, vtable 0x0079cbe4).
// Evidence:  class (RTTI; original file unknown)

#ifndef tShipSystemManager_h
#define tShipSystemManager_h 1

// HYPOTHESIS: 0x1c bytes; tShipSystemsExt holds one at +0x968 with its next member at +0x984.
class tShipSystemManager
{
public:
	// body at 0x00442bf0
	tShipSystemManager();
	virtual ~tShipSystemManager();

	// HYPOTHESIS: a 10-byte setter at 0x005e4a72 that tTacticalStoresManager shares (identical
	// bodies folded by the linker); tShipSystemsExt's constructor passes it its argument.
	void fn_005e4a72( int param_1 );

	char m04[0x18];
};

#endif	// tShipSystemManager_h
