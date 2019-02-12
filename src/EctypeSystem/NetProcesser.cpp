
#include "NetProcesser.h"
#include "socketsvc.hpp"
#include "CommonTemplate.h"
#include "IniFile.h"
#include "LoadDBInfo.h"
#include "NPCInfo.h"
#include "EctypeFightManager.h"
#include "EctypeManager.h"
#include "FightEvent.h"
#include "ServerProtocolHeader.h"
#include "Mutexguard.h"
#include "FightSystemMacro.h"
#include "Time_Protocol.h"
#include "log.hpp"
#define  ZSBB_ECHO(x) printf x;
#define  ECTYPE_ROOM_NUM 1000
int SOCKETSVC_API  event_notify_callback(int server_id, const char * buffer, int length)
{
	char * szbuf = new char[length];
	if (szbuf == NULL){
		return -1;
	} 
	memcpy(szbuf,buffer , length);
	NetProcesser::instance()->parse(szbuf);
	//ZSBB_ECHO(("event_notify_callback %d \n" , server_id));
	return 0;
}

int SOCKETSVC_API  data_receiver_callback(int agent_id, int /*player_id*/, char * buffer, int length)
{
	char * szbuf = new char[length];
	if (szbuf == NULL){
		return -1;
	}
	memcpy(szbuf,buffer , length);
	NetProcesser::instance()->parse(szbuf);
	//ZSBB_ECHO(("data_receiver_callback from %d \n" , agent_id));
	return 0;
}

int SOCKETSVC_API  timer_notify_callback(const void * param,const void * /*param_2*/ , long timerID)
{
	NetProcesser::instance()->parse((char*const)param);
	//ZSBB_ECHO(("timer_notify_callback\n"));
	return 0;

}

void SOCKETSVC_API  socket_disconn_callback(bool player_all, int agent_id, int server_id, int player_id)
{
	//需要添加客户端断开消息
	
}

void SOCKETSVC_API  logic_exception_callback(int /*code*/, int /*agent_id*/, int /*from_id*/, int /*to_id*/)
{
// 	std::cout << "logic exception: \nrequest from " << from_id << " to " << to_id 
// 		<< "\nagent_id " << agent_id << "\nmessage:" << get_error_message(code) << std::endl;

}


NetProcesser* NetProcesser::instance_ = NULL; 
NetProcesser::NetProcesser(void):
dataBaseProxy_(NULL),
ectypeManager_(NULL),
ectypeNpcInfo_ (NULL),
dataBaseMemData_(NULL)
{
	
}

NetProcesser::~NetProcesser(void)
{
}

NetProcesser* NetProcesser::instance()
{
	if(instance_ == NULL){
		instance_ =  new NetProcesser();
	
	}
	return instance_;
}

