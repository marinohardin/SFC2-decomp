#ifndef TSHIPSPECS_H
#define TSHIPSPECS_H

#include <string>
#include "unsorted/tListMember.h"
#include "unsorted/tFighterSpec.h"

// HYPOTHESIS: layout read from the constructor at 0x0040891d (vtable 0x00795930, base vtable
// 0x0079590c = tListMember); the size is 0x6ac. Member types follow how the constructor
// initialises each field (a single zero byte for a char array, fldz for a float).
class tShipSpecs : public tListMember
{
	public:
		tShipSpecs();
		virtual ~tShipSpecs();
		long m_0c;
		long m_10;
		char m_14[0x20];
		long m_34;
		char m_38[0x100];
		char m_138[0x100];
		char m_238[0x100];
		char m_338[0x40];
		long m_378;
		long m_37c;
		long m_380;
		float m_384;
		long m_388;
		long m_38c;
		long m_390;
		long m_394;
		long m_398;
		long m_39c;
		long m_3a0;
		long m_3a4;
		long m_3a8;
		long m_3ac[6];
		long m_3c4;
		fn_00401862_entry m_3c8[25];
		long m_558;
		long m_55c;
		long m_560;
		long m_564;
		long m_568;
		long m_56c;
		long m_570;
		long m_574;
		long m_578;
		long m_57c;
		long m_580;
		long m_584;
		long m_588;
		long m_58c;
		long m_590;
		long m_594;
		long m_598;
		long m_59c;
		long m_5a0;
		long m_5a4[4];
		long m_5b4;
		long m_5b8;
		long m_5bc;
		long m_5c0;
		long m_5c4;
		long m_5c8;
		long m_5cc;
		long m_5d0;
		long m_5d4;
		long m_5d8;
		int m_5dc;
		long m_5e0;
		long m_5e4;
		long m_5e8;
		long m_5ec;
		long m_5f0;
		long m_5f4;
		long m_5f8;
		long m_5fc;
		long m_600;
		long m_604;
		long m_608;
		long m_60c;
		long m_610;
		long m_614;
		long m_618;
		long m_61c;
		long m_620;
		long m_624;
		long m_628;
		float m_62c;
		char m_630[0x20];
		long m_650;
		long m_654;
		long m_658;
		long m_65c;
		long m_660;
		long m_664;
		long m_668;
		// The original builds these with the constructor closure at 0x00401fe6 and destroys them
		// with 0x005f762d, the game's copies of the string's; the library's are elsewhere.
		std::string m_66c[4];
};

#endif
