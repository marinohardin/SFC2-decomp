// Unit:      unsorted/tNetControl.cpp
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)

#include <string.h>
#include "unsorted/tNetControl.h"
#include "unsorted/tObject_0092deb4.h"
// The class and gNetControl (0x008f5b98) are declared in tNetControl.h, which other units include.
// HYPOTHESIS: field_8CC4's 0xc-byte element; 0x0043458a calls its virtual destructor on each.
struct c_tNetControl_8CC4_entry
{
	virtual ~c_tNetControl_8CC4_entry();
	int m_04;
	int m_08;
};

// FUNCTION: STARFLEETOP 0x00431044
tNetControl::tNetControl()
{
	field_8 = 0;
	field_8CB8 = 0;
	field_8CBC = 0;
	field_8CC0 = 0;
	field_8CB4 = 0;
	field_8D00 = 0;
	field_8CD4 = 0;
	field_8CD8 = 0;
	field_8CDC = 0;
	field_10 = 6;
	field_11 = 0;
	field_8D04 = 0;
	field_8D05 = 0;
	field_8D06 = 0;
	field_8D08 = "";
	field_8D30 = 0;
	field_8CEC = 0.0f;
	field_8CE8 = 0;
	field_10 = 6;
	field_C = 0.0f;
	field_11 = 0;
	field_8CE0 = 0;
	field_8D38 = 0.0;
	field_8CE4 = 0;
}

// FUNCTION: STARFLEETOP 0x0043113d
// FUNCTION: STARFLEETOP 0x0043117a
tNetControl::~tNetControl()
{
	fn_00431200();
	fn_00431227();
	fn_0043124e();
	field_8D00 = 0;
}

// FUNCTION: STARFLEETOP 0x00431159
void tNetControl::fn_00431159( unsigned char n )
{
	int value = field_11 + n;
	if( value >= 1 && value <= 6 )
		field_10 = value;
	else
		field_10 = 6;
}

// HYPOTHESIS: 0x004311ec is the compiler-generated atexit destructor of a file-scope object
// at 0x0091f930 (mov ecx, <object>; jmp <destructor>). The jump goes to about 0x004bda12, not
// to tNetControl's destructor, so the object's class is unknown; it is declared here only by
// the two things this stub uses. Replace both with the real class and object, and drop this
// body, once the dynamic initializer that registers it is decompiled.
class c_0091f930
{
public:
	~c_0091f930();
	void fn_004bdd71();
};
extern c_0091f930 g_0091f930;

// FUNCTION: STARFLEETOP 0x004311ec
void fn_004311ec()
{
	g_0091f930.~c_0091f930();
}

// HYPOTHESIS: 0x004311f6 is a second compiler-generated thunk on the same file-scope object
// at 0x0091f930 (mov ecx, <object>; jmp <method>), jumping to about 0x004bdd71. One of this
// pair and 0x004311ec is probably the constructor thunk and the other the atexit destructor;
// the callee here is named by address until its class is decompiled.
// FUNCTION: STARFLEETOP 0x004311f6
void fn_004311f6()
{
	g_0091f930.fn_004bdd71();
}

// HYPOTHESIS: field_8CB8 heads a singly linked list whose nodes hold the next pointer in
// their first word; this frees every node and empties the list. The node type is unknown.
// FUNCTION: STARFLEETOP 0x00431200
void tNetControl::fn_00431200()
{
	void **node = (void **)field_8CB8;
	while( node )
	{
		void **next = (void **)*node;
		delete node;
		node = next;
	}
	field_8CB8 = 0;
}

// HYPOTHESIS: field_8CBC heads a second list of the same shape as field_8CB8's; this frees
// every node and empties it. The node type is unknown.
// FUNCTION: STARFLEETOP 0x00431227
void tNetControl::fn_00431227()
{
	void **node = (void **)field_8CBC;
	while( node )
	{
		void **next = (void **)*node;
		delete node;
		node = next;
	}
	field_8CBC = 0;
}

// HYPOTHESIS: field_8CC0 heads a third list of the same shape as field_8CB8's; this frees
// every node and empties it. The node type is unknown.
// FUNCTION: STARFLEETOP 0x0043124e
void tNetControl::fn_0043124e()
{
	void **node = (void **)field_8CC0;
	while( node )
	{
		void **next = (void **)*node;
		delete node;
		node = next;
	}
	field_8CC0 = 0;
}

// HYPOTHESIS: the entries 0x0043127f reads from the vector its source fills in. Only the
// offsets it reads are known; the object at +0x10 has a 13-byte getter returning a C string.
class c_0043127f_src_name
{
public:
	const char *fn_0060f6d0();
	void fn_0060f810( const char *s );
};

