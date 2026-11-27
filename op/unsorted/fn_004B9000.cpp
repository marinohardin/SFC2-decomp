#include "unsorted/tState_Tactical.h"

// HYPOTHESIS: a getter of a class not yet identified; the symbol table records it as a
// fastcall free function taking the object, which callers in other units also use.
// FUNCTION: STARFLEETOP 0x004b94dc
int __fastcall fn_004b94dc(void *self)
{
	return *(int *)((char *)self + 0x88);
}

// Writes the intersection of this rectangle and the one given into out.
// FUNCTION: STARFLEETOP 0x004b99a4
void fn_004b7f9e_Rect::fn_004b99a4(fn_004b7f9e_Rect *out, const fn_004b7f9e_Rect *other) const
{
	if (left < other->left)
		out->left = other->left;
	else
		out->left = left;
	if (top < other->top)
		out->top = other->top;
	else
		out->top = top;
	if (right > other->right)
		out->right = other->right;
	else
		out->right = right;
	if (bottom > other->bottom)
		out->bottom = other->bottom;
	else
		out->bottom = bottom;
}
