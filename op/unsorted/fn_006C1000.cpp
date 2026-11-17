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
