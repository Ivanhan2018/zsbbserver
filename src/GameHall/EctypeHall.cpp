// @brief 简要描述: 副本大厅实体类  
// @author 作者: 黄 文 才

#include <list>
#include "socketsvc.hpp"
#include "EctypeHall.h"

enum
{
	TEN_THOUSANDE = 10000,
	ECTYPE_SERVER_ID = 1256,
	MAX_MAN_IN_ROOM = 5
};

EctypeHall::EctypeHall(CLoadDBInfo* dbInfo, DB_Proxy* dbPtr, PlayerID2Info &playId2InfoPtr)
: dbInfo_(dbInfo), dbPtr_(dbPtr), playerId2InfoPtr_(playId2InfoPtr)
{
	npcInfo_ = new CNPCInfo;

	npcInfo_->InitNpc(dbPtr_);
}

EctypeHall::~EctypeHall(void)
{
	Ectype2RoomInfo::iterator itEctypeBeg = roomTable_.begin();
	Ectype2RoomInfo::iterator itEctypeEnd = roomTable_.end();
	RoomID2Info::iterator itRoomBeg, itRoomEnd;

	for (; itEctypeBeg!=itEctypeEnd; ++itEctypeBeg)
	{
		RoomID2Info &roomIdInfo = itEctypeBeg->second;

		itRoomBeg = roomIdInfo.begin();

		itRoomEnd = roomIdInfo.end();

		for ( ; itRoomBeg!=itRoomEnd; ++itRoomBeg)
		{
			if (NULL != itRoomBeg->second)
			{
				try
				{
					delete itRoomBeg->second;
				}
				catch (...)
				{
					;
				}

			}
		}
	}

	EntranceId2CampIdRoomInfo::iterator itEntranceIdBeg = entranceId2CampIdRoomInfo_.begin();
	EntranceId2CampIdRoomInfo::iterator itEntranceIdEnd = entranceId2CampIdRoomInfo_.end();
	CampId2RoomInfo::iterator itCampId2RoomInfoBeg, itCampId2RoomInfoEnd;

	for (; itEntranceIdBeg!=itEntranceIdEnd; ++itEntranceIdBeg)
	{
		CampId2RoomInfo &campId2RoomInfo = itEntranceIdBeg->second;

		itCampId2RoomInfoBeg = campId2RoomInfo.begin();
		itCampId2RoomInfoEnd = campId2RoomInfo.end();

		for (; itCampId2RoomInfoBeg!=itCampId2RoomInfoEnd; ++itCampId2RoomInfoBeg)
		{
			RoomID2Info &roomIdInfo = itCampId2RoomInfoBeg->second;

			itRoomBeg = roomIdInfo.begin();

			itRoomEnd = roomIdInfo.end();

			for ( ; itRoomBeg!=itRoomEnd; ++itRoomBeg)
			{
				if (NULL != itRoomBeg->second)
				{
					try
					{
						delete itRoomBeg->second;
					}
					catch (...)
					{
						;
					}

				}
			}
		}
	}
}

bool EctypeHall::onEnterEctypeHall(const ReqHallInfo *msgPtr)
{
//	if (NULL == msgPtr)
//	{
//#ifdef _DEBUG
//		assert(0);
//#endif
//		return false;
//	}

	unsigned int ectypeID = getEctypeID(msgPtr->EctypeID);

	if (0 == ectypeID)
	{
		return false;
	}

	unsigned int playerId = msgPtr->plyaerID;		
	
	PlayerBaseInfo playerBaseInfo = getPlayerBaseInfo(playerId);

	if (0 == playerBaseInfo.playerId)
	{
		return false;
	}

	PlayerID2Info::iterator itFind = playerId2InfoPtr_.find(playerId);

	// 不在副本或战场等中
	if (playerId2InfoPtr_.end() == itFind)
	{
		PlayerInfoPtr playerInfoPtr(new PlayerInfo);

		if (!playerInfoPtr)
		{
			return false;
		}

		playerInfoPtr->playerId = playerId;
		playerInfoPtr->agent = msgPtr->agent;
		playerInfoPtr->playerName = playerBaseInfo.playerName;
		playerInfoPtr->faceID = playerBaseInfo.faceID;
		playerInfoPtr->level = playerBaseInfo.level;
		playerInfoPtr->rank = playerBaseInfo.rank;
		playerInfoPtr->campId = playerBaseInfo.campId;

		playerId2InfoPtr_[playerId] = playerInfoPtr;
	}
	else
	{	
		PlayerInfoPtr playerInfoPtr = itFind->second;
		playerInfoPtr->agent = msgPtr->agent;
		playerInfoPtr->playerName = playerBaseInfo.playerName;
		playerInfoPtr->faceID = playerBaseInfo.faceID;
		playerInfoPtr->level = playerBaseInfo.level;
		playerInfoPtr->rank = playerBaseInfo.rank;
		playerInfoPtr->campId = playerBaseInfo.campId;
	}		

	Notify_Is_In_Ectype tevent;
	tevent.agent = msgPtr->agent;
	tevent.from = msgPtr->from;
	tevent.to = msgPtr->to;
	tevent.playerID = msgPtr->plyaerID;
	tevent.uEctype  = ectypeID;
	tevent.uFromMapID = msgPtr->to;
	tevent.entranceID = msgPtr->EctypeID;
	
	tevent.serialize(sizeof(Notify_Is_In_Ectype));
	event_notify(ECTYPE_SERVER_ID, (char*)&tevent, sizeof(Notify_Is_In_Ectype));      // 发给副本服务器

	return true;
}

bool EctypeHall::onEnterEctypeHallFromArmyList(const ReqHallInfoFromArmyList *msgPtr)
{
	//	if (NULL == msgPtr)
	//	{
	//#ifdef _DEBUG
	//		assert(0);
	//#endif
	//		return false;
	//	}

	unsigned int armyId = msgPtr->armyId;
	ArmyId2EctypeId::iterator it = armyId2EctypeId_.find(armyId);

	unsigned int entranceId = 0;
	ArmyId2EntranceId::iterator itArmy2Entrance = armyId2EntranceId.find(armyId);

	if (armyId2EntranceId.end() != itArmy2Entrance)
	{
		entranceId = itArmy2Entrance->second;
	}

	if (armyId2EctypeId_.end() == it)
	{
		printf("armyid %d is not in ectype", msgPtr->armyId);
		//assert(0);
		return false;
	}

	unsigned int ectypeID = it->second;

	if (0 == ectypeID)
	{
		return false;
	}

	unsigned int playerId = msgPtr->plyaerID;		

	PlayerBaseInfo playerBaseInfo = getPlayerBaseInfo(playerId);

	if (0 == playerBaseInfo.playerId)
	{
		return false;
	}

	PlayerID2Info::iterator itFind = playerId2InfoPtr_.find(playerId);

	// 不在副本或战场等中
	if (playerId2InfoPtr_.end() == itFind)
	{
		PlayerInfoPtr playerInfoPtr(new PlayerInfo);

		if (!playerInfoPtr)
		{
			return false;
		}

		playerInfoPtr->playerId = playerId;
		playerInfoPtr->agent = msgPtr->agent;
		playerInfoPtr->playerName = playerBaseInfo.playerName;
		playerInfoPtr->faceID = playerBaseInfo.faceID;
		playerInfoPtr->level = playerBaseInfo.level;
		playerInfoPtr->rank = playerBaseInfo.rank;
		playerInfoPtr->campId = playerBaseInfo.campId;

		playerId2InfoPtr_[playerId] = playerInfoPtr;
	}
	else
	{	
		PlayerInfoPtr playerInfoPtr = itFind->second;
		playerInfoPtr->agent = msgPtr->agent;
		playerInfoPtr->playerName = playerBaseInfo.playerName;
		playerInfoPtr->faceID = playerBaseInfo.faceID;
		playerInfoPtr->level = playerBaseInfo.level;
		playerInfoPtr->rank = playerBaseInfo.rank;
	}		

	Notify_Is_In_Ectype tevent;
	tevent.agent = msgPtr->agent;
	tevent.from = msgPtr->from;
	tevent.to = msgPtr->to;
	tevent.playerID = msgPtr->plyaerID;
	tevent.uEctype  = ectypeID;
	tevent.uFromMapID = msgPtr->to;
	tevent.entranceID = entranceId;
	tevent.serialize(sizeof(Notify_Is_In_Ectype));

	event_notify(ECTYPE_SERVER_ID, (char*)&tevent, sizeof(Notify_Is_In_Ectype));      // 发给副本服务器

	return true;
}


bool EctypeHall::eventBackIsInEctypeInfo(const Notify_Back_Is_In_Ectype *Event)
{
	//if (NULL == Event)
	//{
	//	assert(0);
	//	return false;
	//}

	unsigned int playerId = Event->playerID;
	PlayerID2Info::iterator itFind = playerId2InfoPtr_.find(playerId);

	if (playerId2InfoPtr_.end() == itFind)
	{
		//assert(0);
		return false;
	}

	PlayerInfoPtr playerInfoPtr = itFind->second;

	unsigned int entranceID = Event->entranceID;

	unsigned int ectypeID = Event->uEctypID;
	
	EctypeInfoTable *ectypeInfo = npcInfo_->getEctypeInfoTable(entranceID);

	if (NULL == ectypeInfo)
	{
		return false;
	}

	// 战场
	if (LIMIT_BATTLEFIELD == ectypeInfo->warType)
	{
		entranceId2CampIdPlayerIds_[entranceID][playerInfoPtr->campId].insert(playerId);
	}
	// 副本
	else
	{
		playerID2EctypeId_[Event->playerID] = ectypeID;
	}

	//EctypeBaseTable ectypetable;
	//ectypetable.setid(ectypeID);

	//if (NULL == dbPtr_)
	//{
	//	return false;
	//}

	//int ret = dbPtr_->db_select(ectypetable);

	//if(ret != DB_Proxy::DB_SUCCEED)
	//{
	//	return false;
	//}

	// 判断是否是单人副本
	bool isSingleEctype = isSingleEctypeFun(entranceID);

	// 在副本中
	if (Event->ret)
	{
		playerInfoPtr->gameHallType = COUNTRYPART;

		// 通知该玩家可以进入副本房间
		RspEnterEctypeSystem Rsp;
		memset(&Rsp, 0, sizeof(RspEnterEctypeSystem));

		Rsp.isSigleEctype = isSingleEctype;
		Rsp.EctypeID = Event->uEctypID;
		Rsp.flag = 1;
		Rsp.EcServerID = ECTYPE_SERVER_ID;
		Rsp.EctypeRoomID = Event->uEctypID;
		Rsp.NpcRoomID = Event->roomid;
		Rsp.Ectypemapid = Event->uEctypID;	

		Rsp.serialize(sizeof(RspEnterEctypeSystem), playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, Event->playerID, RSP_ENTER_ECTYPE_SYSTEM_INFO);
		asynch_write(Rsp.agent,(char*)&Rsp,Rsp.length);

		return true;
	}
    // 不在副本中
	else
	{
		map<int , EctypeInfoTable *> &ecytpeInfo = npcInfo_->ectypeInfos_;
		map<int , EctypeInfoTable *>::iterator itTmp = ecytpeInfo.find(entranceID);

		if (ecytpeInfo.end() != itTmp)
		{
			EctypeInfoTable *ectypeInfoTable = itTmp->second;

			if (NULL != ectypeInfoTable)
			{
				if (1 == ectypeInfoTable->warType)
				{
					return true;
				}
			}
		}

		// 单人副本的处理
		if (isSingleEctype)
		{
			if (!isInEctypeRoom(Event->playerID, true))   // 如果不在副本中就创建一个房间添加到大厅中
			{	
				unsigned long playerID = Event->playerID;
				unsigned int ectypeRoomID = this->getRoomID(ectypeID);  // 生成一个房间的ID

				string &playerName = playerInfoPtr->playerName;

				EctypeRoom* roomPtr = new EctypeRoom(playerName.c_str(), "", "", Event->uEctypID, Event->playerID, ""); 
				
				if (NULL == roomPtr)
				{
					return false;
				}

				roomPtr->setEctypeID(ectypeID);
				roomPtr->setRoomID(ectypeRoomID);

				bool isSucess = addRoomInEctype(ectypeID, ectypeRoomID, roomPtr);
				
				if (!isSucess)
				{
					//assert(0);
					return false;
				}

				// 添加玩家信息到房间
				roomPtr->addPlayer(Event->playerID, playerName.c_str(), playerInfoPtr->level, playerInfoPtr->rank, playerInfoPtr->faceID); 

				// 发送RspSingleEctypeInfo给客户端
				RspSingleEctypeInfo rspSingleEctypeInfo;
				rspSingleEctypeInfo.flag = true;
				rspSingleEctypeInfo.uEctypeRoomID = ectypeRoomID;

				rspSingleEctypeInfo.serialize(sizeof(RspSingleEctypeInfo), playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, Event->playerID, RSP_BACK_SINGLE_ECTYPE_ROOM_INFO);
				asynch_write(rspSingleEctypeInfo.agent,(char*)&rspSingleEctypeInfo, rspSingleEctypeInfo.length);

				return true;
			}
		}
		// 多人副本的处理
		else
		{
			Ectype2RoomInfo::iterator itFind = roomTable_.find(ectypeID);

			// 以下代码用于防止客户端问题导致。。。。。。
			if (itFind != roomTable_.end())
			{
				RoomID2Info &roomSet = itFind->second;
				RoomID2Info::iterator itRoomBeg = roomSet.begin();
				RoomID2Info::iterator itRoomEnd = roomSet.end();

				for (; itRoomBeg != itRoomEnd; ) 
				{
					EctypeRoom* roomPtr = itRoomBeg->second;

					if (NULL == roomPtr)
					{
//#ifdef _DEBUG
//						assert(0);
//#endif

						++itRoomBeg;

						continue;
					}

					if (roomPtr->getRoomHostID() == Event->playerID)
					{
						if (NULL != itRoomBeg->second)
						{
							delete itRoomBeg->second;
						}

						itRoomBeg = roomSet.erase(itRoomBeg);
					}
					else
					{
						++itRoomBeg;
					}
				}
			}

			return rspEnterEctypeHall(Event->playerID, Event->uEctypID, entranceID);
		}
	}

	return true;
}

bool EctypeHall::rspEnterEctypeHall(unsigned long playerId, unsigned int ectypeID, unsigned int entranceID)
{
	PlayerID2Info::iterator itFind = playerId2InfoPtr_.find(playerId);

	if (playerId2InfoPtr_.end() == itFind)
	{
		//assert(0);
		return false;
	}

	PlayerInfoPtr playerInfoPtr = itFind->second;

	unsigned int sizeCopy = 0;
	
	RspHallInfo rspHallInfo;
	memset(&rspHallInfo, 0, sizeof(RspHallInfo));

	if (0 == ectypeID)
	{
		rspHallInfo.flag = false;
		
		rspHallInfo.serialize(sizeof(DISTRIBUTED_HEADER) + sizeof(int) + sizeof(bool), playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, playerId, RSP_ENTER_HALL_INFO);
		asynch_write(playerInfoPtr->agent,(char*)&rspHallInfo, rspHallInfo.length);

		return false; 
	}

	rspHallInfo.flag = true;

	EctypeInfoTable * ectypeInfo = npcInfo_->getEctypeInfoTable(entranceID);

	if (NULL == ectypeInfo)
	{
		return false;
	}

	// 战场
	if (LIMIT_BATTLEFIELD == ectypeInfo->warType)
	{
		EntranceId2CampIdRoomInfo ::iterator itFind = entranceId2CampIdRoomInfo_.find(entranceID);

		if (entranceId2CampIdRoomInfo_.end() != itFind)
		{
			CampId2RoomInfo &campId2RoomInfo = itFind->second;

			CampId2RoomInfo::iterator itCampId2Room = campId2RoomInfo.find(playerInfoPtr->campId);
			
			if (campId2RoomInfo.end() != itCampId2Room)
			{
				GetEntranceIdRoomInfosToClient(itCampId2Room->second, rspHallInfo, playerInfoPtr);
				return true;
			}
		}
	}
	// 副本
	else
	{
		Ectype2RoomInfo::iterator itEctype2RoomInfo = roomTable_.find(ectypeID);
		
		if(itEctype2RoomInfo != roomTable_.end())
		{
			GetEntranceIdRoomInfosToClient(itEctype2RoomInfo->second, rspHallInfo, playerInfoPtr);
			return true;
		}
	}

	// 找到0个房间
	rspHallInfo.iRoomCount = 0;
	rspHallInfo.serialize(sizeof(DISTRIBUTED_HEADER)+sizeof(int)+sizeof(bool), playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, playerId, RSP_ENTER_HALL_INFO);
	
	asynch_write(playerInfoPtr->agent, (char*)&rspHallInfo, rspHallInfo.length);

	return true;
}

