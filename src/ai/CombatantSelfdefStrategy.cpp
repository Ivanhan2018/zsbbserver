// @brief ��Ҫ����: սʿ��������
// @author ����: �� �� ��

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