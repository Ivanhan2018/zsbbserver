#include "ResearchManager.h"
#include "log.hpp"
#include "Time_Protocol.h"
#include "LoadDBInfo.h"

#include "DataServer.h"
#pragma warning(disable:4100)
#pragma warning(disable:4189)
#pragma warning(disable:4127)
#pragma warning(disable:4244)
#pragma warning(disable:4389)
CResearchManager::CResearchManager()
{
}
CResearchManager::~CResearchManager()
{
}
void CResearchManager::SetMapID(int mapid)
{
	if(mapid == MDM_GP_MAP_RESEARCH)
	{
		//InitResearchActivity();
	}
}

//得到所有已经研究了的道具列表
bool CResearchManager::OnGetAllResearchList(int handle, unsigned long lPlayerID)
{
	OnGetResearchingItem(handle, lPlayerID);

	char * buf = 0;
	int count = 0, iItem = 0, Index = 0;;
	PlayerChatRoomTable PlayerScience;
	memset(&PlayerScience, 0, sizeof(PlayerChatRoomTable));
	PlayerScience.setid(lPlayerID);
	if(g_Proxy.db_select(PlayerScience) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("CResearchManager::OnGetAllResearchList %ld 科研 读取金钱  科技点 失败!!!!\n",lPlayerID));
		return false;
	}
	
	char * tmp = new char[sizeof(ITEM_RESEARCH_LIST) + sizeof(tagItemResearch)*300];//大点的科研数目
	if (tmp == NULL) return false;
	ITEM_RESEARCH_LIST * SendData = (ITEM_RESEARCH_LIST *)tmp;
	(*SendData)[Index].iType = 0;
	(*SendData)[Index].iItemID = 0;
	(*SendData)[Index].iLevel = 0;
	(*SendData)[Index].iCount = 0;
	(*SendData)[Index].bResearch = 0;

	ResearchItem ItemList;
	for(int iType = WEAPON_ITEM; iType <= SPECIAL_TOOL_ITEM; iType++)
	{
		buf = 0;
		count = 0;
		memset(&ItemList, 0, sizeof(ResearchItem));
		ItemList.setmasterid(lPlayerID, iType);
		if (g_Proxy.db_multi_select(ItemList, buf, count) != DB_Proxy::DB_SUCCEED)
		{
			continue;
		}
		iItem = count/sizeof(ResearchItem);
		ResearchItem * pItemList = (ResearchItem *)buf;
		int n = 0;
		for (; n < iItem; n++,Index++)
		{
			(*SendData)[Index].iType = iType;
			(*SendData)[Index].iItemID = pItemList[n].itemid_;
			(*SendData)[Index].iLevel = pItemList[n].level_;
			(*SendData)[Index].iCount = pItemList[n].count_;
			
			ResearchConfigure* pResearchParam = g_LoadDBinfo.GetResearchConfig((*SendData)[Index].iItemID);
			if (pResearchParam == NULL) 
			{
				//add by xujincai 20081216
				if(buf)
				{
					delete[] buf;
					buf = 0;
				}
				if(tmp)
				{
					delete[] tmp;
					tmp = 0;
				}
				return 0;
			}
			if ((pResearchParam->science_ <= PlayerScience.science_)&&(pResearchParam->money_ <= PlayerScience.money_))
				(*SendData)[Index].bResearch = true;
			if (Index>=299) break;
		}
		//add by xujincai 20081216
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
	}
	SendData->serialize(Index, MDM_GP_MAP_RESEARCH, lPlayerID,EN_S_SEND_ALL_RESEARCH_ITEM);
	int ret = asynch_write(handle, (char *)SendData, SendData->length);
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
//得到正在科研中的道具技术
bool CResearchManager::OnGetResearchingItem(int handle, unsigned long lPlayerID)
{
	int ret = 0;
	char * tmp = new char[sizeof(RESEARCHING_ITEM_LIST) + sizeof(tagResearchingItemList)*2];//
	RESEARCHING_ITEM_LIST * SendData = (RESEARCHING_ITEM_LIST *)tmp;
	(*SendData)[0].lSortID = 0;
	(*SendData)[0].iType = 0;
	(*SendData)[0].iItemID = 0;
	(*SendData)[0].iLevel = 0;
	(*SendData)[0].iCompletetime = 0;

	ResearchActivity_Iterator it =m_mapResearchItem.find(lPlayerID);
	if (it != m_mapResearchItem.end())
	{
		ResearchConfigure* pResearchParam = g_LoadDBinfo.GetResearchConfig(it->second.iItemID);
		if (pResearchParam == NULL) 
		{
			//add by xujincai 20081216
			if(tmp)
			{
				delete[] tmp;
				tmp = 0;
			}
			return 0;
		}

		int64 lTimeLen = IceUtil::Time::now().toSeconds();
		int lTimeL = int(it->second.lCompleteTime - lTimeLen);
		if (lTimeL <= 0)
		{
			ResearchItem ProAmount,SrcItem;
			memset(&ProAmount,0,sizeof(ResearchItem));
			memset(&SrcItem,0,sizeof(ResearchItem));

			ProAmount.level_ = it->second.iLevel;
			ProAmount.itemid_ = it->second.iItemID;
			ProAmount.count_ = 1;
			ProAmount.type_ = it->second.iTypeID;
			ProAmount.playerid_ = lPlayerID;

			g_Proxy.db_insert(ProAmount);
			ResearchActivity resItem;
			resItem.setmasterid(lPlayerID);
			resItem.setdetailid(ProAmount.type_,ProAmount.itemid_);
			g_Proxy.db_delete(resItem);
			m_mapResearchItem.erase(lPlayerID);
		}
		else
		{		
			(*SendData)[0].lSortID = 0;
			(*SendData)[0].iType = it->second.iTypeID;
			(*SendData)[0].iItemID = it->second.iItemID;
			(*SendData)[0].iLevel = it->second.iLevel;
			(*SendData)[0].iCompletetime = lTimeL;
		}
	}
	SendData->serialize(1, MDM_GP_MAP_RESEARCH, lPlayerID,EN_S_RESEARCHING_ITEM);
	ret = asynch_write(handle, (char *)SendData, SendData->length);
	if (tmp != NULL)
	{
		delete [] tmp;	
		tmp = 0;
	}
	return ret;
}
//科研道具技术
int CResearchManager::OnResearchItem(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	RESEARCH_ITEM * pResItem = (RESEARCH_ITEM *)pDataBuffer;
	int iItemID = pResItem->iItemID;
	int iLevel = pResItem->iLevel;
	int iType = pResItem->iType;
	int ret = 0;
	int iLowerID = 1;
	ERROR_INFO ErrorInfo;
	memset(&ErrorInfo,0,sizeof(ERROR_INFO));

	ResearchItem ResItem;
	ResItem.setmasterid(lPlayerID, iType);
	ResItem.setdetailid(iItemID);
	if (g_Proxy.db_select(ResItem) == DB_Proxy::DB_SUCCEED)
		return 0;
	ResearchConfigure* pResearchParam = g_LoadDBinfo.GetResearchConfig(iItemID);
	if (pResearchParam == NULL) return 0;
	if ( iType == DEFEND_ITEM)
	{
		iLowerID = 201;
	}
	else if ( iType == ANIMALS_TAME_ITEM)
	{
		iLowerID = 100;
	}
	else if ( iType == SPECIAL_TOOL_ITEM)
	{

		iLowerID = 150;
	}

	ResearchItem PlayerResearch;
	if ((iItemID > iLowerID) && (pResearchParam->level_ > 1))
	{
		PlayerResearch.setmasterid(lPlayerID, iType);
		PlayerResearch.setdetailid(iItemID-1);
		int dbRet = g_Proxy.db_select(PlayerResearch);
		if (dbRet == DB_Proxy::DB_EOF) //首先需要科研底级的科研技能
		{
			ErrorInfo.serialize(EN_RESEARCH_ITEM_LOWER, MDM_GP_MAP_RESEARCH, lPlayerID);
			asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
			return 0;	
		}
	}

	ResearchActivity_Iterator it =m_mapResearchItem.find(lPlayerID);
	if (it != m_mapResearchItem.end())
	{
		ErrorInfo.serialize(EN_RESEARCHING_ITEM, MDM_GP_MAP_RESEARCH, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	//已科研
	PlayerResearch.setmasterid(lPlayerID, iType);
	PlayerResearch.setdetailid(iItemID);
	if(g_Proxy.db_select(PlayerResearch) == DB_Proxy::DB_SUCCEED)
	{
		return 0;
	}
	PlayerChatRoomTable PlayerRes;
	memset(&PlayerRes, 0, sizeof(PlayerChatRoomTable));
	PlayerRes.setid(lPlayerID);
	if (g_Proxy.db_select(PlayerRes) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("CResearchManager::OnResearchItem %ld 科研道具技术时 -- 获取玩家金钱,科技点失败 db_select(PlayerRes)\n",lPlayerID));
		return 0;
	}
	
	if (pResearchParam->money_ > PlayerRes.money_)
	{
		ErrorInfo.serialize(EN_MONEY_COUNT, MDM_GP_MAP_RESEARCH, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	if (pResearchParam->science_ > PlayerRes.science_)
	{
		ErrorInfo.serialize(EN_SCIENCE_COUNT, MDM_GP_MAP_RESEARCH, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	memset(&PlayerRes, 0, sizeof(PlayerChatRoomTable));
	PlayerRes.money_ = -pResearchParam->money_;
	PlayerRes.science_ = -pResearchParam->science_;
	PlayerRes.setid(lPlayerID);
	if (g_Proxy.db_increase(PlayerRes)  != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("CResearchManager::OnResearchItem %ld 科研道具技术时 --   失败 db_update(PlayerRes,PlayerRes)\n",lPlayerID));
		return 0;
	}
	RESEARCH_ITEM_NODE * pResearchNode = new RESEARCH_ITEM_NODE;
	pResearchNode->serialize(lPlayerID, iType, iItemID, iLevel, 1, EN_TIMER_RESEARCH);
	//modify by xujincai 20090107 速率使用百分比,同时为了减少乘法运算
	int TimeL = (int)pResearchParam->completetimes_;//*100)/g_SpeedCard.GetResearchValue(lPlayerID);
	int iCurRate = g_SpeedCard.GetResearchValue(lPlayerID);
	if(iCurRate != 100)
	{
		TimeL = TimeL * 100/iCurRate;
	}
	if (TimeL < 1)
		TimeL = 1;

	pResearchNode->TimerID = set_timer(TimeL, (const void *)pResearchNode);

	tagResearchNode ResNode;
	ResNode.iItemID = iItemID;
	ResNode.iLevel = iLevel;
	ResNode.iTypeID = iType;
	ResNode.TimerID = pResearchNode->TimerID;
	ResNode.lCompleteTime = IceUtil::Time::now().toSeconds()+TimeL;

	m_mapResearchItem.insert(make_pair(lPlayerID, ResNode));
	/*
	ResearchActivity *pResearchItem = new ResearchActivity;
	pResearchItem->setmasterid(lPlayerID);
	pResearchItem->setdetailid(iType,iItemID);
	pResearchItem->level_ = iLevel;
	pResearchItem->endtime_ = IceUtil::Time::now().toSeconds()+TimeL;
	m_mapResearchItem[lPlayerID] = pResearchItem;*/
	ResearchActivity resItem;
	resItem.setmasterid(lPlayerID);
	resItem.setdetailid(iType,iItemID);
	resItem.level_ = iLevel;
	resItem.endtime_ = ResNode.lCompleteTime;
	if(g_Proxy.db_insert(resItem) != DB_Proxy::DB_SUCCEED)
	{
		m_mapResearchItem.erase(lPlayerID);
		LOG_TRACE_EX(false,("CResearchManager::OnResearchItem %ld 科研道具技术时 --   失败 db_insert(resItem)\n",lPlayerID));
		return 0;
	}

	SUCCEED_INFO SucceedInfo;
	SucceedInfo.serialize(0, 0, iItemID, iLevel, 1, EN_START_RESEARCH, MDM_GP_MAP_RESEARCH, lPlayerID);
	ret = asynch_write(handle, (char *)&SucceedInfo, SucceedInfo.length);
	//20100409
	PlayerOfficerTable officer;
	officer.setmasterid(lPlayerID);
	officer.setdetailid(lPlayerID);
	g_Proxy.db_select(officer);
	ISystemLog::getInstance()->playerMoney_NewResearch_Log(officer.playerid_,officer.officername_,
		PlayerRes.money_ + pResearchParam->money_,PlayerRes.money_,pResearchParam->money_,iItemID,iLevel);
	ISystemLog::getInstance()->playerScience_NewResearch_Log(officer.playerid_,officer.officername_,
		PlayerRes.science_ + pResearchParam->science_,PlayerRes.science_,pResearchParam->science_,iItemID,iLevel);

	return ret;
}
//科研时间事件
void CResearchManager::OnResearchTimeEvent(void * pDataBuffer, int iDataSize)
{
	if (iDataSize != sizeof(RESEARCH_ITEM_NODE))
	{
		LOG_TRACE_EX(false,("CResearchManager::OnResearchTimeEvent-->消息包非法\n"));
		return;
	}

	RESEARCH_ITEM_NODE * pResearch = (RESEARCH_ITEM_NODE *)pDataBuffer;
	
	int iType = pResearch->iType;
	int iItemID = pResearch->iItemID;
	unsigned long lPlayerID = pResearch->lPlayerID;
	int iLevel = pResearch->iLevel;

	ResearchActivity_Iterator it =m_mapResearchItem.find(lPlayerID);
	if(it == m_mapResearchItem.end())
	{
		return;
	}
	if(it->second.TimerID != pResearch->TimerID || it->second.iItemID != pResearch->iItemID)
	{
		return;
	}
	ResearchActivity resItem;
	resItem.setmasterid(lPlayerID);
	resItem.setdetailid(iType,iItemID);
	if(g_Proxy.db_delete(resItem) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("CResearchManager::OnResearchTimeEvent-->删除科研中的记录失败\n"));
		return;
	}
	m_mapResearchItem.erase(it);


	ResearchItem ProAmount,SrcItem;
	memset(&ProAmount,0,sizeof(ResearchItem));
	memset(&SrcItem,0,sizeof(ResearchItem));

	ProAmount.level_ = iLevel;
	ProAmount.itemid_ = iItemID;
	ProAmount.count_ = 1;
	ProAmount.type_ = iType;
	ProAmount.playerid_ = lPlayerID;

	if (g_Proxy.db_insert(ProAmount) == DB_Proxy::DB_SUCCEED)
	{
		//下发给玩家通知消息
		g_DataServer.broadMsgAndNotify(lPlayerID,0,iItemID,iLevel,iType,EN_S_NOTIFYRESEARCH,EN_S_BROADRESEARCH,MDM_GP_MAP_RESEARCH);
	}
	else
	{
		LOG_TRACE_EX(false,("--CResearchManager::OnResearchTimeEvent --玩家 %ld 科研时间事件道具技术类型(%d) ID (%d) 等级 (%d) db_insert(ProAmount) error \n",lPlayerID,iType,iItemID,iLevel));

	}
}
//道具升级
void CResearchManager::ItemUpGrade(unsigned long lPlayerID, int iType, int iLevel)
{
	char * buf = 0;
	char * Buffer = 0;
	int count = 0, iItem = 0, iCount = 0;
	PlayerCity PlayerCityList;
	memset(&PlayerCityList, 0, sizeof(PlayerCity));
	
	PlayerCityList.setmasterid(lPlayerID);
	if (g_Proxy.db_multi_select(PlayerCityList, buf, count) != DB_Proxy::DB_SUCCEED)
	{
		return;
	}
	iItem = count/sizeof(PlayerCity);
	PlayerCity * pPlayerCityList = (PlayerCity *)buf;

	ProductionAmount ProAmount;
	memset(&ProAmount,0,sizeof(ProductionAmount));
	for (int n = 0; n < iItem; n++)
	{
		ProAmount.setmasterid(pPlayerCityList[n].mapid_, pPlayerCityList[n].cityid_, lPlayerID, iType);
		if (g_Proxy.db_multi_select(ProAmount, Buffer, iCount) == DB_Proxy::DB_SUCCEED)
		{
			int iIndex = iCount/sizeof(ProductionAmount);
			ProductionAmount * pUpGrade = (ProductionAmount *)Buffer;
			for (int i = 0; i < iIndex; i++)
			{
				//pUpGrade[i].level_ = iLevel;
				if (g_Proxy.db_update(pUpGrade[i],pUpGrade[i]) != DB_Proxy::DB_SUCCEED)
				{
					printf("玩家%ld在郡内%ld城市中%ld升级%d等级%d时失败\n",lPlayerID,pPlayerCityList[n].mapid_, pPlayerCityList[n].cityid_,iType,iLevel);
				}
			}
		}
	}
	if (buf != NULL)
	{
		delete [] buf;
		buf = 0;
	}
	if (Buffer != NULL)
	{
		delete [] Buffer;
		Buffer = 0;
	}
}
//取消科研道具技术
int CResearchManager::OnCancelResearchItem(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	CANCEL_RESEARCH * pCancelRes = (CANCEL_RESEARCH *)pDataBuffer;
	
	ResearchActivity_Iterator it =m_mapResearchItem.find(lPlayerID);
	if (it == m_mapResearchItem.end())
		return 0;
	int iItemID = it->second.iItemID;
	int iLevel = it->second.iLevel;
	ResearchConfigure* pResearchParam = g_LoadDBinfo.GetResearchConfig(iItemID);
	if (pResearchParam == NULL) return 0;
	PlayerChatRoomTable PlayerRes;
	memset(&PlayerRes, 0, sizeof(PlayerChatRoomTable));
	PlayerRes.money_ = pResearchParam->money_;
	PlayerRes.science_ = pResearchParam->science_;
	PlayerRes.setid(lPlayerID);
	if (g_Proxy.db_increase(PlayerRes)  != DB_Proxy::DB_SUCCEED)
	{
		return 0;
	}
	cancel_timer(it->second.TimerID);
	ResearchActivity resItem;
	resItem.setmasterid(lPlayerID);
	resItem.setdetailid(it->second.iTypeID,it->second.iItemID);
	if(g_Proxy.db_delete(resItem) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("CResearchManager::OnCancelResearchItem-->删除科研中的记录失败\n"));
		return 0;
	}
	m_mapResearchItem.erase(it);
	SUCCEED_INFO SucceedInfo;

	SucceedInfo.serialize(1, 1, iItemID, pCancelRes->iLevel, 1, EN_CANCEL_RESEARCH_ITEM,MDM_GP_MAP_RESEARCH, lPlayerID);
	int ret = asynch_write(handle, (char *)&SucceedInfo, SucceedInfo.length);
	//20100409
	PlayerOfficerTable officer;
	officer.setmasterid(lPlayerID);
	officer.setdetailid(lPlayerID);
	g_Proxy.db_select(officer);
	ISystemLog::getInstance()->playerMoney_CancelResearch_Log(officer.playerid_,officer.officername_,
		PlayerRes.money_ - pResearchParam->money_,PlayerRes.money_,pResearchParam->money_,iItemID,iLevel);
	ISystemLog::getInstance()->playerScience_CancelResearch_Log(officer.playerid_,officer.officername_,
		PlayerRes.science_ - pResearchParam->science_,PlayerRes.science_,pResearchParam->science_,iItemID,iLevel);
	return ret;
}

//读取武器科研配置表
void CResearchManager::ReadWeaponConfig(int iType)
{
}
void CResearchManager::OnResearchSpeed(unsigned long lPlayerID, int iSortID, int iValue)
{
	//add by xujincai 20090119
	map<unsigned long, tagResearchNode>::iterator iter = m_mapResearchItem.find(lPlayerID);
	if(iter == m_mapResearchItem.end())
		return;

	__int64 iTimes= (iter->second.lCompleteTime - IceUtil::Time::now().toSeconds()) ;//剩余总时间
	iTimes = iTimes *(100-iValue)/100;//加速后的剩余时间

	RESEARCH_ITEM_NODE *pTimerParam = new RESEARCH_ITEM_NODE;
	pTimerParam->serialize(lPlayerID,iter->second.iTypeID,iter->second.iItemID,iter->second.iLevel,1,EN_TIMER_RESEARCH);
	set_timer((int)iTimes, (const void *)pTimerParam);
}
int64 CResearchManager::GetCurrentTime()
{
	return IceUtil::Time::now().toLongDateTime();

}
//立即完成
void CResearchManager::OnImmedComplete(LPImmedComplete_Request msg)
{
	LPImmedComplete_Request req_msg = (LPImmedComplete_Request)msg;
	if(req_msg->length != sizeof(ImmedComplete_Request))
	{
		LOG_TRACE_EX(false,("CResearchManager::OnImmedComplete-->消息包非法\n"));
		return;
	}
	Notify_ImmedComplete_Request notify_msg;
	ImmedComplete_Response rep_msg;
	map<unsigned long, tagResearchNode>::iterator it = m_mapResearchItem.find(req_msg->from);
	if(it == m_mapResearchItem.end())//没有科研
	{
		rep_msg.serialize(req_msg->agent,MDM_GP_MAP_RESEARCH,req_msg->from,111,req_msg->Type,req_msg->SortID,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	__int64 iTimes = it->second.lCompleteTime - IceUtil::Time::now().toSeconds();//剩余时间,一分钟一个GO点
	iTimes /=900;
	if(iTimes <=0)
		iTimes = 1;
	notify_msg.serialize(req_msg->agent,req_msg->from,Research_Speed_Card,req_msg->MapID,req_msg->CityID,it->second.iItemID,
		req_msg->SortID,it->second.iLevel,(int)iTimes,it->second.iTypeID);
	if(event_notify(MDM_GP_MAP_CHARGE,(char*)&notify_msg,notify_msg.length) ==0)
	{//通知失败
		rep_msg.serialize(req_msg->agent,MDM_GP_MAP_RESEARCH,req_msg->from,111,req_msg->Type,req_msg->SortID,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	}
}
//处理收费系统返回的通知消息
void CResearchManager::OnImmedCompleteNotifyMsg(LPNotify_ImmedComplete_Response msg)
{
	ImmedComplete_Response rep_msg;
	if(msg->Error_Code != 0)
	{//扣费失败
		rep_msg.serialize(msg->agent,MDM_GP_MAP_RESEARCH,msg->from,msg->Error_Code,msg->Param.Type,msg->Param.SortID,0);
	}
	else
	{
		//m_mapResearchItem.erase(msg->from);
		//OnResearchTimeEvent   RESEARCH_ITEM_NODE
		map<unsigned long, tagResearchNode>::iterator it = m_mapResearchItem.find(msg->from);
		RESEARCH_ITEM_NODE research_item;
		research_item.serialize(msg->from,msg->Param.ProductionType,msg->Param.ItemID,msg->Param.Level,1,0);
		if(it != m_mapResearchItem.end())
		{
			research_item.TimerID = it->second.TimerID;
		}
		OnResearchTimeEvent(&research_item,research_item.length);
		rep_msg.serialize(msg->agent,MDM_GP_MAP_RESEARCH,msg->from,msg->Error_Code,msg->Param.Type,msg->Param.SortID,msg->Param.GOCount);
	}
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
}

//恢复科研
void CResearchManager::InitResearchActivity(DB_Proxy *pDBProxy)
{
	ResearchActivity resActivity;
	ResearchActivity *pResActivityList = 0;
	char *buf = 0;
	int count = 0;
	if(pDBProxy->db_select_all(resActivity,buf,count) != DB_Proxy::DB_SUCCEED)
		return;
	count = count/sizeof(ResearchActivity);
	pResActivityList = (ResearchActivity*)buf;
	tagResearchNode ResNode;
	RESEARCH_ITEM_NODE * pResearchNode = 0;
	int iTimes = 0;
	for(int i =0;i<count;i++)
	{
		iTimes = int(pResActivityList[i].endtime_ - IceUtil::Time::now().toSeconds());
		if(iTimes <=0)
			iTimes = 5;
		pResearchNode = new RESEARCH_ITEM_NODE;
		pResearchNode->serialize(pResActivityList[i].playerid_, pResActivityList[i].type_, pResActivityList[i].itemid_, pResActivityList[i].level_, 1, EN_TIMER_RESEARCH);
		pResearchNode->TimerID = set_timer(iTimes,(void*)pResearchNode);

		ResNode.iTypeID =  pResActivityList[i].type_;
		ResNode.iItemID =  pResActivityList[i].itemid_;
		ResNode.iLevel =  pResActivityList[i].level_;
		ResNode.TimerID = pResearchNode->TimerID;
		ResNode.lCompleteTime =  pResActivityList[i].endtime_;

		m_mapResearchItem.insert(make_pair(pResActivityList[i].playerid_, ResNode));
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
}