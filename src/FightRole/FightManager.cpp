#include "FightManager.h"
#include "log.hpp"
#include "IniFile.h"
#include "FightSystemMacro.h"
#include "MapServerProtocolHeader.h"
#include "ChargeProtocol.h"
#include "Time_Protocol.h"
#include "LoadDBInfo.h"
#include "ArmyManager.h"
#include "PlayerManager.h"
#include "FightOverManager.h"
#include "FightEvent.h"
#include "RenascenceMgr.h"
#include "NpcArmy.h"
#include "PlayerArmy.h"
#include "AttachedNpcArmy.h"
#include "DropGoodsManager.h"

float FightManager::lossRate_ = static_cast<float>(0.2);
FightManager::FightManager(void):
playerManager_(NULL),
armyManager_(NULL),
dataBaseProxy_(NULL),
curBoutNum_(0),
curProcessQueue_(NULL),
processQueue_(NULL),
isForceExit_(false),
curCycleNum_(0),
fightOverManager_(NULL),
fightRecord_(NULL),
patrolMap_(NULL),
renascenceMgr_(NULL),
seekPather_(NULL),
dropGoodsManager_(NULL),
processedNum_(0),
warType_(HALL_TYPE_INVALID),
mapDataAndFraise_()
{
}
FightManager::~FightManager(void)
{
}
//获得玩家名称
string FightManager::getPlayerName(unsigned int playerID , unsigned int officerID){
	PlayerOfficerTable officertable;
	officertable.setmasterid(playerID);
	officertable.setdetailid(officerID);
	int ret = dataBaseProxy_->db_select(officertable);
	if (ret ==DB_Proxy::DB_SUCCEED){
		return officertable.officername_;
	}
	return string();
}
//获得玩家名称
string FightManager::getCropsName(unsigned int playerID){
	PlayerChatRoomTable playerChatRoomTable;
	playerChatRoomTable.setid(playerID);
	int ret = dataBaseProxy_->db_select(playerChatRoomTable);
	if (ret !=DB_Proxy::DB_SUCCEED){
		return string();
	}
	ConsortiaInfo consortiaInfo;
	consortiaInfo.setid(playerChatRoomTable.consortiaid_);
	ret = dataBaseProxy_->db_select(consortiaInfo);
	if (ret !=DB_Proxy::DB_SUCCEED){
		return string();
	}
	return consortiaInfo.name_;
}
//获得AI等级参数
aistrategy::AiLevelParam * FightManager::getAiLevel(int aiLevel){
	return npcInfo_->getAiLevelParam(aiLevel);
}
//获得巡逻路径
PatrolPosVect * FightManager::getPatrolData(unsigned int armyID){
	map<unsigned int , PatrolPosVect*>::iterator iter;
	if (patrolMap_){
		iter = patrolMap_->find(armyID);
		if (iter != patrolMap_->end()){
			return iter->second;
		}
	}
	return NULL;
}
ActivityPlayerPtr FightManager::getPlayerInfo(unsigned int playerID) const{
	return playerManager_->getActivePlayer(playerID);
}
//移动部队在沙盘上的位置
bool FightManager::moveArmy(IArmyPtr armyPtr , POS_STRUCT & targetPos){
	return armyManager_->moveArmy(armyPtr , targetPos);
}

