// @brief ��Ҫ����: ����AIʵ��
// @author ����: �� �� ��

#include "AppetentAction.h"
#include "CommonFun.h"
#include "SeekPathArithm.h"
#include "windows.h"
#include "log.hpp"

enum
{
	ONE_CELL_VAL = 1,
	TWO_CELL_VAL = 2,
};

AppetentAction::AppetentAction(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathParamPtr, IArmyPtr armyPtrParam, CELL_HAS_ARMY_MAX cellHasArmyMax)
: armyPtr(armyPtrParam), seekPathPtr(seekPathParamPtr) , mapDataPtr(mapDataParamPtr), increaseMoveTime_(0), cellHasArmyMax_(cellHasArmyMax)

{

}

AppetentAction::~AppetentAction(void)
{
}

bool AppetentAction::action(bool isEven,
							unsigned int &enemyID,
							vector<PosXYStru> &movePosVector,
							ENUM_ARMY_STATUS &armyCurrentStatus,
							SOLDIER_TYPE tipTopAttackTar, 
							int isDynamicSeekPath,
							bool isCountermarch,
							bool isPatrol,
							bool isChurchmanRunaway)
{
	if (NULL == armyPtr || NULL == seekPathPtr)
	{
		return false;
	}

	unsigned int playerId = armyPtr->getPlayerID();

	unsigned int moveSpeed = 0;

	IManager * strategyMgr = armyPtr->getStrategyMgr();

	if (NULL == strategyMgr)
	{
		//#ifdef _DEBUG
		//						assert(0 && "��ȡ���Թ�����ָ��ʧ��");
		//#endif
		return false;
	}
	if (isPatrol)
	{
		moveSpeed = PATROL_ARMY_MOVE_PACE;
	}
	else if (isChurchmanRunaway)
	{
		moveSpeed = (strategyMgr->getMgrParam()).aiLevelParam->runAway;
	}
	else
	{
		moveSpeed = armyPtr->getMoveSpeed();
	}		

	bool isUseSelfBlast = false;

	// ��ȡ���ӵ����
	int attackScope = armyPtr->GetAttackScope();	
	
	IArmyPtr enemyPtr = armyPtr->getArmyByID(enemyID);

	IArmy::CardUsedState cardUsedState = armyPtr->getCardState(SELFBLAST_CARD);

	// ʹ�����Ա���
	if (IArmy::IS_USING_CARD == cardUsedState)
	{
		isUseSelfBlast = true;
		attackScope = 1;
	}
	else if (IArmy::FORCE_OFF_CARD == cardUsedState)
	{
		// ʱ���ѵ���ǿ���Ա�
		attEnemyInAttScope(enemyPtr, armyPtr, isUseSelfBlast, true);

		return true;
	}

	bool isChurchmanFollow = false;

	managerParam & mgrParam = strategyMgr->getMgrParam();

	if (PRIEST == armyPtr->GetArmyOccupation()
		&& Move_status == armyCurrentStatus && armyPtr->GetTrusteeshipState())
	{
		isChurchmanFollow = true;
	}

	// ԭ�������Ķ����������,�򹥻�
	if (NULL != enemyPtr)
	{
		if (0 != enemyPtr->GetCurrentHp())
		{
			const std::vector<POS_STRUCT> &enemyPosVectTmp = enemyPtr->GetArmyPos();
			const std::vector<POS_STRUCT> &armyPosVectTmp = armyPtr->GetArmyPos();

			if (isEven && armyPtr->isCanUseWeapon() && !isChurchmanFollow)
			{
				if (enemyIsInAttackScope(armyPosVectTmp, enemyPosVectTmp, attackScope))
				{
					attEnemyInAttScope(enemyPtr, armyPtr, isUseSelfBlast);

					return true;
				}
			}
		}

		enemyPtr = NULL;
	}

	bool isNotWay = false;

	CommonFunctionSingleton * commonFunction = CommonFunctionSingleton::getInstance();

	if (NULL == commonFunction)
	{
#ifdef _DEBUG
		assert(0 && "��ȡ���ܺ�����������ʧ��");
#endif
		return false;
	}

	int movePosVectorSize = static_cast<int>(movePosVector.size());	

	int movePosVectorSizeTmp = movePosVectorSize;

	int iMin = min(moveSpeed, movePosVectorSize);

	PosXYStru nextPos;

	POS_STRUCT posStruct;

	IArmyPtr enemyPtrTmp = NULL;

	vector<POS_STRUCT> movePosVect;

	bool isCanGoNextPos = false;
	TrapStruct trapStruct;		// �Ƿ������˵о��������е����ݽṹ

	SOLDIER_TYPE soldierType = armyPtr->GetArmyOccupation();

	// ����״̬
	if (Attack_status == armyCurrentStatus)
	{
		// ����ߵ��о�������̼���
		movePosVectorSize -= attackScope;

		if (movePosVectorSize < 0)
		{
			movePosVectorSize = 0;
		}

		iMin = min(moveSpeed, movePosVectorSize);

		if (0 == iMin && NULL  != enemyPtr && BUILD == enemyPtr->GetArmyType())
		{
			iMin = 1;
		} 
	}

	if (!movePosVector.empty())
	{
		if (iMin > 0 && 1 == isDynamicSeekPath)
		{
			isNeedDynSeekAndGetPath(movePosVector, mgrParam, soldierType, enemyPtrTmp, movePosVectorSizeTmp, isNotWay, movePosVectorSize, iMin, moveSpeed, armyCurrentStatus);
		}

		for (int i=0; i<iMin; ++i)
		{
			isCanGoNextPos = isCanGo(movePosVector[i], mgrParam, enemyPtrTmp, trapStruct, soldierType, moveSpeed);

			if (NULL != enemyPtrTmp)
			{
				enemyID = enemyPtrTmp->GetArmyID();
				enemyPtr = enemyPtrTmp;
			}

			if (isCanGoNextPos)
			{
				posStruct.PosX = movePosVector[i].xPos;
				posStruct.PosY = movePosVector[i].yPos;

				movePosVect.push_back(posStruct);

				if (trapStruct.isInEnemyTrap)
				{
					break;
				}	
			}
			else
			{
				break;
			}
		}
	}
	movePosVectorSize = static_cast<int>(movePosVect.size());
	if (movePosVectorSize > 0)
	{
		if (!isEven && armyPtr->isCanMove())
		{	
			// ɾ��movePosVector���Ѿ�Ҫ�ߵ�����
			movePosVector.erase(movePosVector.begin(), movePosVector.begin() + movePosVectorSize);
			armyPtr->Move(movePosVect, armyPtr);
		}
	
		// �ƶ�������о���������,�������幥��֮
		if (isEven && trapStruct.isInEnemyTrap)
		{
#ifdef _DEBUG
			if (NULL == trapStruct.trapArmyPtr)
			{
				assert(0 && "����ָ��Ϊ����");
			}
#endif
			attEnemyInAttScope(armyPtr, trapStruct.trapArmyPtr, false);

			if (0 != increaseMoveTime_)
			{
				armyPtr->addMoveTime((-1) *increaseMoveTime_);
				increaseMoveTime_ = 0;
			}

			return true;
		}

		if (PRIEST == armyPtr->GetArmyOccupation())
		{
			if (0 != increaseMoveTime_)
			{
				armyPtr->addMoveTime((-1) *increaseMoveTime_);
				increaseMoveTime_ = 0;
			}

			return true;
		}

		if (NULL != enemyPtr && 0 != enemyPtr->GetCurrentHp())
		{
			const std::vector<POS_STRUCT> &enemyPosVect = enemyPtr->GetArmyPos();
			const std::vector<POS_STRUCT> &armyPosVect = armyPtr->GetArmyPos();

			if (enemyPosVect.empty() || armyPosVect.empty())
			{
				if (0 != increaseMoveTime_)
				{
					armyPtr->addMoveTime((-1) *increaseMoveTime_);
					increaseMoveTime_ = 0;
				}

				return false;
			}

			// �������,�򹥻�
			if (isEven && armyPtr->isCanUseWeapon() &&
				enemyIsInAttackScope(armyPosVect, enemyPosVect, attackScope) 
				&& !isChurchmanFollow)
			{
				attEnemyInAttScope(enemyPtr, armyPtr, isUseSelfBlast);

				if (0 != increaseMoveTime_)
				{
					armyPtr->addMoveTime((-1) *increaseMoveTime_);
					increaseMoveTime_ = 0;
				}

				return true;
			}
		}
		else
		{
			// ִ�к���������ٴ�����ڵĵо�
			if (isCountermarch)
			{
				if (0 != increaseMoveTime_)
				{
					armyPtr->addMoveTime((-1) *increaseMoveTime_);
					increaseMoveTime_ = 0;
				}

				return true;
			}

			// �й��µ����
			if (armyPtr->GetTrusteeshipState())
			{
				if (0 != increaseMoveTime_)
				{
					armyPtr->addMoveTime((-1) *increaseMoveTime_);
					increaseMoveTime_ = 0;
				}

				return true;
			}
			// ���й��£���Ҫ�ж���������޵о�
			else  if (NULL == enemyPtr && Move_status != armyCurrentStatus)
			{
				enemyPtr = commonFunction->getGunshotEnemyToAttack(mapDataPtr, armyPtr, tipTopAttackTar);

				if (NULL == enemyPtr)
				{
					if (0 != increaseMoveTime_)
					{
						armyPtr->addMoveTime((-1) *increaseMoveTime_);
						increaseMoveTime_ = 0;
					}

					return true;
				}

				// ����ڳ��ֵо����򹥻�
				if (isEven && armyPtr->isCanUseWeapon() && 0 != enemyPtr->GetCurrentHp() && !isChurchmanFollow)
				{	
					enemyID = enemyPtr->GetArmyID();

					attEnemyInAttScope(enemyPtr, armyPtr, false);

					if (0 != increaseMoveTime_)
					{
						armyPtr->addMoveTime((-1) *increaseMoveTime_);
						increaseMoveTime_ = 0;
					}
					return true;
				}
			}
		}

	}
	// �й������
	else if (armyPtr->GetTrusteeshipState() && NULL != enemyPtr && 0 != enemyPtr->GetCurrentHp())
	{
		const std::vector<POS_STRUCT> &enemyPosVect = enemyPtr->GetArmyPos();
		const std::vector<POS_STRUCT> &armyPosVect = armyPtr->GetArmyPos();

		if (enemyPosVect.empty() || armyPosVect.empty())
		{
			if (0 != increaseMoveTime_)
			{
				armyPtr->addMoveTime((-1) *increaseMoveTime_);
				increaseMoveTime_ = 0;
			}

			return false;
		}

		// �������,������
		if (!enemyIsInAttackScope(armyPosVect, enemyPosVect, attackScope) )
		{
			if (0 != increaseMoveTime_)
			{
				armyPtr->addMoveTime((-1) *increaseMoveTime_);
				increaseMoveTime_ = 0;
			}

			return true;
		}
		else if (isEven && armyPtr->isCanUseWeapon() && !isChurchmanFollow)
		{
			attEnemyInAttScope(enemyPtr, armyPtr, false);

			if (0 != increaseMoveTime_)
			{
				armyPtr->addMoveTime((-1) *increaseMoveTime_);
				increaseMoveTime_ = 0;
			}
			return true;
		}
	}
	// ���й������
	else if (!armyPtr->GetTrusteeshipState() && NULL == enemyPtr && Move_status != armyCurrentStatus)
	{
		enemyPtr = commonFunction->getGunshotEnemyToAttack(mapDataPtr, armyPtr, tipTopAttackTar);

		if (NULL == enemyPtr)
		{
			if (0 != increaseMoveTime_)
			{
				armyPtr->addMoveTime((-1) *increaseMoveTime_);
				increaseMoveTime_ = 0;
			}

			return true;
		}
		
		// ����ڳ��ֵо����򹥻�
		if (isEven && armyPtr->isCanUseWeapon() && 0 != enemyPtr->GetCurrentHp())
		{	
			enemyID = enemyPtr->GetArmyID();

			attEnemyInAttScope(enemyPtr, armyPtr, false);

			if (0 != increaseMoveTime_)
			{
				armyPtr->addMoveTime((-1) *increaseMoveTime_);
				increaseMoveTime_ = 0;
			}

			return true;
		}	
	}
	
	//  ת��������
	if (!isNotWay && NULL == enemyPtr && ((0 == movePosVectorSize && Move_status == armyCurrentStatus)
		|| (Attack_status == armyCurrentStatus && !isNeedSickEnemy(enemyPtr))))
	{
		armyCurrentStatus = Idle_status;
	}

	if (0 != increaseMoveTime_)
	{
		armyPtr->addMoveTime((-1) *increaseMoveTime_);
		increaseMoveTime_ = 0;
	}

	return true;
}

