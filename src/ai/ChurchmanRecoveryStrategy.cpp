// @brief ��Ҫ����: ��ʦ��������
// @author ����: �� �� ��

#include "ChurchmanRecoveryStrategy.h"
#include "ArmyAction.h"
#include "AppetentAction.h"
#include "PatrolFunSingleton.h"

ChurchmanRecoveryStrategy::ChurchmanRecoveryStrategy(void)
{
}

ChurchmanRecoveryStrategy::~ChurchmanRecoveryStrategy(void)
{
}

bool ChurchmanRecoveryStrategy::thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven)
{	
	if (NULL == strategyMgr)
	{
		return false;
	}

	managerParam &mgrParam = strategyMgr->getMgrParam();

	AiLevelParam *aiLevelParam = mgrParam.aiLevelParam;   // AI�ȼ�����

	if (NULL == aiLevelParam)
	{
#ifdef _DEBUG
		assert(0 && "AI�ȼ������Ľṹָ��ֵΪ��");
#endif
		return true;
	}

	int randomVal = 0;  // �����ֵ

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

	// �����ִ�к�������Ļ�,����ִ�к�����������
	if (mgrParam.isCountermarch)
	{
		if (isEven)
		{
			return true;
		}

		mgrParam.PatrolStruct.isPatrol = false;

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, mgrParam.isCountermarch);	

		return true;
	}

	// ��ȡ�û����õĸ���Ŀ��
	IArmyPtr followObjPtr = armyObj->GetFollowTarget();
	PosXYStru posMoveTo;
	bool isSucess = false;

	listArmyPtr lstFriends;
	IArmyPtr minHPArmy = NULL;
	unsigned int attackScope = armyObj->GetAttackScope();

	// �ж��ڹ�����Χ���Ƿ�����Ҫ��Ѫ���Ѿ�
	int success = CommonFunctionSingleton::getInstance()->getGroupArmy(mapDataParamPtr, armyObj,armyObj->GetAttackScope(), lstFriends);

	lstFriends.push_back(armyObj);

	if (!success)
	{
		return false;
	}

	if (!lstFriends.empty())
	{
		minHPArmy = CommonFunctionSingleton::getInstance()->getMinHPArmy(lstFriends);
	}

	// �ȴ����Ѫ
	if (minHPArmy)
	{
		mgrParam.PatrolStruct.isPatrol = false;

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

		if (enemyPosVect.empty())
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

	if (isEven)
	{
		return true;
	}

	// ��ȡ��󹥻��Լ��ĵо�
	IArmyPtr attackMeArmy  = armyObj->GetLastEnmityTarget();

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
	
	// �Լ�����, ѡ���ƶ�����ȫ�㡣
	if (isNeedMoveToSecurityPos)
	{
		isSucess = CommonFunctionSingleton::getInstance()->MoveToSecurityPos(mapDataParamPtr, seekPathArithmParamPtr, armyObj, attackMeArmy, posMoveTo);

		if (!isSucess)
		{
			return false;
		}

		mgrParam.PatrolStruct.isPatrol = false;

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

		if (NULL == aiLevelParamTmp)
		{
#ifdef _DEBUG
			assert(0);
#endif
			return false;
		}

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParamTmp->tipTopAttackTar, 
			aiLevelParamTmp->isDynamicSeekPath, false, false, true);

		return true;
	}

	// ����û�û�����õĸ���Ŀ��������õĸ���Ŀ������ʱ
	if (NULL == followObjPtr || 0 == followObjPtr->GetCurrentHp())
	{
		return getFollowTarAndFollow(mapDataParamPtr, armyObj, strategyMgr, seekPathArithmParamPtr, isEven);
	}
	// ���򣬸���ָ����Ŀ��
	else
	{
		return followTar(followObjPtr, armyObj, strategyMgr, mapDataParamPtr, seekPathArithmParamPtr, isEven);
	}
}


