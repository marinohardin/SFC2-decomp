// Unit:      unsorted/fn_004D3143.cpp
// Functions: 0x004d3143 to 0x004d3b8d, 61 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)

// HYPOTHESIS: the global at 0x0092e4a0 is an object whose method 0x004d482a takes four
// longs; its class is not recovered yet.
class fn_004d3ef7_object;

class fn_004d482a_owner
{
public:
	long fn_004d482a( long arg1, long arg2, long arg3, long arg4 );
	fn_004d3ef7_object* fn_004d3ef7( long arg1 );
	fn_004d3ef7_object* fn_004d4a02( long arg1, long arg2, long arg3, long arg4 );
	fn_004d3ef7_object* fn_004d42e6( long arg1, long arg2, long arg3, long arg4 );
	fn_004d3ef7_object* fn_004d40b1( long arg1, long arg2, long arg3, long arg4, long arg5, long arg6 );
	fn_004d3ef7_object* fn_004d4728( long arg1, long arg2 );
	long fn_004d4877( long arg1, long arg2 );
	void fn_004d48e1( long arg1, long arg2, long arg3 );
	void fn_004d4a9e( long arg1 );
	void fn_004d3f45( long arg1, long arg2, long arg3 );
	void fn_004d459c( long arg1 );
	void fn_004d45e6( long arg1 );
	void fn_004d4630( long arg1 );
	void fn_004d4687( long arg1 );
	void fn_004d46e3( long arg1 );
	void fn_004d4712( long arg1 );
	void fn_004d4996( long arg1, long arg2, long arg3 );
	void fn_004d449a( long arg1, long arg2 );
};

extern fn_004d482a_owner g_0092e4a0;

// FUNCTION: STARFLEETOP 0x004d315e
long fn_004d315e( long arg1, long arg2, long arg3, long arg4 )
{
	return g_0092e4a0.fn_004d482a( arg1, arg2, arg3, arg4 );
}

// HYPOTHESIS: the global at 0x009254f8 is the object whose method 0x004bf6b9 takes two longs;
// its class is declared only inside fn_004D2000.cpp (fn_004cb4a7_target).
class fn_004d3ef7_object;

class fn_004bf6b9_owner
{
public:
	void fn_004bf6b9( long arg1, long arg2 );
	void fn_004bf6ea( long arg1 );
	void fn_004bf74f( long arg1, float arg2 );
	void fn_004caf1b( fn_004d3ef7_object* arg1 );
	void fn_004caf40( fn_004d3ef7_object* arg1, long arg2 );
	void fn_004cb0ab( fn_004d3ef7_object* arg1 );
	long fn_004cb602( long arg1 );
	void fn_004cb56f( long arg1, long arg2, long arg3 );
	void fn_004cb5bd( long arg1 );
	void fn_004cb12a();
	void fn_004cb4a7( long arg1, long arg2 );
	class fn_004d35bf_object* fn_004cb4f8( long arg1 );
};

extern fn_004bf6b9_owner g_009254f8;

// FUNCTION: STARFLEETOP 0x004d3179
void fn_004d3179( long arg1, long arg2 )
{
	g_009254f8.fn_004bf6b9( arg1, arg2 );
}

// FUNCTION: STARFLEETOP 0x004d318c
void fn_004d318c( long arg1 )
{
	g_009254f8.fn_004bf6ea( arg1 );
}

// FUNCTION: STARFLEETOP 0x004d319b
void fn_004d319b( long arg1, float arg2 )
{
	g_009254f8.fn_004bf74f( arg1, arg2 );
}

// HYPOTHESIS: method 0x004d3ef7 of the global at 0x0092e4a0 looks up an object from a
// handle; 0x004caf1b and 0x004caf40 are methods on that object (class not recovered yet).
class fn_004d3ef7_object;


// FUNCTION: STARFLEETOP 0x004d31b2
bool fn_004d31b2( long arg1, long arg2 )
{
	fn_004d3ef7_object* obj = g_0092e4a0.fn_004d3ef7( arg1 );
	bool ok = false;
	if ( obj != 0 )
	{
		if ( arg2 != 0 )
		{
			g_009254f8.fn_004caf40( obj, arg2 );
		}
		else
		{
			g_009254f8.fn_004caf1b( obj );
		}
		ok = true;
	}
	return ok;
}

