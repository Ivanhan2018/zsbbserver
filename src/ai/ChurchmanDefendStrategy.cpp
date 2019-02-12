// @brief ��Ҫ����: ��ʦפ�ز���
// @author ����: �� �� ��

#include "ChurchmanDefendStrategy.h"
#include "AppetentAction.h"


ChurchmanDefendStrategy::ChurchmanDefendStrategy()
{
}

ChurchmanDefendStrategy::~ChurchmanDefendStrategy(void)
{
}

bool ChurchmanDefendStrategy::thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven)
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

	listArmyPtr lstFriends;

	bool success = CommonFunctionSingleton::getInstance()->getGroupArmy(mapDataParamPtr, armyObj, armyObj->GetAttackScope(), lstFriends);

	lstFriends.push_back(armyObj);
	
	if (!success)
	{
		return false;
	}

	lstFriends.push_back(armyObj);

	if (!lstFriends.empty())
	{
		IArmyPtr minHPArmy = CommonFunctionSingleton::getInstance()->getMinHPArmy(lstFriends);

		if (NULL == minHPArmy)
		{
			return false;
		}
		
		FunParam funParam(mapDataParamPtr, 
							seekPathArithmParamPtr, 
							mgrParam.armyCurrentStatus, 
							mgrParam.movePosVector, 
							mgrParam.moveTagetPos,
							mgrParam.enemyID,
							armyObj,
							strategyMgr);

		attackFunParam attOrMoveFunParm(funParam, minHPArmy->GetArmyID());

		CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID,mgrParam.movePosVector, mgrParam.armyCurrentStatus);

	}

	return true;
}
