//	fn_00523000.cpp: functions of the 0x00523000 page (original file unknown).

#include "unsorted/fn_0090bed4_Object.h"
#include "unsorted/tObject_0090be60.h"
#include "unsorted/tApplication.h"
#include "unsorted/tAIData.h"
#include "unsorted/tEntity.h"
#include "unsorted/fn_004e2b02_ship_type.h"

// HYPOTHESIS: the object 0x00523543 and 0x00523574 return for an ID; tAICaptain.cpp declares
// what is likely the same class as tObject_004b94dc.
class tObject_0051c7af
{
public:
	void fn_0051c76f(int param_1, int param_2);
	void fn_0051c7af(int param_1);
	void fn_0051c7e3(int param_1);
	bool fn_0051bf99(int param_1, int param_2);
	// HYPOTHESIS: returns the object for the ship (0x005235c8 passes its result through).
	tObject_005235a7 *fn_0051c210(fn_004e2b02_ship_type *pShip);
};

// FUNCTION: STARFLEETOP 0x00523027
bool fn_0090bed4_Object::fn_00523027(int param_1, int param_2)
{
	if (param_2 == -1) {
		for (std::list<int>::iterator it = m_16c0.begin(); it != m_16c0.end(); it++) {
			tObject_0051c7af *pObject = fn_00523574(*it);
			if (pObject != NULL)
				pObject->fn_0051c7af(param_1);
		}
	}
	else {
		tObject_0051c7af *pObject = fn_00523543(param_2);
		if (pObject != NULL) {
			pObject->fn_0051c7af(param_1);
			return true;
		}
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x00523086
bool fn_0090bed4_Object::fn_00523086(int param_1, int param_2)
{
	tObject_0051c7af *pObject = fn_00523543(param_1);
	if (pObject != NULL) {
		switch (gObject_0090be60->fn_00427581(param_1, param_2)) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 7:
			pObject->fn_0051c7e3(param_2);
			break;
		case 6:
			pObject->fn_0051c76f(param_2, 1);
			break;
		}
	}
	return true;
}

// FUNCTION: STARFLEETOP 0x005230dd
// HYPOTHESIS: the fourth argument is unused here (ret 0x10); its type is unknown.
bool fn_0090bed4_Object::fn_005230dd(int param_1, int param_2, int param_3, int param_4)
{
	tObject_0051c7af *pObject = fn_00523543(param_1);
	if (pObject == NULL)
		return false;
	return pObject->fn_0051bf99(param_2, param_3);
}

// FUNCTION: STARFLEETOP 0x00523100
void fn_0090bed4_Object::fn_00523100()
{
	m_218.fn_00520e30();
}

// HYPOTHESIS: a table of 0x4000 object pointers indexed by ID; its element type is unknown.
extern void *g_008e1090[0x4000];

// FUNCTION: STARFLEETOP 0x0052310b
// HYPOTHESIS: a member of this class: ecx passes through untouched to 0x0052315a, which takes
// three stack arguments like this function (ret 0xc).
char fn_0090bed4_Object::fn_0052310b(int param_1, int param_2, int param_3)
{
	void *p1 = (param_1 < 0 || param_1 >= 0x4000) ? NULL : g_008e1090[param_1];
	void *p2 = (param_2 < 0 || param_2 >= 0x4000) ? NULL : g_008e1090[param_2];
	if (p1 == NULL || p2 == NULL)
		return 0;
	return fn_0052315a(p1, p2, param_3);
}

// HYPOTHESIS: tAICaptain (declared only in tAICaptain.cpp) is what 0x0051c1f3 returns.
class tObject_0051e3ac
{
public:
	bool fn_0051e3ac(int param_1, int param_2);
};

class tObject_0051c1f3
{
public:
	tObject_0051e3ac *fn_0051c1f3(int param_1);
};

// FUNCTION: STARFLEETOP 0x0052315a
// HYPOTHESIS: the two arguments are objects whose +0x60 and +0x1c fields are IDs.
char fn_0090bed4_Object::fn_0052315a(void *param_1, void *param_2, int param_3)
{
	char result = 0;
	tObject_0051c1f3 *pObject = (tObject_0051c1f3 *) fn_00523543(*(int *) ((char *) param_1 + 0x60));
	if (pObject != NULL) {
		tObject_0051e3ac *pCaptain = pObject->fn_0051c1f3(*(int *) ((char *) param_1 + 0x1c));
		if (pCaptain != NULL) {
			int id = *(int *) ((char *) param_2 + 0x1c);
			result = pCaptain->fn_0051e3ac(id, param_3);
		}
	}
	return result;
}

// HYPOTHESIS: the key names of two path settings; the pointers sit at 0x008591f0 and 0x008591f4.
extern const char *g_p008591f0;
extern const char *g_p008591f4;

// FUNCTION: STARFLEETOP 0x00523198
void fn_0090bed4_Object::fn_00523198()
{
	int len = g_p008cc580->vfunction9("AI", g_p008591f0, 0, 0, 0);
	m_16b8 = new char[len + 1];
	g_p008cc580->vfunction9("AI", g_p008591f0, 0, m_16b8, len);
	len = g_p008cc580->vfunction9("AI", g_p008591f4, 0, 0, 0);
	m_16bc = new char[len + 1];
	g_p008cc580->vfunction9("AI", g_p008591f4, 0, m_16bc, len);
}

// HYPOTHESIS: AI flags that 0x00523222 resets; tAICaptain and tAIHelmOfficer read some of them.
// 0x0096ea1a and 0x0096ea1c are cleared with 16-bit stores, so they are shorts.
extern bool g_0096ea15;
extern bool g_0096ea16;
extern bool g_0096ea17;
extern bool g_0096ea18;
extern bool g_0096ea19;
extern short g_0096ea1a;
extern short g_0096ea1c;
extern char g_0085922a;
extern char g_b0085922b;

// FUNCTION: STARFLEETOP 0x00523222
void fn_0090bed4_Object::fn_00523222()
{
	fn_00523198();
	m_16b5 = 1;
	g_0096ea17 = false;
	g_0096ea18 = false;
	g_0096ea19 = false;
	g_0096ea1a = 0;
	g_0096ea1c = 0;
	g_0085922a = 1;
	g_b0085922b = 1;
	g_0096ea15 = false;
	g_0096ea16 = false;
	memset(m_16dc, 0xff, 100);
}

// HYPOTHESIS: the objects in the table at 0x008e1090 (likely entities): 0x005915ea returns
// their AI data, 0x00410239 hands them a new one.
class tObject_005915ea
{
public:
	tAIData *fn_005915ea();
	void fn_00410239(tAIData *pData);
};

// FUNCTION: STARFLEETOP 0x0052327e
tAIData *fn_0090bed4_Object::fn_0052327e(int param_1, char param_2)
{
	tObject_005915ea *pObject = (param_1 < 0 || param_1 >= 0x4000) ? NULL : (tObject_005915ea *) g_008e1090[param_1];
	if (pObject != NULL) {
		tAIData *pData = pObject->fn_005915ea();
		if (pData != NULL) {
			fn_00523428(pData->m_10);
			if (!param_2 && m_16dc[pData->m_10 * 12] != 0)
				m_16dc[pData->m_10 * 12] = 0;
			return pData;
		}
		char slot = fn_00523370(param_1, param_2);
		if (slot != -1) {
			pData = new tAIData(slot, param_1);
			pObject->fn_00410239(pData);
			return pData;
		}
	}
	return NULL;
}

// FUNCTION: STARFLEETOP 0x00523344
// HYPOTHESIS: the table at +0x16dc holds 12-byte entries whose +8 field is an ID, -1 when free.
void fn_0090bed4_Object::fn_00523344(unsigned char param_1)
{
	if (*(int *) &m_16dc[param_1 * 12 + 8] != -1) {
		m_16d8--;
		fn_005233cb();
	}
	*(int *) &m_16dc[param_1 * 12 + 8] = -1;
}

// FUNCTION: STARFLEETOP 0x00523370
// HYPOTHESIS: takes the first free entry of the table at +0x16dc for the ID and returns its
// index; -1 when the ID is -1 or the table is full.
char fn_0090bed4_Object::fn_00523370(int param_1, char param_2)
{
	if (param_1 != -1) {
		for (int i = 0; i < 100; i++) {
			if (*(int *) &m_16dc[i * 12 + 8] == -1) {
				m_16d8++;
				fn_005233cb();
				*(int *) &m_16dc[i * 12 + 8] = param_1;
				m_16dc[i * 12] = param_2;
				fn_00523428(i);
				return i;
			}
		}
	}
	return -1;
}

// 0.1f, 0.25f and 0.75f in the exe's data.
extern float g_007b0248;
extern float g_007b0244;
extern float g_007b0240;

// FUNCTION: STARFLEETOP 0x005233cb
// HYPOTHESIS: sets three tiers from the count at +0x16d8.
void fn_0090bed4_Object::fn_005233cb()
{
	if (m_16d8 >= 90) {
		m_16d0 = g_007b0248;
		m_16d4 = 5;
		m_16d5 = 0;
	} else if (m_16d8 >= 75) {
		m_16d0 = g_007b0244;
		m_16d4 = 3;
		m_16d5 = 0;
	} else if (m_16d8 >= 50) {
		m_16d0 = g_007b0240;
		m_16d4 = 1;
		m_16d5 = 1;
	}
}

// HYPOTHESIS: a time in seconds, read as a double (tAICaptain.cpp uses it the same way).
extern double g_0090bef0;

// FUNCTION: STARFLEETOP 0x00523428
// HYPOTHESIS: stamps the entry's +4 field with the current time.
void fn_0090bed4_Object::fn_00523428(unsigned char param_1)
{
	double now = g_0090bef0;
	*(float *) &m_16dc[param_1 * 12 + 4] = now;
}

// 0.5f in the exe's data.
extern float g_007b024c;

// FUNCTION: STARFLEETOP 0x00523445
// HYPOTHESIS: frees up to param_1 used entries older than half a second, those with the +0
// flag set first; with param_2 clear, then any old entry.
void fn_0090bed4_Object::fn_00523445(unsigned char param_1, char param_2)
{
	float now = g_0090bef0;
	unsigned char count = 0;
	unsigned char freed = 0;
	unsigned char i;
	for (i = 0; count < m_16d8; i++) {
		if (freed >= param_1)
			return;
		if (i >= 100)
			break;
		if (*(int *) &m_16dc[i * 12 + 8] != -1) {
			count++;
			if (m_16dc[i * 12] != 0 && now - *(float *) &m_16dc[i * 12 + 4] > g_007b024c) {
				fn_00523344(i);
				freed++;
			}
		}
	}
	if (freed < param_1 && !param_2) {
		count = 0;
		for (i = 0; count < m_16d8; i++) {
			if (freed >= param_1)
				return;
			if (i >= 100)
				return;
			if (*(int *) &m_16dc[i * 12 + 8] != -1) {
				count++;
				if (now - *(float *) &m_16dc[i * 12 + 4] > g_007b024c) {
					fn_00523344(i);
					freed++;
				}
			}
		}
	}
}

// HYPOTHESIS: the team list 0x00426f1a returns (tObject_0090be60.h types it as a vector);
// 0x00426e18 returns an ID from it, likely its leader's.
class tObject_00426e18
{
public:
	int fn_00426e18();
};

// FUNCTION: STARFLEETOP 0x00523543
tObject_0051c7af *fn_0090bed4_Object::fn_00523543(int param_1)
{
	if (gObject_0090be60 != NULL) {
		tObject_00426e18 *pList = (tObject_00426e18 *) gObject_0090be60->fn_00426f1a(param_1);
		if (pList != NULL)
			return fn_00523574(pList->fn_00426e18());
	}
	return NULL;
}

// FUNCTION: STARFLEETOP 0x00523574
// HYPOTHESIS: the table entry is returned only when bit 0x800 of its +0x18 field is set.
tObject_0051c7af *fn_0090bed4_Object::fn_00523574(int param_1)
{
	void *pObject = (param_1 < 0 || param_1 >= 0x4000) ? NULL : g_008e1090[param_1];
	if (pObject == NULL)
		return NULL;
	return (*(unsigned long *) ((char *) pObject + 0x18) & 0x800) ? (tObject_0051c7af *) pObject : NULL;
}

// FUNCTION: STARFLEETOP 0x005235a7
tObject_005235a7 *fn_0090bed4_Object::fn_005235a7(int param_1)
{
	fn_004e2b02_ship_type *pShip = gObject_008e1060.fn_0042496f(param_1);
	// MATCH-ONLY: the null path returns the lookup's own (null) result; a literal NULL adds xor eax, eax.
	tObject_005235a7 *pResult = (tObject_005235a7 *) pShip;
	if (pShip != NULL)
		pResult = fn_005235c8(pShip);
	return pResult;
}

// FUNCTION: STARFLEETOP 0x005235c8
tObject_005235a7 *fn_0090bed4_Object::fn_005235c8(fn_004e2b02_ship_type *pShip)
{
	tObject_0051c7af *pObject = fn_00523543(pShip->field_60);
	// MATCH-ONLY: the null path returns the lookup's own (null) result; a literal NULL adds xor eax, eax.
	tObject_005235a7 *pResult = (tObject_005235a7 *) pObject;
	if (pObject != NULL)
		pResult = pObject->fn_0051c210(pShip);
	return pResult;
}

// HYPOTHESIS: two times in seconds, read as doubles; the current one is copied into the other.
extern double g_00c7c150;
extern double g_0096e9c0;

// FUNCTION: STARFLEETOP 0x005235e5
void fn_0090bed4_Object::fn_005235e5(fn_0090bed4_Object *pThis, char param_2, int param_3, int param_4, int param_5)
{
	if (pThis->m_16b6 == param_2 && param_3 == 1 && param_4 == 1) {
		g_0096e9c0 = g_00c7c150;
		if (pThis->m_1688 != NULL)
			*pThis->m_1688 = 1;
		pThis->m_16b6 = -1;
		pThis->m_168c.fn_00521d73(param_5);
		if (pThis->m_16b4)
			pThis->fn_00522f1e();
	}
}

// HYPOTHESIS: a 16-byte element whose constructor is 0x005224e8; its fields are unknown.
class tObject_005224e8
{
public:
	tObject_005224e8();

	char m_pad0000[0x10];
};

// HYPOTHESIS: 0x00523644 constructs 32 of the elements above and clears the count after them.
class tObject_00523644
{
public:
	tObject_00523644();
	void fn_00523668(int index);

	tObject_005224e8 m_0000[32];
	int m_200;
};

// FUNCTION: STARFLEETOP 0x00523644
tObject_00523644::tObject_00523644()
{
	m_200 = 0;
}

// FUNCTION: STARFLEETOP 0x00523668
void tObject_00523644::fn_00523668(int index)
{
	for (int i = index + 1; i < m_200; i++)
		m_0000[i - 1] = m_0000[i];
	m_200--;
}

#include "unsorted/tObject_005237dc.h"
















// FUNCTION: STARFLEETOP 0x005236d8
tObject_005236d8::tObject_005236d8()
	: m_00(-1)
{
}

// HYPOTHESIS: a receiver whose first virtual takes the amount handed to it.
class tObject_005236ea_Target
{
public:
	virtual void vfunction0(float amount);
};

// HYPOTHESIS: a 12-byte entry: the amount it takes, a flag that refuses part amounts, its receiver.
struct tObject_005236ea_Entry
{
	tObject_005236ea_Entry();
	tObject_005236ea_Entry(float amount, bool partial, tObject_005236ea_Target *pTarget);

	float m_00;
	bool m_04;
	tObject_005236ea_Target *m_08;
};

// HYPOTHESIS: 45 entries after a 4-byte field, then their count at +0x220.
class tObject_005236ea
{
public:
	float fn_005236ea(float amount);
	void fn_0052375c(float amount, bool partial, tObject_005236ea_Target *pTarget);
	void fn_005237a1();

	int m_00;
	tObject_005236ea_Entry m_04[45];
	int m_220;
};

// FUNCTION: STARFLEETOP 0x005236ea
float tObject_005236ea::fn_005236ea(float amount)
{
	int i = 0;
	if (0 < m_220) {
		tObject_005236ea_Entry *pEntry = m_04;
		do {
			if (amount <= 0.0f)
				break;
			if (amount < pEntry->m_00) {
				if (!pEntry->m_04) {
					pEntry->m_08->vfunction0(amount);
					amount = 0.0f;
				}
			} else {
				pEntry->m_08->vfunction0(pEntry->m_00);
				amount -= pEntry->m_00;
			}
			i++;
			pEntry++;
		} while (i < m_220);
	}
	return amount;
}

// FUNCTION: STARFLEETOP 0x0052375c
void tObject_005236ea::fn_0052375c(float amount, bool partial, tObject_005236ea_Target *pTarget)
{
	tObject_005236ea_Entry entry(amount, partial, pTarget);
	if (m_220 < 45) {
		m_04[m_220] = entry;
		m_220++;
	}
}

// FUNCTION: STARFLEETOP 0x005237a1
void tObject_005236ea::fn_005237a1()
{
	m_220 = 0;
}

// FUNCTION: STARFLEETOP 0x005237a9
tObject_005236ea_Entry::tObject_005236ea_Entry()
{
	m_04 = false;
	m_08 = 0;
	m_00 = 0.0f;
}

// FUNCTION: STARFLEETOP 0x005237b8
tObject_005236ea_Entry::tObject_005236ea_Entry(float amount, bool partial, tObject_005236ea_Target *pTarget)
{
	m_00 = amount;
	m_04 = partial;
	m_08 = pTarget;
}


// FUNCTION: STARFLEETOP 0x005237d1
tObject_005237d1::tObject_005237d1()
{
	m_00 = true;
	m_04 = 0.0f;
}

// FUNCTION: STARFLEETOP 0x005237dc
tObject_005237dc::tObject_005237dc(int aShipID)
	: m_d8(false)
	, m_140(aShipID)
	, m_148(0)
	, m_14c(0)
{
	m_34b0 = 0;
	m_34cc.m_00 = false;
	m_34cc.m_04 = 0;
	m_34cc.m_08 = 0;
	m_34cc.m_0c = 0;
	m_34dc = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			m_150[i * 6 + j].m_00 = j;
		}
		m_00[i] = 0.0f;
	}
	for (int k = 0; k < 25; k++) {
		m_dc[k] = 0;
		m_10[k].m_00 = false;
		m_10[k].m_04 = 0.0f;
	}
}

