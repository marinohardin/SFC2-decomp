//	sfcstreams.cpp
//
//	The byte stream the game archives and sends its data through.
//

#include "unsorted/sfcstreams.h"

// FUNCTION: STARFLEETOP 0x006729e0
void TextStreamCompress(char *text, tSFCStream &s)
{
	s.mWriteStringToStream(text);
}
