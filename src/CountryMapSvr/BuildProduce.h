/*=================================================================*/
/* 文件名   : BuildProduce.h
/* 文件描述 : 建造建筑物,生产武器,防具
/* 创建时间 : 2008-4-1
/* 创建者   : 曾庆辉
/* 备注     :
/*
/*-----------------------------------------------------------------*/
/* 修改记录1:
/* 修改时间 :
/* . . .
/*=================================================================*/
#ifndef Produce_BUILD_HEADER
#define Produce_BUILD_HEADER

#include "Header.h"
#include "UserInfo.h"
#include "SpeedCard.h"
#include "Time_Protocol.h"
#include "IFightProcesser.h"
#include "LoadMapInfo.h"

class CBuildProduce
{
	//变量定义
private:
	unsigned long							m_lMapID;
	CUserInfo								m_UserInfo;
	unsigned long							m_lFightMapID;
	map<unsigned long, BUILD_CONSTRUCTION_NODE*>			m_mapSpeedEvent;
	//函数定义
public:
	
	CBuildProduce();
	virtual ~CBuildProduce();
	
	//功能函数
public:
	//建造建筑物
	bool OnBuildConstruction(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//已有的建筑物
	bool OnConstruction(int handle, unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID);
	//正在建筑中的建筑物
	void OnBuildingConstruction(int handle, unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID);
	//拆毁玩家在城市中的所有建筑物
	void OnDemolishConstruction(int handle, void * pDataBuffer, int iDataSize);
	//拆毁玩家在城市中的1建筑物，涉及到金钱、建筑
	void OnDemolishAConstruction(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//建筑建筑物,涉及到金钱
	int OnStartBuildCon(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID, int iAuto = 0);
	//取消建筑物建筑,涉及到金钱
	void OnCancelBuild(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//建筑下一个建筑物
	void BuildNextConstruction(unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID);
	//查询玩家是否有建筑物正在建筑
	int SelectPlayerBuild(unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID);
	//建筑建筑物时间事件,涉及到建筑物数量日志
	void ConTimeEvent(void * pDataBuffer, int iDataSize);
	//建造建筑，用于托管类
	void ConstrAutoBulid(void * pDataBuffer, int iDataSize);

	//生产事件
	int OnProduceEvent(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID, int iType, int iMaxLevel, int iTimeEvent);
	//正在生产中的道具
	void OnProduceingItem(int handle, unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID, int iType);
	//生产一个道具类型
	void ProduceNextItemEx(unsigned long playerid,unsigned long mapid,unsigned long cityid);
	//取消生产道具
	void OnCancelProduce(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//查询玩家是否有道具类型正在生产
	int SelectPlayerProduce(unsigned long lMapID, unsigned long lCityID, int iType, unsigned long lPlayerID);
	//生产列队已满
	int ProduceListFlow(unsigned long lMapID, unsigned long lCityID, int iType, unsigned long lPlayerID);
	// 生产时 是否有相对应的建筑物
	int HaveConstruction(int handle, unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID, int iType);
	//生产时 是否有相对应科研技术
	int HaveResearch(int handle, unsigned long lPlayerID, int iType, int iItemID);

	//捐献建筑物
	void Contribute(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//捐献信息
	void OnContributeInfo(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//收集资源
	void OnCollectResource(void * pDataBuffer, int iDataSize);

	//武器生产
	void WeaponProduce(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);

	//防具生产
	void DefendProduce(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);

	//驯兽技术生产
	void AnimalProduce(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	
	//特殊技术生产
	void SpecialProduce(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);

	void OnConstrSpeed(unsigned long lPlayerID,int cityid,int iSortID, int iValue);
	//立即完成LPNotify_ImmedComplete_Response
	void OnImmedComplete(LPImmedComplete_Request msg);
	void OnImmedCompleteNotifyMsg(LPNotify_ImmedComplete_Response msg);//处理收费系统返回的通知消息

	void OnResetMapInfo(LPResetMapInfo_Request msg);//郡地图被占领，清除该郡的所有玩家数据
	void ClearPlayerCity(PlayerCity & playercity);//清除玩家据点,涉及玩家城市日志
	void ClearPlayerProduceActivity(int playerid,int mapid,int cityid);//清除玩家所有正在生产中的任务
	void ClearPlayerProduceAmount(int playerid,int mapid,int cityid);//清除玩家所有正在已生产的武器、防具

	//add by xujincai 20090403
	void OnUpgrateBuild(const char *msg);//升级公共建筑
	//add by xujincai 20090516
	void OnDestroyCity(const char *msg);//拆除据点,涉及城市数量日志

	int GetGarrisonCount();//获取郡守备队数量
	//生产时间事件，涉及部队装备数量日志
	void OnProduceTimeEvent(void * pDataBuffer, int iDataSize);
public:
	void SetMapID(unsigned long lMapID,DB_Proxy *pDBProxy);
	//读取捐献配置表
	void ReadContributeConfig();
	//读取武器生产配置表
	void ReadWeaponConfig(int iType=0);
	
	//读取加速信息
	void ReadProduceState();
	//此郡发生战斗,停止建筑,生产
	void SetFightMapID(unsigned long lMapID);
	//结束战斗
	void OnMapEnFight(const void * pDataBuffer, int iDataSize);

	void InitProActivity(DB_Proxy *pDBProxy);//恢复生产
	void InitConsBuildActivity(DB_Proxy *pDBProxy);//恢复建造
	int UpdatePublicBuildLevel(int buildtype,unsigned long buildid ,int level);
private:
	int64 GetCurrentTime();
	//修正值
	int ModifyValue(unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID, int iConID);
};
extern CSpeedCard								g_SpeedCard;
extern DB_Proxy									g_Proxy;

extern IFightProcesser *						g_IFightProcesser;
#endif