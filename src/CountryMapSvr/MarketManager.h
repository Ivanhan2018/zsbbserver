/*=================================================================*/
/*研发团队  : 深圳市新飞扬
/* 文件名   : MarketManager.h
/* 文件描述 : 市场
/* 创建时间 : 2008-4-6
/* 创建者   : 曾庆辉
/* 备注     :
/*
/*-----------------------------------------------------------------*/
/* 修改记录1:
/* 修改时间 :
/* . . .
/*=================================================================*/
#ifndef MARKET_MANAGER_HEADER
#define MARKET_MANAGER_HEADER

#pragma once

#include "UserInfo.h"

struct tagPlayerInfo
{
	int iCountry;
	char szAccounts[SIZE_32];
	unsigned int iStart;
};
class CMarketManager
{
	//变量定义
private:
	unsigned long							m_lMapID;	
	CUserInfo								m_UserInfo;
	map<unsigned long, tagPlayerInfo>		m_mapPlayerInfo;
	int										m_MarkPrice[3][9];//价格参数：1：卖，2：买
	//函数定义
public:
	CMarketManager();
	virtual ~CMarketManager();
	//功能函数
public:
	//网络处理函数
	int MainMarketManager(void * pDataBuffer, int iDataSize);
	//国家市场商品列表
	int CountryMarket(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//国家市场商品出售,涉及金钱、部队装备等日志
	int CountryMarketBusiness(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//国家市场卖英雄装备，涉及金钱、英雄装备等日志
	int SaleEquip(int agent,int playerid,int sortid);
	//民间市场信息
	int PeopleMarket(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID, int iNext=0);
	//民间市场物品列表
	void OnGoodsList(int handle, int iNext, int iType,unsigned long lPlayerID);
	//物品挂牌
	void OnPutUp(int handle, unsigned long lPlayerID, void * pDataBuffer, int iDataSize);
	//出价
	void OnBidden(int handle, unsigned long lPlayerID, void * pDataBuffer, int iDataSize, int iMarketType=0);
	//一口价
	void OnOnePrice(int handle, unsigned long lPlayerID, void * pDataBuffer, int iDataSize, int iMarketType=0);
	//购买
	int OnBuy(unsigned long lPlayerID, int iMoney,int &curMoney);

	//战利品拍卖信息
	int VendueGoods(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID, int iNext=0);
	//玩家资源量
	int PlayerResourceCount(int handle, void * pDataBuffer, unsigned long lPlayerID);
	//武器列表
	void OnWeaponList(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//防具列表
	void OnDefendList(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//英雄装备
	void OfficerItemList(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
			
	//更新英雄装备
	int OnUpDataEquip (unsigned long lPlayerID, __int64 iItemID, unsigned long lSortID=0);
	//更新资源
	int OnUpDataResource (int handle,int ItemID, int Count, unsigned long lPlayerID,unsigned long lMapID, unsigned long CityID);
	//玩家退出此郡
	void OnExitMapServer(unsigned long lPlayerID);
public:
	void SetMapID(unsigned long lMapID) { m_lMapID = lMapID; };
	void InitMarkPrice();
};
extern DB_Proxy									g_Proxy;
#endif