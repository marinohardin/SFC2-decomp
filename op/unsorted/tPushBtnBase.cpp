// tPushBtnBase.cpp

#include "unsorted/tPushBtnBase.h"
#include "unsorted/tValueBase.h"

// HYPOTHESIS: base class tControl methods (tControl sits between tVisObject and tButton in the
// original hierarchy; tValueBtn.h declares tButton directly on tVisObject for now).
class tControl : public tVisObject
{
public:
	void fn_00685ee0( tPoint pt );
	void fn_00685f10( tPoint pt );
	void fn_00685f40();
	void fn_00685f80();
};

// HYPOTHESIS: 0x0067f830 is a free cdecl function called when the string at +0xe4 contains '.'.
void fn_0067f830( tVisObject *object, const char *name, int a, int b, int c );

// HYPOTHESIS: the global object at 0x00c690d0; 0x00674250 takes the id at +0x10c.
class c_tPushBtnBase_00674250_Owner
{
public:
	void fn_00674250( int param_1 );
};
extern c_tPushBtnBase_00674250_Owner *g_00c690d0;

// HYPOTHESIS: RTTI type descriptor 0x00854a98 (tUIActionBase, derived from tNotifier).
class tUIActionBase : public tNotifier
{
public:
	virtual ~tUIActionBase();
};

// HYPOTHESIS: tNotifier has slot 1 (not yet declared in shared tValueBase.h), called with
// (this + 0x110, 1). Declared via a local subclass to avoid editing the shared header.
class c_tPushBtnBase_Notifier : public tNotifier
{
public:
	virtual void vfunction2( void *sender, int code );
};

// FUNCTION: STARFLEETOP 0x00686640
void tPushBtnBase::vfunction43( tPoint pt )
{
	( (tMachine *)&field_74 )->fn_0067f090( (tVisGroup *)this, 2, 0 );

	tString *str = (tString *)( (unsigned char *)this + 0xe4 );
	unsigned int dot = (unsigned int)str->find( '.' );
	if ( dot < (unsigned int)str->mStrLen() )
	{
		fn_0067f830( this, str->mCharStr(), 0, 0, 0 );
	}
	else
	{
		int id = *(int *)( (unsigned char *)this + 0x10c );
		if ( id != 0 && g_00c690d0 != 0 )
		{
			g_00c690d0->fn_00674250( id );
		}
	}

	tNotifier *action = *(tNotifier **)( (unsigned char *)this + 0x114 );
	if ( dynamic_cast< tUIActionBase * >( action ) == 0 )
	{
		unsigned char *sub = (unsigned char *)this + 0x110;
		tNotifier *target = *(tNotifier **)( sub + 4 );
		if ( target != 0 )
		{
			( (c_tPushBtnBase_Notifier *)target )->vfunction2( sub, 1 );
		}
	}
}

// FUNCTION: STARFLEETOP 0x006866f0
void tPushBtnBase::vfunction44( tPoint pt )
{
	*(int *)( (unsigned char *)this + 0x70 ) = 1;
	( (tControl *)(tVisObject *)this )->fn_00685ee0( pt );
}

// FUNCTION: STARFLEETOP 0x00686720
void tPushBtnBase::vfunction45( tPoint pt )
{
	*(int *)( (unsigned char *)this + 0x70 ) = 0;
	( (tControl *)(tVisObject *)this )->fn_00685f10( pt );
}