bool ChurchmanRecoveryStrategy::getFollowTarAndFollow( IArmyPtrvectInvect * mapDataParamPtr, IArmyPtr armyObj, IManager * strategyMgr, SeekPath * seekPathArithmParamPtr, bool isEven) const 
{
	if (NULL == strategyMgr)
	{
		return false;
	}

	managerParam &mgrParam = strategyMgr->getMgrParam();

	IArmyPtr followArmyObj = NULL;

	// �ж���Ұ���Ƿ���ڵ���ս���ӣ�սʿ-���֣�Ϊ�������
	bool isSucess = CommonFunctionSingleton::getInstance()->getFollowObjPtr(mapDataParamPtr, armyObj, armyObj->GetViewScope(), followArmyObj);

	// ��Ұ��û�к��ʵĸ������
	if (NULL == followArmyObj)
	{
		// ����,������ҪѲ��
		if (! mgrParam.PatrolPosVectPtr->empty())
		{
			isSucess = PatrolFunSingleton::getInstance()->executePatrol(mapDataParamPtr, seekPathArithmParamPtr, armyObj, strategyMgr, isEven);

			if (!isSucess)
			{
				return false;
			}
		}
		// �������, ����AI��ѡ�����
		else if (Counterpart_AI !=  mgrParam.useAiScene)
		{

			PosXYStru posXY;
			CommonFunctionSingleton::getInstance()->GetPosOfCountermarch(mapDataParamPtr, mgrParam.ClassImpedientPtr, armyObj, posXY);

			FunParam funParam(mapDataParamPtr, 
								seekPathArithmParamPtr, 
								 mgrParam.armyCurrentStatus, 
								 mgrParam.movePosVector, 
								 mgrParam.moveTagetPos,
								 mgrParam.enemyID,
								armyObj,
								strategyMgr);

			moveFunParam attOrMoveFunParm(funParam, posXY);

			CommonFunctionSingleton::getInstance()->setMoveTargetPos(attOrMoveFunParm);

			AiLevelParam *aiLevelParamTmp =  mgrParam.aiLevelParam;


			if (NULL == aiLevelParamTmp)
			{
#ifdef _DEBUG
				assert(0);
#endif
				return false;

			}

			AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
			AppetentActionObj.action(isEven, mgrParam.enemyID,  mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParamTmp->tipTopAttackTar, aiLevelParamTmp->isDynamicSeekPath);

			return true;
		}
	}
	// �������ѡ�еĸ������
	else
	{
		// �������ò��Ӹ���Ŀ��
		armyObj->SetFollowTarget(followArmyObj);

		// ��ȡ���沿�ӵ�����
		const vector<POS_STRUCT> &vectorPosTaraget = followArmyObj->GetArmyPos();

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
			 mgrParam.PatrolStruct.isPatrol = false;

			FunParam funParam(mapDataParamPtr, 
								seekPathArithmParamPtr, 
								 mgrParam.armyCurrentStatus, 
								 mgrParam.movePosVector, 
								 mgrParam.moveTagetPos,
								 mgrParam.enemyID,
								armyObj,
								strategyMgr);

			moveFunParam attOrMoveFunParm(funParam,	posTarget);

			CommonFunctionSingleton::getInstance()->setMoveTargetPos(attOrMoveFunParm);

			AiLevelParam *aiLevelParamTmp =  mgrParam.aiLevelParam;

			if (NULL == aiLevelParamTmp)
			{
#ifdef _DEBUG
			assert(0 && "ָ��Ϊ��");
#endif
				return false;
			}

			AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
			AppetentActionObj.action(isEven, mgrParam.enemyID,  mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParamTmp->tipTopAttackTar, aiLevelParamTmp->isDynamicSeekPath);		
		}
	}

	return true;
}

bool ChurchmanRecoveryStrategy::followTar(const IArmyPtr followObjPtr, IArmyPtr armyObj, IManager * strategyMgr, IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, bool isEven) const
{
	if (NULL == strategyMgr)
	{
		return false;
	}

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

	if (distantVal <= attackScope)
	{
		return true;
	}
	else
	{
		mgrParam.PatrolStruct.isPatrol = false;

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
			assert(0 && "ָ��Ϊ��");
#endif
			return false;
		}

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParamTmp->tipTopAttackTar, aiLevelParamTmp->isDynamicSeekPath);	
	}

	return true;
}