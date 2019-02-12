#include "CountryFightManager.h"
#include <boost/serialization/singleton.hpp>

#include "log.hpp"
#include "socketsvc.hpp"


#include "common.h"
#include "IniFile.h"
#include "ProNPCSystem.h"
#include "LoadMapInfo.h"
#include "LoadDBInfo.h"
#include "Time_Protocol.h"
#include "CommInterfaceHeader.h"
#include "ServerProtocolHeader.h"
#include "MapServerProtocolHeader.h"
#include "computinghurt/randomgenerator.h"
#include "FightSystemMacro.h"
#include "statisticscommon.h"
#include "ProLogicSystem.h"
#include "NPCInfo.h"

#include "CountryArmyManager.h"
#include "PlayerManager.h"
#include "PlayerArmy.h"
#include "NpcArmy.h"
#include "BuildArmy.h"
#include "FightEvent.h"
#include "FightOverManager.h"
#include "SeekPathArithm.h"
#include "BroadcastManager.h"
#include "RenascenceMgr.h"
#include "ISystemLog.h"

CountryFightManager::CountryFightManager(void)

{
}
CountryFightManager::~CountryFightManager(void)
{
}

void CountryFightManager::startTimerCycle(unsigned int nt){
	FightNotfiy * msgFightTimer = new FightNotfiy;
	msgFightTimer->serialize(mapInfo_->getMapID());
	set_timer_ms(nt , msgFightTimer);
}
bool CountryFightManager::Initialize(CLoadMapInfo * mapInfo , CLoadDBInfo * dataBaseMemData , IniFile * iniFile){
	if (mapInfo == NULL || dataBaseMemData == NULL || iniFile == NULL){
		return false;
	}
	dropGoodsManager_ = new DropGoodsManager(this);
	if (dropGoodsManager_ == NULL){
		return false;
	}
	fightScene_ = County_AI;
	dataBaseMemData_ = dataBaseMemData;
	mapInfo_ = mapInfo;
	iniFile_ = iniFile;
	
	randomgenerator_ = &boost::serialization::singleton<computinghurt::RandomGenerator>::get_mutable_instance();
	fightReport_ = new statisticssystem::BattlefieldStatisticsAdapter;
	if (fightReport_ == NULL){
		return false;
	}

	dataBaseProxy_ = getNewDbgProxy();
	if (dataBaseProxy_ == NULL){
		return false;
	}
	npcInfo_ = new CNPCInfo;
	if (npcInfo_ == NULL){
		return false;
	}
	propManager_ = new PropManager(this);
	if (propManager_ == NULL){
		return false;
	}
	taskManager_ = new TaskManager(this);
	if (taskManager_ == NULL){
		return false;
	}
	npcInfo_->InitNpc(dataBaseProxy_);
	patrolMap_ = npcInfo_->getPatrolData(mapInfo_->getMapID());
	curProcessID_ = mapInfo_->getMapID();
	broadcastPackManager_ = new BroadcastManager;
	if (broadcastPackManager_ == NULL){
		return false;
	}
	if(broadcastPackManager_->initialize(curProcessID_) == false){
		return false;
	}
	processActiveNum_ = 80;
	curProcessQueue_ = new queue<unsigned int>;
	processQueue_    = new queue<unsigned int>;
	fightOverManager_ = new FightOverManager(100000000, this);
	if (curProcessQueue_ == NULL || processQueue_ == NULL || fightOverManager_ == NULL){
		return false;
	}
	seekPather_ = new SeekPath(mapInfo);
	if (seekPather_ == NULL){
		return false;
	}
	playerManager_ = new PlayerManager(iniFile_);
	if (playerManager_ == NULL){
		return false;
	}

	//初始化战斗记录
	fightRecord_ = new statisticssystem::FightingStatisticsAdapter(lossRate_);
	if (fightRecord_ == NULL){
		return false;
	}
	
	setLabelMap(-1);

	//初始化部队管理器
	CountryArmyManager * tmpArmyManager = new CountryArmyManager;
	armyManager_ = tmpArmyManager;
	if (armyManager_ == NULL){
		return false;
	}
	mapDataAndFraise_.ClassImpedient = mapInfo_;
	mapDataAndFraise_.MapData = getArmyList();

	//初始化复活复制管理器
	renascenceMgr_ = new RenascenceMgr(3001,9999 ,this);
	if (renascenceMgr_ == NULL)
	{
		return false;
	}

	if(tmpArmyManager->Initialize(mapInfo_ , npcInfo_ ,iniFile_ , dataBaseProxy_ ,dataBaseMemData,this) == false){
		return false;
	}

	taskManager_->loadDayTask();
	if(mapInfo_->GetMapState() == war && getAttackCountryID() < 0){
		setMapState(peace);
		//战斗结束消息
		EndFight eventFightOver;
		eventFightOver.cmd = CMD_END_FIGHT; 
		eventFightOver.uMapID = mapInfo_->getMapID();
		eventFightOver.winer = mapInfo_->GetMapOwer() ; // 1 = 部落  2 =  联盟
		eventFightOver.serialize(sizeof(EndFight),4000,curProcessID_,SERVER_BOUND_MAP_OTHER,CMD_END_FIGHT);
		event_notify(SERVER_BOUND_MAP_OTHER,(char*)&eventFightOver,sizeof(EndFight));
	}
	effectManager_.setFightManager(this);



	if (mapInfo_->GetMapState() == war){
		startFight(false , getAttackCountryID());
	}

	//启动战斗系统
	startTask();
	return true;
}

void CountryFightManager::parse(const char * buf){
	if (buf == NULL){
		return ;
	}
	DISTRIBUTED_HEADER * msgHeader = (DISTRIBUTED_HEADER *)buf;
	switch(msgHeader->command){
	case EN_C_TIMER_NOTIFY:
		processTimerEvent((TIMER_HEADER *)buf);
		break;
	case EN_C_COMEIN_MAP:
		processEnterMap((COMEIN_MAP*)buf);
		break;
	case REQ_ENTER_FIGHT_SYSTEM:
		processEnterSystem((REQ_ENTER_FIGHT_SYSTEM_STRUCT*) buf);
		break;
	case REQ_SELECT_MOVE_POS:
		processMove((REQ_MOVE_INFO_STRUCT*)buf);
		break;
	case REQ_SELECT_TRUST_STRATEY:
		processTrusteeship((REQ_TRUST_INFO_STRUCT *)buf);
		break;
	case REQ_SELECT_ATTACK_TARGET:
		processAttack((REQ_ATTACK_TARGET_STRUCT*)buf);
		break;
	case REQ_ARMY_INFO:
		processReqArmyInfo((REQ_ARMY_INFO_STRUCT*)buf);
		break;
	case REQ_LEAVE_FIGHT_SYSTEM_INFO:
		processLeaveSystem((REQ_LEAVE_FIGHT_SYSTEM_STRUCT*) buf);
		break;
	case REQ_ARMY_GO_OUT_INFO:
		processArmyOut((REQ_ARMY_GO_OUT_STRUCT*)buf);
		break;
	case REQ_EXIT_FIGHT_SYSTEM_INFO:
		processExitSystem((REQ_EXIT_FIGHT_SYSTEM_STRUCT*)buf);
		break;
	case EN_C_DELETEARMY:
		processDelArmyMsg((tagDeleteArmy_Request*) buf);
		break;
	case EN_C_EDIT_ARMY:
		processEditArmy((EDIT_ARMY*)buf);
		break;
	case EN_C_FIGHT_CREATEARMY:
		processCreateArmy((tagFIGHT_CreateArmy_Request*)buf);
		break;
	case CMD_BACK_ARMY_REACH:
		processArmyOutRsp((tagBackArmy_Reach*) buf);
		break;
	case CMD_ARMY_ARRIVED:
		processArmyReach((tagArmy_Reach*) buf);
		break;
	case CMD_BACK_ARMY_ECTYPE:
		processEctypeBack((Notify_Back_Army_Ectype*)buf);
		break;
	case EN_C_DELNPCARMY:
		processDelPlayerArms((tagNotify_DelNPCArmy_Request*)buf);
		break;
	case CMD_HERO_STUDY_SKILL:
		processHeroNewSkill((HeroStudySkillEvent*)buf);
		break;
	case CMD_UPDATE_HERO_PARAM:
		processUpdateHeroParam((heroModifyParamEvent*)buf);
	case EN_C_MODIFY_PUBLICBUILD:
		processUpdateBuildLevel((tagModify_PublicBuild_Request *)buf);
		break;
	case CMD_HERO_CLEAR_SKILL:
		processHeroClearSkill((HeroStudySkillEvent*)buf);
		break;
	case REQ_USE_PROP_CARD:
		processUsePropCard((ReqUsePropCard*)buf);
		break;
	case EVENT_ARMY_LEAVE:
		processArmyLeave((EventArmyLeave *) buf);
		break;
	case EN_C_NOTIFY_DAYTASKOFPLAYER:
		taskManager_->receiveTask(((tagNotify_DayTaskOfPlayer_Request *)buf)->playerID,(tagNotify_DayTaskOfPlayer_Request *)buf);
		break;
	default:
		break;
	}
	delete [] buf;
}	
void CountryFightManager::driveBoutNum(bool isIncreaceBout){
	
	queue<unsigned int> * tmpProcessQueue = curProcessQueue_;
	curProcessQueue_ = processQueue_;
	processQueue_ = tmpProcessQueue;
	if (isIncreaceBout){
		curBoutNum_ ++;
	}
	needProcessNum_ = static_cast<int>(curProcessQueue_->size());
	processedNum_ = 0;
}

