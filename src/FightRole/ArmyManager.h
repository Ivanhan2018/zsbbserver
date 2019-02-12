#pragma once
#include <map>
#include "IArmy.h"
#include "Army.h"
using namespace std;
class FightManager;
class CLoadMapInfo;
class CNPCInfo;
class DB_Proxy;
class CLoadDBInfo;
class IniFile;
class BuildArmy;
class ArmyManager
{
public:
	ArmyManager(void);
public:
	~ArmyManager(void);
public:
	//通过部队ID获得army指针
	virtual IArmyPtr getArmyByID(unsigned int armyID) const = 0;
	//待检测添加部队
	virtual bool checkAddArmy(IArmyPtr armyPtr) = 0;
	//胜利后处理
	virtual void attackOrguardSwopOnWin(int winCountryID);
	//保存数据到DBG
	virtual void saveArmyDataToDBG(void);
	//清扫安全区
	virtual void clearSecSection(void);
	//释放特定玩家的部队
	virtual int freeArmyFromQueue(unsigned int playerID);
	//战斗结束后伤害恢复
	virtual void cleanupHurt(void);
	// 收队
	virtual void receiveExcessManInCountry(int excessManNum);
	//获得玩家的部队列表
	virtual void getPlayerArmys(unsigned int playerID , vector<IArmyPtr> & vecArmys  , int countryID, bool isQuitQueue);
	//添加部队到管理器
	virtual bool addArmy(IArmyPtr armyPtr);
	//将部队从管理器中清除
	virtual void eraseArmy(IArmyPtr armyPtr);
	//将部队加入
	bool insertArmyToList(IArmyPtr armyPtr);
	//释放部队列表
	void freeArmyList(list<IArmyPtr> & armyList);
	//初始化
	bool Initialize(CLoadMapInfo *mapInfo ,CNPCInfo * npcInfo ,IniFile * iniFile ,
		DB_Proxy * proxy ,CLoadDBInfo *dataBaseMemData, FightManager * fightManager);
	//创建玩家部队实例
	IArmyPtr createPlayerArmy(unsigned int armyID);
	IArmyPtr createPlayerArmy(ArmyTable & armyTable);
	//创建建筑物部队实例
	BuildArmy* createBuildArmy(PublicBuildParamTable & buildParam,vector<POS_STRUCT> & buildPos,unsigned int armyID);
	//装载建筑物
	virtual BuildArmy*  createBuildArmy(unsigned int buildID ,  list<POS_STRUCT>& pos , const POS_STRUCT &centryPos);
	//构造部队参数
	bool createPlayerArmyParam(PlayerArmyParam & armyParam , ArmyTable & armyTable);
	//判断是否有效部队
	bool isValidArmy(IArmyPtr armyPtr) const;
	//部队移动
	bool moveArmy(IArmyPtr armyPtr , POS_STRUCT & targetPos);
	
	//导出ARMY_ADD信息
	bool exportAddArmyInfo(IArmyPtr armyPtr ,ARMY_ADD& addArmyInfo);
	//是否国防军
	bool isExpeditionArmy(IArmyPtr armyPtr);
	//是否新手村
	bool isNewbie(void);
	//
	void getVecPosFromList(vector<POS_STRUCT> & vecPos , list<POS_STRUCT> & listPos);
	//部队死亡
	void onArmyDie(IArmy* attackPtr ,IArmy* army);
	//处理死亡的部队
	virtual void processDieArmy(void);
	//释放内存
	void release();
	//是否为有效位置
	bool IsValidPos(int x ,int y) const;
	//获得沙盘列表
	inline vector<vector<list<IArmyPtr> > > * getArmyList(void){
		return &mapArmyList_;
	}
	//获得系统中的部队MAP
	inline const map<unsigned int , IArmyPtr>& getArmyMap(void) const{
		return armyMap_;
	}
	//获得特定坐标
	std::list<IArmyPtr> * const getArmysInPos(int x , int y){
		if (mapInfo_->IsValidPos(x , y) == false){
			return NULL;
		}
		return &mapArmyList_[x][y];
	}
	//获得防守方人数
	inline int getDefendArmyNum(void) {
		return defendArmyNum_;
	}
	//获得进攻方人数
	inline int getAttackArmyNum(void) {
		return attackArmyNum_;
	}
	//强制出队
	virtual int quitQueue(void) {
		return 0;
	}
	//是否有效的副本等级
	virtual bool isValidLevel(int /*playerLevel*/){
		return false;
	}
protected:
	DB_Proxy * dataBaseProxy_;
	//地图玩家列表
	vector<vector<list<IArmyPtr> > > mapArmyList_;
	//部队列表
	map<unsigned int , IArmyPtr> armyMap_; 
	//死亡部队队列
	map<unsigned int , IArmyPtr> armyDieMap_;
	//当前地图
	CLoadMapInfo * mapInfo_;
	//NPC信息表
	CNPCInfo * npcInfo_;
	//DBG 缓存数据
	CLoadDBInfo *dataBaseMemData_;
	//战斗管理器
	FightManager* fightManager_;
	//配置文件
	IniFile * iniFile_;
	//进攻方人数
	int attackArmyNum_;
	//防守方人数
	int defendArmyNum_;
	//随机数
	computinghurt::IRandomGenerator* randomgenerator_;
};
