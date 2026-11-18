#include <string.h>
#include "Tiger/tstring.h"
#include "Common/Thread/Optex.h"
#include "unsorted/tEntity.h"
#include "unsorted/tObject_0092deb4.h"
#include "unsorted/tNetControl.h"
#include "unsorted/tApplication.h"
#include "unsorted/tSupplyDockInfo.h"
#include "unsorted/sfcstreams.h"
#include "unsorted/fn_0090bed4_Object.h"
#include "unsorted/fn_004e2b02_ship_type.h"
#include "unsorted/tShipSpecs.h"
char fn_0040a621(int param_1, tShipSpecs *param_2);

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
extern int g_008552b8[];
extern int g_0085529c[];
extern int g_008552b4[];
class tShipSystemMask;
// HYPOTHESIS: the class of the global at 0x008f5118 (original name unknown).
class tObject_008f5118
{
	public:
		void fn_00424d28();
		void fn_00424d37(int param_1);
		unsigned fn_00424d46(char *s, unsigned i);
};
extern tObject_008f5118 g_008f5118;

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

#include "unsorted/tShipSystemMask.h"

























// HYPOTHESIS: the 0x14-byte element of two 0x1b0-element member arrays; constructor
// 0x004cf9b3, destructor 0x004cf9c6 (original name unknown).
class tObject_004cf9b3
{
	public:
		tObject_004cf9b3();
		~tObject_004cf9b3();
		void fn_004d1c60(tListIterator_004cf9b3 *result, tListIterator_004cf9b3 first, tListIterator_004cf9b3 last);
		void fn_004d1bcf(tListIterator_004cf9b3 *result, tListIterator_004cf9b3 position, char **value);
		void fn_00599af1(tListIterator_004cf9b3 *result, char **key);
		int size();
		tListIterator_004cf9b3 begin();
		tListIterator_004cf9b3 end();

	public:
		char m_00[4];
		tListNode_004cf9b3 *m_04;
		char m_08[8];
		int m_10;
};

// HYPOTHESIS: the original inlines these accessors; the list looks like a container whose head
// node is at +0x04 and whose element count is at +0x10.
inline int tObject_004cf9b3::size()
{
	return m_10;
}

inline tListIterator_004cf9b3 tObject_004cf9b3::begin()
{
	tListIterator_004cf9b3 i;
	i.fpNode = m_04->fpNext;
	return i;
}

