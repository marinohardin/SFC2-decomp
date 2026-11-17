// Unit:      unsorted/fn_004D3143.cpp
// Functions: 0x004d3143 to 0x004d3b8d, 61 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)

// HYPOTHESIS: the global at 0x0092e4a0 is an object whose method 0x004d482a takes four
// longs; its class is not recovered yet.
class fn_004d3ef7_object;

class fn_004d482a_owner
{
public:
	long fn_004d482a( long arg1, long arg2, long arg3, long arg4 );
	fn_004d3ef7_object* fn_004d3ef7( long arg1 );
	fn_004d3ef7_object* fn_004d4a02( long arg1, long arg2, long arg3, long arg4 );
	fn_004d3ef7_object* fn_004d42e6( long arg1, long arg2, long arg3, long arg4 );
	fn_004d3ef7_object* fn_004d40b1( long arg1, long arg2, long arg3, long arg4, long arg5, long arg6 );
};

extern fn_004d482a_owner g_0092e4a0;

// FUNCTION: STARFLEETOP 0x004d315e
long fn_004d315e( long arg1, long arg2, long arg3, long arg4 )
{
	return g_0092e4a0.fn_004d482a( arg1, arg2, arg3, arg4 );
}

// HYPOTHESIS: the global at 0x009254f8 is the object whose method 0x004bf6b9 takes two longs;
// its class is declared only inside fn_004D2000.cpp (fn_004cb4a7_target).
class fn_004d3ef7_object;

class fn_004bf6b9_owner
{
public:
	void fn_004bf6b9( long arg1, long arg2 );
	void fn_004bf6ea( long arg1 );
	void fn_004bf74f( long arg1, float arg2 );
	void fn_004caf1b( fn_004d3ef7_object* arg1 );
	void fn_004caf40( fn_004d3ef7_object* arg1, long arg2 );
};

extern fn_004bf6b9_owner g_009254f8;

// FUNCTION: STARFLEETOP 0x004d3179
void fn_004d3179( long arg1, long arg2 )
{
	g_009254f8.fn_004bf6b9( arg1, arg2 );
}

// FUNCTION: STARFLEETOP 0x004d318c
void fn_004d318c( long arg1 )
{
	g_009254f8.fn_004bf6ea( arg1 );
}

// FUNCTION: STARFLEETOP 0x004d319b
void fn_004d319b( long arg1, float arg2 )
{
	g_009254f8.fn_004bf74f( arg1, arg2 );
}

// HYPOTHESIS: method 0x004d3ef7 of the global at 0x0092e4a0 looks up an object from a
// handle; 0x004caf1b and 0x004caf40 are methods on that object (class not recovered yet).
class fn_004d3ef7_object;


// FUNCTION: STARFLEETOP 0x004d31b2
bool fn_004d31b2( long arg1, long arg2 )
{
	fn_004d3ef7_object* obj = g_0092e4a0.fn_004d3ef7( arg1 );
	bool ok = false;
	if ( obj != 0 )
	{
		if ( arg2 != 0 )
		{
			g_009254f8.fn_004caf40( obj, arg2 );
		}
		else
		{
			g_009254f8.fn_004caf1b( obj );
		}
		ok = true;
	}
	return ok;
}

// FUNCTION: STARFLEETOP 0x004d31ed
bool fn_004d31ed( long arg1, long arg2, long arg3, long arg4, long arg5 )
{
	fn_004d3ef7_object* obj = g_0092e4a0.fn_004d4a02( arg1, arg2, arg4, arg5 );
	bool ok = false;
	if ( obj != 0 )
	{
		if ( arg3 != 0 )
		{
			g_009254f8.fn_004caf40( obj, arg3 );
		}
		else
		{
			g_009254f8.fn_004caf1b( obj );
		}
		ok = true;
	}
	return ok;
}

// FUNCTION: STARFLEETOP 0x004d3232
fn_004d3ef7_object* fn_004d3232( long arg1, long arg2, long arg3, long arg4, long arg5 )
{
	fn_004d3ef7_object* obj = g_0092e4a0.fn_004d42e6( arg1, arg2, arg3, arg4 );
	if ( obj != 0 )
	{
		if ( arg5 != 0 )
		{
			g_009254f8.fn_004caf40( obj, arg5 );
		}
		else
		{
			g_009254f8.fn_004caf1b( obj );
		}
	}
	return obj;
}

// FUNCTION: STARFLEETOP 0x004d3278
fn_004d3ef7_object* fn_004d3278( long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long arg7 )
{
	fn_004d3ef7_object* obj = g_0092e4a0.fn_004d40b1( arg1, arg2, arg3, arg4, arg6, arg7 );
	if ( obj != 0 )
	{
		if ( arg5 != 0 )
		{
			g_009254f8.fn_004caf40( obj, arg5 );
		}
		else
		{
			g_009254f8.fn_004caf1b( obj );
		}
	}
	return obj;
}
