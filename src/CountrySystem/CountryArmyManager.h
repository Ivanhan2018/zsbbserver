#pragma once
#include <map>
#include "IArmy.h"
#include "ArmyManager.h"
using namespace std;
class FightManager;
class CLoadMapInfo;
class CNPCInfo;
class DB_Proxy;
class CLoadDBInfo;
class CountyQueueMgr;
class ClearSecuSection;
class IniFile;
class BuildArmy;



//郡中部队管理器
//主要负责郡中所有部队和建筑物的信息
class CountryArmyManager :public ArmyManager
{
public:
	CountryArmyManager(void);
public:
	~CountryArmyManager(void);
public:
	//添加部队到部队管理器中
	bool addArmy(IArmyPtr armyPtr);
	//清除部队管理器中的某部队
	void eraseArmy(IArmyPtr armyPtr);
	bool Initialize(CLoadMapInfo *mapInfo ,CNPCInfo * npcInfo ,IniFile * iniFile ,
		DB_Proxy * proxy ,CLoadDBInfo *dataBaseMemData, FightManager * fightManager);
	
	bool moveArmy(IArmyPtr armyPtr , POS_STRUCT & targetPos);
	//待检测添加部队
	bool checkAddArmy(IArmyPtr armyPtr);
	//导出ARMY_ADD信息
	bool exportAddArmyInfo(IArmyPtr armyPtr ,ARMY_ADD& addArmyInfo);
	//将list转化为vec
	void getVecPosFromList(vector<POS_STRUCT> & vecPos , list<POS_STRUCT> & listPos);
	//部队死亡
	void onArmyDie(IArmy* attackPtr ,IArmy* army);
	//释放特定玩家的部队
	int freeArmyFromQueue(unsigned int playerID);
	//释放
	void release();
	// 收队
	void receiveExcessManInCountry(int excessManNum);
	//将国防军踢回新手村
	void kickExpeditionArmy(int countryID);
	//胜利后处理
	void attackOrguardSwopOnWin(int winCountryID);
	//保存数据到DBG
	void saveArmyDataToDBG(void);
	//清扫安全区
	void clearSecSection(void);
	//获得玩家的部队列表
	void getPlayerArmys(unsigned int playerID , vector<IArmyPtr> & vecArmys , int countryID, bool isQuitQueue);

	//战斗结束后伤害恢复
	void cleanupHurt(void);
	//通过部队ID获得army指针
	virtual IArmyPtr getArmyByID(unsigned int armyID) const;
	//装载巡逻部队
	bool attachPatrolArmy();
	//强制出队
	virtual int quitQueue(void) ;
	//创建反骚扰部队
	void createGuardArmy(int mapId , int countryId);
private:
	//装载部队信息
	bool loadArmyInfo(void);
	//装载建筑物信息
	bool loadBuildInfo(void);
	//是否有GM的进攻部队
	bool isHaveGMAttack() const;
private:
	//郡部队队列管理器
	CountyQueueMgr * armyQueueManager_;
	//郡安全区管理器
	ClearSecuSection * secManager_;
};
