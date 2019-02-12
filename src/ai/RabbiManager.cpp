// @brief 简要描述: 法师策略管理器
// @author 作者: 黄 文 才

#include "RabbiManager.h"

#include "RabbiRecoveryStrategy.h"
#include "RabbiAttackStrategy.h"
#include "RabbiSelfdefenceStragegy.h"
#include "RabbiDefendStrategy.h"

RabbiManager::RabbiManager(IArmyPtrvectInvect * mapDataParamPtr, CLoadMapInfo *ClassImpedientPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjParamPtr, PatrolPosVect *PatrolPosVectParamPtr, 
						   ENUM_USE_SCENE useAiSceneParam, AiLevelParam *aiLevelParamTmp, CELL_HAS_ARMY_MAX cellHasArmyMaxParam, const LandformInfoMap * landformInfoMapPtrParam)
: mapDataPtr(mapDataParamPtr), seekPathArithmPtr(seekPathArithmParamPtr), armyObjPtr(armyObjParamPtr)
{
	try
	{	
		ptrRabbiStrategy[RecoveryAI] = new RabbiRecoveryStrategy();
		ptrRabbiStrategy[AttackAI] = new RabbiAttackStrategy();
		ptrRabbiStrategy[SelfDefenceAI] = new RabbiSelfdefenceStragegy();
		ptrRabbiStrategy[DefendAi] = new RabbiDefendStrategy();
	}
	catch (...)
	{
	}

	managerParam &mgrParam = getMgrParam();

	mgrParam.enemyID = 0;

	mgrParam.armyPtr = NULL;

	mgrParam.calledAttackEnemyID = 0;
	AiStrategy = SelfDefenceAI;

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

RabbiManager::~RabbiManager(void)
{
	if (NULL != ptrRabbiStrategy[RecoveryAI])
	{
		try
		{	
			delete ptrRabbiStrategy[RecoveryAI];
			ptrRabbiStrategy[RecoveryAI] = NULL;
		}
		catch (...)
		{
			;
		}
	}

	if (NULL != ptrRabbiStrategy[AttackAI])
	{	
		try
		{	
			delete ptrRabbiStrategy[AttackAI];
			ptrRabbiStrategy[AttackAI] = NULL;
		}
		catch (...)
		{
			;
		}
	}

	if (NULL != ptrRabbiStrategy[SelfDefenceAI])
	{
		try
		{	
			delete ptrRabbiStrategy[SelfDefenceAI];
			ptrRabbiStrategy[SelfDefenceAI] = NULL;
		}
		catch (...)
		{
			;
		}

	}

	if (NULL != ptrRabbiStrategy[DefendAi])
	{	
		try
		{	
			delete ptrRabbiStrategy[DefendAi];
			ptrRabbiStrategy[DefendAi] = NULL;
		}
		catch (...)
		{
			;
		}
	}
}

bool RabbiManager::thinking(bool isEven)
{
	return ptrRabbiStrategy[AiStrategy]->thinking(mapDataPtr, seekPathArithmPtr, armyObjPtr, this, isEven);
}

void RabbiManager::setStrategy(enumAIStrategy AiStrategyParam)
{
	AiStrategy = AiStrategyParam;
}
