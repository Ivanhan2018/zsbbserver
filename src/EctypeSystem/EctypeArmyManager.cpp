#include "EctypeArmyManager.h"

#include "NPCInfo.h"
#include "FightManager.h"
#include "PlayerManager.h"
#include "NpcArmy.h"
#include "PlayerArmy.h"
#include "BuildArmy.h"
#include "log.hpp"
EctypeArmyManager::EctypeArmyManager(void)
{
}

EctypeArmyManager::~EctypeArmyManager(void)
{
}
bool EctypeArmyManager::initPlayerArmy(Notify_Enter_Ectype* lpEvent , list<IArmyPtr>& armyList , map<int ,int>&countryMap , map<unsigned int , unsigned int>& tmpArmyMap){
	if (lpEvent == NULL){
		return false;
	}
	ArmyTable armyTable;
	PlayerArmy *  armyPtr;
	PlayerArmyParam playerArmy;
	POS_STRUCT tmpPos;
	//加载玩家部队
	for (int i = 0 ; i < lpEvent->iArmyCount; i++){
		memset(&playerArmy , 0 , sizeof(PlayerArmyParam));
		armyTable.setmasterid(lpEvent->getArmyInfo(i).mapId);
		armyTable.setdetailid(lpEvent->getArmyInfo(i).armyId);
		int ret = dataBaseProxy_->db_select(armyTable);
		if (ret == DB_Proxy::DB_SUCCEED){
			if(createPlayerArmyParam(playerArmy ,armyTable) ==false){
				return false;
			}
			armyPtr = new PlayerArmy;
			if (armyPtr == NULL){
				return false;
			}
			if(armyPtr->Initialize(playerArmy , fightManager_ ,Counterpart_AI) == false){
				return false;
			}
			//玩家英雄等级限制
			if(!isValidLevel(armyPtr->GetOfficerLevel())){
				return false;
			}

			ActivityPlayerPtr playerInfo = fightManager_->getPlayerInfo(armyPtr->getPlayerID());
			if (playerInfo.get() != NULL){
				playerInfo->countryID_ = armyPtr->GetCountryID();
			}
			countryMap[armyPtr->GetCountryID()] = armyPtr->GetCountryID();
			armyList.push_back(armyPtr);
			tmpArmyMap[armyPtr->GetArmyID()] = armyPtr->GetArmyID();
		}
	}
	return true;
}
bool EctypeArmyManager::insertArmyToSystem(list<IArmyPtr>& armyList)
{
	PlayerArmy *  armyPtr;
	list<IArmyPtr>::iterator iter;
	POS_STRUCT tmpPos;
	for (iter = armyList.begin() ; iter != armyList.end() ; ++iter)
	{
		armyPtr = dynamic_cast<PlayerArmy*>((*iter));
		if (armyPtr == NULL)
		{
			return false;
		}
		tmpPos = mapInfo_->getPropagatePos(fightManager_->getDoorId(armyPtr->GetCountryID()));
		tmpPos = armyPtr->setGatewayArmyPos(tmpPos);
		
		if (IsValidPos(tmpPos.PosX , tmpPos.PosY) == false){
			LOG_ERROR(("初始化，部队信息错误, 副本ID：%d , 来源郡%d\r\n" , 
			fightManager_->getDoorId(armyPtr->GetCountryID()) ,armyPtr->getFromMapID()));
		}
		armyPtr->setBirthPos(tmpPos);
		if(addArmy(armyPtr) == false){
			return false;
		}
	}
	return true;
}
bool EctypeArmyManager::initNpcArmy(int ectypeID , EctypeInfoTable * ectypeInfo){
	NpcArmy *  armyPtr;
	NpcArmyParam    npcArmy;
	map<unsigned int , EctypeNpcArmyTable*> mapNpcTable;
	if (npcInfo_->getEctypeNpc(ectypeID ,mapNpcTable) == false){
		return false;
	} 
	npcArmy.mapData.ClassImpedient = mapInfo_;
	npcArmy.mapData.MapData = &mapArmyList_;

	int npcNum = static_cast<int>(mapNpcTable.size());
	if (ectypeInfo->npcArmyNum > npcNum){
		assert(0 && "副本部队数量少于限定值！");
		return false;
	}
	if (npcNum == 0){
		return true;
	}
	int npcRealNum = randomgenerator_->generator(ectypeInfo->npcArmyNum , npcNum);
	int odds = npcRealNum * 100 / npcNum ;
	bool isEnterSystem;
	map<unsigned int , EctypeNpcArmyTable*>::iterator npcIter;
	for (npcIter = mapNpcTable.begin(); npcIter != mapNpcTable.end() ;npcIter++){
		npcArmy.npcArmyTable = npcIter->second;
		npcArmy.npcArmyInfo = npcInfo_->getEctypeNpcType(npcArmy.npcArmyTable->npcTypeID);
		if (npcArmy.npcArmyTable->isSurely != 1 && randomgenerator_->generator(0, 100) > odds){
			continue;
		}
		armyPtr = new NpcArmy;
		if (armyPtr == NULL){
			return false;
		}

		if(armyPtr->Initialize(npcArmy , fightManager_ , Counterpart_AI , isEnterSystem) == false){
			return false;
		}
		POS_STRUCT tmpPos = armyPtr->GetArmyPos()[0];
		armyPtr->setBirthPos(tmpPos);
		if (isEnterSystem)
		{
			if(addArmy(armyPtr) == false){
				return false;
			}
		}
		else
		{
			fightManager_->reclaimDeadArmy(armyPtr);
		}
		fightManager_->reclaimRepeatArmy(armyPtr);
	}
	return true;
}
IArmyPtr EctypeArmyManager::getArmyByID(unsigned int armyID) const{
	map<unsigned int , IArmyPtr>::const_iterator iter = armyMap_.find(armyID);
	if (iter != armyMap_.end()){
		return iter->second;
	}
	return NULL;
}
bool EctypeArmyManager::isValidLevel(int playerLevel){
	return (playerLevel >= fightManager_->getEctypeDownLevel() && playerLevel <= fightManager_->getEctypeUpLevel()) ; //目前英雄限制为副本等级+-5
}
//装载建筑物信息
bool EctypeArmyManager::loadBuildInfo(void){

	CityInfo cityInfo;
	list<CityInfo>::iterator cityIter;
	vector<POS_STRUCT> buildPos;
	for (cityIter = mapInfo_->m_CityInfo.begin();cityIter != mapInfo_->m_CityInfo.end(); cityIter ++)//载入的城市
	{
		cityInfo =(*cityIter);
		createBuildArmy(cityInfo.uCityID ,cityInfo.CityPos , cityInfo.centryPos);
	}
	list<SentryInfo>::iterator iter;
	SentryInfo sentryInfo;
	for (iter = mapInfo_->sentry_list.begin(); iter != mapInfo_->sentry_list.end() ;iter ++)
	{
		sentryInfo = (*iter);
		list<POS_STRUCT> pos ;
		pos.push_back(sentryInfo.SerTryPos);
		createBuildArmy(sentryInfo.SertryID , pos , sentryInfo.centryPos);
	}
	createBuildArmy(mapInfo_->GovPos.uGovID  , mapInfo_->GovPos.GovPos , mapInfo_->GovPos.centryPos);

	SpecialBuild spBuild;
	vector<SpecialBuild>::iterator spBuildIter;
	for (spBuildIter = mapInfo_->specialBuild_.begin(); spBuildIter != mapInfo_->specialBuild_.end() ; ++spBuildIter){
		spBuild = (*spBuildIter); 
		list<POS_STRUCT> pos ;
		pos.push_back(spBuild.pos);
		createBuildArmy(spBuild.buildID , pos , spBuild.pos);
	}


	list<ResBuild>::iterator resBuildIter;
	ResBuild resBuild;
	for (resBuildIter = mapInfo_->resList.begin(); resBuildIter != mapInfo_->resList.end() ;++resBuildIter)
	{
		resBuild = (*resBuildIter);
		list<POS_STRUCT> pos ;
		pos.push_back(resBuild.SerTryPos);
		createBuildArmy(resBuild.resId , pos , resBuild.centryPos);
	}
	return true;
}
BuildArmy*  EctypeArmyManager::createBuildArmy(unsigned int buildID , list<POS_STRUCT>& pos , const POS_STRUCT &centryPos){

	PublicBuildParamTable* buildParamPtr ;
	PublicBuildTable *buildTable;
	BuildArmy * builArmy;
	vector<POS_STRUCT> buildPos;
	buildTable = npcInfo_->getEctypeBuild(buildID);
	if (buildTable == NULL){
		return NULL;
	}

	buildParamPtr = npcInfo_->getEctypeBuildParam(buildTable->pbuildingkind_,buildTable->currentlevel_);

	if (buildParamPtr){	
		getVecPosFromList(buildPos , pos);
		builArmy = ArmyManager::createBuildArmy(*buildParamPtr , buildPos , buildID % 10000);
		if (builArmy == NULL)
		{
			LOG_ERROR(("加载建筑物%d失败\n\r" ,  buildID));
		}
		else
		{
			builArmy->setBuildID(buildID);
			builArmy->resetCountryID(static_cast<NeutralState>(buildTable->neutraltrap_));
			builArmy->setDetailType(static_cast<ARCH_TYPES>(buildTable->pbuildingkind_));
			builArmy->setCentryPos(centryPos);
			if(addArmy(builArmy) == false){
				LOG_ERROR(("加入建筑物%d失败\n\r" , buildID));
				delete builArmy;
				builArmy = NULL;
			}
			else
			{
				if(fightManager_->attachNpc(builArmy , builArmy->getTemplateID() , builArmy->getCentryPos() ,Counterpart_AI) == false){
					LOG_ERROR(("建筑物%d绑定NPC错误！" , builArmy->GetArmyID()));
				}
			}
			return builArmy;
		}
	}
	
	return NULL;
}