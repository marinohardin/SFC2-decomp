// Unit:      unsorted/tEntity.cpp
// Evidence:  class (original file unknown)

#include "unsorted/tEntity.h"

// From the SDK's sfcstreams.h.
extern void ArchiveInt32TM(int32 &num, tSFCStream &s, int32u mode, char *text);
extern void ArchiveInt32uTM(int32u &num, tSFCStream &s, int32u mode, char *text);
extern void ArchiveITruthTM(iTruth &num, tSFCStream &s, int32u mode, char *text);
extern void ArchiveFloatTM(float &num, tSFCStream &s, int32u mode, char *text);
extern void ArchiveTPoint3TM(tPoint3 &num, tSFCStream &s, int32u mode, char *text);
extern void ArchiveTitleTM(char *title, tSFCStream &s, int32u mode);
extern void ArchiveSeperatorTM(tSFCStream &s, int32u mode);
extern void IncrementStreamTab(tSFCStream &s);
extern void DecrementStreamTab(tSFCStream &s);

// A 12-byte object with the vtable at 0x00795d10, built on the stack in 0x0041018e and
// filled by 0x00434d0e.
// HYPOTHESIS: the layout and the virtual are inferred from 0x0041018e only.
class tObject_00795d10
{
public:
	virtual void vfunction1();
	void fn_00434d0e(int aValue);

	iTruth fField04;              // +0x04
	int32 fField08;               // +0x08
};

// The original stores every float zero with fldz/fstp; this profile folds them into integer
// stores. Store order and offsets otherwise follow the original.
// FUNCTION: STARFLEETOP 0x0040fefb
tEntity::tEntity()
{
	fRadius = 0.0f;
	fPrev = 0;
	fField0C = 0;
	fNext = 0;
	fField14 = 0;
	fField68 = 0;
	fClassification = 0;
	fID = -1;
	fProperties = 0;
	fIsAlive = 1;
	fTargetID = -1;
	fField5C = 0;
	fBelongsToHumanFleet = 0;
	fTeam = -1;
	fField64 = -1;
	fCurrPos.mSet(0.0f, 0.0f, 0.0f);
	fLastPos.mSet(0.0f, 0.0f, 0.0f);
	fLastMapPos.mSet(0.0f, 0.0f, 0.0f);
	fSelectStatus = 0;
	fField78 = 0;
	fScriptNotify = 0;
	fField10 = 0;
	fField79 = 0;
	fPoint7A.mSet(0.0f, 0.0f, 0.0f);
	fField86 = 0;
}

// HYPOTHESIS: the return type; the original only clears eax after deleting the object.
// FUNCTION: STARFLEETOP 0x0041015b
int tEntity::vfunction2()
{
	delete this;
	return 0;
}

// FUNCTION: STARFLEETOP 0x00410168
float tEntity::fn_00410168()
{
	return gObject_008e1060.fn_004246e4(this, gObject_008e1060.fn_lookup(fTargetID));
}

// FUNCTION: STARFLEETOP 0x0041018e
void tEntity::vfunction7(tObject_0041018e_arg *param_1, int param_2)
{
	if (param_1->fType == 0x3a) {
		tObject_00795d10 local;
		local.fn_00434d0e(param_2);
		if (local.fField04)
			fScriptNotify = local.fField08;
		else
			fScriptNotify |= local.fField08;
	}
}

// FUNCTION: STARFLEETOP 0x0040ff8c
void tEntity::vfunction5()
{
	fIsAlive = 0;
}

// FUNCTION: STARFLEETOP 0x0040ff91
iTruth tEntity::vfunction9()
{
	return fField78;
}

// FUNCTION: STARFLEETOP 0x0040ff95
void tEntity::vfunction10(iTruth aValue)
{
	fField78 = aValue;
}

// FUNCTION: STARFLEETOP 0x0041004d
void tEntity::vfunction11(tSFCStream &s, int32u mode)
{
	ArchiveSeperatorTM(s, mode);
	ArchiveTitleTM("Entity:", s, mode);
	IncrementStreamTab(s);
	ArchiveInt32TM(fClassification, s, mode, "fClassification");
	ArchiveInt32TM(fID, s, mode, "fID");
	ArchiveInt32uTM(fProperties, s, mode, "fProperties");
	ArchiveITruthTM(fIsAlive, s, mode, "fIsAlive");
	ArchiveInt32TM(fTargetID, s, mode, "fTargetID");
	ArchiveFloatTM(fRadius, s, mode, "fRadius");
	ArchiveInt32TM(fSelectStatus, s, mode, "fSelectStatus");
	ArchiveInt32TM(fTeam, s, mode, "fTeam");
	ArchiveTPoint3TM(fCurrPos, s, mode, "fCurrPos");
	ArchiveTPoint3TM(fLastPos, s, mode, "fLastPos");
	ArchiveTPoint3TM(fLastMapPos, s, mode, "fLastMapPos");
	ArchiveITruthTM(fBelongsToHumanFleet, s, mode, "fBelongsToHumanFleet");
	ArchiveInt32TM(fScriptNotify, s, mode, "fScriptNotify");
	DecrementStreamTab(s);
}

// FUNCTION: STARFLEETOP 0x0040ff9f
void tEntity::vfunction13(tID aID)
{
	fTargetID = aID;
}

// FUNCTION: STARFLEETOP 0x0040ffa9
float tEntity::vfunction14()
{
	return fRadius;
}

// HYPOTHESIS: the original's destructor (0x0040ffc9) is not recovered yet; this one is empty.
tEntity::~tEntity() {}
// FUNCTION: STARFLEETOP 0x0040ffad
// MATCH-ONLY: the scalar deleting destructor, written out as a member that calls the destructor.
tEntity *tEntity::scalar_deleting_destructor(unsigned char flags)
{ tEntity::~tEntity(); if (flags & 1) ::operator delete(this); return this; }
