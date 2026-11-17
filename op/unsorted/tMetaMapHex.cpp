// tBuffer::Write is called out of line (0x0042a0fc).
#define DATASTORE_OUT_OF_LINE_WRITE
#define DATASTORE_OUT_OF_LINE_READ
#include "unsorted/tMetaMapHex.h"
#include "Common/Settings.h"
#include "Common/DataStore/DataStoreOutput.h"
#include "Common/DataStore/DataStoreInput.h"

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

	output << m_5c << m_60 << m_64;
	output << m_68 << m_6c;

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

// FUNCTION: STARFLEETOP 0x00539801
void tMetaMapHex::StreamIn( nDataStore::tInput& input )
{
	tOptex::tGuard guard( &m_bc );

	tDatabaseObject::StreamIn( input );

	int32 x;
	int32 y;
	int32 v48;
	int32 v70;
	int32 v84;
	int32 i;
	int32 j;
	int32 key;
	int32 count;
	int32 value;
	int32 count2;
	int32 value2;

	iObjectVersion version = fObjectVersion;
	fObjectVersion = 1;

	switch( version )
	{
		case 0:
		{
			input >> x >> y >> v48 >> m_5c >> m_60 >> m_64;
			input >> m_68 >> m_6c;
			input >> v70 >> v84;
			input >> m_98 >> m_a0;

			m_48[0] = v48;
			m_a8.fn_005b6b62( x, y );
			m_70[0] = v70;
			m_84[0] = v84;
			break;
		}

		case 1:
		{
			// HYPOTHESIS: tHexPoint is a tStreamable; its header does not say so yet.
			input >> *(nDataStore::tStreamable *)&m_a8;

			input.Read( (byte*)&count, sizeof( count ) );
			for( i = 0; i < count; i++ )
			{
				input.Read( (byte*)&key, sizeof( key ) );
				input.Read( (byte*)&value, sizeof( value ) );
				m_48[key] = value;
			}

			input.Read( (byte*)&m_5c, sizeof( m_5c ) );
			input.Read( (byte*)&m_60, sizeof( m_60 ) );
			input.Read( (byte*)&m_64, sizeof( m_64 ) );
			input.Read( (byte*)&m_68, sizeof( m_68 ) );
			input.Read( (byte*)&m_6c, sizeof( m_6c ) );

			input.Read( (byte*)&count2, sizeof( count2 ) );
			for( j = 0; j < count2; j++ )
			{
				input >> key;
				input >> value2;
				fn_0053a73a_map::value_type entry( key, 0 );
				fn_0053a73a_map::iterator it = m_70.insert( entry ).first;
				(*it).second = value2;
			}

			input >> count2;
			for( j = 0; j < count2; j++ )
			{
				input >> key;
				input >> value2;
				fn_0053a73a_map::value_type entry( key, 0 );
				fn_0053a73a_map::iterator it = m_84.insert( entry ).first;
				(*it).second = value2;
			}

			input >> m_98 >> m_a0;
			break;
		}
	}

	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x00539a9e
void tMetaMapHex::SQLFillInsertStream( std::stringstream* SQL, bool lastOne )
{
	tOptex::tGuard guard( &m_bc );

	tDatabaseObject::SQLFillInsertStream( SQL );

	*SQL << m_a8.fn_0055f269() << ", " << m_a8.fn_005e8f21() << ", ";

	for( fn_0053a6cc_map::iterator i = m_48.begin(); i != m_48.end(); ++i )
	{
		*SQL << (*i).second;
		*SQL << ", ";
	}

	*SQL << m_5c << ", " << m_60 << ", " << m_64 << ", " << m_68 << ", " << m_6c << ", ";

	for( fn_0053a73a_map::iterator j = m_70.begin(); j != m_70.end(); ++j )
	{
		*SQL << (*j).second;
		*SQL << ", ";
	}

	// The original steps the first loop's iterator here, not k (the exe increments its slot).
	for( fn_0053a73a_map::iterator k = m_84.begin(); k != m_84.end(); ++i )
	{
		*SQL << (*k).second;
		*SQL << ", ";
	}

	*SQL << m_98 << ", " << m_a0 << " )";

	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x00539c82
void tMetaMapHex::SQLFillUpdateStream( std::stringstream* SQL, bool lastOne )
{
	tOptex::tGuard guard( &m_bc );

	bool first48 = false;
	bool first84 = false;
	bool first70 = false;

	tDatabaseObject::SQLFillUpdateStream( SQL, lastOne );

	*SQL << "X=" << m_a8.fn_0055f269() << ", Y=" << m_a8.fn_005e8f21() << ", EmpireControl=";

	for( fn_0053a6cc_map::iterator i = m_48.begin(); i != m_48.end(); ++i )
	{
		if( !first48 )
		{
			*SQL << (*i).second;
			*SQL << ", CartelControl=";
			first48 = true;
		}
		else
		{
			*SQL << (*i).second;
		}
	}

	*SQL << ", TerrainType=" << m_5c << ", PlanetType=" << m_60 << ", StarBaseType=" << m_64 << ", BaseEconomicPoints=" << m_68 << ", CurrentEconomicPoints=" << m_6c << ", EmpireBaseVictoryPoints=";

	for( fn_0053a73a_map::iterator j = m_70.begin(); j != m_70.end(); ++j )
	{
		if( !first70 )
		{
			*SQL << (*j).second;
			*SQL << ", CartelBaseVictoryPoints=";
			first70 = true;
		}
		else
		{
			*SQL << (*j).second;
			*SQL << ", EmpireCurrentVictoryPoints=";
		}
	}

	// The original steps the first loop's iterator here, not k, as in SQLFillInsertStream.
	for( fn_0053a73a_map::iterator k = m_84.begin(); k != m_84.end(); ++i )
	{
		if( !first84 )
		{
			*SQL << (*k).second;
			*SQL << ", CartelCurrentVictoryPoints=";
			first84 = true;
		}
		else
		{
			*SQL << (*k).second;
		}
	}

	*SQL << ", BaseSpeedPoints=" << m_98 << ", CurrentSpeedPoints=" << m_a0;

	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x00539ecc
const char* tMetaMapHex::SQLTableName() const
{
	m_bc.Enter();
	m_bc.Leave();
	return "MapHex";
}

// FUNCTION: STARFLEETOP 0x00539ee8
const char* tMetaMapHex::SQLFieldNames() const
{
	m_bc.Enter();
	m_bc.Leave();
	return " ( ID, Locked, LockID, X, Y, EmpireControl, CartelControl, TerrainType, PlanetType, StarbaseType, BaseEconomicPoints, CurrentEconomicPoints, EmpireBaseVictoryPoints, CartelBaseVictoryPoints, EmpireCurrentVictoryPoints, CartelCurrentVictoryPoints, BaseSpeedPoints, CurrentSpeedPoints ) ";
}

// FUNCTION: STARFLEETOP 0x00539f04
long tMetaMapHex::fn_00539f04()
{
	tOptex::tGuard guard( &m_bc );
	long x = m_a8.fn_0055f269();
	guard.Leave();
	return x;
}

// FUNCTION: STARFLEETOP 0x00539f50
long tMetaMapHex::fn_00539f50()
{
	tOptex::tGuard guard( &m_bc );
	long y = m_a8.fn_005e8f21();
	guard.Leave();
	return y;
}

// FUNCTION: STARFLEETOP 0x00539f9c
tHexPoint tMetaMapHex::fn_00539f9c()
{
	tOptex::tGuard guard( &m_bc );
	return m_a8;
}

// FUNCTION: STARFLEETOP 0x00539ff1
std::vector< tHexPoint > tMetaMapHex::fn_00539ff1( int param_1, int param_2 )
{
	tOptex::tGuard guard( &m_bc );

	bool check = true;
	if( param_1 == -1 || param_2 == -1 )
	{
		check = false;
	}

	std::vector< tHexPoint > result;
	result.reserve( 6 );

	int x = m_a8.fn_0055f269();
	int y = m_a8.fn_005e8f21();
	int offset = 0;
	if( x % 2 == 0 )
	{
		offset = -1;
	}

	int left = x - 1;
	tHexPoint point( left, y + offset );
	if( !check || fn_0053a287( point, param_1, param_2 ) )
	{
		result.push_back( point );
	}

	point = tHexPoint( x, y - 1 );
	if( !check || fn_0053a287( point, param_1, param_2 ) )
	{
		result.push_back( point );
	}

	point = tHexPoint( x + 1, y + offset );
	if( !check || fn_0053a287( point, param_1, param_2 ) )
	{
		result.push_back( point );
	}

	point = tHexPoint( x + 1, y + offset + 1 );
	if( !check || fn_0053a287( point, param_1, param_2 ) )
	{
		result.push_back( point );
	}

	point = tHexPoint( x, y + 1 );
	if( !check || fn_0053a287( point, param_1, param_2 ) )
	{
		result.push_back( point );
	}

	point = tHexPoint( left, y + offset + 1 );
	if( !check || fn_0053a287( point, param_1, param_2 ) )
	{
		result.push_back( point );
	}

	return result;
}

// FUNCTION: STARFLEETOP 0x0053a287
bool tMetaMapHex::fn_0053a287( tHexPoint& point, unsigned int param_1, unsigned int param_2 )
{
	tOptex::tGuard guard( &m_bc );
	bool result = false;
	if( point.fn_0055f269() >= 0 && point.fn_005e8f21() >= 0 &&
		point.fn_0055f269() < param_1 && point.fn_005e8f21() < param_2 )
	{
		result = true;
	}
	guard.Leave();
	return result;
}

// FUNCTION: STARFLEETOP 0x0053a2fc
std::map< int32, float >& tMetaMapHex::fn_0053a2fc()
{
	// HYPOTHESIS: m_48 is the map tBattles looks up; its declared stand-in type differs from the
	// std::map the header returns, so the reference is converted here.
	m_bc.Enter();
	m_bc.Leave();
	return ( std::map< int32, float >& )m_48;
}

// FUNCTION: STARFLEETOP 0x0053a31c
int32 tMetaMapHex::fn_0053a31c()
{
	m_bc.Enter();
	int32 type = m_5c;
	m_bc.Leave();
	return type;
}

// FUNCTION: STARFLEETOP 0x0053a33c
int32 tMetaMapHex::fn_0053a33c()
{
	m_bc.Enter();
	int32 type = m_60;
	m_bc.Leave();
	return type;
}

// FUNCTION: STARFLEETOP 0x0053a35c
int32 tMetaMapHex::fn_0053a35c()
{
	m_bc.Enter();
	int32 type = m_64;
	m_bc.Leave();
	return type;
}

// FUNCTION: STARFLEETOP 0x0053a37c
long tMetaMapHex::fn_0053a37c()
{
	m_bc.Enter();
	long points = m_68;
	m_bc.Leave();
	return points;
}

// FUNCTION: STARFLEETOP 0x0053a39c
long tMetaMapHex::fn_0053a39c()
{
	m_bc.Enter();
	long points = m_6c;
	m_bc.Leave();
	return points;
}

// FUNCTION: STARFLEETOP 0x0053a3bc
long tMetaMapHex::fn_0053a3bc( int32 param_1 )
{
	tOptex::tGuard guard( &m_bc );
	fn_0053a73a_map::iterator it = m_70.find( param_1 );
	long value = ( *it ).second;
	guard.Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x0053a414
int32 tMetaMapHex::fn_0053a414( int32 param_1 )
{
	tOptex::tGuard guard( &m_bc );
	fn_0053a73a_map::iterator it = m_84.find( param_1 );
	int32 value = ( *it ).second;
	guard.Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x0053a46f
double tMetaMapHex::fn_0053a46f()
{
	m_bc.Enter();
	double points = m_a0;
	m_bc.Leave();
	return points;
}

// FUNCTION: STARFLEETOP 0x0053a49c
void tMetaMapHex::fn_0053a49c( const tHexPoint& point )
{
	m_a8 = point;
}

// FUNCTION: STARFLEETOP 0x0053a4ae
void tMetaMapHex::fn_0053a4ae( int param_1, int param_2 )
{
	tOptex::tGuard guard( &m_bc );
	m_48[param_1] = param_2;
	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x0053a4fe
void tMetaMapHex::fn_0053a4fe( int32 param_1 )
{
	m_bc.Enter();
	m_5c = param_1;
	m_bc.Leave();
}

// FUNCTION: STARFLEETOP 0x0053a522
void tMetaMapHex::fn_0053a522( int32 param_1 )
{
	m_bc.Enter();
	m_60 = param_1;
	m_bc.Leave();
}

// FUNCTION: STARFLEETOP 0x0053a546
void tMetaMapHex::fn_0053a546( int32 param_1 )
{
	m_bc.Enter();
	m_64 = param_1;
	m_bc.Leave();
}

// FUNCTION: STARFLEETOP 0x0053a56a
void tMetaMapHex::fn_0053a56a( long param_1 )
{
	m_bc.Enter();
	m_68 = param_1;
	m_bc.Leave();
}

// FUNCTION: STARFLEETOP 0x0053a58e
void tMetaMapHex::fn_0053a58e( long param_1 )
{
	m_bc.Enter();
	m_6c = param_1;
	m_bc.Leave();
}

// FUNCTION: STARFLEETOP 0x0053a5b2
void tMetaMapHex::fn_0053a5b2( long param_1, int32 param_2 )
{
	tOptex::tGuard guard( &m_bc );
	m_70[param_2] = param_1;
	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x0053a602
void tMetaMapHex::fn_0053a602( long param_1, int32 param_2 )
{
	tOptex::tGuard guard( &m_bc );
	m_84[param_2] = param_1;
	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x0053a655
void tMetaMapHex::fn_0053a655( double param_1 )
{
	m_bc.Enter();
	m_98 = param_1;
	m_bc.Leave();
}

// FUNCTION: STARFLEETOP 0x0053a67c
void tMetaMapHex::fn_0053a67c( double param_1 )
{
	m_bc.Enter();
	m_a0 = param_1;
	m_bc.Leave();
}

// FUNCTION: STARFLEETOP 0x0053a6a3
int32u& fn_0053a6cc_map::operator[]( const int& key )
{
	std::pair< iterator, bool > result = insert( value_type( key, int32u() ) );
	return (*result.first).second;
}

// FUNCTION: STARFLEETOP 0x0053a6cc
fn_0053a6cc_map::tree::~tree()
{
	erase( iterator( m_head->left ), iterator( m_head ) );
	operator delete( m_head );
	m_head = 0, m_size = 0;
	operator delete( m_nil );
	m_nil = 0;
}

// FUNCTION: STARFLEETOP 0x0053a704
fn_0053a6cc_map& fn_0053a6cc_map::operator=( const fn_0053a6cc_map& other )
{
	if( this != &other )
	{
		m_tree.erase( iterator( m_tree.m_head->left ), iterator( m_tree.m_head ) );
		m_tree.m_comp = other.m_tree.m_comp;
		m_tree.fn_0053a981( other.m_tree );
	}
	return *this;
}

// FUNCTION: STARFLEETOP 0x0053a73a
int32& fn_0053a73a_map::operator[]( const int32& key )
{
	std::pair< iterator, bool > result = insert( value_type( key, int32() ) );
	return (*result.first).second;
}

// FUNCTION: STARFLEETOP 0x0053a763
fn_0053a73a_map::tree::~tree()
{
	erase( iterator( m_head->left ), iterator( m_head ) );
	operator delete( m_head );
	m_head = 0, m_size = 0;
	operator delete( m_nil );
	m_nil = 0;
}

// FUNCTION: STARFLEETOP 0x0053a79b
fn_0053a73a_map& fn_0053a73a_map::operator=( const fn_0053a73a_map& other )
{
	if( this != &other )
	{
		m_tree.erase( iterator( m_tree.m_head->left ), iterator( m_tree.m_head ) );
		m_tree.m_comp = other.m_tree.m_comp;
		m_tree.fn_0053aab4( other.m_tree );
	}
	return *this;
}

// HYPOTHESIS: the copy constructor of this compiler's std::vector< tHexPoint > (0x14-byte
// elements); it is written out here so that the out-of-line copy has a definition.
struct fn_0053a7d1_vector
{
	fn_0053a7d1_vector( const fn_0053a7d1_vector& x );
	void reserve( unsigned int n );
	std::allocator< tHexPoint > m_alloc;
	tHexPoint*	m_first;
	tHexPoint*	m_last;
	tHexPoint*	m_end;
};

// HYPOTHESIS: the out-of-line element copy construction 0x0042c9c5 (the library's _Construct).
void fn_0042c9c5( tHexPoint* dst, const tHexPoint& src );

// FUNCTION: STARFLEETOP 0x0053a7d1
fn_0053a7d1_vector::fn_0053a7d1_vector( const fn_0053a7d1_vector& x )
	: m_alloc( x.m_alloc )
{
	int count = ( x.m_first != 0 ? x.m_last - x.m_first : 0 );
	if( count < 0 )
		count = 0;
	m_first = (tHexPoint*)operator new( count * sizeof( tHexPoint ) );
	tHexPoint* last = x.m_last;
	tHexPoint* src = x.m_first;
	tHexPoint* dst = m_first;
	for( ; src != last; ++src, ++dst )
		fn_0042c9c5( dst, *src );
	m_last = dst;
	m_end = dst;
}

// MATCH-ONLY: the library's uninitialized_copy and _Destroy, inlined by the original; their
// parameters become the locals the loops keep.
inline tHexPoint* fn_0053a83f_copy( tHexPoint* first, tHexPoint* last, tHexPoint* dst )
{
	for( ; first != last; ++dst, ++first )
		fn_0042c9c5( dst, *first );
	return dst;
}

inline void fn_0053a83f_destroy( tHexPoint* first, tHexPoint* last )
{
	for( ; first != last; ++first )
		first->~tHexPoint();
}

// FUNCTION: STARFLEETOP 0x0053a83f
void fn_0053a7d1_vector::reserve( unsigned int n )
{
	if( ( m_first == 0 ? 0 : m_end - m_first ) < n )
	{
		int count = n;
		if( count < 0 )
			count = 0;
		tHexPoint* s = (tHexPoint*)operator new( count * sizeof( tHexPoint ) );
		fn_0053a83f_copy( m_first, m_last, s );
		fn_0053a83f_destroy( m_first, m_last );
		operator delete( m_first );
		m_end = s + n;
		int size = ( m_first == 0 ? 0 : m_last - m_first );
		m_first = s;
		m_last = s + size;
	}
}

// FUNCTION: STARFLEETOP 0x0053a8ff
fn_0053a6cc_map::iterator fn_0053a6cc_map::begin() const
{
	return iterator( m_tree.m_head->left );
}

// FUNCTION: STARFLEETOP 0x0053a90d
fn_0053a6cc_map::iterator fn_0053a6cc_map::tree::erase( iterator first, iterator last )
{
	if( m_size == 0 || first.m_node != m_head->left || last.m_node != m_head )
	{
		while( first != last )
			erase( first++ );
		return first;
	}
	else
	{
		fn_0053ab9a( m_head->parent );
		m_head->parent = m_nil;
		m_size = 0;
		m_head->left = m_head;
		m_head->right = m_head;
		return iterator( m_head->left );
	}
}

// FUNCTION: STARFLEETOP 0x0053a981
void fn_0053a6cc_map::tree::fn_0053a981( const tree& other )
{
	_Root() = fn_0053ab16( other._Root(), m_head );
	m_size = other.m_size;
	if( _Root() != m_nil )
	{
		_Lmost() = _Min( _Root() );
		_Rmost() = _Max( _Root() );
	}
	else
	{
		_Lmost() = m_head, _Rmost() = m_head;
	}
}

// FUNCTION: STARFLEETOP 0x0053a9e3
std::pair< fn_0053a6cc_map::iterator, bool > fn_0053a6cc_map::insert( const value_type& v )
{
	std::pair< iterator, bool > result = m_tree.insert( v );
	return std::pair< iterator, bool >( result.first, result.second );
}

// FUNCTION: STARFLEETOP 0x0053aa06
fn_0053a73a_map::iterator fn_0053a73a_map::tree::erase( iterator first, iterator last )
{
	if( m_size == 0 || first.m_node != m_head->left || last.m_node != m_head )
	{
		while( first != last )
			erase( first++ );
		return first;
	}
	else
	{
		fn_0053ac4e( m_head->parent );
		m_head->parent = m_nil;
		m_size = 0;
		m_head->left = m_head;
		m_head->right = m_head;
		return iterator( m_head->left );
	}
}
