// @brief 简要描述: 战士进攻策略
// @author 作者: 黄 文 才

#ifndef _COMBATANTATTACKSTRATEGY_H_H
#define _COMBATANTATTACKSTRATEGY_H_H

#include "icombatantstrategy.h"
#include "BowArrowFigAttackStrategy.h"

class CombatantAttackStrategy :public ICombatantStrategy
{
public:
	CombatantAttackStrategy();
public:
	virtual ~CombatantAttackStrategy(void);

	// 函数描述：执行命令
	// @param mapDataPtr 部队地图数据指针
	// @param seekPathArithmParamPtr 寻路对象的指针
	// @param armyObj 当前操作的部队
	// @param strategyMgr 策略管理器
	// @return 返回true / false ； true: 能够走  false：不能走
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjPtr, IManager *strategyMgrPtr, bool isEven);

private:
	BowArrowFigAttackStrategy bowArrowFigAttackStrategy;
};

#endif