inline tListIterator_004cf9b3 tObject_004cf9b3::end()
{
	tListIterator_004cf9b3 i;
	i.fpNode = m_04;
	return i;
}

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
		void fn_004d0c48();
		void fn_004d0c5d();
		void fn_004d0cb6();
		// HYPOTHESIS: the slots up to vfunction70 in the exe's order (vfunctionN is slot N-1); the
		// ones with no definition here are placeholders, and the slots after 70 are not declared
		// as virtual yet.
		virtual void vfunction2(tSFCStream &s, unsigned mode);
		virtual void vfunction3(char *param_1, int param_2);
		virtual void vfunction4(char *param_1, int param_2, unsigned param_3, int param_4, int param_5);
		virtual void vfunction5(tString *param_1);
		virtual void *vfunction6();
		virtual void vfunction7(int param_1);
		virtual int vfunction8();
		virtual void vfunction9(unsigned param_1);
		virtual int vfunction10() = 0;
		virtual void vfunction11(int param_1, unsigned param_2, int param_3);
		virtual unsigned vfunction12(int param_1);
		virtual unsigned vfunction13(tString *param_1);
		virtual int vfunction14(unsigned param_1);
		virtual int vfunction15(int param_1);
		virtual void vfunction16(unsigned param_1);
		virtual void vfunction17(int param_1, unsigned param_2);
		virtual void vfunction18(int param_1);
		virtual void vfunction19(char *param_1, unsigned param_2);
		virtual char *vfunction20(unsigned param_1);
		virtual void vfunction21(unsigned param_1) = 0;
		virtual int vfunction22(unsigned param_1);
			void fn_004d08e2(tString *param_1, unsigned param_2);
		virtual void vfunction23(int param_1, int param_2);
		virtual int vfunction24(int param_1);
		virtual void vfunction25(int param_1, unsigned param_2);
		virtual int vfunction26(unsigned param_1);
		virtual void vfunction27(int param_1);
		virtual int vfunction28();
		virtual void vfunction29(unsigned char param_1);
		virtual char vfunction30();
		virtual void vfunction31(int param_1);
		virtual int vfunction32();
		virtual void vfunction33(int param_1);
		virtual int vfunction34();
		virtual void vfunction35(bool param_1);
		virtual char vfunction36();
		virtual int vfunction37(char *param_1);
		virtual int vfunction38();
		virtual void vfunction39(int param_1);
		virtual int vfunction40();
		virtual void vfunction41(int param_1);
		virtual int vfunction42();
		virtual void vfunction43(int param_1);
		virtual int vfunction44();
		virtual void vfunction45(int param_1);
		virtual int vfunction46();
		virtual void vfunction47(int param_1);
		virtual int vfunction48();
		virtual void vfunction49();
		virtual void vfunction50(int *param_1);
		virtual void vfunction51(int *param_1);
		virtual void vfunction52(int param_1);
		int fn_004d11e9(int param_1);
		virtual int vfunction53();
		virtual void vfunction54(int param_1);
		virtual char vfunction55();
		virtual void vfunction56(bool param_1);
		virtual bool vfunction57();
		virtual bool vfunction58();
		virtual int vfunction59();
		virtual void vfunction60(int param_1);
		virtual void vfunction61(int param_1);
		virtual int vfunction62();
		virtual void vfunction63(int param_1);
		virtual char vfunction64();
		virtual void vfunction65(float param_1, float param_2, float param_3);
		virtual void vfunction66(float *param_1);
		virtual void vfunction67(int param_1);
		virtual int vfunction68();
		virtual void vfunction69();
		virtual void vfunction70(char *param_1);
		virtual void vfunction71();
		virtual bool vfunction72(unsigned param_1, int param_2);
		virtual void vfunction73();
		virtual void vfunction74(int param_1, int param_2, int param_3);
		virtual int vfunction75(int param_1, int param_2);
		virtual void vfunction76();
		virtual void vfunction77(int param_1, int param_2, int param_3);
		virtual int vfunction78(int param_1, int param_2);
		virtual char vfunction79();
		virtual char vfunction80();
		virtual void vfunction81(int param_1, unsigned param_2);
		virtual int vfunction82(unsigned param_1);
		virtual void vfunction83(int param_1, unsigned param_2);
		virtual int vfunction84(unsigned param_1);
		virtual void vfunction85(int param_1, unsigned param_2);
		virtual tShipSystemMask *vfunction86(unsigned param_1);
		virtual void vfunction87();
		virtual int vfunction88();
		virtual void vfunction89(int param_1);
		virtual void vfunction90(int param_1);
		virtual int vfunction91();
		virtual void vfunction92(int param_1);
		virtual int vfunction93();
		virtual void vfunction94(tShipSystemMask *param_1);
		virtual int vfunction95(int param_1);
		virtual int vfunction96(int param_1);
		virtual int vfunction97(int param_1, int param_2);
		virtual int vfunction98();
		virtual void vfunction99(tShipSystemMask *param_1, int param_2, int param_3, int param_4);
		virtual void vfunction100(unsigned param_1, int param_2, int param_3, int param_4);
		virtual void vfunction101(tShipSystemMask *param_1);
		virtual void vfunction102(unsigned param_1);
		virtual void vfunction103(int param_1);
		virtual int vfunction104();
		virtual void vfunction105(int param_1);
		virtual int vfunction106();
		virtual void vfunction107(int param_1);
		virtual void vfunction108();
		virtual void vfunction109();
		virtual void vfunction110();
		virtual void vfunction111(int *param_1);
		virtual void vfunction112(int param_1);
		virtual void vfunction113(int param_1, unsigned param_2, int param_3, tString *param_4, tString *param_5);
		virtual void vfunction114(fn_004e2b02_ship_type *param_1);
		virtual void vfunction115(char param_1);
		void fn_004d1220();

	private:
		bool m_04;
		char m_05;
		tString m_08;
		int m_30;
		float m_34;
		float m_38;
		float m_3c;
		int m_40[6];
		int m_58[6];
		char m_70[6][0x20];
		tString m_130[6];
		int m_220[6];
		tShipSystemMask m_238[6];
		int m_7d8[6];
		int m_7f0[6];
		int m_808;
		int m_80c[6];
		int m_824[6];
		char m_83c[6][0x20];
		tString m_8fc[6];
		int m_9ec[6];
		tShipSystemMask m_a04[6];
		int m_fa4;
		int m_fa8[4][0x14];
		int m_10e8[16];
		int m_1128;
		int m_112c;
		int m_1130;
		int m_1134;
		int m_1138[4][6];
		tObject_004cf9b3 m_1198[0x12][0x18];
		int m_3358[6];
		int m_3370[6];
		int m_3388[0x24];
		tString m_3418[0x24];
		tString m_39b8[0x24];
		tShipSystemMask m_3f58[0x24];
		char pad_6118[0x61a8 - 0x6118];
		tObject_004cf9b3 m_61a8[0x12][0x18];
		// HYPOTHESIS: the serialization walks 18 rows of 20 ints from here, which reaches over the
		// members that follow; only the rows up to the next member are declared.
		int m_8368[4][0x14];
		int m_84a8;
		int m_84ac;
		int m_84b0;
		bool m_84b4;
		char pad_84b5[0x84b8 - 0x84b5];
		int m_84b8;
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
		char m_8540;
		char pad_8541[0x8544 - 0x8541];
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
	int ishiploop;
	for (ishiploop = 0; ishiploop < 6; ishiploop++) {
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
	for (ishiploop = 0; ishiploop < 0x12; ishiploop++) {
		int count = 0;
		int cursor;
		int limit = 0x18;
		if (s.fn_00423402() == g_0079ed10)
			limit = 0x14;
		for (int hullType = 0; hullType < limit; hullType++) {
			if (mode != 1) {
				count = m_61a8[ishiploop][hullType].size();
				fn_0040e9df(count, s, mode, "localInt");
				tListIterator_004cf9b3 it;
				for (it.fpNode = m_61a8[ishiploop][hullType].m_04->fpNext; it.fpNode != m_61a8[ishiploop][hullType].m_04; it.fn_00450ce5())
					fn_0040ecc4(it.fpNode->fpValue, s, mode, "fShipsSeenList[ishiploop][hullTypes]");
			}
			else {
				fn_0040e9df(count, s, 1, "localInt");
				char name[256];
				name[0] = 0;
				for (cursor = 0; cursor < count; cursor++) {
					fn_0040ecc4(name, s, 1, "fShipsSeenList[ishiploop][hullTypes]");
					vfunction70(name);
				}
			}
		}
	}
	int ievent;
	for (ievent = 0; ievent < 0x12; ievent++) {
		for (int iclass = 0; iclass < 0x14; iclass++)
			fn_0040e9df(m_8368[ievent][iclass], s, mode, "fCareerTotals[ievent][iclass]");
	}
	for (ievent = 0; ievent < 4; ievent++) {
		for (int medals = 0; medals < 6; medals++)
			fn_0040e9df(m_8544[ievent][medals], s, mode, "fMissionStatus[ievent][medals]");
	}
	for (ievent = 0; ievent < 0x10; ievent++)
		fn_0040e9df(m_84cc[ievent], s, mode, "fScriptValues[ievent]");
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
	fn_0040e9df(m_852c, s, mode, "fPing");
	fn_0040ec85(4, &m_8530, s, mode, "fGameServer");
	fn_0040e9df(m_8534, s, mode, "fPlayerColor");
	fn_0040ec85(4, &m_853c, s, mode, "fDifficultyLevel");
	g_p0090bed4->fn_00522c86((float)g_008552d0[m_853c]);
	fn_0060e30e(s, mode);
	fn_0040e691(s);
}

