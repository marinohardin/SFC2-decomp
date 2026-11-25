// HYPOTHESIS: tEntity_DirectFire has no header yet (HEADER REQUEST filed); this reduced
// declaration only gives the factory below the size 0x268 and the constructor at 0x0041ca85.
class tEntity_DirectFire
{
public:
	tEntity_DirectFire();

	char	pad00[0x268];
};

// FUNCTION: STARFLEETOP 0x00421740
tEntity_DirectFire* fn_00421740()
{
	return new tEntity_DirectFire;
}

// HYPOTHESIS: tEntity_PPDFire has no header yet; this small declaration only serves the
// factory below.
class tEntity_PPDFire
{
public:
	tEntity_PPDFire();

	char	pad00[0x2b8];
};

// FUNCTION: STARFLEETOP 0x00421775
tEntity_PPDFire* fn_00421775()
{
	return new tEntity_PPDFire;
}

// HYPOTHESIS: tEntity_ESG has no header yet; this small declaration only serves the
// factory below.
class tEntity_ESG
{
public:
	tEntity_ESG();

	char	pad00[0x260];
};

// FUNCTION: STARFLEETOP 0x004217aa
tEntity_ESG* fn_004217aa()
{
	return new tEntity_ESG;
}

// HYPOTHESIS: tEntity_DriftingSeeker has no header yet; this small declaration only serves the
// factory below.
class tEntity_DriftingSeeker
{
public:
	tEntity_DriftingSeeker();

	char	pad00[0x290];
};

// FUNCTION: STARFLEETOP 0x00421855
tEntity_DriftingSeeker* fn_00421855()
{
	return new tEntity_DriftingSeeker;
}
