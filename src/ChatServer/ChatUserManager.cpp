#include "StdAfx.h"
#include ".\chatusermanager.h"

#include <string>
#include "Global_Macro.h"
#include "DB_Table.h"
#include "NetTask.h"
CChatUserManager* g_ChatUserManager ;

CChatUserManager::CChatUserManager(void)
{
}

CChatUserManager::~CChatUserManager(void)
{
	EmptyChatUserList();
}
CChatUserInfo_prt CChatUserManager::AddNewChatUserInfo(unsigned long ulUserID)
{
	if (ulUserID != 0)
	{
		CChatUserInfo_prt UserInfo = new CChatUserInfo(ulUserID);
		if (UserInfo)
		{
			m_ChatUserList[ulUserID] = UserInfo ;
		}

		return UserInfo;
	}
	else
	{
		return NULL ;
	}
	
}
CChatUserInfo_prt CChatUserManager::GetChatUserInfo(unsigned long ulUserID)
{
	map<unsigned long , CChatUserInfo_prt> ::iterator iter;
	iter = m_ChatUserList.find(ulUserID);
	if (iter != m_ChatUserList.end())
	{

		return iter->second ;
	}
	else
	{
		return NULL ;
	}

}

void CChatUserManager::DeleteChatUserInfo(unsigned long ulUserID)
{
	m_ChatUserList.erase(ulUserID);
}

void CChatUserManager::AddUserName(string Name , unsigned long ulUserId)
{
	IceUtil::Mutex m;
	m.lock();
	m_NameToIDList[Name] = ulUserId;
	m.unlock();
}

unsigned long CChatUserManager::GetUserID(string Name )
{
	map<string , unsigned long > ::iterator iter;
	iter = m_NameToIDList.find(Name);
	if (iter!=m_NameToIDList.end())
	{
		return (*iter).second;
	}
	else
	{
		return 0 ;
	}
}

void CChatUserManager::EmptyChatUserList()
{
	CChatUserInfo_prt UserInfo = NULL ;
	map<unsigned long , CChatUserInfo_prt >::iterator iter;
	for (iter = m_ChatUserList.begin(); iter != m_ChatUserList.end() ; iter ++ )
	{
		UserInfo = iter->second ;
		//m_ChatUserList.erase(iter);
		if (UserInfo)
		{
			//delete UserInfo ;
			UserInfo = 0 ;
		}
	}
	if (!m_ChatUserList.empty())
	{
		m_ChatUserList.clear();
	}

	if (!m_NameToIDList.empty())
	{
		m_NameToIDList.clear();
	}
}

void CChatUserManager::InitUserNameList()
{
	//string str;
	//char name[SIZE_24];
	char* p;
	int len ;
	int tmplen = 0 ;
	PlayerOfficerTable Officertable;

	g_NetTask.proxy.db_select_all(Officertable,p,len);
	if (len>0)
	{
		while (tmplen+sizeof(PlayerOfficerTable)<=len)
		{
			memcpy(&Officertable,p+tmplen,sizeof(PlayerOfficerTable));
			if (Officertable.playerid_ == Officertable.officerid_)
			{
				string str(Officertable.officername_);
				m_NameToIDList[str] = Officertable.playerid_;				
			}
			tmplen+= sizeof(PlayerOfficerTable);
		}
		delete [] p;
	}

}

unsigned long CChatUserManager::GetHeroID(string heroName)
{
	map<string , unsigned long >::iterator iter;
	if (m_NameToIDList.size()==0)
	{
		return 0;
	}
	iter = m_NameToIDList.find(heroName);
	unsigned long officerid = 0;
	if (iter!= m_NameToIDList.end())
	{
		officerid = (*iter).second;
	}

	return officerid;

}

void CChatUserManager::AddFriendList(unsigned long userid,unsigned long friendid)
{
	IceUtil::Mutex m;
	m.lock();
	m_AddFriend_List[userid] = friendid;
	m.unlock();
}

unsigned long CChatUserManager::GetFriendList(unsigned long userid)
{
	unsigned long friendid = 0;
	IceUtil::Mutex m;
	m.lock();
	map<unsigned long , unsigned long >::iterator iter;
	iter = m_AddFriend_List.find(userid);
	if (iter!=m_AddFriend_List.end())
	{
		friendid = (*iter).second;
	}
	m.unlock();
	return friendid;
}

void CChatUserManager::DeleteFriendList(unsigned long userid)
{
	IceUtil::Mutex m;
	m.lock();
	m_AddFriend_List.erase(userid);
	m.unlock();
}

void CChatUserManager::AddMainHero(string strName,unsigned long userid)
{
	m_NameToIDList[strName]= userid;
}

void CChatUserManager::InitMapBelong()
{
	MapInfoTable mapInfo;
	char* p;
	int len, tmplen =0;

	int ret = g_NetTask.proxy.db_select_all(mapInfo,p,len);
	if (len>0)
	{
		while (len>=tmplen+sizeof(MapInfoTable))
		{
			memcpy(&mapInfo,p+tmplen,sizeof(MapInfoTable));
			m_MapBelongList[mapInfo.mapid_] = mapInfo.countryid_;
			tmplen += sizeof(MapInfoTable);
		}
		delete [] p;
	}
}