// FUNCTION: STARFLEETOP 0x004d31ed
bool fn_004d31ed( long arg1, long arg2, long arg3, long arg4, long arg5 )
{
	fn_004d3ef7_object* obj = g_0092e4a0.fn_004d4a02( arg1, arg2, arg4, arg5 );
	bool ok = false;
	if ( obj != 0 )
	{
		if ( arg3 != 0 )
		{
			g_009254f8.fn_004caf40( obj, arg3 );
		}
		else
		{
			g_009254f8.fn_004caf1b( obj );
		}
		ok = true;
	}
	return ok;
}

// FUNCTION: STARFLEETOP 0x004d3232
fn_004d3ef7_object* fn_004d3232( long arg1, long arg2, long arg3, long arg4, long arg5 )
{
	fn_004d3ef7_object* obj = g_0092e4a0.fn_004d42e6( arg1, arg2, arg3, arg4 );
	if ( obj != 0 )
	{
		if ( arg5 != 0 )
		{
			g_009254f8.fn_004caf40( obj, arg5 );
		}
		else
		{
			g_009254f8.fn_004caf1b( obj );
		}
	}
	return obj;
}

// FUNCTION: STARFLEETOP 0x004d3278
fn_004d3ef7_object* fn_004d3278( long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long arg7 )
{
	fn_004d3ef7_object* obj = g_0092e4a0.fn_004d40b1( arg1, arg2, arg3, arg4, arg6, arg7 );
	if ( obj != 0 )
	{
		if ( arg5 != 0 )
		{
			g_009254f8.fn_004caf40( obj, arg5 );
		}
		else
		{
			g_009254f8.fn_004caf1b( obj );
		}
	}
	return obj;
}

// FUNCTION: STARFLEETOP 0x004d32c4
void fn_004d32c4( long arg1 )
{
	fn_004d3ef7_object* obj = g_0092e4a0.fn_004d4728( arg1, 0 );
	g_009254f8.fn_004cb0ab( obj );
	g_0092e4a0.fn_004d4a9e( arg1 );
}

// FUNCTION: STARFLEETOP 0x004d32ef
void fn_004d32ef( long arg1, long arg2, long arg3 )
{
	g_0092e4a0.fn_004d3f45( arg1, arg2, arg3 );
}

// FUNCTION: STARFLEETOP 0x004d3306
void fn_004d3306( long arg1 )
{
	fn_004d3ef7_object* obj = g_0092e4a0.fn_004d4728( arg1, 0 );
	g_009254f8.fn_004cb0ab( obj );
	g_0092e4a0.fn_004d459c( arg1 );
}

// FUNCTION: STARFLEETOP 0x004d3331
void fn_004d3331( long arg1 )
{
	g_0092e4a0.fn_004d45e6( arg1 );
}

// FUNCTION: STARFLEETOP 0x004d3340
void fn_004d3340( long arg1 )
{
	g_0092e4a0.fn_004d4630( arg1 );
}

// FUNCTION: STARFLEETOP 0x004d334f
void fn_004d334f( long arg1 )
{
	g_0092e4a0.fn_004d4687( arg1 );
}

// FUNCTION: STARFLEETOP 0x004d335e
void fn_004d335e( long arg1 )
{
	g_0092e4a0.fn_004d46e3( arg1 );
}

// FUNCTION: STARFLEETOP 0x004d336d
void fn_004d336d( long arg1 )
{
	long result = g_009254f8.fn_004cb602( arg1 );
	if ( result != 0 )
	{
		g_0092e4a0.fn_004d4712( result );
	}
}

#include "Tiger/tstring.h"

// HYPOTHESIS: the object returned by 0x004cb602 has a virtual method at vtable offset 0x4c
// (slot 19); the other slots are not recovered, so they are placeholders.
class fn_004d338b_object
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual void vfunction12();
	virtual void vfunction13();
	virtual void vfunction14( bool arg1 );
	virtual void vfunction15();
	virtual void vfunction16();
	virtual void vfunction17();
	virtual void vfunction18();
	virtual long vfunction19();
	virtual void vfunction20( long arg1 );
	virtual void vfunction21();
	virtual void vfunction22();
	virtual void vfunction23( tString* arg1 );
	void fn_0067ce90();
};

// FUNCTION: STARFLEETOP 0x004d338b
long fn_004d338b( long arg1 )
{
	fn_004d338b_object* obj = (fn_004d338b_object*) g_009254f8.fn_004cb602( arg1 );
	long result = -1;
	if ( obj != 0 )
	{
		result = obj->vfunction19();
	}
	return result;
}

