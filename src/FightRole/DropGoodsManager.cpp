#include "DropGoodsManager.h"
#include <time.h>
#include <math.h>
#include "FightManager.h"
#include "DB_ProxyHeader.h"
#include "log.hpp"

DropGoodsManager::DropGoodsManager(FightManager * fightManager)
:fightManager_(fightManager)
{
	lastSaveTime_ = time(NULL);
}

DropGoodsManager::~DropGoodsManager(void)
{
}
bool DropGoodsManager::processLimit(LIMIT_TYPE limttType , int limitKey){
	LimitTable limitValue; //物品限制表
	limitValue.setmasterid(limttType);
	limitValue.setdetailid(limitKey);
	DB_Proxy *proxy = fightManager_->getCurDBGProxy();
	if (proxy == NULL){
		return false;
	}
	if(proxy->db_select(limitValue) != DB_Proxy::DB_SUCCEED){
		return false;
	}
	if (limitValue.limitval_  >= fightManager_->getLimitValue(limttType,limitKey))
	{
		return false;
	}
	return true;
}
bool DropGoodsManager::processTimeLimit(int& index){
	time_t tnow;
	tm *now;
	tnow = time(NULL);
	now = localtime(&tnow);
	index =  (now->tm_hour >> 1) + 1;
	return processLimit(TIME_LIMIT_TYPE ,index);
}
bool DropGoodsManager::processCountryLimit(int countryID){
	return processLimit(COUNTRY_LIMIT_TYPE , countryID);
}
bool DropGoodsManager::processTotalLimit(void){
	return processLimit(TOTAL_LIMIT_TYPE, 0);
}
bool DropGoodsManager::processCardLevelLimit(int cardLevel){
	return processLimit(CARD_LEVEL_LIMIT_TYPE, cardLevel);
}

bool DropGoodsManager::processArmyCardDrop(IArmyPtr attackArmy , IArmyPtr army ,int armyCardID , int odds){
	
	if(attackArmy->isPlayerArmy() == false){
		return false;
	}
	odds =  static_cast<int>(odds * sqrt(static_cast<float>(attackArmy->getArmyCardDropOdds()) / army->getArmyCardDropOdds()));
	if(fightManager_->generator(0,100) >= odds){
		return false;
	}

	int fromMapID = attackArmy->getFromMapID();
	int cardLevel = fightManager_->getArmyCardLevel(armyCardID);
	int timeIndex = -1;
	if(processCountryLimit(fromMapID) == false){
		return false;
	}
	if (processCardLevelLimit(cardLevel) == false)
	{
		return false;
	}
	if (processTimeLimit(timeIndex) == false)
	{
		return false;
	}
	if (processTotalLimit() == false){
		return false;
	}
	if(!fightManager_->processBooty(attackArmy , armyCardID , ARMYBRAND)){
		return false;
	}
	increaseLimit(TOTAL_LIMIT_TYPE , 0); 
	increaseLimit(CARD_LEVEL_LIMIT_TYPE , cardLevel);
	increaseLimit(COUNTRY_LIMIT_TYPE,fromMapID);
	increaseLimit(TIME_LIMIT_TYPE , timeIndex);
	return true;
}
void DropGoodsManager::processPlayerDrop(IArmyPtr attackArmy , IArmyPtr army){
	if (army->isPlayerArmy() == false)
	{
		return ;
	}
	int cardLevel = getArmyCardLevel(army);
	if (cardLevel == 0){
		return ;
	}
	int odds =  static_cast<int>(20 * sqrt(static_cast<float>(attackArmy->getArmyCardDropOdds()) / army->getArmyCardDropOdds()));
	if(fightManager_->generator(0,100) >= odds){
		return ;
	}
	int armyCardID  = fightManager_->getArmyCardItem(cardLevel);
	if (armyCardID <= 0 ){
		return ;
	}
	if (!processArmyCardDrop(attackArmy , army , armyCardID , 20)){
		return ;
	}
	MsgObtainBooty msgObtainBooty;
	strcpy(msgObtainBooty.winnerName , attackArmy->getPlayerName().c_str());
	strcpy(msgObtainBooty.name , army->getPlayerName().c_str());
	msgObtainBooty.itemID = armyCardID;
	msgObtainBooty.serialize(0, fightManager_->getProcessID() , 0);
	fightManager_->msgBroadcast(&msgObtainBooty , BROAD_MSG_OBTAIN_BOOTY , sizeof(MsgObtainBooty));
}
int DropGoodsManager::getArmyCardLevel(IArmyPtr army) const{
	if (army == NULL || army->isPlayerArmy() == false){
		return 0;
	}
	DB_Proxy* proxy = fightManager_->getCurDBGProxy();
	if (proxy == NULL){
		return 0;
	}
	PlayerOfficerTable playerOfficer;
	playerOfficer.setmasterid(army->getPlayerID());
	playerOfficer.setdetailid(army->getPlayerID());
	if(proxy->db_select(playerOfficer) != DB_Proxy::DB_SUCCEED){
		return 0;
	}
	return static_cast<int>(playerOfficer.militaryrank_ * 0.25 + army->GetOfficerLevel() * 0.05 + 0.25);
}
void DropGoodsManager::resetArmyCardDropRecord(void){

	tm * tmpTm = localtime(&lastSaveTime_);
	int day = tmpTm->tm_mday;
	time_t now = time(NULL);
	tmpTm = localtime(&now);
	if (!(day - tmpTm->tm_mday)){
		return;
	}
	lastSaveTime_ = now;
	LimitTable limitInfo;
	LimitTable * limitPtr = NULL;
	int infoNum = 0;
	DB_Proxy* proxy = fightManager_->getCurDBGProxy();
	if (proxy == NULL){
		return ;
	}
	if(proxy->db_select_all(limitInfo , (char *&)limitPtr , infoNum) != DB_Proxy::DB_SUCCEED){
		return ;
	}
	infoNum /= sizeof(LimitTable);
	for (int i = 0 ; i < infoNum ; i++){
		limitPtr[i].limitval_ = 0;
		if(proxy->db_update(limitPtr[i] , limitPtr[i]) != DB_Proxy::DB_SUCCEED){
			LOG_ERROR(("重设军牌限制信息失败！"));
		}
	}
	CHECK_DELETE(limitPtr);
}

bool DropGoodsManager::increaseLimit(LIMIT_TYPE limttType ,int limitKey){
	LimitTable limitValue;
	limitValue.setmasterid(limttType);
	limitValue.setdetailid(limitKey);
	DB_Proxy *proxy = fightManager_->getCurDBGProxy();
	if (proxy == NULL){
		return false;
	}
	limitValue.limitval_ = 1;
	if(proxy->db_increase(limitValue) != DB_Proxy::DB_SUCCEED){
		return false;
	}
	return true;
}