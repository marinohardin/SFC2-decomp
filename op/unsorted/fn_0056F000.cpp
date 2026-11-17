// Unit:      unsorted/fn_0056F000.cpp
// Functions: 0x0056f041 to 0x0056ffe3, 37 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   game-release (harness/units.json)

class tServCharacter;

// HYPOTHESIS: the member destroyed by 0x00571a7a is a VC6 vector of 32-bit values, the same
// destructor tPanelMgr.cpp gives its vectors (declared only there, as fn_00571a69_vector); this
// declaration carries only the destructor until that type moves to a shared header.
struct fn_00571a7a_vector {
	~fn_00571a7a_vector();
	char m_allocator;
	void **m_first;
	void **m_last;
	void **m_end;
};

// HYPOTHESIS: the function sets the vtable of nDatabase::tQuery<tServCharacter> and destroys a
// member at +0x4 through 0x00571a7a; it is taken to be that class's destructor.
namespace nDatabase {
	template <class T> class tQuery {
	public:
		virtual ~tQuery();
		fn_00571a7a_vector m_results;
	};
}

// FUNCTION: STARFLEETOP 0x0056f041
nDatabase::tQuery<tServCharacter>::~tQuery()
{
}
