#ifndef __gmThread_H__
#define __gmThread_H__

#include "gmGlobalHead.h"
#include "gmFixData.h"
#include "DB_Proxy.h"


class CGmThread
{
public:
	CGmThread();
	~CGmThread();

	//ͳһ����ֵ����:
	//����ֵΪ1�ɹ�������ʧ��
	bool InitGmThread(const char * addr,short port,const char * dns,const char * user,const char * password);
	bool FInitGmThread();

	bool initdb(const char * addr,short port);
	void finitdb();
	
	bool ConData(const char * dsn,const char * user,const char * password);
	void DisConnData();

	//��¼GM
	int OnGmLogin(const DISTRIBUTED_HEADER* pNetRead);
	
	//�ǳ�GM
	int OnGmLogout(const DISTRIBUTED_HEADER* pNetRead);
	
	//�Ƿ����¼
	int OnLoginDisabled(const DISTRIBUTED_HEADER* pNetRead);
	
	//�Ƿ����
	int OnSpeakDisabled(const DISTRIBUTED_HEADER* pNetRead);
	
	//������������
	int OnGetNoticeForces(const DISTRIBUTED_HEADER* pNetRead);
	
	//GM����
	int OnGmCommand(const DISTRIBUTED_HEADER* pNetRead);
	
	//������Ӣ�����¼�
	int OnGmAddMainHero(const DISTRIBUTED_HEADER* pNetRead);
	
	//�޸���������(����)
	void ProcModiNoticeForcesEvent(const DISTRIBUTED_HEADER* pNetRead);

	int ProcNotifyCreateNPCArmy(const DISTRIBUTED_HEADER * pNetRead);

	int ProcNotifyDelNPCArmy(const DISTRIBUTED_HEADER * pNetRead);
private:
	//������OnGmCommand�ֽ⴦����
	//�޸���������
	int ProcModiNoticeForces(short gmid,const tagGMCommand* gmc,int fid,const char * fdata);
	
	//GM������������
	int ProcShowNoticeForces(short gmid,const tagGMCommand* gmc,int fid);

	//GM��ͨ���棨�鲥)
	int ProcNoticePost(short gmid,const tagGMCommand* gmc,int fid,const char * fdata);

	//GM��ͨ���棨�㲥��
	int ProcNoticeBroadcast(short gmid,const tagGMCommand* gmc,const char * fdata);

	//����
	int ProcSpeakingBanned(short gmid,const tagGMCommand* gmc, int playerid,int ienabled);
	
	//����¼
	int ProcLoginBanned(short gmid,const tagGMCommand* gmc,int playerid,int ienabled);

	//��Ǯ �Ƽ���
	int ProcAdd2_3(short gmid,const tagGMCommand* gmc,int playerid,int nums);

	//����ֵ ������
	int ProcAdd4_5(short gmid,const tagGMCommand* gmc,int playerid,int nums);

	//�������Ӣ�����м���
	int ProcClearSkill(short gmid,const tagGMCommand* gmc,int playerid);
	//���Ӣ����ѧ����
	int clearAllOfficerSkill(int playerid,int officerid);

	//���Ӣ�ѱ�����ָ����Ʒ
	int ProcAddBackpackGoods(short gmid,const tagGMCommand* gmc,int playerid,int goodsid,int level,int type);

	//��Ӳֿ���ָ����Ʒ 
	//int ProcAddGoods(int gmid,const tagGMCommand* gmc,int playerid,int mapid,int cityid,int goodsid,int type,int nums);

	//����ָ����������
	int ProcAddArms(short gmid,const tagGMCommand* gmc,int playerid,int mapid,int cityid,int armsid,int nums);

	//ָ���Ŀ����������о����
	int ProcAddResearch(short gmid,const tagGMCommand* gmc,int playerid,int rid,int level,int type);

	//��Ӳֿ���ָ����Ʒ  
	//1��ʳ��ľ�ģ�Ƥ�ϣ���
	//addGoods �˺�����;1;��ͼid;����id;��Ʒid;����
	//��PlayerResource��
	//2.	������
	//addGoods �˺�����;2;��ͼid;����id;��Ʒid;����id;����
	//��ProductionAmount��
	int ProcAddGoods_1(short gmid,const tagGMCommand* gmc,int playerid,int mapid,int cityid,int goodsid,int nums);
	int ProcAddGoods_2(short gmid,const tagGMCommand* gmc,int playerid,int mapid,int cityid,int goodsid,int type,int nums);

	//DBG
	DB_Proxy				Proxy ;
	otl_connect db_;
};


#endif
