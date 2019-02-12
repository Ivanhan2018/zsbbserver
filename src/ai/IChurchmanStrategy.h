// @brief ��Ҫ����: ��ʦ���Գ�����
// @author ����: �� �� ��

#ifndef _ICHURCHMANSTRATEGY_H_H_H
#define _ICHURCHMANSTRATEGY_H_H_H

#include "AiCommon.h"
#include "CommonFun.h"

class IChurchmanStrategy
{
public:
	virtual ~IChurchmanStrategy(){}

	// ����������ִ������
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @param strategyMgr ���Թ�����
	// @return ����true / false �� true: �ܹ���  false��������
	virtual bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjm, IManager *strategyMgr, bool isEven) = 0;
};

#endif
