#include "unsorted/tFighterSpecMgr.h"

// FUNCTION: STARFLEETOP 0x00401c9c
tFighterSpecMgr::tFighterSpecMgr()
    : m20(0), m24(0)
{
    fn_00401ff6();
}

// HYPOTHESIS: the original deletes the polymorphic object at +0x0c, then clears the container at +0x28
// (fn_00402d42_Vec::fn_00402d42) and the one at +0x10 (fn_00402cf9_Vec::fn_00402cf9). Those classes are
// declared only inside fn_00402000.cpp, so this body stays a placeholder until they have a header.
// MATCH-ONLY: the destructor (0x00401d13) is not claimed; only its scalar deleting destructor is exact.
// FUNCTION: STARFLEETOP 0x00401cf7
tFighterSpecMgr::~tFighterSpecMgr()
{
}
