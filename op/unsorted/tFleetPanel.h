//	tFleetPanel.h: tFleetPanel, the panel that lists the fleet's entries.
// Evidence:  the analysis's class names for the panel's functions (original file unknown)

#ifndef tFleetPanel_h
#define tFleetPanel_h 1

#include <vector>

#include "unsorted/tArea3Panel.h"

class tObject_0042496f;
class tObject_0048bd57_arg;
class tObject_0048c1b5_wnd;
class tObject_0048ba83;

class tFleetPanel : public tArea3Panel
{
public:
	tFleetPanel(tObject_0048bd57_arg *pCharacter);
	virtual ~tFleetPanel();
	tFleetPanel *scalar_deleting_destructor(unsigned char flags);
	// HYPOTHESIS: the analysis names 0x0048c1ab tFleetPanel::vfunction2; 0x0048c1b5 is its body.
	void vfunction2();
	// HYPOTHESIS: a member of the panel, since it walks the button array at this+0x1c.
	void fn_0048c34e();
	// HYPOTHESIS: a member of the panel; it walks the same button array as 0x0048c34e.
	void fn_0048c36d(int n);
	// HYPOTHESIS: the analysis names 0x0048c3a6 tFleetPanel::vfunction3.
	void vfunction3(int *param_1);
	// HYPOTHESIS: the analysis names 0x0048c3e7 tFleetPanel::vfunction6; it forwards a float
	// and two further arguments to each fleet button.
	void vfunction6(float f, int param_2, int param_3);
	// HYPOTHESIS: the analysis names 0x0048c418 tFleetPanel::vfunction4.
	void vfunction4();
	// HYPOTHESIS: refreshes the list when neither the object nor its count changed.
	void fn_0048c632(std::vector<int> *pList);
	// HYPOTHESIS: rebuilds the list; 0x0048c418 passes the same list twice.
	void fn_0048c52c(std::vector<int> *pList1, std::vector<int> *pList2);

	int m_n08;
	tObject_0042496f *m_pFleet;
	tObject_0048bd57_arg *m_pCharacter;
	int m_n14;
	int m_n18;
	tObject_0048c1b5_wnd *m_apButton[6];
	tObject_0048c1b5_wnd *m_apLabel[6];
	tObject_0048c1b5_wnd *m_p4c;
	int m_n50;
	int m_n54;
	char m_pad58[0x80];
	tObject_0048ba83 *m_pD8;
	char m_padDC[0xc];
	double m_dE8;
};

#endif // tFleetPanel_h
