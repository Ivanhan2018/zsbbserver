#include "NPCinfo.h"

#include "DBTableHeader.h"


CNPCInfo g_NpcInfo;

CNPCInfo::CNPCInfo()
{
}

CNPCInfo::~CNPCInfo(void)
{
}

void CNPCInfo::InitNpc(DB_Proxy * dataBaseProxy)
{
	char * p;
	int len;
	NPC NpcInfo;
	NPC* pNPCInfo;
	dataBaseProxy_ = dataBaseProxy;
	//int ret ;
	int ret = dataBaseProxy_->db_select_all(NpcInfo,p,len);
	int k =0;
	if(len>0)
	{

		while(k<len && (len-k)>=sizeof(NPC))
		{
			pNPCInfo = new NPC ;
			memcpy(pNPCInfo,p+k,sizeof(NPC));
			m_NPCList[pNPCInfo->npcid_]= pNPCInfo;
			k += sizeof(NPC);
		}
		delete [] p;
		p = NULL;
	}
	EctypeBaseTable npcmapinfo;
	EctypeBaseTable* pNpcMapInfo;
	ret = dataBaseProxy_->db_select_all(npcmapinfo,p ,len);
	k = 0 ;
	if (len>0)
	{
		while (k<len&&(len-k)>=sizeof(EctypeBaseTable))
		{
			pNpcMapInfo = new EctypeBaseTable;
			memcpy(pNpcMapInfo,p+k,sizeof(EctypeBaseTable));
			m_npcMapinfo[pNpcMapInfo->ectypeid_] = pNpcMapInfo;
			k += sizeof(EctypeBaseTable);
		}
		delete [] p ;
		p = NULL;
	}
	loadEctypeNpcType();
	loadEctypeNpc();
	loadEctypeInfo();
	loadPatrolInfo();
	loadFightOverCondition();
	loadAiLevel();
	loadDerateHurt();
	loadNpcTemplate();
	loadEctypeBuildParam();
	loadEctypeBuild();
	loadLandFormMap();
	loadLimitTable();
	loadWarEntranceInfo();
	loadPropCardInfo();
	loadGuarderParam();
}

void CNPCInfo::empty()
{

	map<int ,EctypeBaseTable* >::iterator  iter2;
	EctypeBaseTable* npcmapinfo = NULL;
	for (iter2 = m_npcMapinfo.begin();iter2 != m_npcMapinfo.end() ;iter2 ++)
	{
		npcmapinfo = iter2->second;
		delete npcmapinfo ;
		npcmapinfo = NULL;
	}
	m_npcMapinfo.clear();

	map<unsigned ,NPC*> ::iterator iter3;
	NPC* npc = NULL;
	for (iter3 = m_NPCList.begin() ; iter3 != m_NPCList.end() ; iter3 ++)
	{
		npc = iter3->second;
		delete npc;
		npc = NULL;
	}
	m_NPCList.clear();
	clearEctypeNpcType();
	clearEctypeNpc();
	clearEctypeInfo();
	clearPatrolInfo();
	clearFightOverCondition();
	clearAiLevel();
	clearNpcTemplate();
	clearNpcBuildParam();
	clearEctypeBuild();
	clearDerateHurt();
	clearLandFormMap();
	clearLimitTable();
	clearWarEntranceInfo();
	clearPropCardInfo();
	clearGuarderParam();
}


EctypeBaseTable* CNPCInfo::GetEctypInfo(int ectypeid)
{
	EctypeBaseTable* NpcMapInfo = NULL;
	map<int ,EctypeBaseTable* >::iterator iter;

	iter = m_npcMapinfo.find(ectypeid);
	if (iter != m_npcMapinfo.end())
	{
		NpcMapInfo = iter->second;
	}
	return NpcMapInfo;
	
}
NPC* CNPCInfo::GetNpcInfo(int npcid)
{
	map<unsigned ,NPC*>::iterator iter;
	NPC* npc = NULL;
	iter = m_NPCList.find(npcid);
	if (iter!=m_NPCList.end())
	{
		npc = iter->second;

	}
	return npc;
}


