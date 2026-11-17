#include "Common/Exceptions/Validate.h"
#include "Common/Settings.h"
#include "Common/Database/DatabaseID.h"
#include "Common/Database/Query.h"
#include "unsorted/tCurrentTime.h"
#include "unsorted/tServCharacter.h"
#include "unsorted/tShip.h"
#include "unsorted/tAuctionItem.h"
#include "unsorted/tMetaMapHex.h"
#include "unsorted/tMissionCompleteResults.h"
#include <sstream>
#include <vector>
class tMetaMapDesc; class tPoliticalTensionMatrix; class tNotify;	// HYPOTHESIS: tMetaMapDesc is declared only inside unsorted/tMetaMapDesc.cpp; RTTI names the other two through nDatabase::tQuery<tPoliticalTensionMatrix> and nDatabase::tQuery<tNotify>. Only the names are needed here.
#include "unsorted/tNewsStory.h"
#include "unsorted/tPreparedMissions.h"
class tTurn	// HYPOTHESIS: the record classes below are read from 0x0056c414 only: tTurn is 0x4c bytes with the turn at +0x48; tCampaignInfo holds the starting date index at +0xb8 and the clock at +0x108. Their other members are not known.
{
public:
	char fPad00[0x48];
	long fField48;
};
class tCampaignInfo
{
public:
	tCampaignInfo();
	~tCampaignInfo();
	tCampaignInfo& operator=( const tCampaignInfo& other );
	char fPad00[0xb8];
	long fFieldB8;
	char fPadBC[0x108 - 0xbc];
	tCurrentTime fField108;
	char fPad120[0x168 - 0x120];
};
class fn_0056c414_Database	// HYPOTHESIS: the database held at +4; probably nDatabase::tDatabase, which is declared only inside Common/Database/FileDatabaseMgr.cpp. Its fn_ members are instances of its member templates (the exe's assertions name Database.h).
{
public:
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3( tTurn* turn );
	void fn_0056ef05( nDatabase::tQuery<tTurn>& query, std::vector<tTurn>* result, bool lock );
	void fn_0056f05f( nDatabase::tQuery<tServCharacter>& query, std::vector<tServCharacter>* result, bool lock );
	void fn_0056f19b( nDatabase::tQuery<tServCharacter>& query, std::vector<tServCharacter>* result );
	void fn_0056f2a4( nDatabase::tQuery<tShip>& query, std::vector<tShip>* result, bool lock );
	void fn_0056f3e0( nDatabase::tQuery<tShip>& query, std::vector<tShip>* result );
	void fn_0056f4df( nDatabase::tQuery<tAuctionItem>& query, std::vector<tAuctionItem>* result, bool lock );
	void fn_0056f61b( nDatabase::tQuery<tAuctionItem>& query, std::vector<tAuctionItem>* result );
	void fn_0056f703( nDatabase::tQuery<tMetaMapHex>& query, std::vector<tMetaMapHex>* result, bool lock );
	void fn_0056f83f( nDatabase::tQuery<tMetaMapHex>& query, std::vector<tMetaMapHex>* result );
	void fn_0056f909( const nDatabase::tID& id, tMetaMapHex* hex );
	void fn_0056f961( const nDatabase::tID& id, tShip* ship );
	void fn_0056f9d7( nDatabase::tQuery<tMetaMapDesc>& query, std::vector<tMetaMapDesc>* result, bool lock );
	void fn_0056fb13( nDatabase::tQuery<tMetaMapDesc>& query, std::vector<tMetaMapDesc>* result );
	void fn_0056fbf5( nDatabase::tQuery<tPoliticalTensionMatrix>& query, std::vector<tPoliticalTensionMatrix>* result );
	void fn_0056fcd7( nDatabase::tQuery<tCampaignInfo>& query, std::vector<tCampaignInfo>* result, bool lock );
	void fn_0056fe13( nDatabase::tQuery<tCampaignInfo>& query, std::vector<tCampaignInfo>* result );
	void fn_0056fefb( nDatabase::tQuery<tNotify>& query, std::vector<tNotify>* result );
	void fn_0056ffe3( nDatabase::tQuery<tNewsStory>& query, std::vector<tNewsStory>* result );
	void fn_0057013d( nDatabase::tQuery<tNewsStory>& query, std::vector<tNewsStory>* result, bool lock );
	void fn_00570372( nDatabase::tQuery<tPreparedMissions>& query, std::vector<tPreparedMissions>* result, bool lock );
	void fn_005704ae( nDatabase::tQuery<tPreparedMissions>& query, std::vector<tPreparedMissions>* result );
	void fn_00570578( tServCharacter* character, tServCharacter* characterStore );	// HYPOTHESIS: both pointers are characters (assertion texts "character", "characterStore" at 0x0056dbe5).
	void fn_00570618( nDatabase::tQuery<tPoliticalTensionMatrix>& query, std::vector<tPoliticalTensionMatrix>* result, bool lock );
	void fn_00570754( tCampaignInfo* info );
	void fn_005708ca( tServCharacter* character );
	void fn_00570a40( tAuctionItem* item );
	void fn_00570bb6( tPoliticalTensionMatrix* matrix );
	void fn_00570d2c( tMetaMapHex* hex );
	void fn_00570ea2( tMetaMapDesc* description );
};
tSettings* fn_005d481f();	// HYPOTHESIS: returns the server settings; name not known.
namespace nAsyncDatabaseProcedure {
class tAsyncDatabaseSingle	// HYPOTHESIS: only the slots that rebuilt functions call or define carry signatures.
{
public:
	virtual void vfunction1();
	virtual void vfunction2( tCampaignInfo* campaignInfo );
	virtual void vfunction3( std::vector<nDatabase::tID> ids, std::vector<tServCharacter>* characters, int lock );
	virtual void vfunction4( std::vector<tServCharacter>* characters );
	virtual void vfunction5( std::vector<nDatabase::tID> ids, std::vector<tServCharacter>* characters, int lock );
	virtual void vfunction6();
	virtual void vfunction7( std::vector<tServCharacter>* characters );
	virtual void vfunction8( std::vector<tServCharacter>* characters, int lock );
	virtual void vfunction9( std::vector<nDatabase::tID> ids, std::vector<tShip>* ships, int lock );
	virtual void vfunction10();
	virtual void vfunction11( std::vector<nDatabase::tID> ids, std::vector<tAuctionItem>* items, int lock );
	virtual void vfunction12( std::vector<tAuctionItem>* items );
	virtual void vfunction13( tObject_005b69d0 point, std::vector<tMetaMapHex>* hexes, int lock );
	virtual void vfunction14( nDatabase::tID id, tServCharacter* character, int lock );
	virtual void vfunction15( nDatabase::tID id, tMetaMapHex* hex, int lock );
	virtual bool vfunction16( nDatabase::tID id, tShip* ship );
	virtual void vfunction17( std::vector<tMetaMapHex>* hexes );
	virtual void vfunction18( std::vector<tMetaMapDesc>* descs, int lock );
	virtual void vfunction19( std::vector<tPoliticalTensionMatrix>* matrices );
	virtual void vfunction20( std::vector<tCampaignInfo>* campaignInfos, int lock );
	virtual void vfunction21( std::vector<tNotify>* notifies );
	virtual void vfunction22( nDatabase::tID id, std::vector<tNewsStory>* stories );
	virtual void vfunction23( const tNewsStory::tDetailTime& time, std::vector<tNewsStory>* stories );
	virtual void vfunction24( nDatabase::tID id, bool byCharacterID, std::vector<tPreparedMissions>* queryResult, int lock );
	virtual void vfunction25( tCampaignInfo* campaignInfo );
	virtual void vfunction26( tServCharacter* character, tServCharacter* characterStore );
	virtual void vfunction27( std::vector<tMetaMapHex>* hexes );
	virtual void vfunction28( std::vector<tPoliticalTensionMatrix>* matrices );
	virtual void vfunction29( tServCharacter* character );
	virtual void vfunction30( tAuctionItem* item );
	virtual void vfunction31( tPoliticalTensionMatrix* matrix );
	virtual void vfunction32( tMetaMapHex* hex );
	virtual void vfunction33( tMetaMapDesc* description );
	virtual void vfunction34();
	virtual void vfunction35( tCampaignInfo* info );
	virtual void vfunction36();
	virtual void vfunction37();
	virtual void vfunction38();
	virtual void vfunction39();
	virtual void vfunction40();
	virtual void vfunction41();
	virtual void vfunction42();
	virtual void vfunction43();
	virtual void vfunction44();
	virtual void vfunction45();
	virtual void vfunction46();
	virtual void vfunction47();
	virtual void vfunction48( tCampaignInfo* campaignInfo );
	virtual void vfunction49();
	virtual void vfunction50();
	virtual void vfunction51();
	virtual void vfunction52();
	virtual void vfunction53();
	virtual void vfunction54();
	virtual void vfunction55();
	virtual void vfunction56();
	virtual void vfunction57( tAuctionItem* item );
	virtual void vfunction58( tServCharacter* character );
	fn_0056c414_Database* fDatabase;
	std::string fField08;	// HYPOTHESIS: the name the database is saved under (0x0056db2a).
	long fField18;	// HYPOTHESIS: the save interval in turns (0x0056db2a).
}; };
// FUNCTION: STARFLEETOP 0x0056c414
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction2( tCampaignInfo* campaignInfo )
{
	nDatabase::tQuery<tTurn> query;
	std::vector<tTurn> turns;
	long turn = -1;
	long turnsPerYear;
	long milliSecondsPerTurn;
	long baseYear;
	long startingDate[4];
	fDatabase->fn_0056ef05( query, &turns, true );
	int resultSize = turns.size();
#line 139
	validate( resultSize <= 1 );
	if( resultSize > 0 )
	{
		turn = turns[0].fField48;
		fDatabase->vfunction3( &turns[0] );
	}
	std::vector<tCampaignInfo> campaignInfos;
	vfunction20( &campaignInfos, true );
	tCampaignInfo* DBcampaignInfo = campaignInfos.begin();
#line 154
	validate( DBcampaignInfo );
	if( turn > -1 || !DBcampaignInfo->fField108.fn_005c7eac() )
	{
		campaignInfos.erase( campaignInfos.begin(), campaignInfos.end() );
		vfunction20( &campaignInfos, false );
		DBcampaignInfo = campaignInfos.begin();
#line 166
		validate( DBcampaignInfo );

		fn_005d481f()->GetValue( "Time/Clock/TurnsPerYear", &turnsPerYear );
		fn_005d481f()->GetValue( "Time/Clock/MilliSecondsPerTurn", &milliSecondsPerTurn );
		fn_005d481f()->GetValue( "Time/Clock/StartingDate/BaseYear", &baseYear );
		for( int i = 0; i < 4; i++ )
		{
			std::stringstream key;
			key << "Time/Clock/StartingDate/" << i;
			fn_005d481f()->GetValue( key.str(), &startingDate[i] );
		}

		if( turn == -1 )
		{
			turn = startingDate[DBcampaignInfo->fFieldB8] * turnsPerYear;
		}
		tCurrentTime currentTime( turn, turnsPerYear, milliSecondsPerTurn, baseYear, startingDate[0], startingDate[1], startingDate[2], startingDate[3] );
		DBcampaignInfo->fField108 = currentTime;
		vfunction48( DBcampaignInfo );
	}

	*campaignInfo = *DBcampaignInfo;
}

