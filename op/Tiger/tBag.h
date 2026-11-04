//	tBag.h: tItemBag, items stored under C string labels, with the memory they use tracked.

#ifndef TBAGH
#define TBAGH 1

// The original includes tbasic.h here. The unit map has no place for a header without a unit of
// its own yet, so the declarations this unit needs from it are kept here under its guard.
#ifndef	tbasic_h
#define	tbasic_h	1

typedef	signed long		int32;
typedef	signed long		int32s;
typedef	unsigned long	int32u;

typedef	signed short	int16;
typedef	signed short	int16s;
typedef	unsigned short	int16u;

typedef	signed char		int8;
typedef	signed char		int8s;
typedef unsigned char	int8u;

typedef	int32s			iCount;
typedef	int32s			iIndex;

typedef	int32u			iTag;
typedef	int8u			iTruth;

#define	kFalse	0
#define	kTrue	1

#ifndef NULL
#define NULL    0
#endif

class	GenericException {};
#ifdef assert
#undef assert
#endif
#define	assert(x)	{ if (!(x)) {throw GenericException();} }

#endif	// end tbasic_h block

#include <string.h>

// The original includes tString.h here; tItemBag only names the class.
class tString;

#ifndef byte
	typedef unsigned char byte;
#endif


typedef struct
{
	short fTotalConstructorsCalled;
	short fTotalDestructorsCalled;

	long fTotalBagSize;
	long fLargestTotalBagSize;

} sMasterBagControlType;


typedef enum
{
	kNoBaseBagType = 0,
	kByteBag,
	kLongBag,
	kCharArrayBag,
	kTStringBag,
	kNumBaseBagTypes
} eBaseBagTypes;


class tBaseItemBag;
class tBaseItemBag
{
	public:
	int8			fBagType;
	char 			*fLabel;
	tBaseItemBag	*fNext;

	static sMasterBagControlType 	smMasterControl;

	static void mInitMasterControl(void);

	static void mUpdateMasterMemInfo(long delta);
	static void mUpdateMasterConstructorInfo(long delta);
	static void mUpdateMasterDestructorInfo(long delta);

	static long mTotalMemSize(void);
	static long mLargestMemSize(void);
	static long mTotalConstructors(void);
	static long mTotalDestructors(void);

	static byte* 	mGetMemory(long size);
	static void 	mFreeMemory(void *ptr,long id = 0);

	void *operator new(size_t size);
	void operator delete(void *ptr);

	tBaseItemBag();
	virtual ~tBaseItemBag();
};


class tByteItemBag:public tBaseItemBag
{
	public:
		byte *fData;

	tByteItemBag(byte initalizer);
	virtual ~tByteItemBag();
};


class tLongItemBag:public tBaseItemBag
{
	public:
		long *fData;

	tLongItemBag(long initalizer);
	virtual ~tLongItemBag();
};


class tCharArrayItemBag:public tBaseItemBag
{
	public:
		char *fData;

	tCharArrayItemBag(char *initalizer);
	virtual ~tCharArrayItemBag();
};


class tTStringItemBag:public tBaseItemBag
{
	public:
		tString *fData;

	tTStringItemBag(tString &initalizer);
	virtual ~tTStringItemBag();
};


class tItemBag
{
	protected:
		long			fNumItems;
		tBaseItemBag 	*fBagItemList;

		long 			mAddToList(tBaseItemBag *Item, char *label);
		tBaseItemBag 	*mFindItemWithLabel(char *label);
		long 			mRemoveItemWithLabel(char *label);

	public:

		static long mTotalMemSize(void);
		static long mLargestMemSize(void);
		static long mTotalConstructors(void);
		static long mTotalDestructors(void);

		long mAddItem(byte data, char *label);
		long mGetItem(byte *dest,char *label);

		long mAddItem(long data, char *label);
		long mGetItem(long *dest,char *label);

		long mAddItem(char* data, char *label);
		long mGetItem(char* dest, char *label);

		long mAddItem(tString& data, char *label);
		long mGetItem(tString& dest, char *label);

		long mDelItem(char *label);

		void operator=(tItemBag &b);

		void *operator new(size_t size);
		void operator delete(void *ptr);

		tItemBag(long code);

		tItemBag();
		virtual ~tItemBag();
};

#endif
