// tMetaMapDesc: functions of this class (original file unknown).
#include "Common/Database/DatabaseObject.h"
#include "Common/DataStore/DataStoreOutput.h"
#include "Common/DataStore/DataStoreInput.h"
class tMetaMapDesc : public nDatabase::tDatabaseObject
{
public:
	tMetaMapDesc();	// HYPOTHESIS: defined at 0x00538c9b, called by the allocator's mAllocate.
	// HYPOTHESIS: the three arguments are stored as they come; their meaning is not known.
	tMetaMapDesc( void** param_1, int32u param_2, bool param_3 );

	virtual	void StreamOut( nDataStore::tOutput& output ) const;
	virtual	void StreamIn( nDataStore::tInput& input );
// SQL Operations
	virtual void SQLFillInsertStream( std::stringstream* SQL, bool lastOne = false );
	virtual void SQLFillUpdateStream( std::stringstream* SQL, bool lastOne = false );
	virtual const char* SQLTableName() const;
	virtual const char* SQLFieldNames() const;
	// HYPOTHESIS: streamed and written to SQL as a 32-bit number, so it may not be a pointer.
	void** m_48;
	int32u m_4c;
	bool m_50;
};

// FUNCTION: STARFLEETOP 0x00538cca
tMetaMapDesc::tMetaMapDesc( void** param_1, int32u param_2, bool param_3 )
	: m_48( param_1 ),
	  m_4c( param_2 ),
	  m_50( param_3 )
{
	// HYPOTHESIS: the class streams as version 1; the base is built with the default 0.
	fObjectVersion = 1;
}

// Writes the base object's fields, then this description's three fields, at the current
// version (1).

// FUNCTION: STARFLEETOP 0x00538d00
void tMetaMapDesc::StreamOut( nDataStore::tOutput& output ) const
{
	tDatabaseObject::StreamOut( output );

	// HYPOTHESIS: m_48 is written as a 32-bit value, so it may not be a pointer after all.
	output << (int32u)m_48 << m_4c << m_50;
}

// Reads the base object's fields, then this description's fields in the layout of the
// version they were written with, and leaves the object at the current version (1).
//
// Version 0 has no flag; version 1 adds it.

// FUNCTION: STARFLEETOP 0x00538d3f
void tMetaMapDesc::StreamIn( nDataStore::tInput& input )
{
	tDatabaseObject::StreamIn( input );

	iObjectVersion version = fObjectVersion;
	fObjectVersion = 1;

	switch( version )
	{
	case 0:
		// HYPOTHESIS: version 0 streams no flag; it reads as false.
		input >> (int32u&)m_48 >> m_4c;
		m_50 = false;
		break;
	case 1:
		input >> (int32u&)m_48 >> m_4c >> m_50;
		break;
	}
}

// FUNCTION: STARFLEETOP 0x00538da5
void tMetaMapDesc::SQLFillInsertStream( std::stringstream* SQL, bool lastOne )
{
	tDatabaseObject::SQLFillInsertStream( SQL );

	*SQL << (int32u)m_48 << ", " << m_4c << ", " << m_50 << " )";
}

// Writes the base object's update assignments, then this description's three fields as
// "Width=..., Height=..., CorrectedBaseAIs=..." (the column names of the MapDescription
// table).
// HYPOTHESIS: m_48, m_4c and m_50 are the map's width, height and the corrected-base-AIs
// flag, going by the column each one is written under.

// FUNCTION: STARFLEETOP 0x00538dfe
void tMetaMapDesc::SQLFillUpdateStream( std::stringstream* SQL, bool lastOne )
{
	tDatabaseObject::SQLFillUpdateStream( SQL, lastOne );

	*SQL << "Width=" << (int32u)m_48 << ", Height=" << m_4c << ", CorrectedBaseAIs=" << m_50;
}

// FUNCTION: STARFLEETOP 0x00538e66
const char* tMetaMapDesc::SQLTableName() const
{
	return "MapDescription";
}

