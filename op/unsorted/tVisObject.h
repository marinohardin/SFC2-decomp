//	tVisObject.h: tObjBase and tVisObject, the bases of the visual objects (tListBox_Base).
// Evidence:  class (RTTI; original file unknown)

#ifndef tVisObject_h
#define tVisObject_h 1

class tScrollBar;
class tVisObject;

// The original takes tRect from tbasic.h. The unit map has no place for a header without a unit of
// its own yet, so the part of it these units need is kept here. The members the compiler did not
// inline are out of line in the exe: mClip 0x004c2410, mMakeClippedRect 0x004c6376, mIsEmpty
// 0x0045f4e6.
#ifndef	tbasic_tRect_h
#define	tbasic_tRect_h	1

struct tRect
{
	short	left;
	short	top;
	short	right;
	short	bottom;

	inline tRect()
	{
		left = top = right = bottom = 0;
	}

	inline tRect( int sx, int sy, int ex, int ey )
	{
		left	= (short)sx;
		top		= (short)sy;
		right	= (short)ex;
		bottom	= (short)ey;
	};
	inline tRect( const tRect &T ) { left = T.left; top = T.top; right = T.right; bottom = T.bottom; }
	inline void mSet( int sx, int sy, int ex, int ey )
	{
		left	= (short)sx;
		top		= (short)sy;
		right	= (short)ex;
		bottom	= (short)ey;
	};
	inline tRect & operator = ( const tRect &T )
	{
		left	= T.left;
		top		= T.top;
		right	= T.right;
		bottom	= T.bottom;
		return *this;
	}

	inline unsigned char mIntersects( const tRect *pR )
	{
		return( ( left < pR->right ) && ( top < pR->bottom )
			&& ( right > pR->left ) && ( bottom > pR->top ) );
	};

	inline unsigned char mIsEnclosedBy( const tRect *pE ) const
	{
		return( ( pE->left <= left ) && ( pE->top <= top )
			&& ( pE->right >= right ) && ( pE->bottom >= bottom ) );
	};

	inline void mOffset( int deltaX, int deltaY )
	{
		left += (short)deltaX;  right  += (short)deltaX;
		top  += (short)deltaY;  bottom += (short)deltaY;
	};

	inline void mClip( const tRect *pC )
	{
		if ( left < pC->left )		left	= pC->left;
		if ( top < pC->top )		top		= pC->top;
		if ( right > pC->right )	right	= pC->right;
		if ( bottom > pC->bottom )	bottom	= pC->bottom;
	};

	void mMakeClippedRect( const tRect *pClip, tRect *pNewRect ) const;

	// Out of line in the exe at 0x004c855e (defined in tScrollBar.cpp); tScrollBar::fn_004c84d7
	// (0x004c84d7) calls it.
	unsigned char mPointInside( int x, int y ) const;

	inline unsigned char mIsEmpty( void ) const
	{
		return ( ( left >= right ) || ( top >= bottom ) );
	};

	inline int mWidth( void ) const
	{
		int		wd = right - left;
		return( wd >= 0 ? wd : 0 );
	};

	inline int mHeight( void ) const
	{
		int		ht = bottom - top;
		return( ht >= 0 ? ht : 0 );
	};
};

#endif	// tbasic_tRect_h

// tPoint from the SDK's tbasic.h, kept here for the same reason as tRect.
#ifndef	tbasic_tPoint_h
#define	tbasic_tPoint_h	1

struct tPoint
{
	short	x;
	short	y;

	tPoint() {}

	tPoint( short _x, short _y ) : x( _x ), y( _y ) {}
	tPoint( const tPoint& point ) : x( point.x ), y( point.y ) {}
};

#endif	// tbasic_tPoint_h

struct c_tListBox_Base_Item;
struct c_tScrollBar_FieldA8;

// HYPOTHESIS: the object at tVisObject +0x14 that the visual objects draw on; 0x00675470 fills a
// rectangle in a colour (tListBox_Base::vfunction44, 0x004c679a).
class c_tVisObject_Field14
{
public:
	void fn_00675470( int color,const tRect *rect, int a, int b, int c );
	// HYPOTHESIS: draws an image at a point, clipped to the area given; tScrollBar::vfunction8
	// (0x004c8439) draws its thumb with it.
	void fn_00675280( c_tScrollBar_FieldA8 *image, int x, int y, const tRect *area, int a, int b );
};

