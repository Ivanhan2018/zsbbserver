// @brief ��Ҫ����: ��ʦ��������
// @author ����: �� �� ��

#include "ChurchmanAttackStrategy.h"
#include "AppetentAction.h"

ChurchmanAttackStrategy::ChurchmanAttackStrategy()
{
}

ChurchmanAttackStrategy::~ChurchmanAttackStrategy(void)
{
}

bool ChurchmanAttackStrategy::thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven)
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
//#ifdef _DEBUG
//		assert(0 && "��ƽ״̬�²������ý�����AI");
//#endif
		return true;
	}


	// �����ִ�к�������Ļ�,����ִ�к�����������
	if (mgrParam.isCountermarch)
	{
		if (isEven)
		{
			return true;
		}

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID,mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath, mgrParam.isCountermarch);	
		
		return true;
	}

	listArmyPtr lstFriends;

	IArmyPtr minHPArmy = NULL;

	// �ж��ڹ�����Χ���Ƿ�����Ҫ��Ѫ���Ѿ�
	bool isSucess = CommonFunctionSingleton::getInstance()->getGroupArmy(mapDataParamPtr, armyObj, armyObj->GetAttackScope(), lstFriends);

	lstFriends.push_back(armyObj);

	if (!isSucess)
	{
		return false;
	}

	lstFriends.push_back(armyObj);

	if (!lstFriends.empty())
	{
		minHPArmy = CommonFunctionSingleton::getInstance()->getMinHPArmy(lstFriends);

#ifdef _DEBUG
		if (NULL != minHPArmy)
		{
			int hurtVal = minHPArmy->getCurHurtHp();

			assert(0 != hurtVal);
		}
#endif

	}

	// �ȴ����Ѫ
	if (NULL != minHPArmy)
	{	
		if (!isEven)
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

		CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath);	

	}

	if (isEven)
	{
		return true;
	}

	// ��ȡ��󹥻��Լ��ĵо�
	IArmyPtr attackMeArmy  = armyObj->GetLastEnmityTarget();

	// �������
	IArmyPtr followObjPtr = armyObj->GetFollowTarget();
	PosXYStru posMoveTo;
	bool isNeedMoveToSecurityPos = false;
	bool isNeedFollowQuick = false;

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

	if (NULL != followObjPtr)
	{
		const std::vector<POS_STRUCT> &followPosVect = followObjPtr->GetArmyPos();
		const std::vector<POS_STRUCT> &armyPosVect = armyObj->GetArmyPos();

		if (followPosVect.empty() || armyPosVect.empty())
		{
#ifdef _DEBUG
			assert(0);
#endif
			return false;
		}

		if (getDistanceTwoNode(armyPosVect[0].PosX, armyPosVect[0].PosY, followPosVect[0].PosX, followPosVect[0].PosY) > armyObj->GetAttackScope())
		{
			isNeedFollowQuick = true;
		}
	}

	// �Լ�������Ҫ�ƶ�����ȫ�㡣
	if (isNeedMoveToSecurityPos && !isNeedFollowQuick)
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


		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, 
			aiLevelParam->isDynamicSeekPath, false, false, true);

		return true;
	}

	// ���֮ǰû�и�����󣬻���������������
	if (NULL == followObjPtr || 0 == followObjPtr->GetCurrentHp())
	{
		return getFollowTarAndFollowOrBack(followObjPtr, mapDataParamPtr, armyObj, strategyMgr, seekPathArithmParamPtr, isEven);

	}
	// ���򣬸���ָ����Ŀ��
	else
	{
		return followTarget(followObjPtr, armyObj, mapDataParamPtr, seekPathArithmParamPtr, strategyMgr, isEven);
	}
}

