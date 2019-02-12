
#include "ArmyManager.h"
#include <boost/serialization/singleton.hpp>
#include "computinghurt/randomgenerator.h"
#include "DB_ProxyHeader.h"
#include "FightSystemMacro.h"
#include "FightManager.h"
#include "PlayerArmy.h"
#include "BuildArmy.h"
#include "NPCInfo.h"
#include "LoadMapInfo.h"
#include "DB_Proxy.h"
#include "LoadDBInfo.h"
#include "log.hpp"
#include "equipment.h"
ArmyManager::ArmyManager(void):
dataBaseProxy_(NULL),
randomgenerator_(NULL),
defendArmyNum_(0),
attackArmyNum_(0)
{
}

ArmyManager::~ArmyManager(void)
{
	release();
}
bool ArmyManager::Initialize(CLoadMapInfo *mapInfo ,CNPCInfo * npcInfo ,IniFile * iniFile , 
							 DB_Proxy * proxy ,CLoadDBInfo *dataBaseMemData, FightManager * fightManager){
	mapInfo_ = mapInfo;
	npcInfo_ = npcInfo;
	dataBaseProxy_ = proxy;
	dataBaseMemData_ = dataBaseMemData;
	fightManager_ = fightManager;
	iniFile_ = iniFile;
	randomgenerator_ = &boost::serialization::singleton<computinghurt::RandomGenerator>::get_mutable_instance();
	MapDataStru mapData;
	mapData.MapData = &mapArmyList_;
	mapData.ClassImpedient = mapInfo_;

	//初始化地图沙盘
	list<IArmyPtr> tmpList;
	vector<list<IArmyPtr> > tmpVector;
	tmpVector.resize(mapInfo->m_iMaxY, tmpList);
	mapArmyList_.resize(mapInfo->m_iMaxX, tmpVector);
	return true;
}
bool ArmyManager::insertArmyToList(IArmyPtr armyPtr){
	vector<POS_STRUCT> tmpPos = armyPtr->GetArmyPos(); 
	size_t Len = tmpPos.size();
	for (size_t i = 0; i < Len ; i++){
		if (mapInfo_->IsValidPos(tmpPos[i].PosX , tmpPos[i].PosY) == false){
			return false;
		}
	} 
	if (armyPtr->GetArmyType() == BUILD)
	{
		for(size_t i = 0; i < Len ; i++){
			mapArmyList_[tmpPos[i].PosX][tmpPos[i].PosY].push_back(armyPtr);
		}
	}
	else
	{
		for(size_t i = 0; i < Len ; i++){
			mapArmyList_[tmpPos[i].PosX][tmpPos[i].PosY].push_front(armyPtr);
		}
	}
	
	fightManager_->insertProcessQueue(armyPtr);
	return true;
}

void ArmyManager::release(){
	map<unsigned int , IArmyPtr>::iterator iter;
	for(iter = armyMap_.begin(); iter != armyMap_.end() ; iter++){
		delete iter->second;
		iter->second = NULL;
	}
	armyMap_.clear();
}
void ArmyManager::processDieArmy(void){
	IArmyPtr army;
	map<unsigned int, IArmyPtr>::iterator iter;
	for (iter = armyDieMap_.begin(); iter != armyDieMap_.end() ; ){
		army = iter->second;
		if (army)
		{
			RSP_DESTROY_ARMY msgArmyDie;
			msgArmyDie.uArmyID = army->GetArmyID();
			msgArmyDie.type = army->GetArmyType();
			fightManager_->msgBroadcast(&msgArmyDie,DESTROY_ARMY_INFO,sizeof(RSP_DESTROY_ARMY));
			if (army->GetArmyType() == PLAYER_ARMY){
				delete army;
				army = NULL;
			}
			else if(army->GetArmyType() != BUILD)
			{
				if(!fightManager_->reclaimDeadArmy(army)){
					delete army;
					army = NULL;
				}
			}
		}
		iter = armyDieMap_.erase(iter);
	}
}
bool ArmyManager::isValidArmy(IArmyPtr armyPtr)const{
	if(armyPtr == NULL){
		return false;
	}
	if (armyPtr->GetCurrentHp() <= 0){
		return false;
	}
	return true;
}
void ArmyManager::onArmyDie(IArmy* attackPtr ,IArmy* army){
	if (army->GetArmyType() == BUILD && fightManager_->getWarType() == HALL_TYPE_INVALID){
		PublicBuildTable buildTable;
		buildTable.setmasterid(mapInfo_->getMapID());
		buildTable.setdetailid(mapInfo_->getMapID()*10000+army->GetArmyID());
		if (dataBaseProxy_->db_select(buildTable)!=DB_Proxy::DB_SUCCEED){
			LOG_ERROR(("select build %d info error\n" , buildTable.pbuildingid_));
		}
		if (army->getBuildDetailType() > ARCH_BUILD_TOFT && army->getBuildDetailType() < ARCH_TOFT_END){
			buildTable.pbuildingkind_ = ARCH_BUILD_TOFT;
		}
		buildTable.currentlevel_ = 1;
		buildTable.currentmoney_ = 0;
		if (dataBaseProxy_->db_update(buildTable,buildTable)!=DB_Proxy::DB_SUCCEED){
			LOG_ERROR(("update build %d info error\n" , buildTable.pbuildingid_));
		}
		dataBaseMemData_->UpdatePublicBuildLevel(buildTable.pbuildingid_,1 , buildTable.pbuildingkind_);
		BuildArmy * buildArmy = dynamic_cast<BuildArmy *>(army);
		if (army->getBuildDetailType() > ARCH_BUILD_TOFT && army->getBuildDetailType() < ARCH_TOFT_END){
			buildArmy->setDetailType(ARCH_BUILD_TOFT);
		}
	}
	army->refreshExp();
	eraseArmy(army);
	armyDieMap_.insert(map<unsigned int, IArmyPtr>::value_type(army->GetArmyID() , army));
}