// The column list of the MapDescription table, in the order SQLFillInsertStream writes the
// values: the base object's ID, Locked and LockID, then this description's three fields.
// The leading and trailing spaces let it sit between "INSERT INTO <table>" and "VALUES".
// HYPOTHESIS: this is the SDK's SQLFieldNames, the virtual that follows SQLTableName in
// tDatabaseObject; the exe's vtable gives it the next slot.

// FUNCTION: STARFLEETOP 0x00538e6c
const char* tMetaMapDesc::SQLFieldNames() const
{
	return " ( ID, Locked, LockID, Width, Height, CorrectedBaseAIs ) ";
}

// Not claimed: 0x00538e72 is the library's basic_ostream<char>::operator<<(bool), an inline
// template instance the SQL streams above use, and 0x00538f3e is the tail its try block falls
// through to after its catch handler (no prologue; it runs in the instance's frame). Neither
// has source in game/, so the lift keeps the original's bytes for them.



















// MATCH-ONLY: the SDK's template constructor, written out as this instance's explicit
// specialization so that its annotation names the one instance; the body is the template's.
// HYPOTHESIS: the function really starts at 0x00538f85 (mov eax, unwind table; call
// __EH_prolog), so these bytes cannot match until the two addresses are one function.

// FUNCTION: STARFLEETOP 0x00538f85
template<>
tClassAllocator<tMetaMapDesc>::tClassAllocator()
{
	tDynamicAllocator::Instance()->mAddToAllocList( typeid( tMetaMapDesc ), this );
}

// HYPOTHESIS: 0x00538fc1 (10 bytes, listed as a vfunction1 of this allocator) is not a
// virtual but the compiler's exception-handler stub for the constructor above, which ends
// exactly there (0x00538f8f + 50 bytes); the unwind data pairs it with the funclet at
// 0x007697a4. It has no source of its own and is emitted with the constructor once that
// matches. This empty body only claims the address so it is built and scored; it is not the
// original's code and should not be merged as a match.

// MATCH-ONLY: the SDK's template mAllocate, written out as this instance's explicit
// specialization so that its annotation names the one instance; the body is the template's.
// HYPOTHESIS: the function really starts at 0x00538fc1 (mov eax, unwind table; call
// __EH_prolog), so these bytes cannot match until the two addresses are one function.

// FUNCTION: STARFLEETOP 0x00538fc1
template<>
tAllocatedClass* tClassAllocator<tMetaMapDesc>::mAllocate()
{
	tMetaMapDesc* pClass = new tMetaMapDesc();

	pClass->mSetAllocatorID( fAllocID );

	return pClass;
}

// MATCH-ONLY: the SDK's template mAssignAllocID, written out as this instance's explicit
// specialization so that its annotation names the one instance; the body is the template's.

// FUNCTION: STARFLEETOP 0x0053900a
template<>
void tClassAllocator<tMetaMapDesc>::mAssignAllocID( iAllocatorID allocID )
{
	fAllocID = allocID;
}

// MATCH-ONLY: the scalar deleting destructor is compiler-generated and has no source line,
// so it is written out as a member: destroy, then delete when bit 0 of the flag is set.

// FUNCTION: STARFLEETOP 0x00539016
template<>
tClassAllocator<tMetaMapDesc>* tClassAllocator<tMetaMapDesc>::scalar_deleting_destructor( unsigned char flags )
{
	this->tClassAllocator<tMetaMapDesc>::~tClassAllocator();
	if( flags & 1 )
		::operator delete( this );
	return this;
}

// Not claimed: 0x00539070, 0x0053907c and 0x00539088 are the compiler's atexit stubs (push the
// destructor at 0x004bd431, call atexit, pop, ret) for library statics, and 0x00539094 is its
// adjustor{8} thunk (sub ecx, 8; jmp 0x00538ca9) for the deleting destructor in the tStreamable
// vtable. None has a source line of its own, so the lift keeps the original's bytes for them.































