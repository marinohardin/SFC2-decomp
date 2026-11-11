//	NetMsgClass.h: tNetMsg, the base of the network messages, as the SDK names it.
// Evidence:  SDK header (NetMsgClass.h)

#ifndef NetMsgClass_h
#define NetMsgClass_h 1

// The SDK's map-item macros (NetMsgClass.h).
#define QSI_FIELD_OFFSET(type, field)     ((long)&(((type *)0)->field))
#define QSI_FIELD_SIZE(type,field)        (sizeof( ((type *)0)->field ) )
#define	MAKE_NMMI(type,field)	{	QSI_FIELD_SIZE(type,field), QSI_FIELD_OFFSET(type,field) }

// The SDK's message base (NetMsgClass.h). Only the commands the units use are listed.
struct tNetMsg
{
	enum eCommandID
	{
		kAdjustSpeedAbs = 1,
		kDamage = 15,
		kKillEntity = 21,
		kAIChangeTeamRelationshipToWar = 51
	};

	struct tMapItem
	{
		short fSize;
		unsigned short fOffset;
	};

	virtual const tNetMsg::tMapItem *mGetMap(void) const = 0;
	virtual unsigned int mStreamTo(unsigned char *pStreamBuffer);
	virtual unsigned int mStreamFrom(const unsigned char *pStreamBuffer);
};

#endif	// NetMsgClass_h
