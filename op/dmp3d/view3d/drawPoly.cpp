#include <stddef.h>

#include "dmp3d/common/stream_basicfile.h"
#include "unsorted/tView3DExt.h"
// The blank lines keep the throw on line 42, the line number the exe passes.























// FUNCTION: STARFLEETOP 0x006abe00
void tView3DExt::mAssertVtxCache( int count )
{
	if ( mVtxCache == NULL || mVtxCacheSize < count )
	{
		if ( mVtxCache != NULL )
			delete[] mVtxCache;

		if ( count <= 0x150 )
			count = 0x150;

		mVtxCache = new fn_006abe00_Vtx[ count + 16 ];
		if ( mVtxCache == NULL )
			throw fn_006a2ec0( __FILE__, __LINE__, "tView3DExt::mAssertVtxCache(): Unable to allocate vertex cache" );

		mVtxCacheSize = count;
		mVtxCacheEnd = mVtxCache + count;
	}
}
