#pragma once
#include "armymanager.h"
struct Notify_Enter_Ectype;
struct EctypeInfoTable;
class EctypeArmyManager :public ArmyManager
{
public:
	EctypeArmyManager(void);
public:
	~EctypeArmyManager(void);
public:
	//通过部队ID获得army指针
	virtual IArmyPtr getArmyByID(unsigned int armyID) const;
	//待检测添加部队
	virtual bool checkAddArmy(IArmyPtr armyPtr){
		return addArmy(armyPtr);
	}
	//是否有效的副本等级
	virtual bool isValidLevel(int playerLevel);
	//初始化副本玩家信息
	bool initPlayerArmy(Notify_Enter_Ectype* lpEvent ,list<IArmyPtr>& armyList , map<int ,int>&countryMap ,map<unsigned int , unsigned int>& tmpArmyMap);
	//初始化副本NPC信息
	bool initNpcArmy(int ectypeID , EctypeInfoTable * ectypeInfo);
	//装载建筑物
	bool loadBuildInfo(void);
	//创建建筑物实例
	BuildArmy*  createBuildArmy(unsigned int buildID , list<POS_STRUCT>& pos , const POS_STRUCT &centryPos);
	//添加部队到系统
	bool insertArmyToSystem(list<IArmyPtr>& armyList);
};