// FUNCTION: STARFLEETOP 0x0052389f
tObject_005237dc::~tObject_005237dc()
{
}

// FUNCTION: STARFLEETOP 0x005238aa
int tObject_005237dc::fn_005238aa(void *aValue)
{
	int index = m_34cc.size();
	m_34cc.fn_00571a69(aValue);
	return index;
}

// FUNCTION: STARFLEETOP 0x005238d7
void tObject_005237dc::fn_005238d7(int aColumn, float amount, bool partial, int index, int *pSelector)
{
	// HYPOTHESIS: tObject_005236d8 and tObject_005236ea are one 0x224-byte class (an int, then
	// 45 entries and their count); 0x0052375c is called on the elements of m_150.
	if (pSelector) {
		((tObject_005236ea *)&m_150[pSelector[2] * 6 + aColumn])->fn_0052375c(amount, partial, (tObject_005236ea_Target *)m_34cc.m_04[index]);
	} else {
		for (int i = 0; i < 4; i++) {
			((tObject_005236ea *)&m_150[i * 6 + aColumn])->fn_0052375c(amount, partial, (tObject_005236ea_Target *)m_34cc.m_04[index]);
		}
	}
}

// HYPOTHESIS: the object slot 94 of a ship's 0xad4 object returns; slot 108 (offset 0x1ac) tells
// whether slot 95 (offset 0x178) holds a value to add.
class tObject_00523967_member94
{
public:
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual void vfunction12();
	virtual void vfunction13();
	virtual void vfunction14();
	virtual void vfunction15();
	virtual void vfunction16();
	virtual void vfunction17();
	virtual void vfunction18();
	virtual void vfunction19();
	virtual void vfunction20();
	virtual void vfunction21();
	virtual void vfunction22();
	virtual void vfunction23();
	virtual void vfunction24();
	virtual void vfunction25();
	virtual void vfunction26();
	virtual void vfunction27();
	virtual void vfunction28();
	virtual void vfunction29();
	virtual void vfunction30();
	virtual void vfunction31();
	virtual void vfunction32();
	virtual void vfunction33();
	virtual void vfunction34();
	virtual void vfunction35();
	virtual void vfunction36();
	virtual void vfunction37();
	virtual void vfunction38();
	virtual void vfunction39();
	virtual void vfunction40();
	virtual void vfunction41();
	virtual void vfunction42();
	virtual void vfunction43();
	virtual void vfunction44();
	virtual void vfunction45();
	virtual void vfunction46();
	virtual void vfunction47();
	virtual void vfunction48();
	virtual void vfunction49();
	virtual void vfunction50();
	virtual void vfunction51();
	virtual void vfunction52();
	virtual void vfunction53();
	virtual void vfunction54();
	virtual void vfunction55();
	virtual void vfunction56();
	virtual void vfunction57();
	virtual void vfunction58();
	virtual void vfunction59();
	virtual void vfunction60();
	virtual void vfunction61();
	virtual void vfunction62();
	virtual void vfunction63();
	virtual void vfunction64();
	virtual void vfunction65();
	virtual void vfunction66();
	virtual void vfunction67();
	virtual void vfunction68();
	virtual void vfunction69();
	virtual void vfunction70();
	virtual void vfunction71();
	virtual void vfunction72();
	virtual void vfunction73();
	virtual void vfunction74();
	virtual void vfunction75();
	virtual void vfunction76();
	virtual void vfunction77();
	virtual void vfunction78();
	virtual void vfunction79();
	virtual void vfunction80();
	virtual void vfunction81();
	virtual void vfunction82();
	virtual void vfunction83();
	virtual void vfunction84();
	virtual void vfunction85();
	virtual void vfunction86();
	virtual void vfunction87();
	virtual void vfunction88();
	virtual void vfunction89();
	virtual void vfunction90();
	virtual void vfunction91();
	virtual void vfunction92();
	virtual void vfunction93();
	virtual void vfunction94();
	virtual float vfunction95();
	virtual void vfunction96();
	virtual void vfunction97();
	virtual void vfunction98();
	virtual void vfunction99();
	virtual void vfunction100();
	virtual void vfunction101();
	virtual void vfunction102();
	virtual void vfunction103();
	virtual void vfunction104();
	virtual void vfunction105();
	virtual void vfunction106();
	virtual void vfunction107();
	virtual bool vfunction108();
};

