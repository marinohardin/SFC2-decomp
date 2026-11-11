//	tArea3Panel.h: tArea3Panel, the base class of the fleet panel.
// Evidence:  the analysis's class names for the panel's functions (original file unknown)

#ifndef tArea3Panel_h
#define tArea3Panel_h 1

// HYPOTHESIS: base class of the fleet panel; its constructor is at 0x0048a734.
class tArea3Panel
{
public:
	tArea3Panel();
	// HYPOTHESIS: virtual, since the panel's destructor is reached through a scalar deleting
	// destructor.
	virtual ~tArea3Panel();
	// HYPOTHESIS: the base handler at 0x0048a9c1 that 0x0048c3a6 falls back to.
	virtual void vfunction3(int *param_1);
	// HYPOTHESIS: a string handed to 0x004d3331 by the fleet panel.
	const char *m_psz04;
};

#endif // tArea3Panel_h
