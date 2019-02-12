// @brief 简要描述: 法师策略抽象类
// @author 作者: 黄 文 才

#ifndef _IRABBISTRATEGY_H_H_H
#define _IRABBISTRATEGY_H_H_H

#include "CommonFun.h"

using namespace aistrategy;

class IRabbiStrategy
{
public:
	virtual ~IRabbiStrategy(){}

	// 函数描述：执行命令
	// @param mapDataParamPtr 部队地图数据指针
	// @param seekPathArithmParamPtr 寻路对象的指针
	// @param armyObj 当前操作的部队
	// @param strategyMgr 策略管理器
	// @return 返回true / false ； true: 能够走  false：不能走
	virtual bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr  armyObj, IManager *strategyMgr, bool isEven) = 0;
};
#endif
