// Common/Thread/ResourceGuard.cpp

#include "Common/Thread/ResourceGuard.h"

// FUNCTION: STARFLEETOP 0x0070efa0
tResourceGuard::tGuard::tGuard( tResourceGuard& resGuard )
	: fResGuard( resGuard )
{
	fResGuard.Guard();
}

// FUNCTION: STARFLEETOP 0x0070efc0
tResourceGuard::tGuard::~tGuard()
{
	fResGuard.Unguard();
}
