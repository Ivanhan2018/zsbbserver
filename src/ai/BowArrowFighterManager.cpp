// @brief 简要描述: 弓箭手策略管理器
// @author 作者: 黄 文 才

#include "BowArrowFighterManager.h"
#include "BowArrowFigRecoveryStrategy.h"
#include "BowArrowFigAttackStrategy.h"
#include "BowArrowFigSelfdefStrategy.h"
#include "BowArrowFigDefendStrategy.h"

BowArrowFighterManager::BowArrowFighterManager(IArmyPtrvectInvect * mapDataParamPtr, CLoadMapInfo *ClassImpedientPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjParamPtr, 
											   PatrolPosVect *PatrolPosVectParamPtr, ENUM_USE_SCENE useAiSceneParam, AiLevelParam *aiLevelParamTmp,
											   CELL_HAS_ARMY_MAX cellHasArmyMaxParam, const LandformInfoMap *landformInfoMapPtrParam)
: mapDataPtr(mapDataParamPtr), seekPathArithmPtr(seekPathArithmParamPtr), armyObjPtr(armyObjParamPtr)
{
	try
	{
		ptrBowArrowFigStrategy[RecoveryAI] = new BowArrowFigRecoveryStrategy();
		ptrBowArrowFigStrategy[AttackAI] = new BowArrowFigAttackStrategy();
		ptrBowArrowFigStrategy[SelfDefenceAI] = new BowArrowFigSelfdefStrategy();
		ptrBowArrowFigStrategy[DefendAi] = new BowArrowFigDefendStrategy();
	}
	catch (...)
	{

	}


	AiStrategy = SelfDefenceAI;

	managerParam &mgrParam = getMgrParam();

	mgrParam.calledAttackEnemyID = 0;

	mgrParam.enemyID = 0;

	mgrParam.armyPtr = NULL;

	mgrParam.armyCurrentStatus = Idle_status;

	mgrParam.isCountermarch = false;

	mgrParam.useAiScene = useAiSceneParam;

	mgrParam.PatrolStruct.isPatrol = false;

	mgrParam.aiLevelParam = aiLevelParamTmp;
	mgrParam.cellHasArmyMax = cellHasArmyMaxParam;

	mgrParam.ClassImpedientPtr = ClassImpedientPtr;

	changePatrolPosVect(aiLevelParamTmp, PatrolPosVectParamPtr);
	
	mgrParam.PatrolPosVectPtr = PatrolPosVectParamPtr;

	mgrParam.landformInfoMap = landformInfoMapPtrParam;

	mgrParam.landformAffStru.boutSumWeight = 0;
}

BowArrowFighterManager::~BowArrowFighterManager(void)
{
	if (NULL != ptrBowArrowFigStrategy[RecoveryAI])
	{
		try
		{
			delete ptrBowArrowFigStrategy[RecoveryAI];
			ptrBowArrowFigStrategy[RecoveryAI] = NULL;
		}
		catch (...)
		{
			;
		}
	}

	if (NULL != ptrBowArrowFigStrategy[AttackAI])
	{
		try
		{	
			delete ptrBowArrowFigStrategy[AttackAI];
			ptrBowArrowFigStrategy[AttackAI] = NULL;
		}
		catch (...)
		{
			;
		}

	}

	if (NULL != ptrBowArrowFigStrategy[SelfDefenceAI])
	{
		try
		{		
			delete ptrBowArrowFigStrategy[SelfDefenceAI];
			ptrBowArrowFigStrategy[SelfDefenceAI] = NULL;
		}
		catch (...)
		{
			;
		}
	}
	
	if (NULL != ptrBowArrowFigStrategy[DefendAi])
	{
		try
		{	
			delete ptrBowArrowFigStrategy[DefendAi];
			ptrBowArrowFigStrategy[DefendAi] = NULL;
		}
		catch (...)
		{
			;
		}
	}

}

bool BowArrowFighterManager::thinking(bool isEven)
{
	if (NULL == ptrBowArrowFigStrategy[AiStrategy])
	{
		return false;
	}
	else
	{
		return ptrBowArrowFigStrategy[AiStrategy]->thinking(mapDataPtr, seekPathArithmPtr, armyObjPtr, this, isEven);
	}
}

void BowArrowFighterManager::setStrategy(enumAIStrategy AiStrategyParam)
{
	AiStrategy = AiStrategyParam;
}