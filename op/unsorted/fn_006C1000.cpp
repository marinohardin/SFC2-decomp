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
