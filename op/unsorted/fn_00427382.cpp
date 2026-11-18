// Unit:      unsorted/fn_00427382.cpp
// Functions: 0x00427382 to 0x00427fb2, 37 in op/UNITS.tsv

#include "unsorted/tCurrentTime.h"
#include "unsorted/tPoliticalTensionMatrix.h"
#include "unsorted/tEntity.h"
#include "Common/Database/DatabaseID.h"
#include "Common/Database/DatabaseObject.h"
#include <new>

// HYPOTHESIS: the compiler's implicit memberwise copy constructor, written out.
// FUNCTION: STARFLEETOP 0x0042739e
tCurrentTime::tCurrentTime( const tCurrentTime& source )
	: nDataStore::tStreamable( source ),
	  fField0c( source.fField0c ),
	  fField10( source.fField10 ),
	  fField14( source.fField14 ),
	  fField18( source.fField18 ),
	  fField1c( source.fField1c )
{
	struct tBlock { long fWord[4]; };
	*(tBlock*)fField20 = *(const tBlock*)source.fField20;
}

// HYPOTHESIS: the compiler's implicit memberwise copy constructor, written out.
// FUNCTION: STARFLEETOP 0x004273eb
tPoliticalTensionMatrix::tPoliticalTensionVector::tPoliticalTensionVector( const tPoliticalTensionVector& source )
	: nDataStore::tStreamable( source ),
	  field_0xc( source.field_0xc ),
	  field_0x10( source.field_0x10 )
{
}

// 0x0042743e is tPoliticalTensionVector's implicit destructor: the original declares none, and
// the compiler-generated one stores no vtable. It has no source line of its own, so it is not
// written here.
//
//
//
//

// HYPOTHESIS: the team of the entity with the ID, or -1 when there is no such entity. It
// clears its argument on return and never reads ecx: a __stdcall function, or a member of a
// class not yet found that does not use this.
// FUNCTION: STARFLEETOP 0x0042748f
tID __stdcall fn_0042748f( tID aID )
{
	tEntity *entity = gObject_008e1060.fn_lookup( aID );
	return entity ? entity->fTeam : -1;
}

// HYPOTHESIS: the list 0x00426f1a returns for a team, likely the class tEntity.cpp declares
// locally as tObject_00426b63; 0x00426bac tests a value against it.
class tObject_00426bac
{
public:
	bool fn_00426bac(int aValue);
	int fn_00426bcc();
};

// HYPOTHESIS: 0x00426f1a never reads ecx here (tObject_0090be60.h declares it a member), so
// it is declared as a free __stdcall function, as fn_0042748f above is written.
tObject_00426bac * __stdcall fn_00426f1a( tID aTeam );

// HYPOTHESIS: whether the team list of the entity with the ID passes 0x00426bac for the
// value; false when there is no such entity or list.
// FUNCTION: STARFLEETOP 0x004274b8
bool __stdcall fn_004274b8( tID aID, int aValue )
{
	tEntity *entity = gObject_008e1060.fn_lookup( aID );
	tObject_00426bac *list;
	if (entity && (list = fn_00426f1a( entity->fTeam )) != 0)
		return list->fn_00426bac( aValue );
	return false;
}

// HYPOTHESIS: 0x00426bcc of the team's list, or zero when the team has no list.
// FUNCTION: STARFLEETOP 0x004274f4
int __stdcall fn_004274f4( tID aTeam )
{
	tObject_00426bac *list = fn_00426f1a( aTeam );
	return list ? list->fn_00426bcc() : 0;
}

// HYPOTHESIS: the entry class of the table below; 0x00426da7 is called on it before the
// value at +0xc4 is read.
class tObject_00426da7
{
public:
	void fn_00426da7();
	void fn_00426ca8( int aArg1, int aArg2 );
	void fn_00426e40( int aArg1, int aArg2 );
	void fn_00426e61( int aArg1, int aArg2 );
	bool fn_00426c23( int aArg );
	void fn_00426c5e( int aArg );
	int fn_00426e7a( int aArg );
	char pad00[0x10];
	int field_0x10[20];
	char pad60[0x64];
	int field_0xc4;
	int field_0xc8;
};

