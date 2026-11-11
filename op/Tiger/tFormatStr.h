//	tFormatStr.h: tFormatStr, a format string whose tags are replaced by values.
// Evidence:  the SDK libraries' symbols (the SDK ships no header for it)

#ifndef tFormatStr_h
#define tFormatStr_h 1

class tString;

// HYPOTHESIS: 8 bytes, a vftable and one word; only what the callers use is declared.
class tFormatStr
{
public:
	tFormatStr();
	virtual ~tFormatStr();
	void mTag( char *aTag, tString &aValue );

protected:
	void *field_4;	// HYPOTHESIS: type and name unknown
};

#endif	// tFormatStr_h
