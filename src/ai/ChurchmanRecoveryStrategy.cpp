// @brief 简要描述: 牧师防御策略
// @author 作者: 黄 文 才

#include "ChurchmanRecoveryStrategy.h"
#include "ArmyAction.h"
#include "AppetentAction.h"
#include "PatrolFunSingleton.h"

ChurchmanRecoveryStrategy::ChurchmanRecoveryStrategy(void)
{
}

ChurchmanRecoveryStrategy::~ChurchmanRecoveryStrategy(void)
{
}

bool ChurchmanRecoveryStrategy::thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven)
{	
	if (NULL == strategyMgr)
	{
		return false;
	}

	managerParam &mgrParam = strategyMgr->getMgrParam();

	AiLevelParam *aiLevelParam = mgrParam.aiLevelParam;   // AI等级参数

	if (NULL == aiLevelParam)
	{
#ifdef _DEBUG
		assert(0 && "AI等级参数的结构指针值为空");
#endif
		return true;
	}

	int randomVal = 0;  // 随机数值

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
			bool isSucess = PatrolFunSingleton::getInstance()->executePatrol(mapDataParamPtr, seekPathArithmParamPtr, armyObj, strategyMgr, isEven);

			if (!isSucess)
			{
				return false;
			}
		}

		return true;
	}

	// 如果是执行后退命令的话,继续执行后退命令逃跑
	if (mgrParam.isCountermarch)
	{
		if (isEven)
		{
			return true;
		}

		mgrParam.PatrolStruct.isPatrol = false;

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, mgrParam.isCountermarch);	

		return true;
	}

	// 获取用户设置的跟随目标
	IArmyPtr followObjPtr = armyObj->GetFollowTarget();
	PosXYStru posMoveTo;
	bool isSucess = false;

	listArmyPtr lstFriends;
	IArmyPtr minHPArmy = NULL;
	unsigned int attackScope = armyObj->GetAttackScope();

	// 判断在攻击范围内是否有需要加血的友军
	int success = CommonFunctionSingleton::getInstance()->getGroupArmy(mapDataParamPtr, armyObj,armyObj->GetAttackScope(), lstFriends);

	lstFriends.push_back(armyObj);

	if (!success)
	{
		return false;
	}

	if (!lstFriends.empty())
	{
		minHPArmy = CommonFunctionSingleton::getInstance()->getMinHPArmy(lstFriends);
	}

	// 先处理加血
	if (minHPArmy)
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

		attackFunParam attOrMoveFunParm(funParam, minHPArmy->GetArmyID());

		const std::vector<POS_STRUCT> &armyPosVect = armyObj->GetArmyPos();
		const std::vector<POS_STRUCT> &enemyPosVect = minHPArmy->GetArmyPos();

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
			funParam.enemyID = minHPArmy->GetArmyID();
		}

		//CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);
	
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
		
	}

	if (isEven)
	{
		return true;
	}

	// 获取最后攻击自己的敌军
	IArmyPtr attackMeArmy  = armyObj->GetLastEnmityTarget();

	bool isNeedMoveToSecurityPos = false;

	if (NULL != attackMeArmy
		&& 0 != attackMeArmy->GetCurrentHp()
		&& 0 != aiLevelParam->runAway)
	{
		int enemyAttackScope = attackMeArmy->GetAttackScope();

		const std::vector<POS_STRUCT> &enemyPosVect = attackMeArmy->GetArmyPos();
		const std::vector<POS_STRUCT> &armyPosVect = armyObj->GetArmyPos();

		if (enemyPosVect.empty() || armyPosVect.empty())
		{
			return false;
		}

		if (getDistanceTwoNode(armyPosVect[0].PosX, armyPosVect[0].PosY, enemyPosVect[0].PosX, enemyPosVect[0].PosY) <= enemyAttackScope)
		{
			isNeedMoveToSecurityPos = true;
		}
	}
	
	// 自己被打, 选择移动到安全点。
	if (isNeedMoveToSecurityPos)
	{
		isSucess = CommonFunctionSingleton::getInstance()->MoveToSecurityPos(mapDataParamPtr, seekPathArithmParamPtr, armyObj, attackMeArmy, posMoveTo);

		if (!isSucess)
		{
			return false;
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

		moveFunParam attOrMoveFunParm(funParam, posMoveTo);

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
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParamTmp->tipTopAttackTar, 
			aiLevelParamTmp->isDynamicSeekPath, false, false, true);

		return true;
	}

	// 如果用户没有设置的跟随目标或者设置的跟随目标死亡时
	if (NULL == followObjPtr || 0 == followObjPtr->GetCurrentHp())
	{
		return getFollowTarAndFollow(mapDataParamPtr, armyObj, strategyMgr, seekPathArithmParamPtr, isEven);
	}
	// 否则，跟随指定的目标
	else
	{
		return followTar(followObjPtr, armyObj, strategyMgr, mapDataParamPtr, seekPathArithmParamPtr, isEven);
	}
}


