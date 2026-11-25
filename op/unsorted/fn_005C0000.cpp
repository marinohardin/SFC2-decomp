#include <sstream>
#include <string>
#include <stdlib.h>

// HYPOTHESIS: builds a message in a stringstream and returns it as a string; the tIdiom and
// tLogMessage parts are left out because neither class has a header yet.
// FUNCTION: STARFLEETOP 0x005c057f
std::string fn_005c057f(std::string text, char c)
{
	std::stringstream stream;
	stream << text << c;
	return stream.str();
}

// HYPOTHESIS: same shape as fn_005c057f, with the tIdiom parts left out (no header for tIdiom yet);
// the literals are guesses.
// FUNCTION: STARFLEETOP 0x005c0725
std::string fn_005c0725(std::string text, char c)
{
	std::stringstream stream;
	stream << text << c << text;
	return stream.str();
}

// HYPOTHESIS: same family as fn_005c057f; the original also builds tIdiom temporaries and calls
// tLogMessage and tHexPoint members, left out because tIdiom and tLogMessage have no header yet.
// FUNCTION: STARFLEETOP 0x005c087e
std::string fn_005c087e(std::string text, char c)
{
	std::stringstream stream;
	stream << text << c;
	stream << text << c << text;
	return stream.str();
}

void fn_004bd431();

// HYPOTHESIS: arranges at exit the cleanup that Ghidra labels tApp3dBase::vfunction5, unconfirmed.
// FUNCTION: STARFLEETOP 0x005c0b85
void fn_005c0b85()
{
	atexit(fn_004bd431);
}

// FUNCTION: STARFLEETOP 0x005c0b91
void fn_005c0b91()
{
	atexit(fn_004bd431);
}