// HYPOTHESIS: the object slot 96 of a ship's 0xad4 object returns, likely the class tAICaptain.cpp
// declares as tObject_0042496f_member96 (only there); slot 102 (offset 0x194) returns a count
// and fills a byte, slot 101 (offset 0x190) an integer to add.
class tObject_00523967_member96
{
public:
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual void vfunction12();
	virtual void vfunction13();
	virtual void vfunction14();
	virtual void vfunction15();
	virtual void vfunction16();
	virtual void vfunction17();
	virtual void vfunction18();
	virtual void vfunction19();
	virtual void vfunction20();
	virtual void vfunction21();
	virtual void vfunction22();
	virtual void vfunction23();
	virtual void vfunction24();
	virtual void vfunction25();
	virtual void vfunction26();
	virtual void vfunction27();
	virtual void vfunction28();
	virtual void vfunction29();
	virtual void vfunction30();
	virtual void vfunction31();
	virtual void vfunction32();
	virtual void vfunction33();
	virtual void vfunction34();
	virtual void vfunction35();
	virtual void vfunction36();
	virtual void vfunction37();
	virtual void vfunction38();
	virtual void vfunction39();
	virtual void vfunction40();
	virtual void vfunction41();
	virtual void vfunction42();
	virtual void vfunction43();
	virtual void vfunction44();
	virtual void vfunction45();
	virtual void vfunction46();
	virtual void vfunction47();
	virtual void vfunction48();
	virtual void vfunction49();
	virtual void vfunction50();
	virtual void vfunction51();
	virtual void vfunction52();
	virtual void vfunction53();
	virtual void vfunction54();
	virtual void vfunction55();
	virtual void vfunction56();
	virtual void vfunction57();
	virtual void vfunction58();
	virtual void vfunction59();
	virtual void vfunction60();
	virtual void vfunction61();
	virtual void vfunction62();
	virtual void vfunction63();
	virtual void vfunction64();
	virtual void vfunction65();
	virtual void vfunction66();
	virtual void vfunction67();
	virtual void vfunction68();
	virtual void vfunction69();
	virtual void vfunction70();
	virtual void vfunction71();
	virtual void vfunction72();
	virtual void vfunction73();
	virtual void vfunction74();
	virtual void vfunction75();
	virtual void vfunction76();
	virtual void vfunction77();
	virtual void vfunction78();
	virtual void vfunction79();
	virtual void vfunction80();
	virtual void vfunction81();
	virtual void vfunction82();
	virtual void vfunction83();
	virtual void vfunction84();
	virtual void vfunction85();
	virtual void vfunction86();
	virtual void vfunction87();
	virtual void vfunction88();
	virtual void vfunction89();
	virtual void vfunction90();
	virtual void vfunction91();
	virtual void vfunction92();
	virtual void vfunction93();
	virtual void vfunction94();
	virtual void vfunction95();
	virtual void vfunction96();
	virtual void vfunction97();
	virtual void vfunction98();
	virtual void vfunction99();
	virtual void vfunction100();
	virtual int vfunction101();
	virtual int vfunction102(char *aOut);
};

