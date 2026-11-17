//	tTacticalStoresManager.h: tTacticalStoresManager (RTTI, vtable 0x0079cb64).
// Evidence:  class (RTTI; original file unknown)

#ifndef tTacticalStoresManager_h
#define tTacticalStoresManager_h 1

// HYPOTHESIS: 0x1c bytes; tShipSystemsExt holds one at +0x984 with its next member at +0x9a0.
class tTacticalStoresManager
{
public:
	// body at 0x00441d6c
	tTacticalStoresManager();
	virtual ~tTacticalStoresManager();

	// HYPOTHESIS: a 10-byte setter at 0x005e4a72 that tShipSystemManager shares (identical
	// bodies folded by the linker); tShipSystemsExt's constructor passes it its argument.
	void fn_005e4a72( int param_1 );

	char m04[0x18];
};

#endif	// tTacticalStoresManager_h
