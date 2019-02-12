/*=================================================================*/
/* �ļ���   : BuildProduce.h
/* �ļ����� : ���콨����,��������,����
/* ����ʱ�� : 2008-4-1
/* ������   : �����
/* ��ע     :
/*
/*-----------------------------------------------------------------*/
/* �޸ļ�¼1:
/* �޸�ʱ�� :
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
	//��������
private:
	unsigned long							m_lMapID;
	CUserInfo								m_UserInfo;
	unsigned long							m_lFightMapID;
	map<unsigned long, BUILD_CONSTRUCTION_NODE*>			m_mapSpeedEvent;
	//��������
public:
	
	CBuildProduce();
	virtual ~CBuildProduce();
	
	//���ܺ���
public:
	//���콨����
	bool OnBuildConstruction(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//���еĽ�����
	bool OnConstruction(int handle, unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID);
	//���ڽ����еĽ�����
	void OnBuildingConstruction(int handle, unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID);
	//�������ڳ����е����н�����
	void OnDemolishConstruction(int handle, void * pDataBuffer, int iDataSize);
	//�������ڳ����е�1������漰����Ǯ������
	void OnDemolishAConstruction(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//����������,�漰����Ǯ
	int OnStartBuildCon(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID, int iAuto = 0);
	//ȡ�������ｨ��,�漰����Ǯ
	void OnCancelBuild(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//������һ��������
	void BuildNextConstruction(unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID);
	//��ѯ����Ƿ��н��������ڽ���
	int SelectPlayerBuild(unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID);
	//����������ʱ���¼�,�漰��������������־
	void ConTimeEvent(void * pDataBuffer, int iDataSize);
	//���콨���������й���
	void ConstrAutoBulid(void * pDataBuffer, int iDataSize);

	//�����¼�
	int OnProduceEvent(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID, int iType, int iMaxLevel, int iTimeEvent);
	//���������еĵ���
	void OnProduceingItem(int handle, unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID, int iType);
	//����һ����������
	void ProduceNextItemEx(unsigned long playerid,unsigned long mapid,unsigned long cityid);
	//ȡ����������
	void OnCancelProduce(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//��ѯ����Ƿ��е���������������
	int SelectPlayerProduce(unsigned long lMapID, unsigned long lCityID, int iType, unsigned long lPlayerID);
	//�����ж�����
	int ProduceListFlow(unsigned long lMapID, unsigned long lCityID, int iType, unsigned long lPlayerID);
	// ����ʱ �Ƿ������Ӧ�Ľ�����
	int HaveConstruction(int handle, unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID, int iType);
	//����ʱ �Ƿ������Ӧ���м���
	int HaveResearch(int handle, unsigned long lPlayerID, int iType, int iItemID);

	//���׽�����
	void Contribute(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//������Ϣ
	void OnContributeInfo(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//�ռ���Դ
	void OnCollectResource(void * pDataBuffer, int iDataSize);

	//��������
	void WeaponProduce(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);

	//��������
	void DefendProduce(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);

	//ѱ�޼�������
	void AnimalProduce(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	
	//���⼼������
	void SpecialProduce(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);

	void OnConstrSpeed(unsigned long lPlayerID,int cityid,int iSortID, int iValue);
	//�������LPNotify_ImmedComplete_Response
	void OnImmedComplete(LPImmedComplete_Request msg);
	void OnImmedCompleteNotifyMsg(LPNotify_ImmedComplete_Response msg);//�����շ�ϵͳ���ص�֪ͨ��Ϣ

	void OnResetMapInfo(LPResetMapInfo_Request msg);//����ͼ��ռ�죬����ÿ��������������
	void ClearPlayerCity(PlayerCity & playercity);//�����Ҿݵ�,�漰��ҳ�����־
	void ClearPlayerProduceActivity(int playerid,int mapid,int cityid);//�������������������е�����
	void ClearPlayerProduceAmount(int playerid,int mapid,int cityid);//��������������������������������

	//add by xujincai 20090403
	void OnUpgrateBuild(const char *msg);//������������
	//add by xujincai 20090516
	void OnDestroyCity(const char *msg);//����ݵ�,�漰����������־

	int GetGarrisonCount();//��ȡ���ر�������
	//����ʱ���¼����漰����װ��������־
	void OnProduceTimeEvent(void * pDataBuffer, int iDataSize);
public:
	void SetMapID(unsigned long lMapID,DB_Proxy *pDBProxy);
	//��ȡ�������ñ�
	void ReadContributeConfig();
	//��ȡ�����������ñ�
	void ReadWeaponConfig(int iType=0);
	
	//��ȡ������Ϣ
	void ReadProduceState();
	//�˿�����ս��,ֹͣ����,����
	void SetFightMapID(unsigned long lMapID);
	//����ս��
	void OnMapEnFight(const void * pDataBuffer, int iDataSize);

	void InitProActivity(DB_Proxy *pDBProxy);//�ָ�����
	void InitConsBuildActivity(DB_Proxy *pDBProxy);//�ָ�����
	int UpdatePublicBuildLevel(int buildtype,unsigned long buildid ,int level);
private:
	int64 GetCurrentTime();
	//����ֵ
	int ModifyValue(unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID, int iConID);
};
extern CSpeedCard								g_SpeedCard;
extern DB_Proxy									g_Proxy;

extern IFightProcesser *						g_IFightProcesser;
#endif