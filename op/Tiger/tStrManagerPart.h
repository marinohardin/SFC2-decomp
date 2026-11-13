//	tStrManagerPart.h: tStrManagerPart, the string manager part, bound through the part list.
//	Its type descriptor and its binding's cache pointer at 0x00898b80 name it.

#ifndef tStrManagerPart_h
#define tStrManagerPart_h

#include "Tiger/tpart.h"

class tString;
class tBasicFile;

// HYPOTHESIS: only the virtual slots up to the ones the units here use. Member names are
// tStrMgrCore's, from the SDK library. The compiler lays overloads out in reverse order of
// declaration, so after tPart's six slots this puts mDone at +0x18, the file name load at +0x20
// (tSfcApp3d passes it a name and a flag), mGetStringIndex(const char*) at +0x28 and
// mGetString(int) at +0x34; the order of the first three is a guess.
class tStrManagerPart : public tPart {
public:
	virtual void mDone(void) = 0;
	virtual iTruth mLoadStringFile(const char* fileName, bool flag) = 0;
	virtual iTruth mLoadStringFile(tBasicFile& file, bool flag) = 0;
	virtual int mGetStringIndex(const char* str) = 0;
	virtual int mGetStringIndex(const tString& str) = 0;
	virtual const tString& mGetString(int index) = 0;
	virtual const tString& mGetString(const char* str) = 0;
	virtual const tString& mGetString(const tString& str) = 0;
	virtual long mGetNumElements(long index) = 0;
	virtual long mGetNumElements(const char* str) = 0;
	virtual const tString& mGetRandStr(long index) = 0;
	virtual const tString& mGetRandStr(const char* str) = 0;
	virtual const tString& mGetGrpStr(long group, long element) = 0;
	virtual const tString& mGetGrpStr(const char* group, int element) = 0;

	// FUNCTION: STARFLEETOP 0x0040b0d5
	GenAbstractBinding( tStrManagerPart );
};

#endif	// tStrManagerPart_h
