//	tReconnector.h: tReconnector (RTTI, vtable 0x0079fa4c, 0x3c bytes), a tConnector.
// Evidence:  class (original file unknown)

#ifndef tReconnector_h
#define tReconnector_h 1

#include "unsorted/tConnector.h"

// HYPOTHESIS: the member at +0x2c of tReconnector, destroyed by 0x004832b3.
class fn_004832b3_object
{
public:
	~fn_004832b3_object();

	char m00[0x10];
};

class tReconnector : public tConnector
{
public:
	tReconnector();
	virtual ~tReconnector();
	virtual void fn_00671900( unsigned long a, long b );
	virtual void fn_006719e0( unsigned char a );

	fn_004832b3_object m2c;
};

// The original inlines this into its owners' destructors (0x0049640b); 0x0047f906 is the
// out-of-line copy.
inline tReconnector::~tReconnector()
{
}

#endif
