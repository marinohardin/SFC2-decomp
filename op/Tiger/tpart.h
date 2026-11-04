//	tpart.h: classes for managing individual system "parts" or components.

#pragma once

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

#ifndef	tpart_h
#define	tpart_h	1

// The original includes tpartdefs.h here, which has no unit either; its defaults:
#define	kPartCountMax			100
#define	kPartCachedPtrCountMax	(kPartCountMax*10)

typedef enum
{
	kPartUnregistered = 0,

	kPartRegistered,

	kPartValidInactive,
	kPartValidActive,

	kPartStateLim
}	ePartState;

class	tPart
{
	public:
									tPart			( void );
							virtual	~tPart			( void );

					void	mRegisterSelf	( void );
		static		void	mValidateParts	( void );
		static		void	mActivateParts	( void );
		static		void	mDeactivateParts( void );

		static		void	mResetPartCachePtrList		( void );
		static		void	mAddToPartCachePtrList		( tPart ** addrOfCachePtr );
		static		void	mInvalidatePartCachePtrs	( void );

		static		iCount	mCountCommonParts			( tPart *partPtr );
		static		tPart*	mGetIndexedCommonPart		( tPart *partPtr, iIndex commonPartIndex );
		static		void	mSubstituteCommonPart		( tPart* oldPart, tPart* newPart );

		virtual		iTruth	mIsValid	( void ) = 0;
		virtual		iTruth	mIsReady	( void ) = 0;

		virtual		iTruth	mIsCommon	( tPart* otherPart )
			{ return (otherPart==this); };

		virtual		void	mActivate	( void ) = 0;
		virtual		void	mDeactivate	( void ) = 0;

		ePartState		fPartState;

		int32s			fPartGroupIdent;

		class	tPartStartupException	{};
		class	tPartBindingException	{};

	protected:
		static		int32u	fStaticInitCheck;

		static		iCount	fStaticPartRegCount;
		static		tPart	*fStaticPartRegList[ kPartCountMax ];

		static		iCount	fStaticPartValidCount;
		static		tPart	*fStaticPartValidList[ kPartCountMax ];

		static		iCount	fStaticPartActivateCount;
		static		tPart	*fStaticPartActivateList[ kPartCountMax ];

		static		iCount	fStaticPartCachedPtrCount;
		static		tPart	**fStaticPartCachedPtrList[ kPartCachedPtrCountMax ];
};

#endif	// end tpart_h block
