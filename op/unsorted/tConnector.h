//	tConnector.h: tConnector (RTTI, vtable 0x0079fa5c); tReconnector's first member follows
//	at +0x2c.
// Evidence:  class (original file unknown)

#ifndef tConnector_h
#define tConnector_h 1

class tConnector
{
public:
	tConnector();
	virtual ~tConnector();
	virtual void fn_00671820( long a, int b );
	virtual void fn_00401038();

	char m04[0x2c - 0x04];
};

#endif