class c_0043127f_src
{
public:
	virtual ~c_0043127f_src();
	int field_4;
	char field_8[ 8 ];
	c_0043127f_src_name field_10;
	unsigned char field_10_pad[ 0x1c - 0x11 ];
	int field_1C;
	unsigned char field_20;
	unsigned char field_21[ 0x54 - 0x21 ];
	unsigned int field_54;
	int field_58;
};

// HYPOTHESIS: the 0xa8-byte list node built by 0x0043127f and kept at field_8CB8.
class c_0043127f_node
{
public:
	c_0043127f_node();	// 0x004314c2
	c_0043127f_node *next;
	char field_4[ 0x68 - 0x4 ];
	int field_68;
	unsigned char field_6C;
	unsigned char field_6D;
	unsigned char field_6E;
	int field_70;
	int field_74;
	int field_78;
	int field_7C;
	int field_80;
	int field_84;
	double field_88;
	int field_90;
	unsigned char field_94;
	char field_95[ 7 ];
	int field_9C;
	unsigned char field_A0[ 0xa8 - 0xa0 ];
};

// HYPOTHESIS: the object at tNetControl+0x8d00; only the two virtual slots used here are known.
class c_tNetControl_8D00
{
public:
	virtual void fn_vtbl_00();
	virtual void fn_vtbl_04();
	virtual void fn_vtbl_08();
	virtual void fn_vtbl_0C();
	virtual void fn_vtbl_10();
	virtual void fn_vtbl_14();
	virtual void fn_vtbl_18();
	virtual void fn_vtbl_1C();
	virtual void fn_vtbl_20();
	virtual void fn_vtbl_24();
	virtual void fn_vtbl_28();
	virtual void fn_vtbl_2C();
	virtual void fn_vtbl_30();
	virtual void fn_vtbl_34();
	virtual int fn_vtbl_38( std::vector< c_0043127f_src * > *list, int a, int b );
	virtual int fn_vtbl_3C();
	virtual void fn_vtbl_40();
	virtual void fn_vtbl_44();
	virtual void fn_vtbl_48();
	virtual void fn_vtbl_4C();
	virtual void fn_vtbl_50();
	virtual void fn_vtbl_54();
	virtual void fn_vtbl_58();
	virtual void fn_vtbl_5C();
	virtual void fn_vtbl_60();
	virtual void fn_vtbl_64();
	virtual void fn_vtbl_68();
	virtual void fn_vtbl_6C( int n );
	virtual void fn_vtbl_70();
	virtual void fn_vtbl_74();
	virtual void fn_vtbl_78();
	virtual void fn_vtbl_7C();
	virtual void fn_vtbl_80();
	virtual void fn_vtbl_84();
	virtual void fn_vtbl_88();
	virtual void fn_vtbl_8C();
	virtual void fn_vtbl_90();
	virtual void fn_vtbl_94();
	// HYPOTHESIS: argument types unknown; both are passed through from fn_004317ce.
	virtual void fn_vtbl_98( int a, int b );
	// HYPOTHESIS: argument types unknown; both are passed through from fn_004317eb.
	virtual void fn_vtbl_9C( int a, int b );
	// HYPOTHESIS: argument type unknown; passed through from fn_00431808.
	virtual void fn_vtbl_A0( int a );
};

class c_008e1060
{
public:
	void fn_004239ba( int n );
	int fn_004239d6();
};

extern int g_008f5b84;
extern int g_008f5b3c;
extern c_008e1060 g_008e1060;

// The function starts with its EH prolog at 0x00431275; the symbol keeps the name of the
// address 10 bytes in, where analysis first put the start. The 6-byte copy calls 0x007365b0,
// the CRT memcpy (0x00736e90, the other copy of that code, is the memmove the SDK units call).
// It rebuilds the
// field_8CB8 list from the entries field_8D00 reports, keeping the fields of an old node with
// the same id.
// FUNCTION: STARFLEETOP 0x00431275
void tNetControl::fn_0043127f()
{
	std::vector< c_0043127f_src * > list;
	int result = field_8D00->fn_vtbl_38( &list, 0, 0 );
	if( result != 0 )
		return;

	int count = list.size();
	c_0043127f_node *head = 0;
	c_0043127f_node *tail = 0;
	std::vector< c_0043127f_src * >::iterator it = list.begin();
	for( int i = 0; i < count; i++ )
	{
		c_0043127f_node *node = new c_0043127f_node;
		if( !node )
			return;
		c_0043127f_src *src = *it;
		strcpy( node->field_4, src->field_10.fn_0060f6d0() );
		node->field_68 = src->field_1C;
		node->field_6D = src->field_20;
		node->field_88 = src->field_54;
		memcpy( node->field_95, src->field_8, 6 );
		int mode = src->field_58;
		if( mode == 0 )
			node->field_94 = 1;
		else if( mode == 1 )
			node->field_94 = 0;
		if( field_8CB8 )
		{
			c_0043127f_node *old = fn_004314f5( node->field_68 );
			if( old )
			{
				node->field_6C = old->field_6C;
				node->field_70 = old->field_70;
				node->field_74 = old->field_74;
				node->field_78 = old->field_78;
				node->field_80 = old->field_80;
				node->field_84 = old->field_84;
				node->field_90 = old->field_90;
				node->field_9C = old->field_9C;
				node->field_6E = old->field_6E;
				node->field_7C = old->field_7C;
			}
		}
		it++;
		node->next = 0;
		if( !head )
			head = node;
		else
			tail->next = node;
		tail = node;
	}

	fn_00431200();
	field_8CB8 = (int)head;

	if( g_008f5b84 != field_8D00->fn_vtbl_3C() )
	{
		g_008f5b84 = field_8D00->fn_vtbl_3C();
		gObject_0092deb4->vfunction59( g_008f5b84 );
		g_008e1060.fn_004239ba( g_008f5b84 );
		int value = g_008e1060.fn_004239d6();
		gObject_0092deb4->vfunction10( value, 0, 0 );
	}

	while( list.size() )
	{
		delete list.back();
		list.pop_back();
	}
}

