// tSfcMouseMgr: the game's mouse manager base class.
// Original source file unknown; functions grouped by class (op/UNITS.tsv).

// HYPOTHESIS: the class has a virtual destructor; only its vtable pointer is known.
class tSfcMouseMgr {
public:
    virtual ~tSfcMouseMgr();
};

// FUNCTION: STARFLEETOP 0x0040bdfa
// FUNCTION: STARFLEETOP 0x0040bdf3
tSfcMouseMgr::~tSfcMouseMgr()
{
}
