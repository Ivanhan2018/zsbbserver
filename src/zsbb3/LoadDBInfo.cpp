#include "LoadDBinfo.h"
//#include "NetTask.h"
#include <iostream>

#pragma warning(disable:4100)
#pragma warning(disable:4189)
#pragma warning(disable:4127)
#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4389)
#pragma warning(disable:4706)
#pragma warning(disable:4800)
CLoadDBInfo g_LoadDBinfo ;
CLoadDBInfo::CLoadDBInfo(void)
{
}

CLoadDBInfo::~CLoadDBInfo(void)
{
	Empty();
}
void CLoadDBInfo::LoadParam(DB_Proxy * proxy , int mapID)
{
	if (proxy == NULL){
		return;
	}
	mapID_ = mapID;
	dataBaseProxy_ = proxy;
	
	char* p = NULL ;
	//char *q;
	int len ;
	int ret ;
	int strlen;
	int k = 0;


	EctypeBaseTable EctypeBaseInfo;
	ret = dataBaseProxy_->db_select_all(EctypeBaseInfo,p,len);
	if (ret == DB_Proxy::DB_SUCCEED)
	{
		k =0 ;
		strlen = (int)(sizeof(EctypeBaseTable));
		while(k<len && len-k >= strlen)
		{
			memcpy(&EctypeBaseInfo,p+k,sizeof(EctypeBaseTable));
			k+=strlen;
			//if (EctypeBaseInfo.doorid_/10000 == mapID_)
			{
				m_EctypBaseInfoList.push_back(EctypeBaseInfo);
			}

		}
		if (len>0)
		{
			delete [] p;
			p = NULL;
		}
	}

	PublicBuildTable build;
	PublicBuildTable* pbuild;
	build.setmasterid(mapID_);
	ret = dataBaseProxy_->db_multi_select(build,p,len);
	if (ret == DB_Proxy::DB_SUCCEED)
	{
		k =0;
		strlen = (int)(sizeof(PublicBuildTable));
		while (k<len&& len-k>=strlen)
		{
			pbuild = new PublicBuildTable;
			memcpy(pbuild,p+k,sizeof(PublicBuildTable));
			k += strlen;
			m_PublicBuildList[pbuild->pbuildingid_]= pbuild;
		}
		if (len>0)
		{
			delete [] p;
			p = NULL;
		}
	}

	
	OfficerRaceAttribute OfficerAttr;
	OfficerRaceAttribute* pOfficerAttr;
	ret = dataBaseProxy_->db_select_all(OfficerAttr,p ,len);
	if (ret == DB_Proxy::DB_SUCCEED)
	{
		k = 0 ; 
		strlen = (int)(sizeof(OfficerRaceAttribute));
		while (k<len && len-k >=sizeof(OfficerRaceAttribute))
		{
			pOfficerAttr = new OfficerRaceAttribute;
			memcpy(pOfficerAttr,p+k,sizeof(OfficerRaceAttribute));
			k+=strlen;
			OfficerRaceAttr_List[pOfficerAttr->raceid_] = pOfficerAttr ;
		}
		if (len>0)
		{
			delete [] p ;
			p = NULL;
		}
	}
	
	p = NULL;
	len = 0;
	//
	k = 0;
	PlayerOfficerTable Officertable;

	ret = dataBaseProxy_->db_select_all(Officertable,p,len);
	if (len>0)
	{
		strlen = (int)(sizeof(PlayerOfficerTable));
		while (k+strlen<=len)
		{
			memcpy(&Officertable,p+k,sizeof(PlayerOfficerTable));
			if (Officertable.playerid_ == Officertable.officerid_)
			{
				string str(Officertable.officername_);
				m_UserNameList[Officertable.playerid_] = str;				
			}
			k+= strlen;
		}
		delete [] p;
		p = NULL;
	}

	k = 0 ;
	EquipAntagonism Equip;
	ret = dataBaseProxy_->db_select_all(Equip,p,len);
	if (len>0)
	{
		strlen = (int)(sizeof(EquipAntagonism));
		while(k+strlen<=len)
		{
			memcpy(&Equip,p+k,sizeof(EquipAntagonism));
			m_EquipAntagonism[Equip.weaponid_][Equip.defendid_]=Equip.attackodds_;
			k+=strlen;
		}
		delete [] p;
		p = NULL;
	}

	k = 0 ;
	BatttleField battle;
	BatttleField* pbattle;
	ret = dataBaseProxy_->db_select_all(battle,p,len);
	if (len>0)
	{
		strlen = (int)(sizeof(BatttleField));
		while (k+strlen<=len)
		{
			pbattle = new BatttleField;
			memcpy(pbattle,p+k,sizeof(BatttleField));
			m_BattleInfo[pbattle->mapid_] = pbattle;
			k+=strlen;
		}

		delete [] p;
		p = NULL;
	}
	loadSoldierarmorconfig();//读取甲参数表
	loadQuantityByRank();//读取军衔与数量的限制关系
	loadMailInfoParam();//读取邮件参数表
	loadCardConfig();//读取卡信息
	//读取玩家初始时的部队信息
	loadPlayerInitArmyParam();
	//读取NPC反击配置表
	loadGMNPCParam();
	//读取行动点参数表
	loadActionDotParam();
	//读取是否可以重叠存放的参数
	loadPackStoreParam();
	loadHiredNPCArmyParam();
	loadEquipAttribute();
	loadInlayResultAttribute();
	loadRefiningProbability();
	loadSkillDetail();
	//读取武器属性表
	loadWeaponAttribute();
	//读取防具属性表
	loadDefendAttribute();
	//读取英雄初始技能参数表
	loadOfficerInitSkill();
	//读取打开包厢时，装备的掉落概率参数
	loadDropEquipProbability();
	//读取军牌兑换参数表
	loadArmyBrandParam();
	//加载公共建筑参数表
	loadPublicBuildParam();
	//加载英雄成长系数几率表
	loadHeroGroupRateCapability();
	//读取科研配置表
	loadResearchConfigure();
	//读取生产配置表
	loadProduceConfigure();
	//读取建筑配置表
	loadConstructionConfigure();
	//读取城市信息表
	loadGameCityInfo();
	//加载广播消息参数表
	loadBroadMsgParam();
	//加载日常任务列表
	loadDayTaskParam();
	//加载士兵种族参数表
	loadSoldierParam();
	loadConsortiaUpdateParam();
}

OfficerRaceAttribute* CLoadDBInfo::GetOfficerRaceAttr(int race)
{
	OfficerRaceAttribute* pOfficerRaceAttr = NULL;
	map<int ,OfficerRaceAttribute* >::iterator iter ;
	iter = OfficerRaceAttr_List.find(race);
	if (iter!=OfficerRaceAttr_List.end())
	{
		pOfficerRaceAttr = iter->second;
	}
	return pOfficerRaceAttr ;
}
WeaponAttribute* CLoadDBInfo::GetWeaponAttribute(int weaponid)
{
	WeaponAttribute_Map_Iterator iter;
	WeaponAttribute* pWeaponAttri = NULL;
	iter = m_weaponAttributeMap.find(weaponid);
	if (iter != m_weaponAttributeMap.end())
	{
		pWeaponAttri = iter->second;
	}
	return pWeaponAttri;
}
SoldierParamTable* CLoadDBInfo::GetSoldierParam(int soldiertype)
{
	map<int , SoldierParamTable* > ::iterator iter;
	SoldierParamTable* Param = NULL;
	iter = Soldier_Param_list.find(soldiertype);
	if (iter != Soldier_Param_list.end())
	{
		Param = iter->second;
	}
	return Param ;
}
GameCityInfo* CLoadDBInfo::GetCityInfo(int cityid)
{
	GameCityInfo_Map_Iterator iter;
	GameCityInfo* pCityInfo = NULL;
	iter = m_GameCityInfoMap.find(cityid);
	if (iter != m_GameCityInfoMap.end())
	{
		pCityInfo = iter->second;
	}
	return pCityInfo;
}


DefendAttribute* CLoadDBInfo::GetDefendAttribute(int iDefendID)
{
	DefendAttribute_Map_Iterator iter;
	DefendAttribute* pDefendAttri = NULL;
	iter = m_defendAttributeMap.find(iDefendID);
	if (iter != m_defendAttributeMap.end())
	{
		pDefendAttri = iter->second;
	}
	return pDefendAttri;
}