// FUNCTION: STARFLEETOP 0x004314c2
c_0043127f_node::c_0043127f_node()
{
	field_70 = -1;
	field_74 = -1;
	field_88 = 0.0;
	field_6C = 0;
	field_78 = 0;
	field_7C = 0;
	field_80 = 0;
	field_84 = 0;
	field_90 = 0;
	field_6E = 0;
}

// FUNCTION: STARFLEETOP 0x004314f5
c_0043127f_node *tNetControl::fn_004314f5( int id )
{
	// The list is built on first use. The call in the exe goes to 0x00431275, the EH prolog
	// of the function rebuilt whole as fn_0043127f.
	if( !field_8CB8 )
		fn_0043127f();
	c_0043127f_node *node = (c_0043127f_node *)field_8CB8;
	while( node )
	{
		if( node->field_68 == id )
			return node;
		node = node->next;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00431523
c_0043127f_node *tNetControl::fn_00431523( int id )
{
	// As fn_004314f5, but matched on field_70.
	if( !field_8CB8 )
		fn_0043127f();
	c_0043127f_node *node = (c_0043127f_node *)field_8CB8;
	while( node )
	{
		if( node->field_70 == id )
			return node;
		node = node->next;
	}
	return 0;
}

// HYPOTHESIS: rebuilds the field_8CB8 list (through fn_00431571 when g_008f5b3c is 8, through
// fn_0043127f otherwise) and returns it.
// FUNCTION: STARFLEETOP 0x00431551
c_0043127f_node *tNetControl::fn_00431551()
{
	// The fn_0043127f call goes to its EH prolog at 0x00431275, as in fn_004314f5.
	if( g_008f5b3c == 8 )
		fn_00431571();
	else
		fn_0043127f();
	return (c_0043127f_node *)field_8CB8;
}

extern c_0043127f_src_name g_008f5b08;	// HYPOTHESIS: same getter as the entries' name object

// HYPOTHESIS: the g_008f5b3c == 8 counterpart of fn_0043127f: makes the field_8CB8 list a
// single node for the local id g_008f5b84, unless its head already has that id.
// FUNCTION: STARFLEETOP 0x00431571
void tNetControl::fn_00431571()
{
	c_0043127f_node *head = (c_0043127f_node *)field_8CB8;
	if( head && head->field_68 == g_008f5b84 )
		return;

	fn_00431200();
	c_0043127f_node *node = new c_0043127f_node;
	if( !node )
		return;
	strcpy( node->field_4, g_008f5b08.fn_0060f6d0() );
	node->field_68 = g_008f5b84;
	node->next = 0;
	node->field_88 = 0.0;
	node->field_6D = 1;
	node->field_94 = 1;
	field_8CB8 = (int)node;
}

// HYPOTHESIS: a one-byte object at 0x008f5abb whose constructor at 0x0040ce21 takes the
// string "old times"; its class and purpose are unknown.
class c_0040ce21
{
public:
	c_0040ce21( const char *s );
	~c_0040ce21();
};

// HYPOTHESIS: the settings object pointed to by 0x008cc580; slot 0x18 reads a flag and slot
// 0x20 a string (section, key, default, buffer, buffer size), as the uses here suggest.
class c_008cc580
{
public:
	virtual void fn_vtbl_00();
	virtual void fn_vtbl_04();
	virtual void fn_vtbl_08();
	virtual void fn_vtbl_0C();
	virtual void fn_vtbl_10();
	virtual void fn_vtbl_14();
	virtual char fn_vtbl_18( const char *section, const char *key, int def );
	virtual void fn_vtbl_1C();
	virtual void fn_vtbl_20( const char *section, const char *key, const char *def, char *buffer,
		int size );
};

extern c_008cc580 *g_008cc580;
extern c_0043127f_src_name g_008f5b14;	// HYPOTHESIS: same class as g_008f5b08
extern char g_008f5b90;
c_tNetControl_8D00 *fn_0060f880( int n );

// HYPOTHESIS: reads the player name, the session type and the network debug flag from the
// "Network" settings, refreshes state through fn_00432f5a, makes field_8D00 if there is none,
// and calls its slot 0x64.
// FUNCTION: STARFLEETOP 0x004315ee
void tNetControl::fn_004315ee()
{
	static c_0040ce21 s_0040ce21( "old times" );
	char buffer[ 260 ];

	g_008cc580->fn_vtbl_20( "Network", "PlayerName", "Player1", buffer, sizeof( buffer ) - 1 );
	g_008f5b08.fn_0060f810( buffer );
	g_008cc580->fn_vtbl_20( "Network", "SessionType", "Starfleet", buffer, sizeof( buffer ) - 1 );
	g_008f5b14.fn_0060f810( buffer );
	fn_00432f5a();
	g_008f5b90 = g_008cc580->fn_vtbl_18( "Network", "NetDebug", 0 );
	if( !field_8D00 )
		field_8D00 = fn_0060f880( 1 );
	field_8D00->fn_vtbl_64();
}

// HYPOTHESIS: a global object at 0x0090b670 whose class is unknown; only the method called here.
class c_0090b670
{
public:
	void fn_004584ad();
};

extern c_0090b670 g_0090b670;

// HYPOTHESIS: the shutdown counterpart of fn_004315ee: notifies g_0090b670, runs fn_00431821
// when fn_0043268d says so, then calls field_8D00's slot 0x68 and forgets it.
// FUNCTION: STARFLEETOP 0x004316cf
void tNetControl::fn_004316cf()
{
	g_0090b670.fn_004584ad();
	if( fn_0043268d() )
		fn_00431821();
	if( field_8D00 )
	{
		field_8D00->fn_vtbl_68();
		field_8D00 = 0;
	}
}

extern int g_008fe934;
extern int g_008f5b80;	// HYPOTHESIS: a set of flags; bit 2 is tested here, the whole word read by fn_004317bd
extern char g_008f5b88;
extern int g_0083eca4;	// HYPOTHESIS: the time of the last refresh, negative until the first
extern double g_00c7c148;	// HYPOTHESIS: the current time

// HYPOTHESIS: a periodic update: unless flag 4 of g_008f5b80 or g_008f5b88 is set it only
// calls fn_00431ee0; otherwise it calls field_8D00's slot 0x6c, and every 50000 time units
// (or when fn_00432597 asks) runs fn_004321fd for id 1, or for ids 1 to 6 unless
// fn_004326a4 returns 8, before calling fn_00431ee0.
// FUNCTION: STARFLEETOP 0x00431708
void tNetControl::fn_00431708()
{
	g_008fe934 = 0;
	if( !( g_008f5b80 & 4 ) && !g_008f5b88 )
	{
		fn_00431ee0();
		return;
	}

	field_8D00->fn_vtbl_6C( 1 );
	if( g_0083eca4 < 0 )
		g_0083eca4 = (int)g_00c7c148;
	// HYPOTHESIS: the original holds the current time in a local for this test.
	double now = g_00c7c148;
	if( g_0083eca4 + 50000 < now || fn_00432597() )
	{
		if( fn_004326a4() != 8 )
		{
			for( int i = 1; i < 7; i++ )
				fn_004321fd( i );
		}
		else
			fn_004321fd( 1 );
		g_0083eca4 = (int)g_00c7c148;
	}
	fn_00431ee0();
}

// HYPOTHESIS: returns the flag word g_008f5b80 tested by fn_00431708; this is not used.
// FUNCTION: STARFLEETOP 0x004317bd
int tNetControl::fn_004317bd()
{
	return g_008f5b80;
}

// FUNCTION: STARFLEETOP 0x004317c3
void tNetControl::fn_004317c3()
{
	field_8D00->fn_vtbl_40();
}

// FUNCTION: STARFLEETOP 0x004317ce
void tNetControl::fn_004317ce( int a, int b )
{
	if( field_8D00 )
		field_8D00->fn_vtbl_98( a, b );
}

// FUNCTION: STARFLEETOP 0x004317eb
void tNetControl::fn_004317eb( int a, int b )
{
	if( field_8D00 )
		field_8D00->fn_vtbl_9C( a, b );
}

// FUNCTION: STARFLEETOP 0x00431808
void tNetControl::fn_00431808( int a )
{
	if( field_8D00 )
		field_8D00->fn_vtbl_A0( a );
}
