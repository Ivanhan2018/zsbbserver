// @brief ��Ҫ����: սʿ��������
// @author ����: �� �� ��

#ifndef _COMBATANTRECOVERYSTRATEGY_H_H
#define _COMBATANTRECOVERYSTRATEGY_H_H

#include "icombatantstrategy.h"
#include "BowArrowFigRecoveryStrategy.h"

class CombatantRecoveryStrategy :
	public ICombatantStrategy
{
public:
	CombatantRecoveryStrategy();
public:
	virtual ~CombatantRecoveryStrategy(void);

	// ����������ִ������
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @param strategyMgr ���Թ�����
	// @return ����true / false �� true: �ܹ���  false��������
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven);

private:
	BowArrowFigRecoveryStrategy bowArrowRecovery;
};

#endif