// HYPOTHESIS: the owner of a table of twenty entries at +0x54; class not yet found.
class tObject_0042750f
{
public:
	int fn_0042750f( int aIndex );
	int fn_0042753a( int aIndex );
	void fn_0042755c( int aIndex, int aArg1, int aArg2 );
	int fn_00427581( int aIndex, int aOther );
	int fn_004275ae( tID aID, tID aOther );
	void fn_004275fe( int aIndex, int aArg1, int aArg2 );
	bool fn_00427623( int aArg, int aIndex );
	void fn_00427648( int aArg, int aIndex );
	int fn_00427669( tEntity *aEntity, int aIndex );
	int fn_00427695( int aIndex, int aValue );
	void fn_004276bf( int aIndex, int aArg1, int aArg2 );
	int fn_004276e4( int aIndex, int aValue, int aArg );
	char pad00[0x54];
	tObject_00426da7 *field_0x54[20];
};

// HYPOTHESIS: the value at +0xc4 of the table entry, after 0x00426da7 on it; zero for an
// index out of range or an empty entry.
// FUNCTION: STARFLEETOP 0x0042750f
int tObject_0042750f::fn_0042750f( int aIndex )
{
	tObject_00426da7 *entry;
	if (aIndex < 0 || aIndex >= 20 || (entry = field_0x54[aIndex]) == 0)
		return 0;
	entry->fn_00426da7();
	return entry->field_0xc4;
}

// HYPOTHESIS: the value at +0xc8 of the table entry; zero for an index out of range or an
// empty entry.
// FUNCTION: STARFLEETOP 0x0042753a
int tObject_0042750f::fn_0042753a( int aIndex )
{
	tObject_00426da7 *entry;
	if (aIndex < 0 || aIndex >= 20 || (entry = field_0x54[aIndex]) == 0)
		return 0;
	return entry->field_0xc8;
}

// HYPOTHESIS: passes both arguments to 0x00426ca8 of the table entry; nothing for an index
// out of range or an empty entry.
// FUNCTION: STARFLEETOP 0x0042755c
void tObject_0042750f::fn_0042755c( int aIndex, int aArg1, int aArg2 )
{
	tObject_00426da7 *entry;
	if (aIndex >= 0 && aIndex < 20 && (entry = field_0x54[aIndex]) != 0)
		entry->fn_00426ca8( aArg1, aArg2 );
}

// HYPOTHESIS: entry aIndex's value at +0x10 for entry aOther, true for an entry with itself;
// zero for an index out of range or an empty entry.
// FUNCTION: STARFLEETOP 0x00427581
int tObject_0042750f::fn_00427581( int aIndex, int aOther )
{
	tObject_00426da7 *entry;
	if (aIndex < 0 || aIndex >= 20 || (entry = field_0x54[aIndex]) == 0)
		return 0;
	if (aIndex == aOther)
		return 1;
	return entry->field_0x10[aOther];
}

// HYPOTHESIS: 0x00427581 for the teams of the two entities with the IDs; zero when either
// entity does not exist.
// FUNCTION: STARFLEETOP 0x004275ae
int tObject_0042750f::fn_004275ae( tID aID, tID aOther )
{
	tEntity *entity = gObject_008e1060.fn_lookup( aID );
	tEntity *other = gObject_008e1060.fn_lookup( aOther );
	// MATCH-ONLY: the local makes the first team load into a register before its push, as the
	// original does; the original likely passed the two teams through an inline helper.
	tID team;
	return entity && other ? (team = entity->fTeam, fn_00427581( team, other->fTeam )) : 0;
}

