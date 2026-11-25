#include <sstream>
#include <string>

// HYPOTHESIS: builds a message in a stringstream and returns it as a string; the tIdiom and
// tLogMessage parts are left out because neither class has a header yet.
// FUNCTION: STARFLEETOP 0x005c057f
std::string fn_005c057f(std::string text, char c)
{
	std::stringstream stream;
	stream << text << c;
	return stream.str();
}
