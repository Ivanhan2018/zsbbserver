// @brief 简要描述: 工厂方法，创建对应职业的策略管理器
// @author 作者: 黄 文 才

#ifndef _FACTORYCREATEPROFESSIONMGR
#define _FACTORYCREATEPROFESSIONMGR

#include "IManager.h"
#include "SeekPathArithm.h"

typedef boost::shared_ptr<IManager> ManagerPtr;

class FactoryCreateProfessionMger
{
public:
	FactoryCreateProfessionMger(void);
public:
	virtual ~FactoryCreateProfessionMger(void);

	// 函数描述：创建弓箭手的策略管理器
	// @param mapDataPtr 部队地图数据指针
	// @param seekPathArithmParamPtr 寻路对象的指针
	// @param armyObj 当前操作的部队
	// @return 返回弓箭手策略管理器的智能指针
	ManagerPtr CreateBowArrowFigStrategyMgr(IArmyPtrvectInvect * mapDataParamPtr,  CLoadMapInfo *ClassImpedientPtr, SeekPath * seekPathArithmPtr, IArmyPtr armyObjPtr, 
		PatrolPosVect *PatrolPosVectParamPtr, ENUM_USE_SCENE useAiSceneParam, AiLevelParam *aiLevelParamTmp, CELL_HAS_ARMY_MAX cellHasArmyMax, 
		const LandformInfoMap *landformInfoMapPtrParam) const;

	// 函数描述：创建牧师的策略管理器
	// @param mapDataPtr 部队地图数据指针
	// @param seekPathArithmParamPtr 寻路对象的指针
	// @param armyObj 当前操作的部队
	// @return 返回牧师策略管理器的智能指针
	ManagerPtr CreateChurchmanStrategyMgr(IArmyPtrvectInvect * mapDataParamPtr, CLoadMapInfo *ClassImpedientPtr,  SeekPath * seekPathArithmPtr, IArmyPtr armyObjPtr, 
		PatrolPosVect *PatrolPosVectParamPtr, ENUM_USE_SCENE useAiSceneParam, AiLevelParam *aiLevelParamTmp, CELL_HAS_ARMY_MAX cellHasArmyMax, 
		const LandformInfoMap *landformInfoMapPtrParam) const;
	
	// 函数描述：创建战士的策略管理器
	// @param mapDataPtr 部队地图数据指针
	// @param seekPathArithmParamPtr 寻路对象的指针
	// @param armyObj 当前操作的部队
	// @return 返回战士策略管理器的智能指针
	ManagerPtr CreateCombatantStrategyMgr(IArmyPtrvectInvect * mapDataParamPtr,  CLoadMapInfo *ClassImpedientPtr, SeekPath * seekPathArithmPtr, IArmyPtr armyObjPtr, 
		PatrolPosVect *PatrolPosVectParamPtr, ENUM_USE_SCENE useAiSceneParam,  AiLevelParam *aiLevelParamTmp, CELL_HAS_ARMY_MAX cellHasArmyMax, 
		const LandformInfoMap *landformInfoMapPtrParam) const;

	// 函数描述：创建法师的策略管理器
	// @param mapDataPtr 部队地图数据指针
	// @param seekPathArithmParamPtr 寻路对象的指针
	// @param armyObj 当前操作的部队
	// @return 返回法师策略管理器的智能指针
	ManagerPtr CreateRabbiStrategyMgr(IArmyPtrvectInvect * mapDataParamPtr, CLoadMapInfo *ClassImpedientPtr,  SeekPath * seekPathArithmPtr, IArmyPtr armyObjPtr,
		PatrolPosVect *PatrolPosVectParamPtr, ENUM_USE_SCENE useAiSceneParam,  AiLevelParam *aiLevelParamTmp, CELL_HAS_ARMY_MAX cellHasArmyMax, 
		const LandformInfoMap *landformInfoMapPtrParam) const;

};

#endif
