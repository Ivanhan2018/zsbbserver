// @brief ��Ҫ����: ��ʦ��������
// @author ����: �� �� ��

#include "ChurchmanSelfdefenceStrategy.h"
#include "AppetentAction.h"
#include "PatrolFunSingleton.h"

ChurchmanSelfdefenceStrategy::ChurchmanSelfdefenceStrategy()
{
}

ChurchmanSelfdefenceStrategy::~ChurchmanSelfdefenceStrategy(void)
{
}

bool ChurchmanSelfdefenceStrategy::thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven)
{
	if (NULL == strategyMgr)
	{
		return false;
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
	if (war != armyObj->getMapState()  && Counterpart_AI != mgrParam.useAiScene )
	{
		if (isEven)
		{
			return true;
		}

		if (!mgrParam.PatrolPosVectPtr->empty())
		{
			bool isSucess = PatrolFunSingleton::getInstance()->executePatrol(mapDataParamPtr, seekPathArithmParamPtr, armyObj, strategyMgr, isEven);

			if (!isSucess)
			{
				return false;
			}
		}

		return true;
	}

	unsigned int attackScope = armyObj->GetAttackScope();
	listArmyPtr lstFriends;
	bool isSucess= CommonFunctionSingleton::getInstance()->getGroupArmy(mapDataParamPtr, armyObj,armyObj->GetViewScope(), lstFriends);

	if (!isSucess)
	{
		return false;
	}

	lstFriends.push_back(armyObj);

	if (!lstFriends.empty())
	{		
		IArmyPtr minHPArmy = CommonFunctionSingleton::getInstance()->getMinHPArmy(lstFriends);

		if (NULL == minHPArmy)
		{
			return true;
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

		const std::vector<POS_STRUCT> &armyPosVect = armyObj->GetArmyPos();
		const std::vector<POS_STRUCT> &enemyPosVect = minHPArmy->GetArmyPos();

		if (enemyPosVect.empty() || armyPosVect.empty())
		{
			return false;
		}

		if (!CommonFunctionSingleton::getInstance()->enemyIsInAttackScope(armyPosVect, enemyPosVect, attackScope))
		{
			if (isEven)
			{
				return true;
			}

			CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);
		}
		else
		{
			FunParam &funParam = attOrMoveFunParm.funParam;
			funParam.armyCurrentStatus = Attack_status;	
			funParam.enemyID = minHPArmy->GetArmyID();
		}

		//CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);

		AiLevelParam *aiLevelParamTmp = mgrParam.aiLevelParam;

#ifdef _DEBUG
		if (NULL == aiLevelParamTmp)
		{
			assert(0);
		}
#endif

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParamTmp->tipTopAttackTar, aiLevelParamTmp->isDynamicSeekPath);

		//return true;
	}

	if (isEven)
	{
		return true;
	}

	// ��ȡ��󹥻��Լ��ĵо�
	IArmyPtr attackMeArmy  = armyObj->GetLastEnmityTarget();

	PosXYStru posMoveTo;
	bool isNeedMoveToSecurityPos = false;

	if (NULL != attackMeArmy
		&& 0 != attackMeArmy->GetCurrentHp()
		&& 0 != aiLevelParam->runAway)
	{
		int enemyAttackScope = attackMeArmy->GetAttackScope();

		const std::vector<POS_STRUCT> &enemyPosVect = attackMeArmy->GetArmyPos();
		const std::vector<POS_STRUCT> &armyPosVect = armyObj->GetArmyPos();

		if (enemyPosVect.empty() || armyPosVect.empty())
		{
			return false;
		}

		if (getDistanceTwoNode(armyPosVect[0].PosX, armyPosVect[0].PosY, enemyPosVect[0].PosX, enemyPosVect[0].PosY) <= enemyAttackScope)
		{
			isNeedMoveToSecurityPos = true;
		}
	}

	// �Լ�������Ҫ�ƶ�����ȫ�㡣
	if (isNeedMoveToSecurityPos)
	{
		isSucess = CommonFunctionSingleton::getInstance()->MoveToSecurityPos(mapDataParamPtr, seekPathArithmParamPtr, armyObj, attackMeArmy, posMoveTo);

		if (!isSucess)
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

		moveFunParam attOrMoveFunParm(funParam, posMoveTo);

		CommonFunctionSingleton::getInstance()->setMoveTargetPos(attOrMoveFunParm);

		AiLevelParam *aiLevelParamTmp = mgrParam.aiLevelParam;

#ifdef _DEBUG
		if (NULL == aiLevelParamTmp)
		{
			assert(0);
		}
#endif

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParamTmp->tipTopAttackTar, 
			aiLevelParamTmp->isDynamicSeekPath, false, false, true);

		return true;
	}

	// ����,������ҪѲ��
	if (!mgrParam.PatrolPosVectPtr->empty())
	{
		isSucess = PatrolFunSingleton::getInstance()->executePatrol(mapDataParamPtr, seekPathArithmParamPtr, armyObj, strategyMgr, isEven);

		if (!isSucess)
		{
			return false;
		}
	}

	return true;
}


