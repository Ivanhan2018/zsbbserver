// @brief 简要描述: 战士策略管理器
// @author 作者: 黄 文 才

#include "CombatantManager.h"
#include "CombatantRecoveryStrategy.h"
#include "CombatantAttackStrategy.h"
#include "CombatantSelfdefStrategy.h"
#include "CombatantDefendStrategy.h"

CombatantManager::CombatantManager(IArmyPtrvectInvect * mapDataParamPtr, CLoadMapInfo *ClassImpedientPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjParamPtr, 
								   PatrolPosVect *PatrolPosVectParamPtr, ENUM_USE_SCENE useAiSceneParam, AiLevelParam *aiLevelParamTmp,
								   CELL_HAS_ARMY_MAX cellHasArmyMaxParam, const LandformInfoMap *landformInfoMapPtrParam)
: mapDataPtr(mapDataParamPtr), seekPathArithmPtr(seekPathArithmParamPtr), armyObjPtr(armyObjParamPtr)
{
	try
	{
		ptrCombatantStrategy[RecoveryAI] = new CombatantRecoveryStrategy();
		ptrCombatantStrategy[AttackAI] = new CombatantAttackStrategy();
		ptrCombatantStrategy[SelfDefenceAI] = new CombatantSelfdefStrategy();
		ptrCombatantStrategy[DefendAi] = new CombatantDefendStrategy();
	}
	catch (...)
	{
	}

	managerParam &mgrParam = getMgrParam();

	mgrParam.calledAttackEnemyID = 0;

	mgrParam.armyPtr = NULL;
	
	mgrParam.enemyID = 0; 
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

CombatantManager::~CombatantManager(void)
{
	if (NULL != ptrCombatantStrategy[RecoveryAI])
	{
		try
		{
			delete ptrCombatantStrategy[RecoveryAI];
			ptrCombatantStrategy[RecoveryAI] = NULL;
		}
		catch (...)
		{
			;
		}
	}

	if (NULL != ptrCombatantStrategy[AttackAI])
	{
		try
		{
			delete ptrCombatantStrategy[AttackAI];
			ptrCombatantStrategy[AttackAI] = NULL;
		}
		catch (...)
		{
			;
		}

	}

	if (NULL != ptrCombatantStrategy[SelfDefenceAI])
	{
		try
		{
			delete ptrCombatantStrategy[SelfDefenceAI];
			ptrCombatantStrategy[SelfDefenceAI] = NULL;
		}
		catch (...)
		{
			;
		}
	}

	if (NULL != ptrCombatantStrategy[DefendAi])
	{
		try
		{
			delete ptrCombatantStrategy[DefendAi];
			ptrCombatantStrategy[DefendAi] = NULL;
		}
		catch (...)
		{
			;
		}
	}
}

bool CombatantManager::thinking(bool isEven)
{
	if (NULL == ptrCombatantStrategy[AiStrategy])
	{
		return false;
	}
	else
	{
		return ptrCombatantStrategy[AiStrategy]->thinking(mapDataPtr, seekPathArithmPtr, armyObjPtr, this, isEven);
	}
}

void CombatantManager::setStrategy(enumAIStrategy AiStrategyParam)
{
	AiStrategy = AiStrategyParam;
}