void CountryFightManager::msgBroadcast(DISTRIBUTED_HEADER * msgData , PROTOCOL_FIGHT fightCmd  , int dataLen){
	if (msgData == NULL){
		return ;
	}
	//if (mapInfo_->GetMapState() != war){
	//	msgData->serialize(dataLen,0,mapInfo_->getMapID(),0,fightCmd);
	//	broadcast((char *)msgData , dataLen);
	//	return;
	//}
	switch(fightCmd){
	case RSP_FIGHT_INFO:
	case RSP_MOVE_INFO:
	case DESTROY_ARMY_INFO:
	case RSP_TURN_INFO:
	//case RSP_DELETE_ARMY_INFO:
	//case ADD_ARMY_INFO:
		broadcastPackManager_->insertBroadData(msgData , fightCmd , dataLen);
		break;
	default:
		msgData->serialize(dataLen,0,mapInfo_->getMapID(),0,fightCmd);
		broadcast((char *)msgData , dataLen);
		break;
	}
}

bool CountryFightManager::scheduler(){
	DWORD st  =  GetTickCount();
	armyManager_->processDieArmy();
	armyManager_->quitQueue();
	resetArmyCardDropRecord();
	taskManager_->tidyTask();
	int curProcessActiveNum = 0;
	IArmyPtr armyPtr;
	bool isProcessedArmy = false;
	int victoryCountyID = 0;
	renascenceMgr_->getRebornAndReproNpc(County_AI);
	if (mapInfo_->GetMapState() == war && fightOverManager_->isFightOver(victoryCountyID) == true){
		processFightOver(victoryCountyID);
	}
	processGuardArmy();
	if (curBoutNum_ < 20){
		driveBoutNum(true);
		broadcastTurnInfo();
		startTimerCycle(FIGHT_TIMER_CYCLE);
		curCycleNum_++;
		return true;
	}
	unsigned int armyId;
	while (curProcessQueue_->size() > 0){
		armyId = curProcessQueue_->front();
		armyPtr = getArmyByID(armyId);
		curProcessQueue_->pop();
		if (armyPtr == NULL){
			eraseProcessQueue(armyId);
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
			eraseProcessQueue(armyId);	
		}
		processedNum_ ++;
	}
	driveBoutNum(false);
	while (curProcessQueue_->size() > 0){
		armyId = curProcessQueue_->front();
		armyPtr = getArmyByID(armyId);
		curProcessQueue_->pop();
		if (armyPtr == NULL){
			eraseProcessQueue(armyId);
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
			eraseProcessQueue(armyId);
		}
		processedNum_ ++;
	}

	if (isProcessedArmy){
		curIdleBoutNum_ ++;
	}
	else{
		curIdleBoutNum_ = 0;
	}
	broadcastTurnInfo();
	if (curProcessQueue_->size() == 0){
		driveBoutNum(true);
	}
	printf("消耗%d\n" ,  GetTickCount() - st);
	int realt = FIGHT_TIMER_CYCLE - GetTickCount() + st;
	if (realt <= 100){
		realt = 100;
	}
	startTimerCycle(static_cast<unsigned int>(realt));
	curCycleNum_++;
	return true;
}

void CountryFightManager::broadcastTurnInfo(void){
	if(mapInfo_->GetMapState() != war){
		return broadcastPackManager_->msgBroadcastData();
	}
	RspTurnInfo  boutInfo;
	boutInfo.CurToTalArmyNum = needProcessNum_;
	boutInfo.CurProArmyNum = processedNum_;
	boutInfo.iTurnNum = curBoutNum_;
	boutInfo.mapID = mapInfo_->getMapID();
	msgBroadcast(&boutInfo,RSP_TURN_INFO,sizeof(RspTurnInfo));
	broadcastPackManager_->msgBroadcastData();
}
//重新设置回合信息
void CountryFightManager::resetTurnInfo(void){
	curBoutNum_ = 0;
	processedNum_ = 0;
	needProcessNum_ = 0;
	processedNum_ = 0;
	curCycleNum_ = 0;
	curIdleBoutNum_ = 0;
}
void CountryFightManager::setMapState(mapstate state){
	MapInfoTable mapTable; 
	mapTable.setid(mapInfo_->getMapID());
	mapTable.countryid_ = mapInfo_->GetMapOwer();
	mapTable.state_ = state;
	if(dataBaseProxy_->db_update(mapTable , mapTable) != DB_Proxy::DB_SUCCEED){
		LOG_ERROR(("更新地图%d状态失败！" ,mapInfo_->getMapID()));
	}
	mapInfo_->SetMapState(state);
};
void CountryFightManager::startFight(bool isNeedSendReport , int attackCountry){
	setLabelMap(attackCountry);
	if (mapInfo_->GetMapState() == peace){
		setMapState(war);
	}
	//重置战斗统计和战报
	fightRecord_->reset();
	fightReport_->reset();
	//发送战斗开始战报
	FightingStartInfo msgFightStartInfo;
	fightReport_->onStart(mapInfo_->GetMapOwer() , attackCountry, mapInfo_->getMapID() , msgFightStartInfo);
	fightRecord_->onStart();
	
	const map<unsigned int , IArmyPtr>& armyMap = armyManager_->getArmyMap();
	IArmyPtr armyPtr;
	map<unsigned int , IArmyPtr>::const_iterator iter;
	for (iter = armyMap.begin();iter != armyMap.end();iter ++){
		armyPtr = iter->second;
		if (armyPtr == NULL){
			continue;
		}
		if (armyPtr->GetArmyType() != BUILD){
			onEnterWar(armyPtr);
		}
	}
	if (isNeedSendReport){
		//战斗结束新闻消息通知
		Notify_Add_News_Report fightReportEvent;
		memset(&fightReportEvent,0,sizeof(Notify_Add_News_Report));
		memcpy(fightReportEvent.szConent,&msgFightStartInfo , sizeof(FightingStartInfo));
		fightReportEvent.type = EVENT_FIGHT_START;
		fightReportEvent.serialize(0);
		event_notify(MDM_GP_MAP_CHAT,(char*)&fightReportEvent,sizeof(Notify_Add_News_Report));
	}
	resetTurnInfo();
	
}
bool CountryFightManager::getOutArmyPos(int lastMapID , POS_STRUCT & pos){

	map<unsigned long ,unsigned long>::iterator  doorIter = mapInfo_->m_MapToDoorList.find(lastMapID - 1000);
	list<EncampmentInfo>::iterator encampIter;
	if (doorIter != mapInfo_->m_MapToDoorList.end()){
		for (encampIter = mapInfo_->EncampmentPos.begin() ; encampIter!= mapInfo_->EncampmentPos.end() ; encampIter++)
		{
			if ((*encampIter).uEncampentID == (*doorIter).second){
				pos = (*encampIter).EncapentPos;
				return true;
			}
		}
	}
	pos.PosX = 0;
	pos.PosY = 0;
	pos.weight = 0;
	return false;
}
bool CountryFightManager::getGatewayPos(int entraceID , POS_STRUCT & pos){

	if (entraceID != -1){
		list<Propagate>::iterator gateIter;
		for (gateIter = mapInfo_->propagate_list.begin() ;gateIter != mapInfo_->propagate_list.end() ;gateIter ++){
			if ((*gateIter).uPropagateID == static_cast<unsigned int>(entraceID)){
				pos = (*gateIter).Pos[0];
				return true;
			}
		}
	}
	pos.PosX = mapInfo_->GovPos.centryPos.PosX;
	pos.PosY = mapInfo_->GovPos.centryPos.PosY;
	pos.weight = 0;
	return false;
}
void CountryFightManager::processMove(REQ_MOVE_INFO_STRUCT* buf){
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
	/*if(mapInfo_->GetMapState() == war){
		armyPtr->processMoveMsg(tmpPos);
	}
	else{
		vector<POS_STRUCT> vecPos;
		vecPos.push_back(tmpPos);
		armyPtr->Move(vecPos , armyPtr);
	}*/
	armyPtr->processMoveMsg(tmpPos);
}

void CountryFightManager::processAttack(REQ_ATTACK_TARGET_STRUCT* buf)
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
void CountryFightManager::processDelArmyMsg(tagDeleteArmy_Request* msgDelArmy){
	if (msgDelArmy == NULL){
		return ;
	}
	IArmyPtr armyPtr = getArmyByID(msgDelArmy->ArmyID);
	if (isValidArmy(armyPtr) == true){
		ArmyTable * armyTable = NULL;
		armyPtr->getArmyTable(armyTable);
		if (armyTable == NULL){
			return ;
		}
		if(dataBaseProxy_->db_delete(*armyTable) != DB_Proxy::DB_SUCCEED){
			LOG_ERROR(("删除部队%d信息失败processDelArmyMsg" , armyPtr->GetArmyID()));
		}
		int realState = 0;
		PlayerOfficerTable playerOfficerTable;
		memset(&playerOfficerTable , 0 , sizeof(PlayerOfficerTable));
		playerOfficerTable.setmasterid(armyPtr->getPlayerID());
		playerOfficerTable.setdetailid(armyPtr->GetOfficerID());

		playerOfficerTable.state_  = -1;
		playerOfficerTable.armyid_ = - (int) armyPtr->GetArmyID();

		dataBaseProxy_->db_increase(playerOfficerTable);
		if(playerOfficerTable.state_ != realState){
			//
			LOG_ERROR(("玩家%d英雄%d实际修改后状态%d要求状态%d错误位置processDelArmyMsg\r\n" ,playerOfficerTable.playerid_ ,
				playerOfficerTable.officerid_,playerOfficerTable.state_,realState));
			playerOfficerTable.state_ = realState;
			dataBaseProxy_->db_update(playerOfficerTable , playerOfficerTable);
		}
		processDelArmy(armyPtr , ARMY_ANNIHILATE);
	}
}


