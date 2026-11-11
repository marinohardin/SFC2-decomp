//	Validate.h
//
//	The validate() check: reports a failed expression without stopping.
//	Declarations follow the SDK's header.
//
#if !defined( Validate_h )
#define Validate_h

#pragma once

#include "Common/Exceptions/BaseException.h"

namespace nException
{

	class tValidationFailed : public tBaseException
	{
	public:
		tValidationFailed( const char* expression, const char* file, int32 lineNumber );

		virtual void ReportException( const char* caughtDescription = NULL ) throw();

	protected:
		const char* fExpression;
	};

};

// The original's macros also open with Assert( false ) from superAssert.h, which is not in game/
// yet; no function matched so far has needed code from it.
#define validate( expr )											\
	if( !(expr) )													\
	{																\
		nException::tValidationFailed failed( #expr, __FILE__, __LINE__ );	\
		failed.ReportException();											\
	}

#define validate_throw( expr, validateThrowObject )					\
	if( !(expr) )													\
	{																\
		throw validateThrowObject;									\
	}

#endif