void CNPCInfo::loadEctypeNpcType(){
	FILE *fp = 0;
	fp = fopen("Table/NpcParam.csv","r");
	if(fp == NULL){
		printf("打开副本NPC类型文件失败！\n");
		return;
	}
	char fileBuf[1024]= {0};
	fgets(fileBuf,1024,fp);

	EctypeNpcTypeTable * ectypeNpcType = 0;
	while(!feof(fp))
	{
		if(fgets(fileBuf,1024,fp) == NULL)
			break;
		ectypeNpcType = new EctypeNpcTypeTable;
		if(sscanf(fileBuf,"%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&ectypeNpcType->npcTypeID,&ectypeNpcType->npcName,&ectypeNpcType->level,&ectypeNpcType->raceID,
			&ectypeNpcType->soldierType,&ectypeNpcType->attackPower,&ectypeNpcType->attackType,&ectypeNpcType->weaponType,
			&ectypeNpcType->weaponID,&ectypeNpcType->defendID,&ectypeNpcType->attackScope,&ectypeNpcType->magicDependPower,
			&ectypeNpcType->phsicDependPower , &ectypeNpcType->soldierHp , &ectypeNpcType->movePerNeedTime ,&ectypeNpcType->viewScope,
		    &ectypeNpcType->dropExp ,&ectypeNpcType->skill, &ectypeNpcType->skillLevel,&ectypeNpcType->npcType)<20)
		{
			delete ectypeNpcType;
			ectypeNpcType = 0;
			continue;
		}
		ectypeNpcTypes_.insert(map<unsigned int , EctypeNpcTypeTable*>::value_type(ectypeNpcType->npcTypeID,ectypeNpcType));
	}
	fclose(fp);
}
void CNPCInfo::loadEctypeNpc(){
	for(int i = 1001 ; i <= 2200 ; i ++){
		map<unsigned int , EctypeNpcArmyTable*> ectypeNpcMap;
		if(loadEctypeNpc(i , ectypeNpcMap) == true){
			ectypeNpcArmys_.insert(map<int , map<unsigned int , EctypeNpcArmyTable*> >::value_type(i , ectypeNpcMap));
		}
	}
}
bool CNPCInfo::loadEctypeNpc(int ectypeID , map<unsigned int , EctypeNpcArmyTable*> & ectypeNpcMap){
	char filePath[50] = {0};
	FILE *fp = 0;
	sprintf(filePath,"table/%dPosition.csv",ectypeID);
	fp = fopen(filePath,"r");
	if(fp == NULL){
		return false;
	}
	
	char fileBuf[1024]= {0};
	fgets(fileBuf,1024,fp);
	EctypeNpcArmyTable * ectypeNpcArmyTable = 0;
	while(!feof(fp))
	{
		if(fgets(fileBuf,1024,fp) == NULL)
			break;
		ectypeNpcArmyTable = new EctypeNpcArmyTable;
		if(sscanf(fileBuf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&ectypeNpcArmyTable->ectypeID,&ectypeNpcArmyTable->armyID,&ectypeNpcArmyTable->npcTypeID,&ectypeNpcArmyTable->isNeutral,
			&ectypeNpcArmyTable->isSurely , &ectypeNpcArmyTable->strategyType ,&ectypeNpcArmyTable->aiLevel , &ectypeNpcArmyTable->soldierNum,
			&ectypeNpcArmyTable->posX,&ectypeNpcArmyTable->posY , &ectypeNpcArmyTable->dropItem ,&ectypeNpcArmyTable->dropOdds,&ectypeNpcArmyTable->enterTime , 
			&ectypeNpcArmyTable->quitTime,&ectypeNpcArmyTable->isInfinityReproduce ,&ectypeNpcArmyTable->adscription,&ectypeNpcArmyTable->rebornTime,
			&ectypeNpcArmyTable->armyCardsType , &ectypeNpcArmyTable->cardDropOdds)< 19)
		{
			delete ectypeNpcArmyTable;
			ectypeNpcArmyTable = 0;
			continue;
		}
		//防止NPC 部队ID和建筑物重叠
		if(ectypeNpcArmyTable->armyID < 2000){
			ectypeNpcArmyTable->armyID += 2000;
		}
		if(ectypeNpcArmyTable->quitTime == 0){
			ectypeNpcArmyTable->quitTime = 1000000000;
		}
		ectypeNpcMap.insert(map<unsigned int , EctypeNpcArmyTable*>::value_type(ectypeNpcArmyTable->armyID,ectypeNpcArmyTable));
	}
	fclose(fp);
	return true;
}
void CNPCInfo::loadEctypeInfo(){
	FILE *fp = 0;
	fp = fopen("table/InstanceParam.csv","r");
	if(fp == NULL){
		printf("打开副本NPC信息文件失败！\n");
		return;
	}
	char fileBuf[1024]= {0};
	fgets(fileBuf,1024,fp);

	EctypeInfoTable * ectypeInfoTable = 0;
	while(!feof(fp))
	{
		if(fgets(fileBuf,1024,fp) == NULL)
			break;
		ectypeInfoTable = new EctypeInfoTable;
		if(sscanf(fileBuf,"%d,%d,%d,%d,%d,%d,%[^,],%d,%d,%d,%d",
			&ectypeInfoTable->ectypeID,&ectypeInfoTable->dLevel,&ectypeInfoTable->uLevel,&ectypeInfoTable->mapID,&ectypeInfoTable->npcArmyNum,
			&ectypeInfoTable->limitBout,&ectypeInfoTable->ectypeName,&ectypeInfoTable->entranceID,
			&ectypeInfoTable->limitArmyNum ,&ectypeInfoTable->limitPlayerNum,&ectypeInfoTable->warType)<10)
		{
			delete ectypeInfoTable;
			ectypeInfoTable = 0;
			continue;
		}
		ectypeInfos_.insert(map<int , EctypeInfoTable*>::value_type(ectypeInfoTable->entranceID,ectypeInfoTable));
	}
	fclose(fp);
}
void CNPCInfo::loadPatrolInfo(){
	for(int i = 1001 ; i <= 2200 ; i ++){
		map<unsigned int , PatrolPosVect*> patrolMap;
		if(loadPatrolInfo(i , patrolMap) == true){
			armyPatrolData_.insert(map<int , map<unsigned int , PatrolPosVect*> >::value_type(i , patrolMap));
		}
	}
}
bool CNPCInfo::loadPatrolInfo(int ectypeID , map<unsigned int , PatrolPosVect*> & patrolMap){
	char filePath[50] = {0};
	FILE *fp = 0;
	sprintf(filePath,"table/%dPatrol.csv",ectypeID);
	fp = fopen(filePath,"r");
	if(fp == NULL){
		return false;
	}
	
	char fileBuf[1024]= {0};
	fgets(fileBuf,1024,fp);
	PatrolPosVect * patrolArrayTable = NULL;
	aistrategy::PatrolRecord patrolData;
	int npcArmyID;
	while(!feof(fp))
	{
		if(fgets(fileBuf,1024,fp) == NULL)
			break;
		
		if(sscanf(fileBuf,"%d,%d,%d,%d,%d",
			&ectypeID,&npcArmyID,&patrolData.patrolPos.xPos,
			&patrolData.patrolPos.yPos,&patrolData.settleTime)< 5)
		{
			continue;
		}
		//防止NPC 部队ID和建筑物重叠
		if(npcArmyID < 2000){
			npcArmyID += 2000;
		}
		map<unsigned int , PatrolPosVect*>::iterator iter = patrolMap.find(npcArmyID);
		if (iter == patrolMap.end()){
			patrolArrayTable = new PatrolPosVect;
			assert(patrolArrayTable != NULL);
			patrolMap.insert(map<unsigned int , PatrolPosVect*>::value_type(npcArmyID,patrolArrayTable));
		}
		else{
			patrolArrayTable = iter->second;
		}
		patrolArrayTable->push_back(patrolData);
	}
	fclose(fp);
	return true;
}

