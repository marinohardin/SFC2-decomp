//	tsay.cpp: tSay, named and levelled debug output through the debug part.

#include "Tiger/tsay.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>


tSay	_gGlobalSay("Global Say");

tSay::tMasterControl	tSay::fMasterControl;


// FUNCTION: STARFLEETOP 0x0066c5f0
tSay::tSay(const char *userName, const int32u kTargetID)
{
	fMasterControl.fNumberActiveInstances++;

	if (userName)
	{
		int i;
		for (i = 0; i < kMaxUserName - 1; i++)
		{
			char c = *userName++;
			fUserName[i] = c;
			if (c == 0)
				break;
		}
		fUserName[i] = 0;
	}
	else
	{
		fUserName[0] = 0;
	}

	fLevel			= 0;
	fIndentAmount	= 0;
	fSkipRate		= 0;
	fSkipCntr		= 0;
	fLogFileTarget	= 0;
	fIndentChar		= ' ';
	fTargets		= kTargetID;
	fOnOff			= kTrue;
}

// tSay's scalar deleting destructor is 0x0066c650; this unit does not emit it, so it is not claimed.
// FUNCTION: STARFLEETOP 0x0066c680
tSay::~tSay()
{
	fMasterControl.fNumberActiveInstances--;
}

// FUNCTION: STARFLEETOP 0x0066c6a0
void tSay::mHandleOutput(const int kLevel, const char *stringMessage)
{
	if (!fMasterControl.fOnOff || !fOnOff)
		return;

	if (fLevel > kLevel)
		return;

	if (fSkipRate)
	{
		if (--fSkipCntr >= 0)
			return;

		fSkipCntr = fSkipRate;
	}

	char	buffer[512];
	int		length = 0;

	buffer[0] = 0;

	if (fMasterControl.fNamesOnOff)
	{
		sprintf(buffer, "(%8s)", fUserName);
		length = strlen(buffer);
	}

	if (fMasterControl.fLevelOnOff)
	{
		sprintf(&buffer[length], "<%5d> ", kLevel);
		length = strlen(buffer);
	}

	if (fIndentAmount)
	{
		int		count = sizeof(buffer) - 1 - length;
		if (count > fIndentAmount)
			count = fIndentAmount;

		char	*indent = &buffer[length];
		for (int i = 0; i < count; i++)
			*indent++ = fIndentChar;

		*indent = 0;
	}

	if (buffer[0])
		aDebugPart->mSendLine(buffer, fTargets);

	aDebugPart->mSendLine(stringMessage, fTargets);
}


static char	gFmtStr[1024];

// FUNCTION: STARFLEETOP 0x0066c890
void tSay::mSay(const char *format,...)
{
	va_list	args;

	va_start(args, format);
	vsprintf(gFmtStr, format, args);
	va_end(args);

	mHandleOutput(0, gFmtStr);
}

// FUNCTION: STARFLEETOP 0x0066c8c0
void tSay::mJustSayIt(const char *format,...)
{
	va_list	args;

	va_start(args, format);
	vsprintf(gFmtStr, format, args);
	va_end(args);

	aDebugPart->mSendLine(gFmtStr, fTargets);
}
