#ifndef DATA_SERVER_H
#define DATA_SERVER_H

#pragma once

#define CMDID_LEN			4
#define VERION_LEN			4
#define CHECK_COND_LEN		4

#include "Global_Macro.h"
#include <list>
#include "MapHeader.h"
#include "ServerManager.h"
#include "SpeedCard.h"
#include "FightProcesserFactory.h"
#include "LoadDBInfo.h"
#include "LoadMapInfo.h"
#include <map>
using namespace std;
class CDataServer
{
public:
	CDataServer(void);
	~CDataServer(void);
	bool ReadPacket(char* block , PROTOCOL_FIGHT* cmdID , void * buffer  );
	void GetData(char * block);
	list<int> handle_list ;
	map<unsigned long ,unsigned long > m_Handle_Map;
	unsigned short handle;
	void InitInfo();
	//结束战斗
	void OnEndFightEvent(const void * pDataBuffer, unsigned int length);
	//地图服务器信息
	bool MapServerInfo(int handle,void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	int OnSetTimerEvent(int iCommand,void * pDataBuffer, int iDataSize);
	void OnExitServer(unsigned long lPlayerID);
	void OnOnceSpeed(void * pDataBuffer, int iDataSize);
	//立即完成处理
	void OnImmedComplete(void *msg);
	void OnImmedCompleteNotifyMsg(void *msg);

	int AddArmyInfo(int type ,const char *armyname,int playerid,int officerid,int raceid,int weapon =1,int weaponlevel = 2,int defendtype=0,int defendlevel=1,int soldiertype = 1,int soldiercount=100);
	int getAgentOfPlayer(int playerid);
	void setAgentOfPlayer(int playerid,int agent);
	void onNotifyPlayerOnline(const char *msg);
	void broadMsgAndNotify(int playerid,int mapid,__int64 itemid,short level,short type,int notifycmd,int broadcmd,int svrid);
public:
	CLevySoldiers							m_LevySoldier;
	CBuildProduce							m_BuildProduce;
	CMarketManager							m_MarketManager;
	CResearchManager						m_ResearchManager;
	COtherManager							m_OtherManager;
	CUserInfo								m_UserInfo;
	std::map<int,int>						m_onLinePlayerMap;
	
};
extern CDataServer								g_DataServer ;
extern CSpeedCard								g_SpeedCard;
extern IFightProcesser *						g_IFightProcesser;
#endif