// Unit:      unsorted/fn_00427382.cpp
// Functions: 0x00427382 to 0x00427fb2, 37 in op/UNITS.tsv

#include "unsorted/tCurrentTime.h"
#include "unsorted/tPoliticalTensionMatrix.h"
#include "unsorted/tEntity.h"

// HYPOTHESIS: the compiler's implicit memberwise copy constructor, written out.
// FUNCTION: STARFLEETOP 0x0042739e
tCurrentTime::tCurrentTime( const tCurrentTime& source )
	: nDataStore::tStreamable( source ),
	  field_0xc( source.field_0xc ),
	  field_0x10( source.field_0x10 ),
	  field_0x14( source.field_0x14 ),
	  field_0x18( source.field_0x18 ),
	  field_0x1c( source.field_0x1c ),
	  field_0x20( source.field_0x20 )
{
}

// HYPOTHESIS: the compiler's implicit memberwise copy constructor, written out.
// FUNCTION: STARFLEETOP 0x004273eb
tPoliticalTensionMatrix::tPoliticalTensionVector::tPoliticalTensionVector( const tPoliticalTensionVector& source )
	: nDataStore::tStreamable( source ),
	  field_0xc( source.field_0xc ),
	  field_0x10( source.field_0x10 )
{
}

// HYPOTHESIS: the original declares no destructor; its compiler-generated one stores no
// vtable. That one has no source line to claim, so this declared one (which stores the
// vtable) stands in for it.
// FUNCTION: STARFLEETOP 0x0042743e
tPoliticalTensionMatrix::tPoliticalTensionVector::~tPoliticalTensionVector()
{
}

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
	bool fn_00426c23( int aArg );
	void fn_00426c5e( int aArg );
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
