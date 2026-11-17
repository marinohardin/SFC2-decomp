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

// HYPOTHESIS: a binary search over num elements of width bytes from base; sets *found when
// compare(element, key) returns 0 and returns the first position not below key.
// FUNCTION: STARFLEETOP 0x006c18f0
void *fn_006c18f0(const void *key, const void *base, int num, unsigned long width, int (*compare)(const void *, const void *), int *found)
{
	int lo = 0;
	int hi = num - 1;
	int mid;
	int result;

	*found = 0;
	while (lo <= hi)
	{
		mid = (lo + hi) >> 1;
		result = compare((char *)base + mid * width, key);
		if (result == 0)
		{
			*found = 1;
			hi = mid - 1;
		}
		else if (result < 0)
			lo = mid + 1;
		else
			hi = mid - 1;
	}
	return (char *)base + lo * width;
}

#include <winsock.h>

void fn_006bf560();
// HYPOTHESIS: the original calls GetTickCount through its import thunk, not the import
// pointer, so it saw a prototype without dllimport.
extern "C" DWORD WINAPI GetTickCount(void);
// HYPOTHESIS: fills a sockaddr_in from a host name (or none, for any address) and a port.
int fn_006c2120(char *host, unsigned short port, struct sockaddr_in *saddr, struct hostent **hp);

extern SOCKET g_0088800c;
extern unsigned short g_00c7cbd4;
extern struct sockaddr_in g_00c7cdd8;
extern int g_00c7cdec;
extern int g_00c7cbcc;
extern int g_00c7cdf0;
extern int g_00c7cde8;
extern char g_00c7cbc0[8];

// HYPOTHESIS: opens the UDP socket, binds it to any port, records the bound port, resolves
// the master address on port 29910 and seeds the random generator.
// FUNCTION: STARFLEETOP 0x006c1960
int fn_006c1960(int param)
{
	struct sockaddr_in saddr;
	int saddrlen;

	fn_006bf560();
	g_0088800c = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (g_0088800c == INVALID_SOCKET)
		return -1;
	fn_006c2120(0, 0, &saddr, 0);
	if (bind(g_0088800c, (struct sockaddr *)&saddr, sizeof(saddr)) == SOCKET_ERROR)
		return -1;
	saddrlen = sizeof(saddr);
	if (getsockname(g_0088800c, (struct sockaddr *)&saddr, &saddrlen) == SOCKET_ERROR)
		return -1;
	g_00c7cbd4 = saddr.sin_port;
	fn_006c2120("key.gamespy.com", 29910, &g_00c7cdd8, 0);
	g_00c7cdec = 0;
	g_00c7cbcc = param;
	g_00c7cdf0 = 0;
	g_00c7cde8 = 0;
	// MATCH-ONLY: the original keeps the pending cdecl stack cleanup across this call, which
	// VC6 does only for a cdecl callee; the original's prototype was likely implicit (cdecl).
	srand(((DWORD (__cdecl *)(void))GetTickCount)());
	g_00c7cbc0[0] = 'g';
	g_00c7cbc0[1] = 'a';
	g_00c7cbc0[2] = 'm';
	g_00c7cbc0[3] = 'e';
	g_00c7cbc0[4] = 's';
	g_00c7cbc0[5] = 'p';
	g_00c7cbc0[6] = 'y';
	g_00c7cbc0[7] = '\0';
	return 0;
}

void fn_006c1a90();

// HYPOTHESIS: closes the UDP socket opened by fn_006c1960 and shuts down Winsock.
// FUNCTION: STARFLEETOP 0x006c1a70
void fn_006c1a70()
{
	fn_006c1a90();
	closesocket(g_0088800c);
	g_0088800c = INVALID_SOCKET;
	WSACleanup();
}

// HYPOTHESIS: an entry of the pending list; its first member owns a block with a buffer at
// +0x48.
struct fn_006c1a90_Block
{
	char m00[0x48];
	void *m48;
};
struct fn_006c1a90_Entry
{
	fn_006c1a90_Block *m00;
};

