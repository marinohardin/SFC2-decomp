//	SmartPtr.h
//
//	A reference count and the smart pointers that hold it.
//	Declarations follow the SDK's header; only what the rebuilt functions need is defined yet.
//
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
	// The SDK's conversion to const tnPtr* is left out: tools/manifest.py cannot read it.

	bool operator==( const tnPtr* pPtr) const;
	bool operator!=( const tnPtr* pPtr) const;
	bool operator<( const tnPtr* pPtr) const;

	const tnPtr* Get() const;

protected:
	tnPtr* fObject;

};

////////////////////////////////////////////////////////////////////////////////
// tSmartPtr
//
////////////////////////////////////////////////////////////////////////////////


template< typename tnPtr > class tSmartPtr : public tConstSmartPtr< tnPtr >
{
public:
// Setup
	tSmartPtr();
	tSmartPtr( tnPtr* pObject );
	tSmartPtr( const tSmartPtr& copy );

// Operations
	tSmartPtr& operator=( const tSmartPtr& copy );
	tnPtr* operator=( tnPtr* pPtr );

// Accesors
	// Suply the same accessors that a pointer would
	tnPtr* operator->() const;
	operator tnPtr*() const;

	tnPtr* Get() const;
};


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

template <class tnPtr >
inline tSmartPtr<tnPtr>::operator tnPtr*() const
{
	return fObject;
}

template <class tnPtr >
tnPtr* tSmartPtr<tnPtr>::Get() const
{
	return fObject;
}


#endif // !defined( SmartPtr_h )
