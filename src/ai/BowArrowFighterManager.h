// @brief ��Ҫ����: �����ֲ��Թ�����
// @author ����: �� �� ��

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

	// ����������ִ������
	// @return ����true / false �� true: �ܹ���  false��������
	bool thinking(bool isEven);

	// ����������ִ������
	// @param AiStrategy ��������
	// @return ��
	void setStrategy(enumAIStrategy AiStrategy);
private:
	IBowArrowFigStrategy * ptrBowArrowFigStrategy[endStrategyNum];
	enumAIStrategy AiStrategy;

	IArmyPtrvectInvect * mapDataPtr;
	SeekPath * seekPathArithmPtr;
	IArmyPtr  armyObjPtr;
};

#endif