void EctypeHall::GetEntranceIdRoomInfosToClient(const RoomID2Info &roomInfos, RspHallInfo &rspHallInfo, PlayerInfoPtr playerInfoPtr)
{
	EctypeRoom* roomPtr = NULL;
	unsigned int index = 0;

	RoomID2Info::const_iterator itRoomBeg = roomInfos.begin();
	RoomID2Info::const_iterator itRoomEnd = roomInfos.end();

	for (; itRoomBeg!=itRoomEnd; ++itRoomBeg)
	{
		roomPtr = itRoomBeg->second;

		if (NULL == roomPtr)
		{
			//assert(0);
			continue;
		}

		// 给结构体中的每个房间赋值
		rspHallInfo.Info[index].uEctypeRoomID = roomPtr->getRoomID();
		const string &roomName = roomPtr->getRoomName();
		const string &hostName = roomPtr->getHostName();

		memset(rspHallInfo.Info[index].cName, 0, sizeof(rspHallInfo.Info[index].cName));

		strncpy(rspHallInfo.Info[index].cName, roomName.c_str(), roomName.length()  > SIZE_24 - 1 ? SIZE_24 - 1 : roomName.length());

		rspHallInfo.Info[index].count = roomPtr->getHeroNum();

		memset(rspHallInfo.Info[index].czCreateName, 0, sizeof(rspHallInfo.Info[index].czCreateName));	

		strncpy(rspHallInfo.Info[index].czCreateName, hostName.c_str(), hostName.length()  > SIZE_24 - 1 ? SIZE_24 - 1 : hostName.length());

		if((roomPtr->getPassword()).empty())
		{
			rspHallInfo.Info[index].cPasswordflag = 0;
		}
		else
		{
			rspHallInfo.Info[index].cPasswordflag = 1;
		}

		if (index >= SEND_MAX_ROOM_INFO)
		{
			rspHallInfo.iRoomCount = index;

			rspHallInfo.serialize(sizeof(rspHallInfo), playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, playerInfoPtr->playerId, RSP_ENTER_HALL_INFO);
			asynch_write(playerInfoPtr->agent, (char*)&rspHallInfo, rspHallInfo.length);
			index = 0; // 如果超出最大副本房间数的话，index表示为0
			break;  
		}

		index++;
	}

	if (index > 0)
	{
		rspHallInfo.iRoomCount = index;

		rspHallInfo.serialize(sizeof(DISTRIBUTED_HEADER)+sizeof(int)+sizeof(bool)+sizeof(HallInfo)*index, playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, playerInfoPtr->playerId, RSP_ENTER_HALL_INFO);
		asynch_write(playerInfoPtr->agent, (char*)&rspHallInfo, rspHallInfo.length);
	}
	// 如果index为0则超过了最大副本数，什么都不做，也将房间数置为0以告诉客户端
	else
	{
		rspHallInfo.iRoomCount = 0;

		rspHallInfo.serialize(sizeof(DISTRIBUTED_HEADER)+sizeof(int)+sizeof(bool), playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, playerInfoPtr->playerId, RSP_ENTER_HALL_INFO);
		asynch_write(playerInfoPtr->agent, (char*)&rspHallInfo, rspHallInfo.length);
	}
}

bool EctypeHall::onCreateEctypeRoom(const ReqCreateEctypeRoom *msg)
{
	if (msg == NULL || NULL == dbPtr_)
	{
		return false;
	}

	unsigned int entranceId = msg->DoorID;

	unsigned int ectypeID = getEctypeID(entranceId);

	if (0 == ectypeID)
	{
		return false;
	}

	unsigned long playerId = msg->ulPlayerID;
	PlayerID2Info::iterator itFind = playerId2InfoPtr_.find(playerId);

	if (playerId2InfoPtr_.end() == itFind)
	{
		//assert(0);
		//return false;
		PlayerBaseInfo playerBaseInfo = getPlayerBaseInfo(playerId);

		if (0 == playerBaseInfo.playerId)
		{
			return false;
		}

		PlayerInfoPtr playerInfoPtr(new PlayerInfo);

		if (!playerInfoPtr)
		{
			return false;
		}

		playerInfoPtr->playerId = playerId;
		playerInfoPtr->agent = msg->agent;
		playerInfoPtr->playerName = playerBaseInfo.playerName;
		playerInfoPtr->faceID = playerBaseInfo.faceID;
		playerInfoPtr->level = playerBaseInfo.level;
		playerInfoPtr->rank = playerBaseInfo.rank;
		playerInfoPtr->campId = playerBaseInfo.campId;

		playerId2InfoPtr_[playerId] = playerInfoPtr;

		itFind = playerId2InfoPtr_.find(playerId);
	}

	PlayerInfoPtr playerInfoPtr = itFind->second;

	string &playerName = playerInfoPtr->playerName;

	EctypeInfoTable *ectypeInfo = npcInfo_->getEctypeInfoTable(entranceId);

	if (NULL == ectypeInfo)
	{
		return false;
	}

	RspCreateEctypeRoom rsp;
	rsp.playerID = msg->ulPlayerID;

	if (LIMIT_BATTLEFIELD == ectypeInfo->warType)
	{
		rsp.iEctypeRoomID = this->getBattlefieldRoomID(entranceId);  // 生成一个房间的ID
	}
	else
	{
		rsp.iEctypeRoomID = this->getRoomID(ectypeID);  // 生成一个房间的ID
	}


	memset(rsp.czCreateName, 0, sizeof(rsp.czCreateName));

	strncpy(rsp.czCreateName, playerName.c_str(), playerName.length() > SIZE_16 - 1 ? SIZE_16 - 1 : playerName.length());  // 将房间的名称默认为创建房间的玩家名

	// 没有密码将密码标志置为0
	if (0 == strcmp(msg->szPassWord, "")) 
	{
		rsp.cPasswordflag = 0;
	}
	// 有密码将密码标志置为1
	else
	{
		rsp.cPasswordflag = 1;
	}

	// 如果不在副本中就创建一个房间添加到大厅中
	if (!isInEctypeRoom(msg->ulPlayerID))   
	{	
		char* name = rsp.czCreateName;

		EctypeRoom* roomPtr = NULL;

		roomPtr = new EctypeRoom(name, msg->szEctypeName, msg->szPassWord, ectypeID, msg->ulPlayerID, msg->szNotify); 
		if (roomPtr == NULL)
		{
			return false;
		}
		roomPtr->setEctypeID(ectypeID);
		roomPtr->setRoomID(rsp.iEctypeRoomID);

		memset(rsp.cEctypeRoomName, 0, sizeof(rsp.cEctypeRoomName));

		memcpy(rsp.cEctypeRoomName, msg->szEctypeName, SIZE_16 - 1 );
		rsp.cEctypeRoomName[SIZE_16 - 1] = '\0';
		
		roomPtr->addPlayer(msg->ulPlayerID, rsp.czCreateName, playerInfoPtr->level, playerInfoPtr->rank, playerInfoPtr->faceID);//否则添加玩家信息到房间
		
		// 战场
		if (LIMIT_BATTLEFIELD == ectypeInfo->warType)
		{
			unsigned int campId = playerInfoPtr->campId;
			
			addRoomInBattlefield(entranceId, campId, rsp.iEctypeRoomID, roomPtr);

			// 广播给玩家
			EntranceId2CampIdPlayerIds::iterator itEntranceId2CampId = entranceId2CampIdPlayerIds_.find(entranceId);

			if (entranceId2CampIdPlayerIds_.end() != itEntranceId2CampId)
			{
				 CampId2PlayerIds &campId2PlayerIds = itEntranceId2CampId->second;

				 CampId2PlayerIds::iterator itCampId2PlayerIds = campId2PlayerIds.find(campId);

				 if (campId2PlayerIds.end() != itCampId2PlayerIds)
				 {
					std::set<unsigned int> &playerIdSet = itCampId2PlayerIds->second;
					std::set<unsigned int>::iterator itPlayerIdSetBeg = playerIdSet.begin();
					std::set<unsigned int>::iterator itPlayerIdSetEnd = playerIdSet.end();
					PlayerID2Info::iterator itFindTmp;

					for (; itPlayerIdSetBeg!=itPlayerIdSetEnd; ++itPlayerIdSetBeg)
					{
						itFindTmp = playerId2InfoPtr_.find(*itPlayerIdSetBeg);

						if (playerId2InfoPtr_.end() == itFindTmp)
						{
							//assert(0);
							continue;
						}

						PlayerInfoPtr playerInfoPtrTmp = itFindTmp->second;

						rsp.serialize(sizeof(rsp), playerInfoPtrTmp->agent, msg->to, *itPlayerIdSetBeg, RSP_CREATE_ROOM_INFO);
						asynch_write(playerInfoPtrTmp->agent,(char*)&rsp, sizeof(rsp));
					}
				 }
			}
		}
		// 副本
		else
		{
			addRoomInEctype(ectypeID, rsp.iEctypeRoomID, roomPtr);		
			
			// 创建好房间后，广播给所有在副本中的玩家
			PlayerID2EctypeId::iterator itBeg = playerID2EctypeId_.begin();
			PlayerID2EctypeId::iterator itEnd = playerID2EctypeId_.end();
			PlayerID2Info::iterator itFindTmp;

			for (; itBeg!=itEnd; ++itBeg)
			{
				if (itBeg->second != ectypeID)
				{
					continue;
				}

				itFindTmp = playerId2InfoPtr_.find(itBeg->first);

				if (playerId2InfoPtr_.end() == itFindTmp)
				{
					//assert(0);
					continue;
				}

				PlayerInfoPtr playerInfoPtrTmp = itFindTmp->second;

				rsp.serialize(sizeof(rsp), playerInfoPtrTmp->agent, msg->to, itBeg->first, RSP_CREATE_ROOM_INFO);
				asynch_write(playerInfoPtrTmp->agent,(char*)&rsp, sizeof(rsp));
			}
		}

		return true;
	}

	return false;
}

bool EctypeHall::onAddToEctypeRoom(const ReqAddToEctypeRoom *msg)
{
	//if (msg == NULL)
	//{	
	//	return false;
	//}

	unsigned int ectypeID = getEctypeID(msg->uEctypeID);

	if (0 == ectypeID )
	{
		return false;
	}

	unsigned long playerID = msg->ulPlayerID;
	PlayerID2Info::iterator itFind = playerId2InfoPtr_.find(playerID);

	if (playerId2InfoPtr_.end() == itFind)
	{
		//assert(0);
		//return false;
		PlayerBaseInfo playerBaseInfo = getPlayerBaseInfo(playerID);

		if (0 == playerBaseInfo.playerId)
		{
			return false;
		}

		PlayerInfoPtr playerInfoPtr(new PlayerInfo);

		if (!playerInfoPtr)
		{
			return false;
		}

		playerInfoPtr->playerId = playerID;
		playerInfoPtr->agent = msg->agent;
		playerInfoPtr->playerName = playerBaseInfo.playerName;
		playerInfoPtr->faceID = playerBaseInfo.faceID;
		playerInfoPtr->level = playerBaseInfo.level;
		playerInfoPtr->rank = playerBaseInfo.rank;

		playerId2InfoPtr_[playerID] = playerInfoPtr;

		itFind = playerId2InfoPtr_.find(playerID);
	}

	PlayerInfoPtr playerInfoPtr = itFind->second;

	RspAddToEctypeRoom rsp;
	memset(&rsp, 0, sizeof(RspAddToEctypeRoom));

	rsp.ulPlayerID = msg->ulPlayerID;

	Room2IsReady::iterator itRoom2Waiting = room2IsWaiting.find(msg->ulEctypeRoomID);

	if (room2IsWaiting.end() != itRoom2Waiting)
	{
		if (itRoom2Waiting->second)
		{
			rsp.flag = 3;	 // 此房间已进入战场等待状态

			rsp.serialize(sizeof(rsp), playerInfoPtr->agent, msg->to, msg->ulPlayerID, RSP_ADD_ECTYPE_ROOM_INFO);
			asynch_write(playerInfoPtr->agent,(char*)&rsp,rsp.length);

			return true;
		}
	}

	unsigned int entranceID = msg->uEctypeID;

	EctypeRoom *roomPtr = NULL;

	EctypeInfoTable *ectypeInfo = npcInfo_->getEctypeInfoTable(entranceID);

	if (NULL == ectypeInfo)
	{
		return false;
	}

	// 战场
	if (LIMIT_BATTLEFIELD == ectypeInfo->warType)
	{
		roomPtr = getSpecifRoom(entranceID, playerInfoPtr, msg->ulEctypeRoomID);
	}
	// 副本
	else
	{
		unsigned int ectypeID = getEctypeID(msg->uEctypeID);

		if (0 == ectypeID)
		{
			//assert(0);
			return false;
		}

		roomPtr = getRoom(ectypeID, msg->ulEctypeRoomID);
	}

	if (roomPtr == NULL)
	{
		/*assert(0);*/
		return false;
	}
	
	if (roomPtr == NULL)
	{
		rsp.flag = 0;	// 失败	
		
		rsp.serialize(sizeof(rsp), playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, msg->ulPlayerID, RSP_ADD_ECTYPE_ROOM_INFO);
		asynch_write(playerInfoPtr->agent, (char*)&rsp, rsp.length);
		return false;
	}

	if(strcmp((roomPtr->getPassword()).c_str(), msg->szPassWord) != 0)
	{
		rsp.flag = 4; // 密码错误
		
		rsp.serialize(sizeof(rsp), playerInfoPtr->agent, msg->to, msg->ulPlayerID, RSP_ADD_ECTYPE_ROOM_INFO);
		asynch_write(playerInfoPtr->agent, (char*)&rsp, rsp.length);
		return false;
	}

	if(roomPtr->getHeroNum() >= MAX_MAN_IN_ROOM)
	{
		rsp.flag = 5; //成员已达上限
		
		rsp.serialize(sizeof(rsp), playerInfoPtr->agent, msg->to, msg->ulPlayerID, RSP_ADD_ECTYPE_ROOM_INFO);
		asynch_write(playerInfoPtr->agent, (char*)&rsp,rsp.length);
		return false;
	}

	const PlayerId2BaseInfo &hero_ = roomPtr->getPlayerHeroInfo();
	PlayerId2BaseInfo::const_iterator it = hero_.find(msg->ulPlayerID);

	if (it != hero_.end())
	{
		rsp.flag = 4;	// 自己已在此副本中		
		
		rsp.serialize(sizeof(rsp), playerInfoPtr->agent, msg->to, msg->ulPlayerID, RSP_ADD_ECTYPE_ROOM_INFO);
		asynch_write(playerInfoPtr->agent,(char*)&rsp,rsp.length);
		return 0 ;
	}
	
	roomPtr->addPlayer(msg->ulPlayerID, (playerInfoPtr->playerName).c_str(), playerInfoPtr->level, playerInfoPtr->rank, playerInfoPtr->faceID);

	rsp.flag = 1; // 添加到房间成功

	rsp.faceID = playerInfoPtr->faceID;  

	rsp.uEctypeRoomID = msg->ulEctypeRoomID;

	string &playerName = playerInfoPtr->playerName;

	memset(rsp.Name, 0, SIZE_24);

	strncpy(rsp.Name, playerName.c_str(), playerName.length() > SIZE_24 - 1 ? SIZE_24 - 1 : playerName.length());

	rsp.iLevel = playerInfoPtr->level;
	rsp.iMilitaryRank = playerInfoPtr->rank;

	Ectype2RoomInfo::iterator itTmp = roomTable_.find((unsigned int)ectypeID);

	if (itTmp != roomTable_.end())
	{
		RoomID2Info &roomList = itTmp->second;
		RoomID2Info::iterator itRoomBeg = roomList.begin();
		RoomID2Info::iterator itRoomEnd = roomList.end();
		PlayerID2Info::iterator itFind;
		PlayerId2BaseInfo::const_iterator itPlayerId2BaseInfoBeg, itPlayerId2BaseInfoEnd;

		for (; itRoomBeg!=itRoomEnd; ++itRoomBeg)
		{	
			EctypeRoom *roomPtr = itRoomBeg->second;

			if (NULL == roomPtr)
			{
				//assert(0);
				continue;
			}

			const PlayerId2BaseInfo& playerInfo = roomPtr->getPlayerHeroInfo();

			itPlayerId2BaseInfoBeg = playerInfo.begin();
			itPlayerId2BaseInfoEnd = playerInfo.end();
			
			for (; itPlayerId2BaseInfoBeg!=itPlayerId2BaseInfoEnd; ++itPlayerId2BaseInfoBeg)
			{
				itFind = playerId2InfoPtr_.find(itPlayerId2BaseInfoBeg->first);

				if (playerId2InfoPtr_.end() == itFind)
				{
					//assert(0);
					continue;
				}

				PlayerInfoPtr playerInfoPtrTmp = itFind->second;

				rsp.serialize(sizeof(rsp), playerInfoPtrTmp->agent, msg->to, itPlayerId2BaseInfoBeg->first, RSP_ADD_ECTYPE_ROOM_INFO);
				asynch_write(playerInfoPtrTmp->agent, (char*)&rsp, sizeof(rsp));
			}
		}
	}

	ReqEnterEctypeRoom reqEnterEctypeRoom;
	reqEnterEctypeRoom.DoorID = msg->uEctypeID;
	reqEnterEctypeRoom.uEctypeRoomID = msg->ulEctypeRoomID;
	reqEnterEctypeRoom.ulPlayerID = msg->ulPlayerID;

	return rspEnterEctypeRoomInfo(&reqEnterEctypeRoom);
}

