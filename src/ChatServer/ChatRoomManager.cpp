
#include ".\chatroommanager.h"
#include "log.hpp"
CNpcManager_ptr g_ChatRoomManager  ;
CChatRoomManager::CChatRoomManager(int Num) :m_chatRoomNum(Num)
{
	for (int i = 0 ; i< MAX_CHAT_ROOM_NUM; i ++)
	{
		m_Room[i]=new CChatRoom(0);
		m_Room[i]->SetRoomId(i+1);
	}
}

CChatRoomManager::~CChatRoomManager(void)
{
	EmptyRoom();
}
CChatRoom_ptr CChatRoomManager::AddNewChatRoom(int & RoomId)
{
	CChatRoom_ptr Room  = 0;/*= new CChatRoom(RoomId);
	if (Room)
	{
		m_ChatRoomList[RoomId] = Room ;
		m_chatRoomNum ++ ;
	}*/
	/*IceUtil::Mutex m;
	m.lock();*/
	for (int i = 0 ; i < MAX_CHAT_ROOM_NUM ; i ++)
	{
		if(m_Room[i]->IsEmpty())
		{
			Room = m_Room[i];
			//RoomId = i+1 ;

			Room->SetRoomId(i+1);
			Room->SetDBRoomID(RoomId);
			Room->SetEmpty(false);
			m_chatRoomNum ++;
			break;

		}
	}
	//m.unlock();
	
	return Room ;
}

CChatRoom_ptr CChatRoomManager::GetChatRoom(unsigned long RoomID)
{
	CChatRoom_ptr Room = NULL  ;
	if (RoomID > 5000 && RoomID<0)
	{
		return NULL;
	}
	//Room=m_Room[RoomID];
	for (int i  =0 ;i <MAX_CHAT_ROOM_NUM ; i ++)
	{
		if (m_Room[i]->GetDBRoomID() == RoomID)
		{
			Room  = m_Room[i];
			break ;
		}
	}
	/*map<unsigned long , CChatRoom* >::iterator iter;
	iter = m_ChatRoomList.find(RoomID);
	if (iter!=m_ChatRoomList.end())
	{
		Room = iter->second;
		
	}*/
	return Room ;
}

void CChatRoomManager::DeleteChatRoom(unsigned long RoomID)
{
	CChatRoom_ptr Room = NULL ;
	/*map<unsigned long , CChatRoom* >::iterator iter;
	iter = m_ChatRoomList.find(RoomID);
	if (iter!= m_ChatRoomList.end())
	{
		Room = iter->second ;
		m_ChatRoomList.erase(RoomID);
		delete Room ;
		Room = NULL ;
	}*/
	Room= m_Room[RoomID];
	if (Room)
	{
		Room->EmpytyRoomInfo();
		Room->SetEmpty(true);

		m_chatRoomNum -- ;
	}
	else
	{
		LOG_TRACE_EX(false, ("删除一个不成功的聊天房间ID%d\n",RoomID));
	}
	
}

void CChatRoomManager::EmptyRoom()
{
	CChatRoom_ptr Room = NULL ;
	/*map<unsigned long ,CChatRoom* >::iterator iter;
	for (iter = m_ChatRoomList.begin() ; iter != m_ChatRoomList.end() ; iter ++)
	{
		Room = iter->second ;
		delete Room ;
		Room = NULL ;
	}
	if (!m_ChatRoomList.empty())
	{
		m_ChatRoomList.clear() ;
	}*/
	for (int i = 0 ; i <MAX_CHAT_ROOM_NUM ;i ++)
	{
		Room = m_Room[i];
		//delete Room ;
		Room = 0 ;
	}
	
}