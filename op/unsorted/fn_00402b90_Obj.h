//	fn_00402b90_Obj.h: the facet-like object whose reference count 0x00402b90 increments.
// Evidence:  calls (original name and file unknown)

#ifndef fn_00402b90_Obj_h
#define fn_00402b90_Obj_h 1

class fn_00406887_Base;

// HYPOTHESIS: a facet-like object of the library; m4 is its reference count. 0x00402b90
// (defined in fn_00402000.cpp) increments it; 0x00402b3a returns its base.
class fn_00402b90_Obj
{
public:
    // MATCH-ONLY: fn_00402b90_NOTHROW picks the exception specification. 0x00404184 (in
    // fn_00404000.cpp, which defines it) matches only with a no-throw call; 0x004066a3,
    // 0x004066f1, 0x0040673f and 0x0040678d (fn_00406000.cpp) match only without it.
#ifdef fn_00402b90_NOTHROW
    void fn_00402b90() throw();
#else
    void fn_00402b90();
#endif
    fn_00406887_Base* fn_00402b3a();
    unsigned long m0;
    unsigned long m4;
};

#endif	// fn_00402b90_Obj_h
