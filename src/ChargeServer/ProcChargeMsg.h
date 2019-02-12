#ifndef PROCMESSAGE_H__
#define PROCMESSAGE_H__
#include <stdarg.h>
#include <stdio.h>
#include "Distributed_Header.h"
#include "DB_ProxyHeader.h"
#include "DBTableHeader.h"
#include "ChargeProtocol.h"
#include "ConsortiaProtocol.h"
#include "ZSBB_DataDefine.h"
class CChargeTask;
class CProcChargeMsg
{
public:
	CProcChargeMsg(CChargeTask *pTask,const char *dbgip,int port);
	~CProcChargeMsg();
	void ProcMsg(const char *msg);
private:
	bool ConnectDBG(const char *ip,int port);
	void QueryMoney(const char *msg);						//��ѯGo��
	void BuyCard(const char *msg);							//����
	void UseResourceCard(const char *msg);					//ʹ�û�����Դ��
	void UseResourceSpeedCard(const char *msg);				//ʹ�û�����Դ�������ٿ�
	void UseResruitHeroCard(const char *msg);				//ʹ��Ӣ����ļ��
	void UseSpecialSkillCard(const char *msg);				//ʹ�����⼼�ܿ�
	void UseOtherSpeedCard(const char *msg);				//ʹ�ý��졢���С��������ٿ�
	void UseAutoCard(const char *msg);						//ʹ���йܿ�
	void CancelAuto(const char *msg);						//ȡ���й�
	void QueryAutoList(const char *msg);					//��ѯ�й���Ϣ��δ��ɵ��й��б�)
	void UseHeroEquipCard(const char *msg);					//ʹ��Ӣ��װ����
	void QueryResourceRate(const char *msg);				//��ѯ���ĳ��������
	void UseExpandPackCard(const char *msg);				//ʹ�ñ�����չ��
	void ProcTimerMsg(const char *msg);						//����ʱ��
	void BuildComplete(const char *msg);					//���������·�֪ͨ
	void CollectResourceMsg(const char *msg);				//�����������ռ���Դ֪ͨ��Ϣ
	void ProcBuildNotify(int plyaerid,int mapid,int cityid);//�й�ҵ����
	void ExchangeMoney(const char *msg);					//�һ�
	void ProImmedCompleteNotify(const char *msg);			//���������ص�֪ͨ��Ϣ
	void IncreaseRecourceRate(const char *msg);				//��ʹ�ü��ٿ�ֱ�Ӽ���
	void ElectionNotify(const char *msg);
	void UseHeroResumeCard(const char *msg);				//ʹ�ûָ���
	void TaskQueryMoney(const char *msg);					//����ϵͳ��ѯGO��
	void AddPlayerGODian(const char *msg);					//��ӡ�����GO��
	void onAddOfficerSkillResponse(const char *msg);		//ʹ�ü��ܿ������ͼ������Ϣ
	void onNotifyUseOneCard(const char *msg);				//ʹ��һ������֪ͨ��������֮��֪ͨ
	void onNotifyBuyCard(const char *msg);					//�򿨣����ͼ���뱳�������Ϣ����
	void onNotifyUpdateConsortia(const char *msg);			//���������۳�GO�㡣
	void WriteLogEx(int playerid,CardParam *pCardParam,LogType logtype,int svrid = MDM_GP_MAP_CHARGE);//д��־
public:
	int GetItemPos(int playerid,int officerid,int itemid,int &sortid);
	int GetItemPos(int playerid,int officerid,int itemid,PlayerItemStorageTable &itemrec);
	static int GetItemInfo(PlayerItemStorageTable &itemrec,DB_Proxy *pDBProxy);	//��ȡ���ڱ����еĴ洢��Ϣ
	static int GetUnusedPos(PlayerItemStorageTable &packitem,DB_Proxy *pDBProxy);		//����һ��δʹ�õı�����Ԫ��
	int DelCard(PlayerItemStorageTable &iteminfo);								//�ӱ�����ɾ��ĳ���ߣ�����
	static bool BuildListIsEmpty(int playerid,int mapid,int cityid,DB_Proxy *pDBProxy);//�ж��û�ĳ���еĽ����б��Ƿ�Ϊ��
	int UpdatePlayerState(int playerid,int type,int value);								//�޸��û���ĳ��״̬
	int DelPlayerState(int playerid,int type);											//ɾ���û���ĳ��״̬��Ϣ
private:
	CChargeTask *m_pTask;
	DB_Proxy m_DBProxy;
};
#endif
