#include "ConsortiaOfficial.h"
#include <string.h>
#pragma warning(disable:4100)
#pragma warning(disable:4244)
CConsortiaOfficial::CConsortiaOfficial()
:OfficialID(0),Purview(0),Level(0)
{
}

CConsortiaOfficial::CConsortiaOfficial(int id,const char *name,int purview,short level)
{
	OfficialID = id;
	if(name)
	{
		strcpy(OfficialName,name);
	}
	Purview = purview;
	Level = level;
}

CConsortiaOfficial::~CConsortiaOfficial()
{
}

int CConsortiaOfficial::SetOfficial(int id,const char *name,int purview,short level)
{
	OfficialID = 0;	
	Purview = 0;
	Level = 0;
	if(name)
	{
		strcpy(OfficialName,name);
	}
	return 0;
}

int CConsortiaOfficial::SetOfficialID(int id)
{
	return (OfficialID = id);
}

char* CConsortiaOfficial::SetOfficialName(const char *name)
{
	if(name)
	{
		strcpy(OfficialName,name);
	}
	return OfficialName;
}

int CConsortiaOfficial::SetPurview(int purview)
{
	return (Purview = purview);
}

short CConsortiaOfficial::SetLevel(int level)
{
	return (Level = level);
}


int CConsortiaOfficial::GetOfficialID()
{
	return OfficialID;
}

char* CConsortiaOfficial::GetOfficialName()
{
	return OfficialName;
}

int CConsortiaOfficial::GetPurview()
{
	return Purview;
}

short CConsortiaOfficial::GetLevel()
{
	return Level;
}