//处理玩家部队死亡
bool FightManager::processPlayerArmyDie(IArmy* armyPtr , int playerState , int realState){
	
	PlayerArmy * playerArmy = dynamic_cast<PlayerArmy*>(armyPtr);
	if(playerArmy == NULL || playerArmy->getIsMercenary()){
		return false;
	}
	fightEvent_->NotifyArmyDie(armyPtr , mapInfo_->getMapID() , armyPtr->GetWeaponID());
	ArmyTable * armyTable = NULL;
	armyPtr->getArmyTable(armyTable);
	if (armyTable == NULL){
		return false;
	}
	dataBaseProxy_->db_delete(*armyTable);

	if (armyPtr->isPlayerArmy() == false){
		return true;
	}
	PlayerOfficerTable playerOfficerTable;
	memset(&playerOfficerTable , 0 , sizeof(PlayerOfficerTable));
	playerOfficerTable.setdetailid(armyPtr->GetOfficerID());
	playerOfficerTable.setmasterid(armyPtr->getPlayerID());
	playerOfficerTable.state_ = playerState;
	if (dataBaseProxy_->db_increase(playerOfficerTable) != DB_Proxy::DB_SUCCEED){
		return false;
	}
	if(playerOfficerTable.state_ != realState){
		//
		LOG_ERROR(("玩家%d英雄%d实际修改后状态%d要求状态%dprocessPlayerArmyDie\r\n" ,playerOfficerTable.playerid_ ,
			playerOfficerTable.officerid_,playerOfficerTable.state_,realState));
		playerOfficerTable.state_ = realState;
		dataBaseProxy_->db_update(playerOfficerTable , playerOfficerTable);
	}
	return true;
}
//处理缴获物品
bool FightManager::processBooty(IArmy * armyPtr , int dropItem , USE_PART cardType){
	if(armyPtr == NULL){
		return false;
	}
	if (armyPtr->isPlayerArmy() == false){
		return false;
	}
	if (cardType == TREASUREPACK)
	{
		fightRecord_->onObtainBox(armyPtr , 1);
	}
	fightOverManager_->onGoodsDropped(armyPtr , dropItem);
	tagNewTreasurePack_Request TreaPack;
	TreaPack.serialize(armyPtr->getPlayerID(),armyPtr->GetOfficerID(),dropItem , cardType);
	event_notify(SERVER_BOUND_MAP_OTHER,(char*)&TreaPack,TreaPack.length);
	RspGetRse msgGetBox;
	msgGetBox.ItemId = dropItem;
	msgGetBox.ResNum = 1;
	ActivityPlayerPtr activePlayer = getPlayerInfo(armyPtr->getPlayerID());
	if (activePlayer == NULL || activePlayer->isStayRoom == false){
		return false;
	}
	msgGetBox.serialize(activePlayer->agent_,curProcessID_,activePlayer->playerID_);
	sendMsgToAgent(activePlayer->agent_,(char*)&msgGetBox,msgGetBox.length);
	return true;
}

//通知大地图玩家部队状态修改
int FightManager::notifyModifyArmyState(unsigned int playerid,unsigned int armyid, int mapid,int armyNum,int state,int weapon){
	return fightEvent_->NotifyModifyArmyState(playerid , armyid ,mapid ,armyNum ,state ,weapon);
}

//是否有效部队
bool FightManager::isValidArmy(IArmyPtr armyPtr) const{
	if(armyPtr == NULL){
		return false;
	}
	if (armyPtr->GetCurrentHp() == 0){
		return false;
	}
	return true;
}
//判断战斗是否结束
bool FightManager::isFightOver(int& victoryCountryID){
	const map<unsigned int , IArmyPtr>& armyMap = armyManager_->getArmyMap();
	map<unsigned int , IArmyPtr>::const_iterator iter;
	IArmyPtr armyPtr;
	if (isForceExit_ == true){
		return true;
	}
	victoryCountryID = -1;
	for(iter = armyMap.begin() ;iter != armyMap.end() ;iter ++){
		armyPtr = iter->second;
		if (armyPtr == NULL || armyPtr->GetCurrentHp() == 0 ){
			continue;
		}
		if (armyPtr->getBuildDetailType() >= ARCH_BUILD_TOFT && armyPtr->getBuildDetailType() <= ARCH_TOFT_END){
			continue;
		}
		if (victoryCountryID == -1){
			victoryCountryID = armyPtr->GetCountryID();
		}
		if(victoryCountryID != armyPtr->GetCountryID()){
			return false;
		}
	}
	if (victoryCountryID == -1)
	{
		victoryCountryID = mapInfo_->GetMapOwer();
	}
	return true;
}
bool FightManager::isInWater(const IArmy * armyPtr) const {
	if(armyPtr == NULL){
		return false;
	}
	POS_STRUCT armyPos = armyPtr->GetArmyPos()[0];
	if (mapInfo_->MapInfo[armyPos.PosX][armyPos.PosY] == 1){
		return true ;
	}
	return false;
}
//是否在森林中
bool FightManager::isInForest(const IArmy * armyPtr) const {
	if(armyPtr == NULL){
		return false;
	}
	POS_STRUCT armyPos = armyPtr->GetArmyPos()[0];
	if (mapInfo_->MapInfo[armyPos.PosX][armyPos.PosY] == 2){
		return true ;
	}
	return false;
}

