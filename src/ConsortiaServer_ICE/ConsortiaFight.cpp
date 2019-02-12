#include "ConsortiaFight.h"

CConsortiaFight::CConsortiaFight()
:FightID(0),Consortia1ID(0),Consortia2ID(0),MapID(0),CityID(0),Time(0)
{
}

CConsortiaFight::CConsortiaFight(int id,int con1id,int con2id,int mapid,int cityid,__int64 time):
FightID(id),Consortia1ID(con1id),Consortia2ID(con2id),MapID(mapid),CityID(cityid),Time(time)
{
}

CConsortiaFight::~CConsortiaFight()
{
}

int CConsortiaFight::SetFight(int id,int con1id,int con2id,int mapid,int cityid,__int64 time)
{
	FightID = id;
	Consortia1ID = con1id;
	Consortia2ID = con2id;
	MapID = mapid;
	CityID = cityid;
	Time = time;
	return 0;
}

int CConsortiaFight::SetFightID(int id)
{
	return (FightID = id);
}

int CConsortiaFight::SetConsortia1ID(int consortiaid)
{
	return (Consortia1ID = consortiaid);
}

int CConsortiaFight::SetConsortia2ID(int consortiaid)
{
	return (Consortia2ID = consortiaid);
}

int CConsortiaFight::SetMapID(int mapid)
{
	return (MapID = mapid);
}

int CConsortiaFight::SetCityID(int cityid)
{
	return (CityID = cityid);
}

__int64 CConsortiaFight::SetTime(__int64 time)
{
	return (Time = time);
}

int CConsortiaFight::GetFightID()
{
	return FightID;
}
int CConsortiaFight::GetConsortia1ID()
{
	return Consortia1ID;
}

int CConsortiaFight::GetConsortia2ID()
{
	return Consortia2ID;
}

int CConsortiaFight::GetMapID()
{
	return MapID;
}

int CConsortiaFight::GetCityID()
{
	return CityID;
}

__int64 CConsortiaFight::GetTime()
{
	return Time;
}