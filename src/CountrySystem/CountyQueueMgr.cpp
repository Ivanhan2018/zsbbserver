// @brief 简要描述: 郡战斗进攻方、防守方限制人数队列管理器
// @author 作者: 黄 文 才

#include "CountyQueueMgr.h"
#include "IniFile.h"
#include "FightManager.h"
#include "LoadMapInfo.h"
CountyQueueMgr::CountyQueueMgr(CLoadMapInfo * mapInfo, IniFile * iniFile, FightManager *fightMgrPtr, ArmyManager *armyMgrPtr)
: mapInfo_(mapInfo), fightMgrPtr_(fightMgrPtr), guardCountryId_(0)
{
	attackSideQueuePtr_ = new LimitQueue(mapInfo, iniFile, fightMgrPtr, armyMgrPtr);
	guardSideQueuePtr_  = new LimitQueue(mapInfo, iniFile, fightMgrPtr, armyMgrPtr);
	init();
}

CountyQueueMgr::~CountyQueueMgr(void)
{
}

void CountyQueueMgr::init()
{
	if (NULL == mapInfo_ || NULL == fightMgrPtr_)
	{
#ifdef _DEBUG
		assert(0 && "指针为空");
#endif
		return;
	}
	
	guardCountryId_ = mapInfo_->GetMapOwer();
}

bool CountyQueueMgr::isDirectEnterCounty(const IArmyPtr armyPtr)
{
	if (NULL == armyPtr)
	{
#ifdef _DEBUG
		assert(0 && "部队指针为空");
#endif
		return false;
	}
	ActivityPlayerPtr activePlayer = fightMgrPtr_->getPlayerInfo(armyPtr->getPlayerID());
	if(activePlayer.get() != NULL && mapInfo_->GetMapState() != war){
		if (activePlayer->isStayRoom){
			return true;
		}
	}
	if (guardCountryId_ == static_cast<unsigned int>(armyPtr->GetCountryID()))
	{
		int curArmyTolInCounty = fightMgrPtr_->getDefendArmyNum();
		return guardSideQueuePtr_->isDirectEnterCounty(armyPtr, curArmyTolInCounty);
	}
	else
	{
		int curArmyTolInCounty = fightMgrPtr_->getAttackArmyNum();
		return attackSideQueuePtr_->isDirectEnterCounty(armyPtr, curArmyTolInCounty);
	}
}

void CountyQueueMgr::enterQueue(const IArmyPtr armyPtr)
{
	if (NULL == armyPtr)
	{
#ifdef _DEBUG
		assert(0 && "部队指针为空");
#endif
		return;
	}

	if (guardCountryId_ == static_cast<unsigned int>(armyPtr->GetCountryID()))
	{
		return guardSideQueuePtr_->enterQueue(armyPtr);
	}
	else
	{
		return attackSideQueuePtr_->enterQueue(armyPtr);
	}
}

unsigned int CountyQueueMgr::quitQueue()
{
	int curArmyTolInCounty = fightMgrPtr_->getAttackArmyNum();
	int quitQueueArmyNum = attackSideQueuePtr_->quitQueue(curArmyTolInCounty);

	curArmyTolInCounty = fightMgrPtr_->getDefendArmyNum();
	quitQueueArmyNum += guardSideQueuePtr_->quitQueue(curArmyTolInCounty);

	return quitQueueArmyNum;
}

bool CountyQueueMgr::collectAllOutOfLineArmy(ArmyPtrLst &armylstParam)
{
	armylstParam.clear();

	bool isSucess = false;
	
	isSucess = attackSideQueuePtr_->collectAllOutOfLineArmy(armylstParam);

	if (!isSucess)
	{
		return false;
	}

	isSucess = guardSideQueuePtr_->collectAllOutOfLineArmy(armylstParam);

	if (!isSucess)
	{
		return false;
	}

	return true;
}

unsigned int CountyQueueMgr::compelQuitQueue(unsigned int playerID)
{
	bool isExist = attackSideQueuePtr_->speciPlayerInQueue(playerID);
	
	if (isExist)
	{
		return attackSideQueuePtr_->compelQuitQueue(playerID);
	}
	else
	{
		return guardSideQueuePtr_->compelQuitQueue(playerID);
	}
}

void CountyQueueMgr::attackOrguardSwopOnWin(unsigned int winCountryID)
{
	// 胜利时，胜利方的国家ID与原来的不同，则需要攻守方队列数据对调
	if (winCountryID != guardCountryId_)
	{
		guardCountryId_ = winCountryID;

		LimitQueue *limitQueueTmpPtr = attackSideQueuePtr_;
		attackSideQueuePtr_ = guardSideQueuePtr_;
		guardSideQueuePtr_ = limitQueueTmpPtr;
	}
}

void CountyQueueMgr::searchAllArmyOfSpeciPlayer(unsigned int playerID, ArmyPtrLst &armyLstParam, unsigned int countryId, bool isQuitQueue)
{
	if (countryId == guardCountryId_)
	{
		return guardSideQueuePtr_->searchAllArmyOfSpeciPlayer(playerID, armyLstParam, countryId, isQuitQueue);
	}
	else
	{
		return attackSideQueuePtr_->searchAllArmyOfSpeciPlayer(playerID, armyLstParam, countryId, isQuitQueue);
	}
}