bool EctypeHall::rspEnterEctypeRoomInfo(ReqEnterEctypeRoom *msg)
{
	unsigned int entranceID = msg->DoorID;

	unsigned int ectypeID = getEctypeID(entranceID);

	if (0 == ectypeID)
	{
		return false;
	}

	PlayerID2Info::iterator itFind = playerId2InfoPtr_.find(msg->ulPlayerID);

	if (playerId2InfoPtr_.end() == itFind)
	{
		//assert(0);
		return false;
	}

	PlayerInfoPtr playerInfoPtr = itFind->second;

	unsigned int sizeCopy = 0;

	RspEnterEctypeRoom rsp;
	memset(&rsp, 0, sizeof(RspEnterEctypeRoom));

	EctypeRoom* roomPtr = NULL;

	EctypeInfoTable *ectypeInfo = npcInfo_->getEctypeInfoTable(entranceID);

	if (NULL == ectypeInfo)
	{
		return false;
	}

	// 战场
	if (LIMIT_BATTLEFIELD == ectypeInfo->warType)
	{
		roomPtr = getSpecifRoom(entranceID, playerInfoPtr, msg->uEctypeRoomID);
	}
	// 副本
	else
	{
		roomPtr = getRoom(ectypeID, msg->uEctypeRoomID);
	}

	if (roomPtr == NULL)
	{
		//assert(0);
		return false;
	}

	rsp.count = roomPtr->getHeroNum();

	unsigned int lenth = 0;

	const PlayerId2BaseInfo &playerId2PlayInfo = roomPtr->getPlayerHeroInfo();

	unsigned int roomHostId = roomPtr->getRoomHostID();

	PlayerId2BaseInfo::const_iterator itBeg = playerId2PlayInfo.begin();
	PlayerId2BaseInfo::const_iterator itEnd = playerId2PlayInfo.end();

	int i = 1, j = 0, indexTmp = 0;

	for (; itBeg!=itEnd; ++itBeg)
	{
		if (roomHostId == itBeg->first)
		{
			indexTmp = i;
			i = 0;
		}

		const HeroStruct *heroInfoPtr = itBeg->second;

		if (NULL == heroInfoPtr)
		{
			//assert(0);
			continue;
		}

		rsp.RoomPlayerInfo[i].cflag = roomPtr->getIsReady((*itBeg).first);
		
		const string &name = heroInfoPtr->name;
		unsigned int heroNameSize = static_cast<unsigned int>(sizeof((rsp.RoomPlayerInfo[i]).cHeroName));

		memset((rsp.RoomPlayerInfo[i]).cHeroName, 0, heroNameSize);

		strncpy((rsp.RoomPlayerInfo[i]).cHeroName, name.c_str(), name.length() > heroNameSize - 1 ? heroNameSize - 1 : name.length());

		const PlayerId2IsReady &playId2IsReady = roomPtr->getPlayerStateInfo();

		PlayerId2IsReady::const_iterator itFind = playId2IsReady.find((*itBeg).first);

		if (itFind != playId2IsReady.end() && itFind->second)
		{
			rsp.RoomPlayerInfo[i].faceid = 0;		
		}
		else
		{
			rsp.RoomPlayerInfo[i].faceid = heroInfoPtr->faceid;
		}

		rsp.RoomPlayerInfo[i].iLevel = heroInfoPtr->level;

		rsp.RoomPlayerInfo[i].iMilitrayRank = heroInfoPtr->rank;

		rsp.RoomPlayerInfo[i].playerid = (*itBeg).first;
		
		const PlayerId2MapId2ArmyIds &playerId2ArmyIdSet = roomPtr->getPlayerInfo();
		PlayerId2MapId2ArmyIds::const_iterator itTmp = playerId2ArmyIdSet.find((*itBeg).first);

		if (itTmp != playerId2ArmyIdSet.end())
		{
			const MapId2ArmyIds &mapId2ArmyIds = itTmp->second;

			MapId2ArmyIds::const_iterator itMap2ArmyIdsBeg = mapId2ArmyIds.begin();
			MapId2ArmyIds::const_iterator itMap2ArmyIdsEnd = mapId2ArmyIds.end();

			rsp.RoomPlayerInfo[i].count = 0;
			unsigned int armNameSize = 0;
			unsigned long playerIdTmp = 0;

			for (; itMap2ArmyIdsBeg!=itMap2ArmyIdsEnd; ++itMap2ArmyIdsBeg)
			{
				rsp.RoomPlayerInfo[i].count += static_cast<int>(itMap2ArmyIdsBeg->second.size());

				std::set<unsigned int>::const_iterator itArmyBeg = itMap2ArmyIdsBeg->second.begin();
				std::set<unsigned int>::const_iterator itArmyEnd = itMap2ArmyIdsBeg->second.end();

				IArmyPtr armyPtr = NULL;

				for (j = 0; itArmyBeg!=itArmyEnd; ++itArmyBeg, j++)
				{
					armNameSize = static_cast<unsigned int>(sizeof(rsp.RoomPlayerInfo[i].ArmyName[j]));
					memset(rsp.RoomPlayerInfo[i].ArmyName[j], 0, armNameSize);
					playerIdTmp = itTmp->first;

					PlayerID2Info::iterator itFind = playerId2InfoPtr_.find(playerIdTmp);

					if (playerId2InfoPtr_.end() == itFind)
					{
						//assert(0);
						//return false;
						
						PlayerBaseInfo playerBaseInfo = getPlayerBaseInfo(playerIdTmp);

						if (0 == playerBaseInfo.playerId)
						{
							return false;
						}

						PlayerInfoPtr playerInfoPtr(new PlayerInfo);

						if (!playerInfoPtr)
						{
							return false;
						}

						playerInfoPtr->playerId = playerIdTmp;
						playerInfoPtr->agent = msg->agent;
						playerInfoPtr->playerName = playerBaseInfo.playerName;
						playerInfoPtr->faceID = playerBaseInfo.faceID;
						playerInfoPtr->level = playerBaseInfo.level;
						playerInfoPtr->rank = playerBaseInfo.rank;

						playerId2InfoPtr_[playerIdTmp] = playerInfoPtr;

						itFind = playerId2InfoPtr_.find(playerIdTmp);
					}

					PlayerInfoPtr playerInfoPtr = itFind->second;

					ArmyBaseInfo armyBaseInfo = getArmyBaseInfo(*itArmyBeg, itMap2ArmyIdsBeg->first);

					if (0 == armyBaseInfo.playerId)
					{
						//assert(0);
						continue;
					}
					
					string &armyName = armyBaseInfo.armyName;

					strncpy(rsp.RoomPlayerInfo[i].ArmyName[j], armyName.c_str(), armyName.length() > armNameSize - 1 ? armNameSize - 1 : armyName.length());
				}	
			}
		}
		if (indexTmp){
			i = indexTmp;
			indexTmp = 0;
		}
		else
		{
			i++;
		}
	}

	itBeg = playerId2PlayInfo.begin();
	itEnd = playerId2PlayInfo.end();

	for (; itBeg!=itEnd; ++itBeg)
	{
		itFind = playerId2InfoPtr_.find(itBeg->first);

		if (playerId2InfoPtr_.end() == itFind)
		{
			//assert(0);
			continue;
		}

		playerInfoPtr = itFind->second;

		rsp.serialize(sizeof(DISTRIBUTED_HEADER)+sizeof(int)+sizeof(EctypeRoomPlayerInfo)*i, playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, playerInfoPtr->playerId, RSP_ENTER_ONE_ROOM_INFO);
		asynch_write(playerInfoPtr->agent, (char*)&rsp, rsp.length);
	}

	return true;
}

bool EctypeHall::onAddArmyToRoom(ReqAddArmyToEctype *msg)
{
	//if (NULL == msg)
	//{
	//	return false;
	//}

	unsigned int ectypeID = getEctypeID(msg->DoorID);

	if (0 == ectypeID)
	{
		return false;
	}

	unsigned long ulPlayerID = msg->ulPlayerID;
	bool isSingleEctype = isSingleEctypeFun(msg->DoorID);

	PlayerID2Info::iterator itFind = playerId2InfoPtr_.find(ulPlayerID);

	if (playerId2InfoPtr_.end() == itFind)
	{
		//assert(0);
		//return false;

		PlayerBaseInfo playerBaseInfo = getPlayerBaseInfo(ulPlayerID);

		if (0 == playerBaseInfo.playerId)
		{
			return false;
		}

		PlayerInfoPtr playerInfoPtr(new PlayerInfo);

		if (!playerInfoPtr)
		{
			return false;
		}

		playerInfoPtr->playerId = ulPlayerID;
		playerInfoPtr->agent = msg->agent;
		playerInfoPtr->playerName = playerBaseInfo.playerName;
		playerInfoPtr->faceID = playerBaseInfo.faceID;
		playerInfoPtr->level = playerBaseInfo.level;
		playerInfoPtr->rank = playerBaseInfo.rank;

		playerId2InfoPtr_[ulPlayerID] = playerInfoPtr;

		itFind = playerId2InfoPtr_.find(ulPlayerID);
	}

	PlayerInfoPtr playerInfoPtr = itFind->second;
	unsigned int entranceID = msg->DoorID;

	EctypeRoom* roomPtr = NULL;

	EctypeInfoTable *ectypeInfo = npcInfo_->getEctypeInfoTable(entranceID);

	if (NULL == ectypeInfo)
	{
		return false;
	}

	// 战场
	if (LIMIT_BATTLEFIELD == ectypeInfo->warType)
	{
		roomPtr = getSpecifRoom(entranceID, playerInfoPtr, msg->uEctypeRoomID);
	}
	// 副本
	else
	{
		if (0 == ectypeID)
		{
			//assert(0);
			return false;
		}

		roomPtr = getRoom(ectypeID, msg->uEctypeRoomID);
	}

	if (roomPtr == NULL)
	{
		/*assert(0);*/
		return false;
	}

	roomPtr->DeleteAllArmy(msg->ulPlayerID);

	RspAddArmyToEctype rsp;
	memset(&rsp, 0, sizeof(RspAddArmyToEctype));

	int index = 0;
	IArmyPtr  armyPtr  = NULL;                      
	unsigned int armyNameSize = 0;

	for (int i = 0 ; i < msg->ucArmyNum ; ++i)
	{
		ArmyBaseInfo armyBaseInfo = getArmyBaseInfo((*msg)[i], msg->fromMapId);
	
		armyId2EctypeId_[(*msg)[i]] = ectypeID;

		armyId2EntranceId[(*msg)[i]] = msg->DoorID;

		string &armyName = armyBaseInfo.armyName;

		if (0 == armyBaseInfo.playerId)
		{
			//assert(0);
			continue;
		}

		if (armyBaseInfo.playerId == msg->ulPlayerID)    // 添加属于自己的部队
		{
			armyNameSize = static_cast<unsigned int>(sizeof(rsp.ArmyName[index]));
			memset(rsp.ArmyName[index], 0, armyNameSize);

			strncpy(rsp.ArmyName[index], armyName.c_str(), armyName.length() > armyNameSize - 1 ?  armyNameSize - 1 : armyName.length());

			roomPtr->addArmy(msg->ulPlayerID, msg->fromMapId, (*msg)[i]);

			++index;
		}
	}

	if (!isSingleEctype)
	{
		rsp.ulPlayerID = msg->ulPlayerID;
		rsp.count = index;
		rsp.uEctypeID = ectypeID;

		const PlayerId2BaseInfo &playerId2Info = roomPtr->getPlayerHeroInfo();

		PlayerId2BaseInfo::const_iterator itBeg = playerId2Info.begin();
		PlayerId2BaseInfo::const_iterator itEnd = playerId2Info.end();

		PlayerID2Info::iterator itFind;
		unsigned long playerId = 0;

		for (; itBeg!=itEnd; ++itBeg)
		{
			playerId = (*itBeg).first;
			itFind = playerId2InfoPtr_.find(playerId);

			if (playerId2InfoPtr_.end() == itFind)
			{
				//assert(0);
				//return false;

				PlayerBaseInfo playerBaseInfo = getPlayerBaseInfo(playerId);

				if (0 == playerBaseInfo.playerId)
				{
					return false;
				}

				PlayerInfoPtr playerInfoPtr(new PlayerInfo);

				if (!playerInfoPtr)
				{
					return false;
				}

				playerInfoPtr->playerId = playerId;
				playerInfoPtr->agent = msg->agent;
				playerInfoPtr->playerName = playerBaseInfo.playerName;
				playerInfoPtr->faceID = playerBaseInfo.faceID;
				playerInfoPtr->level = playerBaseInfo.level;
				playerInfoPtr->rank = playerBaseInfo.rank;
				playerInfoPtr->campId = playerBaseInfo.campId;

				playerId2InfoPtr_[playerId] = playerInfoPtr;

				itFind = playerId2InfoPtr_.find(playerId);
			}

			PlayerInfoPtr playerInfoPtr = itFind->second;

			rsp.serialize(sizeof(rsp), playerInfoPtr->agent, msg->to, (*itBeg).first, RSP_ADD_ARMY_TO_ROOM_INFO); // 广播给在此房间的玩家

			asynch_write(playerInfoPtr->agent, (char*)&rsp, rsp.length);
		}
	}
	// 单人副本
	else
	{
		// 置为准备好
		PlayerId2IsReady &playID2IsReady = roomPtr->getPlayerStateInfo();
		playID2IsReady[msg->ulPlayerID] = true;

		ReqEnterEctypeSystem enterEctype;
		enterEctype.ulPlayer = msg->ulPlayerID;
		enterEctype.uEctypeRoomID = roomPtr->getRoomID();
		enterEctype.doorId = msg->DoorID;
		enterEctype.serialize(msg->from, MDM_GP_GAME_HALL_SERVER);
		
		onEnterEctypeSystem(&enterEctype);
	}

	return true;
}