// FUNCTION: STARFLEETOP 0x004d0063
void tSFCPlayer::vfunction108()
{
	m_808 = m_84b8;
	m_fa4 = m_30;
	for (int ishiploop = 0; ishiploop < 6; ishiploop++) {
		m_80c[ishiploop] = m_40[ishiploop];
		m_824[ishiploop] = m_58[ishiploop];
		m_8fc[ishiploop] = m_130[ishiploop];
		m_9ec[ishiploop] = m_220[ishiploop];
		m_a04[ishiploop] = m_238[ishiploop];
		strcpy(m_83c[ishiploop], m_70[ishiploop]);
	}
	for (int ievent = 0; ievent < 4; ievent++) {
		for (int iclass = 0; iclass < 0x14; iclass++)
			m_fa8[ievent][iclass] = m_8368[ievent][iclass];
	}
	for (int imedal = 0; imedal < 4; imedal++) {
		for (int istatus = 0; istatus < 6; istatus++)
			m_1138[imedal][istatus] = m_8544[imedal][istatus];
	}
	for (int ihull = 0; ihull < 0x12; ihull++) {
		for (int iclass = 0; iclass < 0x18; iclass++) {
			tListIterator_004cf9b3 erased;
			m_1198[ihull][iclass].fn_004d1c60(&erased, m_1198[ihull][iclass].begin(), m_1198[ihull][iclass].end());
			int count = m_61a8[ihull][iclass].size();
			tListIterator_004cf9b3 it = m_61a8[ihull][iclass].begin();
			while (count-- != 0) {
				tListIterator_004cf9b3 inserted;
				m_1198[ihull][iclass].fn_004d1bcf(&inserted, m_61a8[ihull][iclass].begin(), &it.fpNode->fpValue);
				it.fn_00450ce5();
			}
		}
	}
	for (int iscript = 0; iscript < 0x10; iscript++)
		m_10e8[iscript] = m_84cc[iscript];
	m_1128 = m_851c;
	m_112c = m_8520;
	m_1130 = m_8524;
	m_1134 = m_8528;
}

// FUNCTION: STARFLEETOP 0x004d0208
void tSFCPlayer::vfunction109()
{
	m_84b8 = m_808;
	m_30 = m_fa4;
	for (int ishiploop = 0; ishiploop < 6; ishiploop++) {
		m_40[ishiploop] = m_80c[ishiploop];
		m_58[ishiploop] = m_824[ishiploop];
		m_130[ishiploop] = m_8fc[ishiploop];
		m_220[ishiploop] = m_9ec[ishiploop];
		m_238[ishiploop] = m_a04[ishiploop];
		strcpy(m_70[ishiploop], m_83c[ishiploop]);
	}
	for (int ievent = 0; ievent < 4; ievent++) {
		for (int iclass = 0; iclass < 0x14; iclass++)
			m_8368[ievent][iclass] = m_fa8[ievent][iclass];
	}
	for (int imedal = 0; imedal < 4; imedal++) {
		for (int istatus = 0; istatus < 6; istatus++)
			m_8544[imedal][istatus] = m_1138[imedal][istatus];
	}
	for (int ihull = 0; ihull < 0x12; ihull++) {
		for (int iclass = 0; iclass < 0x18; iclass++) {
			tListIterator_004cf9b3 erased;
			m_61a8[ihull][iclass].fn_004d1c60(&erased, m_61a8[ihull][iclass].begin(), m_61a8[ihull][iclass].end());
			int count = m_1198[ihull][iclass].size();
			tListIterator_004cf9b3 it = m_1198[ihull][iclass].begin();
			while (count-- != 0) {
				tListIterator_004cf9b3 inserted;
				m_61a8[ihull][iclass].fn_004d1bcf(&inserted, m_61a8[ihull][iclass].begin(), &it.fpNode->fpValue);
				it.fn_00450ce5();
			}
		}
	}
	for (int iscript = 0; iscript < 0x10; iscript++)
		m_84cc[iscript] = m_10e8[iscript];
	m_851c = m_1128;
	m_8520 = m_112c;
	m_8524 = m_1130;
	m_8528 = m_1134;
}

