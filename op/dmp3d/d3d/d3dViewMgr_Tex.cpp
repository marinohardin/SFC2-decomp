#include <windows.h>
#include <string.h>

#include "dmp3d/common/stream_basicfile.h"

// HYPOTHESIS: the DirectDraw objects are used through their C interface (explicit this
// argument, stdcall slots), as the exe's calls show. Only the slots the method calls are
// named, in vtable order. (The class named tDDSurface in the RTTI is unsorted/tDDSurface.h.)
struct tDDPalette
{
	virtual long __stdcall slot_00( void ) = 0;
	virtual long __stdcall slot_04( void ) = 0;
	virtual unsigned long __stdcall Release( void ) = 0;
	virtual long __stdcall slot_0c( void ) = 0;
	virtual long __stdcall slot_10( void ) = 0;
	virtual long __stdcall slot_14( void ) = 0;
	virtual long __stdcall SetEntries( unsigned long flags, unsigned long start, unsigned long count, const unsigned char *entries ) = 0;
};

struct tDDSurfaceIface
{
	virtual long __stdcall QueryInterface( const void *iid, void **out ) = 0;
	virtual long __stdcall slot_04( void ) = 0;
	virtual unsigned long __stdcall Release( void ) = 0;
	virtual long __stdcall slot_0c( void ) = 0;
	virtual long __stdcall slot_10( void ) = 0;
	virtual long __stdcall slot_14( void ) = 0;
	virtual long __stdcall slot_18( void ) = 0;
	virtual long __stdcall slot_1c( void ) = 0;
	virtual long __stdcall slot_20( void ) = 0;
	virtual long __stdcall slot_24( void ) = 0;
	virtual long __stdcall slot_28( void ) = 0;
	virtual long __stdcall slot_2c( void ) = 0;
	virtual long __stdcall slot_30( void ) = 0;
	virtual long __stdcall slot_34( void ) = 0;
	virtual long __stdcall slot_38( void ) = 0;
	virtual long __stdcall slot_3c( void ) = 0;
	virtual long __stdcall slot_40( void ) = 0;
	virtual long __stdcall slot_44( void ) = 0;
	virtual long __stdcall slot_48( void ) = 0;
	virtual long __stdcall slot_4c( void ) = 0;
	virtual long __stdcall GetPalette( tDDPalette **out ) = 0;	// +0x50
	virtual long __stdcall slot_54( void ) = 0;
	virtual long __stdcall slot_58( void ) = 0;
	virtual long __stdcall slot_5c( void ) = 0;
	virtual long __stdcall slot_60( void ) = 0;
	virtual long __stdcall slot_64( void ) = 0;
	virtual long __stdcall slot_68( void ) = 0;
	virtual long __stdcall slot_6c( void ) = 0;
	virtual long __stdcall slot_70( void ) = 0;
	virtual long __stdcall slot_74( void ) = 0;
	virtual long __stdcall slot_78( void ) = 0;
	virtual long __stdcall SetPalette( tDDPalette *palette ) = 0;	// +0x7c
};

struct tDDraw
{
	virtual long __stdcall slot_00( void ) = 0;
	virtual long __stdcall slot_04( void ) = 0;
	virtual long __stdcall slot_08( void ) = 0;
	virtual long __stdcall slot_0c( void ) = 0;
	virtual long __stdcall slot_10( void ) = 0;
	virtual long __stdcall CreatePalette( unsigned long flags, const unsigned char *entries, tDDPalette **out, void *outer ) = 0;	// +0x14
	virtual long __stdcall CreateSurface( const void *desc, tDDSurfaceIface **out, void *outer ) = 0;	// +0x18
};

// HYPOTHESIS: the rectangle a texture face reports, four longs; its name is not known (the
// Tiger tRect, declared in unsorted/tVisObject.h, holds shorts).
struct fn_006a8930_Rect
{
	long	left;
	long	top;
	long	right;
	long	bottom;

	long	Width( void ) const { long w = right - left; return w < 0 ? 0 : w; }
	long	Height( void ) const { long h = bottom - top; return h < 0 ? 0 : h; }
};

// HYPOTHESIS: the texture format description the lookup is keyed on.
struct tTexFormat
{
	char	field_00[0x34];
	long	field_34;		// 1 when palettized
	long	field_38;		// bits per pixel
	bool	fn_006a11f0( tTexFormat *other );
};

class tTexFace
{
public:
	virtual void		slot_00();
	virtual void		slot_04();
	virtual void		slot_08();
	virtual tTexFormat	*slot_0c( void );
	virtual fn_006a8930_Rect	*slot_10( void );
	char				field_04[0x70];	// HYPOTHESIS: the face is 0x74 bytes (the slot allocation is 0xf8)
	tTexFace( void );
	void				fn_006a79f5( tDDSurfaceIface *surf, void *desc, int flag );
};

struct tTexSlot
{
	tDDSurfaceIface	*surface;		// +0x00
	tDDSurfaceIface	*texture;		// +0x04
	unsigned long	desc[0x1f];	// +0x08: the surface description
	tTexFace	face;			// +0x84

	tTexSlot() { surface = 0; texture = 0; }
	void Reset( void )
	{
		if ( texture )
		{
			texture->Release();
			texture = 0;
		}
		if ( surface )
		{
			surface->Release();
			surface = 0;
		}
		memset( desc, 0, sizeof( desc ) );
		face.fn_006a79f5( 0, desc, 0 );
	}
};

