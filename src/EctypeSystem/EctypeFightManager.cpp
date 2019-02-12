#include "EctypeFightManager.h"
#include "log.hpp"
#include "LoadDBInfo.h"
#include "LoadMapInfo.h"
#include "ProNPCSystem.h"
#include "ServerProtocolHeader.h"
#include "MapServerProtocolHeader.h"
#include "socketsvc.hpp"
#include "EctypeManager.h"
#include "PlayerArmy.h"
#include "NPCInfo.h"
#include "NpcArmy.h"
#include "BuildArmy.h"
#include "IniFile.h"
#include "FightSystemMacro.h"
#include "CommInterfaceHeader.h"
#include "FightEvent.h"
#include "boost/serialization/singleton.hpp"
#include "computinghurt/randomgenerator.h"
#include "FightOverManager.h"
#include "SeekPathArithm.h"
#include "RenascenceMgr.h"
#include "Time_Protocol.h"
#include "EctypeArmyManager.h"
#include "PlayerManager.h"
#include "NetProcesser.h"
EctypeFightManager::EctypeFightManager(void)
{
}

EctypeFightManager::~EctypeFightManager(void)
{
	
}
bool EctypeFightManager::initialize(CLoadMapInfo * mapInfo,CLoadDBInfo *dataBaseMemData ,CNPCInfo * ectypeNpcInfo,Notify_Enter_Ectype* lpEvent,int roomID){
	resCount_ = 0;
	roomID_ = roomID;
	curProcessID_ = 1256;
	mapInfo_ = mapInfo;
	ectypeID_ = lpEvent->uEctypeID;
	entranceID_ = lpEvent->entranceID;
	dataBaseMemData_ = dataBaseMemData;
	npcInfo_ = ectypeNpcInfo;
	iniFile_ = &g_IniFile;
	fightScene_ = Counterpart_AI ;
	dropGoodsManager_ = new DropGoodsManager(this);
	if (dropGoodsManager_ == NULL){
		return false;
	}
	ectypeInfo_ = npcInfo_->getEctypeInfoTable(lpEvent->entranceID);
	if (ectypeInfo_ == NULL){
		return false;
	}
	warType_ = static_cast<GAME_HALL_TYPE>(ectypeInfo_->warType);
	if(ectypeInfo_->limitArmyNum * ectypeInfo_->limitPlayerNum < lpEvent->iArmyCount){
		return false;
	}
	dataBaseProxy_ = getNewDbgProxy();
	if (dataBaseProxy_ == NULL){
		return false;
	}
	
	processActiveNum_ = 40;
	curProcessQueue_ = new queue<unsigned int>;
	processQueue_    = new queue<unsigned int>;
	fightOverManager_ = new FightOverManager(ectypeInfo_->limitBout , this);
	if (curProcessQueue_ == NULL || processQueue_ == NULL || fightOverManager_ == NULL){
		return false;
	}
	
	renascenceMgr_ = new RenascenceMgr(3001,9999 ,  this) ;
	if (renascenceMgr_ == NULL){
		return false;
	}
	propManager_ = new PropManager(this);
	if (propManager_ == NULL){
		return false;
	}
	seekPather_ = new SeekPath(mapInfo);
	if (seekPather_ == NULL){
		return false;
	}
	fightRecord_ = new statisticssystem::FightingStatisticsAdapter(lossRate_);
	if (fightRecord_ == NULL){
		return false;
	}
	fightRecord_->onStart();
	patrolMap_ = ectypeNpcInfo->getPatrolData(ectypeID_);

	playerManager_ = new PlayerManager(iniFile_);
	if (playerManager_ == NULL){
		return false;
	}

	EctypeArmyManager * ectypeArmyManager = new EctypeArmyManager;
	armyManager_ = ectypeArmyManager;
	if(ectypeArmyManager == NULL){
		return false;
	}
	if(ectypeArmyManager->Initialize(mapInfo_ , npcInfo_ , 
		iniFile_ ,dataBaseProxy_,dataBaseMemData_,this) == false){
		return false;
	}
	list<IArmyPtr> armyList;
	map<int ,int> countryMap;
	//初始化玩家部队信息
	if(ectypeArmyManager->initPlayerArmy(lpEvent , armyList , countryMap , armysMap_) == false){
		armyManager_->freeArmyList(armyList);
		return false;
	}
	createLabelMap(countryMap , ectypeNpcInfo->getWarEntranceInfo(mapInfo_->getMapID()));
	if(!ectypeArmyManager->insertArmyToSystem(armyList)){
		return false;
	}
	if(isValidEctypeData(lpEvent , ectypeInfo_) == false){
		return false;
	}
	randomgenerator_ = &boost::serialization::singleton<computinghurt::RandomGenerator>::get_mutable_instance();

	//初始化副本NPC信息
	if(ectypeArmyManager->initNpcArmy(lpEvent->uEctypeID , ectypeInfo_) == false){
		return false;
	}
	//加载建筑物
	if(warType_ != FIELD_COUNTRYPART){
		ectypeArmyManager->loadBuildInfo();
	}
	effectManager_.setFightManager(this);
	FightOverConditionArray*  fightOverCondition = ectypeNpcInfo->getFightOverCondition(ectypeID_);
	if (fightOverCondition && fightOverCondition->size() > 0){
		fightOverManager_->setFightOverCondition(*fightOverCondition );
	}
	startTask();
	isRun_ = true;
	notifyZillah(lpEvent);
	return true;
}

void EctypeFightManager::parse(const char * buf){
	DISTRIBUTED_HEADER * msgHeader = (DISTRIBUTED_HEADER *)buf;
	switch(msgHeader->command){
	case EN_C_TIMER_NOTIFY:
		scheduler();
		break;
	case REQ_MOVE_ECTYPE_ARMY_INFO:
		processMove((ReqMoveEctypeArmyInfo*)buf);
		break;
	case REQ_ATTACK_ECTYPE_TARGET_INFO:
		processAttack((ReqAttackTargetInfo*)buf);
		break;
	case REQ_ECTYPE_GET_ARMY_INFO:
		processReqArmyInfo((ReqEctypeGetArmyInfo*)buf);
		break ;
	case REQ_ENTER_NPC_SYSTEM_INFO:
		processEnterSystem((REQ_ENTER_NPC_SYSTEM_STRUCT*) buf);
		break;
	case REQ_SET_ECTYPE_ARMY_TRUST_INFO:
		processTrusteeship((ReqSetEctypeArmyTrust *)buf);
		break ;
	case REQ_ECTYPE_DELETE_ARMY_INFO:
		processDelArmyMsg((ReqEctypeDeleteArmy *)buf);
		break;
	case REQ_LEAVE_ECTYPE_SYSTEM_INFO:
		processLeaveEctype((ReqLeaveEctypeSystem*)buf);
		break;
	case REQ_ECTYPE_SYSTEM_ARMY_RETREAT:
		processArmyRetreat((ReqEctypeRetreatArmyInfo*) buf);
		break;
	case REQ_USE_PROP_CARD:
		processUsePropCard((ReqUsePropCard*)buf);
		break;
	case ADD_ARMY_TO_ROOM:
		processAddArmy((ReqAddArmyToRoom *)buf);
		break;
	default:
		break;
	}
	delete [] buf;
	buf = NULL;
}