bool EctypeHall::onKickOutOneMember(const ReqKickOutEctypeRoom *msg)
{
	//if (NULL == msg)
	//{
	//	assert(0);
	//	return false;
	//}

	unsigned long playerId = msg->ulPlayerID;
	PlayerID2Info::iterator itFind = playerId2InfoPtr_.find(playerId);

	if (playerId2InfoPtr_.end() == itFind)
	{
		//assert(0);
		//return false;
		PlayerBaseInfo playerBaseInfo = getPlayerBaseInfo(playerId);

		if (0 == playerBaseInfo.playerId)
		{
			return false;
		}

		PlayerInfoPtr playerInfoPtr(new PlayerInfo);

		if (!playerInfoPtr)
		{
			return false;
		}

		playerInfoPtr->playerId = playerId;
		playerInfoPtr->agent = msg->agent;
		playerInfoPtr->playerName = playerBaseInfo.playerName;
		playerInfoPtr->faceID = playerBaseInfo.faceID;
		playerInfoPtr->level = playerBaseInfo.level;
		playerInfoPtr->rank = playerBaseInfo.rank;
		playerInfoPtr->campId = playerBaseInfo.campId;

		playerId2InfoPtr_[playerId] = playerInfoPtr;

		itFind = playerId2InfoPtr_.find(playerId);
	}

	PlayerInfoPtr playerInfoPtr = itFind->second;

	unsigned int entranceId = msg->uEctypeID;

	unsigned int ectypeID = getEctypeID(entranceId);

	if (0 == ectypeID)
	{
		return false;
	}

	EctypeRoom *roomPtr = NULL; 

	EctypeInfoTable *ectypeInfo = npcInfo_->getEctypeInfoTable(entranceId);

	if (NULL == ectypeInfo)
	{
		return false;
	}

	// 战场
	if (LIMIT_BATTLEFIELD == ectypeInfo->warType)
	{
		roomPtr = getSpecifRoom(entranceId, playerInfoPtr, msg->uEcytypeRoomID);
	}
	// 副本
	else
	{
		if (0 == ectypeID)
		{
			//assert(0);
			return false;
		}

		roomPtr = getRoom(ectypeID, msg->uEcytypeRoomID);
	}

	if (NULL == roomPtr)
	{
		return false;
	}

	RspKickOutEctypeRoom rsp;

	if (roomPtr->getRoomHostID() != msg->ulPlayerID  // 如果不是房主，返回失败
		|| msg->ulOtherID == msg->ulPlayerID)        // 如果自己踢自己，返回失败
	{
		rsp.flag = false;
		rsp.serialize(sizeof(rsp), playerInfoPtr->agent, msg->from, msg->ulPlayerID, RSP_KICK_OUT_OTHER_INFO);
		asynch_write(playerInfoPtr->agent,(char*)&rsp,sizeof(rsp));

		return false;
	}

	const PlayerId2BaseInfo &playerId2Info= roomPtr->getPlayerHeroInfo();

	PlayerId2BaseInfo::const_iterator it = playerId2Info.find(msg->ulOtherID);

	// 如果在房间内找到要踢的玩家
	if (it != playerId2Info.end())
	{
		rsp.flag = true;
		rsp.uEctypeID = msg->uEctypeID;
		rsp.uEctypeRoomID = msg->uEcytypeRoomID;
		rsp.ulOtherID = msg->ulOtherID;

		PlayerId2BaseInfo::const_iterator itBeg = playerId2Info.begin();
		PlayerId2BaseInfo::const_iterator itEnd = playerId2Info.end();
		PlayerID2Info::iterator itFindTmp;

		// 广播给所有的在此房间的人
		for (; itBeg!=itEnd; ++itBeg)
		{
			itFindTmp = playerId2InfoPtr_.find((*itBeg).first);

			if (playerId2InfoPtr_.end() == itFindTmp)
			{
				//assert(0);
				continue;
			}

			PlayerInfoPtr playerInfoPtrTmp = itFindTmp->second;

			rsp.serialize(sizeof(rsp), playerInfoPtrTmp->agent, msg->from, (*itBeg).first, RSP_KICK_OUT_OTHER_INFO);
			asynch_write(playerInfoPtrTmp->agent, (char*)&rsp, sizeof(rsp));
		}

		roomPtr->kickMemberOrDelMem(msg->ulOtherID);
	}
	else//否则返回失败给房主
	{
		rsp.flag = false;
		rsp.serialize(sizeof(rsp), playerInfoPtr->agent, msg->from, msg->ulPlayerID, RSP_KICK_OUT_OTHER_INFO);
		asynch_write(playerInfoPtr->agent, (char*)&rsp, sizeof(rsp));
	}

	return true;
}

bool EctypeHall::onGetRoomInfo(const ReqEctypeRoomInfo *msg)
{
	//if (NULL == msg)
	//{
	//	assert(0);
	//	return false;
	//}

	unsigned long playerId = msg->from;
	PlayerID2Info::iterator itFind = playerId2InfoPtr_.find(playerId);

	if (playerId2InfoPtr_.end() == itFind)
	{
		//assert(0);
		//return false;
		PlayerBaseInfo playerBaseInfo = getPlayerBaseInfo(playerId);

		if (0 == playerBaseInfo.playerId)
		{
			return false;
		}

		PlayerInfoPtr playerInfoPtr(new PlayerInfo);

		if (!playerInfoPtr)
		{
			return false;
		}

		playerInfoPtr->playerId = playerId;
		playerInfoPtr->agent = msg->agent;
		playerInfoPtr->playerName = playerBaseInfo.playerName;
		playerInfoPtr->faceID = playerBaseInfo.faceID;
		playerInfoPtr->level = playerBaseInfo.level;
		playerInfoPtr->rank = playerBaseInfo.rank;
		playerInfoPtr->campId = playerBaseInfo.campId;

		playerId2InfoPtr_[playerId] = playerInfoPtr;

		itFind = playerId2InfoPtr_.find(playerId);
	}

	PlayerInfoPtr playerInfoPtr = itFind->second;

	EctypeRoom* roomPtr = NULL;

	unsigned int entranceID = msg->uEctypeID;

	EctypeInfoTable *ectypeInfo = npcInfo_->getEctypeInfoTable(entranceID);

	if (NULL == ectypeInfo)
	{
		return false;
	}

	// 战场
	if (LIMIT_BATTLEFIELD == ectypeInfo->warType)
	{
		roomPtr = getSpecifRoom(entranceID, playerInfoPtr, msg->uEctypeRoomID);
	}
	// 副本
	else
	{
		unsigned int ectypeID = getEctypeID(msg->uEctypeID);

		if (0 == ectypeID)
		{
			//assert(0);
			return false;
		}

		roomPtr = getRoom(ectypeID, msg->uEctypeRoomID);
	}
		
	if (roomPtr == NULL)
	{
		/*assert(0);*/
		return false;
	}

	RspEctypeRoomInfo rsp;
	memset(&rsp, 0, sizeof(RspEctypeRoomInfo));

	int count = 0 ;
	unsigned int sizeCopy = 0;
	unsigned int lenth = 0;

	const string &roomNotice = roomPtr->getRoomNotice();
	unsigned int roomNoteSize = sizeof(rsp.RoomNote);

	strncpy(rsp.RoomNote, roomNotice.c_str(), roomNotice.length() > roomNoteSize - 1 ? roomNoteSize - 1 : roomNotice.length());

	const PlayerId2MapId2ArmyIds &playerId2ArmyIds = roomPtr->getPlayerInfo();

	PlayerId2MapId2ArmyIds::const_iterator itBeg = playerId2ArmyIds.begin();
	PlayerId2MapId2ArmyIds::const_iterator itEnd = playerId2ArmyIds.end();

	unsigned int nameLegth = 0;

	for (; itBeg!=itEnd; ++itBeg) // 将此房间内的所有成员名添加到结构体中，返回给客户端
	{
		itFind = playerId2InfoPtr_.find(itBeg->first);

		if (playerId2InfoPtr_.end() == itFind)
		{
			//assert(0);
			continue;
		}

		PlayerInfoPtr playerInfoPtr = itFind->second;

		if (playerInfoPtr)
		{
			string &playerName = playerInfoPtr->playerName;
			nameLegth = static_cast<unsigned int>(sizeof(rsp.name[count++]));

			strncpy(rsp.name[count++], playerName.c_str(), playerName.length() > nameLegth - 1 ?  nameLegth - 1 : playerName.length());
		}
	}

	rsp.count = count;
	rsp.serialize(playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, msg->from, RSP_GET_ONE_ROOM_INFO);
	asynch_write(playerInfoPtr->agent,(char*)&rsp, rsp.length);

	return true;
}

bool EctypeHall::onRoomNotice(const ReqEctypeRoomNote* msg)
{
	//if (NULL == msg)
	//{
	//	//assert(0);
	//	return false;
	//}	
	
	unsigned int entranceID = msg->uEctypID;

	unsigned int ectypeID = getEctypeID(entranceID);

	unsigned long playerId = msg->ulplayerID;
	PlayerID2Info::iterator itFind = playerId2InfoPtr_.find(playerId);

	if (playerId2InfoPtr_.end() == itFind)
	{
		//assert(0);
		return false;
	}

	PlayerInfoPtr playerInfoPtr = itFind->second;

	EctypeRoom *roomPtr = NULL; 

	EctypeInfoTable *ectypeInfo = npcInfo_->getEctypeInfoTable(entranceID);

	if (NULL == ectypeInfo)
	{
		return false;
	}

	// 战场
	if (LIMIT_BATTLEFIELD == ectypeInfo->warType)
	{
		roomPtr = getSpecifRoom(entranceID, playerInfoPtr, msg->uEctypeRoomID);
	}
	// 副本
	else
	{
		if (0 == ectypeID)
		{
			//assert(0);
			return false;
		}

		roomPtr = getRoom(ectypeID, msg->uEctypeRoomID);
	}

	if (roomPtr ==	NULL)
	{
		//assert(0);
		return false;
	}

	RspEctypeRoomNote rsp;
	memset(&rsp, 0, sizeof(RspEctypeRoomNote));

	if (roomPtr->getRoomHostID() == playerId) // 如果是房主
	{
		roomPtr->setRoomNotice(msg->RoomNote);

		memcpy(rsp.RoomNote, msg->RoomNote, SIZE_32 - 1);
		rsp.RoomNote[SIZE_32 - 1] = '\0';

		rsp.uEctypeID = ectypeID;
		rsp.uEctypeRoomID = msg->uEctypeRoomID;

		const PlayerId2BaseInfo &playerID2Info = roomPtr->getPlayerHeroInfo();

		PlayerId2BaseInfo::const_iterator itBeg = playerID2Info.begin();
		PlayerId2BaseInfo::const_iterator itEnd = playerID2Info.end();
		PlayerID2Info::iterator itFind;

		for (; itBeg != itEnd; ++itBeg)  // 就将改变的公告信息包发送给在此房间的每个玩家
		{
			itFind = playerId2InfoPtr_.find(itBeg->first);

			if (playerId2InfoPtr_.end() == itFind)
			{
				//assert(0);
				continue;
			}

			PlayerInfoPtr playerInfoPtr = itFind->second;

			if (playerInfoPtr)
			{	
				rsp.serialize(sizeof(rsp), playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, (*itBeg).first, RSP_SEND_NOTICE_INFO);
				asynch_write(playerInfoPtr->agent, (char*)&rsp, sizeof(rsp));
			}
		}
	}

	return true;
}

