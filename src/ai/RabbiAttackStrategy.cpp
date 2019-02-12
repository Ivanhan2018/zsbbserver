// @brief 简要描述: 法师攻击策略
// @author 作者: 黄 文 才

#include "RabbiAttackStrategy.h"
#include "AppetentAction.h"

RabbiAttackStrategy::RabbiAttackStrategy()
{
}

RabbiAttackStrategy::~RabbiAttackStrategy(void)
{
}

bool RabbiAttackStrategy::thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjParam, IManager *strategyMgr, bool isEven)
{
	/*
	bool isAttackOldTarget = false;

	// 获取部队的射程
	int attackScope = armyObjParam->GetAttackScope();

	// 获取上次攻击的敌军部队指针
	IArmyPtr enemyPtr = armyObjParam->getArmyByID(strategyMgr->enemyID);


	// 如果是执行后退命令的话，继续执行后退命令
	if (strategyMgr->isCountermarch)
	{
		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam);
		AppetentActionObj.action(strategyMgr->enemyID, strategyMgr->moveTagetPos, strategyMgr->movePosVector, strategyMgr->armyCurrentStatus, strategyMgr->isCountermarch);	
		return true;
	}

	// 上次攻击的敌军没有死
	if (NULL != enemyPtr && 0 != enemyPtr->GetCurrentHp())
	{
		const std::vector<POS_STRUCT> &enemyPosVect = enemyPtr->GetArmyPos();
		const std::vector<POS_STRUCT> &armyPosVect = armyObjParam->GetArmyPos();

		if (enemyPosVect.empty() || armyPosVect.empty())
		{
			return false;
		}

		// 并且还在射程内，继续攻击，不切换目标
		if (getDistanceTwoNode(armyPosVect[0].PosX, armyPosVect[0].PosY, enemyPosVect[0].PosX, enemyPosVect[0].PosY) <= attackScope)
		{
			isAttackOldTarget = true;
		}
	}

	// 攻击上次攻击的目标
	if (isAttackOldTarget)
	{
		CommonFunctionSingleton::getInstance()->setAttackTarget(seekPathArithmParamPtr,
			strategyMgr->armyCurrentStatus,
			strategyMgr->movePosVector,
			strategyMgr->moveTagetPos,
			strategyMgr->enemyID,
			armyObjParam,
			strategyMgr,
			strategyMgr->enemyID);

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam);
		AppetentActionObj.action(strategyMgr->enemyID, strategyMgr->moveTagetPos, strategyMgr->movePosVector, strategyMgr->armyCurrentStatus);

		return true;
	}

	// 看射程内是否有敌军需要攻击的
 	enemyPtr = CommonFunctionSingleton::getInstance()->getGunshotEnemyToAttack(mapDataParamPtr, armyObjParam);
	IArmyPtr armyPtrTmp = NULL;
	
	if (NULL != enemyPtr)
	{
		CommonFunctionSingleton::getInstance()->setAttackTarget(seekPathArithmParamPtr,
																				strategyMgr->armyCurrentStatus,
																				strategyMgr->movePosVector,
																				strategyMgr->moveTagetPos,
																				strategyMgr->enemyID,
																				armyObjParam,
																				strategyMgr,
																				enemyPtr->GetArmyID());

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam);
		AppetentActionObj.action(strategyMgr->enemyID, strategyMgr->moveTagetPos, strategyMgr->movePosVector, strategyMgr->armyCurrentStatus);
	}
	// 需要增援其他友军
	else if (0 != strategyMgr->calledAttackEnemyID && (armyPtrTmp= armyObjParam->getArmyByID(strategyMgr->calledAttackEnemyID)) &&  0 != armyPtrTmp->GetCurrentHp())
	{
		CommonFunctionSingleton::getInstance()->setAttackTarget(seekPathArithmParamPtr,
																				strategyMgr->armyCurrentStatus,
																				strategyMgr->movePosVector,
																				strategyMgr->moveTagetPos,
																				strategyMgr->enemyID,
																				armyObjParam,
																				strategyMgr,
																				strategyMgr->calledAttackEnemyID);
		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam);
		AppetentActionObj.action(strategyMgr->enemyID, strategyMgr->moveTagetPos, strategyMgr->movePosVector, strategyMgr->armyCurrentStatus);

	}

	// 攻击之后，在处理跟随
	IArmyPtr followObjPtr = armyObjParam->GetFollowTarget();

	// 如果没有跟随目标或原跟随对象已死亡
	if (NULL == followObjPtr || 0 == followObjPtr->GetCurrentHp())
	{
		followObjPtr = NULL;

		bool isSucess = CommonFunctionSingleton::getInstance()->getFollowObjPtr(mapDataParamPtr, armyObjParam, armyObjParam->GetViewScope(), followObjPtr);

		// 没有合适的跟随对象，选择后退到着陆点
		if (NULL == followObjPtr)
		{
			strategyMgr->isCountermarch = true;

			POS_STRUCT posXY=armyObjParam->GetBirthPos();

			PosXYStru posTarget;
			posTarget.xPos = posXY.PosX;
			posTarget.yPos = posXY.PosY;

			CommonFunctionSingleton::getInstance()->setMoveTargetPos(seekPathArithmParamPtr,
				strategyMgr->armyCurrentStatus,
				strategyMgr->movePosVector,
				strategyMgr->moveTagetPos,
				strategyMgr->enemyID,
				armyObjParam,
				strategyMgr,
				posTarget);

			AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam);
			AppetentActionObj.action(strategyMgr->enemyID, strategyMgr->moveTagetPos, strategyMgr->movePosVector, strategyMgr->armyCurrentStatus);

			return true;
		}
		// 否则跟随选中的跟随对象
		else
		{
			// 设置跟随对象
			armyObjParam->SetFollowTarget(followObjPtr);

			// 获取跟随部队的坐标
			const vector<POS_STRUCT> &vectorPosTaraget = followObjPtr->GetArmyPos();

			const vector<POS_STRUCT> &vectorPosArmy = armyObjParam->GetArmyPos();

			int distantVal = 0;

			PosXYStru posTarget, posArmy;
			posTarget.xPos = vectorPosTaraget[0].PosX;
			posTarget.yPos = vectorPosTaraget[0].PosY;

			posArmy.xPos = vectorPosArmy[0].PosX;
			posArmy.yPos = vectorPosArmy[0].PosY;

			distantVal = getDistanceTwoNode(posArmy.xPos, posArmy.yPos, posTarget.xPos, posTarget.yPos);

			if (distantVal <= ONE_CIRCLE)
			{
				return true;
			}
			else
			{
				CommonFunctionSingleton::getInstance()->setMoveTargetPos(seekPathArithmParamPtr,
					strategyMgr->armyCurrentStatus,
					strategyMgr->movePosVector,
					strategyMgr->moveTagetPos,
					strategyMgr->enemyID,
					armyObjParam,
					strategyMgr,
					posTarget);

				AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam);
				AppetentActionObj.action(strategyMgr->enemyID, strategyMgr->moveTagetPos, strategyMgr->movePosVector, strategyMgr->armyCurrentStatus);
			}
		}
	}
	// 否则，跟随指定的目标
	else
	{
		const vector<POS_STRUCT> &vectorPosTaraget = followObjPtr->GetArmyPos();
		const vector<POS_STRUCT> &vectorPosArmy = armyObjParam->GetArmyPos();
		int attackScope = armyObjParam->GetAttackScope();
		int distantVal = 0;

		PosXYStru posTarget, posArmy;
		posTarget.xPos = vectorPosTaraget[0].PosX;
		posTarget.yPos = vectorPosTaraget[0].PosY;

		posArmy.xPos = vectorPosArmy[0].PosX;
		posArmy.yPos = vectorPosArmy[0].PosY;

		distantVal = getDistanceTwoNode(posArmy.xPos, posArmy.yPos, posTarget.xPos, posTarget.yPos);

		if (distantVal <= ONE_CIRCLE)
		{
			return true;
		}
		else
		{
			CommonFunctionSingleton::getInstance()->setMoveTargetPos(seekPathArithmParamPtr,
				strategyMgr->armyCurrentStatus,
				strategyMgr->movePosVector,
				strategyMgr->moveTagetPos,
				strategyMgr->enemyID,
				armyObjParam,
				strategyMgr,
				posTarget);

			AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam);
			AppetentActionObj.action(strategyMgr->enemyID, strategyMgr->moveTagetPos, strategyMgr->movePosVector, strategyMgr->armyCurrentStatus);
		}
	}

	return true;
	*/

	return bowArrowFigAttackStrategy.thinking(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam, strategyMgr, isEven);
}


