#ifndef _I_STRATEGY_MANAGER_H_H_H
#define _I_STRATEGY_MANAGER_H_H_H

#include "AiCommon.h"

struct managerParam
{
	ENUM_ARMY_STATUS armyCurrentStatus;   // �������й��µ�״̬

	vector<PosXYStru> movePosVector;

	PosXYStru moveTagetPos;

	unsigned int enemyID;   // �о�

	IArmyPtr armyPtr;   // ������

	unsigned int calledAttackEnemyID;  // ������ʱ��Ҫ�����ĵо�

	bool isCountermarch; // �Ƿ�ִ�к�������

	PatrolPosVect *PatrolPosVectPtr;  // Ѳ�ߵ�ṹ������

	ENUM_USE_SCENE useAiScene; // ����AIʹ�ó���

	aistrategy::AiLevelParam *aiLevelParam; // AI�ȼ�����	
	
	const LandformInfoMap *landformInfoMap;	 // ����Ӱ��

	struct LandformAffStru
	{
		unsigned int boutSumWeight;			// �ۻ����ж�ֵ
	};

	LandformAffStru landformAffStru;        // ����Ӱ���ƶ������ݽṹ

	struct PatrolStru
	{
		bool isPatrol;		 // ��ǰ�Ƿ�ִ��Ѳ��
		PosXYStru patrol;    // ��ǰѲ��ǰ����Ѳ�ߵ�
	};

	PatrolStru PatrolStruct;	// Ѳ�����ݽṹ

	CELL_HAS_ARMY_MAX cellHasArmyMax; // һ�����������վ��������

	CLoadMapInfo *ClassImpedientPtr;
};

class IManager
{
public:
	virtual ~IManager(void){};

	virtual bool thinking(bool isEven) = 0;

	virtual void setStrategy(enumAIStrategy AiStrategy) = 0;

	managerParam & getMgrParam()
	{
		return mgrParam;
	}

	void changePatrolPosVect(const aistrategy::AiLevelParam *aiLevelParamTmp, PatrolPosVect * PatrolPosVectParamPtr) const
	{
		if (NULL == aiLevelParamTmp || NULL == PatrolPosVectParamPtr)
		{
			assert(0);
			return;
		}

		if (1 == aiLevelParamTmp->turnPathMode)
		{
			PatrolPosVect patrolPosVect = *PatrolPosVectParamPtr;

			if (1 < patrolPosVect.size())
			{
				PatrolPosVect::iterator itBeg = patrolPosVect.begin();
				PatrolPosVect::iterator itEnd = patrolPosVect.end();

				itEnd -= 2;

				for (; itBeg!=itEnd; --itEnd)
				{
					PatrolPosVectParamPtr->insert(PatrolPosVectParamPtr->end(), *itEnd);
				}
			}
		}	
	}

private:
	managerParam mgrParam;
};

#endif
