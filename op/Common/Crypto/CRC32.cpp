#include "Common/Crypto/CRC32.h"

// FUNCTION: STARFLEETOP 0x006fd970
void tCRC32::GenerateCRCTable()
{
	for( int i = 0; i < kTableSize; i++ )
	{
		int32u crc = i << 24;
		for( int j = 0; j < 8; j++ )
		{
			if( crc & 0x80000000 )
				crc = ( crc << 1 ) ^ 0x04C11DB7;	// HYPOTHESIS: the polynomial literal, not fPolynomial, which VC6 would not fold
			else
				crc = crc << 1;
		}
		fTable[ i ] = crc;
	}
}