// FUNCTION: STARFLEETOP 0x004d04f4
void tSFCPlayer::vfunction7(int param_1)
{
	m_8538 = param_1;
	if (param_1 != 5)
		gObject_0092deb4->vfunction32(0);
}

// FUNCTION: STARFLEETOP 0x004d0516
int tSFCPlayer::vfunction8()
{
	return m_8538;
}

// FUNCTION: STARFLEETOP 0x004d0535
void tSFCPlayer::vfunction9(unsigned param_1)
{
	if (param_1 <= 6)
		m_30 = param_1;
}

// FUNCTION: STARFLEETOP 0x004d051d
bool tSFCPlayer::vfunction58()
{
	// MATCH-ONLY: the original builds the flag with a conditional increment.
	bool bResult = false;
	if (gNetControl.fn_004326a4() != 8)
		bResult++;
	return bResult;
}

// FUNCTION: STARFLEETOP 0x004d0424
void tSFCPlayer::vfunction4(char *param_1, int param_2, unsigned param_3, int param_4, int param_5)
{
	if (param_3 < 6) {
		vfunction19(param_1, param_3);
		if (param_2 != -1)
			vfunction81(param_2, param_3);
		vfunction83(0, param_3);
		vfunction25(param_5, param_3);
		vfunction21(g_008f5118.fn_00424d46(param_1, param_3));
		tShipSystemMask *pEntry = &m_238[param_3];
		pEntry->fn_00447af2(param_1, param_4);
		vfunction94(pEntry);
		if (param_3 >= (unsigned)m_30)
			m_30 = param_3 + 1;
	}
}

// FUNCTION: STARFLEETOP 0x004d0544
char tSFCPlayer::vfunction79()
{
	char bResult = 0;
	for (int i = 0; i < 6; i++) {
		fn_004e2b02_ship_type *iEntity = gObject_008e1060.fn_0042496f(m_40[i]);
		if (iEntity != 0) {
			bResult = 1;
			m_238[i].fn_00447aa7((tObject_0042496f *)iEntity);
		}
	}
	return bResult;
}

// FUNCTION: STARFLEETOP 0x004d0583
char tSFCPlayer::vfunction80()
{
	char bResult = 0;
	for (unsigned i = 0; i < (unsigned)m_30; i++) {
		fn_004e2b02_ship_type *iEntity = gObject_008e1060.fn_0042496f(m_40[i]);
		if (iEntity != 0 && m_238[i].fn_00508f68() != 0) {
			bResult = 1;
			if (m_05 != 0)
				m_238[i].fn_00447b7a((tObject_0042496f *)iEntity);
		}
	}
	return bResult;
}

// FUNCTION: STARFLEETOP 0x004d0618
void tSFCPlayer::vfunction11(int param_1, unsigned param_2, int param_3)
{
	if (param_2 < 6) {
		m_40[param_2] = param_1;
		if (gObject_008e1060.fn_004235f0(param_1)) {
			if (param_2 == 0) {
				gObject_008e1060.fn_004248b5(m_40[0]);
			} else {
				fn_004e2b02_ship_type *iEntity = gObject_008e1060.fn_0042496f(param_1);
				if (iEntity != 0)
					iEntity->vfunction16();
			}
		}
	}
}

// FUNCTION: STARFLEETOP 0x004d066b
unsigned tSFCPlayer::vfunction13(tString *param_1)
{
	for (unsigned i = 0; i < (unsigned)m_30; i++) {
		if (m_130[i] == *param_1)
			return i;
	}
	return 0xffffffff;
}

// FUNCTION: STARFLEETOP 0x004d06a3
unsigned tSFCPlayer::vfunction12(int param_1)
{
	for (unsigned i = 0; i < (unsigned)m_30; i++) {
		if (m_40[i] == param_1)
			return i;
	}
	return 0xffffffff;
}

// FUNCTION: STARFLEETOP 0x004d06c7
int tSFCPlayer::vfunction14(unsigned param_1)
{
	if (param_1 < 6)
		return m_40[param_1];
	return 0;
}

// FUNCTION: STARFLEETOP 0x004d06db
int tSFCPlayer::vfunction15(int param_1)
{
	int i = m_30;
	while (i-- != 0) {
		if (m_40[i] == param_1)
			return i;
	}
	return -1;
}

// FUNCTION: STARFLEETOP 0x004d0802
void tSFCPlayer::vfunction27(int param_1)
{
	m_853c = param_1;
	g_p0090bed4->fn_00522c86((float)g_008552d0[param_1]);
	g_p008cc580->vfunction6("UI", "UIType", m_853c);
}

// FUNCTION: STARFLEETOP 0x004d0844
int tSFCPlayer::vfunction28()
{
	if (!m_04)
		fn_004d0c48();
	return m_853c;
}

// FUNCTION: STARFLEETOP 0x004d085a
void tSFCPlayer::vfunction29(unsigned char param_1)
{
	m_8540 = param_1;
	g_p008cc580->vfunction6("UI", "AutoFilm", param_1);
}

// FUNCTION: STARFLEETOP 0x004d0880
char tSFCPlayer::vfunction30()
{
	if (!m_04)
		fn_004d0c48();
	return m_8540;
}