void EctypeFightManager::msgBroadcast(DISTRIBUTED_HEADER * msgData , PROTOCOL_FIGHT fightCmd  , int dataLen){
	map<unsigned int , ActivityPlayerPtr>::const_iterator iter;
	ActivityPlayerPtr activePlayer;
	const map<unsigned int , ActivityPlayerPtr>& activityPlayers = playerManager_->getPlayerMap();
	for (iter=activityPlayers.begin();iter != activityPlayers.end();iter ++){
		activePlayer = iter->second;
		if (activePlayer && activePlayer->isStayRoom){				
			msgData->serialize(dataLen,activePlayer->agent_,curProcessID_,activePlayer->playerID_,fightCmd);
			sendMsgToAgent(activePlayer->agent_,(char*)msgData,dataLen);
		}
	}
}
bool EctypeFightManager::isFightOver(int& victoryCountryID){
	const map<unsigned int , IArmyPtr>& armyMap = armyManager_->getArmyMap();
	map<unsigned int , IArmyPtr>::const_iterator iter;
	IArmyPtr armyPtr;
	if (isForceExit_ == true){
		return true;
	}
	bool isHavePlayerArmy = false;
	bool isOver = true;
	victoryCountryID = -1;
	for(iter = armyMap.begin() ;iter != armyMap.end() ;iter ++){
		armyPtr = iter->second;
		if (armyPtr == NULL || armyPtr->GetCurrentHp() == 0 ){
			continue;
		}
		if (armyPtr->getBuildDetailType() >= ARCH_BUILD_TOFT && armyPtr->getBuildDetailType() <= ARCH_TOFT_END){
			continue;
		}
		if (armyPtr->GetArmyType() == PLAYER_ARMY){
			isHavePlayerArmy = true;
		}
		if (victoryCountryID == -1){
			victoryCountryID = armyPtr->GetCountryID();
		}
		if(victoryCountryID != armyPtr->GetCountryID()){
			isOver = false;
		}
		if (isHavePlayerArmy && !isOver)
		{
			return false;
		}
	}
	if (!isHavePlayerArmy)
	{
		victoryCountryID  = mapInfo_->GetMapOwer();
	}
	return true;
}
bool EctypeFightManager::scheduler(){
	
	DWORD st  =  GetTickCount();
	int curProcessActiveNum = 0;
	unsigned int armyID;
	IArmyPtr armyPtr;
	bool isProcessedArmy = false;
	int victoryCountyID = NPC_;
	armyManager_->processDieArmy();
	renascenceMgr_->getRebornAndReproNpc(Counterpart_AI);
	if (fightOverManager_->isFightOver(victoryCountyID) == true || (armysMap_.size() == 0 && warType_ != FIELD_COUNTRYPART)){
		processFightOver(victoryCountyID);
		return false ;
	}
	while (curProcessQueue_->size() > 0){
		armyID = curProcessQueue_->front();
		armyPtr = getArmyByID(armyID);
		curProcessQueue_->pop();
		if (armyPtr == NULL){
			eraseProcessQueue(armyID);
			continue;
		}
		processedNum_ ++;
		if (isValidArmy(armyPtr)){
			if(armyPtr->processAction(false)){
				curProcessActiveNum ++;
				isProcessedArmy = true;
			}
			insertProcessQueue(armyPtr , true);
		}
		else{
			eraseProcessQueue(armyID);
		}
		processedNum_ ++;
	}

	driveBoutNum(false);

	while (curProcessQueue_->size() > 0){
		armyID = curProcessQueue_->front();
		armyPtr = getArmyByID(armyID);
		curProcessQueue_->pop();
		if (armyPtr == NULL){
			eraseProcessQueue(armyID);
			continue;
		}
		processedNum_ ++;
		if (isValidArmy(armyPtr)){
			if(armyPtr->processAction(true)){
				curProcessActiveNum ++;
				isProcessedArmy = true;
			}
			insertProcessQueue(armyPtr , true);
		}
		else{
			eraseProcessQueue(armyID);
		}
	}
	updateRes();
	effectManager_.tidyEffect(curBoutNum_);
	if (curProcessQueue_->size() == 0){
		driveBoutNum(true);
	}
	if (isProcessedArmy){
		curIdleBoutNum_ ++;
	}
	else{
		curIdleBoutNum_ = 0;
	}
	
	int realt = FIGHT_TIMER_CYCLE - GetTickCount() + st;
	if (realt <= 100){
		realt = 100;
	}
	startTimerCycle(static_cast<unsigned int>(realt));
	curCycleNum_++;
	return true;
}
void EctypeFightManager::updateRes(void){
	int mapSize = resCollect_.size();
	if (mapSize <= 0){
		return ;
	}
	MsgUpdateRes * msgResPtr = (MsgUpdateRes*)new char [sizeof(MsgUpdateRes) + sizeof(ResInfo) * mapSize * 10];
	if (msgResPtr == NULL){
		return ;
	}
	msgResPtr->count = 0;
	map<int, map<int ,int>>::iterator iter;
	map<int ,int>::iterator subIter;
	for (iter = resCollect_.begin() ; iter != resCollect_.end() ; ++iter)
	{
		map<int,int>& tmpMap = iter->second;
		for (subIter = tmpMap.begin() ; subIter != tmpMap.end() ; ++subIter)
		{
			(*msgResPtr)[msgResPtr->count].countryId = iter->first;
			(*msgResPtr)[msgResPtr->count].resType = subIter->first;
			(*msgResPtr)[msgResPtr->count++].resCount = subIter->second;
		}
	}
	msgResPtr->serialize(curProcessID_ , 0);
	msgBroadcast(msgResPtr , MSG_UPDATE_RES, msgResPtr->length);
	delete [] (char *)msgResPtr;
	msgResPtr = NULL;
}
void EctypeFightManager::notifyZillah(Notify_Enter_Ectype* msgEnterEctype){
	IArmyPtr armyPtr;
	ArmyTable *armyTable;
	for (int i = 0; i < msgEnterEctype->iArmyCount ; i++){
		armyPtr = getArmyByID(msgEnterEctype->getArmyInfo(i).armyId);
		if (armyPtr != NULL){
			armyPtr->getArmyTable(armyTable);
			if (armyTable != NULL){
				armyTable->state_ = 11;
				dataBaseProxy_->db_update(*armyTable ,*armyTable);
			} 
			notifyModifyArmyState(armyPtr->getPlayerID() , armyPtr->GetArmyID() , armyPtr->getFromMapID() ,
				armyPtr->getSoldierNum() ,ARMY_FIGHT_ECTYPE , armyPtr->GetWeaponID());
		}
	}
}
void EctypeFightManager::processMove(ReqMoveEctypeArmyInfo* buf){
	if (buf == NULL){
		return ;
	}
	IArmyPtr armyPtr = getArmyByID(buf->ulArmyID);
	if (isValidArmy(armyPtr) == false){
		return;
	}

	armyPtr->setTrusteeshipState(false);
	//获得逆转坐标
	POS_STRUCT tmpPos = buf->pos;
	if (mapInfo_->IsValidPos(tmpPos.PosX , tmpPos.PosY) == false){
		return ;
	}
	armyPtr->processMoveMsg(tmpPos);
}

