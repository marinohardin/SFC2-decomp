#ifndef UNSORTED_FN_008CC620_OBJECT_H
#define UNSORTED_FN_008CC620_OBJECT_H

// HYPOTHESIS: the object held by the global at 0x008cc620; its third virtual loads a texture
// by file name and returns a handle. The symbol table has no name for its class.
class fn_008cc620_object_00500000
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual long vfunction2( const char *name, long a, long b, long c );
};

extern fn_008cc620_object_00500000 *g_008cc620;

#endif
