// @brief 简要描述: 法师自卫策略
// @author 作者: 黄 文 才

#include "RabbiSelfdefenceStragegy.h"
#include "AppetentAction.h"

RabbiSelfdefenceStragegy::RabbiSelfdefenceStragegy()
{
}

RabbiSelfdefenceStragegy::~RabbiSelfdefenceStragegy(void)
{
}

bool RabbiSelfdefenceStragegy::thinking(IArmyPtrvectInvect * mapDataParamPtr,  SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjParam, IManager *strategyMgr, bool isEven)
{
	/*
	IArmyPtr enemy = CommonFunctionSingleton::getInstance()->getGunshotEnemyToAttack(mapDataParamPtr, armyObjParam, (strategyMgr->aiLevelParam)->tipTopAttackTar);

	if (NULL != enemy)
	{
		CommonFunctionSingleton::getInstance()->setAttackTarget(mapDataParamPtr,
																						seekPathArithmParamPtr,
																						strategyMgr->armyCurrentStatus,
																						strategyMgr->movePosVector,
																						strategyMgr->moveTagetPos,
																						strategyMgr->enemyID,
																						armyObjParam,
																						strategyMgr,
																						enemy->GetArmyID());

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam);
		AppetentActionObj.action(strategyMgr->enemyID, strategyMgr->movePosVector, strategyMgr->armyCurrentStatus);
	}
	else
	{
		if (strategyMgr->calledAttackEnemyID &&  armyObjParam->getArmyByID(strategyMgr->calledAttackEnemyID) && 0 != (armyObjParam->getArmyByID(strategyMgr->calledAttackEnemyID))->GetCurrentHp())
		{
			CommonFunctionSingleton::getInstance()->setAttackTarget(mapDataParamPtr,
																					seekPathArithmParamPtr,
																					strategyMgr->armyCurrentStatus,
																					strategyMgr->movePosVector,
																					strategyMgr->moveTagetPos,
																					strategyMgr->enemyID,
																					armyObjParam,
																					strategyMgr,
																					strategyMgr->calledAttackEnemyID);
			AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam);
			AppetentActionObj.action(strategyMgr->enemyID,  strategyMgr->movePosVector, strategyMgr->armyCurrentStatus);
		}
	}

	return true;

	*/
	return bowArrowFigSelfdefStrategy.thinking(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam, strategyMgr, isEven);
}
