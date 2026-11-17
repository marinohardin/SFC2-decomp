// HYPOTHESIS: little-endian encode of 32-bit words into bytes, the shape of a message
// digest's output step; the routine's name is not known.

// FUNCTION: STARFLEETOP 0x006c13a0
void fn_006c13a0(unsigned char *output, unsigned long *input, unsigned int len)
{
	unsigned int i, j;

	for (i = 0, j = 0; j < len; i++, j += 4) {
		output[j] = (unsigned char)(input[i] & 0xff);
		output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
		output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
		output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
	}
}

// HYPOTHESIS: the matching decode of bytes into little-endian 32-bit words.
// FUNCTION: STARFLEETOP 0x006c13f0
void fn_006c13f0(unsigned long *output, unsigned char *input, unsigned int len)
{
	unsigned int i, j;

	for (i = 0, j = 0; j < len; i++, j += 4)
		output[i] = ((unsigned long)input[j]) | (((unsigned long)input[j+1]) << 8) |
			(((unsigned long)input[j+2]) << 16) | (((unsigned long)input[j+3]) << 24);
}

#include <string.h>

// HYPOTHESIS: a byte copy for the digest's buffers, written through the memcpy intrinsic.
// FUNCTION: STARFLEETOP 0x006c1440
void fn_006c1440(unsigned char *output, unsigned char *input, unsigned int len)
{
	memcpy(output, input, len);
}

// HYPOTHESIS: a byte fill for the digest's buffers, written through the memset intrinsic.
// FUNCTION: STARFLEETOP 0x006c1460
void fn_006c1460(unsigned char *output, int value, unsigned int len)
{
	memset(output, value, len);
}

#include <stdio.h>

// HYPOTHESIS: prints a 16-byte digest as hex text, two characters per byte.
// FUNCTION: STARFLEETOP 0x006c1490
void fn_006c1490(unsigned char *digest, char *output)
{
	unsigned int i;

	for (i = 0; i < 16; i++, output += 2)
		sprintf(output, "%02x", digest[i]);
}

// HYPOTHESIS: an 88-byte digest context (four state words, a two-word bit count and a
// 64-byte block buffer), as the callees below use it.
struct fn_006c0900_ctx
{
	unsigned long state[4];
	unsigned long count[2];
	unsigned char buffer[64];
};

void fn_006c0900(fn_006c0900_ctx *context);
void fn_006c0930(fn_006c0900_ctx *context, unsigned char *input, unsigned int inputLen);
void fn_006c09e0(unsigned char *digest, fn_006c0900_ctx *context);

// HYPOTHESIS: digests a buffer and writes the digest as 32 hex characters.
// FUNCTION: STARFLEETOP 0x006c14c0
void fn_006c14c0(unsigned char *input, unsigned int inputLen, char *output)
{
	fn_006c0900_ctx context;
	unsigned char digest[16];

	fn_006c0900(&context);
	fn_006c0930(&context, input, inputLen);
	fn_006c09e0(digest, &context);
	fn_006c1490(digest, output);
}

#include <stdlib.h>

// HYPOTHESIS: a pool header of fixed-size elements; field meanings are guesses from the
// function below (element size, a count that defaults to 8, and the data block).
struct fn_006c1510_pool
{
	unsigned long field_00;
	unsigned long field_04;
	unsigned long field_08;
	unsigned long field_0c;
	void (*field_10)(void *);
	void *field_14;
};

// FUNCTION: STARFLEETOP 0x006c1510
fn_006c1510_pool *fn_006c1510(unsigned long size, unsigned long count, void (*param)(void *))
{
	fn_006c1510_pool *pool = (fn_006c1510_pool *)malloc(sizeof(fn_006c1510_pool));

	if (count == 0)
		count = 8;
	pool->field_08 = size;
	pool->field_00 = 0;
	pool->field_04 = count;
	pool->field_0c = count;
	pool->field_10 = param;
	pool->field_14 = malloc(size * count);
	return pool;
}

// HYPOTHESIS: releases each element (0x006c15a0), then the data block and the header.
void fn_006c15a0(fn_006c1510_pool *pool, int index);

// FUNCTION: STARFLEETOP 0x006c1560
void fn_006c1560(fn_006c1510_pool *pool)
{
	int i;

	for (i = 0; i < (int)pool->field_00; i++)
		fn_006c15a0(pool, i);
	free(pool->field_14);
	free(pool);
}

// HYPOTHESIS: returns the address of element index in the data block.
void *fn_006c15c0(fn_006c1510_pool *pool, int index);

// HYPOTHESIS: field_10 is a per-element release callback given at creation.
// FUNCTION: STARFLEETOP 0x006c15a0
void fn_006c15a0(fn_006c1510_pool *pool, int index)
{
	if (pool->field_10 != 0)
		pool->field_10(fn_006c15c0(pool, index));
}

// FUNCTION: STARFLEETOP 0x006c15c0
void *fn_006c15c0(fn_006c1510_pool *pool, int index)
{
	return (char *)pool->field_14 + pool->field_08 * index;
}

