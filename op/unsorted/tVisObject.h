//	tVisObject.h: tObjBase and tVisObject, the bases of the visual objects (tListBox_Base).
// Evidence:  class (RTTI; original file unknown)

#ifndef tVisObject_h
#define tVisObject_h 1

class tScrollBar;
class tVisObject;
class tString;

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

// HYPOTHESIS: the object at +0x14 is the surface the visual objects draw on (tSurface.h):
// tListBox_Base::vfunction44 (0x004c679a) fills a rectangle on it with 0x00675470.
class tSurface;

// HYPOTHESIS: the object at tObjBase +0x10 that a surface is shown on; 0x00693fc0 copies the
// pixels of a surface inside a rectangle to it at a position (tSurface::vfunction8, 0x00675a00).
class c_tObjBase_Field10
{
public:
	void fn_00693fc0( int width, void *pixels, int x, int y, const tRect *rect, unsigned char b );
};

// HYPOTHESIS: what the object at tObjBase +0x4c points to (the shape the object answers to
// when it is hit): 0x0067cf10 passes its address, and the point relative to +0x3c, to 0x00696b40;
// 0x0067cf70 sets +0x34 from its +0x4 and +0x6 and the rectangle's size from its +0x0 and +0x2.
struct c_tObjBase_Field4C
{
	short field_00;
	short field_02;
	short field_04;
	short field_06;
};

