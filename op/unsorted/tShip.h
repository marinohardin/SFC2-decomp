//	tShip.h
//
//	The database record of a ship. Only what the rebuilt functions need is declared yet.
//
#pragma once

#if !defined( tShip_h )
#define tShip_h

#include <iosfwd>
#include <string>

#include "Common/Database/DatabaseObject.h"
#include "Common/Database/DatabaseID.h"
#include "Common/DataStore/Streamable.h"

// HYPOTHESIS: the class named in RTTI; 0x70 bytes, copied out of line (0x00450e04).
class tDamageState
{
public:
	tDamageState( const tDamageState& source );
	virtual	~tDamageState();

	int8u	field_0x4[0x6c];
};

// HYPOTHESIS: RTTI names this class tStoresState::tMissileInfo, which is declared only inside
// Source/states/SupplyDockItem.cpp; a stand-in of its size (0xe4 bytes), copied out of line
// (0x004280f1).
class tObject_004280f1
{
public:
	tObject_004280f1( const tObject_004280f1& source );
	~tObject_004280f1();

	int8u	field_0x0[0xe4];
};

// HYPOTHESIS: a 0x10-byte member of tStoresState, copied out of line (0x00428259); likely a
// vector, its element type not yet known.
class tObject_00428259
{
public:
	tObject_00428259( const tObject_00428259& source );
	~tObject_00428259();

	int8u	field_0x0[0x10];
};

// HYPOTHESIS: RTTI names this class tStoresState, which is declared only inside
// Source/states/SupplyDockItem.cpp; until that declaration moves to a header, this is a
// stand-in of its size (0x20c bytes) for the copy constructor 0x00427db2. Its base has the
// vtable of nDataStore::tStreamable (0x007981f4); its own vtable is 0x007982bc.
class tObject_00427db2 : public nDataStore::tStreamable
{
public:
	tObject_00427db2( const tObject_00427db2& source );
	virtual	~tObject_00427db2();

	virtual	void StreamOut( nDataStore::tOutput& output ) const;
	virtual	void StreamIn( nDataStore::tInput& input );

	tObject_004280f1	field_0xc;
	int8u				field_0xf0;
	int8u				field_0xf1;
	int8u				field_0xf2;
	int8u				field_0xf3;
	int8u				field_0xf4;
	int8u				field_0xf5;
	int8u				field_0xf6;
	int8u				field_0xf7;
	int8u				field_0xf8;
	int8u				field_0xf9;
	int8u				field_0xfa;
	int8u				field_0xfb;
	tObject_00428259	field_0xfc;

	// MATCH-ONLY: the array members are wrapped so that the written-out copy constructor
	// can copy them as the implicit one does.
	struct tArray_0x10c
	{
		int8u	v[3];
	};
	struct tArray_0x116
	{
		int16u	v[0x19];
	};
	struct tArray_0x17a
	{
		int16u	v[2];
	};
	struct tArray_0x18c
	{
		std::string	v[4];
	};

	tArray_0x10c		field_0x10c;
	tArray_0x10c		field_0x10f;
	tArray_0x10c		field_0x112;
	tArray_0x116		field_0x116;
	tArray_0x116		field_0x148;
	tArray_0x17a		field_0x17a;
	tArray_0x17a		field_0x17e;
	tArray_0x17a		field_0x182;
	tArray_0x17a		field_0x186;
	tArray_0x18c		field_0x18c;
	tArray_0x18c		field_0x1cc;
};

// HYPOTHESIS: the class named in RTTI, the element of tOfficerList's array; 0x28 bytes, copied
// out of line (0x004281ac).
class tOfficer
{
public:
	tOfficer( const tOfficer& source );
	virtual	~tOfficer();

	int8u	field_0x4[0x24];
};

// HYPOTHESIS: the class named in RTTI (vtable 0x007982d0); 0x124 bytes: the streamable base
// and seven officers, read from its copy constructor at 0x00427fb2.
class tOfficerList : public nDataStore::tStreamable
{
public:
	tOfficerList( const tOfficerList& source );
	virtual	~tOfficerList();

	virtual	void StreamOut( nDataStore::tOutput& output ) const;
	virtual	void StreamIn( nDataStore::tInput& input );

	// MATCH-ONLY: raw storage for seven tOfficer, copied element by element in the constructor;
	// the original's array member has no source form that VC6 copies inline.
	int8u	field_0xc[7 * 0x28];
};

// The class and its base are the exe's RTTI (tShip, nDatabase::tQuery<tShip>,
// nDatabase::tMemberEval<tShip,nDatabase::tID const&,...>). The exe's vtable holds tShip's own
// functions only in the destructor's slot, the two streaming slots and tDatabaseObject's four SQL
// slots (0x00545ad1 to 0x00546021); the rest are the base's, so the class adds no virtual.
class tShip : public nDatabase::tDatabaseObject
{
public:
	// HYPOTHESIS: its allocator news 0x434 bytes and constructs it with 0x00545737.
	tShip();
	// HYPOTHESIS: the compiler's memberwise copy, out of line at 0x00427c6b; the members below
	// are read from it.
	tShip( const tShip& source );
	// HYPOTHESIS: the exe holds the destructor out of line (0x0042801a); 0x0056e095 calls it.
	virtual ~tShip();

	// HYPOTHESIS: a four-byte member getter; identical getters fold to this address.
	int fn_00410223() const;
	std::string fn_00546296() const;

	// HYPOTHESIS: accessors 0x0056e40b queries ships by (tMemberEval<tShip,bool,...>,
	// tMemberEval<tShip,nDatabase::tID const&,...>, tMemberEval<tShip,long,std::less<long> >).
	bool fn_00546278() const;
	const nDatabase::tID& fn_0055658b() const;
	long fn_005462cc() const;

	// HYPOTHESIS: the layout is read from the copy constructor 0x00427c6b; the size (0x434) is
	// also the element size of the query results that copy tShip by value (0x0056f2a4,
	// 0x0056f3e0), and 0x0056e095 takes one by value and returns with ret 0x434.
	nDatabase::tID		field_0x48;
	int8u				field_0x58;
	int32u				field_0x5c;
	int32u				field_0x60;
	int32u				field_0x64;
	int32u				field_0x68;
	std::string			field_0x6c;
	int32u				field_0x7c;
	std::string			field_0x80;
	tDamageState		field_0x90;
	tObject_00427db2	field_0x100;
	tOfficerList		field_0x30c;
	int32u				field_0x430;
};

#endif
