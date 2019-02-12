#include "CommInterfaceHeader.h"
#pragma warning(push)
#pragma warning(disable:4127)
int g_LevelExp[MAX_LEVEL+1];
 

int HeroCredit(const int lCredit)
{
	if (lCredit <= 0) return 0;
	int iRank = 0;

	if(lCredit <10)
		iRank = 0;
	else if(lCredit <77)
		iRank = 1;
	else if(lCredit <207)
		iRank = 2;
	else if(lCredit <410)
		iRank = 3;
	else if(lCredit <705)
		iRank = 4;
	else if(lCredit <1126)
		iRank = 5;
	else if(lCredit <1733)
		iRank = 6;
	else if(lCredit <2635)
		iRank = 7;
	else if(lCredit <4028)
		iRank = 8;
	else if(lCredit <6264)
		iRank = 9;
	else if(lCredit <9978)
		iRank = 10;
	else if(lCredit <14532)
		iRank = 11;
	else if(lCredit <20957)
		iRank = 12;
	else if(lCredit <30733)
		iRank = 13;
	else
		iRank = 14;
	return iRank;
}
//add by xujincai 20090211 根据最新等级计算公式
int GetLevel(int curlevel,int in_exp,int &out_exp)
{
	if(in_exp <0)
		return curlevel;
	//读取等级参数
	static bool bReadParam = false;
	if(!bReadParam)
	{
		FILE *fp = 0;
		fp = fopen("configtable/LevelParam.csv","r");
		if(fp == NULL)
		{
			printf("打开configtable/LevelParam.csv失败。\n");
			return curlevel;
		}

		memset(g_LevelExp,0,sizeof(int) * (MAX_LEVEL+1));
		int iLevel = 0;
		int iExp = 0;
		char buf[32]= {0};
		fgets(buf,32,fp);

		while(!feof(fp))
		{
			if(fgets(buf,32,fp) == NULL)
				break;
			sscanf(buf,"%d,%d",&iLevel,&iExp);
			if(iLevel >MAX_LEVEL || iLevel<0)
				continue;
			g_LevelExp[iLevel] = iExp;
		}
		fclose(fp);
		bReadParam = true;
	}
	int iNeedExp = 0;
	int iNextLevel = 0;
	while(1)
	{
		iNextLevel = curlevel+1;
		if(iNextLevel >MAX_LEVEL)
			break;
		iNeedExp = g_LevelExp[iNextLevel];

		if(in_exp < iNeedExp || iNeedExp<=0)
			break;
		in_exp -= iNeedExp;
		curlevel = iNextLevel;
	}
	out_exp = in_exp;
	return curlevel;
}
//更新玩家资源 输出:0:失败;-1:成功;其它值表示对应的资源不够用.
int OnUpDataPlayerRes(DB_Proxy &Proxy, unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID, tagPlayerResource &PlayerRes)
{
	if (PlayerRes.iFoodCount != 0 || PlayerRes.iWoodCount != 0 || PlayerRes.iIronCount != 0 || PlayerRes.iScience != 0 ||
		PlayerRes.iRedStoneCount!=0 || PlayerRes.iYellowStoneCount!=0 || PlayerRes.iGreenStoneCount!=0 || PlayerRes.iBackIronCount!=0)
	{
		PlayerResource ResByPlayer, Res;
		memset(&ResByPlayer,0,sizeof(PlayerResource));
		memset(&Res,0,sizeof(PlayerResource));

		if (lCityID <= 0 || lMapID <= 0)
		{//资源换成金钱5:1
			PlayerRes.iMoney += (PlayerRes.iFoodCount+PlayerRes.iWoodCount+PlayerRes.iIronCount+PlayerRes.iSkinCount)/5;
		}
		else
		{
			ResByPlayer.setmasterid(lCityID, lMapID);
			ResByPlayer.setdetailid(lPlayerID);
			if (Proxy.db_select(ResByPlayer) != DB_Proxy::DB_SUCCEED)
			{
				return 0;
			}

			ResByPlayer.foodcount_ += PlayerRes.iFoodCount;
			ResByPlayer.content_ -= PlayerRes.iFoodCount;
			Res.foodcount_ = PlayerRes.iFoodCount;

			ResByPlayer.woodcount_ += PlayerRes.iWoodCount;
			ResByPlayer.content_ -= PlayerRes.iWoodCount;
			Res.woodcount_ = PlayerRes.iWoodCount;
		
			ResByPlayer.ironcount_ += PlayerRes.iIronCount;
			ResByPlayer.content_ -= PlayerRes.iIronCount;
			Res.ironcount_ = PlayerRes.iIronCount;
		
			ResByPlayer.skincount_ += PlayerRes.iSkinCount;
			ResByPlayer.content_ -= PlayerRes.iSkinCount;
			Res.skincount_ = PlayerRes.iSkinCount;
			
			ResByPlayer.redstonecount_ += PlayerRes.iRedStoneCount;
			ResByPlayer.content_ -= PlayerRes.iRedStoneCount;
			Res.redstonecount_ = PlayerRes.iRedStoneCount;

			ResByPlayer.yollowstonecount_ += PlayerRes.iYellowStoneCount;
			ResByPlayer.content_ -= PlayerRes.iYellowStoneCount;
			Res.yollowstonecount_ = PlayerRes.iYellowStoneCount;

			ResByPlayer.greenstonecount_ += PlayerRes.iGreenStoneCount;
			ResByPlayer.content_ -= PlayerRes.iGreenStoneCount;
			Res.greenstonecount_ = PlayerRes.iGreenStoneCount;

			ResByPlayer.backironcount_ += PlayerRes.iBackIronCount;
			ResByPlayer.content_ -= PlayerRes.iBackIronCount;
			Res.backironcount_ = PlayerRes.iBackIronCount;

			if (ResByPlayer.foodcount_ < 0) return EN_FOOD;
			if (ResByPlayer.woodcount_  < 0) return EN_WOOD;
			if (ResByPlayer.ironcount_ < 0) return EN_IRON;
			if (ResByPlayer.skincount_ < 0) return EN_SKIN;

			if (ResByPlayer.redstonecount_ < 0) return EN_RED_STONE;
			if (ResByPlayer.yollowstonecount_ < 0) return EN_YELLOW_STONE;
			if (ResByPlayer.greenstonecount_ < 0) return EN_GREEN_STONE;
			if (ResByPlayer.backironcount_  < 0) return EN_BLACK_STONE;

			Res.content_ = ResByPlayer.content_;
			Res.setmasterid(lCityID, lMapID);
			Res.setdetailid(lPlayerID);
			if (Proxy.db_increase(Res) != DB_Proxy::DB_SUCCEED)
			{
				return 0;
			}
		}
	}
	PlayerChatRoomTable PlayerMoney;
	memset(&PlayerMoney, 0, sizeof(PlayerMoney));

	PlayerMoney.money_ = PlayerRes.iMoney;
	PlayerMoney.science_ = PlayerRes.iScience;
	PlayerMoney.contribute_ = PlayerRes.iContribute;

	if (PlayerRes.iScience != 0 || PlayerRes.iMoney != 0 || PlayerRes.iContribute != 0)
	{
		PlayerMoney.setid(lPlayerID);
		if (Proxy.db_increase(PlayerMoney) != DB_Proxy::DB_SUCCEED)
		{
			return 0;
		}
	}
	return -1;
}
//拾到装备
//输入:玩家ID lPlayerID;英雄ID lHeroID;装备ID iItemID;装备等级 iLevel;装备类型(见EQUIP_TYPE枚举) iType
//输出:0:公共背包已满;1:成功
int OnInsertHeroEquip(DB_Proxy &Proxy, unsigned long lPlayerID, unsigned long lHeroID, __int64 iItemID, int iLevel, int iType,
					  bool bOverlap,int skill,int skilllevel,PlayerItemStorageTable &Pack)
{
	memset(&Pack,0,sizeof(PlayerItemStorageTable));
	Pack.playerid_ = lPlayerID;
	Pack.officerid_ = lHeroID;
	if (bOverlap)
	{
		Pack.itemid_ = iItemID;
	}
	GetUnusedPos(Pack,&Proxy);
	if(Pack.showpos_ <0)
		return 0;

	if (Pack.sortid_ >0)
	{//更新数量
		int sortID = Pack.sortid_;
		memset(&Pack,0,sizeof(Pack));
		Pack.setmasterid(lHeroID,lPlayerID);
		Pack.setdetailid(sortID);
		Pack.count_ = 1;//增加1
		if(Proxy.db_increase(Pack) != DB_Proxy::DB_SUCCEED)
		{
			return 0;
		}
	}
	else
	{//新增
		if(lHeroID ==1)
		{
			Pack.type_ = EN_COMM_PACK;
		}
		else
		{
			Pack.type_ = EN_HERO_PACK;
		}
		Pack.itemid_ = iItemID;
		Pack.level_ = iLevel;
		Pack.pos_ = iType;
		Pack.count_ = 1;//add by xujincai 20081215 默认数量为1
		Pack.tech_ = skill;
		Pack.techlevel_ = skilllevel;
		if(Proxy.db_insert (Pack) != DB_Proxy::DB_SUCCEED)
		{
			return 0;
		}
	}
	return Pack.sortid_;
}

