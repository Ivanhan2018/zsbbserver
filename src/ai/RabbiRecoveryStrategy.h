// @brief 简要描述: 法师防御策略
// @author 作者: 黄 文 才

#ifndef _RABBIRECOVERYSTRATEGY_H_H_H
#define _RABBIRECOVERYSTRATEGY_H_H_H

#include "irabbistrategy.h"
#include "BowArrowFigRecoveryStrategy.h"

class RabbiRecoveryStrategy :
	public IRabbiStrategy
{
public:
	RabbiRecoveryStrategy();
public:
	virtual ~RabbiRecoveryStrategy(void);

	// 函数描述：执行命令
	// @param mapDataPtr 部队地图数据指针
	// @param seekPathArithmParamPtr 寻路对象的指针
	// @param armyObj 当前操作的部队
	// @param strategyMgr 策略管理器
	// @return 返回true / false ； true: 能够走  false：不能走
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjParam, IManager *strategyMgr, bool isEven);

private:
	BowArrowFigRecoveryStrategy bowArrowRecovery;
};
#endif
