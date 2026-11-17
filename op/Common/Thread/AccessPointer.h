// Common/Thread/AccessPointer.h

#if !defined( AccessPointer_h )
#define AccessPointer_h

#include "Common/BaseTypes.h"
#include "Common/Thread/Optex.h"

// CLASS PURPOSE: Used to access a guarded pointer
// The members are declared without their bodies, except the destructor the game's units expand
// inline; the game holds out-of-line copies of the rest per instantiation.
template< typename tnPtrType, typename tnPtrGuard >
class tAccessPointer
{
public:
// Setup
	tAccessPointer();
	tAccessPointer( tnPtrType* pointer, const tnPtrGuard* pointerGuard );
	tAccessPointer( tAccessPointer<tnPtrType,tnPtrGuard>& copy );
	tAccessPointer& operator=( tAccessPointer<tnPtrType,tnPtrGuard>& copy );
	~tAccessPointer();

// Operations
	bool IsValid() const;
	void Done();

// Accessors
	tnPtrType& operator*();
	tnPtrType* operator->();

protected:
	mutable tOptex fAccess;
	tnPtrType* fPointer;
	const tnPtrGuard* fPointerGuard;
};

template< typename tnPtrType, typename tnPtrGuard >
inline tAccessPointer<tnPtrType,tnPtrGuard>::~tAccessPointer()
{
	Done();
}

#endif
