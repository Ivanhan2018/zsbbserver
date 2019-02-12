/*ʵ�������*/
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
*˵�������������Ϣ
*���������	
*����ֵ��0:�ɹ�������:ʧ��
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
*˵�����������ID
*���������	
*����ֵ��>=0:���ID��<0:ʧ��
**************************************************************************/
int CPlayerAcc::SetPlayerID(int id)
{
	if(id<0)
		return id;

	return (PlayerID = id);
}
/**************************************************************************
*˵������������ǳ�
*���������	
*����ֵ���������
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
*˵�������������Ӫ
*���������	
*����ֵ����ҵ�ǰ��Ӫ
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
*˵������ȡ���ID
*���������	
*����ֵ�����ID
**************************************************************************/

int CPlayerAcc::GetPlayerID()
{
	return PlayerID;
}
/**************************************************************************
*˵������ȡ����ǳ�
*���������	
*����ֵ����ȡ����ǳ�
**************************************************************************/
char* CPlayerAcc::GetName()
{
	return NickName;
}
/**************************************************************************
*˵������ȡ�����Ӫ
*���������	
*����ֵ����ȡ�����Ӫ
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