// HYPOTHESIS: passes both arguments to 0x00426e40 of the table entry; nothing for an index
// out of range or an empty entry.
// FUNCTION: STARFLEETOP 0x004275fe
void tObject_0042750f::fn_004275fe( int aIndex, int aArg1, int aArg2 )
{
	tObject_00426da7 *entry;
	if (aIndex >= 0 && aIndex < 20 && (entry = field_0x54[aIndex]) != 0)
		entry->fn_00426e40( aArg1, aArg2 );
}

// HYPOTHESIS: 0x00426c23 of the table entry for the argument; false for an index out of range
// or an empty entry.
// FUNCTION: STARFLEETOP 0x00427623
bool tObject_0042750f::fn_00427623( int aArg, int aIndex )
{
	tObject_00426da7 *entry;
	if (aIndex < 0 || aIndex >= 20 || (entry = field_0x54[aIndex]) == 0)
		return false;
	return entry->fn_00426c23( aArg );
}

// HYPOTHESIS: 0x00426c5e of the table entry for the argument; nothing for an index out of
// range or an empty entry.
// FUNCTION: STARFLEETOP 0x00427648
void tObject_0042750f::fn_00427648( int aArg, int aIndex )
{
	tObject_00426da7 *entry;
	if (aIndex >= 0 && aIndex < 20 && (entry = field_0x54[aIndex]) != 0)
		entry->fn_00426c5e( aArg );
}

// HYPOTHESIS: 0x00427581 for the table entry and the entity's team; zero for no entity, an
// index out of range or an empty entry.
// FUNCTION: STARFLEETOP 0x00427669
int tObject_0042750f::fn_00427669( tEntity *aEntity, int aIndex )
{
	if (aEntity == 0 || aIndex < 0 || aIndex >= 20 || field_0x54[aIndex] == 0)
		return 0;
	return fn_00427581( aIndex, aEntity->fTeam );
}

// HYPOTHESIS: the first entry for which 0x00427581 of entry aIndex gives aValue; -1 if none.
// FUNCTION: STARFLEETOP 0x00427695
int tObject_0042750f::fn_00427695( int aIndex, int aValue )
{
	for (int i = 0; i < 20; i++)
		if (fn_00427581( aIndex, i ) == aValue)
			return i;
	return -1;
}

// HYPOTHESIS: passes both arguments to 0x00426e61 of the table entry; nothing for an index
// out of range or an empty entry.
// FUNCTION: STARFLEETOP 0x004276bf
void tObject_0042750f::fn_004276bf( int aIndex, int aArg1, int aArg2 )
{
	tObject_00426da7 *entry;
	if (aIndex >= 0 && aIndex < 20 && (entry = field_0x54[aIndex]) != 0)
		entry->fn_00426e61( aArg1, aArg2 );
}

// HYPOTHESIS: the sum of 0x00426e7a for the argument over the entries for which 0x00427581 of
// entry aIndex gives aValue.
// FUNCTION: STARFLEETOP 0x004276e4
int tObject_0042750f::fn_004276e4( int aIndex, int aValue, int aArg )
{
	int sum = 0;
	int i = 19;
	do
	{
		if (fn_00427581( aIndex, i ) == aValue && field_0x54[i] != 0)
			sum += field_0x54[i]->fn_00426e7a( aArg );
	} while (i--);
	return sum;
}

// HYPOTHESIS: the layout is that of the library's vector of four-byte values (allocator,
// first, last, end): 0x0042772f is its push_back and 0x00427883 its insert of a count of
// copies. Neither has a caller in this unit, so they are written as members of this class.
class tObject_0042772f
{
public:
	void fn_0042772f( const int& aArg );
	void fn_00427883( int *aPos, unsigned int aCount, const int& aValue );
	unsigned int fn_size() const;
	char pad00[0x4];
	int *field_0x4;
	int *field_0x8;
	int *field_0xc;
};

// FUNCTION: STARFLEETOP 0x0042772f
void tObject_0042772f::fn_0042772f( const int& aArg )
{
	fn_00427883( field_0x8, 1, aArg );
}

