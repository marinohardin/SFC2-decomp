// HYPOTHESIS: tEntity_DirectFire has no header yet (HEADER REQUEST filed); this reduced
// declaration only gives the factory below the size 0x268 and the constructor at 0x0041ca85.
class tEntity_DirectFire
{
public:
	tEntity_DirectFire();

	char	pad00[0x268];
};

// FUNCTION: STARFLEETOP 0x00421740
tEntity_DirectFire* fn_00421740()
{
	return new tEntity_DirectFire;
}
