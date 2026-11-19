#include "Common/Exceptions/Validate.h"
#include <map>
#include <string>

// HYPOTHESIS: the owning class and its trade-in rate map are reconstructed from this one
// function's accesses only (the map's head node sits at +0x40 of the object). The key is taken
// by reference and the rate sits at +0x20 of the tree node, which fits a 16-byte std::string key.
class tTradeInRates
{
public:
	double fGetRate( const std::string& shipClass );

	char fPad[0x3c];
	std::map< std::string, double > fTradeInRates;
};

// FUNCTION: STARFLEETOP 0x0055a13a
double tTradeInRates::fGetRate( const std::string& shipClass )
{
	std::map< std::string, double >::iterator rate = fTradeInRates.find( shipClass );
#line 159
	validate( rate != fTradeInRates.end() );
	return rate->second;
}
