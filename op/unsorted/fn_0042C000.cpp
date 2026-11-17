// HYPOTHESIS: a red-black tree of the library's map/set: 0x2c-byte nodes with the value at
// +0x0c, a colour byte at +0x28 and an is-nil byte at +0x29; the head node sits at +0x08.
struct fn_0042c053_Node
{
	fn_0042c053_Node* m00_left;
	fn_0042c053_Node* m04_parent;
	fn_0042c053_Node* m08_right;
	char m0c_value[0x1c];
	char m28_color;
	char m29_isnil;
};

// HYPOTHESIS: constructs a node value in place from another (copy construction).
void fn_0042c948(void* dest, const void* src);

class fn_0042c053_Tree
{
public:
	fn_0042c053_Node* fn_0042c053(fn_0042c053_Node* x, fn_0042c053_Node* parent);

	char m00_pad[0x8];
	fn_0042c053_Node* m08_head;
};

// FUNCTION: STARFLEETOP 0x0042c053
fn_0042c053_Node* fn_0042c053_Tree::fn_0042c053(fn_0042c053_Node* x, fn_0042c053_Node* parent)
{
	fn_0042c053_Node* root = m08_head;
	if (!x->m29_isnil)
	{
		char color = x->m28_color;
		fn_0042c053_Node* y = (fn_0042c053_Node*)operator new(sizeof(fn_0042c053_Node));
		y->m04_parent = parent;
		y->m29_isnil = 0;
		y->m28_color = color;
		fn_0042c948(&y->m0c_value, &x->m0c_value);
		y->m00_left = m08_head;
		y->m08_right = m08_head;
		if (root == m08_head)
			root = y;
		y->m00_left = fn_0042c053(x->m00_left, y);
		y->m08_right = fn_0042c053(x->m08_right, y);
	}
	return root;
}
