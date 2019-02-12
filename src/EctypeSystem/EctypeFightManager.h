#pragma once
#include <queue>
#pragma warning(push)
#pragma warning(disable:4512)
#include "IceUtil.h"
#pragma warning(pop)
#include "FightManager.h"
#include "EctypeManager.h"
#include "fightingstatistics/fightingstatisticsadapter.h"
#include "globalmacrodef.h"
class CLoadDBInfo;
class CLoadMapInfo;
class CNPCInfo;
class FightEvent;
class FightOverManager;
class SeekPath;
class RenascenceMgr;
struct REQ_ENTER_NPC_SYSTEM_STRUCT;
struct REQ_ARMY_INFO_STRUCT;
struct Notify_Enter_Ectype;
struct ReqEctypeGetArmyInfo;
struct ReqMoveEctypeArmyInfo;
struct ReqAttackTargetInfo;
struct ReqSetEctypeArmyTrust;

class EctypeFightManager : public FightManager
{
public:
	EctypeFightManager(void);
public:
	~EctypeFightManager(void);
public:
	//获得副本的roomID
	inline int getRoomID() const {
		return roomID_;
	}
	//设置房间的RoomID
	inline void setRoomID(int roomID){
		roomID_ = roomID;
	}
	
	
	//获得副本等级
	virtual int getEctypeDownLevel(void) const {
		return ectypeInfo_->dLevel;
	}
	//获得副本等级
	virtual int getEctypeUpLevel(void) const {
		return ectypeInfo_->uLevel;
	}
	//设置地图
	bool initialize(CLoadMapInfo * mapInfo,CLoadDBInfo *dataBaseMemData, CNPCInfo * ectypeNpcInfo ,
	Notify_Enter_Ectype* lpEvent,int roomID);
	//副本广播信息
	void msgBroadcast(DISTRIBUTED_HEADER * msgData , PROTOCOL_FIGHT fightCmd  , int dataLen);
	//移动部队在沙盘上的位置
	bool moveArmy(IArmyPtr armyPtr , POS_STRUCT & targetPos);
	//处理玩家部队死亡
	bool processPlayerArmyDie(IArmy* armyPtr , int playerState , int realState);
	//通知大地图玩家部队状态修改
	int notifyModifyArmyState(unsigned int playerid,unsigned int armyid, int mapid,int armyNum,int state,int weapon);
	//获得英雄技能触发的概率
	int getSkillProbability(int skillType , int skillLevel);
	//导出增加部队信息
	void exportAddArmyInfo(IArmyPtr armyPtr ,ARMY_ADD& addArmyInfo);
	//捕获英雄
	void onCatchedHero(IArmy* army, HeroId heroId);
	// @brief 当阵营campId的玩家palyerId的英雄heroId被抓获
	// @param army 部队
	// @param heroId 英雄ID
	void onBeCatchedHero(IArmy* army, HeroId heroId);

	// @brief 当阵营campId的玩家palyerId的英雄heroId受重伤
	// @param army 部队
	// @param heroId 英雄ID
	void onHeroWasInjured(IArmy* army, HeroId heroId);

	// @brief 当阵营campid的玩家playerid的部队armyid死亡了deathNum个士兵时
	// @param army 部队
	// @param deathNum 该部队死亡数量
	void onSoliderDie(IArmy * attackArmy , IArmy* army, unsigned int deathNum);

	// @brief 当阵营campid的玩家playerid的部队armyid杀死了短文killNum个士兵时
	// @param army 部队
	// @param killNum 该部队杀敌数量
	void onKilledEnemy(IArmy* army, unsigned int killNum);
	//刷新资源信息
	void updateRes(void);
	//玩家是否仍有部队
	bool isHaveArmy(unsigned int playerID) const;
	//获得玩家部队的数量
	int getArmyNum(unsigned int playerID) const;

	// @brief 当部队死亡时
	// @param army 部队
	void onArmyDie(IArmy* attackPtr , IArmy* army);
	//开启定时器
	virtual void startTimerCycle(unsigned int nt = FIGHT_TIMER_CYCLE);
	//驱动战斗回合数
	virtual void driveBoutNum(bool isIncreaceBout);
	//进入统计系统
	virtual void onEnterWar(IArmyPtr armyPtr);
	//释放内存
	virtual void release(void);
	//战斗结束处理
	virtual bool processFightOver(int victoryCountryID) ;
	//调度
	virtual bool scheduler();
	//判断战斗是否结束
	virtual bool isFightOver(int& victoryCountryID);
	//资源收集
	virtual void collectRes(IArmyPtr armyPtr);

private:
	//处理客户端move指令
	void processMove(ReqMoveEctypeArmyInfo* buf);
	//处理客户端的攻击指令
	void processAttack(ReqAttackTargetInfo* buf);
	//处理请求部队信息
	void processReqArmyInfo(ReqEctypeGetArmyInfo* lpReq);
	//处理删除部队
	void processDelArmyMsg(ReqEctypeDeleteArmy* msgDelArmy);
	//处理进入系统
	void processEnterSystem(const REQ_ENTER_NPC_SYSTEM_STRUCT* lpReq);
	//处理托管
	void processTrusteeship(ReqSetEctypeArmyTrust *reqMsg);
	//处理玩家撤退
	void processArmyRetreat(ReqEctypeRetreatArmyInfo* msgArmyRetreat);
	//处理玩家离开副本
	void processLeaveEctype(ReqLeaveEctypeSystem * msgLeaveEctype);
	//处理副本添加部队
	void processAddArmy(ReqAddArmyToRoom * msgAddArmys);
	//副本建筑物捐献信息
	//void OnContributeInfo(CITY_INFO * cityInfo)
	//通知郡服务器，部队进入副本成功
	void notifyZillah(Notify_Enter_Ectype* msgEnterEctype);
	//分析
	void parse(const char * buf);
	//将部队踢回郡中
	bool kickArmyOutEctype(vector<unsigned int>& players);
	//将玩家踢出副本
	bool kickPlayerOutEctype(vector<unsigned int>& players);
	//判断副本是否符合条件
	bool isValidEctypeData(Notify_Enter_Ectype* msgReqCreate , EctypeInfoTable * ectypeInfo);
	//创建标签到阵营的映射
	void createLabelMap(map<int , int>& countryMap , map<int , list<WarEntranceInfo *>>* warEntrance);
	
private:
	//当前副本编号
	int ectypeID_;
	//入口ID
	int entranceID_;
	//当前房间ID
	int roomID_;
	EctypeInfoTable * ectypeInfo_; //副本描述表
	//
	map<int , map<int , int>>  resCollect_; //资源
	int resCount_; //资源记录的数量
private:
	NO_COPYABLE_AND_ASSIGN(EctypeFightManager);
};

