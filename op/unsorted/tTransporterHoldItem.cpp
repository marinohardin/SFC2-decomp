#include "unsorted/tTransporterHoldItem.h"
#include <vector>
#include <new>

namespace std {

// MATCH-ONLY: the library's _Construct held out of line for this element; the explicit
// specialization carries the template's body so the claim has a line of its own.
// FUNCTION: STARFLEETOP 0x004282c7
template<>
void _Construct( tTransporterHoldItem* _P, const tTransporterHoldItem& _V )
{
	new ((void*)_P) tTransporterHoldItem(_V);
}
}

// HYPOTHESIS: the argument's class is not known; it is a chainable object whose one-argument
// member (0x0050d6a0) takes a pointer to a dword and returns the object.
class fn_0050d6a0_Stream
{
public:
	fn_0050d6a0_Stream& fn_0050d6a0( int* value );
};

// FUNCTION: STARFLEETOP 0x004282f8
void tTransporterHoldItem::fn_004282f8( fn_0050d6a0_Stream* stream )
{
	stream->fn_0050d6a0( &m_0c ).fn_0050d6a0( &m_10 );
}
