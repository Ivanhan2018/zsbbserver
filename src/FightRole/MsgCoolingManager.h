#pragma once
#include "list"
class FightManager;
struct CoolingInfo;
enum CardType;
class MsgCoolingManager
{
public:
	MsgCoolingManager(FightManager *fightManager);
public:
	~MsgCoolingManager(void);
public:
	//收集卡片的冷却消息
	void insertCoolingData(CardType cardType , unsigned int armyId , unsigned int coolingTime);
	//发送冷却消息到客户端
	void sendCoolingData(unsigned int playerId , int agent);
private:
	std::list<CoolingInfo> msgCoolingData_; //卡片的冷却数据
	FightManager * fightManager_; //战斗框架指针
private:
	MsgCoolingManager(void);
};
