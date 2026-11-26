#include "unsorted/tShipSpecs.h"

// HYPOTHESIS: a container with a 12-byte header and 0x20-byte elements starting at +0xc, and an
// element count at +0x100c; its original class is not known.
class fn_0048179b_Object
{
public:
	char* fn_0048179b();
	char	fUnknown_00[0xc];
	char	fElements[0x1000];
	int		fCount;
};

// FUNCTION: STARFLEETOP 0x0048179b
char* fn_0048179b_Object::fn_0048179b()
{
	return (char*)this + 0xc + fCount * 0x20;
}

// HYPOTHESIS: the global list at 0x008949a0 (declared locally in other units too); 0x0040a6cd fills
// a buffer with up to 0x80 elements of 0x20 bytes for two ids and returns how many it stored.
class fn_00481a1c_List
{
public:
	int fn_0040a6cd( int a, int b, char *buffer, int maxCount, int elementSize );
	char	fPad_00[0x14];
};
extern fn_00481a1c_List g_008949a0;

// HYPOTHESIS: __stdcall (the caller leaves its two arguments to the callee); the rest of the body
// lies beyond the analysed boundary of 89 bytes and is not recovered.
// FUNCTION: STARFLEETOP 0x00481a1c
bool __stdcall fn_00481a1c( int a, int b )
{
	bool found = false;
	char *buffer = new char[0x1000];
	fn_00481a1c_List *list = &g_008949a0;
	int count = list->fn_0040a6cd( a, b, buffer, 0x80, 0x20 );
	int i = count - 1;
	while (count--)
	{
		char *element = buffer + (i << 5);
		i--;
		tShipSpecs specs;
		if (list->fn_0040a6cd( a, b, element, 0, 0 ))
			found = true;
	}
	delete [] buffer;
	return found;
}
