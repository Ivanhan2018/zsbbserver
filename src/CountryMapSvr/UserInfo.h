#ifndef USEING_INFO_HEADER
#define USEING_INFO_HEADER

#pragma once

#include "Header.h"
#include "SpeedCard.h"
#include "ZSBB_DataDefine.h"

#define MAX_TIMELEN_RESOURCE 525600
class CUserInfo
{
		//变量定义
private:
	
	//函数定义
public:
	CUserInfo();
	virtual ~CUserInfo();
	static int										m_iOutPut[4];
	
	//功能函数
public:
	int getConsortiaRate(int playerid,BaseRate &rate);//获取玩家所在公会的产量
	//玩家资源量
	int PlayerResourceCount(int handle, unsigned long MapID, unsigned long CityID, unsigned long lPlayerID,PALEYER_RESOURCE &PlayerRes,unsigned long lFrom=0,unsigned long lTo=0);
	//更新资源
	int OnUpDataResource (int handle, PlayerResource & Res,unsigned long lMapID=0, unsigned long lCityID=0,unsigned long lPlayerID=0, unsigned long lFrom=0,unsigned long lTo=0, int iMode=0,int iItemID=0, int iCount=0,int iMoney=0);
	
	//玩家的兵种数
	void OnCitySoldier(int handle,unsigned long lMapID,unsigned long CityID, unsigned long lFrom, unsigned long lPlayerID);
	//更新玩家的兵种数
	void OnUpDataSoldier (int handle, PLAYER_SOLDIER_LIST & Soldier);

	//武器列表
	void OnWeaponList(int handle,unsigned long lMapID,unsigned long CityID, unsigned long lFrom, unsigned long lPlayerID);
	//防具列表
	void OnDefendList(int handle,unsigned long lMapID,unsigned long CityID, unsigned long lFrom, unsigned long lPlayerID);
	//得到英雄装备
	void OnGetHeroEquip(int handle, unsigned long lFrom, unsigned long lPlayerID);

	//更新道具
	int OnUpDataItem (int ItemID, int iType,int Count, unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID,int &oldcount,int &curcount);
	//在市场交易时更新道具信息
	void OnUpDataItem(int handle,unsigned long lMapID,unsigned long CityID, int iType, int ItemID, int Level, int Count, unsigned long lFrom, unsigned long lPlayerID, int iCost, int iMode = 0);

	//跟据建筑物ID计算此玩家目前对应的资源产量
	int ResourceCount(int ConstructionID, unsigned long MapID, unsigned long CityID, unsigned long lPlayerID);
	void SetOutPut(int iItem, int iValue);
	int64 GetCurrentTime();
};
extern CSpeedCard								g_SpeedCard;
extern DB_Proxy									g_Proxy;
#endif