void CountryFightManager::processUpdateBuildLevel(tagModify_PublicBuild_Request * msgBuildData){
	if (msgBuildData == NULL){
		return ;
	}
	IArmyPtr armyPtr = getArmyByID(msgBuildData->BuildID % 10000);
	if (armyPtr == NULL){
		return ;
	}
	BuildArmy *buildArmyPtr = dynamic_cast<BuildArmy *>(armyPtr);
	if (buildArmyPtr == NULL){
		return ;
	}
	//地基可以创建其他类型建筑
	if (buildArmyPtr->getBuildDetailType() != ARCH_BUILD_TOFT && 
		buildArmyPtr->getBuildDetailType() != msgBuildData->BuildType)
	{
		return ;
	}
	computinghurt::BuildingInfo buildInfo;
	PublicBuildParamTable* param = dataBaseMemData_->GetPublicBuildParam
		(msgBuildData->BuildType , msgBuildData->CurrentLevel);
	if (param == NULL){
		return;
	}
	if(param->templateid_ != buildArmyPtr->getTemplateID()){
		clearAttachedNpc(buildArmyPtr , true);
		buildArmyPtr->setTemplateID(param->templateid_);
		attachNpc(buildArmyPtr , buildArmyPtr->getTemplateID() ,buildArmyPtr->getCentryPos() ,County_AI);
	}
	DefendAttribute * defendAttribute = dataBaseMemData_->GetDefendAttribute(param->denfenid_);
	if (defendAttribute)
	{
		buildInfo.physicDefense = defendAttribute->physiceRec;
		buildInfo.magicDefense = defendAttribute->magicRec;
	}
	buildInfo.hp = param->hp_;
	buildInfo.power = param->attack_;
	buildArmyPtr->upgrade(buildInfo, param);
	buildArmyPtr->setDetailType(static_cast<ARCH_TYPES>(msgBuildData->BuildType));
	armyManager_->insertArmyToList(buildArmyPtr);
}
void CountryFightManager::processHeroClearSkill(HeroStudySkillEvent* msgNewSkill){
	if (msgNewSkill == NULL){
		return ;
	}
	IArmyPtr armyPtr = getArmyByID(msgNewSkill->ulArmyID);
	if (armyPtr == NULL){
		return ;
	}
	PlayerArmy * playerArmy = dynamic_cast<PlayerArmy *>(armyPtr);
	if (playerArmy == NULL){
		return ;
	}
	computinghurt::SkillAndGradeStru skillInfo;
	skillInfo.id = static_cast<SKILL_TYPE>(msgNewSkill->ulskillID);
	skillInfo.level = msgNewSkill->iLevel;
	playerArmy->abandonSkill(skillInfo);
}
void CountryFightManager::processHeroNewSkill(HeroStudySkillEvent* msgNewSkill){
	if (msgNewSkill == NULL){
		return ;
	}

	IArmyPtr armyPtr = getArmyByID(msgNewSkill->ulArmyID);
	if (armyPtr == NULL){
		return ;
	}
	PlayerArmy * playerArmy = dynamic_cast<PlayerArmy *>(armyPtr);
	if (playerArmy == NULL){
		return ;
	}
	computinghurt::SkillInfo skillInfo;
	if(!getSkillInfo(skillInfo ,static_cast<SKILL_TYPE>(msgNewSkill->ulskillID) , msgNewSkill->iLevel)){
		return ;
	}
	playerArmy->upgradeOrLearnedSkill(skillInfo);
}
//刷新英雄参数
void CountryFightManager::processUpdateHeroParam(heroModifyParamEvent* msgHeroParam){
	if (msgHeroParam == NULL){
		return ;
	}

	PlayerOfficerTable playerInfo;
	playerInfo.setmasterid(msgHeroParam->PlayerID);
	playerInfo.setdetailid(msgHeroParam->OfficerID);
	if(dataBaseProxy_->db_select(playerInfo) != DB_Proxy::DB_SUCCEED){
		LOG_ERROR(("查询玩家%d英雄%d失败位置processUpdateHeroParam", msgHeroParam->PlayerID , msgHeroParam->OfficerID));
		return ;
	}
	IArmyPtr armyPtr = getArmyByID(playerInfo.armyid_);
	if (armyPtr == NULL){
		return ;
	}
	PlayerArmy * playerArmy = dynamic_cast<PlayerArmy *>(armyPtr);
	if (playerArmy == NULL){
		LOG_ERROR(("查询玩家%d部队%d失败位置processUpdateHeroParam", msgHeroParam->PlayerID , playerInfo.armyid_));
		return ;
	}
	computinghurt::HeroAttribute heroAttribute;
	heroAttribute.agile = playerInfo.celerity_ + playerInfo.levelcelerity_ + playerInfo.equipcelerity_;
	heroAttribute.endurance = playerInfo.stamina_ + playerInfo.levelstamina_ + playerInfo.equipstamina_;
	heroAttribute.intelligence = playerInfo.wit_ + playerInfo.levelwit_ + playerInfo.equipwit_;
	heroAttribute.power = playerInfo.power_ + playerInfo.levelpower_ + playerInfo.equippower_;
	heroAttribute.physicAttack = playerInfo.attack_;
	heroAttribute.physicDef = playerInfo.physicrecovery_;
	heroAttribute.magicDef = playerInfo.magicrecovery_;
	heroAttribute.magicAttack = playerInfo.magicattack_;
	heroAttribute.HP = playerInfo.herohp_;
	heroAttribute.selfBeinjuredRate = playerInfo.injurerate_;
	heroAttribute.selfCapturedRate = playerInfo.capturerate_;
	heroAttribute.enemyBeinjuredRate = playerInfo.beinjurerate_;
	heroAttribute.ememyCapturedRate = playerInfo.becapturerate_;
	heroAttribute.cruelAttackRate = playerInfo.forcerate_;
	playerArmy->upgradeLevel(heroAttribute , playerInfo);
}
void CountryFightManager::processEctypeBack(Notify_Back_Army_Ectype * msgArmyBack){
	if (msgArmyBack == NULL){
		return ;
	}
	GAME_HALL_TYPE warType = COUNTRYPART;
	EctypeInfoTable *ectypePtr =  npcInfo_->getEctypeInfoTable(msgArmyBack->entranceID);
	if (ectypePtr != NULL)
	{
		warType = static_cast<GAME_HALL_TYPE>(ectypePtr->warType);
	}
	for(int i = 0 ; i < msgArmyBack->iArmyCount; i++){
		IArmyPtr armyPtr = getArmyByID(msgArmyBack->uArmyID[i]);
		if (armyPtr != NULL){
			continue ;
		}
		ArmyTable armyTable;
		armyTable.setmasterid(mapInfo_->getMapID());
		armyTable.setdetailid(msgArmyBack->uArmyID[i]);
		if(dataBaseProxy_->db_select(armyTable) != DB_Proxy::DB_SUCCEED){
			LOG_ERROR(("查找数据错误， 部队ID%d" ,msgArmyBack->uArmyID[i]));
			continue ;
		}
		if (armyTable.state_ == 11){
			armyTable.state_ = 1;
			if(dataBaseProxy_->db_update(armyTable , armyTable) != DB_Proxy::DB_SUCCEED){
				LOG_ERROR(("processEctypeBack刷新部队状态错误， 部队ID%d" ,msgArmyBack->uArmyID[i]));
			}
		}
		armyPtr = armyManager_->createPlayerArmy(armyTable);
		if (armyPtr == NULL){
			LOG_ERROR(("创建部队错误， 部队ID%d ，" ,msgArmyBack->uArmyID[i]));
			return ;
		}
		int limitNum = 3;
		if (armyManager_->isNewbie())
		{
			limitNum = 10;
		}
		POS_STRUCT tmpPos;
		getGatewayPos(msgArmyBack->entranceID , tmpPos);
		if (warType == FIELD_COUNTRYPART)
		{
			tmpPos = armyPtr->getValidPos(tmpPos ,limitNum , 4);
		}
		else
		{
			tmpPos = armyPtr->getValidPos(tmpPos , limitNum ,2);
		}
		
		armyPtr->setArmyPos(tmpPos);
		if(armyManager_->checkAddArmy(armyPtr) == true){
			broadcastAddArmy(armyPtr);
		}
		if (mapInfo_->GetMapState() == peace && mapInfo_->GetMapOwer() != armyPtr->GetCountryID()){
			
			startFight(true , armyPtr->GetCountryID());
		}
	}
}
void CountryFightManager::processArmyReach(tagArmy_Reach * msgArmyReach){
	if(msgArmyReach == NULL){
		return ;
	}
	if(msgArmyReach->lToMapID != mapInfo_->getMapID()){
		LOG_ERROR(("出征到达消息错误 ， 部队ID%d , 来源郡%d , 目的郡%d ,实际郡ID%d" ,
			msgArmyReach->lArmyID ,msgArmyReach->lFromMapID,msgArmyReach->lToMapID,mapInfo_->getMapID()));
		return ;
	}
	IArmyPtr armyPtr = getArmyByID(msgArmyReach->lArmyID);
	if (armyPtr != NULL){
		LOG_ERROR(("出征到达错误，存在相同部队ID%d的部队!"  , msgArmyReach->lArmyID));
		return ;
	}
	ArmyTable armyTable;
	armyTable.setmasterid(msgArmyReach->lFromMapID);
	armyTable.setdetailid(msgArmyReach->lArmyID);
	if(dataBaseProxy_->db_select(armyTable) != DB_Proxy::DB_SUCCEED){
		LOG_ERROR(("查找数据错误， 部队ID%d" ,msgArmyReach->lArmyID));
		return ;
	}
	
	ArmyTable tmpArmyTable;
	POS_STRUCT pos;
	if (armyManager_->isNewbie() && (armyTable.playerid_ == 100 || armyTable.playerid_ == 200))
	{
		pos = mapInfo_->GovPos.GovPos.front();
	}
	else
	{
		getOutArmyPos(msgArmyReach->uMidMapID , pos);
	}
	memcpy(&tmpArmyTable , &armyTable , sizeof(ArmyTable));
	tmpArmyTable.setmasterid(mapInfo_->getMapID());
	tmpArmyTable.state_ = 1;
	tmpArmyTable.x = pos.PosX;
	tmpArmyTable.y = pos.PosY;
	if(dataBaseProxy_->db_update(armyTable , tmpArmyTable) != DB_Proxy::DB_SUCCEED){
		LOG_ERROR(("修正部队MAPID错误， 部队ID%d ， 原郡%d ,目的郡%d" ,msgArmyReach->lArmyID,
			msgArmyReach->lFromMapID,msgArmyReach->lToMapID));
		return ;
	} 
	
	armyPtr = armyManager_->createPlayerArmy(tmpArmyTable);
	if (armyPtr == NULL){
		LOG_ERROR(("创建部队错误， 部队ID%d ，" ,msgArmyReach->lArmyID));
		return ;
	}
	 
	if (armyPtr->isPlayerArmy())
	{
		PlayerOfficerTable officerTable;
		officerTable.setmasterid(armyTable.playerid_);
		officerTable.setdetailid(armyTable.officerid_);
		officerTable.mapid_ = mapInfo_->getMapID() - msgArmyReach->lFromMapID;
		if(dataBaseProxy_->db_increase(officerTable) != DB_Proxy::DB_SUCCEED){
			LOG_ERROR(("修正英雄部队MAPID错误， 部队ID%d ， 原郡%d ,目的郡%d" ,msgArmyReach->lArmyID,
				msgArmyReach->lFromMapID,msgArmyReach->lToMapID));
			return ;
		} 
	}

	if(!isGMArmy(armyPtr)){

		ArmyOutTable armyOutTable;
		armyOutTable.setmasterid(armyPtr->getPlayerID());
		armyOutTable.setdetailid(armyPtr->GetArmyID());
		if(dataBaseProxy_->db_delete(armyOutTable)!= DB_Proxy::DB_SUCCEED){
			if (!armyManager_->isExpeditionArmy(armyPtr))
			{
				LOG_ERROR(("删除部队出征表数据错误错误， 部队ID%d ，玩家ID%d" ,msgArmyReach->lArmyID,
					armyPtr->getPlayerID()));
			}
		}
	}
	else
	{
		armyPtr->setStrategy(AttackAI);
	}
	int limitNum = 3;
	if (armyManager_->isNewbie())
	{
		limitNum = 10;
	}
	pos = armyPtr->getValidPos(pos , limitNum , 2);
	armyPtr->setArmyPos(pos);
	
	tagArmy_Reach eventArmyReach;
	eventArmyReach.serialize(armyPtr->getPlayerID(),msgArmyReach->lFromMapID,0,
		msgArmyReach->lToMapID,msgArmyReach->lArmyID,0,CMD_ARMY_REACH_OK);
	event_notify(SERVER_BOUND_MAP_OTHER,(char*)&eventArmyReach,sizeof(tagArmy_Reach));
	if (mapInfo_->GetMapState() == peace ){
		if (mapInfo_->GetMapOwer() != armyPtr->GetCountryID()){
			//清扫安全区
			armyManager_->clearSecSection();
			//开启战斗
			startFight(true , armyPtr->GetCountryID());
		}
		
	}

	if(armyManager_->checkAddArmy(armyPtr) == true){
		broadcastAddArmy(armyPtr);
	}
	if (armyManager_->isExpeditionArmy(armyPtr) && mapInfo_->GetMapState() == peace)
	{
		kickArmyToNewbie(armyPtr);
	}
}

