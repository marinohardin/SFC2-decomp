// tBattles: functions of this class (original file unknown).
#include "Common/Database/DatabaseObject.h"
#include "Common/Database/DatabaseID.h"
#include "Common/DataStore/DataStoreOutput.h"
#include "Common/DataStore/DataStoreInput.h"
#include "unsorted/tMetaMapHex.h"
#include "unsorted/tMissionCompleteResults.h"
class tBattles : public nDatabase::tDatabaseObject
{
public:
	tBattles();
	tBattles( const tBattles& other ); // HYPOTHESIS: its one call is to the assignment operator.
	tBattles& operator=( const tBattles& other );
	virtual ~tBattles();
	tBattles* scalar_deleting_destructor( unsigned char flags );
	virtual	void StreamOut( nDataStore::tOutput& output ) const;
	virtual	void StreamIn( nDataStore::tInput& input );
	virtual void SQLFillInsertStream( std::stringstream* SQL, bool lastOne = false );
	virtual void SQLFillUpdateStream( std::stringstream* SQL, bool lastOne = false );
	virtual const char* SQLTableName() const;
	virtual const char* SQLFieldNames() const;
	void fn_00548192( int32u value );
	void fn_0054815d( int32u value );
	void fn_00548167( int32u value );
	void fn_00548171( nDatabase::tID value );
	void fn_0054819c( const tMetaMapHex& value );
	void fn_005481ab( const tMissionCompleteResults& value );
	int32u m_48; // HYPOTHESIS: 4-byte fields at 0x48 and 0x4c; the constructor leaves them, operator= copies them.
	int32u m_4c;
	nDatabase::tID m_50;
	// HYPOTHESIS: a 4-byte field at 0x60 that operator= copies; 0x64 is padding.
	int32u m_60;
	char fPad_64[0x4];
	tMetaMapHex m_68;
	tMetaMapHex m_138;
	tMissionCompleteResults m_208;
};
// HYPOTHESIS: the function really starts at 0x00547b57 (mov eax, unwind table; call __EH_prolog), so these bytes cannot match until the two addresses are one function; the rest is one for one.
// FUNCTION: STARFLEETOP 0x00547b57
tBattles::tBattles() : m_68( 0 ), m_138( 0 )
{
}

// FUNCTION: STARFLEETOP 0x00547bc5
tBattles* tBattles::scalar_deleting_destructor( unsigned char flags )
{
	// MATCH-ONLY: the scalar deleting destructor is compiler-generated and has no source line,
	// so it is written out as a member: destroy, then delete when bit 0 of the flag is set.
	// HYPOTHESIS: ~tBattles is defined out of line in another unit; it is called, not inlined.
	this->tBattles::~tBattles();
	if( flags & 1 )
		::operator delete( this );
	return this;
}

// FUNCTION: STARFLEETOP 0x00547be1
tBattles::tBattles( const tBattles& other ) : m_68( 0 ), m_138( 0 )
{
	*this = other;
}

// HYPOTHESIS: the function really starts at 0x00547c5f (mov eax, unwind table; call
// __EH_prolog), so these bytes cannot match until the two addresses are one function. After
// that stub the original destroys the same members in the same order: m_208, m_138, m_68,
// m_50, then the tDatabaseObject base.
// FUNCTION: STARFLEETOP 0x00547c5f
tBattles::~tBattles()
{
}

// FUNCTION: STARFLEETOP 0x00547cce
tBattles& tBattles::operator=( const tBattles& other )
{
	if( this != &other )
	{
		// MATCH-ONLY: VC6 rejects the qualified call nDatabase::tDatabaseObject::operator=( other )
		// (C2352), so the base part is assigned through a cast; it is the same call.
		*static_cast<nDatabase::tDatabaseObject*>( this ) = other;
		m_48 = other.m_48;
		m_4c = other.m_4c;
		m_50 = other.m_50;
		m_60 = other.m_60;
		m_68 = other.m_68;
		m_138 = other.m_138;
		m_208 = other.m_208;
	}
	return *this;
}

// MATCH-ONLY: the original's assignment is compiler-generated, emitted here because
// tBattles::operator= uses it; it is written out member by member, which is the same code.
// FUNCTION: STARFLEETOP 0x00547d3b
tMissionCompleteResults& tMissionCompleteResults::operator=( const tMissionCompleteResults& other )
{
	tStreamable::operator=( other );
	m_0c = other.m_0c;
	m_20 = other.m_20;
	return *this;
}

// FUNCTION: STARFLEETOP 0x00547d6e
void tBattles::StreamOut( nDataStore::tOutput& output ) const
{
	tDatabaseObject::StreamOut( output );

	output << m_48 << m_4c << m_50 << m_60 << m_68 << m_138 << m_208;
}