// HYPOTHESIS: inserts an element at the given index; 0x006c15e0 passes field_00, the end.
void fn_006c1600(fn_006c1510_pool *pool, void *element, int index);

// FUNCTION: STARFLEETOP 0x006c15e0
void fn_006c15e0(fn_006c1510_pool *pool, void *element)
{
	fn_006c1600(pool, element, pool->field_00);
}

// HYPOTHESIS: 0x006c1670 grows the data block when it is full; 0x006c16a0 stores the element.
void fn_006c1670(fn_006c1510_pool *pool);
void fn_006c16a0(fn_006c1510_pool *pool, void *element, int index);

// FUNCTION: STARFLEETOP 0x006c1600
void fn_006c1600(fn_006c1510_pool *pool, void *element, int index)
{
	if (pool->field_00 == pool->field_04)
		fn_006c1670(pool);
	pool->field_00++;
	if (index < (int)pool->field_00 - 1)
		memmove(fn_006c15c0(pool, index + 1), fn_006c15c0(pool, index), (pool->field_00 - index - 1) * pool->field_08);
	fn_006c16a0(pool, element, index);
}

// FUNCTION: STARFLEETOP 0x006c1670
void fn_006c1670(fn_006c1510_pool *pool)
{
	pool->field_04 += pool->field_0c;
	pool->field_14 = realloc(pool->field_14, pool->field_08 * pool->field_04);
}

// FUNCTION: STARFLEETOP 0x006c16a0
void fn_006c16a0(fn_006c1510_pool *pool, void *element, int index)
{
	memcpy(fn_006c15c0(pool, index), element, pool->field_08);
}

// HYPOTHESIS: removes the element at index, moving the later elements down one place.
// FUNCTION: STARFLEETOP 0x006c16e0
void fn_006c16e0(fn_006c1510_pool *pool, int index)
{
	if (index < (int)pool->field_00 - 1)
		memmove(fn_006c15c0(pool, index), fn_006c15c0(pool, index + 1), (pool->field_00 - index - 1) * pool->field_08);
	pool->field_00--;
}

// FUNCTION: STARFLEETOP 0x006c1730
void fn_006c1730(fn_006c1510_pool *pool, int index)
{
	fn_006c15a0(pool, index);
	fn_006c16e0(pool, index);
}

// HYPOTHESIS: replaces the element at index, releasing the old one first.
// FUNCTION: STARFLEETOP 0x006c1750
void fn_006c1750(fn_006c1510_pool *pool, void *element, int index)
{
	fn_006c15a0(pool, index);
	fn_006c16a0(pool, element, index);
}

// HYPOTHESIS: 0x006c18a0 is a binary search over base[0..num) with a compare callback
// (bsearch argument order); 0x006c18f0 the same, also reporting through found.
void *fn_006c18a0(const void *key, const void *base, int num, unsigned long width, int (*compare)(const void *, const void *));
void *fn_006c18f0(const void *key, const void *base, int num, unsigned long width, int (*compare)(const void *, const void *), int *found);

// HYPOTHESIS: searches the elements from start on for key; returns the index, or -1.
// FUNCTION: STARFLEETOP 0x006c1780
int fn_006c1780(fn_006c1510_pool *pool, const void *key, int (*compare)(const void *, const void *), int start, int flag)
{
	int found = 1;
	void *result;

	if (pool == 0 || pool->field_00 == 0)
		return -1;
	if (flag != 0)
		result = fn_006c18f0(key, fn_006c15c0(pool, start), pool->field_00 - start, pool->field_08, compare, &found);
	else
		result = fn_006c18a0(key, fn_006c15c0(pool, start), pool->field_00 - start, pool->field_08, compare);
	if (result == 0 || found == 0)
		return -1;
	return ((int)result - (int)pool->field_14) / (int)pool->field_08;
}

// HYPOTHESIS: calls func on every element with a caller-supplied argument.
// FUNCTION: STARFLEETOP 0x006c1820
void fn_006c1820(fn_006c1510_pool *pool, void (*func)(void *, void *), void *arg)
{
	int i;

	// HYPOTHESIS: the original compares the count signed (jl), so field_00 may be an int.
	for (i = 0; i < (int)pool->field_00; i++)
		func(fn_006c15c0(pool, i), arg);
}

// HYPOTHESIS: returns the first element for which func returns 0, or 0 when none does.
// FUNCTION: STARFLEETOP 0x006c1860
void *fn_006c1860(fn_006c1510_pool *pool, int (*func)(void *, void *), void *arg)
{
	int i;
	void *element;

	for (i = 0; i < (int)pool->field_00; i++)
	{
		element = fn_006c15c0(pool, i);
		if (func(element, arg) == 0)
			return element;
	}
	return 0;
}

// HYPOTHESIS: a linear search over count elements of size bytes from base; returns the first
// element for which func(arg, element) returns 0, or 0 when none does.
// FUNCTION: STARFLEETOP 0x006c18a0
void *fn_006c18a0(void *arg, char *base, int count, int size, int (*func)(void *, void *))
{
	int i;

	for (i = 0; i < count; i++)
	{
		if (func(arg, base + i * size) == 0)
			return base + i * size;
	}
	return 0;
}
