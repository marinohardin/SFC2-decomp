#ifndef TIGER_TPAGING_H
#define TIGER_TPAGING_H

class tPagingBlock;

// HYPOTHESIS: only the members the units reach; the layout is read from the accesses.
class tPagingMechanism
{
public:
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

#endif
