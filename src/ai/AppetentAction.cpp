// @brief 简要描述: 本能AI实现
// @author 作者: 黄 文 才

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
		//						assert(0 && "获取策略管理器指针失败");
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

	// 获取部队的射程
	int attackScope = armyPtr->GetAttackScope();	
	
	IArmyPtr enemyPtr = armyPtr->getArmyByID(enemyID);

	IArmy::CardUsedState cardUsedState = armyPtr->getCardState(SELFBLAST_CARD);

	// 使用了自爆卡
	if (IArmy::IS_USING_CARD == cardUsedState)
	{
		isUseSelfBlast = true;
		attackScope = 1;
	}
	else if (IArmy::FORCE_OFF_CARD == cardUsedState)
	{
		// 时限已到，强制自爆
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

	// 原来攻击的对象在射程内,则攻击
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
		assert(0 && "获取功能函数单例智能失败");
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
	TrapStruct trapStruct;		// 是否陷入了敌军的陷阱中的数据结构

	SOLDIER_TYPE soldierType = armyPtr->GetArmyOccupation();

	// 攻击状态
	if (Attack_status == armyCurrentStatus)
	{
		// 最多走到敌军进入射程即可
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
			// 删除movePosVector中已经要走的坐标
			movePosVector.erase(movePosVector.begin(), movePosVector.begin() + movePosVectorSize);
			armyPtr->Move(movePosVect, armyPtr);
		}
	
		// 移动，掉入敌军的陷阱中,调用陷阱攻击之
		if (isEven && trapStruct.isInEnemyTrap)
		{
#ifdef _DEBUG
			if (NULL == trapStruct.trapArmyPtr)
			{
				assert(0 && "陷阱指针为空了");
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

			// 在射程内,则攻击
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
			// 执行后退命令，则不再打射程内的敌军
			if (isCountermarch)
			{
				if (0 != increaseMoveTime_)
				{
					armyPtr->addMoveTime((-1) *increaseMoveTime_);
					increaseMoveTime_ = 0;
				}

				return true;
			}

			// 托管下的情况
			if (armyPtr->GetTrusteeshipState())
			{
				if (0 != increaseMoveTime_)
				{
					armyPtr->addMoveTime((-1) *increaseMoveTime_);
					increaseMoveTime_ = 0;
				}

				return true;
			}
			// 非托管下，需要判断射程内有无敌军
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

				// 射程内出现敌军，则攻击
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
	// 托管情况下
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

		// 在射程外,不攻击
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
	// 非托管情况下
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
		
		// 射程内出现敌军，则攻击
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
	
	//  转空闲命令
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
	// 如果敌军部队不存在或HP=0时，转空闲状态
	if (NULL == enemyPtr || 0 == enemyPtr->GetCurrentHp())
	{
		return false;
	}

	// 判断攻击的敌军是否逃离视野，如果逃离了视野则不攻击，转空闲状态
	// 获取部队的视野
	int scopeVal = armyPtr->GetViewScope();

	bool isSucess = false;

	// 获取部队的当前坐标
	PosXYStru ptArmy;
	const std::vector<POS_STRUCT> &armyObjPos = armyPtr->GetArmyPos();
	ptArmy.xPos = armyObjPos[0].PosX;
	ptArmy.yPos = armyObjPos[0].PosY;

	// 获取攻击的敌军坐标
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

	// 不在判断是否需要动态寻路时调用
	if (!isUseInDynSeek)
	{
		// 地形影响
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

	// 获取要走的下一格子中站的部队数
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
		assert(0 && "部队HP不为0");
#endif
		return true;

	}

	// 如果下一步要走的点是敌方的栅栏，则进行攻击
	if (ARCH_BARRIER == (*itBeg)->getBuildDetailType())
	{
		if (!armyPtr->IsFriendsArmy(*itBeg) && PRIEST != armyPtr->GetArmyOccupation())
		{
			enemyPtr = *itBeg;
		}
		return false;
	}

	// 格子只有陷阱
	if (ARCH_TRAP == (*itBeg)->getBuildDetailType()
		&& !armyPtr->IsFriendsArmy(*itBeg))
	{
		// 可视的话
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
	// 清空容器
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
//		assert(0 && "距离为0");
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
//		assert(0 && "寻路失败");
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

	//获取部队的射程
	int attackScope = armyPtr->GetAttackScope();

	IArmy::CardUsedState cardUsedState = armyPtr->getCardState(SELFBLAST_CARD);

	// 使用了自爆卡
	if (IArmy::IS_USING_CARD == cardUsedState)
	{
		attackScope = 1;
	}

	TrapStruct trapStruct;
	isCanGoNextPos = isCanGo(movePosVector[0], mgrParam, enemyPtrTmp, trapStruct, soldierType, moveSpeed, true);

	// 一步都不能再走，动态寻路
	if (!isCanGoNextPos)
	{
		PosXYStru posTarget;
		IArmyPtr enemyPtr = armyPtr->getArmyByID(mgrParam.enemyID);

		// 距离目标建筑物的坐标相邻的格子(原来的目的坐标)，已被站满部队或不能到达,重现选取点
		if (1 == movePosVectorSizeTmp && NULL != enemyPtr && BUILD == enemyPtr->GetArmyType())
		{
			const std::vector<POS_STRUCT> &posXYVector = enemyPtr->GetArmyPos();

			if (posXYVector.empty())
			{
				return;
			}

			unsigned int posSize = static_cast<unsigned int>(posXYVector.size());

			srand(static_cast<unsigned int>(time(0)));					// 根据系统时间设置随机数种子
			int index = rand() % posSize;								// 取得区间[0,N)的整数

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
			// 获取目的地坐标
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
			//				assert(0 && "动态寻路失败");
			//#endif
			//return false;
		}
		else
		{
			movePosVectorSize = static_cast<int>(movePosVector.size());

			iMin = min(moveSpeed, movePosVectorSize);

			// 攻击状态
			if (Attack_status == armyCurrentStatus)
			{
				// 最多走到敌军进入射程即可
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
		assert(0 && "攻击的对象指针为空了");
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
	// 使用无敌卡，非牧师不能攻击
	else
	{
		IArmy::CardUsedState cardUsedState = selfarmyPtr->getCardState(INVINCIBILITY_CARD);

		// 使用了无敌卡后，不能攻击别人
		if (IArmy::IS_USING_CARD == cardUsedState)
		{
			return;
		}
	}

	// 当出战报结束的时候，占领郡后，原来的攻击的部队或建筑物成友方的了。这里加临界区的判断
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

	// 出于自爆状态或者强制爆炸状态
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
			assert(0 && "部队坐标为空");
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
			assert(0 && "部队坐标为空");
#endif
			return;
		}

		const POS_STRUCT &posStru = posVect.front();
		targetPosPtr = &posStru;
	}

	PosXYStru pos((*targetPosPtr).PosX, (*targetPosPtr).PosY);
	std::vector<unsigned int> attackedTarget;

	// 出于自爆状态或者强制爆炸状态
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
				//				assert(0 && "部队指针为空了");
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
				//				assert(0 && "部队指针为空了");
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
		// 法师默认攻击全格
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
					//				assert(0 && "部队指针为空了");
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
					LOG_ERROR(("error:法师攻击全格时，攻击者的部队ID为：%d，归属英雄ID为：%d，国家ID为：%d，部队职业:%d， 带队英雄职业:%d;  被攻击的部队ID为：%d，归属英雄ID为：%d，国家ID为：%d，部队职业:%d， 带队英雄职业:%d", 
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
				LOG_ERROR(("error:部队攻击某一部队时，攻击者的部队ID为：%d，归属英雄ID为：%d，国家ID为：%d，部队职业:%d， 带队英雄职业:%d;  被攻击的部队ID为：%d，归属英雄ID为：%d，国家ID为：%d，部队职业:%d， 带队英雄职业:%d", 
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