class tSystemCreationInfo {
public:
	tSystemCreationInfo();
	virtual ~tSystemCreationInfo();
	long m04;
	long m08;
	long m0c;
	long m10;
};

// FUNCTION: STARFLEETOP 0x0040a5b9
tSystemCreationInfo::tSystemCreationInfo()
{
	m0c = -1;
	m10 = 0;
	m04 = 0;
	m08 = 0;
}

// FUNCTION: STARFLEETOP 0x0040a5d1
tSystemCreationInfo::~tSystemCreationInfo()
{
}
