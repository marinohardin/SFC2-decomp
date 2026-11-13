//	tstringtag.h: tStringTag and tStringGroupTag, strings named by a tag and looked up in the
//	string manager part. Only the members a unit here uses are declared.

#ifndef tstringtag_h
#define tstringtag_h

class tString;

// HYPOTHESIS: layout from the constructor alone: the tag text at +0 and a word at +0x10 that
// starts at -1, the string's index once it has been looked up; field names are ours
class tStringTag {
public:
    tStringTag(char* param_1);
    const tString& mGetString();

private:
    char m00[0x10];
    long m10;
};

// HYPOTHESIS: 0x28 bytes, from the stack slot tFormatStr gives it; the group's text at +0 and,
// like tStringTag, a word at +0x10 that starts at -1; the rest is not touched here. Field names
// are ours
class tStringGroupTag {
public:
    tStringGroupTag(char* spec);
    long mGetNumElements();
    const tString& mGetString(long index);

private:
    char m00[0x10];
    long m10;
    char m14[0x14];
};

#endif	// tstringtag_h
