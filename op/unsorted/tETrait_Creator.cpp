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
// FUNCTION: STARFLEETOP 0x0040fed3
tETrait_Creator::~tETrait_Creator()
{
	tETrait_Creator* p = sFirst;
	while (p) {
		if (p == this) {
			if (sFirst == this)
				sFirst = mNext;
			return;
		}
		p = p->mNext;
	}
}
