//	twmemory.cpp: the Windows memory part.

#include "Tiger/tbasic.h"

// HYPOTHESIS: declared only as far as the function below needs; its base, fields and other
// virtuals are not recovered.
class twMemoryPart
{
public:
	virtual	iTruth	mCheckMemory	( void* a, void* b );
};

// FUNCTION: STARFLEETOP 0x0063a900
iTruth	twMemoryPart::mCheckMemory( void*, void* )
{
	return kTrue;
}
