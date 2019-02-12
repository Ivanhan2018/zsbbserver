// @brief 简要描述: 牧师进攻策略
// @author 作者: 黄 文 才

#include "ChurchmanAttackStrategy.h"
#include "AppetentAction.h"

ChurchmanAttackStrategy::ChurchmanAttackStrategy()
{
}

ChurchmanAttackStrategy::~ChurchmanAttackStrategy(void)
{
}

bool ChurchmanAttackStrategy::thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven)
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
	if (war != armyObj->getMapState()  && Counterpart_AI != mgrParam.useAiScene )
	{
//#ifdef _DEBUG
//		assert(0 && "和平状态下不能设置进攻型AI");
//#endif
		return true;
	}


	// 如果是执行后退命令的话,继续执行后退命令逃跑
	if (mgrParam.isCountermarch)
	{
		if (isEven)
		{
			return true;
		}

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID,mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath, mgrParam.isCountermarch);	
		
		return true;
	}

	listArmyPtr lstFriends;

	IArmyPtr minHPArmy = NULL;

	// 判断在攻击范围内是否有需要加血的友军
	bool isSucess = CommonFunctionSingleton::getInstance()->getGroupArmy(mapDataParamPtr, armyObj, armyObj->GetAttackScope(), lstFriends);

	lstFriends.push_back(armyObj);

	if (!isSucess)
	{
		return false;
	}

	lstFriends.push_back(armyObj);

	if (!lstFriends.empty())
	{
		minHPArmy = CommonFunctionSingleton::getInstance()->getMinHPArmy(lstFriends);

#ifdef _DEBUG
		if (NULL != minHPArmy)
		{
			int hurtVal = minHPArmy->getCurHurtHp();

			assert(0 != hurtVal);
		}
#endif

	}

	// 先处理加血
	if (NULL != minHPArmy)
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

		attackFunParam attOrMoveFunParm(funParam, minHPArmy->GetArmyID());

		CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath);	

	}

	if (isEven)
	{
		return true;
	}

	// 获取最后攻击自己的敌军
	IArmyPtr attackMeArmy  = armyObj->GetLastEnmityTarget();

	// 后处理跟随
	IArmyPtr followObjPtr = armyObj->GetFollowTarget();
	PosXYStru posMoveTo;
	bool isNeedMoveToSecurityPos = false;
	bool isNeedFollowQuick = false;

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

	if (NULL != followObjPtr)
	{
		const std::vector<POS_STRUCT> &followPosVect = followObjPtr->GetArmyPos();
		const std::vector<POS_STRUCT> &armyPosVect = armyObj->GetArmyPos();

		if (followPosVect.empty() || armyPosVect.empty())
		{
#ifdef _DEBUG
			assert(0);
#endif
			return false;
		}

		if (getDistanceTwoNode(armyPosVect[0].PosX, armyPosVect[0].PosY, followPosVect[0].PosX, followPosVect[0].PosY) > armyObj->GetAttackScope())
		{
			isNeedFollowQuick = true;
		}
	}

	// 自己被打，需要移动到安全点。
	if (isNeedMoveToSecurityPos && !isNeedFollowQuick)
	{
		isSucess = CommonFunctionSingleton::getInstance()->MoveToSecurityPos(mapDataParamPtr, seekPathArithmParamPtr, armyObj, attackMeArmy, posMoveTo);

		if (!isSucess)
		{
			return false;
		}

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


		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, 
			aiLevelParam->isDynamicSeekPath, false, false, true);

		return true;
	}

	// 如果之前没有跟随对象，或则跟随对象已死亡
	if (NULL == followObjPtr || 0 == followObjPtr->GetCurrentHp())
	{
		return getFollowTarAndFollowOrBack(followObjPtr, mapDataParamPtr, armyObj, strategyMgr, seekPathArithmParamPtr, isEven);

	}
	// 否则，跟随指定的目标
	else
	{
		return followTarget(followObjPtr, armyObj, mapDataParamPtr, seekPathArithmParamPtr, strategyMgr, isEven);
	}
}

bool ChurchmanAttackStrategy::getFollowTarAndFollowOrBack( IArmyPtr &followObjPtr, IArmyPtrvectInvect * mapDataParamPtr, IArmyPtr armyObj, IManager * strategyMgr, SeekPath * seekPathArithmParamPtr, bool isEven)
{
	followObjPtr = NULL;

	managerParam &mgrParam = strategyMgr->getMgrParam();

	bool isSucess = CommonFunctionSingleton::getInstance()->getFollowObjPtr(mapDataParamPtr, armyObj, armyObj->GetViewScope(), followObjPtr);

	if (!isSucess)
	{
		return false;
	}

	// 视野范围内没有合适的跟随对象，选择后退到着陆点.副本中不后退 
	if (NULL == followObjPtr && Counterpart_AI != mgrParam.useAiScene)
	{
		mgrParam.isCountermarch = true;

		POS_STRUCT posXY=armyObj->GetBirthPos();

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

#ifdef _DEBUG
		if (NULL == aiLevelParamTmp)
		{
			assert(0);
		}
#endif


		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParamTmp->tipTopAttackTar, aiLevelParamTmp->isDynamicSeekPath);	

		return true;
	}
	// 否则跟随选中的跟随对象
	else if (NULL != followObjPtr)
	{
		// 设置跟随对象
		armyObj->SetFollowTarget(followObjPtr);

		// 获取跟随部队的坐标
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
		}
	}

	return true;
}

bool ChurchmanAttackStrategy::followTarget(const IArmyPtr followObjPtr, IArmyPtr armyObj, IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IManager * strategyMgr, bool isEven) const
{
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

	// 如果跟随的对象在自己的射程内，就不在移动
	if (distantVal <= attackScope)
	{
		return true;
	}
	else
	{
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
	}

	return true;
}