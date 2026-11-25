// HYPOTHESIS: the owner is tMapRelayS (the symbol table puts the preceding vtable function in
// that class); the field at +0xd4 is passed on to the new 0x68-byte object's constructor.
namespace nAsyncMapProcedures {
class tCreateBaseAIs {
public:
	tCreateBaseAIs( void* arg );
	char pad00[0x68];
};
}

class tMapRelayS {
public:
	void fn_005c11cb( nAsyncMapProcedures::tCreateBaseAIs** out );
	char pad00[0xd4];
	void* m_d4;
};

// FUNCTION: STARFLEETOP 0x005c11cb
void tMapRelayS::fn_005c11cb( nAsyncMapProcedures::tCreateBaseAIs** out )
{
	*out = new nAsyncMapProcedures::tCreateBaseAIs( m_d4 );
}
