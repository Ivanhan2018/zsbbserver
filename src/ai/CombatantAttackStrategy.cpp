// @brief 简要描述: 战士进攻策略
// @author 作者: 黄 文 才

#include "CombatantAttackStrategy.h"

CombatantAttackStrategy::CombatantAttackStrategy()
{
}

CombatantAttackStrategy::~CombatantAttackStrategy(void)
{
}
bool CombatantAttackStrategy::thinking(IArmyPtrvectInvect * mapDataParamPtr,  SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjPtr, IManager *strategyMgrPtr, bool isEven)
{
	return bowArrowFigAttackStrategy.thinking(mapDataParamPtr, seekPathArithmParamPtr, armyObjPtr, strategyMgrPtr, isEven);

}
