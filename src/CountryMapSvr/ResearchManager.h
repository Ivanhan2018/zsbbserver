/*=================================================================*/
/*�з��Ŷ�  : �������·���
/* �ļ���   : ResearchManager.h
/* �ļ����� : ����
/* ����ʱ�� : 2008-3-27
/* ������   : �����
/* ��ע     :
/*
/*-----------------------------------------------------------------*/
/* �޸ļ�¼1:
/* �޸�ʱ�� :
/* . . .
/*=================================================================*/
#ifndef RESEARCH_HEADER
#define RESEARCH_HEADER

#include "UserInfo.h"
#include "SpeedCard.h"
//���е���
struct tagResearchNode
{
	long									TimerID;
	int										iTypeID;
	int										iItemID;					//����ID
	int										iLevel;						//���ߵȼ�
	int64									lCompleteTime;

};
#define										MAX_PRODUCE_NUM				30

class CResearchManager
{
	//��������
	typedef map<unsigned long,tagResearchNode> ResearchActivity_Map;
	typedef map<unsigned long,tagResearchNode>::iterator ResearchActivity_Iterator;
private:
	CUserInfo								m_UserInfo;	
	//map<unsigned long, tagResearchNode>	m_mapResearchItem;
	ResearchActivity_Map					m_mapResearchItem;

	//��������
public:
	CResearchManager();
	virtual ~CResearchManager();
	//���ܺ���
	void SetMapID(int mapid);
public:
	//�õ������Ѿ��о��˵ĵ����б�
	bool OnGetAllResearchList(int handle, unsigned long lPlayerID);
	//���е��߼���,�漰����Ǯ���Ƽ���
	int OnResearchItem(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//�õ����ڿ����еĵ��߼���
	bool OnGetResearchingItem(int handle, unsigned long lPlayerID);
	//ȡ�����е��߼���,�漰����Ǯ���Ƽ���
	int OnCancelResearchItem(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID); 
	//����ʱ���¼�
	void OnResearchTimeEvent(void * pDataBuffer, int iDataSize);
	
	//����ʱ���¼�
	//void OnResearchTimeEvent(void * pDataBuffer, int iDataSize,int iType);
	void OnResearchSpeed(unsigned long lPlayerID, int iSortID, int iValue);

	void OnImmedComplete(LPImmedComplete_Request msg);//�������
	void OnImmedCompleteNotifyMsg(LPNotify_ImmedComplete_Response msg);//�����շ�ϵͳ���ص�֪ͨ��Ϣ
public:
	//��ȡ�����������ñ�
	void ReadWeaponConfig(int iType=0);
	void InitResearchActivity(DB_Proxy *pDBProxy);//�ָ�����
	
private:
	int64 GetCurrentTime();
		//��������
	void ItemUpGrade(unsigned long lPlayerID, int iType, int iLevel);
};
extern CSpeedCard								g_SpeedCard;
extern DB_Proxy									g_Proxy;
#endif