bool ArmyManager::moveArmy(IArmyPtr armyPtr , POS_STRUCT & targetPos){
	if (armyPtr == NULL){
		return false;
	}
	std::vector<POS_STRUCT> armyPos = armyPtr->GetArmyPos();
	int vecLen = static_cast<int> (armyPos.size());
	for(int i = 0 ; i <  vecLen; i++){
		mapArmyList_[armyPos[i].PosX][armyPos[i].PosY].remove(armyPtr);
	}
	mapArmyList_[targetPos.PosX][targetPos.PosY].push_front(armyPtr);
	armyPtr->setArmyPos(targetPos);
	return true;
}
IArmyPtr ArmyManager::createPlayerArmy(unsigned int armyID){
	ArmyTable armyTable;
	armyTable.setdetailid(armyID);
	armyTable.setmasterid(mapInfo_->getMapID());
	if(dataBaseProxy_->db_select(armyTable) != DB_Proxy::DB_SUCCEED){
		return NULL;
	}
	return createPlayerArmy(armyTable);
}
IArmyPtr ArmyManager::createPlayerArmy(ArmyTable & armyTable){
	PlayerArmyParam armyParam;
	memcpy(&armyParam.armyTable , &armyTable , sizeof(ArmyTable));
	armyParam.defendAttribute = dataBaseMemData_->GetDefendAttribute(armyTable.fdefendtype_);
	armyParam.officerRaceAttribute = dataBaseMemData_->GetOfficerRaceAttr(armyTable.raceid_);
	armyParam.soldierParam = dataBaseMemData_->GetSoldierParam(armyTable.soldiertype_);
	armyParam.weaponAttribute =dataBaseMemData_->GetWeaponAttribute(armyTable.fweapon_);
	PlayerOfficerTable officerTable;
	memset(&officerTable ,0 , sizeof(PlayerOfficerTable)); 
	officerTable.setmasterid(armyTable.playerid_);
	officerTable.setdetailid(armyTable.officerid_);
	if(dataBaseProxy_->db_select(officerTable) != DB_Proxy::DB_SUCCEED){
		return NULL;
	}
	memcpy(&armyParam.playerOfficerTable , &officerTable , sizeof(PlayerOfficerTable)); 
	armyParam.mapData.MapData = &mapArmyList_; 
	armyParam.mapData.ClassImpedient = mapInfo_; 
	PlayerArmy* armyPtr = new PlayerArmy;
	if (armyPtr == NULL){
		return NULL;
	}
	if(armyPtr->Initialize(armyParam ,fightManager_ ,County_AI) == false){
		delete armyPtr;
		armyPtr = NULL;
		return NULL;
	}
	PlayerItemStorageTable storageTable;
	PlayerItemStorageTable * storagePtr = NULL;
	int len = 0;
	storageTable.setmasterid(armyPtr->GetOfficerID() , armyPtr->getPlayerID());

	dataBaseProxy_->db_multi_select(storageTable ,(char *&) storagePtr , len);
	len /= sizeof(PlayerItemStorageTable);
	IEquipment equipment(dataBaseMemData_ , 0);
	computinghurt::SkillInfo skillInfo;
	for(int i = 0; i < len ; i++){
		equipment.resetItem(storagePtr[i].itemid_);
		if(fightManager_->getSkillInfo(skillInfo ,static_cast<SKILL_TYPE>(equipment.getBaseSkill()) , equipment.getBaseSkillLevel())){
			 armyPtr->upgradeOrLearnedSkill(skillInfo);
		}
		if(fightManager_->getSkillInfo(skillInfo ,static_cast<SKILL_TYPE>(equipment.getInalySkill()) , equipment.getInalySkillLevel())){
			armyPtr->upgradeOrLearnedSkill(skillInfo);
		}
	}
	if (storagePtr){
		delete [] (char *)storagePtr;
		storagePtr =  NULL;
	}
	return armyPtr;
}
BuildArmy* ArmyManager::createBuildArmy(PublicBuildParamTable & buildParam ,vector<POS_STRUCT> & buildPos ,unsigned int armyID){
	BuildArmyParam buildArmyParam;
	memcpy(&buildArmyParam.buildParam , &buildParam , sizeof(PublicBuildParamTable));
	buildArmyParam.armyID = armyID;
	buildArmyParam.mapData.MapData = &mapArmyList_; 
	buildArmyParam.mapData.ClassImpedient = mapInfo_; 
	buildArmyParam.armyPos = buildPos;
	buildArmyParam.mapData.MapData = &mapArmyList_; 
	buildArmyParam.mapData.ClassImpedient = mapInfo_; 
	buildArmyParam.defendAttribute = dataBaseMemData_->GetDefendAttribute(buildParam.denfenid_);
	BuildArmy* armyPtr = new BuildArmy;
	if (armyPtr == NULL){
		return NULL;
	}
	if(armyPtr->Initialize(buildArmyParam ,fightManager_ ,County_AI) == false){
		delete armyPtr;
		armyPtr = NULL;
		return NULL;
	}
	return armyPtr;
}

