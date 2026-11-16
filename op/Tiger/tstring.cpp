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