//英雄成长

void heroUpgrade(int level,PlayerOfficerTable *officer,PlayerOfficerTable *upofficer)
{//成长基本值为10，一般属性+1/每级
	float fTmpAttri = (level * 10 * officer->grouprate_ + officer->lsttotalattri_) * 0.01f;//剩余主属性+耐力之和

	int iTotalAttri = (int)fTmpAttri;//本次升级，可分配的点数
	fTmpAttri = fTmpAttri - iTotalAttri + 0.000001f;
	iTotalAttri -= 2 * level;
	if (iTotalAttri <=0)
	{
		return;
	}
	float fStamina = 0.0;
	float fMainAttri = 0.0;
	int iStamina = 0;
	int iMainAttri = 0;
	fStamina = iTotalAttri * 0.4f;//耐力
	fMainAttri = (float)iTotalAttri - fStamina;

	fStamina += officer->lststamina_ * 0.01f + 0.000001f ;
	fMainAttri += officer->lstmainattri_ * 0.01f + 0.000001f;
	iStamina = (int)fStamina;
	iMainAttri = (int)fMainAttri;
	fStamina = fStamina - iStamina + 0.000001f;
	fMainAttri = fMainAttri - iMainAttri + 0.000001f;
	if(officer->jobid_ == Saber_OFFICER || officer->jobid_ == WARRIOR_OFFICER)
	{//战士系
		upofficer->levelcelerity_ += level;
		upofficer->levelwit_ += level;
		upofficer->levelpower_ += iMainAttri;
	}
	else if(officer->jobid_ == SHOOTER_OFFICER)
	{//弓箭手系
		upofficer->levelcelerity_ += iMainAttri;
		upofficer->levelwit_ += level;
		upofficer->levelpower_ += level;
	}
	else if(officer->jobid_ == MAGE_OFFICER)
	{//法师系
		upofficer->levelcelerity_ += level;
		upofficer->levelwit_ += iMainAttri;
		upofficer->levelpower_ += level;
	}
	else if(officer->jobid_ == PRIEST_OFFICER)
	{//牧师系
		upofficer->levelcelerity_ += level;
		upofficer->levelwit_ += iMainAttri;
		upofficer->levelpower_ += level;
	}
	else
	{
	}
	upofficer->levelstamina_ += iStamina;
	fTmpAttri = fTmpAttri * 100;
	fStamina = fStamina * 100;
	fMainAttri = fMainAttri * 100;
	upofficer->lsttotalattri_ += (int)fTmpAttri - officer->lsttotalattri_;
	upofficer->lststamina_ += (int)fStamina - officer->lststamina_;
	upofficer->lstmainattri_ += (int)fMainAttri - officer->lstmainattri_;
}
int OnHeroGrowUp(DB_Proxy &Proxy, tagHeroParam & HeroParam,const int method)
{
	PlayerOfficerTable Officer;
	PlayerOfficerTable UpDataOfficer;
	Officer.setmasterid(HeroParam.lPlayerID);
	Officer.setdetailid(HeroParam.lHeroID);
	if (Proxy.db_select(Officer) != DB_Proxy::DB_SUCCEED)
	{
		return 0;
	}
	if (HeroParam.iExp < 0 ) 
		HeroParam.iExp = 0;

	if (HeroParam.iCredit < 0 )
		HeroParam.iCredit = 0;

	
	int iSExp = HeroParam.iExp;
	int iExp = Officer.exp_ + HeroParam.iExp;
	int iCredit = Officer.credit_ + HeroParam.iCredit;
	//20100412
	if (HeroParam.iExp >0)
	{
		ISystemLog::getInstance()->heroExp_Log(Officer.playerid_,Officer.officerid_,Officer.officername_,
			(MethodType)method,Officer.exp_,iExp,HeroParam.iExp);
	}
	int iLevel = GetLevel(Officer.level_,iExp, iSExp);//HeroExp(iExp, Officer.level_,iSExp);
	int iRank = HeroCredit(iCredit);
	if (HeroParam.iCredit >0 && HeroParam.lPlayerID == HeroParam.lHeroID)
	{
		ISystemLog::getInstance()->playerCredit_Log(Officer.playerid_,Officer.officername_,
			(MethodType)method,Officer.credit_,iCredit,HeroParam.iCredit,
			Officer.militaryrank_,iRank,iRank - Officer.militaryrank_);
	}
	int iFlag1=0,iFlag2=0;

	memset(&UpDataOfficer, 0, sizeof(PlayerOfficerTable));
	if(Officer.level_ < iLevel)
	{
		iFlag1 = 1;
		if (iSExp  < 0) iSExp = 0;
		UpDataOfficer.level_ = iLevel-Officer.level_;
		heroUpgrade(UpDataOfficer.level_,&Officer,&UpDataOfficer);
		//add by xujincai 20090205 根据耐力等增加英雄的攻防属性
		UpDataOfficer.jobid_ = Officer.jobid_;
		ProcHeroAttribute(UpDataOfficer);
		UpDataOfficer.jobid_ = 0;
		//add by xujincai 20081227升级增加技能点
		UpDataOfficer.techdot_ = UpDataOfficer.level_ * 5;
	}
	if (Officer.militaryrank_ < iRank)
	{
		iFlag2 = 1;
		UpDataOfficer.militaryrank_ = iRank - Officer.militaryrank_;//modify by xuincai 20090216
	}
	UpDataOfficer.exp_ = iSExp - Officer.exp_;
	UpDataOfficer.credit_ = HeroParam.iCredit;
	UpDataOfficer.enemycount_ = HeroParam.iEnemyCount;
	UpDataOfficer.generalcount_ = HeroParam.iGeneralCount;
	UpDataOfficer.setmasterid(HeroParam.lPlayerID);
	UpDataOfficer.setdetailid(HeroParam.lHeroID);
	PlayerOfficerTable oldUpdataOfficer = UpDataOfficer;
	if (Proxy.db_increase(UpDataOfficer) != DB_Proxy::DB_SUCCEED)
		return 0;
	if(HeroParam.iGeneralCount>0 || HeroParam.iEnemyCount>0)
	{
		WarNote Record;
		Record.playerid_ = HeroParam.lPlayerID;
		Record.officerid_ = HeroParam.lHeroID;
		Record.enterdate_ = IceUtil::Time::now().toSeconds();//HeroParam.lStartFight;
		Record.mapid_ = HeroParam.lMapID;
		Record.enemycount_ = HeroParam.iEnemyCount;
		Record.generalcount_ = HeroParam.iGeneralCount;
		Record.exitdate_ = IceUtil::Time::now().toSeconds();
		Record.state_ = HeroParam.iState;
		Record.freedate_ = 0;
		strcpy(Record.armyname_, HeroParam.szArmyName);
		Proxy.db_insert(Record);
	}
	if (iFlag1 == 1)
	{
		/*
		PullulateNote Record;
		Record.playerid_ = HeroParam.lPlayerID;
		Record.officerid_ = HeroParam.lHeroID;
		Record.type_ = 1;
		Record.level_ = iLevel;
		Record.upgradedate_ = IceUtil::Time::now().toSeconds();
		Proxy.db_insert(Record);
		MILITARY_RANK Rank;
	
		sprintf(Officer.officername_, "%d - %d ...",iTotExp,HeroParam.iExp);
		Rank.serialize(Officer.officername_, 1, 0,0,EN_S_BROADCAST_MILITARY_RANK);
		broadcast((char *)&Rank, Rank.length);
		*/
		if (UpDataOfficer.mapid_ >0 && UpDataOfficer.state_ ==EN_HAVE_ARMY)
		{
			heroModifyParamEvent modifyOfficerParam;
			modifyOfficerParam.serialize(UpDataOfficer.playerid_,UpDataOfficer.officerid_);
			event_notify(UpDataOfficer.mapid_, (char * )&modifyOfficerParam, modifyOfficerParam.length);// 发往服务器,
		}
		ISystemLog::getInstance()->heroExp_Log(Officer.playerid_,Officer.officerid_,Officer.officername_,
			(MethodType)method,iExp,UpDataOfficer.exp_,iExp - UpDataOfficer.exp_);
		ISystemLog::getInstance()->heroSkillDot_Upgroup_Log(Officer.playerid_,Officer.officerid_,Officer.officername_,
			Officer.techdot_,UpDataOfficer.techdot_,oldUpdataOfficer.techdot_);
		ISystemLog::getInstance()->heroUpgroup_Log(Officer.playerid_,Officer.officerid_,Officer.officername_,
			UPGROUPLEVEL_METHOD,Officer.level_,UpDataOfficer.level_,oldUpdataOfficer.level_,
			Officer.levelstamina_,UpDataOfficer.levelstamina_,oldUpdataOfficer.levelstamina_,
			Officer.levelpower_,UpDataOfficer.levelpower_,oldUpdataOfficer.levelpower_,
			Officer.levelcelerity_,UpDataOfficer.levelcelerity_,oldUpdataOfficer.levelcelerity_,
			Officer.levelwit_,UpDataOfficer.levelwit_,oldUpdataOfficer.levelwit_);
	}
	if (iFlag2 == 1)
	{
		PullulateNote Record;
		Record.playerid_ = HeroParam.lPlayerID;
		Record.officerid_ = HeroParam.lHeroID;
		Record.type_ = 2;
		Record.level_ = iRank;
		Record.upgradedate_ = IceUtil::Time::now().toSeconds();
		Proxy.db_insert(Record);

		MILITARY_RANK Rank;	
		Rank.serialize(Officer.officername_, iRank, 0,0,EN_S_BROADCAST_MILITARY_RANK);
		broadcast((char *)&Rank, Rank.length);
	}

	//add by xujincai 20081226英雄为重伤时，发送通知消息至大地图服务器
	if(HeroParam.iState ==EN_GBH)
	{
		HeroGBH_Notify heroGBHmsg;
		heroGBHmsg.serialize(HeroParam.lPlayerID,HeroParam.lHeroID);
		event_notify(SERVER_BOUND_MAP_OTHER,(char*)&heroGBHmsg,heroGBHmsg.length);

		UpDataOfficer.setmasterid(HeroParam.lPlayerID);
		UpDataOfficer.setdetailid(HeroParam.lPlayerID);
		if (Proxy.db_select(UpDataOfficer) != DB_Proxy::DB_SUCCEED)
		{
			return 0;
		}
		HeroGBH_Broadcast broadmsg;
		broadmsg.serialize(UpDataOfficer.officername_,Officer.officername_);
		broadcast((char *)&broadmsg, broadmsg.length);
	}
	//20100302
	if (HeroParam.iExp >0 || HeroParam.iCredit >0)
	{
		Notify_ExpOrCredit_Request notifyCons;
		notifyCons.serialize(HeroParam.lPlayerID,HeroParam.iExp,HeroParam.iCredit);
		event_notify(notifyCons.to,(char*)&notifyCons,notifyCons.length);
	}
	return 1;
}

