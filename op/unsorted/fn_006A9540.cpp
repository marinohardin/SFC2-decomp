#include "unsorted/tView3DExt.h"

// FUNCTION: STARFLEETOP 0x006a9540
void tView3DExt::mInitExt()
{
	mAssertVtxCache( 0x150 );
	field_0x554 = 0;
	field_0x558 = NULL;
	mSetWinding( 7 );
	mSetCullMode( 1 );
	mSetShadeMode( 5 );
	field_0x1ed0 = 1;
	mSetGlobalBlend( 1.0f );
	mEnableGlobalBlend( false );
	tPoint3 v( 1.0f, 0.0f, 10.0f );
	mSetLightPosition( v );
	field_0x1eb8 = true;
	field_0x1edc = NULL;
	field_0x1ee4 = NULL;
	field_0x1ee0 = NULL;
	field_0x1ee8 = NULL;
	field_0x1eec.mInit();
}

// FUNCTION: STARFLEETOP 0x006a95f0
unsigned char tView3DExt::mDone()
{
	if ( mVtxCache )
	{
		delete [] mVtxCache;
		mVtxCache = NULL;
		mVtxCacheSize = 0;
	}
	return true;
}
