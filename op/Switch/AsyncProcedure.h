#if !defined( AsyncProcedure_h )
#define AsyncProcedure_h

#include "Common/Callback.h"
#include "Common/DataStore/DataStorePre.h"

namespace nSwitch
{

class tAddress;
class tSwitch;

// HYPOTHESIS: RTTI names the class; only the members that rebuilt functions use are declared.
// 0x00718f10 and 0x00719050 both assert !IsDead().
class tAsyncProcedure
{
public:
	virtual ~tAsyncProcedure();

protected:
	void fn_00718f10( int stateID, tCallback<const nDataStore::tBuffer*>* pCallback );	// HYPOTHESIS: registers the callback for a state; its sibling 0x00719200 asserts stateID < fStateInfos.size().
	void fn_00719050();
	// HYPOTHESIS: 0x005682b2 passes what fn_00719580 returns for 0xd to fn_00719490, and holds
	// the result in the smart pointer a request answers to; both types are guesses.
	tAddress* fn_00719490( int param_1 );
	int fn_00719580( int param_1 );

	// HYPOTHESIS: 0x00565464 sends its response through the switch at +0x1c and finds the
	// derived procedure's database at +0x44, so the class is 0x44 bytes; the rest is not known.
	char fPad04[0x1c - 0x04];
	tSwitch* m_1c;
	char fPad20[0x44 - 0x20];
};

}

#endif