//用于拍卖的战利品
//营阵ID:iCountry(联盟;部落),战利品ID:iItemID 战利品类型(见GOODS_TYPE枚举):iType 战利品数量:iCount
//输出:0:失败;1:成功
int OnVendueGoods(DB_Proxy &Proxy, int iCountry, unsigned long lMapID, int iItemID, int iType, int iCount)
{
	Market VendueMarket;
	memset(&VendueMarket, 0, sizeof(VendueMarket));

	VendueMarket.playerid_ = lMapID;
	VendueMarket.itemid_ = iItemID;
	VendueMarket.level_ = 0;
	VendueMarket.type_ = iType;
	VendueMarket.count_ = iCount;
	VendueMarket.minprice_ = 0;
	VendueMarket.currentprice_ = 0;
	VendueMarket.onceprice_ = 0;
	VendueMarket.bidcityid_ = 0;
	VendueMarket.starttime_ = IceUtil::Time::now().toSeconds();
	VendueMarket.country_ = iCountry;
	VendueMarket.mapid_ = lMapID;
	VendueMarket.cityid_ = 0;
	VendueMarket.bidmapid_ = 0;
	VendueMarket.bidcityid_ = 0;
	strcpy(VendueMarket.playername_, "");
	if (Proxy.db_insert(VendueMarket) != DB_Proxy::DB_SUCCEED)
		return 0;
	return 1;
}