// FUNCTION: STARFLEETOP 0x004d33a8
long fn_004d33a8( long arg1, long arg2 )
{
	fn_004d338b_object* obj = (fn_004d338b_object*) g_009254f8.fn_004cb602( arg1 );
	long result = -1;
	if ( obj != 0 )
	{
		obj->vfunction20( arg2 );
		result = obj->vfunction19();
	}
	return result;
}

// FUNCTION: STARFLEETOP 0x004d33d4
void fn_004d33d4( long arg1, tString arg2 )
{
	fn_004d338b_object* obj = (fn_004d338b_object*) g_009254f8.fn_004cb602( arg1 );
	if ( obj != 0 )
	{
		obj->vfunction23( &arg2 );
	}
}

// FUNCTION: STARFLEETOP 0x004d3416
void fn_004d3416( long arg1 )
{
	fn_004d33a8( arg1, fn_004d338b( arg1 ) + 1 );
}

// FUNCTION: STARFLEETOP 0x004d342e
void fn_004d342e( long arg1 )
{
	fn_004d33a8( arg1, fn_004d338b( arg1 ) - 1 );
}

// FUNCTION: STARFLEETOP 0x004d3446
long fn_004d3446( long arg1 )
{
	return g_009254f8.fn_004cb602( arg1 );
}

// FUNCTION: STARFLEETOP 0x004d3455
void fn_004d3455( long arg1 )
{
	fn_004d338b_object* obj = (fn_004d338b_object*) g_009254f8.fn_004cb602( arg1 );
	if ( obj != 0 )
	{
		obj->vfunction17();
	}
}

// FUNCTION: STARFLEETOP 0x004d346f
void fn_004d346f( long arg1, long arg2 )
{
	fn_004d338b_object* obj = (fn_004d338b_object*) g_0092e4a0.fn_004d4728( arg2, arg1 );
	if ( obj != 0 )
	{
		obj->vfunction17();
	}
}

// FUNCTION: STARFLEETOP 0x004d348d
void fn_004d348d( long arg1 )
{
	fn_004d338b_object* obj = (fn_004d338b_object*) g_009254f8.fn_004cb602( arg1 );
	if ( obj != 0 )
	{
		obj->fn_0067ce90();
	}
}

// FUNCTION: STARFLEETOP 0x004d34a7
void fn_004d34a7( long arg1, long arg2 )
{
	fn_004d338b_object* obj = (fn_004d338b_object*) g_0092e4a0.fn_004d4728( arg2, arg1 );
	if ( obj != 0 )
	{
		obj->fn_0067ce90();
	}
}

// FUNCTION: STARFLEETOP 0x004d34c5
void fn_004d34c5( long arg1, char arg2 )
{
	fn_004d338b_object* obj = (fn_004d338b_object*) g_009254f8.fn_004cb602( arg1 );
	if ( obj != 0 )
	{
		obj->vfunction14( arg2 != 0 );
	}
}

// FUNCTION: STARFLEETOP 0x004d34e8
void fn_004d34e8( long arg1, long arg2, char arg3 )
{
	fn_004d338b_object* obj = (fn_004d338b_object*) g_0092e4a0.fn_004d4728( arg2, arg1 );
	if ( obj != 0 )
	{
		obj->vfunction14( arg3 != 0 );
	}
}

// FUNCTION: STARFLEETOP 0x004d350f
void fn_004d350f( long arg1, long arg2 )
{
	fn_004d338b_object* obj = (fn_004d338b_object*) g_009254f8.fn_004cb602( arg1 );
	if ( obj != 0 )
	{
		if ( arg2 != obj->vfunction19() )
		{
			obj->vfunction20( arg2 );
		}
	}
}

// FUNCTION: STARFLEETOP 0x004d353e
void fn_004d353e( long arg1, long arg2 )
{
	g_0092e4a0.fn_004d4728( arg1, arg2 );
}

// FUNCTION: STARFLEETOP 0x004d3551
long fn_004d3551( long arg1, long arg2 )
{
	return g_0092e4a0.fn_004d4877( arg1, arg2 );
}

// FUNCTION: STARFLEETOP 0x004d3564
void fn_004d3564( long arg1, long arg2, long arg3 )
{
	g_0092e4a0.fn_004d48e1( arg1, arg2, arg3 );
}

// FUNCTION: STARFLEETOP 0x004d357b
void fn_004d357b( long arg1, long arg2, long arg3 )
{
	g_0092e4a0.fn_004d4996( arg1, arg2, arg3 );
}

