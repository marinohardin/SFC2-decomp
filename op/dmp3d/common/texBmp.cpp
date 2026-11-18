#include <windows.h>

#include "stream_basicfile.h"
#include "texBmp.h"
#include "Tiger/tBag.h"

// HYPOTHESIS: a byte flag set when the host is big-endian; every header field read from the
// file is byte-swapped while it is set.
extern bool g_007b9274;

// HYPOTHESIS: the swap macros are written as the exe computes them.
#define SWAP16( w )	( (WORD)( ( (WORD)(w) >> 8 ) | ( (WORD)(w) << 8 ) ) )
#define SWAP32( l )	( ( ( (unsigned long)(l) >> 24 ) ) | \
					  ( ( (unsigned long)(l) >> 8 ) & 0xff00 ) | \
					  ( ( (unsigned long)(l) << 8 ) & 0xff0000 ) | \
					  ( ( (unsigned long)(l) << 24 ) ) )

// FUNCTION: STARFLEETOP 0x006a48d0
// HYPOTHESIS: as in stream_basicfile.cpp, the literals stand in for __FILE__ and __LINE__.
void tTexBmp::vfunction2( tBasicFile *file )
{
	BITMAPFILEHEADER	fileHeader;
	BITMAPINFOHEADER	infoHeader;
	BITMAPCOREHEADER	coreHeader;
	bool				isCore;
	long				numColors;

	if ( file->mRead( &fileHeader, sizeof( fileHeader ) ) != sizeof( fileHeader ) )
	{
		throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 198,
			"Unable to read BITMAPFILEHEADER section" );
	}
	if ( ( (char *)&fileHeader.bfType )[0] != 'B' || ( (char *)&fileHeader.bfType )[1] != 'M' )
	{
		throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 207,
			"Invalid bitmap file (no 'BM' signature)" );
	}
	if ( file->mRead( &infoHeader, sizeof( infoHeader ) ) != sizeof( infoHeader ) )
	{
		throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 217,
			"Unable to read BITMAPINFOHEADER section" );
	}

	isCore = false;
	if ( g_007b9274 )
		infoHeader.biSize = SWAP32( infoHeader.biSize );

	if ( infoHeader.biSize == sizeof( BITMAPCOREHEADER ) )
	{
		isCore = true;
		coreHeader = *(BITMAPCOREHEADER *)&infoHeader;
		if ( g_007b9274 )
		{
			infoHeader.biWidth = SWAP16( coreHeader.bcWidth );
			infoHeader.biHeight = SWAP16( coreHeader.bcHeight );
			infoHeader.biPlanes = SWAP16( coreHeader.bcPlanes );
			infoHeader.biBitCount = SWAP16( coreHeader.bcBitCount );
		}
		else
		{
			infoHeader.biWidth = coreHeader.bcWidth;
			infoHeader.biHeight = coreHeader.bcHeight;
			infoHeader.biPlanes = coreHeader.bcPlanes;
			infoHeader.biBitCount = coreHeader.bcBitCount;
		}
		infoHeader.biSize = sizeof( BITMAPINFOHEADER );
		infoHeader.biCompression = 0;
		infoHeader.biSizeImage = 0;
		infoHeader.biXPelsPerMeter = 0;
		infoHeader.biYPelsPerMeter = 0;
		infoHeader.biClrUsed = 0;
		infoHeader.biClrImportant = 0;
		if ( !file->mSeekCur( (long)sizeof( BITMAPCOREHEADER ) - (long)sizeof( BITMAPINFOHEADER ) ) )
		{
			throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 267,
				"Error seeking file" );
		}
	}
	else if ( g_007b9274 )
	{
		infoHeader.biWidth = SWAP32( infoHeader.biWidth );
		infoHeader.biHeight = SWAP32( infoHeader.biHeight );
		infoHeader.biPlanes = SWAP16( infoHeader.biPlanes );
		infoHeader.biBitCount = SWAP16( infoHeader.biBitCount );
		infoHeader.biCompression = SWAP32( infoHeader.biCompression );
		infoHeader.biSizeImage = SWAP32( infoHeader.biSizeImage );
		infoHeader.biXPelsPerMeter = SWAP32( infoHeader.biXPelsPerMeter );
		infoHeader.biYPelsPerMeter = SWAP32( infoHeader.biYPelsPerMeter );
		infoHeader.biClrUsed = SWAP32( infoHeader.biClrUsed );
		infoHeader.biClrImportant = SWAP32( infoHeader.biClrImportant );
	}

	if ( infoHeader.biClrUsed == 0 && infoHeader.biBitCount <= 8 )
		numColors = 1 << infoHeader.biBitCount;
	else
		numColors = infoHeader.biClrUsed;

	field_10c = infoHeader.biBitCount;
	field_110 = infoHeader.biWidth;
	field_114 = infoHeader.biHeight < 0 ? -infoHeader.biHeight : infoHeader.biHeight;
	field_12c = 0;
	field_118 = 0;
	field_11c = 0;
	field_120 = ( ( field_10c * field_110 + 31 ) & ~31 ) / 8;
	field_124 = infoHeader.biHeight >= 0;
	field_128 = field_120 * field_114;
	field_13c = 0;
	field_04 = isCore ? 2 : 1;
	field_140 = 0;

	long pos = file->mFilePos();
	if ( numColors != 0 && infoHeader.biBitCount <= 8 )
	{
		field_138 = numColors;
		field_134 = pos;
	}
	else
	{
		field_138 = 0;
		field_134 = 0;
	}

	if ( fileHeader.bfOffBits != 0 )
	{
		field_130 = g_007b9274 ? SWAP32( fileHeader.bfOffBits ) : fileHeader.bfOffBits;
	}
	else if ( numColors != 0 )
		field_130 = pos + ( field_04 == 2 ? numColors * 3 : numColors * 4 );
	else
		field_130 = pos;
}

