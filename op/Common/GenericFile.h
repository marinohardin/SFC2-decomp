//	GenericFile.h: the generic file's items and sections.
//	Only what matched code uses is declared; the names are the SDK library's symbols
//	(??1tGFItem@@UAE@XZ at 0x006fffd0, ??0tGFSection@@QAE@XZ at 0x007007d0).

#if !defined( GenericFile_h )
#define GenericFile_h

#pragma once

class tGFItem
{
public:
	virtual ~tGFItem();
};

// HYPOTHESIS: a section is an item; its destructor is the first virtual, as tGFItem's is.
class tGFSection : public tGFItem
{
public:
	tGFSection();
};

#endif
