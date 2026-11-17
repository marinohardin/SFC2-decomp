// HYPOTHESIS: fn_006c1510_pool is declared in unsorted/fn_006C1000.cpp; only its name is needed here.
struct fn_006c1510_pool;

void fn_006c15e0(fn_006c1510_pool *pool, void *element);

// HYPOTHESIS: the owner keeps a pool of created items at +0x04 and passes its +0x94 to each new one.
struct fn_006cc070_owner
{
	unsigned long field_00;
	fn_006c1510_pool *field_04;
	unsigned char pad_08[0x94 - 0x08];
	unsigned long field_94;
};

void *fn_006cc530(unsigned long param_1, unsigned long param_2, unsigned long param_3, unsigned long param_4, unsigned long param_5);

// FUNCTION: STARFLEETOP 0x006cc070
void *fn_006cc070(fn_006cc070_owner *owner, unsigned long param_1, unsigned long param_2, unsigned long param_3, unsigned long param_4)
{
	void *item = fn_006cc530(param_1, param_2, param_3, param_4, owner->field_94);
	fn_006c15e0(owner->field_04, &item);
	return item;
}

// FUNCTION: STARFLEETOP 0x006cc0b0
int fn_006cc0b0(const char *text, int length, int count)
{
	const char *p = text;
	while (count != 0 && p != text + length)
	{
		if (*p == '\\')
			count--;
		p++;
	}
	if (count == 0)
		return p - text;
	return -1;
}
