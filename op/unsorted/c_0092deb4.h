//	c_0092deb4.h: the object pointed to by the global at 0x0092deb4.
// Evidence:  calls (original name and file unknown)

#ifndef c_0092deb4_h
#define c_0092deb4_h 1

// HYPOTHESIS: the object pointed to by 0x0092deb4; only the virtual slots its users call are
// given types.
class c_0092deb4
{
public:
	virtual void fn_vtbl_00();
	virtual void fn_vtbl_04();
	// HYPOTHESIS: argument types unknown; tState_Campaign passes an entry of its list and -1.
	virtual void fn_vtbl_08( void *entry, int n );
	virtual void fn_vtbl_0C();
	virtual void fn_vtbl_10();
	// HYPOTHESIS: returns the player's name; tState_Campaign stores it as "PlayerName1".
	virtual const char *fn_vtbl_14();
	virtual void fn_vtbl_18();
	virtual void fn_vtbl_1C();
	virtual void fn_vtbl_20();
	virtual void fn_vtbl_24();
	virtual void fn_vtbl_28( int a, int b, int c );
	virtual void fn_vtbl_2C();
	virtual void fn_vtbl_30();
	virtual void fn_vtbl_34();
	virtual void fn_vtbl_38();
	virtual void fn_vtbl_3C();
	virtual void fn_vtbl_40();
	virtual void fn_vtbl_44();
	virtual void fn_vtbl_48();
	virtual void fn_vtbl_4C();
	virtual void fn_vtbl_50();
	virtual void fn_vtbl_54();
	virtual void fn_vtbl_58();
	virtual void fn_vtbl_5C();
	virtual void fn_vtbl_60();
	virtual void fn_vtbl_64();
	virtual void fn_vtbl_68();
	// HYPOTHESIS: returns an int that tPhaserXBank's 0x004e5958 tests for zero.
	virtual int fn_vtbl_6C();
	virtual void fn_vtbl_70();
	virtual void fn_vtbl_74();
	virtual void fn_vtbl_78();
	virtual void fn_vtbl_7C();
	virtual void fn_vtbl_80();
	virtual void fn_vtbl_84();
	virtual void fn_vtbl_88();
	virtual void fn_vtbl_8C();
	virtual void fn_vtbl_90();
	virtual void fn_vtbl_94();
	virtual void fn_vtbl_98();
	// HYPOTHESIS: no arguments; the return type is unknown.
	virtual int fn_vtbl_9C();
	virtual void fn_vtbl_A0();
	virtual void fn_vtbl_A4();
	virtual void fn_vtbl_A8();
	virtual void fn_vtbl_AC();
	virtual void fn_vtbl_B0();
	virtual void fn_vtbl_B4();
	virtual void fn_vtbl_B8();
	virtual void fn_vtbl_BC();
	virtual void fn_vtbl_C0();
	virtual void fn_vtbl_C4();
	virtual void fn_vtbl_C8();
	virtual void fn_vtbl_CC();
	virtual void fn_vtbl_D0();
	virtual void fn_vtbl_D4();
	virtual void fn_vtbl_D8();
	virtual void fn_vtbl_DC();
	virtual void fn_vtbl_E0();
	// HYPOTHESIS: returns a truth value that tPhaserXBank's 0x004e5958 tests.
	virtual bool fn_vtbl_E4();
	virtual void fn_vtbl_E8();
	virtual void fn_vtbl_EC( int n );
	virtual void fn_vtbl_F0();
	virtual void fn_vtbl_F4();
	virtual void fn_vtbl_F8();
	virtual void fn_vtbl_FC();
	virtual void fn_vtbl_100();
	virtual void fn_vtbl_104();
	virtual void fn_vtbl_108();
	virtual void fn_vtbl_10C();
	virtual void fn_vtbl_110();
	virtual void fn_vtbl_114();
	virtual void fn_vtbl_118();
	virtual void fn_vtbl_11C();
	virtual void fn_vtbl_120();
	virtual void fn_vtbl_124();
	virtual void fn_vtbl_128();
	virtual void fn_vtbl_12C();
	virtual void fn_vtbl_130();
	virtual void fn_vtbl_134();
	virtual void fn_vtbl_138();
	virtual void fn_vtbl_13C();
	// HYPOTHESIS: argument types unknown; tState_Campaign passes a small number and 0.
	virtual void fn_vtbl_140( int a, int b );
	// HYPOTHESIS: a lookup by a number and a section name ("HULL_TEMPLATE"); types unknown.
	virtual int fn_vtbl_144( int a, const char *name );
};

extern c_0092deb4 *g_0092deb4;

#endif // c_0092deb4_h