void fn_00671d80( long arg1, long arg2 );

// FUNCTION: STARFLEETOP 0x004d3592
void fn_004d3592( long arg1, long arg2 )
{
	fn_00671d80( arg1, arg2 );
}

// FUNCTION: STARFLEETOP 0x004d35a2
void fn_004d35a2()
{
	g_009254f8.fn_004cb12a();
}

// FUNCTION: STARFLEETOP 0x004d35ac
void fn_004d35ac( long arg1, long arg2 )
{
	g_009254f8.fn_004cb4a7( arg1, arg2 );
}

// HYPOTHESIS: the object found by 0x004cb4f8 is deleted through its virtual destructor
// (slot 0 called with 1); the class is not recovered yet.
class fn_004d35bf_object
{
public:
	virtual ~fn_004d35bf_object();
};

// FUNCTION: STARFLEETOP 0x004d35bf
fn_004d35bf_object* fn_004d35bf( long arg1, char arg2 )
{
	fn_004d35bf_object* obj = g_009254f8.fn_004cb4f8( arg1 );
	if ( arg2 != 0 && obj != 0 )
	{
		delete obj;
		obj = 0;
	}
	return obj;
}

// FUNCTION: STARFLEETOP 0x004d35e3
void fn_004d35e3( long arg1, long arg2, long arg3 )
{
	g_009254f8.fn_004cb56f( arg1, arg2, arg3 );
}

// FUNCTION: STARFLEETOP 0x004d35fa
void fn_004d35fa( long arg1 )
{
	g_009254f8.fn_004cb5bd( arg1 );
}

// HYPOTHESIS: arg1 points to a record with two longs at +0xc/+0x10 and a name string at +0x138;
// its class is not recovered yet.
class fn_004d3609_record
{
public:
	char pad_00[0xc];
	long m0c;
	long m10;
	char pad_14[0x138 - 0x14];
	char m138[1];
};

void fn_007364c0( long arg1, long arg2 );
void fn_007364d0( long arg1, char* arg2 );
void fn_004d3652( long arg1, long arg2, long arg3, long arg4 );

// FUNCTION: STARFLEETOP 0x004d3609
void fn_004d3609( fn_004d3609_record* arg1, long arg2, long arg3 )
{
	fn_007364c0( arg3, arg2 );
	if ( arg1 != 0 )
	{
		char* name = arg1->m138;
		if ( name != 0 && *name != 0 )
		{
			fn_007364d0( arg3, name );
			return;
		}
		fn_004d3652( arg1->m0c, arg1->m10, arg2, arg3 );
	}
}

extern char g_0083d5b8[];
extern char g_0083d508[];
extern char g_0083d4f8[];
extern char g_00855730[];
extern char g_0083d3e8[];
extern char g_0083d3ec[];
extern char g_0085572c[];
extern char g_00855728[];
extern char g_00855724[];
extern char g_00855720[];
extern char g_0085571c[];
extern char g_00855718[];
extern char g_0083d3f0[];
extern char g_0083d4fc[];
extern char g_00855714[];
extern char g_0083a70c[];
extern char g_00855710[];
extern char* g_0083d2a0[];

// FUNCTION: STARFLEETOP 0x004d3652
void fn_004d3652( long arg1, long arg2, long arg3, long arg4 )
{
	fn_007364c0( arg4, arg3 );
	// HYPOTHESIS: the range guard reproduces the original's extra bound test before the switch table.
	if ( arg1 < 25 )
	switch ( arg1 )
	{
	case 0:
		fn_007364d0( arg4, g_0083d5b8 );
		break;
	case 1:
		fn_007364d0( arg4, g_0083d508 );
		break;
	case 2:
		fn_007364d0( arg4, g_0083d4f8 );
		break;
	case 3:
		fn_007364d0( arg4, g_00855730 );
		break;
	case 4:
		fn_007364d0( arg4, g_0083d3e8 );
		break;
	case 5:
		fn_007364d0( arg4, g_0083d3ec );
		break;
	case 6:
		fn_007364d0( arg4, g_0085572c );
		break;
	case 7:
		fn_007364d0( arg4, g_00855728 );
		break;
	case 8:
		fn_007364d0( arg4, g_00855724 );
		break;
	case 9:
		fn_007364d0( arg4, g_00855720 );
		break;
	case 10:
		fn_007364d0( arg4, g_0085571c );
		break;
	case 11:
		fn_007364d0( arg4, g_00855718 );
		break;
	case 12:
		fn_007364d0( arg4, g_0083d3f0 );
		break;
	case 13:
		fn_007364d0( arg4, g_0083d4fc );
		break;
	case 14:
		fn_007364d0( arg4, g_00855714 );
		break;
	case 15:
		fn_007364d0( arg4, g_0083a70c );
		break;
	case 16:
		fn_007364d0( arg4, g_00855710 );
		break;
	}
	if ( arg2 < 0x2c )
	{
		fn_007364d0( arg4, g_0083d2a0[arg2] );
	}
}

