// @brief 简要描述: 弓箭手进攻策略
// @author 作者: 黄 文 才

#include <cstdlib>
#include <ctime>
#include "BowArrowFigAttackStrategy.h"
#include "SeekPathArithm.h"
#include "AppetentAction.h"
#include "CommonFun.h"

BowArrowFigAttackStrategy::BowArrowFigAttackStrategy()
{
	moveTotargetEnemyID = 0;
}

BowArrowFigAttackStrategy::~BowArrowFigAttackStrategy(void)
{
}

bool BowArrowFigAttackStrategy::thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmy * armyObj, IManager *strategyMgr, bool isEven)
{
	if (NULL == strategyMgr)
	{
		return false;
	}

	//unsigned int armyId = armyObj->GetArmyID();

	managerParam &mgrParam = strategyMgr->getMgrParam();

	AiLevelParam *aiLevelParam = mgrParam.aiLevelParam;   // AI等级参数

	if (NULL == aiLevelParam)
	{
#ifdef _DEBUG
		assert(0 && "AI等级参数的结构指针值为空");
#endif
		return false;
	}

	int randomVal = 0;  // 随机数值

	// 获取随机数，与设置的暂停概率比较
	randomVal = CommonFunctionSingleton::getInstance()->GetRandomValFromOneToHundred();

	if (randomVal <= aiLevelParam->pauseProbability)
	{
		return true;
	}

	// 和平状态下，只能执行巡逻
	if (war != armyObj->getMapState() && Counterpart_AI != mgrParam.useAiScene )
	{
//#ifdef _DEBUG
//		assert(0 && "和平状态下不能设置进攻型AI");
//#endif
		return true;
	}

	bool isSuceess = false;
	
	// 获取部队的射程
	int attackScope = armyObj->GetAttackScope();

	const std::vector<POS_STRUCT> &armyPosVect = armyObj->GetArmyPos();

	if (armyPosVect.empty())
	{
		return false;
	}

	// 获取部队的视野
	int viewScope = armyObj->GetViewScope();

	bool isAttack = false, isSucess = false;
	IArmyPtr armyPtrTmp = NULL;

	std::list<IArmyPtr> lstFriendGroup;
	std::list<IArmyPtr>::iterator itBeg;
	std::list<IArmyPtr>::iterator itEnd;

	// 需要增援其他友军
	if (1 == aiLevelParam->isCanCalled && 0 != mgrParam.calledAttackEnemyID)
	{
		armyPtrTmp = armyObj->getArmyByID(mgrParam.calledAttackEnemyID);
	}

	if (NULL != armyPtrTmp && 0 != armyPtrTmp->GetCurrentHp())
	{
		FunParam funParam(mapDataParamPtr, 
							seekPathArithmParamPtr, 
							mgrParam.armyCurrentStatus, 
							mgrParam.movePosVector, 
							mgrParam.moveTagetPos,
							mgrParam.enemyID,
							armyObj,
							strategyMgr);

		attackFunParam attOrMoveFunParm(funParam, mgrParam.calledAttackEnemyID);

		const std::vector<POS_STRUCT> &enemyPosVect = armyPtrTmp->GetArmyPos();

		if (enemyPosVect.empty())
		{
			return false;
		}

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
			funParam.enemyID = armyPtrTmp->GetArmyID();
		}
		
		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath);
		
		return true;
	}		

	/*
	// 如果是执行后退命令的话
	if (mgrParam.isCountermarch)
	{
		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath, mgrParam.isCountermarch);		
		
		return true;
	}
	*/

	IArmyPtr enemyPtr = NULL;

	// 判断射程内有无敌军
	enemyPtr = CommonFunctionSingleton::getInstance()->getGunshotEnemyToAttack(mapDataParamPtr, armyObj, aiLevelParam->tipTopAttackTar);

	// 射程内有敌军先攻击
	if(NULL != enemyPtr)
	{
		if (!isEven)
		{
			return true;
		}

		FunParam funParam(mapDataParamPtr, 
							seekPathArithmParamPtr, 
							mgrParam.armyCurrentStatus, 
							mgrParam.movePosVector, 
							mgrParam.moveTagetPos,
							mgrParam.enemyID,
							armyObj,
							strategyMgr);

		attackFunParam attOrMoveFunParm(funParam, enemyPtr->GetArmyID());

		const std::vector<POS_STRUCT> &enemyPosVect = enemyPtr->GetArmyPos();

		if (enemyPosVect.empty())
		{
			return false;
		}

		if (!CommonFunctionSingleton::getInstance()->enemyIsInAttackScope(armyPosVect, enemyPosVect, attackScope))
		{
			CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);
		}
		else
		{
			FunParam &funParam = attOrMoveFunParm.funParam;
			funParam.armyCurrentStatus = Attack_status;	
			funParam.enemyID = enemyPtr->GetArmyID();
		}

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath);
		
		return true;
	}
	// 射程内无敌军 
	else
	{
		if (isEven)
		{
			return true;
		}

		// 获取上次攻击的敌军部队指针
		IArmyPtr enemyPtr = armyObj->getArmyByID(mgrParam.enemyID);

		// 上次攻击的敌军没有死
		if (NULL != enemyPtr && 0 != enemyPtr->GetCurrentHp())
		{
			const std::vector<POS_STRUCT> &enemyPosVect = enemyPtr->GetArmyPos();
			const std::vector<POS_STRUCT> &armyPosVect = armyObj->GetArmyPos();

			if (enemyPosVect.empty() || armyPosVect.empty())
			{
				return false;
			}

			// 并且还在视野内，继续攻击，不切换目标
			if (CommonFunctionSingleton::getInstance()->enemyIsInAttackScope(armyPosVect, enemyPosVect, viewScope))
			{
				// 攻击上次攻击的目标
				FunParam funParam(mapDataParamPtr, 
					seekPathArithmParamPtr, 
					mgrParam.armyCurrentStatus, 
					mgrParam.movePosVector, 
					mgrParam.moveTagetPos,
					mgrParam.enemyID,
					armyObj,
					strategyMgr);

				attackFunParam attOrMoveFunParm(funParam, mgrParam.enemyID);

				const std::vector<POS_STRUCT> &enemyPosVect = enemyPtr->GetArmyPos();

				if (enemyPosVect.empty())
				{
					return false;
				}

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

					if (NULL != enemyPtr)
					{
						funParam.enemyID = enemyPtr->GetArmyID();
					}
				}

				AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
				AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar,aiLevelParam->isDynamicSeekPath);

				return true;
			}
		}
		else
		{
			enemyPtr = NULL;
		}

		// 根据参数设置的范围，敌我双方进行力量对比
		isSucess = CommonFunctionSingleton::getInstance()
			->getAttackArmy(mapDataParamPtr, armyObj, aiLevelParam->countRange, enemyPtr, isAttack, lstFriendGroup, aiLevelParam->tipTopAttackTar);

		if(!isSucess)
		{
			return false;
		}

		// 有敌军可以攻击
		if (NULL != enemyPtr)
		{
			// if (isAttack ||  Counterpart_AI == mgrParam.useAiScene)
			{
				FunParam funParam(mapDataParamPtr, 
								seekPathArithmParamPtr, 
								 mgrParam.armyCurrentStatus, 
								 mgrParam.movePosVector, 
								 mgrParam.moveTagetPos,
								 mgrParam.enemyID,
								armyObj,
								strategyMgr);

				attackFunParam attOrMoveFunParm(funParam, enemyPtr->GetArmyID());

				const std::vector<POS_STRUCT> &enemyPosVect = enemyPtr->GetArmyPos();

				if (enemyPosVect.empty())
				{
					return false;
				}

				if (!CommonFunctionSingleton::getInstance()->enemyIsInAttackScope(armyPosVect, enemyPosVect, attackScope))
				{
					CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);
				}
				else
				{
					FunParam &funParam = attOrMoveFunParm.funParam;
					funParam.armyCurrentStatus = Attack_status;	
					funParam.enemyID = enemyPtr->GetArmyID();
				}

				AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
				AppetentActionObj.action(isEven, mgrParam.enemyID,  mgrParam.movePosVector,  mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath);


				// 呼叫增援
				return callReinforce(lstFriendGroup, isSuceess, mapDataParamPtr, armyObj, aiLevelParam, isSucess, itBeg, itEnd, enemyPtr);
			}
			/*
			// 否则后退到着陆点，副本无后退。
			else if (Counterpart_AI !=  mgrParam.useAiScene)
			{
				backpedalToBirthPos(armyObj, strategyMgr, mapDataParamPtr, seekPathArithmParamPtr);

				return true;
			}
			*/
		}
		else
		{	
			return moveToNeedAttArmy(mapDataParamPtr, armyObj, seekPathArithmParamPtr, strategyMgr, isEven);
		}
	}

	//return true;
}