// HYPOTHESIS: the parameters' meaning is read from the code: a query that matches any of the
// IDs, run with fn_0056f19b when the flag is set and with fn_0056f05f otherwise.
// FUNCTION: STARFLEETOP 0x0056c7c0
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction3( std::vector<nDatabase::tID> ids, std::vector<tServCharacter>* characters, int lock )
{
	std::vector<nDatabase::tEval<tServCharacter>*> evals;
	nDatabase::tQuery<tServCharacter> query( nDatabase::tQuery<tServCharacter>::kOr );
	for( std::vector<nDatabase::tID>::iterator id = ids.begin(); id != ids.end(); id++ )
	{
		nDatabase::tMemberEval<tServCharacter, const nDatabase::tID&, std::equal_to<nDatabase::tID> >* eval =
			new nDatabase::tMemberEval<tServCharacter, const nDatabase::tID&, std::equal_to<nDatabase::tID> >( &tServCharacter::GetID, *id );
		evals.push_back( eval );
		query.AddEval( eval );
	}

	if( lock )
	{
		fDatabase->fn_0056f19b( query, characters );
	}
	else
	{
		fDatabase->fn_0056f05f( query, characters, true );
	}

	for( std::vector<nDatabase::tEval<tServCharacter>*>::iterator i = evals.begin(); i != evals.end(); i++ )
	{
		delete *i;
	}
}