void EctypeFightManager::processAttack(ReqAttackTargetInfo* buf)
{
	if (buf == NULL){
		return ;
	}
	IArmyPtr armyPtr = getArmyByID(buf->myArmyID);
	if (armyPtr == NULL){
		return;
	}
	armyPtr->setTrusteeshipState(false);
	IArmyPtr targetArmyPtr = getArmyByID(buf->TargetArmyID);
	if (targetArmyPtr == NULL){
		//获得逆转坐标
		POS_STRUCT tmpPos;
		tmpPos.PosX = buf->x;
		tmpPos.PosY = buf->y;
		if (mapInfo_->IsValidPos(tmpPos.PosX , tmpPos.PosY) == false){
			return ;
		}
		armyPtr->processMoveMsg(tmpPos);
		return ;
	}
	armyPtr->processAttackTarget(targetArmyPtr);
}
void EctypeFightManager::processDelArmyMsg(ReqEctypeDeleteArmy* msgDelArmy){
	IArmyPtr armyPtr = getArmyByID(msgDelArmy->ulArmyID);
	if (isValidArmy(armyPtr) == true && armyPtr->isPlayerArmy()){
		ArmyTable * armyTable = NULL;
		armyPtr->getArmyTable(armyTable);
		if (armyTable == NULL){
			return ;
		}
		if(dataBaseProxy_->db_delete(*armyTable) != DB_Proxy::DB_SUCCEED){
			//日志
			LOG_ERROR(("删除玩家%d部队%d地图%d错误，错误位置processDelArmyMsg\r\n" ,armyTable->playerid_ ,
				armyTable->armyid_ ,armyTable->mapid_));
		}
		int realState = 0;
		PlayerOfficerTable playerOfficerTable;
		memset(&playerOfficerTable , 0 , sizeof(PlayerOfficerTable));
		playerOfficerTable.setmasterid(armyPtr->getPlayerID());
		playerOfficerTable.setdetailid(armyPtr->GetOfficerID());
		playerOfficerTable.state_  = -1;
		playerOfficerTable.armyid_ = 0;
		if(dataBaseProxy_->db_update(playerOfficerTable,playerOfficerTable) != DB_Proxy::DB_SUCCEED){
			//日志
			LOG_ERROR(("玩家%d英雄%dUpdate错误，错误位置processDelArmyMsg\r\n" ,playerOfficerTable.playerid_ ,
				playerOfficerTable.officerid_));
		}
		if(playerOfficerTable.state_ != realState){
			//
			LOG_ERROR(("玩家%d英雄%d实际修改后状态%d要求状态%d错误位置processDelArmyMsg\r\n" ,playerOfficerTable.playerid_ ,
				playerOfficerTable.officerid_,playerOfficerTable.state_,realState));
			playerOfficerTable.state_ = realState;
			dataBaseProxy_->db_update(playerOfficerTable , playerOfficerTable);
		}
		fightOverManager_->onArmyDie(NULL , armyPtr);
		armysMap_.erase(armyPtr->GetArmyID());
		armyPtr->refreshExp();
		RspDeleteArmyInfo msgDeleteArmy;
		msgDeleteArmy.ArmyID[0] = msgDelArmy->ulArmyID;
		msgDeleteArmy.count = 1;
		msgBroadcast(&msgDeleteArmy ,RSP_DELETE_ARMY_INFO , sizeof(RspDeleteArmyInfo));
		fightEvent_->NotifyModifyArmyState(armyPtr->getPlayerID() , armyPtr->GetArmyID() , 
			armyPtr->getFromMapID() , 0, ARMY_ANNIHILATE , armyPtr->GetWeaponID());
		armyManager_->eraseArmy(armyPtr);
		delete armyPtr;
		armyPtr = NULL;
	}

}

void EctypeFightManager::processReqArmyInfo(ReqEctypeGetArmyInfo* lpReq){
	if (lpReq == NULL){
		return ;
	}
	RspEctypeGetArmyInfo Rsp;
	memset(&Rsp , 0 , sizeof(RspEctypeGetArmyInfo));
	IArmyPtr armyPtr = getArmyByID(lpReq->ArmyID);
	if (armyPtr != NULL){
		Rsp.ArmyHostID = armyPtr->getPlayerID();
		Rsp.AttackPower = armyPtr->GetAttackPower();
		Rsp.Attacktype = armyPtr->GetAttackType();
		Rsp.soldiercount = armyPtr->getSoldierNum();
		Rsp.DefendPyPower = armyPtr->GetDefendPower(PHYSIC_ATTACK);
		Rsp.defendMgPower =  armyPtr->GetDefendPower(MAGIC_ATTACK);
		Rsp.totalHp = armyPtr->GetCurrentHp();
		if (armyPtr->GetArmyName() != NULL){
			memcpy(Rsp.ArmyName,armyPtr->GetArmyName(),SIZE_16-1);
			Rsp.ArmyName[SIZE_16-1]='\0';
		}
		Rsp.FaceID = armyPtr->GetFaceID();
		Rsp.weaponID = armyPtr->GetWeaponID();
		Rsp.defendID = armyPtr->GetDefendID();
		Rsp.raceID = armyPtr->GetSoldierRace();
		Rsp.TrustFlag = armyPtr->GetTrusteeshipState();
		if (armyPtr->GetOfficerName() != NULL){
			memcpy(Rsp.szHeroName,armyPtr->GetOfficerName(),SIZE_16-1);
			Rsp.szHeroName[SIZE_16-1]='\0';
		}
		if (armyPtr->getPlayerName().size() >= 16){
			memcpy(Rsp.szMainHeroName,armyPtr->getPlayerName().c_str(),SIZE_16-1);	
		}
		else{
			memcpy(Rsp.szMainHeroName,armyPtr->getPlayerName().c_str(),armyPtr->getPlayerName().size());	
		}
		Rsp.szMainHeroName[SIZE_16-1]='\0';
		Rsp.iSodierOccupation = static_cast<unsigned char> (armyPtr->GetArmyOccupation());
		Rsp.iPower = static_cast<unsigned short> (armyPtr->GetOfficerPower());
		Rsp.iStamina = static_cast<unsigned short>(armyPtr->GetOfficerStamina());
		Rsp.iCelerity =static_cast<unsigned short>(armyPtr->GetOfficerCelerity());
		Rsp.iIntellect = static_cast<unsigned short>(armyPtr->GetOfficerIntellect());
		Rsp.iLevel = static_cast<unsigned short>(armyPtr->GetOfficerLevel());
	}
	Rsp.serialize(sizeof(RspEctypeGetArmyInfo) , lpReq->agent,curProcessID_,lpReq->from,RSP_ECTYPE_GET_ARMY_INFO);
	sendMsgToAgent(lpReq->agent,(char*)&Rsp,Rsp.length);
}

