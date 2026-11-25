// Unit:      unsorted/fn_00522000.cpp
// Functions: 0x0052203a to 0x005224f3, 12 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   game-release (harness/units.json)

// HYPOTHESIS: a vector of 8-byte elements; field_8 is the end pointer, the function removes one element.
struct fn_0052203a_vector {
	int field_0;
	int* field_4;
	int* field_8;
	void fn_0052203a(int* pos);
};

// FUNCTION: STARFLEETOP 0x0052203a
void fn_0052203a_vector::fn_0052203a(int* pos)
{
	struct Elem { int a; int b; };
	Elem* dest = (Elem*)pos;
	Elem* last = (Elem*)field_8;
	for (Elem* first = dest + 1; first != last; ++first, ++dest)
		*dest = *first;
	pos = (int*)(last = (Elem*)field_8 - 1);
	field_8 = (int*)last;
}

// HYPOTHESIS: a doubly linked list container whose insert allocates a node, links it before pos and copies the value.
struct fn_00522077_node {
	fn_00522077_node* field_0;
	fn_00522077_node* field_4;
	int field_8;
};

struct fn_00522077_list {
	int field_0;
	int field_4;
	int field_8;
	fn_00522077_node* fn_005220b1(fn_00522077_node* next, fn_00522077_node* prev);
	static void fn_005223af(int* dest, int value);
	void fn_00522077(fn_00522077_node** result, fn_00522077_node* pos, int value);
};

// FUNCTION: STARFLEETOP 0x00522077
void fn_00522077_list::fn_00522077(fn_00522077_node** result, fn_00522077_node* pos, int value)
{
	fn_00522077_node* node = fn_005220b1(pos, pos->field_4);
	pos->field_4 = node;
	node->field_4->field_0 = node;
	fn_005223af(&node->field_8, value);
	field_8++;
	*result = node;
}

// FUNCTION: STARFLEETOP 0x005220b1
fn_00522077_node* fn_00522077_list::fn_005220b1(fn_00522077_node* next, fn_00522077_node* prev)
{
	fn_00522077_node* node = (fn_00522077_node*)operator new(0x40); // HYPOTHESIS: the node holds a 0x38-byte value after the two links
	node->field_0 = next ? next : node;
	node->field_4 = prev ? prev : node;
	return node;
}
