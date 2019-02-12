// @brief 简要描述: 限制人数队列实现
// @author 作者: 黄 文 才

#ifndef _LIMIT_MAN_NUM_QUEUE
#define _LIMIT_MAN_NUM_QUEUE

#include <list>
#include <set>
#include <map>
#include <vector>
#include <assert.h>
#include "boost\shared_ptr.hpp"
#include "IArmy.h"
#include "LoadMapInfo.h"
#include "ArmyManager.h"
#include "FightManager.h"

typedef std::list<IArmyPtr> ArmyPtrLst;
typedef unsigned int ArmyID;
typedef std::map<ArmyID, IArmyPtr> ArmyId2armyPtr;
typedef std::set<unsigned int> PlayerIdSet;
typedef std::map<unsigned int /*玩家ID*/, std::set<unsigned int /*部队ID*/> > PlayerID2ArmyIDs;
typedef std::vector<IArmyPtr> ArmyPtrVect;


class FightManager;
class IniFile;
class CLoadMapInfo;
class LimitQueue
{
public:
	LimitQueue(CLoadMapInfo * mapInfo, IniFile * iniFile, FightManager *fightMgrPtr, ArmyManager *armyMgrPtr);
	virtual ~LimitQueue(void);

	// 函数描述：是否可以直接进入郡
	// @param armyPtr 打算进入郡的部队指针
	// @return true：可以直接进入郡；false：不能直接进入郡，会自动进入队列
	bool isDirectEnterCounty(const IArmyPtr armyPtr, int curArmyTolInCounty);

	// 函数描述：出队列
	// @return 返回出队的部队数
	unsigned int quitQueue(int curArmyTolInCounty);

	// 函数描述：收集在队列中所有的非在线的玩家部队
	// @param armylstParam 返回的所有非在线的玩家部队列表
	// @return 成功与否
	bool collectAllOutOfLineArmy(ArmyPtrLst &armylstParam);
	
	// 函数描述：强制出队
	// @param playerID 玩家ID
	// @return 返回出队的部队数
	unsigned int compelQuitQueue(unsigned int playerID);

	// 函数描述：查找指定玩家的所有部队
	// @param playerID 指定的玩家ID
	// @param armyLstParam 返回指定玩家的部队指针列表
	// @return 无
	void searchAllArmyOfSpeciPlayer(unsigned int playerID, ArmyPtrLst &armyLstParam, unsigned int countryId, bool isQuitQueue);

	// 函数描述：获得指定的armyId的部队实例
	// @param armyId 指定的部队ID
	// @return 部队指针,没找到，返回NULL
	inline IArmyPtr getArmyPtrOnArmyId(ArmyID armyId)
	{
		ArmyId2armyPtr::const_iterator it = armyId2Ptr_.find(armyId);

		if (armyId2Ptr_.end() != it)
		{
			return it->second;
		}
		else
		{
			return NULL;
		}
	}

	// 函数描述：判断指定玩家是否在该队列中
	// @param playerID 指定的玩家ID
	// @return 存在此玩家，而返回true，否则，返回false
	bool speciPlayerInQueue(unsigned int playerID);
	
	// 函数描述：获取强制出队后，需要收队的人数
	// @return 返回应该收队的人数
	inline int getCanQuitNum(int countryId)
	{
		if (0 == countryId)
		{
//#ifdef _DEBUG	
//			assert(0);
//#endif	
			return 0;
		}

		if (NULL == fightMgrPtr_)
		{
#ifdef _DEBUG	
			assert(0);
#endif	
			return 0;
		}

		mapstate state = mapInfo_->GetMapState();
		int canQuitQueArmyNum = 0;
		int curArmyTolInCounty = 0;
		int guardCountryId_ = mapInfo_->GetMapOwer(); // 防守方

		if (countryId == guardCountryId_)
		{
			curArmyTolInCounty = fightMgrPtr_->getDefendArmyNum();
		}
		else
		{
			curArmyTolInCounty = fightMgrPtr_->getAttackArmyNum();
		}

		if (war != state)
		{
			canQuitQueArmyNum = curArmyTolInCounty - totalArmyCountInPeace_;
		}
		else
		{
			canQuitQueArmyNum = curArmyTolInCounty - totalArmyCountInWar_;
		}

		return canQuitQueArmyNum;
	}

	// 函数描述：入队列
	// @param armyPtr 打算进入郡的部队指针
	// @return 无
	void enterQueue(const IArmyPtr armyPtr);

private:
	// 函数描述：初始化
	// @return 无
	void init();

	CLoadMapInfo * mapInfo_;
	IniFile * iniFile_;
	FightManager *fightMgrPtr_; 
	ArmyManager *armyMgrPtr_;

	unsigned int totalArmyCountInWar_;		  // 战争时期，郡中能够存在的最大部队数
	unsigned int totalArmyCountInPeace_;      // 和平时期，郡中能够存在的最大部队数

	ArmyPtrLst lstArmy_;					  // 队列中的部队列表
	ArmyId2armyPtr armyId2Ptr_;               // 在队列中的armyId--->armyPtr的映射容器
		
	PlayerIdSet playerIdSet_;				  // 在队列中的玩家ID集合

	//PlayerID2ArmyIDs playerID2ArmyIDset_;     // 在郡中的玩家ID--->部队ID集合
};

#endif
