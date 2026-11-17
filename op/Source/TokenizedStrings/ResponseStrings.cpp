#include "Common/Exceptions/Validate.h"
#include <map>
#include <string>

// HYPOTHESIS: the key enumeration's other names are unknown; the validate text names its end
// marker, and the compare (3 < key) gives its value.
enum
{
	kOnePastLastKey = 4
};

// HYPOTHESIS: the class owning 0x0060b6d0 and 0x0060b7f4 is unnamed in the exe; both are
// thiscall members that share this. 0x0060b7f4 returns the text stored for a key (it is
// assigned through std::string::assign at 0x0040428e).
class c_0060b6d0
{
public:
	void fn_0060b6d0( int key );
	std::string& fn_0060b7f4( int* key );
	void fn_0060b715();

	char field_0[4];
	// HYPOTHESIS: the key and value types; the validate text names the member, and its
	// size() is read at +0x10.
	std::map< int, std::string > fStringMap;
};

// HYPOTHESIS: the English response texts are an array of pointers starting at 0x0087bd6c.
extern const char* g_0087bd6c[];

// FUNCTION: STARFLEETOP 0x0060b6d0
void c_0060b6d0::fn_0060b6d0( int key )
{
#line 89
	validate( key < kOnePastLastKey );
#line 37
	fn_0060b7f4( &key );
}

// FUNCTION: STARFLEETOP 0x0060b715
void c_0060b6d0::fn_0060b715()
{
	int key;
	key = 0;
	fn_0060b7f4( &key ) = g_0087bd6c[0];
	key = 1;
	fn_0060b7f4( &key ) = g_0087bd6c[1];
	key = 2;
	fn_0060b7f4( &key ) = g_0087bd6c[2];
	key = 3;
	fn_0060b7f4( &key ) = g_0087bd6c[3];
#line 113
	validate( fStringMap.size() == kOnePastLastKey );
#line 55
}
