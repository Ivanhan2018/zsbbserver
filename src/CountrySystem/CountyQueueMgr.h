// @brief 简要描述: 郡战斗进攻方、防守方限制人数队列管理器
// @author 作者: 黄 文 才

#ifndef _COUNTY_QUEUE_MGR_
#define _COUNTY_QUEUE_MGR_

#include "LimitQueue.h"
class CountyQueueMgr
{
public:
	CountyQueueMgr(CLoadMapInfo * mapInfo, IniFile * iniFile, FightManager *fightMgrPtr, ArmyManager *armyMgrPtr);
	virtual ~CountyQueueMgr(void);

	// 函数描述：是否可以直接进入郡
	// @param armyPtr 打算进入郡的部队指针
	// @return true：可以直接进入郡；false：不能直接进入郡，会自动进入队列
	bool isDirectEnterCounty(const IArmyPtr armyPtr);

	// 函数描述：出队列
	// @return 返回出队的部队数
	unsigned int quitQueue();

	// 函数描述：收集在队列中所有的非在线的玩家部队
	// @param armylstParam 返回的所有非在线的玩家部队列表
	// @return 成功与否
	bool collectAllOutOfLineArmy(ArmyPtrLst &armylstParam);

	// 函数描述：强制出队
	// @param playerID 玩家ID
	// @return 返回出队的部队数
	unsigned int compelQuitQueue(unsigned int playerID);

	// 函数描述：顺利时，需要交换攻守时进行交换
	// @param winCountryID 胜利方的国家ID
	// @return 无
	void attackOrguardSwopOnWin(unsigned int winCountryID);

	// 函数描述：查找指定玩家的所有部队
	// @param playerID 指定的玩家ID
	// @param armyLstParam 返回指定玩家的部队指针列表
	// @param isQuitQueue 是否删除出队
	// @return 无
	void searchAllArmyOfSpeciPlayer(unsigned int playerID, ArmyPtrLst &armyLstParam, unsigned int countryId, bool isQuitQueue = false);

	// 函数描述：查找指定的armyId的ArmyPtr
	// @param armyId 指定的部队ID
	// @return 部队指针,没找到，返回NULL
	inline IArmyPtr getArmyPtrOnArmyId(ArmyID armyId)
	{
		IArmyPtr armyPtrTmp = guardSideQueuePtr_->getArmyPtrOnArmyId(armyId);

		if (NULL == armyPtrTmp)
		{
			armyPtrTmp = attackSideQueuePtr_->getArmyPtrOnArmyId(armyId);
		}

		return armyPtrTmp;
	}


	void enterQueue(const IArmyPtr armyPtr);

private: 
	// 函数描述：初始化
	// @return 无
	void init();

	CLoadMapInfo * mapInfo_;
	FightManager *fightMgrPtr_;        // 战斗管理器指针
	
	LimitQueue *attackSideQueuePtr_;	  // 进攻方队列
	LimitQueue *guardSideQueuePtr_;		  // 防守方队列
	unsigned int guardCountryId_;     // 守方的国家ID
};

#endif
