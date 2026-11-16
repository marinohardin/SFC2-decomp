//	tstring.cpp: tString, Tiger's wide character string.

#include "Tiger/tstring.h"

// FUNCTION: STARFLEETOP 0x006230b0
void tString::mInitInstanceControl(void)
{
	fInstanceControl.fTempExternalCharRepresentation = NULL;
	fInstanceControl.fSizeOfExternalCharAllocation = 0;
	fInstanceControl.fTempOperatorSpace = NULL;
	fInstanceControl.fMemLimit = 0;
	fInstanceControl.fDataCount = 0;
}

// FUNCTION: STARFLEETOP 0x006230d0
tString::tString()
{
	// MATCH-ONLY: the original inlines mInitInstanceControl here (built with /Ob2); the
	// profile has no /Ob, so its body is written out.
	fInstanceControl.fTempExternalCharRepresentation = NULL;
	fInstanceControl.fSizeOfExternalCharAllocation = 0;
	fInstanceControl.fTempOperatorSpace = NULL;
	fInstanceControl.fMemLimit = 0;
	fInstanceControl.fDataCount = 0;
}