void CNPCInfo::loadFightOverCondition(){
	FILE *fp = 0;
	fp = fopen("table/InstanceWinCondition.csv","r");
	if(fp == NULL){
		printf("打开副本InstanceWinCondition文件失败！\n");
		return;
	}
	char fileBuf[1024]= {0};
	fgets(fileBuf,1024,fp);

	FightOverConditionArray * fightOverConditionArray = 0;
	FightOverCondition fightOverCondition;
	while(!feof(fp))
	{
		if(fgets(fileBuf,1024,fp) == NULL)
			break;
		if(sscanf(fileBuf,"%d,%d,%d,%d,%d",
			&fightOverCondition.ectypeID,&fightOverCondition.cmdType,&fightOverCondition.targetID,
			&fightOverCondition.param ,&fightOverCondition.condition)<4){
			continue;
		}
		map<int , FightOverConditionArray*>::iterator iter;
		iter = fightOverConditionData_.find(fightOverCondition.ectypeID);
		if (iter == fightOverConditionData_.end()){
			fightOverConditionArray = new FightOverConditionArray;
			assert(fightOverConditionArray != NULL);
			fightOverConditionData_.insert(map<int , FightOverConditionArray*>::value_type(fightOverCondition.ectypeID,fightOverConditionArray));
		}
		else{
			fightOverConditionArray = iter->second;
		}
		fightOverConditionArray->push_back(fightOverCondition);
	}
	fclose(fp);
}
void CNPCInfo::loadAiLevel(){
	FILE *fp = 0;
	fp = fopen("table/AiLevel.csv","r");
	if(fp == NULL){
		printf("打开副本AiLevel文件失败！\n");
		return;
	}
	char fileBuf[1024]= {0};
	fgets(fileBuf,1024,fp);

	aistrategy::AiLevelParam * aiLevelTable = NULL;
	int aiLevel;
	while(!feof(fp))
	{
		if(fgets(fileBuf,1024,fp) == NULL)
			break;
		aiLevelTable = new aistrategy::AiLevelParam;
		if(sscanf(fileBuf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&aiLevel,&aiLevelTable->sightVal,&aiLevelTable->isCanCalled,&aiLevelTable->isDynamicSeekPath,&aiLevelTable->callRangeVal,
			&aiLevelTable->countRange,&aiLevelTable->tipTopAttackTar,&aiLevelTable->pauseProbability , &aiLevelTable->runAway , &aiLevelTable->turnPathMode)< 10)
		{
			delete aiLevelTable;
			aiLevelTable = 0;
			continue;
		}
		aiLevelParamData_.insert(map<int , aistrategy::AiLevelParam*>::value_type(aiLevel,aiLevelTable));
	}
	fclose(fp);
}
void CNPCInfo::loadDerateHurt(){
	FILE *fp = 0;
	fp = fopen("configtable/EquipAntagonism.csv","r");
	if(fp == NULL){
		printf("打开副本EquipAntagonism文件失败！\n");
		return;
	}
	char fileBuf[1024]= {0};
	fgets(fileBuf,1024,fp);

	DerateHurtTable derateHurt ;
	while(!feof(fp))
	{
		if(fgets(fileBuf,1024,fp) == NULL)
			break;
		if(sscanf(fileBuf,"%d,%d,%d",
			&derateHurt.weaponID , &derateHurt.defendID ,&derateHurt.attackOdds)< 3)
		{
			continue;
		}
		derateHurtMap_[derateHurt.weaponID][derateHurt.defendID] = derateHurt.attackOdds;
	}
	fclose(fp);
}
void CNPCInfo::loadNpcTemplate(){
	FILE *fp = 0;
	fp = fopen("table/BuildingNPCtemplate.csv","r");
	if(fp == NULL){
		printf("打开BuildingNPCtemplate.csv文件失败！\n");
		return;
	}
	char fileBuf[1024]= {0};
	fgets(fileBuf,1024,fp);

	vector<NpcTemplate> * vecNpcTemplate;
	NpcTemplate npcTemplate;
	while(!feof(fp))
	{
		if(fgets(fileBuf,1024,fp) == NULL)
			break;
		if(sscanf(fileBuf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&npcTemplate.id , &npcTemplate.npcTypeID ,&npcTemplate.strategyType,&npcTemplate.aiLevel , &npcTemplate.soldierNum,
			&npcTemplate.x , &npcTemplate.y ,&npcTemplate.dropItem, &npcTemplate.dropOdds , &npcTemplate.enterTime,
			&npcTemplate.quitTime , &npcTemplate.isInfinityReproduce ,&npcTemplate.rebornTime,&npcTemplate.armyCardsType,&npcTemplate.cardDropOdds)< 15)
		{
			continue;
		}
		vecNpcTemplate  = getNpcTemplate(npcTemplate.id);
		if (vecNpcTemplate == NULL)
		{
			vecNpcTemplate = new vector<NpcTemplate>;
			if (vecNpcTemplate == NULL){
				return ;
			}
			npcTemplateData_.insert(map<int , vector<NpcTemplate> *>::value_type(npcTemplate.id , vecNpcTemplate));
		}
		vecNpcTemplate->push_back(npcTemplate);
	}
	fclose(fp);
}
void CNPCInfo::loadEctypeBuildParam(){
	FILE *fp = 0;
	fp = fopen("table/EctypeBuildParamtable.csv","r");
	if(fp == NULL){
		printf("打开EctypeBuildParamtable.csv文件失败！\n");
		return;
	}
	char fileBuf[1024]= {0};
	fgets(fileBuf,1024,fp);
	PublicBuildParamTable * buildParamPtr;

	while(!feof(fp))
	{
		if(fgets(fileBuf,1024,fp) == NULL)
			break;
		buildParamPtr = new PublicBuildParamTable;
		if (buildParamPtr == NULL)
		{
			continue;
		}
		if(sscanf(fileBuf,"%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&buildParamPtr->pbuildingkind_ , &buildParamPtr->name_ ,&buildParamPtr->level_,&buildParamPtr->templateid_ , &buildParamPtr->attack_,
			&buildParamPtr->attackkind_, &buildParamPtr->upgradefee_,&buildParamPtr->foodcount_, &buildParamPtr->woodcount_ , &buildParamPtr->ironcount_,
			&buildParamPtr->skincount_ , &buildParamPtr->attackscop_ ,&buildParamPtr->visualrange_,&buildParamPtr->denfenid_,&buildParamPtr->hp_,
			&buildParamPtr->exp_ , &buildParamPtr->skill_ , &buildParamPtr->skillodds_,&buildParamPtr->output_,&buildParamPtr->armyCardsType_,
			&buildParamPtr->cardsDropOdds_)< 15)
		{
			delete buildParamPtr;
			continue;
		}
		ectypeBuildParam_[buildParamPtr->pbuildingkind_][buildParamPtr->level_] = buildParamPtr;
	}
	fclose(fp);
}
void CNPCInfo::loadEctypeBuild(){
	FILE *fp = 0;
	fp = fopen("table/EctypeBuildTable.csv","r");
	if(fp == NULL){
		printf("打开EctypeBuildTable.csv文件失败！\n");
		return;
	}
	char fileBuf[1024]= {0};
	fgets(fileBuf,1024,fp);
	PublicBuildTable * buildPtr;
	while(!feof(fp))
	{
		if(fgets(fileBuf,1024,fp) == NULL)
			break;
		buildPtr = new PublicBuildTable;
		if (buildPtr == NULL)
		{
			continue;
		}
		if(sscanf(fileBuf,"%d,%d,%d,%d,%d,%d",
			&buildPtr->mapid_ , &buildPtr->pbuildingid_ ,&buildPtr->pbuildingkind_,&buildPtr->neutraltrap_ , 
			&buildPtr->currentlevel_,&buildPtr->currentmoney_)< 6)
		{
			delete buildPtr;
			continue;
		}
		ectypeBuild_[buildPtr->pbuildingid_] = buildPtr;
	}
	fclose(fp);
}
void CNPCInfo::loadLandFormMap(void){
	FILE *fp = 0;
	fp = fopen("configtable/landformAffect.csv","r");
	if(fp == NULL){
		printf("打开landformAffect.csv文件失败！\n");
		return;
	}
	char fileBuf[1024]= {0};
	fgets(fileBuf,1024,fp);
	aistrategy::LandformAffect * landFormAffect = NULL;
	while(!feof(fp))
	{
		if(fgets(fileBuf,1024,fp) == NULL)
			break;
		landFormAffect = new aistrategy::LandformAffect;
		if (landFormAffect == NULL)
		{
			continue;
		}
		if(sscanf(fileBuf,"%d,%d,%d,%d,%d",
			&landFormAffect->landfrom , &landFormAffect->soldierType ,&landFormAffect->boutVal,&landFormAffect->attackAffect , 
			&landFormAffect->defendAffect)< 5)
		{
			delete landFormAffect;
			continue;
		}
		landFormMap_[std::pair<LAND_FORM , SOLDIER_TYPE>(landFormAffect->landfrom ,landFormAffect->soldierType)] = landFormAffect;
	}
	fclose(fp);
}
void CNPCInfo::loadLimitTable(void){
	FILE *fp = 0;
	fp = fopen("configtable/ArmyBrandLimitTable.csv","r");
	if(fp == NULL){
		printf("打开ArmyBrandLimitTable.csv文件失败！\n");
		return;
	}
	char fileBuf[1024]= {0};
	fgets(fileBuf,1024,fp);
	LimitTable limitValue;
	while(!feof(fp))
	{
		if(fgets(fileBuf,1024,fp) == NULL)
			break;
		if(sscanf(fileBuf,"%d,%d,%d,%d,%d",
			&limitValue.limittype_ , &limitValue.limitkey_ ,&limitValue.limitval_)< 3)
		{
			continue;
		}
		limitValueMap_[std::pair<LIMIT_TYPE , int>(static_cast<LIMIT_TYPE>(limitValue.limittype_ ),
			limitValue.limitkey_)] = limitValue.limitval_;
	}
	fclose(fp);
}
void CNPCInfo::loadWarEntranceInfo(void){
	FILE *fp = 0;
	fp = fopen("table/SuperBattle.csv","r");
	if(fp == NULL){
		printf("打开SuperBattle.csv文件失败！\n");
		return;
	}
	char fileBuf[1024]= {0};
	fgets(fileBuf,1024,fp);
	WarEntranceInfo * warEntrancePtr;
	while(!feof(fp))
	{
		if(fgets(fileBuf,1024,fp) == NULL)
			break;
		warEntrancePtr = new WarEntranceInfo;
		if (warEntrancePtr == NULL){
			continue;
		}
		if(sscanf(fileBuf,"%d,%d,%d,%d,%d",
			&warEntrancePtr->buildId , &warEntrancePtr->mapId ,&warEntrancePtr->label , &warEntrancePtr->groupId)< 4)
		{
			delete warEntrancePtr;
			continue;
		}
		warEntranceMap_[warEntrancePtr->mapId][warEntrancePtr->groupId].push_back(warEntrancePtr);
	}
	fclose(fp);
}
void CNPCInfo::loadPropCardInfo(void){
	FILE *fp = 0;
	fp = fopen("configtable/PropCardConfig.csv","r");
	if(fp == NULL){
		printf("打开PropCardConfig.csv文件失败！\n");
		return;
	}
	char fileBuf[1024]= {0};
	fgets(fileBuf,1024,fp);
	PropCardConfig * propCardInfo;
	while(!feof(fp))
	{
		if(fgets(fileBuf,1024,fp) == NULL)
			break;
		propCardInfo = new PropCardConfig;
		if (propCardInfo == NULL){
			continue;
		}
		if(sscanf(fileBuf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&propCardInfo->itermId , &propCardInfo->effect ,&propCardInfo->cardType , &propCardInfo->cardValue,
			&propCardInfo->lastTime , &propCardInfo->ectypeLimit ,&propCardInfo->warLimit ,
			&propCardInfo->countryLimit,&propCardInfo->fieldLimit,&propCardInfo->isWarCanUse , &propCardInfo->isPeaceCanUse,
			&propCardInfo->coolingTime,&propCardInfo->isArmyLimitUse)< 13)
		{
			delete propCardInfo;
			continue;
		}
		propCardConfig_[propCardInfo->itermId] = propCardInfo;
	}
	fclose(fp);
}