void EctypeFightManager::processEnterSystem(const REQ_ENTER_NPC_SYSTEM_STRUCT* lpReq){
	if (lpReq == NULL){
		return ;
	}
	ActivityPlayerPtr  playerInfo = getPlayerInfo(lpReq->uplayerID);
	if (playerInfo == NULL){
		return ;
	}
	if (!lpReq->msgFromMapID)
	{
		//发送离开郡消息
		REQ_LEAVE_FIGHT_SYSTEM_STRUCT msgLeaveFight;
		msgLeaveFight.playerID = lpReq->uplayerID;
		msgLeaveFight.serialize(curProcessID_ ,lpReq->msgFromMapID);
		event_notify(lpReq->msgFromMapID , (char *)&msgLeaveFight , sizeof(REQ_LEAVE_FIGHT_SYSTEM_STRUCT));
	}

	playerInfo->agent_ = lpReq->agent;
	playerInfo->ectypeID_ = mapInfo_->getMapID();
	playerInfo->isStayRoom = true;
	RSP_ENTER_FIGHT_SYSTEM_STRUCT* msgEnterNpcSystem = new  RSP_ENTER_FIGHT_SYSTEM_STRUCT;
	if (msgEnterNpcSystem == NULL){
		return ;
	}
	memset(msgEnterNpcSystem , 0 , sizeof(RSP_ENTER_FIGHT_SYSTEM_STRUCT));
	map<unsigned int ,IArmyPtr>::const_iterator iter;
	const map<unsigned int ,IArmyPtr>& armyMap = armyManager_->getArmyMap();
	int armyNum = 0;
	bool isFirst = true;
	IArmyPtr armyPtr ;
	for (iter = armyMap.begin();iter != armyMap.end(); ++iter)
	{
		armyPtr = iter->second;
		if (armyPtr == NULL){
			continue;
		}
		armyPtr->exportArmyInfo(msgEnterNpcSystem->army[armyNum],mapInfo_->getMapID());
		armyNum ++;
		if (armyNum == MAX_MAP_ARMY_NUM){
			if (isFirst)
			{
				msgEnterNpcSystem->flag = 1;
				isFirst = false;
			}
			else
			{
				msgEnterNpcSystem->flag = 0;
			}
			msgEnterNpcSystem->count = armyNum ;
			
			msgEnterNpcSystem->MapID =  mapInfo_->getMapID();
			msgEnterNpcSystem->serialize(sizeof(RSP_ENTER_FIGHT_SYSTEM_STRUCT),playerInfo->agent_,curProcessID_,lpReq->uplayerID,RSP_ENTER_FIGHT_SYSTEM);
			sendMsgToAgent(playerInfo->agent_,(char*)msgEnterNpcSystem,sizeof(RSP_ENTER_FIGHT_SYSTEM_STRUCT));
			memset(msgEnterNpcSystem,0,sizeof(RSP_ENTER_FIGHT_SYSTEM_STRUCT));
			armyNum = 0;
		}		
	}
	if (armyNum > 0){
		msgEnterNpcSystem->count = armyNum ;
		if (isFirst)
		{
			msgEnterNpcSystem->flag = 1;
			isFirst = false;
		}
		else
		{
			msgEnterNpcSystem->flag = 0;
		}
		msgEnterNpcSystem->MapID = mapInfo_->getMapID();
		msgEnterNpcSystem->serialize(sizeof(RSP_ENTER_FIGHT_SYSTEM_STRUCT)-(sizeof(ARMY_POS)*(MAX_MAP_ARMY_NUM -armyNum)),
			playerInfo->agent_,curProcessID_,lpReq->uplayerID,RSP_ENTER_FIGHT_SYSTEM);
		sendMsgToAgent(playerInfo->agent_,(char*)msgEnterNpcSystem,msgEnterNpcSystem->length);
	}
	delete msgEnterNpcSystem;
	msgEnterNpcSystem = NULL;

	BuildArmy * buildArmy;
	BulidInfoList buildData;
	memset(&buildData,0,sizeof(BulidInfoList));
	buildData.mapid = mapInfo_->getMapID();
	int buildNum = 0;
	map<unsigned long , PublicBuildTable* >::iterator buildIter;

	for (iter = armyMap.begin();iter != armyMap.end(); ++iter)
	{
		buildArmy =  dynamic_cast<BuildArmy*> (iter->second);
		if (buildArmy == NULL || buildArmy->GetArmyType() != BUILD || buildArmy->getBuildDetailType() == ARCH_BUILD_TOFT){
			continue ;
		}
		buildData.BuildList[buildNum].ulBuildID = buildArmy->getBuildID();
		buildData.BuildList[buildNum].iBuildType = buildArmy->getBuildDetailType();
		buildData.BuildList[buildNum].iBuildLevel = buildArmy->getBuildLevel();
		buildNum ++;
		if (buildNum>=MAX_ARMY_NUM)
		{
			buildData.count = buildNum;
			buildData.serialize(sizeof(BulidInfoList),playerInfo->agent_,mapInfo_->getMapID(),
				playerInfo->playerID_,RSP_SEND_PUBLIC_BULID_INFO);
			asynch_write(playerInfo->agent_,(char*)&buildData,buildData.length);
			buildNum = 0;
		}
	}
	if (buildNum>0)
	{
		buildData.count = buildNum;
		buildData.serialize(sizeof(BulidInfoList) - sizeof(BulidParam)*(MAX_NUM_ARMY - buildNum),
			playerInfo->agent_,curProcessID_,playerInfo->playerID_,RSP_SEND_PUBLIC_BULID_INFO);
		asynch_write(playerInfo->agent_,(char*)&buildData,buildData.length);
	}
	//导出当前所有效果
	exportEffect(playerInfo->playerID_, playerInfo->agent_);
	//导出卡片锁定时间
	propManager_->exportCardRecord(playerInfo->playerID_ , playerInfo->agent_);
}

void EctypeFightManager::processArmyRetreat(ReqEctypeRetreatArmyInfo* msgArmyRetreat){
	if (msgArmyRetreat == NULL){
		return;
	}
	if (warType_ == LIMIT_BATTLEFIELD)
	{
		return ;
	}
	IArmyPtr armyPtr = getArmyByID(msgArmyRetreat->ArmyID);
	if(isValidArmy(armyPtr) == false || armyPtr->isPlayerArmy() == false){
		return ;
	}
	Notify_Back_Army_Ectype msgArmyBack;
	memset(&msgArmyBack , 0 , sizeof(Notify_Back_Army_Ectype));
	msgArmyBack.uArmyID[0] =armyPtr->GetArmyID(); 
	ArmyTable* armyTabe = NULL;
	armyPtr->getArmyTable(armyTabe);
	if (armyTabe == NULL){
		return ;
	}
	armyTabe->state_ = 1;
	armyTabe->firstcount_ = armyPtr->getSoldierNum();
	if(dataBaseProxy_->db_update(*armyTabe,*armyTabe) != DB_Proxy::DB_SUCCEED){
		LOG_ERROR(("processArmyRetreat修改部队%d状态错误" , armyPtr->GetArmyID()));
	}
	armyPtr->refreshExp();
	msgArmyBack.iArmyCount= 1;
	msgArmyBack.uEctypeID = ectypeID_;
	msgArmyBack.entranceID = entranceID_;
	fightEvent_->NotifyEctypeArmyBack(msgArmyBack ,armyPtr->getFromMapID() , curProcessID_ );

	RspDeleteArmyInfo msgDeleteArmy;
	msgDeleteArmy.ArmyID[0] = armyPtr->GetArmyID();
	msgDeleteArmy.count = 1;
	msgBroadcast(&msgDeleteArmy ,RSP_DELETE_ARMY_INFO , sizeof(RspDeleteArmyInfo));
	fightEvent_->NotifyModifyArmyState(armyPtr->getPlayerID() , armyPtr->GetArmyID() , 
		armyPtr->getFromMapID() , 0, OUT_CITY_TRUST_CONTROY , armyPtr->GetWeaponID());
	armyManager_->eraseArmy(armyPtr);
	if(isHaveArmy(armyPtr->getPlayerID()) == false){
		vector<unsigned int> vecPlayer;
		vecPlayer.push_back(armyPtr->getPlayerID());
		kickPlayerOutEctype(vecPlayer);
		ForceVeer forceVeer;
		forceVeer.fromMap = armyPtr->getFromMapID();
		forceVeer.serialize(msgArmyRetreat->agent,curProcessID_,armyPtr->getPlayerID());
		sendMsgToAgent(msgArmyRetreat->agent , (char*)&forceVeer,forceVeer.length);
	}
	
	delete armyPtr;
	armyPtr = NULL;
}

