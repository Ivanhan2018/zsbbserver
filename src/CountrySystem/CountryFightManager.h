#pragma once
#include <queue>
#pragma warning(push)
#pragma warning(disable:4512)
#include "IceUtil.h"
#pragma warning(pop)
#include "FightManager.h"
#include "Army.h"
#include "PlayerManager.h"
#include "fightingstatistics/fightingstatisticsadapter.h"
#include "battlefieldstatisticsadapter.h"
#include "globalmacrodef.h"
#include "LimitQueue.h"
#include "TaskManager.h"
class IniFile;
class CLoadDBInfo;
class CLoadMapInfo;
class CNPCInfo;
class FightEvent;
class FightOverManager;
class SeekPath;
class ArmyManager;
class RenascenceMgr;
class BroadcastManager;
class TaskManager;
struct REQ_ENTER_NPC_SYSTEM_STRUCT;
struct REQ_ARMY_INFO_STRUCT;
struct Notify_Enter_Ectype;
struct ReqEctypeGetArmyInfo;
struct ReqMoveEctypeArmyInfo;
struct ReqAttackTargetInfo;
struct ReqSetEctypeArmyTrust;
struct REQ_ENTER_FIGHT_SYSTEM_STRUCT;
struct REQ_ARMY_GO_OUT_STRUCT;
struct tagDeleteArmy_Request;
struct tagFIGHT_CreateArmy_Request;
struct EDIT_ARMY;
struct COMEIN_MAP;
struct tagNotify_DelNPCArmy_Request;
struct TIMER_HEADER;
struct LOG_ONLINE_NUM_TIMER;
struct tagModify_PublicBuild_Request;