// FUNCTION: STARFLEETOP 0x004d0896
void tSFCPlayer::vfunction19(char *param_1, unsigned param_2)
{
	if (param_2 < 6 && strlen(param_1) + 1 < 0x20)
		strcpy(m_70[param_2], param_1);
}

// FUNCTION: STARFLEETOP 0x004d08cb
char *tSFCPlayer::vfunction20(unsigned param_1)
{
	if (param_1 < 6)
		return m_70[param_1];
	return 0;
}

// HYPOTHESIS: a virtual slot of the exe's analysis (vfunction21), but it takes two arguments
// while the caller in vfunction4 reaches its slot with one, so it is declared non-virtual here.
// FUNCTION: STARFLEETOP 0x004d08e2
void tSFCPlayer::fn_004d08e2(tString *param_1, unsigned param_2)
{
	if (param_2 < 6)
		m_130[param_2] = *param_1;
}

// FUNCTION: STARFLEETOP 0x004d0901
int tSFCPlayer::vfunction22(unsigned param_1)
{
	if (param_1 < 6)
		return (int)&m_130[param_1];
	return 0;
}

// FUNCTION: STARFLEETOP 0x004d091b
void tSFCPlayer::vfunction23(int param_1, int param_2)
{
	m_7f0[param_2] = param_1;
}

// FUNCTION: STARFLEETOP 0x004d092d
int tSFCPlayer::vfunction24(int param_1)
{
	return m_7f0[param_1];
}

// FUNCTION: STARFLEETOP 0x004d093b
void tSFCPlayer::vfunction25(int param_1, unsigned param_2)
{
	if (param_2 < 6)
		m_7d8[param_2] = param_1;
}

// FUNCTION: STARFLEETOP 0x004d0952
int tSFCPlayer::vfunction26(unsigned param_1)
{
	if (param_1 < 6)
		return m_7d8[param_1];
	return 0;
}

// FUNCTION: STARFLEETOP 0x004d0969
void tSFCPlayer::vfunction81(int param_1, unsigned param_2)
{
	if (param_2 < 6)
		m_58[param_2] = param_1;
}

// FUNCTION: STARFLEETOP 0x004d097d
int tSFCPlayer::vfunction82(unsigned param_1)
{
	if (param_1 < 6)
		return m_58[param_1];
	return -1;
}

// FUNCTION: STARFLEETOP 0x004d0992
void tSFCPlayer::vfunction83(int param_1, unsigned param_2)
{
	if (param_2 < 6)
		m_220[param_2] = param_1;
}

// FUNCTION: STARFLEETOP 0x004d09a9
int tSFCPlayer::vfunction84(unsigned param_1)
{
	if (param_1 < 6)
		return m_220[param_1];
	return 0;
}

// FUNCTION: STARFLEETOP 0x004d09c0
void tSFCPlayer::vfunction85(int param_1, unsigned param_2)
{
	if (param_2 < 6 && param_1 != 0)
		m_238[param_2] = *(tShipSystemMask *)param_1;
}

// FUNCTION: STARFLEETOP 0x004d09e9
tShipSystemMask *tSFCPlayer::vfunction86(unsigned param_1)
{
	if (param_1 < 6)
		return &m_238[param_1];
	return 0;
}

// FUNCTION: STARFLEETOP 0x004d0a1f
void tSFCPlayer::vfunction31(int param_1)
{
	m_84ac = param_1;
}

// FUNCTION: STARFLEETOP 0x004d0a2c
int tSFCPlayer::vfunction32()
{
	return m_84ac;
}

// FUNCTION: STARFLEETOP 0x004d0a06
void tSFCPlayer::vfunction33(int param_1)
{
	if (param_1 < 6)
		m_84b0 = param_1;
}

// FUNCTION: STARFLEETOP 0x004d0a18
int tSFCPlayer::vfunction34()
{
	return m_84b0;
}

// FUNCTION: STARFLEETOP 0x004d0a33
void tSFCPlayer::vfunction35(bool param_1)
{
	m_84b4 = param_1;
}

// FUNCTION: STARFLEETOP 0x004d0a40
char tSFCPlayer::vfunction36()
{
	return m_84b4;
}

// FUNCTION: STARFLEETOP 0x004d0a47
void tSFCPlayer::vfunction39(int param_1)
{
	m_84b8 = param_1;
}

// FUNCTION: STARFLEETOP 0x004d0a54
int tSFCPlayer::vfunction40()
{
	return m_84b8;
}

// FUNCTION: STARFLEETOP 0x004d0a5b
void tSFCPlayer::vfunction52(int param_1)
{
	m_850c = param_1;
}

// FUNCTION: STARFLEETOP 0x004d0a68
int tSFCPlayer::vfunction53()
{
	return m_850c;
}

// FUNCTION: STARFLEETOP 0x004d0a6f
void tSFCPlayer::vfunction54(int param_1)
{
	m_8510 = param_1;
}

// FUNCTION: STARFLEETOP 0x004d0a7c
char tSFCPlayer::vfunction55()
{
	return m_8510;
}

// FUNCTION: STARFLEETOP 0x004d0a83
void tSFCPlayer::vfunction56(bool param_1)
{
	m_8514 = param_1;
}

// FUNCTION: STARFLEETOP 0x004d0a90
bool tSFCPlayer::vfunction57()
{
	return m_8514;
}

