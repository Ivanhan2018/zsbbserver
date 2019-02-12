#include "ChatSystemLogic.h"
#include "ProChatSystem.h"
#include "ChatUserInfo.h"
#include "ChatUserManager.h"
#include "ChatRoomManager.h"
#include "DataTss.h"
#include <string>
#include "NetTask.h"
#include "Protocol.h"
#include "DB_Table.h"
#include <iostream>
#include "NewsReport.h"
#include <crtdbg.h>
#include "log.hpp"
using namespace std ;
int CChatSystemLogic::ReqEnterChatSystem(REQ_ENTER_CHAT_SYSTEM_STRUCT* lpReq)
{
	cout<<"*************ChatLogic::ReqEnterChatSystem()***********"<<endl;
	PlayerChatRoomTable  chatroom ;
	PlayerOfficerTable Officertable;
	int ret ;
	if (lpReq)
	{
		chatroom.setid(lpReq->ulUserID);
		ret = proxy.db_select(chatroom);
		if (ret != DB_Proxy::DB_SUCCEED)
		{
			return 0 ;
		}
		Officertable.setmasterid(lpReq->ulUserID);
		Officertable.setdetailid(lpReq->ulUserID);
		ret = proxy.db_select(Officertable);
		if (ret != DB_Proxy::DB_SUCCEED)
		{
			return 0;
		}
		CChatUserInfo_prt UserInfo = g_ChatUserManager->GetChatUserInfo(lpReq->ulUserID);
		if (UserInfo==NULL)
		{
			UserInfo = g_ChatUserManager->AddNewChatUserInfo(lpReq->ulUserID);
			UserInfo->SetAddHandle(lpReq->agent);
			UserInfo->SetMapID(SERVER_BOUND_MAP_OTHER);
			UserInfo->SetUserName(Officertable.officername_);
			UserInfo->SetCountry(chatroom.country_);
			UserInfo->SetUserLevel(Officertable.level_);
			UserInfo->SetUserMilitaryRank(Officertable.militaryrank_);
			UserInfo->SetChatRoom(chatroom.roomid_);

			//SendNewsReport(lpReq->ulUserID); // 发送战报消息

			ProOnLineNotify(lpReq->ulUserID ,Officertable.officername_);

		}	
		else
		{
			UserInfo->SetAddHandle(lpReq->agent);
		}
		
		//玩家的工会编号，聊天室
		string UserName ;
		UserName= lpReq->Name;
		g_ChatUserManager->m_NameToIDList[UserName] = lpReq->ulUserID ;

		map<unsigned long ,list<unsigned long> > ::iterator iter ;
		iter = g_ChatUserManager->m_mapUserIdList.find(lpReq->ulMapID);
		if (iter!=g_ChatUserManager->m_mapUserIdList.end())
		{
			list<unsigned long> l = iter->second ;
			l.push_back(lpReq->ulUserID);
		}
		else
		{
			list<unsigned long> l ;
			l.push_back(lpReq->ulUserID);
			g_ChatUserManager->m_mapUserIdList[lpReq->ulMapID] = l ;
		}
	}
	return 0 ;
} 

int  CChatSystemLogic::ProChatInfo(CHAT_INFO_STRUCT* lpChatInfo)
{
	cout<<"*************ChatLogic::ProChatInfo()***********"<<endl;


	cout<<lpChatInfo->ulOtherUser<<":"<<lpChatInfo->info<<endl;
	switch(lpChatInfo->ChatType)
	{
		case tBoth:
            talkBoth(lpChatInfo);		
			break;
		case tWorld://=2,
			talkWorld(lpChatInfo);
			break ;
		case tMap://	=3,
			talkMap(lpChatInfo);
			break ;
		case tUnion://	=4,
			talkUnion(lpChatInfo);
			break ;
		case tRoom: //	=5
			talkRoom(lpChatInfo);
			break ;
		case tALL:
			talkALL(lpChatInfo);
			break;
		default:
			break ;
	}
	return 0 ;
}

int  CChatSystemLogic::talkBoth(CHAT_INFO_STRUCT* lpChatInfo)
{
	CChatUserInfo_prt UserInfo = g_ChatUserManager->GetChatUserInfo(lpChatInfo->ulUserID);
	CChatUserInfo_prt UserOther = NULL ;
	if (UserInfo)
	{
		CHAT_INFO_STRUCT Rsp ;
		string UserName = lpChatInfo->ulOtherUser;
		unsigned long OtherID = g_ChatUserManager->m_NameToIDList[UserName];
		UserOther = g_ChatUserManager->GetChatUserInfo(OtherID);
		if (UserOther)
		{
			Rsp.ChatType = lpChatInfo->ChatType ;
			memcpy(Rsp.info , lpChatInfo->info,SIZE_128-1);
			Rsp.info[SIZE_128-1]='\0';
			char* name = UserInfo->GetPlayerName();
			memcpy(Rsp.ulOtherUser,name,SIZE_16-1);
			Rsp.ulOtherUser[SIZE_16-1] ='\0';
		}	
		else
		{
			return 0 ;
		}
		//SEND Massage 
		Rsp.serialize(MDM_GP_MAP_CHAT , OtherID,Rsp.ulOtherUser,lpChatInfo->ChatType,lpChatInfo->info,RSP_SEND_CHAT_INFO);
		asynch_write(UserOther->GetAddHandle(),(char*)&Rsp,sizeof(Rsp));

	}
	UserInfo = NULL ;
	UserOther = NULL ;
	return 0 ;
}

