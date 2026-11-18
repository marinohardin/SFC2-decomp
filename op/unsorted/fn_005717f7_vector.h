//	fn_005717f7_vector.h: the vector of pointers whose destructor is 0x00571a7a.
// Evidence:  calls (original name and file unknown)

#ifndef fn_005717f7_vector_h
#define fn_005717f7_vector_h 1

#include <vector>

// HYPOTHESIS: this compiler's std::vector of pointers (an allocator byte, then first, last and
// end-of-storage pointers). Identical-COMDAT folding leaves one destructor, 0x00571a7a, for every
// such vector; it is claimed as this type's (unsorted/fn_00571000.cpp), and the vectors that are
// destroyed through it are declared as this type: the database's per-type index
// (unsorted/fn_00571000.cpp) and tMetaMapControl's image vectors (tMetaMapControl.h).
struct fn_005717f7_vector {
	// MATCH-ONLY: the library vector's default constructor, inline.
	explicit fn_005717f7_vector(const std::allocator<void*>& al = std::allocator<void*>())
		: m_allocator(al), m_first(0), m_last(0), m_end(0) {}
	~fn_005717f7_vector();
	std::allocator<void*> m_allocator;
	void** m_first;
	void** m_last;
	void** m_end;
};

#endif // fn_005717f7_vector_h
