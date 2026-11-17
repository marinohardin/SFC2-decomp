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
class fn_004d2319_holder;

namespace std
{
	template <class T> struct char_traits;
	template <class T> class allocator;
	template <class C, class Tr, class A> class basic_string
	{
	public:
		basic_string();
		~basic_string() { _Tidy(true); }
		basic_string& assign(const char* s, unsigned int n);
		const char* c_str() const { return _Ptr != 0 ? _Ptr : _Nullstr(); }
	private:
		static const char* _Nullstr() { static const char _C = char(); return &_C; }
		void _Tidy(bool built);
		friend class ::fn_004d2319_holder;
	public:
		char m_al;
		char* _Ptr;
		unsigned int m_len;
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

// HYPOTHESIS: a class with a user destructor body (0x004d22e0) and a string member whose
// destructor (_Tidy(true), 0x005da20a) is inlined.
class fn_004d2265_holder
{
public:
	~fn_004d2265_holder();
	void fn_004d22e0();
	void fn_004d2296(int dst, const char* text);
	fn_004d2238_string m_str;
	// m_len is the string's length member (+8), read directly by 0x004d22e0.
};

// FUNCTION: STARFLEETOP 0x004d2265
fn_004d2265_holder::~fn_004d2265_holder()
{
	fn_004d22e0();
}

// HYPOTHESIS: copies text into m_str, then passes the stored text (or the empty literal) with dst on.
class fn_004cb4a7_target
{
public:
	void fn_004cb4a7(int dst, const char* text);
	void fn_004cb4f8(const char* text);
	void fn_004cb56f(int a, const char* text, int b);
	void fn_004cb5bd(const char* text);
};

extern fn_004cb4a7_target g_009254f8;

// FUNCTION: STARFLEETOP 0x004d22e0
void fn_004d2265_holder::fn_004d22e0()
{
	if (m_str.m_len != 0)
	{
		const char* p = m_str.c_str();
		g_009254f8.fn_004cb4f8(p);
		m_str.assign("", strlen(""));
	}
}

// FUNCTION: STARFLEETOP 0x004d2296
void fn_004d2265_holder::fn_004d2296(int dst, const char* text)
{
	fn_004d22e0();
	if (dst != 0 && text != 0)
	{
		m_str.assign(text, strlen(text));
		const char* p = m_str.c_str();
		g_009254f8.fn_004cb4a7(dst, p);
	}
}

// HYPOTHESIS: a string-like holder whose constructor zeroes its two words, then does what
// basic_string's constructor does; the callees are the same _Tidy and assign.
// HYPOTHESIS: the string member's constructor was inlined here (m_al, _Tidy, assign); the
// member is kept as raw storage so the out-of-line constructor is not called.
class fn_004d2319_holder
{
public:
	fn_004d2319_holder();
	int m_a;
	int m_b;
	char m_str[12];
};

// FUNCTION: STARFLEETOP 0x004d2319
fn_004d2319_holder::fn_004d2319_holder()
{
	m_a = 0;
	m_b = 0;
	fn_004d2238_string* s = (fn_004d2238_string*)m_str;
	char allocator;
	s->m_al = allocator;
	s->_Tidy(false);
	s->assign("", strlen(""));
}

// HYPOTHESIS: a sibling of fn_004d2265_holder: a destructor body (0x004d23db) followed by the
// inlined destruction of a string member at +8.
class fn_004d2353_holder
{
public:
	~fn_004d2353_holder();
	void fn_004d23db();
	void fn_004d2388(int a, const char* text, int b);
	int m_a;
	int m_b;
	fn_004d2238_string m_str;
};

// FUNCTION: STARFLEETOP 0x004d2353
fn_004d2353_holder::~fn_004d2353_holder()
{
	fn_004d23db();
}

// FUNCTION: STARFLEETOP 0x004d2388
void fn_004d2353_holder::fn_004d2388(int a, const char* text, int b)
{
	fn_004d23db();
	if (text != 0)
	{
		m_str.assign(text, strlen(text));
		m_a = a;
		m_b = b;
		const char* p = m_str.c_str();
		g_009254f8.fn_004cb56f(a, p, b);
	}
}

// FUNCTION: STARFLEETOP 0x004d23db
void fn_004d2353_holder::fn_004d23db()
{
	if (m_str.m_len != 0)
	{
		const char* p = m_str.c_str();
		g_009254f8.fn_004cb5bd(p);
		m_str.assign("", strlen(""));
		m_a = 0;
		m_b = 0;
	}
}

// HYPOTHESIS: a function holding one unwinding local: call fn_004d24dc on this under state 0,
// then call fn_005da20a(1) on this once the local is gone.
class fn_004d2420_part
{
public:
	~fn_004d2420_part();
};

class fn_004d2420_holder
{
public:
	void fn_004d24dc();
	void fn_005da20a(int a);
	void fn_004d2420();
};

// FUNCTION: STARFLEETOP 0x004d2420
void fn_004d2420_holder::fn_004d2420()
{
	{
		fn_004d2420_part guard;
		fn_004d24dc();
	}
	fn_005da20a(1);
}
