// @brief ��Ҫ����: ��ʦפ�ز���
// @author ����: �� �� ��

#ifndef _CHURCHMANDEFENDSTRATEGY_H_H
#define _CHURCHMANDEFENDSTRATEGY_H_H

#include "ichurchmanstrategy.h"

class ChurchmanDefendStrategy : public IChurchmanStrategy
{
public:
	ChurchmanDefendStrategy();
public:
	virtual ~ChurchmanDefendStrategy(void);

	// ����������ִ������
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @param strategyMgr ���Թ�����
	// @return ����true / false �� true: �ܹ���  false��������
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven);
};

#endif
