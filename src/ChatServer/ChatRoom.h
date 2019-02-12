/*
聊天室信息
*/
#pragma once
#include "ProChatSystem.h"
#include <list>
#include "DB_Table.h"
#include "NetTask.h"
using namespace std;
class CChatRoom :  virtual public IceUtil::Shared
{
public:
	CChatRoom(unsigned long RoomId);
	~CChatRoom(void);

	void SetRoomMemberNum(int Num)
	{
		m_memberNum = Num ;
	}
	int GetRoomMemberNum()
	{
		return m_memberNum ;
	}
	void ChangeRoomMemberNum(int num)
	{
		m_memberNum +=num;
	}
	void SetMemberCount(int count)
	{
		m_Membercount = count;
	}
	int GetMemberCount()
	{
		return m_Membercount;
	}
	void ChangerMemberCount(int count)
	{
		m_Membercount += count;
	}

	void InitRoom(ChatRoomTable& RoomTable , unsigned  playerID ,DB_Proxy& proxy);//进入房间或离开房间

	void upline(unsigned uPlayerID);//离线

	void SetEmpty(bool flag){m_IsEmpty = flag;}
	bool IsEmpty(){return m_IsEmpty;}

	void EmpytyRoomInfo()
	{
		for (int i = 0 ; i < 8 ; i ++)
		{
			m_PlayerID[i]=0;
			memset(m_PlayerName[i],0,13);
			m_PlayerState[i] = false;
		}

	}


	/*
	ch 改变数
	ulPlayerID 那个玩家ID
	*/
	void ChangeRoomMember(int ch , unsigned long ulPlayerID ,DB_Proxy& proxy);

	void SetRoomId(int id){m_ulRoomID = id;}
	int  GetRoomID(){return m_ulRoomID ;}

	void SetDBRoomID(int id){m_dbRoomID = id;}
	int  GetDBRoomID(){return m_dbRoomID;}
	
	unsigned long m_PlayerID[8];
	char m_PlayerName[8][SIZE_16];
	bool m_PlayerState[8];
private:
	int m_memberNum ; //聊天室在线人数
	int m_Membercount;
	bool m_IsEmpty;
	unsigned long m_ulRoomID;  // 服务器中的聊天室编号
	unsigned long m_dbRoomID;  // 数据库中的聊天室统一编号 用于保存再数据库中的KEY值
	
};

typedef IceUtil::Handle<CChatRoom> CChatRoom_ptr;
