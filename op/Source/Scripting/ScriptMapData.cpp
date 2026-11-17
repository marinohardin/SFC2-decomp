#include "Tiger/tgSound.h"
#include "Source/Scripting/ScriptMapData.h"

// The linker folded this body with tScriptMapData::mGetMapName (the SDK library has both at
// this address); the exe's symbol for it is tChannelAttributes's.
// FUNCTION: STARFLEETOP 0x006b32d0
t3DSoundProfile* tChannelAttributes::mGet3DSoundProfile()
{
    return &m3DSoundProfile;
}

// The linker folded this body with t3DListenerProfile::mGet3DPosition and
// t3DSoundProfile::mGet3DSoundPosition (the SDK library has all three at this address); the
// exe's t3DListenerProfile symbol is claimed by the game's own copy at 0x004355fe.
// FUNCTION: STARFLEETOP 0x006b32e0
const tMapItemList& tScriptMapData::mGetItemList() const
{
    return fMapItemList;
}
