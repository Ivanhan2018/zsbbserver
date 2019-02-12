// @brief ��Ҫ����: ��ʦ��������
// @author ����: �� �� ��

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

	// ��ȡ���ӵ����
	int attackScope = armyObjParam->GetAttackScope();

	// ��ȡ�ϴι����ĵо�����ָ��
	IArmyPtr enemyPtr = armyObjParam->getArmyByID(strategyMgr->enemyID);


	// �����ִ�к�������Ļ�������ִ�к�������
	if (strategyMgr->isCountermarch)
	{
		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam);
		AppetentActionObj.action(strategyMgr->enemyID, strategyMgr->moveTagetPos, strategyMgr->movePosVector, strategyMgr->armyCurrentStatus, strategyMgr->isCountermarch);	
		return true;
	}

	// �ϴι����ĵо�û����
	if (NULL != enemyPtr && 0 != enemyPtr->GetCurrentHp())
	{
		const std::vector<POS_STRUCT> &enemyPosVect = enemyPtr->GetArmyPos();
		const std::vector<POS_STRUCT> &armyPosVect = armyObjParam->GetArmyPos();

		if (enemyPosVect.empty() || armyPosVect.empty())
		{
			return false;
		}

		// ���һ�������ڣ��������������л�Ŀ��
		if (getDistanceTwoNode(armyPosVect[0].PosX, armyPosVect[0].PosY, enemyPosVect[0].PosX, enemyPosVect[0].PosY) <= attackScope)
		{
			isAttackOldTarget = true;
		}
	}

	// �����ϴι�����Ŀ��
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

	// ��������Ƿ��ео���Ҫ������
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
	// ��Ҫ��Ԯ�����Ѿ�
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

	// ����֮���ڴ������
	IArmyPtr followObjPtr = armyObjParam->GetFollowTarget();

	// ���û�и���Ŀ���ԭ�������������
	if (NULL == followObjPtr || 0 == followObjPtr->GetCurrentHp())
	{
		followObjPtr = NULL;

		bool isSucess = CommonFunctionSingleton::getInstance()->getFollowObjPtr(mapDataParamPtr, armyObjParam, armyObjParam->GetViewScope(), followObjPtr);

		// û�к��ʵĸ������ѡ����˵���½��
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
		// �������ѡ�еĸ������
		else
		{
			// ���ø������
			armyObjParam->SetFollowTarget(followObjPtr);

			// ��ȡ���沿�ӵ�����
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
	// ���򣬸���ָ����Ŀ��
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


