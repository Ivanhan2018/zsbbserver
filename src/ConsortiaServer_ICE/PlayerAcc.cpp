/*实现玩家类*/
#include "PlayerAcc.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

CPlayerAcc::CPlayerAcc()
{
	PlayerID = 0;
	Country = 0;
	NickName[0] = 0;
	Agent = 0;
	Online = false;
}

CPlayerAcc::CPlayerAcc(int id,const char* name,int country,bool isonline,int agent)
{
	PlayerID = id;
	if(name)
	{
		strncpy(NickName,name,NAME_LEN);
	}
	NickName[NAME_LEN] = 0;
	Country = country;
	Online = isonline;
	Agent = agent;
}

CPlayerAcc::~CPlayerAcc()
{
}
/**************************************************************************
*说明：设置玩家信息
*输入参数：	
*返回值：0:成功；其他:失败
**************************************************************************/
int	CPlayerAcc::SetPlayer(int id,const char* name,int country)
{
	if(id<0)
		return -1;

	PlayerID = id;
	if(name)
	{
		strncpy(NickName,name,NAME_LEN);
	}
	Country = country;
	return 0;
}
/**************************************************************************
*说明：设置玩家ID
*输入参数：	
*返回值：>=0:玩家ID；<0:失败
**************************************************************************/
int CPlayerAcc::SetPlayerID(int id)
{
	if(id<0)
		return id;

	return (PlayerID = id);
}
/**************************************************************************
*说明：设置玩家昵称
*输入参数：	
*返回值：玩家名称
**************************************************************************/
char* CPlayerAcc::SetName(const char *name)
{
	if(name)
	{
		strncpy(NickName,name,NAME_LEN);
	}
	return NickName;
}
/**************************************************************************
*说明：设置玩家阵营
*输入参数：	
*返回值：玩家当前阵营
**************************************************************************/
int CPlayerAcc::SetCountry(int country)
{
	return (Country = country);	 
}

short CPlayerAcc::SetOfficerRace(short offrace)
{
	return (OfficerRace = offrace);
}

bool CPlayerAcc::SetOnline(bool isonline)
{
	return (Online = isonline);
}
int CPlayerAcc::SetAgent(int agent)
{
	return (Agent = agent);
}
/**************************************************************************
*说明：获取玩家ID
*输入参数：	
*返回值：玩家ID
**************************************************************************/

int CPlayerAcc::GetPlayerID()
{
	return PlayerID;
}
/**************************************************************************
*说明：获取玩家昵称
*输入参数：	
*返回值：获取玩家昵称
**************************************************************************/
char* CPlayerAcc::GetName()
{
	return NickName;
}
/**************************************************************************
*说明：获取玩家阵营
*输入参数：	
*返回值：获取玩家阵营
**************************************************************************/
int CPlayerAcc::GetCountry()
{
	return Country;
}

short CPlayerAcc::GetOfficerRace()
{
	return OfficerRace;
}

bool CPlayerAcc::IsOnline()
{
	return Online;
}

int CPlayerAcc::GetAgent()
{
	return Agent;
}