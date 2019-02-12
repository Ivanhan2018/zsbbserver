// @brief ��Ҫ����: ������פ�ز���
// @author ����: �� �� ��

#include "BowArrowFigDefendStrategy.h"
#include "AppetentAction.h"

BowArrowFigDefendStrategy::BowArrowFigDefendStrategy(void)
{
}

BowArrowFigDefendStrategy::~BowArrowFigDefendStrategy(void)
{
}

bool BowArrowFigDefendStrategy::thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven)
{
	if (NULL == strategyMgr)
	{
		return false;
	}

	if (!isEven)
	{
		return true;
	}

	managerParam &mgrParam = strategyMgr->getMgrParam();

	AiLevelParam *aiLevelParam = mgrParam.aiLevelParam;   // AI�ȼ�����
	int randomVal = 0;  // �����ֵ

	if (NULL == aiLevelParam)
	{
#ifdef _DEBUG
		assert(0 && "AI�ȼ������Ľṹָ��ֵΪ��");
#endif
		return true;
	}

	// ��ȡ������������õ���ͣ���ʱȽ�
	randomVal = CommonFunctionSingleton::getInstance()->GetRandomValFromOneToHundred();

	if (randomVal <= aiLevelParam->pauseProbability)
	{
		return true;
	}

	// ��ƽ״̬�£�ֻ��ִ��Ѳ��
	if (war != armyObj->getMapState() && Counterpart_AI != mgrParam.useAiScene )
	{
		//#ifdef _DEBUG
		//		assert(0 && "��ƽ״̬�²������ý�����AI");
		//#endif
		return true;
	}

	// ��ȡ����ڵĵо�
	IArmyPtr EnemyInGunshotPtr  = CommonFunctionSingleton::getInstance()->getGunshotEnemyToAttack(mapDataParamPtr, armyObj, aiLevelParam->tipTopAttackTar);

	// �������ڳ����˵о����򹥻�
	if (NULL != EnemyInGunshotPtr)
	{
		FunParam funParam(mapDataParamPtr, 
							seekPathArithmParamPtr, 
							mgrParam.armyCurrentStatus, 
							mgrParam.movePosVector, 
							mgrParam.moveTagetPos,
							mgrParam.enemyID,
							armyObj,
							strategyMgr);

		attackFunParam attOrMoveFunParm(funParam, EnemyInGunshotPtr->GetArmyID());

		CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar);
	}

	return true;
}

