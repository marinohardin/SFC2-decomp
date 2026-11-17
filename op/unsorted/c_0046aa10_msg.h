//	c_0046aa10_msg.h: the 0x20-byte message the queue functions 0x0046aa10 and 0x0046aa2b take.
// Evidence:  calls (original name and file unknown)

#ifndef c_0046aa10_msg_h
#define c_0046aa10_msg_h 1

// HYPOTHESIS: a 0x20-byte message; the id is at +0x0 and the constructor sets the fields below.
// tCampaignState.cpp (c_0046aa10_msg), tPanelMgr.cpp and tListBox_Select.cpp still declare their
// own copies under other names; they belong here once those units include this header.
struct c_0046aa10_message
{
	c_0046aa10_message();

	int id;
	int m_04;
	int m_08;
	int m_0C;
	int m_10;
	int m_14;
	int m_18;
	int m_1C;
};

inline c_0046aa10_message::c_0046aa10_message()
{
	id = 0;
	m_04 = -1;
	m_14 = 0;
	m_18 = -1;
	m_1C = -1;
}

// HYPOTHESIS: cdecl; 0x0046aa10 takes the next message (true while there is one), 0x0046aa2b
// posts one.
bool fn_0046aa10(c_0046aa10_message *msg);
void fn_0046aa2b(c_0046aa10_message *msg);

#endif	// c_0046aa10_msg_h