// HYPOTHESIS: the PCX file header as the exe lays it out on the stack; field names follow the
// published PCX format.
struct PcxHeader
{
	unsigned char	manufacturer;
	unsigned char	version;
	unsigned char	encoding;
	unsigned char	bitsPerPixel;
	short			xMin;
	short			yMin;
	short			xMax;
	short			yMax;
	short			hDpi;
	short			vDpi;
	unsigned char	colorMap[48];
	unsigned char	reserved;
	unsigned char	numPlanes;
	short			bytesPerLine;
	short			paletteInfo;
	unsigned char	filler[58];
};

// HYPOTHESIS: inline readers; every byte read reports the same source line, so they are
// functions, not macros. Their names are not known.
__forceinline unsigned char ReadByte( tBasicFile *file )
{
	unsigned char	value;

	if ( file->mRead( &value, 1 ) != 1 )
	{
		throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 369,
			"Unexpected read error" );
	}
	return value;
}

inline short ReadWord( tBasicFile *file )
{
	unsigned char	lo = ReadByte( file );
	unsigned char	hi = ReadByte( file );

	return (short)( ( hi << 8 ) | lo );
}

// FUNCTION: STARFLEETOP 0x006a4df0
void tTexBmp::vfunction3( tBasicFile *file )
{
	PcxHeader	header;
	int			width;
	int			height;
	int			bitCount;
	int			numPlanes;

	header.manufacturer = ReadByte( file );
	header.version = ReadByte( file );
	header.encoding = ReadByte( file );
	header.bitsPerPixel = ReadByte( file );
	header.xMin = ReadWord( file );
	header.yMin = ReadWord( file );
	header.xMax = ReadWord( file ) + 1;
	header.yMax = ReadWord( file ) + 1;
	header.hDpi = ReadWord( file );
	header.vDpi = ReadWord( file );
	if ( file->mRead( header.colorMap, sizeof( header.colorMap ) ) != sizeof( header.colorMap ) )
	{
		throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 405,
			"Unexpected read error" );
	}
	header.reserved = ReadByte( file );
	header.numPlanes = ReadByte( file );
	header.bytesPerLine = ReadWord( file );
	header.paletteInfo = ReadWord( file );
	if ( file->mRead( header.filler, sizeof( header.filler ) ) != sizeof( header.filler ) )
	{
		throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 415,
			"Unexpected seek error" );
	}

	if ( header.manufacturer != 10 )
	{
		throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 424,
			"Invalid or unsupported PCX file" );
	}
	if ( header.version != 5 )
	{
		throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 428,
			"Invalid or unsupported PCX file" );
	}
	if ( header.encoding != 1 )
	{
		throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 432,
			"Invalid or unsupported PCX file" );
	}
	if ( header.bitsPerPixel != 8 )
	{
		throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 436,
			"Invalid or unsupported PCX file" );
	}
	numPlanes = header.numPlanes;

	switch ( numPlanes )
	{
	case 1:
	case 3:
		break;
	default:
		throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 448,
			"Invalid or unsupported PCX file" );
	}

	width = header.xMax - header.xMin;
	height = header.yMax - header.yMin;

	if ( width <= 0 )
	{
		throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 457,
			"Invalid or unsupported PCX file" );
	}
	if ( header.yMax - header.yMin <= 0 )
	{
		throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 461,
			"Invalid or unsupported PCX file" );
	}

	bitCount = numPlanes * 8;
	field_118 = numPlanes;
	field_10c = bitCount;
	field_11c = header.bytesPerLine;
	field_04 = 3;
	field_110 = width;
	field_114 = height;
	field_120 = ( ( bitCount * field_110 + 31 ) & ~31 ) / 8;
	field_124 = 0;
	field_128 = field_120 * field_114;
	field_12c = 0;
	field_130 = file->mFilePos();
	field_134 = 0;
	field_138 = 0;

	if ( field_10c == 8 )
	{
		long	palettePos = file->mFileSize() - 769;

		if ( palettePos < 0 || !file->mSeekSet( palettePos ) )
		{
			throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 490,
				"Invalid or unsupported PCX file" );
		}
		if ( ReadByte( file ) == 12 )
		{
			field_134 = file->mFilePos();
			field_138 = 256;
		}
	}
}

