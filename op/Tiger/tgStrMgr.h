// tgStrManager: the game's string-table manager, a Tiger part (class name from the exe).

#ifndef TGSTRMGR_H
#define TGSTRMGR_H

#include "Tiger/tpart.h"
#include "Tiger/tstring.h"
#include <map>
#include <vector>

class tBasicFile;
class tString;

// HYPOTHESIS: tStrMgrCore (class name from the exe) has a virtual destructor in slot 0 and
// mDone in slot 1, then its two mLoadStringFile overloads (signatures from the exe's
// symbols; MSVC places overloads in reverse declaration order, so the file-name form lands in
// slot 3), then its two mGetStringIndex overloads (the string form in slot 5, by the same
// rule), then its three mGetString overloads (the index form in slot 8, by the same rule),
// then its two mGetNumElements overloads (the long form in slot 10, by the same rule), then
// its two mGetRandStr overloads (the long form in slot 12, by the same rule), then its two
// mGetGrpStr overloads (the long form in slot 14, by the same rule), then its four
// mIsAnElement overloads (signatures from the exe's symbols; only the (long, long),
// (const char *, long) and (long, const tString &) forms' slots are confirmed by matches so
// far); its other methods and its layout are not recovered yet.
class tStrMgrCore
{
	public:
		// Name from the exe's throw info; its enumerators are not recovered.
		enum eStrManagerErr {};

							tStrMgrCore( unsigned char );
		virtual				~tStrMgrCore( void );
		virtual		void	mDone		( void );
		virtual		unsigned char	mLoadStringFile	( const char *, bool );
		virtual		unsigned char	mLoadStringFile	( tBasicFile &, bool );
		virtual		int		mGetStringIndex	( const char * );
		virtual		int		mGetStringIndex	( const tString & );
		virtual		const tString &	mGetString	( int );
		virtual		const tString &	mGetString	( const char * );
		virtual		const tString &	mGetString	( const tString & );
		virtual		long	mGetNumElements	( long );
		virtual		long	mGetNumElements	( const char * );
		virtual		const tString &	mGetRandStr	( long );
		virtual		const tString &	mGetRandStr	( const char * );
		virtual		const tString &	mGetGrpStr	( long, long );
		virtual		const tString &	mGetGrpStr	( const char *, int );
		virtual		unsigned char	mIsAnElement	( long, long );
		virtual		unsigned char	mIsAnElement	( const char *, long );
		virtual		unsigned char	mIsAnElement	( long, const tString & );
		virtual		unsigned char	mIsAnElement	( const char *, const tString & );

		// HYPOTHESIS: layout from mDone's accesses only: two ordered containers whose
		// header pointers sit at +0x4 and +0x18, a vector of owned pointers (first/last
		// at +0x2c/+0x30) and a counter at +0x3c. Key and value types are guesses that
		// only reproduce the two distinct erase instantiations; the pads fill the
		// unexplained 4 bytes after each map.
		std::map<tString, int, less_ss>	fn_00627180_map1;	// type from mGetStringIndex's lower_bound instantiation
		int						fn_00627180_pad1;
		std::map<long, long>	fn_00627180_map2;
		int						fn_00627180_pad2;
		std::vector<tString *>	fn_00627180_vec;
		int						fn_00627180_count;
		unsigned char			fn_00627200_throwOnMiss;	// HYPOTHESIS: byte flag at +0x40; set, a failed lookup throws
};

// HYPOTHESIS: tgStrManager derives from tPart, and mDone is its first own virtual (vtable
// slot 6, the order of the class's methods in the exe's symbols). Its methods forward to an
// embedded tStrMgrCore at offset 0xc, right after tPart; the member's name is not known.
class tgStrManager : public tPart
{
	public:
		virtual		void	mDeactivate	( void );
		virtual		void	mDone		( void );
		virtual		unsigned char	mLoadStringFile	( const char *, bool );
		virtual		unsigned char	mLoadStringFile	( tBasicFile &, bool );
		virtual		int		mGetStringIndex	( const char * );
		virtual		int		mGetStringIndex	( const tString & );
		virtual		const tString &	mGetString	( int );
		virtual		const tString &	mGetString	( const char * );
		virtual		const tString &	mGetString	( const tString & );
		virtual		long	mGetNumElements	( long );
		virtual		long	mGetNumElements	( const char * );
		virtual		const tString &	mGetRandStr	( long );
		virtual		const tString &	mGetRandStr	( const char * );
		virtual		const tString &	mGetGrpStr	( long, long );
		virtual		const tString &	mGetGrpStr	( const char *, int );
		virtual		unsigned char	mIsAnElement	( long, long );
		virtual		unsigned char	mIsAnElement	( const char *, long );
		virtual		unsigned char	mIsAnElement	( long, const tString & );
		virtual		unsigned char	mIsAnElement	( const char *, const tString & );

		tStrMgrCore		fn_0066be50_core;	// HYPOTHESIS: name unknown
};

#endif // TGSTRMGR_H
