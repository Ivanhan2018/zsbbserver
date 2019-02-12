// @brief ��Ҫ����: ��ʦפ�ز���
// @author ����: �� �� ��

#ifndef _RABBI_DEFEND_STRATEGY_H_H
#define _RABBI_DEFEND_STRATEGY_H_H

#include "irabbistrategy.h"
#include "BowArrowFigDefendStrategy.h"

class RabbiDefendStrategy : public IRabbiStrategy
{
public:
	RabbiDefendStrategy();
public:
	virtual ~RabbiDefendStrategy(void);

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

