#include "Common/Database/DatabaseID.h"
#include "Tiger/tpart.h"

namespace nDatabase
{

// FUNCTION: STARFLEETOP 0x006fa440
bool tID::IsValid() const
{
	return fObjectID != 0 ? true : false;
}

// FUNCTION: STARFLEETOP 0x006fa460
bool tID::operator==( const tID& compare ) const
{
	return fObjectID == compare.fObjectID ? true : false;
}

}

// HYPOTHESIS: tgPrefsPart derives from tPart; only the slot mActivate touches (+0xc) is known.
class tgPrefsPart : public tPart
{
public:
	virtual	iTruth	mIsValid	( void );
	virtual	iTruth	mIsReady	( void );
	virtual	void	mActivate	( void );
	virtual	void	mDeactivate	( void );

	int32s			fField0c;
};

// FUNCTION: STARFLEETOP 0x006fa450
void tgPrefsPart::mActivate( void )
{
	fField0c = 0;
}