// FUNCTION: STARFLEETOP 0x0056c8d6
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction4( std::vector<tServCharacter>* characters )
{
	nDatabase::tQuery<tServCharacter> query;
	nDatabase::tMemberEval<tServCharacter, bool, std::equal_to<bool> > eval( &tServCharacter::fn_00543445, true );
	query.AddEval( &eval );
	fDatabase->fn_0056f19b( query, characters );
}














// HYPOTHESIS: reads the characters with vfunction3, keeps those that pass both tests and hands
// the others to vfunction58 when the read did not lock them.
// FUNCTION: STARFLEETOP 0x0056c95c
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction5( std::vector<nDatabase::tID> ids, std::vector<tServCharacter>* characters, int lock )
{
	std::vector<tServCharacter> result;
	vfunction3( ids, characters, lock );
	for( std::vector<tServCharacter>::iterator i = characters->begin(); i != characters->end(); i++ )
	{
		if( i->fn_005433dd() && i->fn_0054340c() )
		{
			result.push_back( *i );
		}
		else if( !lock )
		{
			vfunction58( i );
		}
	}
	*characters = result;
}

// FUNCTION: STARFLEETOP 0x0056cb3b
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction7( std::vector<tServCharacter>* characters )
{
	nDatabase::tQuery<tServCharacter> query;
	nDatabase::tMemberEval<tServCharacter, bool, std::equal_to<bool> > eval1( &tServCharacter::fn_005433e8, true );
	nDatabase::tMemberEval<tServCharacter, bool, std::equal_to<bool> > eval2( &tServCharacter::fn_005433f4, false );
	nDatabase::tMemberEval<tServCharacter, bool, std::equal_to<bool> > eval3( &tServCharacter::fn_00543400, false );
	nDatabase::tMemberEval<tServCharacter, bool, std::equal_to<bool> > eval4( &tServCharacter::fn_005433dd, true );
	query.AddEval( &eval1 );
	query.AddEval( &eval2 );
	query.AddEval( &eval3 );
	query.AddEval( &eval4 );
	fDatabase->fn_0056f05f( query, characters, false );
}



