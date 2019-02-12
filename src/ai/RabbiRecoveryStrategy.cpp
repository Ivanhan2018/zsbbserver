// @brief 简要描述: 法师防御策略
// @author 作者: 黄 文 才

#include "RabbiRecoveryStrategy.h"
#include "ArmyAction.h"
#include "AppetentAction.h"
#include "PatrolFunSingleton.h"

RabbiRecoveryStrategy::RabbiRecoveryStrategy()
{
}

RabbiRecoveryStrategy::~RabbiRecoveryStrategy(void)
{
}

bool RabbiRecoveryStrategy::thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjParam, IManager *strategyMgr, bool isEven)
{
	/*
	bool isAttackOldTarget = false;

	// 获取部队的射程
	int attackScope = armyObjParam->GetAttackScope();

	// 获取上次攻击的敌军部队指针
	IArmyPtr enemyPtr = armyObjParam->getArmyByID(strategyMgr->enemyID);

	// 如果是执行后退命令的话
	if (strategyMgr->isCountermarch)
	{
		strategyMgr->PatrolStruct.isPatrol = false;

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
		strategyMgr->PatrolStruct.isPatrol = false;

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

	// 判断射程内是否有敌军可以攻击的
	enemyPtr = CommonFunctionSingleton::getInstance()->getGunshotEnemyToAttack(mapDataParamPtr, armyObjParam);

	// 先处理攻击
	if (NULL != enemyPtr)
	{
		vector<IArmyPtr> armyVector;
		armyVector.push_back(enemyPtr);
		armyObjParam->Attack(armyVector);
	}

	// 后处理跟随
	// 获取用户设置的跟随目标
	IArmyPtr followObjPtr = armyObjParam->GetFollowTarget();
	IArmyPtr armyPtrTmp = NULL;

	// 如果用户没有设置的跟随目标或者设置的跟随目标死亡时
	if (NULL == followObjPtr || 0 == followObjPtr->GetCurrentHp())
	{
		IArmyPtr followArmyObj = NULL;

		// 判断视野内是否存在的主战部队（战士-射手）为跟随对象
		bool isSucess = CommonFunctionSingleton::getInstance()->getFollowObjPtr(mapDataParamPtr, armyObjParam, armyObjParam->GetViewScope() , followArmyObj);

		// 视野内没有合适的跟随对象
		if (NULL == followArmyObj)
		{
			// 空闲,而且需要巡逻
			 if (!strategyMgr->PatrolPosVectPtr->empty())
			 {
				 isSucess = PatrolFunSingleton::getInstance()->executePatrol(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam, strategyMgr);

				 if (!isSucess)
				 {
					return false;
				 }
				
			 }
			 // 否则，选择后退, 副本AI不选择后退
			 else if (Counterpart_AI != strategyMgr->useAiScene)
			 {
				 PosXYStru posXY;
				 CommonFunctionSingleton::getInstance()->GetPosOfCountermarch(mapDataParamPtr, armyObjParam, posXY);

				 strategyMgr->PatrolStruct.isPatrol = false;

				 CommonFunctionSingleton::getInstance()->setMoveTargetPos(seekPathArithmParamPtr,
					 strategyMgr->armyCurrentStatus,
					 strategyMgr->movePosVector,
					 strategyMgr->moveTagetPos,
					 strategyMgr->enemyID,
					 armyObjParam,
					 strategyMgr,
					 posXY);

				 AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam);
				 AppetentActionObj.action(strategyMgr->enemyID, strategyMgr->moveTagetPos, strategyMgr->movePosVector, strategyMgr->armyCurrentStatus);
			 }
		}
		// 否则跟随选中的跟随对象
		else
		{
			// 获取跟随部队的坐标
			const vector<POS_STRUCT> &vectorPos = followArmyObj->GetArmyPos();

			PosXYStru posTarget;
			posTarget.xPos = vectorPos[0].PosX;
			posTarget.yPos = vectorPos[0].PosY;

			strategyMgr->PatrolStruct.isPatrol = false;

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

			// 重新设置部队跟随目标
			armyObjParam->SetFollowTarget(followArmyObj);
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

		if (distantVal <= attackScope)
		{
			return true;
		}
		else
		{
			strategyMgr->PatrolStruct.isPatrol = false;

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
	return bowArrowRecovery.thinking(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam, strategyMgr, isEven);
}
