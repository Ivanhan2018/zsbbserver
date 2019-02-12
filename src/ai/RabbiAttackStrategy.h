// @brief ��Ҫ����: ��ʦ��������
// @author ����: �� �� ��

#ifndef _RABBIATTACKSTRATEGY_H_H_H
#define _RABBIATTACKSTRATEGY_H_H_H

#include "irabbistrategy.h"
#include "BowArrowFigAttackStrategy.h"

class RabbiAttackStrategy :
	public IRabbiStrategy
{
public:
	RabbiAttackStrategy();
public:
	virtual ~RabbiAttackStrategy(void);

	// ����������ִ������
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @param strategyMgr ���Թ�����
	// @return ����true / false �� true: �ܹ���  false��������
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr  armyObjParam, IManager *strategyMgr, bool isEven);

private:
	BowArrowFigAttackStrategy bowArrowFigAttackStrategy;
};

#endif

