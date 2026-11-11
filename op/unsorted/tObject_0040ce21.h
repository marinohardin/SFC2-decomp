//	tObject_0040ce21.h: the one-byte marker object built by the constructor at 0x0040ce21.
// Evidence:  calls (original name and file unknown)

#ifndef tObject_0040ce21_h
#define tObject_0040ce21_h 1

// HYPOTHESIS: the class of the one-time object 0x00413ac3 builds from a message; its
// constructor (0x0040ce21) refers to "QAReport.txt". The destructor does nothing.
class tObject_0040ce21
{
public:
	tObject_0040ce21(const char *aText);
	~tObject_0040ce21() {}
};

#endif // tObject_0040ce21_h