void CNPCInfo::loadGuarderParam(void){
	FILE *fp = 0;
	fp = fopen("configtable/GuarderTable.csv","r");
	if(fp == NULL){
		printf("打开GuarderTable.csv文件失败！\n");
		return;
	}
	char fileBuf[1024]= {0};
	fgets(fileBuf,1024,fp);
	NpcGuarderParam * npcGuarderParam;
	while(!feof(fp))
	{
		if(fgets(fileBuf,1024,fp) == NULL)
			break;
		npcGuarderParam = new NpcGuarderParam;
		if (npcGuarderParam == NULL){
			continue;
		}
		if(sscanf(fileBuf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&npcGuarderParam->coutryID , &npcGuarderParam->mapID ,&npcGuarderParam->npcID , &npcGuarderParam->strategy,
			&npcGuarderParam->aiLevel , &npcGuarderParam->npcNum ,&npcGuarderParam->dropItem , &npcGuarderParam->dropOdds,
			&npcGuarderParam->armyCard,&npcGuarderParam->armyCardDropOdds)< 10)
		{
			delete npcGuarderParam;
			continue;
		}
		npcGuardersParam_[npcGuarderParam->mapID][npcGuarderParam->coutryID].push_back(npcGuarderParam);
	}
	fclose(fp);
}
void CNPCInfo::clearEctypeNpcType(){
	map<unsigned int , EctypeNpcTypeTable*>::iterator  iter;
	for (iter = ectypeNpcTypes_.begin(); iter != ectypeNpcTypes_.end() ; iter++){
		delete iter->second;
		iter->second = NULL;
	}
	ectypeNpcTypes_.clear();
}
void CNPCInfo::clearEctypeNpc(){
	map<int , map<unsigned int , EctypeNpcArmyTable*> >::iterator iter;
	map<unsigned int , EctypeNpcArmyTable*>::iterator tableIter;

	for(iter = ectypeNpcArmys_.begin(); iter != ectypeNpcArmys_.end();iter ++){
		map<unsigned int , EctypeNpcArmyTable*>& tmpTable = iter->second;
		for(tableIter = tmpTable.begin() ; tableIter != tmpTable.end(); tableIter++){
			delete tableIter->second;
			tableIter->second = NULL;
		}
		tmpTable.clear();
	}
	ectypeNpcArmys_.clear();
}
void CNPCInfo::clearEctypeInfo(){
	map<int , EctypeInfoTable*>::iterator  iter;
	for (iter = ectypeInfos_.begin() ; iter != ectypeInfos_.end() ; iter++){
		delete iter->second;
		iter->second = NULL;
	}
}
void CNPCInfo::clearPatrolInfo(){
	map<int , map<unsigned int , PatrolPosVect*> >::iterator iter;
	map<unsigned int , PatrolPosVect*>::iterator arrayIter;
	for(iter = armyPatrolData_.begin(); iter != armyPatrolData_.end();iter ++){
		map<unsigned int , PatrolPosVect*>& tmpTable = iter->second;
		for(arrayIter = tmpTable.begin() ; arrayIter != tmpTable.end(); arrayIter++){
			delete arrayIter->second;
			arrayIter->second = NULL;
		}
		tmpTable.clear();
	}
	armyPatrolData_.clear();
}
void CNPCInfo::clearAiLevel(){
	map<int , aistrategy::AiLevelParam*>::iterator  iter;
	for (iter = aiLevelParamData_.begin(); iter != aiLevelParamData_.end() ; iter++){
		delete iter->second;
		iter->second = NULL;

	}
	aiLevelParamData_.clear();
}
void CNPCInfo::clearNpcTemplate(){
	map<int , vector<NpcTemplate>*>::iterator  iter;
	for (iter = npcTemplateData_.begin(); iter != npcTemplateData_.end() ; iter++){
		delete iter->second;
		iter->second = NULL;

	}
	npcTemplateData_.clear();
}
void CNPCInfo::clearFightOverCondition(){
	map<int , FightOverConditionArray*>::iterator  iter;
	for (iter = fightOverConditionData_.begin() ; iter != fightOverConditionData_.end() ; iter++){
		delete iter->second;
		iter->second = NULL;
	}
}
void CNPCInfo::clearNpcBuildParam(){
	map<int , map<int , PublicBuildParamTable*> >::iterator iter;
	map<int , PublicBuildParamTable*>::iterator subIter;
	for (iter = ectypeBuildParam_.begin(); iter != ectypeBuildParam_.end() ; ++ iter)
	{
		for (subIter = iter->second.begin() ; subIter != iter->second.end(); ++subIter)
		{
			delete subIter->second;
			subIter->second = NULL;
		}
	}
}
void CNPCInfo::clearEctypeBuild(){
	map<unsigned int , PublicBuildTable*>::iterator iter;
	
	for (iter = ectypeBuild_.begin(); iter != ectypeBuild_.end() ; ++ iter)
	{
		delete iter->second;
		iter->second = NULL;
	}
}
void CNPCInfo::clearDerateHurt(){
	derateHurtMap_.clear();
}
void CNPCInfo::clearLandFormMap(void){
	LandformInfoMap::iterator iter; 
	for(iter = landFormMap_.begin() ; iter != landFormMap_.end() ; ++iter){
		delete iter->second;
		iter->second = NULL;
	}
	landFormMap_.clear();
}
void CNPCInfo::clearLimitTable(void){
	limitValueMap_.clear();
}
void CNPCInfo::clearWarEntranceInfo(void){
	map<int , map<int , list<WarEntranceInfo *>>>::iterator iter;
	map<int , list<WarEntranceInfo *>>::iterator subIter;
	list<WarEntranceInfo *>::iterator listIter;
	for (iter = warEntranceMap_.begin() ; iter != warEntranceMap_.end() ; ++iter)
	{
		for (subIter = iter->second.begin() ;subIter!= iter->second.end() ; ++subIter)
		{
			list<WarEntranceInfo *> &tmpList = subIter->second;
			for (listIter = tmpList.begin() ; listIter!= tmpList.end() ; ++listIter)
			{
				delete *listIter;
				*listIter = NULL;
			}
		}
	}
}
void CNPCInfo::clearPropCardInfo(void){
	map<int ,PropCardConfig*>::iterator iter;
	for (iter = propCardConfig_.begin(); iter != propCardConfig_.end() ;++iter)
	{
		delete iter->second;
		iter->second = NULL;
	}
	propCardConfig_.clear();
}