// FUNCTION: STARFLEETOP 0x00523967
float tObject_005237dc::fn_00523967()
{
	tObject_0042496f *ship = (tObject_0042496f *)gObject_008e1060.fn_0042496f(m_140);
	if (ship == NULL)
		return 0.0f;
	float total = 0.0f;
	tObject_00523967_member94 *first = ship->field_0xad4->vfunction94();
	if (first != NULL && first->vfunction108())
		total = first->vfunction95();
	total = ship->field_0xad4->vfunction47() + total;
	total = ship->field_0xad4->vfunction48() + total;
	total = ship->field_0xad4->vfunction49() + total;
	tObject_0043c0f2 *second = ship->field_0xad4->vfunction80();
	if (second != NULL && second->fn_0043bfc1())
		total = second->fn_0043beed() + total;
	tObject_00523967_member96 *third = (tObject_00523967_member96 *)ship->field_0xad4->vfunction96();
	m_144 = false;
	if (third != NULL) {
		char kind;
		int count = third->vfunction102(&kind);
		if (count >= 1 && count <= 6) {
			m_144 = true;
			return third->vfunction101() + total;
		}
	}
	return total;
}

// FUNCTION: STARFLEETOP 0x00523ad3
void tObject_005237dc::fn_00523ad3()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			((tObject_005236ea *)&m_150[i * 6 + j])->fn_005237a1();
		}
	}
}