PublicBuildParamTable* CLoadDBInfo::GetPublicBuildParam(int buildKind, int level)
{
	map<int,map<int , PublicBuildParamTable*> >::iterator iter;
	iter = publicBuildParam_.find(buildKind);
	if (iter == publicBuildParam_.end()){
		return NULL;
	}
	map<int , PublicBuildParamTable*>::iterator subIter ;
	subIter = iter->second.find(level);
	if (subIter == iter->second.end()){
		return NULL;
	}
	return subIter->second;
}
ConstructionConfigure* CLoadDBInfo::getConstructionConfigure(int iConID, int iLevel)
{
	ConstructionConfigure* pConstrParam = NULL;
	ConstructionConfigure_Multimap_Iterator lowIter,upperIter;
	lowIter = m_ConstructionConfigureMultimap.lower_bound(iConID);
	upperIter = m_ConstructionConfigureMultimap.upper_bound(iConID);
	
	for(; lowIter != upperIter; lowIter++)
	{
		if (lowIter->second->level_ == iLevel)
		{
			pConstrParam = lowIter->second;
			break;
		}
	}
	
	return pConstrParam;
}
ProduceConfigure* CLoadDBInfo::GetProduceConfig(int iProID)
{
	ProduceConfigure_Map_Iterator iter;
	ProduceConfigure* pProConfig = NULL;
	iter = m_ProduceConfigureMap.find(iProID);
	if (iter!=m_ProduceConfigureMap.end())
	{
		pProConfig = iter->second;
	}
	return pProConfig;
}
ResearchConfigure* CLoadDBInfo::GetResearchConfig(int iItemID)
{
	ResearchConfigure_Map_Iterator iter;
	ResearchConfigure* pResearcConfig = NULL;
	iter = m_ResearchConfigureMap.find(iItemID);
	if (iter!=m_ResearchConfigureMap.end())
	{
		pResearcConfig = iter->second;
	}
	return pResearcConfig;
}

PublicBuildTable* CLoadDBInfo::GetPublicBuildInfo(unsigned long buildid)
{
	map<unsigned long ,PublicBuildTable* >::iterator iter;
	PublicBuildTable* pBuild = NULL;
	iter = m_PublicBuildList.find(buildid);
	if (iter != m_PublicBuildList.end())
	{
		pBuild = iter->second;
	}
	return pBuild;
}

void CLoadDBInfo::AddPublicBuildInfo(PublicBuildTable* build)
{
	if (build == NULL)
	{
		return ;
	}
	else
	{
		m_PublicBuildList[build->pbuildingid_] = build;
	}
}


void CLoadDBInfo::UpdatePublicBuildLevel(unsigned long buildid,int level ,int type)
{
	std::cout<<buildid<<"  "<<level<<std::endl;
	map<unsigned long , PublicBuildTable* >::iterator iter;
	iter = m_PublicBuildList.find(buildid);
	PublicBuildTable* pBulid = NULL;
	if (iter != m_PublicBuildList.end())
	{
		pBulid = iter->second;
		pBulid->currentlevel_ = level;
		if(type >0)
		{
			pBulid->pbuildingkind_ = type;
		}
	}
}

void CLoadDBInfo::AddUserName(unsigned long UserID,string str)
{
	m_UserNameList[UserID] = str;
}
string CLoadDBInfo::GetHeroName(unsigned long UserID)
{
	string str;
	map<unsigned long , string>::iterator iter;
	iter = m_UserNameList.find(UserID);
	if (iter!=m_UserNameList.end())
	{
		str = (*iter).second;
	}
	return str;
}
int CLoadDBInfo::GetEquipAntagonismParam(unsigned int WeaponID,unsigned int defendID)
{
	int addo=100;
	map<unsigned int,map<unsigned int ,int> >::iterator iter;
	iter = m_EquipAntagonism.find(WeaponID);
	if (iter!=m_EquipAntagonism.end())
	{
		map<unsigned int ,int>& tmp = iter->second;
		map<unsigned int ,int>::iterator iter1;
		iter1 = tmp.find(defendID);
		if (iter1!= tmp.end())
		{
			addo= (*iter1).second;
		}
	}
	return addo;

}

BatttleField* CLoadDBInfo::GetBattleBaseInfo(unsigned int id)
{
	BatttleField* pBattle = NULL;
	map<unsigned int , BatttleField*>::iterator iter;
	iter = m_BattleInfo.find(id);
	if (iter!=m_BattleInfo.end())
	{
		pBattle = iter->second;
	}

	return pBattle;

}
void CLoadDBInfo::Empty()
{
	map<int , SoldierParamTable* > ::iterator iter1;
	map<int ,OfficerRaceAttribute* >::iterator iter3;
	map<unsigned long, PublicBuildTable* >::iterator buliditer;

	SoldierParamTable* Param = NULL;
	for (iter1 = Soldier_Param_list.begin();iter1!=Soldier_Param_list.end();iter1 ++)
	{
		Param = iter1->second;
		delete Param;
		Param = NULL;
	}
	Soldier_Param_list.clear();

	OfficerRaceAttribute* pOfficerAttr = NULL;
	for (iter3 = OfficerRaceAttr_List.begin() ; iter3 != OfficerRaceAttr_List.end() ; iter3 ++ )
	{
		pOfficerAttr = iter3->second;
		delete pOfficerAttr ;
		pOfficerAttr = NULL;
	}
	OfficerRaceAttr_List.clear();

	map<int,map<int , PublicBuildParamTable*> >::iterator buildParamIter;
	map<int ,PublicBuildParamTable*>::iterator subIter;
	for (buildParamIter = publicBuildParam_.begin() ; buildParamIter != publicBuildParam_.end() ; ++buildParamIter )
	{
		for(subIter = buildParamIter->second.begin(); subIter != buildParamIter->second.end(); ++subIter){
			delete subIter->second;
			subIter->second = NULL;
		}
	}
	publicBuildParam_.clear();

	PublicBuildTable* pBulid = NULL;
	for (buliditer = m_PublicBuildList.begin();buliditer != m_PublicBuildList.end(); buliditer ++)
	{
		pBulid = buliditer->second;
		delete pBulid;
		pBulid = NULL;
	}
	m_PublicBuildList.clear();

	BatttleField* pBattle = NULL;
	map<unsigned int , BatttleField* >::iterator battleiter;
	for (battleiter = m_BattleInfo.begin();battleiter!=m_BattleInfo.end();battleiter++)
	{
		pBattle = battleiter->second;
		delete pBattle;
		pBattle = NULL;
	}
	m_BattleInfo.clear();

	clearSoldierarmorconfig();
	clearQuantityByRand();
	clearMailInfoParam();
	clearCardConfig();
	//清除玩家初始时的部队信息
	clearPlayerInitArmyParam();

	//清除NPC反击配置表
	clearGMNPCParam();
	//清楚行动点参数表
	clearActionDotParam();
	//清除是否可以重叠存放的参数表
	clearPackStoreParam();
	clearHiredNPCArmyParam();
	clearEquipAttribute();
	clearInlayResultAttribute();
	clearRefiningProbability();
	clearSkillDetail();
	clearWeaponAttribute();
	clearDefendAttribute();
	clearOfficerInitSkill();
	clearArmyBrandParam();
	heroGroupRateMap.clear();
	clearResearchConfigure();
	clearProduceConfigure();
	clearConstructionConfigure();
	clearGameCityInfo();
	clearBroadMsgParam();
	clearDayTaskParam();
	clearSoldierParam();
	clearConsortiaUpdateParam();
}

