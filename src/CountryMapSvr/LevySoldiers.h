/*=================================================================*/
/*研发团队  : 深圳市新飞扬
/* 文件名   : LevySoldiers.h
/* 文件描述 : 征兵
/* 创建时间 : 2008-4-20
/* 创建者   : 曾庆辉
/* 备注     :
/*
/*-----------------------------------------------------------------*/
/* 修改记录1:
/* 修改时间 :
/* . . .
/*=================================================================*/
#ifndef LEVY_SOLDIERS_HEADER
#define LEVY_SOLDIERS_HEADER

#include "UserInfo.h"
#include "LoadMapInfo.h"
#include "Time_Protocol.h"

#define	MAX_BLOCK_NUM							250					//城市中障碍物数
#define	MAX_CITY_NUM							255					//城市数量

#define MAX_OFFICER_ITEM_NUM					10
#define MAX_OFFICER_NUM							50


struct tagLevyingSoldiers
{
	unsigned long	lMapID;
	unsigned long	lCityID;
	int				iReceID;
	int				iCount;
	int64			lIndex;
	int64			lStartTime;
	long			lTimerID;
};

class CLevySoldiers
{
	//变量定义
private:	
	unsigned long										m_lMapID;
	int													m_iRaceCount;
	CUserInfo											m_UserInfo;
	OfficerJobAttribute									m_JobAttri[8];
	OfficerRaceAttribute								m_RaceAttri[24];
	
	IceUtil::Mutex										m_mapLevyingListMutex;

	multimap<unsigned long, tagLevyingSoldiers>			m_mapLevyingList;
	int													m_ContentOfCarriage[3];
	//函数定义
public:
	CLevySoldiers();
	virtual ~CLevySoldiers();
	//功能函数
public:
	//网络处理函数
	bool MainLevySoldiers(int handle, void * pDataBuffer, int iDataSize) ;
	//城市信息
	void OnCityInfo(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);

	//获取征兵参数
	int OnLevySoldier(int handle, void * pDataBuffer ,int iDataSize, unsigned long lPlayerID);
	//征兵请求,涉及到金钱日志
	bool OnLevySoldierCount(int handle, void * pDataBuffer ,int iDataSize, unsigned long lPlayerID);
	//正处于招兵中...
	bool OnLevyingSoldiers(int handle, unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID);
	//征兵时间事件,涉及到士兵日志
	bool OnLevyTimeEvent(void * pDataBuffer, int iDataSize);
	//进行下个种族
	void OnExeNextSoldier(unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID);
	//是否建造了兵营
	int HaveCasern(int handle, unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID);

	//装载物资，涉及到装备日志
	int OnLoadMaterial(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//卸载物资，涉及到装备日志
	int OnUninstallMaterial(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//物资查看
	int OnLookMaterial(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);

	//殖民城市,涉及到部队状态、城市日志
	int OnColonizeCity(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//用户点"部队组建"按钮时 发送过来的请求
	bool OnArmyCreate(int handle, void * pDataBuffer ,int iDataSize, unsigned long lPlayerID);
	//用户请求一支部队的信息
	bool OnGetArmyInfo(int handle, void * pDataBuffer ,int iDataSize, unsigned long lPlayerID);
	//用户请求一支部队进行组编
	bool OnArmyEdit(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID) ;
	//组编部队	
	bool OnEditArmy(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID) ;
	//空闲指挥官列表
	void OnOfficerList(int handle, unsigned long lPlayerID);
	//武器列表
	void OnWeaponList(int handle, int CityID, unsigned long lPlayerID);
	//防具列表
	void OnDefendList(int handle, int CityID, unsigned long lPlayerID);
	//郡内的所有部队
	void MapOfArmys(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//郡内哨塔等属性
	void MapOfConstruction(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//训练兵种
	bool OnExeSoldierCount(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID) ;
	//城市内查询玩家的所有部队或指挥官
	void OnPlayerAllArmys(int handle, unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID) ;
	//更新正在生产中的道具时间
	void OnUpDataItem(unsigned long lMapID, int iType);
	//玩家退出此郡
	void OnExitMapServer(unsigned long lPlayerID);
	
	void OnLevySpeed(unsigned long lPlayerID, int iSortID, int iValue);

	//立即完成
	void OnImmedComplete(LPImmedComplete_Request msg);
	//处理收费系统返回的通知消息
	void OnImmedCompleteNotifyMsg(LPNotify_ImmedComplete_Response msg);

	//郡地图被占领，清楚玩家数据
	void OnResetMapInfo(LPResetMapInfo_Request msg);

	//创建部队,涉及装备、士兵、英雄状态等日志
	int ReqCreateArmy(CREATE_ARMY* pCreateArmy);

	//解散部队,涉及装备、士兵、英雄状态等日志
	int ReqUnlayArmy(const char *msg);
	//删除部队,涉及英雄状态等日志,有战斗线程记录日志
	int ReqDeleteArmy(const char *msg);
	// 删除队伍信息
	void DeleteArmyInfo(int agent,unsigned long playerID,unsigned long ArmyID);	
	void AddArmyInfo2Fight(int agent,int playerid,int mapid,int armyid);
	//add by xujincai 20090403
	//组建国防军
	int ReqCreateTirrier(const char *msg);
public:
	void SetMapID(unsigned long lMapID);
	void InitLevySoldierActivity(DB_Proxy *pDBProxy);
	void ArmyTimeEvent(int64 lTimeLen);
	//读取训练兵参数表
	void ReadDBExeSoldierParam();
	//读取征兵种族参数表
	void ReadRaceParam();
	//读取城市信息
	void ReadCityInfo();
private:
	int64 GetCurrentTime();
	
};

extern DB_Proxy									g_Proxy;
#endif