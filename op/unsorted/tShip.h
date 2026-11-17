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

// HYPOTHESIS: tShip derives from nDatabase::tDatabaseObject: its RTTI names vtables for
// tAllocatedClass and tStreamable, and its first virtuals after the base's (slots 6 to 9)
// sit at 0x00545ad1 to 0x00546021.
class tShip : public nDatabase::tDatabaseObject
{
public:
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	// HYPOTHESIS: returns the ship's database ID (the auction item passes it on as one).
	virtual const nDatabase::tID& vfunction12() const;

	// HYPOTHESIS: a four-byte member getter; identical getters fold to this address.
	int fn_00410223() const;
	std::string fn_00546296() const;

	// HYPOTHESIS: the members are not recovered; the size (0x434) is the element size of the
	// query results that copy tShip by value (0x0056f2a4, 0x0056f3e0).
	char tShip_pad[0x434 - 0x48];
};

#endif