//插入处理队列
void FightManager::insertProcessQueue(IArmyPtr armyPtr , bool isDirectInsert){
	if (isDirectInsert){
		processQueue_->push(armyPtr->GetArmyID());
		return ;
	}
	std::map<unsigned int , bool>::iterator iter = isInProcessQueue_.find(armyPtr->GetArmyID());
	if (iter != isInProcessQueue_.end()){
		return ;
	}
	processQueue_->push(armyPtr->GetArmyID());
	isInProcessQueue_[armyPtr->GetArmyID()] = true;
}
//从处理队列中弹出
void FightManager::eraseProcessQueue(unsigned int armyID){
	isInProcessQueue_.erase(armyID);
}
DB_Proxy * FightManager::getNewDbgProxy(){
	DB_Proxy* dataBaseProxy = new DB_Proxy;
	if (dataBaseProxy == NULL){
		return NULL;
	}
	if (!dataBaseProxy->init(iniFile_->strDBIP.c_str(), static_cast<short>(iniFile_->m_DBPort) , curProcessID_)){
		cout<<"connect db fail"<<endl;
		delete dataBaseProxy;
		dataBaseProxy = NULL;
		return NULL;
	}
	return dataBaseProxy;
}

void FightManager::insertPlayerInfo(ActivityPlayerPtr activityPlayer){
	playerManager_->addActivePlayer(activityPlayer);
}
//处理强制删除部队
void FightManager::processDelArmy(IArmyPtr armyPtr , ARMY_TRUST_STATE state){

	armysMap_.erase(armyPtr->GetArmyID());
	fightOverManager_->onArmyDie(NULL , armyPtr);
	fightRecord_->onArmyDie(armyPtr);
	armyPtr->refreshExp();
	RspDeleteArmyInfo msgDeleteArmy;
	msgDeleteArmy.ArmyID[0] = armyPtr->GetArmyID();
	msgDeleteArmy.count = 1;
	msgBroadcast(&msgDeleteArmy ,RSP_DELETE_ARMY_INFO , sizeof(RspDeleteArmyInfo));

	if (armyPtr->isPlayerArmy()){
		fightEvent_->NotifyModifyArmyState(armyPtr->getPlayerID() , armyPtr->GetArmyID() , 
			armyPtr->getFromMapID() , 0, state , armyPtr->GetWeaponID());
	}
	if (armyManager_->isExpeditionArmy(armyPtr)){
		fightEvent_->NotifyArmyDie(armyPtr , mapInfo_->getMapID() , armyPtr->GetWeaponID());
	}
	eraseArmy(armyPtr);
	if (armyPtr->GetArmyType() != BUILD){
		delete armyPtr;
		armyPtr = NULL;
	}
}
void FightManager::enterCountry(std::vector<IArmyPtr> & /*armyPtrVect*/){
	return ;
}
void FightManager::run(void)
{
	char * buffer;
	while (getQueue(buffer)){
		parse(buffer);
		OutputDebugString("CFightThread::run while\n");
	}
}
void FightManager::broadcastAddArmy(IArmyPtr armyPtr){
	if (armyPtr == NULL){
		return;
	}
	ARMY_ADD msgAddArmy;
	if(armyManager_->exportAddArmyInfo(armyPtr , msgAddArmy) == false){
		return ;
	}
	msgAddArmy.serialize(mapInfo_->getMapID() , armyPtr->getPlayerID() , ADD_ARMY_INFO , 0);
	msgBroadcast(&msgAddArmy , ADD_ARMY_INFO ,sizeof(ARMY_ADD));
}
void FightManager::processTimerEvent(TIMER_HEADER * msgTimer){
	if (msgTimer == NULL){
		return ;
	}
	switch(msgTimer->timer_command ){
	case EN_TIMER_FIGHT_NOTFIY:
		scheduler();
		break;
	case EN_TIMER_LOG_NOTFIY:
		processLogPlayerOnline((LOG_ONLINE_NUM_TIMER*)msgTimer);
		break;
	default:
		break;
	}
}
void FightManager::processLogPlayerOnline(LOG_ONLINE_NUM_TIMER * /*msgTimer*/){
	online(playerManager_->getPlayerNum());
	startLogTimer();
}
void FightManager::startLogTimer(void){
	LOG_ONLINE_NUM_TIMER * pTimer = new LOG_ONLINE_NUM_TIMER;
	pTimer->serialize();
	set_timer(1800, (const void *)pTimer);
}
void FightManager::clearAttachedNpc(IArmyPtr armyPtr , bool isKillArmy){
	if (armyPtr == NULL){
		return ;
	}
	const vector<unsigned int>&  armys = armyPtr->getAttachArmys();
	IArmyPtr tmpArmyPtr;
	for (int i = 0 ; i < static_cast<int>(armys.size()) ; i++){
		tmpArmyPtr = getArmyByID(armys[i]);
		if (tmpArmyPtr && isKillArmy){
			processDelArmy(tmpArmyPtr , ARMY_ANNIHILATE);
		}
		renascenceMgr_->eraseArmy(armys[i]);
	}
	armyPtr->clearAttachArmy();
}
void FightManager::TranslateCorrd(const POS_STRUCT & centryPos, POS_STRUCT & realPos ,int nOffsetRow,int nOffsetCol)
{
	realPos.PosX = centryPos.PosX + nOffsetRow;
	realPos.PosY = centryPos.PosY + nOffsetCol;

	if (centryPos.PosX % 2 == 0)	//保存的点击行为奇数，现在点击为偶数		
	{
		if (realPos.PosX  % 2 == 1)
		{
			realPos.PosY++;			//将奇数所对应的列数加1
		}
	}
}