int  CChatSystemLogic::talkMap(CHAT_INFO_STRUCT* lpChatInfo)
{
	CChatUserInfo_prt User = g_ChatUserManager->GetChatUserInfo(lpChatInfo->ulUserID);
	CChatUserInfo_prt Other= NULL;
	map<unsigned long ,list<unsigned long> >::iterator iter;
	map<unsigned long ,CChatUserInfo*>::iterator iterUser;
	CHAT_INFO_STRUCT Rsp ;
	memset(&Rsp, 0 ,sizeof(Rsp));
	if (User && User->GetMapID()!= SERVER_BOUND_MAP_OTHER)
	{
		Rsp.ChatType = lpChatInfo->ChatType;
		//strcpy(Rsp.info,lpChatInfo->info);
		memcpy(Rsp.info , lpChatInfo->info,SIZE_128-1);
		Rsp.info[SIZE_128-1]='\0';
		memcpy(Rsp.ulOtherUser , lpChatInfo->ulOtherUser,SIZE_16-1);
		Rsp.ulOtherUser[SIZE_16-1] ='\0';
		Rsp.ulUserID = lpChatInfo->ulUserID ;
		Rsp.serialize(MDM_GP_MAP_CHAT , User->GetUserID(),lpChatInfo->ulOtherUser,lpChatInfo->ChatType,lpChatInfo->info,RSP_SEND_CHAT_INFO , User->GetAddHandle());
		int ret = broadcast_ex(User->GetMapID(),(char*)&Rsp,Rsp.length);
		if (ret == 0)
		{
			printf("send error");
		}
		/*for (iterUser = g_ChatUserManager->m_ChatUserList.begin(); iterUser != g_ChatUserManager->m_ChatUserList.end(); iterUser ++)
		{
			Other = iterUser->second;
			if (User->GetMapID()==Other->GetMapID())
			{
				assert(lpChatInfo->ulOtherUser);
				Rsp.serialize(Other->GetUserID(),lpChatInfo->ulOtherUser,lpChatInfo->ChatType,lpChatInfo->info,RSP_SEND_CHAT_INFO);
				int ret = asynch_write(Other->GetAddHandle(),(char*)&Rsp,sizeof(Rsp));
				if (ret ==0)
				{
					printf("send error");
				}
			}
		}*/
	}
	return 0 ;
}
int  CChatSystemLogic::talkUnion(CHAT_INFO_STRUCT* lpChatInfo)
{
	CChatUserInfo_prt User = g_ChatUserManager->GetChatUserInfo(lpChatInfo->ulUserID);
	if (User==0 || User->GetUnionID()==0)
	{
		return 0 ;
	}
	CChatUserInfo_prt Other = NULL ;
	CHAT_INFO_STRUCT Rsp;

	//memcpy(&Rsp, lpChatInfo , sizeof(CHAT_INFO_STRUCT));
	Rsp.ChatType = lpChatInfo->ChatType;
	//strcpy(Rsp.info,lpChatInfo->info);
	memcpy(Rsp.info , lpChatInfo->info,SIZE_128-1);
	Rsp.info[SIZE_128-1]='\0';
	memcpy(Rsp.ulOtherUser , lpChatInfo->ulOtherUser,SIZE_16-1);
	Rsp.ulOtherUser[SIZE_16-1] ='\0';
	Rsp.ulUserID = lpChatInfo->ulUserID ;
	map<unsigned long , CChatUserInfo_prt>::iterator iter;
	IceUtil::Mutex m;
	m.lock();
	for (iter = g_ChatUserManager->m_ChatUserList.begin() ;iter != g_ChatUserManager->m_ChatUserList.end() ;iter ++)
	{
		Other = iter->second ;
		if (Other!=0)
		{
			if (Other->GetUnionID() == User->GetUnionID() && Other->GetUnionID()!=0 )
			{
				Rsp.serialize(MDM_GP_MAP_CHAT , Other->GetUserID(),lpChatInfo->ulOtherUser,lpChatInfo->ChatType,lpChatInfo->info,RSP_SEND_CHAT_INFO);
				asynch_write(Other->GetAddHandle(),(char*)&Rsp,sizeof(Rsp));
				//g_NetTask.Socket.SendData(Other->GetAddHandle() ,2,RSP_SEND_CHAT_INFO ,&Rsp ,sizeof(Rsp));
			}
		}		
	}
	m.unlock();
	return 0 ;
}
int  CChatSystemLogic::talkWorld(CHAT_INFO_STRUCT* lpChatInfo) //国家频道
{
	CChatUserInfo_prt User = g_ChatUserManager->GetChatUserInfo(lpChatInfo->ulUserID);
	if(User== NULL)
	{
		return 0 ;
	}
	CChatUserInfo_prt Other = NULL ;
	CHAT_INFO_STRUCT Rsp ;
	
	//memset(&Rsp, 0 , sizeof(Rsp));
	if (User)
	{
		memset(&Rsp,0,sizeof(CHAT_INFO_STRUCT));
		//memcpy(&Rsp , lpChatInfo , sizeof(Rsp));
		Rsp.ChatType = lpChatInfo->ChatType;
		//strcpy(Rsp.info,lpChatInfo->info);
		memcpy(Rsp.info , lpChatInfo->info,SIZE_128-1);
		Rsp.info[SIZE_128-1]='\0';
		memcpy(Rsp.ulOtherUser , lpChatInfo->ulOtherUser,SIZE_16-1);
		Rsp.ulOtherUser[SIZE_16-1] ='\0';
		Rsp.ulUserID = lpChatInfo->ulUserID ;
		Rsp.serialize(MDM_GP_MAP_CHAT , User->GetUserID(),lpChatInfo->ulOtherUser,lpChatInfo->ChatType,lpChatInfo->info,RSP_SEND_CHAT_INFO,User->GetAddHandle());
		map<unsigned long , int >::iterator iter;
		IceUtil::Mutex m;
		m.lock();
		/*for (iter = g_ChatUserManager->m_MapBelongList.begin();iter != g_ChatUserManager->m_MapBelongList.end() ; iter ++)
		{

			if (User->GetContry() == (*iter).second)
			{
				int ret = broadcast_ex((*iter).first,(char*)&Rsp,Rsp.length);
			}

		}*/
		map<unsigned long , CChatUserInfo_prt >::iterator iter1 ;
		for (iter1 = g_ChatUserManager->m_ChatUserList.begin() ; iter1!= g_ChatUserManager->m_ChatUserList.end() ;iter1 ++)
		{
			Other = iter1->second ;
			if(Other->GetContry() == User->GetContry())
			{
				Rsp.serialize(MDM_GP_MAP_CHAT , Other->GetUserID(),lpChatInfo->ulOtherUser,lpChatInfo->ChatType,lpChatInfo->info,RSP_SEND_CHAT_INFO);
				asynch_write(Other->GetAddHandle(),(char*)&Rsp,sizeof(Rsp));
				//g_NetTask.Socket.SendData(Other->GetAddHandle(),2,RSP_SEND_CHAT_INFO,&Rsp,sizeof(Rsp));
			}
		}
		m.unlock();
	}
	User = NULL ;
	Other = NULL ;
	return 0 ;
}
int  CChatSystemLogic::talkRoom(CHAT_INFO_STRUCT* lpChatInfo)
{
	cout<<"*************ChatLogic::talkRoom()***********"<<endl;
	CHAT_INFO_STRUCT Rsp ; 
	CChatUserInfo_prt User = g_ChatUserManager->GetChatUserInfo(lpChatInfo->ulUserID);
	CChatUserInfo_prt Other = NULL ;
	if (!User)
	{
		return 0 ;
	}
	CChatRoom_ptr Room  = g_ChatRoomManager->GetChatRoom(User->GetChatRoom());
	if (Room == NULL)
	{
		return 0 ;
	}
	//memcpy(&Rsp , lpChatInfo , sizeof(Rsp));
	Rsp.ChatType = lpChatInfo->ChatType;
	//strcpy(Rsp.info,lpChatInfo->info);
	memcpy(Rsp.info , lpChatInfo->info,SIZE_128-1);
	Rsp.info[SIZE_128-1]='\0';
	memcpy(Rsp.ulOtherUser , lpChatInfo->ulOtherUser,SIZE_16-1);
	Rsp.ulOtherUser[SIZE_16-1] ='\0';
	Rsp.ulUserID = lpChatInfo->ulUserID ;
	IceUtil::Mutex m;
	m.lock();
	for (int i = 0 ; i < MAX_SIZE_CHAT_ROOM ; i ++)
	{
		if (Room->m_PlayerState[i])
		{
			Other = g_ChatUserManager->GetChatUserInfo(Room->m_PlayerID[i]);
			if (Other)
			{
				Rsp.serialize(MDM_GP_MAP_CHAT , Other->GetUserID(),lpChatInfo->ulOtherUser,lpChatInfo->ChatType,lpChatInfo->info,RSP_SEND_CHAT_INFO);
				asynch_write(Other->GetAddHandle(),(char*)&Rsp,sizeof(Rsp));
			}
		}
	}
	m.unlock();

	return 0 ;
}
int CChatSystemLogic::talkALL(CHAT_INFO_STRUCT* lpChatInfo){
	CChatUserInfo_prt User = g_ChatUserManager->GetChatUserInfo(lpChatInfo->ulUserID);
	if(User== NULL){
		return 0 ;
	}
	CChatUserInfo_prt Other = NULL ;
	CHAT_INFO_STRUCT Rsp ;
	memset(&Rsp,0,sizeof(CHAT_INFO_STRUCT));
	Rsp.ChatType = lpChatInfo->ChatType;
	memcpy(Rsp.info , lpChatInfo->info,SIZE_128-1);
	Rsp.info[SIZE_128-1]='\0';
	memcpy(Rsp.ulOtherUser , lpChatInfo->ulOtherUser,SIZE_16-1);
	Rsp.ulOtherUser[SIZE_16-1] ='\0';
	Rsp.ulUserID = lpChatInfo->ulUserID ;
	Rsp.serialize(MDM_GP_MAP_CHAT , User->GetUserID(),lpChatInfo->ulOtherUser,lpChatInfo->ChatType,lpChatInfo->info,RSP_SEND_CHAT_INFO,User->GetAddHandle());
	map<unsigned long , int >::iterator iter;
	map<unsigned long , CChatUserInfo_prt >::iterator iter1 ;
	for (iter1 = g_ChatUserManager->m_ChatUserList.begin() ; iter1!= g_ChatUserManager->m_ChatUserList.end() ;iter1 ++)
	{
		Other = iter1->second ;
		Rsp.serialize(MDM_GP_MAP_CHAT , Other->GetUserID(),lpChatInfo->ulOtherUser,lpChatInfo->ChatType,lpChatInfo->info,RSP_SEND_CHAT_INFO);
		asynch_write(Other->GetAddHandle(),(char*)&Rsp,sizeof(Rsp));
	}
	return 0;
}
int  CChatSystemLogic::talkSystem(char* Info)
{
	CHAT_INFO_STRUCT lpChatInfo;
	CChatUserInfo_prt Other = NULL ;
	lpChatInfo.ChatType = tSystem;
	strcpy(lpChatInfo.ulOtherUser,"SystemInfo");
	strcpy(lpChatInfo.info,Info);
	map<unsigned long , CChatUserInfo_prt >::iterator iter ;
	IceUtil::Mutex m;
	m.lock();
	for (iter = g_ChatUserManager->m_ChatUserList.begin() ; iter!= g_ChatUserManager->m_ChatUserList.end() ;iter ++)
	{
		Other = iter->second ;
		if(Other)
		{
			lpChatInfo.serialize(MDM_GP_MAP_CHAT , Other->GetUserID(),lpChatInfo.ulOtherUser,lpChatInfo.ChatType,lpChatInfo.info,RSP_SEND_CHAT_INFO);
			asynch_write(Other->GetAddHandle(),(char*)&lpChatInfo,sizeof(lpChatInfo));
		}
	}
	m.unlock();
	return 0 ;

}
int  CChatSystemLogic::ReqChangeMap(REQ_CHANGE_MAP_STRUCT* lpReq)
{
	cout<<"*************ChatLogic::ReqChangeMap()***********"<<endl;
	CChatUserInfo_prt User = g_ChatUserManager->GetChatUserInfo(lpReq->ulUserID);
	if (User)
	{
		User->SetMapID(lpReq->ulMapID);
	}
	return 0 ;
}

