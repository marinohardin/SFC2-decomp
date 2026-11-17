// Common/Thread/SingleAccessWrapper.h

#if !defined( SingleAccessWrapper_h )
#define SingleAccessWrapper_h

#include "Common/BaseTypes.h"
#include "Common/Thread/Optex.h"
#include "Common/Thread/AccessPointer.h"

// CLASS PURPOSE: This class gives you exclusive access to a pointer
// The members are declared without their bodies; the game holds out-of-line copies of them per
// instantiation.
template< typename tPtrType >
class tSingleAccessWrapper
{
public:
//Types
	typedef tAccessPointer< tPtrType, tSingleAccessWrapper > iPtr;
	typedef tAccessPointer< const tPtrType, tSingleAccessWrapper > iConstPtr;

// Setup
	tSingleAccessWrapper();
	tSingleAccessWrapper( tPtrType* pointer );
	tSingleAccessWrapper( const tSingleAccessWrapper& copy );
	~tSingleAccessWrapper();
	tSingleAccessWrapper& operator=( const tSingleAccessWrapper& copy );
	tSingleAccessWrapper& operator=( tPtrType* copy );

// Operations
	void SetPointer( tPtrType* copy );
	void Delete();
	iPtr Lock();
	iConstPtr Lock() const;

// Accessor
	bool IsValid() const;

protected:
	friend iPtr;
	friend iConstPtr;

// Hidden

// Operations
	void Done() const;

// Fields
	tPtrType* fpPointer;
	mutable tOptex fPointerOptex;
};

template< typename tPtrType >
inline tSingleAccessWrapper<tPtrType>::~tSingleAccessWrapper()
{
	Delete();
}

template< typename tPtrType >
inline void tSingleAccessWrapper<tPtrType>::SetPointer( tPtrType* copy )
{
	Delete();

	fpPointer = copy;
}

#endif
