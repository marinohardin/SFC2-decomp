//	tBitmapFontEx.h: tBitmapFontEx, the bitmap font a list box draws its text with.
// Evidence:  class (RTTI: type descriptor 0x0085af40, parent tBitmapFont 0x0085af20; vftable
//            0x007b06b4, 12 slots, constructor 0x00537c4a; original file unknown)

#ifndef tBitmapFontEx_h
#define tBitmapFontEx_h 1

// tBitmapFontEx: 0x24 bytes (the operator new in 0x005380e3 asks for 0x24; the constructor
// 0x00537c4a sets the vftable and clears +0x4 to +0x20). It is not a tObjBase: tListBox_Base keeps
// one at +0xb0 (made by 0x004be773 through 0x005380e3), and calls its slots 2, 4, 11 and 12, which
// are not tObjBase's slots 2, 4, 11 and 12. Its parent tBitmapFont has no vftable of its own.
// The slots are numbered as the other classes number theirs: vfunctionN is slot N-1.
class tBitmapFontEx
{
public:
	// 0x00537cf5: the glyph record (0x18 bytes each, from +0x8) for an index, or none past +0xc.
	virtual void *vfunction1( unsigned short index );
	// 0x0055f269: returns +0xc (the number of glyphs); a function the vftable of tLogMessage shares.
	virtual int vfunction2();
	// 0x00537d0f: the width of a run of text in this font, -1 for the whole string (a string of
	// no characters, 0); tListBox_Base::vfunction53 (0x004c6af8) asks it.
	virtual int vfunction3( unsigned short *text, int length );
	// 0x00537d66: copies a number of bytes from +0x4 to the buffer given.
	virtual void vfunction4( void *buffer, int size );
	// 0x005e8f21: returns +0x10 (the height of the font); tListBox_Base::vfunction43 (0x004c6787)
	// asks it.
	virtual int vfunction5();
	// 0x005e8f25: returns +0x14.
	virtual int vfunction6();
	// HYPOTHESIS: 0x00537d88 loads the font from a file named by the string (it opens it first).
	virtual bool vfunction7( const char *name );
	// HYPOTHESIS: 0x00537f66 reads the font's values (+0x10, +0x14, +0xc ...) from a file.
	virtual void vfunction8( const char *name );
	// 0x005c7fb7: returns +0x1c.
	virtual int vfunction9();
	// 0x0053805e: takes a reference (+0x20 up by one).
	virtual void vfunction10();
	// 0x00538062: gives up a reference, and frees the font at none left; tListBox_Base calls it
	// on the font it replaces.
	virtual void vfunction11();
	// 0x00538093: the same through slot 10; the list box's destructor calls it.
	virtual void vfunction12();

	// HYPOTHESIS: names and types are not known; the constructor clears them all.
	void *field_04;
	void *field_08;	// the glyph records
	int field_0C;	// the number of glyph records
	int field_10;	// the height
	int field_14;
	int field_18;
	int field_1C;
	int field_20;	// the reference count
};
typedef char tBitmapFontEx_size_check[ sizeof( tBitmapFontEx ) == 0x24 ? 1 : -1 ];

#endif	// tBitmapFontEx_h
