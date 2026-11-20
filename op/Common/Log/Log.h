// Common/Log/Log.h

#if !defined( EventLog_h )
#define EventLog_h

#include "Common/BaseTypes.h"
#include "Common/CleanupGlobalPointer.h"
#include "Common/DataStore/DataStoreBuffer.h"

#include <string>

#include "Common/Thread/Optex.h"

namespace nLog
{

// CLASS PURPOSE: To log import events to a central repository
class tLog
{
public:
//Types
	enum tEventType
	{
		kNoEventType,
		kErrorEvent,
		kWarningEvent,
		kInformationEvent,
		kDebugEvent,
		kAuditSuccess,
		kAuditFailure,
	};

//Setup
	static tLog* Instance();
	void SetupFileOutput( bool turnOn, const std::string& fileName );
	void SetupDebugOutput( bool turnOn );
	void SetupConsoleOutput( bool turnOn );
	void SetupEventLog( bool turnOn, const std::string& serverName, const std::string source );

// Install
	void InstallEventLog( const std::string& sourceName, const std::string& messageDll, bool uninstall );

// Operations
	// HYPOTHESIS: the SDK defaults eventID to a message-DLL constant not yet recovered.
	void LogReport(	tEventType eventType,
					const std::string& description,
					int16u category,
					int32u eventID,
					nDataStore::tBuffer* rawData );

protected:

// Operations
	void LogToFile( tEventType eventType,
					const std::string& description,
					int16u category,
					int32u eventID,
					nDataStore::tBuffer* rawData );

	void LogToEventLog( tEventType eventType,
						const std::string& description,
						int16u category,
						int32u eventID,
						nDataStore::tBuffer* rawData );

	void LogToConsole( tEventType eventType,
						const std::string& description,
						int16u category,
						int32u eventID,
						nDataStore::tBuffer* rawData );

	void LogToDebugOut( tEventType eventType,
						const std::string& description,
						int16u category,
						int32u eventID,
						nDataStore::tBuffer* rawData );

	std::string GetEventTypeString( tEventType eventType );

// Fields
	mutable tOptex fLoggingMutex;

	//Event log info
	bool fOutputToEventLog;
	bool fLogOpen;
	void* fLogHandle;

	//Console info
	bool fOutputToConsole;

	// File log
	std::string fFileName;
	bool fOutputToFile;
	bool fErrorFileOutput;


	//Debug info
	bool fOutputToDebug;

private:
//Hidden
	friend tCleanupGlobalPointer< tLog >;
	tLog();
	~tLog();
	tLog( const tLog& copy );
	tLog& operator=( const tLog& copy );

	static tLog* fLog;
	static tCleanupGlobalPointer< tLog > fCleanupSingletion;

};

};

namespace logmsg = nLog;

#endif
