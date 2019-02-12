#ifndef CHAT_LOGIC_H
#define CHAT_LOGIC_H
#include "ProChatSystem.h"
#include "Server_Protocol.h"
#include "DB_Proxy.h"
#include "IceUtil.h"
class CChatSystemLogic
{
public:
	int ReqEnterChatSystem(REQ_ENTER_CHAT_SYSTEM_STRUCT* lpReq); //��������ϵͳ

	int ProChatInfo(CHAT_INFO_STRUCT* lpChatInfo); //����������Ϣ
	int ReqExitChatSystem(REQ_EXIT_CHAT_SYSTEM_STRUCT* lpReq);//�˳�����ϵͳ
	int ReqChangeMap(REQ_CHANGE_MAP_STRUCT* lpReq);//�л�����ͼ��Ϣ
	int ReqEnterChatRoom(REQ_ENTER_CHAT_ROOM_STRUCT* lpReq) ;//����������
	int ReqInviteOtherEnterChatRoom(REQ_INVITE_OTHER_ENTER_CHAT_ROOM_STRUCT* lpReq);//������˼���������
	int ReqExitRoom(REQ_EXIT_CHAT_ROOM_STRUCT* lpReq);//�˳�������


	int talkBoth(CHAT_INFO_STRUCT* lpChatInfo);//˽��
	int talkWorld(CHAT_INFO_STRUCT* lpChatInfo);//��������
	int talkUnion(CHAT_INFO_STRUCT* lpChatInfo);//��������
	int talkMap(CHAT_INFO_STRUCT* lpChatInfo);//��ͼ����
	int talkRoom(CHAT_INFO_STRUCT* lpChatInfo);//����������
	int talkALL(CHAT_INFO_STRUCT* lpChatInfo);//ȫ���㲥
	int talkSystem(char* Info);//ϵͳ��Ϣ
	

	////////////////////////////mail system//////////////////////////////////////////////
	int ReqEnterMailSytemInfo(ReqEnterMailSystem* lpReq);//�����ʼ�ϵͳ
	int ReqGetMailBody(ReqGetMailBodyInfo* lpReq);//��ȡ�ʼ�
	int ReqSendMail(ReqSendMailInfo* lpReq);//�����ʼ�
	int ReqDeleteMail(ReqDeleteMailInfo* lpReq);//ɾ���ʼ�

	///////////////////////////friend system//////////////////////////////////////////////

	int ReqEnterFriendSystemInfo(ReqEnterFriendSystem* lpReq);//�������ϵͳ
	int ReqAddFriendInfo(ReqAddFriend * lpReq);//��Ӻ���
	int ReqDeleteFriendInfo(ReqDeleteFriend* lpReq);//ɾ������
	int ReqAcceptAddFriendInfo(ReqAcceptAddFriend* lpReq);//���պ��������Ϣ

	int EventAddMainHero(Notify_Add_Main_Hero* lpEvent);//�����Ӣ����Ϣ

	void ProOnLineNotify(unsigned long playerid,char * playername);//��������֪ͨ
	void ProDownLineNotify(unsigned long playerid , char * playername);//����������Ϣ

	int NotifySendMail(Notify_Send_Mail* lpEvent);//֪ͨ�����ʼ���Ϣ
	int NotifyAddConsortia(Notify_Add_Consortia* lpEvent);//֪ͨ���빤����Ϣ

	/////////////////////////////New Report system/////////////////////////////////////////
	int SendNewsReport(ReqGetNewsReportInfo* lpReq);//�����µ�ս����Ϣ
	int NotifyAddNewsReport(Notify_Add_News_Report* lpEvent);//֪ͨ����µ�ս����Ϣ
	int NotifyNoticeforcesEvent(MODIFY_NOTICEFORCES_EVENT*lpEvent);//֪ͨ������Ϣ
	int TransmitforcesMail(Notify_BroadMail * lpEvent); //ת�������ʼ�

	void testNewsReport();

public:
	DB_Proxy  proxy;
	IceUtil::Mutex mutex_;


};
#endif