bool NetProcesser::pushDataToManager(const char * buf , int roomID){
	if(isValidRoomID(roomID) == false){
		delete [] buf;
		return false;
	}
	ShareMutexGuard<boost::shared_mutex> guard(ectypeMutex_);
	if (managerStates[roomID] == FIGHTMANAGE_IS_RUNING && fightManagers[roomID]!= NULL){
		fightManagers[roomID]->putQueue((char * const)buf);
		return true;
	}
	delete [] buf;
	return false;
}
void NetProcesser::parse(char *const buf){
	if (buf == NULL){
		return ;
	}
	DISTRIBUTED_HEADER * msgHeader = (DISTRIBUTED_HEADER *)buf;
	switch(msgHeader->command){
	case EN_C_TIMER_NOTIFY:
		pushDataToManager(buf , ((FightNotfiy*)buf)->iFightRoomID);
		break;
	case REQ_ATTACK_ECTYPE_TARGET_INFO:
		pushDataToManager(buf , ((ReqAttackTargetInfo*)buf)->uEctypeRoomID);
		break;
	case CMD_IS_IN_ECTYPE: //玩家是否在副本
		isInEctype((Notify_Is_In_Ectype*)buf);
		delete [] buf;
		break ;
	case CMD_ENTER_ECTYPE: //玩家进入副本
		createRoom((Notify_Enter_Ectype*)buf);
		delete [] buf;
		break;
	case REQ_ENTER_NPC_SYSTEM_INFO:
		pushDataToManager(buf , ((REQ_ENTER_NPC_SYSTEM_STRUCT*)buf)->EctypeRoomID);
		break;
	case REQ_MOVE_ECTYPE_ARMY_INFO:
		pushDataToManager(buf , ((ReqMoveEctypeArmyInfo*)buf)->uRoomID);
		break;
	case REQ_ECTYPE_GET_ARMY_INFO:
		pushDataToManager(buf , ((ReqEctypeGetArmyInfo*)buf)->EctypeRoomID);
		break;
	case CMD_DEL_ECTYPE_PLAYER:
		delPlayerInfo((MsgEctypeDelPlayer*) buf);
		delete [] buf;
		break;
	case CMD_ECTYPE_FIGHT_OVER:
		processEctypeOver((MsgEctypeFightOver *) buf);
		delete [] buf;
		break;
	case REQ_SET_ECTYPE_ARMY_TRUST_INFO:
		pushDataToManager(buf , ((ReqSetEctypeArmyTrust*)buf)->uEctypeRoomID);
		break;
	case REQ_ECTYPE_DELETE_ARMY_INFO:
		pushDataToManager(buf , ((ReqEctypeDeleteArmy *)buf)->RoomID);
		break;
	case REQ_LEAVE_ECTYPE_SYSTEM_INFO:
		pushDataToManager(buf , ((ReqLeaveEctypeSystem*)buf)->uEctypeRoomID);
		break;
	case REQ_ECTYPE_SYSTEM_ARMY_RETREAT:
		pushDataToManager(buf , ((ReqEctypeRetreatArmyInfo*)buf)->EctypeRoomID);
		break;
	case REQ_USE_PROP_CARD:
		pushDataToManager(buf , ((ReqUsePropCard*)buf)->roomID);
		break;
	case ADD_ARMY_TO_ROOM:
		pushDataToManager(buf , ((ReqAddArmyToRoom*)buf)->roomID);
		break ;
	case EN_C_CONTRIBUTE_INFO:
		delete [] buf;
		break;
	
	default:
		delete [] buf;
		break;
	}
}
bool NetProcesser::initialize(){
	
	//设置fightManager集合的初始大小
	fightManagers.resize(MAX_FIGHT_MANAGER_NUM, NULL);
	managerStates.resize(MAX_FIGHT_MANAGER_NUM , FIGHTMANAGE_NO_CREATE);
	ectypeManager_ = new EctypeManager;
	assert(ectypeManager_ != NULL);

	ectypeManager_->loadEctypeMap();

	dataBaseProxy_ = new DB_Proxy;
	assert(dataBaseProxy_ != NULL);

	if (!dataBaseProxy_->init(g_IniFile.strDBIP.c_str(),static_cast<short>(g_IniFile.m_DBPort) , curProcessID_)){
		cout<<"connect db fail"<<endl;
		return false;
	}
	dataBaseMemData_ = new CLoadDBInfo;
	assert(dataBaseMemData_ != NULL);

	dataBaseMemData_->LoadParam(dataBaseProxy_);

	//初始化副本NPC表
	ectypeNpcInfo_ = new CNPCInfo;
	assert(ectypeNpcInfo_ != NULL);

	ectypeNpcInfo_->InitNpc(dataBaseProxy_);

	fightEvent_ = new FightEvent;
	assert(fightEvent_ != NULL);

	//注册服务器
	if (!init(g_IniFile.strAgentID.c_str(),static_cast<unsigned short>(g_IniFile.m_AgentPort) ,g_IniFile.m_LogSvrIP.c_str() , 
		static_cast<unsigned short>(g_IniFile.m_LogSvrPort) ,curProcessID_,2,data_receiver_callback,event_notify_callback,
		timer_notify_callback,socket_disconn_callback,
		logic_exception_callback))
	{
		cout<<"connect agent fail"<<endl;
		return false;
	}
	freeArmyFromEctype();
	return true;
}
int NetProcesser::getEmptyManager(void) 
{
	MutexGuard<boost::shared_mutex> guard(ectypeMutex_);
	for(int i = 0; i < MAX_FIGHT_MANAGER_NUM; i++){
		if(managerStates[i] == FIGHTMANAGE_NO_CREATE){
			managerStates[i] = FIGHTMANAGE_IS_CREATING;
			return i;
		}
	}
	return -1;
}
void NetProcesser::setManagerState(int roomID,FightManagersState state){
	MutexGuard<boost::shared_mutex> guard(ectypeMutex_);
	managerStates[roomID] = state;
}

