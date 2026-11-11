//	tstringdefs.h: the build options of tString.
//
//	Declarations follow the SDK's header.

#ifndef	tstringdefs_h
#define	tstringdefs_h	1

#define kMaxStringClassLength	 50000

#define kGenericStringImpl  (0)
#define kUseMemoryPart		(0)
#define kUnicodeConvert		(0)

#ifndef kDebugMemoryUsage
	#ifdef _DEBUG
		#if kUseMemoryPart
			#define kDebugMemoryUsage	(1)
		#else
			#define kDebugMemoryUsage	(0)
		#endif
	#else
		#define kDebugMemoryUsage	(0)
	#endif
#endif

#endif