// HYPOTHESIS: tVarButtonGroup is not declared in a header yet; only the call below is known.
class tVarButtonGroup
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual bool vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual void vfunction12();
	void fn_004b54ba();
	void vfunction42_for_tVisGroup();
};

// FUNCTION: STARFLEETOP 0x004d375b
void fn_004d375b( tVarButtonGroup* arg1 )
{
	if ( arg1 != 0 )
	{
		arg1->fn_004b54ba();
		arg1->vfunction42_for_tVisGroup();
	}
}

// FUNCTION: STARFLEETOP 0x004d3774
void fn_004d3774( long arg1 )
{
	fn_004d375b( (tVarButtonGroup*) fn_004d3446( arg1 ) );
}

// FUNCTION: STARFLEETOP 0x004d3786
void fn_004d3786( tVarButtonGroup* arg1 )
{
	if ( arg1 != 0 )
	{
		arg1->vfunction12();
		arg1->vfunction42_for_tVisGroup();
	}
}

// FUNCTION: STARFLEETOP 0x004d379f
void fn_004d379f( long arg1 )
{
	fn_004d3786( (tVarButtonGroup*) fn_004d3446( arg1 ) );
}

// FUNCTION: STARFLEETOP 0x004d37b1
bool fn_004d37b1( tVarButtonGroup* arg1 )
{
	if ( arg1 != 0 )
	{
		return arg1->vfunction9();
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x004d37c1
bool fn_004d37c1( long arg1 )
{
	return fn_004d37b1( (tVarButtonGroup*) fn_004d3446( arg1 ) );
}

// FUNCTION: STARFLEETOP 0x004d37d3
void fn_004d37d3( long arg1, long arg2 )
{
	g_0092e4a0.fn_004d449a( arg1, arg2 );
}

#include <windows.h>
#include <string.h>

// FUNCTION: STARFLEETOP 0x004d37e6
char fn_004d37e6( char* arg1, unsigned int arg2 )
{
	char result = 0;
	if ( OpenClipboard( 0 ) )
	{
		HANDLE hMem = GetClipboardData( CF_TEXT );
		if ( hMem )
		{
			char* text = (char*) GlobalLock( hMem );
			if ( text )
			{
				if ( strlen( text ) > arg2 )
				{
					return 0;
				}
				fn_007364c0( (long) arg1, (long) text );
				GlobalUnlock( hMem );
				result = 1;
			}
		}
		CloseClipboard();
		return result;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x004d3843
void fn_004d3843( char* arg1 )
{
	if ( arg1 )
	{
		HGLOBAL hMem = GlobalAlloc( GMEM_DDESHARE, strlen( arg1 ) );
		if ( hMem )
		{
			char* text = (char*) GlobalLock( hMem );
			if ( text )
			{
				fn_007364c0( (long) text, (long) arg1 );
				GlobalUnlock( hMem );
				if ( OpenClipboard( 0 ) )
				{
					EmptyClipboard();
					SetClipboardData( CF_TEXT, hMem );
					CloseClipboard();
				}
			}
		}
	}
}

// HYPOTHESIS: a list-like container; +4 head node, +8 second buffer, +0x10 count.
struct fn_004d38a5_list
{
	// HYPOTHESIS: a by-value iterator holding one node pointer.
	struct iterator
	{
		iterator() {}
		iterator( int** p ) : mPtr( p ) {}
		int** mPtr;
	};
	int m0;
	int** m4;
	void* m8;
	int mc;
	int m10;
	void fn_004d38a5();
	iterator fn_004d38dd( iterator first, iterator last );
};

// FUNCTION: STARFLEETOP 0x004d38a5
void fn_004d38a5_list::fn_004d38a5()
{
	fn_004d38dd( iterator( (int**) *m4 ), iterator( m4 ) );
	::operator delete( m4 );
	m4 = 0;
	m10 = 0;
	::operator delete( m8 );
	m8 = 0;
}
