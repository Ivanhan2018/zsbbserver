// @brief 简要描述: 房间实体类  
// @author 作者: 黄 文 才

#include "EctypeRoom.h"


EctypeRoom::EctypeRoom(const char* szHostName, const char *szRoomName, const char *szPassWord, int mapid, unsigned int hostID, const char *szNotify) 
		: hostName_(szHostName), roomName_(szRoomName), roomPassword_(szPassWord), mapID_(mapid), roomHostID_(hostID), 
					   roomNotice_(szNotify), roomHasArmySum(0)
{

}

EctypeRoom::~EctypeRoom(void)
{
	PlayerId2BaseInfo::iterator itBeg = hero_.begin();
	PlayerId2BaseInfo::iterator itEnd = hero_.end();
	
	for (; itBeg!=itEnd; ++itBeg)
	{
		if (NULL != itBeg->second)
		{
			try
			{
				delete itBeg->second;
				itBeg->second = NULL;
			}
			catch (...)
			{
				;
			}
		}
	}
}

void EctypeRoom::addPlayer(unsigned int playerID, const char *szPlayerName, unsigned int levelParam, unsigned int rankParam, unsigned int faceId)
{
	try
	{
		hero_[playerID] = const_cast<const HeroStruct* >(new HeroStruct(szPlayerName, levelParam, rankParam, faceId));        ////////
	}
	catch (...)
	{
		;
	}

	memberReadyInfo_[playerID] = false;
}

void EctypeRoom::DeleteAllArmy(unsigned int playerID)
{
	PlayerId2MapId2ArmyIds::iterator it = memberTable_.find(playerID);

	if (it != memberTable_.end())
	{
		MapId2ArmyIds &mapId2ArmyIds = it->second;
		MapId2ArmyIds::iterator itBeg = mapId2ArmyIds.begin();
		MapId2ArmyIds::iterator itEnd = mapId2ArmyIds.end();

		for (; itBeg!=itEnd; ++itBeg)
		{
			roomHasArmySum -= static_cast<unsigned int>(itBeg->second.size());
		}

		memberTable_.clear();
	}
}

void EctypeRoom::kickMemberOrDelMem(unsigned int playerID)
{
	PlayerId2MapId2ArmyIds::iterator itPlayerId2ArmyIds = memberTable_.find(playerID);

	if (itPlayerId2ArmyIds != memberTable_.end())
	{
		MapId2ArmyIds &mapId2ArmyIds = itPlayerId2ArmyIds->second;
		MapId2ArmyIds::iterator itBeg = mapId2ArmyIds.begin();
		MapId2ArmyIds::iterator itEnd = mapId2ArmyIds.end();

		for (; itBeg!=itEnd; ++itBeg)
		{
			roomHasArmySum -= static_cast<unsigned int>(itBeg->second.size());
		}
		
		memberTable_.erase(playerID);
	}

	hero_.erase(playerID);
	memberReadyInfo_.erase(playerID);
}

void EctypeRoom::addArmy(unsigned int playerID, unsigned int countryId, unsigned int armyID)
{
	roomHasArmySum++;
	PlayerId2MapId2ArmyIds::iterator it = memberTable_.find(playerID);

	if (it != memberTable_.end())
	{
		MapId2ArmyIds &mapId2ArmyIds = it->second;
		
		mapId2ArmyIds[countryId].insert(armyID);
	}
	else
	{
		memberTable_[playerID][countryId].insert(armyID);
	}

}