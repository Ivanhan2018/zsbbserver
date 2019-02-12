// @brief ��Ҫ����: �����ֲ��Գ�����
// @author ����: �� �� ��

#ifndef _IBOWARROWFIGSTRATEGY_H_H_H
#define _IBOWARROWFIGSTRATEGY_H_H_H

#include "CommonFun.h"
#include "IManager.h"

class IBowArrowFigStrategy
{
public:
	virtual ~IBowArrowFigStrategy(void){}

	// ����������ִ������
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @param strategyMgr ���Թ�����
	// @return ����true / false �� true: �ܹ���  false��������
	virtual bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr,  IArmyPtr armyObj, IManager *strategyMgr, bool isEven) = 0;
};

#endif


