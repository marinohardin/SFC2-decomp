#include "unsorted/fn_0090bed4_Object.h"
#include "unsorted/tIndividualShipSystem.h"

// FUNCTION: STARFLEETOP 0x00522679
void __fastcall fn_00522679(char* self)
{
	*(char*)(self + 0x16b4) = 0;
}

// HYPOTHESIS: the objects the table at +0x10 names.
class tObject_00524883 {
public:
	void fn_00524883();
};

// HYPOTHESIS: a time in seconds, read as a double (tAICaptain.cpp uses it the same way).
extern double g_0090bef0;

// 0.05 in the exe's data.
extern double g_007b0250;

// FUNCTION: STARFLEETOP 0x00522681
// HYPOTHESIS: the table at +0x10 (16-byte entries: object, time double at +8) and its count at
// +0x210 are not in the header yet, so they are read by offset.
void fn_0090bed4_Object::fn_00522681()
{
	char *self = (char *) this;
	double now = g_0090bef0;
	for (int i = 0; i < *(int *) (self + 0x210); i++) {
		double *when = (double *) (self + 0x18 + i * 16);
		if (now >= *when) {
			(*(tObject_00524883 **) (self + 0x10 + i * 16))->fn_00524883();
			*when = *when + g_007b0250;
		}
	}
	if (m_16d4 != 0) {
		float *last = (float *) (self + 0x16cc);
		if ((float) g_0090bef0 - *last > m_16d0) {
			*last = (float) g_0090bef0;
			fn_00523445(m_16d4,m_16d5);
		}
	}
}

// HYPOTHESIS: a member of the object above, called on the same object.
void __fastcall fn_005227c1(char* self);

// FUNCTION: STARFLEETOP 0x0052277b
// HYPOTHESIS: the table at +0x10 is a tObject_00523644; its removal call is not reachable from here yet.
void __fastcall fn_0052277b(char* self, int, void* param_1)
{
	if (param_1 != 0) {
		int count = *(int*)(self + 0x210);
		for (int i = 0; i < count; i++) {
			if (*(void**)(self + 0x10 + i * 16) == param_1) {
				break;
			}
		}
		operator delete(param_1);
		fn_005227c1(self);
	}
}

// FUNCTION: STARFLEETOP 0x005227c1
void __fastcall fn_005227c1(char* self)
{
	int i = 0;
	if (*(int*)(self + 0x210) >= 0) {
		*(double*)(self + 8) = g_007b0250 / (double)*(int*)(self + 0x210);
		if (*(int*)(self + 0x210) > 0) {
			double* p = (double*)(self + 0x18);
			double t = g_0090bef0;
			do {
				*p = t;
				t = t + *(double*)(self + 8);
				i++;
				p += 2;
			} while (i < *(int*)(self + 0x210));
		}
	}
}

// HYPOTHESIS: the object 0x00523543 returns for a fleet ID (tObject_0051c7af in fn_00523000.cpp,
// which has no header yet).
class tObject_0051c668 {
public:
	void fn_0051c668(int param_1);
};

// HYPOTHESIS: an object in the table of 0x4000 object pointers indexed by ID, with a virtual
// at slot 4 that takes no arguments.
class tObject_008e1090 {
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
};

extern tObject_008e1090 *g_008e1090[0x4000];

// FUNCTION: STARFLEETOP 0x005229b9
// HYPOTHESIS: the header declares this member as void; the original returns 0 in al, so it stands
// as a fastcall until the header's return type is changed.
char __fastcall fn_005229b9(fn_0090bed4_Object *self, int, tEntity *pEntity)
{
	vfunction33_result *pOwner = (vfunction33_result *)pEntity;
	tObject_0051c668 *pObject = (tObject_0051c668 *)self->fn_00523543(pOwner->field_60);
	if (pObject != 0) {
		int value = pOwner->fn_00410223();
		pObject->fn_0051c668(value);
	}
	int id = pOwner->fn_00410231();
	tObject_008e1090 *p = (id < 0 || id >= 0x4000) ? 0 : g_008e1090[id];
	if (p != 0)
		p->vfunction4();
	return 0;
}

