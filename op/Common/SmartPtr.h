#if !defined( SmartPtr_h )
#define SmartPtr_h


#include "Common/BaseTypes.h"


////////////////////////////////////////////////////////////////////////////////
// tRefCount
//
////////////////////////////////////////////////////////////////////////////////


class tRefCount
{
public:
// Setup
	tRefCount();

// Operations
	void AddRef() const;
	void Release();

// Accessors
	int32u GetRefCount();

protected:
	virtual ~tRefCount();

private:
	mutable int32 fRefCount;
};



////////////////////////////////////////////////////////////////////////////////
// tConstSmartPtr
//
////////////////////////////////////////////////////////////////////////////////


template< typename tnPtr >
class tConstSmartPtr
{
public:
// Setup
	tConstSmartPtr();
	tConstSmartPtr( const tnPtr* pObject );
	tConstSmartPtr( const tConstSmartPtr& copy );
	tConstSmartPtr& operator=( const tConstSmartPtr& theCopy );

	~tConstSmartPtr();

// Operations
	tnPtr* operator=( const tnPtr* pPtr );

// Accesors
	bool IsValid() const;

	// Suply the same accessors that a pointer would
	const tnPtr* operator->() const;

	bool operator==( const tnPtr* pPtr) const;
	bool operator!=( const tnPtr* pPtr) const;
	bool operator<( const tnPtr* pPtr) const;

	const tnPtr* Get() const;

protected:
	tnPtr* fObject;

};

// tSmartPtr, derived from tConstSmartPtr in the SDK, is declared once code needs it.

////////////////////////////////////////////////////////////////////////////////
// Template functions for SmartPtr
//
////////////////////////////////////////////////////////////////////////////////

template <class tnPtr >
inline tConstSmartPtr<tnPtr>::tConstSmartPtr<tnPtr>( const tnPtr* pObject ) :
	fObject( const_cast< tnPtr* >( pObject ) )
{
	if( fObject )
	{
		fObject->AddRef();
	}
}

template <class tnPtr >
inline tConstSmartPtr<tnPtr>::tConstSmartPtr<tnPtr>( const tConstSmartPtr<tnPtr>& copy ) :
	fObject( const_cast< tnPtr* >( copy.Get() ) )
{
	if( fObject )
	{
		fObject->AddRef();
	}
}

template <class tnPtr >
inline tConstSmartPtr<tnPtr>::~tConstSmartPtr<tnPtr>()
{
	// Release the reference
	if( fObject != NULL )
	{
		fObject->Release();
	}
}

template <class tnPtr >
inline const tnPtr* tConstSmartPtr<tnPtr>::Get() const
{
	return fObject;
}


#endif // !defined( SmartPtr_h )
