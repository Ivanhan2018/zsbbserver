// @brief ��Ҫ����: ��ʦ��������
// @author ����: �� �� ��

#ifndef _RABBIRECOVERYSTRATEGY_H_H_H
#define _RABBIRECOVERYSTRATEGY_H_H_H

#include "irabbistrategy.h"
#include "BowArrowFigRecoveryStrategy.h"

class RabbiRecoveryStrategy :
	public IRabbiStrategy
{
public:
	RabbiRecoveryStrategy();
public:
	virtual ~RabbiRecoveryStrategy(void);

	// ����������ִ������
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @param strategyMgr ���Թ�����
	// @return ����true / false �� true: �ܹ���  false��������
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjParam, IManager *strategyMgr, bool isEven);

private:
	BowArrowFigRecoveryStrategy bowArrowRecovery;
};
#endif