// tObjBase: vftable 0x0079ee08, 36 slots (the destructor in slot 3), constructor 0x0067cb70.
// The layout is read from the constructor and from the 25 functions of 0x0067cb70 to 0x0067d590:
// the objects form a tree (+0xc the parent, +0x24 the first child and +0x28 the last, +0x4 the
// next and +0x8 the previous child of the same parent). Slots 0 to 28 are declared. Slot 29 is
// 0x0067d160, void(), which tScrollBar and tSeqScene override and tMetaMapControl declares
// without virtual (its symbol-table name is non-virtual). Slot 30 is
// 0x0067d1c0, bool( event* ), which tVisGroup, tListBox_Base and tScrollBar each declare with
// their own event type (HEADER REQUEST: those two names settled, then vfunction30 to vfunction36
// here: slot 29 above, slot 31 0x0067cbf0 void( const tRect* ), slot 32 0x0067cc10 void( tRect* ), slot 33
// 0x0046c477 tRect( tRect ), slot 34 0x0067d450 void( c_tObjBase_Field10* ), slot 35 0x0067cf10
// bool( int x, int y )). Slots 2 and 4 are declared as tListBox_Base uses them, on its font,
// which is not a tObjBase: tObjBase's own slot 2 is 0x0067d3a0, bool() and slot 4 is 0x0067d4d0,
// tObjBase*( tObjBase* child ) (HEADER REQUEST: give the font its own class).
class tObjBase
{
public:
	// 0x0067cb70: sets vftable 0x0079ee08 and +0x4 to +0x5c.
	tObjBase();
	// HYPOTHESIS: 0x0067d870; the fleet panel's constructor calls it on its fleet form's +0xc with
	// the form and the form's +0x8 until +0x8 is clear, so it takes a child out of the form.
	void fn_0067d870( tVisObject *pForm, tVisObject *pChild );
	// Out of line members of 0x0067cb70 to 0x0067d590 that are not in the vftable: 0x0067cd70
	// moves the object and its children by an offset, 0x0067cee0 sets bit 5 of +0x40 and tells
	// the parent, 0x0067cf70 sets the object's shape and its visible flag, 0x0067d310 gives up
	// the capture of +0x2c, 0x0067d4a0 sets +0x14 on the object and its children.
	void fn_0067cd70( int dx, int dy );
	void fn_0067cee0();
	void fn_0067cf70( c_tObjBase_Field4C *shape, unsigned char visible );
	bool fn_0067d310( tObjBase *child );
	void fn_0067d4a0( tSurface *surface );
	// 0x0046c408 sets bit 3 of +0x40 from the low bit of the value and tells the parent
	// (tMetaMapControl's constructor (0x005e2f95) calls it directly with 1).
	virtual void vfunction1( unsigned int n );
	// 0x0067d350: +0x30 takes the object (the capture); true when it was taken.
	virtual bool vfunction2( tObjBase *a );
	// HYPOTHESIS: +0x8 measures a run of text in a font, -1 for the whole string;
	// tListBox_Base::vfunction53 (0x004c6af8) asks it of its font.
	virtual int vfunction3( unsigned short *text, int length );
	virtual ~tObjBase();
	// HYPOTHESIS: +0x10 returns a height; tListBox_Base::vfunction43 (0x004c6787) asks it of
	// its font.
	virtual int vfunction5();
	// 0x0067d590: takes a child out of this object's list (and releases the capture); the
	// second value says whether to call the child's slot 17 first. Returns the child.
	virtual tObjBase *vfunction6( tObjBase *child, char flag );
	virtual void vfunction7( int x, int y );	// 0x0067cd50: places the object at a position (the fleet entry's 0x004c2f74)
	// +0x1c draws the object inside the area given (0x0067d040); tMetaMapControl's
	// override (0x005e3fdd) calls it first with its own area.
	virtual void vfunction8( const tRect *area );
	// 0x0067cc70: sets the rectangle at +0x34 and tells the parent.
	virtual void vfunction9( short left, short top, short right, short bottom );
	virtual bool vfunction10();	// 0x0046c436
	virtual void vfunction11();
	virtual void vfunction12();	// +0x2c
	virtual void vfunction13(); virtual bool vfunction14(); virtual void vfunction15( bool a ); virtual void vfunction16(); virtual void vfunction17(); virtual void vfunction18(); virtual void vfunction19(); virtual int vfunction20(); virtual void vfunction21( int n );	// HYPOTHESIS: +0x4c reads a control's value and +0x50 sets its state (the fleet entry's 0x004c27a8)
	virtual void vfunction22();
	virtual void *vfunction23();	// 0x0046c452: returns the address 0x00c692a8
	virtual void vfunction24( const tString &s );	// 0x00401038 (an empty function with one argument); tFleetEntry's classes pass a tString
	virtual void vfunction25();
	virtual void vfunction26();
	virtual void vfunction27();
	virtual bool vfunction28( int a, void *b );	// 0x0046c464: puts a string in the second argument, returns false
	virtual bool vfunction29( int a, int *b );	// 0x0046c458: clears *b, returns false
	// HYPOTHESIS: field types and names unknown; +0x34 is the object's rectangle. The constructor
	// (0x0067cb70) sets +0x4 to +0x5c, so these are tObjBase's.
	// +0x4 and +0x8 link the objects beside this one, one each way (0x004c692a walks
	// both chains).
	tVisObject *field_04;
	tVisObject *field_08;
	// +0xc holds the parent; tScrollBar's destructor (0x004c8201) hands itself to
	// it through 0x0067d311 when its +0x80 is set.
	tVisObject *field_0C;
	c_tObjBase_Field10 *field_10;
	tSurface *field_14;
	// HYPOTHESIS: +0x18 is the child 0x0067d160 and 0x0067ce90 walk with; +0x1c holds flags whose bit 0 says the walk is done; +0x20 counts the children.
	tVisObject *field_18;
	int field_1C;
	int field_20;
	tVisObject *field_24;	// +0x24 is the first child, its siblings follow through +0x4 (the fleet entry's 0x004c2aba)
	tVisObject *field_28;	// the last child
	tVisObject *field_2C;	// the child that has the capture (0x0067d2d0, 0x0067d310)
	tObjBase *field_30;	// the object given by vfunction2
	tRect field_34;
	tPoint field_3C;	// HYPOTHESIS: a position; tSurface's constructor (0x00675080) sets it to its x and y
	int field_40;	// HYPOTHESIS: flags; tListBox_Select::vfunction31 handles a click only when bit 0 is set
	int field_44;	// HYPOTHESIS: an order (-1 for none): 0x0067d4d0 puts a child before the first one whose +0x44 is not less; the fleet panel's constructor (0x0048bd4d) sets its fleet form's to 3000
	int field_48;
	c_tObjBase_Field4C *field_4C;
	int field_50;
	// HYPOTHESIS: a handler tried before the object's own slot 30: 0x0067d1c0 calls it with the event and the object.
	bool ( *field_54 )( int *event, tObjBase *object );
	// HYPOTHESIS: a function 0x0067d160 calls on a child with bit 3 of +0x40 set.
	void ( *field_58 )( tObjBase *object );
	int field_5C;
};
typedef char tObjBase_size_check[ sizeof( tObjBase ) == 0x60 ? 1 : -1 ];

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

// The opening braces share the signature's line, so the listing's first line record of each
// destructor quotes a text no unit has.
inline tObjBase::~tObjBase() {
}

inline tVisObject::~tVisObject() {
	if ( field_60 )
		fn_006849d0( 0 );
}

typedef char tVisObject_size_check[ sizeof( tVisObject ) == 0x64 ? 1 : -1 ];

#endif	// tVisObject_h
