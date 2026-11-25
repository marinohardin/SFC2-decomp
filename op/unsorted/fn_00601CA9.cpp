#include "Switch/AsyncProcedure.h"
#include "Common/DataStore/DataStoreInput.h"
#include "unsorted/nSwitch__tResponse.h"

// HYPOTHESIS: the player list panel is declared in fn_00601983.cpp (no header yet); this unit
// declares only the member it calls, in a scope of its own, so as not to declare the class twice.
namespace nPlayerListPanelLocal
{
	class tPanel
	{
	public:
		void fn_00603aac( int32 param_1 );
	};
};

// HYPOTHESIS: nested member of tMetaClientPlayerListPanel (destructor 0x00603a5f); no header yet.
class tAddItem
{
public:
	~tAddItem();

	int8u	field_00[0x10];
};

// HYPOTHESIS: the response of the get client character request (constructor 0x00553c24); no
// header yet. Base tResponse (0x10 bytes) followed by a tAddItem at +0x10.
class tGetClientCharacterResponse : public nSwitch::tResponse
{
public:
	tGetClientCharacterResponse();

	virtual	void StreamOut( nDataStore::tOutput& output ) const;
	virtual	void StreamIn( nDataStore::tInput& input );

	int32 fn_0058c2e1();

	tAddItem	field_10;
};

class tCharacterInfoProcedure : public nSwitch::tAsyncProcedure
{
public:
	// HYPOTHESIS: the callback for a client character message.
	void fn_00601ca9( const nDataStore::tBuffer* buffer );

protected:
	nPlayerListPanelLocal::tPanel* field_44;
};

// FUNCTION: STARFLEETOP 0x00601ca9
void tCharacterInfoProcedure::fn_00601ca9( const nDataStore::tBuffer* buffer )
{
	nDataStore::tInput input( buffer );
	tGetClientCharacterResponse response;
	input >> response;

	fn_00719050();

	field_44->fn_00603aac( response.fn_0058c2e1() );
}
