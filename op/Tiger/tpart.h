//	tpart.h: classes for managing individual system "parts" or components.

#pragma once

#include "Tiger/tbasic.h"

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

#define	GenAbstractBinding(classx)								\
		static classx *Search_##classx ( void )					\
		{														\
			static	classx	*partCachePtr = (classx*) NULL;		\
			if (partCachePtr)	return	partCachePtr;			\
			else												\
			{													\
				for( int idx = 0; idx<tPart::fStaticPartActivateCount; idx++)			\
				{																		\
					classx	*bindResult = dynamic_cast< classx *>(tPart::fStaticPartActivateList[idx] );	\
					if (bindResult)														\
					{																	\
						tPart::mAddToPartCachePtrList( (tPart**)&partCachePtr );		\
						partCachePtr = bindResult;										\
						return bindResult;												\
					}												\
				}													\
				throw	tPartBindingException();					\
				return (kFalse);									\
			}														\
		};															\
		static iTruth Test_##classx( void )							\
		{															\
			try	{ classx *test = classx::Search_##classx();	}		\
			catch (tPartBindingException)	{ return	kFalse;	}	\
			return kTrue;											\
		}

#define	BindPart(classx)	( classx::Search_##classx() )
#define	TestPart(classx)	( classx::Test_##classx() )
#define	IsCastableAs(classptr,classx)	(dynamic_cast< classx* >(classptr) != NULL)

#endif	// end tpart_h block
