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
int fn_006cc910(fn_006cc0f0_server *server);

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
	unsigned long field_00;
	unsigned short field_04;
	short field_06;
	int field_08;
	fn_006bf5f0_table *field_0c;
	fn_006bf5f0_table *field_10;
};

fn_006cc530_record *fn_006cc730(unsigned long param_1, unsigned short param_2, int param_3, fn_006bf5f0_table *param_4);
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

#include <stdlib.h>

// HYPOTHESIS: an address and port parsed from "host:port"; the address is kept in network
// order as inet_addr returns it, the port as the number written.
struct fn_006cc6f0_endpoint
{
	unsigned long field_00;
	unsigned short field_04;
};

// FUNCTION: STARFLEETOP 0x006cc6f0
void fn_006cc6f0(fn_006cc6f0_endpoint *endpoint, char *text)
{
	char *colon = strchr(text, ':');
	if (colon != 0)
	{
		*colon = '\0';
		endpoint->field_00 = inet_addr(text);
		endpoint->field_04 = atoi(colon + 1);
	}
}

int fn_006cc860(char *key);

// HYPOTHESIS: fn_006cc0f0_server and fn_006cc530_record are one server entry; stores the
// "\key\value" pairs of a reply whose key fn_006cc860 accepts.
// FUNCTION: STARFLEETOP 0x006cc7d0
void fn_006cc7d0(fn_006cc0f0_server *server, char *text)
{
	fn_006cc530_record *record = (fn_006cc530_record *)server;
	char *pair[2];
	char *key = fn_006cc640(text + 1, '\\');
	while (key != 0)
	{
		char *value = fn_006cc640(0, '\\');
		if (value == 0)
			value = "";
		if (fn_006cc860(key) != 0)
		{
			pair[0] = fn_006cc6a0(record, key);
			pair[1] = fn_006cc6a0(record, value);
			fn_006bf6a0(record->field_0c, pair);
		}
		key = fn_006cc640(0, '\\');
	}
}

// HYPOTHESIS: filters out the reply keys that are not stored with a server entry.
// FUNCTION: STARFLEETOP 0x006cc860
int fn_006cc860(char *key)
{
	char *skipped[2] = { "queryid", "final" };
	for (unsigned int i = 0; i < sizeof(skipped) / sizeof(skipped[0]); i++)
	{
		if (strcmp(key, skipped[i]) == 0)
			return 0;
	}
	return 1;
}

int fn_006ccac0(const void *element, int buckets);
int fn_006ccb00(const void *a, const void *b);

// HYPOTHESIS: creates a server entry: address, port, ping (9999 until measured), query kind,
// its key/value table and the shared string table; the callee 0x0072eed0 is malloc.
// FUNCTION: STARFLEETOP 0x006cc730
fn_006cc530_record *fn_006cc730(unsigned long param_1, unsigned short param_2, int param_3, fn_006bf5f0_table *param_4)
{
	fn_006cc530_record *record = (fn_006cc530_record *)malloc(sizeof(fn_006cc530_record));
	record->field_00 = param_1;
	record->field_04 = param_2;
	record->field_06 = 9999;
	record->field_08 = param_3;
	int buckets;
	int count;
	switch (param_3)
	{
	case 0:
		buckets = 4;
		count = 2;
		break;
	case 1:
		buckets = 6;
		count = 2;
		break;
	case 2:
	case 3:
		buckets = 8;
		count = 2;
		break;
	default:
		buckets = 8;
		count = 4;
		break;
	}
	record->field_0c = fn_006bf5f0(8, buckets, count, fn_006ccac0, fn_006ccb00, 0);
	record->field_10 = param_4;
	return record;
}

// HYPOTHESIS: returns a server entry's ping.
// FUNCTION: STARFLEETOP 0x006cc8e0
int fn_006cc8e0(fn_006cc530_record *record)
{
	return record->field_06;
}

// HYPOTHESIS: returns a server entry's address as dotted text; field_00 holds it in network
// order, as inet_addr returns it.
// FUNCTION: STARFLEETOP 0x006cc8f0
char *fn_006cc8f0(fn_006cc0f0_server *server)
{
	fn_006cc530_record *record = (fn_006cc530_record *)server;
	return inet_ntoa(*(in_addr *)&record->field_00);
}

// HYPOTHESIS: returns a server entry's address in network order.
// FUNCTION: STARFLEETOP 0x006cc900
unsigned long fn_006cc900(fn_006cc530_record *record)
{
	return record->field_00;
}

// HYPOTHESIS: returns a server entry's port.
// FUNCTION: STARFLEETOP 0x006cc910
int fn_006cc910(fn_006cc0f0_server *server)
{
	fn_006cc530_record *record = (fn_006cc530_record *)server;
	return record->field_04;
}

char **fn_006cc9a0(fn_006cc0f0_server *server, char *key);

// HYPOTHESIS: returns the value stored under a key of a server entry, or the default given;
// the "hostaddr" key is answered from the address itself.
// FUNCTION: STARFLEETOP 0x006cc920
char *fn_006cc920(fn_006cc0f0_server *server, char *key, char *fallback)
{
	char **pair;
	if (strcmp(key, "hostaddr") == 0)
		return fn_006cc8f0(server);
	pair = fn_006cc9a0(server, key);
	if (pair == 0)
		return fallback;
	return pair[1];
}

// HYPOTHESIS: finds the key/value pair stored under a key of a server entry; the key is first
// looked up in the shared string table, since pairs hold the interned strings.
// FUNCTION: STARFLEETOP 0x006cc9a0
char **fn_006cc9a0(fn_006cc0f0_server *server, char *key)
{
	fn_006cc530_record *record = (fn_006cc530_record *)server;
	char *pair[2];
	char **found = (char **)fn_006bf750(record->field_10, &key);
	if (found == 0)
		return 0;
	pair[0] = *found;
	return (char **)fn_006bf750(record->field_0c, pair);
}

