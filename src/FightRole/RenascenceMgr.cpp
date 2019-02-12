// @brief ��Ҫ����: NPC����������
// @author ����: �� �� ��

#include "RenascenceMgr.h"
#include "NPCInfo.h"
#include "log.hpp"
#include "BitMap.h"
#define  MAX_BASE_ARMY_ID 2000
RenascenceMgr::RenascenceMgr(unsigned int baseStartArmyID , unsigned int baseEndArmyID , 
							 FightManager *fightMgrPtrParam)
							  :fightMgrPtr_(fightMgrPtrParam)
{
	bitMap_ = new BitMap(baseStartArmyID , baseEndArmyID);
}

RenascenceMgr::~RenascenceMgr(void)
{
	try{
		IArmyPtr armyPtr;
		std::map<unsigned int , IArmyPtr>::iterator iter; 
		for (iter = deadArmySet_.begin() ; iter != deadArmySet_.end() ; ++iter){
			armyPtr = iter->second;
			delete armyPtr;
			armyPtr = NULL;
		}
		for (iter = repeatArmySet_.begin() ; iter != repeatArmySet_.end() ; ++iter){
			armyPtr = iter->second;
			if (fightMgrPtr_->getArmyByID(armyPtr->GetArmyID()) == NULL)
			{
				delete armyPtr;
				armyPtr = NULL;
			}
		}
		if(bitMap_){
			delete bitMap_;
			bitMap_ = NULL;
		}
	}
	catch(...)
	{

	}

}

unsigned int RenascenceMgr::getRebornAndReproNpc(ENUM_USE_SCENE state)
{
	return getRebornNpc() + getRepeatNpc(state);
}
//�������NPC
unsigned int RenascenceMgr::getRebornNpc(void){
	DeadArmyMap::iterator iter;
	IArmyPtr armyPtr;
	unsigned int reboAndReproTolNum = 0;
	bool isRevivalTerminate = false;

	for (iter = deadArmySet_.begin() ; iter != deadArmySet_.end() ; ){
		armyPtr = iter->second;
		if(armyPtr->revival(isRevivalTerminate)){
			if(!fightMgrPtr_->forceInsertNewArmy(armyPtr)){
				LOG_ERROR(("�������Ϣʧ�ܣ�λ��getRebornAndReproNpc"));
				iter = deadArmySet_.erase(iter);
				delete armyPtr;
				armyPtr = NULL;
				continue;
			}
			else
			{
				reboAndReproTolNum ++;
				iter = deadArmySet_.erase(iter);
				continue;
			}
		}
		if (isRevivalTerminate){
			delete armyPtr;
			armyPtr = NULL;
			iter = deadArmySet_.erase(iter);
			continue;
		}
		++iter;
	}
	return reboAndReproTolNum;
}
//��ø���NPC
unsigned int RenascenceMgr::getRepeatNpc(ENUM_USE_SCENE state){
	std::map<unsigned int, IArmyPtr>::iterator iter;
	IArmyPtr armyPtr;
	IArmyPtr newArmyPtr;
	unsigned int reboAndReproTolNum = 0;
	bool isRepeatTerminate = false;

	for (iter = repeatArmySet_.begin() ; iter != repeatArmySet_.end() ; ){
		armyPtr = iter->second;
		newArmyPtr = armyPtr->repeat(state , isRepeatTerminate);
		if (newArmyPtr != NULL){
			newArmyPtr->resetID(getNewArmyID());
			if(fightMgrPtr_->forceInsertNewArmy(newArmyPtr) == false){
				LOG_ERROR(("���Ʋ�����Ϣʧ�ܣ�λ��getRebornAndReproNpc"));
				delete newArmyPtr;
				newArmyPtr = NULL;
			}
		}
		
		if (isRepeatTerminate){
			
			iter = repeatArmySet_.erase(iter);
			if(fightMgrPtr_->getArmyByID(armyPtr->GetArmyID()) == NULL){
				delete armyPtr;
				armyPtr = NULL;
			}
			continue;
		}
		++iter;
	}
	return reboAndReproTolNum;
	
}
bool RenascenceMgr::insertDeadArmy(IArmyPtr deadArmyPtr)
{
	if (deadArmyPtr == NULL){
		return false;
	}
	bitMap_->setBitValue(deadArmyPtr->GetArmyID() , false);
	if (deadArmyPtr->isRepeatArmy()){
		return false;
	}
	
	DeadArmyMap::iterator iter;
	iter = deadArmySet_.find(deadArmyPtr->GetArmyID());
	if (iter != deadArmySet_.end())
	{
		return true;
	}

	deadArmySet_.insert(DeadArmyMap::value_type(deadArmyPtr->GetArmyID() , deadArmyPtr));
	return true;
}
unsigned int RenascenceMgr::getNewArmyID(void){
	return bitMap_->getFreeBit();
}
void RenascenceMgr::insertRepeatArmy(IArmyPtr repeatArmyPtr){
	if(!repeatArmyPtr || !repeatArmyPtr->isRepeatArmy()){
		return ;
	}

	std::map<unsigned int, IArmyPtr>::iterator iter;
	iter = repeatArmySet_.find(repeatArmyPtr->GetArmyID());
	if(iter != repeatArmySet_.end()){
		return ;
	}
	repeatArmySet_.insert(std::map<unsigned int, IArmyPtr>::value_type(repeatArmyPtr->GetArmyID() , repeatArmyPtr));
}
void RenascenceMgr::eraseArmy(unsigned int armyID){
	IArmyPtr armyPtr ;
	DeadArmyMap::iterator iter = deadArmySet_.find(armyID);
	if (iter != deadArmySet_.end()){
		armyPtr = iter->second;
		deadArmySet_.erase(iter);
		bitMap_->setBitValue(armyID , false);
		delete armyPtr;
		armyPtr = NULL;
		return;
	}
}
//����Army ID
void RenascenceMgr::reclaimArmyID(unsigned int armyID){
	bitMap_->setBitValue(armyID , false);
}
