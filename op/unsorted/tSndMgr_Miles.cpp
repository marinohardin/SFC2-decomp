#include "unsorted/tSndMgr_Base.h"
#include "unsorted/ParseSFCSoundTable.h"

// HYPOTHESIS: the member classes below are named after their destructors until evidence names
// them; sizes come from the member offsets in tSndMgr_Miles.
class fn_0050cb19_object
{
public:
	~fn_0050cb19_object();

	char pad_0000[0x254];
};

class fn_0050bcd4_object
{
public:
	~fn_0050bcd4_object();

	char pad_0000[0x18];
};

class fn_005d6a72_object
{
public:
	~fn_005d6a72_object();

	char pad_0000[0x18];
};

class fn_0050cb59_object
{
public:
	~fn_0050cb59_object();

	char pad_0000[0x3c8];
};

// HYPOTHESIS: slot layout from the analysis' vtable numbering, which counts from 1
// (vfunction29 is called through +0x70); the others keep their analysis names.
class tSndMgr_Miles : public tSndMgr_Base
{
public:
	virtual ~tSndMgr_Miles();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7( long param_1, long param_2 );
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual void vfunction12();
	virtual void vfunction13();
	virtual void vfunction14();
	virtual void vfunction15();
	virtual void vfunction16();
	virtual void vfunction17();
	virtual void vfunction18();
	virtual void vfunction19( long param_1 );
	virtual long vfunction20();
	virtual void vfunction21();
	virtual void vfunction22();
	virtual void vfunction23();
	virtual void vfunction24();
	virtual void vfunction25();
	virtual void vfunction26();
	virtual void vfunction27();
	virtual void vfunction28( long param_1 );
	// HYPOTHESIS: fourth parameter is a byte flag, fifth a volume-like 0..100 value
	virtual void vfunction29( long param_1, long param_2, char param_3, long param_4 );
	virtual void vfunction30();
	virtual char vfunction31();

	char pad_0004[0x1f1c];
	long field_1f20;
	ParseSFCSoundTable field_1f24;
	fn_0050cb19_object field_1f60;
	fn_0050cb19_object field_21b4;
	char pad_2408[0x18];
	fn_0050bcd4_object field_2420;
	fn_005d6a72_object field_2438;
	fn_0050cb59_object field_2450;
	// HYPOTHESIS: two tThread members (destructor 0x006ff170, size 0xd0c); tThread is declared
	// only inside Common/Thread/ThreadPool.cpp, so they stay byte arrays until a header holds it.
	char field_2818[0xd0c];
	char field_3524[0xd0c];
	char field_4230;
	char pad_4231[3];
	long field_4234;
	// HYPOTHESIS: two tCallback<long>-derived members (the destructor resets both to the
	// tCallback<long> vtable 0x007ab0c0), likely tMilesCallBack and tAILServeCallBack;
	// tCallback is declared only inside Common/Thread/ThreadPool.cpp.
	char field_4238[8];
	char field_4240[8];
};

// FUNCTION: STARFLEETOP 0x0050ed7a
void tSndMgr_Miles::vfunction7( long param_1, long param_2 )
{
	vfunction29( param_1, param_2, 0, 100 );
}

// FUNCTION: STARFLEETOP 0x0050ed8e
void tSndMgr_Miles::vfunction19( long param_1 )
{
	field_1f20 = param_1;
}

// FUNCTION: STARFLEETOP 0x0050ed9b
long tSndMgr_Miles::vfunction20()
{
	return field_1f20;
}

// FUNCTION: STARFLEETOP 0x0050eda2
void tSndMgr_Miles::vfunction28( long param_1 )
{
	if ( param_1 > 32 )
		field_4234 = 32;
	else if ( param_1 < 8 )
		field_4234 = 8;
	else
		field_4234 = param_1;
}

// FUNCTION: STARFLEETOP 0x0050edc5
char tSndMgr_Miles::vfunction31()
{
	return field_4230;
}

// FUNCTION: STARFLEETOP 0x0050ede8
tSndMgr_Miles::~tSndMgr_Miles()
{
	vfunction3();
}
