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

// HYPOTHESIS: layouts guessed from the accesses
class fn_005218f9_pair {
public:
	int m_index;
	int m_value;
};

class fn_005218f9_node {
public:
	int pad0;
	int pad4;
	int pad8;
	int padc;
	fn_005218f9_pair* m_begin;
	fn_005218f9_pair* m_end;
	int pad18;
	int pad1c;
	int pad20;
	fn_005218f9_node* m_next;
};

class fn_005218f9_owner {
public:
	void fn_005218f9(tAIEntryTable* t, fn_005218f9_node* n);
	char pad[0x14c0];
	int m_counts[50];
};

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

// FUNCTION: STARFLEETOP 0x005218f9
void fn_005218f9_owner::fn_005218f9(tAIEntryTable* t, fn_005218f9_node* n)
{
	int i = 0;
	if (0 < t->m_count) {
		int* p = m_counts;
		do {
			*p = 0;
			i++;
			p++;
		} while (i < t->m_count);
	}
	do {
		for (fn_005218f9_pair* q = n->m_begin; q != n->m_end; q++) {
			if (t->m_entries[q->m_index].m_3c == -1) {
				t->m_entries[q->m_index].m_3c = q->m_value;
				m_counts[q->m_value]++;
			}
		}
		n = n->m_next;
	} while (n != 0);
}

// HYPOTHESIS: the argument's class is unknown; only the two floats at +0x14 and +0x18 are known
// FUNCTION: STARFLEETOP 0x00521965
bool __stdcall fn_00521965(const float* p)
{
	return p[5] <= p[6];
}

// HYPOTHESIS: the classes of the owner and of the second argument are unknown; only the offsets used are known
struct fn_0052197e_node {
	char pad0[0x14];
	float field_14;
	float field_18;
	char pad1c[0x10];
	float field_2c;
	char pad30[0xc];
	int field_3c;
};

class fn_0052197e_owner {
public:
	float fn_0052197e(float* amount, int mode, fn_0052197e_node* n);
	char pad[0x14c0];
	int m_counts[50];
};

float __stdcall fn_00521a15(float amount, fn_0052197e_node* n);

// FUNCTION: STARFLEETOP 0x0052197e
float fn_0052197e_owner::fn_0052197e(float* amount, int mode, fn_0052197e_node* n)
{
	float avail = 0.0f;
	if (m_counts[n->field_3c] >= 2) {
		if (*amount > n->field_2c) {
			*amount -= n->field_2c;
			n->field_2c = 0.0f;
		} else {
			n->field_2c -= *amount;
			*amount = 0.0f;
			return avail;
		}
	}
	if (mode == 2) {
		avail = n->field_14 - n->field_18;
	} else if (mode == 3) {
		avail = n->field_14;
	}
	float used;
	if (avail > *amount) {
		used = *amount;
	} else {
		used = avail;
	}
	float r = fn_00521a15(used, n);
	n->field_14 -= used;
	*amount -= used;
	return r;
}