LPSoldierarmorconfig_Item CLoadDBInfo::GetSoldierarmorconfig(int defendid)
{
	Soldierarmorconfig_Map_Iterator iter;
	iter = m_SoldierarmorconfigList.find(defendid);
	if(iter == m_SoldierarmorconfigList.end())
		return NULL;
	
	return iter->second;
}
//读取甲参数表
void CLoadDBInfo::loadSoldierarmorconfig()
{
	
	FILE *fp = 0;
	fp = fopen("configtable/Soldierarmorconfig.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/Soldierarmorconfig.csv失败。\n");
		return;
	}

	char buf[128]= {0};
	fgets(buf,128,fp);

	LPSoldierarmorconfig_Item pItem;
	int iType = 0;
	//m_QuantityByRankList
	while(!feof(fp))
	{
		if(fgets(buf,128,fp) == NULL)
			break;

		pItem = new Soldierarmorconfig_Item;
		if(sscanf(buf,"%d,%d,%d,%d,%d,%d",&iType,&pItem->DefendID,&pItem->Soldier,&pItem->Shooter,&pItem->Churchman,&pItem->Rabbi)<6)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
	
		m_SoldierarmorconfigList[pItem->DefendID] = pItem;
	}
	/*
	Soldierarmorconfig_Map_Iterator iter;
	for(iter = m_SoldierarmorconfigList.begin();iter != m_SoldierarmorconfigList.end();iter++)
	{
		printf("%d,%d,%d,%d,%d\n",iter->second->DefendID,iter->second->Soldier,iter->second->Shooter,iter->second->Churchman,iter->second->Rabbi);
	}
	*/
	fclose(fp);
}
void CLoadDBInfo::clearSoldierarmorconfig()
{
	Soldierarmorconfig_Map_Iterator iter;
	LPSoldierarmorconfig_Item pDelItem = 0;
	for(iter = m_SoldierarmorconfigList.begin();iter != m_SoldierarmorconfigList.end();iter++)
	{
		if((pDelItem=iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_SoldierarmorconfigList.clear();
}
//读取甲参数表
void CLoadDBInfo::loadSoldierParam()
{

	FILE *fp = 0;
	fp = fopen("configtable/SoldierParamTable.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/SoldierParamTable.csv失败。\n");
		return;
	}

	char buf[128]= {0};
	fgets(buf,128,fp);

	SoldierParamTable* pItem;
	while(!feof(fp))
	{
		if(fgets(buf,128,fp) == NULL)
			break;

		pItem = new SoldierParamTable;
		if(sscanf(buf,"%d,%d,%d,%d,%d,%d,%d,%d,%d",&pItem->soldiertype_,&pItem->hp_,&pItem->physicevade_
			,&pItem->magicevade_,&pItem->movefrequency_,&pItem->visualrange_,&pItem->attackscop_,
			&pItem->duckrate_ ,&pItem->shooting_)<9)
		{
			delete pItem;
			pItem = 0;
			continue;
		}

		Soldier_Param_list[pItem->soldiertype_] = pItem;
	}
	fclose(fp);
}
void CLoadDBInfo::clearSoldierParam(){
	map<int , SoldierParamTable* >::iterator iter;
	SoldierParamTable* pDelItem = 0;
	for(iter = Soldier_Param_list.begin();iter != Soldier_Param_list.end();iter++)
	{
		if((pDelItem=iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	Soldier_Param_list.clear();
}
LPQuantityByRank_Item CLoadDBInfo::GetQuantityByRank(int rank)
{
	map<int,LPQuantityByRank_Item>::iterator iter;
	iter = m_QuantityByRankList.find(rank);
	if(iter == m_QuantityByRankList.end())
		return NULL;
	
	return iter->second;
}
//读取军衔与城市、英雄数量关系表
void CLoadDBInfo::loadQuantityByRank()
{
	FILE *fp = 0;
	fp = fopen("configtable/QuantityByRank.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/QuantityByRank.csv失败。\n");
		return;
	}

	char buf[32]= {0};
	fgets(buf,32,fp);

	LPQuantityByRank_Item pItem = 0;
	//m_QuantityByRankList
	while(!feof(fp))
	{
		if(fgets(buf,32,fp) == NULL)
			break;

		pItem = new QuantityByRank_Item;
		if(sscanf(buf,"%d,%d,%d",&pItem->Rank,&pItem->HeroQuantity,&pItem->TownQuantity)<3)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
	
		m_QuantityByRankList[pItem->Rank] = pItem;
	}
	/*
	map<int,LPQuantityByRank_Item>::iterator iter;
	for(iter = m_QuantityByRankList.begin();iter != m_QuantityByRankList.end();iter++)
	{
		printf("%d,%d,%d\n",iter->second->Rank,iter->second->HeroQuantity,iter->second->TownQuantity);
	}
	*/
	fclose(fp);
}
void CLoadDBInfo::clearQuantityByRand()
{
	QuantityByRank_Map_Iterator iter;
	LPQuantityByRank_Item pDelItem = 0;
	for(iter = m_QuantityByRankList.begin();iter != m_QuantityByRankList.end();iter++)
	{
		if((pDelItem = iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_QuantityByRankList.clear();
}
LPMailInfoParam_Item CLoadDBInfo::GetMailInfoParam(int countryid,int type)
{
	int key = type * 10 + countryid;
	MailInfoParam_Map_Iterator iter;
	iter = m_MailInfoParamList.find(key);
	if(iter == m_MailInfoParamList.end())
		return NULL;
	return iter->second;
}
//读取邮件参数表
void CLoadDBInfo::loadMailInfoParam()
{
	FILE *fp = 0;
	fp = fopen("configtable/MailInfoParam.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/MailInfoParam.csv失败。\n");
		return;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);
	char content[256] = {0};

	LPMailInfoParam_Item pItem = 0;
	int iLen = 0;
	//m_QuantityByRankList
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new MailInfoParam_Item;
		if(sscanf(buf,"%d,%d,%[^,],%[^,]",&pItem->CounryID,&pItem->Type,&pItem->Title,&content)<4)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
	
		iLen = strlen(content) -1;
		if(content[iLen] == '\n')
			content[iLen] = 0;
		StrReplace(content,pItem->Content,"\\n","\r\n");
		m_MailInfoParamList[pItem->Type * 10 + pItem->CounryID] = pItem;
		//printf("countryid:%d,type:%d,title:%s,content:%s\n",pItem->CounryID,pItem->Type,pItem->Title,pItem->Content);
	}
	fclose(fp);
}
void CLoadDBInfo::clearMailInfoParam()
{
	MailInfoParam_Map_Iterator iter;
	LPMailInfoParam_Item pDelItem = 0;
	for(iter = m_MailInfoParamList.begin();iter != m_MailInfoParamList.end();iter++)
	{
		if(pDelItem = iter->second)
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_MailInfoParamList.clear();
}
LPCardParam CLoadDBInfo::GetCardConfig(int cardid)
{
	CardParam_Map_Iterator iter;
	iter = m_CardConfigList.find(cardid);
	if(iter == m_CardConfigList.end())
		return NULL;
	return iter->second;
}
//读取卡参数表
int CLoadDBInfo::loadCardConfig()
{
	FILE *fp = 0;
	fp = fopen("configtable/CardConfig.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/CardConfig.csv失败。\n");
		return -1;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	LPCardParam pCardParamItem = 0;
	//
	int iPicture = 0;
	char efcetBuf[128]={0};
	int iOpenEnable = 0;
	int iKind = 0;
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pCardParamItem = new CardParam;
		memset(pCardParamItem,0,sizeof(CardParam));
		if(sscanf(buf,"%d,%[^,],%[^,],%d,%d,%d,%d,%d,%d,%d,%d",
			&pCardParamItem->CardID,&pCardParamItem->CardName,&efcetBuf,&iPicture,&pCardParamItem->Type,&iKind,
			&pCardParamItem->Price,&pCardParamItem->Value,&pCardParamItem->Time,&pCardParamItem->Postion,&iOpenEnable)<11)
		{
			delete pCardParamItem;
			pCardParamItem = 0;
			continue;
		}
		pCardParamItem->OpenEnabled = (iOpenEnable >0)?1:0;
		m_CardConfigList[pCardParamItem->CardID] = pCardParamItem;
	}
	fclose(fp);
	return 0;
}
void CLoadDBInfo::clearCardConfig()
{
	CardParam_Map_Iterator iter;
	LPCardParam pDelCard =0;
	for(iter = m_CardConfigList.begin();iter != m_CardConfigList.end();iter++)
	{
		if(pDelCard = iter->second)
		{
			delete pDelCard;
			pDelCard = 0;
		}
	}
	m_CardConfigList.clear();
}
LPPlayerInitArmyParam_Item CLoadDBInfo::GetPlayerInitArmyParam(int countryid,int jobid)
{
	int key = jobid*10+countryid;
	PlayerInitArmyParam_Map_Iterator iter;
	iter = m_PlayerInitArmyParamList.find(key);
	if (iter == m_PlayerInitArmyParamList.end())
	{
		return NULL;
	}
	return iter->second;
}
//读取玩家初始时的部队信息
void CLoadDBInfo::loadPlayerInitArmyParam()
{
	FILE *fp = 0;
	fp = fopen("configtable/PlayerInitArmyParam.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/PlayerInitArmyParam.csv失败。\n");
		return;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	LPPlayerInitArmyParam_Item pItem = 0;
	//m_PlayerInitArmyParamList
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new PlayerInitArmyParam_Item;
		if(sscanf(buf,"%d,%d,%d,%d,%d,%d,%d,%d,%d",&pItem->CountryID,&pItem->HeroJobID,&pItem->RaceID,
			&pItem->SoldierType,&pItem->WeaponType,&pItem->WeaponLevel,
			&pItem->DefendType,&pItem->DefendLevel,&pItem->Count)<9)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		m_PlayerInitArmyParamList[pItem->HeroJobID * 10 + pItem->CountryID] = pItem;
		/*
		printf("countryid:%d,Jobid:%d,RaceID:%d,SoldierType:%d,WeaponType:%d,Count:%d\n",
		pItem->CountryID,pItem->HeroJobID,pItem->RaceID,pItem->SoldierType,pItem->WeaponType,pItem->Count);
		*/
	}
	fclose(fp);
}
void CLoadDBInfo::clearPlayerInitArmyParam()
{
	PlayerInitArmyParam_Map_Iterator iter;
	LPPlayerInitArmyParam_Item pDelItem = 0;
	for (iter = m_PlayerInitArmyParamList.begin();iter != m_PlayerInitArmyParamList.end();iter++)
	{
		if ((pDelItem = iter->second) != NULL)
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_PlayerInitArmyParamList.clear();
}

//读取NPC反击配置表
void CLoadDBInfo::loadGMNPCParam()
{

	FILE *fp = 0;
	fp = fopen("configtable/GMNPCArmy.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/GMNPCArmy.csv失败。\n");
		return;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	LPGMCreateNPCParam_Item pItem = 0;
	//m_GMCreateNPCParamList 
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new GMCreateNPCParam_Item;
		if(sscanf(buf,"%d,%d,%d,%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d",
			&pItem->CountryID,&pItem->Level,&pItem->PlayerID,&pItem->OfficerID,
			&pItem->AmyName,&pItem->RaceID,&pItem->SoldierType,&pItem->WeaponType,
			&pItem->WeaponLevel,&pItem->DefendType,&pItem->DefendLevel,&pItem->SoldierCount,
			&pItem->AymyCount)<13)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		m_GMCreateNPCParamList.insert(GMCreateNPCParam_MultiMap_Pair(pItem->Level * 10 + pItem->CountryID,pItem));
	}
	fclose(fp);
}
void CLoadDBInfo::clearGMNPCParam()
{
	GMCreateNPCParam_MultiMap_Iterator iter;
	LPGMCreateNPCParam_Item pDelItem = 0;
	for (iter = m_GMCreateNPCParamList.begin();iter != m_GMCreateNPCParamList.end();iter++)
	{
		if (pDelItem = iter->second)
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_GMCreateNPCParamList.clear();
}

GMCreateNPCParam_MultiMap_Iterator CLoadDBInfo::GMCreateNPCParam_Lowerbound(int countryid,int level)
{
	int key = level *10 + countryid;
	return m_GMCreateNPCParamList.lower_bound(key);
}
GMCreateNPCParam_MultiMap_Iterator CLoadDBInfo::GMCreateNPCParam_Upperbound(int countryid,int level)
{
	int key = level *10 + countryid;
	return m_GMCreateNPCParamList.upper_bound(key);
}
//行动点参数
LPActionDotParam CLoadDBInfo::GetActionDotParam(int mapid,int type)
{
	ActionDotParam_Map_Iterator iter = m_ActionDotParamList.find(mapid);
	if (iter != m_ActionDotParamList.end())
	{
		return iter->second;
	}
	iter = m_ActionDotParamList.find(type);
	if (iter != m_ActionDotParamList.end())
	{
		return iter->second;
	}
	return NULL;
}
//读取行动点参数表
void CLoadDBInfo::loadActionDotParam()
{
	FILE *fp = 0;
	fp = fopen("configtable/ActionDotParam.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/ActionDotParam.csv失败。\n");
		return;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	LPActionDotParam pItem = 0;
	//
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new ActionDotParam;
		if(sscanf(buf,"%d,%d:%d,%d:%d,%d,%d,%d,%d,%d",&pItem->MapID,
			&pItem->Day_Start.hour,&pItem->Day_Start.min,&pItem->Day_End.hour,&pItem->Day_End.min,
			&pItem->AttackDot_Day,&pItem->RecoveryDot_Day,&pItem->AttackDot_Night,&pItem->RecoveryDot_Night,
			&pItem->Flag)<10)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		m_ActionDotParamList[pItem->MapID] = pItem;
	}
	fclose(fp);
}
void CLoadDBInfo::clearActionDotParam()
{
	ActionDotParam_Map_Iterator iter;
	LPActionDotParam pDelItem = 0;
	for (iter = m_ActionDotParamList.begin();iter != m_ActionDotParamList.end();iter++)
	{
		if (pDelItem = iter->second)
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_ActionDotParamList.clear();
}

bool CLoadDBInfo::OfficerRacematchSoldier(RACE_TYPE Race ,SOLDIER_TYPE soldiertype)
{
	OfficerRaceAttribute* pOfficerRaceAttr = GetOfficerRaceAttr(Race);

	/*
	int soldier_;
	int huntsman_;
	int prowl_;
	int churchman_;
	int rabbi_;
	int magician_;
	int marksman_;
	int trooper_;

	WARRIOR				=1 ,//战士
	SHOOTER				=2 ,//射手
	HUNTER				=3 ,//猎人
	WARLOCK				=6 ,//术士
	PRIEST				=4 ,//牧师
	MAGE				=5 ,//法师
	ROGUE				=7 ,//潜行者
	SABER				=8 ,//骑兵
	*/
	if (pOfficerRaceAttr->soldier_ == 1)
	{
		if (soldiertype == WARRIOR)
		{
			return true;
		}
	}
	if (pOfficerRaceAttr->huntsman_==1)
	{
		if (soldiertype == HUNTER)
		{
			return true;
		}

	}
	if (pOfficerRaceAttr->prowl_ == 1)
	{
		if (soldiertype == ROGUE)
		{
			return true;
		}
	}
	if (pOfficerRaceAttr->churchman_ ==1 )
	{
		if (soldiertype == PRIEST)
		{
			return true;
		}
	}
	if (pOfficerRaceAttr->rabbi_ == 1)
	{
		if (soldiertype == MAGE)
		{
			return true ;
		}
	}
	if (pOfficerRaceAttr->magician_ == 1)
	{
		if (soldiertype == WARLOCK)
		{
			return true;
		}
	}
	if (pOfficerRaceAttr->marksman_ == 1)
	{
		if (soldiertype == SHOOTER)
		{
			return true;
		}
	}
	if (pOfficerRaceAttr->trooper_ == 1)
	{
		if (soldiertype == SABER)
		{
			return true;
		}
	}
	if (soldiertype == 9 || soldiertype == 10) return true;
	return false;
}

//甲与职业是否匹配
bool CLoadDBInfo::DefendMacthSoldier(int defendid,int soldiertype)
{
	LPSoldierarmorconfig_Item pItem = GetSoldierarmorconfig(defendid);
	if(pItem == NULL)
		return false;
	/*
	WARRIOR				=1 ,//战士
	SHOOTER				=2 ,//射手
	HUNTER				=3 ,//猎人
	WARLOCK				=6 ,//术士
	PRIEST				=4 ,//牧师
	MAGE				=5 ,//法师
	ROGUE				=7 ,//潜行者
	SABER				=8 ,//骑兵
	*/
	bool bMatched = false;
	switch(soldiertype)
	{
	case WARRIOR:
		bMatched = pItem->Soldier;
		break;
	case SHOOTER:
		bMatched = pItem->Shooter;
		break;
	case PRIEST:
		bMatched = pItem->Churchman;
		break;
	case MAGE:
		bMatched = pItem->Rabbi;
		break;
	default:
		bMatched = false;
		break;
	}
	return bMatched;
}
PackStoreParam *CLoadDBInfo::GetPackStoreParam(int postion)
{
	PackStoreParam_Map_Iterator iter = m_PackStoreParamMap.find(postion);
	if (iter != m_PackStoreParamMap.end())
	{
		return iter->second;
	}
	return NULL;
}
int CLoadDBInfo::loadPackStoreParam()
{
	FILE *fp = 0;
	char buf[1024] = {0};
	fp = fopen("configtable/PackStoreParam.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/PackStoreParam.csv失败。\n");
		return 1;
	}

	fgets(buf,1024,fp);
	PackStoreParam* pNewNode = 0;
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pNewNode = new PackStoreParam;
		if(sscanf(buf,"%d,%d",&pNewNode->Postion,&pNewNode->IsOverlap) <2)
		{
			delete pNewNode;
			pNewNode = 0;
			continue;
		}
		m_PackStoreParamMap[pNewNode->Postion] = pNewNode;
	}
	fclose(fp);
	return 0;
}
void CLoadDBInfo::clearPackStoreParam()
{
	//背包是否重叠参数
	PackStoreParam* pPackStoreParam = 0;
	PackStoreParam_Map_Iterator packiter;
	for (packiter = m_PackStoreParamMap.begin();packiter != m_PackStoreParamMap.end();packiter++)
	{
		pPackStoreParam = packiter->second;
		if (pPackStoreParam)
		{
			delete pPackStoreParam;
			pPackStoreParam =0;
		}
	}
	m_PackStoreParamMap.clear();
}
LPGMCreateNPCParam_Item CLoadDBInfo::GetHiredNPCArmyParam(int countryid,int level)
{
	int key = level * 10 + countryid;
	GMCreateNPCParam_Map_Iterator iter = m_HiredNPCArmyList.find(key);
	if (iter == m_HiredNPCArmyList.end())
	{
		return NULL;
	}
	return iter->second;
}
//读取郡守备队参数表
void CLoadDBInfo::loadHiredNPCArmyParam()
{
	FILE *fp = 0;
	fp = fopen("configtable/HiredNPCArmy.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/HiredNPCArmy.csv失败。\n");
		return ;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	LPGMCreateNPCParam_Item pItem = 0;
	//m_HiredNPCArmyList
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new GMCreateNPCParam_Item;
		if(sscanf(buf,"%d,%d,%d,%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&pItem->CountryID,&pItem->Level,&pItem->PlayerID,&pItem->OfficerID,
			&pItem->AmyName,&pItem->RaceID,&pItem->SoldierType,&pItem->WeaponType,
			&pItem->WeaponLevel,&pItem->DefendType,&pItem->DefendLevel,&pItem->SoldierCount,
			&pItem->AymyCount,&pItem->Price)<14)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		m_HiredNPCArmyList.insert(GMCreateNPCParam_MultiMap_Pair(pItem->Level * 10 + pItem->CountryID,pItem));
	}
	fclose(fp);
}
void CLoadDBInfo::clearHiredNPCArmyParam()
{
	GMCreateNPCParam_Map_Iterator iter;
	LPGMCreateNPCParam_Item pDelItem = 0;
	for (iter = m_HiredNPCArmyList.begin();iter != m_HiredNPCArmyList.end();iter++)
	{
		if (pDelItem = iter->second)
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_HiredNPCArmyList.clear();
}
HeroEquipAttribute *CLoadDBInfo::GetHeroEquipAttribute(int itemid)
{
	HeroEquipAttribute_Map_Iterator iter = m_HeroEquipAttributeList.find(itemid);
	if (iter == m_HeroEquipAttributeList.end())
	{
		return NULL;
	}
	return iter->second;
}
//读取装备属性表
void CLoadDBInfo::loadEquipAttribute()
{
	FILE *fp = 0;
	fp = fopen("configtable/EquipAttribute.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/EquipAttribute.csv失败。\n");
		return ;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	HeroEquipAttribute *pItem = 0;
	//m_HeroEquipAttributeList
	int iSale = 0;
	int iPicture = 0;
	char jobs[32]={0};
	int jobIDs[6] = {0};
	int jobCount = 0;
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new HeroEquipAttribute;
		memset(pItem,0,sizeof(HeroEquipAttribute));
		if(sscanf(buf,"%d,%[^,],%d,%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&pItem->itemID,&pItem->name,&pItem->level,&iPicture,&jobs,&pItem->postion,
			&pItem->needStamina,&pItem->needPower,&pItem->needCelerity,&pItem->needWit,&pItem->stamina,&pItem->power,
			&pItem->celerity,&pItem->wit,&pItem->physicAttack,&pItem->physicRecovery,&pItem->magicAttack,&pItem->magicRecovery,
			&pItem->attackSpeed,&pItem->heroHP,&pItem->injureRate,&pItem->captureRate,&pItem->beInjureRate,&pItem->beCaptureRate,
			&pItem->forceRate,&pItem->setID,&pItem->skill,&pItem->skillLevel,&pItem->quality,&pItem->inlayNums,&iSale,&pItem->price)<31)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		pItem->isSale = (iSale >0)?1:0;
		if (jobs[0] == '0')
		{
			memset(pItem->jobIDs,1,6);
			//printf("只用于所有职业\n");
		}
		else
		{
			jobCount = sscanf(jobs,"%d|%d|%d|%d|%d|%d",&jobIDs[0],&jobIDs[1],&jobIDs[2],&jobIDs[3],&jobIDs[4],&jobIDs[5]);
			for (int i=0;i<jobCount;i++)
			{
				pItem->jobIDs[jobIDs[i]] = 1;
			}
		}
		m_HeroEquipAttributeList[pItem->itemID] = pItem;
		insertEquipAttrScalePosLevelMap(pItem);
		//printf("id:%d,pos:%d,price:%d\n",pItem->itemID,pItem->postion,pItem->price);
	}
	fclose(fp);
}

void CLoadDBInfo::insertEquipAttrScalePosLevelMap(HeroEquipAttribute *heroEquipAttri)
{
	HeroEquipAttribute_Multimap_Map *equipPosMap = 0;
	HeroEquipAttribute_Multimap_Map_Map_Iterator scaleIter = heroEquipScalePosLevelMap.find(heroEquipAttri->quality);//根据品质查询
	if (scaleIter ==  heroEquipScalePosLevelMap.end())
	{
		equipPosMap = new HeroEquipAttribute_Multimap_Map;
		heroEquipScalePosLevelMap.insert(HeroEquipAttribute_Multimap_Map_Map_Pair(heroEquipAttri->quality,equipPosMap));
	}
	else
	{
		equipPosMap = scaleIter->second;
	}
	HeroEquipAttribute_Multimap *equipLevelMap = 0;
	HeroEquipAttribute_Multimap_Map_Iterator posIter = equipPosMap->find(heroEquipAttri->postion);
	if (posIter == equipPosMap->end())
	{
		equipLevelMap = new HeroEquipAttribute_Multimap;
		equipPosMap->insert(HeroEquipAttribute_Multimap_Map_Pair(heroEquipAttri->postion,equipLevelMap));
	}
	else
	{
		equipLevelMap = posIter->second;
	}
	equipLevelMap->insert(HeroEquipAttribute_Multimap_Pair(heroEquipAttri->level,heroEquipAttri));
}
void CLoadDBInfo::clearEquipAttribute()
{
	//删除品质->位置->级别列表
	HeroEquipAttribute_Multimap_Map_Map_Iterator scaleIter;
	HeroEquipAttribute_Multimap_Map_Iterator posIter;
	HeroEquipAttribute_Multimap_Map *equipPosMap = 0;
	HeroEquipAttribute_Multimap *equipLevelMap = 0;
	for (scaleIter = heroEquipScalePosLevelMap.begin();scaleIter != heroEquipScalePosLevelMap.end();scaleIter++)
	{
		equipPosMap = scaleIter->second;
		if (equipPosMap == NULL)
		{
			continue;
		}
		for (posIter = equipPosMap->begin();posIter != equipPosMap->end();posIter++)
		{
			if ((equipLevelMap = posIter->second))
			{
				delete equipLevelMap;
				equipLevelMap = 0;
			}
		}
		delete equipPosMap;
		equipPosMap = 0;
	}
	heroEquipScalePosLevelMap.clear();
	HeroEquipAttribute_Map_Iterator iter;
	HeroEquipAttribute *pDelItem = 0;
	for (iter = m_HeroEquipAttributeList.begin();iter != m_HeroEquipAttributeList.end();iter++)
	{
		if ((pDelItem = iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_HeroEquipAttributeList.clear();
}
HeroEquipAttribute *CLoadDBInfo::GetInlayResultAttribute(int itemid,int postion)
{
	HeroEquipAttribute_Map_Iterator iter;
	int key = itemid * 100 + postion;
	iter = m_InlayResultList.find(key);
	if (iter == m_InlayResultList.end())
	{
		return NULL;
	}
	return iter->second;
}
HeroEquipAttribute *CLoadDBInfo::getOneEquip(int scale,int postion,int level)
{
	if (level <=0)
	{
		return NULL;
	}
	HeroEquipAttribute_Multimap_Map_Map_Iterator scaleIter;
	scaleIter = heroEquipScalePosLevelMap.find(scale);
	if (scaleIter == heroEquipScalePosLevelMap.end())
	{
		return NULL;
	}
	HeroEquipAttribute_Multimap_Map *equipPosMap = scaleIter->second;
	if (equipPosMap == NULL)
	{
		return NULL;
	}
	HeroEquipAttribute_Multimap_Map_Iterator posIter = equipPosMap->find(postion);
	if (posIter == equipPosMap->end())
	{
		return NULL;
	}
	HeroEquipAttribute_Multimap *equipLevelMap = posIter->second;
	if (equipLevelMap == NULL)
	{
		return NULL;
	}
	HeroEquipAttribute_Multimap_Iterator lowerIter,upperIter;
	while(level >0)
	{
		lowerIter = equipLevelMap->lower_bound(level);
		upperIter = equipLevelMap->upper_bound(level);
		if (lowerIter != upperIter)
		{
			break;
		}
		--level;
	}
	if (lowerIter == upperIter)
	{
		return NULL;
	}
	HeroEquipAttribute *tmpEquip[100] = {0};
	int count = 0;
	for (;lowerIter != upperIter;lowerIter++)
	{
		tmpEquip[count] = lowerIter->second;
		count++;
	}
	count = rand()%count;
	return tmpEquip[count];
}
//读取精练效果表
void CLoadDBInfo::loadInlayResultAttribute()
{
	FILE *fp = 0;
	fp = fopen("configtable/RefiningResult.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/RefiningResult.csv失败。\n");
		return ;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	HeroEquipAttribute *pItem = 0;
	//m_InlayResultList

	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new HeroEquipAttribute;
		memset(pItem,0,sizeof(HeroEquipAttribute));
		if(sscanf(buf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&pItem->itemID,&pItem->postion,&pItem->stamina,&pItem->power,&pItem->celerity,
			&pItem->wit,&pItem->physicAttack,&pItem->physicRecovery,&pItem->magicAttack,&pItem->attackSpeed,
			&pItem->magicRecovery,&pItem->heroHP,&pItem->injureRate,&pItem->captureRate,&pItem->beInjureRate,
			&pItem->beCaptureRate,&pItem->forceRate,&pItem->skill,&pItem->skillLevel,&pItem->price)<19)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		m_InlayResultList[pItem->itemID*100 + pItem->postion] = pItem;
	}
	fclose(fp);
}
void CLoadDBInfo::clearInlayResultAttribute()
{
	HeroEquipAttribute_Map_Iterator iter;
	HeroEquipAttribute *pDelItem = 0;
	for (iter = m_InlayResultList.begin();iter != m_InlayResultList.end();iter++)
	{
		if ((pDelItem = iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_InlayResultList.clear();
}
RefiningProbability *CLoadDBInfo::GetRefiningProbability(int refiningtime)
{
	RefiningProbability_Map_Iterator iter = m_RefiningProbabilityMap.find(refiningtime);
	if (iter == m_RefiningProbabilityMap.end())
	{
		return NULL;
	}
	return iter->second;
}
//精练成功概率表
void CLoadDBInfo::loadRefiningProbability()
{
	FILE *fp = 0;
	fp = fopen("configtable/RefiningProbability.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/RefiningProbability.csv失败。\n");
		return ;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	RefiningProbability *pItem = 0;
	//m_RefiningProbabilityMap

	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new RefiningProbability;
		memset(pItem,0,sizeof(RefiningProbability));
		if(sscanf(buf,"%d,%d,%d",&pItem->refiningTime,&pItem->succeedRate,&pItem->priceRate)<3)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		m_RefiningProbabilityMap[pItem->refiningTime] = pItem;
	}
	fclose(fp);
}
void CLoadDBInfo::clearRefiningProbability()
{
	RefiningProbability_Map_Iterator iter;
	RefiningProbability *pDelItem = 0;
	for (iter = m_RefiningProbabilityMap.begin();iter != m_RefiningProbabilityMap.end();iter++)
	{
		if((pDelItem = iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_RefiningProbabilityMap.clear();
}
//读取技能描述表
void CLoadDBInfo::loadSkillDetail(void){
	FILE *fp = 0;
	fp = fopen("configtable/skilldetail.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/skilldetail.csv失败。\n");
		return ;
	}

	char readBuffer[1024]= {0};
	fgets(readBuffer,1024,fp);

	SkillDetail *skillDetailData = 0;
	 
	char skillName[50];
	char occupationLimit[50];
	char selfOccupationLimit[50];
	char targetOccupationLimit[50];
	int occupations[6] = {0};
	int readNum;
	int iscanLearn;
	while(!feof(fp))
	{
		if(fgets(readBuffer,1024,fp) == NULL)
			break;
		skillDetailData = new SkillDetail;
		if(sscanf(readBuffer,"%d,%[^,],%d,%d,%[^,],%d,%d,%d,%d,%d,%f,%[^,],%[^,],%d",
			&skillDetailData->skillID,skillName,&skillDetailData->skillType,&iscanLearn,occupationLimit,
			&skillDetailData->levelLimit,&skillDetailData->level,&skillDetailData->consumeSkillDot,
			&skillDetailData->odds,&skillDetailData->effectType,&skillDetailData->effectValue,selfOccupationLimit,
			targetOccupationLimit,&skillDetailData->landLimit)<14)
		{
			delete skillDetailData;
			skillDetailData = NULL;
			continue;
		}
		if (iscanLearn > 0){
			skillDetailData->iscanLearn = true;
		}
		else
		{
			skillDetailData->iscanLearn = false;
		}
		readNum = sscanf(occupationLimit,"%d|%d|%d|%d|%d|%d",&occupations[0],&occupations[1],
			&occupations[2],&occupations[3],&occupations[4],&occupations[5]);
	
		for (int i=0;i<readNum;i++){
			skillDetailData->occupationLimit.insert(std::set<OFFICER_OCCUPATION_TYPE>::value_type(occupations[i]));
		}
		readNum = sscanf(selfOccupationLimit,"%d|%d|%d|%d|%d|%d",&occupations[0],&occupations[1],
			&occupations[2],&occupations[3],&occupations[4],&occupations[5]);
		for (int i=0;i<readNum;i++){
			skillDetailData->selfOccupationLimit.insert(std::set<SOLDIER_TYPE>::value_type(occupations[i]));
		}
		readNum = sscanf(targetOccupationLimit,"%d|%d|%d|%d|%d|%d",&occupations[0],&occupations[1],
			&occupations[2],&occupations[3],&occupations[4],&occupations[5]);

		for (int i=0;i<readNum;i++){
			skillDetailData->targetOccupationLimit.insert(std::set<SOLDIER_TYPE>::value_type(occupations[i]));
		}
		skillDetailMap_[skillDetailData->skillID][skillDetailData->level] = skillDetailData;
	}
	fclose(fp);
}
//释放技能描述表
void CLoadDBInfo::clearSkillDetail(void){
	SkillDetail_Map_Iterator iter;
	std::map<unsigned int ,  SkillDetail*>::iterator subIter;
	for (iter = skillDetailMap_.begin(); iter != skillDetailMap_.end() ; ++iter){
		for(subIter = iter->second.begin(); subIter!= iter->second.end() ; ++subIter){
			delete subIter->second;
			subIter->second = NULL;
		}
	}
	skillDetailMap_.clear();
}

SkillDetail * CLoadDBInfo::getSkillDetail(SKILL_TYPE skillID , int skillLevel) {
	SkillDetail_Map_Iterator iter;
	std::map<unsigned int ,  SkillDetail*>::iterator subIter;
	iter = skillDetailMap_.find(skillID);
	if (iter == skillDetailMap_.end()){
		return NULL;
	}
	subIter = iter->second.find(skillLevel);
	if (subIter == iter->second.end()){
		return NULL;
	}
	return subIter->second;
}

//读取武器属性表
void CLoadDBInfo::loadWeaponAttribute()
{
	FILE *fp = 0;
	fp = fopen("configtable/WeaponAttribute.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/WeaponAttribute.csv失败。\n");
		return ;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	WeaponAttribute *pItem = 0;	//
	char raceInfo[100];
	int race[10];
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new WeaponAttribute;
		memset(raceInfo,0,100);
		if(sscanf(buf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%[^,],%d,%d,%d,%d,%d",
			&pItem->weaponType,&pItem->weaponID,&pItem->cost,
			&pItem->attack,&pItem->hurt,&pItem->singleHands,
			&pItem->soldier,&pItem->exp,&pItem->level,raceInfo ,&pItem->addAttack ,
			&pItem->attackFreq,&pItem->trajectorySpeed,&pItem->attackType,&pItem->actionBout)<15)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		int readNum = sscanf(raceInfo,"%d|%d|%d|%d|%d|%d|%d|%d|%d|%d",&race[0],&race[1],
			&race[2],&race[3],&race[4],&race[5],&race[6],&race[7],&race[8],&race[9]);

		for (int i=0;i<readNum;i++){
			pItem->race[race[i]] = race[i];
		}
		m_weaponAttributeMap[pItem->weaponID] = pItem;
	}
	fclose(fp);
}
void CLoadDBInfo::clearWeaponAttribute()
{
	WeaponAttribute_Map_Iterator iter;
	WeaponAttribute *pDelItem = 0;
	for (iter = m_weaponAttributeMap.begin();iter != m_weaponAttributeMap.end();iter++)
	{
		if ((pDelItem = iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_weaponAttributeMap.clear();
}

//读取防具属性表
void CLoadDBInfo::loadDefendAttribute()
{
	FILE *fp = 0;
	fp = fopen("configtable/DefendAttribute.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/DefendAttribute.csv失败。\n");
		return ;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	DefendAttribute *pItem = 0;	//

	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new DefendAttribute;
		memset(pItem,0,sizeof(DefendAttribute));
		if(sscanf(buf,"%d,%d,%d,%d,%d,%d,%d",
			&pItem->defendID,&pItem->defendType,&pItem->cost,
			&pItem->decRate,&pItem->physiceRec,&pItem->magicRec,&pItem->level)<7)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		m_defendAttributeMap[pItem->defendID] = pItem;
	}
	fclose(fp);
}
void CLoadDBInfo::clearDefendAttribute()
{
	DefendAttribute_Map_Iterator iter;
	DefendAttribute *pDelItem = 0;
	for (iter = m_defendAttributeMap.begin();iter != m_defendAttributeMap.end();iter++)
	{
		if ((pDelItem = iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_defendAttributeMap.clear();
}
bool CLoadDBInfo::hasSpecialitySkill(PlayerSkillTable *skills,int skillnum,int type,int level)
{
	if (skills == NULL)
	{
		return false;
	}
	bool bHasSkill = false;
	SkillDetail *pSkillDetail = NULL;
	for (int i=0;i<skillnum;i++)
	{
		pSkillDetail = getSkillDetail((SKILL_TYPE)skills[i].skillid_,skills[i].skilllevel_);
		if (pSkillDetail == NULL)
		{
			continue;
		}
		if (pSkillDetail->effectType == type && pSkillDetail->effectValue >=level)
		{
			bHasSkill = true;
			break;
		}
	}
	return bHasSkill;
}
//根据职业ID 获取该职业默认的技能参数
OfficerInitSkill_MultiMap_iterator CLoadDBInfo::getOfficerInitSkill_Lowerbound(int jobid)
{
	return officerInitSkillMultiMap.lower_bound(jobid);
}
OfficerInitSkill_MultiMap_iterator CLoadDBInfo::getOfficerInitSkill_Upperbound(int jobid)
{
	return officerInitSkillMultiMap.upper_bound(jobid);
}
//读取英雄初始技能参数表
void CLoadDBInfo::loadOfficerInitSkill()
{
	FILE *fp = 0;
	fp = fopen("configtable/OfficerInitSkill.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/OfficerInitSkill.csv失败。\n");
		return ;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	OfficerInitSkill *pItem = 0;	//

	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new OfficerInitSkill;
		memset(pItem,0,sizeof(OfficerInitSkill));
		if(sscanf(buf,"%d,%d,%d",
			&pItem->jobID,&pItem->skill,&pItem->skillLevel)<3)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		officerInitSkillMultiMap.insert(OfficerInitSkill_MultiMap_Pair(pItem->jobID,pItem));
	}
	fclose(fp);
}
void CLoadDBInfo::clearOfficerInitSkill()
{
	OfficerInitSkill_MultiMap_iterator iter;
	OfficerInitSkill * delOfficerInitSkill = 0;
	for (iter = officerInitSkillMultiMap.begin();iter != officerInitSkillMultiMap.end();iter++)
	{
		if((delOfficerInitSkill = iter->second))
		{
			delete delOfficerInitSkill;
			delOfficerInitSkill = 0;
		}
	}
	officerInitSkillMultiMap.clear();
}
//读取打开宝箱时，装备的掉落概率参数
void CLoadDBInfo::loadDropEquipProbability()
{
	memset(scaleProbability,0,sizeof(int)*100);
	memset(postionProbability,0,sizeof(int)*100);
	FILE *fp = 0;
	//品质概率
	fp = fopen("configtable/DropScaleProbability.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/DropScaleProbability.csv失败。\n");
		return ;
	}

	char buf[128]= {0};
	fgets(buf,128,fp);
	int id = 0;
	int dropOdds = 0;
	int index = 0;
	while(!feof(fp))
	{
		if(fgets(buf,128,fp) == NULL)
			break;
		if(sscanf(buf,"%d,%d",&id,&dropOdds)<2)
		{
			continue;
		}
		if (index + dropOdds >100)
		{
			printf("品质掉率概率之后大于了100.");
			dropOdds = index + dropOdds -100;
		}
		for (int i=0;i<dropOdds;i++)
		{
			scaleProbability[index++] = id;
		}
	}
	fclose(fp);
	//位置概率
	fp = fopen("configtable/DropPosProbability.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/DropPosProbability.csv失败。\n");
		return ;
	}
	index = 0;
	while(!feof(fp))
	{
		if(fgets(buf,128,fp) == NULL)
			break;
		if(sscanf(buf,"%d,%d",&id,&dropOdds)<2)
		{
			continue;
		}
		if (index + dropOdds >100)
		{
			printf("位置掉率概率表配置有误，总概率之后大于了100。");
			dropOdds = index + dropOdds -100;
		}
		for (int i=0;i<dropOdds;i++)
		{
			postionProbability[index++] = id;
		}
	}
	fclose(fp);
}
//获取品质（概率）
int CLoadDBInfo::getEquipScaleByProbability()
{
	return scaleProbability[rand()%100];
}
//获取位置（概率）
int CLoadDBInfo::getEquipPostionByProbability()
{
	return postionProbability[rand()%100];
}
ArmyBrand * CLoadDBInfo::getArmyBrandParamByID(int brandid)
{
	ArmyBrand_Map_Iterator iter = armyBrandMap.find(brandid);
	if (iter == armyBrandMap.end())
	{
		return NULL;
	}
	return iter->second;
}
ArmyBrand * CLoadDBInfo::getArmyBrandParamByLevel(int level)
{
	ArmyBrand_Map_Iterator iter = armyBrandLevelMap.find(level);
	if (iter == armyBrandLevelMap.end())
	{
		return NULL;
	}
	return iter->second;
}
//读取军牌兑换参数表
void CLoadDBInfo::loadArmyBrandParam()
{	
	FILE *fp = 0;
	fp = fopen("configtable/ArmyBrandParam.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/ArmyBrandParam.csv失败。\n");
		return ;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	ArmyBrand *pNewArmyBrand = 0;	//
	int id = 0;
	int level = 0;
	char convert[256] = {0};
	int picture = 0;
	char name[64] = {0};
	int price = 0;

	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		if(sscanf(buf,"%d,%d,%d,%[^,],%[^,],%d",
			&id,&level,&picture,&name,&convert,&price)<5)
		{
			continue;
		}
		pNewArmyBrand = new ArmyBrand(id);
		pNewArmyBrand->setLevel(level);
		pNewArmyBrand->setPrice(price);
		pNewArmyBrand->setConvert(convert);
		armyBrandMap[id] = pNewArmyBrand;
		armyBrandLevelMap[level] = pNewArmyBrand;
		memset(convert,0,256);
	}
	fclose(fp);
}
void CLoadDBInfo::clearArmyBrandParam()
{
	ArmyBrand_Map_Iterator iter;
	ArmyBrand *armyBrand = 0;
	for (iter = armyBrandMap.begin();iter != armyBrandMap.end();iter++)
	{
		if ((armyBrand = iter->second))
		{
			delete armyBrand;
			armyBrand = 0;
		}
	}
	armyBrandMap.clear();
	armyBrandLevelMap.clear();
}
void CLoadDBInfo::loadPublicBuildParam(){
	FILE *fp = 0;
	fp = fopen("configtable/PublicBuildParamtable.csv","r");
	if(fp == NULL){
		printf("打开PublicBuildParamtable.csv文件失败！\n");
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
			continue;
		}
		publicBuildParam_[buildParamPtr->pbuildingkind_][buildParamPtr->level_] = buildParamPtr;
	}
	fclose(fp);
}

//加载英雄成长系数几率表
void CLoadDBInfo::loadHeroGroupRateCapability()
{
	FILE *fp = 0;
	fp = fopen("configtable/HeroCapability.csv","r");
	if(fp == NULL){
		printf("打开configtable/HeroCapability.csv文件失败！\n");
		return;
	}
	char fileBuf[1024]= {0};
	fgets(fileBuf,1024,fp);
	int iMathematics = 0;
	int iProbability = 0;
	int iTotal = 0;

	while(!feof(fp))
	{
		if(fgets(fileBuf,1024,fp) == NULL)
			break;
		if(sscanf(fileBuf,"%d,%d",&iMathematics,&iProbability)< 2)
		{
			continue;
		}
		iTotal += iProbability;
		heroGroupRateMap[iTotal] = iMathematics;
	}
	fclose(fp);
}
//获取成长系数
int CLoadDBInfo::getHeroGroupRate()
{
	int iGroupRate = 0;
	std::map<int,int>::const_iterator iter;
	int iCapability = rand()%10000;
	for (iter = heroGroupRateMap.begin();iter != heroGroupRateMap.end();iter++)
	{
		if (iCapability < iter->first)
		{
			iGroupRate = iter->second;
			break;
		}
	}
	return iGroupRate;
}
//读取科研配置表
void CLoadDBInfo::loadResearchConfigure()
{
	FILE *fp = 0;
	fp = fopen("configtable/ResearchConfigure.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/ResearchConfigure.csv失败。\n");
		return ;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	ResearchConfigure* pItem = 0;
	//m_ResearchConfigureMap
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new ResearchConfigure;
		if(sscanf(buf,"%d,%d,%d,%d,%d,%d",
			&pItem->itemid_,&pItem->level_,&pItem->type_,&pItem->money_,
			&pItem->science_,&pItem->completetimes_)<6)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		m_ResearchConfigureMap[pItem->itemid_] = pItem;
	}
	fclose(fp);
}
void CLoadDBInfo::clearResearchConfigure()
{
	ResearchConfigure_Map_Iterator iter;
	ResearchConfigure *pDelItem = 0;
	for (iter = m_ResearchConfigureMap.begin();iter != m_ResearchConfigureMap.end();iter++)
	{
		if ((pDelItem = iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_ResearchConfigureMap.clear();
}

//读取生产配置表
void CLoadDBInfo::loadProduceConfigure()
{
	FILE *fp = 0;
	fp = fopen("configtable/ProduceConfigure.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/ProduceConfigure.csv失败。\n");
		return ;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	ProduceConfigure* pItem = 0;
	//m_ProduceConfigureMap
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new ProduceConfigure;
		if(sscanf(buf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&pItem->itemid_,&pItem->weanpontypeid_,&pItem->itemtype_,&pItem->level_,&pItem->foodcount_,
			&pItem->woodcount_,&pItem->ironcount_,&pItem->skincount_,&pItem->redstonecount_,&pItem->yellowstonecount_,
			&pItem->greenstonecount_,&pItem->backironcount_,&pItem->money_,&pItem->science_,&pItem->completetimes_)<15)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		m_ProduceConfigureMap[pItem->weanpontypeid_] = pItem;
	}
	fclose(fp);
}
void CLoadDBInfo::clearProduceConfigure()
{
	ProduceConfigure_Map_Iterator iter;
	ProduceConfigure *pDelItem = 0;
	for (iter = m_ProduceConfigureMap.begin();iter != m_ProduceConfigureMap.end();iter++)
	{
		if ((pDelItem = iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_ProduceConfigureMap.clear();
}

//读取建筑配置表
void CLoadDBInfo::loadConstructionConfigure()
{
	FILE *fp = 0;
	fp = fopen("configtable/ConstructionConfigure.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/ConstructionConfigure.csv失败。\n");
		return ;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	ConstructionConfigure* pItem = 0;
	//m_ConstructionConfigureMultimap
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new ConstructionConfigure;
		if(sscanf(buf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&pItem->constructionid_,&pItem->level_,&pItem->completetimes_,&pItem->money_,&pItem->foodcount_,
			&pItem->woodcount_,&pItem->ironcount_,&pItem->skincount_,&pItem->science_,&pItem->modify_)<10)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		m_ConstructionConfigureMultimap.insert(ConstructionConfigure_MultiMap_Pair(pItem->constructionid_,pItem));
	}
	fclose(fp);
}
void CLoadDBInfo::clearConstructionConfigure()
{
	ConstructionConfigure_Multimap_Iterator iter;
	ConstructionConfigure *pDelItem = 0;
	for (iter = m_ConstructionConfigureMultimap.begin();iter != m_ConstructionConfigureMultimap.end();iter++)
	{
		if ((pDelItem = iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_ConstructionConfigureMultimap.clear();
}

//读取城市信息表
void CLoadDBInfo::loadGameCityInfo()
{
	FILE *fp = 0;
	fp = fopen("configtable/GameCityInfo.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/GameCityInfo.csv失败。\n");
		return ;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	GameCityInfo* pItem = 0;
	//m_GameCityInfoMap
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new GameCityInfo;
		if(sscanf(buf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&pItem->cityid_,&pItem->mapid_,&pItem->foodcount_,&pItem->woodcount_,
			&pItem->ironcount_,&pItem->skincount_,&pItem->x_,&pItem->y_,
			&pItem->human_,&pItem->mannikin_,&pItem->fairy_,&pItem->dandiprat_,
			&pItem->halfanimal_,&pItem->cattle_,&pItem->blackfairy_,&pItem->decpesrson_)<16)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		m_GameCityInfoMap[pItem->cityid_] = pItem;
	}
	fclose(fp);
}
void CLoadDBInfo::clearGameCityInfo()
{
	GameCityInfo_Map_Iterator iter;
	GameCityInfo *pDelItem = 0;
	for (iter = m_GameCityInfoMap.begin();iter != m_GameCityInfoMap.end();iter++)
	{
		if ((pDelItem = iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_GameCityInfoMap.clear();
}

//获取广播消息参数
BroadMsgParam * CLoadDBInfo::getBroadMsgParam(int cmd)
{
	BroadMsgParam_Map_Iterator iter;
	iter = broadMsgParamMap.find(cmd);
	if (iter == broadMsgParamMap.end())
	{
		return NULL;
	}
	return iter->second;
}
//加载广播消息参数表
void CLoadDBInfo::loadBroadMsgParam()
{
	FILE *fp = 0;
	fp = fopen("configtable/BroadMsgParam.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/BroadMsgParam.csv失败。\n");
		return ;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	BroadMsgParam* pItem = 0;
	//broadMsgParamMap
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new BroadMsgParam;
		if(sscanf(buf,"%d,%d,%d",&pItem->cmdID,&pItem->minLevel,&pItem->maxLevel)<3)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		if (pItem->maxLevel <=0 || pItem->maxLevel <pItem->minLevel)
		{
			pItem->maxLevel = 9999;
		}
		broadMsgParamMap[pItem->cmdID] = pItem;
	}
	fclose(fp);
}
void CLoadDBInfo::clearBroadMsgParam()
{

	BroadMsgParam_Map_Iterator iter;
	BroadMsgParam *pDelItem = 0;
	for (iter = broadMsgParamMap.begin();iter != broadMsgParamMap.end();iter++)
	{
		if ((pDelItem = iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	broadMsgParamMap.clear();
}

//获取任务参数
DayTask_DetailInfo * CLoadDBInfo::getDayTaskParam(int taskid)
{
	DayTaskParam_Map_Iterator iter;
	iter = dayTaskParamMap.find(taskid);
	if (iter == dayTaskParamMap.end())
	{
		return NULL;
	}
	return iter->second;
}

//加载日常任务列表
void CLoadDBInfo::loadDayTaskParam()
{
	FILE *fp = 0;
	fp = fopen("configtable/TaskBaseTable.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/TaskBaseTable.csv失败。\n");
		return;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	TaskBaseTable taskParam;

	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		if(sscanf(buf,"%d,%[^,],%d,%d,%d,%d,%d,%d,%[^,],%[^,],%d,%d",
			&taskParam.taskid_,&taskParam.taskname_,&taskParam.tasktype_,&taskParam.tasklevel_,&taskParam.playerlevel_,
			&taskParam.playerrank_,&taskParam.taskitem_,&taskParam.prvtaskid_,&taskParam.bonus_,&taskParam.contest_,
			&taskParam.mailtype_,&taskParam.days)<11)
		{
			printf("任务参数配置有误：任务ID：%d\n",taskParam.taskid_);
			continue;
		}

		if(taskParam.tasktype_ == 6/*ttDayTask*/)
		{
			//日常任务,种类:类型:ItemID:Number:Level:联盟MAPID:部落MAPID
			DayTask_DetailInfo *pDayTaskInfo = new DayTask_DetailInfo;
			if (sscanf(taskParam.contest_,"%d:%d:%d:%d:%d:%d:%d",
				&pDayTaskInfo->kindType,&pDayTaskInfo->itemType,&pDayTaskInfo->itemID,&pDayTaskInfo->number,
				&pDayTaskInfo->itemLevel,&pDayTaskInfo->maps[1],&pDayTaskInfo->maps[2]) <7)
			{
				printf("日常任务配置有误：任务ID：%d\n",taskParam.taskid_);
				delete pDayTaskInfo;
				pDayTaskInfo = 0;
				return ;  //日常任务数据错误
			}
			pDayTaskInfo->days = taskParam.days;
			pDayTaskInfo->endTime = 0;
			dayTaskParamMap[taskParam.taskid_] = pDayTaskInfo;
		}
	}
	fclose(fp);
}
void CLoadDBInfo::clearDayTaskParam()
{
	DayTaskParam_Map_Iterator iter;
	DayTask_DetailInfo *pDelItem = 0;
	for (iter = dayTaskParamMap.begin();iter != dayTaskParamMap.end();iter++)
	{
		if ((pDelItem = iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	dayTaskParamMap.clear();
}
//根据级别获取军团升级参数
ConsortiaUpdateParam* CLoadDBInfo::getConsortiaUpdateParam(int level)
{
	ConsortiaUpdateParam_Map_Iterator iter;
	iter = consortiaUpdateParamMap.find(level);
	if (iter == consortiaUpdateParamMap.end())
	{
		return NULL;
	}
	return iter->second;
}
//读取军团升级参数表
void CLoadDBInfo::loadConsortiaUpdateParam()
{
	FILE *fp = 0;
	fp = fopen("configtable/ConsortiaUpdateParam.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/ConsortiaUpdateParam.csv失败。\n");
		return;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	ConsortiaUpdateParam* pItem = 0;
	int iRoyal = 0;
	//consortiaUpdateParamMap
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new ConsortiaUpdateParam;
		if(sscanf(buf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&pItem->level,&iRoyal,&pItem->needMoney,&pItem->needGODian,
			&pItem->needExp,&pItem->needCredit,&pItem->maxUserNum,&pItem->baseRate.foodRate,
			&pItem->baseRate.woodRate,&pItem->baseRate.ironRate,&pItem->baseRate.skinRate,&pItem->extraUserCount,
			&pItem->extraRate.foodRate,&pItem->extraRate.woodRate,&pItem->extraRate.ironRate,&pItem->extraRate.skinRate)<16)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		pItem->isRoyal = (iRoyal >0)?true:false;
		consortiaUpdateParamMap[pItem->level] = pItem;
	}
	fclose(fp);
}
void CLoadDBInfo::clearConsortiaUpdateParam()
{
	ConsortiaUpdateParam_Map_Iterator iter;
	ConsortiaUpdateParam *pDelItem = 0;
	for (iter = consortiaUpdateParamMap.begin();iter != consortiaUpdateParamMap.end();iter++)
	{
		if ((pDelItem = iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	consortiaUpdateParamMap.clear();
}
