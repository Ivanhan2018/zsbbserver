// @brief 简要描述: 战士防御策略
// @author 作者: 黄 文 才

#include "CombatantRecoveryStrategy.h"

CombatantRecoveryStrategy::CombatantRecoveryStrategy()
{
}

CombatantRecoveryStrategy::~CombatantRecoveryStrategy(void)
{
}


bool CombatantRecoveryStrategy::thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven)
{
	return bowArrowRecovery.thinking(mapDataParamPtr, seekPathArithmParamPtr, armyObj, strategyMgr, isEven);
}