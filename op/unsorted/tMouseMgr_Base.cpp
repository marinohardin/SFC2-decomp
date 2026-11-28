// tMouseMgr_Base: base class of the game's mouse managers.
// Original source file unknown; functions grouped by class (op/UNITS.tsv).

class tMouseMgr_Base {
public:
    tMouseMgr_Base();
    virtual ~tMouseMgr_Base();
    tMouseMgr_Base *scalar_deleting_destructor(unsigned char flags);
};

// FUNCTION: STARFLEETOP 0x0040bd69
inline tMouseMgr_Base::~tMouseMgr_Base()
{
}

// FUNCTION: STARFLEETOP 0x0040bd70
// MATCH-ONLY: the scalar deleting destructor is compiler-generated and has no
// source line; it is written out as a member. The original inlines the empty
// destructor here, so the destructor is inline (HYPOTHESIS: defined in the class).
tMouseMgr_Base *tMouseMgr_Base::scalar_deleting_destructor(unsigned char flags)
{ tMouseMgr_Base::~tMouseMgr_Base(); if (flags & 1) ::operator delete(this); return this; }

// tTimerResponse: the callback interface twClock::mSetTimer takes (SDK
// symbol). Its first virtual is what a timer calls when it fires.
#include "Tiger/tTimerResponse.h"
#include "Tiger/tatomic.h"








class tMouseMgr_Ex : public tMouseMgr_Base {
public:
    // tMouseMgr_Ex::tTimer (RTTI): the mouse manager's timer response, which
    // forwards the timer event to the manager that owns it.
    class tTimer : public tTimerResponse {
    public:
        void fn_0040bd8d();
        tMouseMgr_Ex *m_owner; // +8
    };

    ~tMouseMgr_Ex();
    tMouseMgr_Ex *scalar_deleting_destructor(unsigned char flags);
    // The SDK's symbols: mDone is virtual, mAsync protected (the timer is a friend).
    virtual void mDone();
protected:
    friend class tTimer;
    void mAsync();
public:
    unsigned char m_4[0x10]; // HYPOTHESIS: fields +4..+0x13, not touched here
    tAtomicLock m_lock;      // +0x14
};

// FUNCTION: STARFLEETOP 0x0040bd95
// MATCH-ONLY: the scalar deleting destructor is compiler-generated and has no
// source line, so the harness cannot claim it directly; it is written out as a
// member that calls the destructor at 0x0040bdb1, then deletes when bit 0
// of the flag is set.
tMouseMgr_Ex *tMouseMgr_Ex::scalar_deleting_destructor(unsigned char flags)
{
    tMouseMgr_Ex::~tMouseMgr_Ex();
    if (flags & 1)
        ::operator delete(this);
    return this;
}

// The destructor calls mDone, then destroys the tAtomicLock member at +0x14; the original
// inlines ~tMouseMgr_Base here.
#include "Tiger/tatomic.h"
// FUNCTION: STARFLEETOP 0x0040bdb1
tMouseMgr_Ex::~tMouseMgr_Ex()
{
    mDone();
}
// FUNCTION: STARFLEETOP 0x0040bd8d
void tMouseMgr_Ex::tTimer::fn_0040bd8d()
{
    m_owner->mAsync();
}