bool CountryFightManager::kickArmyToNewbie(IArmyPtr armyPtr){
	if (armyManager_->isNewbie()){
		return false;
	}
	unsigned int playerID;
	int newBieMapID;
	map<unsigned int ,IArmyPtr>::iterator mapIter ;
	ArmyTable armyTable ;
	if (armyPtr->GetCountryID() == static_cast<int>(UNION)){
		playerID = 100;
		newBieMapID = 1003;
	}
	else {
		playerID = 200;
		newBieMapID = 1007;
	}
	if (armyPtr->getPlayerID() != playerID)
	{
		return false;
	}
	if (armyPtr->getSoldierNum() != 100){
		//踢部队到新手村
		armyTable.setmasterid(mapInfo_->getMapID());
		armyTable.setdetailid(armyPtr->GetArmyID());
		if (dataBaseProxy_->db_select(armyTable) != DB_Proxy::DB_SUCCEED){ 
			LOG_ERROR(("获得部队%d信息失败！\r\n" , armyPtr->GetArmyID()));
		}
		armyTable.firstcount_ = 100;
		if (dataBaseProxy_->db_update(armyTable,armyTable) !=DB_Proxy::DB_SUCCEED){
			LOG_ERROR(("更新部队%d信息失败！\r\n" , armyPtr->GetArmyID()));
		}
	}
	tagArmy_Reach armyReach;
	armyReach.serialize(armyPtr->getPlayerID(), mapInfo_->getMapID() ,0,newBieMapID,armyPtr->GetArmyID() , 0 , CMD_ARMY_REACH);
	event_notify(SERVER_BOUND_MAP_OTHER,(char*)&armyReach,sizeof(armyReach));
	return true;
}
void CountryFightManager::processArmyOut(REQ_ARMY_GO_OUT_STRUCT * msgArmyOut){
	if (msgArmyOut == NULL){
		return ;
	}
	IArmyPtr armyPtr = getArmyByID(msgArmyOut->ArmyID);
	if (armyPtr == NULL){
		return ;
	}
	tagArmy_Reach eventArmyOut ;
	eventArmyOut.lPlayerID = msgArmyOut->from;
	eventArmyOut.lArmyID = msgArmyOut->ArmyID;
	eventArmyOut.lFromMapID = mapInfo_->getMapID();
	eventArmyOut.lToMapID = msgArmyOut->tagMapID;
	eventArmyOut.lPlayerID = msgArmyOut->from ;
	eventArmyOut.uMidMapID = 0;
	eventArmyOut.serialize(eventArmyOut.lPlayerID,eventArmyOut.lFromMapID,eventArmyOut.uMidMapID,
		eventArmyOut.lToMapID,eventArmyOut.lArmyID , msgArmyOut->sortID , CMD_ARMY_REACH);

	eventArmyOut.agent = msgArmyOut->agent;
	eventArmyOut.from = mapInfo_->getMapID();
	eventArmyOut.to = SERVER_BOUND_MAP_OTHER;
	event_notify(SERVER_BOUND_MAP_OTHER,(char*)&eventArmyOut,sizeof(eventArmyOut)); 
}
void CountryFightManager::processArmyOutRsp(tagBackArmy_Reach * eventArmyOutRsp){
	if (eventArmyOutRsp == NULL){
		return ;
	}
	if (eventArmyOutRsp->ret == 1){
		ArmyTable armyTable ;
		armyTable.setmasterid(eventArmyOutRsp->ulFromMapID);
		armyTable.setdetailid(eventArmyOutRsp->lArmyID);
		if (dataBaseProxy_->db_select(armyTable) == DB_Proxy::DB_SUCCEED){
			if(armyTable.state_ != 2 ){
				LOG_ERROR(("部队%d出征数据错误\r\n",eventArmyOutRsp->lArmyID));
			}
		}
		IArmyPtr armyPtr = getArmyByID(eventArmyOutRsp->lArmyID);
		if (armyPtr == NULL){
			return ;
		}
		if(armyTable.firstcount_ != armyPtr->getSoldierNum()){
			armyTable.firstcount_ = armyPtr->getSoldierNum();
			if (armyTable.firstcount_ <= 0){
				LOG_ERROR(("部队%d数量错误\r\n",eventArmyOutRsp->lArmyID));
			}
			if(dataBaseProxy_->db_update(armyTable , armyTable) != DB_Proxy::DB_SUCCEED){
				LOG_ERROR(("部队%d刷新数据库失败！" , eventArmyOutRsp->lArmyID));
			}
		}
		//广播删除部队消息
		RspDeleteArmyInfo msgDeleteArmy;
		msgDeleteArmy.ArmyID[0] = eventArmyOutRsp->lArmyID;
		msgDeleteArmy.count = 1;
		msgDeleteArmy.serialize(sizeof(RspDeleteArmyInfo) ,0, mapInfo_->getMapID(),0);
		broadcast((char *)(&msgDeleteArmy), sizeof(RspDeleteArmyInfo));
		
		//清除部队列表中的数据
		armyManager_->eraseArmy(armyPtr);
		armyPtr->refreshExp();
		//删除部队指针
		delete armyPtr;
	}
}
void CountryFightManager::processReqArmyInfo(REQ_ARMY_INFO_STRUCT* lpReq){
	if (lpReq == NULL){
		return ;
	}
	ActivityPlayerPtr playerInfo = getPlayerInfo(lpReq->ulUserID);
	if (playerInfo.get() == NULL){
		return ;
	}
	RSP_ARMY_INFO_STRUCT msgRspArmyInfo;
	memset(&msgRspArmyInfo , 0 , sizeof(RSP_ARMY_INFO_STRUCT));
	IArmyPtr armyPtr = getArmyByID(lpReq->ulArmyID);
	if (armyPtr != NULL){
		msgRspArmyInfo.uPlayerID = armyPtr->getPlayerID();
		msgRspArmyInfo.ArmyID = armyPtr->GetArmyID();
		msgRspArmyInfo.AttackPoint = armyPtr->GetAttackPower();
		msgRspArmyInfo.AttackType = armyPtr->GetAttackType();
		msgRspArmyInfo.count = armyPtr->getSoldierNum();
		msgRspArmyInfo.Physic_Defend = armyPtr->GetDefendPower(PHYSIC_ATTACK);
		msgRspArmyInfo.Magic_Defend =  armyPtr->GetDefendPower(MAGIC_ATTACK);
		msgRspArmyInfo.totalHp = armyPtr->GetCurrentHp();
		if (armyPtr->GetArmyName() != NULL){
			memcpy(msgRspArmyInfo.szName,armyPtr->GetArmyName(),SIZE_16-1);
			msgRspArmyInfo.szName[SIZE_16-1]='\0';
		}
		msgRspArmyInfo.iHeroFace = armyPtr->GetFaceID();
		msgRspArmyInfo.iWeaponID = armyPtr->GetWeaponID();
		msgRspArmyInfo.iDefendID = armyPtr->GetDefendID();
		msgRspArmyInfo.iRaceID = armyPtr->GetSoldierRace();
		msgRspArmyInfo.iTrustState = armyPtr->GetTrusteeshipState();
		if (armyPtr->GetOfficerName() != NULL){
			memcpy(msgRspArmyInfo.szHeroName,armyPtr->GetOfficerName(),SIZE_16-1);
			msgRspArmyInfo.szHeroName[SIZE_16-1]='\0';
		}
		if (armyPtr->getPlayerName().size() >= 16){
			memcpy(msgRspArmyInfo.szMainHeroName,armyPtr->getPlayerName().c_str(),SIZE_16-1);	
		}
		else{
			memcpy(msgRspArmyInfo.szMainHeroName,armyPtr->getPlayerName().c_str(),armyPtr->getPlayerName().size());	
		}
		msgRspArmyInfo.szMainHeroName[SIZE_16-1]='\0';
		msgRspArmyInfo.iSoldierOccupation = static_cast<unsigned char> (armyPtr->GetArmyOccupation());
		msgRspArmyInfo.iPower = static_cast<unsigned short> (armyPtr->GetOfficerPower());
		msgRspArmyInfo.iStamina = static_cast<unsigned short>(armyPtr->GetOfficerStamina());
		msgRspArmyInfo.iCelerity =static_cast<unsigned short>(armyPtr->GetOfficerCelerity());
		msgRspArmyInfo.iIntellect = static_cast<unsigned short>(armyPtr->GetOfficerIntellect());
		msgRspArmyInfo.iLevel = static_cast<unsigned short>(armyPtr->GetOfficerLevel());
	}
	msgRspArmyInfo.serialize(lpReq->agent,curProcessID_,playerInfo->playerID_,RSP_ARMY_INFO);
	sendMsgToAgent(lpReq->agent,(char*)&msgRspArmyInfo,msgRspArmyInfo.length);
}

