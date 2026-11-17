// HYPOTHESIS: a red-black tree (std::set<unsigned long>) lower_bound; node layout from the exe.
struct fn_004d2143_node
{
    fn_004d2143_node* m00;
    fn_004d2143_node* m04;
    fn_004d2143_node* m08;
    unsigned long m0c;
};

struct fn_004d2143_tree
{
    int m00;
    fn_004d2143_node* m04;
    fn_004d2143_node* m08;
    fn_004d2143_node* fn_004d2143(const unsigned long& key);
};

// FUNCTION: STARFLEETOP 0x004d2143
fn_004d2143_node* fn_004d2143_tree::fn_004d2143(const unsigned long& key)
{
    fn_004d2143_node* x = m04->m04;
    fn_004d2143_node* y = m04;
    while (x != m08)
    {
        if (x->m0c < key)
            x = x->m08;
        else
        {
            y = x;
            x = x->m00;
        }
    }
    return y;
}

// HYPOTHESIS: a red-black tree iterator step (operator-- of a set iterator); node layout from the exe.
struct fn_004d216d_node
{
    fn_004d216d_node* m00;
    fn_004d216d_node* m04;
    fn_004d216d_node* m08;
    unsigned long m0c;
    char m10;
    char m11;
};

struct fn_004d216d_iter
{
    fn_004d216d_node* m00;
    void fn_004d216d();
};

// FUNCTION: STARFLEETOP 0x004d216d
void fn_004d216d_iter::fn_004d216d()
{
    fn_004d216d_node* y;
    if (m00->m10 == 0 && m00->m04->m04 == m00)
        y = m00->m08;
    else
    {
        fn_004d216d_node* x = m00->m00;
        if (x->m11 == 0)
        {
            while (x->m08->m11 == 0)
                x = x->m08;
            m00 = x;
            return;
        }
        while (y = m00->m04, m00 == y->m00)
            m00 = y;
    }
    m00 = y;
}

#include "Common/Thread/Optex.h"

// HYPOTHESIS: the SDK's tSingleAccessWrapper<std::vector<int> > (declared only inside
// unsorted/tClientCharacter.cpp); this is its destructor, Delete (0x0042cc05) then the optex member.
class tSingleAccessWrapper_vector
{
public:
	~tSingleAccessWrapper_vector();
	void Delete();

protected:
	void* fpPointer;
	mutable tOptex fPointerOptex;
};

// FUNCTION: STARFLEETOP 0x004d21a8
tSingleAccessWrapper_vector::~tSingleAccessWrapper_vector()
{
	Delete();
}

// HYPOTHESIS: the SDK's tSingleAccessWrapper<tObject_005391b0> (declared only inside
// unsorted/tClientCharacter.cpp); its Delete is 0x005ff84a.
class tSingleAccessWrapper_005391b0
{
public:
	~tSingleAccessWrapper_005391b0();
	void Delete();

protected:
	void* fpPointer;
	mutable tOptex fPointerOptex;
};

// FUNCTION: STARFLEETOP 0x004d21d7
tSingleAccessWrapper_005391b0::~tSingleAccessWrapper_005391b0()
{
	Delete();
}

// FUNCTION: STARFLEETOP 0x004d2206
void fn_004d2206(unsigned long* dst, unsigned long* src)
{
	if (dst != 0)
		*dst = *src;
}

// HYPOTHESIS: a string-like constructor taking the empty literal; callees unnamed.
extern "C" unsigned int strlen(const char*);

// HYPOTHESIS: the callees are basic_string::_Tidy (0x005da20a) and basic_string::assign
// (0x0040428e); a reduced stand-in of the standard class keeps this unit free of <string>.
namespace std
{
	template <class T> struct char_traits;
	template <class T> class allocator;
	template <class C, class Tr, class A> class basic_string
	{
	public:
		basic_string();
		basic_string& assign(const char* s, unsigned int n);
	private:
		void _Tidy(bool built);
		char m_al;
	};
}

typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > fn_004d2238_string;

// FUNCTION: STARFLEETOP 0x004d2238
template <>
std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string()
{
	char allocator;
	m_al = allocator; // HYPOTHESIS: copy of an empty allocator base, uninitialised byte
	_Tidy(false);
	assign("", strlen(""));
}
