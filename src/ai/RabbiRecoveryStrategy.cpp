// @brief ��Ҫ����: ��ʦ��������
// @author ����: �� �� ��

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

	// ��ȡ���ӵ����
	int attackScope = armyObjParam->GetAttackScope();

	// ��ȡ�ϴι����ĵо�����ָ��
	IArmyPtr enemyPtr = armyObjParam->getArmyByID(strategyMgr->enemyID);

	// �����ִ�к�������Ļ�
	if (strategyMgr->isCountermarch)
	{
		strategyMgr->PatrolStruct.isPatrol = false;

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

	// �ж�������Ƿ��ео����Թ�����
	enemyPtr = CommonFunctionSingleton::getInstance()->getGunshotEnemyToAttack(mapDataParamPtr, armyObjParam);

	// �ȴ�����
	if (NULL != enemyPtr)
	{
		vector<IArmyPtr> armyVector;
		armyVector.push_back(enemyPtr);
		armyObjParam->Attack(armyVector);
	}

	// �������
	// ��ȡ�û����õĸ���Ŀ��
	IArmyPtr followObjPtr = armyObjParam->GetFollowTarget();
	IArmyPtr armyPtrTmp = NULL;

	// ����û�û�����õĸ���Ŀ��������õĸ���Ŀ������ʱ
	if (NULL == followObjPtr || 0 == followObjPtr->GetCurrentHp())
	{
		IArmyPtr followArmyObj = NULL;

		// �ж���Ұ���Ƿ���ڵ���ս���ӣ�սʿ-���֣�Ϊ�������
		bool isSucess = CommonFunctionSingleton::getInstance()->getFollowObjPtr(mapDataParamPtr, armyObjParam, armyObjParam->GetViewScope() , followArmyObj);

		// ��Ұ��û�к��ʵĸ������
		if (NULL == followArmyObj)
		{
			// ����,������ҪѲ��
			 if (!strategyMgr->PatrolPosVectPtr->empty())
			 {
				 isSucess = PatrolFunSingleton::getInstance()->executePatrol(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam, strategyMgr);

				 if (!isSucess)
				 {
					return false;
				 }
				
			 }
			 // ����ѡ�����, ����AI��ѡ�����
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
		// �������ѡ�еĸ������
		else
		{
			// ��ȡ���沿�ӵ�����
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

			// �������ò��Ӹ���Ŀ��
			armyObjParam->SetFollowTarget(followArmyObj);
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
