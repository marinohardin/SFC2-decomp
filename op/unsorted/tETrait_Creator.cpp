// HYPOTHESIS: tETrait_Creator links itself into a global list of creators; the
// member and list-head names are guesses.
class tETrait_Creator
{
public:
	tETrait_Creator();
	virtual ~tETrait_Creator();

	tETrait_Creator* mNext;
	static tETrait_Creator* sFirst;
};

// FUNCTION: STARFLEETOP 0x0040fea0
tETrait_Creator::tETrait_Creator()
{
	mNext = sFirst;
	sFirst = this;
}

// FUNCTION: STARFLEETOP 0x0040feb7
tETrait_Creator::~tETrait_Creator()
{
}
