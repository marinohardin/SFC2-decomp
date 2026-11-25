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