bool EctypeHall::onEnterEctypeSystem(const ReqEnterEctypeSystem *msg)
{
	unsigned long playerId = msg->ulPlayer;
	PlayerID2Info::iterator itFind = playerId2InfoPtr_.find(playerId);

	if (playerId2InfoPtr_.end() == itFind)
	{
		//assert(0);
		//return false;
		PlayerBaseInfo playerBaseInfo = getPlayerBaseInfo(playerId);

		if (0 == playerBaseInfo.playerId)
		{
			return false;
		}

		PlayerInfoPtr playerInfoPtr(new PlayerInfo);

		if (!playerInfoPtr)
		{
			return false;
		}

		playerInfoPtr->playerId = playerId;
		playerInfoPtr->agent = msg->agent;
		playerInfoPtr->playerName = playerBaseInfo.playerName;
		playerInfoPtr->faceID = playerBaseInfo.faceID;
		playerInfoPtr->level = playerBaseInfo.level;
		playerInfoPtr->rank = playerBaseInfo.rank;
		playerInfoPtr->campId = playerBaseInfo.campId;

		playerId2InfoPtr_[playerId] = playerInfoPtr;

		itFind = playerId2InfoPtr_.find(playerId);
	}

	PlayerInfoPtr playerInfoPtr = itFind->second;

	unsigned int entranceId = msg->doorId;

	unsigned int ectypeID = getEctypeID(entranceId);

	if (0 == ectypeID)
	{
		return false;
	}

	bool isSingleEctype = isSingleEctypeFun(entranceId);

	RspEnterEctypeSystem rsp ;
	memset(&rsp, 0, sizeof(RspEnterEctypeSystem));

	EctypeRoom *roomPtr = NULL;

	bool isBattlefield = false;

	EctypeInfoTable *ectypeInfo = npcInfo_->getEctypeInfoTable(entranceId);

	if (NULL == ectypeInfo)
	{
		return false;
	}

	// 战场
	if (LIMIT_BATTLEFIELD == ectypeInfo->warType)
	{
		isBattlefield = true;
		roomPtr = getSpecifRoom(entranceId, playerInfoPtr, msg->uEctypeRoomID);
	}
	// 副本
	else
	{
		roomPtr = getRoom(ectypeID, msg->uEctypeRoomID);
	}

	if (NULL == roomPtr)
	{
		return false;
	}

	if (roomPtr->getRoomHostID() == msg->ulPlayer) // 如果是房主
	{	
		EctypeRoom *partnershipRoomPtr = NULL;
	
		if (!isBattlefield)
		{
			const PlayerId2IsReady &playId2IsReady = roomPtr->getPlayerStateInfo();

			PlayerId2IsReady::const_iterator itBeg = playId2IsReady.begin();
			PlayerId2IsReady::const_iterator itEnd = playId2IsReady.end();

			for (; itBeg != itEnd; ++itBeg)
			{
				if (!(*itBeg).second) // 只要该房间中的一个玩家没有准备好的话，就返回失败信息给房主
				{
					rsp.flag = 0;
					rsp.isSigleEctype = isSingleEctype;

					rsp.serialize(sizeof(RspEnterEctypeSystem), playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, msg->ulPlayer, RSP_ENTER_ECTYPE_SYSTEM_INFO);
					asynch_write(playerInfoPtr->agent, (char*)&rsp, sizeof(RspEnterEctypeSystem));

					return false;
				}
			}
		}
		else
		{ 
			room2IsReady_[roomPtr->getRoomID()] = true;
			room2IsWaiting[roomPtr->getRoomID()] = true;

			// 找配对
			unsigned int enemyCampId = playerInfoPtr->campId == UNION ? CLAN : UNION;
			partnershipRoomPtr = getPartnershipRoom(entranceId, enemyCampId);

			if (NULL == partnershipRoomPtr)
			{
				// 发给客户端，进入等待界面。。。。。。。。
				RspBattleFieldWaitingInfo rspBattleFieldWaitingInfo;
				memset(&rspBattleFieldWaitingInfo, 0, sizeof(RspBattleFieldWaitingInfo));

				const PlayerId2BaseInfo &playerID2Info = roomPtr->getPlayerHeroInfo();

				PlayerId2BaseInfo::const_iterator itBeg = playerID2Info.begin();
				PlayerId2BaseInfo::const_iterator itEnd = playerID2Info.end();
				PlayerID2Info::iterator itFind;

				for (; itBeg != itEnd; ++itBeg)  
				{
					itFind = playerId2InfoPtr_.find(itBeg->first);

					if (playerId2InfoPtr_.end() == itFind)
					{
						//assert(0);
						continue;
					}

					PlayerInfoPtr playerInfoPtr = itFind->second;

					if (playerInfoPtr)
					{	
						rspBattleFieldWaitingInfo.serialize(sizeof(rspBattleFieldWaitingInfo), playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, (*itBeg).first);
						asynch_write(playerInfoPtr->agent, (char*)&rspBattleFieldWaitingInfo, sizeof(rspBattleFieldWaitingInfo));
					}
				}

				return true;
			}
		}


		// 通知副本服务器,
		PlayerId2MapId2ArmyIds playerId2MapIdArmysTotal;   // 汇总
		int armyCount = 0, playerCount = 0;

		const PlayerId2MapId2ArmyIds &playerId2ArmyIds = roomPtr->getPlayerInfo();

		playerId2MapIdArmysTotal.insert(playerId2ArmyIds.begin(), playerId2ArmyIds.end());

		unsigned int roomHasArmySum = roomPtr->getRoomHasTotalArmyNum();

		if (isBattlefield)
		{
			const PlayerId2MapId2ArmyIds &partnershipPlayerId2ArmyIds = partnershipRoomPtr->getPlayerInfo();
			playerId2MapIdArmysTotal.insert(partnershipPlayerId2ArmyIds.begin(), partnershipPlayerId2ArmyIds.end());

			roomHasArmySum += partnershipRoomPtr->getRoomHasTotalArmyNum();
		}

		PlayerId2MapId2ArmyIds::const_iterator itPlayerBeg = playerId2MapIdArmysTotal.begin();
		PlayerId2MapId2ArmyIds::const_iterator itPlayerEnd = playerId2MapIdArmysTotal.end();

		unsigned int size = static_cast<unsigned int>(sizeof(Notify_Enter_Ectype) + sizeof(unsigned long) * playerId2MapIdArmysTotal.size()  + sizeof(ArmyId2MapId) * roomHasArmySum);
		Notify_Enter_Ectype *pEvent = (Notify_Enter_Ectype *)malloc(size);

		if (NULL == pEvent)
		{
			return false;
		}

		memset(pEvent, 0, size);

		pEvent->uEctypeID = ectypeID;
		pEvent->entranceID = msg->doorId;
		pEvent->iArmyCount = roomHasArmySum;
		pEvent->uEctypeRoom[0] = roomPtr->getRoomID();
	
		if (isBattlefield)
		{
			pEvent->uEctypeRoom[1] = partnershipRoomPtr->getRoomID();
		}

		pEvent->playerCount = static_cast<unsigned int>(playerId2MapIdArmysTotal.size());
		bool isAddArmy = false;

		for (; itPlayerBeg != itPlayerEnd; ++itPlayerBeg)
		{
			isAddArmy = false;
			const MapId2ArmyIds &mapId2ArmyIds = itPlayerBeg->second;

			MapId2ArmyIds::const_iterator itMapId2ArmyIdsBeg = mapId2ArmyIds.begin();
			MapId2ArmyIds::const_iterator itMapId2ArmyIdsEnd = mapId2ArmyIds.end();

			for (; itMapId2ArmyIdsBeg!=itMapId2ArmyIdsEnd; ++itMapId2ArmyIdsBeg)
			{		
				const set<unsigned int> &armySet = itMapId2ArmyIdsBeg->second;

				set<unsigned int>::const_iterator itArmyBeg = armySet.begin();
				set<unsigned int>::const_iterator itArmyEnd = armySet.end();

				for(;itArmyBeg != itArmyEnd; ++itArmyBeg)
				{
					isAddArmy = true;
					pEvent->getArmyInfo(armyCount).armyId = (*itArmyBeg);
					pEvent->getArmyInfo(armyCount++).mapId = itMapId2ArmyIdsBeg->first;
				}
			}

			// 如果某个玩家没有一个部队，就返回失败给房主
			if (!isAddArmy)
			{
				rsp.flag = 0;
				rsp.isSigleEctype = isSingleEctype;
				rsp.serialize(sizeof(RspEnterEctypeSystem), playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, msg->ulPlayer, RSP_ENTER_ECTYPE_SYSTEM_INFO);
				asynch_write(playerInfoPtr->agent, (char*)&rsp, sizeof(RspEnterEctypeSystem));

				return false;
			}

			pEvent->getPlayerId(playerCount++) = itPlayerBeg->first;
		}

		// 配对后，置为false
		if (isBattlefield)
		{
			Room2IsReady::iterator itFind = room2IsReady_.find(roomPtr->getRoomID());

			if (room2IsReady_.end() != itFind)
			{
				itFind->second = false;
			}

			itFind = room2IsReady_.find(partnershipRoomPtr->getRoomID());

			if (room2IsReady_.end() != itFind)
			{
				itFind->second = false;
			}
		}

		pEvent->serialize(size, 0, 0, 0, CMD_ENTER_ECTYPE);
		event_notify(ECTYPE_SERVER_ID, (char*)pEvent, size);

		free(pEvent);
	}
	else
	{
		rsp.flag = 2; // 不是房主不能请求
		rsp.isSigleEctype = isSingleEctype;
		rsp.serialize(sizeof(RspEnterEctypeSystem), playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, msg->ulPlayer, RSP_ENTER_ECTYPE_SYSTEM_INFO);
		asynch_write(playerInfoPtr->agent, (char*)&rsp, sizeof(RspEnterEctypeSystem));
	}

	return true;
}

EctypeRoom * EctypeHall::getPartnershipRoom(unsigned int entranceId, unsigned int campId)
{
	EctypeRoom *roomPtr = NULL;

	EntranceId2CampIdRoomInfo::iterator itFind = entranceId2CampIdRoomInfo_.find(entranceId);

	if (entranceId2CampIdRoomInfo_.end() != itFind)
	{	
		CampId2RoomInfo &campId2RoomInfo = itFind->second;
		CampId2RoomInfo::iterator itCampId2Room = campId2RoomInfo.find(campId);

		if (campId2RoomInfo.end() != itCampId2Room)
		{
			RoomID2Info &roomId2Info = itCampId2Room->second;

			RoomID2Info::iterator itRoomBeg = roomId2Info.begin();
			RoomID2Info::iterator itRoomEnd = roomId2Info.end();

			for (; itRoomBeg!=itRoomEnd; ++itRoomBeg)
			{
				roomPtr = itRoomBeg->second;

				if (NULL == roomPtr)
				{
					assert(0);
					continue;
				}
			
				Room2IsReady::iterator itRoom2IsReady = room2IsReady_.find(roomPtr->getRoomID());

				if (room2IsReady_.end() != itRoom2IsReady)
				{
					if (itRoom2IsReady->second)
					{
						return roomPtr;
					}
				}
			}
		}
	}

	return NULL;
}

bool EctypeHall::onExitEctypeHall(const ReqExitEctypeHall *msg)
{
	//if (NULL == msg)
	//{
	//	assert(0);
	//	return false;
	//}

	unsigned int playerId = msg->playerID;

	PlayerID2Info::iterator itFind = playerId2InfoPtr_.find(playerId);

	if (playerId2InfoPtr_.end() == itFind)
	{
		/*assert(0);*/
		return false;
	}

	PlayerInfoPtr playerInfoPtr = itFind->second;

	unsigned int entranceId = msg->iEctypeID;

	unsigned int ectypeID = getEctypeID(entranceId);

	if (0 == ectypeID)
	{
		return false;
	}

	playerID2EctypeId_.erase(msg->playerID);

	playerId2InfoPtr_.erase(msg->playerID);

	entranceId2CampIdPlayerIds_[entranceId][playerInfoPtr->campId].erase(playerId);

	playerInfoPtr->gameHallType = HALL_TYPE_INVALID;

	return true;
}

bool EctypeHall::onBackEctypeHall(const ReqBackToHall *msg)
{
	//if (NULL == msg)
	//{
	//	assert(0);
	//	return false;
	//}

	unsigned long playerId = msg->ulPlayerID;
	PlayerID2Info::iterator itFind = playerId2InfoPtr_.find(playerId);

	if (playerId2InfoPtr_.end() == itFind)
	{
		//assert(0);
		//return false;
		PlayerBaseInfo playerBaseInfo = getPlayerBaseInfo(playerId);

		if (0 == playerBaseInfo.playerId)
		{
			return false;
		}

		PlayerInfoPtr playerInfoPtr(new PlayerInfo);

		if (!playerInfoPtr)
		{
			return false;
		}

		playerInfoPtr->playerId = playerId;
		playerInfoPtr->agent = msg->agent;
		playerInfoPtr->playerName = playerBaseInfo.playerName;
		playerInfoPtr->faceID = playerBaseInfo.faceID;
		playerInfoPtr->level = playerBaseInfo.level;
		playerInfoPtr->rank = playerBaseInfo.rank;
		playerInfoPtr->campId = playerBaseInfo.campId;

		playerId2InfoPtr_[playerId] = playerInfoPtr;

		itFind = playerId2InfoPtr_.find(playerId);
	}

	PlayerInfoPtr playerInfoPtr = itFind->second;

	unsigned int entranceId = msg->DoorID;

	unsigned int ectypeID = getEctypeID(entranceId);

	if (0 == ectypeID)
	{
		return false;
	}

	EctypeRoom *roomPtr = NULL;

	bool isBattlefield = false;

	EctypeInfoTable *ectypeInfo = npcInfo_->getEctypeInfoTable(entranceId);

	if (NULL == ectypeInfo)
	{
		return false;
	}

	// 战场
	if (LIMIT_BATTLEFIELD == ectypeInfo->warType)
	{
		isBattlefield = true;
		roomPtr = getSpecifRoom(entranceId, playerInfoPtr, msg->iEctypeRoomID);
	}
	// 副本
	else
	{
		roomPtr = getRoom(ectypeID, msg->iEctypeRoomID);
	}

	if (roomPtr == NULL)
	{
		return false;
	}

	RspBackToHall rsp;
	memset(&rsp, 0, sizeof(RspBackToHall));	

	rsp.iEctypeRoomID = msg->iEctypeRoomID;
	rsp.playerID = msg->ulPlayerID;

	string &playerName = playerInfoPtr->playerName;
	unsigned int playerNameSize = static_cast<unsigned int>(sizeof(rsp.Name));

	strncpy(rsp.Name, playerName.c_str(), playerName.length() > playerNameSize - 1 ? playerNameSize - 1 : playerName.length());

	// 如果是房主就删掉房间
	if (roomPtr->getRoomHostID() == msg->ulPlayerID) 
	{
		rsp.IsRoomHost = true;	

		if (!isBattlefield)
		{
			NoticeAllPlayerInEctype(ectypeID, rsp);

			deleteRoom(ectypeID, msg->iEctypeRoomID);
		}
		else
		{
			noticeAllPlayerInBattelfield(entranceId, playerInfoPtr->campId, rsp);
			deleteBattlefieldRoom(entranceId, msg->iEctypeRoomID);
		}

	}
	// 否则删掉玩家
	else 
	{
		rsp.IsRoomHost = false;

		if (!isBattlefield)
		{
			NoticeAllPlayerInRoom(ectypeID, rsp);
		}
		else
		{
			noticeAllPlayerInBattelfieldRoom(entranceId, playerInfoPtr->campId, msg->iEctypeRoomID, rsp);
		}	

		roomPtr->kickMemberOrDelMem(msg->ulPlayerID);
	}

	return true;
}

void EctypeHall::noticeAllPlayerInBattelfield(unsigned int entranceId, unsigned int campId, RspBackToHall &rsp)
{
	EntranceId2CampIdPlayerIds::iterator itFindEntranceId2CampId = entranceId2CampIdPlayerIds_.find(entranceId);

	if (entranceId2CampIdPlayerIds_.end() != itFindEntranceId2CampId)
	{
		CampId2PlayerIds &campId2PlayerIds = itFindEntranceId2CampId->second;
		CampId2PlayerIds::iterator itCampId2Playerids = campId2PlayerIds.find(campId);

		if (campId2PlayerIds.end() != itCampId2Playerids)
		{
			std::set<unsigned int /* 玩家ID */> &playerIds = itCampId2Playerids->second;
			std::set<unsigned int /* 玩家ID */>::iterator itBeg = playerIds.begin();
			std::set<unsigned int /* 玩家ID */>::iterator itEnd = playerIds.end();
			PlayerID2Info::iterator itFind;

			for (; itBeg!=itEnd; ++itBeg)
			{
				itFind = playerId2InfoPtr_.find(*itBeg);

				if (playerId2InfoPtr_.end() == itFind)
				{
					//assert(0);
					continue;
				}

				PlayerInfoPtr playerInfoPtr = itFind->second;

				rsp.serialize(sizeof(rsp), playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, *itBeg, RSP_BACK_ECTYPE_HALL_INFO);
				asynch_write(playerInfoPtr->agent,(char*)&rsp, sizeof(rsp));
			}
		}
	}	

}

