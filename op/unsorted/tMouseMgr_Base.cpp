// tMouseMgr_Base: base class of the game's mouse managers.
// Original source file unknown; functions grouped by class (game/UNITS.tsv).

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
// HYPOTHESIS: slot 0 takes no arguments and returns nothing; its name is not
// known, so it is named after this override.
class tTimerResponse {
public:
    virtual void fn_0040bd8d() = 0;
    long m_4; // HYPOTHESIS: a field of the base sits at +4
};

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
    void fn_006a35e0();
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

// HYPOTHESIS: the code at 0x0040bdb1 (its exception-frame setup and body) is rebuilt as
// fn_0040bdbb::~fn_0040bdbb, which claims 0x0040bdb1. This empty definition is not the original; it stays
// until that code moves here, because a caller or a vtable needs it.
tMouseMgr_Ex::~tMouseMgr_Ex()
{
}

// MATCH-ONLY: placeholder, not original code. 0x0040bdbb is not a function of
// its own: it is the body of tMouseMgr_Ex::~tMouseMgr_Ex, which runs on from
// the __EH_prolog call at 0x0040bdb1 (the destructor calls fn_006a3350, then
// destroys a tAtomicLock member at +0x14, with ~tMouseMgr_Base inlined). It
// cannot be compiled separately; remove this placeholder once the analysis
// treats 0x0040bdb1..0x0040bdf2 as one function. The placeholder repeats that
// destructor's body on a stand-in class of the same layout (named after the
// address; it is not an original class), so the scored range lines up with
// the original body after the EH prolog. The original inlines
// ~tMouseMgr_Base here; the stand-in calls it.
class tLock_0040bdbb {
public:
    tLock_0040bdbb();
    virtual ~tLock_0040bdbb();
    unsigned char fReserved[28];
    unsigned long fLock;
};

class fn_0040bdbb : public tMouseMgr_Base {
public:
    ~fn_0040bdbb();
    void fn_006a3350();
    unsigned char m_4[0x10]; // HYPOTHESIS: fields +4..+0x13, not touched here
    tLock_0040bdbb m_lock;   // +0x14
};

// FUNCTION: STARFLEETOP 0x0040bdb1
fn_0040bdbb::~fn_0040bdbb()
{
    fn_006a3350();
}

// FUNCTION: STARFLEETOP 0x0040bd8d
void tMouseMgr_Ex::tTimer::fn_0040bd8d()
{
    m_owner->fn_006a35e0();
}
