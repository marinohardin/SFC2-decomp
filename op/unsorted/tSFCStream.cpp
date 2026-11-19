#include "Tiger/tstring.h"
#include "Tiger/tvectormath.h"
#include "unsorted/sfcstreams.h"

// FUNCTION: STARFLEETOP 0x0040d60d
tSFCStream::tSFCStream(unsigned char* buffer, long size)
{
    unsigned char* start = buffer;
    unsigned char* end = start + size;
    fMaxPtr = end;
    fBuffer = start;
    fOwned = 0;
    fGetPtr = start;
    fPutPtr = start;
    fReadLock = 0;
    fWriteLock = 0;
    if (start == 0)
        fMaxPtr = 0;
    fTabNumber = 0;
}
