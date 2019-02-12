#ifndef USEING_INFO_HEADER
#define USEING_INFO_HEADER

#pragma once

#include "Header.h"
#include "SpeedCard.h"
#include "ZSBB_DataDefine.h"

#define MAX_TIMELEN_RESOURCE 525600
class CUserInfo
{
		//��������
private:
	
	//��������
public:
	CUserInfo();
	virtual ~CUserInfo();
	static int										m_iOutPut[4];
	
	//���ܺ���
public:
	int getConsortiaRate(int playerid,BaseRate &rate);//��ȡ������ڹ���Ĳ���
	//�����Դ��
	int PlayerResourceCount(int handle, unsigned long MapID, unsigned long CityID, unsigned long lPlayerID,PALEYER_RESOURCE &PlayerRes,unsigned long lFrom=0,unsigned long lTo=0);
	//������Դ
	int OnUpDataResource (int handle, PlayerResource & Res,unsigned long lMapID=0, unsigned long lCityID=0,unsigned long lPlayerID=0, unsigned long lFrom=0,unsigned long lTo=0, int iMode=0,int iItemID=0, int iCount=0,int iMoney=0);
	
	//��ҵı�����
	void OnCitySoldier(int handle,unsigned long lMapID,unsigned long CityID, unsigned long lFrom, unsigned long lPlayerID);
	//������ҵı�����
	void OnUpDataSoldier (int handle, PLAYER_SOLDIER_LIST & Soldier);

	//�����б�
	void OnWeaponList(int handle,unsigned long lMapID,unsigned long CityID, unsigned long lFrom, unsigned long lPlayerID);
	//�����б�
	void OnDefendList(int handle,unsigned long lMapID,unsigned long CityID, unsigned long lFrom, unsigned long lPlayerID);
	//�õ�Ӣ��װ��
	void OnGetHeroEquip(int handle, unsigned long lFrom, unsigned long lPlayerID);

	//���µ���
	int OnUpDataItem (int ItemID, int iType,int Count, unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID,int &oldcount,int &curcount);
	//���г�����ʱ���µ�����Ϣ
	void OnUpDataItem(int handle,unsigned long lMapID,unsigned long CityID, int iType, int ItemID, int Level, int Count, unsigned long lFrom, unsigned long lPlayerID, int iCost, int iMode = 0);

	//���ݽ�����ID��������Ŀǰ��Ӧ����Դ����
	int ResourceCount(int ConstructionID, unsigned long MapID, unsigned long CityID, unsigned long lPlayerID);
	void SetOutPut(int iItem, int iValue);
	int64 GetCurrentTime();
};
extern CSpeedCard								g_SpeedCard;
extern DB_Proxy									g_Proxy;
#endif