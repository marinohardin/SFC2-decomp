// HYPOTHESIS: the class with vtable 0x007b0208 derives from tCalculationInitData (declared
// here only because no header holds it yet); the member at +0x8 is built by fn_0052238b.
class tCalculationInitData
{
public:
	int field_4;
	tCalculationInitData(int a);
	virtual ~tCalculationInitData();
};

class fn_0052238b_Member
{
public:
	int field_0[0xc];
	fn_0052238b_Member();
	~fn_0052238b_Member();
};

class tAIUnitData0208 : public tCalculationInitData
{
public:
	fn_0052238b_Member field_8;
	char pad_38[0xd54 - 0x8 - 0x30];
	struct tSlotA { int field_0; char pad[0xc8]; tSlotA() { field_0 = 0; } } field_d54[2];
	char pad_ee4[0x1144 - 0xd54 - 2 * 0xcc];
	struct tSlotB { int field_0; int field_4; char pad[0x31c]; tSlotB() { field_0 = 0; } } field_1144[2];

	tAIUnitData0208();
};

// FUNCTION: STARFLEETOP 0x00520b5e
tAIUnitData0208::tAIUnitData0208() : tCalculationInitData(2)
{
	field_1144[1].field_4 = 0x32;
}
