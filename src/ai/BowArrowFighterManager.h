// @brief 简要描述: 弓箭手策略管理器
// @author 作者: 黄 文 才

#ifndef _BOW_ARROW_FIGHTERMANAGER_
#define _BOW_ARROW_FIGHTERMANAGER_

#include "IManager.h"
#include "AiCommon.h"
#include "SeekPathArithm.h"

class IBowArrowFigStrategy;
class BowArrowFigRecoveryStrategy;
class BowArrowFigAttackStrategy;
class BowArrowFigSelfdefStrategy;
class BowArrowFigDefendStrategy;

class BowArrowFighterManager : public IManager
{
public:
	BowArrowFighterManager(IArmyPtrvectInvect * mapDataParamPtr, 
						    CLoadMapInfo *ClassImpedientPtr,
							SeekPath * seekPathArithmParamPtr, 
							IArmyPtr armyObjParamPtr, 
							PatrolPosVect *PatrolPosVectParamPtr,
							ENUM_USE_SCENE useAiSceneParam,
							AiLevelParam *aiLevelParamTmp,
							CELL_HAS_ARMY_MAX cellHasArmyMaxParam,
							const LandformInfoMap *landformInfoMapPtrParam);
public:
	virtual ~BowArrowFighterManager(void);

	// 函数描述：执行命令
	// @return 返回true / false ； true: 能够走  false：不能走
	bool thinking(bool isEven);

	// 函数描述：执行命令
	// @param AiStrategy 策略类型
	// @return 无
	void setStrategy(enumAIStrategy AiStrategy);
private:
	IBowArrowFigStrategy * ptrBowArrowFigStrategy[endStrategyNum];
	enumAIStrategy AiStrategy;

	IArmyPtrvectInvect * mapDataPtr;
	SeekPath * seekPathArithmPtr;
	IArmyPtr  armyObjPtr;
};

#endif