void EctypeHall::noticeAllPlayerInBattelfieldRoom(unsigned int entranceId, unsigned int campId,unsigned int roomId, RspBackToHall &rsp)
{
	EntranceId2CampIdRoomInfo::iterator itFind = entranceId2CampIdRoomInfo_.find(entranceId);

	if (entranceId2CampIdRoomInfo_.end() != itFind)
	{
		CampId2RoomInfo &campId2RoomInfo = itFind->second;

		CampId2RoomInfo::iterator itCampId2RoomInfo = campId2RoomInfo.find(campId);

		if (campId2RoomInfo.end() != itCampId2RoomInfo)
		{
			RoomID2Info &roomInfos = itCampId2RoomInfo->second;

			RoomID2Info::iterator itRoomBeg = roomInfos.begin();
			RoomID2Info::iterator itRoomEnd = roomInfos.end();

			for (; itRoomBeg!=itRoomEnd ;++itRoomBeg)
			{	
				EctypeRoom *roomPtr = itRoomBeg->second;

				if (NULL == roomPtr)
				{
					//assert(0);
					continue;
				}

				const PlayerId2BaseInfo& playerInfo = roomPtr->getPlayerHeroInfo();
				PlayerId2BaseInfo::const_iterator itPlayerId2BaseInfoBeg = playerInfo.begin();
				PlayerId2BaseInfo::const_iterator itPlayerId2BaseInfoEnd = playerInfo.end();
				PlayerID2Info::iterator itFind;

				for (; itPlayerId2BaseInfoBeg!=itPlayerId2BaseInfoEnd; ++itPlayerId2BaseInfoBeg)
				{
					itFind = playerId2InfoPtr_.find(itPlayerId2BaseInfoBeg->first);

					if (playerId2InfoPtr_.end() == itFind)
					{
						//assert(0);
						continue;
					}

					PlayerInfoPtr playerInfoPtr = itFind->second;

					if (playerInfoPtr)
					{
						rsp.serialize(sizeof(rsp), playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, itPlayerId2BaseInfoBeg->first, RSP_BACK_ECTYPE_HALL_INFO);
						asynch_write(playerInfoPtr->agent,(char*)&rsp, sizeof(rsp));
					}
				}
			}
		}
	}
}

void EctypeHall::NoticeAllPlayerInEctype(int ectypeID, RspBackToHall &rsp)
{
	// 创建好房间后，广播给所有在副本中的玩家
	PlayerID2EctypeId::iterator itBeg = playerID2EctypeId_.begin();
	PlayerID2EctypeId::iterator itEnd = playerID2EctypeId_.end();
	PlayerID2Info::iterator itFind;

	for (; itBeg!=itEnd; ++itBeg)
	{
		if (itBeg->second != ectypeID)
		{
			continue;
		}

		itFind = playerId2InfoPtr_.find(itBeg->first);

		if (playerId2InfoPtr_.end() == itFind)
		{
			//assert(0);
			continue;
		}

		PlayerInfoPtr playerInfoPtr = itFind->second;

		rsp.serialize(sizeof(rsp), playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, itBeg->first, RSP_BACK_ECTYPE_HALL_INFO);
		asynch_write(playerInfoPtr->agent,(char*)&rsp, sizeof(rsp));
	}
}

void EctypeHall::NoticeAllPlayerInRoom(int ectypeID, RspBackToHall &rsp)
{
	Ectype2RoomInfo::iterator it = roomTable_.find((unsigned int)ectypeID);

	if (it !=roomTable_.end())
	{
		RoomID2Info & roomList = it->second;
		RoomID2Info::iterator itRoomBeg = roomList.begin();
		RoomID2Info::iterator itRoomEnd = roomList.end();
		
		for (; itRoomBeg!=itRoomEnd ;++itRoomBeg)
		{	
			EctypeRoom *roomPtr = itRoomBeg->second;

			if (NULL == roomPtr)
			{
				//assert(0);
				continue;
			}

			const PlayerId2BaseInfo& playerInfo = roomPtr->getPlayerHeroInfo();
			PlayerId2BaseInfo::const_iterator itPlayerId2BaseInfoBeg = playerInfo.begin();
			PlayerId2BaseInfo::const_iterator itPlayerId2BaseInfoEnd = playerInfo.end();
			PlayerID2Info::iterator itFind;

			for (; itPlayerId2BaseInfoBeg!=itPlayerId2BaseInfoEnd; ++itPlayerId2BaseInfoBeg)
			{
				itFind = playerId2InfoPtr_.find(itPlayerId2BaseInfoBeg->first);

				if (playerId2InfoPtr_.end() == itFind)
				{
					//assert(0);
					continue;
				}

				PlayerInfoPtr playerInfoPtr = itFind->second;
				
				if (playerInfoPtr)
				{
					rsp.serialize(sizeof(rsp), playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, itPlayerId2BaseInfoBeg->first, RSP_BACK_ECTYPE_HALL_INFO);
					asynch_write(playerInfoPtr->agent,(char*)&rsp, sizeof(rsp));
				}
			}
		}
	}
}

bool EctypeHall::onEctypeReady(const ReqEctypeReady *msg)
{
	//if (NULL == msg)
	//{
	//	assert(0);
	//	return false;
	//}

	unsigned long playerId = msg->ulPlayerID;
	PlayerID2Info::iterator itFind = playerId2InfoPtr_.find(playerId);

	if (playerId2InfoPtr_.end() == itFind)
	{
		//assert(0);
		//return false;
		PlayerBaseInfo playerBaseInfo = getPlayerBaseInfo(playerId);

		if (0 == playerBaseInfo.playerId)
		{
			return false;
		}

		PlayerInfoPtr playerInfoPtr(new PlayerInfo);

		if (!playerInfoPtr)
		{
			return false;
		}

		playerInfoPtr->playerId = playerId;
		playerInfoPtr->agent = msg->agent;
		playerInfoPtr->playerName = playerBaseInfo.playerName;
		playerInfoPtr->faceID = playerBaseInfo.faceID;
		playerInfoPtr->level = playerBaseInfo.level;
		playerInfoPtr->rank = playerBaseInfo.rank;
		playerInfoPtr->campId = playerBaseInfo.campId;

		playerId2InfoPtr_[playerId] = playerInfoPtr;

		itFind = playerId2InfoPtr_.find(playerId);
	}

	PlayerInfoPtr playerInfoPtr = itFind->second;

	unsigned int entranceId = msg->iDoorID;

	unsigned int ectypeID = getEctypeID(entranceId);

	if (0 == ectypeID)
	{
		return false;
	}

	EctypeRoom *roomPtr = NULL;

	EctypeInfoTable *ectypeInfo = npcInfo_->getEctypeInfoTable(entranceId);

	if (NULL == ectypeInfo)
	{
		return false;
	}

	// 战场
	if (LIMIT_BATTLEFIELD == ectypeInfo->warType)
	{
		roomPtr = getSpecifRoom(entranceId, playerInfoPtr, msg->iEctypeRoomID);
	}
	// 副本
	else
	{
		if (0 == ectypeID)
		{
			//assert(0);
			return false;
		}

		roomPtr = getRoom(ectypeID, msg->iEctypeRoomID);
	}

	if (roomPtr == NULL)
	{
		return false;
	}

	PlayerId2IsReady &playID2IsReady = roomPtr->getPlayerStateInfo();
	playID2IsReady[msg->ulPlayerID] = msg->flag & 1;

	RspEctypeReady rsp;
	memset(&rsp,0,sizeof(RspEctypeReady));

	rsp.ulPlayerID = msg->ulPlayerID;
	rsp.flag = msg->flag;

	const PlayerId2MapId2ArmyIds &playerId2ArmyIds = roomPtr->getPlayerInfo();

	PlayerId2MapId2ArmyIds::const_iterator itBeg = playerId2ArmyIds.begin();
	PlayerId2MapId2ArmyIds::const_iterator itEnd = playerId2ArmyIds.end();

	for (; itBeg != itEnd; ++itBeg)
	{
		PlayerID2Info::iterator itFind = playerId2InfoPtr_.find((*itBeg).first);

		if (playerId2InfoPtr_.end() == itFind)
		{
			//assert(0);
			continue;
		}

		PlayerInfoPtr playerInfoPtr = itFind->second;

		if (playerInfoPtr)
		{
			rsp.serialize(playerInfoPtr->agent, msg->to, (*itBeg).first);  // 将是否准备好的消息发送给所有该房间的玩家
			asynch_write(rsp.agent,(char*)&rsp, rsp.length);
		}
	}

	return true;
}

unsigned int EctypeHall::getEctypeID(unsigned int doorID) const
{
	if (NULL == dbInfo_)
	{
		//assert(0);
		return 0;
	}

	unsigned int ectypeID = 0;

	/*
	list<EctypeBaseTable>::iterator itBeg = dbInfo_->m_EctypBaseInfoList.begin();
	list<EctypeBaseTable>::iterator itEnd = dbInfo_->m_EctypBaseInfoList.end();

	unsigned int ectypeID = 0;

	for (; itBeg!=itEnd; ++itBeg)
	{
		if (((unsigned long)(itBeg->doorid_)) == doorID)
		{
			ectypeID = itBeg->ectypeid_;
			break;
		}
	}
	*/

	map<int , EctypeInfoTable *> &ecytpeInfo = npcInfo_->ectypeInfos_;
	map<int , EctypeInfoTable *>::iterator it = ecytpeInfo.find(doorID);

	if (ecytpeInfo.end() != it)
	{
		EctypeInfoTable *ectypeInfoTable = it->second;

		if (NULL != ectypeInfoTable)
		{
			ectypeID = ectypeInfoTable->ectypeID;
		}
	}

	return ectypeID ;
}

bool EctypeHall::isInEctypeRoom(unsigned int playerID, bool isSingleRoom)
{
	Ectype2RoomInfo::iterator itrEctypeBeg = roomTable_.begin();
	Ectype2RoomInfo::iterator itrEctypeEnd = roomTable_.end();

	for (; itrEctypeBeg!=itrEctypeEnd; ++itrEctypeBeg)//遍历所有副本
	{
		RoomID2Info &rooms =itrEctypeBeg->second;
		RoomID2Info::iterator itrRoom = rooms.begin();
		RoomID2Info::iterator itrRoomEnd = rooms.end();

		for (; itrRoom != itrRoomEnd; ++itrRoom)//遍历所有房间
		{
			EctypeRoom* roomPtr = itrRoom->second;

			if (NULL == roomPtr)
			{
				//assert(0);
				continue;
			}

			const PlayerId2MapId2ArmyIds &playerId2armyIdset = roomPtr->getPlayerInfo();
			PlayerId2MapId2ArmyIds::const_iterator itrPlay = playerId2armyIdset.find(playerID);//找此玩家

			if (itrPlay != roomPtr->getPlayerInfo().end())//如果找到返回true
			{
				if (isSingleRoom)
				{
					// 这里是防止副本服务器出问题后，需要处理单人副本
					deleteRoom(roomPtr->getEctypeID(), roomPtr->getRoomID()); 
					return false;
				}

				return true;
			}
		}
	}

	return false;//否则 没有找到返回false;
}

unsigned int EctypeHall::getRoomID(unsigned int EctypeID)
{
	Ectype2RoomInfo ::iterator itr = roomTable_.find(EctypeID);

	if (itr != roomTable_.end())
	{
		RoomID2Info &room = itr->second;
		RoomID2Info::iterator itrRoom = room.begin();

		if (room.empty())
		{
			return 1;
		}
		else
		{
			itrRoom = room.end();
			itrRoom--;

			EctypeRoom * roomTmp = itrRoom->second;

			if (NULL == roomTmp)
			{
				return 1;
			}
			else
			{
				return roomTmp->getRoomID() + 1; // 取该副本中最大的roomID然后+1
			}
		}
	}
	else
	{
		return 1;//第一个副本房间的ID就定为1
	}

}

unsigned int EctypeHall::getBattlefieldRoomID(unsigned int entranceId)
{
	EntranceId2CampIdRoomInfo::iterator itFindTmp = entranceId2CampIdRoomInfo_.find(entranceId);

	if (entranceId2CampIdRoomInfo_.end() != itFindTmp)
	{
		unsigned int roomId = 0;

		CampId2RoomInfo &campId2RoomInfo = itFindTmp->second;

		CampId2RoomInfo::iterator itCampId2Room = campId2RoomInfo.find(UNION);

		if (campId2RoomInfo.end() != itCampId2Room)
		{
			RoomID2Info &room = itCampId2Room->second;

			RoomID2Info::iterator itrRoom = room.begin();

			if (room.empty())
			{
				roomId = 0;
			}
			else
			{
				itrRoom = room.end();
				itrRoom--;

				EctypeRoom * roomTmp = itrRoom->second;

				if (NULL == roomTmp)
				{
					roomId = 0;
				}
				else
				{
					roomId = roomTmp->getRoomID(); // 取该副本中最大的roomID然后+1
				}
			}
		}

		itCampId2Room = campId2RoomInfo.find(CLAN);

		if (campId2RoomInfo.end() != itCampId2Room)
		{
			RoomID2Info &room = itCampId2Room->second;

			RoomID2Info::iterator itrRoom = room.begin();

			if (room.empty())
			{
				return std::max<unsigned int>(roomId, 0) + 1;
			}
			else
			{
				itrRoom = room.end();
				itrRoom--;

				EctypeRoom * roomTmp = itrRoom->second;

				if (NULL == roomTmp)
				{
					return std::max<unsigned int>(roomId, 0) + 1;
				}
				else
				{
					return std::max<unsigned int>(roomTmp->getRoomID(), roomId) + 1;
				}
			}
		}
		else
		{
			return roomId + 1;
		}
	}

	return 1; // 第一个战场房间的ID就定为1
}

bool EctypeHall::addRoomInEctype(unsigned int ectypeID,unsigned int roomID, const EctypeRoom* room)
{
	if (NULL == room)
	{
		return false;
	}

	roomTable_[ectypeID][roomID] = const_cast<EctypeRoom *>(room);

	return true;
}

bool EctypeHall::addRoomInBattlefield(unsigned int entranceId, unsigned int campId, unsigned int roomId, const EctypeRoom* room)
{	
	if (NULL == room)
	{
		return false;
	}

	entranceId2CampIdRoomInfo_[entranceId][campId][roomId] = const_cast<EctypeRoom *>(room);

	return true;
}

EctypeRoom * EctypeHall::getRoom(unsigned int ectypeID, unsigned int roomID)
{
	Ectype2RoomInfo::iterator it = roomTable_.find(ectypeID);

	if (it == roomTable_.end())
	{
		return NULL;
	}

	RoomID2Info &roomInfo = it->second;

	RoomID2Info::iterator itRoom = roomInfo.find(roomID);

	if (itRoom == roomInfo.end())
	{
		return NULL;
	}

	return roomTable_[ectypeID][roomID];
}


void EctypeHall::deleteRoom(unsigned long ectypeID, unsigned long roomID)
{
	Ectype2RoomInfo::iterator it = roomTable_.find(ectypeID);

	if (it == roomTable_.end())
	{
		return;
	}

	RoomID2Info &roomInfo = it->second;

	RoomID2Info::iterator itRoom = roomInfo.find(roomID);

	if (itRoom == roomInfo.end())
	{
		return ;
	}

	if (NULL != itRoom->second)
	{
		delete itRoom->second;
	}

	roomInfo.erase(itRoom);

	if (roomInfo.empty())
	{
		roomTable_.erase(it);
	}
}

