// @brief ��Ҫ����: ��ʦ���Թ�����
// @author ����: �� �� ��

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

	// ����������ִ������
	// @return ����true / false �� true: �ܹ���  false��������
	bool thinking(bool isEven);

	// ����������ִ������
	// @param AiStrategy ��������
	// @return ��
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