int  CChatSystemLogic::ReqEnterChatRoom(REQ_ENTER_CHAT_ROOM_STRUCT* lpReq) 
{
	cout<<"*************ChatLogic::ReqEnterChatRoom()***********"<<endl;
	// 获取玩家的聊天室
	unsigned long ChatRoom = 0 ;
	char* Name = NULL;
	int iCount = 0 ;
	CChatRoom_ptr Room = NULL;

	RSP_ENTER_CHAT_ROOM_STRUCT Rsp ;
	memset(&Rsp,0,sizeof(RSP_ENTER_CHAT_ROOM_STRUCT));
	RSP_OTHER_ENTER_CHAT_ROOM_STRUCT RspOterEnterRoom ;
	memset(&RspOterEnterRoom,0,sizeof(RSP_OTHER_ENTER_CHAT_ROOM_STRUCT));
	CChatUserInfo_prt User = g_ChatUserManager->GetChatUserInfo(lpReq->ulUserID);
	CChatUserInfo_prt Other = NULL ;
	if (!User)
	{
		return 0 ;
	}
	Name = User->GetPlayerName();
	//判断是否以前有个聊天室记录
	PlayerChatRoomTable Roomtable ;
	ChatRoomTable  Table ;
	int Key = 0;
	int Ret ;
	int RoomNum = g_ChatRoomManager->GetChatRoomNum();
	int RoomID = -1;
	Roomtable.setid(lpReq->ulUserID);
	if (RoomNum < MAX_CHAT_ROOM_NUM)
	{

		Ret = proxy.db_select(Roomtable);
		if(Ret==DB_Proxy::DB_EOF || Roomtable.roomid_ == 0)//没有聊天室记录
		{
			Table.player1_ = lpReq->ulUserID ;
			Table.player2_ = 0 ;
			Table.player3_ = 0 ;
			Table.player4_ = 0 ;
			Table.player5_ = 0 ;
			Table.player6_ = 0 ;
			Table.player7_ = 0 ;
			Table.player8_ = 0 ;
			//Table.setkeyvalue(Key);
			if(proxy.db_insert(Table) ==DB_Proxy::DB_SUCCEED)
			{
				Roomtable.roomid_ = Table.roomid_ ;
				Roomtable.playerid_ = lpReq->ulUserID ;
				if (proxy.db_update(Roomtable,Roomtable)!=DB_Proxy::DB_SUCCEED)
				{
					return 0 ;
				}
			}
			else
			{
				return 0 ;
			}
			Room = g_ChatRoomManager->AddNewChatRoom(Roomtable.roomid_);
			if (Room == NULL)
			{
				return 0 ;
			}
			Room->SetRoomMemberNum(1);
			Room->SetDBRoomID(Table.roomid_);
			Room->InitRoom(Table,lpReq->ulUserID ,proxy);
			User->SetChatRoom(Table.roomid_);
			Roomtable.setid(lpReq->ulUserID);
			Roomtable.roomid_ = Table.roomid_ ;
			if(!proxy.db_update(Roomtable,Roomtable))
				return 0;
			Rsp.flag = true ;
			Rsp.MemberNum = 1;
			Rsp.ulUserID = lpReq->ulUserID ;
			Rsp.Member[0].bstate = true;
			memcpy(Rsp.Member[0].Name,Room->m_PlayerName[0],SIZE_16-1);
			Rsp.Member[0].Name[SIZE_16-1] ='\0';
			Rsp.Member[0].ulUserID = lpReq->ulUserID ;
			Rsp.RoomID = Table.roomid_ ;
			User->SetChatRoom(Table.roomid_);

		}
		else // 有聊天记录
		{
			//判断是否已经有人登陆了聊天室
			Room = g_ChatRoomManager->GetChatRoom(Roomtable.roomid_);
			if (Room) //登陆
			{
				//Room->SetRoomMemberNum(1);
				Room->ChangeRoomMember(1,lpReq->ulUserID,proxy);
				RspOterEnterRoom.ulUserID = lpReq->ulUserID ;
				for (int i = 0 ; i < 8 ; i ++)//通知其他玩家有人近入
				{
					if (Room->m_PlayerState[i])
					{
						Other = g_ChatUserManager->GetChatUserInfo(Room->m_PlayerID[i]);
						if (Other)
						{
							RspOterEnterRoom.serialize(Other->GetAddHandle(),Other->GetUserID(),RSP_OTHER_ENTER_CHAT_ROOM);
							asynch_write(Other->GetAddHandle(),(char*)&RspOterEnterRoom,RspOterEnterRoom.length);
						}
					}
				}					
			}
			else
			{
				Room = g_ChatRoomManager->AddNewChatRoom(Roomtable.roomid_);
				if (Room==0){
					return 0;
				}
				Table.setkeyvalue(Roomtable.roomid_);
				//Table.setid(Roomtable.roomid_);
				Ret = proxy.db_select(Table);
				if (Ret!=1)
				{
					return 0;
				}
				Room->InitRoom(Table , lpReq->ulUserID,proxy);
				Room->SetDBRoomID(Table.roomid_);
				User->SetChatRoom(Roomtable.roomid_);
			}
			Rsp.flag = true ;
			Rsp.ulUserID = lpReq->ulUserID ;
			Rsp.RoomID = Room->GetRoomID();
			Rsp.MemberNum = Room->GetMemberCount();
			for (int i =0 ; i < 8 ; i ++)
			{
				if (Room->m_PlayerID[i]!=0)
				{
					Rsp.Member[iCount].ulUserID = Room->m_PlayerID[i] ;
					memcpy(Rsp.Member[iCount].Name,Room->m_PlayerName[i],SIZE_16-1);
					Rsp.Member[iCount].Name[SIZE_16-1]='\0';
					Rsp.Member[iCount].bstate =Room->m_PlayerState[i];
					iCount ++ ;
				}
			}
		}
	}
	else //聊天室已满 ，等待
	{
		Rsp.flag = false ;
	}
	Rsp.serialize(User->GetAddHandle(),User->GetUserID(),RSP_ENTER_CHAT_ROOM);
	asynch_write(User->GetAddHandle(),(char*)&Rsp,Rsp.length); // 通知其他人上线消息
	return 0 ;
}
int  CChatSystemLogic::ReqInviteOtherEnterChatRoom(REQ_INVITE_OTHER_ENTER_CHAT_ROOM_STRUCT* lpReq)
{
	cout<<"*************ChatLogic::ReqInviteOtherEnterChatRoom()***********"<<endl;

	CChatUserInfo_prt User = g_ChatUserManager->GetChatUserInfo(lpReq->ulUserID);
	CChatUserInfo_prt Other =NULL;// g_ChatUserManager->GetChatUserInfo(lpReq->ulOtherID);
	CChatUserInfo_prt lpOterplayer = NULL;
	CChatRoom_ptr Room = NULL ;
	string str(lpReq->szOtherName);
	//str.copy(lpReq->szReciverPlayer,SIZE_24);
	unsigned long ulOtherID= g_ChatUserManager->GetHeroID(str);
	Other = g_ChatUserManager->GetChatUserInfo(ulOtherID);

	int i = 0 ;

	RSP_INVITE_OTHER_ENTER_CHAT_ROOM_STRUCT Rsp ;
	//RSP_ENTER_CHAT_ROOM_STRUCT InviteToChatRoom ;
	if (Other && Other->GetChatRoom()!=0)
	{

		return 0;
	}
	PlayerChatRoomTable RoomTable;
	memset(&Rsp , 0 , sizeof(RSP_INVITE_OTHER_ENTER_CHAT_ROOM_STRUCT));
	if (User)
	{
		Room = g_ChatRoomManager->GetChatRoom(User->GetChatRoom());
		if (Other) // 在线
		{
			
			if (Room && Room->GetRoomMemberNum()<MAX_SIZE_CHAT_ROOM) // 人数未满
			{
				Rsp.flag = true ;
				RoomTable.setid(ulOtherID);
				int ret = proxy.db_select(RoomTable);
				if (ret == DB_Proxy::DB_SUCCEED)
				{
					RoomTable.roomid_ = User->GetChatRoom() ;
					if(proxy.db_update(RoomTable,RoomTable) != DB_Proxy::DB_SUCCEED )
						return 0;
				}
				//RoomTable.roomid_ = Room->GetDBRoomID();
			
				Room->ChangeRoomMember(1,ulOtherID,proxy) ;

				Other->SetChatRoom(User->GetChatRoom());
				Rsp.ulOtherID = ulOtherID ;
			}
			else//人数已满
			{
				Rsp.flag = false ;
			}
		}
		else // 不在线
		{
			Rsp.flag = false ;
		}
		//发送信息 其他室友
		if (Rsp.flag)
		{
			Rsp.ulOtherID = ulOtherID ;
			memcpy(Rsp.Name , lpReq->szOtherName,SIZE_16-1);
			Rsp.Name[SIZE_16-1]='\0';
			REQ_ENTER_CHAT_ROOM_STRUCT info ;
			info.ulUserID = ulOtherID ;
			//ReqEnterChatRoom(&info);
			for (int i = 0 ; i < MAX_SIZE_CHAT_ROOM ; i ++ )
			{
				if (Room->m_PlayerState[i])
				{
					lpOterplayer = g_ChatUserManager->GetChatUserInfo(Room->m_PlayerID[i]);
					if (lpOterplayer)
					{
						Rsp.serialize(lpOterplayer->GetAddHandle(),lpOterplayer->GetUserID(),RSP_INVITE_OTHER_ENTER_CHAT_ROOM);
						asynch_write(lpOterplayer
							->GetAddHandle(),(char*)&Rsp,Rsp.length);
						//g_NetTask.Socket.SendData(lpOterplayer->GetAddHandle(),2,RSP_INVITE_OTHER_ENTER_CHAT_ROOM,&Rsp,sizeof(Rsp));
					}
					
				}
			}
			
		}
		else
		{
			Rsp.serialize(User->GetAddHandle(),User->GetUserID(),RSP_INVITE_OTHER_ENTER_CHAT_ROOM);
			asynch_write(User->GetAddHandle(),(char*)&Rsp,Rsp.length);
			//g_NetTask.Socket.SendData(User->GetAddHandle(), 2, RSP_INVITE_OTHER_ENTER_CHAT_ROOM,&Rsp , sizeof(Rsp));
		}
	}
	else
	{
		return 0 ;
	}
	return 0 ;
	
}
int  CChatSystemLogic::ReqExitRoom(REQ_EXIT_CHAT_ROOM_STRUCT* lpReq)
{
	cout<<"*************ChatLogic::ReqExitRoom()***********"<<endl;
	CChatUserInfo_prt user = g_ChatUserManager->GetChatUserInfo(lpReq->ulUserID);
	CChatUserInfo_prt Other = NULL;
	RspExitChatRoom Rsp;
	memset(&Rsp,0,sizeof(RspExitChatRoom));
	if (user)
	{
		CChatRoom_ptr Room = g_ChatRoomManager->GetChatRoom(user->GetChatRoom());
		user->SetChatRoom(0);
		if (Room)
		{
			Rsp.ulUserID = lpReq->ulUserID;
			Room->ChangeRoomMember(-1,lpReq->ulUserID,proxy);
			for (int i = 0 ; i < 8 ; i ++)
			{
				if (Room->m_PlayerState[i])
				{
					Other = g_ChatUserManager->GetChatUserInfo(Room->m_PlayerID[i]);
					if (Other)
					{
						Rsp.serialize(sizeof(RspExitChatRoom),Other->GetAddHandle(),MDM_GP_MAP_CHAT,Other->GetUserID());
						asynch_write(Rsp.agent,(char*)&Rsp,Rsp.length);
					}
				}
			}
			
			PlayerChatRoomTable chattable;
			chattable.setid(lpReq->ulUserID);

			int ret = proxy.db_select(chattable);
			if (ret == DB_Proxy::DB_SUCCEED)
			{
				chattable.roomid_ = 0;
				ret = proxy.db_update(chattable,chattable);
				if (ret != DB_Proxy::DB_SUCCEED)
				{
					cout<<"update db error"<<endl;
				}
			}
			if(Room->GetRoomMemberNum()<=0)
			{
				ChatRoomTable Table ;
				Table .setid(Room->GetDBRoomID());
				int ret= proxy.db_delete(Table);
		
				//ret = proxy.db_delete(Table);
				if (ret !=DB_Proxy::DB_SUCCEED)
				{
					return 0 ;
				}
				g_ChatRoomManager->DeleteChatRoom(lpReq->ulRoomID);				
			}
		}
	}
	return 0 ;
}

