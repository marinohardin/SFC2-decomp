//	tFormatStr.h: tFormatStr, a format string whose tags are replaced by values.
// Evidence:  the SDK libraries' symbols (the SDK ships no header for it)

#ifndef tFormatStr_h
#define tFormatStr_h 1

class tString;
class tTagBag;
class tFormatToken;

// HYPOTHESIS: 0x10 bytes, a vftable and three words, from the destructor (0x0062c800), which
// deletes all three; only what the callers use is declared. Field names are ours.
class tFormatStr
{
public:
	tFormatStr();
	virtual ~tFormatStr();
	void mTag( char *aTag, tString &aValue );
	void mTag( char *aTag, long aValue );
	void mTag( char *aTag, char *aValue );
	void mGenFormattedStr( tString &aResult, tString aFormat );

private:
	signed char	mGenerateFormatTokenList( tFormatToken *aTokens );
	void		mAssembleFormattedString( signed char aCount, tFormatToken *aTokens );

protected:
	tTagBag	*m04;	// HYPOTHESIS: the tags, by name
	tString	*m08;	// HYPOTHESIS: the last formatted string
	tString	*m0C;	// HYPOTHESIS: the format string being parsed
};

#endif	// tFormatStr_h
