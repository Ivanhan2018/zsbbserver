// @brief ��Ҫ����: սʿ���Թ�����
// @author ����: �� �� ��

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
	
	// ����������ִ������
	// @return ����true / false �� true: �ܹ���  false��������
	bool thinking(bool isEven);

	// ����������ִ������
	// @param AiStrategy ��������
	// @return ��
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