// MATCH-ONLY: the library's _Construct for one element, written out (below, at 0x00427a53) so that it has source lines here.
namespace std
{
inline void _Construct( tPoliticalTensionMatrix::tPoliticalTensionVector::tPoliticalTension *_P, const tPoliticalTensionMatrix::tPoliticalTensionVector::tPoliticalTension& _V );
}

// MATCH-ONLY: the library's copy constructor of the vector at 0x10 of tPoliticalTensionVector,
// emitted in this unit because the copy constructor above uses it. The original is the
// template in the library header; it is written out here so that it has source lines here.
// FUNCTION: STARFLEETOP 0x00427740
template<>
std::vector< tPoliticalTensionMatrix::tPoliticalTensionVector::tPoliticalTension >::vector( const std::vector< tPoliticalTensionMatrix::tPoliticalTensionVector::tPoliticalTension >& _X )
	: allocator( _X.allocator )
{
	_First = allocator.allocate( _X.size(), (void*)0 );
	_Last = _Ucopy( _X.begin(), _X.end(), _First );
	_End = _Last;
}

// MATCH-ONLY: the library's destructor of the vector at 0x10 of tPoliticalTensionVector, written
// out here, as the copy constructor above is, so that it has source lines here.
// FUNCTION: STARFLEETOP 0x004277ae
template<>
std::vector< tPoliticalTensionMatrix::tPoliticalTensionVector::tPoliticalTension >::~vector()
{
	_Destroy( _First, _Last );
	allocator.deallocate( _First, _End - _First );
	_First = 0, _Last = 0, _End = 0;
}

// MATCH-ONLY: the library vector's helpers (_Ucopy, _Ufill, _Allocate, fill, copy_backward,
// _Destroy, deallocate, size), which the
// original inlines into 0x00427883; written for this class, unclaimed.
inline int *fn_copyConstruct( int *aFirst, int *aLast, int *aDest )
{
	for (; aFirst != aLast; ++aDest, ++aFirst)
		new ((void *)aDest) int( *aFirst );
	return aDest;
}

inline void fn_fillConstruct( int *aFirst, unsigned int aCount, const int& aValue )
{
	for (; 0 < aCount; --aCount, ++aFirst)
		new ((void *)aFirst) int( aValue );
}

inline int *fn_allocate( int aCount )
{
	if (aCount < 0)
		aCount = 0;
	return (int *)operator new( (unsigned int)aCount * sizeof(int) );
}

inline void fn_fill( int *aFirst, int *aLast, const int& aValue )
{
	for (; aFirst != aLast; ++aFirst)
		*aFirst = aValue;
}

inline int *fn_copyBackward( int *aFirst, int *aLast, int *aDest )
{
	while (aFirst != aLast)
		*--aDest = *--aLast;
	return aDest;
}

inline void fn_destroy( int *aFirst, int *aLast )
{
	for (; aFirst != aLast; ++aFirst)
		;
}

inline void fn_deallocate( void *aPointer, unsigned int )
{
	operator delete( aPointer );
}

inline unsigned int tObject_0042772f::fn_size() const
{
	return field_0x4 == 0 ? 0 : field_0x8 - field_0x4;
}

// HYPOTHESIS: the library's vector insert of aCount copies of aValue at aPos.
// FUNCTION: STARFLEETOP 0x00427883
void tObject_0042772f::fn_00427883( int *aPos, unsigned int aCount, const int& aValue )
{
	if ((unsigned int)(field_0xc - field_0x8) < aCount)
	{
		unsigned int n = fn_size() + (aCount < fn_size() ? fn_size() : aCount);
		int *s = fn_allocate( n );
		int *q = fn_copyConstruct( field_0x4, aPos, s );
		fn_fillConstruct( q, aCount, aValue );
		fn_copyConstruct( aPos, field_0x8, q + aCount );
		fn_destroy( field_0x4, field_0x8 );
		fn_deallocate( field_0x4, field_0xc - field_0x4 );
		field_0xc = s + n;
		field_0x8 = s + fn_size() + aCount;
		field_0x4 = s;
	}
	else if ((unsigned int)(field_0x8 - aPos) < aCount)
	{
		fn_copyConstruct( aPos, field_0x8, aPos + aCount );
		fn_fillConstruct( field_0x8, aCount - (field_0x8 - aPos), aValue );
		fn_fill( aPos, field_0x8, aValue );
		field_0x8 += aCount;
	}
	else if (0 < aCount)
	{
		fn_copyConstruct( field_0x8 - aCount, field_0x8, field_0x8 );
		fn_copyBackward( aPos, field_0x8 - aCount, field_0x8 );
		fn_fill( aPos, aPos + aCount, aValue );
		field_0x8 += aCount;
	}
}

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

