// @brief ��Ҫ����: սʿפ�ز���
// @author ����: �� �� ��

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