void CountryFightManager::processExitSystem(REQ_EXIT_FIGHT_SYSTEM_STRUCT * msgExitSystem){
	if(msgExitSystem == NULL){
		return ;
	}
	playerManager_->delActivePlayer(msgExitSystem->uPlayerID);
}
void CountryFightManager::processCreateArmy(tagFIGHT_CreateArmy_Request * msgCreateArmy){
	if (msgCreateArmy == NULL){
		return ;
	}
	/*if (mapInfo_->GetMapState() == war){
		return ;
	}*/
	IArmyPtr armyPtr = armyManager_->createPlayerArmy(msgCreateArmy->ArmyID);
	if (armyPtr == NULL){
		return ;
	}
	POS_STRUCT pos = armyPtr->GetArmyPos()[0];
	pos = armyPtr->getValidPos(pos);
	armyPtr->setArmyPos(pos);
	armyManager_->addArmy(armyPtr);
	broadcastAddArmy(armyPtr);
}
void CountryFightManager::processEditArmy(EDIT_ARMY * msgEditArmy){
	if (msgEditArmy == NULL){
		return ;
	}
	IArmyPtr armyPtr = getArmyByID(msgEditArmy->iArmyID);
	if(armyPtr == NULL){
		return ;
	}
	if (armyPtr->resetSoldierNum(msgEditArmy->iCount + armyPtr->getSoldierNum()) == false){
		return ;
	}
	UpDateArmyInfo msgUpdateArmyInfo;
	msgUpdateArmyInfo.ArmyID = armyPtr->GetArmyID();
	msgUpdateArmyInfo.DefendID = armyPtr->GetDefendID();
	msgUpdateArmyInfo.SoldierNum = armyPtr->getSoldierNum();
	msgUpdateArmyInfo.curHp = armyPtr->GetCurrentHp();
	msgUpdateArmyInfo.totalHp = armyPtr->getInitHp();
	msgUpdateArmyInfo.serialize(msgEditArmy->agent ,mapInfo_->getMapID() , msgEditArmy->from);
	broadcast((char *)&msgUpdateArmyInfo , sizeof(UpDateArmyInfo));
}
void CountryFightManager::processEnterMap(COMEIN_MAP * msgEnterMap){
	if (msgEnterMap == NULL){
		return ;
	}
	REQ_ENTER_FIGHT_SYSTEM_STRUCT msgEnterFightSystem;
	memcpy(&msgEnterFightSystem , msgEnterMap ,sizeof(DISTRIBUTED_HEADER));
	msgEnterFightSystem.command = REQ_ENTER_FIGHT_SYSTEM;
	msgEnterFightSystem.ulMapID = msgEnterMap->lMapID;
	msgEnterFightSystem.ulPlayerID = msgEnterMap->from;
	processEnterSystem(&msgEnterFightSystem);
}
void CountryFightManager::processEnterSystem(const REQ_ENTER_FIGHT_SYSTEM_STRUCT* lpReq){
	//该函数需要优化，建筑信息发了两次
	if (lpReq == NULL){
		return ;
	}
	ActivityPlayerPtr  playerInfo = getPlayerInfo(lpReq->ulPlayerID);
	if (playerInfo.get() == NULL){
		ActivityPlayer * activePlayer = new ActivityPlayer;
		if (activePlayer == NULL){
			return ;
		}
		memset(activePlayer , 0 ,sizeof(activePlayer));
		activePlayer->agent_ = lpReq->agent;
		activePlayer->isStayRoom = true;
		activePlayer->playerID_ = lpReq->ulPlayerID;
		activePlayer->fromMapID_ = mapInfo_->getMapID();
		PlayerChatRoomTable playerChatInfo;
		playerChatInfo.setid(lpReq->ulPlayerID);
		if(dataBaseProxy_->db_select(playerChatInfo) == DB_Proxy::DB_SUCCEED){
			activePlayer->countryID_ = playerChatInfo.country_ ;
		}
		else
		{
			LOG_ERROR(("无法查找到玩家%d阵营信息" , lpReq->ulPlayerID));
		}
		PlayerOfficerTable playerOfficer;
		playerOfficer.setmasterid(lpReq->ulPlayerID);
		playerOfficer.setdetailid(lpReq->ulPlayerID);
		if(dataBaseProxy_->db_select(playerOfficer) == DB_Proxy::DB_SUCCEED){
			activePlayer->faceID_ = static_cast<unsigned int>(playerOfficer.faceid_) ;
		}
		else
		{
			LOG_ERROR(("无法查找到玩家%d主英雄信息" , lpReq->ulPlayerID));
		}
		playerInfo = playerManager_->addActivePlayer(ActivityPlayerPtr(activePlayer));
	}
	if (playerInfo.get() == NULL){
		return ;
	}
	armyManager_->freeArmyFromQueue(lpReq->ulPlayerID);
	playerInfo->agent_ = lpReq->agent;
	playerInfo->ectypeID_ = mapInfo_->getMapID();
	playerInfo->isStayRoom = true;
	RSP_ENTER_FIGHT_SYSTEM_STRUCT* msgEnterNpcSystem = new  RSP_ENTER_FIGHT_SYSTEM_STRUCT;
	if (msgEnterNpcSystem == NULL){
		return ;
	}
	memset(msgEnterNpcSystem , 0 , sizeof(RSP_ENTER_FIGHT_SYSTEM_STRUCT));
	const map<unsigned int ,IArmyPtr>& armyMap = armyManager_->getArmyMap();
	map<unsigned int ,IArmyPtr>::const_iterator iter;
	int armyNum = 0;
	IArmyPtr armyPtr ;
	bool isFirst = true;
	for (iter = armyMap.begin();iter != armyMap.end();iter ++)
	{
		armyPtr = iter->second;
		if (isValidArmy(armyPtr) == false && armyPtr->GetArmyType() != BUILD){
			continue;
		}
		if(armyPtr->getBuildDetailType() == ARCH_BUILD_TOFT){
			continue;
		}
		armyPtr->exportArmyInfo(msgEnterNpcSystem->army[armyNum],mapInfo_->getMapID());
		armyNum ++;
		if (armyNum == MAX_MAP_ARMY_NUM){
			msgEnterNpcSystem->count = armyNum ;
			if (isFirst){
				msgEnterNpcSystem->flag = 1;
				isFirst = false;
			}
			else{
				msgEnterNpcSystem->flag = 0;
			}
			msgEnterNpcSystem->MapID = curProcessID_;
			msgEnterNpcSystem->serialize(sizeof(RSP_ENTER_FIGHT_SYSTEM_STRUCT),playerInfo->agent_,curProcessID_,lpReq->ulPlayerID,RSP_ENTER_FIGHT_SYSTEM);
			sendMsgToAgent(playerInfo->agent_,(char*)msgEnterNpcSystem,sizeof(RSP_ENTER_FIGHT_SYSTEM_STRUCT));
			memset(msgEnterNpcSystem,0,sizeof(RSP_ENTER_FIGHT_SYSTEM_STRUCT));
			armyNum = 0;
		}		
	}
	
	if (armyNum > 0){
		msgEnterNpcSystem->count = armyNum ;
		if (isFirst){
			msgEnterNpcSystem->flag = 1;
			isFirst = false;
		}
		else{
			msgEnterNpcSystem->flag = 0;
		}
		msgEnterNpcSystem->MapID = curProcessID_;
		msgEnterNpcSystem->serialize(sizeof(RSP_ENTER_FIGHT_SYSTEM_STRUCT)-(sizeof(ARMY_POS)*(MAX_MAP_ARMY_NUM -armyNum)),
			playerInfo->agent_,curProcessID_,lpReq->ulPlayerID,RSP_ENTER_FIGHT_SYSTEM);
		sendMsgToAgent(playerInfo->agent_,(char*)msgEnterNpcSystem,msgEnterNpcSystem->length);
	}
	delete msgEnterNpcSystem;
	BulidInfoList buildData;
	memset(&buildData,0,sizeof(BulidInfoList));
	buildData.mapid = mapInfo_->getMapID();
	int buildNum = 0;
	PublicBuildTable* pBuild = NULL;
	map<unsigned long , PublicBuildTable* >::iterator buildIter;
	for (buildIter = dataBaseMemData_->m_PublicBuildList.begin();buildIter != dataBaseMemData_->m_PublicBuildList.end();buildIter ++)
	{
		pBuild = buildIter->second;
		buildData.BuildList[buildNum].ulBuildID = pBuild->pbuildingid_;
		if (pBuild->pbuildingkind_ == ARCH_BUILD_TOFT){
			continue ;
		}
		buildData.BuildList[buildNum].iBuildType = pBuild->pbuildingkind_;
		buildData.BuildList[buildNum].iBuildLevel = pBuild->currentlevel_;
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
			playerInfo->agent_,mapInfo_->getMapID(),playerInfo->playerID_,RSP_SEND_PUBLIC_BULID_INFO);
		asynch_write(playerInfo->agent_,(char*)&buildData,buildData.length);
	}
	if (mapInfo_->GetMapState() != war){
		armyManager_->freeArmyFromQueue(lpReq->ulPlayerID);
	}
	//导出当前所有效果
	exportEffect(playerInfo->playerID_, playerInfo->agent_);
	//导出卡片锁定时间
	propManager_->exportCardRecord(playerInfo->playerID_ , playerInfo->agent_);
}

