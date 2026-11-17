#include "Tiger/tstring.h"
#include "Common/Thread/Optex.h"
#include "unsorted/tObject_0092deb4.h"
#include "unsorted/tSupplyDockInfo.h"
#include "unsorted/sfcstreams.h"
#include "unsorted/fn_0090bed4_Object.h"

void fn_0040e689(tSFCStream &s);
void fn_0040e691(tSFCStream &s);
void fn_0040ed22(tSFCStream &s, unsigned mode);
void fn_0040ecfd(const char *aTitle, tSFCStream &s, unsigned mode);
void fn_0040ec85(int aSize, void *aValue, tSFCStream &s, unsigned mode, const char *aText);
void fn_0040e9df(int &aValue, tSFCStream &s, unsigned mode, const char *aText);
void fn_0040eafc(bool &aValue, tSFCStream &s, unsigned mode, const char *aText);
void fn_0040ecc4(char *aValue, tSFCStream &s, unsigned mode, const char *aText);
class tString;
void fn_0040eba7(tString &aValue, tSFCStream &s, unsigned mode, const char *aText);
void fn_0040ea18(int &aValue, tSFCStream &s, unsigned mode, const char *aText);
void fn_0040eb6e(float *aValue, tSFCStream &s, unsigned mode, const char *aText);
void fn_0060e30e(tSFCStream &s, unsigned mode);
extern double g_008552d0[];
extern int g_0079ed10;

// HYPOTHESIS: a node of the list a 0x14-byte element holds; the value is at +0xc.
struct tListNode_004cf9b3
{
	tListNode_004cf9b3 *fpNext;
	char pad_04[8];
	char *fpValue;
};

// HYPOTHESIS: the iterator advanced by 0x00450ce5.
class tListIterator_004cf9b3
{
	public:
		tListNode_004cf9b3 *fpNode;
		void fn_00450ce5();
};

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

	public:
		char m_00[4];
		tListNode_004cf9b3 *m_04;
		char m_08[8];
		int m_10;
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
		void Delete();

	private:
		void *fpPointer;
		tOptex fPointerOptex;
};

inline tObject_00583828::~tObject_00583828()
{
	Delete();
}

class tSFCPlayer
{
	public:
		tSFCPlayer();
		virtual ~tSFCPlayer();

		void fn_004d0b28();
		// HYPOTHESIS: the slots up to vfunction70 in the exe's order (vfunctionN is slot N-1); the
		// ones with no definition here are placeholders, and the slots after 70 are not declared
		// as virtual yet.
		virtual void vfunction2(tSFCStream &s, unsigned mode);
		virtual void vfunction3() = 0;
		virtual void vfunction4() = 0;
		virtual void vfunction5() = 0;
		virtual void vfunction6() = 0;
		virtual void vfunction7() = 0;
		virtual void vfunction8() = 0;
		virtual void vfunction9() = 0;
		virtual void vfunction10() = 0;
		virtual void vfunction11() = 0;
		virtual void vfunction12() = 0;
		virtual void vfunction13() = 0;
		virtual void vfunction14() = 0;
		virtual void vfunction15() = 0;
		virtual void vfunction16() = 0;
		virtual void vfunction17() = 0;
		virtual void vfunction18() = 0;
		virtual void vfunction19() = 0;
		virtual void vfunction20() = 0;
		virtual void vfunction21() = 0;
		virtual void vfunction22() = 0;
		virtual void vfunction23() = 0;
		virtual void vfunction24() = 0;
		virtual void vfunction25() = 0;
		virtual void vfunction26() = 0;
		virtual void vfunction27() = 0;
		virtual void vfunction28() = 0;
		virtual void vfunction29() = 0;
		virtual void vfunction30() = 0;
		virtual void vfunction31() = 0;
		virtual void vfunction32() = 0;
		virtual void vfunction33() = 0;
		virtual void vfunction34() = 0;
		virtual void vfunction35() = 0;
		virtual void vfunction36() = 0;
		virtual void vfunction37() = 0;
		virtual void vfunction38() = 0;
		virtual void vfunction39() = 0;
		virtual void vfunction40() = 0;
		virtual void vfunction41(int param_1);
		virtual int vfunction42();
		virtual void vfunction43(int param_1);
		virtual int vfunction44();
		virtual void vfunction45(int param_1);
		virtual int vfunction46();
		virtual void vfunction47(int param_1);
		virtual int vfunction48();
		virtual void vfunction49() = 0;
		virtual void vfunction50() = 0;
		virtual void vfunction51() = 0;
		virtual void vfunction52() = 0;
		virtual void vfunction53() = 0;
		virtual void vfunction54() = 0;
		virtual void vfunction55() = 0;
		virtual void vfunction56() = 0;
		virtual void vfunction57() = 0;
		virtual void vfunction58() = 0;
		virtual int vfunction59();
		virtual void vfunction60(int param_1);
		virtual void vfunction61(int param_1);
		virtual int vfunction62();
		virtual void vfunction63(int param_1);
		virtual char vfunction64();
		virtual void vfunction65(float param_1, float param_2, float param_3);
		virtual void vfunction66(float *param_1);
		virtual void vfunction67() = 0;
		virtual void vfunction68() = 0;
		virtual void vfunction69() = 0;
		virtual void vfunction70(char *param_1) = 0;
		int vfunction88();
		void vfunction89(int param_1);
		void vfunction90(int param_1);
		int vfunction91();
		void vfunction92(int param_1);
		int vfunction93();
		void vfunction103(int param_1);
		int vfunction104();
		int vfunction106();

