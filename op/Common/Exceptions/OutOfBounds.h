//	OutOfBounds.h
//
//	Thrown when an index or size falls outside the range an object holds.
//	Declarations follow the SDK's header.
//
#if !defined( OutOfBounds_h )
#define OutOfBounds_h

#pragma once

#include "Common/Exceptions/BaseException.h"

namespace nException
{

	class tOutOfBounds : public tBaseException
	{
	public:
		tOutOfBounds( const char* object, int32 min, int32 max, int32 value, const char* file, int32 lineNumber );
		tOutOfBounds( const tOutOfBounds& other );

		const char* fObject;
		int32 fMin;
		int32 fMax;
		int32 fValue;
	};

};

#endif
