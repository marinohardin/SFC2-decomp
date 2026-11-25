#include "Switch/Switch.h"
#include "Common/Exceptions/Validate.h"
#include "Common/Database/DatabaseID.h"
#include "unsorted/nStoredProcedureArguments__tGetClientCharacterRequest.h"

// HYPOTHESIS: the object 0x00607d61 returns; other units declare it locally under other names
// and no shared header exists yet. Only the getter this function validates is declared.
class fn_0060a59e_class
{
public:
	nSwitch::tAddress* fn_0060a59e();
};

fn_0060a59e_class* fn_00607d61();

// HYPOTHESIS: the message the panel handles; its class is not recovered. A type word at +0 and
// a tID source at +8.
class fn_00602e18_message
{
public:
	int32 field_00;
	int32 field_04;
	nDatabase::tID field_08;
};

// HYPOTHESIS: the player list panel (tMetaClientPlayerListPanel); no header yet, so only the
// members this function reads are declared, in a scope of its own.
class tPlayerListPanelSketch
{
public:
	void fn_00602e18( fn_00602e18_message* message );

protected:
	int8u field_00[0xac];
	tSmartPtr<nSwitch::tAddress> field_ac;
	int8u field_b0[0x1c];
	nSwitch::tSwitch* field_cc;
};

using namespace nStoredProcedureArguments;

// FUNCTION: STARFLEETOP 0x00602e18
void tPlayerListPanelSketch::fn_00602e18( fn_00602e18_message* message )
{
	if ( message )
	{
#line 361
		validate( fn_00607d61()->fn_0060a59e() );
		if ( message->field_00 == 2 )
		{
			tGetClientCharacterRequest request( field_ac, message->field_08, 4 );
			field_cc->fn_00714020( fn_00607d61()->fn_0060a59e(), 8, &request );
		}
	}
}