// HYPOTHESIS: returns the value stored under a key of a server entry as a number, or the
// default given; the "ping" key is answered from the measured ping itself.
// FUNCTION: STARFLEETOP 0x006cc9e0
int fn_006cc9e0(fn_006cc0f0_server *server, char *key, int fallback)
{
	char **pair;
	if (strcmp(key, "ping") == 0)
		return fn_006cc8e0((fn_006cc530_record *)server);
	pair = fn_006cc9a0(server, key);
	if (pair == 0)
		return fallback;
	return atoi(pair[1]);
}

// HYPOTHESIS: a case-insensitive string hash reduced to a table of the given size.
// FUNCTION: STARFLEETOP 0x006cca80
unsigned int fn_006cca80(char *s, unsigned int size)
{
	unsigned int hash = 0;
	for (; *s; s++)
		hash = hash * 0x9ccf9319 + tolower(*s);
	return hash % size;
}

// HYPOTHESIS: a reporting record of 0xf0 bytes: the socket twice, two names, four values
// passed in, state words, the bound port and a flag.
struct fn_006ccb20_record
{
	SOCKET field_00;
	SOCKET field_04;
	char field_08[0x40];
	char field_48[0x80];
	unsigned long field_c8;
	unsigned long field_cc;
	unsigned long field_d0;
	unsigned long field_d4;
	int field_d8;
	int field_dc;
	int field_e0;
	int field_e4;
	char field_e8;
	unsigned long field_ec;
};

void fn_006bf560();
void fn_006ccf20(char *host, int port, sockaddr_in *address, int param_4);
int fn_006ccf00(SOCKET s, char *host, int port);

extern fn_006ccb20_record g_00888748;
extern fn_006ccb20_record *g_00888838;

// HYPOTHESIS: opens a UDP socket bound to the first free port of the hundred from the one
// given (0 lets the system pick), then fills the record (a new one when the caller asks for
// it, else the global one) and contacts the master server.
// FUNCTION: STARFLEETOP 0x006ccb20
int fn_006ccb20(fn_006ccb20_record **out, char *host, int port, char *param_4, char *param_5, unsigned long param_6, unsigned long param_7, unsigned long param_8, unsigned long param_9, unsigned long param_10)
{
	sockaddr_in address;
	int length;
	int result;
	SOCKET s;
	int last;

	if (out != 0)
		*out = 0;
	fn_006bf560();
	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s != INVALID_SOCKET)
	{
		last = port + 100;
		for (; port < last; port++)
		{
			fn_006ccf20(host, port, &address, 0);
			if (address.sin_addr.s_addr == htonl(0x7f000001))
				address.sin_addr.s_addr = INADDR_ANY;
			result = bind(s, (sockaddr *)&address, sizeof(address));
			if (result == 0)
				break;
		}
		if (result != 0)
			return 2;
		if (port == 0)
		{
			length = sizeof(address);
			if (getsockname(s, (sockaddr *)&address, &length) != 0)
				return 2;
			port = ntohs(address.sin_port);
		}
		if (out == 0)
			g_00888838 = &g_00888748;
		else
		{
			g_00888838 = (fn_006ccb20_record *)malloc(sizeof(fn_006ccb20_record));
			*out = g_00888838;
		}
		strcpy(g_00888838->field_08, param_4);
		strcpy(g_00888838->field_48, param_5);
		g_00888838->field_e4 = port;
		g_00888838->field_d8 = 0;
		g_00888838->field_04 = s;
		g_00888838->field_00 = s;
		g_00888838->field_e0 = 0;
		g_00888838->field_dc = 1;
		g_00888838->field_e8 = 1;
		g_00888838->field_ec = param_10;
		g_00888838->field_c8 = param_6;
		g_00888838->field_cc = param_7;
		g_00888838->field_d4 = param_9;
		g_00888838->field_d0 = param_8;
		return fn_006ccf00(s, "master.gamespy.com", 0x6cfc);
	}
	return 1;
}

void fn_006cce00(fn_006ccb20_record *record);
void fn_006ccd60(fn_006ccb20_record *record);

// FUNCTION: STARFLEETOP 0x006ccd40
void fn_006ccd40(fn_006ccb20_record *record)
{
	if (record == 0)
		record = g_00888838;
	fn_006cce00(record);
	fn_006ccd60(record);
}

void fn_006cd230(fn_006ccb20_record *record, char *buffer, sockaddr_in *from);

extern char g_00c7ea68[256];

// HYPOTHESIS: drains the record's socket without blocking and hands every datagram to
// fn_006cd230.
// FUNCTION: STARFLEETOP 0x006ccd60
void fn_006ccd60(fn_006ccb20_record *record)
{
	fd_set set;
	sockaddr_in from;
	int fromlen = sizeof(from);
	timeval timeout = {0, 0};
	int result;

	if (record->field_00 == INVALID_SOCKET)
		return;
	FD_ZERO(&set);
	FD_SET(record->field_00, &set);
	for (;;)
	{
		result = select(FD_SETSIZE, &set, 0, 0, &timeout);
		if (result == SOCKET_ERROR || result == 0)
			break;
		int length = recvfrom(record->field_00, g_00c7ea68, sizeof(g_00c7ea68) - 1, 0, (sockaddr *)&from, &fromlen);
		if (length != SOCKET_ERROR)
		{
			g_00c7ea68[length] = 0;
			fn_006cd230(record, g_00c7ea68, &from);
		}
	}
}