void CountryFightManager::processArmyRetreat(ReqEctypeRetreatArmyInfo* msgArmyRetreat){
	if (msgArmyRetreat == NULL){
		return;
	}
	IArmyPtr armyPtr = getArmyByID(msgArmyRetreat->ArmyID);
	if(isValidArmy(armyPtr) == false){
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
		LOG_ERROR(("更新部队%d,ArmyTable错误!" , armyPtr->GetArmyID()));
	}
	armyPtr->refreshExp();
	msgArmyBack.iArmyCount= 1;
	msgArmyBack.uEctypeID = msgArmyRetreat->EctypeID;
	msgArmyBack.entranceID = msgArmyRetreat->entranceID;
	fightEvent_->NotifyEctypeArmyBack(msgArmyBack ,armyPtr->getFromMapID() , curProcessID_ );

	RspDeleteArmyInfo msgDeleteArmy;
	msgDeleteArmy.ArmyID[0] = armyPtr->GetArmyID();
	msgDeleteArmy.count = 1;
	msgBroadcast(&msgDeleteArmy ,RSP_DELETE_ARMY_INFO , sizeof(RspDeleteArmyInfo));
	fightEvent_->NotifyModifyArmyState(armyPtr->getPlayerID() , armyPtr->GetArmyID() , 
		armyPtr->getFromMapID() , 0, ARMY_ANNIHILATE , armyPtr->GetWeaponID());

	eraseArmy(armyPtr);
	delete armyPtr;
}
void CountryFightManager::processLeaveSystem(REQ_LEAVE_FIGHT_SYSTEM_STRUCT * msgLeaveSystem){
	if (msgLeaveSystem == NULL){
		return ;
	}
	playerManager_->delActivePlayer(msgLeaveSystem->playerID);
}
//部队离开郡
void CountryFightManager::processArmyLeave(EventArmyLeave * eventArmyLeave){

	if (eventArmyLeave == NULL){
		return ;
	}
	IArmyPtr armyPtr = NULL;
	for (int i = 0 ; i < eventArmyLeave->armyNum ; i++)
	{
		armyPtr = getArmyByID((*eventArmyLeave)[i]);
		if (armyPtr == NULL){
			continue;
		}
		if (!armyPtr->isPlayerArmy()){
			continue;
		}
		ArmyTable* armyTabe = NULL;
		armyPtr->getArmyTable(armyTabe);
		if (armyTabe == NULL){
			continue ;
		}
		fightOverManager_->onArmyDie(NULL , armyPtr);
		fightRecord_->onArmyDie(armyPtr);
		armyPtr->refreshExp();
		RspDeleteArmyInfo msgDeleteArmy;
		msgDeleteArmy.ArmyID[0] = armyPtr->GetArmyID();
		msgDeleteArmy.count = 1;
		msgBroadcast(&msgDeleteArmy ,RSP_DELETE_ARMY_INFO , sizeof(RspDeleteArmyInfo));
		fightEvent_->NotifyModifyArmyState(armyPtr->getPlayerID() , armyPtr->GetArmyID() , 
			mapInfo_->getMapID() , 0, eventArmyLeave->armyState , armyPtr->GetWeaponID());
		eraseArmy(armyPtr);
		delete armyPtr;
	}	
}
bool CountryFightManager::processFightOver(int victoryCountryID){

	BattlefieldStatisticsInfo msgFightEndInfo;
	memset(&msgFightEndInfo , 0 , sizeof(BattlefieldStatisticsInfo));
	fightReport_->onOver(victoryCountryID,msgFightEndInfo); 
	if (!isGMPlayer(msgFightEndInfo.excellentplayerId) && msgFightEndInfo.excellentplayerId){
		PlayerOfficerTable playerOfficer;
		playerOfficer.setmasterid(msgFightEndInfo.excellentplayerId);
		playerOfficer.setdetailid(msgFightEndInfo.excellentplayerId);
		if(dataBaseProxy_->db_select(playerOfficer) == DB_Proxy::DB_SUCCEED){
			memcpy(msgFightEndInfo.excellentplayerName , playerOfficer.officername_ , SIZE_24);
			msgFightEndInfo.excellentplayerRank = playerOfficer.militaryrank_;
		}
	}
	if (msgFightEndInfo.mapId > 1000 && msgFightEndInfo.mapId <= 1060)
	{
		//战斗结束新闻消息通知
		Notify_Add_News_Report fightReportEvent;
		memset(&fightReportEvent,0,sizeof(Notify_Add_News_Report));
		memcpy(fightReportEvent.szConent,&msgFightEndInfo , sizeof(BattlefieldStatisticsInfo));
		fightReportEvent.type = EVENT_FIGHT_END;
		fightReportEvent.serialize(0);
		event_notify(MDM_GP_MAP_CHAT,(char*)&fightReportEvent,sizeof(Notify_Add_News_Report));
	}
	

	//处理战斗缴获
	processCapture(victoryCountryID);
	statisticTotalFightInfo fightReport;
	memset(&fightReport , 0 , sizeof(statisticTotalFightInfo));
	fightReport.winer = victoryCountryID;
	fightRecord_->onOver(victoryCountryID);
	fightReport.warType = warType_;
	const map<unsigned int , ActivityPlayerPtr> &activityPlayers = playerManager_->getPlayerMap();
	map<unsigned int , ActivityPlayerPtr>::const_iterator iter;
	ActivityPlayerPtr activePlayer;
	for (iter = activityPlayers.begin();iter != activityPlayers.end();iter ++){
		activePlayer = iter->second;
		if (activePlayer && activePlayer->isStayRoom){	
			fightRecord_->queryFightingReport(activePlayer->countryID_ , activePlayer->playerID_ ,fightReport);
			int dataLen = sizeof(statisticTotalFightInfo) - (sizeof(StatisticArmyFightInfo) *(100 - fightReport.num));
			fightReport.serialize(dataLen,activePlayer->agent_,curProcessID_,activePlayer->playerID_,RSP_FIGHT_VOER_STATIC_INFO);
			sendMsgToAgent(activePlayer->agent_,(char*)&fightReport,dataLen);
		}
	}
	armyManager_->attackOrguardSwopOnWin(victoryCountryID);

	//战斗结束消息
	EndFight eventFightOver;
	eventFightOver.cmd = CMD_END_FIGHT; 
	eventFightOver.uMapID = mapInfo_->getMapID();
	eventFightOver.winer = victoryCountryID ; // 1 = 部落  2 =  联盟
	eventFightOver.serialize(sizeof(EndFight),4000,curProcessID_,SERVER_BOUND_MAP_OTHER,CMD_END_FIGHT);
	event_notify(SERVER_BOUND_MAP_OTHER,(char*)&eventFightOver,sizeof(EndFight));
	
	//重设地图归属，恢复和平
	mapInfo_->SetMapOwer(victoryCountryID);
	setMapState(peace);

	setLabelMap(-1);
	armyManager_->cleanupHurt();
	armyManager_->saveArmyDataToDBG();
	propManager_->clearCardRecord();
	processDelGuardArmy();
	return true;
}
void CountryFightManager::processTrusteeship(REQ_TRUST_INFO_STRUCT *reqMsg){
	if (reqMsg == NULL){
		return ;
	}
	IArmyPtr armyPtr = getArmyByID(reqMsg->ulArmyID);
	if (armyPtr == NULL){
		return ;
	}
	armyPtr->setTrusteeshipState(true);
}
void CountryFightManager::release(){
	armyManager_->saveArmyDataToDBG();
	map<unsigned int ,IArmyPtr>::iterator iter; 
	if(dataBaseProxy_){
		dataBaseProxy_->fini(); 
		CHECK_DELETE(dataBaseProxy_);
	}
	CHECK_DELETE(curProcessQueue_);
	CHECK_DELETE(processQueue_); 
	CHECK_DELETE(fightRecord_);
	CHECK_DELETE(fightOverManager_);
	CHECK_DELETE(seekPather_);
	CHECK_DELETE(armyManager_);
	CHECK_DELETE(broadcastPackManager_);
	CHECK_DELETE(fightReport_);
	CHECK_DELETE(dropGoodsManager_);
	CHECK_DELETE(propManager_);
	CHECK_DELETE(taskManager_);
}
int CountryFightManager::notifyModifyArmyState(unsigned int playerid,unsigned int armyid, int mapid,int armyNum,int state,int weapon){
	return fightEvent_->NotifyModifyArmyState(playerid , armyid ,mapid ,armyNum ,state ,weapon);
}

