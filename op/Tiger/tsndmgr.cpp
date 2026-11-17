//	tsndmgr.cpp: the sound file manager and its play information.

#include "Tiger/tbasic.h"
#include "Tiger/tsndmgr.h"

// HYPOTHESIS: declared only as far as the functions below need: the field mSetNextInSequence
// stores is at 0xbc; the other fields and virtuals are not recovered.
class tAFPlayInfo
{
public:
	virtual	void	mSetNextInSequence	( long next );

	char			fUnknown_0004[0xb8];
	long			field_0xbc;
};

// FUNCTION: STARFLEETOP 0x006639c0
void	tAFPlayInfo::mSetNextInSequence( long next )
{
	field_0xbc = next;
}

// FUNCTION: STARFLEETOP 0x00664870
unsigned long	tBasicSoundFileManager::mGetTrackingIndex( unsigned long handle )
{
	return handle & 0xffff;
}
