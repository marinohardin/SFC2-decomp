//	tAuctionItem.h: tAuctionItem, an auction item as the server's database holds it.

#if !defined( tAuctionItem_h )
#define tAuctionItem_h

#pragma once

#include "Common/Database/DatabaseObject.h"

// The class and its base are the exe's RTTI (tAuctionItem, nDatabase::tQuery<tAuctionItem>,
// nDatabase::tMemberEval<tAuctionItem,nDatabase::tID const&,...>). Its own members are not known yet.
class tAuctionItem : public nDatabase::tDatabaseObject
{
public:
	// HYPOTHESIS: a non-virtual accessor that returns the tID held at +0x5c (lea eax, [ecx+0x5c]);
	// 0x0056d1b7 queries on it.
	const nDatabase::tID& fn_005455b7() const;

	// HYPOTHESIS: tests the item against the campaign clock; 0x0056d2cd drops the items it fails.
	bool fn_00545560( long time );

	// HYPOTHESIS: the item is 0xa8 bytes (0x0056d2cd steps its vector by 0xa8); the tID returned
	// by fn_005455b7 sits at +0x5c. The members' types are not known.
	char fPad48[0xa8 - 0x48];
};

#endif