// FUNCTION: STARFLEETOP 0x0056cc37
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction8( std::vector<tServCharacter>* characters, int lock )
{
	nDatabase::tQuery<tServCharacter> query;
	nDatabase::tMemberEval<tServCharacter, bool, std::equal_to<bool> > eval1( &tServCharacter::fn_0054340c, true );
	nDatabase::tMemberEval<tServCharacter, bool, std::equal_to<bool> > eval2( &tServCharacter::fn_005433dd, false );
	query.AddEval( &eval1 );
	query.AddEval( &eval2 );
	if( lock )
	{
		fDatabase->fn_0056f19b( query, characters );
	}
	else
	{
		fDatabase->fn_0056f05f( query, characters, true );
	}
}

// FUNCTION: STARFLEETOP 0x0056ccff
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction9( std::vector<nDatabase::tID> ids, std::vector<tShip>* ships, int lock )
{
	std::vector<nDatabase::tEval<tShip>*> evals;
	nDatabase::tQuery<tShip> query( nDatabase::tQuery<tShip>::kOr );
	for( std::vector<nDatabase::tID>::iterator id = ids.begin(); id != ids.end(); id++ )
	{
		nDatabase::tMemberEval<tShip, const nDatabase::tID&, std::equal_to<nDatabase::tID> >* eval =
			new nDatabase::tMemberEval<tShip, const nDatabase::tID&, std::equal_to<nDatabase::tID> >( &tShip::GetID, *id );
		evals.push_back( eval );
		query.AddEval( eval );
	}

	if( lock )
	{
		fDatabase->fn_0056f3e0( query, ships );
	}
	else
	{
		fDatabase->fn_0056f2a4( query, ships, true );
	}

	for( std::vector<nDatabase::tEval<tShip>*>::iterator i = evals.begin(); i != evals.end(); i++ )
	{
		delete *i;
	}
}












