#include "ServerManager.h"
#include "commInterface.h"
#include "IniFile.h"
#include "log.hpp"
#include "equipment.h"
#include "LoadDBInfo.h"
#include <set>
#include "DataServer.h"
#pragma warning(disable:4100)
#pragma warning(disable:4189)
#pragma warning(disable:4127)
#pragma warning(disable:4244)
#pragma warning(disable:4389)
#pragma warning(disable:4706)
DB_Proxy								g_Proxy;

COtherManager::COtherManager()
{
	m_lMapID = SERVER_BOUND_MAP_OTHER;
	memset(m_HeroAttri, 0, sizeof(m_HeroAttri));
	memset(m_MapInfo, 0, sizeof(int)*BIG_MAP_SIZE*BIG_MAP_SIZE);
	memset(m_MapState, 0, sizeof(MapInfoTable)*BIG_MAP_SIZE);
	memset(m_MapWarm, 0, sizeof(int)*BIG_MAP_SIZE);
	m_mapMapInfo.clear();
	m_mapRecruitingOfficer.clear();
	m_mapHeroName.clear();
	m_mapRaceParam.clear();
	m_mapJobParam.clear();
	m_mapRecruitMoney.clear();
	m_mapSpecialHero.clear();
	memset(m_MapWarmInfo,0,sizeof(MapWarmInfo) * BIG_MAP_SIZE);
}
COtherManager::~COtherManager()
{
	clearBinConfig();
	clearInitConstructions();
}
void COtherManager::SetMapID(long lMapID,DB_Proxy *pDBProxy)
{ 
	m_lMapID = SERVER_BOUND_MAP_OTHER; 
	static int flag = 0;
	if (flag == 0)
	{
		char * buf = 0;
		int count = 0;
		MapInfoTable MapInfo;
		if (pDBProxy->db_select_all(MapInfo, buf, count) == DB_Proxy::DB_SUCCEED)
		{
			flag = 1;
			MapInfoTable * pMapInfo = (MapInfoTable *)buf;
			int iItem = count/sizeof(MapInfoTable);

			int MapID = 0;
			for (int n = 0; n < iItem; n++)
			{
				MapID = pMapInfo[n].mapid_-1001; 
				m_MapState[MapID].mapid_ = pMapInfo[n].mapid_;
				m_MapState[MapID].countryid_ = pMapInfo[n].countryid_;
				m_MapState[MapID].state_ = pMapInfo[n].state_;
				if(m_MapState[MapID].countryid_ == 1 && m_MapState[MapID].state_ == 100)
				{
					m_UnionSafeMap.push_back(pMapInfo[n].mapid_);
				}
				else if(m_MapState[MapID].countryid_ == 2 && m_MapState[MapID].state_ == 100)
				{
					m_TribeSafeMap.push_back(pMapInfo[n].mapid_);
				}
			}
			if (buf != NULL)
			{
				delete [] buf;
				buf = 0;
			}
			if(m_UnionSafeMap.size() <=0 || m_TribeSafeMap.size() <=0)
			{
				LOG_TRACE_EX(false,("没有新手村联盟：%d,部落：%d",m_UnionSafeMap.size(),m_TribeSafeMap.size()));
				return;
			}
		}
		buf = 0;
	}
	/*
	if(lMapID == m_lMapID)
	{
		ReadPlayerOfficerTable();//用于重伤英雄的自动恢复
		ClearNPCArmyOfBigMap();
	}
	*/
}
//郡状态信息
int COtherManager::MapStateList(int handle, unsigned long lPlayerID)
{
	char * tmp = new char[sizeof(MAP_STATE_LIST) + sizeof(tagMapState)*BIG_MAP_SIZE];
	MAP_STATE_LIST *pMapList = (MAP_STATE_LIST *)tmp;

	int n = 0;
	for (; n < BIG_MAP_SIZE; n++)
	{
		(*pMapList)[n].lMapID = m_MapState[n].mapid_;
		(*pMapList)[n].iCountry = m_MapState[n].countryid_;
		(*pMapList)[n].iState = (m_MapState[n].state_==100)?0:m_MapState[n].state_;
	}

	pMapList->serialize(n, SERVER_BOUND_MAP_OTHER, lPlayerID,EN_S_MAP_LIST_INFO);
	int ret = asynch_write(handle, (char *)pMapList, pMapList->length);
	if (tmp != 0)
	{
		delete []tmp;
		tmp = 0;
	}
	return ret;
}

//玩家城市信息
int COtherManager::PlayerCityInfo(int handle, unsigned long lPlayerID)
{
	char * bbuf = 0;
	int count = 0,ret = 0;

	PlayerCity PCity;
	memset(&PCity, 0, sizeof(PlayerCity));
	PCity.setmasterid(lPlayerID);

	if (g_Proxy.db_multi_select(PCity, bbuf, count) != DB_Proxy::DB_SUCCEED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATA_ERROR, SERVER_BOUND_MAP_OTHER, lPlayerID);

		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("玩家城市信息 error db_select_all  count:%d\n", count/sizeof(PlayerCity)));
		return 0;
	}
	int Item = count/sizeof(PlayerCity);
	PlayerCity *pCity = (PlayerCity *)bbuf;

	char * tmp = new char[sizeof(PLAYER_CITY_LIST) + sizeof(tagPlayerCityList)*Item];
	PLAYER_CITY_LIST *pPlayerCity = (PLAYER_CITY_LIST *)tmp;

	pPlayerCity->serialize(Item, SERVER_BOUND_MAP_OTHER, lPlayerID);
	for (int n = 0; n < Item; n++)
	{
		(*pPlayerCity)[n].lCityID = pCity[n].cityid_;
		(*pPlayerCity)[n].lMapID = pCity[n].mapid_;
		(*pPlayerCity)[n].lState = pCity[n].state_;
	}
	
	ret = asynch_write(handle, (char *)pPlayerCity, pPlayerCity->length);

	if (bbuf != NULL)
	{
		delete [] bbuf;
		bbuf = 0;
	}
	if (tmp != 0) 
	{
		delete [] tmp;
		tmp = 0;
	}
	return ret;
}
//玩家基本信息
int COtherManager::PlayerBaseInfo(int handle, unsigned long lPlayerID)
{ 
	//m_mapOnLinePlayer[lPlayerID] = handle;
	g_DataServer.setAgentOfPlayer(lPlayerID,handle);
	notifyPlayerState2OtherSvr(lPlayerID,handle);
	
	PlayerOfficerTable PlayerTable;
	memset(&PlayerTable, 0, sizeof(PlayerOfficerTable));

	PlayerTable.setmasterid(lPlayerID);
	PlayerTable.setdetailid(lPlayerID);
	int ret = 0;
	int dbRet = g_Proxy.db_select(PlayerTable);
	if (dbRet== DB_Proxy::DB_EOF)
	{
		//没有首领英雄
		HERO_SYSTEM_INFO HeadamHero;
		HeadamHero.serialize(EN_S_NOT_HEADAM_HERO, SERVER_BOUND_MAP_OTHER, lPlayerID, EN_S_HERO_SYSTEM_INFO);
		asynch_write(handle, (char *)&HeadamHero, HeadamHero.length);
		//printf("没有首领英雄 %ld\n",lPlayerID);
		return 0;
	}
	else if (dbRet == DB_Proxy::DB_FAILED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, SERVER_BOUND_MAP_OTHER, lPlayerID);
		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,(" g_Proxy.db_select(PlayerTable); (dbRet == DB_Proxy::DB_FAILED) error \n"));
		return 0;
	}
	//有首领英雄
	HERO_SYSTEM_INFO HeadamHero;
	HeadamHero.serialize(EN_S_HAVE_HEADAM_HERO, SERVER_BOUND_MAP_OTHER, lPlayerID, EN_S_HERO_SYSTEM_INFO);
	asynch_write(handle, (char *)&HeadamHero, HeadamHero.length);

	PlayerChatRoomTable Res;
	memset(&Res, 0, sizeof(Res));
	Res.setid(lPlayerID);
	g_Proxy.db_select(Res);
	
	PLAYER_BASE_INFO PlayerInfo;
	
	PlayerInfo.serialize(lPlayerID, PlayerTable.officername_, PlayerTable.faceid_, Res.consortiaid_, PlayerTable.exp_, PlayerTable.officerrace_, 
		PlayerTable.jobid_, PlayerTable.credit_, PlayerTable.level_, PlayerTable.militaryrank_, 0,Res.country_,Res.science_,Res.money_,
		SERVER_BOUND_MAP_OTHER, lPlayerID);	
	ret = asynch_write(handle, (char *)&PlayerInfo, PlayerInfo.length);
	return ret;
}
//创建指挥官
int COtherManager::OnCreateOfficer(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize != sizeof(CREATE_OFFICER))
	{
		LOG_TRACE_EX(false,("COtherManager::OnCreateOfficer-->消息包非法：玩家%d\n",lPlayerID));
		return 0;
	}

	CREATE_OFFICER *pOfficer = (CREATE_OFFICER *)pDataBuffer;
	int ret = 0,iRaceLower = 1, iRaceUpper = 8;
	int iRace = pOfficer->iRaceID,iCountry = pOfficer->iCountry;
	long signed lMapID = 0, lCityID = 0;
	
	CREATE_OFFICER_FAILD Faild;
	memset(&Faild, 0, sizeof(CREATE_OFFICER_FAILD));
	
	//国家ID不在有效范围内
	if (iCountry < 1 || iCountry > 2)
	{
		Faild.serialize(1, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		return ret;
	}
	//随机事件
	//srand(unsigned long(IceUtil::Time::now().toLongDateTime()));
	if (iCountry == 1)		//联盟
	{
		iRaceLower = 1;
		iRaceUpper = 4;
		lMapID = 1003;//m_UnionSafeMap[rand()%m_UnionSafeMap.size()];
		lCityID = lMapID * 10000 +101+rand()%4;
	}
	else					//部落
	{
		iRaceLower = 5;
		iRaceUpper = 8;
		lMapID = 1007;//m_TribeSafeMap[rand()%m_TribeSafeMap.size()];;
		lCityID = lMapID * 10000 +101+rand()%4;
	}
	//种族ID不在有效范围内
	if (iRace > iRaceUpper || iRace < iRaceLower)		
	{
		Faild.serialize(2, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		return ret;
	}
	if (pOfficer->iJobID > 5 || pOfficer->iJobID < 1)		
	{
		Faild.serialize(6, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		return ret;
	}
	int iJob = 0;
	switch(pOfficer->iJobID)
	{
	case 1:
		iJob = m_mapJobParam[pOfficer->iRaceID-1].iKnight;
		break;
	case 2:
		iJob = m_mapJobParam[pOfficer->iRaceID-1].iSoldier;
		break;
	case 3:
		iJob = m_mapJobParam[pOfficer->iRaceID-1].iShooter;
		break;
	case 4:
		iJob = m_mapJobParam[pOfficer->iRaceID-1].iMaster;
		break;
	case 5:
		iJob = m_mapJobParam[pOfficer->iRaceID-1].iPastor;
		break;
	};
	if (iJob != 1)
	{
		Faild.serialize(7, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		printf("玩家%ld 种族%d 与职业 %d不对应\n",lPlayerID,pOfficer->iRaceID,pOfficer->iJobID);
		return ret;
	}
	char * buf=0;
	int count=0;
	PlayerOfficerTable OfficerTable;
	memset(&OfficerTable, 0, sizeof(PlayerOfficerTable));
	if (g_Proxy.db_select_all(OfficerTable, buf, count) != DB_Proxy::DB_SUCCEED)
	{
		Faild.serialize(4, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
	//	return ret;
	}
	int iItem = count/sizeof(PlayerOfficerTable);
	PlayerOfficerTable * pOfficerTable = (PlayerOfficerTable *)buf;
	for (int n=0; n < iItem; n++)
	{
		if (strcmp(pOfficer->szNick, pOfficerTable[n].officername_) == 0 || pOfficerTable[n].playerid_ == lPlayerID)
		{
			/*delete by xujincai 20081215 for memory
			Faild.serialize(3, SERVER_BOUND_MAP_OTHER, lPlayerID);
			ret = asynch_write(handle, (char *)&Faild, Faild.length);
			printf("%ld 玩家创建首领英雄时 名称已存在 %s\n",lPlayerID,pOfficer->szNick);
			return ret;
			*/
			ret = -100;
			break;
		}
	}
	//add by xujincai 20081215
	if (buf != NULL)
	{
		delete [] buf;
		buf = 0;
	}
	if(ret == -100)
	{//名字已存在
		Faild.serialize(3, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		printf("%ld 玩家创建首领英雄时 名称已存在 %s\n",lPlayerID,pOfficer->szNick);
		return ret;
	}

	//创建主英雄数据
	memset(&OfficerTable, 0, sizeof(PlayerOfficerTable));
	OfficerTable.playerid_ = lPlayerID;
	OfficerTable.officerid_ = lPlayerID;
	OfficerTable.mapid_ = lMapID;
	OfficerTable.cityid_ = lCityID;
	OfficerTable.faceid_ = pOfficer->iFace;
	OfficerTable.officerrace_ = pOfficer->iRaceID;
	OfficerTable.jobid_ = pOfficer->iJobID;
	OfficerTable.level_ = 1;
	OfficerTable.militaryrank_ = 0;
	OfficerTable.state_ = 0;
	OfficerTable.stamina_ += m_mapRaceParam[pOfficer->iRaceID].iEndurance;
	OfficerTable.power_ += m_mapRaceParam[pOfficer->iRaceID].iStrength;
	OfficerTable.celerity_ += m_mapRaceParam[pOfficer->iRaceID].iAgility;
	OfficerTable.wit_ += m_mapRaceParam[pOfficer->iRaceID].iIntelligence;
	OfficerTable.techdot_ = 5;
	OfficerTable.grouprate_ = 80;//0.8的成长系数，存入数据库*100
	ProcHeroAttribute(OfficerTable);//根据基础属性，计算该英雄的二级属性
	strcpy(OfficerTable.officername_,pOfficer->szNick);

	if (g_Proxy.db_insert(OfficerTable) != DB_Proxy::DB_SUCCEED)
	{
		Faild.serialize(5, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		LOG_TRACE_EX(false,("COtherManager::OnCreateOfficer-->玩家%d写入指挥官表失败\n",lPlayerID));
		return ret;
	}
	/*20100407
	*/
	ISystemLog::getInstance()->heroAttribute_Log(OfficerTable.playerid_,OfficerTable.officerid_,OfficerTable.officername_,
		OfficerTable.officerrace_,OfficerTable.jobid_,OfficerTable.faceid_,OfficerTable.level_,OfficerTable.stamina_,
		OfficerTable.power_,OfficerTable.celerity_,OfficerTable.wit_);

	//初始金钱、科技点
	PlayerChatRoomTable Country;
	memset(&Country, 0, sizeof(PlayerChatRoomTable));
	Country.playerid_ = lPlayerID;
	Country.country_ = iCountry;
	Country.money_ = 10000;
	Country.science_ = 5000;
	Country.maxpacknum_ = EN_PACK_GRID_NUM;
	if (g_Proxy.db_insert(Country) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("COtherManager::OnCreateOfficer-->玩家%d创建指挥官 Country db_insert(Country) \n",lPlayerID));
		return 0;
	}
	//初始新手村
	PlayerCity PCity;
	memset(&PCity, 0, sizeof(PlayerCity));
	PCity.mapid_ = lMapID;//OfficerTable.mapid_;
	PCity.playerid_ = lPlayerID;
	PCity.cityid_ = OfficerTable.cityid_;
	PCity.state_ = 100;//100:代表新手村
	if (g_Proxy.db_insert(PCity)!= DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("玩家城市信息 g_Proxy.db_insert(PCity)\n"));
		return 0;
	}

	//初始建筑
	int iWarehouseLevel = 1;
	initPlayerConstruction(lPlayerID,OfficerTable.mapid_, OfficerTable.cityid_,iWarehouseLevel);
	PlayerResource ResByPlayer;
	ResByPlayer.foodcount_ = 10000;
	ResByPlayer.woodcount_ = 10000;
	ResByPlayer.ironcount_ = 5000;
	ResByPlayer.skincount_ = 5000;
	ResByPlayer.yollowstonecount_ = 0;
	ResByPlayer.redstonecount_ = 0;
	ResByPlayer.greenstonecount_ = 0;
	ResByPlayer.backironcount_ = 0;
	ResByPlayer.content_ = 20000;
	ResByPlayer.completetimes_ = IceUtil::Time::now().toSeconds();
	ResByPlayer.mapid_ = OfficerTable.mapid_;
	ResByPlayer.cityid_ = OfficerTable.cityid_;
	ResByPlayer.playerid_ = lPlayerID;
	ConstructionConfigure * pConstrParam = g_LoadDBinfo.getConstructionConfigure(5, iWarehouseLevel);
	if (pConstrParam != NULL) 
	{
		ResByPlayer.content_ = pConstrParam->modify_ - ResByPlayer.foodcount_ - ResByPlayer.woodcount_ - ResByPlayer.ironcount_ -ResByPlayer.skincount_;
	}
	//增加英雄初始精专技能
	OfficerInitSkill_MultiMap_iterator lowerIter,upperIter; 
	OfficerInitSkill *officerInitSkill = NULL;
	lowerIter = g_LoadDBinfo.getOfficerInitSkill_Lowerbound(OfficerTable.jobid_);
	upperIter = g_LoadDBinfo.getOfficerInitSkill_Upperbound(OfficerTable.jobid_);
	for (;lowerIter != upperIter;lowerIter++)
	{
		officerInitSkill = lowerIter->second;
		if (officerInitSkill == NULL)
		{
			continue;
		}
		SkillDetail  *skillDetail = g_LoadDBinfo.getSkillDetail((SKILL_TYPE)officerInitSkill->skill,officerInitSkill->skillLevel);
		
		if(skillDetail)
		{
			PlayerSkillTable officerSkill;
			officerSkill.setmasterid(OfficerTable.playerid_,OfficerTable.officerid_);
			officerSkill.setdetailid(skillDetail->skillID);
			officerSkill.skilltype_ = skillDetail->skillType;
			officerSkill.skilllevel_ = skillDetail->level;
			if (g_Proxy.db_insert(officerSkill) != DB_Proxy::DB_SUCCEED)
			{		
				LOG_TRACE_EX(false,("给玩家 %ld 的首领 %d 添加默认精专技能(%d-%d)时失败!\n",
					OfficerTable.playerid_,OfficerTable.officerid_,skillDetail->skillID,skillDetail->level));
			}
		}
	}
	//添加部队，大地图服务器负责分配对应的资源，然后向郡服务器发送创建部队的消息。
	LPPlayerInitArmyParam_Item pInitArmyParam = g_LoadDBinfo.GetPlayerInitArmyParam(iCountry,OfficerTable.jobid_);
	if (pInitArmyParam != NULL && pInitArmyParam->Count >0)
	{
		//分配资源，兵PlayerSoldierRace、武器、防具ProductionAmount
		PlayerSoldierRace soldierRace;
		soldierRace.setmasterid(OfficerTable.mapid_,OfficerTable.cityid_);
		soldierRace.setdetailid(OfficerTable.playerid_);
		soldierRace.completetimes_ = IceUtil::Time::now().toSeconds();
		switch(pInitArmyParam->RaceID)
		{
		case 1:
			soldierRace.human_ = pInitArmyParam->Count;
			break;
		case 2:
			soldierRace.fairy_ = pInitArmyParam->Count;
			break;
		case 3:
			soldierRace.mannikin_ = pInitArmyParam->Count;
			break;
		case 4:
			soldierRace.dandiprat_ = pInitArmyParam->Count;
			break;
		case 5:
			soldierRace.halfanimal_ = pInitArmyParam->Count;
			break;
		case 6:
			soldierRace.cattle_ = pInitArmyParam->Count;
			break;
		case 7:
			soldierRace.blackfairy_ = pInitArmyParam->Count;
			break;
		case 8:
			soldierRace.decpesrson_ = pInitArmyParam->Count;
			break;
		default:
			break;
		}
		if (g_Proxy.db_insert(soldierRace) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE_EX(false,("给玩家 %ld Face %d 在郡内%ld 城市中%ld 分配的兵soldierRace Error!\n",lPlayerID,OfficerTable.faceid_,OfficerTable.mapid_,OfficerTable.cityid_));
		}
		
		ProductionAmount proAmount;
		if (pInitArmyParam->WeaponType >0)
		{
			proAmount.setmasterid(OfficerTable.mapid_,OfficerTable.cityid_,lPlayerID,WEAPON_ITEM);
			proAmount.setdetailid(pInitArmyParam->WeaponType);
			proAmount.productioncount_ = pInitArmyParam->Count;
			if (g_Proxy.db_insert(proAmount) == DB_Proxy::DB_SUCCEED)
			{
				ResByPlayer.content_ -= pInitArmyParam->Count;
			}
			memset(&proAmount,0,sizeof(ProductionAmount));
		}
		if (pInitArmyParam->DefendType >0)
		{
			proAmount.setmasterid(OfficerTable.mapid_,OfficerTable.cityid_,lPlayerID,DEFEND_ITEM);
			proAmount.setdetailid(pInitArmyParam->DefendType);
			proAmount.productioncount_ = pInitArmyParam->Count;
			if (g_Proxy.db_insert(proAmount) == DB_Proxy::DB_SUCCEED)
			{
				ResByPlayer.content_ -= pInitArmyParam->Count;
			}
		}
	}
	if (g_Proxy.db_insert(ResByPlayer) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("给玩家 %ld Face %d 在郡内%ld 城市中%ld 分配的资源量 g_Proxy.db_insert(ResByPlayer) Error!\n",lPlayerID,OfficerTable.faceid_,OfficerTable.mapid_,OfficerTable.cityid_));
		
		return 0;
	}
	//返回该玩家的基础信息，包括主英雄信息、所有郡状态、玩家初始城镇信息等
	HERO_SYSTEM_INFO HeroInfo;
	HeroInfo.serialize(EN_S_CREATE_HEADAM_HERO, SERVER_BOUND_MAP_OTHER, lPlayerID, EN_S_HERO_SYSTEM_INFO);
	ret = asynch_write(handle, (char *)&HeroInfo, HeroInfo.length);


	PlayerBaseInfo(handle, lPlayerID);

	Notify_Add_Main_Hero AddHero;
	AddHero.iOfficerID = lPlayerID;
	strcpy(AddHero.szOfficerName, pOfficer->szNick);
	AddHero.serialize();
	ret = event_notify(MDM_GP_MAP_CHAT, (char * )&AddHero, sizeof(AddHero));// 发往服务器
	ret = event_notify(MDM_GP_GAME_MANAGE_SERVER, (char * )&AddHero, sizeof(AddHero));// 发往服务器

	MapStateList(handle, lPlayerID);
	PlayerCityInfo(handle, lPlayerID);
	//发送到新手村（郡地图），创建部队命令，为主英雄初始一支部队
	if(pInitArmyParam != NULL&& pInitArmyParam->Count >0)
	{
		CREATE_ARMY createArmy;
		createArmy.serialize(OfficerTable.mapid_,OfficerTable.cityid_,OfficerTable.officerid_,OfficerTable.officername_,
			pInitArmyParam->RaceID,pInitArmyParam->SoldierType,pInitArmyParam->Count,
			pInitArmyParam->WeaponType,pInitArmyParam->WeaponLevel,pInitArmyParam->DefendType,pInitArmyParam->DefendLevel,
			0,0,OfficerTable.playerid_,OfficerTable.mapid_);
		createArmy.agent = handle;
		event_notify(OfficerTable.mapid_,(char*)&createArmy,createArmy.length);
	}
	//发送新手邮件
	LPMailInfoParam_Item pMailInfoParam = 0;
	Notify_Send_Mail email;
	pMailInfoParam = g_LoadDBinfo.GetMailInfoParam(iCountry,MAIL_NEWPLAYER1);
	if(pMailInfoParam)
	{
		email.type = Normal;
		email.ulRecieverPlayerID = lPlayerID;
		strcpy(email.szSendPlayerName,"系统");
		strcpy(email.sztitle,pMailInfoParam->Title);
		strcpy(email.szbody,pMailInfoParam->Content);
		email.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));
	}
	
	pMailInfoParam = g_LoadDBinfo.GetMailInfoParam(iCountry,MAIL_NEWPLAYER2);
	if(pMailInfoParam)
	{
		email.type = Normal;
		email.ulRecieverPlayerID = lPlayerID;
		strcpy(email.szSendPlayerName,"系统");
		strcpy(email.sztitle,pMailInfoParam->Title);
		strcpy(email.szbody,pMailInfoParam->Content);
		email.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));
	}
	
	pMailInfoParam = g_LoadDBinfo.GetMailInfoParam(iCountry,MAIL_NEWPLAYER3);
	if(pMailInfoParam)
	{
		email.type = Normal;
		email.ulRecieverPlayerID = lPlayerID;
		strcpy(email.szSendPlayerName,"系统");
		strcpy(email.sztitle,pMailInfoParam->Title);
		strcpy(email.szbody,pMailInfoParam->Content);
		email.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));
	}
	//通知排行系统
	ClientLogin_Request notifyLogin;
	notifyLogin.serialize(lPlayerID);
	notifyLogin.agent = handle;
	event_notify(MDM_GP_MAP_CONSORTIA,(char*)&notifyLogin,notifyLogin.length);

	return ret;
}
int COtherManager::ProcOfficerActionDot(int millrank,PlayerOfficerTable *pOfficer)
{
	int iPlayerID = pOfficer->playerid_;
	int iOfficerID = pOfficer->officerid_;
	int iMaxActionDot = millrank * g_IniFile.m_ActionDotQuotiety + g_IniFile.m_BaseActionDot - pOfficer->actiondot_;

	int64 iTimes = IceUtil::Time::now().toSeconds() - pOfficer->updatetime_;
	int iActionDot = 0;

	if ( iMaxActionDot >0)
	{
		//计算增加的行动值，如果增加的行动值为0，则不改变英雄的行动值的更新时间
		int64 iTmpTimes = iTimes * g_IniFile.m_ActionDotQuotiety;
		iActionDot = (int)(iTmpTimes/3600);
		if (iActionDot <=0)
		{
			iTimes = 0;
		}
		else
		{
			iTimes =iTimes - (iTimes%3600)/5;
			iActionDot = (iActionDot>iMaxActionDot)?iMaxActionDot:iActionDot;
		}

	}
	if (iTimes)
	{
		memset(pOfficer,0,sizeof(PlayerOfficerTable));
		pOfficer->setmasterid(iPlayerID);
		pOfficer->setdetailid(iOfficerID);
		pOfficer->updatetime_ = iTimes;
		pOfficer->actiondot_ = iActionDot;
		if (g_Proxy.db_increase(*pOfficer) != DB_Proxy::DB_SUCCEED)
		{	
			return 1;
		}
	}
	return 0;
}
//查询指挥官属性
int COtherManager::GetOfficerAttribute(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize != sizeof(GET_OFFICER_INFO))
	{
		LOG_TRACE_EX(false,("COtherManager::GetOfficerAttribute-->消息包非法：玩家%d\n",lPlayerID));
		return 0;
	}

	GET_OFFICER_INFO * pOfficerInfo = (GET_OFFICER_INFO *)pDataBuffer;
	int ret = 0;
	ERROR_INFO ErrorInfo;
	memset(&ErrorInfo, 0, sizeof(ERROR_INFO));
	if (pOfficerInfo->lOfficerID <= 0)
	{
		ErrorInfo.serialize(EN_OFFICER_ID_ERROR, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
		return ret;
	}
	PlayerOfficerTable mainOfficer,Officer;
	//查询英雄表，用于技术行动值（根据军衔）
	mainOfficer.setmasterid(lPlayerID);
	mainOfficer.setdetailid(lPlayerID);
	if (g_Proxy.db_select(mainOfficer) != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_FIND_OFFICER_ERROR, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
		return ret;
	}
	Officer.setmasterid(lPlayerID);
	Officer.setdetailid(pOfficerInfo->lOfficerID);
	if (g_Proxy.db_select(Officer) != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_FIND_OFFICER_ERROR, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
		return ret;
	}
	//根据主英雄的军衔计算行动值
	if (Officer.state_ != EN_GBH)
	{//非重伤
		if (ProcOfficerActionDot(mainOfficer.militaryrank_,&Officer))
		{
			ErrorInfo.serialize(EN_FIND_OFFICER_ERROR, SERVER_BOUND_MAP_OTHER, lPlayerID);
			ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
			return ret;
		}
		
	}
	//返回该英雄的属性值
	OFFICER_ATTRIBUTE OfficerAttri;
	OfficerAttri.serialize(Officer.officerid_,Officer.credit_,Officer.stamina_ + Officer.levelstamina_,Officer.power_ + Officer.levelpower_,
		Officer.celerity_ + Officer.levelcelerity_,Officer.wit_ + Officer.levelwit_,Officer.attack_,
		Officer.physicrecovery_,Officer.magicrecovery_,Officer.level_, Officer.militaryrank_ ,Officer.exp_,Officer.techdot_,
		Officer.actiondot_,Officer.equipstamina_,Officer.equippower_,Officer.equipcelerity_,Officer.equipwit_,Officer.herohp_,
		Officer.magicattack_,SERVER_BOUND_MAP_OTHER,lPlayerID);
	ret = asynch_write(handle, (char *)&OfficerAttri, OfficerAttri.length);
	return ret;
}

