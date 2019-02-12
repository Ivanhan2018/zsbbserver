// @brief ��Ҫ����: ��������������
// @author ����: �� �� ��

#include "BowArrowFigSelfdefStrategy.h"
#include "SeekPathArithm.h"
#include "AppetentAction.h"
#include "PatrolFunSingleton.h"

BowArrowFigSelfdefStrategy::BowArrowFigSelfdefStrategy()
{
}

BowArrowFigSelfdefStrategy::~BowArrowFigSelfdefStrategy(void)
{
}

bool BowArrowFigSelfdefStrategy::thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjParam, IManager *strategyMgr, bool isEven)
{
	if (NULL == strategyMgr)
	{
		return false;
	}

	managerParam &mgrParam = strategyMgr->getMgrParam();

	AiLevelParam *aiLevelParam = mgrParam.aiLevelParam;   // AI�ȼ�����
	int randomVal = 0;  // �����ֵ

	if (NULL == aiLevelParam)
	{
#ifdef _DEBUG
		assert(0 && "AI�ȼ������Ľṹָ��ֵΪ��");
#endif
		return true;
	}

	// ��ȡ������������õ���ͣ���ʱȽ�
	randomVal = CommonFunctionSingleton::getInstance()->GetRandomValFromOneToHundred();

	if (randomVal <= aiLevelParam->pauseProbability)
	{
		return true;
	}

	// ��ƽ״̬�£�ֻ��ִ��Ѳ��
	if (war != armyObjParam->getMapState()  && Counterpart_AI != mgrParam.useAiScene )
	{
		if (isEven)
		{
			return true;
		}

		if (!mgrParam.PatrolPosVectPtr->empty())
		{
			bool isSucess = PatrolFunSingleton::getInstance()->executePatrol(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam, strategyMgr, isEven);

			if (!isSucess)
			{
				return false;
			}
		}

		return true;
	}


	// ������һ���Լ�������Ŀ��	
 	IArmyPtr LastAttackTarget = armyObjParam->GetLastAttackTarget();

	// ��ȡ���һ�ι����Լ���Ŀ��
	IArmyPtr LastAttackedSelf = armyObjParam->GetLastEnmityTarget();

	// ��ȡ���һ���Լ������Ĳ��ӵ�HP
	int HpValOfAttackTarget = 0;

	if (LastAttackTarget)
	{	
		HpValOfAttackTarget = LastAttackTarget->GetCurrentHp();
	}

	int HpValOfAttackedSelf = 0;
	
	if (LastAttackedSelf)
	{
		HpValOfAttackedSelf = LastAttackedSelf->GetCurrentHp();
	}

	// ��ȡ����ڵĵо�
	IArmyPtr EnemyInGunshotPtr  = CommonFunctionSingleton::getInstance()->getGunshotEnemyToAttack(mapDataParamPtr, armyObjParam, aiLevelParam->tipTopAttackTar);

	// �����ǰ���������ǽ��������������ео�����ֱ�ӹ���
	//if (BUILD == armyObjParam->GetArmyType())
	{
		if (NULL != EnemyInGunshotPtr)
		{
			if (!isEven)
			{
				return true;
			}

			AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam, mgrParam.cellHasArmyMax);
			AppetentActionObj.attEnemyInAttScope(EnemyInGunshotPtr, armyObjParam, false);	
			
			return true;
		}
	}
	
	if (isEven)
	{
		return true;
	}

	// ������޵о�������Ұ��ѡ�񹥻�
	if (NULL == EnemyInGunshotPtr)
	{
		// ��ȡ��Ұ��Χ�ڵ�Ҫ�����ĵо�
		bool isSucess = CommonFunctionSingleton::getInstance()
			->getApproEnemyToAttackOnAimRule(mapDataParamPtr, armyObjParam, armyObjParam->GetViewScope(),  EnemyInGunshotPtr, aiLevelParam->tipTopAttackTar);

		if (!isSucess)
		{
			return false;
		}
	}

	// �Լ������������Լ�����ڵĲ��ӣ�������
	if (0 == HpValOfAttackTarget)
	{
		return thinkingInOffset(mapDataParamPtr, seekPathArithmParamPtr, LastAttackedSelf, EnemyInGunshotPtr, armyObjParam, strategyMgr, isEven);
	}
	// δ������
	else
	{
		// ����������Ĳ�����������(������⣬������Ұ��),��׷��Ŀ��
		if (!isAttackArmyIsOutruning(armyObjParam, LastAttackTarget))
		{
			mgrParam.enemyID = LastAttackTarget->GetArmyID();
		}

		return thinkingInOffset(mapDataParamPtr, seekPathArithmParamPtr, LastAttackedSelf, EnemyInGunshotPtr, armyObjParam, strategyMgr, isEven);	
	}
}