bool ChurchmanRecoveryStrategy::getFollowTarAndFollow( IArmyPtrvectInvect * mapDataParamPtr, IArmyPtr armyObj, IManager * strategyMgr, SeekPath * seekPathArithmParamPtr, bool isEven) const 
{
	if (NULL == strategyMgr)
	{
		return false;
	}

	managerParam &mgrParam = strategyMgr->getMgrParam();

	IArmyPtr followArmyObj = NULL;

	// 判断视野内是否存在的主战部队（战士-射手）为跟随对象
	bool isSucess = CommonFunctionSingleton::getInstance()->getFollowObjPtr(mapDataParamPtr, armyObj, armyObj->GetViewScope(), followArmyObj);

	// 视野内没有合适的跟随对象
	if (NULL == followArmyObj)
	{
		// 空闲,而且需要巡逻
		if (! mgrParam.PatrolPosVectPtr->empty())
		{
			isSucess = PatrolFunSingleton::getInstance()->executePatrol(mapDataParamPtr, seekPathArithmParamPtr, armyObj, strategyMgr, isEven);

			if (!isSucess)
			{
				return false;
			}
		}
		// 否则后退, 副本AI不选择后退
		else if (Counterpart_AI !=  mgrParam.useAiScene)
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

			AiLevelParam *aiLevelParamTmp =  mgrParam.aiLevelParam;


			if (NULL == aiLevelParamTmp)
			{
#ifdef _DEBUG
				assert(0);
#endif
				return false;

			}

			AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
			AppetentActionObj.action(isEven, mgrParam.enemyID,  mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParamTmp->tipTopAttackTar, aiLevelParamTmp->isDynamicSeekPath);

			return true;
		}
	}
	// 否则跟随选中的跟随对象
	else
	{
		// 重新设置部队跟随目标
		armyObj->SetFollowTarget(followArmyObj);

		// 获取跟随部队的坐标
		const vector<POS_STRUCT> &vectorPosTaraget = followArmyObj->GetArmyPos();

		const vector<POS_STRUCT> &vectorPosArmy = armyObj->GetArmyPos();

		int attackScope = armyObj->GetAttackScope();
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
			 mgrParam.PatrolStruct.isPatrol = false;

			FunParam funParam(mapDataParamPtr, 
								seekPathArithmParamPtr, 
								 mgrParam.armyCurrentStatus, 
								 mgrParam.movePosVector, 
								 mgrParam.moveTagetPos,
								 mgrParam.enemyID,
								armyObj,
								strategyMgr);

			moveFunParam attOrMoveFunParm(funParam,	posTarget);

			CommonFunctionSingleton::getInstance()->setMoveTargetPos(attOrMoveFunParm);

			AiLevelParam *aiLevelParamTmp =  mgrParam.aiLevelParam;

			if (NULL == aiLevelParamTmp)
			{
#ifdef _DEBUG
			assert(0 && "指针为空");
#endif
				return false;
			}

			AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
			AppetentActionObj.action(isEven, mgrParam.enemyID,  mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParamTmp->tipTopAttackTar, aiLevelParamTmp->isDynamicSeekPath);		
		}
	}

	return true;
}

bool ChurchmanRecoveryStrategy::followTar(const IArmyPtr followObjPtr, IArmyPtr armyObj, IManager * strategyMgr, IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, bool isEven) const
{
	if (NULL == strategyMgr)
	{
		return false;
	}

	managerParam &mgrParam = strategyMgr->getMgrParam();

	const vector<POS_STRUCT> &vectorPosTaraget = followObjPtr->GetArmyPos();
	const vector<POS_STRUCT> &vectorPosArmy = armyObj->GetArmyPos();
	int attackScope = armyObj->GetAttackScope();
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
		mgrParam.PatrolStruct.isPatrol = false;

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
			assert(0 && "指针为空");
#endif
			return false;
		}

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParamTmp->tipTopAttackTar, aiLevelParamTmp->isDynamicSeekPath);	
	}

	return true;
}