void EctypeHall::deleteBattlefieldRoom(unsigned int entranceId, unsigned int roomId)
{
	room2IsWaiting.erase(roomId);

	EctypeRoom *roomPtr = NULL;

	EntranceId2CampIdRoomInfo ::iterator itFindTmp = entranceId2CampIdRoomInfo_.find(entranceId);

	if (entranceId2CampIdRoomInfo_.end() != itFindTmp)
	{
		CampId2RoomInfo &campId2RoomInfo = itFindTmp->second;

		CampId2RoomInfo::iterator itCampId2Room = campId2RoomInfo.find(UNION);

		if (campId2RoomInfo.end() != itCampId2Room)
		{
			RoomID2Info &roomId2Info = itCampId2Room->second;

			RoomID2Info::iterator itRoomId2Info = roomId2Info.find(roomId);

			if (roomId2Info.end() != itRoomId2Info)
			{
				room2IsReady_.erase(itRoomId2Info->first);
				delete itRoomId2Info->second;
				
				roomId2Info.erase(itRoomId2Info);

				if (roomId2Info.empty())
				{
					campId2RoomInfo.erase(UNION);
				}

				if (campId2RoomInfo.empty())
				{
					entranceId2CampIdRoomInfo_.erase(entranceId);
				}
			}	
		}
	
		if (entranceId2CampIdRoomInfo_.empty())
		{
			return;
		}
		
		itCampId2Room = campId2RoomInfo.find(CLAN);

		if (campId2RoomInfo.end() != itCampId2Room)
		{
			RoomID2Info &roomId2Info = itCampId2Room->second;

			RoomID2Info::iterator itRoomId2Info = roomId2Info.find(roomId);

			if (roomId2Info.end() != itRoomId2Info)
			{
				room2IsReady_.erase(itRoomId2Info->first);
				delete itRoomId2Info->second;

				roomId2Info.erase(itRoomId2Info);	

				if (roomId2Info.empty())
				{
					campId2RoomInfo.erase(CLAN);
				}

				if (campId2RoomInfo.empty())
				{
					entranceId2CampIdRoomInfo_.erase(entranceId);
				}
			}
		}
	}
}