/*
bool AppetentAction::GetGoPathPosVector(const IArmyPtr armyPtr, const IArmyPtr enemyPtr, Vect_Pos &movePosVector)
{
	bool isSucess = false;
	const std::vector<POS_STRUCT> & posArmy = armyPtr->GetArmyPos();
	const std::vector<POS_STRUCT> & posEnemy = enemyPtr->GetArmyPos();

	if (posArmy.empty() || posEnemy.empty())
	{
		return false;
	}

	PosXYStru posStartCell(posArmy[0].PosX, posArmy[0].PosY), posEndCell(posEnemy[0].PosX, posEnemy[0].PosY);

	isSucess = seekPathPtr->findPath(posStartCell, posEndCell, movePosVector);

	if (!isSucess)
	{
		return false;
	}

	return true;
}

*/
bool AppetentAction::isNeedSickEnemy(IArmyPtr enemyPtr)
{
	// ����о����Ӳ����ڻ�HP=0ʱ��ת����״̬
	if (NULL == enemyPtr || 0 == enemyPtr->GetCurrentHp())
	{
		return false;
	}

	// �жϹ����ĵо��Ƿ�������Ұ�������������Ұ�򲻹�����ת����״̬
	// ��ȡ���ӵ���Ұ
	int scopeVal = armyPtr->GetViewScope();

	bool isSucess = false;

	// ��ȡ���ӵĵ�ǰ����
	PosXYStru ptArmy;
	const std::vector<POS_STRUCT> &armyObjPos = armyPtr->GetArmyPos();
	ptArmy.xPos = armyObjPos[0].PosX;
	ptArmy.yPos = armyObjPos[0].PosY;

	// ��ȡ�����ĵо�����
	PosXYStru ptEnemy;
	const std::vector<POS_STRUCT> &EnemyObjPos = enemyPtr->GetArmyPos();
	ptEnemy.xPos = EnemyObjPos[0].PosX;
	ptEnemy.yPos = EnemyObjPos[0].PosY;

	Vect_Pos  movePosVector;

	if (ptArmy == ptEnemy)
	{
		return true;
	}

	isSucess = seekPathPtr->findPath(ptArmy, ptEnemy, movePosVector);

	if (!isSucess)
	{
		return false;
	}

	if (static_cast<int>(movePosVector.size()) > scopeVal)
	{
		return false;
	}

	return true;
}