int  CChatSystemLogic::ReqExitChatSystem(REQ_EXIT_CHAT_SYSTEM_STRUCT* lpReq)
{
	cout<<"*************ChatLogic::ReqExitChatSystem()***********"<<endl;
	int Room;
	CChatRoom_ptr ChatRoom = 0;
	RSP_EXIT_CHAT_SYSTEM_STRUCT Rsp;
	memset(&Rsp,0,sizeof(RSP_EXIT_CHAT_SYSTEM_STRUCT));
	CChatUserInfo_prt User = g_ChatUserManager->GetChatUserInfo(lpReq->ulUserID);
	CChatUserInfo_prt Other = NULL;
	if (User)
	{
		ProDownLineNotify(lpReq->ulUserID , User->GetPlayerName());
		Room = User->GetChatRoom();
		if (Room>= 0&& Room<=5000)
		{
			Rsp.ulUserID = lpReq->ulUserID;
			ChatRoom = g_ChatRoomManager->GetChatRoom(Room);
				if (ChatRoom)
			{
				ChatRoom->upline(lpReq->ulUserID);
				for (int i = 0 ; i < 8 ; i ++)
				{
					if (ChatRoom->m_PlayerState[i])
					{
						Other = g_ChatUserManager->GetChatUserInfo(ChatRoom->m_PlayerID[i]);
						if (Other)
						{
							Rsp.serialize(sizeof(RspExitChatRoom),Other->GetAddHandle(),MDM_GP_MAP_CHAT,Other->GetUserID(),RSP_EXIT_CHAT_SYSTEM);
							asynch_write(Rsp.agent,(char*)&Rsp,Rsp.length);
						}
					}
				}
				if (ChatRoom->GetRoomMemberNum() ==0)
				{
					g_ChatRoomManager->DeleteChatRoom(Room);
				}
			}
			
		}
		g_ChatUserManager->DeleteChatUserInfo(lpReq->ulUserID);
	}
	User = NULL;
	return 0 ;
}

////////////////////////////mail system//////////////////////////////////////////////
int  CChatSystemLogic::ReqEnterMailSytemInfo(ReqEnterMailSystem* lpReq)
{
	cout<<"*************ChatLogic::ReqEnterMailSytemInfo()***********"<<endl;
	CChatUserInfo_prt UserInfo = g_ChatUserManager->GetChatUserInfo(lpReq->playerID);
	if (UserInfo == NULL)
	{
		return 0;
	}
	MailTable mailinfo;
	char * p;
	int len =0,count =0;
	int tmplen = 0;
	bool flag = false;
	RspEntermailSystem Rsp;
	memset(&Rsp,0,sizeof(RspEntermailSystem));
	mailinfo.setmasterid(lpReq->playerID);
	int ret = proxy.db_multi_select(mailinfo,p,len);
	if(len>0)
	{
		while (tmplen+sizeof(MailTable)<=len)
		{
			memcpy(&mailinfo,p+tmplen,sizeof(MailTable));
			//count ++;
			strcpy(Rsp.mailInfo[count].sztitle,mailinfo.title_);
			strcpy(Rsp.mailInfo[count].szSendPlayerName,mailinfo.sendplayername_);
			Rsp.mailInfo[count].flag = mailinfo.flag_;
			Rsp.mailInfo[count].ulTime = mailinfo.time_;
			Rsp.mailInfo[count].ulMailID = mailinfo.mailid_;
			//Rsp.mailInfo[count].type = mailinfo.mailtype_;
			Rsp.mailInfo[count].type = static_cast<MAIL_TYPE>(mailinfo.mailtype_);
			count ++;
			if (count==50)
			{
				//send massage 
				Rsp.icount = count;
				Rsp.serialize(sizeof(Rsp),UserInfo->GetAddHandle(),MDM_GP_MAP_CHAT,lpReq->playerID,RSP_ENTER_MAIL_SYSTEM_INFO);
				asynch_write(UserInfo->GetAddHandle(),(char*)&Rsp,Rsp.length);
				count =0;
				flag = true;
			}
			tmplen += sizeof(mailinfo);
		}
		delete [] p;
	}
	if (count>0) // 没有邮件
	{
		Rsp.icount = count;
		Rsp.serialize(sizeof(DISTRIBUTED_HEADER)+sizeof(int)+sizeof(tagMailBaseInfo)*count,UserInfo->GetAddHandle(),MDM_GP_MAP_CHAT,lpReq->playerID,RSP_ENTER_MAIL_SYSTEM_INFO);
		asynch_write(UserInfo->GetAddHandle(),(char*)&Rsp,Rsp.length);

	}
	return 1;
}
int  CChatSystemLogic::ReqGetMailBody(ReqGetMailBodyInfo* lpReq)
{
	cout<<"*************ChatLogic::ReqGetMailBody()***********"<<endl;
	CChatUserInfo_prt UserInfo = g_ChatUserManager->GetChatUserInfo(lpReq->ulUserID);
	if (UserInfo == NULL)
	{
		return 0;
	}
	
	MailTable mailinfo;
	RspGetMaiBodyInfo Rsp;
	memset(&Rsp,0,sizeof(RspGetMaiBodyInfo));
	mailinfo.setmasterid(lpReq->ulUserID);
	mailinfo.setdetailid(lpReq->ulMailID);
	int ret = proxy.db_select(mailinfo);
	if (ret == DB_Proxy::DB_SUCCEED)
	{
		strcpy(Rsp.szMailBody,mailinfo.text_);
		Rsp.ulMailID =lpReq->ulMailID;
		memcpy(Rsp.szPertain,mailinfo.pertain_,SIZE_64-1);
		Rsp.szPertain[SIZE_64-1];
		Rsp.flag = 1;
		mailinfo.flag_ = 0;
		ret = proxy.db_update(mailinfo,mailinfo);
		if (ret != DB_Proxy::DB_SUCCEED)
		{
			cout<<"update db error"<<endl;
		}
	}
	else
	{
		Rsp.flag = 0;
	}
	Rsp.serialize(sizeof(Rsp),UserInfo->GetAddHandle(),MDM_GP_MAP_CHAT,lpReq->ulUserID,RSP_GET_MAIL_BODY_INFO);
	asynch_write(UserInfo->GetAddHandle(),(char*)&Rsp,Rsp.length);
	//mailinfo.flag_ = 1;
	

	return 1;

}
int  CChatSystemLogic::ReqSendMail(ReqSendMailInfo* lpReq)
{
	cout<<"*************ChatLogic::ReqSendMail()***********"<<endl;
	CChatUserInfo_prt UserInfo = g_ChatUserManager->GetChatUserInfo(lpReq->ulPlayerID);

	CChatUserInfo_prt ReciverUser  = NULL;
	if (UserInfo == NULL)
	{
		return 0;
	}
	RspSendMailInfo Rsp;
	memset(&Rsp,0,sizeof(RspSendMailInfo));
	string str(lpReq->szReciverPlayer);
	//str.copy(lpReq->szReciverPlayer,SIZE_24);
	unsigned long ulReciverID= g_ChatUserManager->GetHeroID(str);
	MailTable mailinfo;
	int ret ;
	IceUtil::Time begin = IceUtil::Time::Date(2000, 1, 1);
	if (ulReciverID != 0)
	{
		mailinfo.flag_ = 1;
		mailinfo.receiverplayerid_ = ulReciverID;
		memcpy(mailinfo.sendplayername_ ,UserInfo->GetPlayerName(),SIZE_24-1);
		mailinfo.sendplayername_[SIZE_24-1]='\0';
		memcpy(mailinfo.text_,lpReq->szMailBody,SIZE_256-1);
		mailinfo.text_[SIZE_256-1]='\0';
		memcpy(mailinfo.title_,lpReq->szTitle,SIZE_16-1);
		mailinfo.title_[SIZE_16-1]='\0';
		mailinfo.mailtype_ = Normal;
		mailinfo.time_ = IceUtil::Time::now().toSeconds() - begin.toSeconds();
		ret = proxy.db_insert(mailinfo);
		if (ret!= DB_Proxy::DB_SUCCEED)
		{
			Rsp.flag = 0;
		}
		else
		{
			Rsp.flag = 1;
		}
	}
	else
	{
		Rsp.flag = 0;
	}
	Rsp.serialize(sizeof(RspGetMaiBodyInfo),UserInfo->GetAddHandle(),MDM_GP_MAP_CHAT,lpReq->ulPlayerID,RSP_GET_MAIL_BODY_INFO);
	asynch_write(UserInfo->GetAddHandle(),(char*)&Rsp,Rsp.length);
	//判断
	ReciverUser = g_ChatUserManager->GetChatUserInfo(ulReciverID);
	if (ReciverUser) // 通知有收件人
	{
		NotifyNewMailArrive Notify;
		Notify.flag = 0 ;
		memcpy(Notify.szSendPlayerName, mailinfo.sendplayername_,SIZE_24-1);
		Notify.szSendPlayerName[SIZE_24-1]='\0';
		memcpy(Notify.sztitle,lpReq->szTitle,SIZE_16-1);
		Notify.sztitle[SIZE_16-1] = '\0';
		Notify.type = Normal;
		Notify.ulMailID = mailinfo.mailid_;
		Notify.ulTime = mailinfo.time_;
		Notify.serialize(sizeof(NotifyNewMailArrive),ReciverUser->GetAddHandle(),MDM_GP_MAP_CHAT,ulReciverID,RSP_NEW_MAIL_ARRIVE_INFO);
		asynch_write(Notify.agent,(char*)&Notify,Notify.length);

	}
	return 0;
}
int  CChatSystemLogic::ReqDeleteMail(ReqDeleteMailInfo* lpReq)
{
	cout<<"*************ChatLogic::ReqDeleteMail()***********"<<endl;
	CChatUserInfo_prt UserInfo = g_ChatUserManager->GetChatUserInfo(lpReq->ulUserID);
	if (UserInfo== NULL)
	{
		return 0;
	}
	MailTable mailinfo;
	RspDeleteMailInfo Rsp;
	mailinfo.setmasterid(lpReq->ulUserID);
	mailinfo.setdetailid(lpReq->ulMailID);
	int ret = proxy.db_delete(mailinfo);
	Rsp.mailID = lpReq->ulMailID;
	if (ret == DB_Proxy::DB_SUCCEED)
	{
		Rsp.flag = true;
	}
	else
	{
		Rsp.flag = false;
	}
	Rsp.serialize(sizeof(RspDeleteMailInfo),UserInfo->GetAddHandle(),MDM_GP_MAP_CHAT,lpReq->ulUserID,RSP_DELETE_MAIL_INFO);
	asynch_write(UserInfo->GetAddHandle(),(char*)&Rsp,Rsp.length);

	return 1;
}

