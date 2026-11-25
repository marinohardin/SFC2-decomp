// Common/Thread/ResourceGuard.h

#if !defined( ResourceGuard_h )
#define ResourceGuard_h

#include "Common/BaseTypes.h"
#include "Common/Thread/Optex.h"

// CLASS PURPOSE: Instances of this class will be accessed by multiple
// threads. So, all members of this class (except the constructor and
// destructor) must be thread-safe.
class tResourceGuard
{
public:
//Types
	class tGuard
	{
	public:
	//Setup
		tGuard( tResourceGuard& resGuard );
		~tGuard();

	private:
	//Fields
		tResourceGuard& fResGuard;
	};

//Setup
	tResourceGuard();
	~tResourceGuard();

//Operations
	// IsGuarded is used for debugging
	bool IsGuarded() const;

private:
//Types
	// Guard/Unguard can only be accessed by the nested tGuard class.
	friend class tResourceGuard::tGuard;

//Operations
	void Guard();
	void Unguard();

private:
//Hidden
	tResourceGuard( const tResourceGuard& copy );
	tResourceGuard& operator=( const tResourceGuard& copy );

//Fields
	tOptex fCriticalSection;
	long fGuardCount;   // # of EnterCriticalSection calls
};

#endif