bool AppetentAction::isCanGo(const PosXYStru &nextPos, managerParam & mgrParam, IArmyPtr &enemyPtr, TrapStruct &trapStruct, SOLDIER_TYPE soldierType, int moveSpeed, bool isUseInDynSeek )
{
	//if (armyPtr->isNewbie())
	//{
	//	return true;
	//}

	// �����ж��Ƿ���Ҫ��̬Ѱ·ʱ����
	if (!isUseInDynSeek)
	{
		// ����Ӱ��
		LAND_FORM landform = seekPathPtr->getLandForm(nextPos.xPos, nextPos.yPos);
		
		if (LAND_RIVERWAY == landform || LAND_FOREST == landform)
		{
		    const LandformInfoMap *landformInfoMap = mgrParam.landformInfoMap;
			
			if (NULL == landformInfoMap)
			{
#ifdef _DEBUG
				assert(0);
#endif
				return true;
			}

			LandformInfoMap::const_iterator itFind = landformInfoMap->find(std::make_pair(landform, soldierType));

			if (landformInfoMap->end() != itFind)
			{
				managerParam::LandformAffStru &landformStru = mgrParam.landformAffStru;
				aistrategy::LandformAffect* landformAffPtr = itFind->second;

				if (NULL == landformAffPtr)
				{
					assert(0);
					return true;
				}

				armyPtr->addMoveTime(landformAffPtr->boutVal);

				increaseMoveTime_ = landformAffPtr->boutVal;
			}
			else
			{
				assert(0);
			}
		}

	}

	if (armyPtr->isNewbie())
	{
		return true;
	}

	// ��ȡҪ�ߵ���һ������վ�Ĳ�����
	list<IArmyPtr> & armyLst = (*mapDataPtr)[nextPos.xPos][nextPos.yPos];

	int armyNum = static_cast<int>(armyLst.size());	

	if (armyNum <= 0)
	{
		return true;
	}

	list<IArmyPtr>::iterator itBeg = armyLst.begin();
	//list<IArmyPtr>::iterator itEnd = armyLst.end();

	if (0 == (*itBeg)->GetCurrentHp())
	{
#ifdef _DEBUG
		assert(0 && "����HP��Ϊ0");
#endif
		return true;

	}

	// �����һ��Ҫ�ߵĵ��ǵз���դ��������й���
	if (ARCH_BARRIER == (*itBeg)->getBuildDetailType())
	{
		if (!armyPtr->IsFriendsArmy(*itBeg) && PRIEST != armyPtr->GetArmyOccupation())
		{
			enemyPtr = *itBeg;
		}
		return false;
	}

	// ����ֻ������
	if (ARCH_TRAP == (*itBeg)->getBuildDetailType()
		&& !armyPtr->IsFriendsArmy(*itBeg))
	{
		// ���ӵĻ�
		if (armyPtr->isVisible(*itBeg))
		{
			if (PRIEST != armyPtr->GetArmyOccupation())
			{
				enemyPtr = *itBeg;
			}

			return false;
		}
		else
		{
			trapStruct.isInEnemyTrap = true;
			trapStruct.trapArmyPtr = *itBeg;
			return true;
		}	
	}

	if (!armyPtr->IsFriendsArmy(*itBeg))
	{
		if (PRIEST != armyPtr->GetArmyOccupation())
		{
			enemyPtr = *itBeg;
		}

		return false;
	}		

	if (armyNum < cellHasArmyMax_)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool AppetentAction::GetPathInDynamicSeek(PosXYStru &posTarget, vector<PosXYStru> & movePosVector, unsigned int moveSpeed)
{
	// �������
	movePosVector.clear();

	bool isSucess = false;
	int distVal = 0;
	std::list<PosXYStru> lstAllPosHasArmy;
	std::list<PosXYStru> lstAllPosHasArmyTmp;

	const std::vector<POS_STRUCT> &posVect = armyPtr->GetArmyPos();

	if (posVect.empty())
	{
		return false;
	}

	PosXYStru armyPos(posVect[0].PosX, posVect[0].PosY);

	//distVal = getDistanceTwoNode(armyPos.xPos, armyPos.yPos, posTarget.xPos, posTarget.yPos);

	distVal = moveSpeed;

	if (0 == distVal)
	{
//#ifdef _DEBUG
//		assert(0 && "����Ϊ0");
//#endif
		return false;
	}

	for (int i=1; i<=distVal; ++i)
	{
		isSucess = CommonFunctionSingleton::getInstance()->getSpecifyDistAllPosHasArmy(mapDataPtr, seekPathPtr->GetLoadMapInfoPtr(), armyPtr, lstAllPosHasArmyTmp, i);

		if (!isSucess)
		{
			return false;
		}

		lstAllPosHasArmy.insert(lstAllPosHasArmy.end(), lstAllPosHasArmyTmp.begin(), lstAllPosHasArmyTmp.end());
	}

	lstAllPosHasArmy.remove(posTarget);

	if (!lstAllPosHasArmy.empty())
	{
		seekPathPtr->SetDynamicBlockInfo(lstAllPosHasArmy);
	}

	bool isStaticBlook = armyPtr->isStaticBlook(posTarget.xPos, posTarget.yPos);

	if (isStaticBlook)
	{
		isSucess = armyPtr->getNearPosInOneCircle(posTarget, posTarget);

		if (!isSucess)
		{
			return false;
		}
	}

	isSucess = seekPathPtr->findPath(armyPos, posTarget, movePosVector);

	if (!isSucess)
	{
//#ifdef _DEBUG
//		assert(0 && "Ѱ·ʧ��");
//#endif
		return false;
	}

	return true;
}

void AppetentAction::isNeedDynSeekAndGetPath(vector<PosXYStru> &movePosVector, 
											 managerParam & mgrParam,
											 SOLDIER_TYPE soldierType,
											 IArmyPtr enemyPtrTmp, 
											 int movePosVectorSizeTmp, 
											 bool &isNotWay, 
											 int &movePosVectorSize, 
											 int &iMin, 
											 int moveSpeed, 
											 ENUM_ARMY_STATUS armyCurrentStatus)
{
	bool isSucess = false, isCanGoNextPos = false;

	//��ȡ���ӵ����
	int attackScope = armyPtr->GetAttackScope();

	IArmy::CardUsedState cardUsedState = armyPtr->getCardState(SELFBLAST_CARD);

	// ʹ�����Ա���
	if (IArmy::IS_USING_CARD == cardUsedState)
	{
		attackScope = 1;
	}

	TrapStruct trapStruct;
	isCanGoNextPos = isCanGo(movePosVector[0], mgrParam, enemyPtrTmp, trapStruct, soldierType, moveSpeed, true);

	// һ�����������ߣ���̬Ѱ·
	if (!isCanGoNextPos)
	{
		PosXYStru posTarget;
		IArmyPtr enemyPtr = armyPtr->getArmyByID(mgrParam.enemyID);

		// ����Ŀ�꽨������������ڵĸ���(ԭ����Ŀ������)���ѱ�վ�����ӻ��ܵ���,����ѡȡ��
		if (1 == movePosVectorSizeTmp && NULL != enemyPtr && BUILD == enemyPtr->GetArmyType())
		{
			const std::vector<POS_STRUCT> &posXYVector = enemyPtr->GetArmyPos();

			if (posXYVector.empty())
			{
				return;
			}

			unsigned int posSize = static_cast<unsigned int>(posXYVector.size());

			srand(static_cast<unsigned int>(time(0)));					// ����ϵͳʱ���������������
			int index = rand() % posSize;								// ȡ������[0,N)������

			posTarget.xPos = posXYVector[index].PosX;
			posTarget.yPos = posXYVector[index].PosY;

			bool isStaticBlook = armyPtr ->isStaticBlook(posTarget.xPos, posTarget.yPos);

			if (isStaticBlook)
			{
				bool isSucess = armyPtr->getNearPosInOneCircle(posTarget, posTarget);

				if (!isSucess)
				{
					return;
				}
			}
		}
		else
		{
			// ��ȡĿ�ĵ�����
			posTarget = movePosVector[movePosVectorSizeTmp - 1];
		}

		vector<PosXYStru>  movePosVectorTmp;

		movePosVectorTmp.insert(movePosVectorTmp.end(), movePosVector.begin(), movePosVector.end());

		isSucess = GetPathInDynamicSeek(posTarget, movePosVector, moveSpeed);

		if (!isSucess)
		{
			movePosVector.insert(movePosVector.end(), movePosVectorTmp.begin(), movePosVectorTmp.end());

			isNotWay = true;

			//iMin = 0;
			//#ifdef _DEBUG
			//				assert(0 && "��̬Ѱ·ʧ��");
			//#endif
			//return false;
		}
		else
		{
			movePosVectorSize = static_cast<int>(movePosVector.size());

			iMin = min(moveSpeed, movePosVectorSize);

			// ����״̬
			if (Attack_status == armyCurrentStatus)
			{
				// ����ߵ��о�������̼���
				movePosVectorSize -= attackScope;

				iMin = min(moveSpeed, movePosVectorSize);
			}
		}
	}
}

void AppetentAction::attEnemyInAttScope(IArmyPtr enemyPtr, IArmyPtr selfarmyPtr, bool isUseSelfBlast, bool isForceBlast) const
{
	if (NULL == enemyPtr && !isForceBlast)
	{
#ifdef _DEBUG
		assert(0 && "�����Ķ���ָ��Ϊ����");
#endif
		return;
	}
	
	if (PRIEST == selfarmyPtr->GetArmyOccupation())
	{
		if (!isForceBlast  && NULL != enemyPtr && 0 == enemyPtr->getCurHurtHp())
		{	
			return ;
		}
	}
	// ʹ���޵п�������ʦ���ܹ���
	else
	{
		IArmy::CardUsedState cardUsedState = selfarmyPtr->getCardState(INVINCIBILITY_CARD);

		// ʹ�����޵п��󣬲��ܹ�������
		if (IArmy::IS_USING_CARD == cardUsedState)
		{
			return;
		}
	}

	// ����ս��������ʱ��ռ�쿤��ԭ���Ĺ����Ĳ��ӻ�������ѷ����ˡ�������ٽ������ж�
	if (!isForceBlast && selfarmyPtr->IsFriendsArmy(enemyPtr) && PRIEST != selfarmyPtr->GetArmyOccupation())
	{
		return;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	unsigned int enemyId = 0;

	if (NULL != enemyPtr)
	{
		enemyId = enemyPtr->GetArmyID();
	}

	// �����Ա�״̬����ǿ�Ʊ�ը״̬
	if (isUseSelfBlast || isForceBlast)
	{
		if (isForceBlast)
		{
			enemyId = selfarmyPtr->GetArmyID();
		}

		selfarmyPtr->Attack(enemyId, TWO_CELL_VAL);

		return;
	}

	if (PRIEST != selfarmyPtr->GetArmyOccupation())
	{
		if (NULL != enemyPtr)
		{	
			selfarmyPtr->SetLastAttackTarget(enemyPtr->GetArmyID());
			enemyPtr->SetLastEnmityTarget(selfarmyPtr->GetArmyID());
		}
	}	

	selfarmyPtr->Attack(enemyId);


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	const POS_STRUCT * targetPosPtr = NULL;

	if (isForceBlast)
	{
		const std::vector<POS_STRUCT> &posVect = selfarmyPtr->GetArmyPos();

		if (posVect.empty())
		{
#ifdef _DEBUG
			assert(0 && "��������Ϊ��");
#endif
			return;
		}

		const POS_STRUCT &posStru = posVect.front();
		targetPosPtr = &posStru;
	}
	else
	{
		const std::vector<POS_STRUCT> &posVect = enemyPtr->GetArmyPos();

		if (posVect.empty())
		{
#ifdef _DEBUG
			assert(0 && "��������Ϊ��");
#endif
			return;
		}

		const POS_STRUCT &posStru = posVect.front();
		targetPosPtr = &posStru;
	}

	PosXYStru pos((*targetPosPtr).PosX, (*targetPosPtr).PosY);
	std::vector<unsigned int> attackedTarget;

	// �����Ա�״̬����ǿ�Ʊ�ը״̬
	if (isUseSelfBlast || isForceBlast)
	{
		std::vector<unsigned int> attackedTargetInOneCircle;
		std::vector<IArmyPtr> armysCenter, armysInOneCircle;
		CommonFunctionSingleton::getInstance()->getSelfBlastAttArmys(pos, mapDataPtr, armysCenter, armysInOneCircle);

		std::vector<IArmyPtr>::iterator itBeg = armysCenter.begin();
		std::vector<IArmyPtr>::iterator itEnd = armysCenter.end();

		for (; itBeg!=itEnd; ++itBeg)
		{
			if (NULL == (*itBeg) || 0 == (*itBeg)->GetCurrentHp())
			{
				//#ifdef _DEBUG
				//				assert(0 && "����ָ��Ϊ����");
				//#endif
				continue;
			}

			if (selfarmyPtr->IsFriendsArmy(*itBeg) || ARCH_BUILD_TOFT == (*itBeg)->getBuildDetailType()
				|| (ARCH_TRAP == (*itBeg)->getBuildDetailType() && !selfarmyPtr->isVisible(*itBeg)))
			{
				continue;			
			}

			attackedTarget.push_back((*itBeg)->GetArmyID());
		}

		itBeg = armysInOneCircle.begin();
		itEnd = armysInOneCircle.end();

		for (; itBeg!=itEnd; ++itBeg)
		{
			if (NULL == (*itBeg) || 0 == (*itBeg)->GetCurrentHp())
			{
				//#ifdef _DEBUG
				//				assert(0 && "����ָ��Ϊ����");
				//#endif
				continue;
			}

			if (selfarmyPtr->IsFriendsArmy(*itBeg) || ARCH_BUILD_TOFT == (*itBeg)->getBuildDetailType()
				|| (ARCH_TRAP == (*itBeg)->getBuildDetailType() && !selfarmyPtr->isVisible(*itBeg)))
			{
				continue;			
			}

			attackedTargetInOneCircle.push_back((*itBeg)->GetArmyID());
		}

		selfarmyPtr->Attack(attackedTarget, attackedTargetInOneCircle);
	}
	else
	{
		// ��ʦĬ�Ϲ���ȫ��
		if (MAGE == selfarmyPtr->GetArmyOccupation() || ARCH_TRAP == selfarmyPtr->getBuildDetailType())
		{	
			std::vector<IArmyPtr> Armys;
			CommonFunctionSingleton::getInstance()->getArmysInPos(pos, mapDataPtr, Armys);
			std::vector<IArmyPtr>::iterator itBeg = Armys.begin();
			std::vector<IArmyPtr>::iterator itEnd = Armys.end();

			for (; itBeg!=itEnd; ++itBeg)
			{
				if (NULL == (*itBeg) || 0 == (*itBeg)->GetCurrentHp())
				{
					//#ifdef _DEBUG
					//				assert(0 && "����ָ��Ϊ����");
					//#endif
					continue;
				}

				if (selfarmyPtr->IsFriendsArmy(*itBeg) || ARCH_BUILD_TOFT == (*itBeg)->getBuildDetailType()
					|| (ARCH_TRAP == (*itBeg)->getBuildDetailType() && !selfarmyPtr->isVisible(*itBeg)))
				{
					continue;			
				}

				attackedTarget.push_back((*itBeg)->GetArmyID());

				if (selfarmyPtr->GetCountryID() == enemyPtr->GetCountryID() && selfarmyPtr->GetArmyOccupation() != PRIEST)
				{
					LOG_ERROR(("error:��ʦ����ȫ��ʱ�������ߵĲ���IDΪ��%d������Ӣ��IDΪ��%d������IDΪ��%d������ְҵ:%d�� ����Ӣ��ְҵ:%d;  �������Ĳ���IDΪ��%d������Ӣ��IDΪ��%d������IDΪ��%d������ְҵ:%d�� ����Ӣ��ְҵ:%d", 
						selfarmyPtr->GetArmyID(), selfarmyPtr->GetOfficerID(), selfarmyPtr->GetCountryID(), selfarmyPtr->GetArmyOccupation(), selfarmyPtr->getHeroOccupation(), 
						(*itBeg)->GetArmyID(), (*itBeg)->GetOfficerID(), (*itBeg)->GetCountryID(), (*itBeg)->GetArmyOccupation(), (*itBeg)->getHeroOccupation()));
				}
			}

		}
		else
		{
			attackedTarget.push_back(enemyPtr->GetArmyID());

			if (selfarmyPtr->GetCountryID() == enemyPtr->GetCountryID() && selfarmyPtr->GetArmyOccupation() != PRIEST)
			{
				LOG_ERROR(("error:���ӹ���ĳһ����ʱ�������ߵĲ���IDΪ��%d������Ӣ��IDΪ��%d������IDΪ��%d������ְҵ:%d�� ����Ӣ��ְҵ:%d;  �������Ĳ���IDΪ��%d������Ӣ��IDΪ��%d������IDΪ��%d������ְҵ:%d�� ����Ӣ��ְҵ:%d", 
					selfarmyPtr->GetArmyID(), selfarmyPtr->GetOfficerID(), selfarmyPtr->GetCountryID(), selfarmyPtr->GetArmyOccupation(), selfarmyPtr->getHeroOccupation(), 
					enemyPtr->GetArmyID(), enemyPtr->GetOfficerID(), enemyPtr->GetCountryID(), enemyPtr->GetArmyOccupation(), enemyPtr->getHeroOccupation()));
			}
		}

		if (PRIEST != selfarmyPtr->GetArmyOccupation())
		{
			selfarmyPtr->SetLastAttackTarget(enemyPtr->GetArmyID());
			enemyPtr->SetLastEnmityTarget(selfarmyPtr->GetArmyID());
		}	
		
		selfarmyPtr->Attack(attackedTarget);
	}

	*/
}

bool AppetentAction::enemyIsInAttackScope(const std::vector<POS_STRUCT> &armyPosVect, const std::vector<POS_STRUCT> &enemyPosVect, int attackScope) const
{
	unsigned int posSize = static_cast<unsigned int>(enemyPosVect.size());

	for (unsigned int i=0; i<posSize; ++i)
	{
		if (getDistanceTwoNode(armyPosVect[0].PosX, armyPosVect[0].PosY, enemyPosVect[i].PosX, enemyPosVect[i].PosY) <= attackScope)
		{
			return true;
		}
	}

	return false;
}