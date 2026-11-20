//	InvalidAccess.h
//
//	Thrown when code reaches a function that must never be called.
//
#if !defined( InvalidAccess_h )
#define InvalidAccess_h

#pragma once

#include "Common/Exceptions/BaseException.h"

namespace nException
{

	class tInvalidAccess : public tBaseException
	{
	public:
		tInvalidAccess( const char* pDescription, const char* pFile, int32 lineNumber );

		// HYPOTHESIS: one extra member; the object is 4 bytes larger than tBaseException
		const char* fObject;
	};

};

#endif
