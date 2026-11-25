#include "Tiger/tstring.h"
#include "Tiger/tgStrMgr.h"
#include <vector>

// HYPOTHESIS: the owner is unknown; a vector of strings sits at +0x48 and this clears it (erase over begin and end).
class fn_00424d28_class
{
public:
	char fPad[0x48];
	std::vector<tString> fStrings;
	void fn_00424d28();
	void fn_00424d37(const tString& s);
	tString* fn_00424d46(const char* s);
	int fn_00424e2a(const tString& s);
};

// HYPOTHESIS: the global string manager at 0x008f5118.
extern tStrMgrCore g_008f5118;

// FUNCTION: STARFLEETOP 0x00424d28
void fn_00424d28_class::fn_00424d28()
{
	fStrings.clear();
}

// FUNCTION: STARFLEETOP 0x00424d37
void fn_00424d28_class::fn_00424d37(const tString& s)
{
	fStrings.push_back(s);
}

// FUNCTION: STARFLEETOP 0x00424d46
tString* fn_00424d28_class::fn_00424d46(const char* s)
{
	tString str(g_008f5118.mGetRandStr(s));
	int index = fn_00424e2a(str);
	int slot = -1;
	if (index != -1) {
		int first = index;
		long count = g_008f5118.mGetNumElements(s);
		for (long i = 0; i < count; i++) {
			if (index == -1)
				break;
			str = g_008f5118.mGetGrpStr(s, i);
			index = fn_00424e2a(str);
		}
		if (index != -1)
			slot = first;
	}
	if (slot == -1) {
		slot = fStrings.size();
		fStrings.push_back(str);
	}
	return &fStrings[slot];
}

// MATCH-ONLY: the original inlined a size helper that tests for an empty buffer.
inline unsigned int fn_00424e2a_Size(const std::vector<tString>& v)
{
	if (v.begin() == 0)
		return 0;
	return v.end() - v.begin();
}

// FUNCTION: STARFLEETOP 0x00424e2a
int fn_00424d28_class::fn_00424e2a(const tString& s)
{
	for (unsigned int i = 0; i < fn_00424e2a_Size(fStrings); i++) {
		if (fStrings.begin()[i] == s)
			return i;
	}
	return (unsigned int)-1;
}

// HYPOTHESIS: a map from string to int in the 0x14-byte tree layout (head at +4, nil node of
// its own at +8, multi flag at +0xc, size at +0x10); this is the body that frees it.
struct fn_00424e71_map
{
	struct node
	{
		node* left;
		node* parent;
		node* right;
	};
	class iterator
	{
	public:
		iterator(node* n) : fNode(n) {}
		node* fNode;
	};
	iterator fn_00424f27(iterator first, iterator last);
	void fn_00424e71();

	char fPad[4];
	node* fHead;
	node* fNil;
	bool fMulti;
	unsigned int fSize;
};

// FUNCTION: STARFLEETOP 0x00424e71
void fn_00424e71_map::fn_00424e71()
{
	fn_00424f27(iterator(fHead->left), iterator(fHead));
	operator delete(fHead);
	fHead = 0;
	fSize = 0;
	operator delete(fNil);
	fNil = 0;
}
