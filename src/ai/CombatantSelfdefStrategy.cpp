// @brief 简要描述: 战士自卫策略
// @author 作者: 黄 文 才

#include "CombatantSelfdefStrategy.h"

CombatantSelfdefStrategy::CombatantSelfdefStrategy()
{
}

CombatantSelfdefStrategy::~CombatantSelfdefStrategy(void)
{
}
bool CombatantSelfdefStrategy::thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven)
{
	return bowArrowFigSelfdefStrategy.thinking(mapDataParamPtr, seekPathArithmParamPtr, armyObj, strategyMgr, isEven);
}