void fn_006c1f30(fn_006c1a90_Block *block);
void fn_006c21d0(fn_006c1a90_Entry *entry, int *list);

// HYPOTHESIS: empties the pending list, freeing every entry and the block it owns.
// FUNCTION: STARFLEETOP 0x006c1a90
void fn_006c1a90()
{
	fn_006c1a90_Entry *entry;

	while ((entry = (fn_006c1a90_Entry *)g_00c7cdec) != 0)
	{
		fn_006c1f30(entry->m00);
		fn_006c21d0(entry, &g_00c7cde8);
		if (entry->m00->m48 != 0)
			delete entry->m00->m48;
		delete entry->m00;
		delete entry;
	}
}

// HYPOTHESIS: a pending request; field meanings are guesses from the polling routine below.
struct fn_006c1ae0_Request
{
	unsigned long m00;
	char m04[0x24];
	int m28;
	unsigned long m2c;
	unsigned long m30;
	int m34;
	int m38;
	unsigned long m3c;
	void (*m40)(unsigned long, int, const char *, unsigned long);
	char *m44;
	void *m48;
};
struct fn_006c1ae0_Entry
{
	fn_006c1ae0_Request *m00;
	fn_006c1ae0_Entry *m04;
	fn_006c1ae0_Entry *m08;
};

extern char g_00c7c7c0[];
void fn_006c20e0(char *buffer, int length);
void fn_006c1c70(char *buffer, struct sockaddr_in *from);
void fn_006c1f90(fn_006c1ae0_Request *request);
void fn_006c21d0(fn_006c1ae0_Entry *entry, int *list);

// HYPOTHESIS: polls the UDP socket for replies and ages the pending requests.
// FUNCTION: STARFLEETOP 0x006c1ae0
void fn_006c1ae0()
{
	fd_set set;
	struct timeval timeout;
	struct sockaddr_in saddr;
	int saddrlen;
	int len;
	fn_006c1ae0_Entry *entry;
	fn_006c1ae0_Entry *next;
	fn_006c1ae0_Request *request;
	const char *text;

	saddrlen = sizeof(saddr);
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	set.fd_array[0] = g_0088800c;
	set.fd_count = 1;
	while (1)
	{
		len = select(0x40, &set, 0, 0, &timeout);
		if (len == -1)
			break;
		if (len == 0)
			break;
		len = recvfrom(g_0088800c, g_00c7c7c0, 0x3ff, 0, (struct sockaddr *)&saddr, &saddrlen);
		if (len != -1)
		{
			g_00c7c7c0[len] = 0;
			fn_006c20e0(g_00c7c7c0, len);
			fn_006c1c70(g_00c7c7c0, &saddr);
		}
	}
	for (entry = (fn_006c1ae0_Entry *)g_00c7cdec; entry != 0; entry = entry->m04)
	{
		request = entry->m00;
		switch (request->m38)
		{
		case 0:
			if (GetTickCount() < request->m30 + 2000)
				break;
			if (request->m34 <= 2)
			{
				fn_006c1f90(request);
				break;
			}
		case 1:
			text = "Validated";
			if (request->m38 != 1)
				text = "Validation Timeout";
			request->m40(request->m00, 1, text, request->m3c);
			entry->m00->m38 = 3;
			delete entry->m00->m48;
			entry->m00->m48 = 0;
			break;
		case 2:
			next = entry;
			entry = entry->m08;
			fn_006c21d0(next, &g_00c7cde8);
			request = next->m00;
			text = request->m44;
			if (text == 0)
				text = "";
			request->m40(request->m00, 0, text, request->m3c);
			delete next->m00->m48;
			if (next->m00->m44 != 0)
				delete next->m00->m44;
			delete next->m00;
			delete next;
			break;
		}
	}
}