// FUNCTION: STARFLEETOP 0x0056d1b7
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction11( std::vector<nDatabase::tID> ids, std::vector<tAuctionItem>* items, int lock )
{
	std::vector<nDatabase::tEval<tAuctionItem>*> evals;
	nDatabase::tQuery<tAuctionItem> query( nDatabase::tQuery<tAuctionItem>::kOr );
	for( std::vector<nDatabase::tID>::iterator id = ids.begin(); id != ids.end(); id++ )
	{
		nDatabase::tMemberEval<tAuctionItem, const nDatabase::tID&, std::equal_to<nDatabase::tID> >* eval =
			new nDatabase::tMemberEval<tAuctionItem, const nDatabase::tID&, std::equal_to<nDatabase::tID> >( &tAuctionItem::fn_005455b7, *id );
		evals.push_back( eval );
		query.AddEval( eval );
	}

	if( lock )
	{
		fDatabase->fn_0056f61b( query, items );
	}
	else
	{
		fDatabase->fn_0056f4df( query, items, true );
	}

	for( std::vector<nDatabase::tEval<tAuctionItem>*>::iterator i = evals.begin(); i != evals.end(); i++ )
	{
		delete *i;
	}
}

// FUNCTION: STARFLEETOP 0x0056d2cd
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction12( std::vector<tAuctionItem>* items )
{
	nDatabase::tQuery<tAuctionItem> query;
	fDatabase->fn_0056f4df( query, items, true );
	tCampaignInfo campaignInfo;
	vfunction2( &campaignInfo );
	for( std::vector<tAuctionItem>::iterator i = items->begin(); i != items->end(); )
	{
		if( !i->fn_00545560( campaignInfo.fField108.fn_0055f269() ) )
		{
			vfunction57( i );
			i = items->erase( i );
		}
		else
		{
			i++;
		}
	}
}

// FUNCTION: STARFLEETOP 0x0056d3a0
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction13( tObject_005b69d0 point, std::vector<tMetaMapHex>* hexes, int lock )
{
	nDatabase::tQuery<tMetaMapHex> query;
	nDatabase::tMemberEval<tMetaMapHex, long, std::equal_to<long> > eval1( &tMetaMapHex::fn_00539f04, point.fn_0055f269() );
	nDatabase::tMemberEval<tMetaMapHex, long, std::equal_to<long> > eval2( &tMetaMapHex::fn_00539f50, point.fn_005e8f21() );
	query.AddEval( &eval1 );
	query.AddEval( &eval2 );
	if( lock )
	{
		fDatabase->fn_0056f83f( query, hexes );
	}
	else
	{
		fDatabase->fn_0056f703( query, hexes, true );
	}
}

