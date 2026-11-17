#include "Common/Exceptions/Validate.h"
#include "Common/Settings.h"
#include "unsorted/tCurrentTime.h"

#include <sstream>
#include <string>
#include <vector>

// HYPOTHESIS: the record classes below are read from 0x0056c414 only: tTurn is 0x4c bytes
// with the turn at +0x48; tCampaignInfo holds the starting date index at +0xb8 and the clock
// at +0x108. Their other members are not known.
class tTurn
{
public:
	char fPad00[0x48];
	long fField48;
};

class tCampaignInfo
{
public:
	tCampaignInfo& operator=( const tCampaignInfo& other );

	char fPad00[0xb8];
	long fFieldB8;
	char fPadBC[0x108 - 0xbc];
	tCurrentTime fField108;
};

namespace nDatabase
{

template< class T > class tEval;

// HYPOTHESIS: the layout (a vector at +4, a long at +0x14) is read from 0x0056c414.
template< class T > class tQuery
{
public:
	tQuery() : fField14( 0 ) {}
	virtual ~tQuery() {}

	std::vector< tEval<T>* > fField04;
	long fField14;
};

};

// HYPOTHESIS: the database held at +4; probably nDatabase::tDatabase, which is declared only
// inside Common/Database/FileDatabaseMgr.cpp.
class fn_0056c414_Database
{
public:
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3( tTurn* turn );

	void fn_0056ef05( nDatabase::tQuery<tTurn>& query, std::vector<tTurn>* result, bool lock );
};

// HYPOTHESIS: returns the server settings; name not known.
tSettings* fn_005d481f();

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: only the slots 0x0056c414 calls carry signatures.
class tAsyncDatabaseSingle
{
public:
	virtual void vfunction1();
	virtual void vfunction2( tCampaignInfo* campaignInfo );
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
	virtual void vfunction16();
	virtual void vfunction17();
	virtual void vfunction18();
	virtual void vfunction19();
	virtual void vfunction20( std::vector<tCampaignInfo>* campaignInfos, bool lock );
	virtual void vfunction21();
	virtual void vfunction22();
	virtual void vfunction23();
	virtual void vfunction24();
	virtual void vfunction25();
	virtual void vfunction26();
	virtual void vfunction27();
	virtual void vfunction28();
	virtual void vfunction29();
	virtual void vfunction30();
	virtual void vfunction31();
	virtual void vfunction32();
	virtual void vfunction33();
	virtual void vfunction34();
	virtual void vfunction35();
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

	fn_0056c414_Database* fDatabase;
};

};

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
