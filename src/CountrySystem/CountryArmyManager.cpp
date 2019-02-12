
#include "CountryArmyManager.h"
#include "FightManager.h"
#include "FightSystemMacro.h"
#include "PlayerArmy.h"
#include "BuildArmy.h"
#include "NpcArmy.h"
#include "GuarderArmy.h"
#include "NPCInfo.h"
#include "LoadMapInfo.h"
#include "DB_Proxy.h"
#include "LoadDBInfo.h"
#include "CountyQueueMgr.h"
#include "ClearSecuSection.h"
#include "socketsvc.hpp"
#include "log.hpp"
CountryArmyManager::CountryArmyManager(void):
armyQueueManager_(NULL),
secManager_(NULL)
{
}

CountryArmyManager::~CountryArmyManager(void)
{
	
}
//初始化
bool CountryArmyManager::Initialize(CLoadMapInfo *mapInfo ,CNPCInfo * npcInfo ,IniFile * iniFile , 
							 DB_Proxy * proxy ,CLoadDBInfo *dataBaseMemData, FightManager * fightManager){
	mapInfo_ = mapInfo;
	npcInfo_ = npcInfo;
	dataBaseProxy_ = proxy;
	dataBaseMemData_ = dataBaseMemData;
	fightManager_ = fightManager;
	iniFile_ = iniFile;
	armyQueueManager_ = new CountyQueueMgr(mapInfo_,iniFile_ , fightManager_, this);
	if (armyQueueManager_ == NULL){
		return false;
	}
	MapDataStru mapData;
	mapData.MapData = &mapArmyList_;
	mapData.ClassImpedient = mapInfo_;
	secManager_ = new ClearSecuSection(mapData);
	if (secManager_ == NULL){
		return false;
	}
	//初始化地图沙盘
	list<IArmyPtr> tmpList;
	vector<list<IArmyPtr> > tmpVector;
	tmpVector.resize(41, tmpList);
	mapArmyList_.resize(81, tmpVector);


	if (loadBuildInfo() == false){
		return false;
	}
	if(loadArmyInfo() == false){
		return false;
	}
	
	
	return attachPatrolArmy();
}
//获得特定ID的部队
IArmyPtr CountryArmyManager::getArmyByID(unsigned int armyID) const{
	map<unsigned int , IArmyPtr>::const_iterator iter = armyMap_.find(armyID);
	if (iter != armyMap_.end()){
		return iter->second;
	}
	return armyQueueManager_->getArmyPtrOnArmyId(armyID) ;
}

//添加部队到MAP
bool CountryArmyManager::addArmy(IArmyPtr armyPtr){
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

	if (mapInfo_->GetMapState() == war && armyPtr->GetArmyType() != BUILD){
		fightManager_->onEnterWar(armyPtr);
	}
	if (mapInfo_->GetMapOwer() != war && armyPtr->GetCountryID() != mapInfo_->GetMapOwer())
	{
		fightManager_->setLabelMap(armyPtr->GetCountryID());
	}
	if (armyPtr->GetArmyType()!= BUILD){
		if (mapInfo_->GetMapOwer() == armyPtr->GetCountryID())
		{
			defendArmyNum_ ++;
		}
		else
		{
			attackArmyNum_ ++;
		}
	}
	return true;
}

bool CountryArmyManager::checkAddArmy(IArmyPtr armyPtr){
	if (armyPtr == NULL){
		return false;
	}
	if(armyQueueManager_->isDirectEnterCounty(armyPtr) == false){
		return false;
	}
	return addArmy(armyPtr);
}
void CountryArmyManager::eraseArmy(IArmyPtr armyPtr){
	if (armyPtr == NULL){
		return ;
	}
	if (armyMap_.find(armyPtr->GetArmyID()) == armyMap_.end())
	{
		return ;
	}
	vector<POS_STRUCT> tmpPos = armyPtr->GetArmyPos(); 
	size_t Len = tmpPos.size();
	for(size_t i = 0; i < Len ; i++){
		mapArmyList_[tmpPos[i].PosX][tmpPos[i].PosY].remove(armyPtr);
	}
	if (armyPtr->GetArmyType() != BUILD){
		armyMap_.erase(armyPtr->GetArmyID());
		if (mapInfo_->GetMapOwer() == armyPtr->GetCountryID())
		{
			defendArmyNum_ --;
		}
		else
		{
			attackArmyNum_ --;
		}
	}
	
}
void CountryArmyManager::release(){
	map<unsigned int , IArmyPtr>::iterator iter;
	for(iter = armyMap_.begin(); iter != armyMap_.end() ; iter++){
		delete iter->second;
	}
	armyMap_.clear();
	CHECK_DELETE(armyQueueManager_);
	CHECK_DELETE(secManager_);
}