//赎回英雄
int COtherManager::RedeemHero(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize != sizeof(REDEEM_HERO))
	{
		LOG_TRACE_EX(false,("COtherManager::RedeemHero-->消息包非法：玩家%d\n",lPlayerID));
		return 0;
	}

	REDEEM_HERO * pRedeem = (REDEEM_HERO *)pDataBuffer;
	int ret=0;
	ERROR_INFO ErrorInfo;
	memset(&ErrorInfo, 0, sizeof(ERROR_INFO));
	if (pRedeem->lHeroID <= 0)
	{
		ErrorInfo.serialize(EN_OFFICER_ID_ERROR, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
		return ret;
	}
	//首先判断英雄是否被俘
	PlayerOfficerTable Officer;
	memset(&Officer, 0, sizeof(PlayerOfficerTable));

	Officer.setmasterid(lPlayerID);
	Officer.setdetailid(pRedeem->lHeroID);
	if (g_Proxy.db_select(Officer) != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_FIND_OFFICER_ERROR, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
		LOG_TRACE_EX(false,("玩家%d赎回英雄 %ld  db_select(Officer) error \n",lPlayerID,pRedeem->lHeroID));
		return ret;
	}
	if (Officer.state_ != EN_BY_CAPTIVE)
	{
		return ret;
	}

	//判断价格
	PlayerChatRoomTable ResByPlayer;
	ResByPlayer.setid(lPlayerID);
	if (g_Proxy.db_select(ResByPlayer) != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_DATA_ERROR, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	if(Officer.militaryrank_ <=0)
	{
		pRedeem->iMoney = Officer.level_ * g_IniFile.m_RedeemHeroMoney;
	}
	else
	{
		pRedeem->iMoney = Officer.level_ * Officer.militaryrank_ * g_IniFile.m_RedeemHeroMoney;
	}

	if (ResByPlayer.money_ < pRedeem->iMoney)
	{
		ErrorInfo.serialize(EN_MONEY_COUNT, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	//修改状态
	Officer.state_ = 0;
	if (g_Proxy.db_update(Officer, Officer) != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_OPERATE_DBG_FAILED, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
		LOG_TRACE_EX(false,("玩家%d赎回英雄 %ld  db_update(Officer, Officer)  error \n",lPlayerID,pRedeem->lHeroID));
		return ret;
	}
	//扣除赎金
	memset(&ResByPlayer,0,sizeof(ResByPlayer));
	ResByPlayer.setid(lPlayerID);
	ResByPlayer.money_ -= pRedeem->iMoney;
	if (g_Proxy.db_increase(ResByPlayer) != DB_Proxy::DB_SUCCEED)
	{
	}

	//赎回英雄成功
	HERO_SYSTEM_INFO Hero;
	Hero.serialize(EN_S_REDEEM_HERO_SUCCEED, SERVER_BOUND_MAP_OTHER, lPlayerID, EN_S_HERO_SYSTEM_INFO);
	ret = asynch_write(handle, (char *)&Hero, Hero.length);
	//20100408增加日志
	//状态日志
	ISystemLog::getInstance()->heroState_Ransom_Log(Officer.playerid_,Officer.officerid_,Officer.officername_);
	//金钱日志
	memset(&Officer, 0, sizeof(Officer));
	Officer.setmasterid(lPlayerID);
	Officer.setdetailid(lPlayerID);
	if (g_Proxy.db_select(Officer) == DB_Proxy::DB_SUCCEED)
	{
		ISystemLog::getInstance()->playerMoney_Ransom_Log(Officer.playerid_,Officer.officername_,
			ResByPlayer.money_ + pRedeem->iMoney,ResByPlayer.money_,pRedeem->iMoney,pRedeem->lHeroID);
	}

	//删除俘虏表中的俘虏记录
	char * buf=0;
	int iCount2=0,iItem2=0;
	PlayerCaptiveHero CapHero;

	if (g_Proxy.db_select_all(CapHero, buf, iCount2)!= DB_Proxy::DB_SUCCEED)
	{
		//add by xujincai20081225
		LOG_TRACE_EX(false,("玩家%d赎回英雄%d，赎金：%d，PlayerCaptiveHero表为空.\n",lPlayerID,pRedeem->lHeroID,pRedeem->iMoney));
		return 0;
	}
	iItem2 = iCount2/sizeof(PlayerCaptiveHero);
	PlayerCaptiveHero * pCapHero = (PlayerCaptiveHero *)buf;
	memset(&ResByPlayer,0,sizeof(ResByPlayer));
	for (int n = 0; n< iItem2; n++)
	{
		if (pCapHero[n].byplayerid_ == lPlayerID && pCapHero[n].byofficerid_ == pRedeem->lHeroID)
		{
			g_Proxy.db_delete(pCapHero[n]);
			ResByPlayer.setid(pCapHero[n].playerid_);
			ResByPlayer.money_ += pRedeem->iMoney;
			break;
		}
	}
	//add by xujincai 20081215
	if (buf != NULL)
	{
		delete [] buf;
		buf = 0;
	}
	//把赎金给俘虏方
	if (ResByPlayer.playerid_ >0 )
	{
		if (g_Proxy.db_increase(ResByPlayer) != DB_Proxy::DB_SUCCEED)
		{
			ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, m_lMapID, lPlayerID);
			ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
			return ret;
		}
		//20100408增加日志
		Officer.setmasterid(ResByPlayer.playerid_);
		Officer.setdetailid(ResByPlayer.playerid_);
		if (g_Proxy.db_select(Officer) == DB_Proxy::DB_SUCCEED)
		{
			ISystemLog::getInstance()->playerMoney_Beransom_Log(Officer.playerid_,Officer.officername_,
				ResByPlayer.money_ - pRedeem->iMoney,ResByPlayer.money_,pRedeem->iMoney,lPlayerID,pRedeem->lHeroID);
		}
	}
	return ret;
}
//销毁背包中的装备
int COtherManager::DestroyPackEquip(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize != sizeof(DESTROY_EQUIP))
	{
		LOG_TRACE_EX(false,("COtherManager::DestroyPackEquip-->消息包非法：玩家%d\n",lPlayerID));
		return 0;
	}

	DESTROY_EQUIP * pDestroy = (DESTROY_EQUIP *)pDataBuffer;
	int ret=0;
	int count = pDestroy->Count;
	EXCHANGE_EQUIP_INFO ExchEquip;
	PlayerItemStorageTable Pack;
	Pack.setmasterid(pDestroy->lHeroID,lPlayerID);
	Pack.setdetailid(pDestroy->iSortID);
	if(g_Proxy.db_select(Pack) != DB_Proxy::DB_SUCCEED)
	{
		Pack.setmasterid(1,lPlayerID);
		Pack.setdetailid(pDestroy->iSortID);
		if(g_Proxy.db_select(Pack) != DB_Proxy::DB_SUCCEED)
		{
			ExchEquip.serialize(EN_S_DESTROY_EQUIP_FAILED, pDestroy->iSortID, SERVER_BOUND_MAP_OTHER, lPlayerID);
			ret = asynch_write(handle, (char *)&ExchEquip, ExchEquip.length);
			return 0;
		}
	}
	//销毁N个装备
	if (count <=0 || count >Pack.count_)
	{
		count = Pack.count_;
	}
	if (count >= Pack.count_)
	{
		if (g_Proxy.db_delete(Pack) != DB_Proxy::DB_SUCCEED)
		{
			ExchEquip.serialize(EN_S_DESTROY_EQUIP_FAILED, pDestroy->iSortID, SERVER_BOUND_MAP_OTHER, lPlayerID);
			ret = asynch_write(handle, (char *)&ExchEquip, ExchEquip.length);
			return 0;
		}
	}
	else
	{
		Pack.count_ -= count;
		if (g_Proxy.db_update(Pack,Pack) != DB_Proxy::DB_SUCCEED)
		{
			ExchEquip.serialize(EN_S_DESTROY_EQUIP_FAILED, pDestroy->iSortID, SERVER_BOUND_MAP_OTHER, lPlayerID);
			ret = asynch_write(handle, (char *)&ExchEquip, ExchEquip.length);
			return 0;
		}
	}
	
	ExchEquip.serialize(EN_S_DESTROY_EQUIP_SUCCEED, pDestroy->iSortID, SERVER_BOUND_MAP_OTHER, lPlayerID);
	ret = asynch_write(handle, (char *)&ExchEquip, ExchEquip.length);

	//20100408增加日志
	PlayerOfficerTable officer;
	officer.setmasterid(lPlayerID);
	officer.setdetailid(lPlayerID);
	g_Proxy.db_select(officer);
	ISystemLog::getInstance()->playerEquip_Recover_Log(officer.playerid_,officer.officername_,Pack.pos_,Pack.itemid_,count);

	//增加金钱
	PlayerChatRoomTable playertb;
	playertb.setid(lPlayerID);
	int money = 0;
	if(Pack.pos_ <= ACC_3 || Pack.pos_==INLAYRESOURCE)
	{//英雄装备，有可能为精炼后的装备
		IEquipment iEquipment(&g_LoadDBinfo,Pack.itemid_);
		money= iEquipment.getPrice() * count;
	}
	else if(Pack.pos_ <= EXPANDPACKCARD||Pack.pos_ == LUCKCARD || Pack.pos_ == FIGHTCARD)
	{//卡
		LPCardParam pCardCfg = g_LoadDBinfo.GetCardConfig(Pack.itemid_);
		if(pCardCfg)
		{
			money = pCardCfg->Price * 100 * count;
		}
	}
	else if (Pack.pos_ == ARMYBRAND)
	{//军牌
		ArmyBrand *pArmyBrand = g_LoadDBinfo.getArmyBrandParamByID(Pack.itemid_);
		if (pArmyBrand)
		{
			money  = pArmyBrand->getPrice() * count;
		}
	}
	//返回金钱
	if(money>0)
	{
		playertb.money_ = money;
		if(g_Proxy.db_increase(playertb) == DB_Proxy::DB_SUCCEED)
		{
			//20100408增加日志
			ISystemLog::getInstance()->playerMoney_RecoverEequip_Log(officer.playerid_,officer.officername_,
				playertb.money_ - money,playertb.money_,money,Pack.pos_,Pack.itemid_,count);
		}
	}
	return ret;
}
//得到英雄装备
int COtherManager::OnGetHeroEquip(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize != sizeof(GET_OFFICER_INFO))
	{
		LOG_TRACE_EX(false,("COtherManager::OnGetHeroEquip-->消息包非法：玩家%d\n",lPlayerID));
		return 0;
	}

	GET_OFFICER_INFO * pOfficerInfo = (GET_OFFICER_INFO *)pDataBuffer;
	char * buf=0, *buf1=0;
	int ret=0,count=0,iItem=0,n=0,count1=0,iItem1=0;
	PlayerItemStorageTable HeroPack;
	//玩家身上的装备
	HeroPack.setmasterid(pOfficerInfo->lOfficerID, lPlayerID);
	ret = g_Proxy.db_multi_select(HeroPack, buf, count);
	if (ret != DB_Proxy::DB_SUCCEED)
	{
		
	}
	iItem = count/sizeof(PlayerItemStorageTable);
	PlayerItemStorageTable * pHeroPack = (PlayerItemStorageTable *)buf;

	//公共背包里的装备
	HeroPack.setmasterid(1, lPlayerID);
	ret = g_Proxy.db_multi_select(HeroPack, buf1, count1);
	iItem1 = count1/sizeof(PlayerItemStorageTable);
	PlayerItemStorageTable * pHeroPack1 = (PlayerItemStorageTable *)buf1;

	char * tmp = new char[sizeof(HERO_EQUIP_LIST) + sizeof(tagHeroEquipList)*(iItem+iItem1)];
	HERO_EQUIP_LIST *pHeroPackList = (HERO_EQUIP_LIST *)tmp;

	for (; n < iItem; n++)
	{
		(*pHeroPackList)[n].iSortID = pHeroPack[n].sortid_;
		(*pHeroPackList)[n].InlayItem.NewItemID = pHeroPack[n].itemid_;
		(*pHeroPackList)[n].iPos = pHeroPack[n].pos_;
		(*pHeroPackList)[n].iLevel = pHeroPack[n].level_;
		(*pHeroPackList)[n].iShowPos = pHeroPack[n].showpos_;
		(*pHeroPackList)[n].iType = pHeroPack[n].type_;
		(*pHeroPackList)[n].iCount = pHeroPack[n].count_;
		(*pHeroPackList)[n].Skill = pHeroPack[n].tech_;
		(*pHeroPackList)[n].SkillLevel = pHeroPack[n].techlevel_;
	}
	for (int i=0; i < iItem1; i++,n++)
	{
		(*pHeroPackList)[n].iSortID = pHeroPack1[i].sortid_;
		(*pHeroPackList)[n].InlayItem.NewItemID = pHeroPack1[i].itemid_;
		(*pHeroPackList)[n].iPos = pHeroPack1[i].pos_;
		(*pHeroPackList)[n].iLevel = pHeroPack1[i].level_;
		(*pHeroPackList)[n].iShowPos = pHeroPack1[i].showpos_;
		(*pHeroPackList)[n].iType = pHeroPack1[i].type_;
		(*pHeroPackList)[n].iCount = pHeroPack1[i].count_;
		(*pHeroPackList)[n].Skill = pHeroPack1[i].tech_;
		(*pHeroPackList)[n].SkillLevel = pHeroPack1[i].techlevel_;
	}
	pHeroPackList->serialize(n, SERVER_BOUND_MAP_OTHER, lPlayerID, EN_S_EQUIPS_LIST);

	ret = asynch_write(handle, (char *)pHeroPackList, pHeroPackList->length);

	if (buf != NULL)
	{
		delete [] buf;
		buf = 0;
	}
	if (buf1 != NULL)
	{
		delete [] buf1;
		buf1 = 0;
	}
	if(tmp)
	{
		delete [] tmp;
		tmp = 0;
	}
	return ret;
}
//操作英雄装备
int COtherManager::OnExchangeHeroEquip(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{	
	EXCHANGE_EQUIP_LIST * pPack = (EXCHANGE_EQUIP_LIST *)pDataBuffer;
	if (sizeof(EXCHANGE_EQUIP_LIST)+sizeof(tagExchangeEquipList)*pPack->iCount != iDataSize)
	{
		LOG_TRACE_EX(false,(" COtherManager::OnExchangeHeroEquip-->消息包非法：玩家%d\n",lPlayerID));
		return 0;
	}

	int ret = 0;
	ERROR_INFO ErrorInfo;
	if (pPack->lHeroID <= 0)
	{
		ErrorInfo.serialize(EN_NOT_HERO_ID, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}

	PlayerItemStorageTable Pack,OldPack;

	PlayerOfficerTable Officer;
	Officer.setmasterid(lPlayerID);
	Officer.setdetailid(pPack->lHeroID);
	if (g_Proxy.db_select(Officer) != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_FIND_OFFICER_ERROR, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
		LOG_TRACE_EX(false,(" %ld 操作英雄(%d)装备 db_select(Officer) error \n",lPlayerID,pPack->lHeroID));
		return ret;
	}
	if (Officer.state_ == EN_BY_CAPTIVE)
	{
		return 0;
	}
	
	int iAttack=0;//攻击
	int iStaina=0;//耐力
	int iPower=0;//力量
	int iCelerity=0;//敏捷
	int iWit=0;//智力iPhy=0,iMagic=0,
	int iDefend = 0;//防御
	int iflag=0;
	for (int n = 0; n < pPack->iCount; n++)
	{
		if ((*pPack)[n].iSortID < 1) 
			continue;
		if ((*pPack)[n].iType > EN_COMM_PACK || (*pPack)[n].iType < 1) 
			continue;	

		Pack.sortid_ = (*pPack)[n].iSortID;
		Pack.playerid_ = lPlayerID;
		Pack.officerid_ = pPack->lHeroID;
		Pack.showpos_ = (*pPack)[n].iShowPos;
		Pack.type_ = (*pPack)[n].iType;
		Pack.count_ = 1;
		
		OldPack.setmasterid(Pack.officerid_, lPlayerID);
		OldPack.setdetailid((*pPack)[n].iSortID);
		if (g_Proxy.db_select(OldPack) != DB_Proxy::DB_SUCCEED)
		{
			OldPack.setmasterid(1, lPlayerID);
			OldPack.setdetailid((*pPack)[n].iSortID);
			if (g_Proxy.db_select(OldPack) != DB_Proxy::DB_SUCCEED)
				continue;
		}

		if ((*pPack)[n].iType == EN_COMM_PACK)//3: 公共背包
			Pack.officerid_ = 1;
		Pack.itemid_ = OldPack.itemid_;
		Pack.level_ = OldPack.level_;
		Pack.pos_ = OldPack.pos_;
		OldPack.setmasterid(OldPack.officerid_, OldPack.playerid_);
		OldPack.setdetailid(OldPack.sortid_);
		if (g_Proxy.db_update(OldPack, Pack) != DB_Proxy::DB_SUCCEED)
		{
			continue;
		}

		//add by xujincai 20081226 不是英雄装备，如：卡和宝箱，不进行其他操作,并判断原来的是否在身上
		if(OldPack.pos_ >=11)
		{
			continue;
		}

		HeroEquipAttribute * heroEquipAttr = g_LoadDBinfo.GetHeroEquipAttribute(Pack.itemid_);
		if (heroEquipAttr == NULL)
		{
			LOG_TRACE_EX(false,("玩家%d操作英雄装备%d非法\n",lPlayerID,Pack.itemid_));
			continue;
		}
				
		if (((*pPack)[n].iMode == EN_PUT_ON_EQUIP)  && (OldPack.type_ != 1))//穿上装备
		{
			iflag = 1;
			//攻击力/防御力
			iAttack += heroEquipAttr->physicAttack;
			iDefend += heroEquipAttr->physicRecovery;
			//属性，4项
			/*
			int iStaina=0;//耐力
			int iPower=0;//力量
			int iCelerity=0;//敏捷
			int iWit=0;//智力
			*/
			iStaina += heroEquipAttr->stamina;
			iPower += heroEquipAttr->power;
			iCelerity += heroEquipAttr->celerity;
			iWit += heroEquipAttr->wit;
		}
		else if (((*pPack)[n].iMode == EN_GET_OFF_EQUIP) && (OldPack.type_ == 1))//脱下装备
		{
			iflag = 1;
			
			//攻击力/防御力
			iAttack -= heroEquipAttr->physicAttack;
			iDefend -= heroEquipAttr->magicRecovery;
			//属性，4项
			/*
			int iStaina=0;//耐力
			int iPower=0;//力量
			int iCelerity=0;//敏捷
			int iWit=0;//智力
			*/
			iStaina -= heroEquipAttr->stamina;
			iPower -= heroEquipAttr->power;
			iCelerity -= heroEquipAttr->celerity;
			iWit -= heroEquipAttr->wit;
		}
	}
	
	if (iflag == 1)
	{
		//增加基本属性
		
		memset(&Officer, 0, sizeof(Officer));
		Officer.setmasterid(lPlayerID);
		Officer.setdetailid(pPack->lHeroID);
		Officer.stamina_ = iStaina;
		Officer.power_ = iPower;
		Officer.celerity_ = iCelerity;
		Officer.wit_ = iWit;
		if(g_Proxy.db_increase(Officer) != DB_Proxy::DB_SUCCEED)
		{
			//_ASSERT(false);
			return 0;
		}

		//Officer为更新后的值
		if (Officer.stamina_ < 0) 
		{
			Officer.stamina_ = 0;
		}
		if (Officer.power_ < 0)
		{
			Officer.power_ = 0;
		}
		if (Officer.celerity_ < 0)
		{
			Officer.celerity_ = 0;
		}
		if (Officer.wit_ < 0)
		{
			Officer.wit_ = 0;
		}

		
		ProcHeroAttribute(Officer);
		Officer.attack_ += iAttack;
		Officer.physicrecovery_ += iDefend;
		Officer.magicrecovery_ += iDefend;
		if(Officer.attack_ <0)
		{
			Officer.attack_ = 0;
		}
		if(Officer.physicrecovery_<0)
		{
			Officer.physicrecovery_ = 0;
		}
		if(Officer.magicrecovery_<0)
		{
			Officer.magicrecovery_ =0;
		}
		if (g_Proxy.db_update(Officer, Officer) == DB_Proxy::DB_SUCCEED)
		{
		//	printf("玩家%ld英雄%d  db_update(Officer,Officer) succeed\n",lPlayerID,pPack->lHeroID);
		}
		if(Officer.mapid_>0)
		{
			heroModifyParamEvent modifyOfficerParam;
			modifyOfficerParam.serialize(Officer.playerid_,Officer.officerid_);
			event_notify(Officer.mapid_, (char * )&modifyOfficerParam, modifyOfficerParam.length);// 发往服务器,
		}
	}
	return 1;
}
//检查英雄是否已穿上该类型的装备
bool COtherManager::haveDressEquipOfPostion(int playerid,int officerid,int postion,int sortid)
{
	PlayerItemStorageTable itemtb;
	char * buf = 0;
	int count = 0;
	int iMaxEquipCount = 1;
	if (postion > ARM)
	{
		iMaxEquipCount = 2;
	}
	int iTmpCount = 0;
	itemtb.setmasterid(officerid,playerid);
	if (g_Proxy.db_multi_select(itemtb,buf,count) == DB_Proxy::DB_FAILED)
	{
		return true;
	}
	count = count /sizeof(PlayerItemStorageTable);
	PlayerItemStorageTable *pItemList = (PlayerItemStorageTable*)buf;
	for (int i=0;i<count;++i)
	{
		if (pItemList[i].type_ !=EN_HERO_BODY || pItemList[i].sortid_ == sortid)
		{//非身上
			continue;
		}
		if (pItemList[i].pos_ == postion)
		{
			iTmpCount ++;
		}
	}
	if (buf)
	{
		delete[] buf;
		buf = 0;
	}
	return (iTmpCount >=iMaxEquipCount)?true:false;
}
int COtherManager::OnExchangeHeroEquipEx(const char * pDataBuffer)
{
	EXCHANGE_EQUIP_LIST * req_msg = (EXCHANGE_EQUIP_LIST*)pDataBuffer;
	if (req_msg->iCount < 1 || req_msg->iCount >2)
	{
		LOG_ERROR_EX(false,(" COtherManager::OnExchangeHeroEquip-->消息包非法：玩家%d\n",req_msg->from));
		return 0;
	}
	ERROR_INFO error_Info;
	tagExchangeEquipList *pExchangeEquip[2] = {0,0};
	pExchangeEquip[0] = &((*req_msg)[0]);
	if (req_msg->iCount == 1)
	{
		if (pExchangeEquip[0]->iMode != EN_PUT_ON_EQUIP && pExchangeEquip[0]->iType != EN_HERO_PACK)
		{//判断位置是否可以而动
			PlayerItemStorageTable allItems;
			PlayerItemStorageTable *pItemsList = 0;
			char *itemBuf = 0;
			int itemcount = 0;
			allItems.setmasterid(1,req_msg->from);
			if (g_Proxy.db_multi_select(allItems,itemBuf,itemcount) == DB_Proxy::DB_FAILED)
			{
				error_Info.serialize(EN_OPERATE_DBG_FAILED, SERVER_BOUND_MAP_OTHER, req_msg->from);
				asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);
				LOG_TRACE_EX(false,("玩家 %ld 操作英雄(%d)装备 ,查询公共背包失败。\n",req_msg->from,req_msg->lHeroID));
				return 3;
			}
			itemcount = itemcount/sizeof(PlayerItemStorageTable);
			pItemsList = (PlayerItemStorageTable*)itemBuf;
			for (int i=0;i<itemcount;i++)
			{
				if(pItemsList[i].showpos_ == pExchangeEquip[0]->iShowPos)
				{
					itemcount = -1;
					break;
				}
			}
			delete[] itemBuf;
			itemBuf = 0;
			if (itemcount == -1)
			{
				error_Info.serialize(EN_PACKPOS_HAVEITEM, SERVER_BOUND_MAP_OTHER, req_msg->from);
				asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);
				return -1;
			}
		}
	}
	else if (req_msg->iCount == 2)
	{
		pExchangeEquip[1] = &((*req_msg)[1]);
		if (pExchangeEquip[1]->iMode == EN_GET_OFF_EQUIP)
		{//做个交换
			pExchangeEquip[0] = &((*req_msg)[1]);
			pExchangeEquip[1] = &((*req_msg)[0]);
		}

		if ((pExchangeEquip[0]->iMode == EN_GET_OFF_EQUIP && pExchangeEquip[1]->iMode != EN_PUT_ON_EQUIP)
			||pExchangeEquip[0]->iMode == EN_PUT_ON_EQUIP && pExchangeEquip[1]->iMode != EN_GET_OFF_EQUIP)
		{
			error_Info.serialize(EN_CLIENT_DATA_ERROR, SERVER_BOUND_MAP_OTHER, req_msg->from);
			asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);
			LOG_TRACE_EX(false,("玩家 %ld 操作英雄(%d)装备 ,一个为穿装备，但另一个非脱装备\n",req_msg->from,req_msg->lHeroID));
			return 3;
		}
	}
	else
	{
		return -4;
	}
	PlayerItemStorageTable equipItem1,equipItem2,oldEquipItem1,oldEquipItem2;
	PlayerOfficerTable updateOfficerInfo;
	IEquipment oneEquip(&g_LoadDBinfo,0);
	IEquipment secEquip(&g_LoadDBinfo,0);
	int error_Code = 0;
	if (pExchangeEquip[0]->iMode == EN_PUT_ON_EQUIP || pExchangeEquip[0]->iMode == EN_GET_OFF_EQUIP)
	{//操作英雄身上的装备,需要处理技能

		PlayerOfficerTable officerInfo;
		officerInfo.setmasterid(req_msg->from);
		officerInfo.setdetailid(req_msg->lHeroID);
		if (g_Proxy.db_select(officerInfo) != DB_Proxy::DB_SUCCEED)
		{
			error_Info.serialize(EN_FIND_OFFICER_ERROR, SERVER_BOUND_MAP_OTHER, req_msg->from);
			asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);	
			LOG_TRACE_EX(false,("玩家 %ld 操作英雄(%d)装备 db_select(Officer) error \n",req_msg->from,req_msg->lHeroID));
			return 1;
		}
		if (officerInfo.state_ == EN_BY_CAPTIVE)
		{
			error_Info.serialize(EN_HERO_BY_CAPTIVE, SERVER_BOUND_MAP_OTHER, req_msg->from);
			asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);	
			return 2;
		}
		if (isInWarMapOfOfficer(&officerInfo))
		{//在战斗中，不能穿脱装备
			error_Info.serialize(EN_OFFICER_INWAR, SERVER_BOUND_MAP_OTHER, req_msg->from);
			asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);	
			return 3;
		}
		/*
		EN_HERO_BODY=1,			//1:英雄身上 
		EN_HERO_PACK,			//2:英雄背包
		EN_COMM_PACK,			//3: 公共背包
		*/
		//如果有两件，则第一件为脱，第二件为穿
		int index =0;	
		equipItem1.setdetailid(pExchangeEquip[index]->iSortID);
		if (pExchangeEquip[index]->iMode == EN_PUT_ON_EQUIP )
		{
			equipItem1.setmasterid(1,req_msg->from);//应在公共背包
		}
		else if (pExchangeEquip[index]->iMode == EN_GET_OFF_EQUIP )
		{
			equipItem1.setmasterid(req_msg->lHeroID,req_msg->from);//在英雄身上
		}
		if (g_Proxy.db_select(equipItem1) != DB_Proxy::DB_SUCCEED)
		{
			error_Info.serialize(EN_CLIENT_DATA_ERROR, SERVER_BOUND_MAP_OTHER, req_msg->from);
			asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);
			return 3;
		}
		oldEquipItem1.setmasterid(equipItem1.officerid_,equipItem1.playerid_);
		oldEquipItem1.setdetailid(equipItem1.sortid_);
		oneEquip.resetItem(equipItem1.itemid_);
		if (pExchangeEquip[index]->iMode == EN_PUT_ON_EQUIP )
		{
			if (equipItem1.type_ != EN_COMM_PACK)
			{
				error_Info.serialize(EN_CLIENT_DATA_ERROR, SERVER_BOUND_MAP_OTHER, req_msg->from);
				asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);
				return 3;
			}
			if(equipItem1.pos_ <=ACC_3 && !oneEquip.Dress(&officerInfo,&updateOfficerInfo,error_Code))
			{
				error_Info.serialize(error_Code, SERVER_BOUND_MAP_OTHER, req_msg->from);
				asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);
				return 4;
			}
			//穿装备
			if (haveDressEquipOfPostion(req_msg->from,req_msg->lHeroID,equipItem1.pos_,0))
			{
				error_Info.serialize(EN_HAVEDRESSEQUIPOFPOSTION, SERVER_BOUND_MAP_OTHER, req_msg->from);
				asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);
				return 4;
			}
			equipItem1.type_  = EN_HERO_BODY;
			equipItem1.officerid_ = req_msg->lHeroID;
			equipItem1.showpos_ = pExchangeEquip[index]->iShowPos;
		}
		else if (pExchangeEquip[index]->iMode == EN_GET_OFF_EQUIP)
		{
			if (equipItem1.type_ !=EN_HERO_BODY)
			{
				error_Info.serialize(EN_CLIENT_DATA_ERROR, SERVER_BOUND_MAP_OTHER, req_msg->from);
				asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);
				return 3;
			}
			if(equipItem1.pos_ <=ACC_3 && !oneEquip.Undress(&officerInfo,&updateOfficerInfo))
			{
				return 4;
			}
			equipItem1.type_  = EN_COMM_PACK;
			equipItem1.officerid_ = 1;
			equipItem1.showpos_ = pExchangeEquip[index]->iShowPos;
		}
		if (req_msg->iCount == 2)
		{//
			index = 1;
			equipItem2.setdetailid(pExchangeEquip[index]->iSortID);
			if (pExchangeEquip[index]->iMode == EN_PUT_ON_EQUIP )
			{
				equipItem2.setmasterid(1,req_msg->from);//应在公共背包
			}

			if (g_Proxy.db_select(equipItem2) != DB_Proxy::DB_SUCCEED)
			{
				error_Info.serialize(EN_CLIENT_DATA_ERROR, SERVER_BOUND_MAP_OTHER, req_msg->from);
				asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);
				return 3;
			}
			//穿装备
			if (haveDressEquipOfPostion(req_msg->from,req_msg->lHeroID,equipItem2.pos_,equipItem1.sortid_))
			{
				error_Info.serialize(EN_HAVEDRESSEQUIPOFPOSTION, SERVER_BOUND_MAP_OTHER, req_msg->from);
				asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);
				return 4;
			}
			secEquip.resetItem(equipItem2.itemid_);

			oldEquipItem2.setmasterid(equipItem2.officerid_,equipItem2.playerid_);
			oldEquipItem2.setdetailid(equipItem2.sortid_);
			if (pExchangeEquip[index]->iMode == EN_PUT_ON_EQUIP )
			{
				if (equipItem2.type_ != EN_COMM_PACK)
				{
					error_Info.serialize(EN_CLIENT_DATA_ERROR, SERVER_BOUND_MAP_OTHER, req_msg->from);
					asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);
					return 3;
				}
				if(equipItem2.pos_ <=ACC_3 && !secEquip.Dress(&officerInfo,&updateOfficerInfo,error_Code))
				{
					error_Info.serialize(error_Code, SERVER_BOUND_MAP_OTHER, req_msg->from);
					asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);
					return 4;
				}
				equipItem2.type_  = EN_HERO_BODY;
				equipItem2.officerid_ = req_msg->lHeroID;
				equipItem2.showpos_ = pExchangeEquip[index]->iShowPos;
			}

		}
		//已成功
		if (g_Proxy.db_update(oldEquipItem1,equipItem1) != DB_Proxy::DB_SUCCEED)
		{
			error_Info.serialize(EN_CLIENT_DATA_ERROR, SERVER_BOUND_MAP_OTHER, req_msg->from);
			asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);
			LOG_TRACE_EX(false,("玩家 %ld 操作英雄(%d)装备 db_update(oldEquipItem1:%d) error\n",req_msg->from,req_msg->lHeroID,oldEquipItem1.sortid_));
			return 5;
		}
		else if (officerInfo.mapid_)
		{
			if (pExchangeEquip[0]->iMode == EN_PUT_ON_EQUIP)
			{//处理装备技能
				if (oneEquip.getBaseSkill() >0)
				{
					studySkillNotify(officerInfo.mapid_,officerInfo.armyid_,officerInfo.officerid_,oneEquip.getBaseSkill(),oneEquip.getBaseSkillLevel(),CMD_HERO_STUDY_SKILL);
				}
				if (oneEquip.getInalySkill() >0)
				{
					studySkillNotify(officerInfo.mapid_,officerInfo.armyid_,officerInfo.officerid_,oneEquip.getInalySkill(),oneEquip.getInalySkillLevel(),CMD_HERO_STUDY_SKILL);
				}
			}
			else
			{
				if (oneEquip.getBaseSkill() >0)
				{
					studySkillNotify(officerInfo.mapid_,officerInfo.armyid_,officerInfo.officerid_,oneEquip.getBaseSkill(),oneEquip.getBaseSkillLevel(),CMD_HERO_CLEAR_SKILL);
				}
				if (oneEquip.getInalySkill() >0)
				{
					studySkillNotify(officerInfo.mapid_,officerInfo.armyid_,officerInfo.officerid_,oneEquip.getInalySkill(),oneEquip.getInalySkillLevel(),CMD_HERO_CLEAR_SKILL);
				}
				//studySkillNotify(officerInfo.mapid_,officerInfo.armyid_,officerInfo.officerid_,equipItem1.tech_,equipItem1.techlevel_,CMD_HERO_CLEAR_SKILL);
			}
		}
		if (req_msg->iCount == 2 )
		{
			if(g_Proxy.db_update(oldEquipItem2,equipItem2) != DB_Proxy::DB_SUCCEED)
			{
				error_Info.serialize(EN_CLIENT_DATA_ERROR, SERVER_BOUND_MAP_OTHER, req_msg->from);
				asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);
				LOG_TRACE_EX(false,("玩家 %ld 操作英雄(%d)装备 db_update(oldEquipItem2:%d) error\n",req_msg->from,req_msg->lHeroID,oldEquipItem2.sortid_));
				return 5;
			}
			else if (officerInfo.mapid_ )
			{//处理装备技能
				if (pExchangeEquip[1]->iMode == EN_PUT_ON_EQUIP)
				{
					if (oneEquip.getBaseSkill() >0)
					{
						studySkillNotify(officerInfo.mapid_,officerInfo.armyid_,officerInfo.officerid_,oneEquip.getBaseSkill(),oneEquip.getBaseSkillLevel(),CMD_HERO_STUDY_SKILL);
					}
					if (oneEquip.getInalySkill() >0)
					{
						studySkillNotify(officerInfo.mapid_,officerInfo.armyid_,officerInfo.officerid_,oneEquip.getInalySkill(),oneEquip.getInalySkillLevel(),CMD_HERO_STUDY_SKILL);
					}
				}
				else
				{
					if (secEquip.getBaseSkill() >0)
					{
						studySkillNotify(officerInfo.mapid_,officerInfo.armyid_,officerInfo.officerid_,secEquip.getBaseSkill(),secEquip.getBaseSkillLevel(),CMD_HERO_CLEAR_SKILL);
					}
					if (secEquip.getInalySkill() >0)
					{
						studySkillNotify(officerInfo.mapid_,officerInfo.armyid_,officerInfo.officerid_,secEquip.getInalySkill(),secEquip.getInalySkillLevel(),CMD_HERO_CLEAR_SKILL);
					}
					//studySkillNotify(officerInfo.mapid_,officerInfo.armyid_,officerInfo.officerid_,equipItem2.tech_,equipItem2.techlevel_,CMD_HERO_CLEAR_SKILL);
				}
			}
		}
		if (equipItem1.pos_ <=ACC_3)
		{
			updateOfficerInfo.jobid_ = officerInfo.jobid_;
			ProcHeroAttribute(updateOfficerInfo);
			updateOfficerInfo.jobid_ = 0;
			updateOfficerInfo.setmasterid(officerInfo.playerid_);
			updateOfficerInfo.setdetailid(officerInfo.officerid_);
			if (g_Proxy.db_increase(updateOfficerInfo) != DB_Proxy::DB_SUCCEED)
			{
				error_Info.serialize(EN_CLIENT_DATA_ERROR, SERVER_BOUND_MAP_OTHER, req_msg->from);
				asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);
				LOG_TRACE_EX(false,("玩家 %ld 操作英雄(%d)装备 db_update(updateOfficerInfo) error\n",req_msg->from,req_msg->lHeroID));
				return 5;
			}
			else
			{
				LOG_TRACE_EX(false,("玩家%d的英雄%d,装备(%I64d-%I64d)改变后属性值:S:%d,P:%d,C:%d,W:%d\n",req_msg->from,req_msg->lHeroID,equipItem1.itemid_,equipItem2.itemid_,
					updateOfficerInfo.equipstamina_,updateOfficerInfo.equippower_,updateOfficerInfo.equipcelerity_,updateOfficerInfo.equipwit_));
			}
			if (updateOfficerInfo.mapid_ >0 && updateOfficerInfo.state_ ==EN_HAVE_ARMY)
			{
				heroModifyParamEvent modifyOfficerParam;
				modifyOfficerParam.serialize(updateOfficerInfo.playerid_,updateOfficerInfo.officerid_);
				event_notify(updateOfficerInfo.mapid_, (char * )&modifyOfficerParam, modifyOfficerParam.length);// 发往服务器,
			}
		}
	}
	else
	{//背包中的，如果是两个直接交换即可
		equipItem1.setmasterid(req_msg->lHeroID,req_msg->from);
		equipItem1.setdetailid(pExchangeEquip[0]->iSortID);
		if (g_Proxy.db_select(equipItem1) != DB_Proxy::DB_SUCCEED)
		{
			equipItem1.setmasterid(1,req_msg->from);
			equipItem1.setdetailid(pExchangeEquip[0]->iSortID);
			if (g_Proxy.db_select(equipItem1) != DB_Proxy::DB_SUCCEED)
			{
				error_Info.serialize(EN_CLIENT_DATA_ERROR, SERVER_BOUND_MAP_OTHER, req_msg->from);
				asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);
				return 5;
			}
		}
		oldEquipItem1.setmasterid(equipItem1.officerid_,equipItem1.playerid_);
		oldEquipItem1.setdetailid(equipItem1.sortid_);
		oldEquipItem1.type_ = equipItem1.type_;
		oldEquipItem1.showpos_ = equipItem1.showpos_;

		if (req_msg->iCount == 1)
		{
			equipItem1.type_ = pExchangeEquip[0]->iType;
			equipItem1.officerid_ = req_msg->lHeroID;
			equipItem1.showpos_ = pExchangeEquip[0]->iShowPos;
			if (equipItem1.type_ == EN_COMM_PACK)
			{
				equipItem1.officerid_ = 1;
			}
			if (g_Proxy.db_update(oldEquipItem1,equipItem1) != DB_Proxy::DB_SUCCEED)
			{
				error_Info.serialize(EN_CLIENT_DATA_ERROR, SERVER_BOUND_MAP_OTHER, req_msg->from);
				asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);
				LOG_TRACE_EX(false,("玩家 %ld 操作背包中的装备 db_update(oldEquipItem1：%d) error\n",req_msg->from,req_msg->lHeroID,oldEquipItem1.sortid_));
				return 5;
			}
		}
		else if (req_msg->iCount == 2)
		{
			equipItem2.setmasterid(req_msg->lHeroID,req_msg->from);
			equipItem2.setdetailid(pExchangeEquip[1]->iSortID);
			if (g_Proxy.db_select(equipItem2) != DB_Proxy::DB_SUCCEED)
			{
				equipItem2.setmasterid(1,req_msg->from);
				equipItem2.setdetailid(pExchangeEquip[1]->iSortID);
				if (g_Proxy.db_select(equipItem2) != DB_Proxy::DB_SUCCEED)
				{
					error_Info.serialize(EN_CLIENT_DATA_ERROR, SERVER_BOUND_MAP_OTHER, req_msg->from);
					asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);
					return 5;
				}
			}
			oldEquipItem2.setmasterid(equipItem2.officerid_,equipItem2.playerid_);
			oldEquipItem2.setdetailid(equipItem2.sortid_);
			oldEquipItem2.type_ = equipItem2.type_;
			oldEquipItem2.showpos_ = equipItem2.showpos_;
			//交换两个装备的位置
			equipItem1.officerid_ = equipItem2.officerid_;
			equipItem1.type_ = equipItem2.type_;
			equipItem1.showpos_ = equipItem2.showpos_;


			equipItem2.officerid_ = oldEquipItem1.officerid_;
			equipItem2.type_ = oldEquipItem1.type_;
			equipItem2.showpos_ = oldEquipItem1.showpos_;
			if (g_Proxy.db_update(oldEquipItem1,equipItem1) != DB_Proxy::DB_SUCCEED)
			{
				error_Info.serialize(EN_CLIENT_DATA_ERROR, SERVER_BOUND_MAP_OTHER, req_msg->from);
				asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);
				LOG_TRACE_EX(false,("玩家 %ld 操作背包中的装备 db_update(oldEquipItem1：%d) error\n",req_msg->from,req_msg->lHeroID,oldEquipItem1.sortid_));
				return 5;
			}
			if (g_Proxy.db_update(oldEquipItem2,equipItem2) != DB_Proxy::DB_SUCCEED)
			{
				error_Info.serialize(EN_CLIENT_DATA_ERROR, SERVER_BOUND_MAP_OTHER, req_msg->from);
				asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);
				LOG_TRACE_EX(false,("玩家 %ld 操作背包中的装备 db_update(oldEquipItem1：%d) error\n",req_msg->from,req_msg->lHeroID,oldEquipItem2.sortid_));
				return 5;
			}
		}
	}
	error_Info.serialize(EN_SUCCESSED, SERVER_BOUND_MAP_OTHER, req_msg->from);
	asynch_write(req_msg->agent, (char *)&error_Info, error_Info.length);	
	return 0;
}
//英雄战争记录
int COtherManager::OnHeroWarRecord(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize != sizeof(GET_OFFICER_INFO))
	{
		LOG_TRACE_EX(false,("COtherManager::OnHeroWarRecord-->消息包非法：玩家%d\n",lPlayerID));
		return 0;
	}

	GET_OFFICER_INFO * pOfficerInfo = (GET_OFFICER_INFO *)pDataBuffer;
	char * buf=0;
	int count=0,iItem=0,n=0,ret=0;
	ERROR_INFO ErrorInfo;
	//查询该英雄的战争记录
	WarNote Record;
	Record.setmasterid(lPlayerID, pOfficerInfo->lOfficerID);
	ret = g_Proxy.db_multi_select(Record, buf, count);
	if (ret == DB_Proxy::DB_EOF)
	{
		return ret;
	}
	else if (ret == DB_Proxy::DB_FAILED)
	{
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	iItem = count/sizeof(WarNote);
	WarNote *pRecord = (WarNote *)buf;
	char * tmp = new char[sizeof(HERO_WAR_RECORD_LIST)+sizeof(tagHeroWarRecord)*iItem];
	HERO_WAR_RECORD_LIST * pRecordList = (HERO_WAR_RECORD_LIST *)tmp;
	__int64 baseTime = IceUtil::Time::Date(2000, 1, 1).toSeconds();
	for (n=0; n < iItem; n++)
	{
		(*pRecordList)[n].iSortID = pRecord[n].sortid_;
		(*pRecordList)[n].lEnterDate = pRecord[n].enterdate_ - baseTime;
		(*pRecordList)[n].lMap = pRecord[n].mapid_;
		(*pRecordList)[n].iEnemyCount = pRecord[n].enemycount_;
		(*pRecordList)[n].iGeneralCount = pRecord[n].generalcount_;
		(*pRecordList)[n].lExitDate = pRecord[n].exitdate_ - baseTime;
		(*pRecordList)[n].iState = pRecord[n].state_;
		(*pRecordList)[n].lFreeDate = pRecord[n].freedate_;
		(*pRecordList)[n].lArmyID = 0;//pRecord[n].armyid_;
		strncpy((*pRecordList)[n].Armyname,pRecord[n].armyname_,SIZE_32);
	}
	pRecordList->serialize(n, SERVER_BOUND_MAP_OTHER, lPlayerID);
	ret = asynch_write(handle, (char *)pRecordList, pRecordList->length);
	if (buf != NULL)
	{
		delete [] buf;
		buf = 0;
	}
	//add by xujincai 20081215
	if(tmp)
	{
		delete[] tmp;
		tmp = 0;
	}
	return ret;
}
//英雄成长记录
int COtherManager::OnHeroGrowUpRecord(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize != sizeof(GET_OFFICER_INFO))
	{
		LOG_TRACE_EX(false,("COtherManager::OnHeroGrowUpRecord-->消息包非法：玩家%d\n",lPlayerID));
		return 0;
	}

	GET_OFFICER_INFO * pOfficerInfo = (GET_OFFICER_INFO *)pDataBuffer;
	char * buf=0;
	int count=0,iItem=0,n=0,ret=0;
	ERROR_INFO ErrorInfo;
	//查询成长记录表
	PullulateNote Record;
	Record.setmasterid(lPlayerID, pOfficerInfo->lOfficerID);
	ret = g_Proxy.db_multi_select(Record, buf, count);
	if (ret == DB_Proxy::DB_EOF)
	{
		return ret;
	}
	else if (ret == DB_Proxy::DB_FAILED)
	{
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	iItem = count/sizeof(PullulateNote);
	PullulateNote *pRecord = (PullulateNote *)buf;
	char * tmp = new char[sizeof(HERO_GROWUP_RECORD_LIST)+sizeof(tagHeroGrowUpRecord)*iItem];
	HERO_GROWUP_RECORD_LIST * pRecordList = (HERO_GROWUP_RECORD_LIST *)tmp;
	__int64 baseTime = IceUtil::Time::Date(2000, 1, 1).toSeconds();
	for (n=0; n < iItem; n++)
	{
		(*pRecordList)[n].iSortID = pRecord[n].sortid_;
		(*pRecordList)[n].iType = pRecord[n].type_;
		(*pRecordList)[n].lGrowUpDate = pRecord[n].upgradedate_ - baseTime;
		(*pRecordList)[n].iLevel = pRecord[n].level_;
	}
	pRecordList->serialize(n, SERVER_BOUND_MAP_OTHER, lPlayerID);
	ret = asynch_write(handle, (char *)pRecordList, pRecordList->length);
	if (buf != NULL)
	{
		delete [] buf;
		buf = 0;
	}
	//add by xujincai 20081215
	if(tmp)
	{
		delete[] tmp;
		tmp = 0;
	}
	return ret;
}
//修改指挥官名字
void COtherManager::OnModifyOfficerName(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize != sizeof(MODIFY_OFFICER_NAME))
	{
		LOG_TRACE_EX(false,("COtherManager::OnModifyOfficerName-->消息包非法：玩家%d\n",lPlayerID));
		return ;
	}

	MODIFY_OFFICER_NAME * pOfficerName = (MODIFY_OFFICER_NAME *)pDataBuffer;
	PlayerOfficerTable OfficerName;
	memset(&OfficerName, 0, sizeof(PlayerOfficerTable));

	OfficerName.setmasterid(lPlayerID);
	OfficerName.setdetailid(pOfficerName->lOfficerID);
	if (g_Proxy.db_select(OfficerName) != DB_Proxy::DB_SUCCEED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_RECRUIT_OFFICER_PLAYERID, SERVER_BOUND_MAP_OTHER, lPlayerID);

		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("COtherManager::OnModifyOfficerName->玩家 %d  的指挥官 %d ID不存在! \n",lPlayerID,pOfficerName->lOfficerID));
		return;
	}
	strcpy(OfficerName.officername_, pOfficerName->szOfficerName);
	
	if (g_Proxy.db_update(OfficerName,OfficerName) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("COtherManager::OnModifyOfficerName->玩家 %d  修改指挥官名字 %d  Failad! \n",lPlayerID,pOfficerName->lOfficerID));
	}
}
//指挥官列表
void COtherManager::OnOfficerList(int handle, unsigned long lPlayerID)
{
	char * Buffer=0;
	int iCount = 0 ,iItem = 0,n = 0;
	PlayerOfficerTable Officer;
	memset(&Officer, 0, sizeof(Officer));

	Officer.setmasterid(lPlayerID);
	if (g_Proxy.db_multi_select(Officer, Buffer, iCount)!= DB_Proxy::DB_SUCCEED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, SERVER_BOUND_MAP_OTHER, lPlayerID);
		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return;
	}
	iItem = iCount/sizeof(PlayerOfficerTable);
	PlayerOfficerTable * pOfficer = (PlayerOfficerTable *)Buffer;

	 
	char * tmp = new char[sizeof(PLAYER_OFFICER_LIST) + sizeof(tagPlayer_Officer_List)*(iItem+1)];
	PLAYER_OFFICER_LIST *pOfficerList = (PLAYER_OFFICER_LIST *)tmp;

	//指挥官列表信息
	for (n = 0; n < iItem; n++)
	{
		(*pOfficerList)[n].lOfficerID = pOfficer[n].officerid_;
		(*pOfficerList)[n].iLevel = pOfficer[n].level_;
		(*pOfficerList)[n].iFaceID = pOfficer[n].faceid_;
		(*pOfficerList)[n].iRaceID = pOfficer[n].officerrace_;
		(*pOfficerList)[n].iJobID = pOfficer[n].jobid_;
		(*pOfficerList)[n].iMilitaryRank = pOfficer[n].militaryrank_;
		if(pOfficer[n].state_ <0)
		{
			(*pOfficerList)[n].iState = 0;
		}
		else
		{
			(*pOfficerList)[n].iState = pOfficer[n].state_;
		}
		(*pOfficerList)[n].iTechDon = pOfficer[n].techdot_;
		(*pOfficerList)[n].iType = 0;
		(*pOfficerList)[n].lTime = 0;
		strcpy((*pOfficerList)[n].szOfficerName, pOfficer[n].officername_);
	}
	map<unsigned long, __int64>::iterator it = m_mapRecruitingOfficer.find(lPlayerID);
	if (it != m_mapRecruitingOfficer.end())
	{
		n--;
		(*pOfficerList)[n].iType = 1;
		(*pOfficerList)[n].lTime =(long)( it->second - IceUtil::Time::now().toSeconds());
		n++;
	}
	pOfficerList->serialize(n, SERVER_BOUND_MAP_OTHER, lPlayerID, EN_S_ALL_OFFICERS);
	int ret = asynch_write(handle, (char *)pOfficerList, pOfficerList->length);
	if (Buffer != NULL)
	{
		delete [] Buffer;
		Buffer = 0;
	}
	if (tmp != NULL)
	{
		delete [] tmp;	
		tmp = 0;
	}
	QueryCaptiveList_Request querycaptive;
	querycaptive.serialize(lPlayerID);
	querycaptive.agent = handle;
	OnQueryCaptiveList((char*)&querycaptive);
}
//招募指挥官
int COtherManager::OnRecruitOfficer(int handle,unsigned long lPlayerID)
{
	int ret = 0;
	ERROR_INFO ErrorInfo;
	
	//正在招募英雄...
	map<unsigned long, __int64>::iterator it = m_mapRecruitingOfficer.find(lPlayerID);
	if (it != m_mapRecruitingOfficer.end())
	{
		ErrorInfo.serialize(EN_RECRUITING_HERO, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	char * buf = 0;
	int count = 0,iDataBase = 0;
	PlayerOfficerTable PlayerTable;
	memset(&PlayerTable, 0, sizeof(PlayerOfficerTable));

	PlayerTable.setmasterid(lPlayerID);
	iDataBase = g_Proxy.db_multi_select(PlayerTable, buf, count);
	if (iDataBase == DB_Proxy::DB_EOF)
	{
		ErrorInfo.serialize(EN_RECRUIT_OFFICER_PLAYERID, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);

		LOG_TRACE_EX(false,("招募指挥官 -- 没有此玩家的ID (%ld) 在 PlayerOfficerTable 表中\n",lPlayerID));
		return ret;
	}
	else if (iDataBase == DB_Proxy::DB_FAILED)
	{
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, SERVER_BOUND_MAP_OTHER, lPlayerID);

		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);

		LOG_TRACE_EX(false,("招募指挥官 -- 数据库连接失败, 玩家的ID (%ld) 在 PlayerOfficerTable 表中\n",lPlayerID));
		return ret;
	}
	//根据军衔判断是否可以招募英雄，军衔影响英雄数量
	int iMilitaryRank = 0,iState = 0,iHeroID = 0;
	int iItem = count/sizeof(PlayerOfficerTable);
	PlayerOfficerTable * pHeamadHero = (PlayerOfficerTable *)buf;
	PlayerOfficerTable *pMainHero = 0;
	for(int i = 0; i < iItem; i++)
	{
		if (pHeamadHero[i].officerid_ == lPlayerID)
		{
			iMilitaryRank = pHeamadHero[i].militaryrank_;
			iState = pHeamadHero[i].state_;
			pMainHero = &pHeamadHero[i];
		}
		else
		{
			if (pHeamadHero[i].officerid_ > iHeroID)
				iHeroID = pHeamadHero[i].officerid_;
		}
	}
	/*根据配置表获取军衔和英雄数量的关系*/
	LPQuantityByRank_Item pQuantityByRank = g_LoadDBinfo.GetQuantityByRank(iMilitaryRank);

	if(pQuantityByRank == NULL)
	{
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
		ErrorInfo.serialize(EN_OFFICER_COUNT_UPPER, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	
	if (iItem >= pQuantityByRank->HeroQuantity)//此玩家的指挥官数已达到上限,不能再招募指挥官
	{
		ErrorInfo.serialize(EN_OFFICER_COUNT_UPPER, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		if (buf) delete [] buf;
		return ret;
	}
	//根据英雄数量，计算招募的费用
	int iMoney = 200;
	std::map<int,int>::iterator iter = m_mapRecruitMoney.find(iItem);
	if (iter != m_mapRecruitMoney.end())
	{
		iMoney = iter->second;
	}
	PlayerChatRoomTable PlayerCountry;
	memset(&PlayerCountry, 0, sizeof(PlayerCountry));
	PlayerCountry.setid(lPlayerID);
	if (g_Proxy.db_select(PlayerCountry) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("%ld 招募指挥官时 -- 获取玩家国籍失败\n",lPlayerID));
		if (buf) delete [] buf;
		return 0;
	}
	PlayerCountry.money_ -= iMoney;

	if (PlayerCountry.money_ < 0)//金钱不够
	{
		ErrorInfo.serialize(EN_LEVY_MONEY, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		if (buf) delete [] buf;
		return ret;
	}

	if (g_Proxy.db_update(PlayerCountry,PlayerCountry) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("招募指挥官 db_update(PlayerCountry,PlayerCountry) 玩家 %ld (国籍%d) 招募到了个指挥官 %ld 种族 %d  error\n",lPlayerID,PlayerCountry.country_,
		PlayerTable.officerid_,PlayerTable.officerrace_ ));
		if (buf) delete [] buf;
		return ret;
	}
	SkillDetail * skillDetail  = 0;
	//根据特殊英难概率，抽出特殊英雄，还是普通英雄
	int iSpecialHeroID=-1;
	if (IceUtil::random(1000)<g_IniFile.m_SpecialHeroRate) 
	{//特殊英难，每个玩家拥有的特殊英雄不能重复
		bool bexist=false;
		map<int,tagSpecialHero>::iterator heroit;
		while(1)
		{
			iSpecialHeroID=IceUtil::random((int)m_mapSpecialHero.size())+1001;
			heroit=m_mapSpecialHero.find(iSpecialHeroID);
			if (heroit==m_mapSpecialHero.end()) continue;
			if ((PlayerCountry.country_==1 && heroit->second.RaceID>4) || (PlayerCountry.country_==2 && heroit->second.RaceID<=4)) continue;
			
			bexist=false;
			for (int i=0;i<iItem;i++)
			{
				if ((pHeamadHero[i].officerid_!=lPlayerID) && (strcmp(pHeamadHero[i].officername_,heroit->second.HeroName)==0))
				{
					bexist=true;
					break;
				}
			}
			if (bexist) continue;
			break;
		}

		//插入指挥官表
		memset(&PlayerTable, 0, sizeof(PlayerOfficerTable));
		PlayerTable.playerid_=lPlayerID;
		PlayerTable.officerid_=iHeroID+11;
		strcpy(PlayerTable.officername_,heroit->second.HeroName);
		PlayerTable.jobid_=heroit->second.JodID;
		PlayerTable.officerrace_=heroit->second.RaceID;
		PlayerTable.stamina_= getRandomValue(heroit->second.minStamina,heroit->second.maxStamina);//heroit->second.Stamina;
		PlayerTable.power_= getRandomValue(heroit->second.minPower,heroit->second.maxPower);//heroit->second.Power;
		PlayerTable.celerity_= getRandomValue(heroit->second.minCelerity,heroit->second.maxCelerity);//heroit->second.Celerity;
		PlayerTable.wit_= getRandomValue(heroit->second.minWit,heroit->second.maxWit);//heroit->second.Wit;
		PlayerTable.level_=1;
		PlayerTable.techdot_=5;
		PlayerTable.faceid_ = heroit->second.FaceID;
		skillDetail = g_LoadDBinfo.getSkillDetail((SKILL_TYPE)heroit->second.TechID,heroit->second.TechLevel);//获取技能的配置参数
	}
	else//普通英雄			
	{
		//随机事件
		memset(&PlayerTable, 0, sizeof(PlayerOfficerTable));
		
		PlayerTable.playerid_ = lPlayerID;
		PlayerTable.officerid_ = iHeroID+11;//确定英雄ID
		PlayerTable.mapid_ = 0;
		PlayerTable.cityid_ = 0;
		PlayerTable.level_ = 1;
		PlayerTable.exp_ = 0;

		PlayerTable.militaryrank_ = 0;
		PlayerTable.credit_ = 0;
		PlayerTable.enemycount_ = 0;
		PlayerTable.generalcount_ = 0;
		PlayerTable.state_ = 0;
		//根据阵营随机种族
		if (PlayerCountry.country_ == 1)
			PlayerTable.officerrace_ = 1+rand()%3;
		else
			PlayerTable.officerrace_ = 5+rand()%3;

		//随机职业
		int jobrate=rand()%1000;
		if (jobrate<g_IniFile.m_zsRate)
			PlayerTable.jobid_ =2;
		else if (jobrate<g_IniFile.m_ssRate+g_IniFile.m_zsRate)
			PlayerTable.jobid_=3;
		else if (jobrate<g_IniFile.m_msRate+g_IniFile.m_ssRate+g_IniFile.m_zsRate)
			PlayerTable.jobid_=5;
		else PlayerTable.jobid_=4;
		//PlayerTable.jobid_ = 1+rand()%5;

		int iJob = 0;
		while(1)
		{
			switch(PlayerTable.jobid_)
			{
			case 1:
				iJob = m_mapJobParam[PlayerTable.officerrace_-1].iKnight;
				break;
			case 2:
				iJob = m_mapJobParam[PlayerTable.officerrace_-1].iSoldier;
				break;
			case 3:
				iJob = m_mapJobParam[PlayerTable.officerrace_-1].iShooter;
				break;
			case 4:
				iJob = m_mapJobParam[PlayerTable.officerrace_-1].iMaster;
				break;
			case 5:
				iJob = m_mapJobParam[PlayerTable.officerrace_-1].iPastor;
				break;
			};
			if (iJob == 1) break;
			jobrate=rand()%1000;
			if (jobrate<g_IniFile.m_zsRate)
				PlayerTable.jobid_ =2;	//战士
			else if (jobrate<g_IniFile.m_ssRate+g_IniFile.m_zsRate)
				PlayerTable.jobid_=3;	//射手
			else if (jobrate<g_IniFile.m_msRate+g_IniFile.m_ssRate+g_IniFile.m_zsRate)
				PlayerTable.jobid_=5;	//牧师
			else PlayerTable.jobid_=4;	//法师
			//PlayerTable.jobid_ = 1+rand()%5;
		}
		switch(PlayerTable.officerrace_)
		{
		case 1:
			PlayerTable.faceid_ = 1101+rand()%5;
			break;
		case 2:
			PlayerTable.faceid_ = 2201+rand()%5;
			break;
		case 3:
			PlayerTable.faceid_ = 3101+rand()%5;
			break;
		case 4:
			PlayerTable.faceid_ = 4101+rand()%5;
			break;
		case 5:
			PlayerTable.faceid_ = 5101+rand()%5;
			break;
		case 6:
			PlayerTable.faceid_ = 6101+rand()%5;
			break;
		case 7:
			PlayerTable.faceid_ = 7201+rand()%5;
			break;
		case 8:
			PlayerTable.faceid_ = 8101+rand()%5;
			break;
		};
		if (PlayerTable.faceid_ < 1101) PlayerTable.faceid_ = 1101;
		//性别
		int iSex = 20;
		if (PlayerTable.officerrace_ == 2 || PlayerTable.officerrace_ == 7)
			iSex = 259;

		PlayerTable.stamina_ += int((rand()%11*0.1+0.3)*m_mapRaceParam[PlayerTable.officerrace_].iEndurance);
		PlayerTable.power_ += int((rand()%11*0.1+0.3)*m_mapRaceParam[PlayerTable.officerrace_].iStrength);
		PlayerTable.celerity_ += int((rand()%11*0.1+0.3)*m_mapRaceParam[PlayerTable.officerrace_].iAgility);
		PlayerTable.wit_ += int((rand()%11*0.1+0.3)*m_mapRaceParam[PlayerTable.officerrace_].iIntelligence);
		PlayerTable.techdot_ = 5;
		
		char szName[24];
		memset(szName, 0, sizeof(szName));
		sprintf(szName, "%s · %s", m_mapHeroName[rand()%20].c_str(), m_mapHeroName[iSex+rand()%230].c_str());
		strcpy(PlayerTable.officername_, szName);
		PlayerTable.armyid_ = 0;
		PlayerTable.state_ = 0;
	}
	//根据一级属性，计算二级属性
	ProcHeroAttribute(PlayerTable);
	PlayerTable.grouprate_ = g_LoadDBinfo.getHeroGroupRate();
	iDataBase = g_Proxy.db_insert(PlayerTable);
	
	if (iDataBase == DB_Proxy::DB_SUCCEED)
	{		
		/*20100408记录日志*/
		ISystemLog::getInstance()->heroAttribute_Log(PlayerTable.playerid_,PlayerTable.officerid_,PlayerTable.officername_,
			PlayerTable.officerrace_,PlayerTable.jobid_,PlayerTable.faceid_,PlayerTable.level_,PlayerTable.stamina_,
			PlayerTable.power_,PlayerTable.celerity_,PlayerTable.wit_);
		ISystemLog::getInstance()->playerMoney_Recruit_Log(pMainHero->playerid_,pMainHero->officername_,
			PlayerCountry.money_ + iMoney,PlayerCountry.money_,iMoney,PlayerTable.officerid_,PlayerTable.jobid_,
			PlayerTable.officerrace_,PlayerTable.officerrace_,(iSpecialHeroID<=0)?false:true);
		ISystemLog::getInstance()->playerHeros_Recruit_Log(pMainHero->playerid_,pMainHero->officername_,iItem,iItem+1,
			PlayerTable.officerid_,PlayerTable.officername_,(iSpecialHeroID<=0)?false:true);
		
		if (skillDetail)
		{//需要增加技能,针对特殊英雄
			PlayerSkillTable officerSkill;
			officerSkill.setmasterid(PlayerTable.playerid_,PlayerTable.officerid_);
			officerSkill.setdetailid(skillDetail->skillID);
			officerSkill.skilltype_ = skillDetail->skillType;
			officerSkill.skilllevel_ = skillDetail->level;
			if (g_Proxy.db_insert(officerSkill) != DB_Proxy::DB_SUCCEED)
			{
				LOG_TRACE_EX(false,("招募指挥官 -- 增加英雄的默认技能时失败： 玩家的ID (%ld) \n",lPlayerID));
			}
		}
		//增加英雄初始精专技能
		OfficerInitSkill *officerInitSkill = NULL;
		OfficerInitSkill_MultiMap_iterator lowerIter,upperIter;
		lowerIter = g_LoadDBinfo.getOfficerInitSkill_Lowerbound(PlayerTable.jobid_);
		upperIter = g_LoadDBinfo.getOfficerInitSkill_Upperbound(PlayerTable.jobid_);
		for(;lowerIter != upperIter;lowerIter++)
		{
			officerInitSkill = lowerIter->second;
			if (officerInitSkill == NULL)
			{
				continue;
			}
			skillDetail = g_LoadDBinfo.getSkillDetail((SKILL_TYPE)officerInitSkill->skill,officerInitSkill->skillLevel);

			if(skillDetail)
			{
				PlayerSkillTable officerSkill;
				officerSkill.setmasterid(PlayerTable.playerid_,PlayerTable.officerid_);
				officerSkill.setdetailid(skillDetail->skillID);
				officerSkill.skilltype_ = skillDetail->skillType;
				officerSkill.skilllevel_ = skillDetail->level;
				if (g_Proxy.db_insert(officerSkill) != DB_Proxy::DB_SUCCEED)
				{		
					LOG_TRACE_EX(false,("给玩家 %ld 的首领 %d 添加默认精专技能(%d-%d)时失败!\n",
						PlayerTable.playerid_,PlayerTable.officerid_,skillDetail->skillID,skillDetail->level));
				}
			}
		}
		char * tmp = new char[sizeof(PLAYER_OFFICER_LIST) + sizeof(tagPlayer_Officer_List)];
		PLAYER_OFFICER_LIST *pOfficerList = (PLAYER_OFFICER_LIST *)tmp;

		//指挥官列表信息
		int n = 0;
		(*pOfficerList)[n].lOfficerID = PlayerTable.officerid_;
		(*pOfficerList)[n].iLevel = PlayerTable.level_;
		(*pOfficerList)[n].iRaceID = PlayerTable.officerrace_;
		(*pOfficerList)[n].iJobID = PlayerTable.jobid_;
		(*pOfficerList)[n].iMilitaryRank = PlayerTable.militaryrank_;
		(*pOfficerList)[n].iState = PlayerTable.state_;
		(*pOfficerList)[n].iFaceID = PlayerTable.faceid_;
		(*pOfficerList)[n].iTechDon = PlayerTable.techdot_;
		strcpy((*pOfficerList)[n].szOfficerName, PlayerTable.officername_);
		
		pOfficerList->serialize(1, SERVER_BOUND_MAP_OTHER, lPlayerID, EN_S_RECRUIT_A_OFFICER);

		ret = asynch_write(handle, (char *)pOfficerList, pOfficerList->length);

		if(tmp)
		{
			delete [] tmp;
			tmp  =0;
		}
		//该英雄身上身上应没装备
		PlayerItemStorageTable officeritem;
		officeritem.setmasterid(PlayerTable.officerid_,lPlayerID);
		if (g_Proxy.db_multi_select(officeritem,tmp,iItem) == DB_Proxy::DB_SUCCEED)
		{
			PlayerItemStorageTable *pItemList = (PlayerItemStorageTable*)tmp;
			iItem = iItem/sizeof(PlayerItemStorageTable);
			for (int i=0;i<iItem;i++)
			{
				g_Proxy.db_delete(pItemList[i]);
			}
			delete[] tmp;
			tmp = 0;
		}
		//发送广播iSpecialHeroID,EN_S_BROAD_SPECIALHERO 4384
		Broad_SpecialHero broadMsg;
		if (iSpecialHeroID >0)
		{
			broadMsg.serialize(pMainHero->officername_,PlayerCountry.country_,iSpecialHeroID,PlayerTable.stamina_ + PlayerTable.power_ + PlayerTable.celerity_ + PlayerTable.wit_);

			broadcast((char*)&broadMsg,broadMsg.length);
		}
		else
		{
			BroadMsgParam *pBroadMsgParam = g_LoadDBinfo.getBroadMsgParam(EN_S_BROAD_SPECIALHERO);
			if (pBroadMsgParam)
			{
				iSpecialHeroID = PlayerTable.stamina_ + PlayerTable.power_ + PlayerTable.celerity_ + PlayerTable.wit_;
				if (iSpecialHeroID <= pBroadMsgParam->minLevel || iSpecialHeroID >= pBroadMsgParam->maxLevel)
				{
					broadMsg.serialize(pMainHero->officername_,PlayerCountry.country_,0,iSpecialHeroID);
					broadcast((char*)&broadMsg,broadMsg.length);
				}
			}
		}
	}
	else if (iDataBase == DB_Proxy::DB_FAILED)
	{
		if (buf != NULL)   //玩家指挥官的数据记录
		{
			delete [] buf;
			buf = 0;
		}
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, SERVER_BOUND_MAP_OTHER, lPlayerID);

		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);

		LOG_TRACE_EX(false,("招募指挥官 -- 数据库连接失败, 玩家的ID (%ld) 在 PlayerOfficerTable 表中\n",lPlayerID));
		return ret;
	}
	else
	{
		LOG_TRACE_EX(false,("玩家 %ld (国籍%d) 招募到了1个指挥官 %ld 种族 %d ,但插入数据失败\n",lPlayerID,PlayerCountry.country_,
			PlayerTable.officerid_,PlayerTable.officerrace_ ));
	}

	if (buf != NULL)   //玩家指挥官的数据记录
	{
		delete [] buf;
		buf = 0;
	}

	//招募倒计时
	IceUtil::Time ttt = IceUtil::Time::seconds(3600 + IceUtil::Time::now().toSeconds());
	__int64 lTimeLen = IceUtil::Time::now().toSeconds()+3600;
	RECRUITING_HERO * pTimer = new RECRUITING_HERO;
	pTimer->serialize (lPlayerID, PlayerTable.officerid_, PlayerCountry.country_, EN_TIMER_RECRUITING_HERO);
	set_timer(3600, (const void *)pTimer);
	m_mapRecruitingOfficer.insert(make_pair(lPlayerID, lTimeLen));

	HERO_SYSTEM_INFO HeadamHero;
	HeadamHero.serialize(EN_S_START_RECRUIT_HERO, SERVER_BOUND_MAP_OTHER, lPlayerID, EN_S_HERO_SYSTEM_INFO);
	ret = asynch_write(handle, (char *)&HeadamHero, HeadamHero.length);

	return ret;
	
}
//招募英雄时间事件
int COtherManager::OnRecruitHeroEvent(const void * pDataBuffer, int iDataSize)
{
	if (iDataSize!=sizeof(RECRUITING_HERO))
	{
		LOG_TRACE_EX(false,("COtherManager::OnRecruitHeroEvent-->消息包非法\n"));
		return 0;
	}

	RECRUITING_HERO * pHero = (RECRUITING_HERO *)pDataBuffer;
	unsigned long lPlayerID = pHero->lPlayerID;
	int iItem = pHero->iSortID;
	int iCountry = pHero->iCountry;
	int ret = 0;
	m_mapRecruitingOfficer.erase(lPlayerID);
	return ret;
}
//开除英雄
int COtherManager::OnDismissHero(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize != sizeof(DISMISS_HERO))
	{
		LOG_TRACE_EX(false,("COtherManager::OnDismissHero-->消息包非法：玩家%d\n",lPlayerID));
		return 0;
	}

	int ret = 0;
	DISMISS_HERO * pDismiss = (DISMISS_HERO *)pDataBuffer;
	ERROR_INFO ErrorInfo;
	if (pDismiss->lHeroID == lPlayerID)
	{
		ErrorInfo.serialize(EN_CANNOT_DISMISS_HEADER_HERO, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		
		return ret;
	}
	
	if (pDismiss->lHeroID <= 0)
	{
		ErrorInfo.serialize(EN_OFFICER_ID_ERROR, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
		return ret;
	}
	PlayerOfficerTable PlayerOfficer;
	memset(&PlayerOfficer, 0, sizeof(PlayerOfficerTable));

	//只能开除空闲的指挥官
	PlayerOfficer.setmasterid(lPlayerID);
	PlayerOfficer.setdetailid(pDismiss->lHeroID);
	ret = g_Proxy.db_select(PlayerOfficer);
	if (ret == DB_Proxy::DB_EOF)
	{
		ErrorInfo.serialize(EN_NOT_HERO_ID, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("%ld开除英雄 -- 没有此英雄ID (%ld) 在 PlayerOfficerTable 表中\n",lPlayerID,pDismiss->lHeroID));
		return ret;
	}
	else if (ret == DB_Proxy::DB_FAILED)
	{
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("开除英雄 -- 数据库连接失败, 玩家的ID (%ld) 英雄ID (%ld)在 PlayerOfficerTable 表中\n",lPlayerID,pDismiss->lHeroID));
		
		return ret;
	}
	
	if (PlayerOfficer.state_ == EN_HAVE_ARMY)
	{
		ErrorInfo.serialize(EN_HERO_HAVE_ARMY, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	else if (PlayerOfficer.state_ == EN_BY_CAPTIVE)
	{
		ErrorInfo.serialize(EN_HERO_BY_CAPTIVE, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	else if (PlayerOfficer.state_ == EN_CAPTIVE)
	{
		ErrorInfo.serialize(EN_HERO_CAPTIVE, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	else if(PlayerOfficer.state_ == EN_GBH)
	{
		ErrorInfo.serialize(EN_HERO_GBH, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}

	PlayerOfficer.setmasterid(lPlayerID);
	PlayerOfficer.setdetailid(pDismiss->lHeroID);
	ret = g_Proxy.db_delete(PlayerOfficer);
	
	if (ret == DB_Proxy::DB_FAILED)
	{
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, SERVER_BOUND_MAP_OTHER, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("开除英雄 -3- 数据库连接失败, 玩家的ID (%ld) 英雄ID (%ld)在 g_Proxy.db_delete(PlayerOfficer) \n",lPlayerID,pDismiss->lHeroID));
		
		return ret;
	}
	//开除英雄成功
	HERO_SYSTEM_INFO Hero;
	Hero.serialize(EN_S_DISMISS_HERO_SUCCEED, SERVER_BOUND_MAP_OTHER, lPlayerID, EN_S_HERO_SYSTEM_INFO);
	ret = asynch_write(handle, (char *)&Hero, Hero.length);
	//删除技能
	clearAllOfficerSkill(lPlayerID,pDismiss->lHeroID);

	/*正在招募英雄...
	*/
	char * buf=0;
	int count=0,iItem=0;
	PlayerItemStorageTable HeroPack;

	HeroPack.setmasterid(pDismiss->lHeroID, lPlayerID);
	ret = g_Proxy.db_multi_select(HeroPack, buf, count);
	if (ret == DB_Proxy::DB_SUCCEED)
	{
		iItem = count/sizeof(PlayerItemStorageTable);
		PlayerItemStorageTable * pHeroPack = (PlayerItemStorageTable *)buf;
		for (int n=0; n < iItem; n++)
		{
			pHeroPack[n].setdetailid(pHeroPack[n].sortid_);
			pHeroPack[n].setmasterid(pHeroPack[n].officerid_ ,pHeroPack[n].playerid_);
			g_Proxy.db_delete(pHeroPack[n]);
		}
	}

	if (buf != NULL)
	{
		delete [] buf;
		buf = 0;
	}
	//20100408增加日志
	PlayerOfficerTable allHero;
	allHero.setmasterid(lPlayerID);
	iItem = 0;
	PlayerOfficerTable *pMainHero = 0;
	PlayerOfficerTable *pHeroList = 0;
	if (g_Proxy.db_multi_select(allHero,buf,iItem) == DB_Proxy::DB_SUCCEED)
	{
		iItem = iItem / sizeof(PlayerOfficerTable);
		pHeroList = (PlayerOfficerTable *)buf;
		for (int i=0;i<iItem;++i)
		{
			if (pHeroList[i].playerid_ == pHeroList[i].officerid_)
			{
				pMainHero = &pHeroList[i];
				break;
			}
		}
		if (pMainHero)
		{
			ISystemLog::getInstance()->playerHeros_Fire_Log(pMainHero->playerid_,pMainHero->officername_,iItem+1,iItem,
				PlayerOfficer.officerid_,PlayerOfficer.officername_);
		}
		if (buf)
		{
			delete[] buf;
			buf = 0;
		}
	}
	return ret;
}

//删除英雄的所有技能
void COtherManager::clearAllOfficerSkill(int playerid,int officerid)
{
	PlayerSkillTable officerSkill;
	officerSkill.setmasterid(playerid,officerid);
	char *skillBuf = 0;
	int skillCount = 0;
	if (g_Proxy.db_multi_select(officerSkill,skillBuf,skillCount) != DB_Proxy::DB_SUCCEED)
	{
		return;
	}
	skillCount = skillCount/sizeof(PlayerSkillTable);
	PlayerSkillTable *skillList = (PlayerSkillTable*)skillBuf;
	for (int i=0;i<skillCount;i++)
	{
		if (g_Proxy.db_delete(skillList[i]) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE_EX(false,("clearAllOfficerSkill-->删除玩家%d的英雄%d已学技能%d失败。\n",playerid,officerid,skillList[i].skillid_));
		}
	}
	if (skillBuf)
	{
		delete[] skillBuf;
		skillBuf = 0;
	}
}
//得到指挥官技能
int COtherManager::onOfficerTechList(const char *msg)
{
	GET_OFFICER_INFO *req_msg = (GET_OFFICER_INFO*)msg;
	if (req_msg->length != sizeof(GET_OFFICER_INFO))
	{
		LOG_TRACE_EX(false,("COtherManager::OnOfficerTechList-->消息包非法：玩家%d\n",req_msg->from));
		return 0;
	}

	int ret = 0;
	int playerid = req_msg->from;
	int officerid = req_msg->lOfficerID;
	int handle = req_msg->agent;

	ERROR_INFO ErrorInfo;
	if (officerid <= 0)
	{
		ErrorInfo.serialize(EN_OFFICER_ID_ERROR, SERVER_BOUND_MAP_OTHER, playerid);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
		return ret;
	}
	PlayerOfficerTable officer;

	officer.setmasterid(playerid);
	officer.setdetailid(officerid);
	if (g_Proxy.db_select(officer) != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_FIND_OFFICER_ERROR, SERVER_BOUND_MAP_OTHER, playerid);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
		LOG_TRACE_EX(false,("得到指挥官技能 %ld --%ddb_select(Officer) error \n",playerid,officerid));
		return ret;
	}
	//查询英雄技能表
	PlayerSkillTable officerSkill;
	officerSkill.setmasterid(playerid,officerid);
	char *skillBuf = 0;
	int skillCount = 0;
	if (g_Proxy.db_multi_select(officerSkill,skillBuf,skillCount) == DB_Proxy::DB_FAILED)
	{
		ErrorInfo.serialize(EN_FIND_OFFICER_ERROR, SERVER_BOUND_MAP_OTHER, playerid);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
		LOG_TRACE_EX(false,("得到指挥官技能 %ld --%ddb_select(PlayerSkillTable) error \n",playerid,officerid));
		return ret;
	}
	PlayerSkillTable *skillList = (PlayerSkillTable*)skillBuf;
	skillCount = skillCount/sizeof(PlayerSkillTable);
	char * tmp = new char[sizeof(OFFICER_TECH_LIST) + sizeof(tagOfficerTechList)*skillCount];
	OFFICER_TECH_LIST * pTechList = (OFFICER_TECH_LIST *)tmp;
	for (int i=0;i<skillCount;i++)
	{
		(*pTechList)[i].iTechID = skillList[i].skillid_;
		(*pTechList)[i].Level = skillList[i].skilllevel_;
	}

	pTechList->serialize(officerid,skillCount,SERVER_BOUND_MAP_OTHER,playerid,EN_S_OFFICER_TECH);	 
	ret = asynch_write(handle, (char *)pTechList, pTechList->length);
	if(tmp)
	{
		delete [] tmp;
		tmp = 0;
	}
	if (skillBuf)
	{
		delete[] skillBuf;
		skillBuf = 0;
	}
	return ret;
}
//指挥官学习技能
int COtherManager::onOfficerStudyTech(const char *msg)
{
	STUDY_OFFICER_TECH *req_msg = (STUDY_OFFICER_TECH*)msg;
	if (req_msg->length != sizeof(STUDY_OFFICER_TECH))
	{
		LOG_TRACE_EX(false,("COtherManager::OnOfficerStudyTech-->消息包非法：玩家%d\n",req_msg->from));
		return 0;
	}
	int ret = 0;
	int playerid = req_msg->from;
	int officerid = req_msg->lOfficerID;
	int skillid = req_msg->iTechID;
	int handle = req_msg->agent;
	ERROR_INFO ErrorInfo;
	memset(&ErrorInfo, 0, sizeof(ERROR_INFO));
	if (officerid <= 0)
	{
		ErrorInfo.serialize(EN_OFFICER_ID_ERROR, SERVER_BOUND_MAP_OTHER, playerid);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
		return ret;
	}
	if (skillid<1)
	{
		ErrorInfo.serialize(EN_TECH_ID_ERROR, SERVER_BOUND_MAP_OTHER, playerid);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
		return ret;
	}
	//判断英雄的状态
	PlayerOfficerTable officer,updateOfficer;
	memset(&officer, 0, sizeof(PlayerOfficerTable));

	officer.setmasterid(playerid);
	officer.setdetailid(officerid);
	if (g_Proxy.db_select(officer) != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_FIND_OFFICER_ERROR, SERVER_BOUND_MAP_OTHER, playerid);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
		LOG_TRACE_EX(false,("指挥官学习技能 %ld  db_select(Officer) error \n",playerid));
		return ret;
	}
	if (officer.state_ == EN_BY_CAPTIVE)
	{
		ErrorInfo.serialize(EN_HERO_BY_CAPTIVE, SERVER_BOUND_MAP_OTHER, playerid);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
		return ret;
	}

	if (isInWarMapOfOfficer(&officer))
	{//在战斗中，不能学习
		ErrorInfo.serialize(EN_OFFICER_INWAR, SERVER_BOUND_MAP_OTHER, playerid);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
		return ret;
	}
	//判断英雄是否已学过该技能
	int iCurTechLevel = 0;
	PlayerSkillTable officerSkill;
	officerSkill.setmasterid(playerid,officerid);
	officerSkill.setdetailid(skillid);
	ret = g_Proxy.db_select(officerSkill) ;
	if (ret == DB_Proxy::DB_FAILED)
	{
		ErrorInfo.serialize(EN_DATA_ERROR, SERVER_BOUND_MAP_OTHER, playerid);
		return asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
	}
	else if(ret == DB_Proxy::DB_SUCCEED)
	{//已学
		iCurTechLevel = officerSkill.skilllevel_;
	}

	iCurTechLevel++;

	SkillDetail * pHeroTech  = 0;
	pHeroTech = g_LoadDBinfo.getSkillDetail((SKILL_TYPE)skillid,iCurTechLevel);//获取下级技能的配置参数
	if(pHeroTech == NULL)
	{
		ErrorInfo.serialize(EN_TECH_ID_ERROR, SERVER_BOUND_MAP_OTHER, playerid);
		return asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
	}
	/*else
	{//技能已到上限
	ErrorInfo.serialize(EN_TECH_COUNT_UPPER, SERVER_BOUND_MAP_OTHER, playerid);
	ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
	return ret;
	}
	*/
	//新学习技能需要职业、是否可以学习判断
	if (ret != DB_Proxy::DB_SUCCEED)
	{
		if (!pHeroTech->iscanLearn)
		{
			ErrorInfo.serialize(EN_SKILL_CANTSTUDY, SERVER_BOUND_MAP_OTHER, playerid);
			return asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
		}
		if (!pHeroTech->isSatisfOccupationLimit((OFFICER_OCCUPATION_TYPE)officer.jobid_))
		{
			ErrorInfo.serialize(EN_JOBNOMATCHING, SERVER_BOUND_MAP_OTHER, playerid);
			return asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
		}
	}
	if (officer.level_ < (int)pHeroTech->levelLimit)
	{
		ErrorInfo.serialize(EN_LEVEL_NOT_ENOUGH, SERVER_BOUND_MAP_OTHER, playerid);
		return asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
	}
	if (officer.techdot_ < (int)pHeroTech->consumeSkillDot)
	{
		ErrorInfo.serialize(EN_TECH_DOT_NOT, SERVER_BOUND_MAP_OTHER, playerid);
		return asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
	}
	//添加英雄技能表
	if (ret == DB_Proxy::DB_SUCCEED)
	{//已学
		officerSkill.skilllevel_ = iCurTechLevel;
		if (g_Proxy.db_update(officerSkill,officerSkill) != DB_Proxy::DB_SUCCEED)
		{
			ErrorInfo.serialize(EN_DATA_ERROR, SERVER_BOUND_MAP_OTHER, playerid);
			return asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
		}
	}
	else
	{//新学技能
		officerSkill.skilltype_ = pHeroTech->skillType;
		officerSkill.skilllevel_ = iCurTechLevel;
		if (g_Proxy.db_insert(officerSkill) != DB_Proxy::DB_SUCCEED)
		{
			ErrorInfo.serialize(EN_DATA_ERROR, SERVER_BOUND_MAP_OTHER, playerid);
			return asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);	
		}
	}
	updateOfficer.techdot_ = -(int)pHeroTech->consumeSkillDot;
	updateOfficer.setmasterid(playerid);
	updateOfficer.setdetailid(officerid);
	if(g_Proxy.db_increase(updateOfficer) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("玩家%d指挥官%d学习技能 %ld扣除技能点失败。\n",playerid,officerid,skillid));
	}
	char * tmp = new char[sizeof(OFFICER_TECH_LIST) + sizeof(tagOfficerTechList)];
	OFFICER_TECH_LIST * pTechList = (OFFICER_TECH_LIST *)tmp;
	(*pTechList)[0].iTechID = skillid;
	(*pTechList)[0].Level = iCurTechLevel;
	pTechList->serialize(officerid,1,SERVER_BOUND_MAP_OTHER,playerid,EN_S_STUDY_A_TECH);	 
	ret = asynch_write(handle, (char *)pTechList, pTechList->length);
	if(officer.mapid_ >0)
	{
		studySkillNotify(officer.mapid_,officer.armyid_, officer.officerid_,skillid,iCurTechLevel,CMD_HERO_STUDY_SKILL);
	}
	if(tmp)
	{
		delete[] tmp;
		tmp = 0;
	}
	//20100408增加日志
	if (ret == DB_Proxy::DB_SUCCEED)
	{
		ISystemLog::getInstance()->heroSkillDot_UpSkill_Log(officer.playerid_,officer.officerid_,officer.officername_,
			officer.techdot_,updateOfficer.techdot_,pHeroTech->consumeSkillDot,skillid,iCurTechLevel);
		ISystemLog::getInstance()->heroSkill_UpSkill_Log(officer.playerid_,officer.officerid_,officer.officername_,
			skillid,iCurTechLevel-1,iCurTechLevel);
	}
	else
	{
		ISystemLog::getInstance()->heroSkillDot_StudySkill_Log(officer.playerid_,officer.officerid_,officer.officername_,
			officer.techdot_,updateOfficer.techdot_,pHeroTech->consumeSkillDot,skillid,iCurTechLevel);
		ISystemLog::getInstance()->heroSkill_StudySkill_Log(officer.playerid_,officer.officerid_,officer.officername_,
			skillid,0,iCurTechLevel);
	}
	return ret;
}
//种族天赋配置信息
void COtherManager::ReadRaceGeniusInfo()
{
	ifstream ifs("configtable/RaceParam.csv"); 
	if (ifs == NULL)
	{
		printf("打开文件有误RaceParam.csv\n");
	}
	string buf;
	int a=0,b=0,c=0,n=0;
	getline(ifs, buf);
	tagHeroRaceParam tmpHeroRaceParam;
	while(ifs)
	{
		buf.clear();
		if(getline(ifs, buf)) 
		{
			sscanf(buf.c_str(),"%d,%d,%d,%d,%d,%d",&a,&b,&tmpHeroRaceParam.iEndurance,&tmpHeroRaceParam.iStrength,&tmpHeroRaceParam.iAgility,&tmpHeroRaceParam.iIntelligence);
			m_mapRaceParam[b] = tmpHeroRaceParam;
			n++;
		}
	}
	ifs.close();

	ifstream ifs0("configtable/RaceJob.csv"); 
	if (ifs0 == NULL)
	{
		printf("打开文件有误RaceJob.csv\n");
	}
	a=0,b=0,c=0,n=0;
	getline(ifs0, buf);
	while(ifs0)
	{
		buf.clear();
		if(getline(ifs0, buf)) 
		{
			sscanf(buf.c_str(),"%d,%d,%d,%d,%d,%d",&b,&m_mapJobParam[n].iKnight,&m_mapJobParam[n].iSoldier,&m_mapJobParam[n].iShooter,&m_mapJobParam[n].iMaster,&m_mapJobParam[n].iPastor);

			n++;
		}
	}
	ifs0.close();

	ifstream ifs1("configtable/HeroName.csv"); 
	if (ifs1 == NULL)
	{
		printf("打开文件有误HeroName.csv\n");
	}
	a=0,b=0,c=0,n=0;
	string s,ss;
	getline(ifs1, buf);
	while(ifs1)
	{
		buf.clear();
		if(getline(ifs1, buf)) 
		{
			m_mapHeroName[n] = buf;
			n++;
		}
	}
	ifs1.close();

	ifstream ifs2("configtable/RecruitMoney.csv"); 
	if (ifs2 == NULL)
	{
		printf("打开文件有误RecruitMoney.csv\n");
	}
	a=0,b=0,c=0,n=0;
	getline(ifs2, buf);
	while(ifs2)
	{
		buf.clear();
		if(getline(ifs2, buf)) 
		{
			sscanf(buf.c_str(), "%d,%d",&a,&b);
			m_mapRecruitMoney.insert(make_pair(a, b));
		}
	}
	ifs2.close();

	ifstream ifs3("configtable/OfficerAttri.csv"); 
	if (ifs3 == NULL)
	{
		printf("打开文件有误OfficerAttri.csv\n");
	}
	a=0,b=0,c=0,n=0;
	getline(ifs3, buf);
	while(ifs3)
	{
		buf.clear();
		if(getline(ifs3, buf)) 
		{
			sscanf(buf.c_str(),"%d,%d,%d,%d,%d,%d, %d,%d,%d,%d,%d",&a,&m_HeroAttri[n].iRecruitFree,&m_HeroAttri[n].iTechPoint,
				&m_HeroAttri[n].iOrigEnd,&m_HeroAttri[n].iOrigStr,&m_HeroAttri[n].iOrigAgi,&m_HeroAttri[n].iOrigInt,
				&m_HeroAttri[n].iEndurance,&m_HeroAttri[n].iStrength,&m_HeroAttri[n].iAgility,&m_HeroAttri[n].iIntelligence);
			n++;
		}
	}
	ifs3.close();

	//读取特殊英雄静态表
	ifstream ifs4("configtable/SpecialHeroTable.csv"); 
	if (ifs4 == NULL)
	{
		printf("打开文件有误SpecialHeroTable.csv\n");
	}
	//a=0,b=0,c=0,n=0;
	getline(ifs4,buf);
	tagSpecialHero shero;
	while (ifs4)
	{
		buf.clear();
		if (getline(ifs4,buf))
		{
			if (sscanf(buf.c_str(),"%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",&shero.HeroID, &shero.HeroName,&shero.JodID,\
				&shero.RaceID,&shero.FaceID,&shero.minStamina,&shero.maxStamina,&shero.minPower,&shero.maxPower,
				&shero.minCelerity,&shero.maxCelerity,&shero.minWit,&shero.maxWit,&shero.TechID,&shero.TechLevel)!=15)
			{
				printf("read SpecialHeroTable.csv error  \n");
				continue;
			}
			m_mapSpecialHero.insert(make_pair(shero.HeroID,shero));
		}
	}
	ifs4.close();
}
void COtherManager::ReadCityInfo()
{
	ReadMapData();
}
//读取郡地图数据
void COtherManager::ReadMapData()
{
	int count = 0 ;  // 统计向上走的 次数 当大于3次,认为不可行走,放弃
	int line  = 0 ;
	int row = 0;
	ifstream   ifs("configtable/bigmapinfo.csv"); 
	// 读取地图文件
	string   buf; 
	string   tmpbuf;
	char tmp[4] = "";
	while(ifs)
	{
		if(getline(ifs,   buf)) 
		{
			for (int i = 0 ; i <(int)buf.size(); i ++)
			{
				if (buf[i]!=',')
				{
					tmp[count]=buf[i];
					count ++;
				}
				else if (buf[i]==',' || i == buf.size()-1)
				{
					m_MapInfo[line][row]= atoi(tmp);
					memset(tmp,0,4);
					count =0;
					row ++;
				}
			}
			m_MapInfo[line][row]= atoi(tmp);
			memset(tmp,0,4);
			count =0;
			row ++;

		}
		row = 0;
		line ++;
	}
}


//此郡能否达到
int COtherManager::OnReachMap(int handle, unsigned long lPlayerID, unsigned long lMapID, unsigned long lTargetMapID, unsigned long lArmyID, list<int>* path, int iCase)
{
	
	PlayerChatRoomTable PlayerCountry;
	PlayerCountry.setid(lPlayerID);
	if(lPlayerID <10000)
	{
		PlayerCountry.country_ = lPlayerID/100;
	}
	else if (g_Proxy.db_select(PlayerCountry) != DB_Proxy::DB_SUCCEED)
	{
		return 0;
	}

	ARMY_CANT_MOVE armyMove;
	//add by xujincai 20081216 新手村无法到达
	if((PlayerCountry.country_ != m_MapState[lTargetMapID-1001].countryid_) 
		&& /*(lTargetMapID == 1001 || lTargetMapID == 1006 )*/IsSafeMap(lTargetMapID,m_MapState[lTargetMapID-1001].countryid_))
	{
		if(handle !=0)
		{
			armyMove.serialize(3, SERVER_BOUND_MAP_OTHER, lPlayerID);
			asynch_write(handle, (char *)&armyMove, armyMove.length);
		}
		return 0;
	}
	int len = 0;
	if (iCase == 0)
	{
		int tmpMapInfo[BIG_MAP_SIZE][BIG_MAP_SIZE];
		memcpy(tmpMapInfo,m_MapInfo,sizeof(int)*BIG_MAP_SIZE*BIG_MAP_SIZE);
		ResetMapInfo(tmpMapInfo,PlayerCountry.country_,lMapID,lTargetMapID);
		len = BigMapFindPath(tmpMapInfo,lMapID-1001,lTargetMapID-1001,path);
	}
	else
	{
		len = BigMapFindPath(m_MapInfo,lMapID-1001,lTargetMapID-1001,path);
	}

	if (len <= 0) return 0;
	int iItem = (int)path->size();
	if (iItem <= 0 || iItem >= 50) return 0;
	
	return 1;
}
//部队转移
int COtherManager::OnArmyTransfer(int handle, void * pDataBuffer, int iDataSize, unsigned long lMapID, unsigned long lPlayerID)
{
	if (iDataSize != sizeof(ARMY_TRANSFER))
	{
		LOG_TRACE_EX(false,("COtherManager::OnArmyTransfer-->消息包非法：玩家%d\n",lPlayerID));
		return false;
	}

	ARMY_TRANSFER * pArmyTran = (ARMY_TRANSFER *)pDataBuffer;
	tagArmy_Reach TranArmy;
	TranArmy.serialize(lPlayerID, pArmyTran->lRMapID, 0, pArmyTran->lTargetMapID, pArmyTran->lArmyID,0,CMD_ARMY_REACH);
	//add xujincai 20081223
	TranArmy.agent = pArmyTran->agent;
	OnOutArmyEx(&TranArmy, TranArmy.length, 1,false);//modify by xujincai 20081224  OnOutArmy->OnOutArmyEx
	return 1;
}
//正处于出征中的部队列表
int COtherManager::OnOutingArmysListEx(int handle, unsigned long lMapID, unsigned long lPlayerID)
{
	ArmyOutTable OutingArmy;
	ArmyOutTable *pRetSet = 0;
	char *buf = 0;
	int count = 0;
	OutingArmy.setmasterid(lPlayerID);
	if (g_Proxy.db_multi_select(OutingArmy, buf, count) != DB_Proxy::DB_SUCCEED)
	{
		return 0;
	}
	count = count/sizeof(ArmyOutTable);
	pRetSet = (ArmyOutTable*)buf;
	tagArmy_Reach TranArmy;
	for(int i=0;i<count;i++)
	{
		TranArmy.serialize(lPlayerID, pRetSet[i].frommapid_, 0, pRetSet[i].targetmapid_, pRetSet[i].armyid_,0,CMD_ARMY_REACH);
		TranArmy.agent = handle;
		OnOutArmyEx(&TranArmy, TranArmy.length, 1,true);//modify by xujincai 20081224  OnOutArmy->OnOutArmyEx
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	return 1;
}
int COtherManager::BigMapFindPath(int start,int End ,list<int>* path)
{
	// load big map info  30*30 
//	int MapInfo[BIG_MAP_SIZE][BIG_MAP_SIZE];
	//POS_STRUCT CentryPos[BIG_MAP_SIZE];
	bool s[BIG_MAP_SIZE] ; //是否走过
	int  prevpoint[BIG_MAP_SIZE]; // 的前一个节点
	int dist[BIG_MAP_SIZE]; //到起点的距离
	bool Ischange = false;
	int count = 0 ;  // 统计向上走的 次数 当大于3次,认为不可行走,放弃
	int line  = 0 ;
	int row = 0;
	list<int> open_list;
	for (int i = 0 ;  i < BIG_MAP_SIZE ; i ++)
	{
		s[i] = false ;
		dist[i] = m_MapInfo[start][i];
		if (m_MapInfo[start][i]>0)
		{
			prevpoint[i] = start;
			
			open_list.push_back(i);//可以到达的点呀如站
		}

	}

	if (open_list.size() ==0)
	{
		return -1;
	}
	// 寻路规则,如果两个郡相邻,
	//int dis = 0 ,tempdis = 0 ;
	int pro ;// = open_list.front();
	int len ;
	while (open_list.size()!=0)
	{
		pro = open_list.front();
		open_list.pop_front();
		if (pro == End)
		{
			//寻路成功
			len = dist[pro];
			path->push_front(pro);

			while(pro != start)
			{
				pro =prevpoint[pro];
				path->push_front(pro);

			}

			return len;
		}
		for (int i = 0 ; i < BIG_MAP_SIZE ; i ++)
		{
			if (m_MapInfo[pro][i] > 0)
			{
				if (dist[pro]+m_MapInfo[pro][i]<dist[i])
				{
					dist[i]= dist[pro]+m_MapInfo[pro][i];
					prevpoint[i]= pro;
					//open_list.push_back(i);
					if (i == End)
					{
						//printf("find path\n");
					}
				}
				else if (dist[i]<0)
				{
					dist[i]= dist[pro]+m_MapInfo[pro][i];
					prevpoint[i]= pro;
					open_list.push_back(i);
					if (i == End)
					{
						//printf("find path\n");
					}
				}
				

			}
		}
	}
	return -1 ;

}
int COtherManager::BigMapFindPath(int MapInfo[BIG_MAP_SIZE][BIG_MAP_SIZE],int start,int End ,list<int>* path)
{
	bool s[BIG_MAP_SIZE] ; //是否走过
	int  prevpoint[BIG_MAP_SIZE]; // 的前一个节点
	int dist[BIG_MAP_SIZE]; //到起点的距离
	bool Ischange = false;
	int count = 0 ;  // 统计向上走的 次数 当大于3次,认为不可行走,放弃
	int line  = 0 ;
	int row = 0;
	list<int> open_list;
	for (int i = 0 ;  i < BIG_MAP_SIZE ; i ++)
	{
		s[i] = false ;
		dist[i] = MapInfo[start][i];
		if (MapInfo[start][i]>0)
		{
			prevpoint[i] = start;
			
			open_list.push_back(i);//可以到达的点呀如站
		}

	}

	if (open_list.size() ==0)
	{
		return -1;
	}
	// 寻路规则,如果两个郡相邻,
	int pro ;// = open_list.front();
	int len ;
	while (open_list.size()!=0)
	{
		pro = open_list.front();
		open_list.pop_front();
		if (pro == End)
		{
			//寻路成功
			len = dist[pro];
			path->push_front(pro);

			while(pro != start)
			{
				pro =prevpoint[pro];
				path->push_front(pro);

			}

			return len;
		}
		for (int i = 0 ; i < BIG_MAP_SIZE ; i ++)
		{
			if (MapInfo[pro][i] > 0)
			{
				if (dist[pro]+MapInfo[pro][i]<dist[i])
				{
					dist[i]= dist[pro]+MapInfo[pro][i];
					prevpoint[i]= pro;
					//open_list.push_back(i);
					if (i == End)
					{
						//printf("find path\n");
					}
				}
				else if (dist[i]<0)
				{
					dist[i]= dist[pro]+MapInfo[pro][i];
					prevpoint[i]= pro;
					open_list.push_back(i);
					if (i == End)
					{
						//printf("find path\n");
					}
				}
				

			}
		}
	}
	return -1 ;

}
int COtherManager::ResetMapInfo(int MapInfo[BIG_MAP_SIZE][BIG_MAP_SIZE],int countryid,int startmapid,int destmapid)
{
	int startid = startmapid - 1001;
	int destid = destmapid - 1001;
	int i = 0;
	int j =0;
	for(i = 0;i<BIG_MAP_SIZE;i++)
	{
		if((m_MapState[i].state_ !=0 && m_MapState[i].state_ !=100) || m_MapState[i].countryid_ != countryid)
		{
			for(j=0;j<BIG_MAP_SIZE;j++)
			{
				if(i != destid)
					MapInfo[j][i] = -1;
			}
			for(j=0;j<BIG_MAP_SIZE;j++)
			{
				if(i != startid)
					MapInfo[i][j] = -1;
			}
		}
	}
	return 0;
}
//部队退回时，回到附近的郡（首选本方郡）
int COtherManager::GetNextMapID(int countryid,int start,int end)
{
	if (start == end)
	{
		return -1;
	}
	int iMapID = 0;
	int iStart = start - 1001;
	int iEnd = end - 1001;
	list<int> path;
	int tmpMapInfo[BIG_MAP_SIZE][BIG_MAP_SIZE];
	memcpy(tmpMapInfo,m_MapInfo,sizeof(int)*BIG_MAP_SIZE*BIG_MAP_SIZE);
	ResetMapInfo(tmpMapInfo,countryid,start,end);
	int len = BigMapFindPath(tmpMapInfo,iStart,iEnd,&path);
	list<int>::iterator iter;
	if (len >0)
	{
		iter = path.begin();
		iter++;
		iMapID = (*iter)+1001;
	}
	else
	{
		path.clear();
		len = BigMapFindPath(m_MapInfo,iStart,iEnd,&path);
		if (len >0)
		{
			iter = path.begin();
			iter++;
			iMapID = (*iter)+1001;
		}
	}
	return iMapID;
}
//删除出征部队
void COtherManager::OnDeleteOutArmy(unsigned long lPlayerID, unsigned long lArmyID)
{
	//char * buf = 0;
	//int iCount = 0, iItem = 0;
	ArmyOutTable OutingArmy;
	memset(&OutingArmy, 0, sizeof(ArmyOutTable));
	OutingArmy.setmasterid(lPlayerID);
	OutingArmy.setdetailid(lArmyID);
	if (g_Proxy.db_delete(OutingArmy) != DB_Proxy::DB_SUCCEED)
	{
		return;
	}
	else
	{
	}
}
//计算部队出征时间
int COtherManager::GetMapTimes(unsigned long lPlayerID, unsigned long lMapID, unsigned long lTargetMapID, unsigned long lArmyID, int iTimeLen)
{
	iTimeLen = m_MapInfo[lMapID-1001][lTargetMapID-1001];
	if (iTimeLen<=0)
	{
		iTimeLen = 60;
	}
	return iTimeLen;
}

void COtherManager::ArmyWarmTimeEvent(void * pDataBuffer, int iDataSize)
{
	if (iDataSize != sizeof(ARMY_WARM))
	{
		LOG_TRACE_EX(false,("COtherManager::ArmyWarmTimeEvent-->消息包非法\n"));
		return;
	}

	ARMY_WARM * pWarm = (ARMY_WARM *)pDataBuffer;

	int iMapID = pWarm->lMapID - 1001;
	int iRet = 0;
	if(m_MapWarmInfo[iMapID].UnionCount >0 || m_MapWarmInfo[iMapID].ClanCount >0)
	{
		ARMY_REACH_WARM warmmsg;
		warmmsg.serialize(pWarm->lMapID,SERVER_BOUND_MAP_OTHER,0,m_MapWarmInfo[iMapID].UnionCount,m_MapWarmInfo[iMapID].ClanCount,m_MapWarmInfo[iMapID].AboriginalCount);
		iRet = broadcast((char*)&warmmsg,warmmsg.length);
	}
	m_MapWarmInfo[iMapID].TimerID = 0;
	m_MapWarmInfo[iMapID].UnionCount = 0;
	m_MapWarmInfo[iMapID].ClanCount = 0;
	m_MapWarmInfo[iMapID].AboriginalCount = 0;
}

//确定部队出征OK
int COtherManager::OnArmyReachOKEx(const void *pDataBuffer,int iDataSize)
{
	if (iDataSize != sizeof(tagArmy_Reach))
	{
		LOG_TRACE_EX(false,("COtherManager::OnArmyReachOK-->消息包非法\n"));
		return false;
	}
	tagArmy_Reach * pOutArmy = (tagArmy_Reach *)pDataBuffer;
	//OnDeleteOutArmy(pOutArmy->lPlayerID,pOutArmy->lArmyID);//首先删除出征部队表
	int iTmpID = pOutArmy->lToMapID-1001;
	int  m = 0, ret = 0,iflag = 0,iCountry=0;
	//ARMY_OUT_LIST OutArmyList;
	MapInfoTable MapInfo;
	//memset(&OutArmyList, 0, sizeof(ARMY_OUT_LIST));
	
	ArmyTable Army;
	Army.setmasterid(pOutArmy->lToMapID);
	Army.setdetailid(pOutArmy->lArmyID);
	if (g_Proxy.db_select(Army) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("COtherManager::OnArmyReachOK-->查询部队失败：玩家%d,MapID:%d,ArmyID:%d\n",pOutArmy->lFromMapID,pOutArmy->lToMapID,pOutArmy->lArmyID));
		return false;
	}
	//是否达到敌方郡内
	/*
	MapInfo.setid(pOutArmy->lToMapID);
	if (g_Proxy.db_select(MapInfo) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("COtherManager::OnArmyReachOK-->查询地图失败：玩家%d,MapID:%d,ArmyID:%d\n",pOutArmy->lFromMapID,pOutArmy->lToMapID,pOutArmy->lArmyID));
		return false;
	}
	*/
	//当目标郡为对方郡且处于合并状态时，则战斗开始
	if(m_MapState[iTmpID].state_ == 0 && m_MapState[iTmpID].countryid_ != Army.countryid_)
	{
		iflag = 1;
		//修改地图状态
		m_MapState[iTmpID].state_ = 1;
		MapInfo.setid(pOutArmy->lToMapID);
		MapInfo.countryid_ = m_MapState[iTmpID].countryid_;
		MapInfo.state_ = 1;
		if (g_Proxy.db_update(MapInfo,MapInfo) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE_EX(false,("COtherManager::OnArmyReachOK-->修改地图状态失败：玩家%d,MapID:%d,ArmyID:%d\n",pOutArmy->lFromMapID,pOutArmy->lToMapID,pOutArmy->lArmyID));
		}
	}
	//扣除行动值
	if (Army.playerid_ >10000 && (m_MapState[iTmpID].countryid_ != Army.countryid_ || m_MapState[iTmpID].state_ ==1))
	{
		LPActionDotParam pActionParam = g_LoadDBinfo.GetActionDotParam(pOutArmy->lToMapID,Country_Map);
		if (pActionParam)
		{
			int iNeedDot = 0;
			IceUtil::DateTime curdate;
			IceUtil::Time::now().toDateTime(curdate);
			tagTime_Hour curhour;
			curhour.hour = curdate.hour;
			curhour.min = curdate.min;
			if (pActionParam->Day_Start <curhour  && curhour < pActionParam->Day_End)
			{//白天
				if (m_MapState[iTmpID].countryid_ != Army.countryid_ )
				{//进攻型
					iNeedDot = pActionParam->AttackDot_Day;
				}
				else
				{
					iNeedDot = pActionParam->RecoveryDot_Day;
				}
			}
			else
			{
				if (m_MapState[iTmpID].countryid_ != Army.countryid_)
				{//进攻型
					iNeedDot = pActionParam->AttackDot_Night;
				}
				else
				{
					iNeedDot = pActionParam->RecoveryDot_Night;
				}
			}
			PlayerOfficerTable officer;
			officer.setmasterid(Army.playerid_);
			officer.setdetailid(Army.officerid_);
			if (g_Proxy.db_select(officer) == DB_Proxy::DB_SUCCEED)
			{
				if (iNeedDot >officer.actiondot_)
				{
					iNeedDot = officer.actiondot_;
				}
				memset(&officer,0,sizeof(PlayerOfficerTable));
				officer.setmasterid(Army.playerid_);
				officer.setdetailid(Army.officerid_);
				officer.actiondot_ = - iNeedDot;
				g_Proxy.db_increase(officer);
			}
		}
	}
	/*del by xujincai 20090724 战斗服务器已下发*/
	int x = 0;
	char * tmp = new char[sizeof(MAP_ARMY_LIST) + sizeof(tagArmysList)];
	MAP_ARMY_LIST *pArmysList = (MAP_ARMY_LIST *)tmp;
	if (Army.y%2)
		x = (Army.x + 1)/2;
	else
		x = (Army.x + 2)/2;
	(*pArmysList)[m].lMapID = pOutArmy->lToMapID;
	(*pArmysList)[m].lPlayerID = pOutArmy->lPlayerID;
	(*pArmysList)[m].lArmyID = Army.armyid_;
	(*pArmysList)[m].lOfficerID = Army.officerid_;
	(*pArmysList)[m].iRaceID = Army.raceid_;
	(*pArmysList)[m].iJobID = Army.soldiertype_;
	(*pArmysList)[m].iX = Army.y;
	(*pArmysList)[m].iY = x;
	(*pArmysList)[m].iDirection = Army.direction;
	(*pArmysList)[m].iState = Army.state_;

	(*pArmysList)[m].iWeaponID = Army.fweapon_;
	(*pArmysList)[m].iDefendID = Army.fdefendtype_;
	(*pArmysList)[m].iWeaponLevel = Army.fweaponlevel_;
	(*pArmysList)[m].iDefendLevel = Army.fdefendlevel_;
	(*pArmysList)[m].iExp = Army.fsexp_;

	PlayerOfficerTable Officer;
	Officer.setmasterid(pOutArmy->lPlayerID);
	Officer.setdetailid(Army.officerid_);
	if (g_Proxy.db_select(Officer) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("通知目标郡部队达到  %ld 指挥官(%ld) db_select(Officer) errror \n",pOutArmy->lPlayerID,Army.officerid_));

		//return 0;
	}
	(*pArmysList)[m].iFace = Officer.faceid_;
	strcpy((*pArmysList)[m].szArmyName, Army.armyname_);
	strcpy((*pArmysList)[m].szOfficerName, Officer.officername_);

	if (Army.armyname_[0] =='\0')
		strcpy((*pArmysList)[m].szArmyName, "JK");
	m++;

	unsigned long handle = g_DataServer.getAgentOfPlayer(pOutArmy->lPlayerID);//m_mapOnLinePlayer[pOutArmy->lPlayerID];
	if (handle > 0)
	{
		pArmysList->serialize(m, SERVER_BOUND_MAP_OTHER, pOutArmy->lPlayerID,EN_S_ARMY_REACH);
		ret = asynch_write(handle, (char *)pArmysList, pArmysList->length);
	}
	if (tmp != NULL)
	{
		delete [] tmp;
		tmp = 0;
	}
	
	if (iflag == 1)//发生战斗
	{
		char * tmp1 = new char[sizeof(MAP_STATE_LIST)+sizeof(tagMapState)];
		MAP_STATE_LIST *pMapList = (MAP_STATE_LIST *)tmp1;

		(*pMapList)[0].lMapID = pOutArmy->lToMapID;
		(*pMapList)[0].iCountry = m_MapState[iTmpID].countryid_;
		(*pMapList)[0].iState = m_MapState[iTmpID].state_;
		pMapList->serialize(1, SERVER_BOUND_MAP_OTHER, pOutArmy->lPlayerID,EN_S_MAP_MODFIY);
		ret = broadcast((char *)pMapList, pMapList->length);
		if (ret == 1)
			printf("广播信息成功..%ld...country %d..state %d...\n",(*pMapList)[0].lMapID,(*pMapList)[0].iCountry ,(*pMapList)[0].iState);
		else
			printf("广播信息失败...........\n");
		if(tmp1)
		{
			delete [] tmp1;
			tmp1 = 0;
		}
	}
	return 1;
}
//结束战斗
void COtherManager::OnMapEnFight(const void * pDataBuffer, int iDataSize)
{
	if (iDataSize < sizeof(EndFight))
	{
		LOG_TRACE_EX(false,("COtherManager::OnMapEnFight-->消息包非法\n"));
		return;
	}

	EndFight * pEndFight = (EndFight *)pDataBuffer;
	int ret = 0;

	char * tmp = new char[sizeof(MAP_STATE_LIST) + sizeof(tagMapState)];
	MAP_STATE_LIST *pMapList = (MAP_STATE_LIST *)tmp;

	(*pMapList)[0].lMapID = pEndFight->uMapID;
	(*pMapList)[0].iCountry = pEndFight->winer;
	(*pMapList)[0].iState = 0;
	pMapList->serialize(1, m_lMapID, m_lMapID,EN_S_MAP_MODFIY);

	ret = broadcast((char *)pMapList, pMapList->length);
	if (ret == 1)
		printf("广播信息成功..玩家..分布式服务器ID.%ld...country %d..state %d...\n",(*pMapList)[0].lMapID,(*pMapList)[0].iCountry ,(*pMapList)[0].iState);
	else
		printf("广播信息成功..玩家.分布式服务器ID.%ld...country %d..state %d...\n",(*pMapList)[0].lMapID,(*pMapList)[0].iCountry ,(*pMapList)[0].iState);
	m_MapState[pEndFight->uMapID-1001].state_ = 0;
	if((m_MapState[pEndFight->uMapID-1001].countryid_ !=0) && (m_MapState[pEndFight->uMapID-1001].countryid_ != pEndFight->winer))
	{
		m_MapState[pEndFight->uMapID-1001].countryid_ = pEndFight->winer;
		//通知郡地图清空玩家所有信息add by xujincai 20090216
		tagResetMapInfo_Request resetmap;
		resetmap.serialize(pEndFight->uMapID);
		event_notify(pEndFight->uMapID,(char*)&resetmap,sizeof(tagResetMapInfo_Request));
	}
	m_MapState[pEndFight->uMapID-1001].countryid_ = pEndFight->winer;
	m_MapWarm[pEndFight->uMapID-1001] = 0;
	if (tmp != NULL)
	{
		delete [] tmp;
		tmp = 0;
	}
}
//退出游戏
void COtherManager::OnExitMapServer(unsigned long lPlayerID)
{
	//m_mapOnLinePlayer.erase(lPlayerID);	
}
int64 COtherManager::GetCurrentTime()
{
	/*time_t t = time( 0 ); 
	tm * localtm = localtime(&t) ;
	char tmp[32]="0",TimeLen[64]="0"; 	
	strftime( tmp, sizeof(tmp), "%Y%m%d",localtm); 
	sprintf(TimeLen,"%s%d%d%d",tmp,localtm->tm_hour, localtm->tm_min,localtm->tm_sec );
	return _atoi64(TimeLen);*/
	return IceUtil::Time::now().toLongDateTime();

}
void COtherManager::OnRecruitSpeed(unsigned long lPlayerID, int iValue)
{
	//tagNotify_IncreaseOtherSpeed_Response ReqSpeed;
	//map<unsigned long, BUILD_CONSTRUCTION_NODE*>::iterator it = m_mapSpeedEvent.find(lPlayerID);
	//if (it->first < 1 || it->second == NULL)
	//{
	//	ReqSpeed.serialize(lPlayerID, Research_Speed_Card, 1);
	//	event_notify(MDM_GP_MAP_CHARGE , (char * )&ReqSpeed, ReqSpeed.length);
	//	return;
	//}
	//ReqSpeed.serialize(lPlayerID, Research_Speed_Card, 0);
	//event_notify(MDM_GP_MAP_CHARGE , (char * )&ReqSpeed, ReqSpeed.length);
}

//add by xujincai 20081217
//添加宝箱
int COtherManager::OnNewTreasurePack(const char *msg)
{
	LPNewTreasurePack_Request req_msg = (LPNewTreasurePack_Request)msg;
	if(req_msg->length != sizeof(NewTreasurePack_Request))
	{
		LOG_TRACE_EX(false,("COtherManager::OnNewTreasurePack-->消息包非法\n"));
		return 1;
	}

	NewTreasurePack_Response rep_msg;
	int agent = g_DataServer.getAgentOfPlayer(req_msg->from);
	int iOfficerID = 1;
	//获取玩家Agent
	switch(req_msg->ItemType)
	{
	case TREASUREPACK:
		{
			//宝箱是否合法
			CTreasurePack *treasurePack = getBinConfig(req_msg->ItemID);
			if(treasurePack == NULL)
			{
				return -3;
			}
		}
		break;
	case ARMYBRAND:
		{//军牌
			ArmyBrand *pArmyBrand = g_LoadDBinfo.getArmyBrandParamByID(req_msg->ItemID);
			if (pArmyBrand == NULL)
			{
				return -3;
			}
		}
		break;
	default:
		{
			return -4;
		}
	}
	
	bool bOverlap = false;
	PackStoreParam *pPackStoreParam = g_LoadDBinfo.GetPackStoreParam(req_msg->ItemType);
	if (pPackStoreParam)
	{
		bOverlap = (pPackStoreParam->IsOverlap >0)?true:false;
	}
	int iRet = 0;
	PlayerItemStorageTable iteminfo;
	if(OnInsertHeroEquip(&g_Proxy, req_msg->from, 1, req_msg->ItemID, 0, req_msg->ItemType,bOverlap,0,0,iteminfo) <=0)
	{
		iRet = EN_PACKFULL;
	}
	else
	{
		rep_msg.itemInfo.iSortID = iteminfo.sortid_;
		rep_msg.itemInfo.InlayItem.NewItemID = iteminfo.itemid_;
		rep_msg.itemInfo.iPos = iteminfo.pos_;
		rep_msg.itemInfo.iLevel = iteminfo.level_;
		rep_msg.itemInfo.iShowPos = iteminfo.showpos_;
		rep_msg.itemInfo.iType = iteminfo.type_;
		rep_msg.itemInfo.iCount = iteminfo.count_;
		rep_msg.itemInfo.Skill = iteminfo.tech_;
		rep_msg.itemInfo.SkillLevel = iteminfo.techlevel_;
		//20100408
		PlayerOfficerTable officer;
		officer.setmasterid(req_msg->from);
		officer.setdetailid(req_msg->from);
		if (g_Proxy.db_select(officer) == DB_Proxy::DB_SUCCEED)
		{
			ISystemLog::getInstance()->playerEquip_KillNPC_Log(officer.playerid_,officer.officername_,
				iteminfo.pos_,iteminfo.itemid_,iteminfo.count_);
		}
	}
	//返回客户端，宝箱ID或军牌ID
	if(agent >0)
	{
		rep_msg.serialize(agent,req_msg->from,iRet);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	}
	return 0;
}
//打开宝箱
int COtherManager::OnOpenTreasurePack(const char *msg)
{
	LPOpenTreasurePack_Request req_msg = (LPOpenTreasurePack_Request)msg;
	if(req_msg->length != sizeof(OpenTreasurePack_Request))
	{
		LOG_TRACE_EX(false,("COtherManager::OnOpenTreasurePack-->消息包非法\n"));
		return 1;
	}

	OpenTreasurePack_Response rep_msg;
	int iRet = 0;
	int iType = 0;
	int iShowpos = 0;
	//首先判断宝箱是否存在
	PlayerItemStorageTable iteminfo;
	iteminfo.setmasterid(req_msg->OfficerID,req_msg->from);
	iteminfo.setdetailid(req_msg->SortID);
	if(g_Proxy.db_select(iteminfo) != DB_Proxy::DB_SUCCEED)
	{//不存在
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	if(iteminfo.pos_ != TREASUREPACK)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}

	iType = iteminfo.type_;
	iShowpos = iteminfo.showpos_;

	/*int iMapID = 1001;
	PlayerChatRoomTable playertb;
	playertb.setid(req_msg->from);
	if(g_Proxy.db_select(playertb) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	*/
	PlayerCity playercity;
	PlayerCity *pPlayerCityList = 0;
	char *buf = 0;
	int count = 0;
	playercity.setmasterid(req_msg->from);
	if(g_Proxy.db_multi_select(playercity,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	count = count/sizeof(PlayerCity);
	pPlayerCityList = (PlayerCity*)buf;
	for(int i= 0;i<count;i++)
	{
		if(pPlayerCityList[i].state_ == 100)
		{
			playercity.setdetailid(pPlayerCityList[i].mapid_);
			break;
		}
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}

	//判断新手村的仓库容量是否足够，目前暂定仓库容量不能小于100
	//playercity.setdetailid(iMapID);
	if(g_Proxy.db_select(playercity) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	//获取宝箱内容
	CTreasurePack::TreasurePackItem sureItem,randomItem,equipItem;
	CTreasurePack* treasurePack = getBinConfig(iteminfo.itemid_);
	if(treasurePack == NULL)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	treasurePack->GetItems(&sureItem,&randomItem,&equipItem);
	PlayerResource citycontent;
	if(randomItem.ItemID >0)
	{
		citycontent.setmasterid(playercity.cityid_,playercity.mapid_);
		citycontent.setdetailid(req_msg->from);
		if(g_Proxy.db_select(citycontent) != DB_Proxy::DB_SUCCEED)
		{
			rep_msg.serialize(req_msg->agent,req_msg->from,2);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			return 1;
		}
		if(citycontent.content_ <randomItem.Count)
		{
			rep_msg.serialize(req_msg->agent,req_msg->from,2);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			return 1;
		}
	}
	//首先删除宝箱
	if (iteminfo.count_ <=1)
	{
		if(g_Proxy.db_delete(iteminfo) != DB_Proxy::DB_SUCCEED)
		{
			rep_msg.serialize(req_msg->agent,req_msg->from,111);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			return 1;
		}
	}
	else
	{
		memset(&iteminfo,0,sizeof(iteminfo));
		iteminfo.setmasterid(req_msg->OfficerID,req_msg->from);
		iteminfo.setdetailid(req_msg->SortID);
		iteminfo.count_ = -1;
		if(g_Proxy.db_increase(iteminfo) != DB_Proxy::DB_SUCCEED)
		{
			rep_msg.serialize(req_msg->agent,req_msg->from,111);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			return 1;
		}
	}
	//20100408
	PlayerOfficerTable officer;
	officer.setmasterid(req_msg->from);
	officer.setdetailid(req_msg->from);
	g_Proxy.db_select(officer);
	ISystemLog::getInstance()->playerEquip_OpenChest_Log(officer.playerid_,officer.officername_,
		iteminfo.pos_,iteminfo.itemid_,1);
	//添加金钱
	if(sureItem.Count >0)
	{
		PlayerChatRoomTable playertb;
		//memset(&playertb,0,sizeof(playertb));
		playertb.setid(req_msg->from);
		playertb.money_ = sureItem.Count;
		if(g_Proxy.db_increase(playertb) != DB_Proxy::DB_SUCCEED)
		{
			printf("OnOpenTreasurePack(add player(%d) money) fault.\n",req_msg->from);
		}
		//20100408
		ISystemLog::getInstance()->playerMoney_OpenChest_Log(officer.playerid_,officer.officername_,
			playertb.money_ - sureItem.Count,playertb.money_,sureItem.Count,iteminfo.itemid_,1);
	}
	//添加特殊资源
	/*
	EN_FOOD=1,			//粮食
	EN_WOOD,			//木材
	EN_IRON,			//铁
	EN_SKIN,			//皮
	EN_YELLOW_STONE=5,	//黄宝石
	EN_RED_STONE,		//红宝石
	EN_GREEN_STONE,		//绿宝石
	EN_BLACK_STONE		//黑铁
	*/
	if(randomItem.ItemID >0 && randomItem.Count >0)
	{
		memset(&citycontent,0,sizeof(citycontent));
		citycontent.setmasterid(playercity.cityid_,playercity.mapid_);
		citycontent.setdetailid(req_msg->from);
		if(randomItem.ItemID == EN_FOOD)
		{
			citycontent.foodcount_ = randomItem.Count;
		}
		else if(randomItem.ItemID == EN_WOOD)
		{
			citycontent.woodcount_ = randomItem.Count;
		}
		else if(randomItem.ItemID == EN_IRON)
		{
			citycontent.ironcount_ = randomItem.Count;
		}
		else if(randomItem.ItemID == EN_SKIN)
		{
			citycontent.skincount_ = randomItem.Count;
		}
		else if(randomItem.ItemID == EN_YELLOW_STONE)
		{
			citycontent.yollowstonecount_ = randomItem.Count;
		}
		else if(randomItem.ItemID == EN_RED_STONE)
		{
			citycontent.redstonecount_ = randomItem.Count;
		}
		else if(randomItem.ItemID == EN_GREEN_STONE)
		{
			citycontent.greenstonecount_ = randomItem.Count;
		}
		else if(randomItem.ItemID == EN_BLACK_STONE)
		{
			citycontent.backironcount_ = randomItem.Count;
		}
		else
		{
			randomItem.Count = 0;
		}
		citycontent.content_ = - randomItem.Count;
		if(randomItem.Count >0)
		{
			if(g_Proxy.db_increase(citycontent) != DB_Proxy::DB_SUCCEED)
			{
				printf("OnOpenTreasurePack(add player(%d) special resource) fault.\n",req_msg->from);
			}
		}
	}
	
	//更新装备
	HeroEquipAttribute *heroEquipAttr = 0;
	
	memset(&iteminfo,0,sizeof(iteminfo));
	if(equipItem.ItemID >0 && equipItem.Count >0)
	{
		//获取装备类型
		heroEquipAttr = g_LoadDBinfo.GetHeroEquipAttribute(equipItem.ItemID);
		if(heroEquipAttr)
		{
			PackStoreParam *pPackStoreParam = g_LoadDBinfo.GetPackStoreParam(heroEquipAttr->postion);
			bool bOverlap = false;
			if (pPackStoreParam)
			{
				bOverlap = (pPackStoreParam->IsOverlap>0)?true:false;
			}
			if(OnInsertHeroEquip(&g_Proxy,req_msg->from,req_msg->OfficerID,equipItem.ItemID,heroEquipAttr->level,heroEquipAttr->postion,bOverlap,0,0,iteminfo) <=0)
			{
				iRet = 1;
			}
			else
			{
				//发送广播
				g_DataServer.broadMsgAndNotify(req_msg->from,SERVER_BOUND_MAP_OTHER,equipItem.ItemID,heroEquipAttr->quality,heroEquipAttr->postion,0,EN_S_BROADHEROEQUIP,SERVER_BOUND_MAP_OTHER);
				//20100408
				ISystemLog::getInstance()->playerEquip_OpenChest_Log(officer.playerid_,officer.officername_,
					heroEquipAttr->postion,equipItem.ItemID,1);
			}
		}
	}
	rep_msg.serialize(req_msg->agent,req_msg->from,0,sureItem.Count,randomItem.ItemID,randomItem.Count,
		iteminfo.itemid_,iteminfo.sortid_,iteminfo.level_,iteminfo.pos_,iteminfo.showpos_,iteminfo.type_,iteminfo.count_);

	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	return 0;
}
void COtherManager::loadBinConfig()
{
	FILE *fp = 0;
	fp = fopen("configtable/BinConfig.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/BinConfig.csv失败。\n");
		return ;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);


	CTreasurePack *pNewPackParam = 0;
	int binID;						//宝箱编号
	char sureFlopItem[SIZE_32];		//必掉落的金钱
	char randomFlopItem[SIZE_32];	//随机掉落的资源
	//char randomFlopOdds[SIZE_64];	//随机掉落的装备
	int level = 0;

	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		level = 0;
		if(sscanf(buf,"%d,%[^,],%[^,],%d",
			&binID,&sureFlopItem,&randomFlopItem,&level)<3)
		{
			continue;
		}
		pNewPackParam = new CTreasurePack(&g_LoadDBinfo,binID,level);
		pNewPackParam->InitParam(sureFlopItem,CTreasurePack::SUREFLOP);
		pNewPackParam->InitParam(randomFlopItem,CTreasurePack::RANDOMFLOP);
		//pNewPackParam->InitParam(randomFlopOdds,CTreasurePack::EQUIPFLOP);
		m_TreasureList[binID] = pNewPackParam;
	}
	fclose(fp);
}

void COtherManager::ReadPlayerOfficerTable(DB_Proxy *pDBProxy)
{
	if(GetMapID() != SERVER_BOUND_MAP_OTHER)
	{
		return;
	}
	PlayerOfficerTable offtb;
	PlayerOfficerTable *pRetSet = 0;
	char *buf = 0;
	int count = 0;
	if(pDBProxy->db_select_all(offtb,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		return;
	}
	count = count/sizeof(PlayerOfficerTable);
	pRetSet = (PlayerOfficerTable*)buf;
	int64 curtime = IceUtil::Time::now().toSeconds();
	int64 lTimes = 0;
	for(int i=0;i<count;i++)
	{
		if(pRetSet[i].playerid_<10000)//非玩家英雄
			continue;
		if(pRetSet[i].state_ == EN_GBH)//重伤
		{
			lTimes = pRetSet[i].gbhtime_ -curtime;
			if (lTimes<=0 || lTimes>g_IniFile.m_HeroResumeTime)
			{
				lTimes = 10;
			}
			LPTimer_Hero_Resume resume_timer = new Timer_Hero_Resume;
			resume_timer->serialize(pRetSet[i].playerid_,pRetSet[i].officerid_);
			//24小时
			//if(set_timer(HERO_RESUME_TIME,resume_timer) <0)
			if(set_timer((unsigned long)lTimes,resume_timer) <0)
			{
				printf("ReadPlayerOfficerTable :set_timer fault.\n");
				delete resume_timer;
			}
		}
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
}

//出征部队
int COtherManager::OnOutArmyEx(const void * pDataBuffer, int iDataSize, int iCase,bool bInit)
{
	if (iDataSize != sizeof(tagArmy_Reach)) 
	{
		LOG_TRACE_EX(false,("COtherManager::OnOutArmyEx-->消息包非法\n"));
		return false;
	}

	tagArmy_Reach * pOutArmy = (tagArmy_Reach *)pDataBuffer;
	int ret = 0;
	long timerid = -1;
	int handle = pOutArmy->agent;
	if(handle ==0)
	{
		handle = g_DataServer.getAgentOfPlayer(pOutArmy->lPlayerID);//m_mapOnLinePlayer[pOutArmy->lPlayerID];
	}
	if (pOutArmy->lFromMapID<=1000 || pOutArmy->lToMapID<=1000)
	{
		return 0;
	}
	PlayerChatRoomTable playertb;
	if(pOutArmy->lPlayerID >10000)
	{
		playertb.setid(pOutArmy->lPlayerID);
		if(g_Proxy.db_select(playertb) != DB_Proxy::DB_SUCCEED)
		{
			return 0;
		}
	}
	else
	{
		if (pOutArmy->lToMapID == 1003 || pOutArmy->lToMapID == 1007)
		{
			tagBackArmy_Reach BackArmy;
			BackArmy.serialize(pOutArmy->lPlayerID, pOutArmy->lFromMapID, pOutArmy->lArmyID, 1);
			ret = event_notify(pOutArmy->lFromMapID, (char * )&BackArmy, sizeof(BackArmy));// 发往服务器
			TirrierGoHome(pDataBuffer);
			return 0;
		}
		if(pOutArmy->lPlayerID == 100)
			playertb.country_ = 1;
		else if(pOutArmy->lPlayerID == 200)
			playertb.country_ = 2;
	}
	PlayerOfficerTable Officer;
	ArmyTable Army;
	ArmyOutTable ArmyOut;
	int iSpeed = 100;
	PlayerItemStorageTable itemInfo;

	int iTmpMapID = pOutArmy->lToMapID-1001;
	if(iCase ==1)
	{//增加部队转移的处理
		//查询出征部队
		ArmyOut.setmasterid(pOutArmy->lPlayerID);
		ArmyOut.setdetailid(pOutArmy->lArmyID);
		if (g_Proxy.db_select(ArmyOut) != DB_Proxy::DB_SUCCEED)
		{
			if(handle)
			{
				ERROR_INFO Error;
				Error.serialize(EN_ARMY_TRANSFER, SERVER_BOUND_MAP_OTHER, pOutArmy->lPlayerID);
				ret = asynch_write(handle, (char *)&Error, Error.length);
			}
			ret = 0;
			return 0;
		}
		iSpeed = ArmyOut.speed_;
		//查询部队信息
		Army.setmasterid(ArmyOut.outmapid_);
		Army.setdetailid(pOutArmy->lArmyID);
		if (g_Proxy.db_select(Army) != DB_Proxy::DB_SUCCEED)
		{
			//20090824
			g_Proxy.db_delete(ArmyOut);
			return 0;
		}
		Officer.setmasterid(Army.playerid_);
		Officer.setdetailid(Army.officerid_);
		if (g_Proxy.db_select(Officer) != DB_Proxy::DB_SUCCEED)
		{
		}
		if (!bInit)
		{
			if(ArmyOut.tomapid_ == pOutArmy->lToMapID)
			{//路线未改变
				return 0;
			}

			if ((m_MapState[ArmyOut.frommapid_-1001].countryid_ !=playertb.country_ ) &&(m_MapState[iTmpMapID].countryid_ != playertb.country_))
			{
				return 0;
			}
		}
		pOutArmy->lFromMapID = ArmyOut.frommapid_;

	}
	else
	{
		//获取部队信息
		Army.setmasterid(pOutArmy->lFromMapID);
		Army.setdetailid(pOutArmy->lArmyID);
		if (g_Proxy.db_select(Army) != DB_Proxy::DB_SUCCEED)
		{
			return 0;
		}
		//获取指挥官信息
		if (pOutArmy->lPlayerID >10000)
		{
			Officer.setmasterid(pOutArmy->lPlayerID);
			Officer.setdetailid(Army.officerid_);
			if(g_Proxy.db_select(Officer) != DB_Proxy::DB_SUCCEED )
			{
				if (handle >0)
				{
					ERROR_INFO Error;
					Error.serialize(EN_OPERATE_DBG_FAILED, SERVER_BOUND_MAP_OTHER, pOutArmy->lPlayerID);
					ret = asynch_write(handle, (char *)&Error, Error.length);
				}
				return 0;
			}
		}
		if (pOutArmy->sortID)
		{
			if (getExpeditionSpeedCard(pOutArmy->lPlayerID,&itemInfo) != 0)
			{
				if (handle >0)
				{
					ERROR_INFO Error;
					Error.serialize(EN_NO_ExpeditionSpeedCard, SERVER_BOUND_MAP_OTHER, pOutArmy->lPlayerID);
					ret = asynch_write(handle, (char *)&Error, Error.length);
				}
				return 0;
			}
			CardParam *pCardParam = g_LoadDBinfo.GetCardConfig(itemInfo.itemid_);
			if (pCardParam == NULL)
			{
				if (handle >0)
				{
					ERROR_INFO Error;
					Error.serialize(EN_NO_ExpeditionSpeedCard, SERVER_BOUND_MAP_OTHER, pOutArmy->lPlayerID);
					ret = asynch_write(handle, (char *)&Error, Error.length);
				}
				return 0;
			}
			iSpeed = pCardParam->Value;
		}
		
	}
	//判断行动值,出征到本方和平状态郡不需要
	if (!bInit && pOutArmy->lPlayerID >10000 && (m_MapState[iTmpMapID].countryid_ != playertb.country_ || m_MapState[iTmpMapID].state_ ==1))
	{
		LPActionDotParam pActionParam = g_LoadDBinfo.GetActionDotParam(pOutArmy->lToMapID,Country_Map);
		if (pActionParam)
		{
			int iNeedDot = 0;
			IceUtil::DateTime curdate;
			IceUtil::Time::now().toDateTime(curdate);
			tagTime_Hour curhour;
			curhour.hour = curdate.hour;
			curhour.min = curdate.min;
			if (pActionParam->Day_Start <curhour  && curhour < pActionParam->Day_End)
			{//白天
				if (m_MapState[iTmpMapID].countryid_ != playertb.country_)
				{//进攻型
					iNeedDot = pActionParam->AttackDot_Day;
				}
				else
				{
					iNeedDot = pActionParam->RecoveryDot_Day;
				}
			}
			else
			{
				if (m_MapState[iTmpMapID].countryid_ != playertb.country_)
				{//进攻型
					iNeedDot = pActionParam->AttackDot_Night;
				}
				else
				{
					iNeedDot = pActionParam->RecoveryDot_Night;
				}
			}
			if (Officer.actiondot_ < iNeedDot)
			{//计算一次行动值
				PlayerOfficerTable mainOfficer;
				mainOfficer.setmasterid(Officer.playerid_);
				mainOfficer.setdetailid(Officer.playerid_);
				if (g_Proxy.db_select(mainOfficer) == DB_Proxy::DB_SUCCEED)
				{
					ProcOfficerActionDot(mainOfficer.militaryrank_,&Officer);
				}
			}
			if (Officer.actiondot_ < iNeedDot)
			{
				if (handle>0)
				{
					ERROR_INFO Error;
					Error.serialize(EN_NOT_ACCTIONDOT, SERVER_BOUND_MAP_OTHER, pOutArmy->lPlayerID);
					ret = asynch_write(handle, (char *)&Error, Error.length);
				}
				return 0;
			}
		}
	}
	//寻路操作，目前为最短路径
	list<int> path;
	ret = OnReachMap(handle, pOutArmy->lPlayerID, pOutArmy->lFromMapID, pOutArmy->lToMapID, pOutArmy->lArmyID, &path,0);
	
	if (ret==0 && bInit)
	{
		ret = OnReachMap(handle, pOutArmy->lPlayerID, pOutArmy->lFromMapID, pOutArmy->lToMapID, pOutArmy->lArmyID, &path,1);
	}
	if (ret == 0) 
	{
		if (handle >0)
		{
			ERROR_INFO Error;
			Error.serialize(EN_NOT_REACHMAP, SERVER_BOUND_MAP_OTHER, pOutArmy->lPlayerID);
			ret = asynch_write(handle, (char *)&Error, Error.length);
		}
		return false;
	}
	
	int n = 0, iTimeLen= 0,iItem = (int)path.size();
	char * tmp = new char[sizeof(ARMY_MOVE_PATH) + sizeof(tagOutingArmysList)];//+sizeof(tagArmyMovePath)*15];
	ARMY_MOVE_PATH *pArmyPath = (ARMY_MOVE_PATH *)tmp;

	OUT_ARMY_PATH * pOutArmyTime = new OUT_ARMY_PATH;
	memset(pOutArmyTime,0,sizeof(OUT_ARMY_PATH));
	list<int>::iterator iter;
	int iError = 1;

	for (iter = path.begin();iter!=path.end();)
	{
		(*pArmyPath)[0].Path[n].iRMapID = (*iter)+1001;	
		(*pArmyPath)[0].Path[n].iDMapID = (*(++iter))+1001;
		iTimeLen = GetMapTimes(pOutArmy->lPlayerID, (*pArmyPath)[0].Path[n].iRMapID, (*pArmyPath)[0].Path[n].iDMapID, pOutArmy->lArmyID, 0);
		if(iTimeLen <=0)
		{
			ret = 0;
			goto RETCODEEX;
		}
		(*pArmyPath)[0].Path[n].iAllTime = iTimeLen * iSpeed/100;
		if ((*pArmyPath)[0].Path[n].iAllTime <=0)
		{
			(*pArmyPath)[0].Path[n].iAllTime  = 1;
		}
		(*pArmyPath)[0].Path[n].iTime = iTimeLen;
		pOutArmyTime->iMapList[n] = (*pArmyPath)[0].Path[n].iRMapID;
		if (++n >= (iItem-1)) break;
	}	
	pOutArmyTime->iMapList[n] = *(iter)+1001;
	//memset(&ArmyOut, 0, sizeof(ArmyOutTable));
	if (iCase == 1)
	{//改变路线：转移部队

		int tmpid = ArmyOut.tomapid_ -1001;
		if(!bInit && ArmyOut.tomapid_ == ArmyOut.targetmapid_ && m_MapWarmInfo[tmpid].TimerID >0)
		{//减少数量
			if(playertb.country_ == 1)
				m_MapWarmInfo[tmpid].UnionCount--;
			else if(playertb.country_ == 2)
				m_MapWarmInfo[tmpid].ClanCount--;
			else
				m_MapWarmInfo[tmpid].AboriginalCount--;
			if (m_MapWarmInfo[tmpid].UnionCount ==0 && m_MapWarmInfo[tmpid].ClanCount ==0 && m_MapWarmInfo[tmpid].AboriginalCount ==0)
			{
				cancel_timer(m_MapWarmInfo[tmpid].TimerID);
				m_MapWarmInfo[tmpid].TimerID = 0;
			}
		}
		//ArmyOut.outmapid_ = pOutArmy->lFromMapID;
		ArmyOut.targetmapid_ = pOutArmy->lToMapID;
		ArmyOut.frommapid_ = (*pArmyPath)[0].Path[0].iRMapID;
		ArmyOut.tomapid_ = (*pArmyPath)[0].Path[0].iDMapID;
		ArmyOut.outcityid_ = ArmyOut.outcityid_+1;
		ArmyOut.starttime_ = IceUtil::Time::now().toSeconds()+iTimeLen;
		if (g_Proxy.db_update(ArmyOut, ArmyOut) != DB_Proxy::DB_SUCCEED)
		{
			if(handle)
			{
				ERROR_INFO Error;
				Error.serialize(EN_ARMY_TRANSFER, SERVER_BOUND_MAP_OTHER, pOutArmy->lPlayerID);
				ret = asynch_write(handle, (char *)&Error, Error.length);
			}
			LOG_TRACE_EX(false,("玩家%ld 转移部队%ld失败 db_update\n",pOutArmy->lPlayerID,pOutArmy->lArmyID));
			goto RETCODEEX;
		}
		//应该删除原有定时器：部队到达定时器和预警定时器
		OUT_ARMY_PATH *outtimerparam = 0;
		outtimerparam = DelArmyOutTimer(pOutArmy->lPlayerID,pOutArmy->lArmyID);
		if(outtimerparam && cancel_timer(outtimerparam->TimerID) ==1)
		{
			delete outtimerparam;
			outtimerparam = 0;
		}
	}
	else
	{//出征
		ArmyOut.playerid_ = pOutArmy->lPlayerID;
		ArmyOut.outmapid_ = pOutArmy->lFromMapID;
		ArmyOut.targetmapid_ = pOutArmy->lToMapID;
		ArmyOut.frommapid_ = (*pArmyPath)[0].Path[0].iRMapID;
		ArmyOut.tomapid_ = (*pArmyPath)[0].Path[0].iDMapID;
		ArmyOut.armyid_ = pOutArmy->lArmyID;
		ArmyOut.outcityid_ = 0;//pArmyOut->lCityID;
		ArmyOut.starttime_ = IceUtil::Time::now().toSeconds()+iTimeLen;
		ArmyOut.speed_ = iSpeed;
		if (g_Proxy.db_insert(ArmyOut) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE_EX(false,("COtherManager::OnOutArmyEx 玩家%ld 部队已存在 %ld db_insert(ArmyOut)\n",pOutArmy->lPlayerID,pOutArmy->lArmyID));
			ret  =0;
			goto RETCODEEX;
		}
		//20091007出征成功，修改部队状态
		Army.state_ = GO_OUT_FOR_BATTLE;
		if (g_Proxy.db_update(Army,Army)!= DB_Proxy::DB_SUCCEED)
		{
			if(handle)
			{
				ERROR_INFO Error;
				Error.serialize(EN_DATABASE_CONNECT_FAILED, SERVER_BOUND_MAP_OTHER, pOutArmy->lPlayerID);
				ret = asynch_write(handle, (char *)&Error, Error.length);
			}
			LOG_TRACE_EX(false,("COtherManager::OnOutArmyEx 玩家%ld 部队%d修改部队状态失败%ld db_insert(ArmyOut)\n",pOutArmy->lPlayerID,pOutArmy->lArmyID,Army.state_));
			g_Proxy.db_delete(ArmyOut);
			goto RETCODEEX;
		}
		if (deleteItemOfPack(&g_Proxy,itemInfo) ==DB_Proxy::DB_SUCCEED)
		{//通知使用卡
			Notify_UseOneCard_Request notifyMsg;
			notifyMsg.serialize(handle,pOutArmy->lPlayerID,itemInfo.itemid_,SERVER_BOUND_MAP_OTHER);
			event_notify(notifyMsg.to,(char*)&notifyMsg,notifyMsg.length);
		}
	}
	//处理第一个地图
	iTimeLen = (*pArmyPath)[0].Path[0].iTime;
	pOutArmyTime->iCount = iItem;
	pOutArmyTime->serialize(pOutArmy->lPlayerID, pOutArmy->lArmyID, ArmyOut.frommapid_, ArmyOut.tomapid_, 
		ArmyOut.outmapid_, ArmyOut.targetmapid_, EN_TIMER_ARMY_OUT,Army.officerid_);

	//部队达到时间事件
	iError = 0;
	pOutArmyTime->TimerID = set_timer(iTimeLen, (const void *)pOutArmyTime);//iTime
	
	if(pOutArmyTime->TimerID <0)
	{
		LOG_TRACE_EX(false,("COtherManager::OnOutArmyEx-->设置定时器失败\n"));
		iError = 1;
		goto RETCODEEX;
	}
	//add by xujincai 20090106
	AddArmyOutTimer(pOutArmy->lPlayerID,pOutArmy->lArmyID,pOutArmyTime);

	//若下一个地图为目标郡,且阵营不同并处于和平状态，设置预警定时器
	if((*pArmyPath)[0].Path[0].iDMapID == pOutArmy->lToMapID 
		&&(m_MapState[iTmpMapID].countryid_ !=playertb.country_ || 
		(m_MapState[iTmpMapID].state_ != 0  && m_MapState[iTmpMapID].state_ != 100)))
	{

		if(m_MapWarmInfo[iTmpMapID].TimerID <=0)
		{
			ARMY_WARM *pArmyWarm = new ARMY_WARM;
			pArmyWarm->serialize(pOutArmy->lPlayerID, pOutArmy->lToMapID, pOutArmy->lArmyID,EN_TIMER_ARMY_WARM);
			pArmyWarm->TimerID = set_timer(iTimeLen-g_IniFile.m_WarmTime, (const void *)pArmyWarm);
			m_MapWarmInfo[iTmpMapID].TimerID = pArmyWarm->TimerID;
		}
		if(playertb.country_ == 1)//联盟
		{
			m_MapWarmInfo[iTmpMapID].UnionCount++;
		}
		else if(playertb.country_ == 2)
		{
			m_MapWarmInfo[iTmpMapID].ClanCount++;
		}
		else
		{
			m_MapWarmInfo[iTmpMapID].AboriginalCount++;
		}
	}
	tagBackArmy_Reach BackArmy;
	BackArmy.serialize(pOutArmy->lPlayerID, pOutArmy->lFromMapID, pOutArmy->lArmyID, 1);
	ret = event_notify(pOutArmy->lFromMapID, (char * )&BackArmy, sizeof(BackArmy));// 发往服务器
	if (ret == 1)
	{
		(*pArmyPath)[0].iRaceID = Army.raceid_;
		(*pArmyPath)[0].iSoldierType = Army.soldiertype_;
		(*pArmyPath)[0].iSoldierCount = Army.firstcount_;
		(*pArmyPath)[0].lOfficerID = Officer.officerid_;
		(*pArmyPath)[0].lArmyID = Army.armyid_;
		(*pArmyPath)[0].iItem = iItem-1;
		strcpy((*pArmyPath)[0].szArmyName, Army.armyname_);
		strcpy((*pArmyPath)[0].szOfficerName, Officer.officername_);
		pArmyPath->serialize(1, SERVER_BOUND_MAP_OTHER, pOutArmy->lPlayerID,EN_S_ARMY_MOVE_PATH);
		if(handle)
		{
			ret = asynch_write(handle, (char *)pArmyPath, pArmyPath->length);
		}
	}
	else
	{
		printf("%ld 通知部队 出征失败---从郡内%ld城市中出征到郡%d,部队1有%d\n",pOutArmy->lPlayerID,pOutArmy->lFromMapID, pOutArmy->lToMapID,  pOutArmy->lArmyID);
	}

RETCODEEX:

	if (tmp != NULL)
	{
		delete [] tmp;
		tmp = NULL;
	}
	if(iError)
	{
		delete pOutArmyTime;
		pOutArmyTime = 0;
	}
	return ret;
}


//部队达到时间事件
void COtherManager::ArmyOutTimeEventEx(void * pDataBuffer, int iDataSize)
{	
	if (iDataSize != sizeof(OUT_ARMY_PATH))
	{
		LOG_TRACE_EX(false,("COtherManager::ArmyOutTimeEventEx-->消息包失败\n"));
		return;
	}
	
	OUT_ARMY_PATH * pOutArmy = (OUT_ARMY_PATH *)pDataBuffer;
	if (pOutArmy->isDeleted)
	{
		printf("玩家%d的部队%d从郡%d出征到郡%d定时器已取消\n",pOutArmy->lPlayerID,pOutArmy->lArmyID,pOutArmy->lFromMapID,pOutArmy->lToMapID);
		return;
	}

	long timerid = -1;
	int ret = 0,iflag = 0,iCountry=0,n = 0;
	ArmyOutTable ArmyOut;
	memset(&ArmyOut, 0, sizeof(ArmyOutTable));
	ArmyOut.setmasterid(pOutArmy->lPlayerID);
	ArmyOut.setdetailid(pOutArmy->lArmyID);
	if (g_Proxy.db_select(ArmyOut) != DB_Proxy::DB_SUCCEED)
	{
		printf("玩家%ld %ld到郡%ld 部队%ld ArmyOutTable find failed!!!\n",pOutArmy->lPlayerID,pOutArmy->lFromMapID, pOutArmy->lToMapID,pOutArmy->lArmyID);
		return;
	}
	for (n = 0; n < pOutArmy->iCount; n++)
	{
		if (pOutArmy->iMapList[n] == pOutArmy->lToMapID) 
			break;
	}
	if(n >= pOutArmy->iCount)
	{
		return;
	}
	n++;//下个郡地图
	if(n == pOutArmy->iCount)
	{//最后一个地图：目的郡，通知郡服务器部队到达
		tagArmy_Reach ArmyReach;
		ArmyReach.serialize(pOutArmy->lPlayerID, pOutArmy->lOutMapID, pOutArmy->lFromMapID, pOutArmy->lTargetMapID, pOutArmy->lArmyID,0, CMD_ARMY_ARRIVED);
		ret = event_notify(pOutArmy->lTargetMapID, (char * )&ArmyReach, ArmyReach.length);// 发往服务器
 		if(ret == 1)
		{
			//add by xujincai 20090106
			//OUT_ARMY_PATH *timerparam = 0;
			DelArmyOutTimer(pOutArmy->lPlayerID,pOutArmy->lArmyID);
		}
		else
		{
			OUT_ARMY_PATH * pOutArmyTime = new OUT_ARMY_PATH;
			memcpy(pOutArmyTime,pOutArmy,sizeof(OUT_ARMY_PATH));
			printf("玩家%d部队%d到达目标郡%d，目标郡服务器掉线\n",pOutArmyTime->lPlayerID,pOutArmyTime->lArmyID,pOutArmyTime->lTargetMapID);

			//部队达到时间事件
			pOutArmyTime->TimerID = set_timer(60, (const void *)pOutArmyTime);//iTime
			//add by xujincai 20090106
			if(pOutArmyTime->TimerID  <0)
			{
				LOG_TRACE_EX(false,("COtherManager::ArmyOutTimeEventEx-->通知郡失败，玩家%d设置部队到达定时器失败\n",pOutArmy->lPlayerID));
			}
			AddArmyOutTimer(pOutArmy->lPlayerID,pOutArmy->lArmyID,pOutArmyTime);
		}
	}
	else
	{//继续下个郡地图
		int iToMapID = pOutArmy->iMapList[n];
		if(iToMapID <=0)
		{
			iToMapID = pOutArmy->lTargetMapID;
		}
		int iTimeLen = GetMapTimes(pOutArmy->lPlayerID, pOutArmy->lToMapID, iToMapID, pOutArmy->lArmyID, 0) * ArmyOut.speed_/100;
		if (iTimeLen <=0)
		{
			iTimeLen = 1;
		}
		OUT_ARMY_PATH * pOutArmyTime = new OUT_ARMY_PATH;
		pOutArmyTime->iCount = pOutArmy->iCount;
		for (int i = 0; i < pOutArmy->iCount; i++)
			pOutArmyTime->iMapList[i] = pOutArmy->iMapList[i];
		pOutArmyTime->serialize(pOutArmy->lPlayerID, pOutArmy->lArmyID, pOutArmy->lToMapID, iToMapID, 
			pOutArmy->lOutMapID, pOutArmy->lTargetMapID, EN_TIMER_ARMY_OUT,pOutArmy->lOfficerID);
		//部队达到时间事件
		pOutArmyTime->TimerID = set_timer(iTimeLen, (const void *)pOutArmyTime);//iTime
		//add by xujincai 20090106
		if(pOutArmyTime->TimerID  <0)
		{
			LOG_TRACE_EX(false,("COtherManager::ArmyOutTimeEventEx-->玩家%d设置部队到达定时器失败\n",pOutArmy->lPlayerID));
		}
		AddArmyOutTimer(pOutArmy->lPlayerID,pOutArmy->lArmyID,pOutArmyTime);
		ArmyOut.frommapid_ = pOutArmyTime->lFromMapID;
		ArmyOut.tomapid_ = pOutArmyTime->lToMapID;
		ArmyOut.starttime_ = IceUtil::Time::now().toSeconds()+iTimeLen;
		g_Proxy.db_update(ArmyOut, ArmyOut);

		if(n == pOutArmy->iCount -1)
		{//下个郡地图为目标郡，则进行预警判断
			//若下一个地图为目标郡,且阵营不同并处于和平状态，设置预警定时器
			PlayerChatRoomTable playertb;
			if(pOutArmy->lPlayerID >10000)
			{
				playertb.setid(pOutArmy->lPlayerID);
				g_Proxy.db_select(playertb);
			}
			else
			{
				if(pOutArmy->lPlayerID == 100)
					playertb.country_ = 1;
				else if(pOutArmy->lPlayerID == 200)
					playertb.country_ = 2;
			}
			int iTmpMapID = iToMapID-1001;
			if((m_MapState[iTmpMapID].countryid_ !=playertb.country_)/*&&m_MapState[iToMapID-1001].state_ == 0*/
				||(m_MapState[iTmpMapID].state_ !=0 && m_MapState[iTmpMapID].state_ != 100))
			{
				if(m_MapWarmInfo[iTmpMapID].TimerID <=0)
				{
					ARMY_WARM *pArmyWarm = new ARMY_WARM;
					pArmyWarm->serialize(pOutArmy->lPlayerID, iToMapID, pOutArmy->lArmyID,EN_TIMER_ARMY_WARM);
					pArmyWarm->TimerID = set_timer(iTimeLen-g_IniFile.m_WarmTime, (const void *)pArmyWarm);
					m_MapWarmInfo[iTmpMapID].TimerID = pArmyWarm->TimerID;
				}
				if(playertb.country_ == 1)//联盟
				{
					m_MapWarmInfo[iTmpMapID].UnionCount++;
				}
				else if(playertb.country_ == 2)
				{
					m_MapWarmInfo[iTmpMapID].ClanCount++;
				}
				else
				{
					m_MapWarmInfo[iTmpMapID].AboriginalCount++;
				}
			}
		}
	}
}
void COtherManager::TirrierGoHome(const void *msg)
{
	tagArmy_Reach * pOutArmy = (tagArmy_Reach *)msg;
	if (pOutArmy->lPlayerID == 100 && pOutArmy->lToMapID != 1003)
	{
		return;
	}
	if (pOutArmy->lPlayerID == 200 && pOutArmy->lToMapID != 1007)
	{
		return;
	}
	tagArmy_Reach ArmyReach;
	ArmyReach.serialize(pOutArmy->lPlayerID, pOutArmy->lFromMapID, pOutArmy->lFromMapID, pOutArmy->lToMapID, pOutArmy->lArmyID, 0,CMD_ARMY_ARRIVED);
	event_notify(pOutArmy->lToMapID, (char * )&ArmyReach, ArmyReach.length);// 发往服务器
}
//add by xujincai 20081226
//英雄重伤通知
void COtherManager::OnHeroGBHNotify(void *pDataBuffer)
{
	if(pDataBuffer == NULL)
	{
		LOG_TRACE_EX(false,("COtherManager::OnHeroGBHNotify-->消息包地址非法\n"));
		return ;
	}
	LPHeroGBH_Notify req_msg = (LPHeroGBH_Notify)pDataBuffer;
	if(req_msg->length != sizeof(HeroGBH_Notify))
	{
		LOG_TRACE_EX(false,("COtherManager::OnHeroGBHNotify(-->消息包非法\n"));
		return;
	}
	LOG_ERROR_EX(false,("玩家%d的英雄%d重伤。\n",req_msg->from,req_msg->OfficerID));
	LPTimer_Hero_Resume resume_timer = new Timer_Hero_Resume;
	resume_timer->serialize(req_msg->from,req_msg->OfficerID);
	//24小时
	//if(set_timer(HERO_RESUME_TIME,resume_timer) <0)
	if(set_timer(g_IniFile.m_HeroResumeTime,resume_timer) <0)
	{
		printf("OnHeroGBHNotify :set_timer fault.\n");
		delete resume_timer;
	}
	//计算一次行动值
	PlayerOfficerTable mainOfficer,officer;
	mainOfficer.setmasterid(req_msg->from);
	mainOfficer.setdetailid(req_msg->from);
	if (g_Proxy.db_select(mainOfficer) == DB_Proxy::DB_SUCCEED)
	{
		ProcOfficerActionDot(mainOfficer.militaryrank_,&officer);
	}
}
//英雄重伤恢复定时器
void COtherManager::HeroResumeTimerEvent(void * pDataBuffer)
{
	if(pDataBuffer == NULL)
	{
		LOG_TRACE_EX(false,("COtherManager::HeroResumeTimerEvent-->消息包地址非法\n"));
		return;
	}
	LPTimer_Hero_Resume req_msg = (LPTimer_Hero_Resume)pDataBuffer;
	if(req_msg->length != sizeof(Timer_Hero_Resume))
	{
		LOG_TRACE_EX(false,("COtherManager::HeroResumeTimerEvent-->消息包非法\n"));
		return;
	}
	//首先判断英雄状态
	PlayerOfficerTable officertb;
	officertb.setmasterid(req_msg->PlayerID);
	officertb.setdetailid(req_msg->OfficerID);
	if(g_Proxy.db_select(officertb) != DB_Proxy::DB_SUCCEED)
	{
		LOG_ERROR_EX(false,("玩家%d的英雄%d重伤恢复：查询英雄失败。\n",req_msg->PlayerID,req_msg->OfficerID));
		return;
	}

	if(officertb.state_ != EN_GBH)
	{//非重伤状态
		LOG_ERROR_EX(false,("玩家%d的英雄%d重伤恢复:非重伤状态：%d。\n",req_msg->PlayerID,req_msg->OfficerID,officertb.state_));
		return;
	}

	officertb.state_ = 0;
	officertb.updatetime_ = IceUtil::Time::now().toSeconds();//
	officertb.gbhtime_ = 0;
	if(g_Proxy.db_update(officertb,officertb) != DB_Proxy::DB_SUCCEED)
	{
		LOG_ERROR_EX(false,("玩家%d修改英雄%d重伤状态失败\n",req_msg->PlayerID,req_msg->OfficerID));
		//20100408
		ISystemLog::getInstance()->heroState_Resume_Log(officertb.playerid_,officertb.officerid_,officertb.officername_);
	}
	else
	{
		LOG_ERROR_EX(false,("玩家%d的英雄【%s-%d】重伤恢复成功:%d。\n",req_msg->PlayerID,officertb.officername_,req_msg->OfficerID,officertb.state_));
	}
}


int COtherManager::AddArmyOutTimer(unsigned long playerid,int armyid,OUT_ARMY_PATH *param)
{
	IceUtil::Mutex::Lock lock(m_TimerMutex);
	m_ArmyOutTimerList[armyid] = param;
	return 0;
}
long COtherManager::GetArmyOutTimer(unsigned long playerid,int armyid)
{
	IceUtil::Mutex::Lock lock(m_TimerMutex);
	map<unsigned long,OUT_ARMY_PATH*>::iterator iter;
	iter = m_ArmyOutTimerList.find(armyid);
	if(iter == m_ArmyOutTimerList.end())
		return -1;
	return iter->second->TimerID;
}
OUT_ARMY_PATH* COtherManager::DelArmyOutTimer(unsigned long playerid,int armyid)
{
	IceUtil::Mutex::Lock lock(m_TimerMutex);
	map<unsigned long,OUT_ARMY_PATH*>::iterator iter;
	unsigned long key = armyid;
	iter = m_ArmyOutTimerList.find(key);
	if(iter == m_ArmyOutTimerList.end())
		return NULL;
	OUT_ARMY_PATH *pParam = iter->second;
	pParam->isDeleted = true;
	m_ArmyOutTimerList.erase(iter);
	return pParam;
}
//立即完成,英雄招募
void COtherManager::OnImmedComplete(LPImmedComplete_Request msg)
{
	LPImmedComplete_Request req_msg = (LPImmedComplete_Request)msg;
	if(req_msg->length != sizeof(ImmedComplete_Request))
	{
		LOG_TRACE_EX(false,("COtherManager::OnImmedComplete-->消息包非法\n"));
		return;
	}
	Notify_ImmedComplete_Request notify_msg;
	ImmedComplete_Response rep_msg;
	int iMilitaryRank = 0;
	int iOfficerCount = 0;
	PlayerOfficerTable officer;
	char * buf = 0;
	officer.setmasterid(req_msg->from);
	if (g_Proxy.db_multi_select(officer,buf,iOfficerCount) != DB_Proxy::DB_SUCCEED)
	{
		return;
	}
	PlayerOfficerTable *pOfficerList = (PlayerOfficerTable*)buf;
	iOfficerCount = iOfficerCount/sizeof(PlayerOfficerTable);
	for (int i=0;i<iOfficerCount;i++)
	{
		if (pOfficerList[i].playerid_ == pOfficerList[i].officerid_)
		{
			iMilitaryRank = pOfficerList[i].militaryrank_;
			break;
		}
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	LPQuantityByRank_Item pQuantityByRank = g_LoadDBinfo.GetQuantityByRank(iMilitaryRank);

	if(pQuantityByRank == NULL || iOfficerCount >= pQuantityByRank->HeroQuantity)
	{
		ERROR_INFO errorInfo;
		errorInfo.serialize(EN_OFFICER_COUNT_UPPER, SERVER_BOUND_MAP_OTHER, req_msg->from);
		errorInfo.agent = req_msg->agent;
		asynch_write(req_msg->agent, (char *)&errorInfo, errorInfo.length);
		return;
	}
	map<unsigned long, __int64>::iterator it = m_mapRecruitingOfficer.find(req_msg->from);
	if(it == m_mapRecruitingOfficer.end())//可以招募，不需要使用
	{
		rep_msg.serialize(req_msg->agent,SERVER_BOUND_MAP_OTHER,req_msg->from,3,req_msg->Type,req_msg->SortID,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	__int64 iTimes = it->second - IceUtil::Time::now().toSeconds();//剩余时间,24GO点/小时--》12GO点
	iTimes = (iTimes) /300;
	if (iTimes <=0)
	{
		iTimes = 1;
	}
	notify_msg.serialize(req_msg->agent,req_msg->from,Recruit_Card,req_msg->MapID,req_msg->CityID,req_msg->ItemID,
		req_msg->SortID,req_msg->Level,(int)iTimes,req_msg->ProductionType);
	if(event_notify(MDM_GP_MAP_CHARGE,(char*)&notify_msg,notify_msg.length) ==0)
	{//通知失败
		rep_msg.serialize(req_msg->agent,SERVER_BOUND_MAP_OTHER,req_msg->from,111,req_msg->Type,req_msg->SortID,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	}

}
//处理收费系统返回的通知消息
void COtherManager::OnImmedCompleteNotifyMsg(LPNotify_ImmedComplete_Response msg)
{
	ImmedComplete_Response rep_msg;
	if(msg->Error_Code != 0)
	{//扣费失败
		rep_msg.serialize(msg->agent,SERVER_BOUND_MAP_OTHER,msg->from,msg->Error_Code,msg->Param.Type,msg->Param.SortID,0);
	}
	else
	{
		m_mapRecruitingOfficer.erase(msg->from);
		rep_msg.serialize(msg->agent,SERVER_BOUND_MAP_OTHER,msg->from,msg->Error_Code,msg->Param.Type,msg->Param.SortID,msg->Param.GOCount);
	}
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
}

//add by xujincai 20090224 玩家所有部队信息
//获取部队列表
void COtherManager::OnGetArmyList(const char *msg)
{
	LPGetArmyList_Request req_msg = (LPGetArmyList_Request)msg;
	//_ASSERT(req_msg->length == sizeof(GetArmyList_Request));
	PlayerOfficerTable officertb;
	PlayerOfficerTable *pOfficerList = 0;
	char *buf = 0 ;
	int count = 0;
	officertb.setmasterid(req_msg->from);
	if(g_Proxy.db_multi_select(officertb,buf,count) == DB_Proxy::DB_FAILED)
	{
		//_ASSERT(false);
		return;
	}
	count = count/sizeof(PlayerOfficerTable);
	pOfficerList = (PlayerOfficerTable*)buf;
	LPGetArmyList_Response rep_msg = (LPGetArmyList_Response)new char[sizeof(GetArmyList_Response) + count * sizeof(PlayerArmyInfo)];
	int index = 0;
	ArmyTable armyinfo;
	int iRet = 0;
	for(int i=0;i<count;i++)
	{
		if(pOfficerList[i].state_ != 1)
			continue;
		armyinfo.setmasterid(pOfficerList[i].mapid_);
		armyinfo.setdetailid(pOfficerList[i].armyid_);
		iRet = g_Proxy.db_select(armyinfo);
		if(iRet == DB_Proxy::DB_FAILED)
		{
			continue;
		}
		else if(iRet == DB_Proxy::DB_EOF)
		{//20090824 处理英雄状态错误问题
			pOfficerList[i].state_ = 0;
			g_Proxy.db_update(pOfficerList[i],pOfficerList[i]);
			continue;
		}

		if (armyinfo.firstcount_ <=0)
		{
			g_Proxy.db_delete(armyinfo);
			ArmyOutTable outtb;
			outtb.setmasterid(armyinfo.playerid_);
			outtb.setdetailid(armyinfo.armyid_);
			g_Proxy.db_delete(outtb);
			pOfficerList[i].state_ = 0;
			g_Proxy.db_update(pOfficerList[i],pOfficerList[i]);
			continue;
		}
		(*rep_msg)[index].OfficerID = pOfficerList[i].officerid_;
		strcpy((*rep_msg)[index].OfficerName,pOfficerList[i].officername_);
		(*rep_msg)[index].ArmyID = armyinfo.armyid_;
		strcpy((*rep_msg)[index].ArmyName,armyinfo.armyname_);
		(*rep_msg)[index].Mapid = armyinfo.mapid_;
		(*rep_msg)[index].Soldiertype = armyinfo.soldiertype_;
		(*rep_msg)[index].Raceid = armyinfo.raceid_;
		(*rep_msg)[index].Firstcount = armyinfo.firstcount_;
		(*rep_msg)[index].Fweapon = armyinfo.fweapon_;
		(*rep_msg)[index].Fdefendtype = armyinfo.fdefendtype_;
		(*rep_msg)[index].State = armyinfo.state_;
		index++;

	}
	rep_msg->serialize(req_msg->agent,req_msg->from,index);
	asynch_write(rep_msg->agent,(char*)rep_msg,rep_msg->length);
	delete[] (char*)rep_msg;
	rep_msg = 0;
}
//部队状态改变
void COtherManager::OnModifyArmyState(const char *msg)
{
	LPNotify_ModifyArmyState req_msg = (LPNotify_ModifyArmyState)msg;
	//_ASSERT(req_msg->length == sizeof(Notify_ModifyArmyState));

	int agent = 0;
	ModifyArmyState_Response rep_msg;
	if(req_msg->from == 100 || req_msg->from == 200)
	{
		//国防军,通知公会系统
		//rep_msg.serialize(agent,req_msg->from,req_msg->armyInfo.ArmyID,req_msg->armyInfo.Mapid,req_msg->armyInfo.Firstcount,req_msg->armyInfo.State);
		event_notify(MDM_GP_MAP_CONSORTIA,(char*)req_msg,req_msg->length);
	}
	else
	{
		agent = g_DataServer.getAgentOfPlayer(req_msg->from);//m_mapOnLinePlayer[req_msg->from];
		if(agent <=0)
		{
			//printf("收到玩家%d的部队%d(MapID:%d)状态改变的消息，新状态：%d，但大地图发现玩家不在线（可能需要重新登录）\n",req_msg->from,req_msg->armyInfo.ArmyID,req_msg->armyInfo.Mapid,req_msg->armyInfo.State);

			return;
		}
		//printf("收到玩家%d的部队%d(MapID:%d)状态改变的消息，新状态：%d\n",req_msg->from,req_msg->armyInfo.ArmyID,req_msg->armyInfo.Mapid,req_msg->armyInfo.State);

		rep_msg.armyInfo.ArmyName[0] = 0;
		rep_msg.armyInfo.OfficerName[0] = 0;
		rep_msg.serialize(agent,req_msg->from,req_msg->armyInfo.ArmyID,req_msg->armyInfo.Mapid,req_msg->armyInfo.Firstcount,req_msg->armyInfo.State);

		asynch_write(agent,(char*)&rep_msg,rep_msg.length);
	}
}
//国防军出征
void COtherManager::OnTirrierGoOut(const char *msg)
{
}
//判断是否为新手村
bool COtherManager::IsSafeMap(int mapid,int countryid)
{
	int size = 0;
	int i = 0;
	if(countryid == 1)
	{
		size = (int)m_UnionSafeMap.size();
		for(i = 0;i<size;i++)
		{
			if(m_UnionSafeMap[i] == mapid)
				return true;
		}
	}
	else if(countryid == 2)
	{
		size = (int)m_TribeSafeMap.size();
		for(i = 0;i<size;i++)
		{
			if(m_TribeSafeMap[i] == mapid)
				return true;
		}
	}
	return false;
}
//恢复部队出征
void COtherManager::InitArmyOut(int mapid,DB_Proxy *pDBProxy)
{
	if(mapid != m_lMapID)
		return;

	printf("大地图恢复部队出征\n");
	ArmyOutTable outarmytb;
	ArmyOutTable *pArmyList = 0;
	char *buf = 0;
	int count = 0;
	if (pDBProxy->db_select_all(outarmytb, buf, count) != DB_Proxy::DB_SUCCEED)
	{
		printf("没有正处于出征中的部队列表\n");
		return;
	}
	count = count/sizeof(ArmyOutTable);
	pArmyList = (ArmyOutTable*)buf;
	tagArmy_Reach TranArmy;
	for(int i=0;i<count;i++)
	{
		TranArmy.serialize(pArmyList[i].playerid_, pArmyList[i].frommapid_, 0, pArmyList[i].targetmapid_, pArmyList[i].armyid_,0,CMD_ARMY_REACH);
		TranArmy.agent = 0;
		OnOutArmyEx(&TranArmy, TranArmy.length, 1,true);//modify by xujincai 20081224  OnOutArmy->OnOutArmyEx
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
}
//查询俘虏英雄
void COtherManager::OnQueryCaptiveList(const char *msg)
{
	LPQueryCaptiveList_Request req_msg = (LPQueryCaptiveList_Request)msg;
	char *buf = 0;
	int count = 0;
	PlayerCaptiveHero captivetb;
	PlayerCaptiveHero *pCaptiveList = 0;
	captivetb.setmasterid(req_msg->from);
	if(g_Proxy.db_multi_select(captivetb,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		count = 0;
	}
	count = count/sizeof(PlayerCaptiveHero);
	pCaptiveList = (PlayerCaptiveHero*)buf;
	char *pBuf = new char[sizeof(QueryCaptiveList_Response) + sizeof(CaptiveInfo) * count];
	LPQueryCaptiveList_Response rep_msg = (LPQueryCaptiveList_Response)pBuf;
	PlayerOfficerTable officertb;
	int iIndex = 0;
	__int64 curtime = IceUtil::Time::now().toSeconds();
	for(int i=0;i<count;i++)
	{
		officertb.setmasterid(pCaptiveList[i].byplayerid_);
		officertb.setdetailid(pCaptiveList[i].byofficerid_);
		if(g_Proxy.db_select(officertb) != DB_Proxy::DB_SUCCEED)
			continue;
		if(officertb.state_ != EN_BY_CAPTIVE)
		{
			g_Proxy.db_delete(pCaptiveList[i]);
			continue;
		}
		/*int Officerrace;int Level;int Militaryrank;int Jobid;int Faceid;
	int Stamina;int Power;int Celerity;int Wit;int Techdot;int Attack;int Physicrecovery;int Magicrecovery;
	unsigned int Times;//已抓获时间，单位毫秒*/
		(*rep_msg)[iIndex].PlayerID = pCaptiveList[i].byplayerid_;
		(*rep_msg)[iIndex].OfficerID = pCaptiveList[i].byofficerid_;
		(*rep_msg)[iIndex].Officerrace = officertb.officerrace_;
		(*rep_msg)[iIndex].Level = officertb.level_;
		(*rep_msg)[iIndex].Militaryrank = officertb.militaryrank_;
		(*rep_msg)[iIndex].Jobid = officertb.jobid_;
		(*rep_msg)[iIndex].Faceid = officertb.faceid_;
		(*rep_msg)[iIndex].Stamina = officertb.stamina_ + officertb.levelstamina_;
		(*rep_msg)[iIndex].Power = officertb.power_ + officertb.levelpower_;
		(*rep_msg)[iIndex].Celerity = officertb.celerity_ + officertb.levelcelerity_;
		(*rep_msg)[iIndex].Wit = officertb.wit_ + officertb.levelwit_;
		(*rep_msg)[iIndex].EquipStamina = officertb.equipstamina_;
		(*rep_msg)[iIndex].EquipPower = officertb.equippower_;
		(*rep_msg)[iIndex].EquipCelerity = officertb.equipcelerity_;
		(*rep_msg)[iIndex].EquipWit = officertb.equipwit_;
		(*rep_msg)[iIndex].Techdot = officertb.techdot_;
		(*rep_msg)[iIndex].Attack = officertb.attack_;
		(*rep_msg)[iIndex].Physicrecovery = officertb.physicrecovery_;
		(*rep_msg)[iIndex].Magicrecovery = officertb.magicrecovery_;
		(*rep_msg)[iIndex].MagicAttack = officertb.magicattack_;
		(*rep_msg)[iIndex].Times = unsigned int(curtime - pCaptiveList[i].captivetimes_);
		strcpy((*rep_msg)[iIndex].OfficerName,officertb.officername_);
		if(pCaptiveList[i].byplayerid_ == pCaptiveList[i].byofficerid_)
		{//主英雄
			strcpy((*rep_msg)[iIndex].PlayerName,officertb.officername_);
		}
		else
		{
			officertb.setmasterid(pCaptiveList[i].byplayerid_);
			officertb.setdetailid(pCaptiveList[i].byplayerid_);
			if(g_Proxy.db_select(officertb) == DB_Proxy::DB_SUCCEED)
			{
				strcpy((*rep_msg)[iIndex].PlayerName,officertb.officername_);
			}
		}
		iIndex++;
	}
	rep_msg->serialize(req_msg->agent,req_msg->from,iIndex);
	asynch_write(rep_msg->agent,(char*)rep_msg,rep_msg->length);
	if(pBuf)
	{
		delete[] pBuf;
		pBuf = 0;
	}
}
//创建NPC反击部队
void COtherManager::NotifyCreateNPCArmy(const char *msg)
{
	LPNotify_CreateNPCArmy_Request req_msg = (LPNotify_CreateNPCArmy_Request)msg;
	if (req_msg->from <SERVER_BOUND_MAP_BEGIN || req_msg->from >MAX_SERVER_NUM)
	{
		return;
	}
	Notify_CreateNPCArmy_Response rep_msg;
	rep_msg.serialize(req_msg->agent,req_msg->PlayerID,0,req_msg->GMCmd,req_msg->CountryID,req_msg->Level,
		req_msg->Count,req_msg->FromMap,req_msg->TargetMap,req_msg->Times,req_msg->Interval);
	
	//大地图寻路，避免GM填写的开始郡无法直接到达目的郡,暂定使用相邻的郡,新手村不能增派NPC
	int iTmpFrmMap = req_msg->FromMap-1001;
	int iTmpMapID = req_msg->TargetMap - 1001;
	int iPreMap = req_msg->FromMap;
	if (iTmpFrmMap <0 || iTmpFrmMap >=BIG_MAP_SIZE || iTmpMapID<0 || iTmpMapID>=BIG_MAP_SIZE
		|| req_msg->CountryID <0 || req_msg->CountryID >3 || iTmpMapID == iTmpFrmMap)
	{
		rep_msg.Error_Code = 3;
		event_notify(req_msg->from,(char*)&rep_msg,rep_msg.length);
		return;
	}
	if (m_MapState[iTmpMapID].state_ == 100)
	{//安全村无法到达
		rep_msg.Error_Code = 1;
		event_notify(req_msg->from,(char*)&rep_msg,rep_msg.length);
		return;
	}
	if (m_MapInfo[iTmpFrmMap][iTmpMapID] <=0)
	{
		list<int> path;
		if (BigMapFindPath(m_MapInfo,iTmpFrmMap,iTmpMapID,&path) <0)
		{
			rep_msg.Error_Code = 1;
			event_notify(req_msg->from,(char*)&rep_msg,rep_msg.length);
			return;
		}
		if (path.size() <=1)
		{
			rep_msg.Error_Code = 1;
			event_notify(req_msg->from,(char*)&rep_msg,rep_msg.length);
			return;
		}
		list<int>::reverse_iterator rIter = path.rbegin();
		iPreMap = *(++rIter) + 1001;		
	}

	GMCreateNPCParam_MultiMap_Iterator lower_iter,upper_iter;
	lower_iter = g_LoadDBinfo.GMCreateNPCParam_Lowerbound(req_msg->CountryID,req_msg->Level);
	upper_iter = g_LoadDBinfo.GMCreateNPCParam_Upperbound(req_msg->CountryID,req_msg->Level);
	if (lower_iter == upper_iter)
	{
		rep_msg.Error_Code = 2;
		event_notify(req_msg->from,(char*)&rep_msg,rep_msg.length);
		return;
	}
	int iCount = 0;
	LPGMCreateNPCParam_Item pCreateNPCParam = NULL;
	for (;lower_iter != upper_iter;lower_iter++)
	{
		pCreateNPCParam = lower_iter->second;
		if (pCreateNPCParam ==NULL)
		{
			continue;
		}
		iCount = iCount + (req_msg->Count * pCreateNPCParam->AymyCount);
	}
	if (m_MapState[iTmpMapID].countryid_ !=req_msg->CountryID ||
		(m_MapState[iTmpMapID].state_ != 0  && m_MapState[iTmpMapID].state_ != 100))
	{
		if(m_MapWarmInfo[iTmpMapID].TimerID <=0)
		{
			ARMY_WARM *pArmyWarm = new ARMY_WARM;
			pArmyWarm->serialize(0, req_msg->TargetMap, 0,EN_TIMER_ARMY_WARM);
			pArmyWarm->TimerID = set_timer(g_IniFile.m_WarmTime, (const void *)pArmyWarm);
			m_MapWarmInfo[iTmpMapID].TimerID = pArmyWarm->TimerID;
		}
		if(req_msg->CountryID == 1)//联盟
		{
			m_MapWarmInfo[iTmpMapID].UnionCount += iCount;
		}
		else if(req_msg->CountryID == 2)//部落
		{
			m_MapWarmInfo[iTmpMapID].ClanCount += iCount;
		}
		else
		{
			m_MapWarmInfo[iTmpMapID].AboriginalCount += iCount;
		}
	}
	//设置创建NPC部队的定时器，60秒
	Timer_CreateNPCArmy *pCreateNPCArmy = new Timer_CreateNPCArmy;
	pCreateNPCArmy->serialize(req_msg->CountryID,req_msg->Level,req_msg->Count,iPreMap,req_msg->TargetMap,req_msg->Times,req_msg->Interval);
	pCreateNPCArmy->TimerID = set_timer(60,(char*)pCreateNPCArmy);

	//rep_msg.Error_Code = 3;
	event_notify(req_msg->from,(char*)&rep_msg,rep_msg.length);
}
void COtherManager::CreateNPCArmyTimer(const char *msg)
{
	Timer_CreateNPCArmy *pCreateNPCArmy = (Timer_CreateNPCArmy*)msg;
	GMCreateNPCParam_MultiMap_Iterator lower_iter,upper_iter;
	lower_iter = g_LoadDBinfo.GMCreateNPCParam_Lowerbound(pCreateNPCArmy->CountryID,pCreateNPCArmy->Level);
	upper_iter = g_LoadDBinfo.GMCreateNPCParam_Upperbound(pCreateNPCArmy->CountryID,pCreateNPCArmy->Level);
	LPGMCreateNPCParam_Item pCreateNPCParam = NULL;
	int iCount = 0;
	ArmyTable newNPCArmy;
	tagArmy_Reach armyReach;
	/*	
	armyReach.serialize(pOutArmy->lPlayerID, pOutArmy->lOutMapID, pOutArmy->lFromMapID, pOutArmy->lTargetMapID, pOutArmy->lArmyID, CMD_ARMY_ARRIVED);
	ret = event_notify(pOutArmy->lTargetMapID, (char * )&ArmyReach, ArmyReach.length);// 发往服务器
	*/
	for (;lower_iter != upper_iter;lower_iter++)
	{
		pCreateNPCParam = lower_iter->second;
		if(pCreateNPCParam == NULL)
			continue;
		//本类型部队的总数
		iCount = pCreateNPCParam->AymyCount * pCreateNPCArmy->Count;
		//创建部队（默认所属地图为大地图），通知郡地图部队到达
		memset(&newNPCArmy,0,sizeof(ArmyTable));
		strcpy(newNPCArmy.armyname_,pCreateNPCParam->AmyName);
		newNPCArmy.playerid_ = pCreateNPCParam->PlayerID;
		newNPCArmy.officerid_ = pCreateNPCParam->OfficerID;
		newNPCArmy.mapid_ = SERVER_BOUND_MAP_OTHER;
		newNPCArmy.soldiertype_ = pCreateNPCParam->SoldierType;
		newNPCArmy.raceid_ = pCreateNPCParam->RaceID;
		newNPCArmy.firstcount_ = pCreateNPCParam->SoldierCount;
		newNPCArmy.fsoldierlevel_= 1;
		newNPCArmy.fweapon_ = pCreateNPCParam->WeaponType;
		newNPCArmy.fweaponlevel_ = pCreateNPCParam->WeaponLevel;
		newNPCArmy.fdefendtype_ = pCreateNPCParam->DefendType;
		newNPCArmy.fdefendlevel_ = pCreateNPCParam->DefendLevel;
		newNPCArmy.state_ = 2;
		newNPCArmy.countryid_ = pCreateNPCArmy->CountryID;
		newNPCArmy.direction = 1;
		for (int i=0;i<iCount;i++)
		{
			newNPCArmy.armyid_ = 0;
			if (g_Proxy.db_insert(newNPCArmy) != DB_Proxy::DB_SUCCEED)
			{
				printf("GM 创建所属%d NPC部队失败,目标郡%d\n",pCreateNPCArmy->CountryID,pCreateNPCArmy->TargetMap);
				continue;
			}
			armyReach.serialize(newNPCArmy.playerid_,newNPCArmy.mapid_, pCreateNPCArmy->FromMap, pCreateNPCArmy->TargetMap, newNPCArmy.armyid_, 0,CMD_ARMY_ARRIVED);
			if(event_notify(pCreateNPCArmy->TargetMap, (char * )&armyReach, armyReach.length) <=0)
			{
				g_Proxy.db_delete(newNPCArmy);
				pCreateNPCArmy->Times = 0;
				break;
			}
		}
	}
	--pCreateNPCArmy->Times;
	if (pCreateNPCArmy->Times >0 && pCreateNPCArmy->Interval >0)
	{
		Timer_CreateNPCArmy *pNextCreateNPCArmy = new Timer_CreateNPCArmy;
		pNextCreateNPCArmy->serialize(pCreateNPCArmy->CountryID,pCreateNPCArmy->Level,pCreateNPCArmy->Count,pCreateNPCArmy->FromMap,pCreateNPCArmy->TargetMap,pCreateNPCArmy->Times,pCreateNPCArmy->Interval);
		pNextCreateNPCArmy->TimerID = set_timer(pCreateNPCArmy->Interval,(char*)pNextCreateNPCArmy);
	}
}
void COtherManager::ClearNPCArmyOfBigMap(DB_Proxy *pDBProxy)
{
	ArmyTable armytb;
	char *buf = 0;
	int count = 0;
	armytb.setmasterid(SERVER_BOUND_MAP_OTHER);
	if (pDBProxy->db_multi_select(armytb,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		return;
	}
	ArmyTable *pArmyList = (ArmyTable*)buf;
	count = count/sizeof(ArmyTable);
	for (int i=0;i<count;i++)
	{
		pDBProxy->db_delete(pArmyList[i]);
	}
	if (buf)
	{
		delete[] buf;
		buf = 0;
	}
}
//郡状态改变
void COtherManager::OnNotifyMapState(const char *msg)
{

}
//精练装备
void COtherManager::onInlayEquipment(const char *msg)
{
	LPInlayEquip_Request req_msg = (LPInlayEquip_Request)msg;
	InlayEquip_Response rep_msg;
	PlayerItemStorageTable equipItem,stoneItem;
	PlayerItemStorageTable cardItems[2];
	int successedRate = 0;
	int ret = 0;
	bool bSafeInlay = false;
	LPCardParam pCardConfig = 0;
	int i=0;
	if (req_msg->SortID_Equip <=0 ||req_msg->SortID_InlayResource<=0)
	{
		return;
	}

	equipItem.setmasterid(1,req_msg->from);
	equipItem.setdetailid(req_msg->SortID_Equip);
	if (g_Proxy.db_select(equipItem) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	if (equipItem.count_ <=0)
	{
		return;
	}
	stoneItem.setmasterid(1,req_msg->from);
	stoneItem.setdetailid(req_msg->SortID_InlayResource);
	if (g_Proxy.db_select(stoneItem)!=DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	for (i=0;i<2;i++)
	{
		if(req_msg->SortID_Cards[i] <=0)
			continue;

		cardItems[i].setmasterid(1,req_msg->from);
		cardItems[i].setdetailid(req_msg->SortID_Cards[i]);
		if (g_Proxy.db_select(cardItems[i])!=DB_Proxy::DB_SUCCEED)
		{
			rep_msg.serialize(req_msg->agent,req_msg->from,111);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			return;
		}
		if (cardItems[i].pos_ != LUCKCARD)
		{
			continue;
		}
		pCardConfig = g_LoadDBinfo.GetCardConfig(cardItems[i].itemid_);
		if (pCardConfig)
		{
			if (pCardConfig->Type == EquipLuck_Card)
			{
				successedRate += pCardConfig->Value;
			}
			else if(pCardConfig->Type == EquipSafe_Card)
			{
				bSafeInlay = true;
			}
			else
			{
				cardItems[i].itemid_ = 0;
			}
		}
	}
	IEquipment iEquipment(&g_LoadDBinfo,equipItem.itemid_);
	ret = iEquipment.onInlay(stoneItem.itemid_,successedRate);
	if (ret <0)
	{//不能精练
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	/*精炼失败或成功ret =0:失败;>0成功,无论成功与否都需要把现有的宝石和卡删除；
	如果有保底卡，则保留原装备，否则装备同时删除
	*/
	bool bDelStone = false;
	__int64 oldID = equipItem.itemid_;
	__int64 newID = 0;
	if (ret >0)
	{
		newID = iEquipment.getInlayItemID();
		equipItem.itemid_ = iEquipment.getInlayItemID();
		equipItem.tech_ = iEquipment.getInalySkill();
		equipItem.techlevel_ = iEquipment.getInalySkillLevel();
		if (g_Proxy.db_update(equipItem,equipItem)!=DB_Proxy::DB_SUCCEED)
		{
			LOG_ERROR_EX(false,("玩家%d精练装备(新ItemID:%I64d)",req_msg->from,iEquipment.getInlayItemID()));
		}
		rep_msg.serialize(req_msg->agent,req_msg->from,0);
		bDelStone = true;
	}
	else
	{
		if (!bSafeInlay)
		{
			if (g_Proxy.db_delete(equipItem) != DB_Proxy::DB_SUCCEED)
			{
			}
			bDelStone = true;
		}
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
	}
	//20100408
	PlayerOfficerTable officer;
	officer.setmasterid(req_msg->from);
	officer.setdetailid(req_msg->from);
	g_Proxy.db_select(officer);
	ISystemLog::getInstance()->playerEquip_Inlay_Log(officer.playerid_,officer.officername_,
		equipItem.pos_,oldID,newID,stoneItem.itemid_,cardItems[0].itemid_,cardItems[1].itemid_);

	PlayerItemStorageTable updateItem;
	if (bDelStone)
	{
		if (stoneItem.count_ <=1)
		{
			if (g_Proxy.db_delete(stoneItem)!= DB_Proxy::DB_SUCCEED)
			{
			}
		}
		else
		{
			updateItem.setmasterid(stoneItem.officerid_,stoneItem.playerid_);
			updateItem.setdetailid(stoneItem.sortid_);
			updateItem.count_ = -1;
			if (g_Proxy.db_increase(updateItem) != DB_Proxy::DB_SUCCEED)
			{
			}
		}
		//20100408
	}
	
	for (i=0;i<2;i++)
	{
		if(cardItems[i].itemid_ <=0)
			continue;
		if (cardItems[i].count_ <=1)
		{
			g_Proxy.db_delete(cardItems[i]);
		}
		else
		{
			memset(&updateItem,0,sizeof(updateItem));
			updateItem.setmasterid(cardItems[i].officerid_,cardItems[i].playerid_);
			updateItem.setdetailid(cardItems[i].sortid_);
			updateItem.count_ = -1;
			if (g_Proxy.db_increase(updateItem) != DB_Proxy::DB_SUCCEED)
			{
			}
		}
	}
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	if(rep_msg.Error_Code == 0)
	{//EN_S_BROADINLAYEQUIP,					//广播精练消息4397
		//发送广播
		g_DataServer.broadMsgAndNotify(req_msg->from,SERVER_BOUND_MAP_OTHER,iEquipment.getInlayItemID(),iEquipment.getLevel(),iEquipment.getPostion(),0,EN_S_BROADINLAYEQUIP,SERVER_BOUND_MAP_OTHER);
	}
	
}

void COtherManager::studySkillNotify(int mapid,int armyid,int officerid,int skill,int skilllevel,int cmd)
{
	HeroStudySkillEvent skillEvent;
	skillEvent.serialize(armyid, officerid, skill,skilllevel,cmd);
	if (mapid)
	{
		event_notify(mapid, (char * )&skillEvent, skillEvent.length);// 发往郡服务器
	}
}
//收费系统通知学习技能（使用技能卡）
void COtherManager::onNotifyAddOfficerSkill(const char *msg)
{
	LPNotify_AddOfficerSkill_Request req_msg = (LPNotify_AddOfficerSkill_Request)msg;
	Notify_AddOfficerSkill_Response rep_msg;
	rep_msg.serialize(req_msg->agent,req_msg->PlayerID,req_msg->PackID,req_msg->OfficerID,req_msg->SortID,111);

	//判断英雄是否已学过该技能
	PlayerOfficerTable officer;

	officer.setmasterid(req_msg->PlayerID);
	officer.setdetailid(req_msg->OfficerID);
	if (g_Proxy.db_select(officer) != DB_Proxy::DB_SUCCEED)
	{
		event_notify(req_msg->from,(char*)&rep_msg,rep_msg.length);
		return;
	}
	if (isInWarMapOfOfficer(&officer))
	{//在战斗中，不能学习
		rep_msg.Error_Code = EN_OFFICER_INWAR;
		event_notify(req_msg->from,(char*)&rep_msg,rep_msg.length);
		return;
	}

	int iCurTechLevel = 0;
	PlayerSkillTable officerSkill;
	officerSkill.setmasterid(req_msg->PlayerID,req_msg->OfficerID);
	officerSkill.setdetailid(req_msg->SkillID);
	int ret = g_Proxy.db_select(officerSkill) ;
	if (ret != DB_Proxy::DB_EOF)
	{
		event_notify(req_msg->from,(char*)&rep_msg,rep_msg.length);
		return;
	}

	iCurTechLevel++;

	SkillDetail * pHeroTech  = 0;
	pHeroTech = g_LoadDBinfo.getSkillDetail((SKILL_TYPE)req_msg->SkillID,iCurTechLevel);//获取下级技能的配置参数
	if(pHeroTech == NULL)
	{
		event_notify(req_msg->from,(char*)&rep_msg,rep_msg.length);
		return;
	}
	if (!pHeroTech->isSatisfOccupationLimit((OFFICER_OCCUPATION_TYPE)officer.jobid_))
	{//
		rep_msg.Error_Code = EN_JOBNOMATCHING;
		event_notify(req_msg->from,(char*)&rep_msg,rep_msg.length);
		return;
	}
	//判断技能点是否足够
	if (officer.techdot_ < (int)pHeroTech->consumeSkillDot)
	{
		rep_msg.Error_Code = EN_TECH_DOT_NOT;
		event_notify(req_msg->from,(char*)&rep_msg,rep_msg.length);
		return;
	}
	//添加英雄技能表
	if (ret == DB_Proxy::DB_SUCCEED)
	{//已学
		officerSkill.skilllevel_ = iCurTechLevel;
		if (g_Proxy.db_update(officerSkill,officerSkill) != DB_Proxy::DB_SUCCEED)
		{
			event_notify(req_msg->from,(char*)&rep_msg,rep_msg.length);
			return;
		}
	}
	else
	{//新学技能
		officerSkill.skilltype_ = pHeroTech->skillType;
		officerSkill.skilllevel_ = iCurTechLevel;
		if (g_Proxy.db_insert(officerSkill) != DB_Proxy::DB_SUCCEED)
		{
			event_notify(req_msg->from,(char*)&rep_msg,rep_msg.length);
			return;
		}
	}
	char * tmp = new char[sizeof(OFFICER_TECH_LIST) + sizeof(tagOfficerTechList)];
	OFFICER_TECH_LIST * pTechList = (OFFICER_TECH_LIST *)tmp;
	(*pTechList)[0].iTechID = req_msg->SkillID;
	(*pTechList)[0].Level = iCurTechLevel;
	pTechList->serialize(req_msg->OfficerID,1,SERVER_BOUND_MAP_OTHER,req_msg->PlayerID,EN_S_STUDY_A_TECH);	 
	ret = asynch_write(req_msg->agent, (char *)pTechList, pTechList->length);
	if(officer.mapid_ >0)
	{
		studySkillNotify(officer.mapid_,officer.armyid_, officer.officerid_,req_msg->SkillID,iCurTechLevel,CMD_HERO_STUDY_SKILL);
	}
	if(tmp)
	{
		delete[] tmp;
		tmp = 0;
	}
	rep_msg.Error_Code = 0;
	event_notify(req_msg->from,(char*)&rep_msg,rep_msg.length);
	//扣除技能点
	memset(&officer,0,sizeof(officer));
	officer.setmasterid(req_msg->PlayerID);
	officer.setdetailid(req_msg->OfficerID);
	officer.techdot_ = - (int)pHeroTech->consumeSkillDot;
	if (g_Proxy.db_increase(officer) != DB_Proxy::DB_SUCCEED)
	{
	}
	//EN_S_BROADHEROSKILL,					//广播学习技能4396
	//发送广播
	g_DataServer.broadMsgAndNotify(req_msg->PlayerID,SERVER_BOUND_MAP_OTHER,officerSkill.skillid_,officerSkill.skilllevel_,officerSkill.skilltype_,0,EN_S_BROADHEROSKILL,SERVER_BOUND_MAP_OTHER);
	//20100408增加日志
	if (ret == DB_Proxy::DB_SUCCEED)
	{
		ISystemLog::getInstance()->heroSkillDot_UpSkill_Log(officer.playerid_,officer.officerid_,officer.officername_,
			officer.techdot_ + pHeroTech->consumeSkillDot,officer.techdot_,pHeroTech->consumeSkillDot,
			officerSkill.skillid_,officerSkill.skilllevel_);
		ISystemLog::getInstance()->heroSkill_UpSkill_Log(officer.playerid_,officer.officerid_,officer.officername_,
			officerSkill.skillid_,officerSkill.skilllevel_-1,officerSkill.skilllevel_);
	}
	else
	{
		ISystemLog::getInstance()->heroSkillDot_StudySkill_Log(officer.playerid_,officer.officerid_,officer.officername_,
			officer.techdot_ + pHeroTech->consumeSkillDot,officer.techdot_,pHeroTech->consumeSkillDot,
			officerSkill.skillid_,officerSkill.skilllevel_);
		ISystemLog::getInstance()->heroSkill_StudySkill_Log(officer.playerid_,officer.officerid_,officer.officername_,
			officerSkill.skillid_,0,officerSkill.skilllevel_);
	}
}

void COtherManager::clearBinConfig()
{
	map<int,CTreasurePack*>::iterator iter;
	CTreasurePack *pDelTreasurePack = 0;
	for(iter = m_TreasureList.begin();iter != m_TreasureList.end();iter++)
	{
		if((pDelTreasurePack = iter->second))
		{
			delete pDelTreasurePack;
			pDelTreasurePack = 0;
		}
	}
	m_TreasureList.clear();	
}
CTreasurePack* COtherManager::getBinConfig(int id)
{
	map<int,CTreasurePack*>::iterator iter;
	iter = m_TreasureList.find(id);
	if (iter == m_TreasureList.end())
	{
		return NULL;
	}
	return iter->second;
}
//判断英雄是否在战斗郡
bool COtherManager::isInWarMapOfOfficer(PlayerOfficerTable *officer)
{
	if ( officer->state_ != EN_HAVE_ARMY)
	{//没有带部队
		return false;
	}
	ArmyTable army;
	army.setmasterid(officer->mapid_);
	army.setdetailid(officer->armyid_);
	if (g_Proxy.db_select(army) != DB_Proxy::DB_SUCCEED)
	{
		return false;
	}
	if (army.state_ == GO_OUT_FOR_BATTLE)
	{
		return false;
	}
	if (army.state_ !=OUT_CITY_TRUST_CONTROY && army.state_ != IN_CITY)
	{
		return true;
	}
	int tmpMapID = officer->mapid_ - 1001;
	
	return (m_MapState[tmpMapID].state_ == war);
}
//买卡通知，放入公共背包
void COtherManager::onNotifyBuyCard(const char *msg)
{
	LPNotify_BuyCard_Request req_msg = (LPNotify_BuyCard_Request)msg;
	if (req_msg->from >MAX_SERVER_NUM)
	{
		return;
	}
	bool bOverlap = false;
	PackStoreParam *pPackStoreParam = g_LoadDBinfo.GetPackStoreParam(req_msg->postion);
	if (pPackStoreParam)
	{
		bOverlap = (pPackStoreParam->IsOverlap >0)?true:false;
	}
	short sRet = 0;
	PlayerItemStorageTable iteminfo;
	if(OnInsertHeroEquip(&g_Proxy, req_msg->playerID, 1, req_msg->cardID, 0, req_msg->postion,bOverlap,0,0,iteminfo) <=0)
	{
		sRet = EN_PACKFULL;
	}
	else
	{//20100408
		PlayerOfficerTable officer;
		officer.setmasterid(req_msg->playerID);
		officer.setdetailid(req_msg->playerID);
		g_Proxy.db_select(officer);
		ISystemLog::getInstance()->playerEquip_BuyCharge_Log(officer.playerid_,officer.officername_,
			req_msg->postion,req_msg->cardID,1);
	}
	//返回收费系统
	Notify_BuyCard_Response repBuyCard;
	repBuyCard.serialize(req_msg->agent,req_msg->playerID,req_msg->cardID,sRet);
	if(event_notify(repBuyCard.to,(char*)&repBuyCard,repBuyCard.length) <=0)
	{
		LOG_ERROR_EX(false,("玩家%d买卡加入背包后（%d)，通知收费系统失败",req_msg->playerID,sRet));
	}
	//可以增加一个装备的消息返回
}
void COtherManager::onConvert(const char *msg)
{
	LPConvert_Request req_msg = (LPConvert_Request)msg;
	Convert_Response rep_msg;
	PlayerItemStorageTable iteminfo;
	iteminfo.setmasterid(req_msg->officerID,req_msg->from);
	iteminfo.setdetailid(req_msg->sortID);
	if (g_Proxy.db_select(iteminfo)!=DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	if(deleteItemOfPack(&g_Proxy,iteminfo) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	IBaseAwards *baseAwards = 0;
	if (iteminfo.pos_ == ARMYBRAND)
	{//军牌
		ArmyBrand *armyBrand = g_LoadDBinfo.getArmyBrandParamByID(iteminfo.itemid_);
		if (armyBrand == NULL)
		{
			rep_msg.serialize(req_msg->agent,req_msg->from,111);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			return;
		}
		baseAwards = (IBaseAwards*)armyBrand->getIAwards();
		baseAwards->awardsPlayer(&g_Proxy,&g_LoadDBinfo,req_msg->agent,req_msg->from);
	}
	rep_msg.serialize(req_msg->agent,req_msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
}
int COtherManager::getExpeditionSpeedCard(int playerid,PlayerItemStorageTable *item)
{
	PlayerItemStorageTable itemtb;
	char *buf = 0;
	int count = 0;
	int ret = 2;
	itemtb.setmasterid(1,playerid);
	if (g_Proxy.db_multi_select(itemtb,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		return 1;
	}
	count = count /sizeof(PlayerItemStorageTable);
	PlayerItemStorageTable *pItemList = (PlayerItemStorageTable*)buf;
	LPCardParam pCardParam = 0;
	for (int i=0;i<count;i++)
	{
		if (pItemList[i].pos_ != HEROSAFECARD)
		{
			continue;
		}
		pCardParam = g_LoadDBinfo.GetCardConfig(pItemList[i].itemid_);
		if (pCardParam == NULL)
		{
			continue;
		}
		if (pCardParam->Type == ExpeditionSpeed_Card)
		{
			memcpy(item,&pItemList[i],sizeof(PlayerItemStorageTable));
			ret = 0;
			break;
		}
	}
	if (buf)
	{
		delete[] buf;
		buf = 0;
	}
	return ret;
}
//初始化玩家建筑
void COtherManager::initPlayerConstruction(int playerid,int mapid,int cityid,int &warehouseLevel)
{
	int iSortID = 1;
	InitConstruction_Map_Iterator iter;
	ConstructionConfigure *pConstructionCfg = 0;
	InitConstruction *pInitConstruction = NULL;
	PlayerConstruction playerConstructon;
	playerConstructon.setmasterid(mapid,cityid,playerid);
	warehouseLevel = 1;
	for (iter = m_InitConstructionMap.begin();iter != m_InitConstructionMap.end();iter++)
	{
		pInitConstruction = iter->second;
		if (pInitConstruction == NULL)
		{
			continue;
		}
		pConstructionCfg = g_LoadDBinfo.getConstructionConfigure(pInitConstruction->constructionID,pInitConstruction->level);
		if (pConstructionCfg == NULL)
		{
			continue;
		}
		if (pInitConstruction->constructionID ==5)
		{
			warehouseLevel = pInitConstruction->level;
		}
		playerConstructon.setdetailid(iSortID);
		playerConstructon.constructionid_ = pInitConstruction->constructionID;
		playerConstructon.level_ = pInitConstruction->level;
		playerConstructon.completetimes_ = IceUtil::Time::now().toSeconds();
		playerConstructon.output_ = pConstructionCfg->modify_;
		if (g_Proxy.db_insert(playerConstructon) != DB_Proxy::DB_SUCCEED)
		{
			continue;
		}
		iSortID++;
	}
}
//加载初始玩家建筑表
void COtherManager::loadInitConstructions()
{
	FILE *fp = 0;
	fp = fopen("configtable/PlayerInitConstruction.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/PlayerInitConstruction.csv失败。\n");
		return ;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	InitConstruction* pItem = 0;
	//m_InitConstructionMap
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new InitConstruction;
		if(sscanf(buf,"%d,%d",&pItem->constructionID,&pItem->level)<2)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		m_InitConstructionMap[pItem->constructionID] = pItem;
	}
	fclose(fp);
}
void COtherManager::clearInitConstructions()
{
	InitConstruction_Map_Iterator iter;
	InitConstruction *pDelItem = 0;
	for (iter = m_InitConstructionMap.begin();iter != m_InitConstructionMap.end();iter++)
	{
		if ((pDelItem = iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_InitConstructionMap.clear();
}
void COtherManager::onCommonOpenCard(const char *msg)
{
	LPCommonOpenCard_Request req_msg = (LPCommonOpenCard_Request)msg;
	CommonOpenCard_Response  rep_msg;
	PlayerItemStorageTable itemInfo;
	short code = 111;
	itemInfo.setmasterid(1,req_msg->from);
	itemInfo.setdetailid(req_msg->sortID);
	if (g_Proxy.db_select(itemInfo) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,EN_DATA_ERROR);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	if (itemInfo.pos_ <= ACC_3)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,EN_DATA_ERROR);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	LPCardParam pCardParam = g_LoadDBinfo.GetCardConfig(itemInfo.itemid_);
	if (pCardParam == NULL)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,EN_DATA_ERROR);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	switch (pCardParam->Type)
	{
	case ClearSkill_Card:
		code = clearHeroStudySkill(req_msg->from,req_msg->heroID);
		break;
	default:
		break;
	}
	rep_msg.serialize(req_msg->agent,req_msg->from,code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	if(code == 0)
	{
		//删除卡
		deleteItemOfPack(&g_Proxy,itemInfo);
		Notify_UseOneCard_Request notifyMsg;
		notifyMsg.serialize(req_msg->agent,req_msg->from,pCardParam->CardID,SERVER_BOUND_MAP_OTHER);
		event_notify(MDM_GP_MAP_CHARGE,(char*)&notifyMsg,notifyMsg.length);
	}
}
int COtherManager::clearHeroStudySkill(int playerid,int heroid)
{
	PlayerOfficerTable officerInfo;
	officerInfo.setmasterid(playerid);
	officerInfo.setdetailid(heroid);
	if (g_Proxy.db_select(officerInfo) != DB_Proxy::DB_SUCCEED)
	{
		return EN_OPERATE_DBG_FAILED;
	}
	if (isInWarMapOfOfficer(&officerInfo))
	{//在战斗中，不能穿脱装备	
		return EN_OFFICER_INWAR;
	}
	PlayerSkillTable skilltb;
	char * buf = 0;
	int count = 0;
	skilltb.setmasterid(playerid,heroid);
	if (g_Proxy.db_multi_select(skilltb,buf,count) == DB_Proxy::DB_FAILED)
	{
		return EN_OPERATE_DBG_FAILED;
	}
	count = count/sizeof(PlayerSkillTable);
	if (count <=0)
	{
		return EN_OFFICER_NOSKILL;
	}
	PlayerSkillTable *pSkillList = (PlayerSkillTable*)buf;
	int clearCount = 0;
	int iSumTotalSkillDot = 0;
	int iSkillDot = 0;
	for (int i=0;i<count;i++)
	{
		if(pSkillList[i].skilltype_ >=7 && pSkillList[i].skilltype_ <=15)
		{//精专技能不清除
			continue; 
		}
		if (g_Proxy.db_delete(pSkillList[i]) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE_EX(false,("玩家%d使用技能重置卡重置英雄%d的技能(%d-%d)失败。",playerid,heroid,pSkillList[i].skillid_,pSkillList[i].skilllevel_));
			continue; 
		}
		//获取该技能所消耗的所有技能点
		iSkillDot = getSumSkillDotOfSkill(pSkillList[i].skillid_,pSkillList[i].skilllevel_);
		ISystemLog::getInstance()->heroSkillDot_UpSkill_Log(officerInfo.playerid_,officerInfo.officerid_,officerInfo.officername_,
			officerInfo.techdot_ + iSumTotalSkillDot,officerInfo.techdot_ + iSumTotalSkillDot + iSkillDot,iSkillDot,pSkillList[i].skillid_,pSkillList[i].skilllevel_);
		ISystemLog::getInstance()->heroSkill_ClearSkill_Log(officerInfo.playerid_,officerInfo.officerid_,officerInfo.officername_,
			pSkillList[i].skillid_,pSkillList[i].skilllevel_,0);
		iSumTotalSkillDot += iSkillDot;
		if (officerInfo.mapid_ >0)
		{
			studySkillNotify(officerInfo.mapid_,officerInfo.armyid_,officerInfo.officerid_,pSkillList[i].skillid_,pSkillList[i].skilllevel_,CMD_HERO_CLEAR_SKILL);
		}
		clearCount++;
	}
	if (buf)
	{
		delete[] buf;
		buf = 0;
	}
	if (iSumTotalSkillDot >0)
	{
		memset(&officerInfo,0,sizeof(officerInfo));
		officerInfo.setmasterid(playerid);
		officerInfo.setdetailid(heroid);
		officerInfo.techdot_ = iSumTotalSkillDot;
		if (g_Proxy.db_increase(officerInfo) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE_EX(false,("玩家%d使用技能重置卡重置英雄%d的技能后，返回技能点(%d)失败。",playerid,heroid,iSumTotalSkillDot));
		}
	}
	return (clearCount>0)?0:EN_OFFICER_NOSKILL;
}
int COtherManager::getSumSkillDotOfSkill(int skillid,int level)
{
	SkillDetail *pSkillDetail = 0;
	int sum = 0;
	while(level>=0)
	{
		pSkillDetail = g_LoadDBinfo.getSkillDetail((SKILL_TYPE)skillid,level);
		if (pSkillDetail == NULL)
		{
			break;
		}
		sum += (int)pSkillDetail->consumeSkillDot;
		level--;
	}
	return sum;
}

void COtherManager::onModifyHeroName(const char *msg)
{
	LPModifHeroName_Request req_msg = (LPModifHeroName_Request)msg;
	ModifHeroName_Response rep_msg;
	if(req_msg->from == req_msg->heroID)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,EN_OFFICER_ID_ERROR);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	if (req_msg->newName[0] == 0)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,EN_DATA_ERROR);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	PlayerOfficerTable heroInfo;
	heroInfo.setmasterid(req_msg->from);
	heroInfo.setdetailid(req_msg->heroID);
	if (g_Proxy.db_select(heroInfo) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,EN_OFFICER_ID_ERROR);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	//判断是否为改名卡
	PlayerItemStorageTable itemInfo;
	itemInfo.setmasterid(1,req_msg->from);
	itemInfo.setdetailid(req_msg->sortID);
	if (g_Proxy.db_select(itemInfo) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,EN_NO_HAVECARD);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	if (itemInfo.pos_ != HEROSAFECARD)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,EN_NO_HAVECARD);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	LPCardParam pCardParam = g_LoadDBinfo.GetCardConfig(itemInfo.itemid_);
	if (pCardParam == NULL)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,EN_NO_HAVECARD);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	if(pCardParam->Type != ModifyHeroName_Card)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,EN_NO_HAVECARD);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	//判断是否重名
	if (isExistOfHeroName(req_msg->newName))
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,EN_HERONAMEEXIST);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	//修改名称
	strcpy(heroInfo.officername_,req_msg->newName);
	if (g_Proxy.db_update(heroInfo,heroInfo) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,EN_OPERATE_DBG_FAILED);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	rep_msg.serialize(req_msg->agent,req_msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	if (heroInfo.mapid_ >0 && heroInfo.state_ ==EN_HAVE_ARMY)
	{
		heroModifyParamEvent modifyOfficerParam;
		modifyOfficerParam.serialize(heroInfo.playerid_,heroInfo.officerid_);
		event_notify(heroInfo.mapid_, (char * )&modifyOfficerParam, modifyOfficerParam.length);// 发往服务器,
	}
	if(deleteItemOfPack(&g_Proxy,itemInfo))
	{
		Notify_UseOneCard_Request notifyMsg;
		notifyMsg.serialize(req_msg->agent,req_msg->from,itemInfo.itemid_,SERVER_BOUND_MAP_OTHER);
		event_notify(notifyMsg.to,(char*)&notifyMsg,notifyMsg.length);
	}
}
bool COtherManager::isExistOfHeroName(const char *name)
{
	bool bExist = false;
	PlayerOfficerTable heroTable;
	char * buf = 0;
	int count = 0;
	if (g_Proxy.db_select_all(heroTable,buf,count) == DB_Proxy::DB_FAILED)
	{
		return false;
	}
	count = count/sizeof(PlayerOfficerTable);
	PlayerOfficerTable *pHeroList = (PlayerOfficerTable*)buf;
	for (int i=0;i<count;++i)
	{
		if (pHeroList[i].playerid_<10000)
		{
			continue;
		}
		if (strcmp(name,pHeroList[i].officername_) == 0)
		{
			bExist = true;
			break;
		}
	}
	if (buf)
	{
		delete[] buf;
		buf = 0;
	}
	return bExist;
}
void COtherManager::notifyPlayerState2OtherSvr(int playerid,int agent)
{
	NotifyPlayerOnline notifyMsg;
	//首先市场
	notifyMsg.serialize(playerid,agent,MDM_GP_MAP_MARKET);
	event_notify(notifyMsg.to,(char*)&notifyMsg,notifyMsg.length);
	//科研
	notifyMsg.to = MDM_GP_MAP_RESEARCH;
	event_notify(notifyMsg.to,(char*)&notifyMsg,notifyMsg.length);
	//郡地图
	PlayerCity playerCity;
	char * buf = 0;
	int count = 0;
	playerCity.setmasterid(playerid);
	if (g_Proxy.db_multi_select(playerCity,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		return;
	}
	count = count/sizeof(PlayerCity);
	PlayerCity *pCityList = (PlayerCity*)buf;
	for (int i =0 ;i<count;++i)
	{
		notifyMsg.to = pCityList[i].mapid_;
		event_notify(notifyMsg.to,(char*)&notifyMsg,notifyMsg.length);
	}
	if (buf)
	{
		delete[] buf;
		buf = 0;
	}
}
//释放俘虏
void COtherManager::releaseCaptive(const char *msg)
{
	LPReleaseCaptive_Request req_msg = (LPReleaseCaptive_Request)msg;
	ReleaseCaptive_Response rep_msg;
	PlayerCaptiveHero captivetb;
	char *buf = 0;
	int count = 0;
	captivetb.setmasterid(req_msg->from);
	int ret = g_Proxy.db_multi_select(captivetb,buf,count);
	if (ret == DB_Proxy::DB_FAILED)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	else if(ret == DB_Proxy::DB_EOF)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	count = count /sizeof(PlayerCaptiveHero);
	PlayerCaptiveHero *pCaptiveList = (PlayerCaptiveHero*)buf;
	int i =0;
	for (int i=0;i<count;++i)
	{
		if ((pCaptiveList[i].byplayerid_ == req_msg->captPlayerID) && (pCaptiveList[i].byofficerid_ == req_msg->captOfficerID))
		{
			captivetb.setmasterid(pCaptiveList[i].playerid_);
			captivetb.setdetailid(pCaptiveList[i].officerid_,pCaptiveList[i].byplayerid_,pCaptiveList[i].byofficerid_);
			break;
		}
	}
	if (buf)
	{
		delete[] buf;
		buf = 0;
	}
	if (i >= count)
	{//没有改俘虏
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	PlayerOfficerTable heroInfo;
	heroInfo.setmasterid(req_msg->captPlayerID);
	heroInfo.setdetailid(req_msg->captOfficerID);
	if (g_Proxy.db_select(heroInfo) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	if (heroInfo.state_ == EN_BY_CAPTIVE)
	{
		heroInfo.state_ = EN_LEISURE;
		if (g_Proxy.db_update(heroInfo,heroInfo) != DB_Proxy::DB_SUCCEED)
		{
			rep_msg.serialize(req_msg->agent,req_msg->from,111);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			return;
		}
		//20100409
		ISystemLog::getInstance()->heroState_Release_Log(heroInfo.playerid_,heroInfo.officerid_,heroInfo.officername_,req_msg->from);
	}
	g_Proxy.db_delete(captivetb);//删除俘虏表记录
	rep_msg.serialize(req_msg->agent,req_msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
}