// FUNCTION: STARFLEETOP 0x00547de9
void tBattles::StreamIn( nDataStore::tInput& input )
{
	tDatabaseObject::StreamIn( input );

	input >> m_48 >> m_4c >> m_50 >> m_60 >> m_68 >> m_138 >> m_208;
}

#include <time.h>

// HYPOTHESIS: makes the text of a time for the SQL stream; defined after its caller in this
// unit (0x005480e3).
std::string fn_005480e3( time_t value );

// HYPOTHESIS: the function starts at the stub at 0x00547e67 (see above), so the frame setup
// in the first bytes cannot match until the two addresses are one function.
// FUNCTION: STARFLEETOP 0x00547e67
void tBattles::SQLFillInsertStream( std::stringstream* SQL, bool lastOne )
{
	tDatabaseObject::SQLFillInsertStream( SQL );

	*SQL << "'" << fn_005480e3( m_48 ) << "', " << "'" << fn_005480e3( m_4c ) << "', "
		<< m_208.m_0c.fn_0055f269() << ", " << m_208.m_0c.fn_005e8f21() << ", "
		<< m_50.GetObjectID() << ", " << m_60 << ", "
		<< m_68.fn_0053a414( 0 ) << ", " << m_68.fn_0053a414( 1 ) << ", "
		<< m_68.fn_0053a2fc().find( 0 )->second << ", "
		<< m_68.fn_0053a2fc().find( 1 )->second << ", "
		<< m_138.fn_0053a414( 0 ) << ", " << m_138.fn_0053a414( 1 ) << ", "
		<< m_138.fn_0053a2fc().find( 0 )->second << ", "
		<< m_138.fn_0053a2fc().find( 1 )->second << " )";
}

// FUNCTION: STARFLEETOP 0x005480e3
std::string fn_005480e3( time_t value )
{
	// HYPOTHESIS: the value is a time_t; the text is a SQL date and time. The format string's
	// text (at 0x0085c880 in the exe) is not confirmed; only its arguments are.
	char buffer[100];
	struct tm* local = localtime( &value );
	sprintf( buffer, "%4.4d-%02.2d-%02.2d %02.2d:%02.2d:%02.2d", local->tm_year + 1900, local->tm_mon + 1,
		local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec );
	return std::string( buffer );
}

// HYPOTHESIS: slot 6 of tBattles' table, after SQLFillInsertStream, is SQLFillUpdateStream; the
// body is empty (ret 8). The linker folded identical empty bodies of other classes onto this
// address, which is why the analysis names it after tEntityControlShip.
// FUNCTION: STARFLEETOP 0x0054814e
void tBattles::SQLFillUpdateStream( std::stringstream* SQL, bool lastOne )
{
}

// HYPOTHESIS: slot 8 of the table is SQLTableName; the string's text ("Battles", at 0x0085c8ac)
// is read from the exe.
// FUNCTION: STARFLEETOP 0x00548151
const char* tBattles::SQLTableName() const
{
	return "Battles";
}

// FUNCTION: STARFLEETOP 0x00548157
const char* tBattles::SQLFieldNames() const
{
	return " ( ID, Locked, LockID, BeginTime, EndTime, X, Y, HostID, NumParticipants, "
		"HexBeforeEmpireDV, HexBeforeCartelDV, HexBeforeEmpireRace, HexBeforeCartelRace, "
		"HexAfterEmpireDV, HexAfterCartelDV, HexAfterEmpireRace, HexAfterCartelRace ) ";
	// HYPOTHESIS: slot 9 of the table is SQLFieldNames, the virtual that follows SQLTableName
	// in tDatabaseObject (as in tMetaMapDesc). The text (at 0x0085c8b4) is read from the exe.
}

// HYPOTHESIS: sets the field at 0x48 (the begin time, by the SQL field order); the name is not
// known.
// FUNCTION: STARFLEETOP 0x0054815d
void tBattles::fn_0054815d( int32u value )
{
	m_48 = value;
}

// HYPOTHESIS: sets the field at 0x4c (the end time, by the SQL field order); the name is not
// known. The linker folded an identical body of another class onto this address, which is why
// the analysis names it after tCameraMgrEx. It follows the setter of 0x48 above in the exe, as
// the field at 0x4c follows the field at 0x48 in the class and in the SQL field list; both are
// written with the same parameter type. Its bytes are those of the setter above with 0x4c in
// place of 0x48.
// FUNCTION: STARFLEETOP 0x00548167
void tBattles::fn_00548167( int32u value )
{
	m_4c = value;
}

// HYPOTHESIS: sets the tID at 0x50 (the host, by the SQL field order: HostID is written from
// it); the name is not known. The tID is taken by value, so its destructor runs on return.
// FUNCTION: STARFLEETOP 0x00548171
void tBattles::fn_00548171( nDatabase::tID value )
{
	m_50 = value;
}

