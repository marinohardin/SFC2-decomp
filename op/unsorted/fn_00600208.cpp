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
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > fn_00600208_string;

// FUNCTION: STARFLEETOP 0x0060023b
// FUNCTION: STARFLEETOP 0x00600208
fn_00600208_string g_00c39e48 = "BriefingSP";

// FUNCTION: STARFLEETOP 0x006002a1
// FUNCTION: STARFLEETOP 0x0060026e
fn_00600208_string g_00c39e30 = "DebriefingSP";

// FUNCTION: STARFLEETOP 0x00600307
// FUNCTION: STARFLEETOP 0x006002d4
fn_00600208_string g_00c39e20 = "BriefingMultiMeta";
