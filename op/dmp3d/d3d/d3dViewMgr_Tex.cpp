#include <windows.h>
#include <string.h>

#include "dmp3d/common/stream_basicfile.h"

// HYPOTHESIS: the DirectDraw objects are used through their C interface (explicit this
// argument, stdcall slots), as the exe's calls show. Only the slots the method calls are
// named, in vtable order.
struct tDDPalette;
struct tDDSurface;
struct tDDraw;

struct tDDPaletteVtbl
{
	void	*slot_00;
	void	*slot_04;
	unsigned long ( __stdcall *Release )( tDDPalette *self );
	void	*slot_0c;
	void	*slot_10;
	void	*slot_14;
	long	( __stdcall *SetEntries )( tDDPalette *self, unsigned long flags, unsigned long start, unsigned long count, const unsigned char *entries );
};

struct tDDPalette
{
	tDDPaletteVtbl	*lpVtbl;
};

struct tDDSurfaceVtbl
{
	long	( __stdcall *QueryInterface )( tDDSurface *self, const void *iid, void **out );
	void	*slot_04;
	unsigned long ( __stdcall *Release )( tDDSurface *self );
	void	*slot_0c[17];
	long	( __stdcall *GetPalette )( tDDSurface *self, tDDPalette **out );	// +0x50
	void	*slot_54[10];
	long	( __stdcall *SetPalette )( tDDSurface *self, tDDPalette *palette );	// +0x7c
};

struct tDDSurface
{
	tDDSurfaceVtbl	*lpVtbl;
};

struct tDDrawVtbl
{
	void	*slot_00;
	void	*slot_04;
	void	*slot_08;
	void	*slot_0c;
	void	*slot_10;
	long	( __stdcall *CreatePalette )( tDDraw *self, unsigned long flags, const unsigned char *entries, tDDPalette **out, void *outer );	// +0x14
	long	( __stdcall *CreateSurface )( tDDraw *self, const void *desc, tDDSurface **out, void *outer );	// +0x18
};

struct tDDraw
{
	tDDrawVtbl	*lpVtbl;
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
	bool	fn_006a13b3( void *tex );
};

class tTexFace
{
public:
	virtual void		slot_00();
	virtual void		slot_04();
	virtual void		slot_08();
	virtual tTexFace	*slot_0c( tTexFormat *fmt );
	virtual fn_006a8930_Rect	*slot_10( void );
	char				field_04[0x70];	// HYPOTHESIS: the face is 0x74 bytes (the slot allocation is 0xf8)
	tTexFace( void );
	bool				fn_006a12a2( void );
	void				fn_006a79f5( tDDSurface *surf, void *desc, int flag );
};

struct tTexSlot
{
	tDDSurface	*surface;		// +0x00
	tDDSurface	*texture;		// +0x04
	unsigned long	desc[0x1f];	// +0x08: the surface description
	tTexFace	face;			// +0x84

	tTexSlot() { surface = 0; texture = 0; }
	void Reset( void )
	{
		if ( texture )
		{
			texture->lpVtbl->Release( texture );
			texture = 0;
		}
		if ( surface )
		{
			surface->lpVtbl->Release( surface );
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
			if ( h == height && face->slot_0c( fmt )->fn_006a12a2() )
			{
				if ( palette && fmt->field_34 == 1 )
				{
					tDDSurface *surf = slots[index]->surface;
					tDDPalette *pal = 0;
					surf->lpVtbl->GetPalette( surf, &pal );
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
						pal->lpVtbl->SetEntries( pal, 0, 0, 0x100, entries );
						pal->lpVtbl->Release( pal );
					}
				}
				return face;
			}
		}
	}

	tTexNode *node;
	for ( node = formats; node; node = node->next )
	{
		if ( ( (tTexFormat *)node )->fn_006a13b3( fmt ) )
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

	tTexSlot *s = slots[index];
	s->Reset();

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

	tDDSurface *surf = 0;
	tDDSurface *tex = 0;
	tDDraw *dd = fn_0069d6df();
	long hr = dd->lpVtbl->CreateSurface( dd, desc, &surf, 0 );
	if ( hr )
	{
		fn_006a4470( hr, "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\d3d\\d3dViewMgr_Tex.cpp", 397 );
		throw hr;
	}

	hr = surf->lpVtbl->QueryInterface( surf, (const void *)0x7bb158, (void **)&tex );
	if ( hr < 0 )
	{
		surf->lpVtbl->Release( surf );
		throw hr;
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
		hr = dd->lpVtbl->CreatePalette( dd, pcaps, entries, &pal, 0 );
		if ( hr )
		{
			surf->lpVtbl->Release( surf );
			throw hr;
		}
		surf->lpVtbl->SetPalette( surf, pal );
		pal->lpVtbl->Release( pal );
	}

	memcpy( s->desc, desc, sizeof( desc ) );
	s->texture = tex;
	s->surface = surf;
	s->face.fn_006a79f5( surf, desc, 0 );
	return &s->face;
	}
	catch ( ... )
	{
		s->Reset();
	}
	return 0;
}