// FUNCTION: STARFLEETOP 0x004d0af9
int tSFCPlayer::vfunction38()
{
	int total = 0;
	unsigned i = 0;
	if ((unsigned)m_30 > i)
	{
		char *p = m_70[0];
		do
		{
			total += vfunction37(p);
			i++;
			p += 0x20;
		} while (i < (unsigned)m_30);
	}
	return total;
}

// FUNCTION: STARFLEETOP 0x004d0ab3
int fn_004d0ab3(int param_1)
{
	tShipSpecs specs;
	return fn_0040a621(param_1, &specs) ? specs.m_5dc : 0;
}

// FUNCTION: STARFLEETOP 0x004d0b28
void tSFCPlayer::fn_004d0b28()
{
	m_04 = false;
	m_30 = 0;
	m_8538 = 0;
	m_84b0 = 0;
	m_84ac = 0;
	m_84b4 = false;
	m_852c = 0;
	m_851c = 0;
	m_8520 = 0;
	m_8524 = 0;
	m_8528 = 0;
}

// FUNCTION: STARFLEETOP 0x004d0b67
void tSFCPlayer::vfunction107(int param_1)
{
	m_851c += param_1;
	if (m_851c < 0)
		m_851c = 0;
	if (param_1 > 0) {
		vfunction105(m_8520 + param_1);
		unsigned count = m_30;
		if (count-- != 0) {
			tShipSystemMask *p = &m_238[count];
			count++;
			do {
				for (int i = 0; i < 7; i++) {
					int level = p->fn_00447d41(i);
					if (level <= 3) {
						int value = p->fn_00447d55(i);
						value += param_1;
						if (value >= g_008552b8[level])
							p->fn_00447cf9(i, level + 1);
						p->fn_00447d11(i, value);
					}
				}
				count--;
				p--;
			} while (count > 0);
		}
	}
}

// HYPOTHESIS: the object at 0x0096ed60 is also declared (with another member) inside
// tEntityMgr.cpp under another name, which this unit may not share; fn_0052daa6 maps the
// player's (m_84b8, prestige) pair to a rank. To be merged into one declaration.
class tRankTable_0096ed60
{
public:
	int fn_0052daa6(int param_1, int param_2);
};

extern tRankTable_0096ed60 gObject_0096ed60_rank;

// FUNCTION: STARFLEETOP 0x004d0c20
void tSFCPlayer::vfunction105(int param_1)
{
	m_8520 = param_1;
	m_8524 = gObject_0096ed60_rank.fn_0052daa6(m_84b8, param_1);
}

// FUNCTION: STARFLEETOP 0x004d0c5d
void tSFCPlayer::fn_004d0c5d()
{
	char name[256];
	// HYPOTHESIS: a first call with no buffer returns the setting's length.
	int length = g_p008cc580->vfunction9("Network", "PlayerName", "Player1", 0, 0);
	g_p008cc580->vfunction9("Network", "PlayerName", "Player1", name, length);
	m_08 = name;
}

// FUNCTION: STARFLEETOP 0x004d0cb6
void tSFCPlayer::fn_004d0cb6()
{
	m_853c = g_p008cc580->vfunction5("UI", "UIType", 1);
	g_p0090bed4->fn_00522c86((float)g_008552d0[m_853c]);
	m_8540 = (char)g_p008cc580->vfunction5("UI", "AutoFilm", 0);
}

// FUNCTION: STARFLEETOP 0x004d0d0a
void tSFCPlayer::vfunction94(tShipSystemMask *param_1)
{
	int iType = vfunction98();
	for (int i = 0; i < 7; i++) {
		param_1->fn_00447cf9(i, iType);
		param_1->fn_00447d29(i, g_0085529c[iType]);
		param_1->fn_00447d11(i, g_008552b4[iType]);
	}
}

// FUNCTION: STARFLEETOP 0x004d0d7e
void tSFCPlayer::vfunction99(tShipSystemMask *param_1, int param_2, int param_3, int param_4)
{
	param_1->fn_004475bc(0, param_2);
	param_1->fn_004475bc(1, param_3);
	param_1->fn_004475bc(2, param_4);
}

// FUNCTION: STARFLEETOP 0x004d0d50
void tSFCPlayer::vfunction100(unsigned param_1, int param_2, int param_3, int param_4)
{
	tShipSystemMask *p = vfunction86(param_1);
	if (p != 0) {
		vfunction99(p, param_2, param_3, param_4);
	}
}