void CNPCInfo::clearGuarderParam(void){
	map<int , map<int , list<NpcGuarderParam*>>> ::iterator iter;
	map<int , list<NpcGuarderParam*>>::iterator subIter;
	list<NpcGuarderParam*>::iterator listIter;
	for (iter = npcGuardersParam_.begin() ; iter != npcGuardersParam_.end() ; ++iter)
	{
		for (subIter = iter->second.begin() ;subIter!= iter->second.end() ; ++subIter){
			list<NpcGuarderParam*>& tmpList = subIter->second;
			for (listIter = tmpList.begin() ; listIter != tmpList.end() ; ++listIter)
			{
				delete (*listIter);
				(*listIter) = NULL;
			}
		}
	}
}
bool CNPCInfo::getEctypeNpc(int ectypeID , map<unsigned int , EctypeNpcArmyTable*> & ectypeNpcArms){
	map<int , map<unsigned int , EctypeNpcArmyTable*> >::iterator iter = ectypeNpcArmys_.find(ectypeID);
	if (iter != ectypeNpcArmys_.end()){
		ectypeNpcArms = iter->second;
		return true;
	}
	return false;
}

EctypeNpcTypeTable* CNPCInfo::getEctypeNpcType(int npcArmyTypeID){
	map<unsigned int , EctypeNpcTypeTable*>::iterator iter = ectypeNpcTypes_.find(npcArmyTypeID);
	if (iter != ectypeNpcTypes_.end()){
		return iter->second;
	}
	return NULL;
}
EctypeInfoTable * CNPCInfo::getEctypeInfoTable(int entranceID){
	map<int , EctypeInfoTable*>::iterator iter = ectypeInfos_.find(entranceID);
	if (iter != ectypeInfos_.end()){
		return iter->second;
	}
	return NULL;
}
map<unsigned int , PatrolPosVect*> * CNPCInfo::getPatrolData(int ectypeID){
	map<int , map<unsigned int , PatrolPosVect*> >::iterator iter;
	iter = armyPatrolData_.find(ectypeID);
	if (iter != armyPatrolData_.end()){
		return &iter->second;
	}
	return NULL;
}
FightOverConditionArray* CNPCInfo::getFightOverCondition(int ectypeID){
	map<int , FightOverConditionArray*>::iterator iter;
	iter = fightOverConditionData_.find(ectypeID);
	if (iter != fightOverConditionData_.end()){
		return iter->second;
	}
	return NULL;
}
aistrategy::AiLevelParam * CNPCInfo::getAiLevelParam(int aiLevel){
	map<int , aistrategy::AiLevelParam*>::iterator iter;
	iter = aiLevelParamData_.find(aiLevel);
	if (iter != aiLevelParamData_.end()){
		return iter->second;
	}
	return NULL;
}

