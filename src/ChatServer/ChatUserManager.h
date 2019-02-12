
/*
�����û�������Ϣ
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
	map<string ,unsigned long >m_NameToIDList ; // ������ֵ���ұ��
	map<unsigned long ,list<unsigned long> > m_mapUserIdList ; //��ͼ��Ӧ���
	map<string , unsigned long> m_UserName_list; // ͨ�������������ұ��
	map<unsigned long , unsigned long > m_AddFriend_List; //������ʱ������Ӻ�����Ϣ
	map<unsigned long , int> m_MapBelongList;//��ͼ�������б�

};
extern CChatUserManager* g_ChatUserManager;