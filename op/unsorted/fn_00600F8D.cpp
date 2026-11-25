#include "unsorted/fn_0042a480_object.h"

// HYPOTHESIS: the class is tMedalsPanel (neighbouring functions 0x00600e70..0x00600f61); no
// header declares it yet, so this is a local declaration of only what this function uses.
class tMedalsPanel
{
public:
	void FUN_00600f90( fn_0042a480_object* character );
	void FUN_00600fba( int index );
};

// FUNCTION: STARFLEETOP 0x00600f90
void tMedalsPanel::FUN_00600f90( fn_0042a480_object* character )
{
	for ( int i = 0; i < 13; i++ )
	{
		if ( character->FUN_0042a403( i ) )
		{
			FUN_00600fba( i + 1 );
		}
	}
}