// FUNCTION: STARFLEETOP 0x00523afd
// HYPOTHESIS: the selector and the object 0x005235a7 returns are declared only in tAICaptain.cpp
// and tFleetEntry.cpp; their +0x8 field (the row) and +0x3a44 flag are read through offsets.
void tObject_005237dc::fn_00523afd(tObject_00528a11 *aSelector)
{
	tObject_0042496f *ship = (tObject_0042496f *)gObject_008e1060.fn_0042496f(m_140);
	int i = 0;
	if (ship == NULL)
		return;
	float remaining = ship->field_0xad4->vfunction2();
	remaining -= fn_00523967();
	if (m_144)
		m_14c = 3;
	if (m_14c == 0)
		m_14c = 1;
	tObject_005235a7 *captain = g_p0090bed4->fn_005235a7(m_140);
	if (captain != NULL && *((char *)captain + 0x3a44))
		m_14c = 4;
	fn_00523c59();
	if (m_34dc != NULL) {
		float request = m_34dc->m_14.vfunction3();
		if (request <= remaining) {
			m_34dc->m_14.vfunction0(request);
			m_34dc->m_14.vfunction1();
			remaining -= request;
		}
	}
	for (; i < m_34b0; i++) {
		if (remaining <= 0.0f)
			break;
		if (m_34b4[i] == 5)
			remaining = fn_00523e72(remaining);
		else
			remaining = ((tObject_005236ea *)&m_150[((int *)aSelector)[2] * 6 + m_34b4[i]])->fn_005236ea(remaining);
	}
}

