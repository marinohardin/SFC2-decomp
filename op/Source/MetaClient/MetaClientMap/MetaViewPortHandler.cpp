#include "Common/Thread/Optex.h"
#include "Common/Exceptions/Validate.h"
#include "unsorted/fn_004D3143.h"
#include "unsorted/tVisObject.h"
#include "unsorted/tMetaMapControl.h"

// MATCH-ONLY: inline in the SDK, as in unsorted/tClientCharacter.cpp, which claims its
// out-of-line copy (0x00429a18); the header cannot define it while that unit does.
inline tOptex::tGuard::~tGuard()
{
	if( fNeedToLeave )
	{
		fOptex->Leave();
	}
}

// HYPOTHESIS: the claim at 0x004d353e is declared void, but this caller casts its result; it
// returns what 0x004d4728 finds for a group and a control name.
void fn_004d353e( long arg1, long arg2 );

// HYPOTHESIS: the name of the map panel's control group, a global at 0x0087632c.
extern const char* g_0087632c;

// HYPOTHESIS: only the members this unit reads are placed; the rest of the layout is not
// recovered. fpMetaMapControl is named by the validate text at 0x00876438.
class tMetaClientMapPanel
{
public:
	virtual void vfunction19();
	void fn_005e6091();

	unsigned char field_4[ 0xdc - 0x4 ];
	tOptex field_dc;
	unsigned char field_f0[ 0xf8 - 0xdc - sizeof( tOptex ) ];
	tMetaMapControl* fpMetaMapControl;
	unsigned char field_fc[ 0x1a0 - 0xfc ];
	int field_1a0;
};

// FUNCTION: STARFLEETOP 0x005e5800
void tMetaClientMapPanel::vfunction19()
{
	tOptex::tGuard guard( &field_dc );

	// MATCH-ONLY: the claim at 0x004d353e is declared void; its result is reached through a cast.
	fpMetaMapControl = dynamic_cast<tMetaMapControl*>( ( (tVisObject* (*)( long, long )) fn_004d353e )( (long) g_0087632c, (long) "FakeMap" ) );
	if( fpMetaMapControl == NULL )
	{
		fn_005e6091();
		fpMetaMapControl = dynamic_cast<tMetaMapControl*>( ( (tVisObject* (*)( long, long )) fn_004d353e )( (long) g_0087632c, (long) "FakeMap" ) );
#line 316
		validate( fpMetaMapControl );
#line 54
	}
	fpMetaMapControl->fn_005e3148( field_1a0 );
	fpMetaMapControl->fn_005e3ad8();
	fn_004d3331( g_0087632c );
	guard.Leave();
}