bool EctypeFightManager::isHaveArmy(unsigned int playerID) const{
	map<unsigned int , IArmyPtr>::const_iterator iter;
	const map<unsigned int , IArmyPtr>& armyMap_ = armyManager_->getArmyMap();
	IArmyPtr armyPtr;
	for (iter = armyMap_.begin(); iter != armyMap_.end();iter ++){
		armyPtr = iter->second;
		if (armyPtr->GetCurrentHp() == 0){
			continue;
		}
		if(armyPtr->getPlayerID() == playerID){
			return true ;
		}
	}
	return false;
}
int EctypeFightManager::getArmyNum(unsigned int playerID) const{
	map<unsigned int , IArmyPtr>::const_iterator iter;
	int armyNum = 0;
	const map<unsigned int , IArmyPtr>& armyMap_ = armyManager_->getArmyMap();
	IArmyPtr armyPtr;
	for (iter = armyMap_.begin(); iter != armyMap_.end();iter ++){
		armyPtr = iter->second;
		if (armyPtr->GetCurrentHp() == 0){
			continue;
		}
		if (playerID == armyPtr->getPlayerID())
		{
			armyNum++;
		}
	}
	return armyNum;
}
void EctypeFightManager::processLeaveEctype(ReqLeaveEctypeSystem * msgLeaveEctype){
	if (msgLeaveEctype == NULL){
		return ;
	}
	ActivityPlayerPtr activePlayer = getPlayerInfo(msgLeaveEctype->uUserID);
	if (activePlayer.get() != NULL){
		activePlayer->isStayRoom = false;
	}
	if (!isHaveArmy(msgLeaveEctype->uUserID)){
		playerManager_->delActivePlayer(msgLeaveEctype->uUserID);
		vector<unsigned int> vec;
		vec.push_back(msgLeaveEctype->uUserID);
		kickPlayerOutEctype(vec);
	}
}
bool EctypeFightManager::kickArmyOutEctype(vector<unsigned int>& players){
	map<unsigned int , IArmyPtr>::const_iterator iter;
	const map<unsigned int , IArmyPtr>& armyMap_ = armyManager_->getArmyMap();
	
	IArmyPtr armyPtr;
	int armyNum = 0;
	Notify_Back_Army_Ectype tmpInfo;
	memset(&tmpInfo , 0 , sizeof(Notify_Back_Army_Ectype));
	map<int , Notify_Back_Army_Ectype> tmpMap;
	int fromMapID = 0;
	for (iter = armyMap_.begin(); iter != armyMap_.end();iter ++){
		armyPtr = iter->second;
		if(armyPtr->isPlayerArmy() == false){
			continue ;
		}
		if (armyPtr->GetCurrentHp() == 0){
			continue;
		}
		Notify_Back_Army_Ectype* msgArmyBackPtr = NULL;
		map<int , Notify_Back_Army_Ectype>::iterator tmpIter;
		tmpIter = tmpMap.find(armyPtr->getFromMapID());
		if (tmpIter == tmpMap.end())
		{
			tmpMap[armyPtr->getFromMapID()] = tmpInfo;
			msgArmyBackPtr = &tmpMap[armyPtr->getFromMapID()];
		}
		else
		{
			msgArmyBackPtr = &tmpIter->second;
		}
		msgArmyBackPtr->uArmyID[msgArmyBackPtr->iArmyCount++] =armyPtr->GetArmyID(); 
		ArmyTable* armyTabe = NULL ;
		armyPtr->getArmyTable(armyTabe);
		armyTabe->state_ = 1;
		armyTabe->firstcount_ = armyPtr->getSoldierNum();
		if(dataBaseProxy_->db_update(*armyTabe,*armyTabe) != DB_Proxy::DB_SUCCEED){
			LOG_ERROR(("kickArmyOutEctype修改部队%d状态错误" , armyPtr->GetArmyID()));
		}
		fightEvent_->NotifyModifyArmyState(armyPtr->getPlayerID() , armyPtr->GetArmyID() , 
			armyPtr->getFromMapID() , 0, OUT_CITY_TRUST_CONTROY , armyPtr->GetWeaponID());

		armyPtr->refreshExp();
		players.insert(players.begin() , armyPtr->getPlayerID());
	}	
	map<int , Notify_Back_Army_Ectype>::iterator mapIter;
	for(mapIter = tmpMap.begin() ; mapIter != tmpMap.end() ; ++mapIter){
		Notify_Back_Army_Ectype& msgArmyBack = mapIter->second;
		if (msgArmyBack.iArmyCount > 0)
		{
			msgArmyBack.uEctypeID = ectypeID_;
			msgArmyBack.entranceID = entranceID_;
			ReqLeaveEctypeArmyInfo * leaveEctypePtr = (ReqLeaveEctypeArmyInfo *) new char 
				[sizeof(ReqLeaveEctypeArmyInfo) + sizeof(unsigned int) *msgArmyBack.iArmyCount];
			leaveEctypePtr->ucArmyNum = msgArmyBack.iArmyCount; 
			for (int i = 0 ; i < msgArmyBack.iArmyCount ; i++)
			{
				(*leaveEctypePtr)[i] = msgArmyBack.uArmyID[i];
			}
			leaveEctypePtr->serialize(curProcessID_ , MDM_GP_GAME_HALL_SERVER) ;
			event_notify(MDM_GP_GAME_HALL_SERVER , (char *) leaveEctypePtr , leaveEctypePtr->length);
			delete [] (char *)leaveEctypePtr;
			leaveEctypePtr = NULL;
			fightEvent_->NotifyEctypeArmyBack(msgArmyBack ,mapIter->first , curProcessID_ );
		}
	}
	return true;
}
bool EctypeFightManager::kickPlayerOutEctype(vector<unsigned int>& players){
	int playerNum = static_cast<int>(players.size());
	if (playerNum == 0){
		return false;
	}

	/*if (warType_ == FIELD_COUNTRYPART){
		return false;
	}*/
	MsgEctypeDelPlayer * msgDelEctypePlayer = (MsgEctypeDelPlayer *) new char [sizeof(MsgEctypeDelPlayer) + sizeof(unsigned int) * playerNum];
	if (msgDelEctypePlayer == NULL){
		return false ;
	}
	for(int i = 0 ; i < playerNum ; i ++){
		(*msgDelEctypePlayer)[i] = players[i];
	}
	msgDelEctypePlayer->ectypeID = ectypeID_;
	msgDelEctypePlayer->serialize(playerNum);
	set_timer_ms(1 , msgDelEctypePlayer);
	return true;
}
bool EctypeFightManager::processFightOver(int victoryCountryID){
	vector<unsigned int> players;
	kickArmyOutEctype(players);
	kickPlayerOutEctype(players);
	MsgEctypeFightOver * msgEctypeFightOver = (MsgEctypeFightOver*) new char[sizeof(MsgEctypeFightOver)];
	if (msgEctypeFightOver == NULL){
		isRun_ = false;
		return false;
	}
	msgEctypeFightOver->serialize(roomID_);
	set_timer_ms(1 , msgEctypeFightOver);
	
	
	statisticTotalFightInfo fightReport;
	memset(&fightReport , 0 , sizeof(statisticTotalFightInfo));
	fightReport.winer = victoryCountryID;
	fightRecord_->onOver(victoryCountryID);
	map<unsigned int , ActivityPlayerPtr>::const_iterator iter;
	const map<unsigned int , ActivityPlayerPtr>& activityPlayers = playerManager_->getPlayerMap();
	ActivityPlayerPtr activePlayer;
	for (iter=activityPlayers.begin();iter != activityPlayers.end();iter ++){
		activePlayer = iter->second;
		if (activePlayer && activePlayer->isStayRoom){	
			fightRecord_->queryFightingReport(activePlayer->countryID_ , activePlayer->playerID_ ,fightReport);
			fightReport.warType = warType_;
			int dataLen = sizeof(statisticTotalFightInfo) - (sizeof(StatisticArmyFightInfo) *(100 - fightReport.num));
			fightReport.serialize(dataLen,activePlayer->agent_,curProcessID_,activePlayer->playerID_,RSP_FIGHT_VOER_STATIC_INFO);
			fightReport.winer = victoryCountryID;
			sendMsgToAgent(activePlayer->agent_,(char*)&fightReport,dataLen);
		}
		if (victoryCountryID == activePlayer->countryID_){
			ECTYPE_EVENT ectypeEvent;
			ectypeEvent.serialize(activePlayer->agent_,activePlayer->playerID_,ectypeID_,0,fightReport.CLossNum,0);
			event_notify(ectypeEvent.to,(char*)&ectypeEvent,sizeof(ECTYPE_EVENT));
		}
	}
	
	if (warType_ == FIELD_COUNTRYPART)
	{
		EventEctypeOver eventEctypeOver;
		eventEctypeOver.entraceID = entranceID_;
		eventEctypeOver.serialize(curProcessID_ , MDM_GP_GAME_HALL_SERVER);
		event_notify(MDM_GP_GAME_HALL_SERVER , (char *)&eventEctypeOver , eventEctypeOver.length);
	}
	return true;
}

