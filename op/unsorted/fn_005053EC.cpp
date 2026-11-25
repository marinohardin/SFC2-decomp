// HYPOTHESIS: tFx_Pulsar has no shared header yet; declared here pending a HEADER REQUEST.
class tFx_Pulsar
{
public:
	tFx_Pulsar();

	char	pad00[0x30];
};

// FUNCTION: STARFLEETOP 0x005053ec
void fn_005053ec()
{
	tFx_Pulsar* p = new tFx_Pulsar();
}
