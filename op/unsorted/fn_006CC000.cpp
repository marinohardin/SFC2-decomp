// HYPOTHESIS: fn_006c1510_pool is declared in unsorted/fn_006C1000.cpp; only its name is needed here.
struct fn_006c1510_pool;

void fn_006c15e0(fn_006c1510_pool *pool, void *element);

#include <winsock.h>
#include <string.h>

// HYPOTHESIS: the original calls GetTickCount through its import thunk, not the import
// pointer, so it saw a prototype without dllimport.
extern "C" DWORD WINAPI GetTickCount(void);

void *fn_006c15c0(fn_006c1510_pool *pool, int index);

struct fn_006cc0f0_server;
struct fn_006bf5f0_table;

// HYPOTHESIS: one query slot per socket: the server it waits on, when it was sent and where to.
struct fn_006cc0f0_slot
{
	SOCKET field_00;
	fn_006cc0f0_server *field_04;
	unsigned long field_08;
	sockaddr_in field_0c;
};

// HYPOTHESIS: a server entry; field_06 holds its ping (9999 until measured), field_08 its query kind.
struct fn_006cc0f0_server
{
	unsigned char pad_00[0x06];
	short field_06;
	int field_08;
};

struct fn_006cc070_owner;
typedef void (*fn_006cc0f0_callback)(fn_006cc070_owner *owner, int what, unsigned long param, fn_006cc0f0_server *server, int percent);

// HYPOTHESIS: the owner keeps a pool of created items at +0x04 and passes its +0x94 to each new one;
// fn_006cc0f0 uses the pool as a list of servers queried through the slots at +0x08.
struct fn_006cc070_owner
{
	unsigned long field_00;
	fn_006c1510_pool *field_04;
	fn_006cc0f0_slot *field_08;
	unsigned char pad_0c[0x6c - 0x0c];
	int field_6c;
	int field_70;
	int field_74;
	fn_006cc0f0_callback field_78;
	unsigned long field_7c;
	unsigned char pad_80[0x94 - 0x80];
	fn_006bf5f0_table *field_94;
	int field_98;
};

void *fn_006cc530(char **keys, int count, char *text, unsigned long param_4, fn_006bf5f0_table *param_5);

// FUNCTION: STARFLEETOP 0x006cc070
void *fn_006cc070(fn_006cc070_owner *owner, char **param_1, int param_2, char *param_3, unsigned long param_4)
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

void fn_006cc7d0(fn_006cc0f0_server *server, char *text);
int fn_006cc900(fn_006c1510_pool *pool);
int fn_006cc420(fn_006cc070_owner *owner);
void fn_006cb8e0(fn_006cc070_owner *owner, int param_1);
char *fn_006cc8f0(fn_006cc0f0_server *server);
unsigned short fn_006cc910(fn_006cc0f0_server *server);

extern char *g_00888600[];
extern int g_007b9cc8[];

