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

// HYPOTHESIS: the class's implicit assignment operator, out of line; written memberwise as the
// compiler generates it (the point arrays element by element through tPoint3's operator=).
// FUNCTION: STARFLEETOP 0x004b9d12
fn_0092f478_object &fn_0092f478_object::fn_004b9d12( fn_0092f478_object *other )
{
	field_00 = other->field_00;
	field_04 = other->field_04;
	field_08 = other->field_08;
	field_0c = other->field_0c;
	field_10 = other->field_10;
	field_14 = other->field_14;
	field_18 = other->field_18;
	field_1c = other->field_1c;
	field_20 = other->field_20;
	field_24 = other->field_24;
	field_28 = other->field_28;
	field_2c = other->field_2c;
	field_30 = other->field_30;
	int i;
	for ( i = 0; i < 4; i++ )
		field_40[i] = other->field_40[i];
	for ( i = 0; i < 4; i++ )
		field_70[i] = other->field_70[i];
	for ( i = 0; i < 4; i++ )
		field_a0[i] = other->field_a0[i];
	return *this;
}