// HYPOTHESIS: sets the field at 0x60 (NumParticipants, by the SQL field order); the name is not
// known. It follows the setter of the tID at 0x50 above in the exe.
// FUNCTION: STARFLEETOP 0x00548192
void tBattles::fn_00548192( int32u value )
{
	m_60 = value;
}

// HYPOTHESIS: sets the tMetaMapHex at 0x68 (the hex before the battle, by the SQL field order);
// the name is not known. It follows the setter of the field at 0x60 above in the exe.
// FUNCTION: STARFLEETOP 0x0054819c
void tBattles::fn_0054819c( const tMetaMapHex& value )
{
	m_68 = value;
}

// HYPOTHESIS: sets the tMissionCompleteResults at 0x208 (the exe adds 0x208 to this before the
// call); the name is not known. It follows the setter of the hex at 0x68 above in the exe.
// FUNCTION: STARFLEETOP 0x005481ab
void tBattles::fn_005481ab( const tMissionCompleteResults& value )
{
	m_208 = value;
}

// MATCH-ONLY: the library's assignment of the vector at 0x20 of tMissionCompleteResults,
// emitted in this unit because tMissionCompleteResults::operator= above uses it. The original
// is the template in the library header; it is written out here, line for line, so that it
// has source lines in this unit.
// FUNCTION: STARFLEETOP 0x005481bd
template<>
std::vector< tObject_0053d20b >& std::vector< tObject_0053d20b >::operator=( const std::vector< tObject_0053d20b >& _X )
{
	if( this == &_X )
		;
	else if( _X.size() <= size() )
	{
		iterator _S = std::copy( _X.begin(), _X.end(), _First );
		_Destroy( _S, _Last );
		_Last = _First + _X.size();
	}
	else if( _X.size() <= capacity() )
	{
		const_iterator _S = _X.begin() + size();
		std::copy( _X.begin(), _S, _First );
		_Ucopy( _S, _X.end(), _Last );
		_Last = _First + _X.size();
	}
	else
	{
		_Destroy( _First, _Last );
		allocator.deallocate( _First, _End - _First );
		_First = allocator.allocate( _X.size(), (void*)0 );
		_Last = _Ucopy( _X.begin(), _X.end(), _First );
		_End = _Last;
	}
	return *this;
}

// MATCH-ONLY: the SDK's template constructor, written out as this instance's explicit
// specialization so that its annotation names the one instance; the body is the template's.
// HYPOTHESIS: the function really starts at 0x00548395 (mov eax, unwind table; call
// __EH_prolog), so these bytes cannot match until the two addresses are one function.

// FUNCTION: STARFLEETOP 0x00548395
template<>
tClassAllocator<tBattles>::tClassAllocator()
{
	tDynamicAllocator::Instance()->mAddToAllocList( typeid( tBattles ), this );
}

// HYPOTHESIS: 0x005483d1 (10 bytes, listed as a vfunction1 of this allocator) is not a virtual
// but the opening (mov eax, unwind table; call __EH_prolog) of the function after the
// constructor above, which ends exactly there (0x0054839f + 50 bytes), as at 0x00538fc1 for
// tMetaMapDesc. It has no source of its own. This empty body only claims the address so it is
// built and scored; it is not the original's code and should not be merged as a match.

// MATCH-ONLY: the SDK's template mAllocate, written out as this instance's explicit
// specialization so that its annotation names the one instance; the body is the template's.
// HYPOTHESIS: the function really starts at 0x005483d1 (mov eax, unwind table; call
// __EH_prolog), so these bytes cannot match until the two addresses are one function.

// FUNCTION: STARFLEETOP 0x005483d1
template<>
tAllocatedClass* tClassAllocator<tBattles>::mAllocate()
{
	tBattles* pClass = new tBattles();

	pClass->mSetAllocatorID( fAllocID );

	return pClass;
}

// MATCH-ONLY: the SDK's template mAssignAllocID, written out as this instance's explicit
// specialization so that its annotation names the one instance; the body is the template's.

// FUNCTION: STARFLEETOP 0x0054841d
template<>
void tClassAllocator<tBattles>::mAssignAllocID( iAllocatorID allocID )
{
	fAllocID = allocID;
}

// Not claimed: 0x00548467, 0x00548473 and 0x0054847f are the compiler's atexit stubs (push the
// destructor at 0x004bd431, call atexit, pop, ret) for library statics, and 0x0054848b is its
// adjustor{8} thunk (sub ecx, 8; jmp 0x00547bc5) for the deleting destructor in the tStreamable
// vtable. None has a source line of its own, so the lift keeps the original's bytes for them.






























