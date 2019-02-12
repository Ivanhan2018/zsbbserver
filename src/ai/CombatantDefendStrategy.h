// @brief ��Ҫ����: սʿפ�ز���
// @author ����: �� �� ��

#ifndef _COMBATANTDEFENDSTRATEGY_H_H
#define _COMBATANTDEFENDSTRATEGY_H_H

#include "icombatantstrategy.h"
#include "BowArrowFigDefendStrategy.h"

class CombatantDefendStrategy :
	public ICombatantStrategy
{
public:
	CombatantDefendStrategy();
public:
	virtual ~CombatantDefendStrategy(void);

	// ����������ִ������
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @param strategyMgr ���Թ�����
	// @return ����true / false �� true: �ܹ���  false��������
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven);

private:
	BowArrowFigDefendStrategy bowArrowFigDefStrategy;
};

#endif

