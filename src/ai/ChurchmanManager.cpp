// @brief 简要描述: 牧师策略管理器
// @author 作者: 黄 文 才

#include "ChurchmanManager.h"
#include "ChurchmanRecoveryStrategy.h"
#include "ChurchmanAttackStrategy.h"
#include "ChurchmanSelfdefenceStrategy.h"
#include "ChurchmanDefendStrategy.h"

ChurchmanManager::ChurchmanManager(IArmyPtrvectInvect * mapDataParamPtr, CLoadMapInfo *ClassImpedientPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjParamPtr, 
								   PatrolPosVect *PatrolPosVectParamPtr, ENUM_USE_SCENE useAiSceneParam, AiLevelParam *aiLevelParamTmp,
								   CELL_HAS_ARMY_MAX cellHasArmyMaxParam, const LandformInfoMap *landformInfoMapPtrParam)
: mapDataPtr(mapDataParamPtr), seekPathArithmPtr(seekPathArithmParamPtr), armyObjPtr(armyObjParamPtr)
{
	try
	{
		ptrChurchmanStrategy[RecoveryAI] = new ChurchmanRecoveryStrategy();
		ptrChurchmanStrategy[AttackAI] = new ChurchmanAttackStrategy();
		ptrChurchmanStrategy[SelfDefenceAI] = new ChurchmanSelfdefenceStrategy();
		ptrChurchmanStrategy[DefendAi] = new ChurchmanDefendStrategy();
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

ChurchmanManager::~ChurchmanManager(void)
{
	if (NULL != ptrChurchmanStrategy[RecoveryAI])
	{
		try
		{	
			delete ptrChurchmanStrategy[RecoveryAI];
			ptrChurchmanStrategy[RecoveryAI] = NULL;
		}
		catch (...)
		{
			;
		}

	}

	if (NULL != ptrChurchmanStrategy[AttackAI])
	{
		try
		{	
			delete ptrChurchmanStrategy[AttackAI];
			ptrChurchmanStrategy[AttackAI] = NULL;
		}
		catch (...)
		{
			;
		}

	}

	if (NULL != ptrChurchmanStrategy[SelfDefenceAI])
	{
		try
		{		
			delete ptrChurchmanStrategy[SelfDefenceAI];
			ptrChurchmanStrategy[SelfDefenceAI] = NULL;
		}
		catch (...)
		{
			;
		}

	}

	if (NULL != ptrChurchmanStrategy[DefendAi])
	{
		try
		{	
			delete ptrChurchmanStrategy[DefendAi];
			ptrChurchmanStrategy[DefendAi] = NULL;
		}
		catch (...)
		{
			;
		}

	}
}

bool ChurchmanManager::thinking(bool isEven)
{
	if (NULL == ptrChurchmanStrategy[AiStrategy])
	{
		return false;
	}
	else
	{
		return ptrChurchmanStrategy[AiStrategy]->thinking(mapDataPtr, seekPathArithmPtr, armyObjPtr, this, isEven);
	}

}

void ChurchmanManager::setStrategy(enumAIStrategy AiStrategyParam)
{
	AiStrategy = AiStrategyParam;
}