bool EctypeHall::eventCreateEctypeBack(const Notify_Back_Enter_Ectype *eventCreateEctype)
{
	//if (eventCreateEctype == NULL)
	//{
	//	return false;
	//}

	unsigned int entranceId = eventCreateEctype->entranceID;
	EctypeInfoTable * ectypeInfo = npcInfo_->getEctypeInfoTable(entranceId);

	if (NULL == ectypeInfo)
	{
		return false;
	}

	if (1 == ectypeInfo->warType)
	{
		entranceId2RoomId_[entranceId] = std::make_pair(eventCreateEctype->RoomID, eventCreateEctype->uEctypID);
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	RspEnterEctypeSystem rspEnterEctypeSystem;
	memset(&rspEnterEctypeSystem, 0, sizeof(RspEnterEctypeSystem));

	EctypeRoom *ectypeRoomPtr = NULL;
	EctypeRoom *partnershipRoomPtr = NULL; 
	bool isBattlefield = false;
	
	// 战场
	if (LIMIT_BATTLEFIELD == ectypeInfo->warType)
	{
		isBattlefield = true;
		ectypeRoomPtr = getSpecifRoom(entranceId, eventCreateEctype->uEctypeRoomID[0]);

		if (ectypeRoomPtr == NULL)
		{
			return false;
		}

		partnershipRoomPtr = getSpecifRoom(entranceId, eventCreateEctype->uEctypeRoomID[1]);

		if (NULL == partnershipRoomPtr)
		{
			return false;
		}
		else
		{
			const PlayerId2MapId2ArmyIds &playerId2ArmyIdsTmp = partnershipRoomPtr->getPlayerInfo();

			PlayerId2MapId2ArmyIds::const_iterator itBegTmp = playerId2ArmyIdsTmp.begin();
			PlayerId2MapId2ArmyIds::const_iterator itEndTmp = playerId2ArmyIdsTmp.end();

			RspCancelBattleFieldWaitingInfo rspCancelBattleFieldWaitingInfo;

			for (; itBegTmp!=itEndTmp; ++itBegTmp)
			{
				memset(&rspCancelBattleFieldWaitingInfo, 0, sizeof(RspCancelBattleFieldWaitingInfo));

				rspCancelBattleFieldWaitingInfo.flag = true;

				PlayerID2Info::iterator itFind = playerId2InfoPtr_.find(itBegTmp->first);

				if (playerId2InfoPtr_.end() == itFind)
				{
					//assert(0);
					continue;
				}

				PlayerInfoPtr playerInfoPtr = itFind->second;

				if (playerInfoPtr)
				{
					rspCancelBattleFieldWaitingInfo.serialize(sizeof(RspCancelBattleFieldWaitingInfo), playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, itBegTmp->first);
					asynch_write(playerInfoPtr->agent, (char*)&rspCancelBattleFieldWaitingInfo, sizeof(RspCancelBattleFieldWaitingInfo));
				}

			}
		}
	}
	else
	{
		ectypeRoomPtr = getRoom(eventCreateEctype->uEctypID, eventCreateEctype->uEctypeRoomID[0]);
		
		if (ectypeRoomPtr == NULL)
		{
			return false;
		}
	}

	bool isSingleEctype = isSingleEctypeFun(entranceId);
	
	PlayerId2MapId2ArmyIds playerInfoTotal;  // 汇总

	const PlayerId2MapId2ArmyIds& playerInfo = ectypeRoomPtr->getPlayerInfo();

	playerInfoTotal.insert(playerInfo.begin(), playerInfo.end());

	if (NULL != partnershipRoomPtr)
	{
		const PlayerId2MapId2ArmyIds& partnershipPlayerInfo = partnershipRoomPtr->getPlayerInfo();

		playerInfoTotal.insert(partnershipPlayerInfo.begin(), partnershipPlayerInfo.end());
	}

	PlayerId2MapId2ArmyIds::const_iterator itSinglePlayerID = playerInfoTotal.begin();

	if (itSinglePlayerID == playerInfoTotal.end())
	{
		//assert(0);
		return false;
	}

	PlayerId2MapId2ArmyIds::const_iterator itPlayerId2MapArmyInfoBeg;
	PlayerId2MapId2ArmyIds::const_iterator itPlayerId2MapArmyInfoEnd;
	
	if (eventCreateEctype->ret == 1)
	{
		// 通知该玩家可以进入副本房间
		rspEnterEctypeSystem.isSigleEctype = isSingleEctype;
		rspEnterEctypeSystem.EctypeID = eventCreateEctype->uEctypID;
		rspEnterEctypeSystem.flag = 1;
		rspEnterEctypeSystem.EcServerID = ECTYPE_SERVER_ID;
		rspEnterEctypeSystem.EctypeRoomID = eventCreateEctype->uEctypeRoomID[0];
		rspEnterEctypeSystem.NpcRoomID = eventCreateEctype->RoomID;
		rspEnterEctypeSystem.Ectypemapid = eventCreateEctype->mapid ;	
		
		PlayerID2Info::iterator itFind;
		
		itPlayerId2MapArmyInfoBeg = playerInfoTotal.begin();
		itPlayerId2MapArmyInfoEnd = playerInfoTotal.end();

		for (; itPlayerId2MapArmyInfoBeg!=itPlayerId2MapArmyInfoEnd; ++itPlayerId2MapArmyInfoBeg)
		{	
			itFind = playerId2InfoPtr_.find(itPlayerId2MapArmyInfoBeg->first);

		    if (playerId2InfoPtr_.end() == itFind)
			{
				//assert(0);
		 		continue;
		    }

			PlayerInfoPtr playerInfoPtr = itFind->second;

			if (playerInfoPtr)
			{	
				rspEnterEctypeSystem.serialize(sizeof(RspEnterEctypeSystem), playerInfoPtr->agent, eventCreateEctype->to,
					itPlayerId2MapArmyInfoBeg->first, RSP_ENTER_ECTYPE_SYSTEM_INFO);
				asynch_write(playerInfoPtr->agent,(char*)&rspEnterEctypeSystem,sizeof(RspEnterEctypeSystem));
			}
		}

		// 通知所在郡把部队删除
		PlayerId2MapId2ArmyIds playerId2MapId2ArmyIdsTotal;

		const PlayerId2MapId2ArmyIds &armys = ectypeRoomPtr->getPlayerInfo();
		playerId2MapId2ArmyIdsTotal.insert(armys.begin(), armys.end());

		if (NULL != partnershipRoomPtr)
		{
			const PlayerId2MapId2ArmyIds &armys = partnershipRoomPtr->getPlayerInfo();
			playerId2MapId2ArmyIdsTotal.insert(armys.begin(), armys.end());	
		}

		PlayerId2MapId2ArmyIds::iterator itPlayersBeg = playerId2MapId2ArmyIdsTotal.begin();
		PlayerId2MapId2ArmyIds::iterator itPlayersEnd = playerId2MapId2ArmyIdsTotal.end();

		int index = 0;
		unsigned int size = 0;
		std::set<unsigned int >::const_iterator itArmyIdsBeg, itArmyIdsEnd;
		
		for (; itPlayersBeg!=itPlayersEnd; ++itPlayersBeg)
		{		
			const MapId2ArmyIds &mapId2ArmyIDs = itPlayersBeg->second;

			MapId2ArmyIds::const_iterator itMapId2ArmyIdsBeg = mapId2ArmyIDs.begin();
			MapId2ArmyIds::const_iterator itMapId2ArmyIdsEnd = mapId2ArmyIDs.end();

			for (; itMapId2ArmyIdsBeg!=itMapId2ArmyIdsEnd; ++itMapId2ArmyIdsBeg)
			{
				const std::set<unsigned int > &armyIdset = itMapId2ArmyIdsBeg->second;

				if (armyIdset.empty())
				{
					continue;
				}

				itArmyIdsBeg = armyIdset.begin();
				itArmyIdsEnd = armyIdset.end();

				index = 0;
				size = static_cast<unsigned int>(sizeof(EventArmyLeave) + sizeof(unsigned long) * armyIdset.size());

				EventArmyLeave *pArmyLeaveInfo = (EventArmyLeave *)malloc(size);

				if (NULL == pArmyLeaveInfo)
				{
					return false;
				}

				memset(pArmyLeaveInfo, 0, size);

				for (; itArmyIdsBeg != itArmyIdsEnd; ++itArmyIdsBeg)
				{
					//	填写部队ID
					(*pArmyLeaveInfo)[index++] = *itArmyIdsBeg;
				}

				pArmyLeaveInfo->armyState = ARMY_FIGHT_ECTYPE;
				pArmyLeaveInfo->armyNum = index;

				pArmyLeaveInfo->serialize(MDM_GP_GAME_HALL_SERVER, itMapId2ArmyIdsBeg->first);

				event_notify(itMapId2ArmyIdsBeg->first, (char*)pArmyLeaveInfo, size);

				free(pArmyLeaveInfo);
			}
		}
	}
	else
	{
		if (isSingleEctype)
		{
			rspEnterEctypeSystem.isSigleEctype = isSingleEctype;

			rspEnterEctypeSystem.EctypeID = eventCreateEctype->uEctypID;
			rspEnterEctypeSystem.flag = 3;
			rspEnterEctypeSystem.EcServerID = ECTYPE_SERVER_ID;
			rspEnterEctypeSystem.EctypeRoomID = eventCreateEctype->uEctypeRoomID[0];
			rspEnterEctypeSystem.NpcRoomID = eventCreateEctype->RoomID;
			rspEnterEctypeSystem.Ectypemapid = eventCreateEctype->mapid;

			PlayerID2Info::iterator itFind = playerId2InfoPtr_.find(itSinglePlayerID->first);

			if (playerId2InfoPtr_.end() == itFind)
			{
				//assert(0);
				return false;
			}

			PlayerInfoPtr playerInfoPtr = itFind->second;

			if (playerInfoPtr)
			{
				rspEnterEctypeSystem.serialize(sizeof(RspEnterEctypeSystem), playerInfoPtr->agent, eventCreateEctype->to,
												itSinglePlayerID->first, RSP_ENTER_ECTYPE_SYSTEM_INFO);
				asynch_write(playerInfoPtr->agent, (char*)&rspEnterEctypeSystem, sizeof(RspEnterEctypeSystem));
			}
		}
	}

	//RspBackToHall msgRspToHall;
	//memset(&msgRspToHall, 0, sizeof(RspBackToHall));
	//msgRspToHall.iEctypeRoomID = ectypeRoomPtr->getRoomID();
	//msgRspToHall.playerID = ectypeRoomPtr->getRoomHostID();
	//msgRspToHall.IsRoomHost = true;
	
	PlayerID2Info::iterator itFind;

	itPlayerId2MapArmyInfoBeg = playerInfoTotal.begin();
	itPlayerId2MapArmyInfoEnd = playerInfoTotal.end();

	for (; itPlayerId2MapArmyInfoBeg!=itPlayerId2MapArmyInfoEnd; ++itPlayerId2MapArmyInfoBeg)
	{
		itFind = playerId2InfoPtr_.find(itPlayerId2MapArmyInfoBeg->first);

		if (playerId2InfoPtr_.end() == itFind)
		{
			//assert(0);
			continue;
		}

		PlayerInfoPtr playerInfoPtr = itFind->second;

		//if (playerInfoPtr)
		//{
		//	msgRspToHall.serialize(sizeof(RspBackToHall), playerInfoPtr->agent,
		//		eventCreateEctype->to, itPlayerId2MapArmyInfoBeg->first, RSP_BACK_ECTYPE_HALL_INFO);
		//	asynch_write(playerInfoPtr->agent, (char*)&msgRspToHall, sizeof(RspBackToHall));
		//}

		playerId2InfoPtr_.erase(itPlayerId2MapArmyInfoBeg->first);
		playerID2EctypeId_.erase(itPlayerId2MapArmyInfoBeg->first);

		entranceId2CampIdPlayerIds_[entranceId][playerInfoPtr->campId].erase(itPlayerId2MapArmyInfoBeg->first);
	}

	// 删除房间
	if (!isBattlefield)
	{
		deleteRoom(eventCreateEctype->uEctypID, eventCreateEctype->uEctypeRoomID[0]);
	}
	else
	{
		deleteBattlefieldRoom(entranceId, eventCreateEctype->uEctypeRoomID[0]);
		deleteBattlefieldRoom(entranceId, eventCreateEctype->uEctypeRoomID[1]);
	}

	return true;
}

void EctypeHall::initOneFieldEctypeRoom(unsigned int entraceID)
{
	EctypeInfoTable * ectypeInfoTablePtr = npcInfo_->getEctypeInfoTable(entraceID);

	if (NULL != ectypeInfoTablePtr)
	{
		Notify_Enter_Ectype notityEnterEctype;
		unsigned int sizeTmp = static_cast<unsigned int>(sizeof(Notify_Enter_Ectype));
		memset(&notityEnterEctype, 0, sizeTmp);
		notityEnterEctype.uEctypeID = ectypeInfoTablePtr->ectypeID;
		notityEnterEctype.entranceID = ectypeInfoTablePtr->entranceID;
		//notityEnterEctype.uEctypeRoom = 0;

		notityEnterEctype.serialize(sizeTmp, 0, 0, 0, CMD_ENTER_ECTYPE);
		event_notify(ECTYPE_SERVER_ID, (char*)&notityEnterEctype, sizeTmp);	
	}
}

void EctypeHall::initFieldEctypeRoom()
{
	map<int , EctypeInfoTable*> &ecytpeInfo = npcInfo_->ectypeInfos_;
	EctypeInfoTable *ectypeInfoTable = NULL;
	std::map<int , EctypeInfoTable*>::iterator itBeg = ecytpeInfo.begin();
	std::map<int , EctypeInfoTable*>::iterator itEnd = ecytpeInfo.end();
	unsigned int iRoomId = 0;

	for (; itBeg!=itEnd; ++itBeg)
	{
		ectypeInfoTable = itBeg->second;
		//assert(NULL != ectypeInfoTable);

		if (1 == ectypeInfoTable->warType)
		{
			Notify_Enter_Ectype notityEnterEctype;
			unsigned int sizeTmp = static_cast<unsigned int>(sizeof(Notify_Enter_Ectype));
			memset(&notityEnterEctype, 0, sizeTmp);
			notityEnterEctype.entranceID = ectypeInfoTable->entranceID;
			notityEnterEctype.uEctypeID = ectypeInfoTable->ectypeID;
			notityEnterEctype.uEctypeRoom[0] = iRoomId++;

			notityEnterEctype.serialize(sizeTmp, 0, 0, 0, CMD_ENTER_ECTYPE);
			event_notify(ECTYPE_SERVER_ID, (char*)&notityEnterEctype, sizeTmp);			
		}
	}
}

bool EctypeHall::onExitSingleEctype(ReqExitSingleEctypeInfo *msg)
{
	unsigned int ectypeID = getEctypeID(msg->doorId);
	bool isSingleEctype = isSingleEctypeFun(msg->doorId);

	if (!isSingleEctype)
	{
//#ifdef _DEBUG
//		assert(0);
//#endif
		return false;
	}
	
	// 删除房间
	deleteRoom(ectypeID, msg->iRoomId);

	playerID2EctypeId_.erase(msg->playerID);

	playerId2InfoPtr_.erase(msg->playerID);
	//ActivityPlayerPtr userInfoPtr = fightManager_->getPlayerInfo(msg->playerID);

	//if (userInfoPtr == NULL)
	//{
	//	return false;
	//}	

	//userInfoPtr->ectypeID_ = 0; // 将用户列表中的状态设置为0，表示自己不在副本中了

	return true;
}

bool EctypeHall::onEnterFieldEctype(ReqEnterFieldEctype *buf)
{
	EntranceId2RoomId::iterator it = entranceId2RoomId_.find(buf->doorId);

	if (it != entranceId2RoomId_.end())
	{	
		std::pair<unsigned int /* faceRoomId */, unsigned int /* ectypeId */ > &faceRoomId2EctypeId = it->second;
	
		RspEnterFieldEctype rspEnterFieldEctype;
		rspEnterFieldEctype.armyId = buf->armyID;
		rspEnterFieldEctype.roomId = faceRoomId2EctypeId.first;
		rspEnterFieldEctype.ectypeId = ECTYPE_SERVER_ID;	
		
		armyId2EctypeId_[buf->armyID] = faceRoomId2EctypeId.second;

		armyId2EntranceId[buf->armyID] = buf->doorId;
	
		rspEnterFieldEctype.serialize(sizeof(RspEnterFieldEctype), buf->agent, MDM_GP_GAME_HALL_SERVER, buf->from);
		asynch_write(buf->agent, (char*)&rspEnterFieldEctype, sizeof(RspEnterFieldEctype));
	}

	return true;
}

bool EctypeHall::onArmyLeaveEctype(ReqLeaveEctypeArmyInfo *buf)
{
	unsigned int armyNum = buf->ucArmyNum;
	
	for (unsigned int i=0; i<armyNum; ++i)
	{
		armyId2EctypeId_.erase((*buf)[i]);
		armyId2EntranceId.erase((*buf)[i]);
	}

	return true;
}

bool EctypeHall::isSingleEctypeFun(unsigned int entranceId)
{
	EctypeInfoTable * ectypeInfo = npcInfo_->getEctypeInfoTable(entranceId);
	
	if (ectypeInfo != NULL)
	{
		return ectypeInfo->limitPlayerNum == 1;
	}

	return false;
}

ArmyBaseInfo EctypeHall::getArmyBaseInfo(unsigned int armyId, unsigned int fromMapId)
{
	ArmyTable armyTable;
	armyTable.setmasterid(fromMapId);
	armyTable.setdetailid(armyId);

	if(dbPtr_->db_select(armyTable) != DB_Proxy::DB_SUCCEED)
	{
		//LOG_ERROR(("查找数据错误， 部队ID%d" , armyId));
		return ArmyBaseInfo(0, "");
	}

	return ArmyBaseInfo(armyTable.playerid_, armyTable.armyname_);
}

PlayerBaseInfo EctypeHall::getPlayerBaseInfo(unsigned int playerId)
{
	PlayerOfficerTable playerInfo;
	playerInfo.setmasterid(playerId);
	playerInfo.setdetailid(playerId);
	
	if(dbPtr_->db_select(playerInfo) != DB_Proxy::DB_SUCCEED)
	{
		//LOG_ERROR(("查询玩家%d英雄%d失败位置processUpdateHeroParam", playerId , playerId));
		return PlayerBaseInfo(0, 0, "", 0, 0, 0);
	}

	PlayerChatRoomTable roomtb;
	roomtb.setid(playerId);

	if(dbPtr_->db_select(roomtb) != DB_Proxy::DB_SUCCEED)
	{
		return PlayerBaseInfo(playerId, playerInfo.faceid_, playerInfo.officername_, playerInfo.level_, playerInfo.militaryrank_, 0);
	}

	return PlayerBaseInfo(playerId, playerInfo.faceid_, playerInfo.officername_, playerInfo.level_, playerInfo.militaryrank_, roomtb.country_);
}

void EctypeHall::onFieldEctypeReboot(EventEctypeOver *buf)
{
	entranceId2RoomId_.erase(buf->entraceID);
	initOneFieldEctypeRoom(buf->entraceID);
}

void EctypeHall::onGetArmyInEntranceId(ReqGetArmyEntranceID *buf)
{
	ArmyId2EntranceId::iterator itFind = armyId2EntranceId.find(buf->armyId);

	if (armyId2EntranceId.end() != itFind)
	{
		RspGetArmyEntranceID rspGetArmyEntranceId;
		memset(&rspGetArmyEntranceId, 0, sizeof(RspGetArmyEntranceID));

		rspGetArmyEntranceId.entranceId = itFind->second;

		rspGetArmyEntranceId.serialize(buf->agent, MDM_GP_GAME_HALL_SERVER, buf->from);
		asynch_write(buf->agent, (char*)&rspGetArmyEntranceId, sizeof(RspGetArmyEntranceID));
	}
}

EctypeRoom *EctypeHall::getSpecifRoom(unsigned int entranceID, PlayerInfoPtr playerInfoPtr, unsigned int roomId)
{
	EctypeRoom *roomPtr = NULL;

	EntranceId2CampIdRoomInfo ::iterator itFindTmp = entranceId2CampIdRoomInfo_.find(entranceID);

	if (entranceId2CampIdRoomInfo_.end() != itFindTmp)
	{
		CampId2RoomInfo &campId2RoomInfo = itFindTmp->second;

		CampId2RoomInfo::iterator itCampId2Room = campId2RoomInfo.find(playerInfoPtr->campId);

		if (campId2RoomInfo.end() != itCampId2Room)
		{
			RoomID2Info &roomId2Info = itCampId2Room->second;

			RoomID2Info::iterator itRoomId2Info = roomId2Info.find(roomId);

			if (roomId2Info.end() != itRoomId2Info)
			{
				roomPtr = itRoomId2Info->second;
			}
		}
	}

	return roomPtr;
}

EctypeRoom *EctypeHall::getSpecifRoom(unsigned int entranceID, unsigned int roomId)
{
	EctypeRoom *roomPtr = NULL;

	EntranceId2CampIdRoomInfo ::iterator itFindTmp = entranceId2CampIdRoomInfo_.find(entranceID);

	if (entranceId2CampIdRoomInfo_.end() != itFindTmp)
	{
		CampId2RoomInfo &campId2RoomInfo = itFindTmp->second;

		CampId2RoomInfo::iterator itCampId2Room = campId2RoomInfo.find(UNION);

		if (campId2RoomInfo.end() != itCampId2Room)
		{
			RoomID2Info &roomId2Info = itCampId2Room->second;

			RoomID2Info::iterator itRoomId2Info = roomId2Info.find(roomId);

			if (roomId2Info.end() != itRoomId2Info)
			{
				return itRoomId2Info->second;
			}
		}
		
		itCampId2Room = campId2RoomInfo.find(CLAN);

		if (campId2RoomInfo.end() != itCampId2Room)
		{
			RoomID2Info &roomId2Info = itCampId2Room->second;

			RoomID2Info::iterator itRoomId2Info = roomId2Info.find(roomId);

			if (roomId2Info.end() != itRoomId2Info)
			{
				return itRoomId2Info->second;
			}
		}
		
	}

	return roomPtr;
}

bool EctypeHall::onCancelBattlefieldWaiting(ReqCancelBattleFieldWaitingInfo *buf)
{
	unsigned int roomId = buf->roomId;
	unsigned int playerId = buf->playerId;
	unsigned int campId = buf->campId;
	unsigned int entranceId = buf->entranceId;
	
	EntranceId2CampIdRoomInfo::iterator itFind = entranceId2CampIdRoomInfo_.find(entranceId);
	
	if (entranceId2CampIdRoomInfo_.end() != itFind)
	{
		CampId2RoomInfo &campId2RoomInfo = itFind->second;
		CampId2RoomInfo::iterator itCampId2RoomInfo = campId2RoomInfo.find(campId);
	
		if (campId2RoomInfo.end() != itCampId2RoomInfo)
		{
			RoomID2Info &roomId2Info = itCampId2RoomInfo->second;
			RoomID2Info::iterator itRoomId2Info = roomId2Info.find(roomId);

			if (roomId2Info.end() != itRoomId2Info)
			{
				EctypeRoom *room = itRoomId2Info->second;

				if (NULL == room)
				{
					return false;
				}

				// 取消战场房间等待的玩家是该房间的房主
				if (playerId == room->getRoomHostID())
				{
					//Room2IsReady::iterator itRoom = room2IsReady_.find(roomId);

					//if (room2IsReady_.end() != itRoom)
					//{
					//	itRoom->second = false;
					//}
					//else
					//{
					//	assert(0);
					//}

					Room2IsReady::iterator itRoom = room2IsWaiting.find(roomId);
					
					if (room2IsWaiting.end() != itRoom)
					{
						itRoom->second = false;
					}

					RspCancelBattleFieldWaitingInfo rspCancelBattleFieldWaitingInfo;
					memset(&rspCancelBattleFieldWaitingInfo, 0, sizeof(RspCancelBattleFieldWaitingInfo));

					rspCancelBattleFieldWaitingInfo.flag = true;

					const PlayerId2BaseInfo &playerID2Info = room->getPlayerHeroInfo();

					PlayerId2BaseInfo::const_iterator itBeg = playerID2Info.begin();
					PlayerId2BaseInfo::const_iterator itEnd = playerID2Info.end();
					PlayerID2Info::iterator itFind;

					for (; itBeg != itEnd; ++itBeg)  
					{
						itFind = playerId2InfoPtr_.find(itBeg->first);

						if (playerId2InfoPtr_.end() == itFind)
						{
							//assert(0);
							continue;
						}

						PlayerInfoPtr playerInfoPtr = itFind->second;

						if (playerInfoPtr)
						{	
							rspCancelBattleFieldWaitingInfo.serialize(sizeof(rspCancelBattleFieldWaitingInfo), playerInfoPtr->agent, MDM_GP_GAME_HALL_SERVER, (*itBeg).first);
							asynch_write(playerInfoPtr->agent, (char*)&rspCancelBattleFieldWaitingInfo, sizeof(rspCancelBattleFieldWaitingInfo));
						}
					}

				}
				// 只向取消操作的非法玩家发失败消息
				else
				{
					notifyPlayerCancelBattlefieldFail(buf->agent, buf->from);
				}
			}
			else
			{
				notifyPlayerCancelBattlefieldFail(buf->agent, buf->from);
			}
		}
		else
		{
			notifyPlayerCancelBattlefieldFail(buf->agent, buf->from);
		}
	}
	else
	{
		notifyPlayerCancelBattlefieldFail(buf->agent, buf->from);
	}

	return true;
}

void EctypeHall::notifyPlayerCancelBattlefieldFail(unsigned int agent, unsigned int playerId)
{
	RspCancelBattleFieldWaitingInfo rspCancelBattleFieldWaitingInfo;
	memset(&rspCancelBattleFieldWaitingInfo, 0, sizeof(RspCancelBattleFieldWaitingInfo));

	rspCancelBattleFieldWaitingInfo.flag = false;

	rspCancelBattleFieldWaitingInfo.serialize(sizeof(RspCancelBattleFieldWaitingInfo), agent, MDM_GP_GAME_HALL_SERVER, playerId);
	asynch_write(agent, (char*)&rspCancelBattleFieldWaitingInfo, sizeof(RspCancelBattleFieldWaitingInfo));
}

void EctypeHall::parse(const char *buf)
{
	//if (buf == NULL)
	//{
	//	return ;
	//}
 	DISTRIBUTED_HEADER * msgHeader = (DISTRIBUTED_HEADER *)buf;

	switch(msgHeader->command)
	{
	case REQ_ENTER_HALL_INFO:
		onEnterEctypeHall((ReqHallInfo*)buf);
		break;
	case REQ_GET_ONE_ROOM_INFO:
		onGetRoomInfo((ReqEctypeRoomInfo*)buf);
		break;
	case REQ_ENTER_ONE_ROOM_INFO:
		rspEnterEctypeRoomInfo((ReqEnterEctypeRoom*)buf);
		break;
	case REQ_CREATE_ROOM_INFO:
		onCreateEctypeRoom((ReqCreateEctypeRoom*)buf);
		break;
	case REQ_ADD_ECTYPE_ROOM_INFO:
		onAddToEctypeRoom((ReqAddToEctypeRoom*)buf);
		break;
	case REQ_ADD_ARMY_TO_ROOM_INFO:
		onAddArmyToRoom((ReqAddArmyToEctype*)buf);
		break;
	case REQ_KICK_OUT_OTHER_INFO:
		onKickOutOneMember((ReqKickOutEctypeRoom*)buf);
		break;
	case REQ_ENTER_ECTYPE_SYSTEM_INFO:
		onEnterEctypeSystem((ReqEnterEctypeSystem*)buf);
		break;
	case REQ_LEAVE_ECTYPE_HALL_INFO:
		onExitEctypeHall((ReqExitEctypeHall*)buf);
		break;
	case REQ_SEND_NOTICE_INFO:
		onRoomNotice((ReqEctypeRoomNote*)buf);
		break;
	case REQ_BACK_ECTYPE_HALL_INFO:
		onBackEctypeHall((ReqBackToHall*)buf);
		break;
	case REQ_ECTYPE_READY_INFO:
		onEctypeReady((ReqEctypeReady*)buf);
		break;
	case CMD_BACK_IS_IN_ECTYPE:
		eventBackIsInEctypeInfo((Notify_Back_Is_In_Ectype*)buf);
		break;
	case CMD_BACK_ENTER_ECTYPE:
		eventCreateEctypeBack((Notify_Back_Enter_Ectype*)buf);
		break;
	case REQ_BACK_SINGLE_ECTYPE_INFO:
		onExitSingleEctype((ReqExitSingleEctypeInfo *)buf);
		break;
	case REQ_ENTER_HALL_INFO_FROM_ARMY_LIST:
		onEnterEctypeHallFromArmyList((ReqHallInfoFromArmyList *)buf);
		break;
	case EVENT_ARMY_LEAVE_ECTYPE:
		onArmyLeaveEctype((ReqLeaveEctypeArmyInfo *)buf);
		break;
	case REQ_ENTER_FIELD_ECTYPE:
		onEnterFieldEctype((ReqEnterFieldEctype *)buf);
		break;
	case EVENT_ECTYPE_OVER:
		onFieldEctypeReboot((EventEctypeOver *)buf);
		break;
	case REQ_GET_ARMY_IN_ENTRANCEID:
		onGetArmyInEntranceId((ReqGetArmyEntranceID *)buf);
		break;
	case REQ_CANCEL_BATTLE_FIELD_WAITING_INFO:
		onCancelBattlefieldWaiting((ReqCancelBattleFieldWaitingInfo *)buf);
		break;
	default:
		break;
	}
}