// FUNCTION: STARFLEETOP 0x006a5a20
// The handler (after the function's ret in the exe) closes and releases the file, resets the
// object through slot 7 and fails.
bool tTexBmp::vfunction7( const char *name )
{
	tBasicFile	*file = vfunction1();

	try
	{
		vfunction8();
		if ( file == NULL )
		{
			throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 535,
				"tTexBmp::mInit(): Unable to create tBasicFile object" );
		}

		file->mOpenForRead( name );
		long	pos = file->mFilePos();
		char	signature[2];
		bool	isBmp;

		if ( file->mRead( signature, 2 ) == 2 )
			isBmp = signature[0] == 'B' && signature[1] == 'M';
		file->mSeekSet( pos );

		if ( isBmp )
			vfunction2( file );
		else
			vfunction3( file );

		field_08[0x103] = 0;
		for ( int i = 0; i < 0x103; i++ )
		{
			field_08[i] = name[i];
			if ( name[i] == 0 )
				break;
		}

		file->mClose();
		delete file;
	}
	catch ( fn_006a2ec0_Error error )
	{
		if ( file != NULL )
		{
			file->mClose();
			delete file;
		}
		vfunction8();
		return false;
	}
	return true;
}

// FUNCTION: STARFLEETOP 0x006a5b90
// HYPOTHESIS: fields 0x13c and 0x140 hold owned buffers; the reset frees them and clears the
// whole description.
bool tTexBmp::vfunction8( void )
{
	if ( field_13c != 0 )
		tBaseItemBag::operator delete( (void *)field_13c );
	if ( field_140 != 0 )
		tBaseItemBag::operator delete( (void *)field_140 );

	field_04 = 0;
	field_08[0] = 0;
	field_10c = 0;
	field_114 = 0;
	field_110 = 0;
	field_118 = 0;
	field_11c = 0;
	field_120 = 0;
	field_124 = 0;
	field_128 = 0;
	field_12c = 0;
	field_134 = 0;
	field_130 = 0;
	field_13c = 0;
	field_138 = 0;
	field_140 = 0;
	return true;
}

// FUNCTION: STARFLEETOP 0x006a5c20
// HYPOTHESIS: clears the description without releasing the buffers, in the same order as
// vfunction8.
void tTexBmp::fn_006a5c20( void )
{
	field_04 = 0;
	field_08[0] = 0;
	field_10c = 0;
	field_114 = 0;
	field_110 = 0;
	field_118 = 0;
	field_11c = 0;
	field_120 = 0;
	field_124 = 0;
	field_128 = 0;
	field_12c = 0;
	field_134 = 0;
	field_130 = 0;
	field_13c = 0;
	field_138 = 0;
	field_140 = 0;
}

// FUNCTION: STARFLEETOP 0x006a5c80
// HYPOTHESIS: reads the palette into field_140 as RGBQUADs, widening an OS/2 file's RGBTRIPLEs
// in place, then reads the pixels into field_13c.
void tTexBmp::vfunction4( tBasicFile *file )
{
	if ( field_138 != 0 )
	{
		if ( !file->mSeekSet( field_134 ) )
		{
			throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 643,
				"Error seeking file" );
		}
		field_140 = (long)tBaseItemBag::operator new( field_138 * 4 );
		if ( field_140 == 0 )
		{
			throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 651,
				"Out of memory" );
		}
		if ( field_04 == 2 )
		{
			unsigned long	size = field_138 * 3;

			if ( file->mRead( (void *)field_140, size ) != size )
			{
				throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 663,
					"Unable to read RGBTRIPLE array" );
			}
			RGBTRIPLE	*triples = (RGBTRIPLE *)field_140;
			RGBQUAD		color;
			color.rgbReserved = 0;
			for ( int i = field_138 - 1; i >= 0; i-- )
			{
				color.rgbRed = triples[i].rgbtRed;
				color.rgbBlue = triples[i].rgbtBlue;
				color.rgbGreen = triples[i].rgbtGreen;
				( (RGBQUAD *)field_140 )[i] = color;
			}
		}
		else
		{
			unsigned long	size = field_138 * 4;

			if ( file->mRead( (void *)field_140, size ) != size )
			{
				throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 684,
					"Unable to read RGBQUAD array" );
			}
			RGBQUAD	*quads = (RGBQUAD *)field_140;
			RGBQUAD	color;
			color.rgbReserved = 0;
			for ( int i = field_138 - 1; i >= 0; i-- )
			{
				color.rgbRed = quads[i].rgbRed;
				color.rgbBlue = quads[i].rgbBlue;
				color.rgbGreen = quads[i].rgbGreen;
				( (RGBQUAD *)field_140 )[i] = color;
			}
		}
	}

	if ( !file->mSeekSet( field_130 ) )
	{
		throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 706,
			"Error seeking file" );
	}
	if ( file->mRead( (void *)field_13c, field_128 ) != (unsigned long)field_128 )
	{
		throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 712,
			"Error reading BMP pixel bits" );
	}
}

