// @brief ��Ҫ����: ��ʦ��������
// @author ����: �� �� ��

#ifndef _CHURCHMANSELFDEFENDSTRATEGY_H_H
#define _CHURCHMANSELFDEFENDSTRATEGY_H_H

#include "ichurchmanstrategy.h"

class ChurchmanSelfdefenceStrategy : public IChurchmanStrategy
{
public:
	ChurchmanSelfdefenceStrategy();
public:
	virtual ~ChurchmanSelfdefenceStrategy(void);

	// ����������ִ������
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @param strategyMgr ���Թ�����
	// @return ����true / false �� true: �ܹ���  false��������
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven);
};

#endif