//绑定NPC
bool FightManager::attachNpc(IArmyPtr armyPtr , int templateID , const POS_STRUCT & centryPos , ENUM_USE_SCENE state){
	vector<NpcTemplate> * vecNpcTemplate = npcInfo_->getNpcTemplate(templateID);
	if (vecNpcTemplate == NULL || armyPtr == NULL){
		return false;
	}
	AttachedNpcArmyParam armyParam;
	AttachedNpcArmy *attachArmy;
	bool isEnterSystem;
	armyParam.adscriptionArmyID = armyPtr->GetArmyID();
	armyParam.mapData.ClassImpedient = mapInfo_;
	armyParam.mapData.MapData = armyManager_->getArmyList();
	for (int i = 0 ; i < static_cast<int>(vecNpcTemplate->size()); i++){
		armyParam.npcArmyInfo = npcInfo_->getEctypeNpcType((*vecNpcTemplate)[i].npcTypeID);
		if (armyParam.npcArmyInfo == NULL){
			continue;
		}
		armyParam.npcArmyTable = &(*vecNpcTemplate)[i];
		attachArmy = new AttachedNpcArmy;
		if(attachArmy == NULL){
			continue ;
		}
		if(attachArmy->Initialize(armyParam , this ,state ,isEnterSystem) == false){
			delete attachArmy;
			attachArmy = NULL;
			continue;
		}
		attachArmy->resetID(renascenceMgr_->getNewArmyID());
		POS_STRUCT pos;
		
		TranslateCorrd(armyPtr->getCentryPos(),pos,armyParam.npcArmyTable->x,armyParam.npcArmyTable->y);
		pos = attachArmy->getValidPos(pos , 1);
		attachArmy->setArmyPos(pos);
		if (isEnterSystem){
			if(!forceInsertNewArmy(attachArmy)){
				delete attachArmy;
				attachArmy = NULL;
				continue;
			}
		}
		else
		{
			renascenceMgr_->insertDeadArmy(attachArmy);
		}
		attachArmy->setLordArmy(armyPtr->GetArmyID());
		armyPtr->attachArmy(attachArmy->GetArmyID());
	}
	return true;
}
bool FightManager::isSingleEctype(unsigned int ectypId){
	EctypeInfoTable * ectypeInfo = npcInfo_->getEctypeInfoTable(ectypId);
	if (ectypeInfo != NULL){
		return ectypeInfo->limitPlayerNum == 1;
	}
	return false;
}
bool FightManager::forceInsertNewArmy(IArmyPtr armyPtr , bool isBroadcast){
	if(addArmy(armyPtr) == false){
		return false;
	}
	if(!isBroadcast){
		return true;
	}
	ARMY_ADD msgAddArmy;
	armyManager_->exportAddArmyInfo(armyPtr , msgAddArmy);
	msgAddArmy.serialize(mapInfo_->getMapID() , armyPtr->getPlayerID() , ADD_ARMY_INFO , 0);
	msgBroadcast(&msgAddArmy , ADD_ARMY_INFO , msgAddArmy.length);
	return true;
}

