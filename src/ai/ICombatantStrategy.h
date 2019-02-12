// @brief ��Ҫ����: սʿ���Գ�����
// @author ����: �� �� ��

#ifndef _ICOMBATANTSTRATEGY_H_H_H
#define _ICOMBATANTSTRATEGY_H_H_H

#include "AiCommon.h"
#include "CommonFun.h"

class ICombatantStrategy
{
public:
	virtual ~ICombatantStrategy(){}
	
	// ����������ִ������
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @param strategyMgr ���Թ�����
	// @return ����true / false �� true: �ܹ���  false��������
	virtual bool thinking(IArmyPtrvectInvect * mapDataParamPtr,  SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven) = 0;
};
#endif
