// Unit:      unsorted/tMatchedMission.cpp
// Functions: 0x0053c617 to 0x0053c9b2, 4 in op/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   game-release (harness/units.json)

// HYPOTHESIS: layout and members unknown; the destructor body is not recovered
// (about 14 unwind states at 0x0053c7ae). tEntityMgr.cpp holds a reduced copy of
// this class that a shared header should replace.
class tMatchedMission {
public:
    virtual ~tMatchedMission();
};

// FUNCTION: STARFLEETOP 0x00427382
tMatchedMission::~tMatchedMission()
{
}
