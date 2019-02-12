#include "ConsortiaRanking.h"
#include <stdio.h>

CConsortiaRanking::CConsortiaRanking()
{
	RankingID = Level = Country = CreateTime =0;
	ConName[0] = 0;				//公会名称
	Chairman[0] = 0;				//会长名称
}
CConsortiaRanking::~CConsortiaRanking()
{
}
int CConsortiaRanking::SetRankingID(int id)
{
	return (RankingID = id);
}
void CConsortiaRanking::SetConName(int conid,const char *name)
{
	ConID = conid;
	if(name)
	{
		strncpy(ConName,name,SIZE_32);
	}
}
void CConsortiaRanking::SetChairman(int playerid,const char *name)
{
	PlayerID = playerid;
	if(name)
	{
		strncpy(Chairman,name,SIZE_24);
	}
}
int CConsortiaRanking::SetLevel(int level)
{
	return (Level = level);
}
int CConsortiaRanking::SetUserCount(int count)
{
	return (Count = count);
}
int CConsortiaRanking::SetCountry(int country)
{
	return (Country = country);
}
__int64 CConsortiaRanking::SetCreateTime(__int64 createtime)
{
	return (CreateTime = createtime);
}

int CConsortiaRanking::GetRankingID()
{
	return RankingID;
}
int CConsortiaRanking::GetConsortiaID()
{
	return ConID;
}
char* CConsortiaRanking::GetConName()
{
	return ConName;
}
int CConsortiaRanking::GetChairmanID()
{
	return PlayerID;
}
char* CConsortiaRanking::GetChairman()
{
	return Chairman;
}
int CConsortiaRanking::GetLevel()
{
	return Level;
}
int CConsortiaRanking::GetUserCount()
{
	return Count;
}
int CConsortiaRanking::GetCountry()
{
	return Country;
}
__int64 CConsortiaRanking::GetCreateTime()
{
	return CreateTime;
}

bool CConsortiaRanking::operator<(CConsortiaRanking &conranking)
{
	if(this->GetUserCount() < conranking.GetUserCount())
	{
		return true;
	}
	else if(this->GetUserCount() == conranking.GetUserCount())
	{
		if(this->GetLevel() < conranking.GetLevel())
		{
			return true;
		}
		else if(this->GetLevel() == conranking.GetLevel())
		{
			if(this->GetCreateTime() > conranking.GetCreateTime())
			{
				return true;
			}
		}
	}
	return false;
}