//删除玩家城市
//输出:0:失败;1:成功
int OnDeleteCity(DB_Proxy &Proxy, unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID)
{
	char * bbuf = 0;
	int count = 0,Item = 0;
	PlayerCity PCity;
	PCity.setmasterid(lPlayerID);
	PCity.setdetailid(lMapID);
	Proxy.db_delete(PCity);

	PlayerResource ResByPlayer;
	ResByPlayer.setmasterid(lCityID, lMapID);
	ResByPlayer.setdetailid(lPlayerID);
	Proxy.db_delete(ResByPlayer);
		
	ProductionAmount ProItem;
	for (int i = 1; i < 5; i++)
	{
		ProItem.setmasterid(lMapID, lCityID, lPlayerID, i);
		Proxy.db_multi_select(ProItem, bbuf, count);
		Item = count/sizeof(ProductionAmount);
		ProductionAmount *pProItem = (ProductionAmount *)bbuf;
		for (int n = 0; n < Item; n++)
		{
			pProItem[n].setmasterid(pProItem[n].mapid_, pProItem[n].cityid_, lPlayerID, i);
			pProItem[n].setdetailid(pProItem[n].productionid_);
			Proxy.db_delete(pProItem[n]);
		}
		//add by xujincai 20081216
		if(bbuf)
		{
			delete[] bbuf;
			bbuf = NULL;
		}
		count = 0;
	}
	PlayerConstruction Constr;
	Constr.setmasterid(lMapID, lCityID, lPlayerID);
	Proxy.db_multi_select(Constr, bbuf, count);
	Item = count/sizeof(PlayerConstruction);
	PlayerConstruction *pPlayerConstr = (PlayerConstruction *)bbuf;
	for (int n = 0; n < Item; n++)
	{
		pPlayerConstr[n].setmasterid(lMapID, lCityID, lPlayerID);
		pPlayerConstr[n].setdetailid(pPlayerConstr[n].sortid_);
		Proxy.db_delete(pPlayerConstr[n]);
	}
	PlayerSoldierRace RaceCount;
	RaceCount.setmasterid(lMapID, lCityID);
	RaceCount.setdetailid(lPlayerID);
	Proxy.db_delete(RaceCount);
	//add by xujincai 20081216
	if(bbuf)
	{
		delete[] bbuf;
		bbuf = 0;
	}
	return 1;
}
//删除玩家城市
//输出:0:失败;1:成功
int OnDeleteCity(DB_Proxy *pProxy, unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID)
{
	char * bbuf = 0;
	int count = 0,Item = 0;
	PlayerCity PCity;
	PCity.setmasterid(lPlayerID);
	PCity.setdetailid(lMapID);
	pProxy->db_delete(PCity);

	PlayerResource ResByPlayer;
	ResByPlayer.setmasterid(lCityID, lMapID);
	ResByPlayer.setdetailid(lPlayerID);
	pProxy->db_delete(ResByPlayer);
		
	ProductionAmount ProItem;
	for (int i = 1; i < 5; i++)
	{
		ProItem.setmasterid(lMapID, lCityID, lPlayerID, i);
		pProxy->db_multi_select(ProItem, bbuf, count);
		Item = count/sizeof(ProductionAmount);
		ProductionAmount *pProItem = (ProductionAmount *)bbuf;
		for (int n = 0; n < Item; n++)
		{
			pProItem[n].setmasterid(pProItem[n].mapid_, pProItem[n].cityid_, lPlayerID, i);
			pProItem[n].setdetailid(pProItem[n].productionid_);
			pProxy->db_delete(pProItem[n]);
		}
		//add by xujincai 20081216
		if(bbuf)
		{
			delete[] bbuf;
			bbuf = NULL;
		}
		count = 0;
	}
	PlayerConstruction Constr;
	Constr.setmasterid(lMapID, lCityID, lPlayerID);
	pProxy->db_multi_select(Constr, bbuf, count);
	Item = count/sizeof(PlayerConstruction);
	PlayerConstruction *pPlayerConstr = (PlayerConstruction *)bbuf;
	for (int n = 0; n < Item; n++)
	{
		pPlayerConstr[n].setmasterid(lMapID, lCityID, lPlayerID);
		pPlayerConstr[n].setdetailid(pPlayerConstr[n].sortid_);
		pProxy->db_delete(pPlayerConstr[n]);
	}
	PlayerSoldierRace RaceCount;
	RaceCount.setmasterid(lMapID, lCityID);
	RaceCount.setdetailid(lPlayerID);
	pProxy->db_delete(RaceCount);
	//add by xujincai 20081216
	if(bbuf)
	{
		delete[] bbuf;
		bbuf = 0;
	}
	return 1;
}
//更新玩家资源 输出:0:失败;-1:成功;其它值表示对应的资源不够用.
int OnUpDataPlayerRes(DB_Proxy *pProxy, unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID, tagPlayerResource &PlayerRes)
{
	if (PlayerRes.iFoodCount != 0 || PlayerRes.iWoodCount != 0 || PlayerRes.iIronCount != 0 || PlayerRes.iScience != 0 ||
		PlayerRes.iRedStoneCount!=0 || PlayerRes.iYellowStoneCount!=0 || PlayerRes.iGreenStoneCount!=0 || PlayerRes.iBackIronCount!=0)
	{
		PlayerResource ResByPlayer, Res;
		memset(&ResByPlayer,0,sizeof(PlayerResource));
		memset(&Res,0,sizeof(PlayerResource));

		if (lCityID <= 0 || lMapID <= 0)
		{//资源换成金钱5:1
			PlayerRes.iMoney += (PlayerRes.iFoodCount+PlayerRes.iWoodCount+PlayerRes.iIronCount+PlayerRes.iSkinCount)/5;
		}
		else
		{
			ResByPlayer.setmasterid(lCityID, lMapID);
			ResByPlayer.setdetailid(lPlayerID);
			if (pProxy->db_select(ResByPlayer) != DB_Proxy::DB_SUCCEED)
			{
				return 0;
			}

			ResByPlayer.foodcount_ += PlayerRes.iFoodCount;
			ResByPlayer.content_ -= PlayerRes.iFoodCount;
			Res.foodcount_ = PlayerRes.iFoodCount;

			ResByPlayer.woodcount_ += PlayerRes.iWoodCount;
			ResByPlayer.content_ -= PlayerRes.iWoodCount;
			Res.woodcount_ = PlayerRes.iWoodCount;
		
			ResByPlayer.ironcount_ += PlayerRes.iIronCount;
			ResByPlayer.content_ -= PlayerRes.iIronCount;
			Res.ironcount_ = PlayerRes.iIronCount;
		
			ResByPlayer.skincount_ += PlayerRes.iSkinCount;
			ResByPlayer.content_ -= PlayerRes.iSkinCount;
			Res.skincount_ = PlayerRes.iSkinCount;
			
			ResByPlayer.redstonecount_ += PlayerRes.iRedStoneCount;
			ResByPlayer.content_ -= PlayerRes.iRedStoneCount;
			Res.redstonecount_ = PlayerRes.iRedStoneCount;

			ResByPlayer.yollowstonecount_ += PlayerRes.iYellowStoneCount;
			ResByPlayer.content_ -= PlayerRes.iYellowStoneCount;
			Res.yollowstonecount_ = PlayerRes.iYellowStoneCount;

			ResByPlayer.greenstonecount_ += PlayerRes.iGreenStoneCount;
			ResByPlayer.content_ -= PlayerRes.iGreenStoneCount;
			Res.greenstonecount_ = PlayerRes.iGreenStoneCount;

			ResByPlayer.backironcount_ += PlayerRes.iBackIronCount;
			ResByPlayer.content_ -= PlayerRes.iBackIronCount;
			Res.backironcount_ = PlayerRes.iBackIronCount;

			if (ResByPlayer.foodcount_ < 0) return EN_FOOD;
			if (ResByPlayer.woodcount_  < 0) return EN_WOOD;
			if (ResByPlayer.ironcount_ < 0) return EN_IRON;
			if (ResByPlayer.skincount_ < 0) return EN_SKIN;

			if (ResByPlayer.redstonecount_ < 0) return EN_RED_STONE;
			if (ResByPlayer.yollowstonecount_ < 0) return EN_YELLOW_STONE;
			if (ResByPlayer.greenstonecount_ < 0) return EN_GREEN_STONE;
			if (ResByPlayer.backironcount_  < 0) return EN_BLACK_STONE;

			Res.content_ = ResByPlayer.content_;
			Res.setmasterid(lCityID, lMapID);
			Res.setdetailid(lPlayerID);
			if (pProxy->db_increase(Res) != DB_Proxy::DB_SUCCEED)
			{
				return 0;
			}
		}
	}
	PlayerChatRoomTable PlayerMoney;
	memset(&PlayerMoney, 0, sizeof(PlayerMoney));

	PlayerMoney.money_ = PlayerRes.iMoney;
	PlayerMoney.science_ = PlayerRes.iScience;
	PlayerMoney.contribute_ = PlayerRes.iContribute;

	if (PlayerRes.iScience != 0 || PlayerRes.iMoney != 0 || PlayerRes.iContribute != 0)
	{
		PlayerMoney.setid(lPlayerID);
		if (pProxy->db_increase(PlayerMoney) != DB_Proxy::DB_SUCCEED)
		{
			return 0;
		}
	}
	return -1;
}
//拾到装备
//输入:玩家ID lPlayerID;英雄ID lHeroID;装备ID iItemID;装备等级 iLevel;装备类型(见EQUIP_TYPE枚举) iType
//输出:0:公共背包已满;1:成功
int OnInsertHeroEquip(DB_Proxy *pProxy, unsigned long lPlayerID, unsigned long lHeroID, __int64 iItemID, int iLevel, int iType,
					  bool bOverlap,int skill,int skilllevel,PlayerItemStorageTable &Pack)
{
	memset(&Pack,0,sizeof(PlayerItemStorageTable));
	Pack.playerid_ = lPlayerID;
	Pack.officerid_ = lHeroID;
	if (bOverlap)
	{
		Pack.itemid_ = iItemID;
	}
	GetUnusedPos(Pack,pProxy);
	if(Pack.showpos_ <0)
		return 0;

	if (Pack.sortid_ >0)
	{//更新数量
		int sortID = Pack.sortid_;
		memset(&Pack,0,sizeof(Pack));
		Pack.setmasterid(lHeroID,lPlayerID);
		Pack.setdetailid(sortID);
		Pack.count_ = 1;//增加1
		if(pProxy->db_increase(Pack) != DB_Proxy::DB_SUCCEED)
		{
			return 0;
		}
	}
	else
	{//新增
		if(lHeroID ==1)
		{
			Pack.type_ = EN_COMM_PACK;
		}
		else
		{
			Pack.type_ = EN_HERO_PACK;
		}
		Pack.itemid_ = iItemID;
		Pack.level_ = iLevel;
		Pack.pos_ = iType;
		Pack.count_ = 1;//add by xujincai 20081215 默认数量为1
		Pack.tech_ = skill;
		Pack.techlevel_ = skilllevel;
		if(pProxy->db_insert (Pack) != DB_Proxy::DB_SUCCEED)
		{
			return 0;
		}
	}
	return Pack.sortid_;
}
//英雄成长
int OnHeroGrowUp(DB_Proxy *pProxy, tagHeroParam & HeroParam,const int method)
{
	PlayerOfficerTable Officer;
	PlayerOfficerTable UpDataOfficer;
	Officer.setmasterid(HeroParam.lPlayerID);
	Officer.setdetailid(HeroParam.lHeroID);
	if (pProxy->db_select(Officer) != DB_Proxy::DB_SUCCEED)
	{
		return 0;
	}
	if (HeroParam.iExp < 0 ) 
		HeroParam.iExp = 0;

	if (HeroParam.iCredit < 0 )
		HeroParam.iCredit = 0;

	
	int iSExp = HeroParam.iExp;
	int iExp = Officer.exp_ + HeroParam.iExp;
	int iCredit = Officer.credit_ + HeroParam.iCredit;
	//20100412
	if (HeroParam.iExp >0)
	{
		ISystemLog::getInstance()->heroExp_Log(Officer.playerid_,Officer.officerid_,Officer.officername_,
			(MethodType)method,Officer.exp_,iExp,HeroParam.iExp);
	}
	int iLevel = GetLevel(Officer.level_,iExp, iSExp);//HeroExp(iExp, Officer.level_,iSExp);
	int iRank = HeroCredit(iCredit);
	if (HeroParam.iCredit >0 && HeroParam.lPlayerID == HeroParam.lHeroID)
	{
		ISystemLog::getInstance()->playerCredit_Log(Officer.playerid_,Officer.officername_,
			(MethodType)method,Officer.credit_,iCredit,HeroParam.iCredit,
			Officer.militaryrank_,iRank,iRank - Officer.militaryrank_);
	}
	int iFlag1=0,iFlag2=0;

	memset(&UpDataOfficer, 0, sizeof(PlayerOfficerTable));
	if(Officer.level_ < iLevel)
	{
		iFlag1 = 1;
		if (iSExp  < 0) iSExp = 0;
		UpDataOfficer.level_ = iLevel-Officer.level_;
		heroUpgrade(UpDataOfficer.level_,&Officer,&UpDataOfficer);
		//add by xujincai 20090205 根据耐力等增加英雄的攻防属性
		UpDataOfficer.jobid_ = Officer.jobid_;
		ProcHeroAttribute(UpDataOfficer);
		UpDataOfficer.jobid_ = 0;
		//add by xujincai 20081227升级增加技能点
		UpDataOfficer.techdot_ = UpDataOfficer.level_ * 5;
	}
	if (Officer.militaryrank_ < iRank)
	{
		iFlag2 = 1;
		UpDataOfficer.militaryrank_ = iRank - Officer.militaryrank_;//modify by xuincai 20090216
	}
	UpDataOfficer.exp_ = iSExp - Officer.exp_;
	UpDataOfficer.credit_ = HeroParam.iCredit;
	UpDataOfficer.enemycount_ = HeroParam.iEnemyCount;
	UpDataOfficer.generalcount_ = HeroParam.iGeneralCount;
	UpDataOfficer.setmasterid(HeroParam.lPlayerID);
	UpDataOfficer.setdetailid(HeroParam.lHeroID);
	PlayerOfficerTable oldUpdataOfficer = UpDataOfficer;
	if (pProxy->db_increase(UpDataOfficer) != DB_Proxy::DB_SUCCEED)
		return 0;
	if(HeroParam.iGeneralCount>0 || HeroParam.iEnemyCount>0)
	{
		WarNote Record;
		Record.playerid_ = HeroParam.lPlayerID;
		Record.officerid_ = HeroParam.lHeroID;
		Record.enterdate_ = IceUtil::Time::now().toSeconds();//HeroParam.lStartFight;
		Record.mapid_ = HeroParam.lMapID;
		Record.enemycount_ = HeroParam.iEnemyCount;
		Record.generalcount_ = HeroParam.iGeneralCount;
		Record.exitdate_ = IceUtil::Time::now().toSeconds();
		Record.state_ = HeroParam.iState;
		Record.freedate_ = 0;
		strcpy(Record.armyname_, HeroParam.szArmyName);
		pProxy->db_insert(Record);
	}
	if (iFlag1 == 1)
	{
		/*
		PullulateNote Record;
		Record.playerid_ = HeroParam.lPlayerID;
		Record.officerid_ = HeroParam.lHeroID;
		Record.type_ = 1;
		Record.level_ = iLevel;
		Record.upgradedate_ = IceUtil::Time::now().toSeconds();
		Proxy.db_insert(Record);
		MILITARY_RANK Rank;
	
		sprintf(Officer.officername_, "%d - %d ...",iTotExp,HeroParam.iExp);
		Rank.serialize(Officer.officername_, 1, 0,0,EN_S_BROADCAST_MILITARY_RANK);
		broadcast((char *)&Rank, Rank.length);
		*/
		if (UpDataOfficer.mapid_ >0 && UpDataOfficer.state_ ==EN_HAVE_ARMY)
		{
			heroModifyParamEvent modifyOfficerParam;
			modifyOfficerParam.serialize(UpDataOfficer.playerid_,UpDataOfficer.officerid_);
			event_notify(UpDataOfficer.mapid_, (char * )&modifyOfficerParam, modifyOfficerParam.length);// 发往服务器,
		}
		ISystemLog::getInstance()->heroExp_Log(Officer.playerid_,Officer.officerid_,Officer.officername_,
			(MethodType)method,iExp,UpDataOfficer.exp_,iExp - UpDataOfficer.exp_);
		ISystemLog::getInstance()->heroSkillDot_Upgroup_Log(Officer.playerid_,Officer.officerid_,Officer.officername_,
			Officer.techdot_,UpDataOfficer.techdot_,oldUpdataOfficer.techdot_);
		ISystemLog::getInstance()->heroUpgroup_Log(Officer.playerid_,Officer.officerid_,Officer.officername_,
			UPGROUPLEVEL_METHOD,Officer.level_,UpDataOfficer.level_,oldUpdataOfficer.level_,
			Officer.levelstamina_,UpDataOfficer.levelstamina_,oldUpdataOfficer.levelstamina_,
			Officer.levelpower_,UpDataOfficer.levelpower_,oldUpdataOfficer.levelpower_,
			Officer.levelcelerity_,UpDataOfficer.levelcelerity_,oldUpdataOfficer.levelcelerity_,
			Officer.levelwit_,UpDataOfficer.levelwit_,oldUpdataOfficer.levelwit_);
	}
	if (iFlag2 == 1)
	{
		PullulateNote Record;
		Record.playerid_ = HeroParam.lPlayerID;
		Record.officerid_ = HeroParam.lHeroID;
		Record.type_ = 2;
		Record.level_ = iRank;
		Record.upgradedate_ = IceUtil::Time::now().toSeconds();
		pProxy->db_insert(Record);

		MILITARY_RANK Rank;	
		Rank.serialize(Officer.officername_, iRank, 0,0,EN_S_BROADCAST_MILITARY_RANK);
		broadcast((char *)&Rank, Rank.length);
	}

	//add by xujincai 20081226英雄为重伤时，发送通知消息至大地图服务器
	if(HeroParam.iState ==EN_GBH)
	{
		HeroGBH_Notify heroGBHmsg;
		heroGBHmsg.serialize(HeroParam.lPlayerID,HeroParam.lHeroID);
		event_notify(SERVER_BOUND_MAP_OTHER,(char*)&heroGBHmsg,heroGBHmsg.length);

		UpDataOfficer.setmasterid(HeroParam.lPlayerID);
		UpDataOfficer.setdetailid(HeroParam.lPlayerID);
		if (pProxy->db_select(UpDataOfficer) != DB_Proxy::DB_SUCCEED)
		{
			return 0;
		}
		HeroGBH_Broadcast broadmsg;
		broadmsg.serialize(UpDataOfficer.officername_,Officer.officername_);
		broadcast((char *)&broadmsg, broadmsg.length);
	}
	//20100302
	if (HeroParam.iExp >0 || HeroParam.iCredit >0)
	{
		Notify_ExpOrCredit_Request notifyCons;
		notifyCons.serialize(HeroParam.lPlayerID,HeroParam.iExp,HeroParam.iCredit);
		event_notify(notifyCons.to,(char*)&notifyCons,notifyCons.length);
	}
	return 1;
}

