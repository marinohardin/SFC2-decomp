// Unit:      unsorted/tPhysicalEntity.cpp
// Evidence:  class (original file unknown)

#include "Source/entity/entity_physical.h"

// MATCH-ONLY: the harness finds a function by the "; File" line that the listing writes
// with the class's vftable; this emits it until the class's constructor is decompiled.
tPhysicalEntity *fn_probe_vftable()
{
	return new tPhysicalEntity;
}

// FUNCTION: STARFLEETOP 0x004105dd
tMatrix *tPhysicalEntity::vfunction28(bool aValue)
{
	return fn_00413b34(aValue);
}

// FUNCTION: STARFLEETOP 0x004105e9
int *tPhysicalEntity::vfunction40()
{
	return &field_0x11c;
}

// FUNCTION: STARFLEETOP 0x004105f0
char *tPhysicalEntity::vfunction42()
{
	return field_0x13c;
}

// FUNCTION: STARFLEETOP 0x004105f7
char *tPhysicalEntity::vfunction44()
{
	return field_0x15c;
}

// FUNCTION: STARFLEETOP 0x004105fe
void tPhysicalEntity::vfunction54(float aValue)
{
	field_0x17c = aValue;
}

// FUNCTION: STARFLEETOP 0x0041060b
float tPhysicalEntity::vfunction55()
{
	return field_0x17c;
}

// FUNCTION: STARFLEETOP 0x00410612
void tPhysicalEntity::vfunction56(float aX, float aY, float aZ)
{
	field_0x198.mSet(aX, aY, aZ);
}

// FUNCTION: STARFLEETOP 0x0041062f
tPoint3 tPhysicalEntity::vfunction57()
{
	return field_0x198;
}

// FUNCTION: STARFLEETOP 0x00410650
int tPhysicalEntity::vfunction59()
{
	return field_0x18c;
}

// FUNCTION: STARFLEETOP 0x00410657
void tPhysicalEntity::vfunction61()
{
	field_0x190 = true;
}

// FUNCTION: STARFLEETOP 0x0041065f
void tPhysicalEntity::vfunction62()
{
	field_0x190 = false;
}

// FUNCTION: STARFLEETOP 0x00410667
void tPhysicalEntity::vfunction64()
{
	vfunction63();
}
