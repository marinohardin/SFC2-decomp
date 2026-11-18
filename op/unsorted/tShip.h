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

// The class and its base are the exe's RTTI (tShip, nDatabase::tQuery<tShip>,
// nDatabase::tMemberEval<tShip,nDatabase::tID const&,...>). The exe's vtable holds tShip's own
// functions only in the destructor's slot, the two streaming slots and tDatabaseObject's four SQL
// slots (0x00545ad1 to 0x00546021); the rest are the base's, so the class adds no virtual.
class tShip : public nDatabase::tDatabaseObject
{
public:
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

	// HYPOTHESIS: the members are not recovered; the size (0x434) is the element size of the
	// query results that copy tShip by value (0x0056f2a4, 0x0056f3e0), and 0x0056e095 takes one
	// by value and returns with ret 0x434.
	char tShip_pad[0x434 - 0x48];
};

#endif