// tObjBase: vftable 0x0079ee08, destructor in slot 3. Only the slots called are declared.
class tObjBase
{
public:
	// HYPOTHESIS: 0x0067cb70 in the SDK library range; it sets vftable 0x0079ee08 and +0x34 to +0x5c.
	tObjBase();
	// HYPOTHESIS: 0x0067d870; the fleet panel's constructor calls it on its fleet form's +0xc with
	// the form and the form's +0x8 until +0x8 is clear, so it takes a child out of the form.
	void fn_0067d870( tVisObject *pForm, tVisObject *pChild );
	// HYPOTHESIS: slot 0 (0x0046c408) takes one value; tMetaMapControl's constructor
	// (0x005e2f95) calls it directly with 1.
	virtual void vfunction1( unsigned int n );
	virtual void vfunction2();
	// HYPOTHESIS: +0x8 measures a run of text in a font, -1 for the whole string;
	// tListBox_Base::vfunction53 (0x004c6af8) asks it of its font.
	virtual int vfunction3( unsigned short *text, int length );
	virtual ~tObjBase();
	// HYPOTHESIS: +0x10 returns a height; tListBox_Base::vfunction43 (0x004c6787) asks it of
	// its font.
	virtual int vfunction5();
	virtual void vfunction6();
	virtual void vfunction7( int x, int y );	// HYPOTHESIS: places the object at a position (the fleet entry's 0x004c2f74)
	// HYPOTHESIS: +0x1c draws the object inside the area given (0x0067d040); tMetaMapControl's
	// override (0x005e3fdd) calls it first with its own area.
	virtual void vfunction8( const tRect *area );
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual void vfunction12();	// +0x2c
	virtual void vfunction13(); virtual bool vfunction14(); virtual void vfunction15( bool a ); virtual void vfunction16(); virtual void vfunction17(); virtual void vfunction18(); virtual void vfunction19(); virtual int vfunction20(); virtual void vfunction21( int n );	// HYPOTHESIS: +0x4c reads a control's value and +0x50 sets its state (the fleet entry's 0x004c27a8)
	// HYPOTHESIS: field types and names unknown; +0x34 is the object's rectangle. The constructor
	// (0x0067cb70) sets +0x4 to +0x5c, so these are tObjBase's.
	// HYPOTHESIS: +0x4 and +0x8 link the objects beside this one, one each way (0x004c692a walks
	// both chains).
	tVisObject *field_04;
	tVisObject *field_08;
	// HYPOTHESIS: +0xc holds an object that tScrollBar's destructor (0x004c8201) hands itself to
	// through 0x0067d311 when its +0x80 is set.
	tVisObject *field_0C;
	unsigned char field_10[ 0x14 - 0x10 ];
	c_tVisObject_Field14 *field_14;
	unsigned char field_18[ 0x24 - 0x18 ]; tVisObject *field_24; unsigned char field_28[ 0x34 - 0x28 ];	// HYPOTHESIS: +0x24 is the first child, its siblings follow through +0x4 (the fleet entry's 0x004c2aba)
	tRect field_34;
	tPoint field_3C;	// HYPOTHESIS: a position; tSurface's constructor (0x00675080) sets it to its x and y
	int field_40;	// HYPOTHESIS: flags; tListBox_Select::vfunction31 handles a click only when bit 0 is set
	int field_44;	// HYPOTHESIS: a delay or interval; the fleet panel's constructor (0x0048bd4d) sets its fleet form's to 3000
	int field_48;
	unsigned char field_4C[ 0x60 - 0x4c ];
};

// tVisObject: vftable 0x007a2340.
class tVisObject : public tObjBase
{
public:
	// HYPOTHESIS: inline; tScrollBar's constructor (0x004c8113) sets the vftable and +0x60 in place.
	tVisObject()
	{
		field_60 = 0;
	}
	virtual ~tVisObject();
	// HYPOTHESIS: a member of tVisObject or tObjBase; it lies in the SDK library range.
	void fn_006849d0( int a );
	// HYPOTHESIS: a member of tVisObject; tListBox_Base::vfunction31 calls it directly after scrolling.
	void fn_0045f4c2();
	// HYPOTHESIS: a member of tVisObject; tListBox_Base::vfunction58 calls it directly after
	// changing an item's key.
	void fn_0045f4ec();
	// HYPOTHESIS: a member of tVisObject in the SDK library range; tListBox_Select::vfunction66
	// calls it directly when its flag is set and bit 0 of field_40 is clear.
	void fn_0067ce90();
	// HYPOTHESIS: a member of tVisObject in the SDK library range; tScrollBar's destructor
	// (0x004c8201) calls it on the object at +0xc with itself.
	void fn_0067d311( tVisObject *object );
	// HYPOTHESIS: a member of tVisObject in the SDK library range; tScrollBar::vfunction31
	// (0x004c8587) calls it on the object at +0xc with itself when a press starts, and goes on
	// only when it returns true. Its pair is 0x0067d311.
	bool fn_0067d2d0( tVisObject *object );
	int fn_0067cc50( int a );	// HYPOTHESIS: in the SDK library range; tScrollBar::mLoad (0x004c878a)

	// HYPOTHESIS: the object ends at +0x64: tScrollBar's constructor (0x004c8113) starts its own
	// members there.
	int field_60;
};

inline tObjBase::~tObjBase()
{
}

inline tVisObject::~tVisObject()
{
	if ( field_60 )
		fn_006849d0( 0 );
}

#endif	// tVisObject_h