void fn_006c1da0(char *buffer, int ok);
void fn_006c1ea0(char *buffer, struct sockaddr_in *from);

// HYPOTHESIS: dispatches a received reply on the key between its first two backslashes. The
// three key literals (0x00888040, 0x00888038, 0x00888030) are below the string dump's length
// limit and were not read back; their sizes fit the 4/8/8-byte slots.
// FUNCTION: STARFLEETOP 0x006c1c70
void fn_006c1c70(char *buffer, struct sockaddr_in *from)
{
	char key[32];
	char *end;
	int len;

	key[0] = 0;
	if (buffer[0] == '\\')
	{
		end = strchr(buffer + 1, '\\');
		len = end - buffer;
		if (len <= 32)
		{
			strncpy(key, buffer + 1, len - 1);
			key[len - 1] = 0;
		}
		if (key[0] != 0)
		{
			if (strcmp(key, "uok") == 0)
				fn_006c1da0(buffer, 1);
			else if (strcmp(key, "unok") == 0)
				fn_006c1da0(buffer, 0);
			else if (strcmp(key, "ison") == 0)
				fn_006c1ea0(buffer, from);
		}
	}
}

char *fn_006c1fd0(char *buffer, const char *key);
fn_006c1ae0_Request *fn_006c1e40(char *key);

// HYPOTHESIS: marks the pending request named in a reply as validated or refused.
// FUNCTION: STARFLEETOP 0x006c1da0
void fn_006c1da0(char *buffer, int ok)
{
	char key[33];
	int id;
	fn_006c1ae0_Request *request;

	id = atoi(fn_006c1fd0(buffer, "skey"));
	strncpy(key, fn_006c1fd0(buffer, "cd"), 32);
	key[32] = 0;
	request = fn_006c1e40(key);
	if (request != 0 && request->m28 == id && request->m38 != 3)
	{
		if (ok)
		{
			request->m38 = 1;
			return;
		}
		request->m38 = 2;
		request->m44 = strdup(fn_006c1fd0(buffer, "errmsg"));
	}
}

// HYPOTHESIS: finds the pending request whose key matches.
// FUNCTION: STARFLEETOP 0x006c1e40
fn_006c1ae0_Request *fn_006c1e40(char *key)
{
	fn_006c1ae0_Entry *entry;

	for (entry = (fn_006c1ae0_Entry *)g_00c7cdec; entry != 0; entry = entry->m04)
	{
		if (strcmp(key, entry->m00->m04) == 0)
			return entry->m00;
	}
	return 0;
}

// HYPOTHESIS: answers a status query with whether the named request has finished.
// FUNCTION: STARFLEETOP 0x006c1ea0
void fn_006c1ea0(char *buffer, struct sockaddr_in *from)
{
	char reply[64];
	int id;
	fn_006c1ae0_Request *request;
	int len;

	id = atoi(fn_006c1fd0(buffer, "skey"));
	request = fn_006c1e40(fn_006c1fd0(buffer, "cd"));
	if (request != 0 && request->m38 == 3)
		len = sprintf(reply, "\\uon\\\\skey\\%d", id);
	else
		len = sprintf(reply, "\\uoff\\\\skey\\%d", id);
	fn_006c20e0(reply, len);
	sendto(g_0088800c, reply, len, 0, (struct sockaddr *)from, sizeof(*from));
}

// HYPOTHESIS: tells the key server that the request's player has disconnected.
// FUNCTION: STARFLEETOP 0x006c1f30
void fn_006c1f30(fn_006c1ae0_Request *request)
{
	char buffer[512];
	int len;

	len = sprintf(buffer, "\\disc\\\\pid\\%d\\cd\\%s\\ip\\%d", g_00c7cbcc, request->m04, request->m2c);
	fn_006c20e0(buffer, len);
	sendto(g_0088800c, buffer, len, 0, (struct sockaddr *)&g_00c7cdd8, sizeof(g_00c7cdd8));
}
