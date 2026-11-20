#include "Common/DataStore/Streamable.h"

namespace nDataStore
{

// MATCH-ONLY: the scalar deleting destructor of tStreamable (the destructor itself is 0x006f8ed0);
// the out-of-class destructor is defined in this unit so that the deleting form can be claimed on it.
// FUNCTION: STARFLEETOP 0x00427366
tStreamable::~tStreamable()
{
}

}
