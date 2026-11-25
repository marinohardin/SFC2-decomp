// HYPOTHESIS: the owner is tMapRelayS (the symbol table puts the preceding vtable function in
// that class); the field at +0xd4 is passed on to the new 0x68-byte object's constructor.
namespace nAsyncMapProcedures {
class tCreateBaseAIs {
public:
	tCreateBaseAIs( void* arg );
	char pad00[0x68];
};
}

// HYPOTHESIS: the 0x1cc-byte object is the class whose constructor is at 0x005aca6f (the symbol
// table calls it tMemberCallback<nAsyncMapProcedures::tInitialize, nDataStore::tBuffer const*>,
// which does not fit: this constructor takes two pointers). Real name and header unknown.
class tMemberCallback_tInitialize {
public:
	tMemberCallback_tInitialize( void* a, void* b );
	char pad00[0x1cc];
};

// HYPOTHESIS: the 0x48-byte object built at 0x005b3286 (the symbol table calls it
// tMemberCallback<nAsyncMapProcedures::tRequestMapSize, nDataStore::tBuffer const*>, which does
// not fit a one-argument constructor). Real name and header unknown.
class tMemberCallback_tRequestMapSize {
public:
	tMemberCallback_tRequestMapSize( void* a );
	char pad00[0x48];
};

// HYPOTHESIS: the object built at 0x005b2dbc (the symbol table calls it
// tMemberCallback<nAsyncMapProcedures::tRequestMapData, nDataStore::tBuffer const*>); size unknown.
class tMemberCallback_tRequestMapData {
public:
	tMemberCallback_tRequestMapData( void* a );
	char pad00[0x48];
};

// HYPOTHESIS: the object built at 0x005b26e4 (the symbol table calls it
// tMemberCallback<nAsyncMapProcedures::tProcessMap, nDataStore::tBuffer const*>); one-argument
// constructor and 0x48-byte size taken from the call site. Real name and header unknown.
class tMemberCallback_tProcessMap {
public:
	tMemberCallback_tProcessMap( void* a );
	char pad00[0x48];
};

// HYPOTHESIS: the object built at 0x005b349b (the symbol table calls it
// tMemberCallback<nAsyncMapProcedures::tRequestTerrainAtHex, nDataStore::tBuffer const*>); size unknown.
class tMemberCallback_tRequestTerrainAtHex {
public:
	tMemberCallback_tRequestTerrainAtHex( void* a );
	char pad00[0x48];
};

class tMemberCallback_tMoveAccepted;
class tMemberCallback_tCharacterLoggedOn;
class tMemberCallback_tTurnBreak;
class tMemberCallback_tGetStartLocation;
class tMemberCallback_tGetHex;
class tMemberCallback_tGetHexForUpdate;

class tMapRelayS {
public:
	void fn_005c14b5( tMemberCallback_tGetHex** out );
	void fn_005c14f8( tMemberCallback_tGetHexForUpdate** out );
	void fn_005c1472( tMemberCallback_tGetStartLocation** out );
	void fn_005c13e8( tMemberCallback_tCharacterLoggedOn** out );
	void fn_005c13a2( tMemberCallback_tMoveAccepted** out );
	void fn_005c1298( tMemberCallback_tRequestMapData** out );
	void fn_005c120e( tMemberCallback_tInitialize** out );
	void fn_005c11cb( nAsyncMapProcedures::tCreateBaseAIs** out );
	void fn_005c1255( tMemberCallback_tRequestMapSize** out );
	void fn_005c12db( tMemberCallback_tProcessMap** out );
	void fn_005c131e( tMemberCallback_tRequestTerrainAtHex** out );
	void fn_005c142b( tMemberCallback_tTurnBreak** out );
	char pad00[0xd4];
	void* m_d4;
};

// FUNCTION: STARFLEETOP 0x005c11cb
void tMapRelayS::fn_005c11cb( nAsyncMapProcedures::tCreateBaseAIs** out )
{
	*out = new nAsyncMapProcedures::tCreateBaseAIs( m_d4 );
}

// FUNCTION: STARFLEETOP 0x005c120e
void tMapRelayS::fn_005c120e( tMemberCallback_tInitialize** out )
{
	*out = new tMemberCallback_tInitialize( m_d4, this );
}

// FUNCTION: STARFLEETOP 0x005c1255
void tMapRelayS::fn_005c1255( tMemberCallback_tRequestMapSize** out )
{
	*out = new tMemberCallback_tRequestMapSize( m_d4 );
}

