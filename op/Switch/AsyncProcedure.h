#if !defined( AsyncProcedure_h )
#define AsyncProcedure_h

#include "Common/Callback.h"
#include "Common/DataStore/DataStorePre.h"

namespace nSwitch
{

// HYPOTHESIS: RTTI names the class; only the members that rebuilt functions use are declared.
// 0x00718f10 and 0x00719050 both assert !IsDead().
class tAsyncProcedure
{
public:
	virtual ~tAsyncProcedure();

protected:
	void fn_00718f10( int stateID, tCallback<const nDataStore::tBuffer*>* pCallback );	// HYPOTHESIS: registers the callback for a state; its sibling 0x00719200 asserts stateID < fStateInfos.size().
	void fn_00719050();
};

}

#endif
