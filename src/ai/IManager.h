#ifndef _I_STRATEGY_MANAGER_H_H_H
#define _I_STRATEGY_MANAGER_H_H_H

#include "AiCommon.h"

struct managerParam
{
	ENUM_ARMY_STATUS armyCurrentStatus;   // 部队在托管下的状态

	vector<PosXYStru> movePosVector;

	PosXYStru moveTagetPos;

	unsigned int enemyID;   // 敌军

	IArmyPtr armyPtr;   // 自身部队

	unsigned int calledAttackEnemyID;  // 被呼叫时需要攻击的敌军

	bool isCountermarch; // 是否执行后退命令

	PatrolPosVect *PatrolPosVectPtr;  // 巡逻点结构体向量

	ENUM_USE_SCENE useAiScene; // 策略AI使用场景

	aistrategy::AiLevelParam *aiLevelParam; // AI等级参数	
	
	const LandformInfoMap *landformInfoMap;	 // 地形影响

	struct LandformAffStru
	{
		unsigned int boutSumWeight;			// 累积的行动值
	};

	LandformAffStru landformAffStru;        // 地形影响移动的数据结构

	struct PatrolStru
	{
		bool isPatrol;		 // 当前是否执行巡逻
		PosXYStru patrol;    // 当前巡逻前往的巡逻点
	};

	PatrolStru PatrolStruct;	// 巡逻数据结构

	CELL_HAS_ARMY_MAX cellHasArmyMax; // 一个格子最多能站几个部队

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
