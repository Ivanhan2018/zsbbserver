// @brief ��Ҫ����: սʿ��������
// @author ����: �� �� ��

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

	// ����������ִ������
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @param strategyMgr ���Թ�����
	// @return ����true / false �� true: �ܹ���  false��������
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjPtr, IManager *strategyMgrPtr, bool isEven);

private:
	BowArrowFigAttackStrategy bowArrowFigAttackStrategy;
};

#endif
