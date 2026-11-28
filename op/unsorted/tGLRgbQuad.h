//	tGLRgbQuad.h: tGLRgbQuad, the 4-byte colour the 3D view and drawing context pass by value
//	(name from the SDK's mangled symbols, as ?mSetClearColor@tSWView3D@@UAEEUtGLRgbQuad@@@Z).
#ifndef tGLRgbQuad_h
#define tGLRgbQuad_h 1

// HYPOTHESIS: the byte order of a Windows RGBQUAD (blue, green, red, then a fourth byte):
// tSWView3D's mSetClearColor( tGLRgbQuad ) passes bytes 2, 1 and 0 as red, green and blue.
// A user-declared constructor is also a hypothesis: tGLDC's colour setters return one through
// a hidden pointer, which VC6 does only for a class that is not plain data.
struct tGLRgbQuad
{
	tGLRgbQuad() {}

	unsigned char	field_0x00;    // blue
	unsigned char	field_0x01;    // green
	unsigned char	field_0x02;    // red
	unsigned char	field_0x03;
};

#endif // tGLRgbQuad_h
