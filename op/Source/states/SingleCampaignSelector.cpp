// Source/states/SingleCampaignSelector.cpp

#include "Common/Exceptions/Validate.h"
#include "Common/DateAndTime.h"
#include "Common/Settings.h"
#include "Tiger/tstring.h"
#include "unsorted/tGameState.h"
#include "unsorted/tListBox_Select.h"
#include "unsorted/tSFCFileDatabaseMgr.h"
#include "unsorted/fn_004d290d_object.h"

#include <string>
#include <vector>
#include <sstream>

// HYPOTHESIS: returns the game's tSettings (0x0040c868).
tSettings* fn_0040c868();

// HYPOTHESIS: names of this state's buttons (0x0084a4b0 holds "SingleCampaignSelector",
// 0x0084a4bc "LoadGame", 0x0084a4c0 "DeleteGame").
extern void *g_0084a4b0;
extern void *g_0084a4bc;
extern void *g_0084a4c0;

namespace tState_Namespace_SingleCampaignSelector
{

class tState_SingleCampaignSelector : public tGameState
{
public:
	void fn_0047eb5d( tListBox_Select *listBox );
	void fn_0047ebca();

	// HYPOTHESIS: members unknown before +0x30; the list boxes of the saved campaigns' names,
	// empires and dates, the selected entry, and the buttons.
	char m_0x0c[0x30 - 0x0c];
	tListBox_Select *m_0x30;
	tListBox_Select *m_0x34;
	tListBox_Select *m_0x38;
	int m_0x3c;
	fn_004d290d_object m_0x40;
	char m_0xa4[0xb0 - 0xa4];
};

// FUNCTION: STARFLEETOP 0x0047ebca
void tState_SingleCampaignSelector::fn_0047ebca()
{
	tString text;

	if ( m_0x30 )
		m_0x30->vfunction51();
	if ( m_0x34 )
		m_0x34->vfunction51();
	if ( m_0x38 )
		m_0x38->vfunction51();

	tSFCFileDatabaseMgr* dbMgr = fn_00576475();
	dbMgr->SetPath( ".\\MetaAssets\\db" );

	const std::vector<fn_00576e12_entry>& campaignList = dbMgr->fn_00576e12();
	const std::vector<nDatabase::tFileDatabaseMgr::tDatabaseInfo>& dbList = dbMgr->GetListOfDatabases( true );

#line 683
	validate( campaignList.size() == dbList.size() );

	m_0x40.fn_004d269b( g_0084a4b0, g_0084a4bc, campaignList.size() > 0 );
	m_0x40.fn_004d269b( g_0084a4b0, g_0084a4c0, campaignList.size() > 0 );

	std::vector<std::string> empireNames;
	std::string empireName;
	fn_0040c868()->GetList( "RaceNames/FullEmpireNames", &empireNames, NULL );

	for ( unsigned int i = 0; i < campaignList.size(); i++ )
	{
		const fn_00576e12_entry& campaign = campaignList[i];
		const nDatabase::tFileDatabaseMgr::tDatabaseInfo& info = dbList[i];

		if ( m_0x30 )
		{
			text = info.fField0c.c_str();
			m_0x30->vfunction55( &text, -1, 0, -1 );
		}

		if ( m_0x34 && !campaign.m_0xa8.empty() )
		{
			if ( empireNames.size() > campaign.m_0xa8[0] )
				text = empireNames[campaign.m_0xa8[0]];
			else
				text = "???";
			m_0x34->vfunction55( &text, -1, 0, -1 );
		}

		if ( m_0x38 )
		{
			std::stringstream date;
			date << info.fField1c.GetYear() << "." << info.fField1c.GetMonth() << "." << info.fField1c.GetDay() << "." << info.fField1c.GetHour() << "." <<info.fField1c.GetMinute();
			text = date.str().c_str();
			m_0x38->vfunction55( &text, -1, 0, -1 );
		}
	}

	if ( dbList.size() > 0 )
	{
		if ( m_0x3c >= (int)dbList.size() )
			m_0x3c = dbList.size() - 1;
		else if ( m_0x3c < 0 )
			m_0x3c = 0;

		if ( m_0x30 )
		{
			m_0x30->vfunction70( m_0x3c );
			fn_0047eb5d( m_0x30 );
		}
	}
	else
		m_0x3c = -1;
}

}
