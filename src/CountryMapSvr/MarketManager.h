/*=================================================================*/
/*�з��Ŷ�  : �������·���
/* �ļ���   : MarketManager.h
/* �ļ����� : �г�
/* ����ʱ�� : 2008-4-6
/* ������   : �����
/* ��ע     :
/*
/*-----------------------------------------------------------------*/
/* �޸ļ�¼1:
/* �޸�ʱ�� :
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
	//��������
private:
	unsigned long							m_lMapID;	
	CUserInfo								m_UserInfo;
	map<unsigned long, tagPlayerInfo>		m_mapPlayerInfo;
	int										m_MarkPrice[3][9];//�۸������1������2����
	//��������
public:
	CMarketManager();
	virtual ~CMarketManager();
	//���ܺ���
public:
	//���紦����
	int MainMarketManager(void * pDataBuffer, int iDataSize);
	//�����г���Ʒ�б�
	int CountryMarket(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//�����г���Ʒ����,�漰��Ǯ������װ������־
	int CountryMarketBusiness(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//�����г���Ӣ��װ�����漰��Ǯ��Ӣ��װ������־
	int SaleEquip(int agent,int playerid,int sortid);
	//����г���Ϣ
	int PeopleMarket(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID, int iNext=0);
	//����г���Ʒ�б�
	void OnGoodsList(int handle, int iNext, int iType,unsigned long lPlayerID);
	//��Ʒ����
	void OnPutUp(int handle, unsigned long lPlayerID, void * pDataBuffer, int iDataSize);
	//����
	void OnBidden(int handle, unsigned long lPlayerID, void * pDataBuffer, int iDataSize, int iMarketType=0);
	//һ�ڼ�
	void OnOnePrice(int handle, unsigned long lPlayerID, void * pDataBuffer, int iDataSize, int iMarketType=0);
	//����
	int OnBuy(unsigned long lPlayerID, int iMoney,int &curMoney);

	//ս��Ʒ������Ϣ
	int VendueGoods(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID, int iNext=0);
	//�����Դ��
	int PlayerResourceCount(int handle, void * pDataBuffer, unsigned long lPlayerID);
	//�����б�
	void OnWeaponList(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//�����б�
	void OnDefendList(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//Ӣ��װ��
	void OfficerItemList(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
			
	//����Ӣ��װ��
	int OnUpDataEquip (unsigned long lPlayerID, __int64 iItemID, unsigned long lSortID=0);
	//������Դ
	int OnUpDataResource (int handle,int ItemID, int Count, unsigned long lPlayerID,unsigned long lMapID, unsigned long CityID);
	//����˳��˿�
	void OnExitMapServer(unsigned long lPlayerID);
public:
	void SetMapID(unsigned long lMapID) { m_lMapID = lMapID; };
	void InitMarkPrice();
};
extern DB_Proxy									g_Proxy;
#endif