struct tTexNode
{
	char		field_00[0x5c];
	tTexNode	*next;			// +0x5c
};

class tD3DViewMgr
{
public:
	tDDraw		*fn_0069d6df( void );
	tTexFace	*fn_006a8930( int index, tTexFormat *fmt, int width, int height, unsigned char *palette );

	char		field_0[0x3cdc];
	unsigned long	flags;
	char		field_3ce0[0x3f60 - 0x3ce0];
	tTexSlot	*slots[0x1000];
	tTexNode	*formats;
};

void fn_006a4470( long hr, const char *file, int line );

#define TEX_CHECK( hr ) \
	do { long r = (hr); if ( r ) { fn_006a4470( r, "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\d3d\\d3dViewMgr_Tex.cpp", 397 ); throw r; } } while ( 0 )

// FUNCTION: STARFLEETOP 0x006a8930
tTexFace *tD3DViewMgr::fn_006a8930( int index, tTexFormat *fmt, int width, int height, unsigned char *palette )
{
	index--;
	if ( (unsigned int)index >= 0x1000 )
		return 0;

	tTexFace *face = (tTexFace *)slots[index];
	if ( face )
	{
		face = (tTexFace *)( (char *)face + 0x84 );
		long w = face->slot_10()->Width();
		if ( w == width )
		{
			long h = face->slot_10()->Height();
			if ( h == height && face->slot_0c()->fn_006a11f0( fmt ) )
			{
				if ( palette && fmt->field_34 == 1 )
				{
					tDDSurfaceIface *surf = slots[index]->surface;
					tDDPalette *pal = 0;
					surf->GetPalette( &pal );
					if ( pal )
					{
						unsigned char entries[0x400];
						bool alpha = ( flags >> 7 ) & 1;
						for ( int i = 0; i < 0x100; i++ )
						{
							entries[i * 4 + 0] = palette[i * 4 + 2];
							entries[i * 4 + 1] = palette[i * 4 + 1];
							entries[i * 4 + 2] = palette[i * 4 + 0];
							if ( alpha )
								entries[i * 4 + 3] = palette[i * 4 + 3];
							else
								entries[i * 4 + 3] = 0;
						}
						pal->SetEntries( 0, 0, 0x100, entries );
						pal->Release();
					}
				}
				return face;
			}
		}
	}

	tTexNode *node;
	for ( node = formats; node; node = node->next )
	{
		if ( ( (tTexFormat *)node )->fn_006a11f0( fmt ) )
			break;
	}
	if ( !node )
		return 0;

	if ( !slots[index] )
	{
		slots[index] = new tTexSlot;
		if ( !slots[index] )
			return 0;
	}

	struct { tTexSlot *p; } sp;
	sp.p = slots[index];
	tTexSlot &r = *sp.p;
	r.Reset();

	unsigned long desc[0x1f];
	memset( desc, 0, sizeof( desc ) );
	desc[0] = 0x7c;
	desc[1] = 0x101007;
	desc[3] = width;
	desc[2] = height;
	memcpy( &desc[0x12], (char *)node + 0x3c, 0x20 );
	// HYPOTHESIS: the original wrapped the creation in a catch-all that resets the slot
	try {
	desc[0x1a] = 0x1000;
	desc[0x1b] = 0x10;
	desc[0x1e] = 0;

	tDDSurfaceIface *surf = 0;
	tDDSurfaceIface *tex = 0;
	tDDraw *dd = fn_0069d6df();
	long hr = dd->CreateSurface( desc, &surf, 0 );
	if ( hr )
	{
		fn_006a4470( hr, "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\d3d\\d3dViewMgr_Tex.cpp", 397 );
		throw hr;
	}

	long hr2 = surf->QueryInterface( (const void *)0x7bb158, (void **)&tex );
	if ( hr2 < 0 )
	{
		surf->Release();
		throw hr2;
	}

	if ( ( (tTexFormat *)node )->field_34 == 1 && ( (tTexFormat *)node )->field_38 == 8 )
	{
		unsigned char entries[0x400];
		bool alpha = ( flags >> 7 ) & 1;
		if ( palette )
		{
			for ( int i = 0; i < 0x100; i++ )
			{
				entries[i * 4 + 0] = palette[i * 4 + 2];
				entries[i * 4 + 1] = palette[i * 4 + 1];
				entries[i * 4 + 2] = palette[i * 4 + 0];
				if ( alpha )
					entries[i * 4 + 3] = palette[i * 4 + 3];
				else
					entries[i * 4 + 3] = 0;
			}
		}
		else
			memset( entries, 0, sizeof( entries ) );

		unsigned long pcaps = 0x44;
		if ( flags & 0x80 )
			pcaps = 0x444;
		tDDPalette *pal = 0;
		dd = fn_0069d6df();
		long hr3 = dd->CreatePalette( pcaps, entries, &pal, 0 );
		if ( hr3 )
		{
			surf->Release();
			throw hr3;
		}
		surf->SetPalette( pal );
		pal->Release();
	}

	memcpy( r.desc, desc, sizeof( desc ) );
	r.texture = tex;
	r.surface = surf;
	r.face.fn_006a79f5( surf, desc, 0 );
	return &r.face;
	}
	catch ( ... )
	{
		sp.p->Reset();
	}
	return 0;
}
