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