// HYPOTHESIS: the class named in RTTI (vtable 0x00798264), the element of the vector at 0x3c
// of tPlayingTeamInfo; layout read from its copy constructor at 0x00427c6b.
class tShip : public nDatabase::tDatabaseObject
{
public:
	tShip( const tShip& source );
	virtual	~tShip();

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

// HYPOTHESIS: the class named in RTTI (vtable 0x00798234); layout read from its copy
// constructor at 0x00427ab7: a string, four scalars, an ID, a vector copied out of line
// (0x00427bb2), two scalars and a byte.
class tPlayingTeamInfo : public nDataStore::tStreamable
{
public:
	tPlayingTeamInfo( const tPlayingTeamInfo& source );
	virtual	~tPlayingTeamInfo();

	virtual	void StreamOut( nDataStore::tOutput& output ) const;
	virtual	void StreamIn( nDataStore::tInput& input );

	std::string			field_0xc;
	int32u				field_0x1c;
	int32u				field_0x20;
	int32u				field_0x24;
	int32u				field_0x28;
	nDatabase::tID		field_0x2c;
	std::vector< tShip >	field_0x3c;
	int32u				field_0x4c;
	int32u				field_0x50;
	int8u				field_0x54;
};

namespace std
{
// FUNCTION: STARFLEETOP 0x00427a53
inline void _Construct( tPoliticalTensionMatrix::tPoliticalTensionVector::tPoliticalTension *_P, const tPoliticalTensionMatrix::tPoliticalTensionVector::tPoliticalTension& _V )
{
	new ((void *)_P) tPoliticalTensionMatrix::tPoliticalTensionVector::tPoliticalTension( _V );
}

// MATCH-ONLY: the library's _Construct for one element, written out so that it has source lines
// here; not inline, since nothing in this unit instantiates it.
// FUNCTION: STARFLEETOP 0x00427a8a
void _Construct( tPlayingTeamInfo *_P, const tPlayingTeamInfo& _V )
{
	new ((void *)_P) tPlayingTeamInfo( _V );
}
}



// HYPOTHESIS: the compiler's implicit memberwise copy constructor, written out.
// FUNCTION: STARFLEETOP 0x00427ab7
tPlayingTeamInfo::tPlayingTeamInfo( const tPlayingTeamInfo& source )
	: nDataStore::tStreamable( source ),
	  field_0xc( source.field_0xc ),
	  field_0x1c( source.field_0x1c ),
	  field_0x20( source.field_0x20 ),
	  field_0x24( source.field_0x24 ),
	  field_0x28( source.field_0x28 ),
	  field_0x2c( source.field_0x2c ),
	  field_0x3c( source.field_0x3c ),
	  field_0x4c( source.field_0x4c ),
	  field_0x50( source.field_0x50 ),
	  field_0x54( source.field_0x54 )
{
}

// MATCH-ONLY: the implicit destructor, written out; not inlined into the deleting destructor,
// whose call the linker bound to the copy at 0x0053e638.
// FUNCTION: STARFLEETOP 0x00427b96
inline tPlayingTeamInfo::~tPlayingTeamInfo()
{
}

// MATCH-ONLY: the library's copy constructor of the vector at 0x3c of tPlayingTeamInfo, written
// out here, as the one at 0x00427740 is, so that it has source lines here.
// FUNCTION: STARFLEETOP 0x00427bb2
template<>
std::vector< tShip >::vector( const std::vector< tShip >& _X )
	: allocator( _X.allocator )
{
	_First = allocator.allocate( _X.size(), (void*)0 );
	_Last = _Ucopy( _X.begin(), _X.end(), _First );
	_End = _Last;
}

// MATCH-ONLY: the library's _Construct for one element of the vector at 0x3c of
// tPlayingTeamInfo, written out so that it has source lines here.
namespace std
{
// FUNCTION: STARFLEETOP 0x00427c1d
inline void _Construct( tShip *_P, const tShip& _V )
{
	new ((void *)_P) tShip( _V );
}
}



// HYPOTHESIS: the compiler's implicit memberwise copy constructor, written out.
// FUNCTION: STARFLEETOP 0x00427c6b
tShip::tShip( const tShip& source )
	: nDatabase::tDatabaseObject( source ),
	  field_0x48( source.field_0x48 ),
	  field_0x58( source.field_0x58 ),
	  field_0x5c( source.field_0x5c ),
	  field_0x60( source.field_0x60 ),
	  field_0x64( source.field_0x64 ),
	  field_0x68( source.field_0x68 ),
	  field_0x6c( source.field_0x6c ),
	  field_0x7c( source.field_0x7c ),
	  field_0x80( source.field_0x80 ),
	  field_0x90( source.field_0x90 ),
	  field_0x100( source.field_0x100 ),
	  field_0x30c( source.field_0x30c ),
	  field_0x430( source.field_0x430 )
{
}

// MATCH-ONLY: the implicit destructor, written out; not inlined into the deleting destructor,
// whose call the linker bound to the copy at 0x0042801a.
// FUNCTION: STARFLEETOP 0x00427d96
inline tShip::~tShip()
{
}

// HYPOTHESIS: the compiler's implicit memberwise copy constructor, written out. The original
// copies the two string arrays in loops inside this function; the wrapper's copy here is
// called out of line instead.
// FUNCTION: STARFLEETOP 0x00427db2
tObject_00427db2::tObject_00427db2( const tObject_00427db2& source )
	: nDataStore::tStreamable( source ),
	  field_0xc( source.field_0xc ),
	  field_0xf0( source.field_0xf0 ),
	  field_0xf1( source.field_0xf1 ),
	  field_0xf2( source.field_0xf2 ),
	  field_0xf3( source.field_0xf3 ),
	  field_0xf4( source.field_0xf4 ),
	  field_0xf5( source.field_0xf5 ),
	  field_0xf6( source.field_0xf6 ),
	  field_0xf7( source.field_0xf7 ),
	  field_0xf8( source.field_0xf8 ),
	  field_0xf9( source.field_0xf9 ),
	  field_0xfa( source.field_0xfa ),
	  field_0xfb( source.field_0xfb ),
	  field_0xfc( source.field_0xfc ),
	  field_0x10c( source.field_0x10c ),
	  field_0x10f( source.field_0x10f ),
	  field_0x112( source.field_0x112 ),
	  field_0x116( source.field_0x116 ),
	  field_0x148( source.field_0x148 ),
	  field_0x17a( source.field_0x17a ),
	  field_0x17e( source.field_0x17e ),
	  field_0x182( source.field_0x182 ),
	  field_0x186( source.field_0x186 ),
	  field_0x18c( source.field_0x18c ),
	  field_0x1cc( source.field_0x1cc )
{
}

// HYPOTHESIS: the compiler's implicit memberwise copy constructor, written out. Left implicit,
// VC6 builds the original's bytes (the array copied in a loop inside this function), but that
// copy has no source line to claim; the wrapper's copy here is called out of line instead.
// FUNCTION: STARFLEETOP 0x00427fb2
tOfficerList::tOfficerList( const tOfficerList& source )
	: nDataStore::tStreamable( source )
{
	for ( int i = 0; i < 7; i++ )
		new ( &field_0xc[i * 0x28] ) tOfficer( ( (const tOfficer *)source.field_0xc )[i] );
}
