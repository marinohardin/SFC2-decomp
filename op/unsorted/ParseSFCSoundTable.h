#if !defined( ParseSFCSoundTable_h )
#define ParseSFCSoundTable_h

// HYPOTHESIS: size 0x3c, from its place in tSndMgr_Miles (+0x1f24 up to the next member at
// +0x1f60); the fields are not known yet. The vtable (0x007aaf98) has 21 slots; slots 3 to 9
// are inherited from ParseTabData (vtable 0x007aaf4c), which is not declared yet. Only the
// slots tSndMgr_Miles calls carry parameters.
class ParseSFCSoundTable
{
public:
	~ParseSFCSoundTable();

	// HYPOTHESIS: parses the named sound table
	virtual void vfunction1( const char *param_1 );
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	// HYPOTHESIS: slots 11 to 19 take a sound id; tSndMgr_Miles::vfunction29 (0x0050f2fc) reads
	// from them the file name, a looping flag, a priority, two ranges and a group with its limit.
	virtual const char *vfunction11( long param_1 );
	virtual void vfunction12();
	virtual char vfunction13( long param_1 );
	virtual void vfunction14();
	virtual long vfunction15( long param_1 );
	virtual char vfunction16( long param_1, unsigned long *param_2, unsigned long *param_3 );
	virtual char vfunction17( long param_1, unsigned long *param_2, unsigned long *param_3 );
	virtual long vfunction18( long param_1 );
	virtual unsigned long vfunction19( long param_1, char *param_2 );
	virtual void vfunction20();
	// HYPOTHESIS: takes the two zip directories of tSndMgr_Miles (sounds, voice)
	virtual void vfunction21( void *param_1, void *param_2 );

	char pad_0004[0x38];
};

#endif