bool BowArrowFigSelfdefStrategy::thinkingInOffset(IArmyPtrvectInvect * mapDataParamPtr, 
												  SeekPath * seekPathArithmParamPtr, 
												  const IArmyPtr LastAttackedSelf, 
												  const IArmyPtr EnemyInGunshotPtr, 
												  IArmyPtr  armyObjParam, 
												  IManager *strategyMgr,
												  bool isEven) const
{
	if (NULL == strategyMgr)
	{
		return false;
	}
	
	managerParam &mgrParam = strategyMgr->getMgrParam();

	// ���������ֳ����˵о����򹥻�
	if (NULL != EnemyInGunshotPtr)
	{
		FunParam funParam(mapDataParamPtr, 
							seekPathArithmParamPtr, 
							mgrParam.armyCurrentStatus, 
							mgrParam.movePosVector, 
							mgrParam.moveTagetPos,
							mgrParam.enemyID,
							armyObjParam,
							strategyMgr);

		attackFunParam attOrMoveFunParm(funParam, EnemyInGunshotPtr->GetArmyID());

		CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);

		AiLevelParam *aiLevelParam = mgrParam.aiLevelParam;

		if (NULL == aiLevelParam)
		{
#ifdef _DEBUG
			assert(0);
#endif
			return false;
		}

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector,mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath);

		return true;
	}

	// �Լ�û�б������ĵо�����,��תΪ����״̬
	if (NULL != LastAttackedSelf)
	{	
		FunParam funParam(mapDataParamPtr, 
							seekPathArithmParamPtr, 
							mgrParam.armyCurrentStatus, 
							mgrParam.movePosVector, 
							mgrParam.moveTagetPos,
							mgrParam.enemyID,
							armyObjParam,
							strategyMgr);

		attackFunParam attOrMoveFunParm(funParam, LastAttackedSelf->GetArmyID());

		CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);

		AiLevelParam *aiLevelParam = mgrParam.aiLevelParam;

		if (NULL == aiLevelParam)
		{
#ifdef _DEBUG
			assert(0);
#endif
			return false;
		}

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath);
	
		return true;
	}

	// ����,������ҪѲ��
	if (!mgrParam.PatrolPosVectPtr->empty())
	{
		bool isSucess = PatrolFunSingleton::getInstance()->executePatrol(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam, strategyMgr, isEven);

		if (!isSucess)
		{
			return false;
		}
	}

	/*
	if (mgrParam.calledAttackEnemyID && armyObjParam->getArmyByID(mgrParam.calledAttackEnemyID) &&  0 != (armyObjParam->getArmyByID(mgrParam.calledAttackEnemyID))->GetCurrentHp())
	{
		FunParam funParam(mapDataParamPtr, 
						seekPathArithmParamPtr, 
						mgrParam.armyCurrentStatus, 
						mgrParam.movePosVector, 
						mgrParam.moveTagetPos,
						mgrParam.enemyID,
						armyObjParam,
						strategyMgr);

		attackFunParam attOrMoveFunParm(funParam, mgrParam.calledAttackEnemyID);

		CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);

		AiLevelParam *aiLevelParam =mgrParam.aiLevelParam;

		if (NULL == aiLevelParam)
		{
#ifdef _DEBUG
			assert(0);
#endif
			return false;
		}

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObjParam);
		AppetentActionObj.action(mgrParam.enemyID, mgrParam.movePosVector,mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath);
	}
*/
	return true;
}

bool BowArrowFigSelfdefStrategy::isAttackArmyIsOutruning(const IArmyPtr armyObjParam, const IArmyPtr LastAttackTarget) const
{
	if (!LastAttackTarget)
	{
		return false;
	}

	int attackScopeVal = armyObjParam->GetAttackScope();
	int viewScopeVal = armyObjParam->GetViewScope();
	
	PosXYStru posStart, posEnd;

	int sizeVal  = 0;

	const std::vector<POS_STRUCT> &armyPosVector = armyObjParam->GetArmyPos();

	if (armyPosVector.empty())
	{
#ifdef _DEBUG
		assert(0 && "��ȡ��������ʧ��");
#endif
	}

	posStart.xPos = (armyPosVector[0]).PosX;
	posStart.yPos = (armyPosVector[0]).PosY;

	const std::vector<POS_STRUCT> &enemyPosVector = LastAttackTarget->GetArmyPos();
	
	if (enemyPosVector.empty())
	{
#ifdef _DEBUG
		assert(0 && "��ȡ��������ʧ��");
#endif
	}

	posEnd.xPos = (enemyPosVector[0]).PosX;
	posEnd.yPos = (enemyPosVector[0]).PosY;


	sizeVal = getDistanceTwoNode(posStart.xPos, posStart.yPos, posEnd.xPos, posEnd.yPos);

	if (sizeVal > attackScopeVal && sizeVal <= viewScopeVal)
	{
		return true;
	}
	else
	{
		return false;
	}
}