// FUNCTION: STARFLEETOP 0x006cc0f0
int fn_006cc0f0(fn_006cc070_owner *owner)
{
	fd_set set;
	timeval timeout;
	sockaddr_in from;
	int fromlen;
	char buffer[1500];
	int pending;
	int i;

	pending = 0;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	fromlen = sizeof(from);
	FD_ZERO(&set);
	for (i = 0; i < owner->field_6c; i++)
	{
		if (owner->field_08[i].field_04 != 0)
		{
			pending++;
			FD_SET(owner->field_08[i].field_00, &set);
		}
	}
	if (pending > 0)
	{
		int result = select(FD_SETSIZE, &set, 0, 0, &timeout);
		if (result != SOCKET_ERROR && result != 0)
		{
			for (i = 0; i < owner->field_6c; i++)
			{
				if (owner->field_08[i].field_04 != 0 && FD_ISSET(owner->field_08[i].field_00, &set))
				{
					fromlen = sizeof(from);
					int length = recvfrom(owner->field_08[i].field_00, buffer, sizeof(buffer) - 1, 0, (sockaddr *)&from, &fromlen);
					if (from.sin_addr.s_addr == owner->field_08[i].field_0c.sin_addr.s_addr && from.sin_port == owner->field_08[i].field_0c.sin_port)
					{
						if (length != SOCKET_ERROR)
						{
							buffer[length] = 0;
							BOOL final =strstr(buffer, "\\final\\") != 0;
							fn_006cc0f0_server *server = owner->field_08[i].field_04;
							if (server->field_06 == 9999)
								server->field_06 = (short)(GetTickCount() - owner->field_08[i].field_08);
							fn_006cc7d0(server, buffer);
							if (final)
							{
								owner->field_78(owner, 2, owner->field_7c, server, owner->field_70 * 100 / fn_006cc900(owner->field_04));
								owner->field_08[i].field_04 = 0;
							}
						}
						else
						{
							owner->field_08[i].field_04 = 0;
						}
					}
				}
			}
		}
	}
	for (i = 0; i < owner->field_6c; i++)
	{
		if (owner->field_08[i].field_04 != 0)
		{
			if (GetTickCount() - owner->field_08[i].field_08 > 3000)
				owner->field_08[i].field_04 = 0;
		}
	}
	if (owner->field_74 != 0 || owner->field_70 >= fn_006cc900(owner->field_04) && pending == 0)
	{
		fn_006cc420(owner);
		fn_006cb8e0(owner, 0);
		return 0;
	}
	for (i = 0; i < owner->field_6c; i++)
	{
		if (owner->field_70 >= fn_006cc900(owner->field_04))
			return 0;
		if (owner->field_08[i].field_04 == 0)
		{
			fn_006cc0f0_server *server = *(fn_006cc0f0_server **)fn_006c15c0(owner->field_04, owner->field_70);
			owner->field_70++;
			owner->field_08[i].field_04 = server;
			owner->field_08[i].field_0c.sin_family = AF_INET;
			owner->field_08[i].field_0c.sin_addr.s_addr = inet_addr(fn_006cc8f0(server));
			owner->field_08[i].field_0c.sin_port = htons(fn_006cc910(server));
			sendto(owner->field_08[i].field_00, g_00888600[server->field_08], g_007b9cc8[server->field_08], 0, (sockaddr *)&owner->field_08[i].field_0c, sizeof(sockaddr_in));
			owner->field_08[i].field_08 = GetTickCount();
		}
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x006cc420
int fn_006cc420(fn_006cc070_owner *owner)
{
	for (int i = 0; i < owner->field_6c; i++)
		closesocket(owner->field_08[i].field_00);
	return 0;
}

// FUNCTION: STARFLEETOP 0x006cc460
int fn_006cc460(fn_006cc070_owner *owner)
{
	if (owner->field_00 != 0)
		owner->field_74 = 1;
	return 0;
}

void fn_006c1560(fn_006c1510_pool *pool);
fn_006c1510_pool *fn_006c1510(unsigned long size, unsigned long count, void (*param)(void *));
void fn_006cc510(void *element);

// HYPOTHESIS: 0x006bf5f0 builds a hashed table (element size, bucket count, a third size and
// three callbacks: hash, compare, free); 0x006bf660 frees it. Kept opaque here.
void fn_006bf660(fn_006bf5f0_table *table);
fn_006bf5f0_table *fn_006bf5f0(int size, int buckets, int count, int (*hash)(const void *, int), int (*compare)(const void *, const void *), void (*release)(void *));
int fn_006cca60(const void *element, int buckets);
int fn_006ccae0(const void *a, const void *b);
void fn_006ccb10(void *element);

// HYPOTHESIS: resets the owner's lists while it is idle (field_00 == 0); returns 4 when busy.
// FUNCTION: STARFLEETOP 0x006cc480
int fn_006cc480(fn_006cc070_owner *owner)
{
	if (owner->field_00 != 0)
		return 4;
	fn_006c1560(owner->field_04);
	owner->field_04 = fn_006c1510(4, 0x40, fn_006cc510);
	fn_006bf660(owner->field_94);
	owner->field_94 = fn_006bf5f0(4, 500, 4, fn_006cca60, fn_006ccae0, fn_006ccb10);
	owner->field_98 = 0;
	return 0;
}

// FUNCTION: STARFLEETOP 0x006cc4f0
int fn_006cc4f0(fn_006cc070_owner *owner)
{
	return fn_006cc900(owner->field_04);
}

// HYPOTHESIS: a key/value record; its table of pairs sits at +0x0c.
struct fn_006cc530_record
{
	unsigned char pad_00[0x0c];
	fn_006bf5f0_table *field_0c;
	fn_006bf5f0_table *field_10;
};

fn_006cc530_record *fn_006cc730(unsigned long param_1, unsigned long param_2, unsigned long param_3, fn_006bf5f0_table *param_4);
char *fn_006cc640(char *text, char separator);
void fn_006cc6f0(fn_006cc530_record *record, char *text);
char *fn_006cc6a0(fn_006cc530_record *record, char *text);
void fn_006bf6a0(fn_006bf5f0_table *table, void *element);

extern char g_008886f8[];

// FUNCTION: STARFLEETOP 0x006cc530
void *fn_006cc530(char **keys, int count, char *text, unsigned long param_4, fn_006bf5f0_table *param_5)
{
	int i = 0;
	char *pair[2];
	fn_006cc530_record *record = fn_006cc730(0, 0, param_4, param_5);
	char *value = fn_006cc640(text + 1, '\\');
	while (i < count)
	{
		char *key = keys[i];
		if (value == 0)
			value = "";
		if (strcmp(key, g_008886f8) == 0)
			fn_006cc6f0(record, value);
		else
		{
			pair[0] = fn_006cc6a0(record, key);
			pair[1] = fn_006cc6a0(record, value);
			fn_006bf6a0(record->field_0c, pair);
		}
		i++;
		if (i < count)
			value = fn_006cc640(0, '\\');
	}
	return record;
}

// HYPOTHESIS: the cursor of a strtok-like splitter, kept between calls.
char *g_00c7ea60;

// FUNCTION: STARFLEETOP 0x006cc640
char *fn_006cc640(char *text, char separator)
{
	if (text != 0)
		g_00c7ea60 = text;
	char *start = g_00c7ea60;
	if (*g_00c7ea60 != '\0')
	{
		while (*g_00c7ea60 != '\0' && *g_00c7ea60 != separator)
			g_00c7ea60++;
		if (g_00c7ea60 == start)
			start = 0;
	}
	else
		start = 0;
	if (*g_00c7ea60 != '\0')
	{
		*g_00c7ea60 = '\0';
		g_00c7ea60++;
	}
	return start;
}

void *fn_006bf750(fn_006bf5f0_table *table, void *element);

// HYPOTHESIS: interns a string in the record's string table at +0x10; 0x0073a6a6 is taken
// to be _strdup.
// FUNCTION: STARFLEETOP 0x006cc6a0
char *fn_006cc6a0(fn_006cc530_record *record, char *text)
{
	char **found = (char **)fn_006bf750(record->field_10, &text);
	if (found != 0)
		return *found;
	text = _strdup(text);
	fn_006bf6a0(record->field_10, &text);
	return text;
}