void CountryFightManager::onCatchedHero(IArmy* army, HeroId heroId){
	fightRecord_->onCatchedHero(army ,heroId );
}

void CountryFightManager::onBeCatchedHero(IArmy* army, HeroId heroId){
	fightRecord_->onBeCatchedHero(army ,heroId );
	fightReport_->onHeroBeCatched(army);
}

void CountryFightManager::onHeroWasInjured(IArmy* army, HeroId heroId){
	fightRecord_->onHeroWasInjured(army , heroId);
	fightReport_->onHeroBeHurt(army);
}
void CountryFightManager::onEnterWar(IArmyPtr armyPtr){
	fightRecord_->onEnterWar(armyPtr);
}	

void CountryFightManager::onSoliderDie(IArmy * attackArmy , IArmy* army, unsigned int deathNum){
	fightRecord_->onSoliderDie(army , deathNum);
	fightReport_->onSoldierDie(army,deathNum);
	fightRecord_->onCaptured(attackArmy, army, deathNum);
}


void CountryFightManager::onKilledEnemy(IArmy* army, unsigned int killNum){
	fightRecord_->onKilledEnemy(army , killNum);
	fightReport_->onKilledEnemy(army , killNum);
}

void CountryFightManager::onArmyDie(IArmy* attackPtr ,IArmy* army){
	if (attackPtr == NULL || army == NULL){
		return ;
	}
	fightRecord_->onArmyDie(army);
	if (army->GetArmyType() == BUILD && attackPtr != army){
		fightReport_->onBuildingDestroyed(army->GetCountryID() , army->getBuildDetailType());
	}
	fightOverManager_->onArmyDie(attackPtr , army);
	taskManager_->onArmyDie(attackPtr , army);
	armyManager_->onArmyDie(attackPtr , army);
	
}
void CountryFightManager::enterCountry(ArmyPtrVect & armyPtrVect){
	ArmyPtrVect::iterator iter;
	IArmyPtr armyPtr;
	for (iter = armyPtrVect.begin();iter != armyPtrVect.end();iter++){
		armyPtr = (*iter);
		POS_STRUCT pos = armyPtr->GetArmyPos()[0];
		armyPtr->getValidPos(pos);
		armyPtr->setArmyPos(pos);
		if (addArmy(*iter) == false){
			continue;
		}
		broadcastAddArmy(*iter);
	}
}

void CountryFightManager::processDelPlayerArms(tagNotify_DelNPCArmy_Request * msgPlayerInfo){
	if (msgPlayerInfo == NULL){
		return ;
	}
	std::vector<IArmyPtr>::iterator iter;
	ArmyTable * armyTable = NULL;
	IArmyPtr armyPtr;
	std::vector<IArmyPtr> vecArmys;
	int delArmyNum = 0;
	//最好使用宏
	armyManager_->getPlayerArmys(6 , vecArmys , msgPlayerInfo->CountryID ,true);
	for (iter = vecArmys.begin();iter != vecArmys.end(); iter ++){

		armyPtr = *iter;
		if (armyPtr->GetCountryID() != msgPlayerInfo->CountryID){
			continue;
		}
		armyPtr->getArmyTable(armyTable);
		if (armyTable == NULL){
			continue ;
		}
		if(dataBaseProxy_->db_delete(*armyTable) != DB_Proxy::DB_SUCCEED){
			LOG_ERROR(("删除部队%d信息失败processDelPlayerArms" , armyPtr->GetArmyID()));
		}
		processDelArmy(armyPtr , ARMY_ANNIHILATE);
		delArmyNum ++;
	}
	tagNotify_DelNPCArmy_Response tmptagNotify_DelNPCArmy_Response;
	tmptagNotify_DelNPCArmy_Response.serialize(msgPlayerInfo->agent ,msgPlayerInfo->PlayerID , 
		delArmyNum , msgPlayerInfo->GMCmd , mapInfo_->getMapID() , msgPlayerInfo->CountryID);
	event_notify(msgPlayerInfo->from , (char *)&tmptagNotify_DelNPCArmy_Response , tmptagNotify_DelNPCArmy_Response.length);

}
GOODS_TYPE getGoodsType(int goodsId){
	if (goodsId < 150)
	{
		return WEAPON_ITEM;
	}
	if (goodsId >= 150 && goodsId < 200)
	{
		return SPECIAL_TOOL_ITEM;
	}
	return DEFEND_ITEM;
}

