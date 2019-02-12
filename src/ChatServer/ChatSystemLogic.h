#ifndef CHAT_LOGIC_H
#define CHAT_LOGIC_H
#include "ProChatSystem.h"
#include "Server_Protocol.h"
#include "DB_Proxy.h"
#include "IceUtil.h"
class CChatSystemLogic
{
public:
	int ReqEnterChatSystem(REQ_ENTER_CHAT_SYSTEM_STRUCT* lpReq); //进入聊天系统

	int ProChatInfo(CHAT_INFO_STRUCT* lpChatInfo); //处理聊天休息
	int ReqExitChatSystem(REQ_EXIT_CHAT_SYSTEM_STRUCT* lpReq);//退出聊天系统
	int ReqChangeMap(REQ_CHANGE_MAP_STRUCT* lpReq);//切换郡地图信息
	int ReqEnterChatRoom(REQ_ENTER_CHAT_ROOM_STRUCT* lpReq) ;//进入聊天室
	int ReqInviteOtherEnterChatRoom(REQ_INVITE_OTHER_ENTER_CHAT_ROOM_STRUCT* lpReq);//邀请别人加入聊天室
	int ReqExitRoom(REQ_EXIT_CHAT_ROOM_STRUCT* lpReq);//退出聊天室


	int talkBoth(CHAT_INFO_STRUCT* lpChatInfo);//私聊
	int talkWorld(CHAT_INFO_STRUCT* lpChatInfo);//势力聊天
	int talkUnion(CHAT_INFO_STRUCT* lpChatInfo);//工会聊天
	int talkMap(CHAT_INFO_STRUCT* lpChatInfo);//地图聊天
	int talkRoom(CHAT_INFO_STRUCT* lpChatInfo);//聊天室聊天
	int talkALL(CHAT_INFO_STRUCT* lpChatInfo);//全服广播
	int talkSystem(char* Info);//系统信息
	

	////////////////////////////mail system//////////////////////////////////////////////
	int ReqEnterMailSytemInfo(ReqEnterMailSystem* lpReq);//进入邮件系统
	int ReqGetMailBody(ReqGetMailBodyInfo* lpReq);//获取邮件
	int ReqSendMail(ReqSendMailInfo* lpReq);//发送邮件
	int ReqDeleteMail(ReqDeleteMailInfo* lpReq);//删除邮件

	///////////////////////////friend system//////////////////////////////////////////////

	int ReqEnterFriendSystemInfo(ReqEnterFriendSystem* lpReq);//进入好友系统
	int ReqAddFriendInfo(ReqAddFriend * lpReq);//添加好友
	int ReqDeleteFriendInfo(ReqDeleteFriend* lpReq);//删除好友
	int ReqAcceptAddFriendInfo(ReqAcceptAddFriend* lpReq);//接收好友添加信息

	int EventAddMainHero(Notify_Add_Main_Hero* lpEvent);//添加主英雄信息

	void ProOnLineNotify(unsigned long playerid,char * playername);//处理在线通知
	void ProDownLineNotify(unsigned long playerid , char * playername);//处理下线信息

	int NotifySendMail(Notify_Send_Mail* lpEvent);//通知发送邮件信息
	int NotifyAddConsortia(Notify_Add_Consortia* lpEvent);//通知加入工会信息

	/////////////////////////////New Report system/////////////////////////////////////////
	int SendNewsReport(ReqGetNewsReportInfo* lpReq);//发送新的战报信息
	int NotifyAddNewsReport(Notify_Add_News_Report* lpEvent);//通知添加新的战报信息
	int NotifyNoticeforcesEvent(MODIFY_NOTICEFORCES_EVENT*lpEvent);//通知势力信息
	int TransmitforcesMail(Notify_BroadMail * lpEvent); //转发势力邮件

	void testNewsReport();

public:
	DB_Proxy  proxy;
	IceUtil::Mutex mutex_;


};
#endif