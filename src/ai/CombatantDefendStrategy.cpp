// @brief 简要描述: 战士驻守策略
// @author 作者: 黄 文 才

#include "CombatantDefendStrategy.h"

CombatantDefendStrategy::CombatantDefendStrategy()
{
}

CombatantDefendStrategy::~CombatantDefendStrategy(void)
{
}

bool CombatantDefendStrategy::thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven)
{
	return bowArrowFigDefStrategy.thinking(mapDataParamPtr, seekPathArithmParamPtr, armyObj, strategyMgr, isEven);
}
