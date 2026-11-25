// HYPOTHESIS: the insertion sort of the standard library's sort, instantiated for unsigned int.
// FUNCTION: STARFLEETOP 0x004260a4
void fn_004260a4(unsigned int* first, unsigned int* last)
{
	if (first == last)
		return;
	for (unsigned int* i = first + 1; i != last; ++i)
	{
		unsigned int val = *i;
		if (!(val < *first))
		{
			unsigned int* p = i;
			unsigned int* q = p - 1;
			while (val < *q)
			{
				*p = *q;
				p = q;
				--q;
			}
			*p = val;
		}
		else
		{
			unsigned int* p = i;
			if (first != p)
			{
				do
				{
					*p = *(p - 1);
					--p;
				} while (first != p);
			}
			*first = val;
		}
	}
}
