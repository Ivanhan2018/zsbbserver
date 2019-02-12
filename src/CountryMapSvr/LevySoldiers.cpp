#include "LevySoldiers.h"
//#include "BuildProduce.h"
#include "log.hpp"
#include "DataServer.h"
#pragma warning(disable:4100)
#pragma warning(disable:4189)
#pragma warning(disable:4127)
#pragma warning(disable:4244)
#pragma warning(disable:4389)
CLevySoldiers::CLevySoldiers()
{
	m_lMapID = 0L;
	memset(m_JobAttri, 0, sizeof(m_JobAttri));
	memset(m_RaceAttri, 0, sizeof(m_RaceAttri));
	
	m_mapLevyingList.clear();
	m_ContentOfCarriage[0] = 400;
	m_ContentOfCarriage[1] = 700;
	m_ContentOfCarriage[2] = 1000;
}
CLevySoldiers::~CLevySoldiers()
{
}
void CLevySoldiers::SetMapID(unsigned long lMapID)
{ 
	m_lMapID = lMapID;
	/*
	if(m_lMapID >1000 && m_lMapID <1100)
	{
		InitLevySoldierActivity();
	}
	*/
}	
//网络处理函数
bool CLevySoldiers::MainLevySoldiers(int handle, void * pDataBuffer, int iDataSize) 
{
	return true;
}
//征兵
int CLevySoldiers::OnLevySoldier(int handle, void * pDataBuffer ,int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize < sizeof(CITY_INFO))
	{
		LOG_TRACE_EX(false,("CLevySoldiers::OnLevySoldier-->消息包非法：玩家%d\n",lPlayerID));
		return 0;
	}

	CITY_INFO *pCityID = (CITY_INFO *)pDataBuffer;
	GameCityInfo * pRaceParam = g_LoadDBinfo.GetCityInfo(pCityID->lCityID);
	if (pRaceParam == NULL) return 0;
	int ret = 0, n = 0;

	char * tmp = new char[sizeof(LEVY_SOLDIER_PARAM) + sizeof(tagLevy_Soldier_Param)*8];
	LEVY_SOLDIER_PARAM * SendData = (LEVY_SOLDIER_PARAM *)tmp;
	
	for (n = 0; n < 8; n++)
	{
		(*SendData)[n].iRaceType = n+1;
		switch(n+1)
		{
		case 1://人类
			(*SendData)[n].iCurrentCount = pRaceParam->human_*100;
			break;
		case 2:
			(*SendData)[n].iCurrentCount = pRaceParam->fairy_*100;
			break;
		case 3:
			(*SendData)[n].iCurrentCount = pRaceParam->mannikin_*100;
			break;
		case 4:
			(*SendData)[n].iCurrentCount = pRaceParam->dandiprat_*100;
			break;
		case 5:
			(*SendData)[n].iCurrentCount = pRaceParam->halfanimal_*100;
			break;
		case 6:
			(*SendData)[n].iCurrentCount = pRaceParam->cattle_*100;
			break;
		case 7:
			(*SendData)[n].iCurrentCount = pRaceParam->blackfairy_*100;
			break;
		case 8:
			(*SendData)[n].iCurrentCount = pRaceParam->decpesrson_*100;
			break;
		};
		//if (pSoldierList[n].iCurrentCount <= 0) pSoldierList[n].iCurrentCount = 100;
	}
	SendData->serialize(n, m_lMapID, lPlayerID);
	ret = asynch_write(handle, (char *)SendData, SendData->length);
	
	OnLevyingSoldiers(handle, pCityID->lMapID, pCityID->lCityID, lPlayerID);
	
	if (tmp != NULL)
	{
		delete [] tmp;
		tmp = 0;
	}

	return ret;
}
//正处于招兵中...
bool CLevySoldiers::OnLevyingSoldiers(int handle, unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID)
{
	//IceUtil::LockT<IceUtil::Mutex> Lock(m_mapLevyingListMutex);
	int ret = 0;
	int iCount =(int) m_mapLevyingList.size();
	if (iCount<=0)
	{	
		return false;
	}
	multimap<unsigned long, tagLevyingSoldiers>::iterator it = m_mapLevyingList.lower_bound(lPlayerID);
	multimap<unsigned long, tagLevyingSoldiers>::iterator endit = m_mapLevyingList.upper_bound(lPlayerID);
	char * tmp = new char[sizeof(LEVYING_SOLDIER) + sizeof(tagLevying_Soldier)*iCount];
	LEVYING_SOLDIER * SendData = (LEVYING_SOLDIER *)tmp;
	int n = 0;
	int64 lTimeLen = IceUtil::Time::now().toSeconds();
	for (; it != endit; it++)
	{
		(*SendData)[n].iRaceID = it->second.iReceID;
		(*SendData)[n].iCount = it->second.iCount;
		if (it->second.lStartTime > 0)
		{
			int iTimeL = int(it->second.lStartTime - lTimeLen);
			(*SendData)[n].iTime = iTimeL;
			(*SendData)[n].iState = 1;
		}
		else
		{
			(*SendData)[n].iTime = it->second.iCount;
			(*SendData)[n].iState = 2;
		}
		if (++n>=iCount) break;
	}
	SendData->serialize(n, lMapID, lPlayerID);
	ret = asynch_write(handle, (char *)SendData, SendData->length);
	if (tmp != NULL)
	{
		delete [] tmp;	
		tmp = 0;
	}

	//m_mapLevyingListMutex.unlock();
	return ret;
}
//征兵数量
bool CLevySoldiers::OnLevySoldierCount(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize < sizeof(LEVY_SOLDIER_COUNT))
	{
		LOG_TRACE_EX(false,("CLevySoldiers::OnLevySoldierCount-->消息包非法：玩家%d\n",lPlayerID));
		return false;
	}

	LEVY_SOLDIER_COUNT *pRace = (LEVY_SOLDIER_COUNT *)pDataBuffer;

	ERROR_INFO ErrorInfo;
	memset(&ErrorInfo,0,sizeof(ERROR_INFO));

	int ret = HaveCasern(handle, lPlayerID, pRace->lMapID, pRace->lCityID);
	if (ret == 0)
	{
		ErrorInfo.serialize(EN_NO_BULID_CASERN, pRace->lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	if(pRace->iRace < 1 || pRace->iRace > 8)
	{
		ErrorInfo.serialize(EN_LEVY_RACE_ID, pRace->lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	if(pRace->iCount < 1 || pRace->iCount > 500)
	{
		ErrorInfo.serialize(EN_LEVY_COUNT, pRace->lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	GameCityInfo * pRaceParam = g_LoadDBinfo.GetCityInfo(pRace->lCityID);
	if (pRaceParam == NULL)
	{
		//_ASSERT(false);
		return 0;
	}
	OfficerRaceAttribute * pRaceAttri = g_LoadDBinfo.GetOfficerRaceAttr(pRace->iRace);
	if (pRaceAttri == NULL)
	{
		//_ASSERT(false);
		return 0;
	}
	int iflag = 0;
	int iTimeL = pRace->iCount*100;
	switch(pRace->iRace)
	{
	case 1://人类
		if (pRaceParam->human_ < 1) return 0;
		iTimeL /= pRaceParam->human_;
		break;
	case 2:
		if (pRaceParam->fairy_ < 1) return 0;
		iTimeL /= pRaceParam->fairy_;
		break;
	case 3:
		if (pRaceParam->mannikin_ < 1) return 0;
		iTimeL /= pRaceParam->mannikin_;
		break;
	case 4:
		if (pRaceParam->dandiprat_ < 1) return 0;
		iTimeL /= pRaceParam->dandiprat_;
		break;
	case 5:
		if (pRaceParam->halfanimal_ < 1) return 0;
		iTimeL /= pRaceParam->halfanimal_;
		break;
	case 6:
		if (pRaceParam->cattle_ < 1) return 0;
		iTimeL /= pRaceParam->cattle_;
		break;
	case 7:
		if (pRaceParam->blackfairy_ < 1) return 0;
		iTimeL /= pRaceParam->blackfairy_;
		break;
	case 8:
		if (pRaceParam->decpesrson_ < 1) return 0;
		iTimeL /= pRaceParam->decpesrson_;
		break;
	};
	if (iTimeL < 1) return 0;
	PALEYER_RESOURCE PlayerRes;
	memset(&PlayerRes, 0, sizeof(PALEYER_RESOURCE));

	int iNeedFoodCount = pRaceAttri->food_*pRace->iCount;//add by xujincai 20081226 运算一次，避免多次乘法运算
	int iNeedMoney = pRaceAttri->money_*pRace->iCount ;
	m_UserInfo.PlayerResourceCount(0, pRace->lMapID, pRace->lCityID, lPlayerID, PlayerRes, pRace->lMapID, lPlayerID);
	//if(pRaceAttri->food_*pRace->iCount > PlayerRes.iFoodCount)
	if(iNeedFoodCount > PlayerRes.iFoodCount)
	{
		ErrorInfo.serialize(EN_LEVY_FOOD_COUNT, pRace->lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	//if(pRaceAttri->money_*pRace->iCount > PlayerRes.iMoney)
	if(iNeedMoney > PlayerRes.iMoney)
	{
		ErrorInfo.serialize(EN_LEVY_MONEY, pRace->lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	
	PlayerResource UpDataRes;
	memset(&UpDataRes, 0, sizeof(UpDataRes));
//	UpDataRes.money_ = -m_RaceAttri[pRace->iRace].money_ * pRace->iCount;
	UpDataRes.foodcount_ = -iNeedFoodCount;
	//UpDataRes.content_ = UpDataRes.foodcount_;//mondify by xujincai 20081226征兵消耗粮食，库存容量应该增加，而不是减少
	UpDataRes.content_  = iNeedFoodCount;

	UpDataRes.setmasterid(pRace->lCityID, pRace->lMapID);
	UpDataRes.setdetailid(lPlayerID);
	ret = g_Proxy.db_increase(UpDataRes);
	if (ret != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_DATA_ERROR, pRace->lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return 0;
	}
	PlayerChatRoomTable Country;
	memset(&Country, 0, sizeof(PlayerChatRoomTable));
	//Country.money_ -= pRaceAttri->money_*pRace->iCount;del by xujincai 20081226 避免多次乘法运算
	Country.money_ = - iNeedMoney;
	Country.setid(lPlayerID);
	if (g_Proxy.db_increase(Country) != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_DATA_ERROR, pRace->lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return 0;
	}
	//20100409
	PlayerOfficerTable officer;
	officer.setmasterid(lPlayerID);
	officer.setdetailid(lPlayerID);
	g_Proxy.db_select(officer);
	ISystemLog::getInstance()->playerMoney_Draft_Log(officer.playerid_,officer.officername_,
		PlayerRes.iMoney,Country.money_,iNeedMoney,pRace->lMapID,pRace->lCityID,pRace->iRace,pRace->iCount);

	tagLevyingSoldiers Levying;
	Levying.lMapID = pRace->lMapID;
	Levying.lCityID = pRace->lCityID;
	Levying.iReceID = pRace->iRace;
	Levying.iCount = pRace->iCount;
	Levying.lIndex = 0;

	IceUtil::LockT<IceUtil::Mutex> Lock(m_mapLevyingListMutex);
	multimap<unsigned long, tagLevyingSoldiers>::iterator itLevy = m_mapLevyingList.find(lPlayerID);

	if (itLevy != m_mapLevyingList.end())
	{//排队
		Levying.lStartTime = 0;
		iflag = 1;
	}
	else
		Levying.lStartTime = IceUtil::Time::now().toSeconds()+iTimeL;
	

	LevySoldierActivity levysoldier;
	levysoldier.playerid_ = lPlayerID;
	levysoldier.mapid_ = pRace->lMapID;
	levysoldier.cityid_ = pRace->lCityID;
	levysoldier.raceid_ = pRace->iRace;
	levysoldier.count_ = pRace->iCount;
	levysoldier.endtime_ = Levying.lStartTime;
	if(g_Proxy.db_insert(levysoldier) != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_DATA_ERROR, pRace->lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		
		LOG_TRACE_EX(false,("CLevySoldiers::OnLevySoldierCount-->LevySoldierActivity：玩家%d,mapid:%d,cityid:%d,raceid:%d,count:%d,endtime:%ld\n",
			lPlayerID,levysoldier.mapid_,levysoldier.cityid_,levysoldier.raceid_,levysoldier.count_,levysoldier.endtime_));
		return 0;
	}
	Levying.lIndex = levysoldier.sortid_;

	LEVYING_SOLDIER_LIST * pLevying = new LEVYING_SOLDIER_LIST;
	pLevying->serialize(lPlayerID, pRace->lMapID, pRace->lCityID, pRace->iRace, pRace->iCount,levysoldier.sortid_, EN_TIMER_LEVYING);
	long lTimeID = set_timer(iTimeL, (const void *)pLevying);
	pLevying->TimerID = lTimeID;
	Levying.lTimerID = lTimeID;
	m_mapLevyingList.insert(make_pair(lPlayerID, Levying));
	Lock.release();

	LEVY_SOLDIER_INFO LevySoldier;
	if (iflag == 1) 
	{		
		LevySoldier.serialize(pRace->iRace, pRace->iCount, 2, pRace->lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&LevySoldier, LevySoldier.length);
		return ret;
	}
	
	LevySoldier.serialize(pRace->iRace, pRace->iCount, 1, pRace->lMapID, lPlayerID);
	ret = asynch_write(handle, (char *)&LevySoldier, LevySoldier.length);
	return ret;
}
//征兵时间事件
bool CLevySoldiers::OnLevyTimeEvent(void * pDataBuffer, int iDataSize)
{
	if (iDataSize < sizeof(LEVYING_SOLDIER_LIST))
	{
		LOG_TRACE_EX(false,("CLevySoldiers::OnLevyTimeEvent-->消息包非法\n"));
		return false;
	}

	LEVYING_SOLDIER_LIST * pLevying = (LEVYING_SOLDIER_LIST *)pDataBuffer;

	unsigned long lPlayerID = pLevying->lPlayerID;
	unsigned long lMapID = pLevying->lMapID;
	unsigned long lCityID = pLevying->lCityID;
	int iRaceID = pLevying->iReceID;
	int iCount = pLevying->iCount;
	int iSortID = pLevying->iSortID;
	int iflag = 0;

	//modify by xujincai 20090127 加速、立即完成时，避免原定时器做征兵操作
	IceUtil::LockT<IceUtil::Mutex> Lock(m_mapLevyingListMutex);
	multimap<unsigned long, tagLevyingSoldiers>::iterator it = m_mapLevyingList.lower_bound(lPlayerID);
	multimap<unsigned long, tagLevyingSoldiers>::iterator endit = m_mapLevyingList.upper_bound(lPlayerID);

	for (; it != endit; it++)
	{
		if (it->second.lMapID == lMapID && it->second.lCityID == lCityID && it->second.iReceID == iRaceID 
			&& it->second.lStartTime >0 && it->second.iCount == iCount && it->second.lIndex == iSortID)
		{
			m_mapLevyingList.erase(it);
			iflag = 1;
			break;
		}
	}

	Lock.release();
	if(!iflag)
	{//
		return false;
	}
	LevySoldierActivity levysoldier;
	levysoldier.setmasterid(lPlayerID,lMapID,lCityID);
	levysoldier.setdetailid(iSortID);
	if(g_Proxy.db_delete(levysoldier) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("CLevySoldiers::OnLevyTimeEvent-->LevySoldierActivity\n"));
		return false;
	}

	iflag = 0;
	PlayerSoldierRace RaceCount;
	memset(&RaceCount, 0, sizeof(RaceCount));
	
	RaceCount.setmasterid(lMapID, lCityID);
	RaceCount.setdetailid(lPlayerID);
	int dbRet = g_Proxy.db_select(RaceCount);
	if (dbRet == DB_Proxy::DB_EOF)
	{
		memset(&RaceCount, 0, sizeof(RaceCount));
		iflag = 1;
	}
	else if (dbRet == DB_Proxy::DB_FAILED)
	{
		//_ASSERT(false);
		return 0;
	}
	int oldRaceCount =0;
	int curRaceCount = 0;
		
	RaceCount.playerid_ = lPlayerID;
	RaceCount.mapid_ = lMapID;
	RaceCount.cityid_ = lCityID;
	RaceCount.completetimes_ = IceUtil::Time::now().toSeconds();
	switch(iRaceID)
	{
	case 1://人类
		oldRaceCount = RaceCount.human_;
		RaceCount.human_ += iCount;
		curRaceCount = RaceCount.human_;
		break;
	case 2://精灵
		oldRaceCount = RaceCount.fairy_;
		RaceCount.fairy_ += iCount;
		curRaceCount = RaceCount.fairy_;
		break;
	case 3://矮人
		oldRaceCount = RaceCount.mannikin_;
		RaceCount.mannikin_ += iCount;
		curRaceCount = RaceCount.mannikin_;
		break;
	case 4://侏儒
		oldRaceCount = RaceCount.dandiprat_;
		RaceCount.dandiprat_ += iCount;
		curRaceCount = RaceCount.dandiprat_;
		break;
	case 5://半兽人
		oldRaceCount = RaceCount.halfanimal_;
		RaceCount.halfanimal_ += iCount;
		curRaceCount = RaceCount.halfanimal_;
		break;
	case 6://牛头人
		oldRaceCount = RaceCount.cattle_;
		RaceCount.cattle_ += iCount;
		curRaceCount = RaceCount.cattle_;
		break;
	case 7://黑精灵
		oldRaceCount = RaceCount.blackfairy_;
		RaceCount.blackfairy_ += iCount;
		curRaceCount = RaceCount.blackfairy_;
		break;
	case 8://亡灵
		oldRaceCount = RaceCount.decpesrson_;
		RaceCount.decpesrson_ += iCount;
		curRaceCount = RaceCount.decpesrson_;
		break;
	};
		
	RaceCount.setmasterid(lMapID, lCityID);
	RaceCount.setdetailid(lPlayerID);
	if (iflag == 0)
	{
		if (g_Proxy.db_update(RaceCount,RaceCount) == DB_Proxy::DB_SUCCEED)
		{
		}
		else
		{
			LOG_TRACE_EX(false,("更新玩家 %ld 征兵 %d 数量%d db_update(RaceCount,RaceCount) error\n",RaceCount.playerid_,iRaceID,iCount));
		
		}
	}
	else
	{
		if (g_Proxy.db_insert(RaceCount) == DB_Proxy::DB_SUCCEED)
		{
		}
		else
		{
			LOG_TRACE_EX(false,("玩家 %ld 征兵 %d 数量%d db_insert(RaceCount) error \n",RaceCount.playerid_,iRaceID,iCount));
		}
	}
	OnExeNextSoldier(lPlayerID, lMapID, lCityID);

	g_DataServer.broadMsgAndNotify(lPlayerID,lMapID,iRaceID,0,0,EN_S_NOTIFYCONSCRIPTION,EN_S_BROADPRODUCT,lMapID);
	//20100409
	PlayerOfficerTable officer;
	officer.setmasterid(lPlayerID);
	officer.setdetailid(lPlayerID);
	g_Proxy.db_select(officer);
	ISystemLog::getInstance()->playerSoldier_FinDraft_Log(officer.playerid_,officer.officername_,
		lMapID,lCityID,iRaceID,oldRaceCount,curRaceCount,iCount);
	return true;
}
//进行下个种族
void CLevySoldiers::OnExeNextSoldier(unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID)
{
	multimap<unsigned long, tagLevyingSoldiers>::iterator it = m_mapLevyingList.lower_bound(lPlayerID);
	multimap<unsigned long, tagLevyingSoldiers>::iterator endit = m_mapLevyingList.upper_bound(lPlayerID);
	multimap<unsigned long, tagLevyingSoldiers>::iterator tmpiter;
	int sortid = -1;
	
	for (; it != endit; it++)
	{
		if (it->second.lMapID != lMapID || it->second.lCityID != lCityID)
			continue;
		if(sortid == -1)
		{
			sortid = it->second.lIndex;
			tmpiter = it;
		}
		else if(it->second.lIndex <sortid)
		{
			sortid = it->second.lIndex;
			tmpiter = it;
		}
	}
	if(sortid <= -1)
	{//未有
		return;
	}
	GameCityInfo * pRaceParam = g_LoadDBinfo.GetCityInfo(lCityID);
	if (pRaceParam == NULL) return ;
	int iTimeL = tmpiter->second.iCount*100;
	switch(tmpiter->second.iReceID)
	{
	case 1://人类
		iTimeL /= pRaceParam->human_;
		break;
	case 2:
		iTimeL /= pRaceParam->fairy_;
		break;
	case 3:
		iTimeL /= pRaceParam->mannikin_;
		break;
	case 4:
		iTimeL /= pRaceParam->dandiprat_;
		break;
	case 5:
		iTimeL /= pRaceParam->halfanimal_;
		break;
	case 6:
		iTimeL /= pRaceParam->cattle_;
		break;
	case 7:
		iTimeL /= pRaceParam->blackfairy_;
		break;
	case 8:
		iTimeL /= pRaceParam->decpesrson_;
		break;
	};
	tmpiter->second.lStartTime = IceUtil::Time::now().toSeconds()+iTimeL;
	//修改LevySoldierActivity
	LevySoldierActivity levysoldier;
	levysoldier.setmasterid(lPlayerID,tmpiter->second.lMapID, tmpiter->second.lCityID);
	levysoldier.setdetailid(tmpiter->second.lIndex);
	levysoldier.raceid_ = tmpiter->second.iReceID;
	levysoldier.count_ = tmpiter->second.iCount;
	levysoldier.endtime_ = tmpiter->second.lStartTime;
	if(g_Proxy.db_update(levysoldier,levysoldier) != DB_Proxy::DB_SUCCEED)
	{
		//_ASSERT(false);
		return;
	}
	LEVYING_SOLDIER_LIST * pLevying = new LEVYING_SOLDIER_LIST;
	pLevying->serialize(lPlayerID, tmpiter->second.lMapID, tmpiter->second.lCityID, tmpiter->second.iReceID, tmpiter->second.iCount,tmpiter->second.lIndex, EN_TIMER_LEVYING);
	long lTimeID = set_timer(iTimeL, (const void *)pLevying);
	pLevying->TimerID = lTimeID;
	tmpiter->second.lTimerID = lTimeID;

}
//是否建造了兵营
int CLevySoldiers::HaveCasern(int handle,unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID)
{
	char * buf = 0;
	int count = 0;
	int ret = 0,n = 0;
	PlayerConstruction Constr;
	memset(&Constr, 0, sizeof(PlayerConstruction));

	Constr.setmasterid(lMapID, lCityID, lPlayerID);
	int dbRet = g_Proxy.db_multi_select(Constr, buf, count);
	if (dbRet == DB_Proxy::DB_EOF)
	{
		return 0;
	}
	else if (dbRet == DB_Proxy::DB_FAILED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, lMapID, lPlayerID);
		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("bool CBuildProduce::HaveConstruction %ld g_Proxy.db_multi_select(Constr, buf, count) error \n ",lPlayerID));
		return 2;
	}
	int ConCount = count/sizeof(PlayerConstruction);
	PlayerConstruction *pPlayerCon = (PlayerConstruction *)buf;
	//首先需要建造兵营才能征兵
	for (int i=0; i < ConCount; i++)
	{
		if (pPlayerCon[i].constructionid_ == 7)
		{
			//return 1;
			//add by xujincai 20081216
			ret = 1;
			break;
		}
	}
	if (buf != NULL)
	{
		delete [] buf;
		buf = 0;
	}
	return ret;
}
//部队创建
bool CLevySoldiers::OnArmyCreate(int handle, void * pDataBuffer ,int iDataSize, unsigned long lPlayerID)
{
	CITY_INFO * pCity = (CITY_INFO *)pDataBuffer;
	
	//空闲指挥官列表
	OnOfficerList(handle, lPlayerID);
	//玩家的兵种数
	m_UserInfo.OnCitySoldier(handle, pCity->lMapID, pCity->lCityID, pCity->lMapID, lPlayerID);
	//武器列表
	m_UserInfo.OnWeaponList(handle, pCity->lMapID, pCity->lCityID, pCity->lMapID, lPlayerID);
	m_UserInfo.OnDefendList(handle, pCity->lMapID, pCity->lCityID, pCity->lMapID, lPlayerID);

	return true;
}
//部队组编
bool CLevySoldiers::OnArmyEdit(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	CITY_INFO * pCity = (CITY_INFO *)pDataBuffer;
	//空闲指挥官列表
	//OnOfficerList(handle, lPlayerID);
	//OnPlayerAllArmys(handle, 1001,3,lPlayerID);
	//玩家的兵种数
	m_UserInfo.OnCitySoldier(handle, pCity->lMapID, pCity->lCityID, pCity->lMapID, lPlayerID);
	//防具列表
	m_UserInfo.OnDefendList(handle, pCity->lMapID, pCity->lCityID, pCity->lMapID, lPlayerID);
	return true;
}
//组编部队
bool CLevySoldiers::OnEditArmy(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID) 
{
	EDIT_ARMY * pEditArmy = (EDIT_ARMY *)pDataBuffer;
	CREATE_ARMY_FAILD Faild;
	int iCount = pEditArmy->iCount;
	bool flag = false;
	int ret = 0;
	int iItemType = WEAPON_ITEM;
	if (g_MapInfo->GetMapState() == war)
	{
		Faild.serialize(EN_MAP_INWAR, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		return false;
	}
	
	ArmyTable Army;
	memset(&Army, 0, sizeof(ArmyTable));
	Army.setmasterid(m_lMapID);
	Army.setdetailid(pEditArmy->iArmyID);
	if (g_Proxy.db_select(Army) != DB_Proxy::DB_SUCCEED)
	{
		printf("g_Proxy.db_select(Army) != DB_Proxy::DB_SUCCEED");
		return false;
	}
	//add by xujincai 20081226 增加武器类型的判断，修改特殊部队的重组失败问题
	if(Army.fweapon_ >=150)
	{
		iItemType = SPECIAL_TOOL_ITEM;
	}
	else if(Army.fweapon_ >= 100 && Army.fweapon_ < 150)
	{
		iItemType = ANIMALS_TAME_ITEM;
	}

	if(Army.firstcount_ >=1 && (Army.fweapon_ ==150 ||Army.fweapon_ ==151 || Army.fweapon_ ==152))
	{
		Faild.serialize(5, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		return false;
	}

	//数量超过上下限
	if ((Army.firstcount_+iCount) > 100 || iCount < 1)
	{
		Faild.serialize(5, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		return false;
	}

	//种族数量
	flag = false;
	PlayerSoldierRace RaceCount;
	memset(&RaceCount, 0, sizeof(RaceCount));
	RaceCount.setmasterid(m_lMapID, pEditArmy->lCityID);
	RaceCount.setdetailid(lPlayerID);
	if (g_Proxy.db_select(RaceCount) != DB_Proxy::DB_SUCCEED)
	{
		return false;
	}
	int oldRace = 0,curRace = 0;
	switch(Army.raceid_)
	{
	case 1:
		if (RaceCount.human_ < iCount)
			flag = true;
		oldRace = RaceCount.human_ ;
		RaceCount.human_ -= iCount;
		curRace = RaceCount.human_ ;
		break;
	case 2:
		if (RaceCount.fairy_ < iCount)
			flag = true;
		oldRace = RaceCount.fairy_ ;
		RaceCount.fairy_ -= iCount;
		curRace = RaceCount.fairy_ ;
		break;
	case 3:
		if (RaceCount.mannikin_ < iCount)
			flag = true;
		oldRace = RaceCount.mannikin_ ;
		RaceCount.mannikin_ -= iCount;
		curRace = RaceCount.mannikin_ ;
		break;
	case 4:
		if (RaceCount.dandiprat_ < iCount)
			flag = true;
		oldRace = RaceCount.dandiprat_ ;
		RaceCount.dandiprat_ -= iCount;
		curRace = RaceCount.dandiprat_ ;
		break;
	case 5:
		if (RaceCount.halfanimal_ < iCount)
			flag = true;
		oldRace = RaceCount.halfanimal_ ;
		RaceCount.halfanimal_ -= iCount;
		curRace = RaceCount.halfanimal_ ;
		break;
	case 6:
		if (RaceCount.cattle_ < iCount)
			flag = true;
		oldRace = RaceCount.cattle_ ;
		RaceCount.cattle_ -= iCount;
		curRace = RaceCount.cattle_ ;
		break;
	case 7:
		if (RaceCount.blackfairy_ < iCount)
			flag = true;
		oldRace = RaceCount.blackfairy_ ;
		RaceCount.blackfairy_ -= iCount;
		curRace = RaceCount.blackfairy_ ;
		break;
	case 8:
		if (RaceCount.decpesrson_ < iCount)
			flag = true;
		oldRace = RaceCount.decpesrson_ ;
		RaceCount.decpesrson_ -= iCount;
		curRace = RaceCount.decpesrson_ ;
		break;
	};
	if (flag)
	{
		Faild.serialize(1, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		return false;
	}
	//add by xujincai 20081219
	ProductionAmount ItemInfo;
	PlayerResource CityContent;
	CityContent.setmasterid(pEditArmy->lCityID,m_lMapID);
	CityContent.setdetailid(lPlayerID);
	//判断武器数量
	ItemInfo.setmasterid(m_lMapID,pEditArmy->lCityID,lPlayerID,iItemType);//modify by xujincai 20081226
	ItemInfo.setdetailid(Army.fweapon_);
	if(g_Proxy.db_select(ItemInfo) != DB_Proxy::DB_SUCCEED)
	{
		Faild.serialize(2, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		return false;
	}
	if(ItemInfo.productioncount_ < iCount)
	{
		Faild.serialize(2, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		return false;
	}
	//判断防具数量,防具也不能改变
	if(Army.fdefendtype_ >0)
	{
		memset(&ItemInfo,0,sizeof(ItemInfo));
		ItemInfo.setmasterid(m_lMapID,pEditArmy->lCityID,lPlayerID,DEFEND_ITEM);
		ItemInfo.setdetailid(Army.fdefendtype_);
		if(g_Proxy.db_select(ItemInfo) != DB_Proxy::DB_SUCCEED)
		{
			Faild.serialize(3, m_lMapID, lPlayerID);
			ret = asynch_write(handle, (char *)&Faild, Faild.length);
			return false;
		}

		if(ItemInfo.productioncount_ < iCount)
		{
			Faild.serialize(3, m_lMapID, lPlayerID);
			ret = asynch_write(handle, (char *)&Faild, Faild.length);
			return false;
		}
	}
	//20100412
	PlayerOfficerTable officer;
	officer.setmasterid(lPlayerID);
	officer.setdetailid(lPlayerID);
	g_Proxy.db_select(officer);

	//减少玩家兵的数量RaceCount;
	RaceCount.setmasterid(m_lMapID, pEditArmy->lCityID);
	RaceCount.setdetailid(lPlayerID);
	if (g_Proxy.db_update(RaceCount,RaceCount) != DB_Proxy::DB_SUCCEED)
	{
		printf("种族数量g_Proxy.db_update(RaceCount) != DB_Proxy::DB_SUCCEED");
		return false;
	}
	ISystemLog::getInstance()->playerSoldier_Rearmy_Log(officer.playerid_,officer.officerid_,officer.officername_,
		m_lMapID,pEditArmy->lCityID,Army.raceid_,oldRace,curRace,iCount);

	//减少玩家武器数量
	memset(&ItemInfo,0,sizeof(ItemInfo));
	ItemInfo.setmasterid(m_lMapID,pEditArmy->lCityID,lPlayerID,iItemType);
	ItemInfo.setdetailid(Army.fweapon_);
	ItemInfo.productioncount_ = - iCount;
	if (g_Proxy.db_increase(ItemInfo) != DB_Proxy::DB_SUCCEED)
	{
		printf("减少玩家武器数量g_Proxy.db_update(RaceCount) != DB_Proxy::DB_SUCCEED");
		return false;
	}
	ISystemLog::getInstance()->playerProduce_Rearmy_Log(officer.playerid_,officer.officerid_,officer.officername_,
		m_lMapID,pEditArmy->lCityID,iItemType,Army.fweapon_,ItemInfo.productioncount_ + iCount,ItemInfo.productioncount_,iCount);
	CityContent.content_ = iCount;
	//减少玩家防具数量
	if(Army.fdefendtype_ >0)
	{
		memset(&ItemInfo,0,sizeof(ItemInfo));
		ItemInfo.setmasterid(m_lMapID,pEditArmy->lCityID,lPlayerID,DEFEND_ITEM);
		ItemInfo.setdetailid(Army.fdefendtype_);
		ItemInfo.productioncount_ = - iCount;

		if (g_Proxy.db_increase(ItemInfo) != DB_Proxy::DB_SUCCEED)
		{
			return false;
		}
		CityContent.content_ += iCount;
		ISystemLog::getInstance()->playerProduce_Rearmy_Log(officer.playerid_,officer.officerid_,officer.officername_,
			m_lMapID,pEditArmy->lCityID,DEFEND_ITEM,Army.fdefendtype_,ItemInfo.productioncount_ + iCount,ItemInfo.productioncount_,iCount);

	}
	//增加玩家库存
	if(g_Proxy.db_increase(CityContent) != DB_Proxy::DB_SUCCEED)
	{
		printf("增加玩家库存g_Proxy.db_update(RaceCount) != DB_Proxy::DB_SUCCEED");
		return false;
	}
	//修改部队信息
	if(pEditArmy->szName[0] != 0)
	{
		strncpy(Army.armyname_,pEditArmy->szName,SIZE_32);
	}
	Army.fdefendtype_ = pEditArmy->iDefendID;
	//Army.fdefendlevel_ = 0;
	Army.firstcount_ += iCount;

	Army.setmasterid(m_lMapID);
	Army.setdetailid(pEditArmy->iArmyID);
	if (g_Proxy.db_update(Army,Army)!= DB_Proxy::DB_SUCCEED)
	{
		printf("组编部队2 g_Proxy.db_update(Army) error\n");
		return false;
	}
	//ModifyArmyInfo(lPlayerID,pEditArmy->iArmyID,pEditArmy->iCount,pEditArmy->iDefendID);  // mouze

	return true;
}
//城市内查询玩家的所有部队或指挥官
void CLevySoldiers::OnPlayerAllArmys(int handle, unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID) 
{
	char * Buffer;
	int count = 0;
	ArmyTable Army;
	memset(&Army, 0, sizeof(ArmyTable));

	Army.setmasterid(lMapID);
	if (g_Proxy.db_multi_select(Army, Buffer, count) != DB_Proxy::DB_SUCCEED)
	{
		printf("玩家 %ld ,在 %ld 城市内没有部队!\n",lPlayerID,lCityID);
	}
	int Item = count/sizeof(ArmyTable);
	ArmyTable *pArmys = (ArmyTable *)Buffer;
	
	char * tmp = new char[sizeof(MAP_ARMY_LIST) + sizeof(tagArmysList)*Item];
	MAP_ARMY_LIST *pArmysList = (MAP_ARMY_LIST *)tmp;
	
	pArmysList->serialize(Item, SERVER_BOUND_MAP_OTHER, lPlayerID, EN_S_ARMYS_LIST);
	for (int n = 0; n < Item; n++)
	{
		(*pArmysList)[n].lPlayerID = pArmys[n].playerid_;
		(*pArmysList)[n].lArmyID = pArmys[n].armyid_;
		(*pArmysList)[n].lOfficerID = pArmys[n].officerid_;
		(*pArmysList)[n].iRaceID = pArmys[n].raceid_;
		(*pArmysList)[n].iJobID = pArmys[n].soldiertype_;
		(*pArmysList)[n].iX = pArmys[n].x;
		(*pArmysList)[n].iY = pArmys[n].y;
		(*pArmysList)[n].iDirection = pArmys[n].direction;
		(*pArmysList)[n].iState = pArmys[n].state_;
	}
	if (Buffer != NULL)
	{
		delete [] Buffer;
		Buffer = 0;
	}
	if(tmp)
	{
		delete [] tmp;
		tmp = 0;
	}
}
//部队信息
bool CLevySoldiers::OnGetArmyInfo(int handle,void * pDataBuffer ,int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize < sizeof(GET_ARMY_INFO))
	{
		LOG_TRACE_EX(false,("CLevySoldiers::OnGetArmyInfo-->消息包非法：玩家%d\n",lPlayerID));
		return 0;
	}
	
	GET_ARMY_INFO *pArmyID = (GET_ARMY_INFO *)pDataBuffer;
	unsigned long lMapID = pArmyID->lMapID;
	unsigned long lArmyID = pArmyID->lArmyID;
	int ret = 0,x = 0;
	ArmyTable Army;
	memset(&Army, 0, sizeof(ArmyTable));
	Army.setmasterid(lMapID);
	Army.setdetailid(lArmyID);
	if (g_Proxy.db_select(Army)!= DB_Proxy::DB_SUCCEED)
	{
	}
	char * tmp = new char[sizeof(MAP_ARMY_LIST) + sizeof(tagArmysList)];
	MAP_ARMY_LIST * pArmysList = (MAP_ARMY_LIST *)tmp;

	PlayerOfficerTable Officer;
	memset(&Officer, 0, sizeof(PlayerOfficerTable));
	Officer.setmasterid(lPlayerID);
	Officer.setdetailid(Army.officerid_);
	if (g_Proxy.db_select(Officer)!= DB_Proxy::DB_SUCCEED)
	{
		printf("玩家 %ld 在郡内 %ld 城市中 的部队 %ld and 指挥官 %ld faild\n",lPlayerID, lMapID, lArmyID,Army.officerid_);
	}
	if (Army.y%2)
	{
		x = (Army.x+1)/2;
	}
	else
	{
		x = (Army.x+2)/2;
	}
	int iState = Army.state_>>6;
	if (iState > 0) iState = iState - 1;
	(*pArmysList)[0].lMapID = lMapID;
	(*pArmysList)[0].lArmyID = lArmyID;
	(*pArmysList)[0].lOfficerID = Army.officerid_;
	(*pArmysList)[0].lPlayerID = Army.playerid_;
	(*pArmysList)[0].iRaceID = Army.raceid_;
	(*pArmysList)[0].iJobID = Army.soldiertype_;
	(*pArmysList)[0].iX = Army.y;					//跟客户端对应 格子坐标
	(*pArmysList)[0].iY = x;
	(*pArmysList)[0].iCount = Army.firstcount_;
	(*pArmysList)[0].iDirection = Army.direction;
	(*pArmysList)[0].iState = iState;
	(*pArmysList)[0].iWeaponID = Army.fweapon_;
	(*pArmysList)[0].iDefendID = Army.fdefendtype_;
	(*pArmysList)[0].iWeaponLevel = Army.fweaponlevel_;
	(*pArmysList)[0].iDefendLevel = Army.fdefendlevel_;
	(*pArmysList)[0].iExp = 100;

	(*pArmysList)[0].iFace = Officer.faceid_;
	strcpy((*pArmysList)[0].szArmyName, Army.armyname_);
	strcpy((*pArmysList)[0].szOfficerName, Officer.officername_);

	pArmysList->serialize(1, m_lMapID, lPlayerID, EN_S_SEND_ARMY_INFO);
	ret = asynch_write(handle, (char *)pArmysList, pArmysList->length);
	printf("部队信息 %ld,X: %d Y: %d X:%d\n",Army.armyid_,x,Army.y,Army.x);

	if(tmp)
	{
		delete [] tmp;
		tmp = 0;
	}
	return ret;
}
//空闲指挥官列表
void CLevySoldiers::OnOfficerList(int handle, unsigned long lPlayerID)
{
	char * Buffer = 0;
	int iCount = 0 ,iItem = 0,Index = 0;
	PlayerOfficerTable Officer;
	memset(&Officer, 0, sizeof(Officer));

	Officer.setmasterid(lPlayerID);
	if (g_Proxy.db_multi_select(Officer, Buffer, iCount)!= DB_Proxy::DB_SUCCEED)
	{
	}

	iItem = iCount/sizeof(PlayerOfficerTable);


	PlayerOfficerTable * pOfficer = (PlayerOfficerTable *)Buffer;
	
	char * tmp = new char[sizeof(PLAYER_OFFICER_LIST) + sizeof(tagPlayer_Officer_List)*iItem];
	PLAYER_OFFICER_LIST *pOfficerList = (PLAYER_OFFICER_LIST *)tmp;

	//指挥官列表信息
	for (int n = 0; n < iItem; n++)
	{		
		if (pOfficer[n].state_ > 0) continue;
		(*pOfficerList)[Index].lOfficerID = pOfficer[n].officerid_;
		(*pOfficerList)[Index].iLevel = pOfficer[n].level_;
		(*pOfficerList)[Index].iRaceID = pOfficer[n].officerrace_;
		(*pOfficerList)[Index].iJobID = pOfficer[n].jobid_;
		(*pOfficerList)[Index].iMilitaryRank = pOfficer[n].militaryrank_;

		(*pOfficerList)[Index].iState = 0;//pOfficer[n].state_;
		(*pOfficerList)[Index].iTechDon = 5+pOfficer[n].level_;
		strcpy((*pOfficerList)[Index].szOfficerName, pOfficer[n].officername_);
		Index++;
	}
	pOfficerList->serialize(Index, m_lMapID, lPlayerID, EN_S_FREE_OFFICERS);

	int ret = asynch_write(handle, (char *)pOfficerList, pOfficerList->length);

	if (Buffer != NULL)
	{
		delete [] Buffer;
		Buffer = 0;
	}
	if(tmp)
	{
		delete [] tmp;
		tmp = 0;
	}
}
//武器列表
void CLevySoldiers::OnWeaponList(int handle,int CityID, unsigned long lPlayerID)
{
	int count = 0;
	char *Buffer;
	ProductionAmount Weapon;
	Weapon.setmasterid(m_lMapID, CityID, lPlayerID,WEAPON_ITEM);
	if (g_Proxy.db_multi_select(Weapon, Buffer, count) != DB_Proxy::DB_SUCCEED)
	{
	}
	int Item = count/sizeof(ProductionAmount);
	ProductionAmount *pProAmount = (ProductionAmount *)Buffer;

	char * tmp = new char[sizeof(WEAPON_LIST) + sizeof(tagItem_List)*Item];
	WEAPON_LIST *pWeaponList = (WEAPON_LIST *)tmp;

	pWeaponList->serialize(Item, m_lMapID, lPlayerID, EN_S_WEAPON_LIST);
	for (int n = 0; n < Item; n++)
	{
		(*pWeaponList)[n].iItemID = pProAmount[n].productionid_;
		(*pWeaponList)[n].iType = 0;//pProAmount[n].level_;
		(*pWeaponList)[n].iCount = pProAmount[n].productioncount_;
	}
	int ret = asynch_write(handle, (char *)pWeaponList, pWeaponList->length);

	if (Buffer != NULL)
	{
		delete [] Buffer;
		Buffer = 0;
	}
	if(tmp)
	{
		delete [] tmp;
		tmp = 0;
	}
}
//防具列表
void CLevySoldiers::OnDefendList(int handle, int CityID, unsigned long lPlayerID)
{
	int iSendSize = 0, count = 0;
	char *Buffer;
	ProductionAmount Defend;
	Defend.setmasterid(m_lMapID, CityID, lPlayerID,DEFEND_ITEM);
	if (g_Proxy.db_multi_select(Defend, Buffer, count) != DB_Proxy::DB_SUCCEED)
	{
	}
	int Item = count/sizeof(ProductionAmount);
	//tagArr_Count ArrCount;
	ProductionAmount *pDefend = (ProductionAmount *)Buffer;

	char * tmp = new char[sizeof(DEFEND_LIST) + sizeof(tagItem_List)*Item];
	DEFEND_LIST *pDefendList = (DEFEND_LIST *)tmp;

	pDefendList->serialize(Item, m_lMapID, lPlayerID, EN_S_DEFEND_LIST);
	for (int j = 0; j < Item; j++)
	{
		(*pDefendList)[j].iItemID = pDefend[j].productionid_;
		(*pDefendList)[j].iType = 0;//pDefend[j].level_;
		(*pDefendList)[j].iCount = pDefend[j].productioncount_;
	}
	int ret = asynch_write(handle, (char *)pDefendList, pDefendList->length);

	if (Buffer != NULL)
	{
		delete [] Buffer;
		Buffer = 0;
	}
	if(tmp)
	{
		delete [] tmp;
		tmp =0;
	}
}
//郡内部队列表信息
void CLevySoldiers::MapOfArmys(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	COMEIN_MAP *pMapID= (COMEIN_MAP *)pDataBuffer;
	PlayerOfficerTable Officer;
	memset(&Officer, 0, sizeof(PlayerOfficerTable));
	Officer.setmasterid(lPlayerID);
	Officer.setdetailid(lPlayerID);
	g_Proxy.db_select(Officer);
	COMEIN_EXIT_MAP tagMap;
	tagMap.serialize(Officer.officername_, pMapID->lMapID, 1, 0, 0, EN_S_BROADCAST_COMEIN_EXIT_MAP);
	broadcast_ex(pMapID->lMapID, (char *)&tagMap, tagMap.length);
}
//郡内哨塔等属性
void CLevySoldiers::MapOfConstruction(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	return;
}
//读取训练兵参数表
void CLevySoldiers::ReadDBExeSoldierParam()
{
}
//读取征兵种族参数表
void CLevySoldiers::ReadRaceParam()
{
	OfficerRaceAttribute RaceAttri;
	char * Buffer = 0;
	int count = 0;
	if (g_Proxy.db_select_all(RaceAttri, Buffer, count) != DB_Proxy::DB_SUCCEED)
	{
		printf("读取征兵种族参数表g_Proxy.db_select_all(RaceAttri, Buffer, count) error !\n");
		return;
	}
	m_iRaceCount = count/sizeof(OfficerRaceAttribute);
	OfficerRaceAttribute *pRaceAttri = (OfficerRaceAttribute *)Buffer;

	for (int n = 0; n < m_iRaceCount; n++)
	{		
		m_RaceAttri[n].raceid_ = pRaceAttri[n].raceid_;
		m_RaceAttri[n].keepfee_ = pRaceAttri[n].keepfee_;
		m_RaceAttri[n].food_  = pRaceAttri[n].food_;
		m_RaceAttri[n].money_ = pRaceAttri[n].money_;
		m_RaceAttri[n].target_ = pRaceAttri[n].target_;
		m_RaceAttri[n].evade_ = pRaceAttri[n].evade_;
		m_RaceAttri[n].hp_ = pRaceAttri[n].hp_;
		m_RaceAttri[n].soldier_ = pRaceAttri[n].soldier_;
		m_RaceAttri[n].huntsman_ = pRaceAttri[n].huntsman_;
		m_RaceAttri[n].prowl_ = pRaceAttri[n].prowl_;
		m_RaceAttri[n].churchman_ = pRaceAttri[n].churchman_;
		m_RaceAttri[n].rabbi_ = pRaceAttri[n].rabbi_;
		m_RaceAttri[n].magician_ = pRaceAttri[n].magician_;
		m_RaceAttri[n].marksman_ = pRaceAttri[n].marksman_;
		m_RaceAttri[n].trooper_ = pRaceAttri[n].trooper_;
	}
	if (Buffer != NULL)
	{
		delete [] Buffer;
		Buffer = 0;
	}
}
//城市信息
void CLevySoldiers::OnCityInfo(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize != sizeof(CITY_INFO))
	{ 
		LOG_TRACE_EX(false,("CLevySoldiers::OnCityInfo-->消息包非法：玩家%d\n",lPlayerID));
		return ;
	}

	CITY_INFO * pCity = (CITY_INFO *)pDataBuffer;
	PlayerChatRoomTable Res;
	memset(&Res, 0, sizeof(Res));
	Res.setid(lPlayerID);
	if (g_Proxy.db_select(Res) != DB_Proxy::DB_SUCCEED)
	{
		printf("城市信息 g_Proxy.db_select(Res) \n");
	}
	MapInfoTable Map_Info;
	memset(&Map_Info, 0, sizeof(Map_Info));
	Map_Info.setid(pCity->lMapID);
	if (g_Proxy.db_select(Map_Info) != DB_Proxy::DB_SUCCEED)
	{
		printf("郡内部队列表信息 g_Proxy.db_select(Map_Info) \n");
	}
	if (Map_Info.countryid_ != Res.country_) return;

	int ret = 0, iflag = 0;
	PlayerCity PCity;
	memset(&PCity, 0, sizeof(PlayerCity));
	PCity.setmasterid(lPlayerID);
	PCity.setdetailid(pCity->lMapID);
	if (g_Proxy.db_select(PCity) == DB_Proxy::DB_SUCCEED)
	{
		if (PCity.cityid_ == pCity->lCityID)
			iflag = 1;
	}
	else
	{
		printf("玩家%ld得到郡内%ld城市中%ld的信息失败\n",lPlayerID,pCity->lMapID,pCity->lCityID);
	}
	//这个城市基本资料信息,征兵信息	
	GameCityInfo * pRaceParam = g_LoadDBinfo.GetCityInfo(pCity->lCityID);
	if (pRaceParam == NULL) return ;
	if (iflag == 0)
	{
		MAP_CITY_INFO City_Info;
		City_Info.serialize(pRaceParam->foodcount_, pRaceParam->woodcount_, pRaceParam->ironcount_, pRaceParam->skincount_, pRaceParam->human_, pRaceParam->mannikin_,pRaceParam->fairy_,pRaceParam->dandiprat_, pRaceParam->halfanimal_,
			pRaceParam->cattle_, pRaceParam->blackfairy_, pRaceParam->decpesrson_, m_lMapID, lPlayerID);

		ret = asynch_write(handle, (char *)&City_Info, City_Info.length);
	}
	else
	{
		//玩家在城市的基本资料信息
		PALEYER_RESOURCE PlayerRes;
		memset(&PlayerRes, 0, sizeof(PALEYER_RESOURCE));
		ret = m_UserInfo.PlayerResourceCount(handle, pCity->lMapID, pCity->lCityID, lPlayerID, PlayerRes, pCity->lMapID, lPlayerID);
		
	}
}
void CLevySoldiers::ReadCityInfo()
{
	
}

int64 CLevySoldiers::GetCurrentTime()
{
	/*time_t t = time( 0 ); 
	tm * localtm = localtime(&t) ;
	char tmp[32]="0",TimeLen[64]="0"; 	
	strftime( tmp, sizeof(tmp), "%Y%m%d",localtm); 
	sprintf(TimeLen,"%s%d%d%d",tmp,localtm->tm_hour, localtm->tm_min,localtm->tm_sec );
	return _atoi64(TimeLen);*/
	return IceUtil::Time::now().toLongDateTime();

}
//训练兵数量
bool CLevySoldiers::OnExeSoldierCount(int handle, void * pDataBuffer ,int iDataSize, unsigned long lPlayerID)
{
	return true;
}
//更新正在生产中的道具时间                                                      
void CLevySoldiers::OnUpDataItem(unsigned long lMapID, int iType)
{
	int64 lTime = GetCurrentTime();
	char * buf = 0;
	int iCount = 0,	iItem = 0;
	ProductionActivity Production;
	memset(&Production, 0, sizeof(ProductionActivity));

	Production.setmasterid(lMapID, iType,0,0);
	if ( g_Proxy.db_multi_select(Production, buf, iCount) == DB_Proxy::DB_SUCCEED)
	{
		iItem = iCount/sizeof(ProductionActivity);
		ProductionActivity * pProduction = (ProductionActivity *)buf;
		for (int n = 0; n < iItem; n++)
		{
			pProduction[n].starttime_ = lTime-pProduction[n].starttime_;
			pProduction[n].setmasterid(pProduction[n].mapid_, iType,pProduction[n].cityid_,pProduction[n].playerid_);
			pProduction[n].setdetailid(pProduction[n].productionid_,pProduction[n].level_,pProduction[n].sortid_);
			if (g_Proxy.db_update(pProduction[n],pProduction[n]) != DB_Proxy::DB_SUCCEED)
			{
				printf("玩家%ld在郡%ld城市中%d生产ID%d等级 %d 没有\n",pProduction[n].playerid_,lMapID,pProduction[n].cityid_, pProduction[n].productionid_, pProduction[n].level_);
			}
		}
	}
	if (buf != NULL)
	{
		delete [] buf;
		buf = 0;
	}
}
//玩家退出此郡
void CLevySoldiers::OnExitMapServer(unsigned long lPlayerID)
{
}
// 装载物资
int CLevySoldiers::OnLoadMaterial(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	MATERIAL_LIST * pList = (MATERIAL_LIST *)pDataBuffer;
	if (sizeof(MATERIAL_LIST)+sizeof(tagMaterials)*pList->iCount != iDataSize)
	{
		LOG_TRACE_EX(false,("CLevySoldiers::OnLoadMaterial-->消息包非法：玩家%d\n",lPlayerID));	
		return 0;
	}

	int ret = 0;
	ERROR_INFO ErrorInfo;
	if (pList->lArmyID <= 0)
	{
		ErrorInfo.serialize(EN_NOT_ARMY_ID, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	//查询部队信息，判断是否为运输部队
	ArmyTable Army;
	Army.setmasterid(pList->lMapID);
	Army.setdetailid(pList->lArmyID);
	if (g_Proxy.db_select(Army) != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_NOT_ARMY_ID, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	if (Army.fweapon_ < 175 || Army.fweapon_ > 177)
	{
		ErrorInfo.serialize(EN_NOT_TRANSIT_ARMY, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	//查询该部队已装载的物资
	LoadMaterial Mat;
	int iTotalCount = Army.firstcount_ * m_ContentOfCarriage[Army.fweapon_ - 175];
	int iAllCount = 0;
	int i = 0;
	for (i = 0; i < pList->iCount; i++)
	{
		iAllCount += (*pList)[i].iCount;
	}

	int count = 0;
	char *buf = 0;
	Mat.setmasterid(lPlayerID, pList->lArmyID);
	if (g_Proxy.db_multi_select(Mat, buf, count) == DB_Proxy::DB_SUCCEED)
	{
		LoadMaterial * pMatList = (LoadMaterial *)buf;
		count = count/sizeof(LoadMaterial);
		for(i = 0;i<count;i++)
		{
			iAllCount += pMatList[i].count_;
		}
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	
	if(iAllCount > iTotalCount)
	{
		ErrorInfo.serialize(EN_ARMY_NO_CONTENT, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;//EN_ARMY_NO_CONTENT
	}
	PlayerResource ResByPlayer;
	memset(&ResByPlayer, 0,sizeof(ResByPlayer));
	ResByPlayer.setmasterid(pList->lCityID, pList->lMapID);
	ResByPlayer.setdetailid(lPlayerID);
	if (g_Proxy.db_select(ResByPlayer) != DB_Proxy::DB_SUCCEED)
	{
		return 0;
	}
	MARKET_INFO MarketInfo;
	//20100409
	PlayerOfficerTable officer;
	officer.setmasterid(lPlayerID);
	officer.setdetailid(Army.officerid_);
	g_Proxy.db_select(officer);
	//ProductionAmount oldProAmount,curProAmount;
	int oldProCount = 0,curProCount = 0;
	for (i = 0; i < pList->iCount; i++)
	{
		switch((*pList)[i].iType)
		{
		case WEAPON_ITEM:				//武器
		case DEFEND_ITEM:				//防具
		case ANIMALS_TAME_ITEM:			//野兽材料
		case SPECIAL_TOOL_ITEM:			//特殊工具

			ret = m_UserInfo.OnUpDataItem((*pList)[i].iMatID, (*pList)[i].iType, -(*pList)[i].iCount, lPlayerID, pList->lMapID, pList->lCityID,oldProCount,curProCount);
			if (ret == 2)
			{
				MarketInfo.serialize((*pList)[i].iType, (*pList)[i].iMatID, (*pList)[i].iCount, EN_GOODS_COUNT, m_lMapID, lPlayerID);
				asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
			}
			else if (ret == 1)
			{
				Mat.setmasterid(lPlayerID, pList->lArmyID);
				Mat.setdetailid((*pList)[i].iType, (*pList)[i].iMatID);
				if (g_Proxy.db_select(Mat) != DB_Proxy::DB_SUCCEED)
				{
					Mat.playerid_ = lPlayerID;
					Mat.armyid_ = pList->lArmyID;
					Mat.type_ = (*pList)[i].iType;
					Mat.matid_ = (*pList)[i].iMatID;
					Mat.count_ = (*pList)[i].iCount;
					if (g_Proxy.db_insert(Mat) != DB_Proxy::DB_SUCCEED)
					{
						printf("%ld 装载物资type %d matid %d\n",lPlayerID,Mat.type_,Mat.matid_);
						continue;
					}
				}
				else
				{
					Mat.count_ += (*pList)[i].iCount;
					if (g_Proxy.db_update(Mat, Mat) != DB_Proxy::DB_SUCCEED)
					{
						printf("%ld 装载db_update物资type %d matid %d failed\n",lPlayerID,Mat.type_,Mat.matid_);
						continue;
					}
				}
				ResByPlayer.content_ += (*pList)[i].iCount;
				//20100409
				ISystemLog::getInstance()->playerProduce_Load_Log(officer.playerid_,officer.officerid_,officer.officername_,
					pList->lMapID,pList->lCityID,(*pList)[i].iType,(*pList)[i].iMatID,oldProCount,
					curProCount,(*pList)[i].iCount);
			}
			break;
		case EN_BASIC_RESOURCE:			//基本物资
			{
				int iC = 0;
				switch((*pList)[i].iMatID)
				{
				case EN_FOOD:			//粮食
					ResByPlayer.foodcount_ -= (*pList)[i].iCount;
					iC = ResByPlayer.foodcount_;
					if (iC < 0)
					{
						ResByPlayer.foodcount_ += (*pList)[i].iCount;
					}
					break;
				case EN_WOOD:			//木材
					ResByPlayer.woodcount_ -= (*pList)[i].iCount;
					iC = ResByPlayer.woodcount_;
					if (iC < 0)
					{
						ResByPlayer.woodcount_ += (*pList)[i].iCount;
					}
					break;
				case EN_IRON:			//铁
					ResByPlayer.ironcount_ -= (*pList)[i].iCount;
					iC = ResByPlayer.ironcount_;
					if (iC < 0)
					{
						ResByPlayer.ironcount_ += (*pList)[i].iCount;
					}
					break;
				case EN_SKIN:			//皮
					ResByPlayer.skincount_ -= (*pList)[i].iCount;
					iC = ResByPlayer.skincount_;
					if (iC < 0)
					{
						ResByPlayer.skincount_ += (*pList)[i].iCount;
					}
					break;
				case EN_RED_STONE:		//红宝石
					ResByPlayer.redstonecount_ -= (*pList)[i].iCount;
					iC = ResByPlayer.redstonecount_;
					if (iC < 0)
					{
						ResByPlayer.redstonecount_ += (*pList)[i].iCount;
					}
					break;
				case EN_YELLOW_STONE:		//黄宝石
					ResByPlayer.yollowstonecount_ -= (*pList)[i].iCount;
					iC = ResByPlayer.yollowstonecount_;
					if (iC < 0)
					{
						ResByPlayer.yollowstonecount_ += (*pList)[i].iCount;
					}
					break;
				case EN_GREEN_STONE:	//绿宝石
					ResByPlayer.greenstonecount_ -= (*pList)[i].iCount;
					iC = ResByPlayer.greenstonecount_;
					if (iC < 0)
					{
						ResByPlayer.greenstonecount_ += (*pList)[i].iCount;
					}
					break;
				case EN_BLACK_STONE:	//黑铁
					ResByPlayer.backironcount_ -= (*pList)[i].iCount;
					iC = ResByPlayer.backironcount_;
					if (iC < 0)
					{
						ResByPlayer.backironcount_ += (*pList)[i].iCount;
					}
					break;
				};
				if (iC < 0)
				{
					MarketInfo.serialize((*pList)[i].iType, (*pList)[i].iMatID, (*pList)[i].iCount, EN_GOODS_COUNT, m_lMapID, lPlayerID);
					asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
				}
				else
				{
					Mat.setmasterid(lPlayerID, pList->lArmyID);
					Mat.setdetailid((*pList)[i].iType, (*pList)[i].iMatID);
					if (g_Proxy.db_select(Mat) != DB_Proxy::DB_SUCCEED)
					{
						Mat.playerid_ = lPlayerID;
						Mat.armyid_ = pList->lArmyID;
						Mat.type_ = (*pList)[i].iType;
						Mat.matid_ = (*pList)[i].iMatID;
						Mat.count_ = (*pList)[i].iCount;
						if (g_Proxy.db_insert(Mat) != DB_Proxy::DB_SUCCEED)
						{
							printf("%ld 装载物资type %d matid %d failed\n",lPlayerID,Mat.type_,Mat.matid_);
							continue;
						}
					}
					else
					{
						Mat.count_ += (*pList)[i].iCount;
						if (g_Proxy.db_update(Mat, Mat) != DB_Proxy::DB_SUCCEED)
						{
							printf("%ld 装载db_update物资type %d matid %d failed\n",lPlayerID,Mat.type_,Mat.matid_);
							continue;
						}
					}
					ResByPlayer.content_ += (*pList)[i].iCount;
				}
			}
			break;
		};
	}

	ResByPlayer.setmasterid(pList->lCityID, pList->lMapID);
	ResByPlayer.setdetailid(lPlayerID);
	if (g_Proxy.db_update(ResByPlayer, ResByPlayer) != DB_Proxy::DB_SUCCEED)
		return 0;

	return 1;
}
//卸载物资  
int CLevySoldiers::OnUninstallMaterial(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	MATERIAL_LIST * pList = (MATERIAL_LIST *)pDataBuffer;
	if (sizeof(MATERIAL_LIST)+sizeof(tagMaterials)*pList->iCount != iDataSize)
	{
		LOG_TRACE_EX(false,("CLevySoldiers::OnUninstallMaterial-->消息包非法：玩家%d\n",lPlayerID));
		return 0;
	}

	int ret = 0;
	ERROR_INFO ErrorInfo;
	//查询当前库容
	PlayerResource ResByPlayer;
	ResByPlayer.setmasterid(pList->lCityID, pList->lMapID);
	ResByPlayer.setdetailid(lPlayerID);

	if (g_Proxy.db_select(ResByPlayer) != DB_Proxy::DB_SUCCEED)
	{
		return 0;
	}
	//计算总数量，若总数量大于仓库剩余容量，则不卸载
	LoadMaterial Mat;
	int i = 0;
	int iTotal = 0;
	for(i = 0;i<pList->iCount;i++)
	{
		if((*pList)[i].iCount <0)
			return 0;

		iTotal += (*pList)[i].iCount;
	}
	if(iTotal > ResByPlayer.content_)
	{
		ErrorInfo.serialize(EN_CITY_NO_CONTENT, m_lMapID, lPlayerID);
		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return 0;
	}

	int iflag = 0;
	iTotal = 0;
	ArmyTable army;
	army.setmasterid(m_lMapID);
	army.setdetailid(pList->lArmyID);
	if (g_Proxy.db_select(army) != DB_Proxy::DB_SUCCEED)
	{
		return 0;
	}
	memset(&ResByPlayer,0,sizeof(PlayerResource));
	ResByPlayer.setmasterid(pList->lCityID, pList->lMapID);
	ResByPlayer.setdetailid(lPlayerID);
	//20100409
	PlayerOfficerTable officer;
	officer.setmasterid(lPlayerID);
	officer.setdetailid(army.officerid_);
	g_Proxy.db_select(officer);
	
	int oldProCount = 0,curProCount = 0;

	for (i = 0; i < pList->iCount; i++)
	{
		Mat.setmasterid(lPlayerID, pList->lArmyID);
		Mat.setdetailid((*pList)[i].iType, (*pList)[i].iMatID);
		if (g_Proxy.db_select(Mat) != DB_Proxy::DB_SUCCEED)
		{
			printf("%ld 卸载物资type %d matid %d failed\n",lPlayerID,Mat.type_,Mat.matid_);
			continue;
		}
		if ((*pList)[i].iCount > Mat.count_ )
		{
			continue;
		}
		if ((*pList)[i].iCount == Mat.count_)
		{
			/*
			Mat.setmasterid(lPlayerID, pList->lArmyID);
			Mat.setdetailid((*pList)[i].iType, (*pList)[i].iMatID);
			*/
			if (g_Proxy.db_delete(Mat) != DB_Proxy::DB_SUCCEED)
			{
				continue;
			}
		}
		else
		{
			Mat.count_ -= (*pList)[i].iCount;
			Mat.setmasterid(lPlayerID, pList->lArmyID);
			Mat.setdetailid((*pList)[i].iType, (*pList)[i].iMatID);
			if (g_Proxy.db_update(Mat, Mat) != DB_Proxy::DB_SUCCEED)
			{
				continue;
			}
		}
		switch((*pList)[i].iType)
		{
		case WEAPON_ITEM:				//武器
		case DEFEND_ITEM:				//防具
		case ANIMALS_TAME_ITEM:			//野兽材料
		case SPECIAL_TOOL_ITEM:			//特殊工具
			//调用该函数，已经更新了仓库容量

			ret = m_UserInfo.OnUpDataItem((*pList)[i].iMatID, (*pList)[i].iType, (*pList)[i].iCount, lPlayerID, pList->lMapID, pList->lCityID,oldProCount,curProCount);
			if(ret !=1)
			{
				LOG_TRACE_EX(false,("CLevySoldiers::OnUninstallMaterial-->玩家%d在城市%d-%d卸载%d-%d,数量%d失败\n",
					lPlayerID,pList->lMapID, pList->lCityID,(*pList)[i].iMatID, (*pList)[i].iType, (*pList)[i].iCount));	
			}
			else
			{
				//20100409
				ISystemLog::getInstance()->playerProduce_Unload_Log(officer.playerid_,officer.officerid_,officer.officername_,
					(*pList)[i].iMatID,pList->lCityID,(*pList)[i].iType,(*pList)[i].iMatID,
					oldProCount,curProCount,(*pList)[i].iCount);
			}
			break;
		case EN_BASIC_RESOURCE:			//基本物资
			{
				iTotal += (*pList)[i].iCount;
				switch((*pList)[i].iMatID)
				{
				case EN_FOOD:			//粮食
					ResByPlayer.foodcount_ += (*pList)[i].iCount;
					break;
				case EN_WOOD:			//木材
					ResByPlayer.woodcount_ += (*pList)[i].iCount;
					break;
				case EN_IRON:			//铁
					ResByPlayer.ironcount_ += (*pList)[i].iCount;
					break;
				case EN_SKIN:			//皮
					ResByPlayer.skincount_ += (*pList)[i].iCount;
					break;
				case EN_RED_STONE:		//红宝石
					ResByPlayer.redstonecount_ += (*pList)[i].iCount;
					break;
				case EN_YELLOW_STONE:		//黄宝石
					ResByPlayer.yollowstonecount_ += (*pList)[i].iCount;
					break;
				case EN_GREEN_STONE:	//绿宝石
					ResByPlayer.greenstonecount_ += (*pList)[i].iCount;
					break;
				case EN_BLACK_STONE:	//黑铁
					ResByPlayer.backironcount_ += (*pList)[i].iCount;
					break;
				};
			}
			break;
		};
	}
	if (iTotal >0)
	{
		ResByPlayer.content_ = -iTotal;
		if (g_Proxy.db_increase(ResByPlayer) != DB_Proxy::DB_SUCCEED)
			return 0;
	}
	return 1;
}
//物资查看
int CLevySoldiers::OnLookMaterial(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize != sizeof(LOOK_MATERIALS))
	{
		LOG_TRACE_EX(false,("CLevySoldiers::OnLookMaterial-->消息包非法：玩家%d\n",lPlayerID));
		return 0;
	}

	LOOK_MATERIALS * pLook = (LOOK_MATERIALS *)pDataBuffer;

	char *buf = 0;
	int count = 0;
	LoadMaterial Mat;
	Mat.setmasterid(lPlayerID, pLook->lArmyID);
	if (g_Proxy.db_multi_select(Mat, buf, count) != DB_Proxy::DB_SUCCEED)
	{

	}
	LoadMaterial * pMat = (LoadMaterial *)buf;
	int iItem = count/sizeof(LoadMaterial);

	char * tmp = new char[sizeof(MATERIAL_LIST) + sizeof(tagMaterials)*iItem];
	MATERIAL_LIST *pMatList = (MATERIAL_LIST *)tmp;

	for (int i = 0; i < iItem; i++)
	{
		(*pMatList)[i].iType = pMat[i].type_;
		(*pMatList)[i].iMatID = pMat[i].matid_;
		(*pMatList)[i].iCount = pMat[i].count_;
	}
	pMatList->serialize(iItem, m_lMapID, lPlayerID, EN_S_LOOK_MATERIAL);
	int ret = asynch_write(handle, (char *)pMatList, pMatList->length);

	if (buf != NULL)
	{
		delete [] buf;
		buf = 0;
	}
	if (tmp != NULL)
	{
		delete [] tmp;
		tmp = 0;
	}

	return ret;
}
//殖民城市
int CLevySoldiers::OnColonizeCity(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize != sizeof(COLONIZE_CITY))
	{
		LOG_TRACE_EX(false,("CLevySoldiers::OnColonizeCity->消息包非法：玩家%d\n",lPlayerID));
		return -1;
	}

	COLONIZE_CITY * pCity = (COLONIZE_CITY *)pDataBuffer;

	int ret = 0;
	ERROR_INFO ErrorInfo;
	if (g_MapInfo->GetMapState() == war)
	{
		ErrorInfo.serialize(EN_MAP_INWAR, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return 1;
	}
	if (pCity->lArmyID <= 0 || pCity->lMapID != pCity->lCityID/10000)
	{
		ErrorInfo.serialize(EN_NOT_ARMY_ID, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return 1;
	}

	//先后现判断玩家的城市个数是否已达10个,20090610改为有军衔限制数量
	PlayerOfficerTable playerofficer;
	playerofficer.setmasterid(lPlayerID);
	playerofficer.setdetailid(lPlayerID);
	if(g_Proxy.db_select(playerofficer) != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return 2;
	}
	char *buf = 0;
	int count = 0,iItem = 0;
	PlayerCity PCity;
	memset(&PCity, 0, sizeof(PlayerCity));
	PCity.setmasterid(lPlayerID);
	if (g_Proxy.db_multi_select(PCity, buf, count) != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return 2;
	}
	if (buf != NULL)
	{
		delete [] buf;
		buf = 0;
	}
	iItem = count/sizeof(PlayerCity);
	LPQuantityByRank_Item pQuantityByRank = g_LoadDBinfo.GetQuantityByRank(playerofficer.militaryrank_);

	if(pQuantityByRank == NULL)
	{
		ErrorInfo.serialize(EN_COLONIZE_UP_10, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return 3;
	}
	if (iItem >= pQuantityByRank->TownQuantity)
	{
		ErrorInfo.serialize(EN_COLONIZE_UP_10, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return 3;
	}

	ArmyTable Army;
	Army.setmasterid(pCity->lMapID);
	Army.setdetailid(pCity->lArmyID);
	if (g_Proxy.db_select(Army) != DB_Proxy::DB_SUCCEED)
	{//部队不存在
		ErrorInfo.serialize(EN_NOT_ARMY_ID, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return 2;
	}
	if (Army.fweapon_ < 150 || Army.fweapon_ > 152)
	{//非殖民部队
		ErrorInfo.serialize(EN_NOT_COLONIZE_ARMY, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return 3;
	}
	PlayerChatRoomTable Res;
	memset(&Res, 0, sizeof(Res));
	Res.setid(lPlayerID);
	if (g_Proxy.db_select(Res) != DB_Proxy::DB_SUCCEED)
	{
		//_ASSERT(false);
		return 2;
	}
	MapInfoTable Map_Info;
	memset(&Map_Info, 0, sizeof(Map_Info));
	Map_Info.setid(pCity->lMapID);
	if (g_Proxy.db_select(Map_Info) != DB_Proxy::DB_SUCCEED)
	{
		//_ASSERT(false);
		return 2;
	}
	if (Map_Info.countryid_ != Res.country_)
	{
		ErrorInfo.serialize(EN_NOT_SELF_MAP, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return 4;
	}
	
	memset(&PCity, 0, sizeof(PlayerCity));
	PCity.setmasterid(lPlayerID);
	PCity.setdetailid(pCity->lMapID);
	if (g_Proxy.db_select(PCity) == DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_HAVE_CITY, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return 2;
	}
	PCity.playerid_ = lPlayerID;
	PCity.mapid_ = pCity->lMapID;
	PCity.cityid_ = pCity->lCityID;
	PCity.state_ = 0;
	if (g_Proxy.db_insert(PCity) != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_COLONIZE_CITY_FAILED, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return 2;
	}
	
	char * bbuf = 0;
	memset(&PCity, 0, sizeof(PlayerCity));
	PCity.setmasterid(lPlayerID);

	if (g_Proxy.db_multi_select(PCity, bbuf, count) != DB_Proxy::DB_SUCCEED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATA_ERROR, m_lMapID, lPlayerID);

		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("玩家城市信息 error db_select_all  count:%d\n", count/sizeof(PlayerCity)));
		return 0;
	}
	int Item = count/sizeof(PlayerCity);
	PlayerCity *pCityList = (PlayerCity *)bbuf;

	char * tmp = new char[sizeof(PLAYER_CITY_LIST) + sizeof(tagPlayerCityList)*Item];
	PLAYER_CITY_LIST *pPlayerCity = (PLAYER_CITY_LIST *)tmp;

	pPlayerCity->serialize(Item, m_lMapID, lPlayerID);
	for (int n = 0; n < Item; n++)
	{
		(*pPlayerCity)[n].lCityID = pCityList[n].cityid_;
		(*pPlayerCity)[n].lMapID = pCityList[n].mapid_;
	}

	ret = asynch_write(handle, (char *)pPlayerCity, pPlayerCity->length);
	g_DataServer.setAgentOfPlayer(lPlayerID,handle);
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
	ErrorInfo.serialize(EN_COLONIZE_CITY, m_lMapID, lPlayerID);
	ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
	DeleteArmyInfo(handle,lPlayerID, pCity->lArmyID);

	//发送创建部队信息至大地图，暂时由郡服务器返回，避免大地图负担add by xujincai 20090225
	ModifyArmyState_Response modifyarmy_rep;
	modifyarmy_rep.serialize(handle,lPlayerID,pCity->lArmyID,m_lMapID,0,ARMY_ANNIHILATE,m_lMapID);
	asynch_write(modifyarmy_rep.agent,(char*)&modifyarmy_rep,modifyarmy_rep.length);

	PlayerResource ResByPlayer;
	memset(&ResByPlayer, 0,sizeof(ResByPlayer));

	ResByPlayer.foodcount_ = 0;
	ResByPlayer.woodcount_ = 0;
	ResByPlayer.ironcount_ = 0;
	ResByPlayer.skincount_ = 0;
	ResByPlayer.yollowstonecount_ = 0;
	ResByPlayer.redstonecount_ = 0;
	ResByPlayer.greenstonecount_ = 0;
	ResByPlayer.backironcount_ = 0;
	ResByPlayer.content_ = 50000;
	ResByPlayer.completetimes_ = IceUtil::Time::now().toSeconds();
	ResByPlayer.mapid_ = pCity->lMapID;
	ResByPlayer.cityid_ = pCity->lCityID;
	ResByPlayer.playerid_ = lPlayerID;

	//加入仓库
	int lConID = 5;
	ConstructionConfigure * pConstrParam = g_LoadDBinfo.getConstructionConfigure(lConID, 1);
	if (pConstrParam == NULL) return 0;

	ResByPlayer.content_ = pConstrParam->modify_;
	g_Proxy.db_insert(ResByPlayer);

	PlayerConstruction PlayerCon;
	memset(&PlayerCon, 0, sizeof(PlayerConstruction));
	PlayerCon.constructionid_ = lConID;
	PlayerCon.level_ = 1;
	PlayerCon.output_ = pConstrParam->modify_;
	PlayerCon.completetimes_ = IceUtil::Time::now().toSeconds();

	PlayerCon.setmasterid(pCity->lMapID, pCity->lCityID, lPlayerID);
	PlayerCon.mapid_ = pCity->lMapID;
	PlayerCon.cityid_ = pCity->lCityID;
	PlayerCon.playerid_ = lPlayerID;
	PlayerCon.sortid_ = 1;
	g_Proxy.db_insert(PlayerCon);

	//20100409
	ISystemLog::getInstance()->playerCitys_Colony_Log(playerofficer.playerid_,playerofficer.officername_,
		iItem,Item,pCity->lMapID,pCity->lCityID);
	playerofficer.setdetailid(Army.officerid_);
	g_Proxy.db_select(playerofficer);
	ISystemLog::getInstance()->herostate_colony_Log(playerofficer.playerid_,playerofficer.officerid_,playerofficer.officername_,
		pCity->lMapID,pCity->lCityID);
	return 0;
}
void CLevySoldiers::OnLevySpeed(unsigned long lPlayerID, int iSortID, int iValue)
{
}


//立即完成
void CLevySoldiers::OnImmedComplete(LPImmedComplete_Request msg)
{
	LPImmedComplete_Request req_msg = (LPImmedComplete_Request)msg;
	if(req_msg->length != sizeof(ImmedComplete_Request))
	{
		LOG_TRACE_EX(false,("CLevySoldiers::OnImmedComplete-->消息包非法\n"));
		return;
	}
	Notify_ImmedComplete_Request notify_msg;
	ImmedComplete_Response rep_msg;

	IceUtil::LockT<IceUtil::Mutex> Lock(m_mapLevyingListMutex);
	multimap<unsigned long, tagLevyingSoldiers>::iterator it = m_mapLevyingList.lower_bound(req_msg->from);
	multimap<unsigned long, tagLevyingSoldiers>::iterator endit = m_mapLevyingList.upper_bound(req_msg->from);

	__int64 iTimes  = -1;
	for (; it != endit; it++)
	{
		if (it->second.lMapID == req_msg->MapID && it->second.lCityID == req_msg->CityID && it->second.lStartTime >0)
		{
			iTimes = it->second.lStartTime - IceUtil::Time::now().toSeconds();
			break;
		}
	}

	Lock.release();
	if(iTimes <0)
		return ;
	//收费标准 8Go点/小时
	iTimes = (iTimes)/450;
	if(iTimes <=0)
		iTimes = 1;

	notify_msg.serialize(req_msg->agent,req_msg->from,Conscription_Speed_Card,req_msg->MapID,req_msg->CityID,req_msg->ItemID,
		req_msg->SortID,req_msg->Level,(int)iTimes,req_msg->ProductionType);
	if(event_notify(MDM_GP_MAP_CHARGE,(char*)&notify_msg,notify_msg.length) ==0)
	{//通知失败
		rep_msg.serialize(req_msg->agent,req_msg->MapID,req_msg->from,111,req_msg->Type,req_msg->SortID,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	}
}
//处理收费系统返回的通知消息
void CLevySoldiers::OnImmedCompleteNotifyMsg(LPNotify_ImmedComplete_Response msg)
{
	ImmedComplete_Response rep_msg;
	
	LEVYING_SOLDIER_LIST levying_soldier_item;
	int iRaceID = 0;
	int iCount = 0;
	long lTimerID = 0;
	if(msg->Error_Code != 0)
	{//扣费失败
		rep_msg.serialize(msg->agent,msg->Param.MapID,msg->from,msg->Error_Code,msg->Param.Type,msg->Param.SortID,0);
	}
	else
	{//OnLevyTimeEvent(void * pDataBuffer, int iDataSize)  LEVYING_SOLDIER_LIST
		IceUtil::LockT<IceUtil::Mutex> Lock(m_mapLevyingListMutex);
		multimap<unsigned long, tagLevyingSoldiers>::iterator it = m_mapLevyingList.lower_bound(msg->from);
		multimap<unsigned long, tagLevyingSoldiers>::iterator endit = m_mapLevyingList.upper_bound(msg->from);

		bool bFlag = false;
		__int64 iTimes  = 0;
		for (; it != endit; it++)
		{
			if (it->second.lMapID == msg->Param.MapID && it->second.lCityID == msg->Param.CityID && it->second.lStartTime >0)
			{
				iRaceID = it->second.iReceID;
				iCount = it->second.iCount;
				lTimerID = it->second.lTimerID;
				bFlag = true;
				break;
			}
		}

		Lock.release();
		if(!bFlag)
		{
			
		}
		else
		{
			cancel_timer(lTimerID);
			levying_soldier_item.serialize(msg->from,msg->Param.MapID,msg->Param.CityID,iRaceID,iCount,it->second.lIndex,EN_TIMER_LEVYING);

			OnLevyTimeEvent(&levying_soldier_item,levying_soldier_item.length);
		}
		rep_msg.serialize(msg->agent,msg->Param.MapID,msg->from,msg->Error_Code,msg->Param.Type,msg->Param.SortID,msg->Param.GOCount);
	}
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
}
//郡地图被占领，清楚玩家数据
void CLevySoldiers::OnResetMapInfo(LPResetMapInfo_Request msg)
{//清空征募列表
	//20090616 清空表数据
	/*
	*/
	multimap<unsigned long, tagLevyingSoldiers>::iterator iter;
	LevySoldierActivity delItem;
	for(iter = m_mapLevyingList.begin();iter != m_mapLevyingList.end();iter++)
	{
		delItem.setmasterid(iter->first,iter->second.lMapID,iter->second.lCityID);
		delItem.setdetailid(iter->second.lIndex);
		g_Proxy.db_delete(delItem);
	}
	m_mapLevyingList.clear();
}
void CLevySoldiers::InitLevySoldierActivity(DB_Proxy *pDBProxy)
{
	LevySoldierActivity levysoldiertb;
	LevySoldierActivity *pLevySoldierList = 0;
	char *buf = 0;
	int count = 0;
	if(pDBProxy->db_select_all(levysoldiertb,buf,count) != DB_Proxy::DB_SUCCEED)
		return;
	count = count/sizeof(LevySoldierActivity);
	pLevySoldierList = (LevySoldierActivity*)buf;
	tagLevyingSoldiers Levying;
	LEVYING_SOLDIER_LIST * pLevying = 0;
	int iTimes = 0;
	/*
	
	Levying.lMapID = pRace->lMapID;
	Levying.lCityID = pRace->lCityID;
	Levying.iReceID = pRace->iRace;
	Levying.iCount = pRace->iCount;
	Levying.lIndex = 0;
	pLevying->serialize(lPlayerID, pRace->lMapID, pRace->lCityID, pRace->iRace, pRace->iCount,levysoldier.sortid_, EN_TIMER_LEVYING);
	long lTimeID = set_timer(iTimeL, (const void *)pLevying);
	pLevying->TimerID = lTimeID;
	printf("玩家 %ld 进入征兵中种族%d 数量%d 所需时间%d,sortid:%d\n",lPlayerID,pRace->iRace, pRace->iCount,iTimeL,Levying.lIndex);
	Levying.lTimerID = lTimeID;
	m_mapLevyingList.insert(make_pair(lPlayerID, Levying));
	*/
	for(int i=0;i<count;i++)
	{
		if(pLevySoldierList[i].mapid_ != m_lMapID)
			continue;

		Levying.lMapID = pLevySoldierList[i].mapid_;
		Levying.lCityID = pLevySoldierList[i].cityid_;
		Levying.iReceID = pLevySoldierList[i].raceid_;
		Levying.iCount = pLevySoldierList[i].count_;
		Levying.lIndex = pLevySoldierList[i].sortid_;
		Levying.lStartTime = pLevySoldierList[i].endtime_;
		if(pLevySoldierList[i].endtime_ >0)
		{//定时器
			iTimes = pLevySoldierList[i].endtime_ - IceUtil::Time::now().toSeconds();
			if(iTimes <=0)
				iTimes = 5;
			pLevying = new LEVYING_SOLDIER_LIST;
			pLevying->serialize(pLevySoldierList[i].playerid_, pLevySoldierList[i].mapid_, pLevySoldierList[i].cityid_,
				pLevySoldierList[i].raceid_, pLevySoldierList[i].count_,pLevySoldierList[i].sortid_, EN_TIMER_LEVYING);
			pLevying->TimerID = set_timer(iTimes, (const void *)pLevying);
			Levying.lTimerID = pLevying->TimerID;
		}
		else
		{
		}
		m_mapLevyingList.insert(make_pair(pLevySoldierList[i].playerid_, Levying));
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
}
int CLevySoldiers::ReqCreateArmy(CREATE_ARMY* pCreateArmy)
{
	if (g_MapInfo->GetMapState() == war)
	{
		CREATE_ARMY_FAILD Faild;
		Faild.serialize(EN_MAP_INWAR, m_lMapID, pCreateArmy->from);
		asynch_write(pCreateArmy->agent, (char *)&Faild, Faild.length);
		LOG_TRACE_EX(false,("%ld %d 创建部队为战斗状态，无法创建\n",pCreateArmy->from,pCreateArmy->lOfficerID));
		return 0;
	}
	int iCount = pCreateArmy->iFSoldierCount;
	int iRaceID = pCreateArmy->iRaceID;
	bool flag = false;
	int ret = 0;
	unsigned long lPlayerID = pCreateArmy->from;
	int handle = pCreateArmy->agent;
	int m_lMapID = pCreateArmy->lMapID;

	PlayerChatRoomTable playerinfo;
	playerinfo.setid(lPlayerID);
	if (g_Proxy.db_select(playerinfo)!= DB_Proxy::DB_SUCCEED)
	{
		return 0;
	}
	PlayerOfficerTable Officer;
	memset(&Officer, 0, sizeof(Officer));
	Officer.setmasterid(lPlayerID);
	Officer.setdetailid(pCreateArmy->lOfficerID);
	if (g_Proxy.db_select(Officer) != DB_Proxy::DB_SUCCEED)
	{
		CREATE_ARMY_FAILD Faild;
		Faild.serialize(6, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		LOG_TRACE_EX(false,("%ld %d 创建部队指挥官ID 非法\n",lPlayerID,pCreateArmy->lOfficerID));
		return 0;
	}
	if(Officer.state_ > EN_LEISURE)
	{
		CREATE_ARMY_FAILD Faild;
		Faild.serialize(6, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		LOG_TRACE_EX(false,("%ld %d 创建部队指挥官状态非空闲\n",lPlayerID,pCreateArmy->lOfficerID));
		return 0;
	}
	//数量超过上下限
	if (iCount > 100 || iCount < 1 ||pCreateArmy->lOfficerID <= 0 || pCreateArmy->iFWeaponID <= 0)
	{
		CREATE_ARMY_FAILD Faild;
		Faild.serialize(5, m_lMapID, pCreateArmy->from);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		printf("%ld %d 创建部队数量超过上下限\n",lPlayerID,iCount);
		return 0;
	}
	WeaponAttribute* weaponAttribute = g_LoadDBinfo.GetWeaponAttribute(pCreateArmy->iFWeaponID);
	if (weaponAttribute == NULL)
	{
		CREATE_ARMY_FAILD Faild;
		Faild.serialize(7, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		printf("%ld %d 创建部队武器ID 非法\n",lPlayerID,pCreateArmy->iFWeaponID);
		return 0;
	}
	//add by xujincai 20081226 增加殖民马车部队数量只能为一的限制
	if(weaponAttribute->weaponType == 20 && iCount >1)
	{
		CREATE_ARMY_FAILD Faild;
		Faild.serialize(5, m_lMapID, pCreateArmy->from);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		printf("%ld %d 创建殖民马车部队数量超过上下限\n",lPlayerID,iCount);
		return 0;
	}
	int iSoldier = weaponAttribute->soldier;
	//判断武器与职业是否匹配add by xujincai 20081219
	if((iSoldier != pCreateArmy->iSoldiertype) && (weaponAttribute->soldier != 9) && (weaponAttribute->soldier != 10))
	{
		CREATE_ARMY_FAILD Faild;
		Faild.serialize(4, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		printf("%ld %d 创建部队武器与职业不匹配\n",lPlayerID,pCreateArmy->iSoldiertype);
		return 0;
	}

	DefendAttribute *defendAttribute = NULL;
	
	//判断甲是否匹配
	if(pCreateArmy->iDefendID >0 )
	{
		defendAttribute = g_LoadDBinfo.GetDefendAttribute(pCreateArmy->iDefendID);
		if (defendAttribute == NULL || !g_LoadDBinfo.DefendMacthSoldier(pCreateArmy->iDefendID,pCreateArmy->iSoldiertype))
		{
			CREATE_ARMY_FAILD Faild;
			Faild.serialize(8, m_lMapID, lPlayerID);
			ret = asynch_write(handle, (char *)&Faild, Faild.length);
			printf("%ld %d 创建部队防具ID 非法(或防具与职业不匹配)\n",lPlayerID,pCreateArmy->iFWeaponID);
			return 0;
		}
	}
	//跟武器类型 确定此种族可做的职业

	OfficerRaceAttribute* m_RaceAttri = g_LoadDBinfo.GetOfficerRaceAttr(pCreateArmy->iRaceID);

	if (m_RaceAttri== NULL)
	{
		CREATE_ARMY_FAILD Faild;
		Faild.serialize(7, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		//printf("%ld %d 创建部队非法\n",lPlayerID,pCreateArmy->iFWeaponID);
		return 0;
	}
	if (!g_LoadDBinfo.OfficerRacematchSoldier(static_cast<RACE_TYPE>(m_RaceAttri->raceid_),static_cast<SOLDIER_TYPE>(iSoldier)))
	{
		CREATE_ARMY_FAILD Faild;
		Faild.serialize(4, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		printf("%ld 创建部队跟武器(%d)类型 确定此种族(%d)可做的职业\n",lPlayerID,pCreateArmy->iFWeaponID,iRaceID);
		return 0;
	}
	//add by xujincai 20091118 武器/防具装精技能的的判断
	int iItemType = WEAPON_ITEM;
	if (pCreateArmy->iFWeaponID >= 150)
		iItemType = SPECIAL_TOOL_ITEM;
	if (pCreateArmy->iFWeaponID >= 100 && pCreateArmy->iFWeaponID < 150)
		iItemType = ANIMALS_TAME_ITEM;

	PlayerSkillTable playerSkill;
	char *skillBuf = 0;
	int skillCount = 0;
	playerSkill.setmasterid(lPlayerID,pCreateArmy->lOfficerID);
	if (g_Proxy.db_multi_select(playerSkill,skillBuf,skillCount) ==DB_Proxy::DB_FAILED)
	{
		CREATE_ARMY_FAILD Faild;
		Faild.serialize(5, m_lMapID, pCreateArmy->from);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		printf("玩家%ld 的英雄%d 技能查询失败。\n",lPlayerID,pCreateArmy->lOfficerID);
		return 0;
	}
	skillCount = skillCount/sizeof(PlayerSkillTable);
	PlayerSkillTable *pPlayerSkills = (PlayerSkillTable*)skillBuf;
	if (iItemType != SPECIAL_TOOL_ITEM && !g_LoadDBinfo.hasSpecialitySkill(pPlayerSkills,skillCount,weaponAttribute->weaponType,weaponAttribute->level))
	{
		if (skillBuf)
		{
			delete[] skillBuf;
			skillBuf = 0;
		}
		CREATE_ARMY_FAILD Faild;
		Faild.serialize(9, m_lMapID, pCreateArmy->from);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		printf("玩家%ld 的英雄%d 未学习武器精专技能（武器：%d-%d)。\n",
			lPlayerID,pCreateArmy->lOfficerID,weaponAttribute->weaponType,weaponAttribute->level);
		return 0;
	}
	if (defendAttribute && !g_LoadDBinfo.hasSpecialitySkill(pPlayerSkills,skillCount,defendAttribute->defendType,defendAttribute->level))
	{
		if (skillBuf)
		{
			delete[] skillBuf;
			skillBuf = 0;
		}
		CREATE_ARMY_FAILD Faild;
		Faild.serialize(9, m_lMapID, pCreateArmy->from);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		printf("玩家%ld 的英雄%d 未学习防具精专技能（防具：%d-%d)。\n",
			lPlayerID,pCreateArmy->lOfficerID,defendAttribute->defendType,defendAttribute->level);
		return 0;
	}
	if (skillBuf)
	{
		delete[] skillBuf;
		skillBuf = 0;
	}

	//种族数量
	flag = false;
	PlayerSoldierRace RaceCount,UpdateRaceCount;
	memset(&RaceCount,0,sizeof(RaceCount));
	RaceCount.setmasterid(m_lMapID, pCreateArmy->lCityID);
	RaceCount.setdetailid(lPlayerID);
	if (g_Proxy.db_select(RaceCount) != DB_Proxy::DB_SUCCEED)
	{
		printf("%d 创建部队在此城市(%d)没有兵种数!\n",lPlayerID, pCreateArmy->lCityID);
	}
	int oldRace = 0;
	switch(pCreateArmy->iRaceID)
	{
	case 1:
		if (RaceCount.human_ < iCount)
			flag = true;
		oldRace = RaceCount.human_ ;
		UpdateRaceCount.human_ = -iCount;
		break;
	case 2:
		if (RaceCount.fairy_ < iCount)
			flag = true;
		oldRace = RaceCount.fairy_ ;
		UpdateRaceCount.fairy_ = -iCount;
		break;
	case 3:
		if (RaceCount.mannikin_ < iCount)
			flag = true;
		oldRace = RaceCount.mannikin_ ;
		UpdateRaceCount.mannikin_ = -iCount;
		break;
	case 4:
		if (RaceCount.dandiprat_ < iCount)
			flag = true;
		oldRace = RaceCount.dandiprat_ ;
		UpdateRaceCount.dandiprat_ = -iCount;
		break;
	case 5:
		if (RaceCount.halfanimal_ < iCount)
			flag = true;
		oldRace = RaceCount.halfanimal_ ;
		UpdateRaceCount.halfanimal_ = -iCount;
		break;
	case 6:
		if (RaceCount.cattle_ < iCount)
			flag = true;
		oldRace = RaceCount.cattle_ ;
		UpdateRaceCount.cattle_ = -iCount;
		break;
	case 7:
		if (RaceCount.blackfairy_ < iCount)
			flag = true;
		oldRace = RaceCount.blackfairy_ ;
		UpdateRaceCount.blackfairy_ = -iCount;
		break;
	case 8:
		if (RaceCount.decpesrson_ < iCount)
			flag = true;
		oldRace = RaceCount.decpesrson_ ;
		UpdateRaceCount.decpesrson_ = -iCount;
		break;
	};
	if (flag)
	{
		CREATE_ARMY_FAILD Faild;
		Faild.serialize(1, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		printf("%ld %d 创建部队种族数量不够\n",lPlayerID,iCount);
		return 0;
	}
	//判断武器数量add by xujincai 20081226 避免由于武器、防具数量不够时，玩家兵数量减少的问题
	// 武器数量

	PlayerResource citycontent;
	ProductionAmount ItemInfo;
	ItemInfo.setmasterid(m_lMapID, pCreateArmy->lCityID, lPlayerID, iItemType);
	ItemInfo.setdetailid(pCreateArmy->iFWeaponID);
	int oldWeapon = 0;
	int oldDefend = 0;
	if (g_Proxy.db_select(ItemInfo) != DB_Proxy::DB_SUCCEED)
	{
		printf("%d 创建部队武器数量g_Proxy.db_select(ItemInfo)\n",lPlayerID);
	}
	if (ItemInfo.productioncount_ < pCreateArmy->iFSoldierCount)
	{
		CREATE_ARMY_FAILD Faild;
		Faild.serialize(2, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&Faild, Faild.length);
		printf("%ld %d 创建部队武器数量不够\n",lPlayerID,iCount,ItemInfo.productioncount_ );
		return 0;
	}
	oldWeapon = ItemInfo.productioncount_;
	//判断防具数量add by xujincai 20081226 避免由于武器、防具数量不够时，玩家兵数量减少的问题// 武器数量
	if (pCreateArmy->iDefendID > 0)
	{
		memset(&ItemInfo, 0, sizeof(ItemInfo));
		ItemInfo.setmasterid(m_lMapID, pCreateArmy->lCityID, lPlayerID, DEFEND_ITEM);
		ItemInfo.setdetailid(pCreateArmy->iDefendID);
		if (g_Proxy.db_select(ItemInfo) != DB_Proxy::DB_SUCCEED)
		{
			printf("%ld 创建部队防具（%d)数量g_Proxy.db_select(ItemInfo)\n",lPlayerID,pCreateArmy->iDefendID);
		}
		// 防具数量
		if (ItemInfo.productioncount_ < pCreateArmy->iFSoldierCount)
		{
			CREATE_ARMY_FAILD Faild;
			Faild.serialize(3, m_lMapID, lPlayerID);
			ret = asynch_write(handle, (char *)&Faild, Faild.length);
			printf("%ld %d 创建部队防具数量不够\n",lPlayerID,iCount,ItemInfo.productioncount_ );
			return 0;
		}
		oldDefend = ItemInfo.productioncount_ ;
	}
	//扣除兵
	UpdateRaceCount.setmasterid(m_lMapID, pCreateArmy->lCityID);
	UpdateRaceCount.setdetailid(lPlayerID);
	//if (g_Proxy.db_update(RaceCount,RaceCount) != DB_Proxy::DB_SUCCEED)del by xujincai 20081226
	if (g_Proxy.db_increase(UpdateRaceCount) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("%d g_Proxy.db_update(RaceCount)\n",lPlayerID));
	}
	ISystemLog::getInstance()->playerSoldier_CreatArmy_Log(Officer.playerid_,Officer.officerid_,Officer.officername_,
		m_lMapID,pCreateArmy->lCityID,pCreateArmy->iRaceID,oldRace,oldRace - iCount,iCount);
	//扣除武器	
	memset(&ItemInfo, 0, sizeof(ItemInfo));
	ItemInfo.productioncount_ = -pCreateArmy->iFSoldierCount;
	ItemInfo.setmasterid(m_lMapID, pCreateArmy->lCityID, lPlayerID, iItemType);//modify by xujincai 20081226 WEAPON_ITEM->iItemType
	ItemInfo.setdetailid(pCreateArmy->iFWeaponID);
	if(g_Proxy.db_increase(ItemInfo) != DB_Proxy::DB_SUCCEED)
	{
		printf("%ld 组建部队，扣除武器失败（%d)\n",lPlayerID,pCreateArmy->iFWeaponID);
	}
	else
	{
		citycontent.content_ +=pCreateArmy->iFSoldierCount;//使用武器，库存增加
	}
	ISystemLog::getInstance()->playerProduce_CreateArmy_Log(Officer.playerid_,Officer.officerid_,Officer.officername_,
		m_lMapID,pCreateArmy->lCityID,iItemType,pCreateArmy->iFWeaponID,oldWeapon,ItemInfo.productioncount_,pCreateArmy->iFSoldierCount);
	//防具ID 非法
	//扣除防具
	if (pCreateArmy->iDefendID > 0)
	{	
		memset(&ItemInfo, 0, sizeof(ItemInfo));	
		ItemInfo.productioncount_ = -pCreateArmy->iFSoldierCount;
		ItemInfo.setmasterid(m_lMapID, pCreateArmy->lCityID, lPlayerID, DEFEND_ITEM);
		ItemInfo.setdetailid(pCreateArmy->iDefendID);
		if (g_Proxy.db_increase(ItemInfo) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE_EX(false,("%ld 创建部队更新道具数量g_Proxy.db_update(ItemInfo)\n",lPlayerID));
		}
		else
		{
			citycontent.content_ +=pCreateArmy->iFSoldierCount;//使用武器，库存增加

			ISystemLog::getInstance()->playerProduce_CreateArmy_Log(Officer.playerid_,Officer.officerid_,Officer.officername_,
				m_lMapID,pCreateArmy->lCityID,DEFEND_ITEM,pCreateArmy->iFWeaponID,oldDefend,ItemInfo.productioncount_,pCreateArmy->iFSoldierCount);

		}
	}
	//修改库存容量add by xujincai 20081226
	if(citycontent.content_ >0)
	{
		citycontent.setmasterid(pCreateArmy->lCityID,m_lMapID);
		citycontent.setdetailid(lPlayerID);
		if (g_Proxy.db_increase(citycontent) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE_EX(false,("%ld 创建部队更新仓库容量失败g_Proxy.db_update(ItemInfo)\n",lPlayerID));
		}
	}
	ArmyTable Army;
	memset(&Army, 0, sizeof(Army));
	//部队信息
	Army.playerid_ = lPlayerID;
	Army.officerid_ = pCreateArmy->lOfficerID;
	Army.mapid_ = m_lMapID;
	Army.cityid_ = pCreateArmy->lCityID;
	Army.raceid_ = pCreateArmy->iRaceID;
	Army.soldiertype_ = pCreateArmy->iSoldiertype;
	Army.fweapon_ = pCreateArmy->iFWeaponID;
	Army.fdefendtype_ = pCreateArmy->iDefendID;
	Army.fweaponlevel_ = pCreateArmy->iWeaponLevel;
	Army.fdefendlevel_ = pCreateArmy->iDefendLevel;
	Army.fsexp_ = 0;
	Army.firstcount_ = iCount;
	Army.soldiertype_ = iSoldier;
	CityInfo cInfo;
	list<CityInfo>::iterator cIter;
	POS_STRUCT tmpPos;
	tmpPos.PosX = 0;
	tmpPos.PosY = 0;
	for (cIter  = g_MapInfo->m_CityInfo.begin(); cIter != g_MapInfo->m_CityInfo.end();  cIter ++)
	{
		cInfo = (*cIter);
		if (cInfo.uCityID%10 == pCreateArmy->lCityID%10)
		{
			tmpPos = cInfo.CityPos.front();
			break ;
		}
	}
	Army.x = tmpPos.PosX;
	Army.y = tmpPos.PosY;
	Army.state_ = 1;
	Army.direction = 1;
	Army.countryid_ = playerinfo.country_;                                                
	strcpy(Army.armyname_, pCreateArmy->szName);

	/*
	Officer.state_ = 1;

	Officer.setmasterid(lPlayerID);
	Officer.setdetailid(pCreateArmy->lOfficerID);
	if (g_Proxy.db_update(Officer,Officer) != DB_Proxy::DB_SUCCEED)
	{
	printf("玩家 %ld 给指挥官 %ld 更新部队 %ld  失败!\n",lPlayerID,pCreateArmy->lOfficerID,Army.armyid_);
	}
	else
	{
	printf("玩家 %ld 给指挥官 %ld 更新部队 %ld  成功!\n",lPlayerID,pCreateArmy->lOfficerID,Army.armyid_);
	}*/
	Army.setmasterid(m_lMapID);
	if (g_Proxy.db_insert(Army) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("%ld 创建部队 已存在 部队ID: %d\n",lPlayerID,Army.armyid_));
		return false;
	}
	else
		printf("玩家  %ld,创建部队成功, 部队ID: %d,指挥官ID;%d,郡ID:%d,城市ID:%d,兵种数: %d,state %d dir %d\n",lPlayerID,Army.armyid_,Army.officerid_,Army.mapid_,Army.cityid_,Army.firstcount_,Army.state_,Army.direction);

	//更新指挥官
	Officer.armyid_ = Army.armyid_;
	Officer.state_ = EN_HAVE_ARMY;
	//add by xujincai 20081222
	Officer.mapid_ = m_lMapID;
	Officer.cityid_ = Army.cityid_;

	Officer.setmasterid(lPlayerID);
	Officer.setdetailid(pCreateArmy->lOfficerID);
	if (g_Proxy.db_update(Officer,Officer) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("玩家 %ld 给指挥官 %ld 分配部队 %ld  失败!\n",lPlayerID,pCreateArmy->lOfficerID,Army.armyid_));
	}
	else
	{
		printf("玩家 %ld 给指挥官 %ld 分配部队 %ld  成功!\n",lPlayerID,pCreateArmy->lOfficerID,Army.armyid_);
	}
	ISystemLog::getInstance()->heroState_CreateArmy_Log(Officer.playerid_,Officer.officerid_,Officer.officername_);
	AddArmyInfo2Fight(pCreateArmy->agent,lPlayerID,m_lMapID,Army.armyid_);
	//发送创建部队信息至大地图，暂时由郡服务器返回，避免大地图负担
	ModifyArmyState_Response modifyarmy_rep;
	modifyarmy_rep.serialize(pCreateArmy->agent,lPlayerID,Army.armyid_,m_lMapID,Army.firstcount_,Army.state_,m_lMapID);
	strcpy(modifyarmy_rep.armyInfo.ArmyName,Army.armyname_);
	strcpy(modifyarmy_rep.armyInfo.OfficerName,Officer.officername_);
	modifyarmy_rep.armyInfo.Fdefendtype = Army.fdefendlevel_;
	modifyarmy_rep.armyInfo.Fweapon = Army.fweapon_;
	modifyarmy_rep.armyInfo.Raceid = Army.raceid_;
	modifyarmy_rep.armyInfo.Soldiertype = Army.soldiertype_;
	asynch_write(modifyarmy_rep.agent,(char*)&modifyarmy_rep,modifyarmy_rep.length);
	return ret != 0;
}

//add by xujincai 20081218
//解散部队
int CLevySoldiers::ReqUnlayArmy(const char *msg)
{
	LPUnlayArmy_Request req_msg = (LPUnlayArmy_Request)msg;
	LOG_TRACE_EX((req_msg->length == sizeof(UnlayArmy_Request)),("ReqUnlayArmy-->消息包非法\n"));
	if(req_msg->length != sizeof(UnlayArmy_Request))
		return 1;

	UnlayArmy_Response rep_msg;
	int iMapID = req_msg->to;	
	int iArmyID = req_msg->ArmyID;
	int iRet = 0;
	if(g_MapInfo->GetMapState() == war)
	{//战斗状态，无法解散
		rep_msg.serialize(req_msg->agent,iMapID,req_msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	//判断是否有据点
	PlayerCity city;
	city.setmasterid(req_msg->from);
	city.setdetailid(iMapID);
	if(g_Proxy.db_select(city) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,iMapID,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 2;
	}
	//判断部队是否存在
	ArmyTable armytb;
	armytb.setmasterid(iMapID);
	armytb.setdetailid(iArmyID);
	iRet = g_Proxy.db_select(armytb);
	if(iRet == DB_Proxy::DB_EOF)
	{
		rep_msg.serialize(req_msg->agent,iMapID,req_msg->from,3);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 3;
	}
	else if(iRet == DB_Proxy::DB_FAILED)
	{
		rep_msg.serialize(req_msg->agent,iMapID,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 3;
	}
	//如果为运输部队，则判断是否具有物质
	if (armytb.fweapon_ >=175 && armytb.fweapon_ <= 177)
	{
		LoadMaterial loadmat;
		int count = 0;
		char *buf = 0;
		loadmat.setmasterid(req_msg->from, armytb.armyid_);
		if (g_Proxy.db_multi_select(loadmat, buf, count) == DB_Proxy::DB_SUCCEED)
		{
			delete[] buf;
			buf = 0;
			rep_msg.serialize(req_msg->agent,iMapID,req_msg->from,5);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			return 3;
		}
	}
	//当前部队数量计入当前地图本人据点内
	PlayerSoldierRace soldiertb,oldSoldier;
	oldSoldier.setmasterid(iMapID,city.cityid_);
	oldSoldier.setdetailid(req_msg->from);
	iRet = g_Proxy.db_select(oldSoldier);//用于判断该城市是否已有士兵数据

	memset(&soldiertb,0,sizeof(PlayerSoldierRace));
	soldiertb.setmasterid(iMapID,city.cityid_);
	soldiertb.setdetailid(req_msg->from);
	int oldRace = 0;
	switch(armytb.raceid_)
	{
	case 1:
		oldRace = oldSoldier.human_;
		soldiertb.human_ = armytb.firstcount_;
		break;
	case 2:
		oldRace = oldSoldier.fairy_;
		soldiertb.fairy_ = armytb.firstcount_;
		break;
	case 3:
		oldRace = oldSoldier.mannikin_;
		soldiertb.mannikin_ = armytb.firstcount_;
		break;
	case 4:
		oldRace = oldSoldier.dandiprat_;
		soldiertb.dandiprat_ = armytb.firstcount_;
		break;
	case 5:
		oldRace = oldSoldier.halfanimal_;
		soldiertb.halfanimal_ = armytb.firstcount_;
		break;
	case 6:
		oldRace = oldSoldier.cattle_;
		soldiertb.cattle_ = armytb.firstcount_;
		break;
	case 7:
		oldRace = oldSoldier.blackfairy_;
		soldiertb.blackfairy_ = armytb.firstcount_;
		break;
	case 8:
		oldRace = oldSoldier.decpesrson_;
		soldiertb.decpesrson_ = armytb.firstcount_;
		break;
	};

	if(iRet == DB_Proxy::DB_SUCCEED)
	{
		iRet = g_Proxy.db_increase(soldiertb);
	}
	else
	{
		soldiertb.completetimes_ = IceUtil::Time::now().toSeconds();
		iRet = g_Proxy.db_insert(soldiertb);
	}
	if(iRet != DB_Proxy::DB_SUCCEED)
	{//退兵失败
		rep_msg.serialize(req_msg->agent,iMapID,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 4;
	}
	PlayerOfficerTable officer;
	officer.setmasterid(req_msg->from);
	officer.setdetailid(armytb.officerid_);
	g_Proxy.db_select(officer);
	ISystemLog::getInstance()->playerSoldier_UnlayArmy_Log(officer.playerid_,officer.officerid_,officer.officername_,
		iMapID,city.cityid_,armytb.raceid_,oldRace,oldRace + armytb.firstcount_,armytb.firstcount_);
	/*
	//修改指挥官状态为空闲
	PlayerOfficerTable officertb,updateofficertb;
	officertb.setmasterid(req_msg->from);
	officertb.setdetailid(armytb.officerid_);
	if(g_Proxy.db_select(officertb) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,iMapID,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		LOG_TRACE_EX(false,("update officer(%d - %d) state fault.\n",req_msg->from,armytb.officerid_));
		return 2;
	}
	updateofficertb.setmasterid(req_msg->from);
	updateofficertb.setdetailid(armytb.officerid_);
	updateofficertb.state_ = -officertb.state_;
	updateofficertb.armyid_ = -officertb.armyid_;
	updateofficertb.mapid_ = -officertb.mapid_;
	updateofficertb.cityid_ = -officertb.cityid_;
	if(g_Proxy.db_increase(updateofficertb) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,iMapID,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		LOG_TRACE_EX(false,("update officer(%d - %d) state fault.\n",req_msg->from,armytb.officerid_));
		return 4;
	}
	rep_msg.serialize(req_msg->agent,iMapID,req_msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	DeleteArmyInfo(req_msg->agent,rep_msg.from,req_msg->ArmyID);
	rep_msg.serialize(req_msg->agent,iMapID,req_msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);

	//发送创建部队信息至大地图，暂时由郡服务器返回，避免大地图负担add by xujincai 20090225
	ModifyArmyState_Response modifyarmy_rep;
	modifyarmy_rep.serialize(req_msg->agent,req_msg->from,iArmyID,iMapID,0,ARMY_ANNIHILATE,iMapID);
	asynch_write(modifyarmy_rep.agent,(char*)&modifyarmy_rep,modifyarmy_rep.length);

	*/
	DeleteArmyInfo(req_msg->agent,req_msg->from,req_msg->ArmyID);
	//仓库容量
	PlayerResource playerresourcetb;
	playerresourcetb.setmasterid(city.cityid_,city.mapid_);
	playerresourcetb.setdetailid(req_msg->from);
	if(g_Proxy.db_select(playerresourcetb) != DB_Proxy::DB_SUCCEED)
	{//失败
		//rep_msg.serialize(req_msg->agent,iMapID,req_msg->from,111);
		//asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 0;
	}

	//仓库已满，直接丢弃
	if(playerresourcetb.content_ <=0)
	{
		return 0;
	}
	int chgValue = 0;
	//退回武器WEAPON_ITEM
	int iContent = playerresourcetb.content_;
	ProductionAmount weaponAmount,updateWeaponAmout;
	if(armytb.fweapon_ >0)
	{
		int itemType = WEAPON_ITEM;
		if(armytb.fweapon_ >=150)
		{
			itemType = SPECIAL_TOOL_ITEM;
			weaponAmount.setmasterid(iMapID,city.cityid_,req_msg->from,SPECIAL_TOOL_ITEM);
			updateWeaponAmout.setmasterid(iMapID,city.cityid_,req_msg->from,SPECIAL_TOOL_ITEM);
		}
		else if(armytb.fweapon_ >=100)
		{
			itemType = ANIMALS_TAME_ITEM;
			weaponAmount.setmasterid(iMapID,city.cityid_,req_msg->from,ANIMALS_TAME_ITEM);
			updateWeaponAmout.setmasterid(iMapID,city.cityid_,req_msg->from,ANIMALS_TAME_ITEM);
		}
		else
		{
			itemType = WEAPON_ITEM;
			weaponAmount.setmasterid(iMapID,city.cityid_,req_msg->from,WEAPON_ITEM);
			updateWeaponAmout.setmasterid(iMapID,city.cityid_,req_msg->from,WEAPON_ITEM);
		}
		weaponAmount.setdetailid(armytb.fweapon_);
		updateWeaponAmout.setdetailid(armytb.fweapon_);
		iRet = g_Proxy.db_select(weaponAmount);

		iContent -= armytb.firstcount_;
		chgValue = armytb.firstcount_;
		if(iContent >=0)
		{
			updateWeaponAmout.productioncount_ = armytb.firstcount_;
		}
		else
		{
			iContent = 0;
			chgValue = playerresourcetb.content_;
			updateWeaponAmout.productioncount_ = playerresourcetb.content_;
		}
		if(iRet == DB_Proxy::DB_SUCCEED)
		{
			if(g_Proxy.db_increase(updateWeaponAmout) != DB_Proxy::DB_SUCCEED)
			{
				LOG_TRACE_EX(false,("ReqUnlayArmy:退回武器失败(%d - %d)\n",req_msg->from,armytb.officerid_));
				iContent = playerresourcetb.content_; 
			}
		}
		else
		{
			if(g_Proxy.db_insert(updateWeaponAmout) != DB_Proxy::DB_SUCCEED)
			{
				LOG_TRACE_EX(false,("ReqUnlayArmy:退回武器失败(%d - %d)\n",req_msg->from,armytb.officerid_));
				iContent = playerresourcetb.content_; 
			}
		}
		ISystemLog::getInstance()->playerProduce_UnlayArmy_Log(officer.playerid_,officer.officerid_,officer.officername_,
			iMapID,city.cityid_,itemType,armytb.fweapon_,weaponAmount.productioncount_,updateWeaponAmout.productioncount_,
			chgValue);
	}

	//退回防具DEFEND_ITEM
	if(armytb.fdefendtype_ && iContent >0)
	{
		memset(&weaponAmount,0,sizeof(weaponAmount));
		memset(&updateWeaponAmout,0,sizeof(updateWeaponAmout));
		weaponAmount.setmasterid(iMapID,city.cityid_,req_msg->from,DEFEND_ITEM);
		weaponAmount.setdetailid(armytb.fdefendtype_);
		updateWeaponAmout.setmasterid(iMapID,city.cityid_,req_msg->from,DEFEND_ITEM);
		updateWeaponAmout.setdetailid(armytb.fdefendtype_);

		iRet = g_Proxy.db_select(weaponAmount);
		iContent -= armytb.firstcount_;
		if(iContent >=0)
		{
			chgValue = armytb.firstcount_;
			updateWeaponAmout.productioncount_ = armytb.firstcount_;
		}
		else
		{
			iContent = 0;
			chgValue = playerresourcetb.content_;
			updateWeaponAmout.productioncount_ = playerresourcetb.content_;
		}
		if(iRet == DB_Proxy::DB_SUCCEED)
		{
			if(g_Proxy.db_increase(updateWeaponAmout) != DB_Proxy::DB_SUCCEED)
			{
				LOG_TRACE_EX(false,("ReqUnlayArmy:退回防具(%d - %d)\n",req_msg->from,armytb.officerid_));
				iContent = playerresourcetb.content_; 
			}
		}
		else
		{
			if(g_Proxy.db_insert(updateWeaponAmout) != DB_Proxy::DB_SUCCEED)
			{
				LOG_TRACE_EX(false,("ReqUnlayArmy:退回防具(%d - %d)\n",req_msg->from,armytb.officerid_));
				iContent = playerresourcetb.content_; 
			}
		}
		ISystemLog::getInstance()->playerProduce_UnlayArmy_Log(officer.playerid_,officer.officerid_,officer.officername_,
			iMapID,city.cityid_,DEFEND_ITEM,armytb.fdefendtype_,weaponAmount.productioncount_,updateWeaponAmout.productioncount_,
			chgValue);
	}
	if(iContent != playerresourcetb.content_)
	{
		iContent = iContent - playerresourcetb.content_;
		memset(&playerresourcetb,0,sizeof(playerresourcetb));
		playerresourcetb.setmasterid(city.cityid_,city.mapid_);
		playerresourcetb.setdetailid(req_msg->from);
		playerresourcetb.content_ = iContent;
		if(g_Proxy.db_increase(playerresourcetb) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE_EX(false,("ReqUnlayArmy:玩家%d减少仓库容量失败\n",req_msg->from));
		}
	}
	return 0;
}
//删除部队
int CLevySoldiers::ReqDeleteArmy(const char *msg)
{
	LPDeleteArmy_Request req_msg = (LPDeleteArmy_Request)msg;
	LOG_TRACE_EX((req_msg->length == sizeof(DeleteArmy_Request)),("ReqDeleteArmy-->消息包非法\n"));
	if(req_msg->length != sizeof(DeleteArmy_Request))
	{
		return 1;
	}
	DeleteArmy_Response rep_msg;
	int iMapID = req_msg->to;
	int iCode = 0;
	//查询该部队的指挥官信息
	ArmyTable armytb;
	armytb.setmasterid(iMapID);
	armytb.setdetailid(req_msg->ArmyID);
	if(g_Proxy.db_select(armytb) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,iMapID,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	/*
	//修改指挥官状态为空闲
	PlayerOfficerTable officertb,updateofficertb;
	officertb.setmasterid(req_msg->from);
	officertb.setdetailid(armytb.officerid_);
	if(g_Proxy.db_select(officertb) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,iMapID,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		LOG_TRACE_EX(false,("update officer(%d - %d) state fault.\n",req_msg->from,armytb.officerid_));
		return 2;
	}
	updateofficertb.setmasterid(req_msg->from);
	updateofficertb.setdetailid(armytb.officerid_);
	updateofficertb.state_ = -officertb.state_;
	updateofficertb.armyid_ = -officertb.armyid_;
	updateofficertb.mapid_ = -officertb.mapid_;
	updateofficertb.cityid_ = -officertb.cityid_;
	if(g_Proxy.db_increase(updateofficertb) != DB_Proxy::DB_SUCCEED)
	{
	rep_msg.serialize(req_msg->agent,iMapID,req_msg->from,111);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	LOG_TRACE_EX(false,("update officer(%d - %d) state fault.\n",req_msg->from,armytb.officerid_));
	return 2;
	}
	officertb.state_ = 0;
	officertb.armyid_ = 0;
	officertb.mapid_ = 0;
	officertb.mapid_ = 0;
	officertb.cityid_ = 0;
	if(g_Proxy.db_update(officertb,officertb) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,iMapID,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		LOG_TRACE_EX(false,("update officer(%d - %d) state fault.\n",req_msg->from,armytb.officerid_));
		return 2;
	}
	iCode  = 0;
	/*删除部队
	if(g_Proxy.db_delete(armytb) != DB_Proxy::DB_SUCCEED)
	{
	iCode =111;
	printf("Delete army(%d - %d) fault.\n",req_msg->from,armytb.officerid_);
	}
	rep_msg.serialize(req_msg->agent,iMapID,req_msg->from,iCode);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	*/

	//如果为运输部队，则删除马车所装载的物质
	if (armytb.fweapon_ >=175 && armytb.fweapon_ <= 177)
	{
		LoadMaterial loadmat;
		LoadMaterial *pLoadMatList = 0;
		int count = 0;
		char *buf = 0;
		loadmat.setmasterid(req_msg->from, armytb.armyid_);
		if (g_Proxy.db_multi_select(loadmat, buf, count) == DB_Proxy::DB_SUCCEED)
		{
			count = count/sizeof(LoadMaterial);
			pLoadMatList = (LoadMaterial*)buf;
			for(int i=0;i<count;i++)
			{
				g_Proxy.db_delete(pLoadMatList[i]);
			}

			delete[] buf;
			buf = 0;
		}
	}
	/*
	//发送创建部队信息至大地图，暂时由郡服务器返回，避免大地图负担add by xujincai 20090225
	ModifyArmyState_Response modifyarmy_rep;
	modifyarmy_rep.serialize(req_msg->agent,req_msg->from,req_msg->ArmyID,iMapID,0,ARMY_ANNIHILATE,iMapID);
	asynch_write(modifyarmy_rep.agent,(char*)&modifyarmy_rep,modifyarmy_rep.length);
	*/
	return 0;
}
void CLevySoldiers::DeleteArmyInfo(int agent,unsigned long playerID,unsigned long ArmyID)
{
	LPDeleteArmy_Request req_msg = new DeleteArmy_Request;
	req_msg->serialize(playerID,g_MapInfo->getMapID(),ArmyID);
	req_msg->agent = agent;
	g_IFightProcesser->pushDataToManager((char *) req_msg);
}
void CLevySoldiers::AddArmyInfo2Fight(int agent,int playerid,int mapid,int armyid)
{
	LPFIGHT_CreateArmy_Request req_msg = new FIGHT_CreateArmy_Request;
	req_msg->serialize(agent,playerid,mapid,armyid);
	g_IFightProcesser->pushDataToManager((char *) req_msg);
}
//组建国防军
int CLevySoldiers::ReqCreateTirrier(const char *msg)
{
	LPNotify_CreateTirrier_Request req_msg = (LPNotify_CreateTirrier_Request)msg;
	Notify_CreateTirrier_Response rep_msg;
	int officerid = req_msg->CountryID * 100;
	int iRet =g_DataServer.AddArmyInfo(2," ",officerid,officerid,0,req_msg->WeaponType,req_msg->WeaponLevel,req_msg->DefendType,req_msg->DefendLevel,req_msg->SoldierType,100);
	rep_msg.serialize(req_msg->agent,req_msg->from,iRet,req_msg->CountryID,req_msg->TirrierType);
	event_notify(MDM_GP_MAP_CONSORTIA,(char*)&rep_msg,rep_msg.length);
	return 0;
}