///////////////////////////friend system///////////////////////////////////////////////

int  CChatSystemLogic::ReqEnterFriendSystemInfo(ReqEnterFriendSystem* lpReq)
{
	cout<<"*************ChatLogic::ReqEnterFriendSystemInfo()***********"<<endl;
	CChatUserInfo_prt UserInfo = g_ChatUserManager->GetChatUserInfo(lpReq->ulUserID);
	//CChatUserInfo* UserFriend = NULL;
	if (UserInfo == NULL)
	{
		return 0;
	}
	CChatUserInfo_prt userFriend = NULL;
	RspEnterFriendSystem Rsp;
	memset(&Rsp,0,sizeof(RspEnterFriendSystem));
	//RspNotifyFriendOnline Notify;
	//Notify.ulFriendID = lpReq->ulUserID;
	//memcpy(Notify.szName,UserInfo->GetPlayerName(),SIZE_24-1);
	//Notify.szName[SIZE_24-1]='\0';
	FirendTable friendtable;
	PlayerOfficerTable Officer;
	friendtable.setmasterid(lpReq->ulUserID,lpReq->type);
	char* p;
	int len,tmplen =0;
	int count =0;
	int ret = proxy.db_multi_select(friendtable,p,len);
	bool flag = true;
	if (len>0)
	{
		while(tmplen+sizeof(FirendTable)<=len)
		{
			memcpy(&friendtable,p+tmplen,sizeof(FirendTable));
			Rsp.firend[count].ulUserID = friendtable.firendplayerid_;
			Officer.setmasterid(friendtable.firendplayerid_);
			Officer.setdetailid(friendtable.firendplayerid_);
			ret = proxy.db_select(Officer);
			if (ret == DB_Proxy::DB_SUCCEED)
			{
				Rsp.firend[count].ilevel = Officer.level_;
				Rsp.firend[count].military_rank = Officer.militaryrank_;
			}
			else
			{
				Rsp.firend[count].ilevel = friendtable.firendlevel_;

				Rsp.firend[count].military_rank = friendtable.militaryrank_;
			}
			
			
			memcpy(Rsp.firend[count].szName,friendtable.firendname_,SIZE_24-1);
			Rsp.firend[count].szName[SIZE_24-1]='\0';
			userFriend = g_ChatUserManager->GetChatUserInfo(friendtable.firendplayerid_);
			if (userFriend && userFriend->GetUserID() != UserInfo->GetUserID())
			{
				Rsp.iCount = count;
				Rsp.firend[count].bIsOnline = true;
				Rsp.firend[count].coutryid = userFriend->GetContry();			 
			}
			else
			{
				PlayerChatRoomTable chatroom;
				chatroom.setid(lpReq->ulUserID);
				ret = proxy.db_select(chatroom);
				if (ret != DB_Proxy::DB_SUCCEED)
				{
					return 0 ;
				}
				Rsp.firend[count].coutryid = chatroom.country_;				
				Rsp.firend[count].bIsOnline = false;
			}

			count ++;
			if (count>=50)
			{
				Rsp.iCount = count;
				Rsp.type = lpReq->type;
				Rsp.serialize(sizeof(RspEnterFriendSystem),UserInfo->GetAddHandle(),MDM_GP_MAP_CHAT,lpReq->ulUserID,RSP_ENTER_FRIEND_SYSTEM_INFO);
				asynch_write(UserInfo->GetAddHandle(),(char*)&Rsp,Rsp.length);
				count =0;
				flag = false;
			}
			tmplen+=sizeof(FirendTable);
		}
		
		delete [] p;
	}
	if (count>0)
	{
		Rsp.iCount = count;
		Rsp.type = lpReq->type;
		Rsp.serialize(sizeof(int)*2+sizeof(DISTRIBUTED_HEADER)+sizeof(FriendInfo)*count,UserInfo->GetAddHandle(),MDM_GP_MAP_CHAT,lpReq->ulUserID,RSP_ENTER_FRIEND_SYSTEM_INFO);
		asynch_write(UserInfo->GetAddHandle(),(char*)&Rsp,Rsp.length);
	}
	return 1;
}
int  CChatSystemLogic::ReqAddFriendInfo(ReqAddFriend * lpReq)
{
	cout<<"*************ChatLogic::ReqAddFriendInfo()***********"<<endl;
	
	CChatUserInfo_prt UserInfo = g_ChatUserManager->GetChatUserInfo(lpReq->ulUserID);
	
	if (UserInfo == NULL){
		return 0;
	}
	RspAddFriend Rsp;
	memset(&Rsp,0,sizeof(RspAddFriend));
	unsigned long FriendID = g_ChatUserManager->GetHeroID(lpReq->szFirendName);
	if (FriendID == lpReq->ulUserID)
	{
		Rsp.flag = 4;
		Rsp.type = lpReq->type;
		Rsp.serialize(sizeof(RspAddFriend),lpReq->agent,MDM_GP_MAP_CHAT,lpReq->ulUserID,RSP_ADD_FRIEND_INFO);
		asynch_write(UserInfo->GetAddHandle(),(char*)&Rsp,Rsp.length);
		return 0;
	}
	CChatUserInfo_prt UserFriend = g_ChatUserManager->GetChatUserInfo(FriendID);
	if (lpReq->type == FRIEND)
	{
		FirendTable f;
		f.setmasterid(lpReq->ulUserID,1);
		f.setdetailid(FriendID);
		int ret = proxy.db_select(f);
		if (ret == DB_Proxy::DB_SUCCEED)
		{
			Rsp.flag = 4;
			Rsp.type = lpReq->type;
			Rsp.serialize(sizeof(RspAddFriend),UserInfo->GetAddHandle(),MDM_GP_MAP_CHAT,lpReq->ulUserID,RSP_ADD_FRIEND_INFO);
			asynch_write(UserInfo->GetAddHandle(),(char*)&Rsp,Rsp.length);
			return 0;
		}
		f.setmasterid(lpReq->ulUserID,2);
		f.setdetailid(FriendID);
		ret = proxy.db_select(f);
		if(ret == DB_Proxy::DB_SUCCEED)
		{
			 Rsp.flag = 4;
			 Rsp.type = lpReq->type;
			 Rsp.serialize(sizeof(RspAddFriend),UserInfo->GetAddHandle(),MDM_GP_MAP_CHAT,lpReq->ulUserID,RSP_ADD_FRIEND_INFO);
			 asynch_write(UserInfo->GetAddHandle(),(char*)&Rsp,Rsp.length);
			 return 0;
		}
		if (FriendID==0)
		{
			Rsp.flag = 0; // 玩家不存在
			
		}
		else
		{
			if (UserFriend == NULL)
			{
				//邮件方式处理
				Notify_Send_Mail Event;
				strcpy(Event.sztitle,"Invite add Friend");
				sprintf(Event.szbody,"%s Invite you Friend",UserInfo->GetPlayerName());
				memcpy(Event.szPertain,&(lpReq->ulUserID),sizeof(unsigned long));
				memcpy(Event.szSendPlayerName,UserInfo->GetPlayerName(),SIZE_16-1);
				Event.szSendPlayerName[SIZE_16-1]='\0';
				Event.ulRecieverPlayerID = FriendID;
				Event.type = Invite_add_friend;
				NotifySendMail(&Event);
				Rsp.flag = 2; // 玩家不再线
				g_ChatUserManager->AddFriendList(lpReq->ulUserID,FriendID);
			}
			else
			{
				RspInviteFriend InviteFriend;
				InviteFriend.iLevel = UserInfo->GetUserLevel();
				InviteFriend.iMilitary_rank = UserInfo->GetUserMilitaryRank();
				memcpy(InviteFriend.szName,UserInfo->GetPlayerName(),SIZE_24-1);
				InviteFriend.szName[SIZE_24-1]='\0';
				InviteFriend.ulFriendID = lpReq->ulUserID;
				InviteFriend.serialize(sizeof(RspInviteFriend),UserFriend->GetAddHandle(),MDM_GP_MAP_CHAT,FriendID,RSP_INVITE_ADD_FRIEND_INFO);
				asynch_write(UserFriend->GetAddHandle(),(char*)&InviteFriend,InviteFriend.length);
				g_ChatUserManager->AddFriendList(lpReq->ulUserID,FriendID);
				return 1;
			}
		}
	}
	else  // 黑名单
	{
		FirendTable f;
		f.setmasterid(lpReq->ulUserID,2);
		f.setdetailid(FriendID);
		int ret = proxy.db_select(f);
		if (ret == DB_Proxy::DB_SUCCEED)
		{
			Rsp.type = lpReq->type;
			Rsp.flag = 4;
			Rsp.serialize(sizeof(RspAddFriend),UserInfo->GetAddHandle(),MDM_GP_MAP_CHAT,lpReq->ulUserID,RSP_ADD_FRIEND_INFO);
			asynch_write(UserInfo->GetAddHandle(),(char*)&Rsp,Rsp.length);
			return 0;
		}
		PlayerChatRoomTable ChatRoom;
		ChatRoom.setid(FriendID);
		ret = proxy.db_select(ChatRoom);
		if (ret != DB_Proxy::DB_SUCCEED)
		{
			return 0;
		}
		if (UserFriend == NULL)
		{
			Rsp.flag = 1; // 玩家不在线
			FirendTable T,T1;
			T.setmasterid(lpReq->ulUserID,FRIEND);
			T.setdetailid(FriendID);
			
			ret = proxy.db_select(T);

			if (ret == DB_Proxy::DB_SUCCEED)
			{
				memcpy(&T1,&T,sizeof(FirendTable));
				T1.firendtype_ = lpReq->type;
				ret = proxy.db_update(T,T1);
				Rsp.flag = 1;
				Rsp.Info.ilevel = T.firendlevel_;
				Rsp.Info.military_rank = T.militaryrank_;
				Rsp.Info.coutryid = ChatRoom.country_;
				strcpy(Rsp.Info.szName,T.firendname_);
				Rsp.type = lpReq->type;
			}
			else
			{
			
				PlayerOfficerTable Officer;
				Officer.setmasterid(FriendID);
				Officer.setdetailid(FriendID);
				ret = proxy.db_select(Officer);
				if (ret != DB_Proxy::DB_SUCCEED)
				{
					return 0;
				}

				T.firendlevel_ = Officer.level_;
				strcpy(T.firendname_,Officer.officername_);
				T.firendplayerid_ = FriendID;
				T.firendtype_ = lpReq->type;
				T.militaryrank_ = Officer.militaryrank_;
				proxy.db_insert(T);
				Rsp.flag = 1;
				Rsp.Info.ilevel = Officer.level_;
				Rsp.Info.military_rank = Officer.militaryrank_;
				Rsp.Info.coutryid = ChatRoom.country_;
				strcpy(Rsp.Info.szName,Officer.officername_);
				Rsp.type = lpReq->type;
			}
			
		}
		else
		{
			FirendTable T, T1;
			T.setmasterid(lpReq->ulUserID,FRIEND);
			T.setdetailid(FriendID);
			int ret = proxy.db_select(T);

			if (ret == DB_Proxy::DB_SUCCEED)
			{
				memcpy(&T1,&T,sizeof(FirendTable));
				T1.firendtype_ = lpReq->type;
				ret = proxy.db_update(T,T1);
			}
			else
			{
				T.firendlevel_ = UserFriend->GetUserLevel();
				strcpy(T.firendname_,UserFriend->GetPlayerName());
				T.firendplayerid_ = UserFriend->GetUserID();
				T.firendtype_ = lpReq->type;
				T.militaryrank_ = UserFriend->GetUserMilitaryRank();
				proxy.db_insert(T);
			}
			Rsp.flag = 1;
			Rsp.Info.ilevel = UserFriend->GetUserLevel();
			Rsp.Info.military_rank = UserFriend->GetUserMilitaryRank();
			memcpy(Rsp.Info.szName,lpReq->szFirendName,SIZE_24-1);
			Rsp.Info.szName[SIZE_24-1]='\0';
			Rsp.type = lpReq->type;
			Rsp.Info.coutryid = ChatRoom.country_;
		}
	}
	Rsp.serialize(sizeof(RspAddFriend),UserInfo->GetAddHandle(),MDM_GP_MAP_CHAT,lpReq->ulUserID,RSP_ADD_FRIEND_INFO);
	asynch_write(UserInfo->GetAddHandle(),(char*)&Rsp,Rsp.length);
	
	return 0;
}