// FUNCTION: STARFLEETOP 0x004d0e71
void tSFCPlayer::vfunction67(int param_1) { m_8534 = param_1; }
// FUNCTION: STARFLEETOP 0x004d0e7e
int tSFCPlayer::vfunction68() { return m_8534; }
// FUNCTION: STARFLEETOP 0x004d0e85
void tSFCPlayer::vfunction69()
{
	for (int ihull = 0; ihull < 0x12; ihull++) {
		for (int iclass = 0; iclass < 0x18; iclass++) {
			tListIterator_004cf9b3 erased;
			m_61a8[ihull][iclass].fn_004d1c60(&erased, m_61a8[ihull][iclass].begin(), m_61a8[ihull][iclass].end());
		}
	}
}
// FUNCTION: STARFLEETOP 0x004d1017
bool tSFCPlayer::vfunction72(unsigned param_1, int param_2)
{
	if (param_1 == 0xfffffffe || (int)param_1 >= 0x12 || param_2 == -1 || param_2 >= 0x18 || m_61a8[param_1][param_2].size() == 0)
		return false;
	return true;
}
// FUNCTION: STARFLEETOP 0x004d104f
void tSFCPlayer::vfunction73()
{
	for (unsigned ievent = 4; ievent-- > 0; )
		for (unsigned iclass = 0x14; iclass-- > 0; )
			m_8368[ievent][iclass] = 0;
}
// FUNCTION: STARFLEETOP 0x004d1073
void tSFCPlayer::vfunction74(int param_1, int param_2, int param_3)
{
	if (param_2 == -1 || param_2 >= 0x14)
		return;
	m_8368[param_1][param_2] += param_3;
	int total = m_8368[param_1][param_2];
	if (param_1 == 0)
	{
		if (total >= 0x14)
			vfunction77(2, 0, 1);
		if (total >= 0x32)
			vfunction77(2, 1, 1);
	}
	else if (param_1 == 1)
	{
		if (total >= 10)
			vfunction77(2, 2, 1);
		if (total >= 0x1e)
			vfunction77(2, 3, 1);
	}
}
// FUNCTION: STARFLEETOP 0x004d1100
int tSFCPlayer::vfunction75(int param_1, int param_2)
{
	if (param_2 == -1 || param_2 >= 0x14)
		return 0;
	return m_8368[param_1][param_2];
}
// FUNCTION: STARFLEETOP 0x004d1126
void tSFCPlayer::vfunction76()
{
	for (unsigned ievent = 4; ievent-- > 0; )
		for (unsigned iclass = 6; iclass-- > 0; )
			m_8544[ievent][iclass] = 0;
}
// FUNCTION: STARFLEETOP 0x004d114a
void tSFCPlayer::vfunction77(int param_1, int param_2, int param_3)
{
	if (param_2 < 6)
		m_8544[param_1][param_2] = param_3;
}
// FUNCTION: STARFLEETOP 0x004d116b
int tSFCPlayer::vfunction78(int param_1, int param_2)
{
	if (param_2 < 6)
		return m_8544[param_1][param_2];
	return 0;
}
// FUNCTION: STARFLEETOP 0x004d118c
void tSFCPlayer::vfunction49()
{
	for (unsigned i = 16; i-- > 0; )
		m_84cc[i] = 0;
}
// FUNCTION: STARFLEETOP 0x004d11a1
void tSFCPlayer::vfunction50(int *param_1)
{
	for (unsigned i = 16; i-- > 0; )
		m_84cc[i] = param_1[i];
}
// FUNCTION: STARFLEETOP 0x004d11c5
void tSFCPlayer::vfunction51(int *param_1)
{
	for (unsigned i = 16; i-- > 0; )
		param_1[i] = m_84cc[i];
}
// FUNCTION: STARFLEETOP 0x004d11e9
int tSFCPlayer::fn_004d11e9(int param_1)
{
	int free_slot = 0;
	for (int i = 0; i < 6; i++)
	{
		if (m_3358[i] == param_1)
			return i;
		if (m_3358[i] == 0)
			free_slot = i;
	}
	m_3358[free_slot] = param_1;
	return free_slot;
}
// FUNCTION: STARFLEETOP 0x004d1220
void tSFCPlayer::fn_004d1220()
{
	for (int i = 0; i < 6; i++)
	{
		m_3358[i] = 0;
		m_3370[i] = 0;
		for (int j = 0; j < 6; j++)
		{
			m_3388[i * 6 + j] = -1;
			m_3418[i * 6 + j] = "";
			m_39b8[i * 6 + j] = "";
			m_3f58[i * 6 + j].fn_0051da3d();
		}
	}
}
// FUNCTION: STARFLEETOP 0x004d12a2
void tSFCPlayer::vfunction111(int *param_1)
{
	vfunction108();
	int row = fn_004d11e9((int)param_1);
	m_84b8 = 0;
	m_30 = m_3370[row];
	for (int i = 0; i < 6; i++)
	{
		m_58[i] = m_3388[row * 6 + i];
		m_130[i] = m_39b8[row * 6 + i];
		m_238[i] = m_3f58[row * 6 + i];
		strcpy(m_70[i], m_3418[row * 6 + i].mCharStr());
	}
}
// FUNCTION: STARFLEETOP 0x004d137c
void tSFCPlayer::vfunction112(int param_1)
{
	int row = fn_004d11e9(param_1);
	for (int i = 0; i < 6; i++)
		m_3f58[row * 6 + i] = m_238[i];
	vfunction109();
}
// FUNCTION: STARFLEETOP 0x004d13ce
void tSFCPlayer::vfunction113(int param_1, unsigned param_2, int param_3, tString *param_4, tString *param_5)
{
	int row = fn_004d11e9(param_1);
	if (param_2 >= (unsigned)m_3370[row])
		m_3370[row] = param_2 + 1;
	int idx = row * 6 + param_2;
	m_3388[idx] = param_3;
	m_39b8[idx] = *param_5;
	m_3418[idx] = *param_4;
	m_3f58[idx].fn_00447af2(param_4->mCharStr(), 0);
}
// FUNCTION: STARFLEETOP 0x004d1449
void tSFCPlayer::vfunction114(fn_004e2b02_ship_type *param_1)
{
	fn_004e2b02_ship_type *ship = gObject_008e1060.fn_0042496f(param_1->field_1c);
	if (ship) {
		int id = ship->field_870;
		tString name(ship->vfunction42());
		tString label(ship->field_87d);
		for (int i = 0; i < 6; i++) {
			for (unsigned j = 0; j < (unsigned)m_3370[i]; j++) {
				if (m_3388[i * 6 + j] == id && m_39b8[i * 6 + j] == name && m_3418[i * 6 + j] == label) {
					m_3f58[i * 6 + j].fn_00447b7a((tObject_0042496f *)ship);
					return;
				}
			}
		}
	}
}
// FUNCTION: STARFLEETOP 0x004d1580
void tSFCPlayer::vfunction115(char param_1) { m_05 = param_1; }
// FUNCTION: STARFLEETOP 0x004d158a
void tSFCPlayer::vfunction17(int param_1, unsigned param_2)
{
	int row = fn_004d11e9(param_1);
	if (param_2 < (unsigned)m_3370[row]) {
		for (unsigned i = param_2; i < 5; i++) {
			m_3388[row * 6 + i] = m_3388[row * 6 + i + 1];
			m_3418[row * 6 + i] = m_3418[row * 6 + i + 1];
			m_39b8[row * 6 + i] = m_39b8[row * 6 + i + 1];
			m_3f58[row * 6 + i] = m_3f58[row * 6 + i + 1];
		}
		m_3370[row]--;
	}
}
// FUNCTION: STARFLEETOP 0x004d1629
void tSFCPlayer::vfunction18(int param_1)
{
	int row = fn_004d11e9(param_1);
	m_3358[row] = 0;
	m_3370[row] = 0;
	for (int j = 0; j < 6; j++)
	{
		m_3388[row * 6 + j] = -1;
		m_3418[row * 6 + j] = "";
		m_39b8[row * 6 + j] = "";
		m_3f58[row * 6 + j].fn_0051da3d();
	}
}
// FUNCTION: STARFLEETOP 0x004d0e12
int tSFCPlayer::vfunction95(int param_1) { return g_0085529c[param_1]; }
// FUNCTION: STARFLEETOP 0x004d0e20
int tSFCPlayer::vfunction96(int param_1) { return g_008552b4[param_1]; }
// FUNCTION: STARFLEETOP 0x004d0e2e
int tSFCPlayer::vfunction97(int param_1, int param_2)
{
	if (param_1 == 4 || param_1 == 5 || param_1 == 0)
		return 100;
	int lo = g_008552b4[param_1];
	int hi = g_008552b8[param_1];
	if (param_2 < lo)
		param_2 = lo;
	if (param_2 > hi)
		param_2 = hi;
	return ((param_2 - lo) * 100) / (hi - lo);
}
// FUNCTION: STARFLEETOP 0x004d0de7
int tSFCPlayer::vfunction98()
{
	if (vfunction58())
		return 3;
	return m_853c == 0 ? 3 : (m_853c == 1) + 1;
}

