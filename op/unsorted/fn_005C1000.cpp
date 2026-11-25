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
class tMemberCallback_tUpdateHex;
class tMemberCallback_tChangeHexAttribute;
class tMemberCallback_tBaseTransitionVictoryPointAdjustment;
class tMemberCallback_fn_005c15f3;
class tMemberCallback_tMoveAIs;
class tMemberCallback_tEconomicStatusRequest;
class tMemberCallback_tGetPoliticalTension;
class tMemberCallback_tGetPoliticalTensionMatrix;
class tMemberCallback_tGetPoliticalTensionMatrixForUpdate;
class tMemberCallback_tUpdatePoliticalTensionMatrix;
class tMapRelayS {
public:
	void fn_005c1531( tMemberCallback_tUpdateHex** out );
	void fn_005c1574( tMemberCallback_tChangeHexAttribute** out );
	void fn_005c15ba( tMemberCallback_tBaseTransitionVictoryPointAdjustment** out );
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
	void fn_005c15f3( tMemberCallback_fn_005c15f3** out );
	void fn_005c1639( tMemberCallback_tMoveAIs** out );
	void fn_005c167f( tMemberCallback_tEconomicStatusRequest** out );
	void fn_005c16c2( tMemberCallback_tGetPoliticalTension** out );
	void fn_005c1705( tMemberCallback_tGetPoliticalTensionMatrix** out );
	void fn_005c1748( tMemberCallback_tGetPoliticalTensionMatrixForUpdate** out );
	void fn_005c1781( tMemberCallback_tUpdatePoliticalTensionMatrix** out );
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

// HYPOTHESIS: the object built at 0x005b4a0b (the symbol table calls it
// tMemberCallback<nAsyncMapProcedures::tUpdateHex, nDataStore::tBuffer const*>); size unknown.
class tMemberCallback_tUpdateHex {
public:
	tMemberCallback_tUpdateHex( void* a );
	char pad00[0x74];
};

// FUNCTION: STARFLEETOP 0x005c1531
void tMapRelayS::fn_005c1531( tMemberCallback_tUpdateHex** out )
{
	*out = new tMemberCallback_tUpdateHex( m_d4 );
}

// HYPOTHESIS: the object built at 0x005a9b56 (the symbol table calls it
// tMemberCallback<nAsyncMapProcedures::tChangeHexAttribute, nDataStore::tBuffer const*>);
// one-argument constructor, size above 0x7f from the allocation. Real name and header unknown.
class tMemberCallback_tChangeHexAttribute {
public:
	tMemberCallback_tChangeHexAttribute( void* a );
	char pad00[0x88];
};

// FUNCTION: STARFLEETOP 0x005c1574
void tMapRelayS::fn_005c1574( tMemberCallback_tChangeHexAttribute** out )
{
	*out = new tMemberCallback_tChangeHexAttribute( m_d4 );
}

// HYPOTHESIS: the object built at 0x005a94bd (the symbol table calls it
// tMemberCallback<nAsyncMapProcedures::tBaseTransitionVictoryPointAdjustment, nDataStore::tBuffer const*>).
class tMemberCallback_tBaseTransitionVictoryPointAdjustment {
public:
	tMemberCallback_tBaseTransitionVictoryPointAdjustment();
	char pad00[0x44];
};

// FUNCTION: STARFLEETOP 0x005c15ba
void tMapRelayS::fn_005c15ba( tMemberCallback_tBaseTransitionVictoryPointAdjustment** out )
{
	*out = new tMemberCallback_tBaseTransitionVictoryPointAdjustment;
}

// HYPOTHESIS: the object built at 0x005b3701; size, constructor arguments and real name unknown.
class tMemberCallback_fn_005c15f3 {
public:
	tMemberCallback_fn_005c15f3( void* a );
	char pad00[0xc0];
};

// FUNCTION: STARFLEETOP 0x005c15f3
void tMapRelayS::fn_005c15f3( tMemberCallback_fn_005c15f3** out )
{
	*out = new tMemberCallback_fn_005c15f3( m_d4 );
}

// HYPOTHESIS: the object built at 0x005ae434 (the symbol table calls it a tMemberCallback
// instance for nAsyncMapProcedures::tMoveAIs); size and arguments guessed.
class tMemberCallback_tMoveAIs {
public:
	tMemberCallback_tMoveAIs( void* a );
	char pad00[0x94];
};

// FUNCTION: STARFLEETOP 0x005c1639
void tMapRelayS::fn_005c1639( tMemberCallback_tMoveAIs** out )
{
	*out = new tMemberCallback_tMoveAIs( m_d4 );
}

// HYPOTHESIS: the object built at 0x005ab671 (the symbol table calls it a tMemberCallback
// instance for nAsyncMapProcedures::tEconomicStatusRequest); size guessed.
class tMemberCallback_tEconomicStatusRequest {
public:
	tMemberCallback_tEconomicStatusRequest( void* a );
	char pad00[0x48];
};

// FUNCTION: STARFLEETOP 0x005c167f
void tMapRelayS::fn_005c167f( tMemberCallback_tEconomicStatusRequest** out )
{
	*out = new tMemberCallback_tEconomicStatusRequest( m_d4 );
}

// HYPOTHESIS: the object built at 0x005ac284 (the symbol table calls it a tMemberCallback
// instance for nAsyncMapProcedures::tGetPoliticalTension); size guessed. Real name and header unknown.
class tMemberCallback_tGetPoliticalTension {
public:
	tMemberCallback_tGetPoliticalTension( void* a );
	char pad00[0x48];
};

// FUNCTION: STARFLEETOP 0x005c16c2
void tMapRelayS::fn_005c16c2( tMemberCallback_tGetPoliticalTension** out )
{
	*out = new tMemberCallback_tGetPoliticalTension( m_d4 );
}

// HYPOTHESIS: the object built at 0x005ac452 (the symbol table calls it a tMemberCallback
// instance for nAsyncMapProcedures::tGetPoliticalTensionMatrix); size guessed. Real name and header unknown.
class tMemberCallback_tGetPoliticalTensionMatrix {
public:
	tMemberCallback_tGetPoliticalTensionMatrix( void* a );
	char pad00[0x48];
};

// FUNCTION: STARFLEETOP 0x005c1705
void tMapRelayS::fn_005c1705( tMemberCallback_tGetPoliticalTensionMatrix** out )
{
	*out = new tMemberCallback_tGetPoliticalTensionMatrix( m_d4 );
}

// HYPOTHESIS: the object built at 0x005ac60b (the symbol table calls it a tMemberCallback
// instance for nAsyncMapProcedures::tGetPoliticalTensionMatrixForUpdate); no argument, size 0x60 from the allocation.
class tMemberCallback_tGetPoliticalTensionMatrixForUpdate {
public:
	tMemberCallback_tGetPoliticalTensionMatrixForUpdate();
	char pad00[0x60];
};

// FUNCTION: STARFLEETOP 0x005c1748
void tMapRelayS::fn_005c1748( tMemberCallback_tGetPoliticalTensionMatrixForUpdate** out )
{
	*out = new tMemberCallback_tGetPoliticalTensionMatrixForUpdate;
}

// HYPOTHESIS: the object built at 0x005b4d87, a tMemberCallback instance for
// nAsyncMapProcedures::tUpdatePoliticalTensionMatrix with a tBuffer const* argument.
class tMemberCallback_tUpdatePoliticalTensionMatrix {
public:
	tMemberCallback_tUpdatePoliticalTensionMatrix( void* a );
	char pad00[0xc4];
};

// FUNCTION: STARFLEETOP 0x005c1781
void tMapRelayS::fn_005c1781( tMemberCallback_tUpdatePoliticalTensionMatrix** out )
{
	*out = new tMemberCallback_tUpdatePoliticalTensionMatrix( m_d4 );
}