bool EctypeFightManager::processPlayerArmyDie(IArmy * armyPtr , int playerState , int realState){
	if(armyPtr == NULL){
		return false;
	}
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
void EctypeFightManager::processTrusteeship(ReqSetEctypeArmyTrust *reqMsg){
	if (reqMsg == NULL){
		return ;
	}
	IArmyPtr armyPtr = getArmyByID(reqMsg->ulArmyID);
	if (armyPtr == NULL){
		return ;
	}
	armyPtr->setTrusteeshipState(true);
}
void EctypeFightManager::release(void){
	map<unsigned int ,IArmyPtr>::iterator iter; 
	if (dataBaseProxy_){
		dataBaseProxy_->fini(); 
		CHECK_DELETE(dataBaseProxy_);
	}
	CHECK_DELETE(curProcessQueue_);
	CHECK_DELETE(processQueue_); 
	CHECK_DELETE(fightRecord_);
	CHECK_DELETE(fightOverManager_);
	CHECK_DELETE(seekPather_);
	CHECK_DELETE(renascenceMgr_);
	CHECK_DELETE(armyManager_);
	CHECK_DELETE(playerManager_);
	CHECK_DELETE(propManager_);
	CHECK_DELETE(dropGoodsManager_);
} 
int EctypeFightManager::notifyModifyArmyState(unsigned int playerid,unsigned int armyid, int mapid,int armyNum,int state,int weapon){
	return fightEvent_->NotifyModifyArmyState(playerid , armyid ,mapid ,armyNum ,state ,weapon);
}

void EctypeFightManager::exportAddArmyInfo(IArmyPtr armyPtr ,ARMY_ADD& addArmyInfo){

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
}
void EctypeFightManager::onCatchedHero(IArmy* army, HeroId heroId){
	fightRecord_->onCatchedHero(army ,heroId );
}

void EctypeFightManager::onBeCatchedHero(IArmy* army, HeroId heroId){
	fightRecord_->onBeCatchedHero(army ,heroId );
}

void EctypeFightManager::onHeroWasInjured(IArmy* army, HeroId heroId){
	fightRecord_->onHeroWasInjured(army , heroId);
}


void EctypeFightManager::onSoliderDie(IArmy*  /*attackArmy*/ , IArmy* army, unsigned int deathNum){
	fightRecord_->onSoliderDie(army , deathNum);
}


void EctypeFightManager::onKilledEnemy(IArmy* army, unsigned int killNum){
	fightRecord_->onKilledEnemy(army , killNum);
}

void EctypeFightManager::onArmyDie(IArmy* attackPtr ,IArmy* army){
	if (attackPtr == NULL || army == NULL){
		return ;
	}
	//
	army->clearEffect();
	armyManager_->onArmyDie(attackPtr , army);
	fightOverManager_->onArmyDie(attackPtr , army);
	armysMap_.erase(army->GetArmyID());
}


bool EctypeFightManager::isValidEctypeData(Notify_Enter_Ectype* msgReqCreate ,EctypeInfoTable * ectypeInfo){
	if (msgReqCreate == NULL || ectypeInfo == NULL){
		return false;
	}
	if (ectypeInfo->limitPlayerNum < msgReqCreate->playerCount){
		return false;
	}
	map<unsigned int , IArmyPtr>::const_iterator iter;
	const map<unsigned int , IArmyPtr> & armyMap = armyManager_->getArmyMap();
	int armyNum;
	for (int i = 0 ; i < msgReqCreate->playerCount ; i ++){
		armyNum = 0;
		for (iter = armyMap.begin(); iter != armyMap.end() ; iter++){
			if (msgReqCreate->getPlayerId(i)== iter->second->getPlayerID()){
				armyNum ++;
			}
		}
		if (armyNum > ectypeInfo->limitArmyNum){
			return false;
		}
	}
	return true;
}
void EctypeFightManager::driveBoutNum(bool isIncreaceBout){

	queue<unsigned int> * tmpProcessQueue = curProcessQueue_;
	curProcessQueue_ = processQueue_;
	processQueue_ = tmpProcessQueue;
	needProcessNum_ = static_cast<int>(curProcessQueue_->size());
	if (isIncreaceBout){
		curBoutNum_ ++;
	}
	else{
		return ;
	}
	
	EctypeTurnInfo ectyeBoutInfo;
	ectyeBoutInfo.RooID = getRoomID();
	ectyeBoutInfo.CurToTalArmyNum = needProcessNum_;
	ectyeBoutInfo.CurProArmyNum = processedNum_;
	ectyeBoutInfo.iTurnNum = curBoutNum_;
	msgBroadcast(&ectyeBoutInfo,RSP_ECTYPE_TURN_INFO,sizeof(EctypeTurnInfo));
	processedNum_ = 0;
}
void EctypeFightManager::startTimerCycle(unsigned int nt){
	FightNotfiy * msgFightTimer = new FightNotfiy;
	msgFightTimer->serialize(roomID_);
	set_timer_ms(nt , msgFightTimer);
}
void EctypeFightManager::onEnterWar(IArmyPtr armyPtr){
	fightRecord_->onEnterWar(armyPtr);
}
//处理副本添加部队
void EctypeFightManager::processAddArmy(ReqAddArmyToRoom * msgAddArmys){
	if(warType_ == LIMIT_BATTLEFIELD || warType_ == COUNTRYPART || msgAddArmys == NULL){
		return ;
	}
	RspAddArmyToRoom *rspAddArmyToRoom = (RspAddArmyToRoom*)new char [sizeof(RspAddArmyToRoom) + sizeof(RspInfo) * msgAddArmys->count];
	if( rspAddArmyToRoom ==NULL){
		return ;
	}
	EventArmyLeave * msgArmyLeavePtr = (EventArmyLeave *)new char [sizeof(EventArmyLeave) + sizeof(unsigned int ) * msgAddArmys->count];
	if (msgArmyLeavePtr == NULL)
	{
		delete [] (char *)rspAddArmyToRoom;
		return ;
	}
	NetProcesser * processer_ = NetProcesser::instance();        
	if (processer_ == NULL)
	{
		delete [] (char *)rspAddArmyToRoom;
		delete [] (char *)msgArmyLeavePtr;
		return ;
	}
	int armyNum = getArmyNum(msgAddArmys->from);
	rspAddArmyToRoom->state = false;
	rspAddArmyToRoom->count = 0;
	msgArmyLeavePtr->armyNum = 0;
	ActivityPlayerPtr managerPlayerInfo = processer_->getActivePlayer(msgAddArmys->from);
	if (managerPlayerInfo.get() != NULL)
	{
		if(managerPlayerInfo->doorID_ != entranceID_){
			for (int i = 0 ; i < msgAddArmys->count; i++){
				(*rspAddArmyToRoom)[rspAddArmyToRoom->count].armyId = (*msgAddArmys)[i];
				(*rspAddArmyToRoom)[rspAddArmyToRoom->count++].failReason = INVALID_ROOM_REQ;
			}
			rspAddArmyToRoom->serialize(curProcessID_ ,msgAddArmys->from );
			sendMsgToAgent(msgAddArmys->agent , (char *)rspAddArmyToRoom , rspAddArmyToRoom->length);
			delete [] (char *)rspAddArmyToRoom;
			delete [] (char *)msgArmyLeavePtr;
			return ;
		}
	}
	
	
	//获得位置
	for (int i = 0 ; i < msgAddArmys->count; i++){
		if (playerManager_->getPlayerNum() >= ectypeInfo_->limitPlayerNum)
		{
			//部队达到上限
			(*rspAddArmyToRoom)[rspAddArmyToRoom->count].armyId = (*msgAddArmys)[i];
			(*rspAddArmyToRoom)[rspAddArmyToRoom->count++].failReason = PLAYER_NUM_REACH_LIMIT;
			continue;
		}
		if (armyNum >= ectypeInfo_->limitArmyNum){
			//部队达到上限
			(*rspAddArmyToRoom)[rspAddArmyToRoom->count].armyId = (*msgAddArmys)[i];
			(*rspAddArmyToRoom)[rspAddArmyToRoom->count++].failReason = ARMY_NUM_REACH_LIMIT;
			continue;
		}
		ArmyTable armyTable;
		armyTable.setmasterid(msgAddArmys->fromMapID);
		armyTable.setdetailid((*msgAddArmys)[i]);
		if(dataBaseProxy_->db_select(armyTable) != DB_Proxy::DB_SUCCEED){
			//查询部队失败
			(*rspAddArmyToRoom)[rspAddArmyToRoom->count].armyId = (*msgAddArmys)[i];
			(*rspAddArmyToRoom)[rspAddArmyToRoom->count++].failReason = INVALID_ARMY_INFO;
			continue;
		}
		if (armyTable.state_ != 1){
			//部队状态非法
			(*rspAddArmyToRoom)[rspAddArmyToRoom->count].armyId = (*msgAddArmys)[i];
			(*rspAddArmyToRoom)[rspAddArmyToRoom->count++].failReason = INVALID_ARMY_INFO;
			continue;
		}
		IArmyPtr armyPtr = armyManager_->createPlayerArmy(armyTable);
		if (armyPtr == NULL){
			//创建部队失败
			(*rspAddArmyToRoom)[rspAddArmyToRoom->count].armyId = (*msgAddArmys)[i];
			(*rspAddArmyToRoom)[rspAddArmyToRoom->count++].failReason = ECTYPE_SYS_ERR;
			continue;
		}
		if (!armyManager_->isValidLevel(armyPtr->GetOfficerLevel()))
		{
			//部队达到上限
			(*rspAddArmyToRoom)[rspAddArmyToRoom->count].armyId = (*msgAddArmys)[i];
			(*rspAddArmyToRoom)[rspAddArmyToRoom->count++].failReason = INVALID_OFFICE_LEAVL_;
			continue;
		}
		if (getAttackCountryID() == NPC_)
		{
			EctypeArmyManager * ectypeArmyManager = dynamic_cast<EctypeArmyManager*>(armyManager_);
			if(ectypeArmyManager != NULL){
				labelMap_[1].label = 0;
				labelMap_[1].countryId = armyPtr->GetCountryID();
				labelMap_[1].doorId = -1;
				ectypeArmyManager->loadBuildInfo();
			}
			
		}
		rspAddArmyToRoom->state = true;
		POS_STRUCT pos = mapInfo_->getPropagatePos(0);
		pos = armyPtr->getValidPos(pos);
		armyPtr->setArmyPos(pos);
		armyManager_->addArmy(armyPtr);
		broadcastAddArmy(armyPtr);
		(*msgArmyLeavePtr)[msgArmyLeavePtr->armyNum ++]= (*msgAddArmys)[i];
		armyNum ++;
		
	}
	
	rspAddArmyToRoom->serialize(curProcessID_ ,msgAddArmys->from );
	sendMsgToAgent(msgAddArmys->agent , (char *)rspAddArmyToRoom ,rspAddArmyToRoom->length);
	if (rspAddArmyToRoom->state){
		ActivityPlayerPtr  playerInfo = getPlayerInfo(msgAddArmys->from);
		if (playerInfo == NULL){
			playerInfo = ActivityPlayerPtr(new ActivityPlayer);
			if (playerInfo != NULL){
				memset(playerInfo.get() , 0 , sizeof(ActivityPlayer));
				playerInfo->playerID_ = msgAddArmys->from;
				playerInfo->fightRoomID_ = msgAddArmys->roomID;
				playerInfo->ectypeID_ = mapInfo_->getMapID();
				playerInfo->doorID_ = entranceID_;
				PlayerChatRoomTable playerChatInfo;
				playerChatInfo.setid(msgAddArmys->from);
				if(dataBaseProxy_->db_select(playerChatInfo) == DB_Proxy::DB_SUCCEED){
					playerInfo->countryID_ = playerChatInfo.country_ ;
				}
				else
				{
					LOG_ERROR(("无法查找到玩家%d阵营信息" , msgAddArmys->from));
				}
				playerManager_->addActivePlayer(playerInfo);
			}
		}
		processer_->insertPlayer(playerInfo);
		msgArmyLeavePtr->armyState = ARMY_FIGHT_ECTYPE;
		msgArmyLeavePtr->serialize(curProcessID_ ,msgAddArmys->fromMapID);
		event_notify(msgAddArmys->fromMapID , (char *) msgArmyLeavePtr , msgArmyLeavePtr->length);
		RspEnterEctypeSystem Rsp;
		memset(&Rsp, 0, sizeof(RspEnterEctypeSystem));
		Rsp.isSigleEctype = true;
		Rsp.EctypeID = ectypeID_;
		Rsp.flag = 1;
		Rsp.EcServerID = curProcessID_;
		Rsp.EctypeRoomID = 0;
		Rsp.NpcRoomID = msgAddArmys->roomID;
		Rsp.Ectypemapid = mapInfo_->getMapID();	
		Rsp.serialize(sizeof(RspEnterEctypeSystem), msgAddArmys->agent, curProcessID_, msgAddArmys->from, RSP_ENTER_ECTYPE_SYSTEM_INFO);
		asynch_write(Rsp.agent,(char*)&Rsp,Rsp.length);
	}
	delete [] (char *)rspAddArmyToRoom;
	delete [] (char *)msgArmyLeavePtr;
}


void EctypeFightManager::createLabelMap(map<int , int>& countryMap , map<int , list<WarEntranceInfo *>>* warEntrance){
	if (warEntrance == NULL){
		if (countryMap.size() == 1){
			labelMap_[0].label = 0;
			labelMap_[0].countryId = NPC_;
			labelMap_[0].doorId = -1;
			labelMap_[1].label = 1;
			labelMap_[1].countryId = countryMap.begin()->second;
			labelMap_[1].doorId = -1;
			labelMap_[2].label = 2;
			labelMap_[2].countryId = NPC_;
			labelMap_[2].doorId = -1;
			return ;
		}
		else
		{
			labelMap_[0].label = 0;
			labelMap_[0].countryId = NPC_;
			labelMap_[0].doorId = -1;
			labelMap_[1].label = 1;
			labelMap_[1].countryId = NPC_;
			labelMap_[1].doorId = -1;
			labelMap_[2].label = 2;
			labelMap_[2].countryId = NPC_;
			labelMap_[2].doorId = -1;
		}
	}
	else
	{
		list<WarEntranceInfo *> & tmpList = (*warEntrance)[getRandomValue(1 , warEntrance->size())];
		list<WarEntranceInfo *>::iterator iter;
		for(iter = tmpList.begin() ; iter != tmpList.end() ; ++iter){
			labelMap_[(*iter)->label].label = (*iter)->label;
			labelMap_[(*iter)->label].doorId = (*iter)->buildId;
		}
		if (labelMap_.size() != countryMap.size())
		{
			assert(0 && "战场阵营数据配置错误！");
		}
		map<int , DetailEntranceInfo>::iterator mapIter;
		map<int , int>::iterator mapCountryIter;
		int index = 0;
		mapCountryIter = countryMap.begin();
		for (mapIter = labelMap_.begin() ; mapIter != labelMap_.end(); ++mapIter){
			mapIter->second.countryId = mapCountryIter->second;
			++mapCountryIter;
		}
	}
}
//资源收集
void EctypeFightManager::collectRes(IArmyPtr armyPtr){
	if (armyPtr->getBuildDetailType() != ARCH_RES_BUILDING){
		return ;
	}
	if (armyPtr->GetCountryID() == NPC_){
		return ;
	}
	map<int , map<int , int>>::iterator iter = resCollect_.find(armyPtr->GetCountryID());
	map<int ,int>::iterator subIter;
	if (iter == resCollect_.end()){
		resCount_ ++;
		resCollect_[armyPtr->GetCountryID()][armyPtr->GetAttackType()] = armyPtr->GetAttackPower();
	}
	else
	{	
		subIter = iter->second.find(armyPtr->GetAttackType());
		if (subIter == iter->second.end()){
			resCount_ ++;
			iter->second[static_cast<int>(armyPtr->GetAttackType())] = armyPtr->GetAttackPower();
		}
		else
		{
			subIter->second += armyPtr->GetAttackPower();
		}
	}
	fightOverManager_->onUpdateRes(armyPtr->GetCountryID(),armyPtr->GetAttackType() ,resCollect_[armyPtr->GetCountryID()][armyPtr->GetAttackType()]);
}
//void EctypeFightManager::OnContributeInfo(CITY_INFO * cityInfo)
//{
//	if (iDataSize < sizeof(CITY_INFO))
//	{
//		LOG_TRACE_EX(false,("CBuildProduce::OnContributeInfo-->消息包非法：玩家%d\n",lPlayerID));
//		return ;
//	}
//
//	CITY_INFO * pCity = (CITY_INFO *)pDataBuffer;
//
//	int ret = 0;
//	PlayerChatRoomTable ResByPlayer;
//	ERROR_INFO ErrorInfo;
//	ResByPlayer.setid(lPlayerID);
//	if (g_Proxy.db_select(ResByPlayer) != DB_Proxy::DB_SUCCEED)
//	{
//		ErrorInfo.serialize(EN_DATA_ERROR, m_lMapID, lPlayerID);
//		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
//		return;
//	}
//	Treasury Tr;
//	Tr.setid(ResByPlayer.country_);
//	ret = g_Proxy.db_select(Tr);
//	if (ret == DB_Proxy::DB_FAILED)
//	{
//		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, m_lMapID, lPlayerID);
//		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
//		return;
//	}
//	else if (ret == DB_Proxy::DB_EOF)
//	{
//		Tr.money_ = 0;
//	}
//
//	PublicBuildTable PublicBuild;
//	PublicBuild.setmasterid(pCity->lMapID);
//	PublicBuild.setdetailid(pCity->lCityID);
//	if (g_Proxy.db_select(PublicBuild) != DB_Proxy::DB_SUCCEED)
//	{
//		return;
//	}
//	int iTolMoney = 0;
//	PublicBuildParamTable * pBuildParam = g_LoadDBinfo.GetPublicBuildParam(PublicBuild.pbuildingkind_, PublicBuild.currentlevel_);
//	if (pBuildParam == NULL)
//	{
//		LOG_TRACE_EX(false,("CBuildProduce::OnContributeInfo-->该公共建筑%d-%d不存在：玩家%d\n",PublicBuild.pbuildingkind_, PublicBuild.currentlevel_,lPlayerID));
//		return;
//	}
//	PublicBuildParamTable * pBuildParam1 = g_LoadDBinfo.GetPublicBuildParam(PublicBuild.pbuildingkind_, PublicBuild.currentlevel_+1);
//	if (pBuildParam1 != NULL) iTolMoney=pBuildParam1->upgradefee_;
//
//	CONTRIBUTE_INFO ConInfo;
//	ConInfo.serialize(pCity->lCityID, PublicBuild.currentlevel_, PublicBuild.currentmoney_, iTolMoney, pBuildParam->attack_, pBuildParam->hp_,
//		pBuildParam->attackscop_, pBuildParam->visualrange_, Tr.money_, ResByPlayer.money_, pCity->lMapID, lPlayerID);
//	ret = asynch_write(handle, (char *)&ConInfo, ConInfo.length);
//}