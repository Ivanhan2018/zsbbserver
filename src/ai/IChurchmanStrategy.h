// @brief 简要描述: 牧师策略抽象类
// @author 作者: 黄 文 才

#ifndef _ICHURCHMANSTRATEGY_H_H_H
#define _ICHURCHMANSTRATEGY_H_H_H

#include "AiCommon.h"
#include "CommonFun.h"

class IChurchmanStrategy
{
public:
	virtual ~IChurchmanStrategy(){}

	// 函数描述：执行命令
	// @param mapDataPtr 部队地图数据指针
	// @param seekPathArithmParamPtr 寻路对象的指针
	// @param armyObj 当前操作的部队
	// @param strategyMgr 策略管理器
	// @return 返回true / false ； true: 能够走  false：不能走
	virtual bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjm, IManager *strategyMgr, bool isEven) = 0;
};

#endif
