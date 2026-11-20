// Common/Log/LogStream.h

#if !defined( EventLogStream_h )
#define EventLogStream_h

#include "Common/BaseTypes.h"

#include <ostream>
#include <string>
#include <streambuf>

#include "Common/Log/Log.h"
#include "Common/Thread/Optex.h"

namespace nLog
{

// CLASS PURPOSE: Used to log events using a stream
class tLogStream : public std::ostream
{
public:
//Setup
	tLogStream( tLog::tEventType eventType = tLog::kNoEventType );
	// MATCH-ONLY: the SDK declares no destructor; this declared one stands in for the
	// implicit one so its deleting destructor (0x006feed0) has a definition to sit on.
	~tLogStream();

	tLogStream& operator()( const char* category, int8u level = 10 );

protected:
//Types
	class tStreamBuf : public std::streambuf
	{
	public:
		tStreamBuf( tLog* eventLog );
		~tStreamBuf();

		virtual std::streamsize xsputn(const char* text, std::streamsize n);
		virtual int overflow(int ch);
		virtual std::streamsize xsgetn(char* text, std::streamsize n);
		virtual int underflow();
		virtual int uflow();
		virtual int sync();


		virtual void Report();
		virtual void LockAccess( bool lock = true );
		virtual void RunCommand();
		virtual void SetEventType( tLog::tEventType eventType );

	protected:
		tOptex fAccess;
		std::string fEventBuffer;
		std::string fCommand;
		tLog::tEventType fEventType;
		tLog* fLog;
		bool fLockType;
	};

// Fields
	tStreamBuf fStreamBuf;
};


// Event log stream manipulators
// WARNING!! You must stream in report when you are done with your msg!!!!!!!
// If you don't the message stream will be locked forever!!!!!
std::ostream& __cdecl report( std::ostream& stream );
std::ostream& __cdecl set_warning( std::ostream& stream );
std::ostream& __cdecl set_error( std::ostream& stream );
std::ostream& __cdecl set_info( std::ostream& stream );
std::ostream& __cdecl set_audit_success( std::ostream& stream );
std::ostream& __cdecl set_audit_fail( std::ostream& stream );

extern tLogStream out;
extern tLogStream warning;
extern tLogStream error;
extern tLogStream info;
extern tLogStream debug;
};

namespace logmsg = nLog;
#endif
