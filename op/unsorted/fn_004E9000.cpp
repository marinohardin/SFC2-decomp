// HYPOTHESIS: tPlasmaR_Creator (RTTI) registers itself like tETrait_Creator; its constructor
// is 0x004e926d in its own unit. It has no shared header yet, so it is declared here.
class tPlasmaR_Creator
{
public:
	tPlasmaR_Creator();
	virtual ~tPlasmaR_Creator();
};

// FUNCTION: STARFLEETOP 0x004e924d
tPlasmaR_Creator g_0092f0f0;
