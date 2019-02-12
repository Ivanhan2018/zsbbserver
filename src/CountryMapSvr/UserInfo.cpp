#include "UserInfo.h"
#include "log.hpp"
#include "LoadDBInfo.h"
#pragma warning(disable:4127)
#pragma warning(disable:4189)
#pragma warning(disable:4701)
CUserInfo::CUserInfo()
{
}
CUserInfo::~CUserInfo()
{
}
int CUserInfo::m_iOutPut[] = {100,100,100,100};
//获取玩家所在公会的产量
int CUserInfo::getConsortiaRate(int playerid,BaseRate &rate)
{
	rate.foodRate = rate.woodRate = rate.ironRate = rate.skinRate = 100;

	PlayerChatRoomTable playertb;
	playertb.setid(playerid);
	if (g_Proxy.db_select(playertb) != DB_Proxy::DB_SUCCEED)
	{
		return -1;
	}
	if (playertb.consortiaid_ <=0)
	{
		return 1;
	}
	ConsortiaInfo consInfo;
	consInfo.setid(playertb.consortiaid_);
	if (g_Proxy.db_select(consInfo) != DB_Proxy::DB_SUCCEED)
	{
		return -1;
	}
	ConsortiaUpdateParam *pParam = g_LoadDBinfo.getConsortiaUpdateParam(consInfo.level_);
	if (pParam == NULL)
	{
		return -2;
	}
	rate.foodRate += pParam->baseRate.foodRate;
	rate.woodRate += pParam->baseRate.woodRate;
	rate.ironRate += pParam->baseRate.ironRate;
	rate.skinRate += pParam->baseRate.skinRate;
	if (pParam->extraUserCount >0)
	{
		int tmpRate = consInfo.count_ / pParam->extraUserCount;
		rate.foodRate += tmpRate * pParam->extraRate.foodRate;
		rate.woodRate += tmpRate * pParam->extraRate.woodRate;
		rate.ironRate += tmpRate * pParam->extraRate.ironRate;
		rate.skinRate += tmpRate * pParam->extraRate.skinRate;
	}
	return 0;
}
//玩家资源量
int CUserInfo::PlayerResourceCount(int handle, unsigned long MapID, unsigned long CityID, unsigned long lPlayerID, PALEYER_RESOURCE &PlayerRes,unsigned long lFrom,unsigned long lTo)
{
	int iProRes = 0, ret = 0;
	PlayerResource ResByPlayer;
	RESOURCE_RATE ResRate,ResRate_Precent;
	memset(&ResRate, 0, sizeof(RESOURCE_RATE));
	memset(&ResByPlayer, 0,sizeof(ResByPlayer));
	ResByPlayer.setmasterid(CityID, MapID);
	ResByPlayer.setdetailid(lPlayerID);
	int dbRet = g_Proxy.db_select(ResByPlayer);
	if ( dbRet == DB_Proxy::DB_EOF)
	{
		printf("玩家 %ld 在郡内%ld 城市中%ld 没有资源量 g_Proxy.db_select(ResByPlayer) Error!\n",lPlayerID,MapID,CityID);
		return false;
	}
	else if (dbRet == DB_Proxy::DB_FAILED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, MapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("玩家资源量 money  db_select(ResByPlayer) dbRet == DB_Proxy::DB_FAILED error\n"));
		return ret;
	}
	PlayerResource oldRes = ResByPlayer;

	PlayerChatRoomTable Res;
	memset(&Res, 0, sizeof(Res));
	Res.setid(lPlayerID);
	dbRet = g_Proxy.db_select(Res) ;
	if (dbRet == DB_Proxy::DB_EOF)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATA_ERROR, MapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("CUserInfo::PlayerResourceCount-->查询玩家%d失败（PlayerChatRoomTable）。 \n",lPlayerID));;
		return 0;
	}
	else if (dbRet == DB_Proxy::DB_FAILED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, MapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("玩家资源量1 money  g_Proxy.db_select(Res) error \n"));
		return 0;
	}
	PlayerOfficerTable officer;
	officer.setmasterid(lPlayerID);
	officer.setdetailid(lPlayerID);
	g_Proxy.db_select(officer);
	PlayerChatRoomTable oldScience;
	oldScience = Res;
	if (handle>0)
	{
		//int iFoodRate = 0, iWoodRate = 0, iSkinRate = 0, iIronRate = 0, iScience = 0;
		unsigned long lTotaRes = 0L;
		char * buf = 0;
		int count = 0;
		int64 lTime = IceUtil::Time::now().toSeconds();
		PlayerConstruction PlayerConstr;
		memset(&PlayerConstr, 0, sizeof(PlayerConstruction));

		PlayerConstr.setmasterid(MapID, CityID, lPlayerID);
		dbRet = g_Proxy.db_multi_select(PlayerConstr, buf, count) ;
		int ConCount = count/sizeof(PlayerConstruction);
		PlayerConstruction *pPlayerCon = (PlayerConstruction *)buf;

		//城市中仓库容量限制
		lTotaRes = ResByPlayer.content_;
		if (lTotaRes<=0)
		{
			iProRes = 1;
		}
		GameCityInfo * pResParam = g_LoadDBinfo.GetCityInfo(CityID);
		if (pResParam == NULL) 
		{
			//_ASSERT(false);
			//add by xujincai 20081216
			if(buf)
			{
				delete[] buf;
				buf = 0;
			}
			return 0;
		}
		unsigned int itmp = 0;
		/*delete by xujincai 20090217
		for (int in = 0; in < 4; in++)
		{
			if (m_iOutPut[in] < 1 || m_iOutPut[in] > 100) m_iOutPut[in] = 1;
		}*/
		/*
		产量=(筑物等级产量*本郡R资源含量*R资源对应的公共建筑等级修正值*(1+收费修正系数))/(100*100)
		*/

		//获取生产速率
		ResRate_Precent.iFoodRate  = g_SpeedCard.GetFoodValue(lPlayerID);
		ResRate_Precent.iWoodRate  = g_SpeedCard.GetWoodValue(lPlayerID);		
		ResRate_Precent.iSkinRate  = g_SpeedCard.GetSkinValue(lPlayerID);
		ResRate_Precent.iIronRate  = g_SpeedCard.GetIronValue(lPlayerID);
		ResRate_Precent.iScience = 100;
		unsigned __int64 iCurRate = 0;
		int TimeLen = 0;
		int iHour = 0;
		int iFlag = 0;
		int tmprate[3] = {100,10000,1000000};
		BaseRate consRate;
		getConsortiaRate(lPlayerID,consRate);
		for (int n = 0; n < ConCount; n++)
		{
			iFlag = 0;
			itmp = 0;
			if ((pPlayerCon[n].constructionid_ == 1))
			{
				//modify by xujincai 20090107 速率使用百分比,同时为了减少乘法运算
				iCurRate = pPlayerCon[n].output_ * pResParam->foodcount_; //* m_iOutPut[0] * ResRate_Precent.iFoodRate;
				if(m_iOutPut[0] >100)
				{
					iCurRate = iCurRate *m_iOutPut[0];
					iFlag++;
				}
				if(ResRate_Precent.iFoodRate >100)
				{
					iCurRate = iCurRate *ResRate_Precent.iFoodRate;
					iFlag++;
				}
				iCurRate = iCurRate/tmprate[iFlag];
				if (consRate.foodRate >100)
				{
					iCurRate = iCurRate * consRate.foodRate/100;
				}
				ResRate.iFoodRate += (int)iCurRate;
				if(iProRes ==1)
					continue;

				TimeLen = int(lTime - pPlayerCon[n].completetimes_);
				if(TimeLen > MAX_TIMELEN_RESOURCE)
				{
					TimeLen = MAX_TIMELEN_RESOURCE;
				}
				if(TimeLen <=0)
					continue;

				if(TimeLen >3600)
				{
					iHour = TimeLen /3600;
					TimeLen = TimeLen % 3600;
					itmp = (unsigned int)iCurRate * iHour;
				}
				if(TimeLen)
				{
					itmp = itmp + (unsigned int)iCurRate * TimeLen /3600;
				}
				if (itmp<=0) 
					continue;
				pPlayerCon[n].completetimes_ = lTime;
				pPlayerCon[n].setmasterid(pPlayerCon[n].mapid_, pPlayerCon[n].cityid_, pPlayerCon[n].playerid_);
				pPlayerCon[n].setdetailid(pPlayerCon[n].sortid_);
				g_Proxy.db_update(pPlayerCon[n],pPlayerCon[n]);
				if (lTotaRes<=itmp)
				{
					iProRes = 1;
					itmp = lTotaRes;
				}
				lTotaRes  -= itmp;
				ResByPlayer.foodcount_ += itmp;
			}
			else if ((pPlayerCon[n].constructionid_ == 2))
			{
				//modify by xujincai 20090107 速率使用百分比,同时为了减少乘法运算
				iCurRate = pPlayerCon[n].output_ * pResParam->woodcount_;//*  m_iOutPut[3] * ResRate_Precent.iWoodRate/1000000;
				
				if(m_iOutPut[3] >100)
				{
					iCurRate = iCurRate *m_iOutPut[3];
					iFlag++;
				}
				if(ResRate_Precent.iWoodRate >100)
				{
					iCurRate = iCurRate *ResRate_Precent.iWoodRate;
					iFlag++;
				}
				iCurRate = iCurRate/tmprate[iFlag];
				if (consRate.woodRate >100)
				{
					iCurRate = iCurRate * consRate.woodRate/100;
				}
				ResRate.iWoodRate  += (int)iCurRate;
				if(iProRes ==1)
					continue;

				TimeLen = int(lTime - pPlayerCon[n].completetimes_);
				if(TimeLen > MAX_TIMELEN_RESOURCE)
				{
					TimeLen = MAX_TIMELEN_RESOURCE;
				}
				if(TimeLen <=0)
					continue;
				if(TimeLen >3600)
				{
					iHour = TimeLen /3600;
					TimeLen = TimeLen % 3600;
					itmp = (unsigned int)iCurRate * iHour;
				}
				if(TimeLen)
				{
					itmp = itmp + (unsigned int)iCurRate * TimeLen /3600;
				}
				if (itmp<=0) 
					continue;
				pPlayerCon[n].completetimes_ = lTime;
				pPlayerCon[n].setmasterid(pPlayerCon[n].mapid_, pPlayerCon[n].cityid_, pPlayerCon[n].playerid_);
				pPlayerCon[n].setdetailid(pPlayerCon[n].sortid_);
				g_Proxy.db_update(pPlayerCon[n],pPlayerCon[n]);
				if (lTotaRes<=itmp)
				{
					iProRes = 1;
					itmp = lTotaRes;
				}
				lTotaRes  -= itmp;
				ResByPlayer.woodcount_ += itmp;

			}
			else if ((pPlayerCon[n].constructionid_ == 3))
			{
				//modify by xujincai 20090107 速率使用百分比,同时为了减少乘法运算
				iCurRate = pPlayerCon[n].output_ * pResParam->skincount_;// * m_iOutPut[2] * ResRate_Precent.iSkinRate /1000000;
				if(m_iOutPut[2] >100)
				{
					iCurRate = iCurRate *m_iOutPut[2];
					iFlag++;
				}
				if(ResRate_Precent.iSkinRate >100)
				{
					iCurRate = iCurRate *ResRate_Precent.iSkinRate;
					iFlag++;
				}
				iCurRate = iCurRate/tmprate[iFlag];
				if (consRate.skinRate >100)
				{
					iCurRate = iCurRate * consRate.skinRate/100;
				}
				ResRate.iSkinRate += (int)iCurRate;
				if(iProRes ==1)
					continue;

				TimeLen = int(lTime - pPlayerCon[n].completetimes_);
				if(TimeLen > MAX_TIMELEN_RESOURCE)
				{
					TimeLen = MAX_TIMELEN_RESOURCE;
				}
				if(TimeLen <=0)
					continue;
				if(TimeLen >3600)
				{
					iHour = TimeLen /3600;
					TimeLen = TimeLen % 3600;
					itmp = (unsigned int)iCurRate * iHour;
				}
				if(TimeLen)
				{
					itmp = itmp + (unsigned int)iCurRate * TimeLen /3600;
				}
				if (itmp<=0) 
					continue;
				pPlayerCon[n].completetimes_ = lTime;
				pPlayerCon[n].setmasterid(pPlayerCon[n].mapid_, pPlayerCon[n].cityid_, pPlayerCon[n].playerid_);
				pPlayerCon[n].setdetailid(pPlayerCon[n].sortid_);
				g_Proxy.db_update(pPlayerCon[n],pPlayerCon[n]);

				if (lTotaRes<=itmp)
				{
					iProRes = 1;
					itmp = lTotaRes;
				}
				lTotaRes  -= itmp;
				ResByPlayer.skincount_ += itmp;

			}
			else if ((pPlayerCon[n].constructionid_ == 4))
			{
				//modify by xujincai 20090107 速率使用百分比,同时为了减少乘法运算
				iCurRate = pPlayerCon[n].output_ * pResParam->ironcount_;//* m_iOutPut[1] * ResRate_Precent.iIronRate/1000000;
				if(m_iOutPut[1] >100)
				{
					iCurRate = iCurRate *m_iOutPut[1];
					iFlag++;
				}
				if(ResRate_Precent.iIronRate >100)
				{
					iCurRate = iCurRate *ResRate_Precent.iIronRate;
					iFlag++;
				}
				iCurRate = iCurRate/tmprate[iFlag];

				if (consRate.ironRate >100)
				{
					iCurRate = iCurRate * consRate.ironRate/100;
				}
				ResRate.iIronRate += (int)iCurRate;
				if(iProRes ==1)
					continue;

				int TimeLen = int(lTime - pPlayerCon[n].completetimes_);
				if(TimeLen > MAX_TIMELEN_RESOURCE)
				{
					TimeLen = MAX_TIMELEN_RESOURCE;
				}
				if(TimeLen <=0)
					continue;
				if(TimeLen >3600)
				{
					iHour = TimeLen /3600;
					TimeLen = TimeLen % 3600;
					itmp = (unsigned int)iCurRate * iHour;
				}
				if(TimeLen)
				{
					itmp = itmp + (unsigned int)iCurRate * TimeLen /3600;
				}
				if (itmp<=0) 
					continue;
				pPlayerCon[n].completetimes_ = lTime;
				pPlayerCon[n].setmasterid(pPlayerCon[n].mapid_, pPlayerCon[n].cityid_, pPlayerCon[n].playerid_);
				pPlayerCon[n].setdetailid(pPlayerCon[n].sortid_);
				g_Proxy.db_update(pPlayerCon[n],pPlayerCon[n]);

				if (lTotaRes<=itmp)
				{
					iProRes = 1;
					itmp = lTotaRes;
				}
				lTotaRes  -= itmp;
				ResByPlayer.ironcount_ += itmp;

			}
			else if (pPlayerCon[n].constructionid_ == 6)
			{
				ResRate.iScience += pPlayerCon[n].output_;
				int TimeLen = int(lTime - pPlayerCon[n].completetimes_);
				if(TimeLen > MAX_TIMELEN_RESOURCE)
				{
					TimeLen = MAX_TIMELEN_RESOURCE;
				}
				if(TimeLen <=0)
					continue;

				if(TimeLen >3600)
				{
					iHour = TimeLen /3600;
					TimeLen = TimeLen % 3600;
					itmp = pPlayerCon[n].output_ * iHour;
				}
				if(TimeLen)
				{
					itmp = itmp + pPlayerCon[n].output_ * TimeLen /3600;
				}

				if (itmp<=0) 
					continue;
				pPlayerCon[n].completetimes_ = lTime;
				pPlayerCon[n].setmasterid(pPlayerCon[n].mapid_, pPlayerCon[n].cityid_, pPlayerCon[n].playerid_);
				pPlayerCon[n].setdetailid(pPlayerCon[n].sortid_);
				g_Proxy.db_update(pPlayerCon[n],pPlayerCon[n]);

				Res.science_ += itmp;

			}
		}
		ResByPlayer.content_ = lTotaRes;

		ResByPlayer.setmasterid(CityID, MapID);
		ResByPlayer.setdetailid(lPlayerID);
		if (g_Proxy.db_update(ResByPlayer,ResByPlayer) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE_EX(false,("CUserInfo::PlayerResourceCount %ld db_update(ResByPlayer,ResByPlayer) error \n",lPlayerID));
			return 0;
		}

		if ( Res.money_ < 0)
			Res.money_  = 0;
		if (Res.science_ < 0)
			Res.science_ = 0;

		Res.setid(lPlayerID);
		if (g_Proxy.db_update(Res,Res) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE_EX(false,("CUserInfo::PlayerResourceCount %ld db_update(Res,Res) error \n",lPlayerID));
			return 0;
		}
		if (buf != NULL)
		{
			delete [] buf;
			buf = 0;
		}
		//20100409
		if (oldScience.science_ != Res.science_)
		{
			ISystemLog::getInstance()->playerScience_Produce_Log(officer.playerid_,officer.officername_,
				oldScience.science_,Res.science_,Res.science_ - oldScience.science_,ResRate.iScience);
		}
		ISystemLog::getInstance()->playerResource_Log(officer.playerid_,officer.officername_, MapID,CityID,
			EN_FOOD,oldRes.foodcount_,ResByPlayer.foodcount_,ResByPlayer.foodcount_ - oldRes.foodcount_,ResRate.iFoodRate);
		ISystemLog::getInstance()->playerResource_Log(officer.playerid_,officer.officername_, MapID,CityID,
			EN_WOOD,oldRes.woodcount_,ResByPlayer.woodcount_,ResByPlayer.woodcount_ - oldRes.woodcount_,ResRate.iWoodRate);
		ISystemLog::getInstance()->playerResource_Log(officer.playerid_,officer.officername_, MapID,CityID,
			EN_IRON,oldRes.ironcount_,ResByPlayer.ironcount_,ResByPlayer.ironcount_ - oldRes.ironcount_,ResRate.iIronRate);
		ISystemLog::getInstance()->playerResource_Log(officer.playerid_,officer.officername_, MapID,CityID,
			EN_SKIN,oldRes.skincount_,ResByPlayer.skincount_,ResByPlayer.skincount_ - oldRes.skincount_,ResRate.iSkinRate);
	}
	PlayerRes.iFoodCount = ResByPlayer.foodcount_;
	PlayerRes.iWoodCount = ResByPlayer.woodcount_;
	PlayerRes.iIronCount = ResByPlayer.ironcount_;
	PlayerRes.iSkinCount = ResByPlayer.skincount_;
	PlayerRes.iRedStoneCount = ResByPlayer.redstonecount_;
	PlayerRes.iYellowStoneCount = ResByPlayer.yollowstonecount_;
	PlayerRes.iGreenStoneCount = ResByPlayer.greenstonecount_;
	PlayerRes.iBackIronCount = ResByPlayer.backironcount_;
	PlayerRes.iMoney = Res.money_;
	PlayerRes.iScience = Res.science_;
	//

	if (handle <= 0 || lFrom <= 0 || lTo <= 0) return 1;
	PlayerRes.serialize(ResByPlayer.foodcount_,ResByPlayer.woodcount_,ResByPlayer.ironcount_,ResByPlayer.skincount_,ResByPlayer.redstonecount_,
		ResByPlayer.yollowstonecount_,ResByPlayer.greenstonecount_,ResByPlayer.backironcount_,PlayerRes.iScience,PlayerRes.iMoney,Res.contribute_,
		ResByPlayer.content_,lFrom,lTo);
	ret = asynch_write(handle, (char *)&PlayerRes, PlayerRes.length);

	ResRate.serialize(ResRate.iFoodRate, ResRate.iWoodRate, ResRate.iSkinRate, ResRate.iIronRate, ResRate.iScience, lFrom, lTo,EN_S_RESOURCE_RATE);
	ret = asynch_write(handle, (char *)&ResRate, ResRate.length);

	//add by xujincai 20090110增加返回百分比速率
	ResRate_Precent.serialize(ResRate_Precent.iFoodRate,ResRate_Precent.iWoodRate,ResRate_Precent.iSkinRate,ResRate_Precent.iIronRate,
		ResRate_Precent.iScience,lFrom,lTo,EN_S_RESOURCE_PERCENTRATE);
	asynch_write(handle, (char *)&ResRate_Precent, ResRate_Precent.length);
	return ret;
}
//跟据建筑物ID计算此玩家目前对应的资源产量
int CUserInfo::ResourceCount(int ConstructionID, unsigned long MapID, unsigned long CityID, unsigned long lPlayerID)
{
	/*PlayerConstruction Constr;
	Constr.setmasterid(MapID, CityID, lPlayerID);
	Constr.setdetailid(ConstructionID);

	if (g_Proxy.db_select(Constr) != DB_Proxy::DB_SUCCEED)
	{
		printf("玩家: %ld 跟据建筑物ID(%d)计算 失败,因还没有建筑此建筑物!!!\n",lPlayerID,ConstructionID);
		return 0;
	}
	 
	int64 lTime = GetCurrentTime();
	int64 TimeLen = (lTime - Constr.completetimes_)/60;
	if ((TimeLen > 0) && (TimeLen < 525600))
	{
		Constr.completetimes_ = lTime;

		Constr.setmasterid(MapID, CityID, lPlayerID);
		Constr.setdetailid(ConstructionID,0);
		g_Proxy.db_update(Constr,Constr);

		return Constr.output_ * TimeLen;
	}*/

	return 0;
}
//更新资源
int CUserInfo::OnUpDataResource (int handle,  PlayerResource & Res,unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID,unsigned long lFrom,unsigned long lTo, int iMode,int iItemID, int iCount,int iMoney)
{
	int ret = 0;
	MARKET_INFO MarketInfo;
	PlayerResource ResByPlayer;
	ResByPlayer.setmasterid(lCityID, lMapID);
	ResByPlayer.setdetailid(lPlayerID);
	int dbRet = g_Proxy.db_select(ResByPlayer);
	if (dbRet == DB_Proxy::DB_FAILED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, lFrom, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("更新玩家(%ld)资源 g_Proxy.db_select(ResByPlayer) == DB_FAILED Error!\n",lPlayerID));
		return 0;
	}
	else if (dbRet == DB_Proxy::DB_EOF)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATA_ERROR, lFrom, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("更新玩家(%ld)资源1 g_Proxy.db_select(ResByPlayer) == DB_EOF Error!\n",lPlayerID));
		return 0;//EN_CREATE_ARMY_COUNT
	}

	// 1:卖; 2:买
	if (iMode == 2)
	{
		//add by xujincai 20081224增加库存容量判断
		if(ResByPlayer.content_ <iCount)
		{
			ERROR_INFO ErrorInfo;
			ErrorInfo.serialize(EN_CITY_NO_CONTENT, lFrom, lPlayerID);
			ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
			printf("更新玩家(%ld)资源1 g_Proxy.db_select(ResByPlayer) == DB_EOF Error!\n",lPlayerID);
			return 0;//
		}
		memset(&ResByPlayer,0,sizeof(PlayerResource));
		ResByPlayer.content_ = -iCount;
	}
	else//卖，需要判断库存
	{
		//20090817增加库存的判断
		memset(&ResByPlayer,0,sizeof(PlayerResource));
		ResByPlayer.setmasterid(lCityID, lMapID);
		ResByPlayer.setdetailid(lPlayerID);
		if(g_Proxy.db_select(ResByPlayer) != DB_Proxy::DB_SUCCEED)
		{
			return 0;
		}
		if (ResByPlayer.foodcount_ + Res.foodcount_ <0)
		{
			return 0;
		}
		if (ResByPlayer.woodcount_ + Res.woodcount_ <0)
		{
			return 0;
		}
		if (ResByPlayer.ironcount_ + Res.ironcount_ <0)
		{
			return 0;
		}
		if (ResByPlayer.skincount_ + Res.skincount_ <0)
		{
			return 0;
		}
		if (ResByPlayer.redstonecount_ + Res.redstonecount_ <0)
		{
			return 0;
		}
		if (ResByPlayer.yollowstonecount_ + Res.yollowstonecount_ <0)
		{
			return 0;
		}
		if (ResByPlayer.greenstonecount_ + Res.greenstonecount_ <0)
		{
			return 0;
		}
		if (ResByPlayer.backironcount_ + Res.backironcount_ <0)
		{
			return 0;
		}
		memset(&ResByPlayer,0,sizeof(PlayerResource));
		ResByPlayer.content_ = iCount;
	}
	ResByPlayer.setmasterid(lCityID, lMapID);
	ResByPlayer.setdetailid(lPlayerID);
	ResByPlayer.foodcount_ += Res.foodcount_;
	ResByPlayer.woodcount_ += Res.woodcount_;
	ResByPlayer.ironcount_ += Res.ironcount_;
	ResByPlayer.skincount_ += Res.skincount_;

	ResByPlayer.redstonecount_ += Res.redstonecount_;
	ResByPlayer.yollowstonecount_ += Res.yollowstonecount_;
	ResByPlayer.greenstonecount_ += Res.greenstonecount_;
	ResByPlayer.backironcount_ += Res.backironcount_;

	PlayerChatRoomTable PlayerRes;
	PlayerRes.setid(lPlayerID);
	if (g_Proxy.db_select(PlayerRes) != DB_Proxy::DB_SUCCEED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, lFrom, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("更新玩家(%ld)资源1 g_Proxy.db_select(PlayerRes)  Error!\n",lPlayerID));
		return  0;
	}
	PlayerRes.money_ += iMoney;
	if (PlayerRes.money_ < 0)
	{
		MarketInfo.serialize(6, iItemID, iCount, EN_GOODS_NOT_ENOUGHT_GOLD, lFrom, lPlayerID);
		ret = asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
		return 0;
	}
	memset(&PlayerRes, 0, sizeof(PlayerRes));
	PlayerRes.setid(lPlayerID);
	PlayerRes.money_ = iMoney;
	if (g_Proxy.db_increase (PlayerRes) != DB_Proxy::DB_SUCCEED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, lFrom, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("更新玩家(%ld)资源3 db_update (PlayerRes,PlayerRes)  Error!\n",lPlayerID));
		return 0;
	}
	ResByPlayer.setmasterid(lCityID, lMapID);
	ResByPlayer.setdetailid(lPlayerID);
	if(g_Proxy.db_increase(ResByPlayer) != DB_Proxy::DB_SUCCEED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, lFrom, lPlayerID);

		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		//返回钱
		memset(&PlayerRes, 0, sizeof(PlayerRes));
		PlayerRes.setid(lPlayerID);
		PlayerRes.money_ = -iMoney;
		if (g_Proxy.db_increase (PlayerRes) != DB_Proxy::DB_SUCCEED)
		{
			ERROR_INFO ErrorInfo;
			ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, lFrom, lPlayerID);
			ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
			//_ASSERT(false);
			return 0;
		}
		LOG_TRACE_EX(false,("更新玩家(%ld)资源4 db_update (ResByPlayer,ResByPlayer)  Error!\n",lPlayerID));
		return 0;
	}
	return 1;
}
//玩家的兵种数
void CUserInfo::OnCitySoldier(int handle,unsigned long lMapID,unsigned long CityID, unsigned long lFrom, unsigned long lPlayerID)
{
	PlayerSoldierRace SoldierRace;
	memset(&SoldierRace, 0, sizeof(SoldierRace));

	SoldierRace.setmasterid(lMapID, CityID);
	SoldierRace.setdetailid(lPlayerID);
	int iRet = g_Proxy.db_select(SoldierRace);
	if ( iRet == DB_Proxy::DB_FAILED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, lFrom, lPlayerID);
		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ;
	}
	else if(iRet == DB_Proxy::DB_EOF)
	{
	}

	PLAYER_SOLDIER_LIST SoldierList;
	SoldierList.serialize(SoldierRace.human_, SoldierRace.fairy_, SoldierRace.mannikin_, SoldierRace.dandiprat_,
		SoldierRace.halfanimal_, SoldierRace.cattle_, SoldierRace.blackfairy_, SoldierRace.decpesrson_, lMapID, lPlayerID);
	int ret = asynch_write(handle, (char *)&SoldierList, SoldierList.length);
}
//更新玩家的兵种数
void CUserInfo::OnUpDataSoldier (int handle, PLAYER_SOLDIER_LIST & Soldier)
{
}
//武器列表
void CUserInfo::OnWeaponList(int handle,unsigned long lMapID,unsigned long CityID, unsigned long lFrom, unsigned long lPlayerID)
{
	int count = 0,count1 = 0,count2 = 0;
	int Item = 0, Item1 = 0, Item2 = 0;
	char *Buffer=0,*Buffer1=0,*Buffer2=0;
	ProductionAmount Weapon,AnimalItem,SpecialItem;
	memset(&Weapon, 0, sizeof(ProductionAmount));
	memset(&AnimalItem, 0, sizeof(ProductionAmount));
	memset(&SpecialItem, 0, sizeof(ProductionAmount));
	Weapon.setmasterid(lMapID, CityID, lPlayerID,WEAPON_ITEM);

	int dbRet = g_Proxy.db_multi_select(Weapon, Buffer, count);
	if ( dbRet == DB_Proxy::DB_EOF) 
	{
		/*delete by xujincai 20081228 当没有武器时，无法返回特殊工具的问题
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATA_ERROR, lFrom, lPlayerID);
		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ;
		*/
	}
	else if (dbRet == DB_Proxy::DB_FAILED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, lFrom, lPlayerID);
		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ;
	}
	Item = count/sizeof(ProductionAmount);
	ProductionAmount *pProAmount = (ProductionAmount *)Buffer;

	AnimalItem.setmasterid(lMapID, CityID, lPlayerID,ANIMALS_TAME_ITEM);
	if (g_Proxy.db_multi_select(AnimalItem, Buffer1, count1) != DB_Proxy::DB_SUCCEED) 
	{
	}
	SpecialItem.setmasterid(lMapID, CityID, lPlayerID,SPECIAL_TOOL_ITEM);
	if (g_Proxy.db_multi_select(SpecialItem, Buffer2, count2) != DB_Proxy::DB_SUCCEED) 
	{
	}
	Item1 = count1/sizeof(ProductionAmount);
	Item2 = count2/sizeof(ProductionAmount);
	
	char * tmp = new char[sizeof(WEAPON_LIST) + sizeof(tagItem_List)*Item + sizeof(tagItem_List)*Item1 + sizeof(tagItem_List)*Item2];
	WEAPON_LIST *pWeaponList = (WEAPON_LIST *)tmp;

	int n = 0,i = 0;
	for (n = 0; n < Item; n++)
	{
		if (pProAmount[n].productionid_ < 1 || pProAmount[n].productionid_ >= 100 || pProAmount[n].productioncount_ <= 0) continue;
		(*pWeaponList)[i].lMapID = lMapID;
		(*pWeaponList)[i].lCityID = CityID;
		(*pWeaponList)[i].iItemID = pProAmount[n].productionid_;
		(*pWeaponList)[i].iType = WEAPON_ITEM;
		(*pWeaponList)[i].iCount = pProAmount[n].productioncount_;

		i++;
	}
	pProAmount = 0;
	pProAmount = (ProductionAmount *)Buffer1;

	for (n = 0; n < Item1; n++)
	{
		if (pProAmount[n].productionid_ < 100 || pProAmount[n].productionid_ >= 150 || pProAmount[n].productioncount_ <= 0) continue;
		(*pWeaponList)[i].lMapID = lMapID;
		(*pWeaponList)[i].lCityID = CityID;
		(*pWeaponList)[i].iItemID = pProAmount[n].productionid_;
		(*pWeaponList)[i].iType = ANIMALS_TAME_ITEM;
		(*pWeaponList)[i].iCount = pProAmount[n].productioncount_ ;

		i++;
	}
	pProAmount = 0;
	pProAmount = (ProductionAmount *)Buffer2;
	
	for (n = 0; n < Item2; n++)
	{
		if (pProAmount[n].productionid_ < 150 || pProAmount[n].productionid_ > 199 || pProAmount[n].productioncount_ <= 0) continue;
		(*pWeaponList)[i].lMapID = lMapID;
		(*pWeaponList)[i].lCityID = CityID;
		(*pWeaponList)[i].iItemID = pProAmount[n].productionid_;
		(*pWeaponList)[i].iType = SPECIAL_TOOL_ITEM;
		(*pWeaponList)[i].iCount = pProAmount[n].productioncount_ ;

		i++;
	}
	pWeaponList->serialize(i, lFrom, lPlayerID, EN_S_WEAPON_LIST);
	int ret = asynch_write(handle, (char *)pWeaponList, pWeaponList->length);

	if (Buffer != NULL)
	{
		delete [] Buffer;
		Buffer = 0;
	}
	if (Buffer1 != NULL)
	{
		delete [] Buffer1;
		Buffer1 = 0;
	}
	if (Buffer2 != NULL)
	{
		delete [] Buffer2;
		Buffer2 = 0;
	}
	if (tmp != NULL)
	{
		delete [] tmp;
		tmp = 0;
	}
}
//防具列表
void CUserInfo::OnDefendList(int handle,unsigned long lMapID,unsigned long CityID, unsigned long lFrom, unsigned long lPlayerID)
{
	int iSendSize = 0, count = 0;
	char *Buffer;
	ProductionAmount Defend;
	memset(&Defend, 0, sizeof(Defend));
	Defend.setmasterid(lMapID, CityID, lPlayerID,DEFEND_ITEM);
	int dbRet = g_Proxy.db_multi_select(Defend, Buffer, count);
	if (dbRet == DB_Proxy::DB_EOF) 
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATA_ERROR, lFrom, lPlayerID);
		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ;
	}
	else if (dbRet == DB_Proxy::DB_FAILED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, lFrom, lPlayerID);
		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ;
	}
	int Item = count/sizeof(ProductionAmount);

	ProductionAmount *pDefend = (ProductionAmount *)Buffer;

	char * tmp = new char[sizeof(DEFEND_LIST) + sizeof(tagItem_List)*Item];
	DEFEND_LIST *pDefendList = (DEFEND_LIST *)tmp;

	int j = 0,i = 0;
	for (; j < Item; j++)
	{
		if (pDefend[j].productionid_ < 200 || pDefend[j].productionid_ > 250 || pDefend[j].productioncount_ <= 0) continue;

		(*pDefendList)[i].lMapID = lMapID;
		(*pDefendList)[i].lCityID = CityID;
		(*pDefendList)[i].iItemID = pDefend[j].productionid_;
		(*pDefendList)[i].iType = DEFEND_ITEM;
		(*pDefendList)[i].iCount = pDefend[j].productioncount_;
		i++;
	}
	pDefendList->serialize(i, lFrom, lPlayerID, EN_S_DEFEND_LIST);
	int ret = asynch_write(handle, (char *)pDefendList, pDefendList->length);

	if (Buffer != NULL)
	{
		delete [] Buffer;
		Buffer = 0;
	}
	if(tmp)
	{
		delete [] tmp;
		tmp =0 ;
	}
}
//得到英雄装备
void CUserInfo::OnGetHeroEquip(int handle,unsigned long lFrom, unsigned long lPlayerID)
{
	char * buf=0;
	int count=0,iItem=0;
	PlayerItemStorageTable HeroPack;

	HeroPack.setmasterid(1, lPlayerID);
	if (g_Proxy.db_multi_select(HeroPack, buf, count) == DB_Proxy::DB_SUCCEED)
	{
		iItem = count/sizeof(PlayerItemStorageTable);
		PlayerItemStorageTable * pHeroPack = (PlayerItemStorageTable *)buf;

		char * tmp = new char[sizeof(HERO_EQUIP_LIST) + sizeof(tagHeroEquipList)*iItem];
		HERO_EQUIP_LIST *pHeroPackList = (HERO_EQUIP_LIST *)tmp;
		int n =0;
		for (n= 0; n < iItem; n++)
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
		pHeroPackList->serialize(n, lFrom, lPlayerID, EN_S_MARKET_OFFICER_ITEM_LIST);
		asynch_write(handle, (char *)pHeroPackList, pHeroPackList->length);
		if (tmp != NULL)
		{
			delete [] tmp;
			tmp = 0;
		}
	}
	if (buf != NULL)
	{
		delete [] buf;
		buf = 0;
	}
}
//更新道具
void CUserInfo::OnUpDataItem(int handle,unsigned long lMapID,unsigned long lCityID, int iType, int ItemID, int Level, int Count, unsigned long lFrom, unsigned long lPlayerID, int iCost, int iMode)
{
	int ret = 0;
	MARKET_INFO MarketInfo;

	if (Count <1 )
	{
		MarketInfo.serialize(iType, ItemID, Count, EN_GOODS_COUNT_ERROR, lFrom, lPlayerID);
		ret = asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
		return;
	}
	int iRet = 0;
	int iFlag = 0;
	int iCode = 0;
	//ERROR_INFO ErrorInfo;
	ProductionAmount prodItem;
	PlayerResource playerRes;
	PlayerChatRoomTable playerMoney;

	if(iMode == 1)
	{//卖,减少数量
		Count = -Count;
		iCode = EN_GOODS_SALE_FAILED;
	}
	else
	{//买
		iCode = EN_GOODS_BUY_FAILED;
	}
	int oldCount = 0;
	prodItem.setmasterid(lMapID, lCityID, lPlayerID, iType);
	prodItem.setdetailid(ItemID);
	iRet = g_Proxy.db_select(prodItem);
	if (iRet == DB_Proxy::DB_FAILED)
	{
		MarketInfo.serialize(iType, ItemID, Count, iCode, lFrom, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
		return;
	}

	if (iCost < 0) 
		iCost = 100;
	oldCount = prodItem.productioncount_;

	if(Count >0)
	{//需要判断容量,代表是买
		playerRes.setmasterid(lCityID, lMapID);
		playerRes.setdetailid(lPlayerID);
		if(g_Proxy.db_select(playerRes) != DB_Proxy::DB_SUCCEED)
		{
			MarketInfo.serialize(iType, ItemID, Count, iCode, lFrom, lPlayerID);
			asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
			return;
		}

		if(Count > playerRes.content_)
		{
			MarketInfo.serialize(iType, ItemID, Count, iCode, lFrom, lPlayerID);
			asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
			return;
		}

		memset(&prodItem,0,sizeof(ProductionAmount));
		prodItem.setmasterid(lMapID, lCityID, lPlayerID, iType);
		prodItem.setdetailid(ItemID);
		prodItem.productioncount_= Count;
		if(iRet == DB_Proxy::DB_EOF)
		{//新增
			if(g_Proxy.db_insert(prodItem) == DB_Proxy::DB_SUCCEED)
				iFlag = 1;
		}
		else
		{//修改
			if(g_Proxy.db_increase(prodItem) == DB_Proxy::DB_SUCCEED)
				iFlag = 1;
		}
		
	}
	else if(iRet == DB_Proxy::DB_SUCCEED && Count + prodItem.productioncount_ >=0)
	{//卖

		memset(&prodItem,0,sizeof(ProductionAmount));
		prodItem.setmasterid(lMapID, lCityID, lPlayerID, iType);
		prodItem.setdetailid(ItemID);
		prodItem.productioncount_= Count;

		if(g_Proxy.db_increase(prodItem) == DB_Proxy::DB_SUCCEED)
				iFlag = 1;
	}
	//修改仓库容量、金钱
	if(iFlag == 1)
	{
		PlayerOfficerTable officer;
		officer.setmasterid(lPlayerID);
		officer.setdetailid(lPlayerID);
		g_Proxy.db_select(officer);
		memset(&playerRes,0,sizeof(PlayerResource));
		playerRes.setmasterid(lCityID, lMapID);
		playerRes.setdetailid(lPlayerID);
		playerRes.content_ = -Count;
		g_Proxy.db_increase(playerRes);
		//

		playerMoney.setid(lPlayerID);
		g_Proxy.db_select(playerMoney);
		int oldmoney = playerMoney.money_;
		memset(&playerMoney,0,sizeof(playerMoney));
		playerMoney.setid(lPlayerID);
		playerMoney.money_ = -Count * iCost;
		g_Proxy.db_increase(playerMoney);
		if(iMode == 1)
		{//卖,减少数量

			if (Count <0)
			{
				Count = -Count;
			}
			iCode = EN_GOODS_SALE_SUCCEED;
			ISystemLog::getInstance()->playerProduce_SaleCountry_Log(officer.playerid_,officer.officername_,
				lMapID,lCityID,iType,ItemID,oldCount,prodItem.productioncount_,Count);
			ISystemLog::getInstance()->playerMoney_SaleArmyEquipCountry_Log(officer.playerid_,officer.officername_,
				oldmoney,playerMoney.money_,playerMoney.money_ - oldmoney,
				prodItem.mapid_,prodItem.cityid_,prodItem.type_,prodItem.productionid_,Count);
		}
		else
		{//买
			iCode = EN_GOODS_BUY_SUCCEED;
		}

	}

	MarketInfo.serialize(iType, ItemID, Count, iCode, lFrom, lPlayerID);
	asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
	return;
}
//更新道具
int CUserInfo::OnUpDataItem(int ItemID, int iType,int Count, unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID,int &oldcount,int &curcount)
{	
	if(Count ==0)
		return 0;

	ProductionAmount prodItem;
	PlayerResource playerRes;
	int iRet = 0;
	int iFlag = 0;

	prodItem.setmasterid(lMapID, lCityID, lPlayerID, iType);
	prodItem.setdetailid(ItemID);
	iRet = g_Proxy.db_select(prodItem);
	if(iRet == DB_Proxy::DB_FAILED)
		return 0;
	oldcount = prodItem.productioncount_;

	if(Count>0)
	{//需要判断仓库容量		
		playerRes.setmasterid(lCityID, lMapID);
		playerRes.setdetailid(lPlayerID);
		if(g_Proxy.db_select(playerRes) != DB_Proxy::DB_SUCCEED)
			return 0;

		if(Count > playerRes.content_)
			return 3;

		memset(&prodItem,0,sizeof(ProductionAmount));
		prodItem.setmasterid(lMapID, lCityID, lPlayerID, iType);
		prodItem.setdetailid(ItemID);
		prodItem.productioncount_= Count;
		if(iRet == DB_Proxy::DB_EOF)
		{//新增
			if(g_Proxy.db_insert(prodItem) == DB_Proxy::DB_SUCCEED)
				iFlag = 1;
		}
		else
		{//修改
			if(g_Proxy.db_increase(prodItem) == DB_Proxy::DB_SUCCEED)
				iFlag = 1;
		}
	}
	else
	{//减少
		if(iRet != DB_Proxy::DB_SUCCEED || Count + prodItem.productioncount_ <0)
			return 2;//库存不够减少

		memset(&prodItem,0,sizeof(ProductionAmount));
		prodItem.setmasterid(lMapID, lCityID, lPlayerID, iType);
		prodItem.setdetailid(ItemID);
		prodItem.productioncount_= Count;

		if(g_Proxy.db_increase(prodItem) == DB_Proxy::DB_SUCCEED)
				iFlag = 1;
	}

	//修改仓库容量
	if(iFlag == 1)
	{
		curcount = prodItem.productioncount_;
		memset(&playerRes,0,sizeof(PlayerResource));
		playerRes.setmasterid(lCityID, lMapID);
		playerRes.setdetailid(lPlayerID);
		playerRes.content_ = -Count;
		g_Proxy.db_increase(playerRes);
	}

	return iFlag;		
}

void CUserInfo::SetOutPut(int iItem, int iValue)
{
	if (iItem > 3) return;

	m_iOutPut[iItem] = iValue;
}

int64 CUserInfo::GetCurrentTime()
{
	IceUtil::Time t = IceUtil::Time::now();
	//unsigned __int64 lTimeLen = t.toLongDateTime();
	return t.toLongDateTime();
}