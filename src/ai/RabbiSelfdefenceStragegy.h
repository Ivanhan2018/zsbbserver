// @brief ��Ҫ����: ��ʦ��������
// @author ����: �� �� ��

#ifndef _RABBI_SELFDEFENCESTRATEGY_H_H
#define _RABBI_SELFDEFENCESTRATEGY_H_H

#include "irabbistrategy.h"
#include "BowArrowFigSelfdefStrategy.h"

class RabbiSelfdefenceStragegy :
	public IRabbiStrategy
{
public:
	RabbiSelfdefenceStragegy();
public:
	virtual ~RabbiSelfdefenceStragegy(void);

	// ����������ִ������
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @param strategyMgr ���Թ�����
	// @return ����true / false �� true: �ܹ���  false��������
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjParam, IManager *strategyMgr, bool isEven);

private:
	BowArrowFigSelfdefStrategy bowArrowFigSelfdefStrategy;
};
#endif
