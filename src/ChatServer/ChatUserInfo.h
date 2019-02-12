#pragma once
#include "IceUtil.h"
class CChatUserInfo :virtual public IceUtil::Shared
{
public:
	CChatUserInfo(unsigned long ulUserId);
	~CChatUserInfo(void);
	void SetMapID(unsigned long mapid);
	unsigned long GetMapID();

	void SetUnionID(unsigned long UnionID);
	unsigned long GetUnionID();

	void SetChatRoom(unsigned long RoomID);
	unsigned long GetChatRoom();

	void SetAddHandle(int handle);
	int GetAddHandle();

	void SetUserName(char * name);
	char* GetPlayerName();
	unsigned long GetUserID(){return m_ulUserID;}

	void SetCountry(int country){m_country = country;}
	int  GetContry(){return m_country ;}

	void SetUserLevel(int level){m_Level = level;}
	int  GetUserLevel(){return m_Level;}

	void SetUserMilitaryRank(int rank){m_Military_rank = rank;}
	int  GetUserMilitaryRank(){return m_Military_rank;}
private:
	unsigned long m_ulUserID; //
	unsigned long m_MapID; // ���ڿ���ͼ
	unsigned long m_ulUnionID; //���ڹ�����
	unsigned long m_ChatRoomID;//���������ұ��
	unsigned short m_country ;
	char cUserName[16];
	int m_Level;	//��ҵȼ�
	int m_Military_rank;//��Ҿ���
	int m_handle;		
};
typedef IceUtil::Handle<CChatUserInfo> CChatUserInfo_prt;
