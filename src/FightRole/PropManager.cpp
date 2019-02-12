#include "PropManager.h"
#include "FightManager.h"
#include "MsgCoolingManager.h"

PropCard::PropCard(FightManager * fightManager ,IArmyPtr armyPtr, PropCardConfig * propCardPtr):
fightManager_(fightManager),
armyPtr_(armyPtr),
propCardPtr_(propCardPtr),
effectId_(0)
{
	startBout_ = fightManager_->getCurBoutNum();
	
}

PropCard::~PropCard(void){

}
bool PropCard::isForceOff(void){
	return fightManager_->getCurBoutNum() >= startBout_ + propCardPtr_->lastTime;
}
void PropCard::useCard(void){
	if (propCardPtr_->effect != 0){
		effectId_ = armyPtr_->useEffect(static_cast<ENUM_SPECIALLY_EFFECT>(propCardPtr_->effect) 
			, NULL , propCardPtr_->lastTime);
	}
}
void PropCard::clearCard(void){
	if (propCardPtr_->cardType == SPEEDUP_CARD){
		armyPtr_->addMoveSpeed(-propCardPtr_->cardValue);
	}
	if (effectId_ != 0){
		fightManager_->clearEffect(effectId_);
	}
}

PropManager::PropManager(FightManager* fightManager):
fightManager_(fightManager)
{
}

PropManager::~PropManager(void)
{
}
bool PropManager::isCanUserCard(IArmyPtr armyPtr , PropCardConfig * propCardPtr){
	if (propCardPtr == NULL || armyPtr == NULL){
		return false;
	}
	if(armyPtr->isCanUseCard(static_cast<CardType>(propCardPtr->cardType)) == false)
	{
		return false;
	}
	int limitNum = 0;
	switch(fightManager_->getWarType()){
	case COUNTRYPART:
		limitNum = propCardPtr->ectypeLimit;
		break;
	case FIELD_COUNTRYPART:
		limitNum = propCardPtr->fieldLimit;
		break;
	case LIMIT_BATTLEFIELD:
	case UNLIMIT_BATTLEFIELD:
		limitNum = propCardPtr->warLimit;
	default:
		if (fightManager_->getMapState() != war && propCardPtr->isPeaceCanUse > 0)
		{
			limitNum = propCardPtr->countryLimit;
		}
		if(fightManager_->getMapState() == war && propCardPtr->isWarCanUse > 0){
			limitNum = propCardPtr->countryLimit;
		}
	}
	if (limitNum <= 0){
		return false;
	}
	map<unsigned int , map<CardType , int>>::iterator iter;
	iter = playerUsePropRecord_.find(armyPtr->getPlayerID());
	if (iter == playerUsePropRecord_.end()){
		return true;
	}
	map<CardType , int>::iterator subIter = iter->second.find(static_cast<CardType>(propCardPtr->cardType));
	if (subIter == iter->second.end()){
		return true;
	}
	if(subIter->second >= limitNum){
		return false;
	}
	map<unsigned int,map<CardType,map<unsigned int , int>>>::iterator tmpMainIter;
	tmpMainIter = cardCoolingRecord_.find(armyPtr->getPlayerID());
	if (tmpMainIter == cardCoolingRecord_.end()){
		return true;
	}
	map<CardType,map<unsigned int , int>>::iterator tmpIter = tmpMainIter->second.find(static_cast<CardType>(propCardPtr->cardType));
	if (tmpIter == tmpMainIter->second.end()){
		return true;
	}
	map<unsigned int , int>::iterator tmpSubIter;
	unsigned int armyId = 0; 
	if (propCardPtr->isArmyLimitUse){
		armyId = armyPtr->GetArmyID();
	}
	tmpSubIter = tmpIter->second.find(armyId);
	if (tmpSubIter == tmpIter->second.end()){
		return true;
	}
	return tmpSubIter->second <= fightManager_->getCurBoutNum();
}
void PropManager::useCard(IArmyPtr armyPtr , PropCardConfig * propCardPtr){
	if (propCardPtr == NULL || armyPtr == NULL){
		return ;
	}
	armyPtr->useCard(propCardPtr);
	playerUsePropRecord_[armyPtr->getPlayerID()][static_cast<CardType>(propCardPtr->cardType)] ++;
	unsigned int armyId = 0; 
	if (propCardPtr->isArmyLimitUse){
		armyId = armyPtr->GetArmyID();
	}
	cardCoolingRecord_[armyPtr->getPlayerID()][static_cast<CardType>(propCardPtr->cardType)][armyId] = propCardPtr->coolingTime + fightManager_->getCurBoutNum();
}
void PropManager::clearCardRecord(void){
	playerUsePropRecord_.clear();
	cardCoolingRecord_.clear();
}

PropCard * PropManager::getNewCard(IArmyPtr armyPtr ,  PropCardConfig * propCardPtr){
	if (armyPtr == NULL || propCardPtr == NULL){
		return NULL;
	}
	PropCard * tmpPtr = new PropCard(fightManager_ ,armyPtr, propCardPtr);
	return tmpPtr;
}
void PropManager::exportCardRecord(unsigned int playerId , int agent) const{
	map<unsigned int,map<CardType,map<unsigned int,int>>>::const_iterator tmpMainIter = cardCoolingRecord_.find(playerId);
	if (tmpMainIter == cardCoolingRecord_.end()){
		return ;
	}
	const map<CardType,map<unsigned int , int>>&tmpMap = tmpMainIter->second;
	map<CardType,map<unsigned int , int>>::const_iterator iter;
	map<unsigned int , int>::const_iterator subIter;
	MsgCoolingManager msgManager(fightManager_);
	for (iter = tmpMap.begin(); iter != tmpMap.end() ;++iter){
		const map<unsigned int , int>&tmpSubMap = iter->second;
		for (subIter = tmpSubMap.begin();subIter != tmpSubMap.end();++subIter){
			msgManager.insertCoolingData(iter->first , subIter->first , subIter->second - fightManager_->getCurBoutNum());
		}
	}
	msgManager.sendCoolingData(playerId,agent);
}