bool BowArrowFigAttackStrategy::callReinforce( std::list<IArmyPtr> &lstFriendGroup, bool &isSuceess, IArmyPtrvectInvect * mapDataParamPtr, IArmy * armyObj, 
											  AiLevelParam * aiLevelParamTmp, bool isSucess, std::list<IArmyPtr>::iterator &itBeg, std::list<IArmyPtr>::iterator &itEnd,  
											  const IArmyPtr enemyPtr ) const
{
	lstFriendGroup.clear();
	// 寻找以自己为中心的所有友军(已托管部队（或NPC部队）)
	isSuceess = CommonFunctionSingleton::getInstance()->getGroupArmy(mapDataParamPtr, armyObj, aiLevelParamTmp->callRangeVal, lstFriendGroup);

	if (!isSucess)
	{
		return false;
	}

	// 呼叫友军增援
	itBeg = lstFriendGroup.begin();
	itEnd = lstFriendGroup.end();
	IArmyPtr armyPtrTmp = NULL;

	for (; itBeg!=itEnd; ++itBeg)
	{
//#ifdef _DEBUG
		if (*itBeg == armyObj)
		{
			//assert(0 && "加了自己了");
			continue;
		}
//#endif

		if (BUILD == (*itBeg)->GetArmyType())
		{
			continue;
		}

		IManager * strategyMgr = (*itBeg)->getStrategyMgr();

		if (NULL == strategyMgr)
		{
			//#ifdef _DEBUG
			//						assert(0 && "获取策略管理器指针失败");
			//#endif
			return false;
		}

		managerParam &mgrParam = strategyMgr->getMgrParam();

		armyPtrTmp = NULL;

		if (0 != (mgrParam.calledAttackEnemyID))
		{
			armyPtrTmp = armyObj->getArmyByID(mgrParam.calledAttackEnemyID);
		}

		// 如果该友军没有被呼叫过或呼叫要攻击的敌军已死亡
		if (0 == (mgrParam.calledAttackEnemyID) 
			|| (NULL != armyPtrTmp && 0 == armyPtrTmp->GetCurrentHp()))
		{
			mgrParam.calledAttackEnemyID = enemyPtr->GetArmyID();			
		}
	}

	return true;
}

