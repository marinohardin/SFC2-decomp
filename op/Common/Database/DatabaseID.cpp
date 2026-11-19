#include "Common/Database/DatabaseID.h"

namespace nDatabase
{

// FUNCTION: STARFLEETOP 0x006fa440
bool tID::IsValid() const
{
	return fObjectID != 0 ? true : false;
}

}
