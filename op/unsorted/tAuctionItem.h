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
};

#endif
