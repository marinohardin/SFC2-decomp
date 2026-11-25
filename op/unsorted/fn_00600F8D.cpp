#include "unsorted/fn_0042a480_object.h"
#include "unsorted/tValueBtn.h"

// HYPOTHESIS: the class is tMedalsPanel (neighbouring functions 0x00600e70..0x00600f61); no
// header declares it yet, so this is a local declaration of only what this function uses.
class tMedalsPanel
{
public:
	void FUN_00600f90( fn_0042a480_object* character );
	void FUN_00600fba( int index );
	// MATCH-ONLY: 0x004a0384 is a member of the object at tValueBtn +0x110, not of tMedalsPanel;
	// declared here only until a header declares it.
	void fn_004a0384();
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

// HYPOTHESIS: the string 0x00601054 builds for medal number index (a lookup name); a free
// function that pops its own arguments.
std::string __stdcall fn_00601054( int index );
extern std::string g_00c39f28; // HYPOTHESIS: the control group name, defined in fn_00600BE5.cpp
void fn_004d353e( long arg1, long arg2 );

// FUNCTION: STARFLEETOP 0x00600fba
void tMedalsPanel::FUN_00600fba( int index )
{
	// MATCH-ONLY: the claim at 0x004d353e is declared void; its result is reached through a cast.
	tValueBtn* button = dynamic_cast<tValueBtn*>( ( (tVisObject* (*)( long, long )) fn_004d353e )( (long) g_00c39f28.c_str(), (long) fn_00601054( index ).c_str() ) );
	button->vfunction21( 1 );
	( (tMedalsPanel*) ( (char*) button + 0x110 ) )->fn_004a0384();
	button->vfunction13();
	button->vfunction15( true );
}
