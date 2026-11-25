// Unit:      unsorted/fn_00600BE5.cpp
// Functions: 0x00600be5 to 0x00600e47, 12 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   game-release (harness/units.json)

// HYPOTHESIS: an owning-pointer holder (auto_ptr style); class and names are not known.
class fn_00600be5_target
{
public:
	virtual void vfunction0();
	virtual ~fn_00600be5_target();
};

struct fn_00600be5_holder
{
	~fn_00600be5_holder();
	fn_00600be5_holder& fn_00600be5(fn_00600be5_holder& other);
	bool m_owns;
	fn_00600be5_target* m_ptr;
};

// FUNCTION: STARFLEETOP 0x00600be5
fn_00600be5_holder& fn_00600be5_holder::fn_00600be5(fn_00600be5_holder& other)
{
	if (m_ptr != other.m_ptr)
	{
		if (m_owns && m_ptr)
			delete m_ptr;
		m_owns = other.m_owns;
		other.m_owns = false;
		m_ptr = other.m_ptr;
	}
	else if (other.m_owns)
	{
		m_owns = true;
		other.m_owns = false;
	}
	return *this;
}

// FUNCTION: STARFLEETOP 0x00600c26
fn_00600be5_holder::~fn_00600be5_holder()
{
	if (m_owns && m_ptr)
		delete m_ptr;
}

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
		// MATCH-ONLY: the original inlined the const char* constructor into the initializer.
		__forceinline basic_string(const char* s)
		{
			char allocator;
			m_al = allocator; // HYPOTHESIS: uninitialised allocator byte
			_Tidy(false);
			assign(s, strlen(s));
		}
		~basic_string()
		{
			_Tidy(true);
		}
		basic_string& assign(const char* s, unsigned int n);
	private:
		void _Tidy(bool built);
	public:
		char m_al;
		char* _Ptr;
		unsigned int m_len;
		unsigned int m_res;
	};
}

// HYPOTHESIS: the global's name is not known.
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > fn_00600be5_string;

// FUNCTION: STARFLEETOP 0x00600caf
// FUNCTION: STARFLEETOP 0x00600c7c
fn_00600be5_string g_00c39f40 = "MetaMedals";

// HYPOTHESIS: the global's name is not known.
// FUNCTION: STARFLEETOP 0x00600d15
// FUNCTION: STARFLEETOP 0x00600ce2
fn_00600be5_string g_00c39f28 = "Medals";

// HYPOTHESIS: the global's name is not known.
// FUNCTION: STARFLEETOP 0x00600d48
fn_00600be5_string g_00c39f18 = "cRankMedals_";