int CNPCInfo::getHurtOdds(int weaponType , int defendType){
	map<int ,map<int , int>>::iterator iter = derateHurtMap_.find(weaponType);
	if (iter == derateHurtMap_.end()){
		return 100;
	}
	map<int , int >::iterator tmpIter = iter->second.find(defendType);
	if (tmpIter == iter->second.end()){
		return 100;
	}
	return tmpIter->second;
}
vector<NpcTemplate> * CNPCInfo::getNpcTemplate(int templateID){
	map<int , vector<NpcTemplate> *>::iterator iter;
	iter = npcTemplateData_.find(templateID);
	if (iter != npcTemplateData_.end()){
		return iter->second;
	}
	return NULL;
}
PublicBuildParamTable * CNPCInfo::getEctypeBuildParam(int buildType , int buildLeve){
	map<int , map<int , PublicBuildParamTable*> >::iterator iter;
	iter = ectypeBuildParam_.find(buildType);
	if (iter == ectypeBuildParam_.end()){
		return NULL;
	}
	map<int , PublicBuildParamTable*>::iterator subIter;
	subIter = iter->second.find(buildLeve);
	if (subIter == iter->second.end()){
		return NULL;
	}
	return subIter->second;
}
//获得建筑物信息
PublicBuildTable * CNPCInfo::getEctypeBuild(unsigned int buildID){
	map<unsigned int , PublicBuildTable*>::iterator iter = ectypeBuild_.find(buildID);
	if (iter == ectypeBuild_.end()){
		return NULL;
	}
	return iter->second;
}
//获得地形影响信息
aistrategy::LandformAffect * CNPCInfo::getLandFormAffect(LAND_FORM landForm, SOLDIER_TYPE soldierType){
	LandformInfoMap::iterator iter = landFormMap_.find(std::pair<LAND_FORM , SOLDIER_TYPE>(landForm , soldierType));
	if (iter != landFormMap_.end()){
		return iter->second;
	}
	return NULL;
}
int CNPCInfo::getLimitValue(LIMIT_TYPE limitType , int limitKey){
	map<std::pair<LIMIT_TYPE , int> , int>::iterator iter;
	iter = limitValueMap_.find(std::pair<LIMIT_TYPE,int>(limitType , limitKey));
	if (iter == limitValueMap_.end()){
		return 0;
	}
	return iter->second;
}
map<int ,list<WarEntranceInfo *>>* CNPCInfo::getWarEntranceInfo(unsigned int mapid){
	map<int , map<int , list<WarEntranceInfo *>>>::iterator iter = warEntranceMap_.find(mapid);
	if (iter != warEntranceMap_.end())
	{
		return &iter->second;
	}
	return NULL;
}
PropCardConfig * CNPCInfo::getPropCardInfo(int itemId){
	map<int ,PropCardConfig*>::iterator iter = propCardConfig_.find(itemId);
	if (iter != propCardConfig_.end()){
		return iter->second;
	}
	return NULL;
}
const list<NpcGuarderParam*>* CNPCInfo::getGuarderParam(int mapId , int countryID) const{
	map<int , map<int , list<NpcGuarderParam*>>> ::const_iterator iter;
	map<int , list<NpcGuarderParam*>>::const_iterator subIter;
	iter = npcGuardersParam_.find(mapId);
	if (iter == npcGuardersParam_.end()){
		return NULL;
	}
	subIter =  iter->second.find(countryID);
	if (subIter == iter->second.end()){
		return NULL;
	}
	return &subIter->second;
}