//郡战斗管理器
//负责郡中战斗和和平状态下的事物
class CountryFightManager : public FightManager 
{
public:
	CountryFightManager(void);
public:
	~CountryFightManager(void);
public:
	//战斗结束处理
	virtual bool processFightOver(int victoryCountryID) ;
	//调度
	virtual bool scheduler(void);
	//删除特定部队
	virtual void processDelArmy(IArmyPtr armyPtr , ARMY_TRUST_STATE state);
	//设置地图
	bool Initialize(CLoadMapInfo * mapInfo , CLoadDBInfo * dataBaseMemData , IniFile * iniFile);
	//副本广播信息
	void msgBroadcast(DISTRIBUTED_HEADER * msgData , PROTOCOL_FIGHT fightCmd  , int dataLen);
	//释放内存	
	void release();
	//通知大地图玩家部队状态修改
	int notifyModifyArmyState(unsigned int playerid,unsigned int armyid, int mapid,int armyNum,int state,int weapon);
	//捕获英雄
	void onCatchedHero(IArmy* army, HeroId heroId);
	// @brief 当阵营campId的玩家palyerId的英雄heroId被抓获
	void onBeCatchedHero(IArmy* army, HeroId heroId);
	// @brief 当阵营campId的玩家palyerId的英雄heroId受重伤
	void onHeroWasInjured(IArmy* army, HeroId heroId);
	// @brief 当阵营campid的玩家playerid的部队armyid死亡了deathNum个士兵时
	void onSoliderDie(IArmy * attackArmy ,IArmy* army, unsigned int deathNum);
	// @brief 当阵营campid的玩家playerid的部队armyid杀死了短文killNum个士兵时
	void onKilledEnemy(IArmy* army, unsigned int killNum);
	//进入统计系统
	void onEnterWar(IArmyPtr armyPtr);
	// @brief 当部队死亡时
	void onArmyDie(IArmy* attackPtr , IArmy* army);
	// @brief 当部队所属的玩家获得宝箱时
	void onObtainBox(const IArmy* army, int num);
	//释放
	void enterCountry(ArmyPtrVect & armyPtrVect);
	//获得出征部队的位置
	bool getOutArmyPos(int lastMapID , POS_STRUCT & pos);
	//获得传送门的位置
	bool getGatewayPos(int entraceID , POS_STRUCT & pos);
	//设置标签到阵营的映射
	virtual void setLabelMap(int attackCountry);
private:
	//处理客户端move指令
	void processMove(REQ_MOVE_INFO_STRUCT* buf);
	//处理客户端攻击指令
	void processAttack(REQ_ATTACK_TARGET_STRUCT* buf);
	//请求部队信息
	void processReqArmyInfo(REQ_ARMY_INFO_STRUCT* lpReq);
	//进入战斗系统
	void processEnterSystem(const REQ_ENTER_FIGHT_SYSTEM_STRUCT* lpReq);
	//处理托管
	void processTrusteeship(REQ_TRUST_INFO_STRUCT *reqMsg);
	//离开战斗系统
	void processLeaveSystem(REQ_LEAVE_FIGHT_SYSTEM_STRUCT * msgLeaveSystem);
	//退出战斗系统
	void processExitSystem(REQ_EXIT_FIGHT_SYSTEM_STRUCT * msgExitSystem);
	//组建部队
	void processCreateArmy(tagFIGHT_CreateArmy_Request * msgCreateArmy);
	//删除部队
	void processDelArmyMsg(tagDeleteArmy_Request* msgDelArmy);
	//部队离开郡
	void processArmyLeave(EventArmyLeave * eventArmyLeave);
	//整编部队
	void processEditArmy(EDIT_ARMY * msgEditArmy);
	//部队从副本中回归
	void processEctypeBack(Notify_Back_Army_Ectype * msgArmyBack);
	//部队出征
	void processArmyOut(REQ_ARMY_GO_OUT_STRUCT * msgArmyOut);
	//响应部队出征
	void processArmyOutRsp(tagBackArmy_Reach * eventArmyOutRsp);
	//部队出征到达
	void processArmyReach(tagArmy_Reach * msgArmyReach);
	//删除特定玩家的所有部队
	void processDelPlayerArms(tagNotify_DelNPCArmy_Request * msgPlayerInfo);
	//学习技能
	void processHeroNewSkill(HeroStudySkillEvent* msgNewSkill);
	//清除技能
	void processHeroClearSkill(HeroStudySkillEvent* msgNewSkill);
	//刷新英雄参数
	void processUpdateHeroParam(heroModifyParamEvent* msgHeroParam);
	//记录当前在线玩家的数量
	void processLogPlayerOnline(LOG_ONLINE_NUM_TIMER * msgTimer);
	//升级建筑物
	void processUpdateBuildLevel(tagModify_PublicBuild_Request * msgBuildData);
	//处理玩家撤退
	void processArmyRetreat(ReqEctypeRetreatArmyInfo* msgArmyRetreat);
	//处理进入郡消息
	void processEnterMap(COMEIN_MAP * msgEnterMap);
	//处理玩家缴获
	void processCapture(int winCountryID);
	//处理玩家缴获武器防具
	void captureWeapons(std::map<unsigned int , std::map<unsigned int, int>>& playerWeapons,bool isRefresh);
	//处理玩家缴获资源
	void captureResource(int attackCountryID);
	//协议解析
	void parse(const char * buf);
	//设置郡中状态
	void setMapState(mapstate state);
	//开启战斗
	void startFight(bool isNeedSendReport , int attackCountry);
	//驱动回合数
	void driveBoutNum(bool isIncreaceBout);
	//开启定时器
	void startTimerCycle(unsigned int nt = FIGHT_TIMER_CYCLE);
	//广播回合信息
	void broadcastTurnInfo(void);
	//重新设置回合信息
	void resetTurnInfo(void);
	//踢部队回新手村
	bool kickArmyToNewbie(IArmyPtr armyPtr);
	//发送丢弃物品的邮件
	void sendDiscardGoodsMail(unsigned int playerID , int countryId , string& playerName);
	//处理反骚扰部队
	void processGuardArmy(void);
	//删除反骚扰部队
	void processDelGuardArmy(void);

private:
	//每周期处理库活动部队的数量 
	int processActiveNum_;
	//玩家阵营ID
	int countryID_;
	//组包器
	BroadcastManager * broadcastPackManager_;
	//战报
	statisticssystem::BattlefieldStatisticsAdapter * fightReport_;
	//日常任务
	TaskManager* taskManager_;
private:
	NO_COPYABLE_AND_ASSIGN(CountryFightManager);
};
