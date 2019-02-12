#pragma once
#include "NPCInfo.h"
#include "IArmy.h"
#include <map>

struct FightOverRecord{
	IArmyPtr armyPtr;
	int      cmdType;
	unsigned int targetID;
};
typedef vector<FightOverRecord> FightOverRecordArray;
class FightManager;
struct FightOverCondition;
class FightOverManager
{
public:
	FightOverManager(int limitMaxBoutNum ,FightManager * fightManager);
public:
	~FightOverManager(void);
public:
	//战斗是否结束
	bool isFightOver(int& victoryCountryID);
	//获得胜利玩家
	Armys * getWinner(void);
	//部队死亡
	void onArmyDie(IArmyPtr attacker , IArmyPtr dieArmy);
	//物品掉落
	void onGoodsDropped(IArmyPtr attacker , int droppedItem);
	//刷新资源
	void onUpdateRes(int countryId,int resType , int resNum);
	//设置战斗结束条件
	void setFightOverCondition(FightOverConditionArray & fightOverConditionArray); 
protected:
	FightManager *fightManager_;
	FightOverConditionArray fightOverConditionArray_; //结束条件
	bool isFightOver_; //战斗是否结束
	int	 limitMaxBoutNum_; //最大回合限制
	bool isDefaultMode_; //是否默认模式 
	int winCountryID_; //胜利方的阵营ID
};
