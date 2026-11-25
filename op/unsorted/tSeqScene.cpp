// tSeqScene.cpp: the class declaration only; the functions are written one at a time.

#include "unsorted/tSeqScene.h"

// FUNCTION: STARFLEETOP 0x0067dcb0
void tSeqScene::vfunction43()
{
	field_F0 = 0;
	field_EC &= ~1;
	field_D8 = field_D4;
	if (field_D4 && field_E0)
		vfunction41(field_D4);
}
