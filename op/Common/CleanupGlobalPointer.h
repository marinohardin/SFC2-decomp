//	CleanupGlobalPointer.h
//
//	Deletes a global pointer, if it was allocated.
//
#pragma once

#if !defined( CleanupGlobalPointer_h )
#define CleanupGlobalPointer_h

template< class tClass >
class tCleanupGlobalPointer
{
public:
	tCleanupGlobalPointer( tClass** ppGlobalPointer );
	virtual ~tCleanupGlobalPointer();

protected:
	tClass** fppGlobalPointer;

private:
	tCleanupGlobalPointer( const tCleanupGlobalPointer& copy );
};

template< class tClass >
tCleanupGlobalPointer< tClass >::tCleanupGlobalPointer( tClass** ppGlobalPointer )
	: fppGlobalPointer( ppGlobalPointer )
{
}

// HYPOTHESIS: the destructor deletes the pointed-to object and clears the pointer.
template< class tClass >
tCleanupGlobalPointer< tClass >::~tCleanupGlobalPointer()
{
	if( *fppGlobalPointer )
	{
		delete *fppGlobalPointer;
		*fppGlobalPointer = 0;
	}
}

#endif
