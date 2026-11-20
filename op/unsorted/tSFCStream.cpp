#include "Tiger/tstring.h"
#include "Tiger/tvectormath.h"
#include "unsorted/sfcstreams.h"

// FUNCTION: STARFLEETOP 0x0040d60d
tSFCStream::tSFCStream(unsigned char* buffer, long size)
{
    fBuffer = buffer;
    fOwned = 0;
    fMaxPtr = fBuffer + size;
    fGetPtr = fBuffer;
    fPutPtr = fBuffer;
    fReadLock = 0;
    fWriteLock = 0;
    if (fBuffer == 0)
        fMaxPtr = 0;
    fTabNumber = 0;


}
