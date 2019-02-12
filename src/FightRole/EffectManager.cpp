#include "EffectManager.h"
#include "FightManager.h"
EffectManager::EffectManager(void):
effectId_(1)
{
}

EffectManager::~EffectManager(void)
{
}
void EffectManager::tidyEffect(int curBoutNum){
	IArmyPtr armyPtr;
	IArmyPtr recvArmyPtr;
	map<unsigned int ,EffectDetail>::iterator iter;
	for (iter = effectMap_.begin() ; iter != effectMap_.end() ; ){
		if(iter->second.endBout_ <= curBoutNum){
			armyPtr = fightManager_->getArmyByID(iter->second.useArmyId);
			if (armyPtr){
				recvArmyPtr = fightManager_->getArmyByID(iter->second.receiveArmyId);
				if (recvArmyPtr)
				{
					MsgChangeCountry tmpMsg;
					tmpMsg.armyId = recvArmyPtr->GetArmyID();
					tmpMsg.countryId = armyPtr->GetCountryID();
					tmpMsg.serialize(fightManager_->getProcessID() , 0);
					fightManager_->msgBroadcast(&tmpMsg ,MSG_CHANGE_COUNTRY , tmpMsg.length);
					recvArmyPtr->setCountryID(armyPtr->GetCountryID());
					recvArmyPtr->SetLastEnmityTarget(0);
				}
				armyPtr->clearEffectByType(iter->second.effectType ,false);
			}
			
			iter = effectMap_.erase(iter);
		}
		else{
			++iter;
		}
	}
}
bool EffectManager::clearEffect(unsigned int effectId){
	map<unsigned int ,EffectDetail>::iterator iter = effectMap_.find(effectId);
	if (iter == effectMap_.end()){
		return false;
	} 
	if (iter->second.endBout_ <= fightManager_->getCurBoutNum())
	{
		effectMap_.erase(iter);
		return false;
	}
	effectMap_.erase(iter);
	return true;
}