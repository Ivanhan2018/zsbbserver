// @brief ��Ҫ����: Ѱ·�㷨��
// @author ����:

#include <assert.h>
#include "windows.h"
#include "SeekPathArithm.h"

#define SPACE_VALUE_BETWEEN_NODE   1

SeekPath::SeekPath(const CLoadMapInfo *MapDataParamPtr)
: MapDataPtr(MapDataParamPtr)
{
	Init(static_cast<int>((MapDataPtr->MapInfo).size()), static_cast<int>((MapDataPtr->MapInfo)[0].size()));
}

SeekPath::~SeekPath(void)
{
}
