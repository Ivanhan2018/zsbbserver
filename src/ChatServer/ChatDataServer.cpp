#include "ChatDataServer.h"
#include "CommandPool.h"
#include "ProChatSystem.h"
#include "ChatSystemLogic.h"

CChatDataServer g_GetChatData ;
CChatDataServer::CChatDataServer(void)
{
}

CChatDataServer::~CChatDataServer(void)
{
}
void CChatDataServer::GetData(char* block ,CChatSystemLogic& Logic)
{
	char* buffer = NULL ;
	PROTOCOL_CHAT Cmd = static_cast<PROTOCOL_CHAT>(0) ;
	DISTRIBUTED_HEADER* phead = (DISTRIBUTED_HEADER*)block;
	if (phead->length <27)
	{
		printf("packet error");
		return ;
	}
	else
	{
		/*
		Function lpFunction = NULL ;
		Cmd=static_cast<PROTOCOL_CHAT>(phead->command);
		lpFunction =CCommandPool::m_RecvCommandPool[Cmd];
		if (lpFunction != NULL)
		{
			( *lpFunction )( buffer);
			g_DataTss.PutData(phead->agent);
		}
		lpFunction = NULL ;
		*/
		Cmd = static_cast<PROTOCOL_CHAT>(phead->command);
		switch(Cmd)
		{
		case REQ_ENTER_CHAT_SYSTEM:
			 Logic.ReqEnterChatSystem((REQ_ENTER_CHAT_SYSTEM_STRUCT *)block);
			 break;
		case REQ_ENTER_CHAT_ROOM:
			  Logic.ReqEnterChatRoom((REQ_ENTER_CHAT_ROOM_STRUCT*)block);
			 break;
		case REQ_INVITE_OTHER_ENTER_CHAT_ROOM:
			 Logic.ReqInviteOtherEnterChatRoom((REQ_INVITE_OTHER_ENTER_CHAT_ROOM_STRUCT*)block);
			break;
		case REQ_EXIT_CHAT_ROOM :
			 Logic.ReqExitRoom((REQ_EXIT_CHAT_ROOM_STRUCT*)block);
			break;
		case REQ_EXIT_CHAT_SYSTEM :
			 Logic.ReqExitChatSystem((REQ_EXIT_CHAT_SYSTEM_STRUCT*)block);
			break;
		case REQ_SNED_CHAT_INFO :
			  Logic.ProChatInfo((CHAT_INFO_STRUCT*)block);
			 break;
		case REQ_CHANGE_MAP_INFO:
			  Logic.ReqChangeMap((REQ_CHANGE_MAP_STRUCT*)block);
			 break;
		/////////////////////////////////mail system/////////////////////////////////////////
		case REQ_ENTER_MAIL_SYSTEM_INFO:
			  Logic.ReqEnterMailSytemInfo((ReqEnterMailSystem*) block);
			 break;
		case REQ_GET_MAIL_BODY_INFO:
			 Logic.ReqGetMailBody((ReqGetMailBodyInfo*)block);
			break;
		case REQ_SEND_MAIL_INFO:
			 Logic.ReqSendMail((ReqSendMailInfo*)block);
			break;
		case REQ_DELETE_MAIL_INFO:
			 Logic.ReqDeleteMail((ReqDeleteMailInfo*)block);
			break;
		////////////////////////////////friend system//////////////////////////////////////////
		case REQ_ENTER_FRIEND_SYSTEM_INFO:
			 Logic.ReqEnterFriendSystemInfo((ReqEnterFriendSystem*)block);
			break;
		case REQ_ADD_FRIEND_INFO:
			 Logic.ReqAddFriendInfo((ReqAddFriend*)block);
			break;
		case REQ_ACCEPT_ADD_FRIEND_INFO:
			 Logic.ReqAcceptAddFriendInfo((ReqAcceptAddFriend*)block);
			break;
		case REQ_DELETE_FRIEND_INFO:
			 Logic.ReqDeleteFriendInfo((ReqDeleteFriend*)block);
			break;
		case REQ_NEWS_REPORT_INFO:
			Logic.SendNewsReport((ReqGetNewsReportInfo*)block);
			break;
		/////////////////////////////////Event system/////////////////////////////////////////
		case CMD_ADD_MAIN_HERO:
			 Logic.EventAddMainHero((Notify_Add_Main_Hero*)block);
			 break;
		case CMD_ADD_CONSORTIA_NOTIFY:
			Logic.NotifyAddConsortia((Notify_Add_Consortia*)block);
			break;
		case CMD_ADD_MAIl_NOTIFY:
			Logic.NotifySendMail((Notify_Send_Mail*)block);
			break;
		case CMD_ADD_NEWS_REPORT_NOTIFY:
			Logic.NotifyAddNewsReport((Notify_Add_News_Report*)block);
			break;

		case GC_NOTICEFORCES_EVENT:
			Logic.NotifyNoticeforcesEvent((MODIFY_NOTICEFORCES_EVENT*)block);
			break;	
		case CMD_BROADMAIL_NOTIFY:
			Logic.TransmitforcesMail((Notify_BroadMail*)block);
			break;

		}
		delete [] block;
	}

}