void NetProcesser::isInEctype(const Notify_Is_In_Ectype* lpEvent) 
{
	if (lpEvent == NULL){
		return;
	}
	ActivityPlayerPtr playerPtr = ectypeManager_->getActivePlayer(lpEvent->playerID);
	if(playerPtr == NULL){
		fightEvent_->NotifyIsInEctype(0, lpEvent);
		return;
	}
	if (managerStates[playerPtr->fightRoomID_] == FIGHTMANAGE_IS_RUNING){
		fightEvent_->NotifyIsInEctype(1, playerPtr , lpEvent->entranceID);
		return;
	}
	fightEvent_->NotifyIsInEctype(0,lpEvent);
}

void NetProcesser::createRoom(Notify_Enter_Ectype* lpEvent){
	CLoadMapInfo* mapInfo;
	int roomID;
	list<ActivityPlayerPtr> tmpList;
	bool isCreateSuccess = true;
	list<ActivityPlayerPtr>::iterator iter;
	mapInfo= ectypeManager_->getMapInfo(lpEvent->uEctypeID);
	if (mapInfo == NULL){
		fightEvent_->NotifyCreateRoomBack(false , -1 ,lpEvent);
		return ;
	}
	//加锁
	//todo
	roomID = getEmptyManager();
	if (roomID < 0 ) {
		fightEvent_->NotifyCreateRoomBack(false , roomID , lpEvent);
		return ;
	}
	isCreateSuccess = createActPlayerList(tmpList , lpEvent , roomID);
	if (isCreateSuccess){
		fightManagers[roomID] = new EctypeFightManager;
		if (fightManagers[roomID] == NULL){
			isCreateSuccess = false;
		}
		else{
			isCreateSuccess = fightManagers[roomID]->initialize(mapInfo , dataBaseMemData_, ectypeNpcInfo_ ,lpEvent ,roomID);
			if (isCreateSuccess == false){
				fightManagers[roomID]->release();
			}
			else{
				for(iter = tmpList.begin();iter != tmpList.end(); iter++){
					fightManagers[roomID]->insertPlayerInfo(*iter);
				}
			}
		}
	}
	//添加用户到副本管理器中
	if (isCreateSuccess){
		for(iter = tmpList.begin();iter != tmpList.end(); iter++){
			ectypeManager_->addActivePlayer(*iter);
		}
	}
	else{
		ZSBB_DELETE(fightManagers[roomID]);
	}
	tmpList.clear();
	if (isCreateSuccess == false){
		setManagerState(roomID ,FIGHTMANAGE_NO_CREATE);
	}
	else{
		setManagerState(roomID ,FIGHTMANAGE_IS_RUNING);
		fightManagers[roomID]->startTimerCycle();
	}
	fightEvent_->NotifyCreateRoomBack(isCreateSuccess , roomID , lpEvent);
}

