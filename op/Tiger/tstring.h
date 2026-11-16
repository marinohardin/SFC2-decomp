//	tstring.h: tString, Tiger's wide character string.
//
//	Declarations follow the SDK's header. Not declared yet: the inline operator!= and the
//	less_ss and less_ps comparators, whose bodies no unit here uses yet, and the members of
//	tString::iterator that no unit here uses yet. The original also
//	includes tlearn.h, tcglue.h, tigerstl.h and tBag.h here; nothing below needs them yet.

#ifndef tstring_h
#define tstring_h

#pragma once

#include "Tiger/tbasic.h"
#include "Tiger/tstringdefs.h"

#include <vector>
#include <iosfwd>	// std::string, without the locale statics <string> brings in

class tItemBag;

typedef iWideChar	iBasicStringElement;

class tString;

typedef struct
{
	mutable char	*fTempExternalCharRepresentation;
	mutable int32	fSizeOfExternalCharAllocation;
	mutable tString	*fTempOperatorSpace;
	int32			fMemLimit;
	int32			fDataCount;
	std::vector< iBasicStringElement >	fMemSpace;	// the original spells it xvector( iBasicStringElement ), from tigerstl.h
} tInstanceStringControl;

iTruth operator== (const tString &s1,const tString &s2);
iTruth operator<  (const tString &s1,const tString &s2);

class tString
{
	protected:
		tInstanceStringControl fInstanceControl;

		static void* mGetMemory(int32 size);
		static void	mFreeMemory(void* ptr, int32 id=0);

		void 	mInitInstanceControl(void);
		int32 	mExpansionRoom(const char *s);
		int32 	mExpansionRoom(const iBasicStringElement *s);
		int32 	mExpansionRoom(int32 size);
		iTruth 	mIncorporateCharList(const iBasicStringElement *s, int32 lengthToCopy);
		iTruth 	mIncorporateCharList(const iBasicStringElement *s);
		iTruth 	mIncorporateCharList(const char *s, int32 lengthToCopy);
		iTruth 	mIncorporateCharList(const char *s);
		iBasicStringElement *mDataStr(void);

	public:
		class 	iterator;
		friend 	iterator;
		// MATCH-ONLY: the original (/Ob2) inlines operator= and the constructors into NumToString; written out there, they need this access.
		friend void NumToString(tString &destStr, int32 data, int32s minDigits);

		tString(const char *s);
		tString(const char *s, int32u length);
		tString(const iBasicStringElement *s);
		tString(const iBasicStringElement c);
		tString(const tString &s);
		tString(const iterator &i);
		tString(const std::string& s);
		tString();
		virtual ~tString();

		int32 mGetMemSize(void);
		int32 mGetDataCount(void);
		void  mReserve(int32 size);

		char* mCharStr( char* myMemory = NULL );
		iBasicStringElement *mWideStr(void);
		unsigned char* mPascStr(void);

		int			mCopyCharStr( char *str, int max_size );
		int			mCopyWideStr( iWideChar *str, int max_size );
		int			mCopyPascStr( unsigned char *str, int max_size );

		tString&	mAppendInt32s(int32s val,int32s minDigits = 1);
		int32s		mGetInt32s(void);
		float		mGetFloat(void);

		void		mEndStrAt(int32 strPosition);
		void		mEndStrAt(iterator strPosition);

		void 		operator=  (const char* s);
		tString& 	operator=  (const tString &s);
		tString& 	operator+  (const tString &s);
		tString& 	operator+= (const tString &s);
		tString&	operator+= (const iBasicStringElement &c);

		friend iTruth operator== (const tString &s1,const tString &s2);
		friend iTruth operator<  (const tString &s1,const tString &s2);
		friend iTruth operator>  (const tString &s1,const tString &s2);
		friend iTruth operator<= (const tString &s1,const tString &s2);
		friend iTruth operator>= (const tString &s1,const tString &s2);

		friend int16 mCStrCompare(const tString &s1, const char* s2);
		friend iTruth operator== (const tString &s1,const char *s2);
		friend iTruth operator>  (const tString &s1,const char *s2);
		friend iTruth operator<  (const tString &s1,const char *s2);
		friend iTruth operator<= (const tString &s1,const char *s2);
		friend iTruth operator>= (const tString &s1,const char *s2);
		friend iTruth operator!= (const tString &s1,const char *s2);

		iterator				mStrChr(iBasicStringElement c);
		iterator				mStrRChr(iBasicStringElement c);
		int						mStrLen(void) const;
		// HYPOTHESIS: returns the string's characters; tListBox_Base::vfunction44 (0x004c679a) draws them.
		unsigned short			*fn_00618920();
		void					mToUpper(void);
		void					mToLower(void);
		int32s					mStrICmp(const tString &s) const;
		int32s					mStrCmp(const tString &s) const;
		int32s					mStringCompare(const tString &s) const;

		tString& operator+ (const char *s);
		tString& operator+= (const char *s);
		void operator= (const iBasicStringElement* s);
		void operator= (const unsigned char* s);

		iterator				begin();
		iterator				end();
		tString 				substr(int32 start, int32 length);
		tString 				substr(int32 start);
		iBasicStringElement&	operator[](int32 n);
		int32 					size() const;
		int32					find(char);
		int32					find(tString &, int32 startPos = 0);
		int32					rfind(tString &, int32 startPos);
		int32					find(const char *, int32 startPos = 0);
		int32					rfind(const char *, int32 startPos);
		int32					rfind(const char *);

		char*				mOSString();
		tString*			mAppendOSStr(char*	osStr);

		// The original defines the iterator after the class, as tString::iterator; it is
		// defined here, in the body, because tools/manifest.py cannot read that form yet.
		// Only the members a unit here uses are declared.
		class iterator
		{
			friend tString;

			public:
				iterator();
				iterator(int32 index, tString *s);
				iterator(const iterator &i);

				iBasicStringElement&	operator*();
				iterator				operator+(int32 n);
				iterator				operator-(int32 n);
				iterator				operator++(int);
				iterator				operator++();
				iterator				operator--(int);
				iTruth					operator==(const iterator i);
				iTruth					operator!=(const iterator i);
				iTruth					operator<(const iterator i);

			protected:
				int32		fIndex;
				tString		*fString;
		};
};

int32 StringToNum(char *str);
void NumToString(tString &destStr, int32 data, int32s minDigits = 1);

#endif
