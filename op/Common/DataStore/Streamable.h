//	Streamable.h
//
//	The base class of every object that streams in and out of a data store.
//	Declarations follow the SDK's header.
//
#if !defined( Streamable_h )
#define Streamable_h

#pragma once

#include "Common/BaseTypes.h"
#include "Common/DataStore/DataStorePre.h"
#include "Common/Exceptions/BaseException.h"

#include <string>
#include <map>

class type_info;

namespace nDataStore
{

class tStreamable
{
public:
	typedef int32u iObjectID;
	typedef int8u iObjectVersion;

	class tCRCFailed : public except::tBaseException
	{
	public:
		tCRCFailed( const std::string& toObject, const std::string& forObject, const char* file, int32 lineNumber );
		virtual ~tCRCFailed();

		virtual void ReportException( const char* caughtDescription = NULL );

	protected:
		std::string fToObject;
		std::string fForObject;
	};

	tStreamable( iObjectVersion versionNumber = 0 );
	virtual ~tStreamable();

	friend nDataStore::tInput& operator>>( nDataStore::tInput& input, tStreamable& object );
	friend nDataStore::tOutput& operator<<( nDataStore::tOutput& output, const tStreamable& object );

	virtual	void StreamOut( nDataStore::tOutput& output ) const = 0;
	virtual	void StreamIn( nDataStore::tInput& input ) = 0;

	iObjectID		GetObjectID() const;
	iObjectVersion	GetVersion() const;

public:
	class tObjectIDInfo
	{
	public:
		tObjectIDInfo();

		std::string fObjectName;
		int32u		fCRC;
	};

	typedef std::map< const type_info*, tObjectIDInfo > iObjectIDInfoLookup;
	static iObjectIDInfoLookup fObjectIDInfoLookup;

	mutable iObjectID	fObjectID;
	iObjectVersion 		fObjectVersion;
};

// The friend declarations above do not make these visible to ordinary lookup.
tInput& operator>>( tInput& input, tStreamable& object );
tOutput& operator<<( tOutput& output, const tStreamable& object );

};

#endif
