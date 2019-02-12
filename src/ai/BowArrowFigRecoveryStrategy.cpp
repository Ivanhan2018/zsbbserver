// @brief 简要描述: 弓箭手防御策略
// @author 作者: 黄 文 才

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

	bool isAttack = false;  // 初始化为不攻击
	bool isSucess = true;   
	bool isSatisfyConOfCountermarch = false;
	IArmyPtr armyPtrTmp = NULL;

	AiLevelParam *aiLevelParam = mgrParam.aiLevelParam;   // AI等级参数
	int randomVal = 0;  // 随机数值

	if (NULL == aiLevelParam)
	{
#ifdef _DEBUG
		assert(0 && "AI等级参数的结构指针值为空");
#endif
		return false;
	}

	std::list<IArmyPtr> lstFriendGroup;

	// 获取随机数，与设置的暂停概率比较
	randomVal = CommonFunctionSingleton::getInstance()->GetRandomValFromOneToHundred();

	if (randomVal <= aiLevelParam->pauseProbability)
	{
		return true;
	}

	// 和平状态下，只能执行巡逻
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

	// 增援其他部队
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

	// 如果是执行后退命令的话, 继续执行后退命令
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

	// 获取上次攻击的敌军部队指针
	IArmyPtr enemyPtr = armyObj->getArmyByID(mgrParam.enemyID);
	
	// 获取部队的视野
	int viewScope = armyObj->GetViewScope();

	// 上次攻击的敌军没有死
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

		// 并且还在视野内，继续攻击，不切换目标
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

	// 获取射程内攻击的敌军
	IArmyPtr EnemyInGunshotPtr = CommonFunctionSingleton::getInstance()->getGunshotEnemyToAttack(mapDataParamPtr, armyObj, aiLevelParam->tipTopAttackTar);

	// 射程内出现敌军，则攻击
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

	// 获取视野范围内的要攻击的敌军，并判断是否采取攻击
	isSucess = CommonFunctionSingleton::getInstance()
		->getAttackArmy(mapDataParamPtr, armyObj, aiLevelParam->countRange,  EnemyInGunshotPtr, isAttack, lstFriendGroup, aiLevelParam->tipTopAttackTar);

	if (!isSucess)
	{
		return false;
	}

	// 攻击视野内的敌军
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

		// 呼叫增援
		return callFriendArmyToReinforce(lstFriendGroup, isSucess, mapDataParamPtr, armyObj, strategyMgr, armyPtrTmp, EnemyInGunshotPtr);

	}

	// 是否满足后退条件
	isSatisfyConOfCountermarch = isSatisfyCondiOfCountermarch(mapDataParamPtr, armyObj);

	// 选择后退，并满足后退条件，副本中NPC不后退
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
	// 选择了后退，但又不满足后退条件，选择攻击，搏命一击
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
	int countCellsHasEnemy = 0; // 统计相邻格子里有敌军的个数

	// 获取指定相邻格子的部队的list
	bool isSucess = CommonFunctionSingleton::getInstance()->getSpecifyDistanceEnemy(mapDataParamPtr, armyObj, AI_ONE_VALUE_NUM, false, false, lstFriendArmy, lstEnemy, countCellsHasEnemy);

	if (!isSucess)
	{
#ifdef _DEBUG
		assert(0 && "调用获取指定相邻的部队的list失败");
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
	// 寻找以自己为中心的所有友军(已托管部队（或NPC部队）)
	isSucess = CommonFunctionSingleton::getInstance()->getGroupArmy(mapDataParamPtr, armyObj, aiLevelParamTmp->callRangeVal, lstFriendGroup);

	if (!isSucess)
	{
		return false;
	}

	std::list<IArmyPtr>::iterator itBeg, itEnd;
	
	// 呼叫友军增援
	itBeg = lstFriendGroup.begin();
	itEnd = lstFriendGroup.end();

	for (; itBeg!=itEnd; ++itBeg)
	{

		if (*itBeg == armyObj)
		{
#ifdef _DEBUG
			assert(0 && "加了自己了");
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
			//				assert(0 && "获取策略管理器指针失败");
			//#endif
			return false;
		}

		armyPtrTmp = NULL;

		managerParam &mgrParamTmp = strategyMgr->getMgrParam();

		if (0 != mgrParamTmp.calledAttackEnemyID)
		{
			armyPtrTmp = armyObj->getArmyByID(mgrParamTmp.calledAttackEnemyID);
		}

		// 如果该友军没有被呼叫过或呼叫要攻击的敌军已死亡
		if (0 == mgrParamTmp.calledAttackEnemyID
			|| (NULL != armyPtrTmp && 0 == armyPtrTmp->GetCurrentHp()))
		{
			mgrParamTmp.calledAttackEnemyID = EnemyInGunshotPtr->GetArmyID();				
		}
	}

	return true;
}