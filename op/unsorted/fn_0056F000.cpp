// Unit:      unsorted/fn_0056F000.cpp
// Functions: 0x0056f041 to 0x0056ffe3, 37 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   game-release (harness/units.json)

// HYPOTHESIS: the function sets the vtable of nDatabase::tQuery<tServCharacter> and calls the
// shared member destructor at 0x00571a7a; it is taken to be that class's destructor.
class tServCharacter;

namespace nDatabase {
	template <class T> class tQuery {
	public:
		virtual ~tQuery();
		void fn_00571a7a();
	};
}

// FUNCTION: STARFLEETOP 0x0056f041
nDatabase::tQuery<tServCharacter>::~tQuery()
{
	fn_00571a7a();
}
