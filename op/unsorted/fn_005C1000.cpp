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

class tMapRelayS {
public:
	void fn_005c1298( tMemberCallback_tRequestMapData** out );
	void fn_005c120e( tMemberCallback_tInitialize** out );
	void fn_005c11cb( nAsyncMapProcedures::tCreateBaseAIs** out );
	void fn_005c1255( tMemberCallback_tRequestMapSize** out );
	void fn_005c12db( tMemberCallback_tProcessMap** out );
	void fn_005c131e( tMemberCallback_tRequestTerrainAtHex** out );
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
