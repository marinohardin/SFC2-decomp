//	<Name>	NetMsgClass.cpp
//
//
//	<Info>	DESCRIPTION
//


#include "Source/network/NetMsgClass.h"
#include "unsorted/sfcstreams.h"
#include "Tiger/tstring.h"
#include "dmp3d/common/stream_basicfile.h"


//--------------------------------------------------------------------------------
//
//
// FUNCTION: STARFLEETOP 0x00434a14
unsigned int tNetMsg::mStream(const void *pStreamBuffer, unsigned char bOutput, tNetMsg *pMsg)
{

	int32u	tally		= 0;
	int8u	*pStream	= (int8u*)pStreamBuffer;
	const tNetMsg::tMapItem *p = pMsg->mGetMap();
	for (; p->fSize; p++)
	{
		int32	itemSize	= p->fSize;
		int8u	*pThis		= (int8u*)pMsg + p->fOffset;
		switch (itemSize)
		{
			case tNetMsg::kSizeInt8:
			case tNetMsg::kSizeInt16:
			case tNetMsg::kSizeInt32:
			case tNetMsg::kSizeInt64:
			if (bOutput)
			{
				tSFCStream::mIO(pStream,pThis,itemSize);
			}
			else
			{
				tSFCStream::mIO(pThis,pStream,itemSize);
			}
			break;

			case tNetMsg::kSizeTPoint3:
			{
				const int kSize = sizeof(float);
				int8u	*s = bOutput ? pThis : pStream;
				int8u	*d = bOutput ? pStream : pThis;
				tSFCStream::mIO(d,s,kSize);	d += kSize; s += kSize;
				tSFCStream::mIO(d,s,kSize);	d += kSize; s += kSize;
				tSFCStream::mIO(d,s,kSize);
			}
			break;

			//
			//
			case tNetMsg::kSizeTString:
			if (bOutput)
			{
				tString			&str	= *((tString*)pThis);
				const int16u	kLen	= (int16u)str.mStrLen();

				int8u	*d = pStream;
				tSFCStream::mIO(d,&kLen,sizeof(kLen));	// Send out length (as int16u)
				d += sizeof(kLen);

				for (int32u i=0; i<kLen; i++)
				{
					iWideChar	ch = str[i];
					tSFCStream::mIO(d,&ch,sizeof(ch));
					d += sizeof(ch);
				}

				itemSize = (int32u)d - (int32u)pStream;
			}
			else
			{
				const int8u	*s		= pStream;
				tString		&str	= *((tString*)pThis);

				// Get length
				//
				int16u	len;
				tSFCStream::mIO(&len,s,sizeof(len));
				s += sizeof(len);

				// Empty out the dest string, then retrieve source string
				// character by character
				//
				str	= "";
				for (int i=0; i<len; i++)
				{
					iWideChar	ch;
					tSFCStream::mIO(&ch,s,sizeof(ch));
					s += sizeof(ch);
					str += ch;
				}

				itemSize = (int32u)s - (int32u)pStream;
			}
			break;

			case tNetMsg::kSizeString:
			if (bOutput)
			{
				const char		*str	= (char*)pThis;
				const int16u	kLen	= strlen(str);

				int8u	*d = pStream;
				tSFCStream::mIO(d,&kLen,sizeof(kLen));	// Send out length (as int16u)
				d += sizeof(kLen);

				for (int32u i=0; i<kLen; i++)
				{
					*d++ = *str++;
				}

				itemSize = (int32u)d - (int32u)pStream;
			}
			else
			{
				const int8u	*s	= pStream;
				int8u		*d	= (int8u*)pThis;

				// Get length
				//
				int16u	len;
				tSFCStream::mIO(&len,s,sizeof(len));
				s += sizeof(len);

				// Retrieve source string character by character
				//
				while (len-- > 0)
				{
					*d++ = *s++;
				}
				*d = 0;		// add nul terminator

				itemSize = (int32u)s - (int32u)pStream;
			}
			break;


			case tNetMsg::kSizeSFCStream:
			if (bOutput)
			{
				tSFCStream *sfcStream	= (tSFCStream *)pThis;
				const int32u	kLen	= (int32u)sfcStream->mGetReadSize();

				int8u	*d = pStream;
				int8u	*s = sfcStream->mGetReadPtr();
				tSFCStream::mIO(d,&kLen,sizeof(kLen));	// Send out length (as int16u)
				d += sizeof(kLen);
				for (int i=0; i<kLen; i++)
				{
					*d++ = *s++;
				}

				itemSize = (int32u)d - (int32u)pStream;
			}
			else
			{
				tSFCStream *sfcStream	= (tSFCStream *)pThis;
				const int8u	*s = pStream;
				int8u	*d = sfcStream->mGetWritePtr();

				// Get length
				//
				int32u	len;
				tSFCStream::mIO(&len,s,sizeof(len));
				s += sizeof(len);

				for (int i=0; i<len; i++)
				{
					*d++ = *s++;
				}
				sfcStream->mSetReadSize(len);

				itemSize = (int32u)s - (int32u)pStream;
			}
			break;

			default:
			if (itemSize < 0)
			{
				// Negative sizes are a special flag indicating that the
				// buffer should be copied as-is, no byte swapping at all
				// Only values of -4 or less will be recognized -- so be very very
				//	careful using this!!!
				itemSize = -itemSize;
				int8u	*s;
				int8u	*d;
				if (bOutput)
				{
					s = pThis;
					d = pStream;
				}
				else
				{
					s = pStream;
					d = pThis;
				}
				for (int i=0; i<itemSize; i++)
				{
					*d++ = *s++;
				}
			}
			else
			{
				// Unknown commands are currently fatal!
				//
				throw fn_006a2ec0(__FILE__,__LINE__,
					"tNetMsg::mStream(): Don't know how to stream this!\n"
				);
			}
		}

		pStream += itemSize;
		tally += itemSize;
	}
	return(tally);
}
