// @brief ��Ҫ����: ��ʦ���Գ�����
// @author ����: �� �� ��

#ifndef _IRABBISTRATEGY_H_H_H
#define _IRABBISTRATEGY_H_H_H

#include "CommonFun.h"

using namespace aistrategy;

class IRabbiStrategy
{
public:
	virtual ~IRabbiStrategy(){}

	// ����������ִ������
	// @param mapDataParamPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @param strategyMgr ���Թ�����
	// @return ����true / false �� true: �ܹ���  false��������
	virtual bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr  armyObj, IManager *strategyMgr, bool isEven) = 0;
};
#endif
