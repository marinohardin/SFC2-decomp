// tBuffer::Write is called out of line (0x0042a0fc).
#define DATASTORE_OUT_OF_LINE_WRITE
#include "unsorted/tMetaMapHex.h"
#include "Common/Settings.h"
#include "Common/DataStore/DataStoreOutput.h"

// Inline in the SDK; the unwind code of StreamOut below calls a copy.
inline tOptex::tGuard::~tGuard()
{
	if( fNeedToLeave )
	{
		fOptex->Leave();
	}
}

// HYPOTHESIS: returns the thread state whose default use the optex takes (it is passed to
// tThreadState::SetCurThreadState elsewhere); the name is not known.
tThreadState *fn_005d4c97();
// HYPOTHESIS: returns the settings the HexValues lists are read from; the name is not known.
tSettings *fn_005d481f();

// HYPOTHESIS: the values read once from the HexValues settings and a flag that they are read;
// their names and where they are defined are not known.
extern bool g_00c3849c;
extern std::vector< long > g_00c38488;
extern std::vector< long > g_00c38470;
extern std::vector< float > g_00c38460;

// FUNCTION: STARFLEETOP 0x005391b0
tMetaMapHex::tMetaMapHex( bool param_1 )
	: tDatabaseObject( 1 ), m_5c( 0 ), m_60( 0 ), m_64( 0 ), m_68( 0 ), m_6c( 0 ),
	  m_98( 0.0 ), m_a0( 0.0 ), m_a8( 0, 0 ), m_bc( fn_005d4c97()->DefaultThreadObjectUse() )
{
	for( fn_0053a6cc_map::iterator it = m_48.begin(); it != m_48.end(); ++it )
		(*it).second = 23;

	if( param_1 )
	{
		if( !g_00c3849c )
		{
			fn_005d481f()->GetList( "HexValues/Economy", &g_00c38488, NULL );
			fn_005d481f()->GetList( "HexValues/Victory", &g_00c38470, NULL );
			fn_005d481f()->GetList( "HexValues/Speed", &g_00c38460, NULL );
			g_00c3849c = true;
		}

		m_6c = m_68 = g_00c38488[0];
		m_a0 = m_98 = g_00c38460[0];
		m_84[0] = m_70[0] = g_00c38470[0];
		m_84[1] = m_70[1] = g_00c38470[0];
	}
}

// FUNCTION: STARFLEETOP 0x005394f4
tMetaMapHex::tMetaMapHex( const tMetaMapHex& other )
	: m_48( std::allocator< int32 >(), std::less< int32 >(), false ),
	  m_70( std::allocator< int32 >(), std::less< int32 >(), false ),
	  m_84( std::allocator< int32 >(), std::less< int32 >(), false )
{
	*this = other;
}

// The SDK's inline destructor; its body in view lets the compiler see the call cannot throw.
// HYPOTHESIS: the original took it from the SDK header; here it is repeated as
// Common/Thread/ThreadPool.cpp and unsorted/tClientCharacter.cpp do.
inline tOptex::~tOptex()
{
	if( fUse != tThreadState::kIgnoreThreadObject )
	{
		if( fUseCriticalSections && fpSharedInfo != NULL )
		{
			::DeleteCriticalSection( &fpSharedInfo->fCriticalSection );
		}

		if( fEvent != NULL )
		{
			::CloseHandle( fEvent );
		}

		if( fFileMapping == NULL )
		{
			delete fpSharedInfo;
		}
		else
		{
			::UnmapViewOfFile( fpSharedInfo );
			::CloseHandle( fFileMapping );
		}
	}
}

// FUNCTION: STARFLEETOP 0x0053943e
// FUNCTION: STARFLEETOP 0x00539464
tMetaMapHex::~tMetaMapHex()
{
	m_bc.Enter();
	m_bc.Leave();
}

// FUNCTION: STARFLEETOP 0x005395b2
tMetaMapHex& tMetaMapHex::operator=( const tMetaMapHex& other )
{
	if( this != &other )
	{
		tDatabaseObject::operator=( other );
		m_48 = other.m_48;
		m_5c = other.m_5c;
		m_60 = other.m_60;
		m_64 = other.m_64;
		m_68 = other.m_68;
		m_6c = other.m_6c;
		m_70 = other.m_70;
		m_84 = other.m_84;
		m_98 = other.m_98;
		m_a0 = other.m_a0;
		m_a8 = other.m_a8;
	}

	return *this;
}

// FUNCTION: STARFLEETOP 0x0053963a
void tMetaMapHex::StreamOut( nDataStore::tOutput& output ) const
{
	tOptex::tGuard guard( &m_bc );

	tDatabaseObject::StreamOut( output );

	// HYPOTHESIS: tHexPoint is a tStreamable; its header does not say so yet.
	output << *(const nDataStore::tStreamable *)&m_a8;

	output << m_48.m_tree.m_size;
	for( fn_0053a6cc_map::iterator i = m_48.begin(); i != m_48.end(); ++i )
	{
		output << (*i).first;
		output << (*i).second;
	}

	output << m_5c;
	output << m_60;
	output << m_64;
	output << m_68;
	output << m_6c;

	output << m_70.m_tree.m_size;
	for( fn_0053a73a_map::iterator j = m_70.begin(); j != m_70.end(); ++j )
	{
		output << (*j).first;
		output << (*j).second;
	}

	output << m_84.m_tree.m_size;
	for( fn_0053a73a_map::iterator k = m_84.begin(); k != m_84.end(); ++k )
	{
		output << (*k).first;
		output << (*k).second;
	}

	output << m_98 << m_a0;
}
