// HYPOTHESIS: a getter of a class not yet identified; the symbol table records it as a
// fastcall free function taking the object, which callers in other units also use.
// FUNCTION: STARFLEETOP 0x004b94dc
int __fastcall fn_004b94dc(void *self)
{
	return *(int *)((char *)self + 0x88);
}
