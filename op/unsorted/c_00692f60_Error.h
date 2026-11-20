#ifndef C_00692F60_ERROR_H
#define C_00692F60_ERROR_H

#include "Tiger/tstring.h"

// HYPOTHESIS: the exception the asset classes throw (tSurface when its pixels cannot be
// allocated, tCastMgr when a cast file does not open); 0x00692f60 builds it from a message and a
// value, and it is thrown with the throw information at 0x008273b8. Its size is read from the
// constructor's frame.
class c_00692f60_Error
{
public:
	c_00692f60_Error( const tString &text, int a );
	virtual ~c_00692f60_Error();
	unsigned char data[ 0x28 ];
};

// HYPOTHESIS: the object at 0x00c690d0; 0x006740f0 returns the text of an error id.
class c_00c690d0_Strings
{
public:
	const tString &fn_006740f0( int id );
};

extern c_00c690d0_Strings *g_00c690d0;
// HYPOTHESIS: the text an empty message starts from; the original reaches it at 0x0083b344, not
// the pooled empty literal.
extern const char g_0083b344[];

// HYPOTHESIS: 0x006741e0 looks up an error id by its name.
int fn_006741e0( const char *name );

#endif
