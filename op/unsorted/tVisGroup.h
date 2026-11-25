//	tVisGroup.h: tVisGroup, a visual object that holds others.
// Evidence:  class (RTTI: tVisGroup type descriptor 0x0084d040, vftable 0x007b7d18, 40 slots,
// base tVisObject; original file unknown). Object size 0xd4: the highest store of the constructor 0x0067da80 is the
// word at +0xd0, and tRadioGroup puts its second vftable at +0xd4 (the exact tFleetEntry constructor).

#ifndef tVisGroup_h
#define tVisGroup_h 1

#include "unsorted/tVisObject.h"
#include "Tiger/tstring.h"
#include <list>

class tVisGroup;

// HYPOTHESIS: the library's map< tString, int, less_ss > as the original's library lays it out
// (the toolchain's xtree keeps a static nil node): an allocator byte, a comparison byte, the head
// node at +4, a nil node of its own at +8, the multi flag at +0xc and the size at +0x10; nodes
// of 0x3c bytes. Only what tVisGroup's constructor (0x0067da80) builds is declared.
struct c_tMachine_Field14
{
	struct node
	{
		node*	left;
		node*	parent;
		node*	right;
		std::pair< const tString, int >	value;
		char	color;
		char	isnil;
	};
	struct _Pr
	{
	};
	struct _A
	{
	};

	c_tMachine_Field14( const _Pr& pred = _Pr(), const _A& al = _A() )
		: allocator( al ), key_compare( pred ), _Multi( false )
	{
		_Init();
	}
	~c_tMachine_Field14();
	void _Init()
	{
		_Nil = _Buynode( 0, 1 );
		_Nil->isnil = 1;
		_Left( _Nil ) = 0, _Right( _Nil ) = 0;
		_Head = fn_006373b0( _Nil, 0 );
		_Lmost() = _Head, _Rmost() = _Head;
		_Size = 0;
	}
	static node*& _Left( node* p )
	{
		return p->left;
	}
	static node*& _Right( node* p )
	{
		return p->right;
	}
	node*& _Lmost() const
	{
		return _Left( _Head );
	}
	node*& _Rmost() const
	{
		return _Right( _Head );
	}
	// MATCH-ONLY: the library's _Buynode( parent, colour ), which the original expands inline
	// for the nil node and calls out of line (0x006373b0) for the head.
	node* _Buynode( node* parent, char color )
	{
		node* s = (node*)operator new( sizeof( node ) );
		s->parent = parent;
		s->color = color;
		s->isnil = 0;
		return s;
	}
	node* fn_006373b0( node* parent, char color );

	_A	allocator;
	_Pr	key_compare;
	node*	_Head;
	node*	_Nil;
	bool	_Multi;
	unsigned int	_Size;
};

// HYPOTHESIS: the part of tMachine at +4, built as one object: the original keeps its address
// apart from tMachine's, and stores the int only once the tString is constructed.
struct c_tMachine_Field04
{
	c_tMachine_Field04()
	{
		field_24 = 0;
	}

	std::vector< void * >	field_00;
	c_tMachine_Field14	field_10;
	int	field_24;
	tString	field_28;
};

// tMachine: vftable 0x007b7dbc, 0x6c bytes, destroyed by 0x0067f490 (the name is the analysis's).
// HYPOTHESIS: the member types past the map are read from tVisGroup's constructor alone; the
// element types of the vector and the list are not known.
class tMachine
{
public:
	tMachine()
	{
		field_68 = 0;
		field_64 = 1;
		field_54 = 0;
	}
	virtual ~tMachine();
	// HYPOTHESIS: 0x0067f010 takes the group and a pointer into an event record, and answers
	// whether the machine handled it.
	bool fn_0067f010( tVisGroup *group, int *data );
	void fn_0067f090( tVisGroup *group, int param_2, int param_3 );

	c_tMachine_Field04	field_04;
	int	field_54;
	std::list< void * >	field_58;
	int	field_64;
	int	field_68;
};

// HYPOTHESIS: tFleetEntry.h still declares a reduced tVisGroup of its own; it must include this
// header instead (HEADER REQUEST in the lane notes). Slot n of the vftable is vfunction(n+1),
// as the analysis names them; slots 1 to 21 are tObjBase's.
class tVisGroup : public tVisObject
{
public:
	tVisGroup();
	virtual ~tVisGroup();
	virtual void vfunction22();
	virtual void vfunction23();
	virtual void vfunction24();
	virtual void vfunction25();
	virtual void vfunction26();
	virtual void vfunction27();
	virtual void vfunction28();
	virtual void vfunction29();
	virtual void vfunction30();
	// HYPOTHESIS: the event handler of tObjBase (slot 31), as in tListBox_Base; 0x0067d1c0 is the base's.
	virtual bool vfunction31( int *event );	// 0x0067dd00
	virtual void vfunction32();
	virtual void vfunction33();
	virtual void vfunction34();
	virtual void vfunction35();
	virtual void vfunction36();
	virtual void vfunction37();	// 0x0067e340
	virtual void vfunction38();
	virtual void vfunction39();
	virtual void vfunction40();	// 0x0067dcf0
	// HYPOTHESIS: past slot 40 the real vtable is the derived class's; these are the slots the
	// code that goes through a tVisGroup pointer calls (kept from the reduced declaration that
	// tFleetEntry.h held before), and tSeqScene overrides 41 to 44.
	virtual int vfunction41(int);
	virtual void vfunction42();
	virtual void vfunction43();
	virtual void vfunction44();
	virtual void vfunction45();
	virtual void vfunction46();
	virtual void vfunction47();
	virtual void vfunction48( bool b );
	virtual void vfunction49( bool b );

	// HYPOTHESIS: tObjBase::vfunction31 (0x0067d1c0), which tObjBase.h does not declare yet.
	bool fn_0067d1c0( int *event );

	int field_64;	// HYPOTHESIS: the constructor never writes it
	// The same type sits at tButton +0x74, whose destructor runs 0x0067f490 too.
	tMachine field_68;
};

typedef char tVisGroup_size_is_0xd4[ sizeof( tVisGroup ) == 0xd4 ? 1 : -1 ];

#endif	// tVisGroup_h
