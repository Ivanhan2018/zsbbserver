/*=================================================================*/
/*研发团队  : 深圳市新飞扬
/* 文件名   : ServerManager.h
/* 文件描述 : 大地图服务器
/* 创建时间 : 2008-3-20
/* 创建者   : 曾庆辉
/* 备注     :
/*
/*-----------------------------------------------------------------*/
/* 修改记录1:
/* 修改时间 :
/* . . .
/*=================================================================*/
#ifndef __SERVER_MANAGER__
#define __SERVER_MANAGER__
#include "DBTableHeader.h"
#include "DB_Proxy.h"
#include "MapHeader.h"
#include "Time_Protocol.h"
#include "TreasurePack.h"
#include "ZSBB_DataDefine.h"
#include "ISystemLog.h"

struct tagRaceAttribute
{
	int iRecruitFree;
	int iTechPoint;
	int iOrigEnd;
	int iOrigStr;
	int iOrigAgi;
	int iOrigInt;
	int iEndurance;
	int iStrength;
	int iAgility;
	int iIntelligence;
};
struct tagHeroRaceParam
{
	int iEndurance;
	int iStrength;
	int iAgility;
	int iIntelligence;
};
struct tagHeroJobParam
{
	int iKnight;
	int iSoldier;
	int iShooter;
	int iMaster;
	int iPastor;
};

/*
typedef struct tagArmyOutTimer
{
	long TimerID;
	char *TimerParam;
}ArmyOutTimer,*LPArmyOutTimer;

#define HERO_RESUME_TIME	10800//3小时
*/

typedef struct tagMapWarmInfo
{
	long TimerID;
	unsigned int UnionCount;	//联盟部队数量
	unsigned int ClanCount;		//部落部队数量
	unsigned int AboriginalCount;//土著部队数量
}MapWarmInfo,*LPMapWarmInfo;
class COtherManager 
{
private:
	long									m_lMapID;				
	MapInfoTable							m_MapState[BIG_MAP_SIZE];
	tagRaceAttribute						m_HeroAttri[8];						//种族天赋
	map<unsigned long ,MapInfoTable>		m_mapMapInfo;
	CUserInfo								m_UserInfo;
	int										m_MapInfo[BIG_MAP_SIZE][BIG_MAP_SIZE];
	//map<unsigned long, int>					m_mapOnLinePlayer;					//在线玩家
	map<unsigned long, __int64>				m_mapRecruitingOfficer;				//正在招募英雄...
	map<int,tagSpecialHero>					m_mapSpecialHero;						//特殊英难表

	map<int,string>							m_mapHeroName;						//英雄名称
	map<int,tagHeroRaceParam>				m_mapRaceParam;
	map<int,tagHeroJobParam>				m_mapJobParam;
	map<int, int>							m_mapRecruitMoney;
	int										m_MapWarm[BIG_MAP_SIZE];

	//add by xujincai 20081218
	map<int,CTreasurePack*>					m_TreasureList;						//宝箱配置表
	//add by xujincai 20090106 为部队转移问题而增加
	map<unsigned long,OUT_ARMY_PATH*>		m_ArmyOutTimerList;					//部队出征定时器
	IceUtil::Mutex							m_TimerMutex;
	
	MapWarmInfo								m_MapWarmInfo[BIG_MAP_SIZE];		//预警信息，
	vector<int>								m_UnionSafeMap;						//联盟安全郡
	vector<int>								m_TribeSafeMap;						//部落安全郡

