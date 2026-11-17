// ScriptMapData.h: interface for the tScriptMapData class.
#ifndef SCRIPTMAPDATA_H
#define SCRIPTMAPDATA_H

#include <vector>

#include "Common/BaseTypes.h"

class tMapObjectPoint
{
public:
	tMapObjectPoint();
	tMapObjectPoint( int32 x, int32 y );

	int32 fX;
	int32 fY;
};

class tMapObject;
typedef std::vector<tMapObject> tMapItemList;

// HYPOTHESIS: declared only as far as mGetItemList needs it; the SDK header's tMapObject,
// fMapName (a std::string) and fMapLines follow fMapItemList.
class tScriptMapData
{
public:
	const tMapItemList& mGetItemList() const;

protected:
	tMapObjectPoint fMapDimension;
	tMapItemList fMapItemList;
};

#endif