// FUNCTION: STARFLEETOP 0x0056d48d
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction14( nDatabase::tID id, tServCharacter* character, int lock )
{
#line 598
	validate( character );
	std::vector<nDatabase::tID> ids;
	std::vector<tServCharacter> characters;
	ids.push_back( id );
	vfunction3( ids, &characters, lock );
	if( characters.size() == 1 )
	{
		*character = characters[0];
	}
}
#line 419
// HYPOTHESIS: fn_0056f909 reads the hex with that ID; the third argument, unread, is taken to be
// the lock flag of the neighbouring slots.
// FUNCTION: STARFLEETOP 0x0056d573
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction15( nDatabase::tID id, tMetaMapHex* hex, int lock )
{
#line 615
	validate( hex );
	fDatabase->fn_0056f909( id, hex );
}
#line 430
// HYPOTHESIS: fn_0056f961 reads the ship with that ID; the assertion text names it "ship".

// FUNCTION: STARFLEETOP 0x0056d5e0
bool nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction16( nDatabase::tID id, tShip* ship )
{
#line 625
	validate( ship );
	fDatabase->fn_0056f961( id, ship );
	return true;
}
#line 441
// FUNCTION: STARFLEETOP 0x0056d64f
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction17( std::vector<tMetaMapHex>* hexes )
{
#line 636
	validate( hexes );
	nDatabase::tQuery<tMetaMapHex> query;
	fDatabase->fn_0056f83f( query, hexes );
}
#line 450
// FUNCTION: STARFLEETOP 0x0056d6dc
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction18( std::vector<tMetaMapDesc>* descs, int lock )
{
	nDatabase::tQuery<tMetaMapDesc> query;
	if( lock )
	{
		fDatabase->fn_0056fb13( query, descs );
	}
	else
	{
		fDatabase->fn_0056f9d7( query, descs, true );
	}
}

// FUNCTION: STARFLEETOP 0x0056d74e
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction19( std::vector<tPoliticalTensionMatrix>* matrices )
{
	nDatabase::tQuery<tPoliticalTensionMatrix> query;
	fDatabase->fn_0056fbf5( query, matrices );
}

// FUNCTION: STARFLEETOP 0x0056d7a8
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction20( std::vector<tCampaignInfo>* campaignInfos, int lock )
{
	nDatabase::tQuery<tCampaignInfo> query;
	if( lock )
	{
		fDatabase->fn_0056fe13( query, campaignInfos );
	}
	else
	{
		fDatabase->fn_0056fcd7( query, campaignInfos, true );
	}
}

// FUNCTION: STARFLEETOP 0x0056d81a
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction21( std::vector<tNotify>* notifies )
{
	nDatabase::tQuery<tNotify> query;
	fDatabase->fn_0056fefb( query, notifies );
}

#include "unsorted/tNewsStory.h"

// FUNCTION: STARFLEETOP 0x0056d874
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction22( nDatabase::tID id, std::vector<tNewsStory>* stories )
{
	nDatabase::tQuery<tNewsStory> query;
	nDatabase::tMemberEval<tNewsStory, const nDatabase::tID&, std::equal_to<nDatabase::tID> > eval( &tNewsStory::fn_00572b2b, id );
	if( id.IsValid() )
	{
		query.AddEval( &eval );
	}
	fDatabase->fn_0056ffe3( query, stories );
}

// FUNCTION: STARFLEETOP 0x0056d91b
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction23( const tNewsStory::tDetailTime& time, std::vector<tNewsStory>* stories )
{
	nDatabase::tQuery<tNewsStory> query;
	nDatabase::tMemberEval<tNewsStory, tNewsStory::tDetailTime, std::less<tNewsStory::tDetailTime> > eval( &tNewsStory::fn_0054745f, time );
	fDatabase->fn_0057013d( query, stories, false );
}

// FUNCTION: STARFLEETOP 0x0056d9d3
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction24( nDatabase::tID id, bool byCharacterID, std::vector<tPreparedMissions>* queryResult, int lock )
{
	nDatabase::tMemberEval<tPreparedMissions, nDatabase::tID, std::equal_to<nDatabase::tID> > characterEval( &tPreparedMissions::fn_0053f22f, id );
	nDatabase::tMemberEval<tPreparedMissions, const nDatabase::tID&, std::equal_to<nDatabase::tID> > idEval( &tPreparedMissions::fn_00572b2b, id );
	nDatabase::tQuery<tPreparedMissions> query;
	if( byCharacterID )
	{
		query.AddEval( &characterEval );
	}
	else
	{
		query.AddEval( &idEval );
	}
	if( lock )
	{
		fDatabase->fn_005704ae( query, queryResult );
	}
	else
	{
		fDatabase->fn_00570372( query, queryResult, true );
	}
#line 766
	validate( byCharacterID || ( queryResult->size() <= 1 ) );
}
#line 540
#include "unsorted/tSFCFileDatabaseMgr.h"

