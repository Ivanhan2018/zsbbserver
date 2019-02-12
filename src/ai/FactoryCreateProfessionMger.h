// @brief ��Ҫ����: ����������������Ӧְҵ�Ĳ��Թ�����
// @author ����: �� �� ��

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

	// �������������������ֵĲ��Թ�����
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @return ���ع����ֲ��Թ�����������ָ��
	ManagerPtr CreateBowArrowFigStrategyMgr(IArmyPtrvectInvect * mapDataParamPtr,  CLoadMapInfo *ClassImpedientPtr, SeekPath * seekPathArithmPtr, IArmyPtr armyObjPtr, 
		PatrolPosVect *PatrolPosVectParamPtr, ENUM_USE_SCENE useAiSceneParam, AiLevelParam *aiLevelParamTmp, CELL_HAS_ARMY_MAX cellHasArmyMax, 
		const LandformInfoMap *landformInfoMapPtrParam) const;

	// ����������������ʦ�Ĳ��Թ�����
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @return ������ʦ���Թ�����������ָ��
	ManagerPtr CreateChurchmanStrategyMgr(IArmyPtrvectInvect * mapDataParamPtr, CLoadMapInfo *ClassImpedientPtr,  SeekPath * seekPathArithmPtr, IArmyPtr armyObjPtr, 
		PatrolPosVect *PatrolPosVectParamPtr, ENUM_USE_SCENE useAiSceneParam, AiLevelParam *aiLevelParamTmp, CELL_HAS_ARMY_MAX cellHasArmyMax, 
		const LandformInfoMap *landformInfoMapPtrParam) const;
	
	// ��������������սʿ�Ĳ��Թ�����
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @return ����սʿ���Թ�����������ָ��
	ManagerPtr CreateCombatantStrategyMgr(IArmyPtrvectInvect * mapDataParamPtr,  CLoadMapInfo *ClassImpedientPtr, SeekPath * seekPathArithmPtr, IArmyPtr armyObjPtr, 
		PatrolPosVect *PatrolPosVectParamPtr, ENUM_USE_SCENE useAiSceneParam,  AiLevelParam *aiLevelParamTmp, CELL_HAS_ARMY_MAX cellHasArmyMax, 
		const LandformInfoMap *landformInfoMapPtrParam) const;

	// ����������������ʦ�Ĳ��Թ�����
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @return ���ط�ʦ���Թ�����������ָ��
	ManagerPtr CreateRabbiStrategyMgr(IArmyPtrvectInvect * mapDataParamPtr, CLoadMapInfo *ClassImpedientPtr,  SeekPath * seekPathArithmPtr, IArmyPtr armyObjPtr,
		PatrolPosVect *PatrolPosVectParamPtr, ENUM_USE_SCENE useAiSceneParam,  AiLevelParam *aiLevelParamTmp, CELL_HAS_ARMY_MAX cellHasArmyMax, 
		const LandformInfoMap *landformInfoMapPtrParam) const;

};

#endif
