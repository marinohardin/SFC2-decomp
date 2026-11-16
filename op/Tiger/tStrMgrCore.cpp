// tStrMgrCore.cpp: tStrMgrCore, the string-table core behind tgStrManager.

#include "Tiger/tgStrMgr.h"

// FUNCTION: STARFLEETOP 0x00627180
void tStrMgrCore::mDone( void )
{
	fn_00627180_map1.clear();
	fn_00627180_map2.clear();
	for ( std::vector<tString *>::iterator it = fn_00627180_vec.begin(); it != fn_00627180_vec.end(); ++it )
	{
		delete *it;
	}
	fn_00627180_vec.clear();
	fn_00627180_count = 0;
}
