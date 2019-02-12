// @brief ��Ҫ����: ��ʦ��������
// @author ����: �� �� ��
#ifndef _CHURCHMANATTACKSTRATEGY_H_H
#define _CHURCHMANATTACKSTRATEGY_H_H

#include "ichurchmanstrategy.h"

class ChurchmanAttackStrategy :public IChurchmanStrategy
{
public:
	ChurchmanAttackStrategy();
public:
	virtual ~ChurchmanAttackStrategy(void);

	// ����������ִ������
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @param strategyMgr ���Թ�����
	// @return ����true / false �� true: �ܹ���  false��������
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr  armyObj, IManager *strategyMgr, bool isEven);

private:
	bool followTarget( IArmyPtr followObjPtr, IArmyPtr armyObj, IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IManager * strategyMgr, bool isEven ) const;

	bool getFollowTarAndFollowOrBack( IArmyPtr &followObjPtr, IArmyPtrvectInvect * mapDataParamPtr, IArmyPtr armyObj, IManager * strategyMgr, SeekPath * seekPathArithmParamPtr, bool isEven);
};

#endif