int  CChatSystemLogic::ReqAcceptAddFriendInfo(ReqAcceptAddFriend* lpReq)
{
	cout<<"*************ChatLogic::ReqAcceptAddFriendInfo()***********"<<endl;
	CChatUserInfo_prt Info = g_ChatUserManager->GetChatUserInfo(lpReq->ulUserID);	
	if (Info == NULL)
	{
		return 0;
	}
	CChatUserInfo_prt UserFriend = g_ChatUserManager->GetChatUserInfo(lpReq->ulFriendID);
	RspAddFriend Rsp;
	RspAcceptAddFriend RspAccept;
	FirendTable Table1,Table ,fTable;
	PlayerOfficerTable officerTable;
	int ret ;
	memset(&Rsp,0,sizeof(RspAddFriend));
	if (lpReq->flag) // 接受
	{
		unsigned long FriendID = g_ChatUserManager->GetFriendList(lpReq->ulFriendID);
		if (FriendID != lpReq->ulUserID)
		{
			RspAccept.flag = 0; //失败
			
		}

		else 
		{
			if (UserFriend == NULL)
			{
				RspAccept.flag =2; // 对方不再线
				officerTable.setmasterid(lpReq->ulFriendID);
				officerTable.setdetailid(lpReq->ulFriendID);
				ret = proxy.db_select(officerTable);
				if (ret != DB_Proxy::DB_SUCCEED)
				{
					return 0;
				}
				Table1.firendlevel_ = officerTable.level_;
				strcpy(Table1.firendname_,officerTable.officername_);
				Table1.firendplayerid_ = lpReq->ulFriendID;
				Table1.playerid_ = lpReq->ulUserID;
				Table1.militaryrank_ = officerTable.militaryrank_;
			}
			else
			{

				Table1.firendlevel_ = UserFriend->GetUserLevel();
				strcpy(Table1.firendname_,UserFriend->GetPlayerName());
				Table1.firendplayerid_ = lpReq->ulFriendID;
				Table1.playerid_ = lpReq->ulUserID;
				Table1.militaryrank_ = UserFriend->GetUserMilitaryRank();

				RspAccept.flag = 1;
				Rsp.flag =1;
				Rsp.type = FRIEND;
				Rsp.Info.ilevel = Info->GetUserLevel();
				Rsp.Info.military_rank = Info->GetUserMilitaryRank();
				memcpy(Rsp.Info.szName ,Info->GetPlayerName(),SIZE_24-1);
				Rsp.Info.szName[SIZE_24-1]='\0';
				Rsp.Info.bIsOnline = true;
				Rsp.Info.ulUserID = lpReq->ulUserID;
				Rsp.Info.coutryid = Info->GetContry();
				
			}
			Table.playerid_ = lpReq->ulFriendID;
			Table.firendplayerid_ = lpReq->ulUserID;
			Table.firendlevel_ = Info->GetUserLevel();
			Table.militaryrank_ = Info->GetUserMilitaryRank();
			strcpy(Table.firendname_,Info->GetPlayerName());
			//向数据库中插入记录
			Table.firendtype_ = FRIEND;
			memcpy(&fTable,&Table,sizeof(FirendTable));
			fTable.firendtype_ = BLACKLIST;
			if (proxy.db_select(fTable)==DB_Proxy::DB_SUCCEED) //如果一方在黑名单中取消
			{
				return 0 ;
			}
			memcpy(&fTable,&Table1,sizeof(FirendTable));
			fTable.firendtype_ = BLACKLIST;
			if (proxy.db_select(fTable)==DB_Proxy::DB_SUCCEED)//如果一方在黑名单中取消
			{
				return 0 ;
			}
			ret = proxy.db_insert(Table);
			if (ret == DB_Proxy::DB_FAILED)
			{
				cout<<"Insert db error"<<endl;
				return 0;
			}
			Table1.firendtype_ = FRIEND;
			ret = proxy.db_insert(Table1);
			if (ret == DB_Proxy::DB_FAILED)
			{
				cout<<"Insert db error"<<endl;
				return 0 ;
			}
			if (UserFriend)
			{
				Rsp.Info.bIsOnline = true;
				Rsp.Info.ilevel = Info->GetUserLevel();
				Rsp.Info.military_rank = Info->GetUserMilitaryRank();
				Rsp.Info.ulUserID = Info->GetUserID();
				Rsp.Info.coutryid = Info->GetContry();
				memcpy(Rsp.Info.szName,Info->GetPlayerName(),SIZE_24-1);
				Rsp.Info.szName[SIZE_24-1]='\0';
				Rsp.type = FRIEND;
				Rsp.serialize(sizeof(RspAddFriend),UserFriend->GetAddHandle(),MDM_GP_MAP_CHAT,lpReq->ulFriendID,RSP_ADD_FRIEND_INFO);
				asynch_write(UserFriend->GetAddHandle(),(char*)&Rsp,Rsp.length);
			}
			Rsp.flag = true;
			if (UserFriend)
			{
				Rsp.Info.bIsOnline = true;
			}
			else
			{
				Rsp.Info.bIsOnline = false;
			}
			Rsp.Info.ilevel = Table1.firendlevel_;
			Rsp.Info.military_rank = Table1.militaryrank_;
			if (UserFriend)
			{
				memcpy(Rsp.Info.szName,UserFriend->GetPlayerName(),SIZE_24-1);
				Rsp.Info.szName[SIZE_24-1]='\0';
			}
			else
			{
				memcpy(Rsp.Info.szName,officerTable.officername_,SIZE_24-1);
				Rsp.Info.szName[SIZE_24-1]='\0';
			}
			
			Rsp.Info.ulUserID = lpReq->ulFriendID;
			Rsp.serialize(sizeof(RspAddFriend),Info->GetAddHandle(),MDM_GP_MAP_CHAT,lpReq->ulUserID,RSP_ADD_FRIEND_INFO);
			asynch_write(Info->GetAddHandle(),(char*)&Rsp,Rsp.length);
		}
	}
	else
	{
		if (UserFriend)
		{
			Rsp.flag = 3;
			Rsp.type = FRIEND;
			Rsp.serialize(sizeof(RspAddFriend),UserFriend->GetAddHandle(),MDM_GP_MAP_CHAT,lpReq->ulFriendID,RSP_ADD_FRIEND_INFO);
			asynch_write(UserFriend->GetAddHandle(),(char*)&Rsp,Rsp.length);
		}
		

	}
	return 1;
}

