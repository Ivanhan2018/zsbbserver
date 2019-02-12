// @brief 简要描述: 牧师驻守策略
// @author 作者: 黄 文 才

#include "ChurchmanDefendStrategy.h"
#include "AppetentAction.h"


ChurchmanDefendStrategy::ChurchmanDefendStrategy()
{
}

ChurchmanDefendStrategy::~ChurchmanDefendStrategy(void)
{
}

bool ChurchmanDefendStrategy::thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven)
{
	if (NULL == strategyMgr)
	{
		return false;
	}

	if (!isEven)
	{
		return true;
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
	if (war != armyObj->getMapState() && Counterpart_AI != mgrParam.useAiScene )
	{
		//#ifdef _DEBUG
		//		assert(0 && "和平状态下不能设置进攻型AI");
		//#endif
		return true;
	}

	listArmyPtr lstFriends;

	bool success = CommonFunctionSingleton::getInstance()->getGroupArmy(mapDataParamPtr, armyObj, armyObj->GetAttackScope(), lstFriends);

	lstFriends.push_back(armyObj);
	
	if (!success)
	{
		return false;
	}

	lstFriends.push_back(armyObj);

	if (!lstFriends.empty())
	{
		IArmyPtr minHPArmy = CommonFunctionSingleton::getInstance()->getMinHPArmy(lstFriends);

		if (NULL == minHPArmy)
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

		attackFunParam attOrMoveFunParm(funParam, minHPArmy->GetArmyID());

		CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID,mgrParam.movePosVector, mgrParam.armyCurrentStatus);

	}

	return true;
}
