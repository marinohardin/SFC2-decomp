// HYPOTHESIS: slot layout from the analysis' vtable numbering, which counts from 1
// (vfunction29 is called through +0x70); the others keep their analysis names.
class tSndMgr_Miles
{
public:
	virtual void vfunction1();
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
	virtual void vfunction20();
	virtual void vfunction21();
	virtual void vfunction22();
	virtual void vfunction23();
	virtual void vfunction24();
	virtual void vfunction25();
	virtual void vfunction26();
	virtual void vfunction27();
	virtual void vfunction28();
	// HYPOTHESIS: fourth parameter is a byte flag, fifth a volume-like 0..100 value
	virtual void vfunction29( long param_1, long param_2, char param_3, long param_4 );

	char pad_0004[0x1f1c];
	long field_1f20;
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
