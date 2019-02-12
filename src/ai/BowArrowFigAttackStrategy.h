// @brief ��Ҫ����: �����ֽ�������
// @author ����: �� �� ��

#ifndef _BOWARROWFIGATTACKSTRATEGY_H_H
#define _BOWARROWFIGATTACKSTRATEGY_H_H

#include "ibowarrowfigstragegy.h"

class BowArrowFigAttackStrategy :public IBowArrowFigStrategy
{
public:
	BowArrowFigAttackStrategy();
public:
	virtual ~BowArrowFigAttackStrategy(void);

	// ����������ִ������
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @param strategyMgr ���Թ�����
	// @return ����true / false �� true: �ܹ���  false��������
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr,  IArmyPtr armyObj, IManager *strategyMgr, bool isEven);

private:
	bool callReinforce( std::list<IArmyPtr> &lstFriendGroup, bool &isSuceess, IArmyPtrvectInvect * mapDataParamPtr, IArmy * armyObj, AiLevelParam * aiLevelParamTmp, bool isSucess, std::list<IArmyPtr>::iterator &itBeg, std::list<IArmyPtr>::iterator &itEnd, const IArmyPtr enemyPtr) const;
	
	void backpedalToBirthPos(IArmy * armyObj, IManager * strategyMgr, IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, bool isEven);

	bool moveToNeedAttArmy( IArmyPtrvectInvect * mapDataParamPtr, IArmy * armyObj, SeekPath * seekPathArithmParamPtr, IManager * strategyMgr, bool isEven);

	unsigned int moveTotargetEnemyID;
};

#endif
