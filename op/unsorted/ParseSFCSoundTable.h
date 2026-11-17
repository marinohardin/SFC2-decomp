#if !defined( ParseSFCSoundTable_h )
#define ParseSFCSoundTable_h

// HYPOTHESIS: size 0x3c, from its place in tSndMgr_Miles (+0x1f24 up to the next member at
// +0x1f60); the fields are not known yet.
class ParseSFCSoundTable
{
public:
	~ParseSFCSoundTable();

	char pad_0000[0x3c];
};

#endif
