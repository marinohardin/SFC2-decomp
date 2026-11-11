//	BaseException.h
//
//	The base of every exception the Common library throws.
//	Declarations follow the SDK's header.
//
#if !defined( BaseException_h )
#define BaseException_h

#pragma once

#include "Common/BaseTypes.h"

#include <exception>

namespace nException
{

class tBaseException : public std::exception
{
public:
	tBaseException() throw();
	tBaseException( const char* pDescription, const char* pFile = NULL, int32 lineNumber = -1 ) throw();
	virtual ~tBaseException() throw();

	virtual void ReportHeader( const char* caughtDescription = NULL ) throw();
	virtual void ReportFooter() throw();
	virtual void ReportException( const char* caughtDescription = NULL ) throw();

	virtual const char* GetDescription() throw();
	virtual const char* GetFile() throw();
	virtual int32 GetLineNumber() throw();

protected:
	const char* fpFileName;
	int32		fLineNumber;
};

};

namespace except = nException;

#endif
