//	fn_0040ce21_type.h: the marker object built from a name string by 0x0040ce21.
// Evidence:  calls (original name and file unknown)

#ifndef fn_0040ce21_type_h
#define fn_0040ce21_type_h 1

// HYPOTHESIS: the marker object built from a name string by 0x0040ce21. The same type is
// declared inside game/Source/entity/entity_physical.cpp, game/unsorted/tCloakSystem.cpp and
// game/unsorted/tNetControl.cpp; those units should include this header instead.
class fn_0040ce21_type
{
public:
    fn_0040ce21_type(const char* name);
    ~fn_0040ce21_type() {}
};

#endif // fn_0040ce21_type_h
