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
	virtual void vfunction11();
	virtual void vfunction12();
	virtual void vfunction13();
	virtual void vfunction14();
	virtual void vfunction15();
	virtual void vfunction16();
	virtual void vfunction17();
	virtual void vfunction18();
	virtual void vfunction19();
	virtual void vfunction20();
	// HYPOTHESIS: takes the two zip directories of tSndMgr_Miles (sounds, voice)
	virtual void vfunction21( void *param_1, void *param_2 );

	char pad_0004[0x38];
};

#endif
