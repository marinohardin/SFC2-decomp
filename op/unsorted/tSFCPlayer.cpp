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
		char pad_84ac[0x85a4 - 0x84ac];
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