void ArmyManager::getVecPosFromList(vector<POS_STRUCT> & vecPos , list<POS_STRUCT> & listPos){
	vecPos.clear();
	list<POS_STRUCT>::iterator iter;
	for (iter = listPos.begin() ; iter != listPos.end() ; iter++){
		vecPos.push_back(*iter);
	}
}

bool ArmyManager::exportAddArmyInfo(IArmyPtr armyPtr ,ARMY_ADD& addArmyInfo){
	if (armyPtr == NULL){
		return false;
	}
	POS_STRUCT armyPos = armyPtr->GetArmyPos()[0];
	armyPtr->exportArmyInfo(addArmyInfo.Army, mapInfo_->getMapID());
	return true;
}
//添加部队到管理器
bool ArmyManager::addArmy(IArmyPtr armyPtr){
	if(armyPtr == NULL){
		return false;
	}
	if (getArmyByID(armyPtr->GetArmyID()) != NULL){
		return false;
	}
	
	if (armyPtr->getBuildDetailType() != ARCH_BUILD_TOFT)
	{
		if (insertArmyToList(armyPtr) == false){
			return false;
		}
	}

	//插入部队列表
	armyMap_.insert(map<unsigned int,IArmyPtr>::value_type(armyPtr->GetArmyID() , armyPtr));
	if (armyPtr->GetArmyType() != BUILD){
		fightManager_->onEnterWar(armyPtr);
	}
	return true;
}
//将部队从管理器中清除
void ArmyManager::eraseArmy(IArmyPtr armyPtr){
	if (armyPtr == NULL){
		return ;
	}
	vector<POS_STRUCT> tmpPos = armyPtr->GetArmyPos(); 
	size_t Len = tmpPos.size();
	for(size_t i = 0; i < Len ; i++){
		mapArmyList_[tmpPos[i].PosX][tmpPos[i].PosY].remove(armyPtr);
	}
	if (armyPtr->GetArmyType() != BUILD){
		armyMap_.erase(armyPtr->GetArmyID());
	}
	if (armyPtr->isPlayerArmy() && fightManager_->getWarType() != HALL_TYPE_INVALID)
	{
		ReqLeaveEctypeArmyInfo * leaveEctypePtr = (ReqLeaveEctypeArmyInfo *) new char 
			[sizeof(ReqLeaveEctypeArmyInfo) + sizeof(unsigned int)];
		leaveEctypePtr->ucArmyNum = 1; 
		(*leaveEctypePtr)[0] = armyPtr->GetArmyID();

		leaveEctypePtr->serialize(fightManager_->getProcessID() , MDM_GP_GAME_HALL_SERVER) ;
		event_notify(MDM_GP_GAME_HALL_SERVER , (char *) leaveEctypePtr , leaveEctypePtr->length);
		delete [] (char*)leaveEctypePtr;
		leaveEctypePtr = NULL;
	}
	
}
bool ArmyManager::IsValidPos(int x ,int y) const{
	return mapInfo_->IsValidPos(x,y);
}
bool ArmyManager::createPlayerArmyParam(PlayerArmyParam & armyParam , ArmyTable & armyTable)
{
	memcpy(&armyParam.armyTable , &armyTable , sizeof(ArmyTable));
	armyParam.defendAttribute = dataBaseMemData_->GetDefendAttribute(armyTable.fdefendtype_);
	armyParam.officerRaceAttribute = dataBaseMemData_->GetOfficerRaceAttr(armyTable.raceid_);
	armyParam.soldierParam = dataBaseMemData_->GetSoldierParam(armyTable.soldiertype_);
	armyParam.weaponAttribute =dataBaseMemData_->GetWeaponAttribute(armyTable.fweapon_);
	PlayerOfficerTable officerTable;
	memset(&officerTable , 0 , sizeof(PlayerOfficerTable));
	officerTable.setmasterid(armyTable.playerid_);
	officerTable.setdetailid(armyTable.officerid_);
	if(dataBaseProxy_->db_select(officerTable) != DB_Proxy::DB_SUCCEED){
		return false;
	}
	memcpy(&armyParam.playerOfficerTable , &officerTable , sizeof(PlayerOfficerTable)); 
	armyParam.mapData.MapData = getArmyList(); 
	armyParam.mapData.ClassImpedient = mapInfo_; 
	return true;
}
void ArmyManager::attackOrguardSwopOnWin(int /*winCountryID*/){
	return ;
}
//保存数据到DBG
void ArmyManager::saveArmyDataToDBG(void){
	return;
}
//清扫安全区
void ArmyManager::clearSecSection(void){
	return ;
}
int ArmyManager::freeArmyFromQueue(unsigned int /*playerID*/){
	return 0;
}
void ArmyManager::cleanupHurt(void){
	return;
}
void ArmyManager::receiveExcessManInCountry(int /*excessManNum*/){
	return ;
}
void ArmyManager::getPlayerArmys(unsigned int /*playerID*/ , vector<IArmyPtr> & /*vecArmys*/  , int /*countryID*/, bool /*isQuitQueue*/){
	return ;
}
BuildArmy*  ArmyManager::createBuildArmy(unsigned int buildID , list<POS_STRUCT>& pos , const POS_STRUCT &centryPos){

	PublicBuildParamTable* buildParamPtr ;
	PublicBuildTable buildTable;
	BuildArmy * builArmy;
	vector<POS_STRUCT> buildPos;
	buildTable.setmasterid(mapInfo_->getMapID());
	buildTable.setdetailid(buildID);
	if (dataBaseProxy_->db_select(buildTable) == DB_Proxy::DB_SUCCEED)
	{
		buildParamPtr = dataBaseMemData_->GetPublicBuildParam(buildTable.pbuildingkind_,buildTable.currentlevel_);
		
		if (buildParamPtr){	
			getVecPosFromList(buildPos , pos);
			builArmy = createBuildArmy(*buildParamPtr , buildPos , buildID % 10000);
			if (builArmy == NULL)
			{
				LOG_ERROR(("加载建筑物%d失败\n\r" ,  buildID));
			}
			else
			{
				builArmy->setBuildID(buildID);
				builArmy->setCountryID(mapInfo_->GetMapOwer());
				builArmy->setDetailType(static_cast<ARCH_TYPES>(buildTable.pbuildingkind_));
				builArmy->setCentryPos(centryPos);
				if(addArmy(builArmy) == false){
					LOG_ERROR(("加入建筑物%d失败\n\r" , buildID));
					delete builArmy;
					builArmy = NULL;
				}
				else
				{
					if(fightManager_->attachNpc(builArmy , builArmy->getTemplateID() , builArmy->getCentryPos() ,County_AI) == false){
						LOG_ERROR(("建筑物%d绑定NPC错误！" , builArmy->GetArmyID()));
					}
				}
				return builArmy;
			}
		}
	}
	return NULL;
}
bool ArmyManager::isExpeditionArmy(IArmyPtr armyPtr){
	if (armyPtr == NULL){
		return false;
	}
	if (armyPtr->getPlayerID() == 100 || armyPtr->getPlayerID() == 200){
		return true;
	}
	return false;
}
bool ArmyManager::isNewbie(){
	if (mapInfo_ == NULL){
		return false;
	}
	if (mapInfo_->getMapID() == 1003 || mapInfo_->getMapID() == 1007)
	{
		return true;
	}
	return false;
}
void ArmyManager::freeArmyList(list<IArmyPtr> & armyList){
	list<IArmyPtr>::iterator iter;
	for (iter = armyList.begin() ; iter != armyList.end() ; ++iter)
	{
		delete (*iter);
		(*iter)  = NULL;
	}
	armyList.clear();
}