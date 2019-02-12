#pragma once
#include "Global_Macro.h"
#include "IArmy.h"
class FightManager;
struct PropCardConfig;



class PropCard{
public:
	PropCard(FightManager * fightManager ,IArmyPtr armyPtr, PropCardConfig * propCardPtr);
public:
	~PropCard(void);
public:
	void useCard(void);
	void clearCard(void);
	bool isForceOff(void);
private:
	int startBout_; //起始回合
	IArmyPtr armyPtr_;  //部队指针
	PropCardConfig * propCardPtr_; //卡配置数据
	unsigned int effectId_;  //效果ID
	FightManager * fightManager_;
};

//道具管理器
class PropManager
{
public:
	PropManager(FightManager* fightManager);
public:
	~PropManager(void);
public:
	//特定部队是否可以使用某卡片
	bool isCanUserCard(IArmyPtr armyPtr , PropCardConfig * propCardPtr);
	//使用卡片
	void useCard(IArmyPtr armyPtr , PropCardConfig * propCardPtr);
	//清除卡片使用记录
	void clearCardRecord(void);
	//获得一张空白卡片
	PropCard * getNewCard(IArmyPtr armyPtr ,  PropCardConfig * propCardPtr);
	//导出玩家的卡片使用情况
	void exportCardRecord(unsigned int playerId , int agent) const;

private:
	FightManager* fightManager_;
	//玩家使用道具卡记录<玩家ID , <卡类型 , 使用次数>>
	map<unsigned int , map<CardType , int>> playerUsePropRecord_; 
	//<玩家ID,<卡片类型,<部队ID,冷却回合数>>>
	map<unsigned int,map<CardType,map<unsigned int , int>>> cardCoolingRecord_; 
};
