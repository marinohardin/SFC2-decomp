//	tSFCFileDatabaseMgr.h: tSFCFileDatabaseMgr, the game's manager of saved campaigns.
// Evidence:  class (RTTI, vftable 0x007b2ee4; original file unknown)

#ifndef tSFCFileDatabaseMgr_h
#define tSFCFileDatabaseMgr_h 1

#include "Common/Database/FileDatabaseMgr.h"

#include <vector>

// HYPOTHESIS: a 0x168-byte entry of the list fn_00576e12 returns, one per saved campaign (the
// assertion in SingleCampaignSelector.cpp calls it campaignList). Only the vector at +0xa8 is
// known: its first element indexes the list of full empire names.
struct fn_00576e12_entry
{
	char m_0x00[0xa8];
	std::vector<int> m_0xa8;
	char m_0xb8[0x168 - 0xb8];
};

class tSFCFileDatabaseMgr : public nDatabase::tFileDatabaseMgr
{
public:
	// HYPOTHESIS: 0x00576e12 (7 bytes, next to this class's virtual functions) returns a
	// member list.
	const std::vector<fn_00576e12_entry>& fn_00576e12();
};

// HYPOTHESIS: returns the game's tSFCFileDatabaseMgr (0x00576475).
tSFCFileDatabaseMgr* fn_00576475();

#endif	// tSFCFileDatabaseMgr_h
