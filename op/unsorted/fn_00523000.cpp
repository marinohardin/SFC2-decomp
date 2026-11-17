//	fn_00523000.cpp: functions of the 0x00523000 page (original file unknown).

#include "unsorted/fn_0090bed4_Object.h"
#include "unsorted/tObject_0090be60.h"

// HYPOTHESIS: the object 0x00523543 and 0x00523574 return for an ID; tAICaptain.cpp declares
// what is likely the same class as tObject_004b94dc.
class tObject_0051c7af
{
public:
	void fn_0051c76f(int param_1, int param_2);
	void fn_0051c7af(int param_1);
	void fn_0051c7e3(int param_1);
	bool fn_0051bf99(int param_1, int param_2);
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
