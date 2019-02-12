// @brief ��Ҫ����: ��ʦ���Թ�����
// @author ����: �� �� ��

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

	// ����������ִ������
	// @return ����true / false �� true: �ܹ���  false��������
	bool thinking(bool isEven);

	// ����������ִ������
	// @param AiStrategy ��������
	// @return ��
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
