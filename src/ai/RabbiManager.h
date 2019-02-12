// @brief 简要描述: 法师策略管理器
// @author 作者: 黄 文 才

#ifndef _RABBIMANAGER_H_H_H
#define _RABBIMANAGER_H_H_H

#include "IManager.h"
#include "AiCommon.h"
#include "SeekPathArithm.h"

class IRabbiStrategy;
class RabbiRecoveryStrategy;
class RabbiAttackStrategy;
class RabbiSelfdefenceStragegy;
class RabbiDefendStrategy;

class RabbiManager : public IManager
{
public:
	RabbiManager(IArmyPtrvectInvect * mapDataParamPtr, CLoadMapInfo *ClassImpedientPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjParamPtr, PatrolPosVect *PatrolPosVectParamPtr, 
		ENUM_USE_SCENE useAiSceneParam, AiLevelParam *aiLevelParamTmp, CELL_HAS_ARMY_MAX cellHasArmyMaxParam, const LandformInfoMap * landformInfoMapPtrParam);
public:
	virtual ~RabbiManager(void);

	// 函数描述：执行命令
	// @return 返回true / false ； true: 能够走  false：不能走
	bool thinking(bool isEven);

	// 函数描述：执行命令
	// @param AiStrategy 策略类型
	// @return 无
	void setStrategy(enumAIStrategy AiStrategy);

private:
	RabbiManager();
	IArmyPtrvectInvect * mapDataPtr;
	SeekPath * seekPathArithmPtr;
	IArmyPtr  armyObjPtr;
	IRabbiStrategy * ptrRabbiStrategy[endStrategyNum];	
	enumAIStrategy AiStrategy;
};

#endif
