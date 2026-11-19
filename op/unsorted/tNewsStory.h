//	tNewsStory.h: tNewsStory, a news story as the server's database holds it.

#if !defined( tNewsStory_h )
#define tNewsStory_h

#pragma once

#include "Common/Database/DatabaseObject.h"

// The class and its base are the exe's RTTI (tNewsStory, nDatabase::tQuery<tNewsStory>,
// nDatabase::tMemberEval<tNewsStory,nDatabase::tID const&,...>). Its own members are not known yet.
class tNewsStory : public nDatabase::tDatabaseObject
{
public:
	// The nested class and its base are the exe's RTTI (tNewsStory::tDetailTime, vtable
	// 0x007b0c5c). HYPOTHESIS: two longs at +0xc and +0x10, read from the copy 0x0056d91b inlines.
	class tDetailTime : public nDataStore::tStreamable
	{
	public:
		virtual ~tDetailTime();

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );

		// HYPOTHESIS: an equality and an ordering test on two detail times.
		char fn_0054695e( const tDetailTime& other ) const;
		bool fn_00546986( const tDetailTime& other ) const;

		long fField0C;
		long fField10;
	};

	// HYPOTHESIS: a non-virtual accessor that returns a tID; 0x0056d874 queries on it.
	const nDatabase::tID& fn_00572b2b() const;
	// HYPOTHESIS: a non-virtual accessor that returns the story's time; 0x0056d91b queries on it.
	tDetailTime fn_0054745f() const;
	// HYPOTHESIS: two bytes that 0x005de1b8 and 0x005de222 sort stories by, and the text
	// 0x005de0cf compares for an ASCII flag.
	char fn_00547448() const;
	char fn_0054744c() const;
	class fn_0040651e_type* fn_00547450( int ascii ) const;

	// HYPOTHESIS: the record is 0x80 bytes (0x0056ffe3 steps back one element by 0x80). The
	// members are not recovered.
	char fPad_48[0x80 - 0x48];
};

// HYPOTHESIS: the text type returned by tNewsStory::fn_00547450; 0x0040651e compares two.
class fn_0040651e_type
{
public:
	int fn_0040651e( fn_0040651e_type* other );
};

#endif
