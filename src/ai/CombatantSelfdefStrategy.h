// @brief ��Ҫ����: սʿ��������
// @author ����: �� �� ��
#ifndef _COMBATANSELFDEFSTRATEGY_H_H
#define _COMBATANSELFDEFSTRATEGY_H_H

#include "icombatantstrategy.h"
#include "BowArrowFigSelfdefStrategy.h"

class CombatantSelfdefStrategy :
	public ICombatantStrategy
{
public:
	CombatantSelfdefStrategy();
public:
	virtual ~CombatantSelfdefStrategy(void);

	// ����������ִ������
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @param strategyMgr ���Թ�����
	// @return ����true / false �� true: �ܹ���  false��������
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven);

private:
	BowArrowFigSelfdefStrategy bowArrowFigSelfdefStrategy;

};

#endif
