#include "MsgCoolingManager.h"
#include "socketsvc.hpp"
#include "FightManager.h"
MsgCoolingManager::MsgCoolingManager(FightManager *fightManager):
fightManager_(fightManager)
{
}

MsgCoolingManager::~MsgCoolingManager(void)
{
}
void MsgCoolingManager::insertCoolingData(CardType cardType , unsigned int armyId , unsigned int coolingTime){
	if (!coolingTime){
		return ;
	}
	CoolingInfo tmpInfo;
	tmpInfo.cardType = cardType;
	tmpInfo.armyId = armyId;
	tmpInfo.coolingTime = coolingTime;
	msgCoolingData_.push_back(tmpInfo);
}
void MsgCoolingManager::sendCoolingData(unsigned int playerId , int agent){
	if (0 >= msgCoolingData_.size()){
		return ;
	}
	MsgPropCardCooling* tmpMsgPtr =(MsgPropCardCooling*) new char[sizeof(MsgPropCardCooling) + sizeof(CoolingInfo) * msgCoolingData_.size()];
	if (tmpMsgPtr == NULL){
		return ;
	}
	list<CoolingInfo>::iterator iter;
	int msgIndex = 0;
	for (iter = msgCoolingData_.begin() ; iter != msgCoolingData_.end() ;++iter)
	{
		(*tmpMsgPtr)[msgIndex++] = *iter;
	}
	tmpMsgPtr->count = msgIndex;
	tmpMsgPtr->serialize(agent,fightManager_->getMapID() , playerId);
	sendMsgToAgent(agent , (char*) tmpMsgPtr , tmpMsgPtr->length);
	delete [] (char*)tmpMsgPtr;
}