// FUNCTION: STARFLEETOP 0x004d0dce
void tSFCPlayer::vfunction101(tShipSystemMask *param_1)
{
	for (int i = 0; i <= 3; i++) {
		param_1->fn_00447926(i, 0);
	}
}

// FUNCTION: STARFLEETOP 0x004d0dac
void tSFCPlayer::vfunction102(unsigned param_1)
{
	tShipSystemMask *p = vfunction86(param_1);
	if (p != 0) {
		vfunction101(p);
	}
}

// FUNCTION: STARFLEETOP 0x004d0c48
void tSFCPlayer::fn_004d0c48()
{
	fn_004d0c5d();
	fn_004d0cb6();
	m_04 = true;
}

// FUNCTION: STARFLEETOP 0x004d07c9
void tSFCPlayer::vfunction5(tString *param_1)
{
	m_08 = *param_1;
	g_p008cc580->vfunction10("Network", "PlayerName", m_08.mCharStr());
}

// FUNCTION: STARFLEETOP 0x004d07b6
void *tSFCPlayer::vfunction6()
{
	if (!m_04)
		fn_004d0c48();
	return &m_08;
}

// FUNCTION: STARFLEETOP 0x004d0701
void tSFCPlayer::vfunction16(unsigned param_1)
{
	if (param_1 < (unsigned)m_30) {
		for (unsigned i = param_1; i < 5; i++) {
			unsigned next = i + 1;
			m_40[i] = m_40[next];
			m_58[i] = m_58[next];
			strcpy(m_70[i], m_70[next]);
			m_130[i] = m_130[next];
			m_220[i] = m_220[next];
			m_238[i] = m_238[next];
		}
		m_30--;
	}
}

// FUNCTION: STARFLEETOP 0x004d05ef
void tSFCPlayer::vfunction87()
{
	for (int i = 0; i < 6; i++) {
		m_238[i].fn_0051da3d();
		m_40[i] = 0;
	}
}

// FUNCTION: STARFLEETOP 0x004d04b6
void tSFCPlayer::vfunction110()
{
	g_008f5118.fn_00424d28();
	for (unsigned i = 0; i < (unsigned)vfunction10(); i++)
		g_008f5118.fn_00424d37(vfunction22(i));
}

// FUNCTION: STARFLEETOP 0x004d03a3
void tSFCPlayer::vfunction3(char *param_1, int param_2)
{
	vfunction9(0);
	vfunction110();
	if (param_1 != 0 && param_2 != -1)
		vfunction4(param_1, param_2, 0, 0, 0);
	vfunction90(0);
	vfunction92(0);
	vfunction103(0);
	vfunction105(0);
	vfunction73();
	vfunction69();
	fn_004d1220();
}
