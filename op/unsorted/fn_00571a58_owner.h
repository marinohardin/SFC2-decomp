//	fn_00571a58_owner.h: the vector-like owner of the forwarding members 0x00571a58 to 0x00571acd.
// Evidence:  calls (original name and file unknown)

#ifndef fn_00571a58_owner_h
#define fn_00571a58_owner_h 1

// HYPOTHESIS: a vector-like object (first, last and end-of-storage pointers after an allocator
// dword) whose members forward to the library's out-of-line insert of several instances; they
// are claimed in unsorted/fn_00571000.cpp. 0x00571a69 is one folded copy of a pointer vector's
// push_back, which fn_0053434B.cpp's 0x00534454 calls through this class.
class fn_00571a58_inner;
class fn_00571e33_elem;
class fn_00571a58_owner {
public:	char fn_00571a58_pad[4];
	fn_00571a58_inner* fn_00571a58_first;
	fn_00571a58_inner* fn_00571a58_ptr;
	fn_00571a58_inner* fn_00571a58_limit;
	void fn_00571a58(int arg);
	void fn_00571a69(int arg);
	void fn_00571a9a(int arg);
	void fn_00571aab(int arg);
	void fn_00571abc(int arg);
	unsigned int size() const;
	void fn_00571bda(fn_00571a58_inner* pos, unsigned int m, const fn_00571a58_inner& x);
	void fn_00571e33(fn_00571e33_elem* pos, unsigned int m, const fn_00571e33_elem& x);
	void fn_00571acd(int arg);
};

#endif // fn_00571a58_owner_h
