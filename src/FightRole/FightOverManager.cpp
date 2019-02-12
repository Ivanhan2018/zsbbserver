#include "FightOverManager.h"
#include "IArmy.h"
#include "FightManager.h"
FightOverManager::FightOverManager(int limitMaxBoutNum , FightManager * fightManager):
limitMaxBoutNum_(limitMaxBoutNum),
fightManager_(fightManager),
isFightOver_(false),
isDefaultMode_(true),
winCountryID_(NPC_)
{
}

FightOverManager::~FightOverManager(void)
{
}
bool FightOverManager::isFightOver(int& victoryCountryID){
	if (fightManager_->getCurBoutNum() >= limitMaxBoutNum_){
		victoryCountryID = winCountryID_;
		return true;
	}
	if (isDefaultMode_){
		return fightManager_->isFightOver(victoryCountryID);
	}
	if(fightOverConditionArray_.size() == 0){
		victoryCountryID = winCountryID_;
		return true;
	}
	return false;
}
void FightOverManager::setFightOverCondition(FightOverConditionArray & fightOverConditionArray){
	fightOverConditionArray_ = fightOverConditionArray;
	isDefaultMode_ = false;
}
Armys * FightOverManager::getWinner(void){
	return NULL;
}

void FightOverManager::onArmyDie(IArmyPtr attacker , IArmyPtr dieArmy){
	FightOverConditionArray::iterator iter;
	for (iter=fightOverConditionArray_.begin();iter!= fightOverConditionArray_.end();iter++){
		if(iter->cmdType != GET_DROP_ITEM && iter->targetID == static_cast<int>(dieArmy->GetArmyID())){
			winCountryID_ = attacker->GetCountryID();
			if (iter->condition == 1){
				fightOverConditionArray_.clear();
				return ;
			}
			fightOverConditionArray_.erase(iter);
			break;
		}
	}
	
}
void FightOverManager::onGoodsDropped(IArmyPtr attacker, int droppedItem){
	FightOverConditionArray::iterator iter;
	for (iter=fightOverConditionArray_.begin();iter!= fightOverConditionArray_.end();iter++){
		if(iter->cmdType == GET_DROP_ITEM && iter->targetID == droppedItem){
			iter->param--;
			winCountryID_ = attacker->GetCountryID();
			if (iter->param <= 0){
				if (iter->condition == 1){
					fightOverConditionArray_.clear();
					return ;
				}
				fightOverConditionArray_.erase(iter);
			}
			break;
		}
	}
}
void FightOverManager::onUpdateRes(int countryId,int resType , int resNum){
	FightOverConditionArray::iterator iter;
	for (iter=fightOverConditionArray_.begin();iter!= fightOverConditionArray_.end();iter++){
		if(iter->cmdType == GET_RES_LIMIT && iter->targetID == resType){
			winCountryID_ = countryId;
			if (iter->param <= resNum){
				if (iter->condition == 1){
					fightOverConditionArray_.clear();
					return ;
				}
				fightOverConditionArray_.erase(iter);
			}
			break;
		}
	}
}