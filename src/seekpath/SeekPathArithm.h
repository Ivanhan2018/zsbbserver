// @brief ��Ҫ����: Ѱ·�㷨��
// @author ����: 

#ifndef _SEEK_PATH_ARITHM_H_H_H
#define _SEEK_PATH_ARITHM_H_H_H

#include <vector>
#include "AiCommon.h"
#include "../include/commonprotocol/Global_Macro.h"

using namespace aistrategy;

class SeekPath : public ISeekPath
{
public:
	SeekPath(const CLoadMapInfo *MapDataParamPtr);
	~SeekPath(void);

	//��õ�ͼ�赲��Ϣ
	inline bool IsBlockoff(int x, int y) const
	{
		if (NULL != MapDataPtr)
		{
			return MapDataPtr->IsBlockoff(x, y);
		}
		else
		{
#ifdef _DEBUG
			assert(0 && "�ϰ����ͼָ��Ϊ�գ�");
#endif
			return false;
		}
	}

	virtual int getLandFormWeightVal(int x, int y) const
	{
		if (NULL != MapDataPtr)
		{
			switch (MapDataPtr->getLandForm(x, y))
			{
			case LAND_FOREST:
				return LAND_FOREST_WEIGHT_VALUE;
			case LAND_RIVERWAY:
				return LAND_RIVERWAY_WEIGHT_VALUE;
			default:
				return LAND_PLAIN_WEIGHT_VALUE;
			}
		}
		else
		{
#ifdef _DEBUG
			assert(0 && "�ϰ����ͼָ��Ϊ�գ�");
#endif
			return false;
		}
	}

	inline const CLoadMapInfo * GetLoadMapInfoPtr()
	{
		return MapDataPtr;
	}

	inline LAND_FORM getLandForm(int x, int y) const
	{
		if (NULL != MapDataPtr)
		{
			return MapDataPtr->getLandForm(x, y);
		}
		else
		{
#ifdef _DEBUG
			assert(0 && "�ϰ����ͼָ��Ϊ�գ�");
#endif
			return LAND_INVALID;
		}
	}

private:
	const CLoadMapInfo *MapDataPtr;
};


#endif