// FUNCTION: STARFLEETOP 0x005c1298
void tMapRelayS::fn_005c1298( tMemberCallback_tRequestMapData** out )
{
	*out = new tMemberCallback_tRequestMapData( m_d4 );
}

// FUNCTION: STARFLEETOP 0x005c12db
void tMapRelayS::fn_005c12db( tMemberCallback_tProcessMap** out )
{
	*out = new tMemberCallback_tProcessMap( m_d4 );
}

// FUNCTION: STARFLEETOP 0x005c131e
void tMapRelayS::fn_005c131e( tMemberCallback_tRequestTerrainAtHex** out )
{
	*out = new tMemberCallback_tRequestTerrainAtHex( m_d4 );
}

// HYPOTHESIS: the object built at 0x005adf3c (the symbol table calls it
// tMemberCallback<nAsyncMapProcedures::tMoveAccepted, nDataStore::tBuffer const*>); one-argument
// constructor, size unknown. Real name and header unknown.
class tMemberCallback_tMoveAccepted {
public:
	tMemberCallback_tMoveAccepted( void* a );
	char pad00[0xc0];
};

// FUNCTION: STARFLEETOP 0x005c13a2
void tMapRelayS::fn_005c13a2( tMemberCallback_tMoveAccepted** out )
{
	*out = new tMemberCallback_tMoveAccepted( m_d4 );
}

// HYPOTHESIS: the object built at 0x005aa306 (the symbol table calls it
// tMemberCallback<nAsyncMapProcedures::tCharacterLoggedOn, nDataStore::tBuffer const*>); size unknown.
class tMemberCallback_tCharacterLoggedOn {
public:
	tMemberCallback_tCharacterLoggedOn( void* a );
	char pad00[0x4c];
};

// FUNCTION: STARFLEETOP 0x005c13e8
void tMapRelayS::fn_005c13e8( tMemberCallback_tCharacterLoggedOn** out )
{
	*out = new tMemberCallback_tCharacterLoggedOn( m_d4 );
}

// HYPOTHESIS: the object built at 0x005b431c (the symbol table calls it
// tMemberCallback<nAsyncMapProcedures::tTurnBreak, nDataStore::tBuffer const*>); size unknown.
class tMemberCallback_tTurnBreak {
public:
	tMemberCallback_tTurnBreak( tMapRelayS* a, void* b );
	char pad00[0xf4];
};

// FUNCTION: STARFLEETOP 0x005c142b
void tMapRelayS::fn_005c142b( tMemberCallback_tTurnBreak** out )
{
	*out = new tMemberCallback_tTurnBreak( this, m_d4 );
}

// HYPOTHESIS: the object built at 0x005ac897 (the symbol table calls it
// tMemberCallback<nAsyncMapProcedures::tGetStartLocation, nDataStore::tBuffer const*>); size
// taken from its siblings. Real name and header unknown.
class tMemberCallback_tGetStartLocation {
public:
	tMemberCallback_tGetStartLocation( void* a );
	char pad00[0x48];
};

// FUNCTION: STARFLEETOP 0x005c1472
void tMapRelayS::fn_005c1472( tMemberCallback_tGetStartLocation** out )
{
	*out = new tMemberCallback_tGetStartLocation( m_d4 );
}

// HYPOTHESIS: the object built at 0x005abbd9 (the symbol table calls it
// tMemberCallback<nAsyncMapProcedures::tGetHex, nDataStore::tBuffer const*>); size taken from its siblings.
class tMemberCallback_tGetHex {
public:
	tMemberCallback_tGetHex( void* a );
	char pad00[0x48];
};

// FUNCTION: STARFLEETOP 0x005c14b5
void tMapRelayS::fn_005c14b5( tMemberCallback_tGetHex** out )
{
	*out = new tMemberCallback_tGetHex( m_d4 );
}

// HYPOTHESIS: the object built at 0x005abdc0 (the symbol table calls it
// tMemberCallback<nAsyncMapProcedures::tGetHexForUpdate, nDataStore::tBuffer const*>); takes no
// argument, size 0x74 from the allocation. Real name and header unknown.
class tMemberCallback_tGetHexForUpdate {
public:
	tMemberCallback_tGetHexForUpdate();
	char pad00[0x74];
};

// FUNCTION: STARFLEETOP 0x005c14f8
void tMapRelayS::fn_005c14f8( tMemberCallback_tGetHexForUpdate** out )
{
	*out = new tMemberCallback_tGetHexForUpdate;
}
