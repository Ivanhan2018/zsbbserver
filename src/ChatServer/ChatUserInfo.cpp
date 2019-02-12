
#include ".\chatuserinfo.h"
#include "memory.h"
CChatUserInfo::CChatUserInfo(unsigned long ulUserId):m_ulUserID(ulUserId)
{
	m_ulUnionID =0;
}

CChatUserInfo::~CChatUserInfo(void)
{
}
void CChatUserInfo::SetAddHandle(int handle)
{
	m_handle = handle;
}
int CChatUserInfo::GetAddHandle()
{
	return m_handle ;
}

void CChatUserInfo::SetChatRoom(unsigned long RoomID)
{
	m_ChatRoomID = RoomID ;
}

unsigned long CChatUserInfo::GetChatRoom()
{
	return m_ChatRoomID ;
}

void CChatUserInfo::SetMapID(unsigned long mapid)
{
	m_MapID = mapid ;
}

unsigned long CChatUserInfo::GetMapID()
{
	return m_MapID ;
}

void CChatUserInfo::SetUnionID(unsigned long UnionID)
{
	m_ulUnionID = UnionID ;
}

unsigned long CChatUserInfo::GetUnionID()
{
	return m_ulUnionID ;
}

void CChatUserInfo::SetUserName(char * name)
{
	memcpy(cUserName,name,15);
	cUserName[15]='\0';
}

char* CChatUserInfo::GetPlayerName()

{
	return cUserName ;
}