#include "FightEvent.h"
#include "ServerProtocolHeader.h"
#include "socketsvc.hpp"
#include "MapServerProtocolHeader.h"
#include "ServerProtocolHeader.h"
FightEvent::FightEvent(void){

}
FightEvent::~FightEvent(void){

}
int FightEvent::NotifyArmyReached(IArmyPtr armyPtr ,int fromMapID,int midMapID,int toMapID){
	if (armyPtr == NULL){
		return -1;
	}
	tagArmy_Reach armyNotify; 
	armyNotify.serialize(armyPtr->getPlayerID(),fromMapID,midMapID,toMapID,armyPtr->GetArmyID() , 0 , CMD_ARMY_REACH);
	armyNotify.agent = 0;
	armyNotify.from = armyPtr->getPlayerID();
	armyNotify.to = SERVER_BOUND_MAP_OTHER;
	return event_notify(SERVER_BOUND_MAP_OTHER,(char*)&armyNotify,sizeof(tagArmy_Reach)); 		
}
int FightEvent::NotifyGetBox(IArmyPtr armyPtr ,int boxID , USE_PART cardType){
	tagNewTreasurePack_Request TreaPack;
	TreaPack.serialize(armyPtr->getPlayerID(),armyPtr->GetOfficerID(),boxID , cardType);
	return event_notify(SERVER_BOUND_MAP_OTHER,(char*)&TreaPack,TreaPack.length);
}
int FightEvent::NotifyArmyDie(IArmyPtr armyPtr, int mapID , int weaponID){
	//通知大地图队伍挂了
	tagNotify_ModifyArmyState notfiystate;
	notfiystate.serialize(armyPtr->getPlayerID(),armyPtr->GetArmyID(),mapID ,0,ARMY_ANNIHILATE,weaponID);
	return event_notify(MDM_GP_MAP_CONSORTIA,(char*)&notfiystate,sizeof(tagNotify_ModifyArmyState));
}
int FightEvent::NotifyCreateRoomBack(int createState ,int ectypeRoomID , const Notify_Enter_Ectype* lpEvent)
{
	Notify_Back_Enter_Ectype Event;
	Event.RoomID= ectypeRoomID;
	Event.ret = createState;
	Event.uEctypID = lpEvent->uEctypeID;
	Event.uEctypeRoomID[0]= lpEvent->uEctypeRoom[0] ;
	Event.uEctypeRoomID[1]= lpEvent->uEctypeRoom[1] ;
	Event.mapid = lpEvent->uEctypeID;
	Event.entranceID = lpEvent->entranceID;
	Event.serialize(sizeof(Notify_Back_Enter_Ectype),4000,0, MDM_GP_GAME_HALL_SERVER);
	return event_notify(MDM_GP_GAME_HALL_SERVER,(char*)&Event,sizeof(Notify_Back_Enter_Ectype));
}
int FightEvent::NotifyIsInEctype(bool state , const Notify_Is_In_Ectype* lpEvent){
	Notify_Back_Is_In_Ectype Event;
	memset(&Event,0,sizeof(Notify_Back_Is_In_Ectype));
	Event.playerID = lpEvent->playerID;
	Event.ret = state;
	Event.uEctypID = lpEvent->uEctype;
	Event.entranceID = lpEvent->entranceID;
	Event.serialize(sizeof(Notify_Back_Is_In_Ectype));
	return event_notify(MDM_GP_GAME_HALL_SERVER,(char*)&Event,sizeof(Notify_Back_Is_In_Ectype));
}
int FightEvent::NotifyIsInEctype(bool state , ActivityPlayerPtr playerPtr , int entranceID){
	if (playerPtr == NULL){
		return - 1;
	}
	Notify_Back_Is_In_Ectype Event;
	memset(&Event,0,sizeof(Notify_Back_Is_In_Ectype));
	Event.playerID = playerPtr->playerID_;
	Event.ret = state;
	Event.uEctypID = playerPtr->ectypeID_;
	Event.roomid = playerPtr->fightRoomID_;
	Event.entranceID = entranceID;
	Event.serialize(sizeof(Notify_Back_Is_In_Ectype));
	
	return event_notify(MDM_GP_GAME_HALL_SERVER,(char*)&Event,sizeof(Notify_Back_Is_In_Ectype));
}
int FightEvent::NotifyModifyArmyState(unsigned int playerid,unsigned int armyid, int mapid,int armyNum,int state,int weapon){
	tagNotify_ModifyArmyState tag;
	tag.serialize(playerid,armyid,mapid,armyNum,state,weapon);
	return event_notify(SERVER_BOUND_MAP_OTHER,(char*)&tag,sizeof(tagNotify_ModifyArmyState));
}
//Notify_Back_Army_Ectype

int FightEvent::NotifyEctypeArmyBack(Notify_Back_Army_Ectype& ectypeArmysInfo,int fromMapID,int curProcessID){
	ectypeArmysInfo.serialize(sizeof(Notify_Back_Army_Ectype),0,curProcessID,fromMapID);
	return event_notify(fromMapID,(char*)&ectypeArmysInfo,sizeof(Notify_Back_Army_Ectype));
}