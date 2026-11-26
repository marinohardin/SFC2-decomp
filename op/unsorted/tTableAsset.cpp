#include "unsorted/tAsset.h"

// FUNCTION: STARFLEETOP 0x006896d0
tTableAsset::tTableAsset()
{
	// HYPOTHESIS: the byte at +0x6c is stored from an uninitialized local, as the original did.
	char c;
	short five = 5;
	*((char*)this + 0x6c) = c;
	*(int*)((char*)this + 0x70) = 0;
	*(int*)((char*)this + 0x74) = 0;
	*(int*)((char*)this + 0x78) = 0;
	*(short*)((char*)this + 0x5e) = 0;
	*(short*)((char*)this + 0x5c) = 0;
	*(short*)((char*)this + 0x62) = 0;
	*(short*)((char*)this + 0x60) = 0;
	*(int*)((char*)this + 0x68) = 0;
	*(short*)((char*)this + 0x66) = five;
	*(short*)((char*)this + 0x64) = five;
}
