// @brief 简要描述: 法师驻守策略
// @author 作者: 黄 文 才

#include "RabbiDefendStrategy.h"
#include "AppetentAction.h"

RabbiDefendStrategy::RabbiDefendStrategy()
{
}

RabbiDefendStrategy::~RabbiDefendStrategy(void)
{
}

bool RabbiDefendStrategy::thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven)
{
	/*
	IArmyPtr enemyPtr = CommonFunctionSingleton::getInstance()->getGunshotEnemyToAttack(mapDataParamPtr, armyObj, (strategyMgr->aiLevelParam)->tipTopAttackTar);

	if (NULL != enemyPtr)
	{
		attackFunParam attOrMoveFunParm(mapDataParamPtr, 
											seekPathArithmParamPtr, 
											strategyMgr->armyCurrentStatus, 
											strategyMgr->movePosVector, 
											strategyMgr->moveTagetPos,
											strategyMgr->enemyID,
											armyObj,
											strategyMgr,
											enemyPtr->GetArmyID());

		CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj);
		AppetentActionObj.action(strategyMgr->enemyID, strategyMgr->movePosVector, strategyMgr->armyCurrentStatus, (strategyMgr->aiLevelParam)->tipTopAttackTar);
	}

	return true;
	*/
	return bowArrowFigDefStrategy.thinking(mapDataParamPtr, seekPathArithmParamPtr, armyObj, strategyMgr, isEven);
}
