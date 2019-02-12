// @brief 简要描述: 工厂方法，创建对应职业的策略管理器
// @author 作者: 黄 文 才

#include "FactoryCreateProfessionMger.h"
#include "BowArrowFighterManager.h"
#include "ChurchmanManager.h"
#include "CombatantManager.h"
#include "RabbiManager.h"

FactoryCreateProfessionMger::FactoryCreateProfessionMger(void)
{
}

FactoryCreateProfessionMger::~FactoryCreateProfessionMger(void)
{
}

ManagerPtr FactoryCreateProfessionMger::CreateBowArrowFigStrategyMgr(IArmyPtrvectInvect * mapDataParamPtr, 
																	 CLoadMapInfo *ClassImpedientPtr,
																	 SeekPath * seekPathArithmPtr, 
																	 IArmyPtr armyObjPtr, 
																	 PatrolPosVect *PatrolPosVectParamPtr, 
																	 ENUM_USE_SCENE useAiSceneParam,  
																	 AiLevelParam *aiLevelParamTmp,
																	 CELL_HAS_ARMY_MAX cellHasArmyMax,
																	 const LandformInfoMap *landformInfoMapPtrParam) const
{
	try
	{
		return ManagerPtr(new BowArrowFighterManager(mapDataParamPtr, ClassImpedientPtr, seekPathArithmPtr, armyObjPtr, PatrolPosVectParamPtr, 
						                             useAiSceneParam, aiLevelParamTmp, cellHasArmyMax, landformInfoMapPtrParam));
	}
	catch (...)
	{
		return ManagerPtr();
	}

}

ManagerPtr FactoryCreateProfessionMger::CreateChurchmanStrategyMgr(IArmyPtrvectInvect * mapDataParamPtr, 
																   CLoadMapInfo *ClassImpedientPtr,
																   SeekPath * seekPathArithmPtr,
																   IArmyPtr armyObjPtr,  
																   PatrolPosVect *PatrolPosVectParamPtr, 
																   ENUM_USE_SCENE useAiSceneParam,  
																   AiLevelParam *aiLevelParamTmp, 
																   CELL_HAS_ARMY_MAX cellHasArmyMax,
																   const LandformInfoMap *landformInfoMapPtrParam) const
{
	try
	{
		return ManagerPtr(new ChurchmanManager(mapDataParamPtr, 
												ClassImpedientPtr,
											   seekPathArithmPtr, 
											   armyObjPtr, 
											   PatrolPosVectParamPtr, 
											   useAiSceneParam, 
											   aiLevelParamTmp, 
											   cellHasArmyMax,
											   landformInfoMapPtrParam));
	}
	catch (...)
	{	
		return ManagerPtr();
	}
}

ManagerPtr FactoryCreateProfessionMger::CreateCombatantStrategyMgr(IArmyPtrvectInvect * mapDataParamPtr, 
																   CLoadMapInfo *ClassImpedientPtr,
																   SeekPath * seekPathArithmPtr, 
																   IArmyPtr armyObjPtr,  
																   PatrolPosVect *PatrolPosVectParamPtr, 
																   ENUM_USE_SCENE useAiSceneParam,  
																   AiLevelParam *aiLevelParamTmp, 
																   CELL_HAS_ARMY_MAX cellHasArmyMax,
																   const LandformInfoMap *landformInfoMapPtrParam) const
{
	try
	{	
		return ManagerPtr(new CombatantManager(mapDataParamPtr, ClassImpedientPtr, seekPathArithmPtr, armyObjPtr, PatrolPosVectParamPtr, 
			useAiSceneParam, aiLevelParamTmp, cellHasArmyMax, landformInfoMapPtrParam));
	}
	catch (...)
	{	
		return ManagerPtr();
	}
}

ManagerPtr FactoryCreateProfessionMger::CreateRabbiStrategyMgr(IArmyPtrvectInvect * mapDataParamPtr,
															   CLoadMapInfo *ClassImpedientPtr,
															   SeekPath * seekPathArithmPtr, 
															   IArmyPtr armyObjPtr,  
															   PatrolPosVect *PatrolPosVectParamPtr, 
															   ENUM_USE_SCENE useAiSceneParam,  
															   AiLevelParam *aiLevelParamTmp, 
															   CELL_HAS_ARMY_MAX cellHasArmyMax,
															   const LandformInfoMap *landformInfoMapPtrParam) const
{
	try
	{
		return ManagerPtr(new RabbiManager(mapDataParamPtr, ClassImpedientPtr, seekPathArithmPtr, armyObjPtr, PatrolPosVectParamPtr, 
			useAiSceneParam, aiLevelParamTmp, cellHasArmyMax, landformInfoMapPtrParam));
	}
	catch (...)
	{	
		return ManagerPtr();
	}
}