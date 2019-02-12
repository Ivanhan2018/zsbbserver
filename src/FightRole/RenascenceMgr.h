// @brief 简要描述: NPC重生管理器
// @author 作者: 黄 文 才

#ifndef _SHIREFIGHT_RENASCENCEMGR_H_H_H_H
#define _SHIREFIGHT_RENASCENCEMGR_H_H_H_H

#include <set>
#include <map>
#include "FightManager.h"

typedef unsigned int DeadCycleNum;

typedef std::map<unsigned int /* NPC Army ID */, IArmyPtr> DeadArmyMap;
class BitMap;
class RenascenceMgr
{
public:
	RenascenceMgr(unsigned int baseStartArmyID , unsigned int baseEndArmyID , 
				   FightManager *fightMgrPtrParam);

	virtual ~RenascenceMgr(void);

	// 函数描述：实现NPC死后重生/定时繁殖
	// @return 返回NPC重生和繁殖的总数
	unsigned int getRebornAndReproNpc(ENUM_USE_SCENE state);

	// 函数描述：把死亡的NPC的ID和死亡时的周期数存入映射容器
	// @return 无
	bool insertDeadArmy(IArmyPtr deadArmyPtr);
	//收集复制部队的基部队
	void insertRepeatArmy(IArmyPtr deadArmyPtr);
	//清除特定部队
	void eraseArmy(unsigned int armyID);
	//获取部队ID
	unsigned int getNewArmyID(void);
	//回收部队ID
	void reclaimArmyID(unsigned int armyID) ;
private:
	//获得重生NPC
	unsigned int getRebornNpc(void);
	//获得复制NPC
	unsigned int getRepeatNpc(ENUM_USE_SCENE state);
private:
	RenascenceMgr & operator = (const RenascenceMgr & renascenceMgrParam);
	RenascenceMgr(const RenascenceMgr & renascenceMgrParam);
	
	FightManager * fightMgrPtr_;				// 战斗系统管理器指针
	BitMap * bitMap_; //部队ID管理

	DeadArmyMap deadArmySet_;			// NPC死亡的部队ID映射容器(armyid ----> CurCycleNum)
	std::map<unsigned int /* NPC Army ID */, IArmyPtr>  repeatArmySet_; // 复制部队管理器
	//
};

#endif

