#ifndef PROCMESSAGE_H__
#define PROCMESSAGE_H__
#include <stdlib.h>
#include <stdarg.h>
#include "Distributed_Header.h"
#include "DB_ProxyHeader.h"
#include "DBTableHeader.h"
#include "ConsortiaDataDefine.h"
#include "Server_Protocol.h"
#include "ProChatSystem.h"
#include "socketsvc.hpp"
#include "ChargeProtocol.h"
#include "ConsortiaManage.h"
#include "ISystemLog.h"

class CConsortiaManage;

class CProcMessage
{
public:
	CProcMessage(CConsortiaManage *pConManage);
	~CProcMessage();
	void ProcMsg(const char *msg);
private:
	bool ConnectDBG();//����DBG
	bool OnDefault(const char *msg);				//Ĭ�ϴ���
	bool OnCreateConsortia(const char *msg);		//��������
	bool OnQueryConsortiaList(const char *msg);		//��ѯ�����б�
	bool OnDeleteConsortial(const char *msg);		//ɾ������
	bool OnQueryUserConsortiaInfo(const char *msg);	//��ѯ�û�����������Ϣ
	bool OnModifyPlacard(const char *msg);			//�޸Ĺ���
	bool OnQueryLogo(const char *msg);				//��ѯLOGO
	bool OnModifyLogo(const char *msg);				//�޸�LOGO
	bool OnContributeBank(const char *msg);			//������Դ������
	bool OnPayoutBank(const char *msg);				//֧�乫�����е���Դ
	bool OnQueryBank(const char *msg);				//��ѯ��������
	bool OnAllyApply(const char *msg);				//��������(�����Ƿ�Ӧ������Ӫ���ƣ�����ͬ����Ӫ���޷����ˣ�������)
	bool OnAllyConsortia_Agree(const char *msg);	//ͬ�����
	bool OnAllyConsortia_Refuse(const char *msg);	//�ܾ�����
	bool OnUnallyConsortia(const char *msg);		//����
	bool OnFightApply(const char *msg);				//Լս����
	bool OnFightConsortia_Agree(const char *msg);	//ͬ��Լս
	bool OnFightConsortia_Refuse(const char *msg);	//ͬ��Լս
	bool OnQueryFight(const char *msg);				//Լս��ѯ
	bool OnJoinApply(const char *msg);				//������빫��
	bool OnJoinConsortia_Agree(const char *msg);	//ͬ����빫��
	bool OnJoinConsortia_Refuse(const char *msg);	//�ܾ����빫��
	bool OnQueryConUserList(const char *msg);		//��ѯ�����Ա�б�
	bool OnKickUser(const char *msg);				//Kickĳ��Ա
	bool OnQuitConsortia(const char *msg);			//�����˳�����
	bool OnInviteJoinConsortia(const char *msg);	//������빫��
	bool OnInviteJoinCon_Agree(const char *msg);	//ͬ������
	bool OnInviteJoinCon_Refuse(const char *msg);	//�ܾ�����
	bool OnPoseOfficial(const char *msg);			//��ְ
	bool OnDeposeOfficial(const char *msg);			//��ְ                                                   
	bool OnQueryOfficial(const char *msg);			//ְλ��ѯ
	bool OnQueryCreateConLimit(const char *msg);	//��ѯ�������������
	bool OnModifyConTenet(const char *msg);			//�޸���ּ
	bool OnClientLogin(const char *msg);			//��½
	bool OnTranChairman(const char *msg);			//�ƽ��᳤
	bool OnQueryPlayerRanking(const char *msg);		//��ȡ��������
	bool OnQueryConsortiaRanking(const char *msg);	//��ȡ��������
	bool OnQueryFriendsRanking(const char *msg);	//��ȡ����������������
	bool OnQueryMoneyRanking(const char *msg);		//��ȡ����������������
	bool OnQueryGeneralRanking(const char *msg);	//��ȡ����������������

	//add 20100302
	bool onModifyPurview(const char *msg);			//��Ȩ
	void onUpdateConsortia(const char *msg);		//����
	void onNotifyUpdateConsortia(const char *msg);	//�շ�ϵͳ����
	int updateConsortia(CConsortia *pConsortia,ConsortiaUpdateParam *pUpdateParam);	//�����ľ������
	void onContributeMoney(const char *msg);		//��Ǯ������
	void onSalaried(const char *msg);				//��ȡ����
	void onNotifyPlayerExpOrHonors(const char *msg);//��һ�ȡ�˾��������
	void onModifySignID(const char *msg);			//�޸ľ��ű�ʶ
	//����ϵͳ����
	bool OnQueryUserInfoOfCountry(const char *msg);	//��ѯ��ҵĹ������
	bool OnQueryCouncilList(const char *msg);		//��ѯ����Ӫ����Ա�б�
	bool OnTakePartInElection(const char *msg);	//�μӾ�ѡ������Ϊ��Ա��
	bool OnQueryCandidateList(const char *msg);//��ѯ��ѡ���б�
	bool OnPlayerBallot(const char *msg);				//ͶƱ
	bool OnPlayerDelate(const char *msg);				//����
	bool OnQueryDelateInfo(const char *msg);			//��ѯ������Ϣ
	bool OnDelateBallot(const char *msg);				//����ͶƱ
	bool OnModifyElectionCreed(const char *msg);		//�޸Ĳ�ѡ����
	bool OnCreateTirrier(const char *msg);				//������������������
	bool OnTirrierGoOut(const char *msg);				//����������
	bool OnUpgradeBuild(const char *msg);				//������������
	bool OnModifyNotice(const char *msg);				//�޸���������
	bool OnQueryTirrierCount(const char *msg);			//��ѯ����������
	bool OnElectionNotify(const char *msg);				//���յ���ѡ�۷ѣ��۷ѷ��������ص���Ϣ
	bool OnCreateTirrierNotify(const char *msg);		//֪ͨ����ͼ����������
	bool OnUpgradeBuildNotify(const char *msg);			//֪ͨ����ͼ������������
	bool OnVoteEndTimer(const char *msg);				//ѡ�ٽ�����ʱ��
	bool OnUnderWriteEndTimer(const char *msg);			//����ǩ������
	bool OnDelateVoteEndTimer(const char *msg);			//����ͶƱ����
	bool OnTirrierGoOutTimer(const char *msg);			//������������ʱ��
	bool OnModifyTirrierState(const char *msg);			//������������
	bool OnMarketRate(const char *msg);					//�г���˰
private:
	//int GetPlayerInfo(int playerid,char *name,int &country,bool &isonline,int &agent);//��ȡ�����Ϣ���ڴ��в��������ѯ���ݿ�
	int PoseOfficial(int player1id,int player2id,int jobid,int &consortiaid);//�ι���ְλ
	int DeposeOfficial(int player1id,int player2id,int &consortiaid);//�⹫��ְλ
	int SendMsg2Consortia(int consortiaid,CHAT_TYPE type,const char *msg,int nomsg_playerid = 0);//�·������ڲ���Ϣ,nomsg_playeridΪ0ʱ��ȫ���·�
	int SendMsg2Map(int country,CHAT_TYPE msg_type,const char *msg,int snd_type);//�·��㲥��Ϣ��������ͼ��
	int SendMsg2Player(int playerid,CHAT_TYPE type,const char *msg);//��ĳ������·���Ϣ
private:
	CConsortiaManage *m_ConsortiaManage;
	DB_Proxy m_DBProxy;
};


#endif
