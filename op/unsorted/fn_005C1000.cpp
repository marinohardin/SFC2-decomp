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

class tMapRelayS {
public:
	void fn_005c120e( tMemberCallback_tInitialize** out );
	void fn_005c11cb( nAsyncMapProcedures::tCreateBaseAIs** out );
	void fn_005c1255( tMemberCallback_tRequestMapSize** out );
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
