// @brief ��Ҫ����: ������פ�ز���
// @author ����: �� �� ��

#ifndef _BOWARROWFIGDFSTRAGEGY_H_H
#define _BOWARROWFIGDFSTRAGEGY_H_H

#include "ibowarrowfigstragegy.h"

class BowArrowFigDefendStrategy : public IBowArrowFigStrategy
{
public:
	BowArrowFigDefendStrategy();
public:
	virtual ~BowArrowFigDefendStrategy(void);

	// ����������ִ������
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @param strategyMgr ���Թ�����
	// @return ����true / false �� true: �ܹ���  false��������
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven);
};

#endif