bool ChurchmanAttackStrategy::getFollowTarAndFollowOrBack( IArmyPtr &followObjPtr, IArmyPtrvectInvect * mapDataParamPtr, IArmyPtr armyObj, IManager * strategyMgr, SeekPath * seekPathArithmParamPtr, bool isEven)
{
	followObjPtr = NULL;

	managerParam &mgrParam = strategyMgr->getMgrParam();

	bool isSucess = CommonFunctionSingleton::getInstance()->getFollowObjPtr(mapDataParamPtr, armyObj, armyObj->GetViewScope(), followObjPtr);

	if (!isSucess)
	{
		return false;
	}

	// ��Ұ��Χ��û�к��ʵĸ������ѡ����˵���½��.�����в����� 
	if (NULL == followObjPtr && Counterpart_AI != mgrParam.useAiScene)
	{
		mgrParam.isCountermarch = true;

		POS_STRUCT posXY=armyObj->GetBirthPos();

		PosXYStru posTarget;
		posTarget.xPos = posXY.PosX;
		posTarget.yPos = posXY.PosY;

		FunParam funParam(mapDataParamPtr, 
							seekPathArithmParamPtr, 
							mgrParam.armyCurrentStatus, 
							mgrParam.movePosVector, 
							mgrParam.moveTagetPos,
							mgrParam.enemyID,
							armyObj,
							strategyMgr);

		moveFunParam attOrMoveFunParm(funParam, posTarget);

		CommonFunctionSingleton::getInstance()->setMoveTargetPos(attOrMoveFunParm);

		AiLevelParam *aiLevelParamTmp = mgrParam.aiLevelParam;

#ifdef _DEBUG
		if (NULL == aiLevelParamTmp)
		{
			assert(0);
		}
#endif


		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParamTmp->tipTopAttackTar, aiLevelParamTmp->isDynamicSeekPath);	

		return true;
	}
	// �������ѡ�еĸ������
	else if (NULL != followObjPtr)
	{
		// ���ø������
		armyObj->SetFollowTarget(followObjPtr);

		// ��ȡ���沿�ӵ�����
		const vector<POS_STRUCT> &vectorPosTaraget = followObjPtr->GetArmyPos();

		const vector<POS_STRUCT> &vectorPosArmy = armyObj->GetArmyPos();

		int attackScope = armyObj->GetAttackScope();
		int distantVal = 0;

		PosXYStru posTarget, posArmy;
		posTarget.xPos = vectorPosTaraget[0].PosX;
		posTarget.yPos = vectorPosTaraget[0].PosY;

		posArmy.xPos = vectorPosArmy[0].PosX;
		posArmy.yPos = vectorPosArmy[0].PosY;

		distantVal = getDistanceTwoNode(posArmy.xPos, posArmy.yPos, posTarget.xPos, posTarget.yPos);

		if (distantVal <= attackScope)
		{
			return true;
		}
		else
		{
			FunParam funParam(mapDataParamPtr, 
								seekPathArithmParamPtr, 
								mgrParam.armyCurrentStatus, 
								mgrParam.movePosVector, 
								mgrParam.moveTagetPos,
								mgrParam.enemyID,
								armyObj,
								strategyMgr);

			moveFunParam attOrMoveFunParm(funParam, posTarget);

			CommonFunctionSingleton::getInstance()->setMoveTargetPos(attOrMoveFunParm);

			AiLevelParam *aiLevelParamTmp = mgrParam.aiLevelParam;

			if (NULL == aiLevelParamTmp)
			{
#ifdef _DEBUG
			assert(0);
#endif
				return false;

			}

			AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
			AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParamTmp->tipTopAttackTar, aiLevelParamTmp->isDynamicSeekPath);	
		}
	}

	return true;
}

bool ChurchmanAttackStrategy::followTarget(const IArmyPtr followObjPtr, IArmyPtr armyObj, IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IManager * strategyMgr, bool isEven) const
{
	managerParam &mgrParam = strategyMgr->getMgrParam();

	const vector<POS_STRUCT> &vectorPosTaraget = followObjPtr->GetArmyPos();
	const vector<POS_STRUCT> &vectorPosArmy = armyObj->GetArmyPos();
	int attackScope = armyObj->GetAttackScope();
	int distantVal = 0;

	PosXYStru posTarget, posArmy;
	posTarget.xPos = vectorPosTaraget[0].PosX;
	posTarget.yPos = vectorPosTaraget[0].PosY;

	posArmy.xPos = vectorPosArmy[0].PosX;
	posArmy.yPos = vectorPosArmy[0].PosY;

	distantVal = getDistanceTwoNode(posArmy.xPos, posArmy.yPos, posTarget.xPos, posTarget.yPos);

	// �������Ķ������Լ�������ڣ��Ͳ����ƶ�
	if (distantVal <= attackScope)
	{
		return true;
	}
	else
	{
		FunParam funParam(mapDataParamPtr, 
						seekPathArithmParamPtr, 
						mgrParam.armyCurrentStatus, 
						mgrParam.movePosVector, 
						mgrParam.moveTagetPos,
						mgrParam.enemyID,
						armyObj,
						strategyMgr);

		moveFunParam attOrMoveFunParm(funParam, posTarget);

		CommonFunctionSingleton::getInstance()->setMoveTargetPos(attOrMoveFunParm);

		AiLevelParam *aiLevelParamTmp = mgrParam.aiLevelParam;

		if (NULL == aiLevelParamTmp)
		{
#ifdef _DEBUG
		assert(0);
#endif
			return false;

		}
		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParamTmp->tipTopAttackTar, aiLevelParamTmp->isDynamicSeekPath);	
	}

	return true;
}