	InitConstruction_Map					m_InitConstructionMap;				//初始玩家建筑
//函数定义
public:
	COtherManager();
	virtual ~COtherManager();
	//接口函数
private:
	//add by xujincai 20090106
	int AddArmyOutTimer(unsigned long playerid,int armyid,OUT_ARMY_PATH *param);
	long GetArmyOutTimer(unsigned long playerid,int armyid);
	OUT_ARMY_PATH* DelArmyOutTimer(unsigned long playerid,int armyid);

public:
	//郡状态信息
	int MapStateList(int handle, unsigned long lPlayerID);
	//玩家城市信息
	int PlayerCityInfo(int handle, unsigned long lPlayerID);
	//玩家基本信息
	int PlayerBaseInfo(int handle, unsigned long lPlayerID);
	//通知郡、科研、市场等服务器，玩家上线、下线
	void notifyPlayerState2OtherSvr(int playerid,int agent);
public:
	//创建指挥官
	int OnCreateOfficer(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//查询指挥官属性
	int GetOfficerAttribute(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//赎回英雄,涉及到英雄状态的改变、双方的金钱日志(3个)
	int RedeemHero(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//销毁背包中的装备，涉及到装备的改变、金钱日志(2个)
	int DestroyPackEquip(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//得到英雄装备
	int OnGetHeroEquip(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//操作英雄装备
	//检查英雄是否已穿上该类型的装备
	bool haveDressEquipOfPostion(int playerid,int officerid,int postion,int sortid);
	int OnExchangeHeroEquip(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);//已作废
	int OnExchangeHeroEquipEx(const char * pDataBuffer);
	//英雄战争记录
	int OnHeroWarRecord(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//英雄简成长记录
	int OnHeroGrowUpRecord(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//指挥官列表
	void OnOfficerList(int handle, unsigned long lPlayerID);
	//招募指挥官,涉及到金钱、英雄信息、英雄数量日志（3个）
	int OnRecruitOfficer(int handle, unsigned long lPlayerID);
	//招募英雄时间事件
	int OnRecruitHeroEvent(const void * pDataBuffer, int iDataSize);
	//开除英雄,涉及到英雄数量日志（1个）
	int OnDismissHero(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//删除英雄的所有技能
	void clearAllOfficerSkill(int playerid,int officerid);
	//得到指挥官技能
	int onOfficerTechList(const char *msg);
	//指挥官学习技能,涉及到技能点、技能数量的日志（2个）
	int onOfficerStudyTech(const char *msg);
	//修改指挥官名字
	void OnModifyOfficerName(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);

	//部队转移
	int OnArmyTransfer(int handle, void * pDataBuffer, int iDataSize, unsigned long lMapID, unsigned long lPlayerID) ;
	//正处于出征中的部队列表//add by xujincai 20090106
	int OnOutingArmysListEx(int handle, unsigned long lMapID, unsigned long lPlayerID);
	//郡地图中寻路径
	int BigMapFindPath(int start,int End ,list<int>* path);
	int BigMapFindPath(int MapInfo[BIG_MAP_SIZE][BIG_MAP_SIZE],int start,int End ,list<int>* path);
	int ResetMapInfo(int MapInfo[BIG_MAP_SIZE][BIG_MAP_SIZE],int countryid,int startmapid,int destmapid);
	//部队退回时，回到附近的郡（首选本方郡）
	int GetNextMapID(int countryid,int start,int end);
	//计算部队出征时间
	int GetMapTimes(unsigned long lPlayerID, unsigned long lMapID, unsigned long lTargetMapID, unsigned long lArmyID, int iTimeLen);
	//删除出征部队
	void OnDeleteOutArmy(unsigned long lPlayerID, unsigned long lArmyID);
	//确定部队出征OK
	int OnArmyReachOKEx(const void *pDataBuffer,int iDataSize);
	//此郡能否达到
	int OnReachMap(int handle, unsigned long lPlayerID, unsigned long lMapID, unsigned long lTargetMapID, unsigned long lArmyID, list<int>* path, int iCase);
	//结束战斗
	void OnMapEnFight(const void * pDataBuffer, int iDataSize);

	void ArmyWarmTimeEvent(void * pDataBuffer, int iDataSize);
	//未用
	void OnRecruitSpeed(unsigned long lPlayerID, int iValue);

	//add by xujincai 20081217,涉及到大地图服务器
	//添加宝箱,涉及装备日志
	int OnNewTreasurePack(const char *msg);
	//打开宝箱,升级装备日志、金钱日志等
	int OnOpenTreasurePack(const char *msg);

	//add by xujincai 20081223 重写部队出征，处理预警问题
	int OnOutArmyEx(const void * pDataBuffer, int iDataSize, int iCase,bool bInit);
	void ArmyOutTimeEventEx(void * pDataBuffer, int iDataSize);
	void TirrierGoHome(const void *msg);

	//add by xujincai 20081226增加英雄重伤恢复
	//英雄重伤通知
	void OnHeroGBHNotify(void *pDataBuffer);
	//英雄重伤恢复定时器,涉及英雄状态日志
	void HeroResumeTimerEvent(void * pDataBuffer);
	//立即完成
	void OnImmedComplete(LPImmedComplete_Request msg);
	//处理收费系统返回的通知消息
	void OnImmedCompleteNotifyMsg(LPNotify_ImmedComplete_Response msg);

	//add by xujincai 20090224 玩家所有部队信息
	//获取部队列表
	void OnGetArmyList(const char *msg);
	//部队状态改变
	void OnModifyArmyState(const char *msg);

	//add by xujincai 20090403 国家信息新增
	//国防军出征
	void OnTirrierGoOut(const char *msg);	

	bool IsSafeMap(int mapid,int countryid);//判断是否为新手村

	//add by xujincai 20090624
	//查询俘虏英雄
	void OnQueryCaptiveList(const char *msg);

	//创建NPC反击部队
	void NotifyCreateNPCArmy(const char *msg);
	void CreateNPCArmyTimer(const char *msg);

	//清除大地图的NPC部队
	void ClearNPCArmyOfBigMap(DB_Proxy *pDBProxy);

	//计算英雄行动值
	int ProcOfficerActionDot(int millrank,PlayerOfficerTable *pOfficer);
	void OnNotifyMapState(const char *msg);//郡状态改变

	//精练装备,涉及装备日志和精炼日志
	void onInlayEquipment(const char *msg);
	//学习技能通知，用于通知到郡（战斗）
	void studySkillNotify(int mapid,int armyid,int officerid,int skill,int skilllevel,int cmd);
	//收费系统通知学习技能（使用技能卡）,涉及技能点和技能日志
	void onNotifyAddOfficerSkill(const char *msg);
	//判断英雄是否在战斗郡
	bool isInWarMapOfOfficer(PlayerOfficerTable *officer);
	//买卡通知，放入公共背包，涉及装备日志
	void onNotifyBuyCard(const char *msg);
	//军牌兑换，涉及金钱、荣誉点等日志
	void onConvert(const char *msg); 

	//查询玩家出征加速卡
	int getExpeditionSpeedCard(int playerid,PlayerItemStorageTable *item);
	//初始化玩家建筑
	void initPlayerConstruction(int playerid,int mapid,int cityid,int &warehouseLevel);

	//通用打开卡，统一右键点击就能打开的卡
	void onCommonOpenCard(const char *msg);
	//清除英雄技能，涉及技能、技能点日志
	int clearHeroStudySkill(int playerid,int heroid);
	//获取一个技能消耗的所有技能点
	int getSumSkillDotOfSkill(int skillid,int level);

	//修改英雄名称
	void onModifyHeroName(const char *msg);
	//判断是否已存在该名字的英雄
	bool isExistOfHeroName(const char *name);
	//释放俘虏，涉及英雄状态日志
	void releaseCaptive(const char *msg);
public:
	void SetMapID(long lMapID,DB_Proxy *pDBProxy);
	int GetMapID() { return m_lMapID; };
	//种族天赋
	void ReadRaceGeniusInfo();
	//读取城市信息
	void ReadCityInfo();
	//读取郡地图列表信息
	void ReadMapListInfo();
	//读取郡地图数据
	void ReadMapData();

	//读取宝箱配置表
	void loadBinConfig();
	void clearBinConfig();
	CTreasurePack* getBinConfig(int id);
	//读取英雄列表，对于重伤的英雄进行自动恢复
	void ReadPlayerOfficerTable(DB_Proxy *pDBProxy);
	//退出游戏
	void OnExitMapServer(unsigned long lPlayerID);

	void InitArmyOut(int mapid,DB_Proxy *pDBProxy);//恢复部队出征	

	//加载初始玩家建筑表
	void loadInitConstructions();
	void clearInitConstructions();
	
private:
	int64 GetCurrentTime();
	
};

extern DB_Proxy									g_Proxy;
#endif
