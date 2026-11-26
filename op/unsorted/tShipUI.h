//	tShipUI.h: tShipUI (RTTI, vtable 0x007a54c4, 0x1104 bytes), the ship display the game states embed.
// Evidence:  class (RTTI; original file unknown)

#ifndef tShipUI_h
#define tShipUI_h 1

// HYPOTHESIS: a 0x20-byte entry of the list at tShipUI+0xc (tCampaignState indexes it by +0x100c).
struct tShipUI_listEntry
{
	char m_pad[0x20];
};

// The constructor (0x004cd034) stores the vtable, clears the words at +0x100c, +0x1010 and the
// pointers at +0x1018, +0x101c and +0x1020, clears the flag at +0x1100, and registers the names "ShipIndex", "VariantText", "VesselStats",
// "VesselName", "cVesselProfile", "cTurnClass", "cMoveCost" and the shield names with the
// helper at 0x004d35ac; the destructor (0x004cd115) unregisters them (0x004d35bf) and deletes the
// three pointers. The vtable has one slot, the destructor.
// Embedded by tCampaignState (+0xc), tState_Skirmish (+0x14) and tState_Multiplayer (+0xe64); every
// one's next member starts 0x1104 bytes on. The vessel library state's destructor (0x0048a5ea)
// destroys one at +0x30 of its own.
class tShipUI
{
public:
	tShipUI();
	virtual ~tShipUI();
	// HYPOTHESIS: three arguments; the split of the pushes between this call and the two
	// virtual calls inside its argument list is read from the register use at the call
	// sites, not from the callee. Types unknown. The callee itself reads four stack words
	// (+0x8 to +0x14 above ebp) and ends with ret 0x10, so the declaration (and the symbol
	// the callers' evidence gives, three ints) disagrees with it; it stays at three so the
	// callers, which compile exact, keep their bytes.
	void fn_004cd1db( int a, int b, int id );
	void fn_004cd2c9();
	void fn_004cd2f6();
	void fn_004cd739();

	// HYPOTHESIS: 0x004cd1db stores the words at ebp+8 and ebp+0xc here.
	int					m_04;
	int					m_08;
	tShipUI_listEntry	m_0c[0x80];
	// HYPOTHESIS: an index into the list at +0xc.
	int					m_100c;
	// 0x004cd1db sets +0x1010 from a call (0x0040a6cd) and +0x1014 from the stack word at ebp+0x14
	// (the fourth argument by the callee's ret 0x10).
	int					m_1010;
	int					m_1014;
	// HYPOTHESIS: owned objects: the destructor deletes each and clears it.
	void *				m_1018;
	void *				m_101c;
	void *				m_1020;
	// 0x004cd1db stores its third argument here.
	int					m_1024;
	char				m_pad_1028[0x1100 - 0x1028];
	unsigned char		m_1100;
	char				m_pad_1101[0x3];
};

typedef char tShipUI_size_check[ sizeof( tShipUI ) == 0x1104 ? 1 : -1 ];

#endif
