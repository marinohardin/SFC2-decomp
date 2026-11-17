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
		inline basic_string(const char* s, char al);
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
		unsigned int m_res;
	};
}

typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > fn_004d2238_string;

// MATCH-ONLY: the original inlined the const char* constructor into its callers.
inline std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string(const char* s, char al)
{
	m_al = al;
	_Tidy(false);
	assign(s, strlen(s));
}

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
	fn_004d2265_holder(); // HYPOTHESIS: out of line at 0x004d2238 (its use of the return value shows it)
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
// HYPOTHESIS: the object found by fn_004cb4f8; deleted through its virtual destructor.
class fn_004cb4f8_result
{
public:
	virtual ~fn_004cb4f8_result();
};

class tString;

class fn_004cb4a7_target
{
public:
	void fn_004bf6b9(const char* text, tString& s);
	void fn_004cb4a7(int dst, const char* text);
	fn_004cb4f8_result* fn_004cb4f8(const char* text);
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
	char m_str[16];
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

// HYPOTHESIS: a sibling of fn_004d2265_holder: a destructor body (0x004d24dc) followed by the
// inlined destruction of a string member at +0.
class fn_004d2420_holder
{
public:
	~fn_004d2420_holder();
	void fn_004d24dc();
	void fn_004d2451(const char* text);
	void fn_004d2521(const char* text);
	void fn_004d2573(fn_004d2238_string s);
	void fn_004d25af(int value);
	fn_004d2238_string m_str;
};

// FUNCTION: STARFLEETOP 0x004d24dc
void fn_004d2420_holder::fn_004d24dc()
{
	if (m_str.m_len != 0)
	{
		const char* p = m_str.c_str();
		fn_004cb4f8_result* r = g_009254f8.fn_004cb4f8(p);
		if (r != 0)
			delete r;
		m_str.assign("", strlen(""));
	}
}

// FUNCTION: STARFLEETOP 0x004d2420
fn_004d2420_holder::~fn_004d2420_holder()
{
	fn_004d24dc();
}

#include "unsorted/tValueBase.h"

// HYPOTHESIS: a stand-in for Tiger's tString (Tiger/tstring.h), which cannot be included here: its <vector> clashes with the
// reduced basic_string above. Only the layout (0x28 bytes, vtable) and the default constructor
// (0x006230d0) are used by this unit.
// MATCH-ONLY: the class must carry Tiger's name so the constructor call binds to tString::tString;
// the manifest reads a second declaration of tString as an error, so the macro keeps the stand-in's
// own name in the source text.
#define fn_004d2451_string tString
class fn_004d2451_string
{
public:
	fn_004d2451_string();
	fn_004d2451_string(const char* s);
	virtual ~fn_004d2451_string();
protected:
	char m_rest[0x24];
};

// HYPOTHESIS: a value object with a string member at +0x18 (0x40 bytes, vtable 0x0079b8a4); name unknown.
class fn_004d2451_value : public tValueBase
{
public:
	virtual ~fn_004d2451_value();
	fn_004d2451_string m_name;
};

// FUNCTION: STARFLEETOP 0x004d2451
void fn_004d2420_holder::fn_004d2451(const char* text)
{
	fn_004d24dc();
	if (text != 0)
	{
		m_str.assign(text, strlen(text));
		fn_004d2451_value* value = new fn_004d2451_value;
		g_009254f8.fn_004cb4a7((int)value, m_str.c_str());
	}
}

// HYPOTHESIS: builds a temporary tString from text and hands the held string and it to fn_004bf6b9.
// FUNCTION: STARFLEETOP 0x004d2521
void fn_004d2420_holder::fn_004d2521(const char* text)
{
	g_009254f8.fn_004bf6b9(m_str.c_str(), fn_004d2451_string(text));
}

// HYPOTHESIS: takes a string by value (the standard layout is 16 bytes; the callee destroys it).
// FUNCTION: STARFLEETOP 0x004d2573
void fn_004d2420_holder::fn_004d2573(fn_004d2238_string s)
{
	fn_004d2521(s.c_str());
}

// HYPOTHESIS: a reduced stand-in for std::stringstream (keeps this unit free of <sstream>, which
// clashes with the reduced basic_string above). Callees: constructor 0x00402d88,
// operator<<(int) 0x0041a2f0 region, str() 0x0042d292, virtual-base destructor 0x00401fd2.
namespace std
{
	class ios_base
	{
	public:
		virtual ~ios_base();
		char m_base[0x38];
	};
	template <class C, class Tr> class basic_ios : public ios_base
	{
	public:
		virtual ~basic_ios() {}
		char m_ios[0x8];
	};
	template <class C, class Tr> class basic_istream : virtual public basic_ios<C, Tr>
	{
	public:
		virtual ~basic_istream() {}
		int m_chcount;
	};
	template <class C, class Tr> class basic_ostream : virtual public basic_ios<C, Tr>
	{
	public:
		virtual ~basic_ostream() {}
		basic_ostream<C, Tr>& operator<<(int v);
	};
	template <class C, class Tr> class basic_iostream : public basic_istream<C, Tr>, public basic_ostream<C, Tr>
	{
	public:
		virtual ~basic_iostream() {}
	};
	template <class C, class Tr, class A> class basic_stringbuf
	{
	public:
		~basic_stringbuf();
		char m_buf[0x3c];
	};
	template <class C, class Tr, class A> class basic_stringstream : public basic_iostream<C, Tr>
	{
	public:
		basic_stringstream(int mode = 3);
		virtual ~basic_stringstream() {}
		basic_string<C, Tr, A> str() const;
		basic_stringbuf<C, Tr, A> _Sb;
	};
}

// FUNCTION: STARFLEETOP 0x004d25af
void fn_004d2420_holder::fn_004d25af(int value)
{
	std::basic_stringstream<char, std::char_traits<char>, std::allocator<char> > stream;
	stream << value;
	fn_004d2521(stream.str().c_str());
}

// HYPOTHESIS: the 0x64-byte object at tPanelMgr+0x100 (declared as fn_004d290d_object inside
// tPanelMgr.cpp, which cannot be included); this reduced copy only carries what its destructor
// (0x004d2636) touches: a base part, two 0x14-byte parts, then the 0x004d2353 and 0x004d2420 holders.
class fn_00460b03_part
{
public:
	~fn_00460b03_part();
	char m_0x00[0x14];
};

class fn_00460b3b_part
{
public:
	~fn_00460b3b_part();
	char m_0x00[0x14];
};

class fn_00460acb_base
{
public:
	~fn_00460acb_base();
	char m_0x00[0x14];
};

class fn_004d2636_object : public fn_00460acb_base
{
public:
	~fn_004d2636_object();
	void fn_004d290d();
	fn_00460b03_part m_0x14;
	fn_00460b3b_part m_0x28;
	fn_004d2353_holder m_0x3c;
	fn_004d2420_holder m_0x54;
};

// FUNCTION: STARFLEETOP 0x004d2636
fn_004d2636_object::~fn_004d2636_object()
{
	fn_004d290d();
}

#include "unsorted/tVisObject.h"

// HYPOTHESIS: RTTI names tVisStateObject (derived from tVisObject); its layout is not recovered yet.
class tVisStateObject : public tVisObject
{
};

// HYPOTHESIS: the global at 0x0092e4a0 is an object whose method 0x004d4728 finds a tVisObject
// from the second argument; its class is not recovered yet.
class fn_004d4728_owner
{
public:
	tVisObject* fn_004d4728( int a );
};

extern fn_004d4728_owner g_0092e4a0;

// FUNCTION: STARFLEETOP 0x004d269b
bool __stdcall fn_004d269b( int a, int b, char c )
{
	tVisStateObject* p = 0;
	if ( a && b )
	{
		p = dynamic_cast<tVisStateObject*>( g_0092e4a0.fn_004d4728( b ) );
		if ( p )
		{
			p->vfunction13();
			p->vfunction15( c == 0 );
		}
	}
	return p != 0;
}

// HYPOTHESIS: a map from string to fn_004d2265_holder* (operator[] is 0x004d2add); name unknown.
class fn_004d26fb_map
{
public:
	void fn_004d26fb(int a, const char* text);
	fn_004d2265_holder*& fn_004d2add(const fn_004d2238_string& key);
};

// FUNCTION: STARFLEETOP 0x004d26fb
void fn_004d26fb_map::fn_004d26fb(int a, const char* text)
{
	fn_004d2265_holder* holder = 0;
	if (a != 0 && text != 0)
	{
		holder = new fn_004d2265_holder;
		holder->fn_004d2296(a, text);
		char allocator;
		fn_004d2238_string key(text, allocator);
		fn_004d2add(key) = holder;
	}
}

// HYPOTHESIS: a map from an id to fn_004d2353_holder* (operator[] is 0x004d2b4c) at +0x14; name unknown.
class fn_004d2b4c_map
{
public:
	fn_004d2353_holder*& fn_004d2b4c(const unsigned long& key);
	char m_0x00[0xc];
};

class fn_004d2797_owner
{
public:
	void fn_004d2797(unsigned long a, const char* text, int b);
	char m_0x00[0x14];
	fn_004d2b4c_map m_0x14;
};

// FUNCTION: STARFLEETOP 0x004d2797
void fn_004d2797_owner::fn_004d2797(unsigned long a, const char* text, int b)
{
	fn_004d2353_holder* holder = 0;
	if (text != 0)
	{
		// HYPOTHESIS: fn_004d2319_holder is the constructor of fn_004d2353_holder.
		holder = (fn_004d2353_holder*)new fn_004d2319_holder;
		holder->fn_004d2388(a, text, b);
		m_0x14.fn_004d2b4c(a) = holder;
	}
}