// FUNCTION: STARFLEETOP 0x006a5f50
// HYPOTHESIS: decodes a PCX file's run-length pixels into field_13c, one row of field_120
// bytes at a time, interleaving the three planes of a 24-bit file, then reads its 256-entry
// RGB palette into field_140 as RGBQUADs.
void tTexBmp::vfunction5( tBasicFile *file )
{
	switch ( field_118 )
	{
	case 1:
	case 3:
		break;
	default:
		// The exe's message here is its "Internal error" constant (0x00884ae4), the one the
		// texture loader's default case throws.
		throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 732,
			"Internal error" );
	}
	if ( !file->mSeekSet( field_130 ) )
	{
		throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 737,
			"Error seeking file" );
	}

	bool			isPaletted = field_10c == 8;
	long			width = field_110;
	unsigned char	*row = (unsigned char *)field_13c;

	for ( long y = field_114; y > 0; y--, row += field_120 )
	{
		int		plane = 0;
		int		x = 0;

		while ( x < width )
		{
			unsigned char	value = ReadByte( file );
			int				count;

			if ( ( value & 0xc0 ) == 0xc0 )
			{
				count = value & 0x3f;
				value = ReadByte( file );
			}
			else
				count = 1;

			if ( isPaletted )
			{
				while ( count-- )
				{
					if ( x < width )
						row[x] = value;
					x++;
				}
			}
			else
			{
				while ( count-- )
				{
					if ( x < width )
						row[x * 3 + plane] = value;
					x++;
					if ( x >= width && ++plane < 3 )
						x = 0;
				}
			}
		}
	}

	if ( field_134 != 0 )
	{
		if ( !file->mSeekSet( field_134 ) )
		{
			throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 810,
				"Error seeking file" );
		}
		field_140 = (long)tBaseItemBag::operator new( field_138 * 4 );
		if ( field_140 == 0 )
		{
			throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 818,
				"Out of memory" );
		}
		if ( file->mRead( (void *)field_140, 0x300 ) != 0x300 )
		{
			throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 826,
				"Unable to read RGBTRIPLE array" );
		}
		unsigned char	*triples = (unsigned char *)field_140;
		RGBQUAD			color;
		color.rgbReserved = 0;
		for ( int i = field_138 - 1; i >= 0; i-- )
		{
			color.rgbRed = triples[i * 3];
			color.rgbGreen = triples[i * 3 + 1];
			color.rgbBlue = triples[i * 3 + 2];
			( (RGBQUAD *)field_140 )[i] = color;
		}
	}
}

// FUNCTION: STARFLEETOP 0x006a62f0
// The exe's error string names this method tTexBmp::mLock. field_12c counts the locks; the
// first one reads the pixels into a buffer of field_128 bytes.
bool tTexBmp::mLock( void )
{
	if ( field_04 == 0 )
		return false;
	if ( field_12c > 0 )
	{
		field_12c++;
		return true;
	}

	tBasicFile	*file = vfunction1();

	try
	{
		if ( file == NULL )
		{
			throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 870,
				"tTexBmp::mLock(): Unable to create tBasicFile object" );
		}

		file->mOpenForRead( field_08 );
		field_13c = (long)tBaseItemBag::operator new( field_128 );
		if ( field_13c == 0 )
		{
			throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 885,
				"Out of memory" );
		}

		switch ( field_04 )
		{
		case 1:
		case 2:
			vfunction4( file );
			break;
		case 3:
			vfunction5( file );
			break;
		default:
			throw fn_006a2ec0( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\texBmp.cpp", 904,
				"Internal error" );
		}

		file->mClose();
		delete file;
		field_12c++;
	}
	catch ( fn_006a2ec0_Error error )
	{
		if ( file != NULL )
		{
			file->mClose();
			delete file;
		}
		if ( field_13c != 0 )
		{
			tBaseItemBag::operator delete( (void *)field_13c );
			field_13c = 0;
		}
		if ( field_140 != 0 )
		{
			tBaseItemBag::operator delete( (void *)field_140 );
			field_140 = 0;
		}
		return false;
	}
	return true;
}
