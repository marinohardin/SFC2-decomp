#include <sstream>
#include <string>

// HYPOTHESIS: the object 0x00607d61 returns is the class the symbol table calls fn_0060a54d_class,
// which other units already declare locally; no shared header exists yet, so this unit names it
// after its one method.
class fn_0060a4b8_class
{
public:
	void fn_0060a4b8( const std::string& str, int param_2 );
};

fn_0060a4b8_class* fn_00607d61();
std::ostream& __cdecl fn_00404e8a( std::ostream& os, const std::string& str );

// FUNCTION: STARFLEETOP 0x00601498
void __stdcall fn_00601498( const std::string& param_1, int param_2 )
{
	std::stringstream stream;
	fn_00404e8a( stream << "*** ", param_1 ) << " ***";
	fn_00607d61()->fn_0060a4b8( stream.str(), param_2 );
}

// HYPOTHESIS: the object fn_00600ab8 returns is the class that owns fn_00600b4a; its name is unknown.
class fn_00600b4a_class
{
public:
	void fn_00600b4a();
};

fn_00600b4a_class* __stdcall fn_00600ab8( int param_1, int param_2, int param_3 );

// FUNCTION: STARFLEETOP 0x00601540
void __stdcall fn_00601540( int param_1, int param_2 )
{
	fn_00600ab8( param_1, 5, 1 )->fn_00600b4a();
}

// HYPOTHESIS: fn_005a616c is a member of an empty class, called on a temporary; its name is unknown.
class fn_005a616c_class
{
public:
	void fn_005a616c( std::string str, int a, int b, int c, int d );
};

// FUNCTION: STARFLEETOP 0x00601557
void __stdcall fn_00601557( const std::string& param_1, int param_2 )
{
	std::stringstream stream;
	fn_00404e8a( stream << "*** ", param_1 ) << " ***";
	fn_005a616c_class().fn_005a616c( stream.str(), 0, 3, 3, 0 );
}

// HYPOTHESIS: a second global std::string built from the same literal; its dynamic initializer is this function.
// FUNCTION: STARFLEETOP 0x0060161d
std::string g_00c39ff0 = "MessengerRelayNameC";