// FUNCTION: STARFLEETOP 0x00523c41
void tObject_005237dc::fn_00523c41(int aMode)
{
	if (m_14c != 3)
		m_14c = aMode;
}

// FUNCTION: STARFLEETOP 0x00523c59
void tObject_005237dc::fn_00523c59()
{
	if (m_14c == 0)
		return;
	if (m_148 == m_14c)
		return;
	m_148 = m_14c;
	m_14c = 0;
	m_34b0 = 0;
	switch (m_148) {
	case 1:
		m_34b4[m_34b0++] = 4;
		m_34b4[m_34b0++] = 0;
		m_34b4[m_34b0++] = 1;
		m_34b4[m_34b0++] = 3;
		m_34b4[m_34b0++] = 2;
		m_34b4[m_34b0++] = 5;
		break;
	case 2:
		m_34b4[m_34b0++] = 4;
		m_34b4[m_34b0++] = 3;
		m_34b4[m_34b0++] = 0;
		m_34b4[m_34b0++] = 1;
		m_34b4[m_34b0++] = 5;
		m_34b4[m_34b0++] = 2;
		break;
	case 3:
		m_34b4[m_34b0++] = 1;
		m_34b4[m_34b0++] = 2;
		m_34b4[m_34b0++] = 4;
		m_34b4[m_34b0++] = 0;
		m_34b4[m_34b0++] = 3;
		m_34b4[m_34b0++] = 5;
		break;
	case 4:
		m_34b4[m_34b0++] = 4;
		m_34b4[m_34b0++] = 5;
		m_34b4[m_34b0++] = 3;
		m_34b4[m_34b0++] = 0;
		m_34b4[m_34b0++] = 1;
		m_34b4[m_34b0++] = 2;
		break;
	}
}

