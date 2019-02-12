// @brief 简要描述: 牧师策略管理器
// @author 作者: 黄 文 才

#ifndef _CHURCHMANMANAGER_H_H
#define _CHURCHMANMANAGER_H_H

#include "IManager.h"
#include "AiCommon.h"
#include "SeekPathArithm.h"

class IChurchmanStrategy;
class ChurchmanRecoveryStrategy;
class ChurchmanAttackStrategy;
class ChurchmanSelfdefenceStrategy;
class ChurchmanDefendStrategy;

class ChurchmanManager : public IManager
{
public:
	ChurchmanManager(IArmyPtrvectInvect * mapDataParamPtr, CLoadMapInfo *ClassImpedientPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjParamPtr, PatrolPosVect *PatrolPosVectParamPtr, 
		ENUM_USE_SCENE useAiSceneParam, AiLevelParam *aiLevelParamTmp, CELL_HAS_ARMY_MAX cellHasArmyMaxParam, const LandformInfoMap *landformInfoMapPtrParam);
public:
	virtual ~ChurchmanManager(void);

	// 函数描述：执行命令
	// @return 返回true / false ； true: 能够走  false：不能走
	bool thinking(bool isEven);

	// 函数描述：执行命令
	// @param AiStrategy 策略类型
	// @return 无
	void setStrategy(enumAIStrategy AiStrategy);
private:
	IChurchmanStrategy * ptrChurchmanStrategy[endStrategyNum];
	enumAIStrategy AiStrategy;

	IArmyPtrvectInvect * mapDataPtr;
	SeekPath * seekPathArithmPtr;
	IArmyPtr  armyObjPtr;

	ChurchmanManager();
	ChurchmanManager(const ChurchmanManager&);
	ChurchmanManager &operator=(const ChurchmanManager&);
};

#endif
