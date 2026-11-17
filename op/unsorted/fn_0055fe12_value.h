#ifndef FN_0055FE12_VALUE_H
#define FN_0055FE12_VALUE_H

#include <string>

// HYPOTHESIS: the queued value: one string, copied by a copy constructor the original calls
// out of line (0x0055fe12); its destructor inlines to _Tidy (0x005da20a).
struct fn_0055fe12_value
{
	fn_0055fe12_value( const fn_0055fe12_value& other );
	fn_0055fe12_value( const std::string& value, int a );
	~fn_0055fe12_value();

	std::string field_00;
	int field_10;
};

#endif // FN_0055FE12_VALUE_H
