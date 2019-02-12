// @brief 简要描述: 弓箭手进攻策略
// @author 作者: 黄 文 才

#ifndef _BOWARROWFIGATTACKSTRATEGY_H_H
#define _BOWARROWFIGATTACKSTRATEGY_H_H

#include "ibowarrowfigstragegy.h"

class BowArrowFigAttackStrategy :public IBowArrowFigStrategy
{
public:
	BowArrowFigAttackStrategy();
public:
	virtual ~BowArrowFigAttackStrategy(void);

	// 函数描述：执行命令
	// @param mapDataPtr 部队地图数据指针
	// @param seekPathArithmParamPtr 寻路对象的指针
	// @param armyObj 当前操作的部队
	// @param strategyMgr 策略管理器
	// @return 返回true / false ； true: 能够走  false：不能走
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr,  IArmyPtr armyObj, IManager *strategyMgr, bool isEven);

private:
	bool callReinforce( std::list<IArmyPtr> &lstFriendGroup, bool &isSuceess, IArmyPtrvectInvect * mapDataParamPtr, IArmy * armyObj, AiLevelParam * aiLevelParamTmp, bool isSucess, std::list<IArmyPtr>::iterator &itBeg, std::list<IArmyPtr>::iterator &itEnd, const IArmyPtr enemyPtr) const;
	
	void backpedalToBirthPos(IArmy * armyObj, IManager * strategyMgr, IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, bool isEven);

	bool moveToNeedAttArmy( IArmyPtrvectInvect * mapDataParamPtr, IArmy * armyObj, SeekPath * seekPathArithmParamPtr, IManager * strategyMgr, bool isEven);

	unsigned int moveTotargetEnemyID;
};

#endif