int  CChatSystemLogic::ReqDeleteFriendInfo(ReqDeleteFriend* lpReq)
{
	cout<<"*************ChatLogic::ReqDeleteFriendInfo()***********"<<endl;
	CChatUserInfo_prt UserInfo = g_ChatUserManager->GetChatUserInfo(lpReq->ulUserID);
	if (UserInfo == NULL)
	{
		return 0 ;
	}
	bool flag = false;
	RspDeleteFriend Rsp;
	Rsp.flag = 1;
	Rsp.type = lpReq->type;
	FirendTable Table;
	Table.setmasterid(lpReq->ulUserID,lpReq->type);
	Table.setdetailid(lpReq->ulFirendID);
	int ret = proxy.db_select(Table);
	if (ret == DB_Proxy::DB_SUCCEED)
	{
		proxy.db_delete(Table);
		flag = true;
		//return 0;
	}
	if (lpReq->type == FRIEND)
	{
		Table.setmasterid(lpReq->ulFirendID,lpReq->type);
		Table.setdetailid(lpReq->ulUserID);
		proxy.db_delete(Table);
		//Rsp.ulFriendID = lpReq->ulUserID;
		
	}
	if (flag)
	{
		Rsp.ulFriendID = lpReq->ulFirendID;
		Rsp.serialize(sizeof(RspDeleteFriend),UserInfo->GetAddHandle(),MDM_GP_MAP_CHAT,lpReq->ulUserID);
		asynch_write(Rsp.agent,(char*)&Rsp,Rsp.length);
		CChatUserInfo_prt UserFriend = g_ChatUserManager->GetChatUserInfo(lpReq->ulFirendID);
		if (UserFriend && lpReq->type == FRIEND)
		{
			Rsp.ulFriendID = lpReq->ulUserID;
			Rsp.serialize(sizeof(RspDeleteFriend),UserFriend->GetAddHandle(),MDM_GP_MAP_CHAT,lpReq->ulFirendID);
			asynch_write(Rsp.agent,(char*)&Rsp,Rsp.length);
		}
	}	
	
	return 1;
}

int  CChatSystemLogic::EventAddMainHero(Notify_Add_Main_Hero* lpEvent)
{
	cout<<"*************ChatLogic::EventAddMainHero()***********"<<endl;
    string str(lpEvent->szOfficerName);
	//str.copy(lpEvent->szOfficerName,24);
	g_ChatUserManager->AddMainHero(str,lpEvent->iOfficerID);
	return 0 ;
}

void  CChatSystemLogic::ProOnLineNotify(unsigned long playerid ,char* playerName)
{
	cout<<"*************ChatLogic::ProDownLineNotify()***********"<<endl;
	CChatUserInfo_prt UserFriend;
	FirendTable table;
	char* p;
	int len;
	int tmplen =0;
	RspNotifyFriendOnline Rsp;
	Rsp.ulFriendID = playerid;
	memcpy(Rsp.szName,playerName,SIZE_24-1);
	Rsp.szName[SIZE_24-1]='\0';
	table.setmasterid(playerid,FRIEND);
	int ret = proxy.db_multi_select(table,p,len);
	if (len>0)
	{
		while (tmplen+sizeof(FirendTable)<=len)
		{
			memcpy(&table,p+tmplen,sizeof(FirendTable));
			UserFriend = g_ChatUserManager->GetChatUserInfo(table.firendplayerid_);
			if (UserFriend && UserFriend->GetUserID() != playerid)
			{
				Rsp.serialize(sizeof(RspNotifyFriendOnline),UserFriend->GetAddHandle(),MDM_GP_MAP_CHAT,table.firendplayerid_,RSP_NOTIFY_FRIEND_ONLINE);
				asynch_write(Rsp.agent,(char*)&Rsp,Rsp.length);
			}
			tmplen+=sizeof(FirendTable);
		}
		delete [] p;
	}
}

void  CChatSystemLogic::ProDownLineNotify(unsigned long playerid , char* playerName)
{
	cout<<"*************ChatLogic::ProDownLineNotify()***********"<<endl;
	CChatUserInfo_prt UserFriend;
	FirendTable table;
	char* p;
	int len;
	int tmplen =0;
	RspNotifyFriendDownLine Rsp;
	Rsp.ulFriendID = playerid;
	memcpy(Rsp.szName,playerName,SIZE_24-1);
	Rsp.szName[SIZE_24-1]='\0';
	table.setmasterid(playerid,FRIEND);
	int ret = proxy.db_multi_select(table,p,len);
	if (len>0)
	{
		while (tmplen+sizeof(FirendTable)<=len)
		{
			memcpy(&table,p+tmplen,sizeof(FirendTable));
			UserFriend = g_ChatUserManager->GetChatUserInfo(table.firendplayerid_);
			if (UserFriend)
			{
				Rsp.serialize(sizeof(RspNotifyFriendDownLine),UserFriend->GetAddHandle(),MDM_GP_MAP_CHAT,table.firendplayerid_,RSP_NOTIFY_FRIEND_DOWNLINE);
				asynch_write(Rsp.agent,(char*)&Rsp,Rsp.length);
			}
			tmplen+=sizeof(FirendTable);
		}
		delete [] p;
	}

}

int CChatSystemLogic::NotifySendMail(Notify_Send_Mail* lpEvent)
{
	cout<<"*************ChatLogic::NotifySendMail()***********"<<endl;
	CChatUserInfo_prt UserInfo = g_ChatUserManager->GetChatUserInfo(lpEvent->ulRecieverPlayerID);
	MailTable mail;
	IceUtil::Time begin = IceUtil::Time::Date(2000, 1, 1);
	//mail.flag_ = 0;
	mail.receiverplayerid_ = lpEvent->ulRecieverPlayerID;
	memcpy(mail.sendplayername_ ,lpEvent->szSendPlayerName,SIZE_24-1);
	mail.sendplayername_[SIZE_24-1]='\0';
	memcpy(mail.title_ ,lpEvent->sztitle,SIZE_16-1);
	mail.title_[SIZE_16-1]='\0';
	memcpy(mail.text_,lpEvent->szbody,SIZE_256-1);
	mail.text_[SIZE_256-1] = '\0';
	memcpy(mail.pertain_,lpEvent->szPertain,63);
	mail.pertain_[63]='\0';
	mail.time_ = IceUtil::Time::now().toSeconds() - begin.toSeconds();
	mail.mailtype_ = lpEvent->type;
	mail.flag_ = 1;
	int ret = proxy.db_insert(mail);
	LOG_TRACE_EX(ret == DB_Proxy::DB_SUCCEED,("in insert db error\n"));
	if (ret != DB_Proxy::DB_SUCCEED){
		return 0;
	}
	NotifyNewMailArrive Notify;
	if (UserInfo) // 接受用户在线，直接通知
	{
		memset(&Notify,0,sizeof(NotifyNewMailArrive));
		Notify.ulMailID = mail.mailid_;
		Notify.flag =1;
		Notify.type =lpEvent->type;
		strcpy(Notify.szSendPlayerName,mail.sendplayername_);
		memcpy(Notify.sztitle, mail.title_,SIZE_16-1);
		Notify.sztitle[SIZE_16-1]='\0';

		Notify.ulTime = mail.time_ ;
		Notify.serialize(sizeof(NotifyNewMailArrive),UserInfo->GetAddHandle(),MDM_GP_MAP_CHAT,lpEvent->ulRecieverPlayerID);
		asynch_write(Notify.agent,(char*)&Notify,Notify.length);
	}
	return 1 ;
}

int CChatSystemLogic::NotifyAddConsortia(Notify_Add_Consortia* lpEvent)
{
	cout<<"*************ChatLogic::NotifyAddConsortia()***********"<<endl;
	CChatUserInfo_prt UserInfo = g_ChatUserManager->GetChatUserInfo(lpEvent->ulPlayerID);
	if (UserInfo)
	{
		UserInfo->SetUnionID(lpEvent->ulConsortiaID);
	}
	PlayerChatRoomTable Table;
	Table.setid(lpEvent->ulPlayerID);
	int ret = proxy.db_select(Table);
	if (ret == DB_Proxy::DB_SUCCEED)
	{
		Table.consortiaid_ = lpEvent->ulConsortiaID;
		ret = proxy.db_update(Table,Table);
		LOG_TRACE_EX(ret == DB_Proxy::DB_SUCCEED,("update db error\n"));
	}
	return 1;
}

