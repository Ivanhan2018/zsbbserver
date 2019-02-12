#include ".\chatroom.h"
#include "DB_Table.h"
#include "ProChatSystem.h"
CChatRoom::CChatRoom(unsigned long RoomId): m_ulRoomID(RoomId)
{
	m_Membercount =0;
	for (int i = 0 ;i < 8 ; i ++)
	{
		m_PlayerID[i] =0;
	}
}

CChatRoom::~CChatRoom(void)
{

}
void CChatRoom::InitRoom(ChatRoomTable& RoomTable , unsigned  ulPlayerID ,DB_Proxy & proxy)
{
	int iCount = 0 ;
	int iOnlinecount = 0;
	m_PlayerID[0]= RoomTable.player1_ ;
	m_PlayerID[1]= RoomTable.player2_ ;
	m_PlayerID[2]= RoomTable.player3_ ;
	m_PlayerID[3]= RoomTable.player4_ ;
	m_PlayerID[4]= RoomTable.player5_ ;
	m_PlayerID[5]= RoomTable.player6_ ;
	m_PlayerID[6]= RoomTable.player7_ ;
	m_PlayerID[7]= RoomTable.player8_ ;
	for (int i = 0 ; i < 8 ; i ++)
	{
		if (m_PlayerID[i] == ulPlayerID)
		{			
			m_PlayerState[i] = true ;
		}
		else
		{
			m_PlayerState[i] = false ;
		}
		strcpy(m_PlayerName[i], "");		
		if (m_PlayerID[i]!=0)
		{
			PlayerOfficerTable Offier ;
			Offier.setmasterid(RoomTable.player1_);
			Offier.setdetailid(RoomTable.player1_);

			if (proxy.db_select(Offier)!=1)
			{
				return ;
			}
			memcpy(m_PlayerName[i],Offier.officername_,SIZE_16);
			iCount ++ ;
		}
	}
	SetRoomMemberNum(1);
	SetMemberCount(iCount);
}


void CChatRoom::ChangeRoomMember(int ch, unsigned long ulPlayerID , DB_Proxy& proxy)
{

	//CHAT_ROOM_MEMBER_STRUCT* Member ;
	PlayerOfficerTable oTable ;
	oTable.setmasterid(ulPlayerID);
	oTable.setdetailid(ulPlayerID);
	int ret ;
	if(proxy.db_select(oTable)!=DB_Proxy::DB_SUCCEED)
		return ;
	int mark = -1 ;
	if (ch>0) // 添加一个成员
	{
		for(int i = 0 ; i < 8 ; i ++)
		{
			if (m_PlayerID[i] == ulPlayerID) //已经室该聊天室成员
			{
				memcpy(m_PlayerName[i], oTable.officername_ , 13);
				m_PlayerState[i] = true ;
			
				break ;
			}
			if (mark == -1 && m_PlayerID[i]== 0 )
			{
				mark = i ;
			}
			
		}
		if (mark != -1) //新添加的成员
		{
			m_PlayerID[mark] = ulPlayerID ;
			m_PlayerState[mark] = true ;
			memcpy(m_PlayerName[mark], oTable.officername_ , 13);
			m_Membercount ++ ; //总的人数 +1
		}
		else
		{
			m_memberNum ++ ; //在线人数加1
		}
		
		
	}
	else //删除一个成员
	{
		for (int i = 0 ; i < 8 ; i ++ )
		{
			if (m_PlayerID[i] == ulPlayerID)
			{
				strcpy(m_PlayerName[i],"");
				m_PlayerState[i] = false ;
				m_PlayerID[i] = 0 ;
				break;
			}
		}
		PlayerChatRoomTable Table ;
		Table.setid(ulPlayerID);
		ret = proxy.db_select(Table);
		if (ret == DB_Proxy::DB_SUCCEED)
		{
			Table.roomid_ = 0 ;
			if(proxy.db_update(Table,Table) == DB_Proxy::DB_SUCCEED)
			{
				m_memberNum -- ;
				m_Membercount --;
			}

		}		
		//delete Member ;
	}

	ChatRoomTable RoomTable ;
	RoomTable.setid(m_ulRoomID);
	RoomTable.player1_ = m_PlayerID[0];
	RoomTable.player2_ = m_PlayerID[1];
	RoomTable.player3_ = m_PlayerID[2];
	RoomTable.player4_ = m_PlayerID[3];
	RoomTable.player5_ = m_PlayerID[4];
	RoomTable.player6_ = m_PlayerID[5];
	RoomTable.player7_ = m_PlayerID[6];
	RoomTable.player8_ = m_PlayerID[7];
	if (!proxy.db_update(RoomTable ,RoomTable))
	{
		return ;
		printf("update db error");
	}
	
}
void CChatRoom::upline(unsigned uPlayerID)
{
	for (int i = 0 ; i < 8 ; i ++)
	{
		if (m_PlayerID[i] == uPlayerID)
		{
			m_PlayerState[i] = false ;
			m_memberNum--;
		}
	}
}
//bool CChatRoom::AddMember(unsigned long ulUserID)
//{
//
//}