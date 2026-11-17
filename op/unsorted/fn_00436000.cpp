// Unit:      unsorted/fn_00436000.cpp
// Functions: 0x00436006 to 0x004364b6, 37 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   game-release (harness/units.json)

class tPath;
class twPath;

namespace std
{
	template <class _Ty> struct less;
}

// HYPOTHESIS: the full tCountMap is declared only inside op/Tiger/tgfsys.cpp; this unit needs
// just the virtual destructor, whose deleting form is the function below.
template <class tKeyType, class tValueType, class tCompare>
class tCountMap
{
public:
	virtual ~tCountMap();
};

// HYPOTHESIS: the destructor body (0x00435e84) is not recovered; empty here.
// FUNCTION: STARFLEETOP 0x004361cb
template <>
tCountMap<tPath,twPath,std::less<tPath> >::~tCountMap()
{
}
