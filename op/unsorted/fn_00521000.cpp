// HYPOTHESIS: layouts guessed from the accesses; no header holds these classes yet
class tAIEntry {
public:
	int m_id;
	int pad4;
	int pad8;
	int padc;
	int m_10;
	int m_14;
	int pad18;
	int pad1c;
	int pad20;
	float m_24;
	int m_28;
	int pad2c;
	int pad30;
	int pad34;
	int m_38;
	int m_3c;
};

extern float g_0079561c;
// HYPOTHESIS: both functions are members of one unknown class (the original spills ecx and reloads it for the call)
class tAIEntryTable;
class tAIIdList;

class tAIEntryOwner {
public:
	int fn_00521450(tAIEntryTable* t, tAIIdList* a, tAIIdList* b);
	char fn_00521965(tAIEntry* e);
};

class tAIIdList {
public:
	int m_ids[50];
	int m_count;
};

class tAIEntryTable {
public:
	tAIEntry m_entries[50];
	int m_count;
};

// FUNCTION: STARFLEETOP 0x00521450
int tAIEntryOwner::fn_00521450(tAIEntryTable* t, tAIIdList* a, tAIIdList* b)
{
	tAIEntryTable* tb = t;
	int i = 0;
	tAIEntry* p = tb->m_entries;
	for (; i < tb->m_count; i++, p++) {
		if (p->m_id != -1) {
			p->m_10 = p->m_14;
			if (this->fn_00521965(p)) {
				p->m_28 = 2;
			}
		}
	}
	int counts[2];
	tAIEntry* e;
	counts[0] = 0;
	counts[1] = 0;
	e = tb->m_entries;
	int j;
	for (j = 0; j < tb->m_count; j++, e++) {
		if (e->m_id != -1) {
			if (e->m_28 == 0) {
				counts[e->m_38]++;
			} else {
				if (e->m_24 == g_0079561c) {
					continue;
				}
				if (e->m_28 == 0) {
					counts[e->m_38]++;
				}
			}
			if (tb->m_entries[e->m_3c].m_28 != 0) {
				tAIIdList* l = a;
				if (e->m_38 != 0) {
					l = b;
				}
				if (l->m_count < 50) {
					l->m_ids[l->m_count] = e->m_id;
					l->m_count++;
				}
			}
		}
	}
	if (counts[0] != 0 && counts[1] != 0) {
		return 0;
	}
	return 1;
}

// FUNCTION: STARFLEETOP 0x00521530
unsigned int __stdcall fn_00521530(unsigned int v)
{
	if (v == 0) {
		return 1;
	}
	return (v == 1) ? 0 : v;
}

#include "unsorted/fn_005717f7_vector.h"
// HYPOTHESIS: a class of unknown name; only the two vectors destroyed here are known
class fn_005218c3_owner {
public:
	~fn_005218c3_owner();
	int field_0;
	int field_4;
	int field_8;
	fn_005717f7_vector field_c;
	int field_1c;
	int field_20;
	int field_24;
	fn_005717f7_vector field_28;
};

// FUNCTION: STARFLEETOP 0x005218c3
fn_005218c3_owner::~fn_005218c3_owner()
{
	// The body is empty in the original: the two vectors (field_28, then field_c) are destroyed by
	// the compiler-generated epilogue, under the EH frame; the bytes (54) are exact without more code.
}