int CChatSystemLogic::SendNewsReport(ReqGetNewsReportInfo* lpReq)
{
	cout<<"*************ChatLogic::SendNewsReport()***********"<<endl;
	CChatUserInfo_prt UserInfo = g_ChatUserManager->GetChatUserInfo(lpReq->playerID);
	RspNewsReport Rsp;
	memset(&Rsp,0,sizeof(RspNewsReport));
	if (UserInfo == NULL)
	{
		return 0;
	}
	NewsTable* News;
	int count = 0;
	map<unsigned int , NewsTable*>::iterator iter;
	for (iter = g_NewsReport.m_NewsList.begin();iter != g_NewsReport.m_NewsList.end();iter ++)
	{
		News = iter->second;
		memset(&(Rsp.News[count]),0,SIZE_128);
		memcpy(&(Rsp.News[count]),News,sizeof(tagNewsReport));
		if (Rsp.News[count].type == Map_Event1)
		{
			NewsFightReprotEvent_1 Event;
			memset(&Event,0,sizeof(NewsFightReprotEvent_1));
			memcpy(&Event,Rsp.News[count].szConent,sizeof(NewsFightReprotEvent_1));
			printf("time = %d,mapid =%d, type = %d\n",Event.time,Event.MapID,Event.type);
		}
		count ++;
		if (count>=MAX_NEWS_NUM)
		{
			Rsp.count = count;
			Rsp.serialize(sizeof(RspNewsReport),UserInfo->GetAddHandle(),MDM_GP_MAP_CHAT,lpReq->playerID,RSP_NEWS_REPROT_INFO);
			asynch_write(Rsp.agent,(char*)&Rsp,Rsp.length);
			count =0;
		}
	}
    if (count>0)
    {
		Rsp.count = count;
		Rsp.serialize(sizeof(int)+sizeof(tagNewsReport)*count + sizeof(DISTRIBUTED_HEADER),UserInfo->GetAddHandle(),MDM_GP_MAP_CHAT,lpReq->playerID,RSP_NEWS_REPROT_INFO);
		asynch_write(Rsp.agent,(char*)&Rsp,Rsp.length);
    }
	return 1;
}

int CChatSystemLogic::NotifyAddNewsReport(Notify_Add_News_Report* lpEvent)
{
	cout<<"*************ChatLogic::NotifyAddNewsReport()***********"<<endl;
	NewsTable* News;
	News = new NewsTable;
	News->newstype_ = lpEvent->type;
	memcpy(&(News->content_),lpEvent->szConent,SIZE_128-1);
	News->content_[127]='\0';

	int NewsID = g_NewsReport.insertNews(News,proxy);


	RspNotifyNewsReport Rsp;
	Rsp.Reprot.iNewsReportID = NewsID;
	memcpy(Rsp.Reprot.szConent,lpEvent->szConent,SIZE_128-1);
	Rsp.Reprot.szConent[127] = '\0';

	/*map<unsigned long , CChatUserInfo*>::iterator iter;
	CChatUserInfo* UserInfo = NULL;
	for(iter = g_ChatUserManager->m_ChatUserList.begin() ; iter != g_ChatUserManager->m_ChatUserList.end();iter ++)
	{
		UserInfo = iter->second;
		if (UserInfo)
		{
			Rsp.serialize(UserInfo->GetAddHandle(),MDM_GP_MAP_CHAT,UserInfo->GetUserID());
			asynch_write(Rsp.agent,(char*)&Rsp,Rsp.length);
		}
	}*/

	Rsp.serialize(4000,MDM_GP_MAP_CHAT,0);
	int  ret = broadcast((char*)&Rsp,Rsp.length);

	return 1;
}

void CChatSystemLogic::testNewsReport()
{
	NewsFightReprotEvent_1 Event1;
	Event1.MapID = 1025;
	Event1.time = IceUtil::Time::now().toSeconds();
	Event1.type = 1;
	NewsTable table;
	table.newstype_ = 1;
	memset(table.content_,0,SIZE_128);
	memcpy(table.content_,&Event1,sizeof(NewsFightReprotEvent_1));
	table.content_[SIZE_128-1]='\0';
	int ret = proxy.db_insert(table);
	if(ret != DB_Proxy::DB_SUCCEED)
	{
		return ;
	}
	NewsFightReprotEvent_2 Event2;
	Event2.MapID = 1025;
	Event2.time = IceUtil::Time::now().toSeconds();
	table.newstype_ = 2; 
	memset(table.content_,0,SIZE_128);
	memcpy(table.content_,&Event2,sizeof(NewsFightReprotEvent_2));
	table.content_[SIZE_128-1]='\0';

	ret = proxy.db_insert(table);
	if(ret != DB_Proxy::DB_SUCCEED)
	{
		return ;
	}
}

int CChatSystemLogic::NotifyNoticeforcesEvent(MODIFY_NOTICEFORCES_EVENT*lpEvent)
{
	CHAT_INFO_STRUCT Rsp ;
	memset(&Rsp,0,sizeof(CHAT_INFO_STRUCT));
	Rsp.ChatType = tAffiche;
	memcpy(Rsp.info,lpEvent->cData,lpEvent->DataLen);
	Rsp.info[lpEvent->DataLen]='\0';
	strcpy(Rsp.ulOtherUser,"势力公告");
	char name[SIZE_16];
	if (lpEvent->ForcesID == 1)
	{
		strcpy(name,"联盟势力公告");
	}
	else
	{
		strcpy(name,"部落势力公告");
	}
	Rsp.ulUserID = 0;
	CChatUserInfo_prt Other = 0;
	CHAT_TYPE type ;
	if (lpEvent->NoticeID ==1 )
	{
		type = tAffiche;
	}
	else
	{
		type = tComonAffiche;
	}

	//Rsp.serialize(User->GetUserID(),"势力公告",tAffiche,Rsp.info,lpEvent->cData,RSP_SEND_CHAT_INFO,4000);
	map<unsigned long , int >::iterator iter;
	map<unsigned long , CChatUserInfo_prt >::iterator iter1 ;
	mutex_.lock();
	for (iter1 = g_ChatUserManager->m_ChatUserList.begin() ; iter1!= g_ChatUserManager->m_ChatUserList.end() ;iter1 ++)
	{
		Other = iter1->second ;
		if(Other->GetContry() == lpEvent->ForcesID)
		{
			Rsp.serialize(MDM_GP_MAP_CHAT , Other->GetUserID(),name,type,Rsp.info,RSP_SEND_CHAT_INFO);
			asynch_write(Other->GetAddHandle(),(char*)&Rsp,sizeof(Rsp));
			//g_NetTask.Socket.SendData(Other->GetAddHandle(),2,RSP_SEND_CHAT_INFO,&Rsp,sizeof(Rsp));
		}
	}
	mutex_.unlock();

	return 0;
}

int CChatSystemLogic::TransmitforcesMail(Notify_BroadMail * lpEvent)
{
	PlayerChatRoomTable PlayerInfo;
	CChatUserInfo_prt ReciverUser;
	char * p ;
	int len;
	int ret  = proxy.db_select_all(PlayerInfo,p,len);
	int tmplen=0;
	MailTable mail;
	IceUtil::Time begin = IceUtil::Time::Date(2000, 1, 1);
	mail.flag_ = 1;
	mail.mailtype_ = Normal;
	memcpy(mail.sendplayername_ ,lpEvent->szSendPlayerName,SIZE_16-1);
	mail.sendplayername_[SIZE_16-1]='\0';
	memcpy(mail.text_,lpEvent->szbody,SIZE_256-1);
	mail.text_[SIZE_256-1]='\0';
	memcpy(mail.title_,lpEvent->sztitle,SIZE_16-1);
	//strcpy(mail.pertain_,"");
	memset(mail.pertain_, 0, SIZE_64);

	mail.title_[SIZE_16-1]='\0';
	mail.time_ = IceUtil::Time::now().toSeconds() - begin.toSeconds();
	while(tmplen+sizeof(PlayerChatRoomTable)<=len)
	{
		memcpy(&PlayerInfo,p+tmplen,sizeof(PlayerChatRoomTable));
		if (PlayerInfo.country_ == lpEvent->CountryID || lpEvent->CountryID ==0)
		{
			mail.receiverplayerid_ = PlayerInfo.playerid_;

			ReciverUser = g_ChatUserManager->GetChatUserInfo(PlayerInfo.playerid_);
			proxy.db_insert(mail);
			if (ReciverUser) // 通知有收件人
			{
				NotifyNewMailArrive Notify;
				Notify.flag = 0 ;
				memcpy(Notify.szSendPlayerName, lpEvent->szSendPlayerName,SIZE_24-1);
				Notify.szSendPlayerName[SIZE_24-1]='\0';
				memcpy(Notify.sztitle,lpEvent->sztitle,SIZE_16-1);
				Notify.sztitle[SIZE_16-1] = '\0';
				Notify.type = Normal;
				Notify.ulMailID = mail.mailid_;
				Notify.ulTime = mail.time_;
				Notify.serialize(sizeof(NotifyNewMailArrive),ReciverUser->GetAddHandle(),MDM_GP_MAP_CHAT,mail.receiverplayerid_,RSP_NEW_MAIL_ARRIVE_INFO);
				asynch_write(Notify.agent,(char*)&Notify,Notify.length);

			}
		}
		tmplen +=sizeof(PlayerChatRoomTable);
	}
	return 0 ;
}