//	tMetaClientMapPanel.h
//
//	The map panel of the meta client and the objects it holds. Only the members the rebuilt
//	functions use are placed.
//
#if !defined( tMetaClientMapPanel_h )
#define tMetaClientMapPanel_h

#include "Common/SmartPtr.h"
#include "Common/Thread/Optex.h"
#include "Common/Thread/SingleAccessWrapper.h"
#include "unsorted/tHexPoint.h"
#include "unsorted/tMetaMapControl.h"
#include "unsorted/tVisObject.h"
#include "unsorted/fn_005ea9ce_pred.h"

class fn_005e6642_record;
class fn_005e78c6_object;
class fn_005eadc8_class;
// HYPOTHESIS: only the members read here are placed; the class at 0x1b8 and its argument are not recovered. The member at 0xfc is some container; only its 0x54b6cf method is placed. Its +0x24 takes the panel's new value (0x005e8e55); that object's size is unknown. The wrapper at +0x78 (0x174 in the panel) holds an object of unknown type whose 0x005e907e takes the panel's argument (0x005e6469).
class fn_00717950_object
{
public:
	bool fn_00717950();
};
class fn_005e907e_object
{
public:
	void fn_005e907e( long arg );
};
class tMetaClientMapQueueMember
{
public:
	void fn_005e8e55( int arg );
	unsigned char field_0;
};
class tMetaClientMapSub
{
public:
	void fn_005e94ef( long arg );
	void fn_005e9499();
	unsigned char field_0[ 0xc ];
	void* field_c;
	long field_10;
};
class tMetaClientMapQueue
{
public:
	void fn_0054b6cf( long arg );
	const tHexPoint& fn_0058c2e1();
	int fn_005e8efa();
	int fn_00423402();
	int fn_005e8efe();
	int fn_0055f269();
	unsigned char field_0[ 0x24 ];
	tMetaClientMapQueueMember field_24;
	unsigned char field_25[ 0x48 - 0x25 ];
	tSingleAccessWrapper< std::vector< fn_005e36cd_entry > > field_48;
	tSingleAccessWrapper< std::vector< fn_005ea9ce_elem > > field_60;
	tSingleAccessWrapper< fn_005e907e_object > field_78;
};
// HYPOTHESIS: a cursor over the list at 0x1b8 (node pointer, list pointer) and the receiver of its result.
class tMetaClientMapCursor
{
public:
	long fn_005e9cb0();
	void* fNode;
	tMetaClientMapSub* fList;
};
class tMetaClientMapMember
{
public:
	void fn_005b69b0( void* source );
	unsigned char field_0[ 0x14 ];
};
class tMetaClientMapReceiver
{
public:
	tMetaClientMapReceiver* fn_005e5ba6( long arg );
	long field_0;
	tMetaClientMapMember field_4;
	tMetaClientMapMember field_18;
	long field_2c;
};
class tMetaClientMapPanel
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual void vfunction12();
	virtual void vfunction13();
	virtual void vfunction14();
	virtual void vfunction15();
	virtual long vfunction16();
	virtual long vfunction17();
	virtual void vfunction18();
	virtual void vfunction19();
	virtual void vfunction20();
	int fn_005e5957( unsigned long flag );
	bool fn_005e5a8f();
	void fn_005e5abe();
	void fn_005e5ae0( long arg );
	bool fn_005e5b2d( tMetaClientMapReceiver* receiver );
	void fn_005e5bd7( long arg );
	void fn_005e5bfe( long arg );
	void fn_005e5c25( long arg );
	void fn_005e5c72( const tHexPoint& position );
	tHexPoint fn_005e5ced();
	tHexPoint fn_005e5d7a();
	long fn_005e5dde();
	long fn_005e5e01();
	void fn_005e5e24( int arg );
	void fn_005e5edc();
	void fn_005e5ee7( char arg );
	void fn_005e8c29( const tRect& area );
	void fn_005e6091();
	void fn_005e78c6( tSmartPtr< fn_005e78c6_object > object, int flag, int unused );
	void fn_005e5ef4();
	// HYPOTHESIS: the callers in unsorted/tMetaMapControl.cpp pass these the lists and areas they
	// hold; the classes of the arguments are not recovered.
	void fn_005e6aeb( int param_1, std::vector<fn_005e36cd_entry>& list );
	void fn_005e6dea( int param_1, std::vector<fn_005e381f_entry>& list );
	void fn_005e7539( const tRect* area, fn_005eadc8_class* object );
	void fn_005e7778( int param_1, const tRect* area );
	void fn_005e7f60( const tPoint* point, const tRect* area );
	void fn_005e800f( const tPoint* point, const tRect* area );
	void fn_005e620c( long x, long y, bool* changed ); // HYPOTHESIS: 0x005e65e5 tests a position and 0x005e7925 turns it into a hex; argument types are not recovered.
	bool fn_005e65e5( const tPoint* point, const tRect* area );
	void fn_005e62c4( const fn_005ea9ce_elem& entry );
	void fn_005e6469( long arg );
	bool fn_005e64ea();
	bool fn_005e653d( const tHexPoint& position );
	tHexPoint fn_005e7925( long x, long y, int arg );
	bool fn_005e6642( const fn_005e6642_record& record );
	void fn_005e66b7( const fn_005ea9ce_elem& entry );
	void fn_005e6a41();
	// HYPOTHESIS: 0x005e6991 moves every entry with the id to the position.
	void fn_005e67a8( const nDatabase::tID& id, const tHexPoint& position, long value, nDatabase::tID other );
	void fn_005e6991( const nDatabase::tID& id, const tHexPoint& position );
	unsigned char field_4[ 0xd8 - 0x4 ];
	fn_00717950_object* field_d8; // HYPOTHESIS: an object of unknown type; only 0x00717950 is called on it (0x005e64ea).
	tOptex field_dc;
	unsigned char field_f0[ 0xf8 - 0xdc - sizeof( tOptex ) ];
	tMetaMapControl* fpMetaMapControl;
	tMetaClientMapQueue field_fc;
	bool field_18c;
	unsigned char field_18d[ 0x190 - 0x18d ];
	long field_190;
	long field_194;
	unsigned char field_198[ 0x1a0 - 0x198 ];
	int field_1a0;
	unsigned char field_1a4[ 0x1b8 - 0x1a4 ];
	tMetaClientMapSub field_1b8;
	char field_1cc;
	unsigned char field_1cd[ 0x1d0 - 0x1cd ];
	tHexPoint field_1d0;
};

#endif
