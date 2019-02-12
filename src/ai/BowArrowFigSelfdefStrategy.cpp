// @brief 简要描述: 弓箭手自卫策略
// @author 作者: 黄 文 才

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

	AiLevelParam *aiLevelParam = mgrParam.aiLevelParam;   // AI等级参数
	int randomVal = 0;  // 随机数值

	if (NULL == aiLevelParam)
	{
#ifdef _DEBUG
		assert(0 && "AI等级参数的结构指针值为空");
#endif
		return true;
	}

	// 获取随机数，与设置的暂停概率比较
	randomVal = CommonFunctionSingleton::getInstance()->GetRandomValFromOneToHundred();

	if (randomVal <= aiLevelParam->pauseProbability)
	{
		return true;
	}

	// 和平状态下，只能执行巡逻
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


	// 获得最近一次自己攻击的目标	
 	IArmyPtr LastAttackTarget = armyObjParam->GetLastAttackTarget();

	// 获取最近一次攻击自己的目标
	IArmyPtr LastAttackedSelf = armyObjParam->GetLastEnmityTarget();

	// 获取最近一次自己攻击的部队的HP
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

	// 获取射程内的敌军
	IArmyPtr EnemyInGunshotPtr  = CommonFunctionSingleton::getInstance()->getGunshotEnemyToAttack(mapDataParamPtr, armyObjParam, aiLevelParam->tipTopAttackTar);

	// 如果当前部队类型是建筑物，并且射程内有敌军，就直接攻击
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

	// 射程内无敌军，在视野内选择攻击
	if (NULL == EnemyInGunshotPtr)
	{
		// 获取视野范围内的要攻击的敌军
		bool isSucess = CommonFunctionSingleton::getInstance()
			->getApproEnemyToAttackOnAimRule(mapDataParamPtr, armyObjParam, armyObjParam->GetViewScope(),  EnemyInGunshotPtr, aiLevelParam->tipTopAttackTar);

		if (!isSucess)
		{
			return false;
		}
	}

	// 自己主动攻击在自己射程内的部队，被消灭
	if (0 == HpValOfAttackTarget)
	{
		return thinkingInOffset(mapDataParamPtr, seekPathArithmParamPtr, LastAttackedSelf, EnemyInGunshotPtr, armyObjParam, strategyMgr, isEven);
	}
	// 未被消灭
	else
	{
		// 如果被攻击的部队正在逃脱(在射程外，并在视野内),则追击目标
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

	// 如果射程内又出现了敌军，则攻击
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

	// 自己没有被射程外的敌军攻击,则转为空闲状态
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

	// 空闲,而且需要巡逻
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
		assert(0 && "获取部队坐标失败");
#endif
	}

	posStart.xPos = (armyPosVector[0]).PosX;
	posStart.yPos = (armyPosVector[0]).PosY;

	const std::vector<POS_STRUCT> &enemyPosVector = LastAttackTarget->GetArmyPos();
	
	if (enemyPosVector.empty())
	{
#ifdef _DEBUG
		assert(0 && "获取部队坐标失败");
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