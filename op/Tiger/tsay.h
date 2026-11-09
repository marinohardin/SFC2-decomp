//	tsay.h: tSay, named and levelled debug output through the debug part.

#ifndef tsay_h
#define tsay_h

// The original includes tbasic.h and tdebug.h here; tdebug.h includes tpart.h, which carries the
// tbasic.h declarations this unit needs.
#include "Tiger/tpart.h"

// The original includes tdebug.h here. The unit map has no place for a header without a unit of
// its own yet, so the declarations this unit needs from it are kept here under its guard.
#ifndef	tdebug_h
#define	tdebug_h

class	tDebugPart: public tPart
{
protected:
	static	tDebugPart	*fTheDebugPart;

public:
	tDebugPart()			{	fTheDebugPart = this;	}
	virtual	~tDebugPart()	{	};

	GenAbstractBinding( tDebugPart );

	enum
	{
		kDefault	= 0x0001,
		kDebugger	= 0x0002,
		kComPort	= 0x0004,
		kWindow		= 0x0008,
		k2ndMonitor	= 0x0010,
		kMonoMonitor= 0x0020,

			_kPredefinedMax,
			_kPredefinedMask	= (_kPredefinedMax-1) | (_kPredefinedMax-2)
	};

	virtual void	mSendLine(const char *str, const int32u kTargets=kDefault)	= 0;

	virtual int32u	mCreateLogFileTarget(
						const char	*filename,
						iTruth		bAppend	= kFalse,
						iTruth		bCreateIfNecessary = kTrue)					= 0;

	virtual void	mCloseLogFileTarget(const char *filename)					= 0;
	virtual void	mCloseLogFileTarget(const int32u kTargetID)					= 0;

	static void mSendLineAlways(const char *str, const int32u kTargets=kDefault)
	{
		if (fTheDebugPart)
		{
			fTheDebugPart->mSendLine(str,kTargets);
		}
	}

	static	tDebugPart	*mGet(void)
	{
		return( fTheDebugPart );
	}
};

#define	aDebugPart	(BindPart(tDebugPart))

#endif	// #ifndef tdebug_h

class tSay
{
private:
	struct tMasterControl
	{
		int		fNumberActiveInstances;
		iTruth	fOnceOnly;
		iTruth	fOnOff;
		iTruth	fNamesOnOff;
		iTruth	fLevelOnOff;

		inline tMasterControl()
		{
			fNumberActiveInstances 	= 0;
			fOnceOnly				= kTrue;
			fOnOff 					= kTrue;
			fNamesOnOff				= kTrue;
			fLevelOnOff				= kTrue;
		}
	};

private:
	static tMasterControl 	fMasterControl;

	enum	{	kMaxUserName = 80	};
	char	fUserName[kMaxUserName];
	int		fLevel;
	char	fIndentChar;
	int		fIndentAmount;
	int		fSkipRate;
	int		fSkipCntr;
	int32u	fTargets;
	int32u	fLogFileTarget;
	iTruth	fOnOff;

protected:
	void	mHandleOutput(const int kLevel, const char *stringMessage);

public:
	tSay(const char *userName=NULL, const int32u kTargetID=tDebugPart::kDebugger);
	virtual ~tSay();

	static	void mSetMasterOnOff(const iTruth bOnOff);
	static	void mSetShowNamesOnOff(const iTruth bOnOff);
	static	void mSetShowLevelsOnOff(const iTruth bOnOff);

	void	mSetOnOff(const iTruth bOnOff);
	void	mSetLevel(const int kLevel);

	void	mSetIndentChar(const char c);
	void	mSetIndent(const int kIndent);
	int		mIncrementIndent(const int kAmount);

	void	mSetSkipRate(const int kSkipRate);

	void	mEnableTarget(const int32u kTargetID);
	void	mDisableTarget(const int32u kTargetID);

	iTruth	mSetLogFile(const char *name);
	iTruth	mCloseLogFile(void);

	void	mJustSayIt(const char *format,...);
	void	mSay(const char *format,...);
	void	mSay(const int kLevel, const char *format,...);
};

extern tSay	_gGlobalSay;

#endif	// #ifndef tsay_h