// HYPOTHESIS: reads the campaign record, copies it out and, once the first three calls are past,
// saves the current database every fField18 turns under the name in fField08.
// FUNCTION: STARFLEETOP 0x0056db2a
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction25( tCampaignInfo* campaignInfo )
{
	std::vector<tCampaignInfo> campaignInfos;
	vfunction20( &campaignInfos, false );
	tCampaignInfo* DBcampaignInfo = campaignInfos.begin();
#line 779
	validate( DBcampaignInfo );
	*campaignInfo = *DBcampaignInfo;
	static unsigned int count = 0;	// HYPOTHESIS: a function-local counter; it could be a global.
	if( count >= 3 )
	{
		if( DBcampaignInfo->fField108.fn_0055f269() % fField18 == 0 )
		{
			fn_00576475()->Save( fField08 );
		}
	}
	else
	{
		count++;
	}
}
#line 567
// FUNCTION: STARFLEETOP 0x0056dbe5
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction26( tServCharacter* character, tServCharacter* characterStore )
{
#line 807
	validate( character );
	validate( characterStore );
	fDatabase->fn_00570578( character, characterStore );
}
#line 576
// FUNCTION: STARFLEETOP 0x0056dc5e
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction27( std::vector<tMetaMapHex>* hexes )
{
	nDatabase::tQuery<tMetaMapHex> query;
	fDatabase->fn_0056f703( query, hexes, true );
}

// FUNCTION: STARFLEETOP 0x0056dcba
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction28( std::vector<tPoliticalTensionMatrix>* matrices )
{
	nDatabase::tQuery<tPoliticalTensionMatrix> query;
	fDatabase->fn_00570618( query, matrices, true );
}

// HYPOTHESIS: the record type is a guess from the assertion text "info"; fn_00570754 stores it.
// FUNCTION: STARFLEETOP 0x0056dd16
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction35( tCampaignInfo* info )
{
#line 836
	validate( info );
	fDatabase->fn_00570754( info );
}
#line 599
// HYPOTHESIS: the record type is a guess from the assertion text "character"; fn_005708ca stores it.
// FUNCTION: STARFLEETOP 0x0056dd5e
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction29( tServCharacter* character )
{
#line 845
	validate( character );
	fDatabase->fn_005708ca( character );
}
#line 608
// HYPOTHESIS: the record type is a guess from the assertion text "item"; fn_00570a40 stores it.
// FUNCTION: STARFLEETOP 0x0056dda6
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction30( tAuctionItem* item )
{
#line 855
	validate( item );
	fDatabase->fn_00570a40( item );
}
#line 618
// HYPOTHESIS: the record type is a guess from the assertion text "matrix"; fn_00570bb6 stores it.
// FUNCTION: STARFLEETOP 0x0056ddee
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction31( tPoliticalTensionMatrix* matrix )
{
#line 865
	validate( matrix );
	fDatabase->fn_00570bb6( matrix );
}
#line 626
// FUNCTION: STARFLEETOP 0x0056de36
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction32( tMetaMapHex* hex )
{
#line 875
	validate( hex );
	fDatabase->fn_00570d2c( hex );
}
#line 634
// HYPOTHESIS: the record type is a guess from the assertion text "description"; fn_00570ea2 stores it.
// FUNCTION: STARFLEETOP 0x0056de7e
void nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction33( tMetaMapDesc* description )
{
#line 885
	validate( description );
	fDatabase->fn_00570ea2( description );
}
