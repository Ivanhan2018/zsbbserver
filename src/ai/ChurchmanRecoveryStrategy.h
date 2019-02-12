// @brief ��Ҫ����: ��ʦ��������
// @author ����: �� �� ��

#ifndef _CHURCHMANRECOVERYSTRATEGY_H_H
#define _CHURCHMANRECOVERYSTRATEGY_H_H

#include "ichurchmanstrategy.h"

class ChurchmanRecoveryStrategy : public IChurchmanStrategy
{
public:
	ChurchmanRecoveryStrategy();
public:
	virtual ~ChurchmanRecoveryStrategy(void);

	// ����������ִ������
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @param strategyMgr ���Թ�����
	// @return ����true / false �� true: �ܹ���  false��������
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr,  SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven);

private:
	bool getFollowTarAndFollow( IArmyPtrvectInvect * mapDataParamPtr, IArmyPtr armyObj, IManager * strategyMgr, SeekPath * seekPathArithmParamPtr, bool isEven) const;

	bool followTar( IArmyPtr followObjPtr, IArmyPtr armyObj, IManager * strategyMgr, IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr,  bool isEven) const;
};

#endif