void BowArrowFigAttackStrategy::backpedalToBirthPos( IArmy * armyObj, IManager * strategyMgr, IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, bool isEven)
{
	managerParam &mgrParam = strategyMgr->getMgrParam();

	POS_STRUCT posXY = armyObj->GetBirthPos();

	mgrParam.isCountermarch = true;

	PosXYStru posTarget;
	posTarget.xPos = posXY.PosX;
	posTarget.yPos = posXY.PosY;	

	FunParam funParam(mapDataParamPtr, 
						seekPathArithmParamPtr, 
						mgrParam.armyCurrentStatus, 
						mgrParam.movePosVector, 
						mgrParam.moveTagetPos,
						mgrParam.enemyID,
						armyObj,
						strategyMgr);

	moveFunParam attOrMoveFunParm(funParam, posTarget);

	CommonFunctionSingleton::getInstance()->setMoveTargetPos(attOrMoveFunParm);

	AiLevelParam *aiLevelParamTmp = mgrParam.aiLevelParam;

	if (NULL == aiLevelParamTmp)
	{
#ifdef _DEBUG
	assert(0);
#endif
		return;
	}

	AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
	AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParamTmp->tipTopAttackTar, aiLevelParamTmp->isDynamicSeekPath);
}

bool BowArrowFigAttackStrategy::moveToNeedAttArmy( IArmyPtrvectInvect * mapDataParamPtr, IArmy * armyObj, SeekPath * seekPathArithmParamPtr, IManager * strategyMgr, bool isEven)
{
	managerParam &mgrParam = strategyMgr->getMgrParam();

	IArmyPtr moveTotargetEnemy = armyObj->getArmyByID(moveTotargetEnemyID);

	if (NULL == moveTotargetEnemy || 0 == moveTotargetEnemy->GetCurrentHp())
	{
		moveTotargetEnemy = CommonFunctionSingleton::getInstance()->getNearestEnemyOutView(mapDataParamPtr,armyObj);

		if (NULL == moveTotargetEnemy)
		{
			return false;
		}
		
		moveTotargetEnemyID = moveTotargetEnemy->GetArmyID();
	}
	/*
	else
	{
		const std::vector<POS_STRUCT> &enemyPosVect = moveTotargetEnemy->GetArmyPos();
		const std::vector<POS_STRUCT> &armyPosVect = armyObj->GetArmyPos();

		int tDistance = getDistanceTwoNode(armyPosVect[0].PosX, armyPosVect[0].PosY, enemyPosVect[0].PosX, enemyPosVect[0].PosY);

		if (tDistance < armyObj->GetViewScope())
		{
			moveTotargetEnemy = CommonFunctionSingleton::getInstance()->getNearestEnemyOutView(mapDataParamPtr,armyObj);
		}
	}
	

	if (NULL == moveTotargetEnemy)
	{
		return true;
	}
*/

	const std::vector<POS_STRUCT>  &posXYVector = moveTotargetEnemy->GetArmyPos();

	if (posXYVector.empty())
	{
		return false;
	}

	PosXYStru posTarget;

	if (BUILD != moveTotargetEnemy->GetArmyType())
	{
		posTarget.xPos = posXYVector[0].PosX;
		posTarget.yPos = posXYVector[0].PosY;	
	}
	else
	{
		unsigned int posSize = static_cast<unsigned int>(posXYVector.size());

		srand(static_cast<unsigned int>(time(0)));					// 根据系统时间设置随机数种子
		unsigned int index = rand() % posSize;	// 取得区间[0,N)的整数

		posTarget.xPos = posXYVector[index].PosX;
		posTarget.yPos = posXYVector[index].PosY;	
	}

	FunParam funParam(mapDataParamPtr, 
						seekPathArithmParamPtr, 
						mgrParam.armyCurrentStatus, 
						mgrParam.movePosVector, 
						mgrParam.moveTagetPos,
						mgrParam.enemyID,
						armyObj,
						strategyMgr);

	moveFunParam attOrMoveFunParm(funParam, posTarget);

	CommonFunctionSingleton::getInstance()->setMoveTargetPos(attOrMoveFunParm);

	AiLevelParam *aiLevelParamTmp = mgrParam.aiLevelParam;

	if (NULL == aiLevelParamTmp)
	{
#ifdef _DEBUG
	assert(0);
#endif
		return false;

	}

	AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
	AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParamTmp->tipTopAttackTar, aiLevelParamTmp->isDynamicSeekPath);

	return true;
}

