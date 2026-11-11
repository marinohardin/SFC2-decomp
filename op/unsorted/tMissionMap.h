//	tMissionMap.h: the map a mission is played on, as the SDK's tmissionmap.h declares it.
#ifndef tMissionMap_h
#define tMissionMap_h

struct tPoint3;

struct tMapPoint
{
	short	x;
	short	y;

	void operator=(tMapPoint rhs)
	{
		x = rhs.x;
		y = rhs.y;
	}
};

class tMissionMap
{
public:
	tMissionMap(void);
	virtual ~tMissionMap();

	// Map to screen location conversion
	tPoint3 mConvertLocation(tMapPoint);
	// screen to map location conversion
	tMapPoint m3DToMapPoint(tPoint3 LocationIn3D);

	// returns map width and height
	static tMapPoint mGetMapDimension(void) { return fMapSize; }

protected:
	// HYPOTHESIS: at 0x00c3ac8c; tAICaptain's 0x0051feca reads the map size there.
	static tMapPoint fMapSize;
};

// HYPOTHESIS: the mission map of the battle being played; tAICaptain's 0x0051feca converts
// points with it.
extern tMissionMap *g_0090b734;

#endif
