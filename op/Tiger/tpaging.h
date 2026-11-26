#ifndef TIGER_TPAGING_H
#define TIGER_TPAGING_H

class tPagingBlock;

// HYPOTHESIS: only the members the units reach; the layout is read from the accesses.
class tPagingMechanism
{
public:
	// Slot 0 is the deleting destructor (0x00434fbe): it restores the vtable, calls mDeactivate and
	// frees the object when the flag is set. Declared as a destructor, twMemoryPart's
	// mResizeHandle and mResizeHandleZeroIt (0x006397f0, 0x00639d90) lose 0.05% (their unwind
	// code changes), so it stays a plain slot until the unit that defines it is written.
	virtual	void	vfunction0	( void );
	virtual	void*	mNewBlock	( long a, long b, char* c );
	virtual	void*	mResizeBlock	( void* a, long b );
	virtual	void*	mDuplicateBlock	( void* a );
	virtual	unsigned char	mDelBlock	( void* a );
	virtual	long	mNewHandle	( long a, long b, char* c );
	virtual	int	mNewHandleZeroIt	( long a, long b, char* c );
	virtual	long	mResizeHandle	( long a, long b );
	virtual	long	mResizeHandleZeroIt	( long a, long b );
	virtual	long	mDuplicateHandle	( long a );
	virtual	unsigned char	mDelHandle	( long a );
	virtual	void*	mLockHandle	( long handle );
	virtual	unsigned char	vfunction12	( long a );
	virtual	unsigned char	vfunction13	( long a );
	virtual	void	vfunction14	( void );
	virtual	void	vfunction15	( void );
	virtual	void	vfunction16	( void );
	virtual	void	vfunction17	( void );
	virtual	unsigned char	mCheckForBoundsErrors	( void );
	// HYPOTHESIS: slot 19 is mGetHandleFromPointer (mDelBlock calls it at offset 0x4c).
	virtual	long	mGetHandleFromPointer	( void* a );
	virtual	unsigned char	mGetPtrsFromHandle	( long handle, tPagingBlock*& page, unsigned char*& entry, long& index );
	virtual	unsigned char	mIsThisAPagingPointer	( void* a );
	void	mDeactivate	( void );
	int		fPageCount;
	int		fTableSize;
	char**	fTable;
	char*	fLastPage;
protected:
	unsigned char	mAddPage	( void );
	void	mClosePage	( tPagingBlock* page );
};

// twMemoryPart::fPaging is followed by its member at 0x28, so the part holds 0x14 bytes.
typedef char tPagingMechanism_size_is_0x14[sizeof(tPagingMechanism) == 0x14 ? 1 : -1];

#endif
