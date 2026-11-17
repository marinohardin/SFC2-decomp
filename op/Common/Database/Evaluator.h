//	Evaluator.h
//
//	The unary predicates a database query is built from.
//	Declarations follow the SDK's header.
//
#pragma once

#if !defined( Evaluator_h )
#define Evaluator_h

#include "Common/BaseTypes.h"
#include <functional>

namespace nDatabase
{

// CLASS PURPOSE: A interface to unary predicates that evalute the pointer to tClass
template< class tClass >
class tEval : public std::unary_function< tClass*, bool >
{
public:
	virtual bool operator()( const tClass* value ) const = 0;
};

// HYPOTHESIS: the name and the three template arguments are the exe's RTTI
// (tMemberEval<tServCharacter,nDatabase::tID const &,std::equal_to<nDatabase::tID> >, vtable
// 0x007b2a70); the SDK does not have the class. 0x0056c7c0 builds one in 0x18 bytes: the
// accessor (a pointer to a member function) at +8 and the value at +0x10. The members at +4
// and +0x14 are not written there; that the comparison object is one of them is a guess.
template< class tClass, class tValue, class tCompare >
class tMemberEval : public tEval< tClass >
{
public:
	typedef tValue ( tClass::*iAccessor )() const;

	tMemberEval( iAccessor accessor, tValue value );

	virtual bool operator()( const tClass* value ) const;

protected:
	tCompare fCompare;
	iAccessor fAccessor;
	tValue fValue;
	int32u fUnknown14;
};

template< class tClass, class tValue, class tCompare >
inline tMemberEval< tClass, tValue, tCompare >::tMemberEval( iAccessor accessor, tValue value ) :
	fAccessor( accessor ),
	fValue( value )
{
}

};
namespace db = nDatabase;

#endif
