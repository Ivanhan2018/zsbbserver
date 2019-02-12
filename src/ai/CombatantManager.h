// @brief 简要描述: 战士策略管理器
// @author 作者: 黄 文 才

#ifndef _COMBATANTMANAGER_H_H_H
#define _COMBATANTMANAGER_H_H_H

#include "IManager.h"
#include "AiCommon.h"
#include "SeekPathArithm.h"

class ICombatantStrategy;
class CombatantRecoveryStrategy;
class CombatantAttackStrategy;
class CombatantSelfdefStrategy;
class CombatantDefendStrategy;

class CombatantManager : public IManager
{
public:
	CombatantManager(IArmyPtrvectInvect * mapDataParamPtr, CLoadMapInfo *ClassImpedientPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjParamPtr, PatrolPosVect *PatrolPosVectParamPtr, 
		ENUM_USE_SCENE useAiSceneParam, AiLevelParam *aiLevelParamTmp, CELL_HAS_ARMY_MAX cellHasArmyMaxParam, const LandformInfoMap *landformInfoMapPtrParam);
public:
	virtual ~CombatantManager(void);
	
	// 函数描述：执行命令
	// @return 返回true / false ； true: 能够走  false：不能走
	bool thinking(bool isEven);

	// 函数描述：执行命令
	// @param AiStrategy 策略类型
	// @return 无
	void setStrategy(enumAIStrategy AiStrategy);
private:
	CombatantManager();
	ICombatantStrategy * ptrCombatantStrategy[endStrategyNum];
	enumAIStrategy AiStrategy;

	IArmyPtrvectInvect * mapDataPtr;
	SeekPath * seekPathArithmPtr;
	IArmyPtr  armyObjPtr;
};

#endif

