// @brief ��Ҫ����: սʿ��������
// @author ����: �� �� ��

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
