// @brief ��Ҫ����: �����ӿں��� 
// @author ����: �� �� ��

#pragma warning(push)
#pragma warning(disable:4512)
#pragma warning(disable:4127)
#pragma warning(disable:4100)
#include <boost/random.hpp>
#pragma warning(pop)

#include "CommonFun.h"
#include "ProFightSystem.h"
#include "SeekPathArithm.h"

#if defined(_WIN32)
#	define _WIN32_WINNT 0x0502
#endif

#include <boost/asio/detail/mutex.hpp>

CommonFunctionSingleton * CommonFunctionSingleton::comFunInstance = NULL;

using namespace boost;

enum 
{
	ONE_CELL_VAL = 1
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool StatFriendArmyAndEnemyStru::fun(int xPos, int yPos)
{
	if (NULL == mapDataPtr_ || NULL == armyObjParam_)
	{
		return false;
	}

	const list<IArmyPtr> &lstIArmyPtr  = (*mapDataPtr_)[xPos][yPos];
	bool cellHasEnemy = false, isFriend = false;
	list<IArmyPtr>::const_iterator itArmyPtrBeg = lstIArmyPtr.begin();
	list<IArmyPtr>::const_iterator itArmyPtrEnd = lstIArmyPtr.end();
	
	IArmyPtr lastEnmityTarget = NULL;

	for (; itArmyPtrBeg!=itArmyPtrEnd; ++itArmyPtrBeg)
	{
		lastEnmityTarget = (*itArmyPtrBeg)->GetLastEnmityTarget();

		if (ARCH_BUILD_TOFT == (*itArmyPtrBeg)->getBuildDetailType()
			|| ARCH_BARRIER == (*itArmyPtrBeg)->getBuildDetailType()
			|| 0 == (*itArmyPtrBeg)->GetCurrentHp()
			|| *itArmyPtrBeg == armyObjParam_
			|| (isOnlyBuilding_ && BUILD !=(*itArmyPtrBeg)->GetArmyType())
			|| ((ARCH_OCCUPY_BUILDING_START < (*itArmyPtrBeg)->getBuildDetailType() && (*itArmyPtrBeg)->getBuildDetailType() < ARCH_OCCUPY_BUILDING_END)
			    && NULL != lastEnmityTarget && armyObjParam_->GetArmyID() != lastEnmityTarget->GetArmyID() && PLAYER_ARMY != armyObjParam_->GetArmyType()))
		{
			//return true;
			continue;
		}

		// ֻͳ���йܵĲ���
		if (isOnlyTrusteeship_ && !(*itArmyPtrBeg)->GetTrusteeshipState())
		{
			//return true;
			continue;
		}

		if (ARCH_TRAP == (*itArmyPtrBeg)->getBuildDetailType())
		{
			if (armyObjParam_->IsFriendsArmy(*itArmyPtrBeg)
				|| (!armyObjParam_->IsFriendsArmy(*itArmyPtrBeg)
				&& !armyObjParam_->isVisible(*itArmyPtrBeg)))
			{
				//return true;
			    continue;
			}
		}

		isFriend = armyObjParam_->IsFriendsArmy(*itArmyPtrBeg);

		if (PRIEST == armyObjParam_->GetArmyOccupation())
		{
			isFriend = !isFriend;
		}

		// �Ѿ�
		if (isFriend)
		{
			lstFriendArmy_.push_back(*itArmyPtrBeg);
		}
		// �о�
		else
		{
			lstEnemy_.push_back(*itArmyPtrBeg);
			cellHasEnemy = true;
		}
	}

	if (cellHasEnemy)
	{
		++countCellsHasEnemy_;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool SpecifyDistAllPosHasArmy::fun(int xPos, int yPos)
{
	if (NULL == mapDataPtr_ || NULL == staticBlockMapPtr_)
	{
		return false;
	}

	const list<IArmyPtr> &lstIArmyPtr = (*mapDataPtr_)[xPos][yPos];		

	if (lstIArmyPtr.empty())
	{
		return true;
	}

	lstAllPosHasArmy_.push_back(PosXYStru(xPos, yPos));	

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool SecurityPosTarget::fun(int xPos, int yPos)
{
	if (NULL == mapDataParamPtr_ || NULL == armyObj_)
	{
		return false;
	}

	const list<IArmyPtr> &lstArmy = (*mapDataParamPtr_)[xPos][yPos];

	if (lstArmy.empty())
	{
		if (!seekPathArithmParamPtr_->IsBlockoff(xPos, yPos))
		{
			lstFindPos_.push_back(PosXYStru(xPos, yPos));
		}
	
		return true;
	}

	list<IArmyPtr>::const_iterator itArmy = lstArmy.begin();

	if (!armyObj_->IsFriendsArmy(*itArmy))
	{
		return true;
	}
	else
	{				
		lstFindPos_.push_back(PosXYStru(xPos, yPos));
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
boost::asio::detail::mutex mutex;

CommonFunctionSingleton::CommonFunctionSingleton()
{
}

CommonFunctionSingleton * CommonFunctionSingleton::getInstance()
{
	if (NULL == comFunInstance)
	{
		boost::asio::detail::scoped_lock<boost::asio::detail::mutex> lock(mutex);
		
		if (NULL == comFunInstance)
		{
			comFunInstance = new CommonFunctionSingleton();
		}
	}

	return comFunInstance;
}

IArmyPtr CommonFunctionSingleton::getMaxHPArmy(const std::list<IArmyPtr> &listArmy)
{
	if (listArmy.empty())
	{
		return NULL;
	}

	std::list<IArmyPtr>::const_iterator itBeg = listArmy.begin();
	std::list<IArmyPtr>::const_iterator itEnd = listArmy.end();

	IArmyPtr army = (*itBeg);

	for (++itBeg; itBeg!=itEnd; ++itBeg)
	{
		if ((*itBeg)->GetCurrentHp() > army->GetCurrentHp())
		{
			army = *itBeg;
		}
	}

	return army;
}

IArmyPtr CommonFunctionSingleton::getMinHPArmy(const std::list<IArmyPtr> &listArmy)
{
	if (listArmy.empty())
	{
		return NULL;
	}

	IArmyPtr army = NULL;
	std::list<IArmyPtr>::const_iterator itBen = listArmy.begin();
	std::list<IArmyPtr>::const_iterator itEnd = listArmy.end();
	
	// ��ȡ��һ�������˵��Ѿ�
	while (itBen != itEnd)
	{
		if (0 != (*itBen)->getCurHurtHp() && 0 != (*itBen)->GetCurrentHp())
		{
			army = (*itBen);
			break;
		}

		++itBen;
	}

	if (NULL == army)
	{
		return NULL;
	}

	for (; itBen!=itEnd; ++itBen)
	{
		if (NULL == *itBen)
		{
#ifdef _DEBUG
		assert(0 && "ָ��Ϊ��");
#endif
			continue;

		}	
		
		if (0 != (*itBen)->GetCurrentHp()
			&& 0 != (*itBen)->getCurHurtHp()  && (*itBen)->GetCurrentHp() < army->GetCurrentHp())
		{
			army = *itBen;
		}
	}

	return army;
}

bool CommonFunctionSingleton::getApproEnemyToAttackOnAimRule(IArmyPtrvectInvect * mapDataPtr, IArmyPtr armyObjParam, const int scopeVal, IArmyPtr &enemyPtr, SOLDIER_TYPE tipTopAttackTar)
{
	listArmyPtr lstFriendArmy;
	listArmyPtr lstEnemy;
	int number = 0;

	listArmyPtr lstAllEnemy;
	listArmyPtr EnemyClergyContainer;		// ��ʦ����
	listArmyPtr EnemyRabbiContainer;		// ��ʦ����
	listArmyPtr EnemyBowContainer;			// ����������
	listArmyPtr EnemyWarriorContainer;		// սʿ����
	IArmyPtr buildingArmy = NULL;	// ������
	IArmyPtr goodsArmy = NULL ; // ��Ʒ

	for(int i=1; i<=scopeVal; ++i)
	{
		lstFriendArmy.clear();
		lstEnemy.clear();

		if(getSpecifyDistanceEnemy(mapDataPtr, armyObjParam, i, false, false, lstFriendArmy, lstEnemy, number))//����Ѱ��ÿһȦ�ĵ���
		{
			std::list<IArmyPtr>::iterator itBeg = lstEnemy.begin();
			std::list<IArmyPtr>::iterator itEnd = lstEnemy.end();

			// ��ÿһȦ�ĵ��˽��з���push�����Ե�������
			for (; itBeg!=itEnd; ++itBeg)
			{
				if (BUILD != (*itBeg)->GetArmyType())
				{
					lstAllEnemy.push_back(*itBeg);	
				}

				if ((*itBeg)->GetArmyOccupation() == PRIEST)
				{
					EnemyClergyContainer.push_back(*itBeg);
					continue;
				}
				else if ((*itBeg)->GetArmyOccupation() == MAGE)
				{
					EnemyRabbiContainer.push_back(*itBeg);
					continue;
				}
				else if ((*itBeg)->GetArmyOccupation() == SHOOTER)
				{
					EnemyBowContainer.push_back(*itBeg);
					continue;
				}
				else if ((*itBeg)->GetArmyOccupation() == WARRIOR)
				{
					EnemyWarriorContainer.push_back(*itBeg);
					continue;
				}
				else if (BUILD != (*itBeg)->GetArmyType())
				{
					goodsArmy = *itBeg;
					continue;
				}

				// ��������������
				if (!buildingArmy 
					&& BUILD == (*itBeg)->GetArmyType()
					&& ARCH_TRAP != (*itBeg)->getBuildDetailType())
				{
					buildingArmy = *itBeg;
					continue;
				}

				// �з����壬������ӵĻ�
				if (!buildingArmy 
					&& ARCH_TRAP == (*itBeg)->getBuildDetailType() 
					&& armyObjParam->isVisible(*itBeg))
				{
					buildingArmy = *itBeg;
					continue;
				}
			}
		}
		else
		{
			return false;
		}
	}

	if (PRIEST == armyObjParam->GetArmyOccupation())
	{
		lstAllEnemy.push_back(armyObjParam);
		enemyPtr = getMinHPArmy(lstAllEnemy);
		return true;
	}
	else
	{
		IArmyPtr tArmyPtr = getArmyByPriority(EnemyClergyContainer,EnemyRabbiContainer,EnemyBowContainer,EnemyWarriorContainer, tipTopAttackTar);

		if (tArmyPtr != NULL)
		{
			enemyPtr = tArmyPtr;
			return true;
		}
		else
		{
			// ����������
			if (buildingArmy != NULL)
			{
				enemyPtr = buildingArmy;

				return true;
			}

			//��Ʒ
			if (goodsArmy !=NULL)
			{
				enemyPtr = goodsArmy;

				return true;
			}
		}
	}

	return true;
}

IArmyPtr CommonFunctionSingleton::getGunshotEnemyToAttack(IArmyPtrvectInvect * mapDataPtr, IArmyPtr armyObjParam, const SOLDIER_TYPE tipTopAttackTar)
{
	// ��ȡ�������
	int scopeVal = armyObjParam->GetAttackScope();

	IArmyPtr enemyPtr = NULL;

	bool isSuceess = getApproEnemyToAttackOnAimRule(mapDataPtr, armyObjParam, scopeVal, enemyPtr, tipTopAttackTar);

	if (!isSuceess)
	{
		return NULL;
	}

	return enemyPtr;
}

bool CommonFunctionSingleton::invalidateXYpos(IArmyPtrvectInvect * mapDataPtr, int Xpos, int Ypos)
{
	int XMaxVal = static_cast<int>((mapDataPtr->size())) - 1;

	// У��X��������ֵ
	if (XMaxVal < 0)
	{
		return false;
	}

	int YMaxVal = static_cast<int>(mapDataPtr[0].size()) - 1;

	// У��Y��������ֵ
	if (YMaxVal < 0)
	{	
		return false;
	}

	// У����������
	if (0 > Xpos || Xpos > XMaxVal || 0 > Ypos || Ypos > YMaxVal)
	{
		return false;
	}

	return true;
}

/*
bool CommonFunctionSingleton::StatFriendArmyAndEnemy(const list<IArmyPtr> &lstIArmyPtr,
							IArmyPtr  armyObjParam,
							bool isOnlyBuilding,
						    bool isOnlyTrusteeship,
							listArmyPtr &lstFriendArmy,
							listArmyPtr &lstEnemy,
							int &countCellsHasEnemy)
{
	bool cellHasEnemy = false, isFriend = false;

	list<IArmyPtr>::const_iterator itArmyPtrBeg = lstIArmyPtr.begin();
	list<IArmyPtr>::const_iterator itArmyPtrEnd = lstIArmyPtr.end();	

	for (; itArmyPtrBeg!=itArmyPtrEnd; ++itArmyPtrBeg)
	{
		if (ARCH_BUILD_TOFT == (*itArmyPtrBeg)->getBuildDetailType()
			|| ARCH_BARRIER == (*itArmyPtrBeg)->getBuildDetailType()
			|| 0 == (*itArmyPtrBeg)->GetCurrentHp())
		{
			continue;
		}

		// ֻͳ���йܵĲ���
		if (isOnlyTrusteeship && !(*itArmyPtrBeg)->GetTrusteeshipState())
		{
			continue;
		}

		if (0 == (*itArmyPtrBeg)->GetCurrentHp() || *itArmyPtrBeg == armyObjParam
			|| ARCH_BUILD_TOFT == (*itArmyPtrBeg)->getBuildDetailType())
		{
			continue;
		}

		if (ARCH_TRAP == (*itArmyPtrBeg)->getBuildDetailType()
			&& armyObjParam->IsFriendsArmy(*itArmyPtrBeg))
		{
			continue;
		}

		if (ARCH_TRAP == (*itArmyPtrBeg)->getBuildDetailType()
			&& !armyObjParam->IsFriendsArmy(*itArmyPtrBeg)
			&& !armyObjParam->isVisible(*itArmyPtrBeg))
		{
			continue;
		}

		isFriend = armyObjParam->IsFriendsArmy(*itArmyPtrBeg);

		if (PRIEST == armyObjParam->GetArmyOccupation())
		{
			isFriend = !isFriend;
		}

		// �Ѿ�
		if (isFriend)
		{
			if (isOnlyBuilding && BUILD !=(*itArmyPtrBeg)->GetArmyType())
			{
				continue;
			}
			lstFriendArmy.push_back(*itArmyPtrBeg);
		}
		// �о�
		else
		{
			if (isOnlyBuilding && BUILD !=(*itArmyPtrBeg)->GetArmyType())
			{
				continue;
			}
			lstEnemy.push_back(*itArmyPtrBeg);
			cellHasEnemy = true;
		}
	}

	if (cellHasEnemy)
	{
		++countCellsHasEnemy;
	}

	return true;
}

*/

bool CommonFunctionSingleton::getSpecifyDistancePos(const IArmyPtr armyObjParam, 
													const int distanceVal, 
													std::vector<PosXYStru> &vectPos,
													IArmyPtrvectInvect * mapDataPtr)
{
	// ������ʱ�������ӵ�X��Y����
	int Xpos = 0, Ypos = 0;

	bool isSucess = true;

	// ����(��������)�Ķ�ά����

	if (distanceVal <= 0)
	{
		return false;
	}

	vectPos.clear();

	// ��ȡ���ӵ�λ��
	const vector<POS_STRUCT> & armyPos = armyObjParam->GetArmyPos();
	unsigned int posSize = static_cast<unsigned int>(armyPos.size());

	if (0 == posSize)
	{
#ifdef _DEBUG
		assert(0);
#endif
		return false;
	}

	for (unsigned int i=0; i<posSize; ++i)
	{
		Xpos = armyPos[i].PosX;
		Ypos = armyPos[i].PosY;

		// У�鲿������
		isSucess = invalidateXYpos(mapDataPtr, Xpos, Ypos);

		if (!isSucess)
		{
			return false;
		}

		isSucess = GetAllSpecifyPos(PosXYStru(Xpos, Ypos), distanceVal, vectPos, mapDataPtr);

		if (!isSucess)
		{
			return false;
		}
	}

	return true;
}

bool CommonFunctionSingleton::GetAllSpecifyPos(const PosXYStru &posStart,
											   const int distanceVal, 
											   std::vector<PosXYStru> &vectPos,
											   const IArmyPtrvectInvect * mapDataPtr)
{
	// ����(��������)�Ķ�ά����
	int Xpos = posStart.xPos;
	int Ypos = posStart.yPos;

	int	XMaxVal = static_cast<int>((*mapDataPtr).size()) - 1;

	// У��X��������ֵ
	if (XMaxVal < 0)
	{
		return false;
	}

	int YMaxVal = static_cast<int>((*mapDataPtr)[0].size()) - 1;

	// У��Y��������ֵ
	if (YMaxVal < 0)
	{	
		return false;
	}

	/*

	// ��������Ϊ������ʱ
	if (Xpos % 2)
	{
	for (int i=1; i<=distanceVal; i++)
	{
	Ypos += (i % 2) * (-1);
	}
	}
	// ����Ϊż����
	else
	{
	for (int i=0; i<distanceVal; i++)
	{
	Ypos += (i % 2) * (-1);
	}
	}	

	Xpos = Xpos + distanceVal * (-1);

	*/

	if (Xpos & 1)
	{
		Ypos = Ypos - (distanceVal >> 1) - (distanceVal & 1);
	}
	else
	{
		Ypos = Ypos - (distanceVal >> 1);
	}

	Xpos = Xpos - distanceVal;

	// ����ѭ����������
	/*
	y+1
	------------->
	^			-
	-x			- x
	--			- +
	-2			- 2
	-			-
	-			-
	y-1		-
	<------------V

	*/

	// ����y+1
	for (int i=1; i<=distanceVal; ++i)
	{
		Ypos += 1;

		if ((Xpos>=0 && Xpos<=XMaxVal) && (Ypos>=0 && Ypos<=YMaxVal))
		{
			vectPos.push_back(PosXYStru(Xpos, Ypos));
		}
	}

	// ����x+2
	for (int i=1; i<=distanceVal; ++i)
	{
		Xpos += 2;

		if ((Xpos>=0 && Xpos<=XMaxVal) && (Ypos>=0 && Ypos<=YMaxVal))
		{
			vectPos.push_back(PosXYStru(Xpos, Ypos));
		}   
	}
	// ����y-1
	for (int i=1; i<=distanceVal; ++i)
	{
		Ypos -= 1;

		if ((Xpos>=0 && Xpos<=XMaxVal) && (Ypos>=0 && Ypos<=YMaxVal))
		{
			vectPos.push_back(PosXYStru(Xpos, Ypos));
		}   
	}

	// ����x-2
	for (int i=1; i<=distanceVal; ++i)
	{
		Xpos -= 2;

		if ((Xpos>=0 && Xpos<=XMaxVal) && (Ypos>=0 && Ypos<=YMaxVal))
		{
			vectPos.push_back(PosXYStru(Xpos, Ypos));
		}   
	}

	if (0 == vectPos.size())
	{
		return false;
	}
	else
	{
		return true;
	}

}

template <typename A>
bool CommonFunctionSingleton::GetAllSpecifyPos(IArmyPtrvectInvect * mapDataPtr,
											   const PosXYStru &posStart,
											   const int distanceVal, 
											   A &a)
{
	// ����(��������)�Ķ�ά����
	int Xpos = posStart.xPos;
	int Ypos = posStart.yPos;
	bool isNotFindPos = true;

	int	XMaxVal = static_cast<int>((*mapDataPtr).size()) - 1;

	// У��X��������ֵ
	if (XMaxVal < 0)
	{
		return false;
	}

	int YMaxVal = static_cast<int>((*mapDataPtr)[0].size()) - 1;

	// У��Y��������ֵ
	if (YMaxVal < 0)
	{	
		return false;
	}

	/*

	// ��������Ϊ������ʱ
	if (Xpos % 2)
	{
		for (int i=1; i<=distanceVal; i++)
		{
			Ypos += (i % 2) * (-1);
		}
	}
	// ����Ϊż����
	else
	{
		for (int i=0; i<distanceVal; i++)
		{
			Ypos += (i % 2) * (-1);
		}
	}	

	Xpos = Xpos + distanceVal * (-1);

	*/

	if (Xpos & 1)
	{
		Ypos = Ypos - (distanceVal >> 1) - (distanceVal & 1);
	}
	else
	{
		Ypos = Ypos - (distanceVal >> 1);
	}

	Xpos = Xpos - distanceVal;

	// ����ѭ����������
	/*
	y+1
	------------->
	^			-
	-x			- x
	--			- +
	-2			- 2
	-			-
	-			-
	y-1		-
	<------------V

	*/

	// ����y+1
	for (int i=1; i<=distanceVal; ++i)
	{
		Ypos += 1;

		if ((Xpos>=0 && Xpos<=XMaxVal) && (Ypos>=0 && Ypos<=YMaxVal))
		{
			isNotFindPos = false;
			a.fun(Xpos, Ypos);
		}
	}

	// ����x+2
	for (int i=1; i<=distanceVal; ++i)
	{
		Xpos += 2;

		if ((Xpos>=0 && Xpos<=XMaxVal) && (Ypos>=0 && Ypos<=YMaxVal))
		{
			isNotFindPos = false;
			a.fun(Xpos, Ypos);
		}   
	}
	// ����y-1
	for (int i=1; i<=distanceVal; ++i)
	{
		Ypos -= 1;

		if ((Xpos>=0 && Xpos<=XMaxVal) && (Ypos>=0 && Ypos<=YMaxVal))
		{
			isNotFindPos = false;
			a.fun(Xpos, Ypos);
		}   
	}

	// ����x-2
	for (int i=1; i<=distanceVal; ++i)
	{
		Xpos -= 2;

		if ((Xpos>=0 && Xpos<=XMaxVal) && (Ypos>=0 && Ypos<=YMaxVal))
		{
			isNotFindPos = false;
			a.fun(Xpos, Ypos);
		}   
	}

	if (isNotFindPos)
	{
		return false;
	}

	return true;
}

template <typename A>
bool CommonFunctionSingleton::getSpecifyDistancePos(IArmyPtrvectInvect * mapDataPtr,
													const IArmyPtr armyObjParam, 
													const int distanceVal, 
													A &a)
{
	// ������ʱ�������ӵ�X��Y����
	int Xpos = 0, Ypos = 0;

	bool isSucess = true;

	// ����(��������)�Ķ�ά����

	if (distanceVal <= 0)
	{
		return false;
	}

	// ��ȡ���ӵ�λ��
	const vector<POS_STRUCT> & armyPos = armyObjParam->GetArmyPos();
	unsigned int posSize = static_cast<unsigned int>(armyPos.size());

	if (0 == posSize)
	{
#ifdef _DEBUG
		assert(0);
#endif
		return false;
	}

	for (unsigned int i=0; i<posSize; ++i)
	{
		Xpos = armyPos[i].PosX;
		Ypos = armyPos[i].PosY;

		// У�鲿������
		isSucess = invalidateXYpos(mapDataPtr, Xpos, Ypos);

		if (!isSucess)
		{
			return false;
		}

		isSucess = GetAllSpecifyPos(mapDataPtr, PosXYStru(Xpos, Ypos), distanceVal, a);

		if (!isSucess)
		{
			return false;
		}
	}

	return true;
}

bool CommonFunctionSingleton::getSpecifyDistAllPosHasArmy(IArmyPtrvectInvect * mapDataPtr,
														  const CLoadMapInfo * staticBlockMapPtr,
														  IArmyPtr armyObjParam,
														  std::list<PosXYStru> &lstAllPosHasArmy,
														  const int distanceVal)																																
{
	// �������
	lstAllPosHasArmy.clear();

	// ָ��Ȧ���������1
	if (distanceVal < ONE_CIRCLE)
	{
		return false;
	}

	// ����(��������)�Ķ�ά����
	bool isSucess = true;

	SpecifyDistAllPosHasArmy specifyDisAllPosHasArmy(mapDataPtr, staticBlockMapPtr, lstAllPosHasArmy);
	isSucess = getSpecifyDistancePos<SpecifyDistAllPosHasArmy>(mapDataPtr, armyObjParam, distanceVal, specifyDisAllPosHasArmy);

	if (!isSucess)
	{
		return false;
	}

	return true;
}




bool CommonFunctionSingleton::getSpecifyDistanceEnemy(IArmyPtrvectInvect * mapDataPtr,
													  IArmyPtr armyObjParam, 
													  int distanceVal,
													  bool isOnlyBuilding,
													  bool isOnlyTrusteeship,
													  listArmyPtr &lstFriendArmy,
											          listArmyPtr &lstEnemy,
													  int &countCellsHasEnemy)
{
   // �������
   lstFriendArmy.clear();
   lstEnemy.clear();

   // ָ��Ȧ���������1
   if (distanceVal < ONE_CIRCLE)
   {
	   return false;
   }
  
   // ����(��������)�Ķ�ά����
   bool isSucess = true;

   StatFriendArmyAndEnemyStru statFriArmyAndEnemy(armyObjParam, mapDataPtr, isOnlyBuilding, isOnlyTrusteeship, lstFriendArmy, lstEnemy, countCellsHasEnemy);
   isSucess = getSpecifyDistancePos<StatFriendArmyAndEnemyStru>(mapDataPtr, armyObjParam, distanceVal, statFriArmyAndEnemy);

   if (!isSucess)
   {
	   return false;
   }

   return true;
}

bool CommonFunctionSingleton::getGroupArmy(IArmyPtrvectInvect * mapDataPtr, IArmyPtr armyObjSelf, const int disVal, std::list<IArmyPtr>& lstArmy)
{
	listArmyPtr lstFriendArmy;
	listArmyPtr lstEnemy;
	int number = 0;
	
	std::list<IArmyPtr>::iterator itBeg;
	std::list<IArmyPtr>::iterator itEnd;

	for (int i=1; i<=disVal; ++i)
	{
		lstFriendArmy.clear();
		lstEnemy.clear();

		if(getSpecifyDistanceEnemy(mapDataPtr, armyObjSelf, i, false, true, lstFriendArmy, lstEnemy, number))
		{
		    if (PRIEST == armyObjSelf->GetArmyOccupation())
			{
				itBeg = lstEnemy.begin();
				itEnd = lstEnemy.end();

				for (; itBeg!=itEnd; ++itBeg)
				{
					if (BUILD == (*itBeg)->GetArmyType())
					{
						continue;
					}

					lstArmy.push_back(*itBeg);
				}
			}
			else
			{
				itBeg = lstFriendArmy.begin();
				itEnd = lstFriendArmy.end();

				for (; itBeg!=itEnd; ++itBeg)
				{
					lstArmy.push_back(*itBeg);
				}
			}
		}
		else
		{
			return false;
		}
	}

	/*
	const std::vector<POS_STRUCT> &vectPos = armyObjSelf->GetArmyPos();

	if (vectPos.empty())
	{
		return false;
	}

//	IArmyPtrvectInvect &MapData = *(mapDataAndFraise.MapData);
	
	int PosX = (vectPos[0]).PosX;
	int PosY = (vectPos[0]).PosY;

	list<IArmyPtr> &selfPointArmy = (*mapDataPtr)[PosX][PosY];

	itBeg = selfPointArmy.begin();
	itEnd = selfPointArmy.end();

	for (; itBeg!=itEnd; itBeg++)
	{
		// �йܣ������ǳ��ڿ��е�
		//if (Idle_status == armyObjSelf->getArmyActionStatus() && armyObjSelf->GetTrusteeshipState())
		{
			lstArmy.push_back(*itBeg);
		}	
	}
	*/

	return true;
}

int CommonFunctionSingleton::getGroupArmyHP(const std::list<IArmyPtr>& listArmy)
{
	int HPVal = 0;
	std::list<IArmyPtr>::const_iterator itBeg = listArmy.begin();
	std::list<IArmyPtr>::const_iterator itEnd= listArmy.end();

	for (; itBeg!=itEnd; ++itBeg)
	{
		HPVal += (*itBeg)->GetCurrentHp();
	}

	return HPVal;

	//return std::for_each(listArmy.begin(), listArmy.end(), CalculationSumHP());
}

int CommonFunctionSingleton::getGroupArmyFP(const std::list<IArmyPtr>& listArmy)
{
	int	FPVal = 0;

	std::list<IArmyPtr>::const_iterator itBeg = listArmy.begin();
	std::list<IArmyPtr>::const_iterator itEnd = listArmy.end();

	for (; itBeg!=itEnd; ++itBeg)
	{
		FPVal += (*itBeg)->GetAttackPower();
	}

	return FPVal;

	//return std::for_each(listArmy.begin(), listArmy.end(), CalculationSumFP());
}

bool CommonFunctionSingleton::getAttackArmy(IArmyPtrvectInvect * mapDataPtr,
											IArmyPtr armyObjSelf, 
											const int disVal,  
											IArmyPtr &TargetArmyPtr, 
											bool &isAttack, 
											std::list<IArmyPtr> &lstFriendGroup,
											const SOLDIER_TYPE tipTopAttackTar)
{
	std::list<IArmyPtr> lstEnemyGroup;
	int armyTotalHpAndFp = 0;
	int enemyTotalHpAndFp = 0;

	lstFriendGroup.clear();

	// ��ȡ���ӵ���Ұ
	//int scopeVal = armyObjSelf->GetViewScope();

	bool isSuceess = getApproEnemyToAttackOnAimRule(mapDataPtr, armyObjSelf, disVal, TargetArmyPtr, tipTopAttackTar);

	if (!isSuceess)
	{
		return false;
	}

	if (!TargetArmyPtr)
	{
		return true;
	}

	// Ѱ�����Լ�Ϊ���ĵ������Ѿ�(���йܲ��ӣ���NPC���ӣ�)
	isSuceess = getGroupArmy(mapDataPtr, armyObjSelf, disVal, lstFriendGroup);

	// ���Լ�������ȥ
	lstFriendGroup.push_back(armyObjSelf);

	if(!isSuceess) 
	{
		return false;
	}

	// Ѱ����Ŀ��BΪ���İ����Լ����ӵ����ео�(���йܲ��ӣ���NPC���ӣ�)
	isSuceess = getGroupArmy(mapDataPtr, TargetArmyPtr, disVal, lstEnemyGroup);

	lstEnemyGroup.push_back(TargetArmyPtr);
	
	if(!isSuceess) 
	{
		return false;
	}
	
	std::list<IArmyPtr>::iterator itBeg = lstEnemyGroup.begin();
	std::list<IArmyPtr>::iterator itEnd = lstEnemyGroup.end();

	bool isEnemyAllChurchman = true;

	for (; itBeg!=itEnd; ++itBeg)
	{
		if (PRIEST != (*itBeg)->GetArmyOccupation())
		{
			isEnemyAllChurchman = false;
			break;
		}
	}

	// ����о���ֻ����ʦ���Ͳ����������Աȣ�ѡ�񹥻�
	if (isEnemyAllChurchman)
	{
		isAttack = true;
	}
	else
	{
		enemyTotalHpAndFp = getGroupArmyFP(lstEnemyGroup) + getGroupArmyHP(lstEnemyGroup);

		armyTotalHpAndFp = getGroupArmyFP(lstFriendGroup) + getGroupArmyHP(lstFriendGroup);

		// �������ͣ�����
		if(enemyTotalHpAndFp > 2 * armyTotalHpAndFp)
		{
			isAttack = false;
		}
		// �������
		else
		{
			isAttack = true;
		}

	}

	return true;
}

bool CommonFunctionSingleton::GetPosOfCountermarch(IArmyPtrvectInvect * mapDataPtr, CLoadMapInfo *ClassImpedientPtr, IArmyPtr armyObjParam, PosXYStru &posXY)
{
	bool isSucess = true;

	IArmyPtr bastillePtr = NULL; // �Ǳ������� ��
	IArmyPtr governmentPtr = NULL; // ��������

	const std::vector<POS_STRUCT> &armyPos = armyObjParam->GetArmyPos();

	if (armyPos.empty())
	{
#ifdef _DEBUG
		assert(0 && "��������Ϊ��");
#endif
		return false;
	}

	list<CityInfo> &lstCityInfo = ClassImpedientPtr->m_CityInfo;
	std::map<PosXYStru, IArmyPtr> Pos2Bastille;
   
	if (!lstCityInfo.empty())
	{
		list<CityInfo>::iterator itBeg = lstCityInfo.begin();
		list<CityInfo>::iterator itEnd = lstCityInfo.end();
		
		list<POS_STRUCT>::iterator itPos; 
		unsigned int cityId = 0;

		for (; itBeg!=itEnd; ++itBeg)
		{
			cityId = itBeg->uCityID % 10000;
			bastillePtr = armyObjParam->getArmyByID(cityId);

			if (NULL == bastillePtr)
			{
#ifdef _DEBUG
				assert(0 && "����ָ��Ϊ��");
#endif
				return false;
			}

			// ���Ѿ���������
			if (!armyObjParam->IsFriendsArmy(bastillePtr))
			{
				bastillePtr = NULL;
				continue;
			}

			if (0 != bastillePtr->GetCurrentHp())
			{
				list<POS_STRUCT> &lstCityPos = itBeg->CityPos;
				
				if (lstCityPos.empty())
				{
#ifdef _DEBUG
					assert(0 && "��������Ϊ��");
#endif
					return false;
				}

				itPos = lstCityPos.begin();


				Pos2Bastille[PosXYStru(itPos->PosX, itPos->PosY)] = bastillePtr;
			}
		}
	}

	// �ҵ���û���ݻٵĳ���
	if (!Pos2Bastille.empty())
	{
		unsigned int distVal = 0;
		std::map<PosXYStru, IArmyPtr>::iterator itPosBeg = Pos2Bastille.begin();
		std::map<PosXYStru, IArmyPtr>::iterator itPosEnd = Pos2Bastille.end();
		
		unsigned int disValTmp = 0;

		const PosXYStru &posXY = itPosBeg->first;

		bastillePtr = itPosBeg->second;

		distVal = getDistanceTwoNode(armyPos[0].PosX, armyPos[0].PosY, posXY.xPos, posXY.yPos);

		for (++itPosBeg; itPosBeg!=itPosEnd; ++itPosBeg)
		{
			const PosXYStru &posXYTmp = itPosBeg->first;

			disValTmp = getDistanceTwoNode(armyPos[0].PosX, armyPos[0].PosY, posXYTmp.xPos, posXYTmp.yPos);

			if (distVal > disValTmp)
			{
				distVal = disValTmp;
				bastillePtr = itPosBeg->second;
			}
		}
	}
	else
	{
		GovInfo &GovPos = ClassImpedientPtr->GovPos;

		unsigned int uGovID = GovPos.uGovID % 10000;

		governmentPtr = armyObjParam->getArmyByID(uGovID);

		if (NULL == governmentPtr)
		{
#ifdef _DEBUG
			assert(0 && "��������ָ��Ϊ��");
#endif
			return false;
		}

		// ���Ѿ���������
		if (!armyObjParam->IsFriendsArmy(governmentPtr))
		{
			governmentPtr = NULL;
		}
		else if (0 != governmentPtr->GetCurrentHp())
		{
			list<POS_STRUCT> &lstGovPos = GovPos.GovPos;

			if (lstGovPos.empty())
			{
#ifdef _DEBUG
				assert(0 && "�������ĵ�����Ϊ��");
#endif
				return false;
			}
		}
	}

	/*
	for(;;)
	{
		isSucess = getSpecifyDistanceEnemy(mapDataPtr, armyObjParam, scopeVal, true, true, lstFriendArmy, lstEnemy, countCellsHasEnemy, setArmyId);

		if (!isSucess)
		{
			break;
		}

		if (!lstFriendArmy.empty())
		{
			std::list<IArmyPtr>::const_iterator itBen = lstFriendArmy.begin();
			std::list<IArmyPtr>::const_iterator itEnd = lstFriendArmy.end();

			for (; itBen!=itEnd; itBen++)
			{
				// ���Ѿ���������
				if (!armyObjParam->IsFriendsArmy(*itBen))
				{
					continue;
				}

				// �Ǳ�/����
				if ((*itBen)->getBuildDetailType() == BUILDINGTYPE_BASTILE)
				{
					bastillePtr = *itBen;
					hasFindbastille = true;
					break;
				}

				// ��������
				if ((*itBen)->getBuildDetailType() == BUILDINGTYPE_GOVERNMENT)
				{
					governmentPtr = *itBen;
				}

			}

			if (hasFindbastille)
			{
				break;
			}
		}

		if (scopeVal < XMaxVal)
		{
			scopeVal++;
		}
		else
		{
			return false;
		}

	}
	*/

	// ����û�гǱ�����������
	if (NULL == bastillePtr && NULL == governmentPtr)
	{
		return true;
	}

	pairInSet setXYpos;
	pairInSet::iterator itTmp;
	int Xpos = 0, Ypos = 0;
	bool isFindTargetPos = false;
	std::vector<PosXYStru> vectPos;

	// ������/�Ǳ�������һ��Ϊ�ƶ�Ŀ��
	if (NULL != bastillePtr)
	{
		const vector<POS_STRUCT> &PosVector = bastillePtr->GetArmyPos();
		vector<POS_STRUCT>::const_iterator itBeg = PosVector.begin();
		vector<POS_STRUCT>::const_iterator itEnd = PosVector.end();

		for (; itBeg!=itEnd; ++itBeg)
		{
			setXYpos.insert(make_pair(itBeg->PosX, itBeg->PosY));
		}

		for (itBeg = PosVector.begin(); itBeg!=itEnd; ++itBeg)
		{
			Xpos = itBeg->PosX;
			Ypos = itBeg->PosY;
			vectPos.clear();

			isSucess = getSpecifyDistancePos(armyObjParam, ONE_CIRCLE, vectPos, mapDataPtr);

			if (!isSucess)
			{
				return false;
			}

			isFindTargetPos = isFindCountermarchPos(mapDataPtr, vectPos, setXYpos, posXY);

			if (isFindTargetPos)
			{
				break;
			}
		}

		// �ڳ���/�Ǳ�������һ��û���ҵ����ʵĸ���,Ĭ���Ƶ������ĳһ��������
		if (!isFindTargetPos)
		{
//#ifdef _DEBUG
//			assert(0 && "�ڳ����Ե��һ��û���ҵ����ʵ�λ��");
//#endif
			posXY.xPos = Xpos;
			posXY.yPos = Ypos;	
		}
	}
	// �������������������3��Χ���ƶ�
	else
	{
		const vector<POS_STRUCT> &PosVector = governmentPtr->GetArmyPos();
		vector<POS_STRUCT>::const_iterator itBeg = PosVector.begin();
		vector<POS_STRUCT>::const_iterator itEnd = PosVector.end();

		for (; itBeg!=itEnd; ++itBeg)
		{
			setXYpos.insert(make_pair(itBeg->PosX, itBeg->PosY));
		}

		for (itBeg = PosVector.begin(); itBeg!=itEnd; ++itBeg)
		{
			Xpos = itBeg->PosX;
			Ypos = itBeg->PosY;

			for (int scopeValTmp=ONE_CIRCLE; scopeValTmp<FOUR_CIRCLE; ++scopeValTmp)
			{
				vectPos.clear();

				isSucess = getSpecifyDistancePos(armyObjParam, scopeValTmp, vectPos, mapDataPtr);

				if (!isSucess)
				{
					return false;
				}

				isFindTargetPos = isFindCountermarchPos(mapDataPtr, vectPos, setXYpos, posXY);

				if (isFindTargetPos)
				{
					break;
				}
			}

			if (isFindTargetPos)
			{
				break;
			}
		}
		// ����������������û���ҵ����ʵĸ���,Ĭ���Ƶ��������ĵ�ĳһ��������
		if (!isFindTargetPos)
		{
#ifdef _DEBUG
			assert(0 && "�ڳ����Ե��һ��û���ҵ����ʵ�λ��");
#endif
			posXY.xPos = Xpos;
			posXY.yPos = Ypos;	
		}
	}

	return true;
}

bool CommonFunctionSingleton::isFindCountermarchPos(IArmyPtrvectInvect * /*mapDataPtr*/, const std::vector<PosXYStru> &vectPos, const pairInSet &setXYpos, PosXYStru &posXY)
{
	std::vector<PosXYStru>::const_iterator lstPosBeg, lstPosEnd;
	pairInSet::const_iterator itTmp;
	int XposTmp = 0, YposTmp = 0;

	lstPosBeg = vectPos.begin();
	lstPosEnd = vectPos.end();

	for (; lstPosBeg!=lstPosEnd; ++lstPosBeg)
	{
		XposTmp = lstPosBeg->xPos;
		YposTmp = lstPosBeg->yPos;

		// ����Ҳ����ʹ�ò�����õ����Ǳ�Ե���ֲ��ǽ������
		itTmp = setXYpos.find(make_pair(XposTmp, YposTmp));

		//IArmyPtrvectInvect *MapData = mapDataAndFraise.MapData;

		//list<IArmyPtr> &lstMapData = (*mapDataPtr)[XposTmp][YposTmp];

		if (itTmp == setXYpos.end() /*&& lstMapData.size() < 5*/)
		{
			posXY.xPos = XposTmp;
			posXY.yPos = YposTmp;
			break;
		}
	}

	if (lstPosBeg!=lstPosEnd)
	{
		return true;
	}
	else
	{	
		return false;
	}
}



bool CommonFunctionSingleton::getFollowObjPtr(IArmyPtrvectInvect * mapDataPtr , IArmyPtr armyObj,  const int iWithinScopeVal, IArmyPtr &followArmyObj)
{
	listArmyPtr lstFriendArmy;
	listArmyPtr lstEnemy;
	int number = 0;
	bool isSucess = false;

	listArmyPtr::iterator itBeg, itEnd;

	ArmyPtrVector friendContainer;

	// �ӵ�һȦ��ʼ�ң������Լ�����Ѿ����ӣ�
	for (int i=1; i<=iWithinScopeVal * 2; ++i)
	{
		lstFriendArmy.clear();
		lstEnemy.clear();

		isSucess = getSpecifyDistanceEnemy(mapDataPtr, armyObj, i, false, false, lstFriendArmy, lstEnemy, number);

		if (!isSucess)
		{
			return false;
		}

		if (PRIEST == armyObj->GetArmyOccupation())
		{
			if (lstEnemy.empty())
			{
				continue;
			}
			// �ҵ����Ѿ�
			else
			{
				itBeg = lstEnemy.begin();
				itEnd = lstEnemy.end();

				for (; itBeg!=itEnd; ++itBeg)
				{
					// �����սʿ���߹�����
					if ((*itBeg)->GetArmyOccupation() == WARRIOR 
						|| (*itBeg)->GetArmyOccupation()== SHOOTER
						|| (*itBeg)->GetArmyOccupation()== MAGE) 
					{
						friendContainer.push_back(*itBeg);
					}
				}
			}
		}
		else
		{
			if (lstFriendArmy.empty())
			{
				continue;
			}
			// �ҵ����Ѿ�
			else
			{
				itBeg = lstFriendArmy.begin();
				itEnd = lstFriendArmy.end();

				for (; itBeg!=itEnd; ++itBeg)
				{
					// �����սʿ���߹�����
					if ((*itBeg)->GetArmyOccupation() == WARRIOR 
						|| (*itBeg)->GetArmyOccupation()== SHOOTER
						|| (*itBeg)->GetArmyOccupation()== MAGE) 
					{
						friendContainer.push_back(*itBeg);
					}
				}
			}
		}

	}

	//IArmyPtrvectInvect *MapData = mapDataAndFraise.MapData;
	const std::vector<POS_STRUCT>& vectPos = armyObj->GetArmyPos();

	if (vectPos.empty())
	{
		return false;
	}

	int Xpos = vectPos[0].PosX;
	int Ypos = vectPos[0].PosY;

	list<IArmyPtr> &selfPointArmy = (*mapDataPtr)[Xpos][Ypos];

	itBeg = selfPointArmy.begin();
	itEnd = selfPointArmy.end();

	for (; itBeg!=itEnd; ++itBeg)
	{
		// �������Լ�
		if (*itBeg == armyObj)
		{
			continue;
		}

		// �����սʿ���߹�����
		if((*itBeg)->GetArmyOccupation() == WARRIOR 
			||(*itBeg)->GetArmyOccupation()== SHOOTER
			|| (*itBeg)->GetArmyOccupation()== MAGE)
		{
			friendContainer.push_back(*itBeg);
		}
	}

	// �޺��ʵĸ������
	if (friendContainer.empty())
	{
		return true;
	}

	ArmyPtrVector::iterator itVectBeg = friendContainer.begin();
	ArmyPtrVector::iterator itVectEnd = friendContainer.end();
	IArmyPtr army = *itVectBeg;

	for (++itVectBeg; itVectBeg!=itVectEnd; ++itVectBeg)
	{
		//����Ƿ�ʦ
		if (army->GetArmyOccupation() == MAGE)
		{
			if ((*itVectBeg)->GetArmyOccupation() == WARRIOR)
			{
				army = (*itVectBeg);
			}
			if ((*itVectBeg)->GetArmyOccupation() == SHOOTER)
			{
				army = (*itVectBeg);
			}
			if ((*itVectBeg)->GetArmyOccupation() == MAGE
				&& (*itVectBeg)->GetCurrentHp() > army->GetCurrentHp())
			{
				army=(*itVectBeg);
			}

			continue;
		}
		// ����ǹ�����
		if (army->GetArmyOccupation() == SHOOTER)
		{
			// ����ȽϵĶ�����սʿ����սʿ����
			if ((*itVectBeg)->GetArmyOccupation() == WARRIOR)
			{
				army = (*itVectBeg);
			}

			// ����ȽϵĶ����ǹ����־�ѡѪ���
			if ((*itVectBeg)->GetArmyOccupation() == SHOOTER 
				&& (*itVectBeg)->GetCurrentHp() > army->GetCurrentHp())
			{
				army = (*itVectBeg);
			}

			continue;
		}

		// �����սʿ
		if (army->GetArmyOccupation() == WARRIOR)
		{
			// ����ȽϵĶ�����սʿ��Ѫ���
			if((*itVectBeg)->GetArmyOccupation() == WARRIOR 
				&& (*itVectBeg)->GetCurrentHp() > army->GetCurrentHp())
			{
				army=(*itVectBeg);
			}

			continue;
		}

	}

	followArmyObj = army;

	return true;
}


bool CommonFunctionSingleton::setMoveTargetPos(moveFunParam &attOrMoveFunParm)
{
	FunParam &funParam = attOrMoveFunParm.funParam;

	if (NULL == funParam.seekPathPtr || NULL == funParam.strategyManagerPtr)
	{
		return false;
	}

	std::list<PosXYStru> lstAllPosHasArmy;

	bool isSucess = false;
	/*
	bool isSucess = CommonFunctionSingleton::getInstance()
		->getSpecifyDistAllPosHasArmy(funParam.mapDataParamPtr,(funParam.seekPathPtr)->GetLoadMapInfoPtr(), funParam.armyPtr, lstAllPosHasArmy, AI_ONE_VALUE_NUM, true);

	if (!isSucess)
	{
		return false;
	}
*/
	//if (AI_FOUR_VALUE_NUM == lstAllPosHasArmy.size())
	//{
	//	return true;
	//}

	// ���й�״̬��Ϊ����
	if (funParam.strategyManagerPtr && !(funParam.armyPtr)->GetTrusteeshipState())
	{
		(funParam.strategyManagerPtr)->getMgrParam().armyCurrentStatus = Idle_status;
	}

	// �������һ�������еĹ���Ŀ��
	funParam.enemyID = 0;
	(funParam.strategyManagerPtr)->getMgrParam().enemyID = 0;

	// �ƶ�Ŀ����ͬʱ��������Ѱ·
	if (attOrMoveFunParm.posTarget == funParam.moveTagetPos && 0 != (funParam.movePosVector).size())
	{
		return true;
	}
	else
	{
		(funParam.movePosVector).clear();

		// ��ȡ��������
		const std::vector<POS_STRUCT> &armyObjPos = (funParam.armyPtr)->GetArmyPos();

		if (0 == armyObjPos.size())
		{
#ifdef _DEBUG
			assert(0 && "��������Ϊ��");
#endif
			return false;
		}

		PosXYStru ptArmy;
		ptArmy.xPos = armyObjPos[0].PosX;
		ptArmy.yPos = armyObjPos[0].PosY;

		if (ptArmy == attOrMoveFunParm.posTarget)
		{
			return true;
		}

		if (ptArmy != attOrMoveFunParm.posTarget)
		{
			PosXYStru &posTargetTmp = attOrMoveFunParm.posTarget;
			
			bool isStaticBlook = funParam.armyPtr->isStaticBlook(posTargetTmp.xPos, posTargetTmp.yPos);
			
			if (isStaticBlook)
			{
				 bool isSucess = funParam.armyPtr->getNearPosInOneCircle(posTargetTmp, posTargetTmp);

				 if (!isSucess)
				 {
					 return false;
				 }
			}

			// ����Ѱ·
			isSucess = (funParam.seekPathPtr)->findPath(ptArmy, attOrMoveFunParm.posTarget, funParam.movePosVector);

			if (!isSucess || 0 == (funParam.movePosVector).size())
			{
				return false;
			}

			funParam.moveTagetPos = attOrMoveFunParm.posTarget;

			funParam.armyCurrentStatus = Move_status;
		}
	}

	return true;
}

bool CommonFunctionSingleton::setAttackTarget(attackFunParam &attOrMoveFunParm)
{
	FunParam &funParam = attOrMoveFunParm.funParam;

	if (NULL == funParam.seekPathPtr || NULL == funParam.strategyManagerPtr)
	{
		return false;
	}

	bool isSucess =  false;
	std::list<PosXYStru> lstAllPosHasArmy;
/*
	bool isSucess = CommonFunctionSingleton::getInstance()->getSpecifyDistAllPosHasArmy(funParam.mapDataParamPtr, (funParam.seekPathPtr)->GetLoadMapInfoPtr(), funParam.armyPtr, lstAllPosHasArmy, AI_ONE_VALUE_NUM, true);

	if (!isSucess)
	{
		return false;
	}
*/
	//if (AI_FOUR_VALUE_NUM == lstAllPosHasArmy.size())
	//{
	//	return true;
	//}

	// ���й�״̬��Ϊ����
	if (funParam.strategyManagerPtr && !(funParam.armyPtr)->GetTrusteeshipState())
	{
		(funParam.strategyManagerPtr)->getMgrParam().armyCurrentStatus = Idle_status;
	}

	funParam.armyCurrentStatus = Attack_status;	

	// ��ȡ�о�����
	const std::vector<POS_STRUCT> &enemyObjPos = ((funParam.armyPtr)->getArmyByID(attOrMoveFunParm.attackTargetArmyID))->GetArmyPos();

	PosXYStru ptEnemy;
	ptEnemy.xPos = enemyObjPos[0].PosX;
	ptEnemy.yPos = enemyObjPos[0].PosY;

	// ����Ŀ��������߹���Ŀ����ͬ���ǽ����������£���Ҫ����Ѱ·
	if (funParam.enemyID != attOrMoveFunParm.attackTargetArmyID 
		|| (funParam.enemyID == attOrMoveFunParm.attackTargetArmyID && BUILD != ((funParam.armyPtr)->getArmyByID(attOrMoveFunParm.attackTargetArmyID))->GetArmyType() && ptEnemy != funParam.moveTagetPos))
	{
		// ��ȡ��������
		PosXYStru ptArmy;
		const std::vector<POS_STRUCT> &armyObjPos = (funParam.armyPtr)->GetArmyPos();
		ptArmy.xPos = armyObjPos[0].PosX;
		ptArmy.yPos = armyObjPos[0].PosY;

		(funParam.movePosVector).clear();

		if (ptEnemy != ptArmy)
		{
			bool isStaticBlook = funParam.armyPtr->isStaticBlook(ptEnemy.xPos, ptEnemy.yPos);

			if (isStaticBlook)
			{
				bool isSucess = funParam.armyPtr->getNearPosInOneCircle(ptEnemy, ptEnemy);

				if (!isSucess)
				{
					return false;
				}
			}

			// ����Ѱ·
			isSucess = (funParam.seekPathPtr)->findPath(ptArmy, ptEnemy, funParam.movePosVector);

			if (!isSucess || 0 == (funParam.movePosVector).size())
			{
				return false;
			}		
		}
		funParam.enemyID = attOrMoveFunParm.attackTargetArmyID;

		funParam.moveTagetPos = ptEnemy;
	}

	return true;
}

bool CommonFunctionSingleton::MoveToSecurityPos(IArmyPtrvectInvect * mapDataParamPtr,  
												SeekPath * seekPathArithmParamPtr,
												IArmyPtr armyObj, 
												const IArmyPtr attackMeArmy, 
												PosXYStru &posMoveTo)
{
	bool isSucess = false;
	PosXYStru armyPos;
	int lengthPath = 0, lengthPathTmp = 0;
	std::list<PosXYStru> lstPos;
	std::list<PosXYStru> lstFindPos;
	std::list<PosXYStru>::iterator itBeg, itEnd;
	list<IArmyPtr>::iterator itArmy;

	const std::vector<POS_STRUCT> &armyPosVect = armyObj->GetArmyPos();

	int viewScope = attackMeArmy->GetViewScope();

	int twiceViewScpoe = 2 * viewScope;
	
	if (armyPosVect.empty())
	{
		return false;
	}
	
	armyPos.xPos = armyPosVect[0].PosX;
	armyPos.yPos = armyPosVect[0].PosY;

	for (++viewScope; viewScope <= twiceViewScpoe; ++viewScope)
	{

		lstFindPos.clear();

		SecurityPosTarget securityPosTarget(armyObj, mapDataParamPtr, seekPathArithmParamPtr, lstFindPos);
		isSucess = getSpecifyDistancePos<SecurityPosTarget>(mapDataParamPtr, attackMeArmy, viewScope, securityPosTarget);

		if (!isSucess)
		{
			return false;
		}
		
		if (!lstFindPos.empty())
		{
			break;
		}
	}

	if (lstFindPos.empty())
	{
		return true;
	}

	itBeg = lstFindPos.begin();
	itEnd = lstFindPos.end();

	posMoveTo = *itBeg;
	lengthPath = getDistanceTwoNode(armyPos.xPos, armyPos.yPos, (*itBeg).xPos, (*itBeg).yPos);

	for (++itBeg; itBeg!=itEnd; ++itBeg)
	{
		lengthPathTmp = getDistanceTwoNode(armyPos.xPos, armyPos.yPos, (*itBeg).xPos, (*itBeg).yPos);
		if (lengthPathTmp < lengthPath)
		{
			lengthPath = lengthPathTmp;
			posMoveTo = *itBeg;
		}
	}

	
	return true;
}

IArmyPtr CommonFunctionSingleton::getNearestEnemyOutView(IArmyPtrvectInvect * mapDataPtr, IArmyPtr armyObjParam)
{
	listArmyPtr lstFriendArmy;
	listArmyPtr lstEnemy;
	int number = 0;

	listArmyPtr EnemyClergyContainer;		// ��ʦ����
	listArmyPtr EnemyRabbiContainer;		// ��ʦ����
	listArmyPtr EnemyBowContainer;			// ����������
	listArmyPtr EnemyWarriorContainer;		// սʿ����
	listArmyPtr goodsArmyContainer;			// ��Ʒ����
	listArmyPtr buildingArmyContainer;		// ����������
	int totalEnemy = 0;						// �ܵо���

//	for(int i = armyObjParam->GetAttackScope()+1; totalEnemy < SELF_DEFINE_ARMY_NUM; ++i)
	for(int i = armyObjParam->GetAttackScope()+1; totalEnemy < 1; ++i)
	{
		lstFriendArmy.clear();
		lstEnemy.clear();
		
		if(getSpecifyDistanceEnemy(mapDataPtr,armyObjParam,i,false,false,lstFriendArmy,lstEnemy,number))
		{
			std::list<IArmyPtr>::iterator itr = lstEnemy.begin();
			std::list<IArmyPtr>::iterator itrEnd = lstEnemy.end();
			
			for (; itr!=itrEnd; ++itr)
			{
				if (0 == (*itr)->GetCurrentHp())
				{
					continue;
				}

				if ((*itr)->GetArmyOccupation() == PRIEST)
				{
					EnemyClergyContainer.push_back(*itr);
				}
				else if ((*itr)->GetArmyOccupation() == MAGE)
				{
					EnemyRabbiContainer.push_back(*itr);
				}
				else if ((*itr)->GetArmyOccupation() == SHOOTER)
				{
					EnemyBowContainer.push_back(*itr);
				}
				else if ((*itr)->GetArmyOccupation() == WARRIOR)
				{
					EnemyWarriorContainer.push_back(*itr);
				}
				else if ( (*itr)->GetArmyType() == BUILD)
				{
					buildingArmyContainer.push_back(*itr);
				}
				else 
				{
					goodsArmyContainer.push_back(*itr);
				}			
				
				++totalEnemy;
			}
		}
		else
		{
			break;
		}
	}

	if(!EnemyClergyContainer.empty())   
	{
		return getMinDistanceArmy(armyObjParam,EnemyClergyContainer);
	}

	if (!EnemyRabbiContainer.empty())
	{
		return getMinDistanceArmy(armyObjParam,EnemyRabbiContainer);	
	}

	if (!EnemyBowContainer.empty())
	{
		return getMinDistanceArmy(armyObjParam,EnemyBowContainer);	
	}

	if (!EnemyWarriorContainer.empty())
	{
		return getMinDistanceArmy(armyObjParam,EnemyWarriorContainer);
	}

	if (!goodsArmyContainer.empty())
	{
		return getMinDistanceArmy(armyObjParam,goodsArmyContainer);
	}

	if (!buildingArmyContainer.empty())
	{
		return getMinDistanceArmy(armyObjParam,buildingArmyContainer);
	}

	return NULL;
}

IArmyPtr CommonFunctionSingleton:: getMinDistanceArmy(const IArmyPtr armyObjParam, const std::list<IArmyPtr> &listArmy)
{
	std::list<IArmyPtr> minDisListArmy; 	// ��̾���о�����						
	std::list<IArmyPtr>::const_iterator itBeg = listArmy.begin();
	std::list<IArmyPtr>::const_iterator itEnd = listArmy.end();
	
	minDisListArmy.push_back(*itBeg);

	const std::vector<POS_STRUCT> &enemyPosVect = (*itBeg)->GetArmyPos();
	const std::vector<POS_STRUCT> &armyPosVect = armyObjParam->GetArmyPos();

	if (enemyPosVect.empty() || armyPosVect.empty())
	{
#ifdef _DEBUG
		assert(0);
#endif
		return NULL;
	}

	int distanctTmp = 0;

	int minDistan = getDistanceTwoNode(armyPosVect[0].PosX, armyPosVect[0].PosY, enemyPosVect[0].PosX, enemyPosVect[0].PosY);

	for (++itBeg; itBeg!=itEnd; ++itBeg)
	{
		const std::vector<POS_STRUCT> &enemyPosVectTmp = (*itBeg)->GetArmyPos();
		
		distanctTmp = getDistanceTwoNode(armyPosVect[0].PosX, armyPosVect[0].PosY, enemyPosVectTmp[0].PosX, enemyPosVectTmp[0].PosY);
		
		if (distanctTmp < minDistan)
		{
			minDisListArmy.clear();
			minDisListArmy.push_back(*itBeg);
		}
		else if (distanctTmp == minDistan)
		{
			minDisListArmy.push_back(*itBeg);
		}
	}

	if (minDisListArmy.empty())
	{
		return NULL;
	}

	return getMaxHPArmy(minDisListArmy);
}

int CommonFunctionSingleton::GetRandomValFromOneToHundred()
{
	static uniform_int<> distribution(RAMDOM_FROM_VALUE, RAMDOM_TO_VALUE);
	static mt19937 engine ;
	static variate_generator<mt19937 &, uniform_int<> > myrandom (engine, distribution);

	return myrandom();
}

IArmyPtr CommonFunctionSingleton::getArmyByPriority(const listArmyPtr &enemyClergyContainer, const listArmyPtr &enemyRabbiContainer, const listArmyPtr &enemyBowContainer, const listArmyPtr& enemyWarriorContainer, const SOLDIER_TYPE tipTopAttackTar)
{
	vector<const listArmyPtr *> allEnemyContainer;

	if ( tipTopAttackTar == WARRIOR)//�����սʿ������˳��Ϊսʿ����ʦ����ʦ������
	{
		if (!enemyWarriorContainer.empty())
		{
			allEnemyContainer.push_back(&enemyWarriorContainer);
		}

		if (!enemyClergyContainer.empty())
		{
			allEnemyContainer.push_back(&enemyClergyContainer);
		}
	
		if (!enemyRabbiContainer.empty())
		{
			allEnemyContainer.push_back(&enemyRabbiContainer);
		}

		if (!enemyBowContainer.empty())
		{
			allEnemyContainer.push_back(&enemyBowContainer);	
		}
	}
	else if (tipTopAttackTar == SHOOTER)//����ǹ����֣�����˳��Ϊ���֣���ʦ����ʦ��սʿ
	{
		if (!enemyBowContainer.empty())
		{
			allEnemyContainer.push_back(&enemyBowContainer);	
		}

		if (!enemyClergyContainer.empty())
		{
			allEnemyContainer.push_back(&enemyClergyContainer);
		}

		if (!enemyRabbiContainer.empty())
		{
			allEnemyContainer.push_back(&enemyRabbiContainer);
		}

		if (!enemyWarriorContainer.empty())
		{
			allEnemyContainer.push_back(&enemyWarriorContainer);
		}
	}
	else if(tipTopAttackTar == MAGE)//����Ƿ�ʦ������˳��Ϊ��ʦ����ʦ�����֣�սʿ
	{
		if (!enemyRabbiContainer.empty())
		{
			allEnemyContainer.push_back(&enemyRabbiContainer);
		}

		if (!enemyClergyContainer.empty())
		{
			allEnemyContainer.push_back(&enemyClergyContainer);
		}

		if (!enemyBowContainer.empty())
		{
			allEnemyContainer.push_back(&enemyBowContainer);	
		}

		if (!enemyWarriorContainer.empty())
		{
			allEnemyContainer.push_back(&enemyWarriorContainer);
		}
	}
	else if(tipTopAttackTar == PRIEST || tipTopAttackTar == SOLDIERTYPE_NONE)//�������ʦ������˳��Ϊ��ʦ����ʦ�����֣�սʿ
	{
		if (!enemyClergyContainer.empty())
		{
			allEnemyContainer.push_back(&enemyClergyContainer);
		}

		if (!enemyRabbiContainer.empty())
		{
			allEnemyContainer.push_back(&enemyRabbiContainer);
		}

		if (!enemyBowContainer.empty())
		{
			allEnemyContainer.push_back(&enemyBowContainer);
		}
	
		if (!enemyWarriorContainer.empty())
		{
			allEnemyContainer.push_back(&enemyWarriorContainer);
		}
	}

	IArmyPtr enemyPtr = NULL;

	int size = static_cast<int>(allEnemyContainer.size());

	for (int i = 0; i <size; ++i)
	{
		enemyPtr = getMaxHPArmy(*allEnemyContainer[i]);

		if (NULL != enemyPtr)
		{
			return enemyPtr;
		}
	}

	return enemyPtr;
}

bool CommonFunctionSingleton::GetProximateNotStaticBlockPos(const CLoadMapInfo *ClassImpedient, const PosXYStru &armyPos, IArmyPtrvectInvect * mapDataPtr, PosXYStru &moveTagetPos)
{
	bool isBlock = ClassImpedient->IsBlockoff(moveTagetPos.xPos, moveTagetPos.yPos);
	int distVal = 1, shortcutPath = 0;
	bool isSucess = false;
	std::vector<PosXYStru> vectPos;
	std::vector<PosXYStru> ::iterator itBeg, itEnd;

	if (isBlock)
	{
		for (;;)
		{
			isSucess = GetAllSpecifyPos(moveTagetPos, distVal, vectPos, mapDataPtr);

			if (!isSucess)
			{
				return false;
			}
		
			itBeg = vectPos.begin();

			for (; itBeg!=vectPos.end();)
			{
				if (ClassImpedient->IsBlockoff(itBeg->xPos, itBeg->yPos))
				{
					itBeg = vectPos.erase(itBeg);
				}
				else
				{
					++itBeg;
				}
			}

			if (!vectPos.empty())
			{
				break;
			}

			distVal++;
		}
	}
	else
	{
		return true;
	}

	if (vectPos.empty())
	{
		return true;
	}

	itBeg = vectPos.begin();
	itEnd = vectPos.end();

	moveTagetPos = *itBeg;

	shortcutPath = getDistanceTwoNode(armyPos.xPos, armyPos.yPos, itBeg->xPos, itBeg->yPos);
	
	int pathLengValTmp = 0;

	for (++itBeg; itBeg!=itEnd; ++itBeg)
	{
		pathLengValTmp = getDistanceTwoNode(armyPos.xPos, armyPos.yPos, itBeg->xPos, itBeg->yPos);

		if (shortcutPath > pathLengValTmp)
		{
			shortcutPath = pathLengValTmp;
			moveTagetPos = *itBeg;
		}
	}

	return true;
}

bool CommonFunctionSingleton::getArmysInPos(const PosXYStru &pos, const IArmyPtrvectInvect * mapDataPtr, std::vector<IArmyPtr> &Armys)
{
	if (NULL == mapDataPtr)
	{
		return false;
	}
	
	const list<IArmyPtr> &armyLst = (*mapDataPtr)[pos.xPos][pos.yPos];
	Armys.clear();

	Armys.insert(Armys.end(), armyLst.begin(), armyLst.end());

	return true;
}

bool CommonFunctionSingleton::getSelfBlastAttArmys(const PosXYStru &pos, const IArmyPtrvectInvect * mapDataPtr, std::vector<IArmyPtr> &armysCenter, std::vector<IArmyPtr> &armysInOneCircle)
{
	if (NULL == mapDataPtr)
	{
		return false;
	}

	std::vector<PosXYStru> vectPos;
	armysCenter.clear();
	armysInOneCircle.clear();
	const list<IArmyPtr> &armyLst = (*mapDataPtr)[pos.xPos][pos.yPos];
	
	armysCenter.insert(armysCenter.end(), armyLst.begin(), armyLst.end());

	GetAllSpecifyPos(pos, ONE_CELL_VAL, vectPos, mapDataPtr);

	std::vector<PosXYStru>::iterator itBeg = vectPos.begin();
	std::vector<PosXYStru>::iterator itEnd = vectPos.end();

	for (; itBeg!=itEnd; ++itBeg)
	{
		const list<IArmyPtr> &armyLst = (*mapDataPtr)[itBeg->xPos][itBeg->yPos];	
		
		armysInOneCircle.insert(armysInOneCircle.end(), armyLst.begin(), armyLst.end());
	}

	return true;
}

bool CommonFunctionSingleton::enemyIsInAttackScope(const std::vector<POS_STRUCT> &armyPosVect, const std::vector<POS_STRUCT> &enemyPosVect, int attackScope)
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