// FUNCTION: STARFLEETOP 0x00523e72
float tObject_005237dc::fn_00523e72(float amount)
{
	tObject_0042496f *ship = (tObject_0042496f *)gObject_008e1060.fn_0042496f(m_140);
	if (amount > 0.0f && m_d8 > 0) {
		if (m_148 == 4)
			return 0.0f;
		if (ship->field_0x870 == 0x13)
			amount = 100.0f;
		fn_00523ffd(amount, true);
		bool done = false;
		while (!done && amount > 0.0f) {
			float best = -1.0f;
			char index = -1;
			for (int i = 0; i < m_d8; i++) {
				if (m_10[i].m_00 && best < m_10[i].m_04) {
					best = m_10[i].m_04;
					index = i;
				}
			}
			if (index >= 0) {
				float want = m_dc[index]->vfunction3();
				if (want == -1.0f || amount < want) {
					m_dc[index]->vfunction0(amount);
					amount = 0.0f;
				} else {
					if (!m_dc[index]->vfunction0(want))
						m_10[index].m_00 = false;
					amount -= want;
				}
				if (amount != 0.0f)
					fn_00523ffd(amount, false);
			} else {
				done = true;
			}
		}
	}
	for (int i = 0; i < m_d8; i++)
		m_dc[i]->vfunction1();
	return amount;
}

// FUNCTION: STARFLEETOP 0x00523ffd
void tObject_005237dc::fn_00523ffd(float amount, bool aFirst)
{
	for (int i = 0; i < m_d8; i++) {
		if (aFirst || m_10[i].m_00) {
			float weight = m_dc[i]->vfunction4();
			weight = m_dc[i]->vfunction5() + weight;
			m_10[i].m_04 = weight;
			float b = m_dc[i]->vfunction2();
			if (weight <= 0.0f || b < 0.0f || b > amount)
				m_10[i].m_00 = false;
			else
				m_10[i].m_00 = true;
		}
	}
}
