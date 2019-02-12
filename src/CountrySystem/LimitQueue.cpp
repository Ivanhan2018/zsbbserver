// @brief 简要描述: 限制人数队列实现
// @author 作者: 黄 文 才

#include "LimitQueue.h"
#include "IniFile.h"
#include "FightManager.h"

LimitQueue::LimitQueue(CLoadMapInfo * mapInfo, IniFile * iniFile, FightManager *fightMgrPtr, ArmyManager *armyMgrPtr)
: mapInfo_(mapInfo), iniFile_(iniFile), fightMgrPtr_(fightMgrPtr),armyMgrPtr_(armyMgrPtr), totalArmyCountInWar_(0), totalArmyCountInPeace_(0)
{
	init();
}

LimitQueue::~LimitQueue(void)
{
	ArmyPtrLst::iterator itBeg = lstArmy_.begin();
	ArmyPtrLst::iterator itEnd = lstArmy_.end();

	for (; itBeg!=itEnd; itBeg++)
	{
		try
		{
			if (NULL != *itBeg)
			{	
				delete *itBeg;
				*itBeg = NULL;
			}
		}
		catch (...)
		{
			;
		}
	}

}

void LimitQueue::init()
{
	if (NULL == iniFile_)
	{
#ifdef _DEBUG
		assert(0 && "指针为空");
#endif
		return;
	}

	totalArmyCountInWar_ = iniFile_->m_MaxArmyNum;
	totalArmyCountInPeace_ = iniFile_->m_MaxPeasArmyNum;
}

bool LimitQueue::isDirectEnterCounty(const IArmyPtr armyPtr, int curArmyTolInCounty)
{
	mapstate state = mapInfo_->GetMapState();

	if (war != state)
	{
		if (curArmyTolInCounty < static_cast<int>(totalArmyCountInPeace_))
		{
			//unsigned int playerID = armyPtr->getPlayerID();

			//playerID2ArmyIDset_[playerID].insert(armyPtr->GetArmyID());

			return true;
		}
		else
		{
			enterQueue(armyPtr);
			return false;
		}
	}
	else
	{
		if (curArmyTolInCounty < static_cast<int>(totalArmyCountInWar_))
		{
			return true;
		}
		else
		{
			enterQueue(armyPtr);
			return false;
		}
	}
}

unsigned int LimitQueue::quitQueue(int curArmyTolInCounty)
{
	int canQuitQueArmyNum = 0;
	unsigned int count = 0;
	mapstate state = mapInfo_->GetMapState();

	if (war != state)
	{
		canQuitQueArmyNum = totalArmyCountInPeace_ - curArmyTolInCounty;
	}
	else
	{
		canQuitQueArmyNum = totalArmyCountInWar_ - curArmyTolInCounty;
	}

	if (canQuitQueArmyNum <= 0)
	{
		return 0;
	}

	// 有部队在郡中的玩家的部队先出队列
	ArmyPtrLst::iterator itBeg = lstArmy_.begin();
	ArmyPtrLst::iterator itEnd = lstArmy_.end();
	unsigned int playerId = 0;
	PlayerID2ArmyIDs::iterator it;
	ArmyId2armyPtr::iterator itTmp;
	ActivityPlayerPtr activityPlayerPtr;
	ArmyPtrVect armyPtrVector;

	for (; itBeg!=itEnd && canQuitQueArmyNum > 0; )
	{
		playerId = (*itBeg)->getPlayerID();
		
		activityPlayerPtr = fightMgrPtr_->getPlayerInfo(playerId);
		
		if (activityPlayerPtr)
		{	
			count++;
			canQuitQueArmyNum--;

			//playerID2ArmyIDset_[playerId].insert((*itBeg)->GetArmyID());

			itTmp = armyId2Ptr_.find((*itBeg)->GetArmyID());

			if (itTmp != armyId2Ptr_.end())
			{
				armyId2Ptr_.erase(itTmp);
			}
	//		else
	//		{
	//			assert(0);
	//		}	

			armyPtrVector.push_back(*itBeg);

			itBeg = lstArmy_.erase(itBeg);
		}
		else
		{
			itBeg++;
		}
	}

	if (canQuitQueArmyNum <= 0)
	{
		return count;
	}

	itBeg = lstArmy_.begin();
	itEnd = lstArmy_.end();

	for ( ; itBeg!=itEnd && canQuitQueArmyNum > 0; )
	{
		count++;
		canQuitQueArmyNum--;

		//playerID2ArmyIDset_[(*itBeg)->getPlayerID()].insert((*itBeg)->GetArmyID());

		itTmp = armyId2Ptr_.find((*itBeg)->GetArmyID());

		if (itTmp != armyId2Ptr_.end())
		{
			armyId2Ptr_.erase(itTmp);
		}
		//else
		//{
		//	assert(0);
		//}	

		armyPtrVector.push_back(*itBeg);

		itBeg = lstArmy_.erase(itBeg);
	}

	fightMgrPtr_->enterCountry(armyPtrVector);

	return count;
}

