#include "Common/Exceptions/Validate.h"
#include <map>

// HYPOTHESIS: the owning class and its trade-in rate map are reconstructed from this one
// function's accesses only (the map's head node sits at +0x40 of the object).
class tTradeInRates
{
public:
	double fGetRate( int shipClass );

	char fPad[0x3c];
	std::map< int, double > fTradeInRates;
};

// FUNCTION: STARFLEETOP 0x0055a13a
double tTradeInRates::fGetRate( int shipClass )
{
	std::map< int, double >::iterator rate = fTradeInRates.find( shipClass );
#line 159
	validate( rate != fTradeInRates.end() );
	return rate->second;
}