//用于拍卖的战利品
//营阵ID:iCountry(联盟;部落),战利品ID:iItemID 战利品类型(见GOODS_TYPE枚举):iType 战利品数量:iCount
//输出:0:失败;1:成功
int OnVendueGoods(DB_Proxy *pProxy, int iCountry, unsigned long lMapID, int iItemID, int iType, int iCount)
{
	Market VendueMarket;
	memset(&VendueMarket, 0, sizeof(VendueMarket));

	VendueMarket.playerid_ = lMapID;
	VendueMarket.itemid_ = iItemID;
	VendueMarket.level_ = 0;
	VendueMarket.type_ = iType;
	VendueMarket.count_ = iCount;
	VendueMarket.minprice_ = 0;
	VendueMarket.currentprice_ = 0;
	VendueMarket.onceprice_ = 0;
	VendueMarket.bidcityid_ = 0;
	VendueMarket.starttime_ = IceUtil::Time::now().toSeconds();
	VendueMarket.country_ = iCountry;
	VendueMarket.mapid_ = lMapID;
	VendueMarket.cityid_ = 0;
	VendueMarket.bidmapid_ = 0;
	VendueMarket.bidcityid_ = 0;
	strcpy(VendueMarket.playername_, "");
	if (pProxy->db_insert(VendueMarket) != DB_Proxy::DB_SUCCEED)
		return 0;
	return 1;
}