void NetProcesser::delPlayerInfo(MsgEctypeDelPlayer * msgEctypeDelPlayer){
	for(int i = 0; i< msgEctypeDelPlayer->playerNum ; i++){
		ectypeManager_->delActivePlayer((*msgEctypeDelPlayer)[i]);
	}
}
void NetProcesser::processEctypeOver(MsgEctypeFightOver * msgEctypeFightOver){

	if (msgEctypeFightOver ==NULL){
		return ;
	}
	int roomID = msgEctypeFightOver->roomID;
	if (isValidRoomID(roomID) == false){
		return ;
	}
	setManagerState(roomID , FIGHTMANAGE_HAS_STOPED);
	FightManager * fightManager = fightManagers[roomID];
	if(fightManager == NULL){
		setManagerState(roomID , FIGHTMANAGE_NO_CREATE);
		return ;
	}
	fightManagers[roomID] = NULL;
	fightManager->stopTask();
	fightManager->release();
	delete fightManager;
	fightManager = NULL;
	ectypeManager_->delActivePlayersByRoomID(roomID);
	setManagerState(roomID , FIGHTMANAGE_NO_CREATE);
}
void NetProcesser::freeArmyFromEctype(){
	ArmyTable tmpArmyTable;
	ArmyTable * pArmyTable = NULL;
	int iLen = 0;
	dataBaseProxy_->db_select_all(tmpArmyTable , (char *&)pArmyTable , iLen);
	iLen /= sizeof(ArmyTable);
	for(int i = 0 ; i < iLen ; i++){
		if(pArmyTable[i].state_ == 11){
			Notify_Back_Army_Ectype Event;
			int count = 0;
			memset(&Event , 0 , sizeof(Notify_Back_Army_Ectype));
			Event.uArmyID[count++] = pArmyTable[i].armyid_; 
			Event.iArmyCount= count;
			Event.uEctypeID = -1;
			Event.serialize(sizeof(Notify_Back_Army_Ectype),0,curProcessID_,pArmyTable[i].mapid_);
			event_notify(pArmyTable[i].mapid_,(char*)&Event,sizeof(Notify_Back_Army_Ectype));
			fightEvent_->NotifyModifyArmyState(pArmyTable[i].playerid_,pArmyTable[i].armyid_ ,pArmyTable[i].mapid_ 
				, pArmyTable[i].firstcount_ ,OUT_CITY_TRUST_CONTROY ,pArmyTable[i].fweapon_);
		}
	}
	if (pArmyTable){
		delete [] (char *)pArmyTable;
		pArmyTable = NULL;
	}
}
bool NetProcesser::isValidRoomID(int roomID) const{
	if (roomID >= ECTYPE_ROOM_NUM || roomID < 0){
		return false;
	}
	return true;
}
bool NetProcesser::createActPlayerList(list<ActivityPlayerPtr>& actPlayerList,Notify_Enter_Ectype* lpEvent,int roomID){
	ActivityPlayerPtr activityPlayer ;
	for (int i = 0 ; i <lpEvent->playerCount; i ++){
		activityPlayer = ActivityPlayerPtr(new ActivityPlayer);
		if (activityPlayer != NULL){
			memset(activityPlayer.get() , 0 , sizeof(ActivityPlayer));
			activityPlayer->playerID_ = lpEvent->getPlayerId(i);
			activityPlayer->fightRoomID_ = roomID;
			activityPlayer->ectypeID_ = lpEvent->uEctypeID;
			PlayerChatRoomTable playerInfo;
			playerInfo.setid(lpEvent->getPlayerId(i));
			if(dataBaseProxy_->db_select(playerInfo) == DB_Proxy::DB_SUCCEED){
				activityPlayer->countryID_ = playerInfo.country_ ;
			}
			else
			{
				LOG_ERROR(("无法查找到玩家%d阵营信息" , lpEvent->getPlayerId(i)));
			}

			actPlayerList.push_front(activityPlayer);

		}
		else{
			actPlayerList.clear();
			return false;
		}
	}
	return true;
}
void NetProcesser::insertPlayer(ActivityPlayerPtr playerPtr){
	ectypeManager_->addActivePlayer(playerPtr);
}
ActivityPlayerPtr NetProcesser::getActivePlayer(unsigned int playerId){
	return ectypeManager_->getActivePlayer(playerId);
}
void NetProcesser::release(){
	if (instance_){
		for (int i = 0; i< ECTYPE_ROOM_NUM ; i++){
			setManagerState(i , FIGHTMANAGE_HAS_STOPED);
			if (fightManagers[i] != NULL){
				fightManagers[i]->stopTask();
				fightManagers[i]->release();
				delete fightManagers[i];
				fightManagers[i] = NULL;
			}
		}
		if (dataBaseProxy_){
			dataBaseProxy_->fini();
			CHECK_DELETE(dataBaseProxy_);
		}
		CHECK_DELETE(ectypeManager_);
		if (dataBaseMemData_){
			dataBaseMemData_->Empty();
			CHECK_DELETE(dataBaseMemData_);
		}
		if (ectypeNpcInfo_)
		{
			ectypeNpcInfo_->empty();
		}
		CHECK_DELETE(ectypeNpcInfo_);
		delete instance_;
		instance_ = NULL;
	}
}
