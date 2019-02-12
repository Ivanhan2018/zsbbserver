// @brief 简要描述: 弓箭手驻守策略
// @author 作者: 黄 文 才

#include "BowArrowFigDefendStrategy.h"
#include "AppetentAction.h"

BowArrowFigDefendStrategy::BowArrowFigDefendStrategy(void)
{
}

BowArrowFigDefendStrategy::~BowArrowFigDefendStrategy(void)
{
}

bool BowArrowFigDefendStrategy::thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven)
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

	// 获取射程内的敌军
	IArmyPtr EnemyInGunshotPtr  = CommonFunctionSingleton::getInstance()->getGunshotEnemyToAttack(mapDataParamPtr, armyObj, aiLevelParam->tipTopAttackTar);

	// 如果射程内出现了敌军，则攻击
	if (NULL != EnemyInGunshotPtr)
	{
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
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar);
	}

	return true;
}

