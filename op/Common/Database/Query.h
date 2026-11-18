//	Query.h
//
//	A database query: a collection of evaluators joined by and or or.
//	Declarations follow the SDK's header.
//
#pragma once

#if !defined( Query_h )
#define Query_h

#include "Common/BaseTypes.h"
#include "Common/Database/Evaluator.h"

#include <vector>

namespace nDatabase
{

// CLASS PURPOSE: Query is a unary predicate class then evaluates collection
// of unary evalulators to find out if the object should be in the list
template< class tClass >
class tQuery : public std::unary_function< tClass*, bool >
{
public:
//Types
	typedef tEval<tClass> iEvaluator;
	typedef std::vector<iEvaluator*> iEvaluators;

	enum eOpperation
	{
		kAnd,
		kOr
	};

//Setup
	tQuery( eOpperation opperation = kAnd );
	virtual ~tQuery();

//Operations
	virtual void AddEval( iEvaluator* eval );
	virtual bool operator()( const tClass* value ) const;

protected:
//Fields
	iEvaluators fEvaluators;
	eOpperation fOpperation;
};



// TEMPLATE FUNCTIONS
// HYPOTHESIS: the game expands the constructor, the destructor and AddEval where a query is
// used (0x0056c414, 0x0056c7c0), so its copy of these definitions is inline.
template< class tClass >
inline tQuery<tClass>::tQuery( eOpperation opperation ) :
	fOpperation( opperation )
{}


template< class tClass >
inline tQuery<tClass>::~tQuery()
{}

template< class tClass >
inline void tQuery<tClass>::AddEval( iEvaluator* eval )
{
	fEvaluators.push_back( eval );
}

};
namespace db = nDatabase;

#endif
