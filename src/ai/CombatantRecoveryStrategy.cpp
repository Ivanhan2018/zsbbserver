// @brief ��Ҫ����: սʿ��������
// @author ����: �� �� ��

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