//部队死亡
void CountryArmyManager::onArmyDie(IArmy* attackPtr ,IArmy* army){
	if (army->GetArmyType() == BUILD){
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
		else
		{
			fightManager_->clearAttachedNpc(army , false);
		}
	}
	army->refreshExp();
	eraseArmy(army);
	armyDieMap_.insert(map<unsigned int, IArmyPtr>::value_type(army->GetArmyID() , army));
}
//移动部队
bool CountryArmyManager::moveArmy(IArmyPtr armyPtr , POS_STRUCT & targetPos){
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
//装载部队信息
bool CountryArmyManager::loadArmyInfo(void){
	ArmyTable armyTable;
	armyTable.setmasterid(mapInfo_->getMapID());
	ArmyTable* armyTablePtr = NULL;
	IArmyPtr armyPtr;
	int armyNum ;
	if(dataBaseProxy_->db_multi_select(armyTable,(char *&)armyTablePtr,armyNum) == DB_Proxy::DB_FAILED){
		return false;
	}
	armyNum /= sizeof(ArmyTable);  
	for(int i = 0 ; i < armyNum ; i++)
	{
		if (armyTablePtr[i].state_ == 2){
			continue;
		}
		if (armyTablePtr[i].firstcount_ <= 0){
			dataBaseProxy_->db_delete(armyTablePtr[i]);
			continue;
		}
		armyPtr = createPlayerArmy(armyTablePtr[i]);
		if (armyPtr == NULL){
			continue;
		}
		POS_STRUCT pos = armyPtr->GetArmyPos()[0];
		if(isNewbie()){
			pos = armyPtr->getValidPos(pos , 10 , 0);
		}
		else
		{
			pos = armyPtr->getValidPos(pos , 3 , 0);
		}
		armyPtr->setArmyPos(pos);
		if (armyPtr->isPlayerArmy() == false){
			if (armyPtr->GetCountryID() == static_cast<int>(mapInfo_->GetMapOwer())){
				armyPtr->setStrategy(RecoveryAI);
			}
			armyPtr->setTrusteeshipState(true);
		}
		checkAddArmy(armyPtr);
		if (armyPtr->GetCountryID() != mapInfo_->GetMapOwer()){
			if (mapInfo_->GetMapState() != war)
			{
				mapInfo_->SetMapState(war);
				tagArmy_Reach eventArmyReach;
				eventArmyReach.serialize(armyPtr->getPlayerID(),armyPtr->getFromMapID() ,0,
					mapInfo_->getMapID(),armyPtr->GetArmyID(),0,CMD_ARMY_REACH_OK);
				event_notify(SERVER_BOUND_MAP_OTHER,(char*)&eventArmyReach,sizeof(tagArmy_Reach));
			}
			fightManager_->setLabelMap(armyPtr->GetCountryID());
		}
	}
	if(armyTablePtr){
		delete [] armyTablePtr; 
	}
	return true;
}
//装载NPC部队信息
bool CountryArmyManager::attachPatrolArmy(){

	IArmyPtr govArmyPtr = getArmyByID(mapInfo_->GovPos.uGovID % 10000);
	if (govArmyPtr == NULL){
		return true;
	}
	NpcArmy *  armyPtr;
	NpcArmyParam    npcArmy;
	map<unsigned int , EctypeNpcArmyTable*> mapNpcTable;
	//允许郡中没有NPC
	if (npcInfo_->getEctypeNpc(mapInfo_->getMapID() ,mapNpcTable) == false){
		return true;
	} 
	npcArmy.mapData.ClassImpedient = mapInfo_;
	npcArmy.mapData.MapData = &mapArmyList_;
	bool isEnterSystem;
	map<unsigned int , EctypeNpcArmyTable*>::iterator npcIter;
	for (npcIter = mapNpcTable.begin(); npcIter != mapNpcTable.end() ;npcIter++){
		npcArmy.npcArmyTable = npcIter->second;
		npcArmy.npcArmyInfo = npcInfo_->getEctypeNpcType(npcArmy.npcArmyTable->npcTypeID);

		if (getArmyByID(npcArmy.npcArmyTable->armyID) != NULL){
			continue;
		}

		armyPtr = new NpcArmy;
		if (armyPtr == NULL){
			return false;
		}

		if(armyPtr->Initialize(npcArmy , fightManager_ , County_AI , isEnterSystem) == false){
			return false;
		}
		POS_STRUCT tmpPos = armyPtr->GetArmyPos()[0];
		armyPtr->setBirthPos(tmpPos);
		if (isEnterSystem){
			if(!fightManager_->forceInsertNewArmy(armyPtr)){
				delete armyPtr;
				continue;
			}
		}
		else
		{
			fightManager_->reclaimDeadArmy(armyPtr);
		}
		armyPtr->setLordArmy(govArmyPtr->GetArmyID());
		govArmyPtr->attachArmy(armyPtr->GetArmyID());
	}
	return true;
}


//将list转化为vector
void CountryArmyManager::getVecPosFromList(vector<POS_STRUCT> & vecPos , list<POS_STRUCT> & listPos){
	vecPos.clear();
	list<POS_STRUCT>::iterator iter;
	for (iter = listPos.begin() ; iter != listPos.end() ; iter++){
		vecPos.push_back(*iter);
	}
}
//装载建筑物信息
bool CountryArmyManager::loadBuildInfo(void){

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
	return true;
}

//导出添加部队信息
bool CountryArmyManager::exportAddArmyInfo(IArmyPtr armyPtr ,ARMY_ADD& addArmyInfo){
	if (armyPtr == NULL){
		return false;
	}
	POS_STRUCT armyPos = armyPtr->GetArmyPos()[0];
	addArmyInfo.Army.bMove = false;
	addArmyInfo.Army.MapID = mapInfo_->getMapID();
	addArmyInfo.Army.PosX = armyPos.PosX;
	addArmyInfo.Army.PosY = armyPos.PosY;
	addArmyInfo.Army.Direction = armyPtr->getDirection();
	addArmyInfo.Army.ArmyInfo.ulArmyID = armyPtr->GetArmyID();
	addArmyInfo.Army.ArmyInfo.ulOfficerID = armyPtr->GetOfficerID();
	addArmyInfo.Army.ArmyInfo.ulPlayerID = armyPtr->getPlayerID();
	addArmyInfo.Army.ArmyInfo.Soldier.count = armyPtr->getSoldierNum();
	addArmyInfo.Army.ArmyInfo.Soldier.Race = armyPtr->GetSoldierRace();
	addArmyInfo.Army.ArmyInfo.Soldier.weaponID = armyPtr->GetWeaponID();
	addArmyInfo.Army.ArmyInfo.Soldier.defendID =armyPtr->GetDefendID();
	addArmyInfo.Army.ArmyInfo.iTotalHP = armyPtr->GetCurrentHp();
	addArmyInfo.Army.ArmyInfo.iCurrentHp = armyPtr->GetCurrentHp();
	addArmyInfo.Army.ArmyInfo.heroAttack = armyPtr->GetAttackPower();
	addArmyInfo.Army.ArmyInfo.soldierAttack = armyPtr->GetAttackPower();
	addArmyInfo.Army.ArmyInfo.ArmyType = armyPtr->GetArmyType() ;
	addArmyInfo.Army.ArmyInfo.Role = (ARMY_ROLE)armyPtr->GetCountryID();
	addArmyInfo.Army.ArmyInfo.Soldier.SoldierType = armyPtr->GetArmyOccupation();
	if (armyPtr->GetArmyName() != NULL){
		strcpy(addArmyInfo.Army.ArmyInfo.ArmyName,armyPtr->GetArmyName());
	}
	addArmyInfo.Army.ArmyInfo.ucArmyState = 0;
	return true;
}

//释放特定玩家的部队
int CountryArmyManager::freeArmyFromQueue(unsigned int playerID)
{
	return armyQueueManager_->compelQuitQueue(playerID);
}

void CountryArmyManager::receiveExcessManInCountry(int excessManNum)
{
	if (excessManNum <= 0)
	{
		return;
	}
	map<unsigned int , IArmyPtr>::iterator itBeg = armyMap_.begin();
	map<unsigned int , IArmyPtr>::iterator itEnd = armyMap_.end();
	PlayerArmy * armyPtr = NULL;
	ActivityPlayerPtr activityPlayerPtr;

	for (; itBeg!=itEnd && 0 != excessManNum; )
	{
		armyPtr = dynamic_cast<PlayerArmy *>(itBeg->second);

		if (NULL == armyPtr || armyPtr->GetArmyType() == BUILD ){
			itBeg++;
			continue;
		}

		activityPlayerPtr = fightManager_->getPlayerInfo(armyPtr->getPlayerID());

		if (!activityPlayerPtr)
		{
			excessManNum--;
			const vector<POS_STRUCT> &tmpPos = armyPtr->GetArmyPos(); 
			size_t Len = tmpPos.size();

			for(size_t i = 0; i < Len ; i++)
			{
				mapArmyList_[tmpPos[i].PosX][tmpPos[i].PosY].remove(armyPtr);
			}

			itBeg = armyMap_.erase(itBeg);
			armyQueueManager_->enterQueue(armyPtr);
			if (armyPtr->GetArmyType() != BUILD){
				if (mapInfo_->GetMapOwer() == armyPtr->GetCountryID())
				{
					defendArmyNum_ --;
				}
				else
				{
					attackArmyNum_ --;
				}
			}
		}
		else
		{
			itBeg++;
		}
	}
}

//将部队踢回新手村
void CountryArmyManager::kickExpeditionArmy(int countryID){
	if (isNewbie()){
		return ;
	}
	unsigned int playerID;
	int newBieMapID;
	map<unsigned int ,IArmyPtr>::iterator mapIter ;
	ArmyTable armyTable ;
	IArmyPtr armyPtr = NULL ;
	if (countryID == static_cast<int>(UNION)){
		playerID = 100;
		newBieMapID = 1003;
	}
	else {
		playerID = 200;
		newBieMapID = 1007;
	}
	for (mapIter = armyMap_.begin() ;mapIter != armyMap_.end(); mapIter ++)
	{
		armyPtr = mapIter->second;
		if (armyPtr == NULL){
			continue ;
		}
		if (armyPtr->getPlayerID() == playerID){
			//踢部队到新手村
			armyTable.setmasterid(mapInfo_->getMapID());
			armyTable.setdetailid(armyPtr->GetArmyID());
			if (dataBaseProxy_->db_select(armyTable) != DB_Proxy::DB_SUCCEED){ 
				LOG_ERROR(("获得部队%d信息失败！\r\n" , armyPtr->GetArmyID()));
				continue  ;
			}
			armyTable.firstcount_ = 100;
			if (dataBaseProxy_->db_update(armyTable,armyTable) !=DB_Proxy::DB_SUCCEED){
				LOG_ERROR(("更新部队%d信息失败！\r\n" , armyPtr->GetArmyID()));
				continue ;
			}
			tagArmy_Reach armyReach;
			armyReach.serialize(armyPtr->getPlayerID(), mapInfo_->getMapID() ,0,newBieMapID,armyPtr->GetArmyID() , 0 , CMD_ARMY_REACH);
			event_notify(SERVER_BOUND_MAP_OTHER,(char*)&armyReach,sizeof(armyReach));
		}
	}
	ArmyPtrLst armyList;
	armyQueueManager_->searchAllArmyOfSpeciPlayer(playerID , armyList , true);
	ArmyPtrLst::iterator iter;
	for (iter = armyList.begin(); iter != armyList.end() ; iter ++){
		armyPtr = *iter;
		armyTable.setmasterid(mapInfo_->getMapID());
		armyTable.setdetailid(armyPtr->GetArmyID());
		if (dataBaseProxy_->db_select(armyTable) != DB_Proxy::DB_SUCCEED){ 
			LOG_ERROR(("获得部队%d信息失败！\r\n" , armyPtr->GetArmyID()));
			continue  ;
		}
		armyTable.firstcount_ = 100;
		if (dataBaseProxy_->db_update(armyTable,armyTable) !=DB_Proxy::DB_SUCCEED){
			LOG_ERROR(("更新部队%d信息失败！\r\n" , armyPtr->GetArmyID()));
			continue ;
		}
		tagArmy_Reach armyReach;
		armyReach.serialize(armyPtr->getPlayerID(), mapInfo_->getMapID() ,0,newBieMapID,armyPtr->GetArmyID() , 0 , CMD_ARMY_REACH);
		event_notify(SERVER_BOUND_MAP_OTHER,(char*)&armyReach,sizeof(armyReach));
	}
}


//出队列
int CountryArmyManager::quitQueue(void){
	return armyQueueManager_->quitQueue();
}



void CountryArmyManager::attackOrguardSwopOnWin(int winCountryID){
	if (winCountryID != mapInfo_->GetMapOwer()){
		map<unsigned int , IArmyPtr>::iterator iter;
		IArmyPtr armyPtr;
		for (iter = armyMap_.begin(); iter != armyMap_.end() ; iter ++){
			armyPtr = iter->second;
			if (armyPtr == NULL){
				continue;
			}
			if (armyPtr->GetArmyType() == BUILD){
				BuildArmy * buildArmy = dynamic_cast<BuildArmy*>(armyPtr);
				if (buildArmy != NULL){
					buildArmy->setCountryID(winCountryID);
					if (buildArmy->getBuildDetailType() > ARCH_BUILD_TOFT && 
						buildArmy->getBuildDetailType() < ARCH_TOFT_END)
					{
						buildArmy->armyOnDie(buildArmy , false , false);
					}
				}
			}
		}
		assert(defendArmyNum_ == 0);
		defendArmyNum_ = attackArmyNum_;
		attackArmyNum_ = 0;
	}
	else
	{
		assert(attackArmyNum_ == 0);
	}
	kickExpeditionArmy(winCountryID);
	processDieArmy();
	armyQueueManager_->attackOrguardSwopOnWin(winCountryID);
}

//保存数据到DB
void CountryArmyManager::saveArmyDataToDBG(void){
	IArmyPtr armyPtr;
	ArmyTable * armyTablePtr;
	map<unsigned int , IArmyPtr>::iterator iter;
	for (iter = armyMap_.begin();iter != armyMap_.end();iter ++){
		armyPtr = iter->second;
		if (armyPtr->GetArmyType() == PLAYER_ARMY){
			armyPtr->getArmyTable(armyTablePtr);
			if (armyTablePtr != NULL){
				armyTablePtr->firstcount_ = armyPtr->getSoldierNum();
				armyTablePtr->x = armyPtr->GetArmyPos()[0].PosX;
				armyTablePtr->y = armyPtr->GetArmyPos()[0].PosY;
				if (armyTablePtr->firstcount_ <= 0)
				{
					if(dataBaseProxy_->db_delete(*armyTablePtr) != DB_Proxy::DB_SUCCEED){
						LOG_ERROR(("部队%d删除失败saveArmyDataToDBG\r\n" ,armyTablePtr->armyid_));
					}
					else
					{
						int realState = 0;
						PlayerOfficerTable playerOfficerTable;
						memset(&playerOfficerTable , 0 , sizeof(PlayerOfficerTable));
						playerOfficerTable.setmasterid(armyPtr->getPlayerID());
						playerOfficerTable.setdetailid(armyPtr->GetOfficerID());
						dataBaseProxy_->db_select(playerOfficerTable);
						if(static_cast<unsigned int>(playerOfficerTable.armyid_) != armyPtr->GetArmyID()){
							playerOfficerTable.state_  = -1;
							playerOfficerTable.armyid_ = - (int) armyPtr->GetArmyID();
							dataBaseProxy_->db_increase(playerOfficerTable);
							if(playerOfficerTable.state_ != realState){
								LOG_ERROR(("玩家%d英雄%d实际修改后状态%d要求状态%d错误位置saveArmyDataToDBG\r\n" ,
								playerOfficerTable.playerid_ ,playerOfficerTable.officerid_,playerOfficerTable.state_,realState));
								playerOfficerTable.state_ = realState;
								dataBaseProxy_->db_update(playerOfficerTable , playerOfficerTable);
							}
						}
					}
					continue;
				}
				if(dataBaseProxy_->db_update(*armyTablePtr , *armyTablePtr) != DB_Proxy::DB_SUCCEED){
					LOG_ERROR(("部队%d更新失败saveArmyDataToDBG\r\n" ,armyTablePtr->armyid_));
				}
			}
		}
	}
}

//清扫安全区
void CountryArmyManager::clearSecSection(void){
	secManager_->clearingSecuSection();
}


//获得特定玩家的所有部队
//playerID 玩家ID
//vecArmys 收集后的结果
//countryID 阵营
//isQuitQueue 是否强制出队
void CountryArmyManager::getPlayerArmys(unsigned int playerID , vector<IArmyPtr> & vecArmys ,int countryID, bool isQuitQueue){
	vecArmys.clear();
	map<unsigned int , IArmyPtr>::iterator iter;
	IArmyPtr armyPtr;
	for (iter = armyMap_.begin(); iter != armyMap_.end(); iter++){
		armyPtr = iter->second;
		if (armyPtr->getPlayerID() == playerID){
			vecArmys.push_back(armyPtr);
		}
	}
	ArmyPtrLst armyList;
	armyQueueManager_->searchAllArmyOfSpeciPlayer(playerID , armyList , countryID ,isQuitQueue);
	ArmyPtrLst::iterator listIter;
	for(listIter = armyList.begin(); listIter != armyList.end() ;listIter++){
		vecArmys.push_back(*listIter);
	}
}

//清除伤害 ， 郡战斗结束后 所有受伤单位 自动治愈伤害
void CountryArmyManager::cleanupHurt(void){
	IArmyPtr armyPtr;
	PlayerArmy* playerArmyPtr;
	map<unsigned int , IArmyPtr>::iterator iter;
	for (iter = armyMap_.begin();iter != armyMap_.end();){
		armyPtr = iter->second;
		if (armyPtr == NULL){
			++iter;
			continue;
		}
		playerArmyPtr = dynamic_cast<PlayerArmy*>(armyPtr);
		if (playerArmyPtr && playerArmyPtr->getIsMercenary())
		{
			
			RspDeleteArmyInfo msgDeleteArmy;
			msgDeleteArmy.ArmyID[0] = armyPtr->GetArmyID();
			msgDeleteArmy.count = 1;
			fightManager_->msgBroadcast(&msgDeleteArmy ,RSP_DELETE_ARMY_INFO , sizeof(RspDeleteArmyInfo));
			vector<POS_STRUCT> tmpPos = armyPtr->GetArmyPos(); 
			size_t Len = tmpPos.size();
			for(size_t i = 0; i < Len ; i++){
				mapArmyList_[tmpPos[i].PosX][tmpPos[i].PosY].remove(armyPtr);
			}
		
			iter = armyMap_.erase(iter);
			if (mapInfo_->GetMapOwer() == armyPtr->GetCountryID())
			{
				defendArmyNum_ --;
			}
			else
			{
				attackArmyNum_ --;
			}
			delete armyPtr;
			continue;
		}
		if (armyPtr->GetArmyType() == BUILD){
			BuildArmy * buildArmy = dynamic_cast<BuildArmy *>(armyPtr);
			if (buildArmy != NULL){
				if (armyPtr->GetCurrentHp() == 0)
				{
						computinghurt::BuildingInfo buildInfo;
						PublicBuildParamTable* param = dataBaseMemData_->GetPublicBuildParam(buildArmy->getBuildDetailType(),1);
						if (param == NULL){
							continue;
						}
						DefendAttribute * defendAttribute = dataBaseMemData_->GetDefendAttribute(param->denfenid_);
						if (defendAttribute)
						{
							buildInfo.physicDefense = defendAttribute->physiceRec;
							buildInfo.magicDefense = defendAttribute->magicRec;
						}

						buildInfo.hp = param->hp_;
						buildInfo.power = param->attack_;
						buildArmy->upgrade(buildInfo, param);
						if(buildArmy->getBuildDetailType() != ARCH_BUILD_TOFT){
							insertArmyToList(buildArmy);
						}
						buildArmy->clearHurt();
				}
				fightManager_->clearAttachedNpc(armyPtr , true);
				fightManager_->attachNpc(armyPtr ,buildArmy->getTemplateID() ,buildArmy->getCentryPos() ,County_AI);
			}
		}
		else
		{
			armyPtr->setStrategy(RecoveryAI);
		}
		armyPtr->clearHurt();
		armyPtr->setCountryID(mapInfo_->GetMapOwer());
		++iter;
	}
	attachPatrolArmy();
	
}
//创建防骚扰部队
//mapId 地图ID
//countryId 阵营ID

void CountryArmyManager::createGuardArmy(int mapId , int countryId){
	if (isHaveGMAttack()){
		return ;
	}
	const list<NpcGuarderParam*>* tmpList = npcInfo_->getGuarderParam(mapId , countryId);
	if (NULL == tmpList){
		return ;
	}
	GuarderArmyParam guarderArmyParam;
	GuarderArmy* guarderArmy;

	POS_STRUCT tmpPos;
	guarderArmyParam.mapData.ClassImpedient = mapInfo_;
	guarderArmyParam.mapData.MapData =  &mapArmyList_;
	list<NpcGuarderParam*>::const_iterator iter;
	for (iter = tmpList->begin() ; iter != tmpList->end() ; ++iter){
		
		guarderArmy = new GuarderArmy;
		if (!guarderArmy){
			continue;
		}
		guarderArmyParam.npcArmyTable = *iter;
		guarderArmyParam.npcArmyInfo =  npcInfo_->getEctypeNpcType(guarderArmyParam.npcArmyTable->npcID);
		if (!guarderArmyParam.npcArmyInfo){
			delete guarderArmy;
			guarderArmy = NULL;
			continue;
		}
		if(!guarderArmy->Initialize(guarderArmyParam,fightManager_,County_AI)){
			delete guarderArmy;
			guarderArmy = NULL;
			continue;
		}
		tmpPos = guarderArmy->GetBirthPos();
		tmpPos = guarderArmy->getValidPos(tmpPos);
		guarderArmy->setArmyPos(tmpPos);
		guarderArmy->resetID(fightManager_->getNewArmyID());
		if(!fightManager_->forceInsertNewArmy(guarderArmy)){
			delete guarderArmy;
			guarderArmy = NULL;
			continue;
		}
	}
}
//判断进攻方是否有GM部队  即是否在搞活动
bool CountryArmyManager::isHaveGMAttack() const{
	IArmyPtr armyPtr;
	PlayerArmy* playerArmyPtr;
	map<unsigned int , IArmyPtr>::const_iterator iter;
	for (iter = armyMap_.begin();iter != armyMap_.end();++iter){
		armyPtr = iter->second;
		if (armyPtr == NULL){
			continue;
		}
		playerArmyPtr = dynamic_cast<PlayerArmy*>(armyPtr);
		if (playerArmyPtr)
		{
			if(playerArmyPtr->getPlayerID() == 6 && playerArmyPtr->GetCountryID() != mapInfo_->GetMapOwer()){
				return true;
			}
		}
	}
	return false;
}
