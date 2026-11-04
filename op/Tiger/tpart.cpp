//	tpart.cpp: tPart, the registry of system parts and their startup and shutdown order.

#include "Tiger/tpart.h"


int32u	tPart::fStaticInitCheck = 777;

iCount	tPart::fStaticPartRegCount;
tPart	*tPart::fStaticPartRegList[ kPartCountMax ];

iCount	tPart::fStaticPartValidCount;
tPart	*tPart::fStaticPartValidList[ kPartCountMax ];

iCount	tPart::fStaticPartActivateCount;
tPart	*tPart::fStaticPartActivateList[ kPartCountMax ];

iCount	tPart::fStaticPartCachedPtrCount;
tPart	**tPart::fStaticPartCachedPtrList[ kPartCachedPtrCountMax ];


// FUNCTION: STARFLEETOP 0x006289b0
tPart::tPart( void )
{
	fPartState = kPartUnregistered;

	assert( fStaticInitCheck == 777 );
	assert( fStaticPartRegCount < kPartCountMax );

	fStaticPartRegList[ fStaticPartRegCount++ ] = this;
	fPartState = kPartRegistered;

	fPartGroupIdent = -1;
}

// FUNCTION: STARFLEETOP 0x00628a50
tPart::~tPart( void )
{
}

// FUNCTION: STARFLEETOP 0x00628a60
void	tPart::mValidateParts( void )
{
	assert( fStaticInitCheck == 777 );
	assert( fStaticPartRegCount <= kPartCountMax );
	assert( fStaticPartValidCount == 0 );
	assert( fStaticPartCachedPtrCount == 0 );

	for( iIndex regIndex = 0; regIndex < fStaticPartRegCount; regIndex++ )
	{
		assert( fStaticPartValidCount < kPartCountMax );

		if (fStaticPartRegList[ regIndex ]->mIsValid())
		{
			fStaticPartValidList[ fStaticPartValidCount++ ] = fStaticPartRegList[ regIndex ];
			fStaticPartRegList[ regIndex ]->fPartState = kPartValidInactive;
		}
	}

	int32s	nextGroupIdent = 100;

	for( iIndex outer = 0; outer < fStaticPartValidCount; outer++ )
	{
		tPart	*outerPart = fStaticPartValidList[ outer ];

		if (outerPart->fPartGroupIdent == -1)
		{
			outerPart->fPartGroupIdent = nextGroupIdent++;
		}

		for( iIndex inner = outer + 1; inner < fStaticPartValidCount; inner++ )
		{
			tPart	*innerPart = fStaticPartValidList[ inner ];

			if (outerPart->mIsCommon( innerPart ))
			{
				assert( innerPart->mIsCommon( outerPart ) );
				innerPart->fPartGroupIdent = outerPart->fPartGroupIdent;
			}
			else
			{
				assert( outerPart->fPartGroupIdent != innerPart->fPartGroupIdent );
			}
		}
	}
}

// FUNCTION: STARFLEETOP 0x00628c20
void	tPart::mActivateParts( void )
{
	assert( fStaticInitCheck == 777 );
	assert( fStaticPartValidCount <= kPartCountMax );
	assert( fStaticPartValidCount > 0 );
	assert( fStaticPartActivateCount == 0 );

	while( fStaticPartActivateCount < fStaticPartValidCount )
	{
		iCount	activatedCount = 0;

		for( iIndex validIndex = 0; validIndex < fStaticPartValidCount; validIndex++ )
		{
			tPart	*part = fStaticPartValidList[ validIndex ];

			assert( part->fPartGroupIdent >= 0 );

			if (part->fPartState == kPartValidInactive)
			{
				if (part->mIsReady())
				{
					iTruth	unique = kTrue;

					for( iIndex prevIndex = 0; prevIndex < validIndex; prevIndex++ )
					{
						tPart	*prevPart = fStaticPartValidList[ prevIndex ];
						if ((prevPart->fPartState == kPartValidActive) && (prevPart->fPartGroupIdent == part->fPartGroupIdent))
						{
							unique = kFalse;
						}
					}

					if (unique)
					{
						part->mActivate();
						part->fPartState = kPartValidActive;
						fStaticPartActivateList[ fStaticPartActivateCount++ ] = part;
						activatedCount++;
					}
				}
			}
		}

		if (activatedCount == 0)
		{
			throw tPartStartupException();
		}
	}
}

// FUNCTION: STARFLEETOP 0x00628da0
void	tPart::mDeactivateParts( void )
{
	assert( fStaticInitCheck == 777 );
	assert( fStaticPartValidCount <= kPartCountMax );
	assert( fStaticPartActivateCount <= kPartCountMax );

	while( fStaticPartActivateCount )
	{
		tPart	*part = fStaticPartActivateList[ --fStaticPartActivateCount ];

		assert( part->fPartState == kPartValidActive );
		part->mDeactivate();
	}
}

// FUNCTION: STARFLEETOP 0x00628e50
void	tPart::mAddToPartCachePtrList( tPart ** addrOfCachePtr )
{
	assert( fStaticPartCachedPtrCount < kPartCachedPtrCountMax );

	fStaticPartCachedPtrList[ fStaticPartCachedPtrCount++ ] = addrOfCachePtr;
}
