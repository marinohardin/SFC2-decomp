// Unit:      unsorted/fn_00571000.cpp
// Functions: 0x00571018 to 0x00571e33, 37 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   game-release (harness/units.json)

#include <typeinfo>
#include "Common/Thread/Optex.h"
#include "Common/DynamicAllocator.h"
#include "Common/Exceptions/Validate.h"
#include "Common/Database/DatabaseID.h"

// Inline in the SDK; Optex.h declares it only.
inline tOptex::tGuard::~tGuard()
{
	if (fNeedToLeave) {
		fOptex->Leave();
	}
}

// HYPOTHESIS: the database object type this instantiation of the SDK's member template
// tDatabase::AssignObjectToDB handles (virtual slots 0x2c SetLockID, 0x30 GetID, 0x3c
// AttachToDB); its name and members are not recovered.
class fn_00571018_object : public tAllocatedClass {
public:
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void SetLockID(long lockID);
	virtual const nDatabase::tID& GetID();
	virtual void vfunction13();
	virtual void vfunction14();
	virtual void AttachToDB(const nDatabase::tID& id);
	fn_00571018_object& operator=(const fn_00571018_object& other);
};

// HYPOTHESIS: the database's storage; slot 4 is AddToStorage.
class fn_00571018_storage {
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual nDatabase::tID AddToStorage(fn_00571018_object* pObject);
};

// HYPOTHESIS: the callee 0x00705900 is nDatabase::tDatabase::AddObjectToDBIndex(tDatabaseObject*,
// iAllocatorID); tDatabase is declared only inside Common/Database/FileDatabaseMgr.cpp, so no
// second copy is made here and the symbol differs until it moves to a header.
// HYPOTHESIS: stands in for nDatabase::tDatabase (fAccess at +0x4, fDatabaseStorage at +0x40)
// until its header is rebuilt; VC6 cannot instantiate the SDK's member template here without
// its caller.
class fn_00571018_owner {
public:
	virtual void vfunction0();
	void AddObjectToDBIndex(fn_00571018_object* pActualObject, iAllocatorID allocID);
	void fn_00571018(fn_00571018_object* pObject);
	void fn_0057118e(fn_00571018_object* pObject);
	void fn_00571304(fn_00571018_object* pObject);
	void fn_0057147a(fn_00571018_object* pObject);
	tOptex fAccess;
	char fn_00571018_pad[0x40 - 4 - sizeof(tOptex)];
	fn_00571018_storage* fDatabaseStorage;
};

// FUNCTION: STARFLEETOP 0x00571018
void fn_00571018_owner::fn_00571018(fn_00571018_object* pObject)
{
	tOptex::tGuard guard(&fAccess);
	// MATCH-ONLY: the original releases the guard explicitly after the tID temporary is gone
	// (no state reset before Leave); a block round the body reproduces that.
	{
#line 152
	validate(pObject);
	iAllocatorID allocID = tDynamicAllocator::Instance()->mGetAllocatorID(typeid(*pObject));
#line 154
	validate(allocID != kInvalidAllocatorID);

	fn_00571018_object* pActualObject = static_cast<fn_00571018_object*>(tDynamicAllocator::Instance()->mAllocateClass(allocID));
#line 157
	validate(pActualObject);
	*pActualObject = *pObject;

#line 160
	validate(pActualObject->GetID().IsValid() == false);

	const nDatabase::tID& newID = fDatabaseStorage->AddToStorage(pActualObject);
	pActualObject->AttachToDB(newID);

	pActualObject->SetLockID(0);

	AddObjectToDBIndex(pActualObject, allocID);

	*pObject = *pActualObject;
	}
	guard.Leave();
}

// HYPOTHESIS: a third instantiation of the same member template, for another database object type.
// FUNCTION: STARFLEETOP 0x00571304
void fn_00571018_owner::fn_00571304(fn_00571018_object* pObject)
{
	tOptex::tGuard guard(&fAccess);
	{
#line 152
	validate(pObject);
	iAllocatorID allocID = tDynamicAllocator::Instance()->mGetAllocatorID(typeid(*pObject));
#line 154
	validate(allocID != kInvalidAllocatorID);

	fn_00571018_object* pActualObject = static_cast<fn_00571018_object*>(tDynamicAllocator::Instance()->mAllocateClass(allocID));
#line 157
	validate(pActualObject);
	*pActualObject = *pObject;

#line 160
	validate(pActualObject->GetID().IsValid() == false);

	const nDatabase::tID& newID = fDatabaseStorage->AddToStorage(pActualObject);
	pActualObject->AttachToDB(newID);

	pActualObject->SetLockID(0);

	AddObjectToDBIndex(pActualObject, allocID);

	*pObject = *pActualObject;
	}
	guard.Leave();
}

#line 101
// HYPOTHESIS: a second instantiation of the same member template as 0x00571018, for another
// database object type; names are not recovered.
// FUNCTION: STARFLEETOP 0x0057118e
void fn_00571018_owner::fn_0057118e(fn_00571018_object* pObject)
{
	tOptex::tGuard guard(&fAccess);
	{
#line 152
	validate(pObject);
	iAllocatorID allocID = tDynamicAllocator::Instance()->mGetAllocatorID(typeid(*pObject));
#line 154
	validate(allocID != kInvalidAllocatorID);

	fn_00571018_object* pActualObject = static_cast<fn_00571018_object*>(tDynamicAllocator::Instance()->mAllocateClass(allocID));
#line 157
	validate(pActualObject);
	*pActualObject = *pObject;

#line 160
	validate(pActualObject->GetID().IsValid() == false);

	const nDatabase::tID& newID = fDatabaseStorage->AddToStorage(pActualObject);
	pActualObject->AttachToDB(newID);

	pActualObject->SetLockID(0);

	AddObjectToDBIndex(pActualObject, allocID);

	*pObject = *pActualObject;
	}
	guard.Leave();
}

#line 300
// HYPOTHESIS: a fourth instantiation of the same member template, for another database object type.
// FUNCTION: STARFLEETOP 0x0057147a
void fn_00571018_owner::fn_0057147a(fn_00571018_object* pObject)
{
	tOptex::tGuard guard(&fAccess);
	{
#line 152
	validate(pObject);
	iAllocatorID allocID = tDynamicAllocator::Instance()->mGetAllocatorID(typeid(*pObject));
#line 154
	validate(allocID != kInvalidAllocatorID);

	fn_00571018_object* pActualObject = static_cast<fn_00571018_object*>(tDynamicAllocator::Instance()->mAllocateClass(allocID));
#line 157
	validate(pActualObject);
	*pActualObject = *pObject;

#line 160
	validate(pActualObject->GetID().IsValid() == false);

	const nDatabase::tID& newID = fDatabaseStorage->AddToStorage(pActualObject);
	pActualObject->AttachToDB(newID);

	pActualObject->SetLockID(0);

	AddObjectToDBIndex(pActualObject, allocID);

	*pObject = *pActualObject;
	}
	guard.Leave();
}