bool FightManager::reclaimDeadArmy(IArmyPtr deadArmyPtr){
	return renascenceMgr_->insertDeadArmy(deadArmyPtr);
}
void FightManager::reclaimRepeatArmy(IArmyPtr armyPtr){
	renascenceMgr_->insertRepeatArmy(armyPtr);
}
//获得防具信息
DefendAttribute * FightManager::getDefendAttribute(int defendID) const{
	return dataBaseMemData_->GetDefendAttribute(defendID);
}
//获得防具信息
WeaponAttribute * FightManager::getWeaponAttribute(int weaponID) const{
	return dataBaseMemData_->GetWeaponAttribute(weaponID);
}
bool FightManager::getSkillInfo(computinghurt::SkillInfo& skillInfo , SKILL_TYPE skillID , int skillLevel){
	SkillDetail * skillDetailInfo = dataBaseMemData_->getSkillDetail(skillID , skillLevel);
	if (skillDetailInfo == NULL){
		return false;
	}
	skillInfo.skillAndGradeStru.id = skillDetailInfo->skillID;
	skillInfo.skillAndGradeStru.level = skillDetailInfo->level;
	
	skillInfo.effectType = skillDetailInfo->effectType;
	skillInfo.effectVal = skillDetailInfo->effectValue;
	skillInfo.landForm = skillDetailInfo->landLimit;
	skillInfo.objectOccupation = skillDetailInfo->selfOccupationLimit;
	skillInfo.otherOccupation = skillDetailInfo->targetOccupationLimit;
	skillInfo.probability = skillDetailInfo->odds;
	return true;
}
void FightManager::processPropCardError(ReqUsePropCard* buf , int errCode){
	RspUsePropCard propCardErr;
	propCardErr.serialize(buf->agent ,buf->from , getProcessID() ,buf->cardType,errCode);
	sendMsgToAgent(buf->agent ,(char *)buf , buf->length);
}
void FightManager::processUsePropCard(ReqUsePropCard* buf){
	if (buf == NULL){
		return;
	}
	IArmyPtr armyPtr = getArmyByID(buf->armyID);
	if (armyPtr == NULL){
		return processPropCardError(buf , 1); //目标部队不存在
	}
	
	PlayerItemStorageTable storageTable;
	PlayerItemStorageTable *storageTablePtr = NULL;
	int cardNum = 0;
	storageTable.setmasterid(1,armyPtr->getPlayerID());
	if(dataBaseProxy_->db_multi_select(storageTable , (char *&)storageTablePtr ,cardNum)!=DB_Proxy::DB_SUCCEED){
		return processPropCardError(buf , 2); //该类型的卡不存在
	}
	cardNum /= sizeof(PlayerItemStorageTable);
	int i;
	PropCardConfig* propCardParam = NULL;
	for (i = 0 ; i < cardNum ; i++)
	{
		propCardParam = npcInfo_->getPropCardInfo(static_cast<int>(storageTablePtr[i].itemid_));
		if (propCardParam != NULL && propCardParam->cardType == buf->cardType){
			break;
		}
	}
	if (i < cardNum){
		storageTable = storageTablePtr[i];
	}
	if (storageTablePtr){
		delete [] (char *)storageTablePtr;
		storageTablePtr = NULL;
	}
	if ( i >= cardNum){
		return processPropCardError(buf , 2); //该类型的卡不存在
	}
	if(armyPtr->getCardState(static_cast<CardType>(buf->cardType)) != IArmy::NO_USED_CARD){
		return processPropCardError(buf ,4); //该类型的卡正在使用
	}
	if(propManager_->isCanUserCard(armyPtr , propCardParam) == false){
		return processPropCardError(buf , 3); //此种类型卡已达使用上限
	}
	
	if(dataBaseProxy_->db_delete(storageTable) != DB_Proxy::DB_SUCCEED){
		LOG_ERROR(("删除卡%d失败" , storageTable.sortid_));
		return ;
	}
	propManager_->useCard(armyPtr ,propCardParam);
	tagNotify_UseOneCard_Request msgUseCard;
	msgUseCard.serialize(buf->agent , buf->from , static_cast<int>(storageTable.itemid_) , mapInfo_->getMapID());
	event_notify(msgUseCard.to , (char *) &msgUseCard , sizeof(tagNotify_UseOneCard_Request));
	switch(propCardParam->cardType){
	case AddSoldier_Card:
		processAddSodier(buf , armyPtr , propCardParam);
		break;
	case SPEEDUP_CARD:
		processSpeedCard(armyPtr , propCardParam);
		break;
	case MERCENARY_CARD:
		processMercenaryCard(buf , armyPtr , propCardParam);
		break;
	default:
		break;
	}
};
void FightManager::processAddSodier(ReqUsePropCard* buf , IArmyPtr armyPtr , PropCardConfig* propCardParam){
	
	if(armyPtr->addSoldierNum(propCardParam->cardValue) == 0){
		//数量非法
		LOG_ERROR(("援兵卡%d数量%d设置错误失败" , buf->cardType ,propCardParam->cardValue));
		return ;
	}
	if (mapInfo_->GetMapState() == war){
		fightRecord_->onAddSoldiers(armyPtr);
	}
	UpDateArmyInfo msgUpdateArmyInfo;
	msgUpdateArmyInfo.ArmyID = armyPtr->GetArmyID();
	msgUpdateArmyInfo.DefendID = armyPtr->GetDefendID();
	msgUpdateArmyInfo.SoldierNum = armyPtr->getSoldierNum();
	msgUpdateArmyInfo.curHp = armyPtr->GetCurrentHp();
	msgUpdateArmyInfo.totalHp = armyPtr->getInitHp();
	msgUpdateArmyInfo.serialize(buf->agent ,mapInfo_->getMapID() , buf->from);
	msgBroadcast(&msgUpdateArmyInfo , static_cast<PROTOCOL_FIGHT>(msgUpdateArmyInfo.command) , sizeof(UpDateArmyInfo));
}
void FightManager::processSpeedCard(IArmyPtr armyPtr , PropCardConfig* propCardParam){
	armyPtr->addMoveSpeed(propCardParam->cardValue);
}
//使用佣兵卡
void FightManager::processMercenaryCard(ReqUsePropCard* buf ,IArmyPtr armyPtr , PropCardConfig* propCardParam){
	GMCreateNPCParam_MultiMap_Iterator lower_iter,upper_iter;
	lower_iter = dataBaseMemData_->GMCreateNPCParam_Lowerbound(armyPtr->GetCountryID(),3);
	upper_iter = dataBaseMemData_->GMCreateNPCParam_Upperbound(armyPtr->GetCountryID(),3);
	LPGMCreateNPCParam_Item pCreateNPCParam = NULL;
	int iCount = 0;
	ArmyTable newNPCArmy;
	PlayerArmy * playerArmyPtr;
	IArmyPtr army;
	for (;lower_iter != upper_iter;lower_iter++)
	{
		pCreateNPCParam = lower_iter->second;
		if(pCreateNPCParam == NULL)
			continue;
		//本类型部队的总数
		iCount = pCreateNPCParam->AymyCount * propCardParam->cardValue;
		//创建部队（默认所属地图为大地图），通知郡地图部队到达
		memset(&newNPCArmy,0,sizeof(ArmyTable));
		strcpy(newNPCArmy.armyname_,"佣兵");
		newNPCArmy.playerid_ = pCreateNPCParam->PlayerID;
		newNPCArmy.officerid_ = pCreateNPCParam->OfficerID;
		newNPCArmy.mapid_ = getProcessID();
		newNPCArmy.soldiertype_ = pCreateNPCParam->SoldierType;
		newNPCArmy.raceid_ = pCreateNPCParam->RaceID;
		newNPCArmy.firstcount_ = pCreateNPCParam->SoldierCount;
		newNPCArmy.fsoldierlevel_= 1;
		newNPCArmy.fweapon_ = pCreateNPCParam->WeaponType;
		newNPCArmy.fweaponlevel_ = pCreateNPCParam->WeaponLevel;
		newNPCArmy.fdefendtype_ = pCreateNPCParam->DefendType;
		newNPCArmy.fdefendlevel_ = pCreateNPCParam->DefendLevel;
		newNPCArmy.state_ = 2;
		newNPCArmy.countryid_ = armyPtr->GetCountryID();
		newNPCArmy.direction = 1;
		newNPCArmy.x = armyPtr->GetArmyPos()[0].PosX;
		newNPCArmy.y = armyPtr->GetArmyPos()[0].PosY;
		for (int i=0;i<iCount;i++){
			newNPCArmy.armyid_ = renascenceMgr_->getNewArmyID();
			army = armyManager_->createPlayerArmy(newNPCArmy);
			if (army == NULL){
				LOG_ERROR(("佣兵卡创建部队错误， 部队ID%d ，" ,newNPCArmy.armyid_));
				return ;
			}
			playerArmyPtr = dynamic_cast<PlayerArmy*>(army);
			playerArmyPtr->setMercenary();
			playerArmyPtr->setPlayerName(armyPtr->getPlayerName());
			POS_STRUCT pos  = armyPtr->GetArmyPos()[0];
			pos = army->getValidPos(pos);
			army->setArmyPos(pos);
			if(armyManager_->checkAddArmy(army) == true){
				broadcastAddArmy(army);
			}
		}
	}
}
void FightManager::onCaptiveHero(IArmyPtr attackPtr , IArmyPtr armyPtr){
	PlayerCaptiveHero heroData;
	heroData.playerid_ = attackPtr->getPlayerID();
	heroData.officerid_ = attackPtr->GetOfficerID();
	heroData.byplayerid_ = armyPtr->getPlayerID();
	heroData.byofficerid_ = armyPtr->GetOfficerID();
	heroData.captivetimes_ = IceUtil::Time::now().toSeconds()+3600;
	
	PlayerOfficerTable Officer,byOfficer,byHero;
	byOfficer.setmasterid(heroData.byplayerid_);
	byOfficer.setdetailid(heroData.byplayerid_);
	if(dataBaseProxy_->db_select(byOfficer) != DB_Proxy::DB_SUCCEED)
	{
		LOG_ERROR(("查询玩家%d英雄%d数据出错！onCaptiveHero" , heroData.byplayerid_ , heroData.byplayerid_));
		return ;
	}

	byHero.setmasterid(heroData.byplayerid_);
	byHero.setdetailid(heroData.byofficerid_);
	if(dataBaseProxy_->db_select(byHero) != DB_Proxy::DB_SUCCEED)
	{
		LOG_ERROR(("查询玩家%d英雄%d数据出错！onCaptiveHero" , heroData.byplayerid_ , heroData.byofficerid_));
		return ;
	}

	Officer.setmasterid(heroData.playerid_);
	Officer.setdetailid(heroData.playerid_);

	if(dataBaseProxy_->db_select(Officer) != DB_Proxy::DB_SUCCEED)
	{
		LOG_ERROR(("查询玩家%d英雄%d数据出错！onCaptiveHero" , heroData.playerid_ , heroData.playerid_));
		return ;
	}

	if (dataBaseProxy_->db_insert(heroData)!=DB_Proxy::DB_SUCCEED){
		LOG_ERROR(("插入英雄数据出错！onCaptiveHero"));
		return  ;
	}


	CAPTIVE_HERO Captive;
	Captive.serialize(byOfficer.officername_, byHero.officername_,Officer.officername_,0,0,EN_S_BROADCAST_CAPTIVE_HERO);

	broadcast((char *)&Captive, Captive.length);
	memset(&Officer,0,sizeof(Officer));
	Officer.setmasterid(heroData.playerid_);
	Officer.setdetailid(heroData.playerid_);
	Officer.generalcount_ = 1;
	if(dataBaseProxy_->db_increase(Officer) != DB_Proxy::DB_SUCCEED){
		LOG_ERROR(("更新英雄表出错！onCaptiveHero"));
	}
}
int FightManager::getCountryID(unsigned int playerID){
	PlayerChatRoomTable playerTable;
	playerTable.setid(playerID);
	if(dataBaseProxy_->db_select(playerTable) != DB_Proxy::DB_SUCCEED){
		return -1;
	}
	return playerTable.country_;
}
int FightManager::getArmyCardLevel(int armyCardID) const{
	ArmyBrand * armyCardPtr = dataBaseMemData_->getArmyBrandParamByID(armyCardID);
	if (armyCardPtr)
	{
		return armyCardPtr->getLevel();
	}
	return -1;
}
int FightManager::getArmyCardItem(int armyCardLevel) const{
	ArmyBrand * armyCardPtr = dataBaseMemData_->getArmyBrandParamByLevel(armyCardLevel);
	if (armyCardPtr)
	{
		return armyCardPtr->getID();
	}
	return -1;
}
//处理军牌掉落
bool FightManager::processArmyCardDrop(IArmyPtr attackArmy , IArmyPtr army ,int armyCardID , int odds){
	return dropGoodsManager_->processArmyCardDrop(attackArmy , army ,armyCardID ,odds);
}
//处理玩家军牌掉落
void FightManager::processPlayerDrop(IArmyPtr attackArmy , IArmyPtr army){
	dropGoodsManager_->processPlayerDrop(attackArmy , army);
}
void FightManager::resetArmyCardDropRecord(void){
	dropGoodsManager_->resetArmyCardDropRecord();
}
int FightManager::getLabelCountryID(int label){
	map<int , DetailEntranceInfo>::iterator iter = labelMap_.find(label);
	if (iter != labelMap_.end()){
		return iter->second.countryId;
	}
	return NPC_;
}
int FightManager::getDoorId(int countryId){
	map<int , DetailEntranceInfo> ::iterator iter;
	for(iter = labelMap_.begin() ; iter != labelMap_.end() ; ++iter){
		if (iter->second.countryId == countryId)
		{
			return iter->second.doorId;
		}
	}
	return  -1;
}
//激发特效
unsigned int FightManager::useEffect(EffectDetail& effectDetail){
	effectManager_.insertEffect(effectDetail);
	//发送特效到客户端

	MsgSpeciallyEffect* tmpPtr = (MsgSpeciallyEffect*)new  char[sizeof(MsgSpeciallyEffect) + sizeof(SpeciallyEffect)];
	if (tmpPtr == NULL){
		return effectDetail.effectId;
	}
	tmpPtr->count = 1;

	(*tmpPtr)[0].effectId = effectDetail.effectId;
	(*tmpPtr)[0].effectType = effectDetail.effectType;
	(*tmpPtr)[0].useArmyId = effectDetail.useArmyId;
	(*tmpPtr)[0].receiveArmyId = effectDetail.receiveArmyId;
	(*tmpPtr)[0].startBout_ = effectDetail.startBout_;
	(*tmpPtr)[0].endBout_ = effectDetail.endBout_;
	tmpPtr->serialize(0 , curProcessID_ , 0);
	msgBroadcast(tmpPtr , MSG_SPECIALLY_EFFECT , tmpPtr->length);
	delete [] (char *)tmpPtr;
	tmpPtr = NULL;
	return effectDetail.effectId;
}
//消除特效
bool  FightManager::clearEffect(unsigned int effectId){
	bool flag = effectManager_.clearEffect(effectId);
	if (flag){
		//发送清除特效消息
		MsgClearEffect * tmpPtr = (MsgClearEffect * )new char [sizeof(MsgClearEffect) + sizeof(unsigned int)];
		if (tmpPtr == NULL){
			return flag;
		}
		tmpPtr->count = 1;
		(*tmpPtr)[0] = effectId;
		tmpPtr->serialize(0 ,curProcessID_ , 0);
		msgBroadcast(tmpPtr , MSG_CLEAR_EFFECT , tmpPtr->length);
		delete [] (char *)tmpPtr;
		tmpPtr = NULL;
	}
	return flag;
}
//获得指定ID的特效信息
EffectDetail *FightManager::getEffectByID(unsigned int effectId){
	return effectManager_.getEffectByID(effectId);
}
void FightManager::exportEffect(unsigned int playerId , int agent){
	//将所有特效发到客户端
	map<unsigned int ,EffectDetail>& tmpMap = effectManager_.getEffectMap();
	if (tmpMap.size() <= 0){
		return ;
	}
	map<unsigned int ,EffectDetail>::iterator iter;
	MsgSpeciallyEffect* tmpPtr = (MsgSpeciallyEffect*)new  char[sizeof(MsgSpeciallyEffect) + sizeof(SpeciallyEffect) * tmpMap.size()];
	if (tmpPtr == NULL){
		return;
	}
	tmpPtr->count = 0;
	for (iter = tmpMap.begin() ; iter!= tmpMap.end() ; ++iter){
		(*tmpPtr)[tmpPtr->count].effectId = iter->second.effectId;
		(*tmpPtr)[tmpPtr->count].effectType = iter->second.effectType;
		(*tmpPtr)[tmpPtr->count].useArmyId = iter->second.useArmyId;
		(*tmpPtr)[tmpPtr->count].receiveArmyId = iter->second.receiveArmyId;
		(*tmpPtr)[tmpPtr->count].startBout_ = iter->second.startBout_;
		(*tmpPtr)[tmpPtr->count++].endBout_ = iter->second.endBout_;
	}
	tmpPtr->serialize(agent , curProcessID_ , playerId);
	sendMsgToAgent(agent , (char *) tmpPtr , tmpPtr->length);
	delete [] (char *)tmpPtr;
	tmpPtr = NULL;
}
PropCard * FightManager::getNewCard(IArmyPtr armyPtr ,  PropCardConfig * propCardPtr){
	return propManager_->getNewCard(armyPtr , propCardPtr);
}
DayTask_DetailInfo * FightManager::getDayTaskParam(int taskId){
	return dataBaseMemData_->getDayTaskParam(taskId);
}
unsigned int FightManager::getNewArmyID(void){
	return renascenceMgr_->getNewArmyID();
}
//回收Army ID
void FightManager::reclaimArmyID(unsigned int armyID){
	renascenceMgr_->reclaimArmyID(armyID);
}