int GetUnusedPos(PlayerItemStorageTable &packitem,DB_Proxy *pDBProxy)
{
	packitem.showpos_ = -1;
	packitem.sortid_ = 0;
	PlayerItemStorageTable itemPos;
	PlayerItemStorageTable *pRetSet = 0;
	char *retbuf = 0;
	int count = 0;
	int iRet = 0;
	int iPackNum = EN_PACK_GRID_NUM;
	if(packitem.officerid_ == 1)
	{
		PlayerChatRoomTable playertb;
		//取公共背包数量
		playertb.setid(packitem.playerid_);
		if(pDBProxy->db_select(playertb) != DB_Proxy::DB_SUCCEED)
		{
			packitem.showpos_  = -2;
			return -2;
		}
		iPackNum = playertb.maxpacknum_;
	}
	itemPos.setmasterid(packitem.officerid_,packitem.playerid_);
	iRet = pDBProxy->db_multi_select(itemPos,retbuf,count);
	if(iRet == DB_Proxy::DB_FAILED)
	{
		packitem.showpos_ = -2;
	}
	else if(iRet == DB_Proxy::DB_EOF)
	{//背包为空
		packitem.showpos_ = 0;
	}
	else
	{
		count = count /sizeof(PlayerItemStorageTable);
		pRetSet = (PlayerItemStorageTable*)retbuf;
		int tmppos[100] = {0};
		int i = 0;
		for(i = 0;i<count && i<iPackNum;i++)
		{
			if(pRetSet[i].type_ == 1)//英雄身上的除外
				continue;

			tmppos[pRetSet[i].showpos_] = 1;
			if (pRetSet[i].itemid_ == packitem.itemid_)
			{
				memcpy(&packitem,&pRetSet[i],sizeof(PlayerItemStorageTable));
				break;
			}
		}
		for(i=0;i<iPackNum && packitem.showpos_ <0;i++)
		{
			if(tmppos[i] ==0)
			{
				packitem.showpos_ = i;
				break;
			}
		}
	}
	if(retbuf)
	{
		delete[] retbuf;
		retbuf = 0;
	}
	return packitem.showpos_;
}
//查询玩家的新手村
int GetHomeCityInfo(DB_Proxy *pDBProxy,int playerid,int &mapid,int &cityid)
{
	char * buf = 0;
	int count = 0;
	PlayerCity playercity;
	PlayerCity *pCityList = 0;

	//获取玩家城市
	playercity.setmasterid(playerid);
	if(pDBProxy->db_multi_select(playercity,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		return 1;
	}
	count = count/sizeof(PlayerCity);
	pCityList = (PlayerCity*)buf;
	int i = 0;
	for(i=0;i<count;i++)
	{
		if(pCityList[i].state_ == 100)
		{
			mapid = pCityList[i].mapid_;
			cityid = pCityList[i].cityid_;
			break;
		}
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	if(i>=count)
	{
		return 2;
	}
	return 0;
}
//add by xujincai 20090205//更新武器、防具等,更新到玩家新手村城市（仓库容量足够），该函数只用于增加，
int UpdateProductionAmount(DB_Proxy *pDBProxy, unsigned long lPlayerID, int iItemID, int iType, int iCount)
{
	if(iCount <=0)
		return -1;

	int iFlag = 0;
	char *buf = 0;
	int count = 0;
	PlayerCity playercity;
	PlayerCity *pCityList = 0;

	PlayerResource playerres;
	ProductionAmount proAmount;

	//获取玩家城市
	playercity.setmasterid(lPlayerID);
	if(pDBProxy->db_multi_select(playercity,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		return 1;
	}
	count = count/sizeof(PlayerCity);
	pCityList = (PlayerCity*)buf;
	playerres.setdetailid(lPlayerID);
	int i = 0;
	for(i=0;i<count;i++)
	{
		//if(pCityList[i].mapid_ == 1001 || pCityList[i].mapid_ == 1006)
		if(pCityList[i].state_ == 100)
		{
			playerres.setmasterid(pCityList[i].cityid_,pCityList[i].mapid_);
			break;
		}
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	if(i>=count)
	{
		return 2;
	}
	if(pDBProxy->db_select(playerres) != DB_Proxy::DB_SUCCEED)
	{
		return 3;
	}

	if(playerres.content_ < iCount)
		return 4;//容量不够

	proAmount.setmasterid(playerres.mapid_,playerres.cityid_,lPlayerID,iType);
	proAmount.setdetailid(iItemID);
	iFlag = pDBProxy->db_select(proAmount);
	proAmount.productioncount_ = iCount;
	if(iFlag == DB_Proxy::DB_SUCCEED)
	{//增加
		iFlag = 5;
		if(pDBProxy->db_increase(proAmount) == DB_Proxy::DB_SUCCEED)
			iFlag = 0;
		
	}
	else if(iFlag == DB_Proxy::DB_EOF)
	{//新增
		iFlag = 5;
		if(pDBProxy->db_insert(proAmount) == DB_Proxy::DB_SUCCEED)
			iFlag = 0;
	}
	else
	{
		iFlag = 5;
		return 5;
	}
	//添加成功，修改仓库容量
	if(iFlag == 0)
	{
		PlayerResource updateRes;
		updateRes.setmasterid(playerres.cityid_,playerres.mapid_);
		updateRes.setdetailid(lPlayerID);
		updateRes.content_ = -iCount;
		pDBProxy->db_increase(updateRes);
	}
	return iFlag;
}
int UpdateProductionAmountEx(DB_Proxy *pDBProxy,unsigned long lPlayerID, int mapid,int ciytid,int iItemID, int iType, 
							 int iCount,int &oldcount,int &curcount)
{

	if(iCount <=0)
		return -1;

	int iFlag = 0;
	PlayerResource playerres;
	ProductionAmount proAmount;
	playerres.setdetailid(lPlayerID);
	playerres.setmasterid(ciytid,mapid);
	if(pDBProxy->db_select(playerres) != DB_Proxy::DB_SUCCEED)
	{
		return 3;
	}
	oldcount = proAmount.productioncount_;
	if(playerres.content_ < iCount)
		return 4;//容量不够

	proAmount.setmasterid(playerres.mapid_,playerres.cityid_,lPlayerID,iType);
	proAmount.setdetailid(iItemID);
	iFlag = pDBProxy->db_select(proAmount);
	proAmount.productioncount_ = iCount;
	if(iFlag == DB_Proxy::DB_SUCCEED)
	{//增加
		iFlag = 5;
		if(pDBProxy->db_increase(proAmount) == DB_Proxy::DB_SUCCEED)
			iFlag = 0;

	}
	else if(iFlag == DB_Proxy::DB_EOF)
	{//新增
		iFlag = 5;
		if(pDBProxy->db_insert(proAmount) == DB_Proxy::DB_SUCCEED)
			iFlag = 0;
	}
	else
	{
		iFlag = 5;
		return 5;
	}
	//添加成功，修改仓库容量
	if(iFlag == 0)
	{
		curcount = proAmount.productioncount_;
		PlayerResource updateRes;
		updateRes.setmasterid(playerres.cityid_,playerres.mapid_);
		updateRes.setdetailid(lPlayerID);
		updateRes.content_ = -iCount;
		pDBProxy->db_increase(updateRes);
	}
	return iFlag;
}
void ProcHeroAttribute(PlayerOfficerTable &hero)
{//新表把基本属性分为三部分：初始、升级、装备
	/*
	Saber_OFFICER				=1, //圣骑士
	WARRIOR_OFFICER				=2 ,//战士
	SHOOTER_OFFICER				=3 ,//弓箭手
	MAGE_OFFICER				=4 ,//法师
	PRIEST_OFFICER				=5 ,//牧师
	HUNTER_OFFICER				=6 ,//猎人
	WARLOCK_OFFICER				=7 ,//术士
	Rogue_OFFICER				=8 ,//潜行者
	*/
	int totalPower = hero.power_ + hero.levelpower_ + hero.equippower_;//力量
	int totalStamina = hero.stamina_ + hero.levelstamina_ + hero.equipstamina_;//耐力
	int totalCelerity = hero.celerity_ + hero.levelcelerity_ + hero.equipcelerity_;//敏捷
	int totalWit = hero.wit_ + hero.levelwit_ + hero.equipwit_;//智力
	if(hero.jobid_ == Saber_OFFICER || hero.jobid_ == WARRIOR_OFFICER)
	{//战士系
		hero.attack_ += totalPower* 5;//物理攻击力
		hero.forcerate_ += totalPower/3000;//暴击率
		hero.physicrecovery_ += totalPower * 5;//物理防御
		hero.magicrecovery_ += totalWit * 5;//魔法防御
		hero.herohp_ += totalStamina;//HP
	}
	else if(hero.jobid_ == SHOOTER_OFFICER)
	{//弓箭手系
		hero.attack_ += totalCelerity * 5;//攻击力
		hero.forcerate_ += totalCelerity/3000;//暴击率
		hero.physicrecovery_ += totalPower * 5;//物理防御
		hero.magicrecovery_ += totalWit * 5;//魔法防御
		hero.herohp_ += totalStamina;//HP
	}
	else if(hero.jobid_ == MAGE_OFFICER)
	{//法师系
		hero.magicattack_ += totalWit * 5;//攻击力
		hero.forcerate_ += totalWit/3000;//暴击率
		hero.physicrecovery_ += totalPower * 5;//物理防御
		hero.magicrecovery_ += totalWit * 5;//魔法防御
		hero.herohp_ += totalStamina;//HP
	}
	else if(hero.jobid_ == PRIEST_OFFICER)
	{//牧师系
		hero.magicattack_ += totalWit * 5;//攻击力
		hero.forcerate_ += totalWit/3000;//暴击率
		hero.physicrecovery_ += totalPower * 5;//物理防御
		hero.magicrecovery_ += totalWit * 5;//魔法防御
		hero.herohp_ += totalStamina;//HP
	}
	else
	{
		hero.attack_ = 0;//攻击力
		hero.physicrecovery_ = 0;//物理防御
		hero.magicrecovery_ = 0;//魔法防御
	}
}
/*删除背包中的一个项目
@param dbProxy:DBG代理
@param item:项目信息，该项目的全部详细信息
*/
int deleteItemOfPack(DB_Proxy *dbProxy,PlayerItemStorageTable &item)
{
	if (item.sortid_ <=0)
	{
		return -1;
	}
	if (item.count_ <=1)
	{
		return dbProxy->db_delete(item);
	}
	PlayerItemStorageTable updateItem;
	updateItem.setmasterid(item.officerid_,item.playerid_);
	updateItem.setdetailid(item.sortid_);
	updateItem.count_ = -1;
	int iRet= dbProxy->db_increase(updateItem);
	//20100409
	if (iRet == DB_Proxy::DB_SUCCEED && item.pos_ != ARMYBRAND && item.pos_ != TREASUREPACK)
	{
		PlayerOfficerTable officer;
		officer.setmasterid(item.playerid_);
		officer.setdetailid(item.playerid_);
		dbProxy->db_select(officer);
		ISystemLog::getInstance()->playerEquip_Use_Log(officer.playerid_,officer.officername_,item.pos_,item.itemid_,1);
	}
	return iRet;
}

GOODS_TYPE getGoodsTypeEx(int goodsId)
{
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
//从一段范围值内随机一个数
int getRandomValue(int minvalue,int maxvalue)
{
	int tmpvalue = 0;
	if (minvalue == maxvalue)
	{
		return minvalue;
	}
	else if (minvalue >maxvalue)
	{
		tmpvalue = minvalue - maxvalue;
		minvalue = maxvalue;
	}
	else
	{
		tmpvalue = maxvalue - minvalue;
	}
	return minvalue + rand()%tmpvalue;
}
#pragma warning(pop)
