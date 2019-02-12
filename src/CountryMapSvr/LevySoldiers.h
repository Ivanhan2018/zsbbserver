/*=================================================================*/
/*�з��Ŷ�  : �������·���
/* �ļ���   : LevySoldiers.h
/* �ļ����� : ����
/* ����ʱ�� : 2008-4-20
/* ������   : �����
/* ��ע     :
/*
/*-----------------------------------------------------------------*/
/* �޸ļ�¼1:
/* �޸�ʱ�� :
/* . . .
/*=================================================================*/
#ifndef LEVY_SOLDIERS_HEADER
#define LEVY_SOLDIERS_HEADER

#include "UserInfo.h"
#include "LoadMapInfo.h"
#include "Time_Protocol.h"

#define	MAX_BLOCK_NUM							250					//�������ϰ�����
#define	MAX_CITY_NUM							255					//��������

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
	//��������
private:	
	unsigned long										m_lMapID;
	int													m_iRaceCount;
	CUserInfo											m_UserInfo;
	OfficerJobAttribute									m_JobAttri[8];
	OfficerRaceAttribute								m_RaceAttri[24];
	
	IceUtil::Mutex										m_mapLevyingListMutex;

	multimap<unsigned long, tagLevyingSoldiers>			m_mapLevyingList;
	int													m_ContentOfCarriage[3];
	//��������
public:
	CLevySoldiers();
	virtual ~CLevySoldiers();
	//���ܺ���
public:
	//���紦����
	bool MainLevySoldiers(int handle, void * pDataBuffer, int iDataSize) ;
	//������Ϣ
	void OnCityInfo(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);

	//��ȡ��������
	int OnLevySoldier(int handle, void * pDataBuffer ,int iDataSize, unsigned long lPlayerID);
	//��������,�漰����Ǯ��־
	bool OnLevySoldierCount(int handle, void * pDataBuffer ,int iDataSize, unsigned long lPlayerID);
	//�������б���...
	bool OnLevyingSoldiers(int handle, unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID);
	//����ʱ���¼�,�漰��ʿ����־
	bool OnLevyTimeEvent(void * pDataBuffer, int iDataSize);
	//�����¸�����
	void OnExeNextSoldier(unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID);
	//�Ƿ����˱�Ӫ
	int HaveCasern(int handle, unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID);

	//װ�����ʣ��漰��װ����־
	int OnLoadMaterial(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//ж�����ʣ��漰��װ����־
	int OnUninstallMaterial(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//���ʲ鿴
	int OnLookMaterial(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);

	//ֳ�����,�漰������״̬��������־
	int OnColonizeCity(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//�û���"�����齨"��ťʱ ���͹���������
	bool OnArmyCreate(int handle, void * pDataBuffer ,int iDataSize, unsigned long lPlayerID);
	//�û�����һ֧���ӵ���Ϣ
	bool OnGetArmyInfo(int handle, void * pDataBuffer ,int iDataSize, unsigned long lPlayerID);
	//�û�����һ֧���ӽ������
	bool OnArmyEdit(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID) ;
	//��ಿ��	
	bool OnEditArmy(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID) ;
	//����ָ�ӹ��б�
	void OnOfficerList(int handle, unsigned long lPlayerID);
	//�����б�
	void OnWeaponList(int handle, int CityID, unsigned long lPlayerID);
	//�����б�
	void OnDefendList(int handle, int CityID, unsigned long lPlayerID);
	//���ڵ����в���
	void MapOfArmys(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//��������������
	void MapOfConstruction(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//ѵ������
	bool OnExeSoldierCount(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID) ;
	//�����ڲ�ѯ��ҵ����в��ӻ�ָ�ӹ�
	void OnPlayerAllArmys(int handle, unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID) ;
	//�������������еĵ���ʱ��
	void OnUpDataItem(unsigned long lMapID, int iType);
	//����˳��˿�
	void OnExitMapServer(unsigned long lPlayerID);
	
	void OnLevySpeed(unsigned long lPlayerID, int iSortID, int iValue);

	//�������
	void OnImmedComplete(LPImmedComplete_Request msg);
	//�����շ�ϵͳ���ص�֪ͨ��Ϣ
	void OnImmedCompleteNotifyMsg(LPNotify_ImmedComplete_Response msg);

	//����ͼ��ռ�죬����������
	void OnResetMapInfo(LPResetMapInfo_Request msg);

	//��������,�漰װ����ʿ����Ӣ��״̬����־
	int ReqCreateArmy(CREATE_ARMY* pCreateArmy);

	//��ɢ����,�漰װ����ʿ����Ӣ��״̬����־
	int ReqUnlayArmy(const char *msg);
	//ɾ������,�漰Ӣ��״̬����־,��ս���̼߳�¼��־
	int ReqDeleteArmy(const char *msg);
	// ɾ��������Ϣ
	void DeleteArmyInfo(int agent,unsigned long playerID,unsigned long ArmyID);	
	void AddArmyInfo2Fight(int agent,int playerid,int mapid,int armyid);
	//add by xujincai 20090403
	//�齨������
	int ReqCreateTirrier(const char *msg);
public:
	void SetMapID(unsigned long lMapID);
	void InitLevySoldierActivity(DB_Proxy *pDBProxy);
	void ArmyTimeEvent(int64 lTimeLen);
	//��ȡѵ����������
	void ReadDBExeSoldierParam();
	//��ȡ�������������
	void ReadRaceParam();
	//��ȡ������Ϣ
	void ReadCityInfo();
private:
	int64 GetCurrentTime();
	
};

extern DB_Proxy									g_Proxy;
#endif