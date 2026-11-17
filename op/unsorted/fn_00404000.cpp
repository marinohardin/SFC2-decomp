// HYPOTHESIS: a counter pair held through pointers at +0x1c and +0x2c; class and name unknown.
class fn_00404014_Counters
{
public:
    char pad_00[0x1c];
    int* m_pDown;
    char pad_20[0x0c];
    int* m_pUp;
    int fn_00404014();
};

// FUNCTION: STARFLEETOP 0x00404014
int fn_00404014_Counters::fn_00404014()
{
    *m_pUp += 1;
    *m_pDown -= 1;
    return *m_pDown;
}

// HYPOTHESIS: another counter pair, held through pointers at +0x20 and +0x30; class and name unknown.
class fn_00404034_Counters
{
public:
    char pad_00[0x20];
    int* m_pUp;
    char pad_24[0x0c];
    int* m_pDown;
    int fn_00404034();
};

// FUNCTION: STARFLEETOP 0x00404034
int fn_00404034_Counters::fn_00404034()
{
    *m_pDown = *m_pDown - 1;
    return (*m_pUp)++;
}

#include <string>

// HYPOTHESIS: std::vector<std::string>::erase(first, last); the vector's end pointer sits at +8.
class fn_00404044_Vector
{
public:
    char pad_00[0x08];
    std::string* _Last;
    std::string* fn_00404044(std::string* _F, std::string* _L);
};

// MATCH-ONLY: the library's copy and _Destroy, inlined by the original.
inline std::string* fn_00404044_Copy(std::string* _F, std::string* _L, std::string* _X)
{
    for (; _F != _L; ++_X, ++_F)
        _X->assign(*_F, 0, std::string::npos);
    return _X;
}

inline void fn_00404044_Destroy(std::string* _F, std::string* _L)
{
    for (; _F != _L; ++_F)
        _F->~basic_string();
}

// FUNCTION: STARFLEETOP 0x00404044
std::string* fn_00404044_Vector::fn_00404044(std::string* _F, std::string* _L)
{
    std::string* _S = fn_00404044_Copy(_L, _Last, _F);
    fn_00404044_Destroy(_S, _Last);
    _Last = _S;
    return _F;
}