bool LimitQueue::collectAllOutOfLineArmy(ArmyPtrLst &armylstParam)
{
	if (NULL == fightMgrPtr_)
	{
		return false;
	}

	ArmyPtrLst::iterator itBeg = lstArmy_.begin();
	ArmyPtrLst::iterator itEnd = lstArmy_.end();
	ActivityPlayerPtr activityPlayerPtr;

	for (; itBeg != itEnd; itBeg++)
	{
		activityPlayerPtr = fightMgrPtr_->getPlayerInfo((*itBeg)->getPlayerID());

		if (activityPlayerPtr && !(activityPlayerPtr->isStayRoom))
		{
			armylstParam.push_back(*itBeg);
		}
	}

	return true;
}

unsigned int LimitQueue::compelQuitQueue(unsigned int playerID)
{
	if (NULL == fightMgrPtr_)
	{
		return false;
	}

	PlayerIdSet::iterator it = playerIdSet_.find(playerID);
	
	if (it == playerIdSet_.end())
	{
		return 0;
	}
	else
	{
		playerIdSet_.erase(it);
	}

	ArmyPtrLst::iterator itBeg = lstArmy_.begin();
	ArmyPtrLst::iterator itEnd = lstArmy_.end();

	unsigned int count = 0;

	ArmyPtrVect armyPtrVector;

	ArmyId2armyPtr::iterator itTmp;

	PlayerID2ArmyIDs::iterator itPlayer2ArmyIDs;

	int countryId = 0;  // 国家ID

	for (; itBeg != itEnd; )
	{
		if ((*itBeg)->getPlayerID() == playerID)
		{
			armyPtrVector.push_back(*itBeg);

			if (0 == countryId)
			{
				countryId = (*itBeg)->GetCountryID();
			}

			count++;

			//playerID2ArmyIDset_[playerID].insert((*itBeg)->GetArmyID());

			itTmp = armyId2Ptr_.find((*itBeg)->GetArmyID());

			if (itTmp != armyId2Ptr_.end())
			{
				armyId2Ptr_.erase(itTmp);
			}
			//else
			//{
			//	//assert(0);
			//	return count;
			//}	
			
			itBeg = lstArmy_.erase(itBeg);
		}
		else
		{
			itBeg++;
		}
	}

	fightMgrPtr_->enterCountry(armyPtrVector);

	// 收队
	if (NULL == armyMgrPtr_)
	{
#ifdef _DEBUG
		assert(0);
#endif
		return count;
	}

	int excessNum = getCanQuitNum(countryId);

	if (excessNum > 0)
	{
		armyMgrPtr_->receiveExcessManInCountry(excessNum);
	}

	return count;
}

void LimitQueue::searchAllArmyOfSpeciPlayer(unsigned int playerID, ArmyPtrLst &armyLstParam, unsigned int countryId, bool isQuitQueue)
{
	ArmyPtrLst::iterator itBeg = lstArmy_.begin();
	ArmyPtrLst::iterator itEnd = lstArmy_.end();

	ArmyPtrVect armyPtrVector;

	ArmyId2armyPtr::iterator itTmp;

	for (; itBeg != itEnd;)
	{
		if (countryId != static_cast<unsigned int>((*itBeg)->GetCountryID()))
		{
			++itBeg;
			continue;
		}

		if ((*itBeg)->getPlayerID() == playerID)
		{
			armyLstParam.push_back(*itBeg);
		}

		if (isQuitQueue)
		{
			ArmyId2armyPtr::iterator itTmp = armyId2Ptr_.find((*itBeg)->GetArmyID());

			if (itTmp != armyId2Ptr_.end())
			{
				armyId2Ptr_.erase(itTmp);
			}

			itBeg = lstArmy_.erase(itBeg);
		}
		else
		{
			++itBeg;
		}
	}

	return;
}

void LimitQueue::enterQueue(const IArmyPtr armyPtr)
{
	lstArmy_.push_back(armyPtr);
	armyId2Ptr_[armyPtr->GetArmyID()] = armyPtr;
	playerIdSet_.insert(armyPtr->getPlayerID());
}

bool LimitQueue::speciPlayerInQueue(unsigned int playerID)
{
	PlayerIdSet::iterator it = playerIdSet_.find(playerID);

	if (it == playerIdSet_.end())
	{
		return false;
	}

	return true;
}