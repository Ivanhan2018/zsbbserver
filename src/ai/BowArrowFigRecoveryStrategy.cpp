// @brief ��Ҫ����: �����ַ�������
// @author ����: �� �� ��

#include "BowArrowFigRecoveryStrategy.h"
#include "ArmyAction.h"
#include "AppetentAction.h"
#include "PatrolFunSingleton.h"
#include "CommonFun.h"

namespace
{
	enum
	{
		TWO_NUM = 2,
		THIRTY = 30
	};
}

BowArrowFigRecoveryStrategy::BowArrowFigRecoveryStrategy(void)
{
}

BowArrowFigRecoveryStrategy::~BowArrowFigRecoveryStrategy(void)
{
}

bool BowArrowFigRecoveryStrategy::thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven)
{
	if (NULL == strategyMgr)
	{
		return false;
	}

	managerParam &mgrParam = strategyMgr->getMgrParam();

	bool isAttack = false;  // ��ʼ��Ϊ������
	bool isSucess = true;   
	bool isSatisfyConOfCountermarch = false;
	IArmyPtr armyPtrTmp = NULL;

	AiLevelParam *aiLevelParam = mgrParam.aiLevelParam;   // AI�ȼ�����
	int randomVal = 0;  // �����ֵ

	if (NULL == aiLevelParam)
	{
#ifdef _DEBUG
		assert(0 && "AI�ȼ������Ľṹָ��ֵΪ��");
#endif
		return false;
	}

	std::list<IArmyPtr> lstFriendGroup;

	// ��ȡ������������õ���ͣ���ʱȽ�
	randomVal = CommonFunctionSingleton::getInstance()->GetRandomValFromOneToHundred();

	if (randomVal <= aiLevelParam->pauseProbability)
	{
		return true;
	}

	// ��ƽ״̬�£�ֻ��ִ��Ѳ��
	if (war != armyObj->getMapState()  && Counterpart_AI != mgrParam.useAiScene )
	{
		if (isEven)
		{
			return true;
		}

		if (!mgrParam.PatrolPosVectPtr->empty())
		{
			isSucess = PatrolFunSingleton::getInstance()->executePatrol(mapDataParamPtr, seekPathArithmParamPtr, armyObj, strategyMgr, isEven);

			if (!isSucess)
			{
				return false;
			}
		}

		return true;
	}

	unsigned int attackScope = armyObj->GetAttackScope();

	// ��Ԯ��������
	if (1 == aiLevelParam->isCanCalled && 0 != mgrParam.calledAttackEnemyID )
	{
		armyPtrTmp = armyObj->getArmyByID(mgrParam.calledAttackEnemyID);
	}
	
	if (NULL != armyPtrTmp &&  0 != armyPtrTmp->GetCurrentHp())
	{
		mgrParam.PatrolStruct.isPatrol = false;

		FunParam funParam(mapDataParamPtr, 
							seekPathArithmParamPtr, 
							mgrParam.armyCurrentStatus, 
							mgrParam.movePosVector, 
							mgrParam.moveTagetPos,
							mgrParam.enemyID,
							armyObj,
							strategyMgr);

		attackFunParam attOrMoveFunParm(funParam, mgrParam.calledAttackEnemyID);

		//CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);

		const std::vector<POS_STRUCT> &enemyPosVect = armyPtrTmp->GetArmyPos();
		const std::vector<POS_STRUCT> &armyPosVect = armyObj->GetArmyPos();

		if (!CommonFunctionSingleton::getInstance()->enemyIsInAttackScope(armyPosVect, enemyPosVect, attackScope))
		{
			if (isEven)
			{
				return true;
			}

			CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);
		}
		else
		{
			FunParam &funParam = attOrMoveFunParm.funParam;
			funParam.armyCurrentStatus = Attack_status;	
			funParam.enemyID = mgrParam.calledAttackEnemyID;
		}

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath);

		return true;
	}

	// �����ִ�к�������Ļ�, ����ִ�к�������
	if (mgrParam.isCountermarch)
	{
		if (isEven)
		{
			return true;
		}

		mgrParam.PatrolStruct.isPatrol = false;

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath, mgrParam.isCountermarch);	
		
		return true;
	}

	// ��ȡ�ϴι����ĵо�����ָ��
	IArmyPtr enemyPtr = armyObj->getArmyByID(mgrParam.enemyID);
	
	// ��ȡ���ӵ���Ұ
	int viewScope = armyObj->GetViewScope();

	// �ϴι����ĵо�û����
	if (NULL != enemyPtr && 0 != enemyPtr->GetCurrentHp())
	{
		const std::vector<POS_STRUCT> &enemyPosVect = enemyPtr->GetArmyPos();
		const std::vector<POS_STRUCT> &armyPosVect = armyObj->GetArmyPos();

		if (enemyPosVect.empty() || armyPosVect.empty())
		{
			return false;
		}

		if (!CommonFunctionSingleton::getInstance()->enemyIsInAttackScope(armyPosVect, enemyPosVect, attackScope))
		{
			if (isEven)
			{
				return true;
			}
		}

		// ���һ�����Ұ�ڣ��������������л�Ŀ��
		if (getDistanceTwoNode(armyPosVect[0].PosX, armyPosVect[0].PosY, enemyPosVect[0].PosX, enemyPosVect[0].PosY) <= viewScope)
		{
			mgrParam.PatrolStruct.isPatrol = false;

			FunParam funParam(mapDataParamPtr, 
				seekPathArithmParamPtr, 
				mgrParam.armyCurrentStatus, 
				mgrParam.movePosVector, 
				mgrParam.moveTagetPos,
				mgrParam.enemyID,
				armyObj,
				strategyMgr);

			attackFunParam attOrMoveFunParm(funParam, mgrParam.enemyID);

			CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);

			AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
			AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath);

			return true;
		}
	}

	// ��ȡ����ڹ����ĵо�
	IArmyPtr EnemyInGunshotPtr = CommonFunctionSingleton::getInstance()->getGunshotEnemyToAttack(mapDataParamPtr, armyObj, aiLevelParam->tipTopAttackTar);

	// ����ڳ��ֵо����򹥻�
	if (NULL != EnemyInGunshotPtr)
	{
		if (!isEven)
		{
			return true;
		}

		mgrParam.PatrolStruct.isPatrol = false;

		FunParam funParam(mapDataParamPtr, 
							seekPathArithmParamPtr, 
							mgrParam.armyCurrentStatus, 
							mgrParam.movePosVector, 
							mgrParam.moveTagetPos,
							mgrParam.enemyID,
							armyObj,
							strategyMgr);

		attackFunParam attOrMoveFunParm(funParam, EnemyInGunshotPtr->GetArmyID());

		CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath);

		return true;
	}

	if (isEven)
	{
		return true;
	}

	// ��ȡ��Ұ��Χ�ڵ�Ҫ�����ĵо������ж��Ƿ��ȡ����
	isSucess = CommonFunctionSingleton::getInstance()
		->getAttackArmy(mapDataParamPtr, armyObj, aiLevelParam->countRange,  EnemyInGunshotPtr, isAttack, lstFriendGroup, aiLevelParam->tipTopAttackTar);

	if (!isSucess)
	{
		return false;
	}

	// ������Ұ�ڵĵо�
	if (NULL != EnemyInGunshotPtr && isAttack)
	{
		mgrParam.PatrolStruct.isPatrol = false;

		FunParam funParam(mapDataParamPtr, 
						seekPathArithmParamPtr, 
						mgrParam.armyCurrentStatus, 
						mgrParam.movePosVector, 
						mgrParam.moveTagetPos,
						mgrParam.enemyID,
						armyObj,
						strategyMgr);

		attackFunParam attOrMoveFunParm(funParam, EnemyInGunshotPtr->GetArmyID());

		CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);


		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath);

		// ������Ԯ
		return callFriendArmyToReinforce(lstFriendGroup, isSucess, mapDataParamPtr, armyObj, strategyMgr, armyPtrTmp, EnemyInGunshotPtr);

	}

	// �Ƿ������������
	isSatisfyConOfCountermarch = isSatisfyCondiOfCountermarch(mapDataParamPtr, armyObj);

	// ѡ����ˣ����������������������NPC������
	if (Counterpart_AI != mgrParam.useAiScene
		     && !isAttack && isSatisfyConOfCountermarch && NULL != EnemyInGunshotPtr)
	{
		PosXYStru posXY;

		CommonFunctionSingleton::getInstance()->GetPosOfCountermarch(mapDataParamPtr, mgrParam.ClassImpedientPtr, armyObj, posXY);

		FunParam funParam(mapDataParamPtr, 
							seekPathArithmParamPtr, 
							mgrParam.armyCurrentStatus, 
							mgrParam.movePosVector, 
							mgrParam.moveTagetPos,
							mgrParam.enemyID,
							armyObj,
							strategyMgr);

		moveFunParam attOrMoveFunParm(funParam, posXY);

		CommonFunctionSingleton::getInstance()->setMoveTargetPos(attOrMoveFunParm);

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector,mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath);

		return true;
	}

	if (!mgrParam.PatrolPosVectPtr->empty())
	{
		isSucess = PatrolFunSingleton::getInstance()->executePatrol(mapDataParamPtr, seekPathArithmParamPtr, armyObj, strategyMgr, isEven);
		
		if (!isSucess)
		{
			return false;
		}

		return true;
		
	}
	// ѡ���˺��ˣ����ֲ��������������ѡ�񹥻�������һ��
	if (NULL != EnemyInGunshotPtr && !isAttack && !isSatisfyConOfCountermarch && NULL != EnemyInGunshotPtr)
	{
		mgrParam.PatrolStruct.isPatrol = false;

		FunParam funParam(mapDataParamPtr, 
						seekPathArithmParamPtr, 
						mgrParam.armyCurrentStatus, 
						mgrParam.movePosVector, 
						mgrParam.moveTagetPos,
						mgrParam.enemyID,
						armyObj,
						strategyMgr);

		attackFunParam attOrMoveFunParm(funParam, EnemyInGunshotPtr->GetArmyID());

		CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath);
	}

	return true;
}