	private:
		bool m_04;
		tString m_08;
		int m_30;
		float m_34;
		float m_38;
		float m_3c;
		int m_40[6];
		int m_58;
		char pad_5c[0x70 - 0x5c];
		char m_70[6][0x20];
		tString m_130[6];
		int m_220[6];
		tObject_004454c1 m_238[6];
		char pad_7d8[0x7f0 - 0x7d8];
		int m_7f0[6];
		char pad_808[0x80c - 0x808];
		int m_80c[6];
		int m_824;
		char pad_828[0x83c - 0x828];
		char m_83c[6][0x20];
		tString m_8fc[6];
		int m_9ec[6];
		tObject_004454c1 m_a04[6];
		int m_fa4;
		char pad_fa8[0x1198 - 0xfa8];
		tObject_004cf9b3 m_1198[0x1b0];
		char pad_3358[0x3418 - 0x3358];
		tString m_3418[0x24];
		tString m_39b8[0x24];
		tObject_004454c1 m_3f58[0x24];
		char pad_6118[0x61a8 - 0x6118];
		tObject_004cf9b3 m_61a8[0x1b0];
		// HYPOTHESIS: the serialization walks 18 * 20 ints from here, which reaches over the members
		// that follow; only the part up to the next member is declared.
		int m_8368[0x50];
		int m_84a8;
		int m_84ac;
		int m_84b0;
		bool m_84b4;
		char pad_84b5[0x84bc - 0x84b5];
		int m_84bc;
		int m_84c0;
		int m_84c4;
		int m_84c8;
		int m_84cc[16];
		int m_850c;
		int m_8510;
		bool m_8514;
		int m_8518;
		int m_851c;
		int m_8520;
		int m_8524;
		int m_8528;
		int m_852c;
		int m_8530;
		int m_8534;
		int m_8538;
		int m_853c;
		char pad_8540[0x8544 - 0x8540];
		int m_8544[4][6];

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

// FUNCTION: STARFLEETOP 0x004cf940
int tSFCPlayer::vfunction88()
{
	return m_8518;
}

// FUNCTION: STARFLEETOP 0x004cf947
void tSFCPlayer::vfunction89(int param_1)
{
	m_8518 = param_1;
}

// FUNCTION: STARFLEETOP 0x004cf954
void tSFCPlayer::vfunction90(int param_1)
{
	m_8524 = param_1;
}

// FUNCTION: STARFLEETOP 0x004cf961
int tSFCPlayer::vfunction91()
{
	return m_8524;
}

// FUNCTION: STARFLEETOP 0x004cf968
void tSFCPlayer::vfunction92(int param_1)
{
	m_8528 = param_1;
}

// FUNCTION: STARFLEETOP 0x004cf975
int tSFCPlayer::vfunction93()
{
	return m_8528;
}

// FUNCTION: STARFLEETOP 0x004cf97c
void tSFCPlayer::vfunction103(int param_1)
{
	m_851c = param_1;
}

// FUNCTION: STARFLEETOP 0x004cf989
int tSFCPlayer::vfunction104()
{
	return m_851c;
}

// FUNCTION: STARFLEETOP 0x004cf990
int tSFCPlayer::vfunction106()
{
	return m_8520;
}

// FUNCTION: STARFLEETOP 0x004cf997
// FUNCTION: STARFLEETOP 0x004cf9cb
tSFCPlayer::~tSFCPlayer()
{
	fn_004d0b28();
	m_85a4.Delete();
}

void __stdcall fn_004d1b74(char *param_1, char *param_2);

// FUNCTION: STARFLEETOP 0x004cf9b3
void fn_004cf9b3()
{
	char local_6;
	char local_5;
	fn_004d1b74(&local_6, &local_5);
}

// FUNCTION: STARFLEETOP 0x004cfb31
void tSFCPlayer::vfunction2(tSFCStream &s, unsigned mode)
{
	fn_0040ed22(s, mode);
	fn_0040ecfd("The Player:", s, mode);
	fn_0040e689(s);
	fn_0040eafc(m_04, s, mode, "fIsInit");
	fn_0040eba7(m_08, s, mode, "fPlayerName");
	fn_0040ea18(m_30, s, mode, "fNumOfShips");
	fn_0040ec85(4, &m_8538, s, mode, "fGameType");
	fn_0040eb6e(&m_34, s, mode, "fStartPos");
	for (int ishiploop = 0; ishiploop < 6; ishiploop++) {
		fn_0040e9df(m_40[ishiploop], s, mode, "fShipID[ishiploop]");
		fn_0040ec85(4, &m_58, s, mode, "fShipType[ishiploop]");
		fn_0040ecc4(m_70[ishiploop], s, mode, "fShipName[ishiploop]");
		fn_0040eba7(m_130[ishiploop], s, mode, "fShipCallName[ishiploop]");
		fn_0040e9df(m_220[ishiploop], s, mode, "fShipDamage[ishiploop]");
		fn_0040e9df(m_80c[ishiploop], s, mode, "fSavedShipID[ishiploop]");
		fn_0040ec85(4, &m_824, s, mode, "fSavedShipType[ishiploop]");
		fn_0040ecc4(m_83c[ishiploop], s, mode, "fSavedShipName[ishiploop]");
		fn_0040eba7(m_8fc[ishiploop], s, mode, "fSavedShipCallName[ishiploop]");
		fn_0040e9df(m_9ec[ishiploop], s, mode, "fSavedShipDamage[ishiploop]");
	}
	fn_0040ea18(m_fa4, s, mode, "fSavedNumOfShips");
	if (mode == 1)
		vfunction69();
	for (int hullType = 0; hullType < 0x1b0; hullType += 0x18) {
		int count = 0;
		int cursor;
		int limit = 0x18;
		if (s.fn_00423402() == g_0079ed10)
			limit = 0x14;
		for (int ishiploop = 0; ishiploop < limit; ishiploop++) {
			if (mode != 1) {
				// HYPOTHESIS: the original indexes the 0x14-byte elements from the start of the
				// object (element + 0x61a8 bytes), so the displacement stays out of the index.
				int *base = (int *)this;
				cursor = (int)(base + (hullType + ishiploop) * 5);
				count = *(int *)(cursor + 0x61b8);
				fn_0040e9df(count, s, mode, "localInt");
				tListIterator_004cf9b3 it;
				it.fpNode = (*(tListNode_004cf9b3 **)(cursor + 0x61ac))->fpNext;
				if (it.fpNode != *(tListNode_004cf9b3 **)(cursor + 0x61ac)) {
					do {
						fn_0040ecc4(it.fpNode->fpValue, s, mode, "fShipsSeenList[ishiploop][hullType]");
						it.fn_00450ce5();
					} while (it.fpNode != *(tListNode_004cf9b3 **)(cursor + 0x61ac));
				}
			}
			else {
				fn_0040e9df(count, s, 1, "localInt");
				char name[256];
				name[0] = 0;
				for (cursor = 0; cursor < count; cursor++) {
					fn_0040ecc4(name, s, 1, "fShipsSeenList[ishiploop][hullType]");
					vfunction70(name);
				}
			}
		}
	}
	int *pTotal = m_8368;
	for (int ievent = 0; ievent < 0x12; ievent++) {
		for (int iclass = 0; iclass < 0x14; iclass++) {
			fn_0040e9df(*pTotal, s, mode, "fCareerTotals[ievent][iclass]");
			pTotal++;
		}
	}
	int *pStatus = &m_8544[0][0];
	for (int ievent2 = 0; ievent2 < 4; ievent2++) {
		for (int medals = 0; medals < 6; medals++) {
			fn_0040e9df(*pStatus, s, mode, "fMissionStatus[ievent][medals]");
			pStatus++;
		}
	}
	for (int ievent3 = 0; ievent3 < 0x10; ievent3++)
		fn_0040e9df(m_84cc[ievent3], s, mode, "fScriptValues[ievent]");
	fn_0040e9df(m_84a8, s, mode, "fPlayersNetID");
	fn_0040ec85(4, &m_84ac, s, mode, "fPlayerType");
	fn_0040e9df(m_84b0, s, mode, "fPlayerTeam");
	fn_0040eafc(m_84b4, s, mode, "fIsUseCD");
	fn_0040ec85(4, &m_84bc, s, mode, "fPreviousEnemyRace");
	fn_0040ec85(4, &m_84c0, s, mode, "fPreviousAlliedRace");
	fn_0040ec85(4, &m_84c4, s, mode, "fPreviousVictoryLevel");
	fn_0040ec85(4, &m_84c8, s, mode, "fPreviousSystem");
	fn_0040ec85(4, &m_850c, s, mode, "fUniverse");
	fn_0040ec85(4, &m_8510, s, mode, "fGameLevel");
	fn_0040eafc(m_8514, s, mode, "fIsShakedown");
	fn_0040e9df(m_8518, s, mode, "fLocation");
	fn_0040e9df(m_851c, s, mode, "fPrestige");
	fn_0040e9df(m_8520, s, mode, "fCareerPrestige");
	fn_0040e9df(m_8524, s, mode, "fRankLevel");
	fn_0040e9df(m_8528, s, mode, "fEliteRank");
	fn_0040e9df(m_852c, s, mode, "fGameServer");
	fn_0040ec85(4, &m_8530, s, mode, "fPlayerColor");
	fn_0040e9df(m_8534, s, mode, "fDifficultyLevel");
	fn_0040ec85(4, &m_853c, s, mode, "fDifficultyLevel");
	g_p0090bed4->fn_00522c86((float)g_008552d0[m_853c]);
	fn_0060e30e(s, mode);
	fn_0040e691(s);
}
