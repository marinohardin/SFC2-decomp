#include "Common/Thread/Optex.h"

// HYPOTHESIS: returns the thread state whose default use the optex takes; the name is not known.
tThreadState *fn_005d4c97();

// MATCH-ONLY: the manifest reads a global's definition only without a parameter list.
#define DEFAULT_THREAD_USE fn_005d4c97()->DefaultThreadObjectUse()

// HYPOTHESIS: a global optex at 0x00c3a138; the original's name is not known.
// FUNCTION: STARFLEETOP 0x0060237c
tOptex g_00c3a138 = DEFAULT_THREAD_USE;
