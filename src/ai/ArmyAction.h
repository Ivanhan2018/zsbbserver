// @brief ��Ҫ����: ����ӿ��࣬ʵ��AI����
// @author ����: �� �� ��

#ifndef _ARMACTION_SINGLETON_H_H
#define _ARMACTION_SINGLETON_H_H

#include "AiCommon.h"
#include "SeekPathArithm.h"
#include "FactoryCreateProfessionMger.h"
#include "AppetentAction.h"

enum
{
	ONE_CELL_VAL = 1,
};

class ArmyAction
{
public:
	ArmyAction(MapDataStru & mapDataAndFraiseParam, IArmyPtr armyPtrObj, ENUM_USE_SCENE enumUseScene, AiLevelParam *aiLevelParamTmp, SeekPath * seekPathArithmPtrParam,
			   const LandformInfoMap *landformInfoMapPtrParam);
	~ArmyAction();

	// ������������ȡ�����ڷ��й��µ�״̬
	// @return ����Attack_status��Move_status��Idle_status
	inline ENUM_ARMY_STATUS getArmStatus() const
	{
		return armyCurrentStatus;
	}

	// ������������ȡ�������й��µ�״̬
	// @return ����Attack_status��Move_status��Idle_status
	inline ENUM_ARMY_STATUS getArmyStatusOnTrusteeship() const
	{
		if (strategyManagerPtr)
		{
			return strategyManagerPtr->getMgrParam().armyCurrentStatus;
		}
		else
		{
			return Idle_status;
		}
	}

	// �������������ò����ƶ�����
	// @param moveTagetPosParam �ƶ���Ŀ���
	// @return ���óɹ����
	bool setMoveTargetPos(PosXYStru &moveTagetPosParam);

	// ��������������ִ������
	// @param isEven �Ƿ�Ϊż����false������ֻ�����ƶ�    trueż����ֻ������
	// @return ����true / false ; true:�ܹ��ƶ��򹥻��� false:����
	bool thinking(bool isEven);

	// �������������ù�������
	// @param attackTargetPtr ���ӵĹ�������
	// @return ���óɹ����
	bool setAttackTarget(const IArmyPtr attackTargetPtr);

	// �����������ú��������ڣ�ָ�����������ŵĵ����꣬�ڸ�����������Χ������ʵĵ㣨�����ŵ���ǰ����
	// @param posInput ������������
	// @param posOutput ���������ʵĵ�����
	// @return ���óɹ����
	bool getAdaptiveWhereaboutsPos(const PosXYStru &posInput, PosXYStru &posOutput) const;

	// ����������ָ��ĳ�㣬���������Χ������ʵĵ㣨�ǽ��������û��ռ��5�ˣ��ǵо�վ�ĸ�
	// @param posInput ������������
	// @param posOutput ���������ʵĵ�����
	// @return ���óɹ����
	bool getAdaptivePos(const PosXYStru &posInput, PosXYStru &posOutput, unsigned int cellHasArmyNum = CELL_HAS_MAX_THREE_ARMY_NUM, unsigned int distanceVal = ONE_CELL_VAL);

	bool getNearPosInOneCircle(const PosXYStru &posInput, PosXYStru &posOutput);

	// �������������ò�������
	// @param AiStrategy ��������
	// @return ���óɹ����
	void setStrategy(enumAIStrategy AiStrategy) const;

	// ������������ȡ���ӵĲ��Թ�����
	// @return ���Թ�����ָ��
	inline IManager * getStrategyMgr() const
	{
		if (strategyManagerPtr)
		{
			return strategyManagerPtr.get();
		}
		else
		{
			return NULL;
		}

	}

	// ��������:����Ѳ�ߵ�
	// @param PatrolPosVectParam Ѳ�ߵ������
	// @return ��
	inline void setPatrolPoses(const PatrolPosVect &PatrolPosVectParam)
	{
		PatrolPosVector = PatrolPosVectParam;
	}

	inline bool isStaticBlook(int x, int y) const
	{
#ifdef _DEBUG
		if (NULL == mapDataAndFraise.ClassImpedient)
		{
			return false;
		}
#endif
		return (mapDataAndFraise.ClassImpedient)->IsBlockoff(x, y);
	}

	bool getSelfBlastAttArmys(const PosXYStru &pos, std::vector<IArmyPtr> &armysCenter, std::vector<IArmyPtr> &armysInOneCircle) const;
	bool getArmysInPos(const PosXYStru &pos, std::vector<IArmyPtr> &Armys) const;
private:
	ArmyAction();
	ArmyAction(const ArmyAction &armyAction);

	// ����������������Ӧ�Ĳ��Թ�����
	// @return ��
	void CreateStrategyManger();

	// ��������:У�鵱ǰ���Ƿ����Ҫ�󣨷ǽ��������û��ռ��5�ˣ��ǵо�վ�ĸ�
	// @param posXY ��ҪУ��ĵ�����
	// @param loadMapInfoPtr ���ӵ�ͼ����ָ��
	// @param armyInvect �ϰ����ͼ����ָ��
	// @return �Ƿ�����
	bool isAdativePos(const PosXYStru &posXY, const CLoadMapInfo *loadMapInfoPtr, const IArmyPtrvectInvect &armyInvect, unsigned int cellHasArmyNum);

	MapDataStru mapDataAndFraise;
	SeekPath * seekPathArithmPtr;
	
	const LandformInfoMap *landformInfoMapPtr_;	 // ����Ӱ��

	ENUM_ARMY_STATUS armyCurrentStatus;   // �����ڷ��й��µ�״̬

	vector<PosXYStru> movePosVector;

	PosXYStru moveTagetPos;

	unsigned int enemyID;

	IArmyPtr armyPtr;

	ManagerPtr strategyManagerPtr;

	PatrolPosVect PatrolPosVector;  // Ѳ�ߵ�ṹ������

	ENUM_USE_SCENE useAiScene; // ����AIʹ�ó���

	AiLevelParam aiLevelParam; // AI�ȼ�����

	CELL_HAS_ARMY_MAX cellHasArmyMax_;	// һ��������վ����ಿ����.Ŀǰ��������һ������վһ�����ӣ����ڿ�����Ϊ5������

};

#endif
