
/*
聊天用户管理信息
*/
#pragma once
#include "ChatUserInfo.h"
#include <map>
#include <list>
#include <string>
using namespace std;
class CChatUserManager
{
public:
	CChatUserManager(void);
	~CChatUserManager(void);

	CChatUserInfo_prt AddNewChatUserInfo(unsigned long ulUserID);
	CChatUserInfo_prt GetChatUserInfo(unsigned long ulUserID);
	void DeleteChatUserInfo(unsigned long ulUserID);
	void EmptyChatUserList();	

	void AddUserName(string Name , unsigned long ulUserId);
	unsigned long GetUserID(string Name );

	void InitUserNameList();

	unsigned long  GetHeroID(string heroName);
	void AddMainHero(string strName,unsigned long userid);

	void AddFriendList(unsigned long userid,unsigned long friendid);
	unsigned long GetFriendList(unsigned long userid);
	void DeleteFriendList(unsigned long userid);

	void InitMapBelong();

public:
	map<unsigned long , CChatUserInfo_prt> m_ChatUserList ;
	map<string ,unsigned long >m_NameToIDList ; // 玩家名字到玩家编号
	map<unsigned long ,list<unsigned long> > m_mapUserIdList ; //地图对应玩家
	map<string , unsigned long> m_UserName_list; // 通过玩家名字找玩家编号
	map<unsigned long , unsigned long > m_AddFriend_List; //用于临时保存添加好友信息
	map<unsigned long , int> m_MapBelongList;//地图所属性列表

};
extern CChatUserManager* g_ChatUserManager;