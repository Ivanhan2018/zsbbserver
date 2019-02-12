/*
聊天室管理信息
*/
#pragma once
#include <map>
#include "ProChatSystem.h"
#include "ChatRoom.h"
#include <list>
using namespace std;
class CChatRoomManager :  virtual public IceUtil::Shared
{
public:
	CChatRoomManager(int Num = 0);
	~CChatRoomManager(void);

	CChatRoom_ptr AddNewChatRoom(int & RoomId);
	CChatRoom_ptr GetChatRoom(unsigned long RoomID);
	void DeleteChatRoom(unsigned long RoomID);

	void EmptyRoom();
	int GetChatRoomNum()
	{
		return m_chatRoomNum ;
	}

	//map<unsigned long , CChatRoom*> m_ChatRoomList;
	CChatRoom_ptr m_Room[MAX_CHAT_ROOM_NUM];//聊天室数量
private :
	int m_chatRoomNum ;
};
typedef IceUtil::Handle<CChatRoomManager> CNpcManager_ptr;
//extern CChatRoomManager* g_ChatRoomManager ;
extern CNpcManager_ptr g_ChatRoomManager;
