//	tDynamicMemoryFreeListViaHandles.h: tDynamicMemoryFreeListViaHandles, a free list whose members are named by handles.
// Evidence:  class (original file unknown)

#ifndef tDynamicMemoryFreeListViaHandles_h
#define tDynamicMemoryFreeListViaHandles_h 1

class tListMemberWithHandle;

// The slot order is the exe's vtable of the class (0x00798e90), 44 slots. The names of the
// slots the SDK library's symbols give are used as-is; the others are named by their slot.
// HYPOTHESIS: the odd slots before each SDK method that takes a member are its handle forms:
// each looks the member up with mGetListMemberWithHandleFromHandle and calls the member form
// (the exe keeps their out-of-line copies at 0x004353b0 to 0x004355a4).
// HYPOTHESIS: no base class is declared: the exe has a tDynamicMemoryFreeList (vtable
// 0x007958b0, 22 slots) whose slots this vtable does not keep as a prefix, and a base with a
// destructor of its own would add to the inlined destructor below what 0x0066df50 does not
// have.
class tDynamicMemoryFreeListViaHandles
{
public:
	// HYPOTHESIS: an inline constructor (the fields it sets are the ones vfunction2 resets).
	tDynamicMemoryFreeListViaHandles() { field_0xc = 0; field_0x4 = -1; field_0x8 = -1; }
	// The destructor is inline (defined below): the exe keeps 11-byte out-of-line copies of it
	// (0x0043583d, 0x00646b90) that store the vtable and call mClearList, and the sound memory
	// manager's destructor (0x0066df50) inlines it.
	virtual ~tDynamicMemoryFreeListViaHandles();																// 0
	// HYPOTHESIS: maps a handle to its list member; derived lists override it (the name is the
	// SDK library's override in tSoundMemoryBlockList).
	virtual tListMemberWithHandle *mGetListMemberWithHandleFromHandle(long handle);							// 1
	// HYPOTHESIS: resets the list to empty (0x00435391).
	virtual void vfunction2();																					// 2
	// HYPOTHESIS: slots 3 and 4 return the handles of the list's ends (0x0066e0f0 and 0x0066ea20
	// start their walks from them); which is the head is not known.
	virtual long vfunction3();																					// 3
	virtual long vfunction4();																					// 4
	virtual tListMemberWithHandle *vfunction5();																// 5
	virtual tListMemberWithHandle *vfunction6();																// 6
	virtual long vfunction7(long handle);																		// 7
	virtual long mGetPreviousListMemberHandle(tListMemberWithHandle *member);									// 8
	virtual long vfunction9(long handle);																		// 9
	virtual long mGetNextListMemberHandle(tListMemberWithHandle *member);										// 10
	virtual tListMemberWithHandle *vfunction11(long handle);													// 11
	virtual tListMemberWithHandle *vfunction12(tListMemberWithHandle *member);									// 12
	virtual tListMemberWithHandle *vfunction13(long handle);													// 13
	virtual tListMemberWithHandle *vfunction14(tListMemberWithHandle *member);									// 14
	virtual long vfunction15(long handle);																		// 15
	virtual long mGetPreviousCircularListMemberHandle(tListMemberWithHandle *member);							// 16
	virtual tListMemberWithHandle *vfunction17(tListMemberWithHandle *member);									// 17
	virtual tListMemberWithHandle *vfunction18(long handle);													// 18
	virtual long vfunction19(long handle);																		// 19
	virtual long mGetNextCircularListMemberHandle(tListMemberWithHandle *member);								// 20
	virtual tListMemberWithHandle *vfunction21(tListMemberWithHandle *member);									// 21
	virtual tListMemberWithHandle *vfunction22(long handle);													// 22
	virtual tListMemberWithHandle *mGetListMemberAtThisIndex(long index);										// 23
	virtual void vfunction24(long handle);																		// 24
	virtual unsigned char mAddToFrontOfList(tListMemberWithHandle *member);										// 25
	virtual unsigned char vfunction26(long handle);																	// 26
	virtual unsigned char mAddToBackOfList(tListMemberWithHandle *member);										// 27
	virtual void vfunction28(long handle, long otherHandle);													// 28
	virtual unsigned char mInsertAfterListMember(tListMemberWithHandle *member, tListMemberWithHandle *otherMember);	// 29
	virtual void vfunction30(long handle, long otherHandle);													// 30
	virtual unsigned char mInsertBeforeListMember(tListMemberWithHandle *member, tListMemberWithHandle *otherMember);	// 31
	virtual void vfunction32(long handle);																		// 32
	virtual unsigned char mRemoveFromList(tListMemberWithHandle *member);										// 33
	virtual unsigned char vfunction34(long handle);																// 34
	virtual unsigned char mIsListMember(tListMemberWithHandle *member);											// 35
	virtual void vfunction36(long handle);																		// 36
	virtual long mGetZeroIndexedListMemberPosition(tListMemberWithHandle *member);								// 37
	virtual void vfunction38();																					// 38
	// HYPOTHESIS: the number of members (0x0066e0f0 tests it before reading the first one).
	virtual long vfunction39();																					// 39
	virtual void mClearList();																					// 40
	virtual void mDeleteListMembers();																			// 41
	virtual void vfunction42();																					// 42
	virtual void mDumpList();																					// 43

	// HYPOTHESIS: the list is 0x10 bytes (the sound memory manager holds two, at +0x14 and
	// +0x24); field names unknown.
	long field_0x4;
	long field_0x8;
	long field_0xc;
};

// The scalar deleting destructor the compiler generates from this definition is 0x004355a5.
// It is not claimed here: its unit (unsorted/tDynamicMemoryFreeListViaHandles.cpp) holds only
// the forwarders, none of which makes the class's vtable, so the unit emits no deleting
// destructor.
inline tDynamicMemoryFreeListViaHandles::~tDynamicMemoryFreeListViaHandles()
{
	mClearList();
}

#endif