bool BowArrowFigRecoveryStrategy::isSatisfyCondiOfCountermarch(IArmyPtrvectInvect * mapDataParamPtr, IArmyPtr armyObj) const
{
	listArmyPtr lstFriendArmy;
	listArmyPtr lstEnemy;
	int countCellsHasEnemy = 0; // ͳ�����ڸ������ео��ĸ���

	// ��ȡָ�����ڸ��ӵĲ��ӵ�list
	bool isSucess = CommonFunctionSingleton::getInstance()->getSpecifyDistanceEnemy(mapDataParamPtr, armyObj, AI_ONE_VALUE_NUM, false, false, lstFriendArmy, lstEnemy, countCellsHasEnemy);

	if (!isSucess)
	{
#ifdef _DEBUG
		assert(0 && "���û�ȡָ�����ڵĲ��ӵ�listʧ��");
#endif
		return false;
	}

	if (countCellsHasEnemy > TWO_NUM || armyObj->getSoldierNum() < THIRTY)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool BowArrowFigRecoveryStrategy::callFriendArmyToReinforce( std::list<IArmyPtr> &lstFriendGroup, bool &isSucess, IArmyPtrvectInvect * mapDataParamPtr, IArmyPtr armyObj, IManager * strategyMgr,  IArmyPtr &armyPtrTmp, const IArmyPtr EnemyInGunshotPtr ) const
{
	if (NULL == strategyMgr)
	{
		return false;
	}

	managerParam &mgrParam = strategyMgr->getMgrParam();

	AiLevelParam *aiLevelParamTmp = mgrParam.aiLevelParam;

	if (NULL == aiLevelParamTmp)
	{
#ifdef _DEBUG
		assert(0);
#endif
		return false;
	}

	lstFriendGroup.clear();
	// Ѱ�����Լ�Ϊ���ĵ������Ѿ�(���йܲ��ӣ���NPC���ӣ�)
	isSucess = CommonFunctionSingleton::getInstance()->getGroupArmy(mapDataParamPtr, armyObj, aiLevelParamTmp->callRangeVal, lstFriendGroup);

	if (!isSucess)
	{
		return false;
	}

	std::list<IArmyPtr>::iterator itBeg, itEnd;
	
	// �����Ѿ���Ԯ
	itBeg = lstFriendGroup.begin();
	itEnd = lstFriendGroup.end();

	for (; itBeg!=itEnd; ++itBeg)
	{

		if (*itBeg == armyObj)
		{
#ifdef _DEBUG
			assert(0 && "�����Լ���");
#endif
			continue;
		}


		if (BUILD == (*itBeg)->GetArmyType())
		{
			continue;
		}

		IManager * strategyMgrTmp = (*itBeg)->getStrategyMgr();

		if (NULL == strategyMgrTmp)
		{
			//#ifdef _DEBUG
			//				assert(0 && "��ȡ���Թ�����ָ��ʧ��");
			//#endif
			return false;
		}

		armyPtrTmp = NULL;

		managerParam &mgrParamTmp = strategyMgr->getMgrParam();

		if (0 != mgrParamTmp.calledAttackEnemyID)
		{
			armyPtrTmp = armyObj->getArmyByID(mgrParamTmp.calledAttackEnemyID);
		}

		// ������Ѿ�û�б����й������Ҫ�����ĵо�������
		if (0 == mgrParamTmp.calledAttackEnemyID
			|| (NULL != armyPtrTmp && 0 == armyPtrTmp->GetCurrentHp()))
		{
			mgrParamTmp.calledAttackEnemyID = EnemyInGunshotPtr->GetArmyID();				
		}
	}

	return true;
}