#include "unsorted/tServCharacter.h"

// FUNCTION: STARFLEETOP 0x00544719
const char* tServCharacter::SQLTableName() const
{
	return "ServCharacter";
}

// FUNCTION: STARFLEETOP 0x0054471f
const char* tServCharacter::SQLFieldNames() const
{
	return " ( ID, Locked, LockID, WONLogon, LastLoggedOn, MissionsPlayedVectorSize, MissionsPlayedVector, IPAddress, "
		"NextMissionTitle, NextMissionScore, HomeWorldLocationX, HomeWorldLocationY, LastMustPlayLocationX, "
		"LastMustPlayLocationY, PreparedMissionsID, OpenShipBidsSize, OpenShipBids, CharacterName, "
		"ShipCacheVectorSize, ShipCacheVector, CharacterRace, CharacterRank, CharacterPoliticalControl, Flags, "
		"CharacterLastBattleResult, CharacterLocationX, CharacterLocationY, CharacterRating, Medals, "
		"CharacterCurrentPrestige, CharacterLifetimePrestige, CharacterBattlesPlayed, MissionSlotIdx, "
		"MoveCompletes, MoveDestinationX, MoveDestinationY ) ";
}

// HYPOTHESIS: the element class of this vector is not known: 0x10 bytes with a virtual
// destructor, a dword at +4, a byte at +8 and a dword at +0xc, which the assignment copies.
class fn_00544725_elem
{
public:
	virtual ~fn_00544725_elem();

	int m_04;
	char m_08;
	int m_0c;
};

#include <vector>

template<> std::vector< fn_00544725_elem >::iterator std::vector< fn_00544725_elem >::erase( iterator _P );

// HYPOTHESIS: single erase of the vector of the elements above: the elements after it are
// assigned down one place, the last is destroyed and the end moves back.
// FUNCTION: STARFLEETOP 0x00544725
template<>
std::vector< fn_00544725_elem >::iterator std::vector< fn_00544725_elem >::erase( iterator _P )
{
	fn_00544725_elem *last = _Last;
	fn_00544725_elem *dest = _P;
	for ( fn_00544725_elem *src = _P + 1; src != last; ++dest, ++src )
		*dest = *src;
	fn_00544725_elem *e = _Last;
	for ( fn_00544725_elem *p = e - 1; p != e; ++p )
		p->~fn_00544725_elem();
	--_Last;
	return _P;
}

// MATCH-ONLY: a user so that the specialization above is emitted out of line; the original's
// callers of 0x00544725 are in other units.
void fn_00544725_user( std::vector< fn_00544725_elem > *v )
{
	v->erase( v->begin() );
}

template<> std::vector< tServCharacter::tShipCache >::iterator std::vector< tServCharacter::tShipCache >::erase( iterator _P );

// FUNCTION: STARFLEETOP 0x0054477b
template<>
std::vector< tServCharacter::tShipCache >::iterator std::vector< tServCharacter::tShipCache >::erase( iterator _P )
{
	tServCharacter::tShipCache *last = _Last;
	tServCharacter::tShipCache *dest = _P;
	for ( tServCharacter::tShipCache *src = _P + 1; src != last; ++dest, ++src )
		*dest = *src;
	tServCharacter::tShipCache *e = _Last;
	for ( tServCharacter::tShipCache *p = e - 1; p != e; ++p )
		p->~tShipCache();
	--_Last;
	return _P;
}

// MATCH-ONLY: a user so that the specialization above is emitted out of line.
void fn_0054477b_user( std::vector< tServCharacter::tShipCache > *v )
{
	v->erase( v->begin() );
}

// HYPOTHESIS: the destructor of the wrapper at +0x5c of tServCharacter, over the vector whose
// erase is 0x00544725; 0x005447ff is its Delete.
// MATCH-ONLY: written as a specialization so the out-of-line copy sits in this unit.
#include "Common/Thread/SingleAccessWrapper.h"

// The SDK's inline tOptex destructor, the same body tClientCharacter.cpp claims as the game's
// copy (0x004296c8). With its body in view the compiler sets no unwind state before the
// member's destructor call, as the original does.
// MATCH-ONLY: the SDK declares no throw(); the operator delete in the body would otherwise keep
// the unwind state store (as in Source/MetaClient/MetaClientMap/MetaViewPortHandler.cpp).
// HYPOTHESIS: it belongs in Common/Thread/Optex.h, shared by the units, once they can move.
inline tOptex::~tOptex() throw()
{
	if (fUse != tThreadState::kIgnoreThreadObject) {
		if (fUseCriticalSections && fpSharedInfo != NULL)
			::DeleteCriticalSection(&fpSharedInfo->fCriticalSection);
		if (fEvent != NULL)
			::CloseHandle(fEvent);
		if (fFileMapping == NULL) {
			delete fpSharedInfo;
		} else {
			::UnmapViewOfFile(fpSharedInfo);
			::CloseHandle(fFileMapping);
		}
	}
}

template<> void tSingleAccessWrapper< std::vector< fn_00544725_elem > >::Delete();

// FUNCTION: STARFLEETOP 0x005447d0
template<> tSingleAccessWrapper< std::vector< fn_00544725_elem > >::~tSingleAccessWrapper()
{
	Delete();
}

// MATCH-ONLY: a user so that the specialization above is emitted out of line.
void fn_005447d0_user( tSingleAccessWrapper< std::vector< fn_00544725_elem > > *w )
{
	w->~tSingleAccessWrapper();
}

// FUNCTION: STARFLEETOP 0x005447ff
template<> void tSingleAccessWrapper< std::vector< fn_00544725_elem > >::Delete()
{
	fPointerOptex.Enter();
	std::vector< fn_00544725_elem > *p = fpPointer;
	if ( p != 0 )
	{
		delete p;
		fpPointer = 0;
	}
	fPointerOptex.Leave();
}

#include "Common/DataStore/DataStoreOutput.h"

// HYPOTHESIS: the vector writer instantiated for std::string (the size, then each string).
namespace nDataStore
{
// FUNCTION: STARFLEETOP 0x0054491b
template<>
tOutput& operator<<( tOutput& output, const std::vector< std::string >& value )
{
	int32u size = value.size();
	output.Write( (const byte*)&size, sizeof( size ) );

	for( std::vector< std::string >::const_iterator i = value.begin();
		 i != value.end();
		 i++ )
	{
		output << *i;
	}

	return output;
}
}

extern "C" int atexit( void ( *func )( void ) );

void fn_004bd431();

// HYPOTHESIS: arranges at exit the cleanup at 0x004bd431 (Ghidra labels it tApp3dBase::vfunction5, unconfirmed).
// FUNCTION: STARFLEETOP 0x00544b06
void fn_00544b06()
{
	atexit( fn_004bd431 );
}

// FUNCTION: STARFLEETOP 0x00544b12
void fn_00544b12()
{
	atexit( fn_004bd431 );
}
