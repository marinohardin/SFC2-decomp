// HYPOTHESIS: a member of an unidentified class that returns a one-bit flag kept in
// bit 8 of the dword at +0x114, read as a bit-field (`shr eax, 8; and al, 1`).
class fn_005430ad_owner
{
public:
	bool fn_005430ad();

	char m_pad[0x114];
	unsigned int m114_0 : 8;
	unsigned int m114_8 : 1;
};

// FUNCTION: STARFLEETOP 0x005430ad
bool fn_005430ad_owner::fn_005430ad()
{
	return m114_8 ? true : false;
}
