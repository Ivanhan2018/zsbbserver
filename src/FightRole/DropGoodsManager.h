#pragma once
#include "IArmy.h"
class FightManager;

//掉落物品管理器
class DropGoodsManager
{
public:
	DropGoodsManager(FightManager * fightManager);
public:
	~DropGoodsManager(void);
public:
	//处理军牌掉落
	bool processArmyCardDrop(IArmyPtr attackArmy , IArmyPtr army ,int armyCardID , int odds);
	//处理玩家军牌掉落
	void processPlayerDrop(IArmyPtr attackArmy , IArmyPtr army);
	//重新设置军牌统计信息
	void resetArmyCardDropRecord(void);
private:
	bool processTimeLimit(int& index); //处理时间限制
	bool processCountryLimit(int countryID); //处理郡限制
	bool processTotalLimit(void); //处理总限制
	bool processCardLevelLimit(int cardLevel); //处理军牌等级限制
	bool processLimit(LIMIT_TYPE limttType , int limitKey); //处理限制
	int  getArmyCardLevel(IArmyPtr army) const;	//获得军牌等级
	bool increaseLimit(LIMIT_TYPE limttType ,int limitKey); //递增限制
private:
	FightManager * fightManager_; //战斗管理器
	time_t lastSaveTime_; //上次存盘时间
private:
	NO_COPYABLE_AND_ASSIGN(DropGoodsManager);
};
