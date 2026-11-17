#include "Tiger/tstring.h"
#include "unsorted/tObject_0092deb4.h"
#include "unsorted/tSupplyDockInfo.h"

// HYPOTHESIS: the 0xf0-byte element of four member arrays; constructor 0x004454c1, destructor
// 0x004459be (original name unknown).
class tObject_004454c1
{
	public:
		tObject_004454c1();
		~tObject_004454c1();

	private:
		char m_00[0xf0];
};

// HYPOTHESIS: the 0x14-byte element of two 0x1b0-element member arrays; constructor
// 0x004cf9b3, destructor 0x004cf9c6 (original name unknown).
class tObject_004cf9b3
{
	public:
		tObject_004cf9b3();
		~tObject_004cf9b3();

	private:
		char m_00[0x14];
};

// HYPOTHESIS: the 0xf8-byte object tSFCPlayer allocates first; constructor 0x004292ab.
class tObject_004292ab
{
	public:
		tObject_004292ab();

	private:
		char m_00[0xf8];
};

// HYPOTHESIS: a 0x18-byte member that takes ownership of a new object; both members at +0x85a4
// and +0x85bc are built by the constructor at 0x00583828.
class tObject_00583828
{
	public:
		tObject_00583828(void *object);
		~tObject_00583828();

	private:
		char m_00[0x18];
};

class tSFCPlayer
{
	public:
		tSFCPlayer();
		virtual ~tSFCPlayer();

		void fn_004d0b28();
			// HYPOTHESIS: a virtual in slot 41 in the exe; only the other slots are missing here.
			void vfunction41(int param_1);
			int vfunction42();
			void vfunction43(int param_1);
			int vfunction44();
			void vfunction45(int param_1);
			int vfunction46();
			void vfunction47(int param_1);
			int vfunction48();
			int vfunction59();
			void vfunction60(int param_1);
			void vfunction61(int param_1);
			int vfunction62();
			void vfunction63(int param_1);
			char vfunction64();
			void vfunction65(float param_1, float param_2, float param_3);
			void vfunction66(float *param_1);

	private:
		char m_04;
		tString m_08;
		int m_30;
		float m_34;
		float m_38;
		float m_3c;
		char pad_40[0x130 - 0x40];
		tString m_130[6];
		char pad_220[0x238 - 0x220];
		tObject_004454c1 m_238[6];
		char pad_7d8[0x7f0 - 0x7d8];
		int m_7f0[6];
		char pad_808[0x8fc - 0x808];
		tString m_8fc[6];
		char pad_9ec[0xa04 - 0x9ec];
		tObject_004454c1 m_a04[6];
		char pad_fa4[0x1198 - 0xfa4];
		tObject_004cf9b3 m_1198[0x1b0];
		char pad_3358[0x3418 - 0x3358];
		tString m_3418[0x24];
		tString m_39b8[0x24];
		tObject_004454c1 m_3f58[0x24];
		char pad_6118[0x61a8 - 0x6118];
		tObject_004cf9b3 m_61a8[0x1b0];
		char pad_8368[0x84a8 - 0x8368];
		int m_84a8;
		char pad_84ac[0x84bc - 0x84ac];
			int m_84bc;
			int m_84c0;
			int m_84c4;
			int m_84c8;
			char pad_84cc[0x852c - 0x84cc];
			int m_852c;
			int m_8530;
			char pad_8534[0x85a4 - 0x8534];

		tObject_00583828 m_85a4;
		tObject_00583828 m_85bc;
};

// HYPOTHESIS: the analysis names this address ~tSFCPlayer, but it builds every member, stores
// the vftable 0x007a55c4 last and registers the first player in the pointer at 0x0092deb4: it is
// the constructor.
// FUNCTION: STARFLEETOP 0x004cf692
tSFCPlayer::tSFCPlayer()
	: m_85a4(new tObject_004292ab), m_85bc(new tSupplyDockInfo)
{
	if (gObject_0092deb4 == 0) {
		// HYPOTHESIS: the pointer at 0x0092deb4 holds the local player.
		gObject_0092deb4 = (tObject_0092deb4 *)this;
		m_04 = 0;
		m_84a8 = 0;
		m_30 = 0;
		m_08 = "";
		m_34 = 0.0f;
		m_38 = 0.0f;
		m_3c = 0.0f;
		for (int i = 0; i < 6; i++)
			m_7f0[i] = 0;
		fn_004d0b28();
	}
}

// FUNCTION: STARFLEETOP 0x004cf884
void tSFCPlayer::vfunction41(int param_1)
{
	m_84bc = param_1;
}

// FUNCTION: STARFLEETOP 0x004cf891
int tSFCPlayer::vfunction42()
{
	return m_84bc;
}

// FUNCTION: STARFLEETOP 0x004cf898
void tSFCPlayer::vfunction43(int param_1)
{
	m_84c0 = param_1;
}

// FUNCTION: STARFLEETOP 0x004cf8a5
int tSFCPlayer::vfunction44()
{
	return m_84c0;
}

// FUNCTION: STARFLEETOP 0x004cf8ac
void tSFCPlayer::vfunction45(int param_1)
{
	m_84c4 = param_1;
}

// FUNCTION: STARFLEETOP 0x004cf8b9
int tSFCPlayer::vfunction46()
{
	return m_84c4;
}

// FUNCTION: STARFLEETOP 0x004cf8c0
void tSFCPlayer::vfunction47(int param_1)
{
	m_84c8 = param_1;
}

// FUNCTION: STARFLEETOP 0x004cf8cd
int tSFCPlayer::vfunction48()
{
	return m_84c8;
}

// FUNCTION: STARFLEETOP 0x004cf8d4
int tSFCPlayer::vfunction59()
{
	return m_84a8;
}

// FUNCTION: STARFLEETOP 0x004cf8db
void tSFCPlayer::vfunction60(int param_1)
{
	m_84a8 = param_1;
}

// FUNCTION: STARFLEETOP 0x004cf8e8
void tSFCPlayer::vfunction61(int param_1)
{
	m_852c = param_1;
}

// FUNCTION: STARFLEETOP 0x004cf8f5
int tSFCPlayer::vfunction62()
{
	return m_852c;
}

// FUNCTION: STARFLEETOP 0x004cf8fc
void tSFCPlayer::vfunction63(int param_1)
{
	m_8530 = param_1;
}

// FUNCTION: STARFLEETOP 0x004cf909
char tSFCPlayer::vfunction64()
{
	return m_8530;
}

// FUNCTION: STARFLEETOP 0x004cf910
void tSFCPlayer::vfunction65(float param_1, float param_2, float param_3)
{
	m_34 = param_1;
	m_38 = param_2;
	m_3c = param_3;
}

// FUNCTION: STARFLEETOP 0x004cf928
void tSFCPlayer::vfunction66(float *param_1)
{
	param_1[0] = m_34;
	param_1[1] = m_38;
	param_1[2] = m_3c;
}