void CountryFightManager::captureWeapons(std::map<unsigned int , std::map<unsigned int, int>>& playerWeapons,bool isRefresh){

	GetBootyInfo bootyInfo;
	PlayerResource playerResource;
	ProductionAmount productionAmount;
	PlayerWeapons::iterator weaponIter;
	WeaponMap::iterator weaponMapIter;
	GOODS_TYPE goodType;
	bool isInsertTable;
	for(weaponIter = playerWeapons.begin(); weaponIter != playerWeapons.end() ; weaponIter++){
		int index = 0;
		WeaponMap & weaponMap = weaponIter->second;
		PlayerCity playerCity;
		playerCity.setmasterid(weaponIter->first);
		int mapId = getNewbieID(getCountryID(weaponIter->first));
		if (mapId <= 0){
			continue;
		}
		playerCity.setdetailid(mapId);
		if(dataBaseProxy_->db_select(playerCity) != DB_Proxy::DB_SUCCEED){
			LOG_ERROR(("获得玩家%d新手村" , weaponIter->first));
			continue;
		}
		
		for (weaponMapIter = weaponMap.begin(); weaponMapIter != weaponMap.end() ; weaponMapIter++){
			isInsertTable = false;
			int srcNum = 0;
			playerResource.setmasterid(playerCity.cityid_ ,playerCity.mapid_);
			playerResource.setdetailid(weaponIter->first);
			if(dataBaseProxy_->db_select(playerResource) == DB_Proxy::DB_SUCCEED && playerResource.content_ > weaponMapIter->second){
				isInsertTable = true;
			}
			else
			{
				continue;
			}
		
			if (index < 100 && isInsertTable){
				goodType = getGoodsType(weaponMapIter->first);
				productionAmount.setmasterid(playerResource.mapid_ , playerResource.cityid_
					, playerResource.playerid_ , goodType);
				productionAmount.setdetailid(weaponMapIter->first);
				if (dataBaseProxy_->db_select(productionAmount) == DB_Proxy::DB_SUCCEED){
					srcNum = productionAmount.productioncount_;
					productionAmount.productioncount_+= weaponMapIter->second ;
					if (dataBaseProxy_->db_update(productionAmount,productionAmount) == DB_Proxy::DB_FAILED)
					{
						LOG_ERROR(("玩家%d刷新物品%d数量%d失败！" , productionAmount.playerid_ , 
							productionAmount.productionid_ , productionAmount.productioncount_));
						continue;
					}
				}
				else{
					productionAmount.productioncount_ = weaponMapIter->second ;
					
					if(dataBaseProxy_->db_insert(productionAmount) == DB_Proxy::DB_FAILED){
						LOG_ERROR(("玩家%d插入物品%d数量%d失败！" , productionAmount.playerid_ , 
							productionAmount.productionid_ , productionAmount.productioncount_));
						continue;
					}
				}

				ISystemLog::getInstance()->playerProduce_NPC_Log(playerResource.playerid_,getPlayerName(playerResource.playerid_,playerResource.playerid_).c_str() ,
				mapInfo_->getMapID() ,playerResource.cityid_,goodType , weaponMapIter->first ,srcNum ,productionAmount.productioncount_ , weaponMapIter->second);
				
				bootyInfo.Booty[index].itemID = weaponMapIter->first;
				bootyInfo.Booty[index].itemNum = weaponMapIter->second;
				bootyInfo.Booty[index].itemType = goodType; //武器
				index++;
			}
		}
		bootyInfo.MapID = mapInfo_->getMapID();
		bootyInfo.count = index;
		bootyInfo.isNeedRefresh = isRefresh;
		ActivityPlayerPtr activePlayer = getPlayerInfo(weaponIter->first);
		if (activePlayer.get() != NULL && index > 0){
			bootyInfo.serialize(sizeof(GetBootyInfo) - sizeof(BootyInfo)*(100 - index),
				activePlayer->agent_,mapInfo_->getMapID(),activePlayer->playerID_);
			asynch_write(activePlayer->agent_,(char*)&bootyInfo,bootyInfo.length);
		}
	}
}
void CountryFightManager::processCapture(int winCountryID){
	PlayerWeapons playerWeapons;
	fightRecord_->getWeapons(playerWeapons);
	captureWeapons(playerWeapons , true); //武器
	PlayerArmors playerArmors;
	fightRecord_->getArmors(playerArmors);
	captureWeapons(playerArmors , false); //防具
	if (mapInfo_->GetMapOwer() != winCountryID && mapInfo_->GetMapOwer() != NPC_){
		captureResource(winCountryID);
	}
}
void CountryFightManager::captureResource(int attackCountryID){
	int sumfoodcount_ = 0;
	int sumwoodcount_ = 0 ;
	int sumironcount_ =0;
	int sumskincount_ =0;
	int sumredstonecount_ =0;
	int sumyollowstonecount_ =0;
	int sumgreenstonecount_ =0;
	int sumbackironcount_ =0;
	
	PlayerResource playerResource;
	PlayerResource* playerResourcePtr = NULL;
	list<CityInfo>::iterator iter;
	for (iter = mapInfo_->m_CityInfo.begin() ; iter != mapInfo_->m_CityInfo.end() ; iter ++){
		int num = 0;
		playerResource.setmasterid(iter->uCityID ,mapInfo_->m_MapID);
		dataBaseProxy_->db_multi_select(playerResource,(char*&)playerResourcePtr,num);
		num /= sizeof(PlayerResource);
		for(int i = 0 ; i < num ; i++){
			sumfoodcount_ += playerResourcePtr[i].foodcount_;
			sumwoodcount_ += playerResourcePtr[i].woodcount_;
			sumironcount_ += playerResourcePtr[i].ironcount_;
			sumredstonecount_ += playerResourcePtr[i].redstonecount_;
			sumyollowstonecount_ += playerResourcePtr[i].yollowstonecount_;
			sumgreenstonecount_ += playerResourcePtr[i].greenstonecount_;
			sumbackironcount_ += playerResourcePtr[i].backironcount_;
			sumskincount_ += playerResourcePtr[i].skincount_;
		}
		if(playerResourcePtr){
			delete [] playerResourcePtr;
			playerResourcePtr = NULL;
		}
	}
	int killNum = 0;
	PlayerWeapons playerWeapons;
	PlayerResource playerIncResource;
	fightRecord_->getCapturedWeapon(attackCountryID , playerWeapons);
	PlayerWeapons::iterator weaponIter;
	WeaponMap::iterator weaponMapIter;
	for(weaponIter = playerWeapons.begin(); weaponIter != playerWeapons.end() ; weaponIter++){
		WeaponMap & weaponMap = weaponIter->second;
		for (weaponMapIter = weaponMap.begin(); weaponMapIter != weaponMap.end() ; weaponMapIter++){
			killNum += weaponMapIter->second;
		}
	}
	if (killNum == 0){
		return ;
	}
	for(weaponIter = playerWeapons.begin(); weaponIter != playerWeapons.end() ; weaponIter++){
		WeaponMap & weaponMap = weaponIter->second;
		float playerKillNum = 0;
		for (weaponMapIter = weaponMap.begin(); weaponMapIter != weaponMap.end() ; weaponMapIter++){
			playerKillNum += weaponMapIter->second;
		}
		int cityNum = 0;
		PlayerCity playerCity;
		PlayerCity *playerCityPtr = NULL;
		playerCity.setmasterid(weaponIter->first);
		dataBaseProxy_->db_multi_select(playerCity ,(char*&)playerCityPtr,cityNum);
		cityNum /= sizeof(PlayerCity); 

		float rate = playerKillNum / killNum;
		memset(&playerIncResource , 0 , sizeof(PlayerResource));
		int cnt = playerIncResource.foodcount_ = static_cast<int>(sumfoodcount_ * rate);
		cnt += playerIncResource.woodcount_ = static_cast<int>(sumwoodcount_ * rate);
		cnt += playerIncResource.ironcount_ = static_cast<int>(sumironcount_ * rate);
		cnt += playerIncResource.redstonecount_ = static_cast<int>(sumredstonecount_ * rate);
		cnt += playerIncResource.yollowstonecount_ = static_cast<int>(sumyollowstonecount_ * rate);
		cnt += playerIncResource.greenstonecount_ = static_cast<int>(sumgreenstonecount_ * rate);
		cnt += playerIncResource.backironcount_ = static_cast<int>(sumbackironcount_ * rate);
		cnt += playerIncResource.skincount_ = static_cast<int>(sumskincount_ * rate);
		playerIncResource.content_ = - cnt;
		for(int i = 0 ; i < cityNum ; i++){
			playerResource.setmasterid(playerCityPtr[i].cityid_ , playerCityPtr[i].mapid_);
			playerResource.setdetailid(weaponIter->first);
			if(dataBaseProxy_->db_select(playerResource) == DB_Proxy::DB_SUCCEED){
				if(playerResource.content_ > cnt){
					playerIncResource.setmasterid(playerCityPtr[i].cityid_ , playerCityPtr[i].mapid_);
					playerIncResource.setdetailid(weaponIter->first);
					if(dataBaseProxy_->db_increase(playerIncResource) != DB_Proxy::DB_SUCCEED){
						LOG_ERROR(("分配资源插入数据库错误！"));
					}
					break;
				}
			}
		}
		if(playerCityPtr){
			delete [] playerCityPtr;
			playerCityPtr = NULL;
		}
	}
}

void CountryFightManager::processDelArmy(IArmyPtr armyPtr , ARMY_TRUST_STATE state){
	FightManager::processDelArmy(armyPtr , state);
	broadcastPackManager_->msgBroadcastData();
}

void CountryFightManager::setLabelMap(int attackCountry){
	labelMap_[0].label = 0;
	labelMap_[0].countryId = mapInfo_->GetMapOwer();
	labelMap_[0].doorId = -1;
	labelMap_[1].label = 1;
	labelMap_[1].countryId = attackCountry;
	labelMap_[1].doorId = -1;
	labelMap_[2].label = 2;
	labelMap_[2].countryId = NPC_;
	labelMap_[2].doorId = -1;
}
void CountryFightManager::sendDiscardGoodsMail(unsigned int playerID , int countryId , string& playerName){
	Notify_Send_Mail discardGoodsMail;
	memset(&discardGoodsMail , 0 , sizeof(Notify_Send_Mail));
	/*
	if (countryId == CLAN)
	{
		strcpy(discardGoodsMail.szbody , "由于寒风之地仓库容量不足，导致缴获的武器多余部分无法存放，对阁下造成损失的深感遗憾，请整理或扩充仓库容量。");
	}
	else
	{
		strcpy(discardGoodsMail.szbody , "由于寂静高地仓库容量不足，导致缴获的武器多余部分无法存放，对阁下造成损失的深感遗憾，请整理或扩充仓库容量。");
	}
	discardGoodsMail.type = ;
	discardGoodsMail.szSendPlayerName
	strcpy()
	discardGoodsMail.sztitle
	*/
}


void CountryFightManager::processGuardArmy(void){
	if (war != mapInfo_->GetMapState()){
		return ;
	}
	CountryArmyManager * armyManagerPtr = dynamic_cast<CountryArmyManager *>(armyManager_);
	if(2400 == curBoutNum_ ){
		return armyManagerPtr->createGuardArmy(0 , mapInfo_->GetMapOwer());
	}
	if (3000 == curBoutNum_ && armyManager_->getAttackArmyNum()){
		return armyManagerPtr->createGuardArmy(mapInfo_->getMapID() , mapInfo_->GetMapOwer());
	}
}

void CountryFightManager::processDelGuardArmy(void){
	
	std::vector<IArmyPtr>::iterator iter;
	IArmyPtr armyPtr;
	std::vector<IArmyPtr> vecArmys;
	//最好使用宏
	armyManager_->getPlayerArmys(50 , vecArmys , mapInfo_->GetMapOwer() ,true);
	for (iter = vecArmys.begin();iter != vecArmys.end(); iter ++){

		armyPtr = *iter;
		if (armyPtr->GetCountryID() !=  mapInfo_->GetMapOwer()){
			continue;
		}
		processDelArmy(armyPtr , ARMY_ANNIHILATE);
	}
}