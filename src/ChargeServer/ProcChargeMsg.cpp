#include "ProcChargeMsg.h"
#include "ChargeTask.h"
#include "AutoPlayer.h"
#include "log.hpp"
#include <stdio.h>
#include <string.h>
#pragma warning(disable:4127)
#pragma warning(disable:4389)
#define SUBGODIAN_LOG_SIZE 50
CProcChargeMsg::CProcChargeMsg(CChargeTask *pTask,const char *dbgip,int port):m_pTask(pTask)
{
	ConnectDBG(dbgip,port);
}

CProcChargeMsg::~CProcChargeMsg()
{
	m_DBProxy.fini();
}

bool CProcChargeMsg::ConnectDBG(const char *ip,int port)
{
	m_DBProxy.fini();
	return m_DBProxy.init(ip,(short)port,MDM_GP_MAP_CHARGE);
}

void CProcChargeMsg::ProcMsg(const char *msg)
{
	DISTRIBUTED_HEADER *head = (DISTRIBUTED_HEADER*)msg;
#ifdef PRINT_HINTINFO
	int start = GetTickCount();
	int end = start;
#endif;
	switch(head->command)
	{
	case CGC_C_QUERYMONEY:				//查询Go点
		{
			QueryMoney(msg);
		}
		break;
	case CGC_C_BUYCARD:				//购买卡
		{
			BuyCard(msg);
		}
		break;
	case CGC_C_RESOURCECARD:				//使用基本资源卡（消费卡）
		{
			UseResourceCard(msg);
		}
		break;
	case CGC_C_RESOURCESPEEDCARD:				//生产加速卡
		{
			UseResourceSpeedCard(msg);
		}
		break;
	case CGC_C_RESRUITHEROCARD:				//招募英雄卡
		{
			UseResruitHeroCard(msg);
		}
		break;
	case CGC_C_SPECIALSKILLCARD:				//特殊技能卡
		{
			UseSpecialSkillCard(msg);
		}
		break;
	case CGC_C_OTHERSPEEDCARD:					//建造、科研、征兵加速卡
		{
			UseOtherSpeedCard(msg);
		}
		break;
	case CGC_C_AUTOCARD:						//使用托管卡
		{
			UseAutoCard(msg);
		}
		break;
	case CGC_C_CANCELAUTO:						//取消托管
		{
			CancelAuto(msg);
		}
		break;
	case CGC_C_QUERYAUTOLIST:					//查询
		{
			QueryAutoList(msg);
		}
		break;
	case CGC_C_HEROEQUIPCARD:					//装备卡
		{
			UseHeroEquipCard(msg);
		}
		break;
	case CGC_C_QUERYRESOURCERATE:				//查询当前某生产率
		{
			QueryResourceRate(msg);
		}
		break;
	case CGC_C_EXPANDPACKCARD:
		{
			UseExpandPackCard(msg);
		}
		break;
	case CGC_C_UEEHERORESUME://使用恢复卡
		{
			UseHeroResumeCard(msg);
		}
		break;
	case CGC_C_TIMERNOTIFY:						//时间事件
		{
			ProcTimerMsg(msg);
		}
		break;
	case CGC_S_NOTIFY_BUILDCOMPLETE:			//建造完成通知，有郡地图服务器自动下发通知
		{
			BuildComplete(msg);
		}
		break;
	case CGC_S_NOTIFY_COLLECTRESOURCE:			//资源收集返回消息
		{
			CollectResourceMsg(msg);
		}
		break;
	case CGC_C_EXCHANGE:						//兑换
		{
			ExchangeMoney(msg);
		}
		break;
	case CGC_C_NOTIFY_IMMEDCOMPLETE://业务服务器的通知
		{
			ProImmedCompleteNotify(msg);
		}
		break;
	case CGC_C_INCREASERECOURCERATE://不使用加速卡直接加速
		{
			IncreaseRecourceRate(msg);
		}
		break;
	case CGC_C_NOTIFY_ELECTION://参加竞选扣费
		{
			ElectionNotify(msg);
		}
		break;
	case CGC_C_NOTIFY_QUERYMONEY://任务系统查询GO点
		{
			TaskQueryMoney(msg);
		}
		break;
	case CGC_C_NOTIFY_ADDMONEY://添加GO点
		{
			AddPlayerGODian(msg);
		}
		break;
	case CGC_S_NOTIFY_ADDOFFICERSKILL://使用特殊技能卡，大地图返回
		{
			onAddOfficerSkillResponse(msg);
		}
		break;
	case CGC_C_NOTIFY_USEONECARD:
		{
			onNotifyUseOneCard(msg);
		}
		break;
	case CGC_S_NOTIFY_BUYCARD://购买卡，大地图增加卡返回的消息处理
		{
			onNotifyBuyCard(msg);
		}
		break;
	case CC_C_NOTIFY_UPDATECONSORTIA://军团升级扣除GO点
		{
			onNotifyUpdateConsortia(msg);
		}
		break;
	default:
		{
			printf("Unknown command.\n");
		}
		break;
	}
	
			
#ifdef PRINT_HINTINFO
			end = GetTickCount();
			start = end - start;
			if(start >50)
			{
				printf("command %d use time:%d\n",head->command,start);
			}
#endif
}

void CProcChargeMsg::QueryMoney(const char *msg)
{
	LPQueryMoney_Request req_msg = (LPQueryMoney_Request)msg;
	if(req_msg->length != sizeof(QueryMoney_Request))
	{
		LOG_TRACE(("CProcChargeMsg::QueryMoney-->消息包非法：玩家%d\n",req_msg->from));
		return;
	}
	QueryMoney_Response rep_msg;
	int money;
#ifdef PRINT_HINTINFO
	int start = GetTickCount();
#endif
	int iRet = GetGoDian(req_msg->from,money);
	//if(iRet == -1)
	//{//SOCKET 断开
	//	if(m_pTask->ReconnectWebServer() ==0)
	//	{//重练WebServer成功，再次查询
	//		if(GetGoDian(req_msg->from,money) <=0)
	//			money = -1;
	//	}
	//	else
	//	{
	//		money = -1;
	//	}
	//}
	//else 
	if( iRet<=0)
	{
		//失败
		money = -1;
	}
#ifdef PRINT_HINTINFO
	int end = GetTickCount();
	start = end - start;
	if(start >=50)
	{
		printf("PlayerID: %d,QueryMoney(%d) use time:%d\n",req_msg->from,money, start);
	}
#endif
	rep_msg.serialize(req_msg->agent,req_msg->from,money);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
}

void CProcChargeMsg::BuyCard(const char *msg)
{
	LPBuyCard_Request req_msg = (LPBuyCard_Request)msg;
	if(req_msg->length != sizeof(BuyCard_Request))
	{
		LOG_TRACE(("CProcChargeMsg::BuyCard-->消息包非法：玩家%d\n",req_msg->from));
		return;
	}
	BuyCard_Response rep_msg;
	LPCardParam pCardinfo = m_pTask->GetCardParam(req_msg->ItemID);
	if(pCardinfo == NULL)
	{//不存在该类卡
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	char szbuf[SUBGODIAN_LOG_SIZE] = {0};
	sprintf(szbuf,"购买卡%s(%d)",pCardinfo->CardName,pCardinfo->CardID);
	//扣除Go点
	int iRet = SubGoDian(req_msg->from,pCardinfo->Price,szbuf);
	if(iRet <0)
	{//失败
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	else if(iRet ==0)
	{//Go点不足
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	Notify_BuyCard_Request notifyBuyCard;
	notifyBuyCard.serialize(req_msg->agent,req_msg->from,req_msg->ItemID,pCardinfo->Postion);
	if (event_notify(notifyBuyCard.to,(char*)&notifyBuyCard,notifyBuyCard.length) <=0)
	{//失败
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		char szbuf[SUBGODIAN_LOG_SIZE] = {0};
		sprintf(szbuf,"购买卡%s(%d)失败。",pCardinfo->CardName,pCardinfo->CardID);
		SubGoDian(req_msg->from,-pCardinfo->Price,szbuf);
	}
}

void CProcChargeMsg::UseResourceCard(const char *msg)
{	
	LPUseResourceCard_Request req_msg = (LPUseResourceCard_Request)msg;
	if(req_msg->length != sizeof(UseResourceCard_Request))
	{
		LOG_TRACE(("CProcChargeMsg::UseResourceCard-->消息包非法：玩家%d\n",req_msg->from));
		return;
	}
	UseResourceCard_Response rep_msg;

	PlayerItemStorageTable iteminfo;
	iteminfo.setmasterid(req_msg->OfficerID,req_msg->from);
	iteminfo.setdetailid(req_msg->SortID);
	int iRet = GetItemInfo(iteminfo,&m_DBProxy);
	if(iRet)
	{//获取失败或不存在该道具
		rep_msg.serialize(req_msg->agent,req_msg->from,iRet);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}

	//判断卡信息
	LPCardParam pCardinfo = m_pTask->GetCardParam(iteminfo.itemid_);
	if(pCardinfo == NULL)
	{//不存在该类卡
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	//判断是否为基本资源卡
	if(pCardinfo->Type <=ResourceCard_Begin || pCardinfo->Type >= ResourceCard_End)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,5);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	iRet = 111;

	//首先判断玩家当前城市的仓库容量PlayerResource
	PlayerResource resinfo;
	resinfo.setmasterid(req_msg->CityID,req_msg->MapID);
	resinfo.setdetailid(req_msg->from);
	if(m_DBProxy.db_select(resinfo) == DB_Proxy::DB_SUCCEED)
	{
		if(resinfo.content_ >= pCardinfo->Value)
		{//容量够
			//增加玩家资源,且减少仓库容量
			PlayerResource updateinfo;
			updateinfo.setmasterid(req_msg->CityID,req_msg->MapID);
			updateinfo.setdetailid(req_msg->from);
			updateinfo.content_ = 0 - pCardinfo->Value;
			switch(pCardinfo->Type)
			{
			case Food_Card:			{updateinfo.foodcount_ = pCardinfo->Value;		}break;
			case Wood_Card:			{updateinfo.woodcount_ = pCardinfo->Value;		}break;
			case Skin_Card:			{updateinfo.skincount_ = pCardinfo->Value;		}break;
			case Iron_Card:			{updateinfo.ironcount_ = pCardinfo->Value;		}break;
			case RedStone_Card:		{updateinfo.redstonecount_ = pCardinfo->Value;	}break;
			case YollowStone_Card:	{updateinfo.yollowstonecount_ = pCardinfo->Value;	}break;
			case GreenStone_Card:	{updateinfo.greenstonecount_ = pCardinfo->Value;	}break;
			case BackIron_Card:		{updateinfo.backironcount_ = pCardinfo->Value;	}break;
			default:{}break;
			}
			if(m_DBProxy.db_increase(updateinfo) == DB_Proxy::DB_SUCCEED)
			{
				iRet = 0;
			}
		}
		else
		{
			iRet = 2;
		}
	}
	if(iRet ==0)
	{
		iRet = DelCard(iteminfo);
	}
	rep_msg.serialize(req_msg->agent,req_msg->from,iRet);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	//记录日志
	WriteLogEx(req_msg->from,pCardinfo,UseCard_Log);
}

void CProcChargeMsg::UseResourceSpeedCard(const char *msg)
{	
	LPUseResourceSpeedCard_Request req_msg = (LPUseResourceSpeedCard_Request)msg;
	if(req_msg->length != sizeof(UseResourceSpeedCard_Request))
	{
		LOG_TRACE(("CProcChargeMsg::UseResourceSpeedCard-->消息包非法：玩家%d\n",req_msg->from));
		return;
	}
	UseResourceSpeedCard_Response rep_msg;

	PlayerItemStorageTable iteminfo;
	iteminfo.setmasterid(req_msg->OfficerID,req_msg->from);
	iteminfo.setdetailid(req_msg->SortID);
	int iRet = GetItemInfo(iteminfo,&m_DBProxy);
	if(iRet)
	{//获取失败或不存在该道具
		rep_msg.serialize(req_msg->agent,req_msg->from,iRet);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}

	//判断卡信息
	LPCardParam pCardinfo = m_pTask->GetCardParam(iteminfo.itemid_);
	if(pCardinfo == NULL)
	{//不存在该类卡
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	//判断是否为基本资源生产加速卡
	if(pCardinfo->Type  <= Resource_SpeedCard_Begin || pCardinfo->Type >= Resource_SpeedCard_End)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,5);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	iRet = 111;
	//基本资源提速卡,针对玩家（包括玩家的所有城市）
	Notify_IncreaseSpeed_Request nofityBase;
	nofityBase.serialize(req_msg->from,pCardinfo->Type,pCardinfo->Value);
	//查询玩家城市
	PlayerCity playercity;
	playercity.setmasterid(req_msg->from);
	PlayerCity *pRetSet = 0;
	char *retbuf = 0;
	int count = 0;
	if(m_DBProxy.db_multi_select(playercity,retbuf,count) == DB_Proxy::DB_SUCCEED)
	{
		iRet = 0;
		count = count /sizeof(PlayerCity);
		pRetSet = (PlayerCity*)retbuf;
		for(int i=0;i<count;i++)
		{
			event_notify(pRetSet[i].mapid_,(char*)&nofityBase,nofityBase.length);
		}
	}
	if(retbuf)
	{
		delete[] retbuf;
		retbuf = 0;
	}
	
	if(iRet ==0)
	{
		//写当前速率到数据库
		if(UpdatePlayerState(req_msg->from,pCardinfo->Type,pCardinfo->Value+100))
		{
			printf("update player state fault(%d,%d,%d)\n",req_msg->from,pCardinfo->Type,pCardinfo->Value);
		}
		//增加一个定时器
		LPNotify_Timer nTimer = new Notify_Timer;
		nTimer->serialize(req_msg->from,pCardinfo->Type,0,0);
		nTimer->Param.TimerID = set_timer(pCardinfo->Time,nTimer);
		if(nTimer->Param.TimerID >=0)
		{
			ChargeSvrTimer svrTimer;
			//若定时器已存在，则取消原定时器
			LPNotify_Timer oldTimerParam = 0;
			oldTimerParam = m_pTask->GetTimerID(req_msg->from,pCardinfo->Type);
			if(oldTimerParam)
			{
				cancel_timer(oldTimerParam->Param.TimerID);
				svrTimer.setmasterid(MDM_GP_MAP_CHARGE);
				svrTimer.setdetailid(oldTimerParam->Param.TimerID);
				if(m_DBProxy.db_delete(svrTimer) != DB_Proxy::DB_SUCCEED)
				{
					printf("Insert timer param to db fault.\n");
				}
				memset(&svrTimer,0,sizeof(ChargeSvrTimer));
				delete oldTimerParam;
				oldTimerParam = 0;
			}
			m_pTask->AddTimerNotifyMsg(req_msg->from,pCardinfo->Type,nTimer);
			
#ifdef PRINT_HINTINFO
			printf("Timer id = %d\n",nTimer->Param.TimerID);
#endif
			//加入数据库
			svrTimer.setmasterid(MDM_GP_MAP_CHARGE);
			svrTimer.setdetailid(nTimer->Param.TimerID);
			svrTimer.time_ = IceUtil::Time::now().toSeconds() + pCardinfo->Time;//有效期
			memcpy(svrTimer.param_,&(nTimer->Param),sizeof(ChargeSvr_TimerParam));
			if(m_DBProxy.db_insert(svrTimer) != DB_Proxy::DB_SUCCEED)
			{
				printf("Insert timer param to db fault.\n");
			}
		}
		else
		{
			delete nTimer;
			nTimer = 0;
		}
		iRet = DelCard(iteminfo);
	}
	rep_msg.serialize(req_msg->agent,req_msg->from,iRet);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	//记录日志
	WriteLogEx(req_msg->from,pCardinfo,UseCard_Log);
}

void CProcChargeMsg::UseResruitHeroCard(const char *msg)
{	
	LPUseResruitHeroCard_Request req_msg = (LPUseResruitHeroCard_Request)msg;
	if(req_msg->length != sizeof(UseResruitHeroCard_Request))
	{
		LOG_TRACE(("CProcChargeMsg::UseResruitHeroCard-->消息包非法：玩家%d\n",req_msg->from));
		return;
	}
	UseResruitHeroCard_Response rep_msg;

	PlayerItemStorageTable iteminfo;
	iteminfo.setmasterid(req_msg->OfficerID,req_msg->from);
	iteminfo.setdetailid(req_msg->SortID);
	int iRet = GetItemInfo(iteminfo,&m_DBProxy);
	if(iRet)
	{//获取失败或不存在该道具
		rep_msg.serialize(req_msg->agent,req_msg->from,iRet);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}

	//判断卡信息
	LPCardParam pCardinfo = m_pTask->GetCardParam(iteminfo.itemid_);
	if(pCardinfo == NULL)
	{//不存在该类卡
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	//判断是否为英雄招募卡
	if(pCardinfo->Type  !=Recruit_Card)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,5);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	if((iRet = DelCard(iteminfo)) ==0)
	{		
		//招募卡，发送到大地图
		Notify_IncreaseOtherSpeed_Request notifyRecuit;
		notifyRecuit.serialize(req_msg->from,0,0,pCardinfo->Type,pCardinfo->Value,0);
		event_notify(SERVER_BOUND_MAP_OTHER,(char*)&notifyRecuit,notifyRecuit.length);
	}
	rep_msg.serialize(req_msg->agent,req_msg->from,iRet);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	//记录日志
	WriteLogEx(req_msg->from,pCardinfo,UseCard_Log);
}

void CProcChargeMsg::UseSpecialSkillCard(const char *msg)
{
	LPUseSpecialSkillCard_Request req_msg = (LPUseSpecialSkillCard_Request)msg;
	if(req_msg->length != sizeof(UseSpecialSkillCard_Request))
	{
		LOG_TRACE(("CProcChargeMsg::UseSpecialSkillCard-->消息包非法：玩家%d\n",req_msg->from));
		return;
	}
	UseSpecialSkillCard_Response rep_msg;

	PlayerItemStorageTable iteminfo;
	iteminfo.setmasterid(req_msg->OfficerID,req_msg->from);
	iteminfo.setdetailid(req_msg->SortID);
	int iRet = GetItemInfo(iteminfo,&m_DBProxy);
	if(iRet)
	{//获取失败或不存在该道具
		rep_msg.serialize(req_msg->agent,req_msg->from,iRet);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}

	//判断卡信息
	LPCardParam pCardinfo = m_pTask->GetCardParam(iteminfo.itemid_);
	if(pCardinfo == NULL)
	{//不存在该类卡
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	//判断是否为特殊技能卡
	if(pCardinfo->Type  <= SkillCard_Begin || pCardinfo->Type >= SkillCard_End)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,5);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	Notify_AddOfficerSkill_Request notifyAddOfficerSkill;
	notifyAddOfficerSkill.serialize(req_msg->agent,req_msg->from,req_msg->OfficerID,req_msg->HeroID,req_msg->SortID,pCardinfo->Value);
	if(event_notify(notifyAddOfficerSkill.to,(char*)&notifyAddOfficerSkill,notifyAddOfficerSkill.length) <=0)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	}
}

void CProcChargeMsg::UseOtherSpeedCard(const char *msg)
{
	LPUseOtherSpeedCard_Request req_msg = (LPUseOtherSpeedCard_Request)msg;
	if(req_msg->length != sizeof(UseOtherSpeedCard_Request))
	{
		LOG_TRACE(("CProcChargeMsg::UseOtherSpeedCard-->消息包非法：玩家%d\n",req_msg->from));
		return;
	}
	UseOtherSpeedCard_Response rep_msg;

	PlayerItemStorageTable iteminfo;
	iteminfo.setmasterid(req_msg->OfficerID,req_msg->from);
	iteminfo.setdetailid(req_msg->SortID);
	int iRet = GetItemInfo(iteminfo,&m_DBProxy);
	if(iRet)
	{//获取失败或不存在该道具
		rep_msg.serialize(req_msg->agent,req_msg->from,iRet);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}

	//判断卡信息
	LPCardParam pCardinfo = m_pTask->GetCardParam(iteminfo.itemid_);
	if(pCardinfo == NULL)
	{//不存在该类卡
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	//判断是否为加速卡卡
	if(pCardinfo->Type  <= OtherSpeedCard_Begin || pCardinfo->Type >= OtherSpeedCard_End)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,5);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}

	iRet = DelCard(iteminfo);
	if(iRet ==0)
	{
		//提速类,某一项的加速（建筑、科研、征兵）
		Notify_IncreaseOtherSpeed_Request notifySpeed;
		notifySpeed.serialize(req_msg->from,req_msg->MapID,req_msg->CityID,pCardinfo->Type,pCardinfo->Value,req_msg->SortID);
		//科研为单独的系统
		if(pCardinfo->Type == Research_Speed_Card)
		{
			//发通知消息至科研服务器
			event_notify(MDM_GP_MAP_RESEARCH,(char*)&notifySpeed,notifySpeed.length);
		}
		else
		{
			//发通知消息至郡地图
			event_notify(req_msg->MapID,(char*)&notifySpeed,notifySpeed.length);
		}
	}
	rep_msg.serialize(req_msg->agent,req_msg->from,iRet);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	//记录日志
	WriteLogEx(req_msg->from,pCardinfo,UseCard_Log);
}

void CProcChargeMsg::UseAutoCard(const char *msg)
{
	LPUseAutoCard_Request req_msg = (LPUseAutoCard_Request)msg;
	if(req_msg->length != (sizeof(UseAutoCard_Request)+req_msg->Count*sizeof(AutoConstruction)))
	{
		LOG_TRACE(("CProcChargeMsg::UseAutoCard-->消息包非法：玩家%d\n",req_msg->from));
		return;
	}
	UseAutoCard_Response rep_msg;

	PlayerItemStorageTable iteminfo;
	iteminfo.setmasterid(req_msg->OfficerID,req_msg->from);
	iteminfo.setdetailid(req_msg->SortID);
	int iRet = GetItemInfo(iteminfo,&m_DBProxy);
	if(iRet)
	{//获取失败或不存在该道具
		rep_msg.serialize(req_msg->agent,req_msg->from,iRet);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}

	//判断卡信息
	LPCardParam pCardinfo = m_pTask->GetCardParam(iteminfo.itemid_);
	if(pCardinfo == NULL)
	{//不存在该类卡
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	//判断是否为托管卡
	if(pCardinfo->Type  !=Auto_Card)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,5);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}

	//托管数量操作该类卡的限制
	if(pCardinfo->Value <req_msg->Count || req_msg->Count <=0)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	//已在托管中
	AutoPlayer_Ptr playerptr;
	if(m_pTask->GetAutoPlayer(req_msg->from,playerptr) ==0)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,3);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	//添加到列表中
	playerptr = new CAutoPlayer(req_msg->from);
	m_pTask->AddAutoPlayer(playerptr);
	
	//写入托管标志,首先判断给标志的记录是否存在，存在则修改，否则添加
	if(UpdatePlayerState(req_msg->from,Auto_Card,1))
	{
		printf("update player sate fault(playerid:%d,state:1.\n",req_msg->from);
		rep_msg.serialize(req_msg->agent,req_msg->from,4);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	iRet = DelCard(iteminfo);
	rep_msg.serialize(req_msg->agent,req_msg->from,iRet);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));

	//
	LPAutoConstruction pNewCons = 0;
	int i = 0;
	
	int o_mapid = 0;
	int o_cityid = 0;
	LPAutoConstruction pConsList = (LPAutoConstruction)(req_msg+1);
	for(i=0;i<req_msg->Count;i++)
	{
		pNewCons = new AutoConstruction;
		*pNewCons = pConsList[i];
		playerptr->AddConstruction(pNewCons);

	}
	for(i =0;i<req_msg->Count;i++)
	{
		if((o_mapid != pConsList[i].MapID) || (o_cityid != pConsList[i].CityID))
		{//不同的城市只判断一次
			o_mapid = pConsList[i].MapID;
			o_cityid = pConsList[i].CityID;
			if(!BuildListIsEmpty(req_msg->from,o_mapid,o_cityid,&m_DBProxy))
			{
				continue;
			}
			ProcBuildNotify(req_msg->from,o_mapid,o_cityid);
		}
	}
	//写本地文件
	playerptr->WriteList2File();
	//
	//记录日志
	WriteLogEx(req_msg->from,pCardinfo,UseCard_Log);
}
//取消托管
void CProcChargeMsg::CancelAuto(const char *msg)
{
	LPCancelAuto_Request req_msg = (LPCancelAuto_Request)msg;
	if(req_msg->length != sizeof(CancelAuto_Request))
	{
		LOG_TRACE(("CProcChargeMsg::CancelAuto-->消息包非法：玩家%d\n",req_msg->from));
		return;
	}
	/*DelPlayerState(req_msg->from,Auto_Card);
	return;*/
	CancelAuto_Response rep_msg;
	AutoPlayer_Ptr playerptr;
	if(m_pTask->GetAutoPlayer(req_msg->from,playerptr))
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	m_pTask->DelAutoPlayer(req_msg->from);
	rep_msg.serialize(req_msg->agent,req_msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	//修改托管标志，删除记录
	DelPlayerState(req_msg->from,Auto_Card);
}

//查询托管信息（未完成的托管列表)
void CProcChargeMsg::QueryAutoList(const char *msg)
{
	LPQueryAutoList_Request req_msg = (LPQueryAutoList_Request)msg;
	if(req_msg->length != sizeof(QueryAutoList_Request))
	{
		LOG_TRACE(("CProcChargeMsg::QueryAutoList-->消息包非法：玩家%d\n",req_msg->from));
		return;
	}
	LPQueryAutoList_Response rep_msg = 0;
	int size = sizeof(QueryAutoList_Response);
	//未在托管中
	AutoPlayer_Ptr playerptr;
	if(m_pTask->GetAutoPlayer(req_msg->from,playerptr))
	{
		rep_msg = new QueryAutoList_Response;
		rep_msg->serialize(size,req_msg->agent,req_msg->from,-1);
		asynch_write(rep_msg->agent,(char*)rep_msg,size);
		delete rep_msg;
		rep_msg = 0;
		return;
	}
	size += sizeof(AutoConstruction)*20;
	rep_msg = (LPQueryAutoList_Response)new char[size];
	LPAutoConstruction pList = (LPAutoConstruction)(rep_msg+1);
	int count = 0;
	if(playerptr.get())
	{
		playerptr->GetConstructionList(pList,count);
	}
	else
	{
	}
	size = sizeof(QueryAutoList_Response) + sizeof(AutoConstruction) * count;
	rep_msg->serialize(size,req_msg->agent,req_msg->from,-1);
	asynch_write(rep_msg->agent,(char*)rep_msg,size);
	delete[] (char*)rep_msg;
	rep_msg = 0;
}

void CProcChargeMsg::UseHeroEquipCard(const char *msg)
{
	LPUseHeroEquipCard_Request req_msg = (LPUseHeroEquipCard_Request)msg;
	if(req_msg->length != sizeof(UseHeroEquipCard_Request))
	{
		LOG_TRACE(("CProcChargeMsg::UseHeroEquipCard-->消息包非法：玩家%d\n",req_msg->from));
		return;
	}
	UseHeroEquipCard_Response rep_msg;

	PlayerItemStorageTable iteminfo;
	iteminfo.setmasterid(req_msg->OfficerID,req_msg->from);
	iteminfo.setdetailid(req_msg->SortID);
	int iRet = GetItemInfo(iteminfo,&m_DBProxy);
	if(iRet)
	{//获取失败或不存在该道具
		rep_msg.serialize(req_msg->agent,req_msg->from,iRet);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}

	//判断卡信息
	LPCardParam pCardinfo = m_pTask->GetCardParam(iteminfo.itemid_);
	if(pCardinfo == NULL)
	{//不存在该类卡
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	//判断是否为装备卡
	if(pCardinfo->Type  <= HeroEquipCard_Begin || pCardinfo->Type >= HeroEquipCard_End)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,5);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}

	//获取装备信息
	HeroEquipAttribute* heroEquipAttribute = m_pTask->GetEquipAttribute(pCardinfo->Value);
	if(heroEquipAttribute == NULL)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	iRet = 111;
	//装备类,即使用卡交换装备
	
	//使用卡的位置存放装备
	PlayerItemStorageTable equipinfo;
	PackStoreParam *pPackStoreParam = m_pTask->GetPackStoreParam(heroEquipAttribute->postion);
	if (pPackStoreParam && pPackStoreParam->IsOverlap)
	{
		equipinfo.itemid_ = pCardinfo->Value;
	}
	if (GetUnusedPos(equipinfo,&m_DBProxy)<0)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	if (equipinfo.sortid_ <=0)
	{
		equipinfo.setmasterid(req_msg->OfficerID,req_msg->from);
		equipinfo.type_ = iteminfo.type_;
		equipinfo.itemid_ = pCardinfo->Value;
		equipinfo.level_ = heroEquipAttribute->level;
		equipinfo.pos_ = heroEquipAttribute->postion;
		//equipinfo.showpos_ = iteminfo.showpos_;
		equipinfo.count_ = 1;
		equipinfo.tech_ = heroEquipAttribute->skill;
		equipinfo.techlevel_ = heroEquipAttribute->skillLevel;
		if(m_DBProxy.db_insert(equipinfo) == DB_Proxy::DB_SUCCEED)
		{
			iRet = 0;
		}
	}
	else
	{
		int isortid = equipinfo.sortid_;
		memset(&equipinfo,0,sizeof(equipinfo));
		equipinfo.setmasterid(req_msg->OfficerID,req_msg->from);
		equipinfo.setdetailid(isortid);
		equipinfo.count_ = 1;
		if (m_DBProxy.db_increase(equipinfo) != DB_Proxy::DB_SUCCEED)
		{
			iRet = 0;
		}

	}

	if(iRet ==0)
	{
		iRet = DelCard(iteminfo);
	}
	rep_msg.serialize(req_msg->agent,req_msg->from,iRet);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	//记录日志
	WriteLogEx(req_msg->from,pCardinfo,UseCard_Log);
}

void CProcChargeMsg::QueryResourceRate(const char *msg)
{
	LPQueryResourceRate_Request req_msg = (LPQueryResourceRate_Request)msg;
	if(req_msg->length != sizeof(QueryResourceRate_Request))
	{
		LOG_TRACE(("CProcChargeMsg::QueryResourceRate-->消息包非法：玩家%d\n",req_msg->from));
		return;
	}
	QueryResourceRate_Response rep_msg;
	PlayerProduceState statetb;
	statetb.setmasterid(req_msg->Type);
	statetb.setdetailid(req_msg->from);
	int value = 0;
	int iRet = m_DBProxy.db_select(statetb) ;
	if(iRet== DB_Proxy::DB_SUCCEED)
	{
		value = statetb.value_;
	}
	else if(iRet == DB_Proxy::DB_EOF)
	{
		if(req_msg->Type >Resource_SpeedCard_Begin && req_msg->Type <Resource_SpeedCard_End)
		{
			value = 100;
		}
		else
		{
			value = 0;
		}
	}
	else
	{
		value = -1;
	}
	rep_msg.serialize(req_msg->agent,req_msg->from,value);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
}

void CProcChargeMsg::UseExpandPackCard(const char *msg)
{	
	LPUseExpandPackCard_Request req_msg = (LPUseExpandPackCard_Request)msg;
	if(req_msg->length != sizeof(UseExpandPackCard_Request))
	{
		LOG_TRACE(("CProcChargeMsg::UseExpandPackCard-->消息包非法：玩家%d\n",req_msg->from));
		return;
	}
	UseExpandPackCard_Response rep_msg;

	PlayerItemStorageTable iteminfo;
	iteminfo.setmasterid(req_msg->OfficerID,req_msg->from);
	iteminfo.setdetailid(req_msg->SortID);
	int iRet = GetItemInfo(iteminfo,&m_DBProxy);
	if(iRet)
	{//获取失败或不存在该道具
		rep_msg.serialize(req_msg->agent,req_msg->from,iRet,20);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}

	//判断卡信息
	LPCardParam pCardinfo = m_pTask->GetCardParam(iteminfo.itemid_);
	if(pCardinfo == NULL)
	{//不存在该类卡
		rep_msg.serialize(req_msg->agent,req_msg->from,111,20);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	//判断是否为背包扩展卡
	if(pCardinfo->Type  !=ExpandPack_Card)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,5,20);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	//判断背包容量是否已达上线
	PlayerChatRoomTable playertb;
	playertb.setid(req_msg->from);
	if(m_DBProxy.db_select(playertb) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111,20);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	if(playertb.maxpacknum_ >= MAX_PACKNUM)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,2,playertb.maxpacknum_ );
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}

	iRet = 0;
	if(req_msg->PackID == 1)
	{//公共背包
		PlayerChatRoomTable playertb2;
		playertb2.setid(req_msg->from);
		if(playertb.maxpacknum_ + pCardinfo->Value >MAX_PACKNUM)
		{
			playertb2.maxpacknum_ = MAX_PACKNUM - playertb.maxpacknum_ ;
		}
		else
		{
			playertb2.maxpacknum_ = pCardinfo->Value;
		}

		if(m_DBProxy.db_increase(playertb2) != DB_Proxy::DB_SUCCEED)
		{
			rep_msg.serialize(req_msg->agent,req_msg->from,2,playertb.maxpacknum_ );
			asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
			return;
		}
		iRet = DelCard(iteminfo);
		rep_msg.serialize(req_msg->agent,req_msg->from,iRet,playertb.maxpacknum_ + pCardinfo->Value );
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		//记录日志
		WriteLogEx(req_msg->from,pCardinfo,UseCard_Log);
	}
	else
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111,20 );
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	}
}

//处理定时器消息
void CProcChargeMsg::ProcTimerMsg(const char *msg)
{
	LPNotify_Timer req_msg = (LPNotify_Timer)msg;
	if(req_msg->Param.TimerType >Resource_SpeedCard_Begin && req_msg->Param.TimerType <Resource_SpeedCard_End)
	{//基本资源加速结束
		Notify_NormalSpeed nofityBase;
		nofityBase.serialize(req_msg->Param.PlayerID,req_msg->Param.TimerType);
		//查询玩家城市
		PlayerCity playercity;
		playercity.setmasterid(req_msg->Param.PlayerID);
		PlayerCity *pRetSet = 0;
		char *retbuf = 0;
		int count = 0;
		if(m_DBProxy.db_multi_select(playercity,retbuf,count) == DB_Proxy::DB_SUCCEED)
		{
			count = count /sizeof(PlayerCity);
			pRetSet = (PlayerCity*)retbuf;
			for(int i=0;i<count;i++)
			{
				event_notify(pRetSet[i].mapid_,(char*)&nofityBase,nofityBase.length);
			}
		}
		if(retbuf)
		{
			delete[] retbuf;
			retbuf = 0;
		}
		if(DelPlayerState(req_msg->Param.PlayerID,req_msg->Param.TimerType))
		{
			//assert(false);
			LOG_TRACE(("update player state fault(%d,%d,%d)\n",req_msg->Param.PlayerID,req_msg->Param.TimerType,100));
		}
	}
	else if(req_msg->Param.TimerType == Auto_Card)
	{
		//通知郡服务器收集资源
		Notify_CollectResource_Request collectmsg;
		collectmsg.serialize(req_msg->Param.PlayerID,req_msg->Param.MapID,req_msg->Param.CityID);
		event_notify(req_msg->Param.MapID,(char*)&collectmsg,collectmsg.length);
	}
	else
	{
	}
	m_pTask->DelTimerNotifyMsg(req_msg->Param.PlayerID,req_msg->Param.TimerType);
	//从定时器表中删除ChargeSvrTimer
	ChargeSvrTimer svrTimer;
	svrTimer.setmasterid(MDM_GP_MAP_CHARGE);
	svrTimer.setdetailid(req_msg->Param.TimerID);
	if(m_DBProxy.db_delete(svrTimer) != DB_Proxy::DB_SUCCEED)
	{
		printf("Delete timer param fault(svrid:%d,timerid:%d)\n",MDM_GP_MAP_CHARGE,req_msg->Param.TimerID);
	}
}

void CProcChargeMsg::BuildComplete(const char *msg)
{
	LPNotify_BuildComplete_Response req_msg = (LPNotify_BuildComplete_Response)msg;
//	
//#ifdef PRINT_HINTINFO
//	printf("command:%d,playerid:%d,mapid:%d,cityid:%d\n",req_msg->command,req_msg->PlayerID,req_msg->MapID,req_msg->CityID);
//#endif

	if(req_msg->Error_Code == 0)
	{//郡服务器下发正常结束通知
		ProcBuildNotify(req_msg->PlayerID,req_msg->MapID,req_msg->CityID);
	}
	else if(req_msg->Error_Code == 1)
	{//资源不足通知
		LPNotify_Timer nTimer = new Notify_Timer;
		nTimer->serialize(req_msg->PlayerID,Auto_Card,req_msg->MapID,req_msg->CityID);
		nTimer->Param.TimerID = set_timer(m_pTask->GetCheckResourceTime(),nTimer);
		if(nTimer->Param.TimerID >=0)
		{
			m_pTask->AddTimerNotifyMsg(req_msg->PlayerID,Auto_Card,nTimer);
			
#ifdef PRINT_HINTINFO
			printf("Timer id = %d\n",nTimer->Param.TimerID);
#endif
		}
		else
		{
			delete nTimer;
			nTimer = 0;
		}
	}
}

void CProcChargeMsg::CollectResourceMsg(const char *msg)
{
	LPNotify_CollectResource_Response req_msg = (LPNotify_CollectResource_Response)msg;

	if(BuildListIsEmpty(req_msg->PlayerID,req_msg->MapID,req_msg->CityID,&m_DBProxy))
	{
		ProcBuildNotify(req_msg->PlayerID,req_msg->MapID,req_msg->CityID);
	}
}
void CProcChargeMsg::ProcBuildNotify(int plyaerid,int mapid,int cityid)
{
	int iRet = 0;
	AutoConstruction curAutoCons;
	int Count = -1;
	iRet = m_pTask->GetConstruction(plyaerid,mapid,cityid,curAutoCons,&m_DBProxy,Count);//playerptr->GetConstruction(o_mapid,o_cityid,curAutoCons,&m_DBProxy);
	if(Count ==0)
	{//删除托管玩家，并修改状态
		m_pTask->DelAutoPlayer(plyaerid);//玩家托管列表为空，则删除玩家，并修改玩家托管状态
		if(DelPlayerState(plyaerid,Auto_Card))
		{
			printf("update player state fault(%d,%d,%d)\n",plyaerid,Auto_Card,0);
		}
	}
	if(iRet ==0)
	{
		//发送建造
		Nodify_BuildConstruction_Request notify_build;
		notify_build.serialize(plyaerid,&curAutoCons);
		event_notify(curAutoCons.MapID,(char*)&notify_build,sizeof(notify_build));
#ifdef PRINT_HINTINFO
		printf("ProcBuildNotify(mapid:%d,cityid:%d,sortid:%d,level:%d) count:%d\n",
			curAutoCons.MapID,curAutoCons.CityID,curAutoCons.SortID,curAutoCons.Level,Count);
#endif
	}
	else if(iRet == -1)
	{//定时器
		LPNotify_Timer nTimer = new Notify_Timer;
		nTimer->serialize(plyaerid,Auto_Card,mapid,cityid);
		nTimer->Param.TimerID = set_timer(m_pTask->GetCheckResourceTime(),nTimer);
		if(nTimer->Param.TimerID >=0)
		{
			m_pTask->AddTimerNotifyMsg(plyaerid,Auto_Card,nTimer);
			
#ifdef PRINT_HINTINFO
			printf("Timer id = %d\n",nTimer->Param.TimerID);
#endif
		}
		else
		{
			delete nTimer;
			nTimer = 0;
		}
	}
	else if(iRet == -2)
	{//结束
	}
	else
	{
	}
}
//兑换
void CProcChargeMsg::ExchangeMoney(const char *msg)
{
	LPExchange_Request req_msg = (LPExchange_Request)msg;
	if(req_msg->length != sizeof(Exchange_Request))
	{
		LOG_TRACE(("CProcChargeMsg::ExchangeMoney-->消息包非法：玩家%d\n",req_msg->from));
		return;
	}
	Exchange_Response rep_msg;
	int iRet = 0;
	int iGoCount = 0;
	if(req_msg->Type == GO2Money)
	{ 
		if(req_msg->Count > 10000)
		{
			rep_msg.serialize(req_msg->agent,req_msg->from,req_msg->Type,-3);
			asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
			return;
		}
		if(GetGoDian(req_msg->from,iGoCount) <=0)
		{
			rep_msg.serialize(req_msg->agent,req_msg->from,req_msg->Type,-2);
			asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
			return;
		}
		if(iGoCount < req_msg->Count)
		{//不足
			rep_msg.serialize(req_msg->agent,req_msg->from,req_msg->Type,-1);
			asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
			return;
		}
		//扣除GO点
		char szbuf[SUBGODIAN_LOG_SIZE] = {0};
		sprintf(szbuf,"兑换游戏金钱。");
		iRet = SubGoDian(req_msg->from,req_msg->Count,szbuf);
		if(iRet <0)
		{//失败
			rep_msg.serialize(req_msg->agent,req_msg->from,req_msg->Type,-2);
			asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
			return;
		}
		else if(iRet ==0)
		{//Go点不足
			rep_msg.serialize(req_msg->agent,req_msg->from,req_msg->Type,-1);
			asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
			return;
		}
		//添加金钱
		PlayerChatRoomTable playertb;
		playertb.setid(req_msg->from);
		playertb.money_ = req_msg->Count * m_pTask->GetExchageRate(req_msg->Type);
		if(m_DBProxy.db_increase(playertb) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE(("CProcChargeMsg::ExchangeMoney-->玩家%d兑换%d\n",req_msg->from,req_msg->Count));
		}
		rep_msg.serialize(req_msg->agent,req_msg->from,req_msg->Type,playertb.money_);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		QueryMoney_Response msg_gomoney;
		msg_gomoney.serialize(req_msg->agent,req_msg->from,iGoCount - req_msg->Count);
		asynch_write(msg_gomoney.agent,(char*)&msg_gomoney,msg_gomoney.length);
	}
	else if(req_msg->Type == Money2GO)
	{
	}
}
//服务器返回的通知消息
void CProcChargeMsg::ProImmedCompleteNotify(const char *msg)
{
	LPNotify_ImmedComplete_Request req_msg = (LPNotify_ImmedComplete_Request)msg;
	if(req_msg->length != sizeof(Notify_ImmedComplete_Request))
	{
		LOG_TRACE(("CProcChargeMsg::ProImmedCompleteNotify-->消息包非法：玩家%d\n",req_msg->from));
		return;
	}
	Notify_ImmedComplete_Response rep_msg;
	int iRet = 0;
	int iPlayerID = req_msg->PlayerID;
	//扣除GO点
	char szbuf[SUBGODIAN_LOG_SIZE] = {0};
	if(req_msg->Param.Type == Recruit_Card)
	{
		sprintf(szbuf,"立即招募:%d",req_msg->Param.Type);
	}
	else
	{
		sprintf(szbuf,"立即完成:%d,%d,%d,%d,%d,%d",req_msg->Param.Type,req_msg->Param.MapID,req_msg->Param.CityID,req_msg->Param.ItemID,
			req_msg->Param.SortID,req_msg->Param.ProductionType);
	}
#ifdef PRINT_HINTINFO
	printf(szbuf);
#endif
	iRet = SubGoDian(iPlayerID,req_msg->Param.GOCount,szbuf);
	if(iRet <0)
	{//失败
		iRet = 111;
	}
	else if(iRet ==0)
	{//Go点不足
		iRet = 1;
	}
	else
	{
		iRet = 0;
	}

	rep_msg.serialize(req_msg->agent,req_msg->PlayerID,req_msg->Param.Type,req_msg->Param.MapID,req_msg->Param.CityID,req_msg->Param.ItemID,
		req_msg->Param.SortID,req_msg->Param.Level,iRet,req_msg->Param.GOCount,req_msg->Param.ProductionType);
	event_notify(req_msg->from,(char*)&rep_msg,rep_msg.length);
	if (iRet ==0)
	{
		m_pTask->WriteLog(&m_DBProxy,MDM_GP_MAP_CHARGE,iPlayerID,m_pTask->GetLogFormat(ImmedComplete_Log),
			req_msg->Param.Type,req_msg->Param.MapID,req_msg->Param.CityID,req_msg->Param.ItemID,
			req_msg->Param.SortID,req_msg->Param.ProductionType,req_msg->Param.GOCount);
	}
}
//不使用加速卡直接加速
void CProcChargeMsg::IncreaseRecourceRate(const char *msg)
{
	LPIncreaeRecourceRate_Request req_msg = (LPIncreaeRecourceRate_Request)msg;
	IncreaeRecourceRate_Response rep_msg;
	int iRet  = 0;
	//扣除GO点
	LPSpeedParam_Node pSpeedParam = m_pTask->GetSpeedParam(req_msg->Type,req_msg->Rate);
	if(pSpeedParam ==0)
	{
		LOG_TRACE(("CProcChargeMsg::IncreaseRecourceRate-->玩家%d请求的加速类型%d-%d不存在\n",req_msg->from,req_msg->Type,req_msg->Rate));
		return;
	}
	int iNeedGO = pSpeedParam->Price;//暂时这样计算,应该有一个配置表，根据加速类型、加速率
	char szbuf[SUBGODIAN_LOG_SIZE] = {0};
	sprintf(szbuf,"直接加速,类型%d,加速率%d。",req_msg->Type,req_msg->Rate);
	iRet = SubGoDian(req_msg->from,iNeedGO,szbuf);
	if(iRet <0)
	{//失败
		iRet = -111;
	}
	else if(iRet ==0)
	{//Go点不足
		iRet = -1;
	}
	if(iRet <=0)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,iRet,req_msg->Type,req_msg->Rate);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	iRet = -111;
	//基本资源提速卡,针对玩家（包括玩家的所有城市）
	Notify_IncreaseSpeed_Request nofityBase;
	nofityBase.serialize(req_msg->from,(CardType)req_msg->Type,req_msg->Rate);
	//查询玩家城市
	PlayerCity playercity;
	playercity.setmasterid(req_msg->from);
	PlayerCity *pRetSet = 0;
	char *retbuf = 0;
	int count = 0;
	if(m_DBProxy.db_multi_select(playercity,retbuf,count) == DB_Proxy::DB_SUCCEED)
	{
		iRet = iNeedGO;
		count = count /sizeof(PlayerCity);
		pRetSet = (PlayerCity*)retbuf;
		for(int i=0;i<count;i++)
		{
			event_notify(pRetSet[i].mapid_,(char*)&nofityBase,nofityBase.length);
		}
	}
	if(retbuf)
	{
		delete[] retbuf;
		retbuf = 0;
	}
	
	if(iRet >=0)
	{
		//写当前速率到数据库
		if(UpdatePlayerState(req_msg->from,req_msg->Type,req_msg->Rate+100))
		{
			printf("update player state fault(%d,%d,%d)\n",req_msg->from,req_msg->Type,req_msg->Rate);
		}
		//增加一个定时器
		LPNotify_Timer nTimer = new Notify_Timer;
		nTimer->serialize(req_msg->from,(CardType)req_msg->Type,0,0);

		nTimer->Param.TimerID = set_timer(pSpeedParam->Times,nTimer);
		if(nTimer->Param.TimerID >=0)
		{
			ChargeSvrTimer svrTimer;
			//若定时器已存在，则取消原定时器
			LPNotify_Timer oldTimerParam = 0;
			oldTimerParam  = m_pTask->GetTimerID(req_msg->from,req_msg->Type);
			if(oldTimerParam)
			{
#ifdef PRINT_HINTINFO
				printf("Delete old timer:%d\n",oldTimerParam->Param.TimerID);
#endif
				cancel_timer(oldTimerParam->Param.TimerID);
				svrTimer.setmasterid(MDM_GP_MAP_CHARGE);
				svrTimer.setdetailid(oldTimerParam->Param.TimerID);
				if(m_DBProxy.db_delete(svrTimer) != DB_Proxy::DB_SUCCEED)
				{
					printf("Insert timer param to db fault.\n");
				}
				memset(&svrTimer,0,sizeof(ChargeSvrTimer));
				delete oldTimerParam;
				oldTimerParam = 0;
			}
			m_pTask->AddTimerNotifyMsg(req_msg->from,req_msg->Type,nTimer);
			
#ifdef PRINT_HINTINFO
			printf("Timer id = %d\n",nTimer->Param.TimerID);
#endif
			//加入数据库
			svrTimer.setmasterid(MDM_GP_MAP_CHARGE);
			svrTimer.setdetailid(nTimer->Param.TimerID);
			svrTimer.time_ = IceUtil::Time::now().toSeconds() + pSpeedParam->Times;//有效期
			memcpy(svrTimer.param_,&(nTimer->Param),sizeof(ChargeSvr_TimerParam));
			if(m_DBProxy.db_insert(svrTimer) != DB_Proxy::DB_SUCCEED)
			{
				printf("Insert timer param to db fault.\n");
			}
		}
		else
		{
			delete nTimer;
			nTimer = 0;
		}
	}
	rep_msg.serialize(req_msg->agent,req_msg->from,iRet,req_msg->Type,req_msg->Rate);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	//记录日志
	if (iRet >0)
	{
		m_pTask->WriteLog(&m_DBProxy,MDM_GP_MAP_CHARGE,req_msg->from,m_pTask->GetLogFormat(ImmedResourceRate_Log),req_msg->Type,req_msg->Rate,iNeedGO);
	}
}

void CProcChargeMsg::ElectionNotify(const char *msg)
{
	LPNotify_Election_Request req_msg = (LPNotify_Election_Request)msg;
	if(req_msg->length != sizeof(Notify_Election_Request))
	{
		LOG_TRACE(("CProcChargeMsg::ProImmedCompleteNotify-->消息包非法：玩家%d\n",req_msg->from));
		return;
	}
	Notify_Election_Response rep_msg;
	int iRet = 0;
	char szbuf[SUBGODIAN_LOG_SIZE] = {0};
	sprintf(szbuf,"参加阵营%d部门%d选举。",req_msg->CountryID,req_msg->Dept);
	//扣除GO点
#ifdef PRINT_HINTINFO
	printf("playerid:%d,count:%d\n",req_msg->from,req_msg->GOMoney);
#endif
	iRet = SubGoDian(req_msg->from,req_msg->GOMoney,szbuf);
	if(iRet <0)
	{//失败
		iRet = 111;
	}
	else if(iRet ==0)
	{//Go点不足
		iRet = 1;
	}
	else
	{
		iRet = 0;
	}

	rep_msg.serialize(req_msg->agent,req_msg->from,iRet,req_msg->CountryID,req_msg->Dept,req_msg->Creed,req_msg->Money);
	event_notify(MDM_GP_MAP_CONSORTIA,(char*)&rep_msg,rep_msg.length);
	if (iRet ==0)
	{
		m_pTask->WriteLog(&m_DBProxy,MDM_GP_MAP_CHARGE,req_msg->from,m_pTask->GetLogFormat(Election_Log),req_msg->Dept,req_msg->GOMoney);
	}
}
//使用恢复卡
void CProcChargeMsg::UseHeroResumeCard(const char *msg)
{
	LPUseHeroResume_Request req_msg = (LPUseHeroResume_Request)msg;
	if(req_msg->length != sizeof(UseHeroResume_Request))
	{
		LOG_TRACE(("CProcChargeMsg::UseHeroResumeCard-->消息包非法：玩家%d\n",req_msg->from));
		return;
	}
	UseHeroResume_Response rep_msg;

	PlayerItemStorageTable iteminfo;
	iteminfo.setmasterid(req_msg->OfficerID,req_msg->from);
	iteminfo.setdetailid(req_msg->SortID);
	int iRet = GetItemInfo(iteminfo,&m_DBProxy);
	if(iRet)
	{
		iteminfo.setmasterid(1,req_msg->from);
		iRet = GetItemInfo(iteminfo,&m_DBProxy);
	}
	if(iRet)
	{//获取失败或不存在该道具
		rep_msg.serialize(req_msg->agent,req_msg->from,iRet,req_msg->HeroID);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}

	//判断卡信息
	LPCardParam pCardinfo = m_pTask->GetCardParam(iteminfo.itemid_);
	if(pCardinfo == NULL)
	{//不存在该类卡
		rep_msg.serialize(req_msg->agent,req_msg->from,111,req_msg->HeroID);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	//判断是否为恢复卡
	if(pCardinfo->Type  !=HeroSafe_Card)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,2,req_msg->HeroID);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	PlayerOfficerTable officertb;
	officertb.setmasterid(req_msg->from);
	officertb.setdetailid(req_msg->HeroID);
	if(m_DBProxy.db_select(officertb) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111,req_msg->HeroID);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	if(officertb.state_ != EN_GBH)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,3,req_msg->HeroID);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	iRet = DelCard(iteminfo);
	if(iRet ==0)
	{
		officertb.state_ = 0;
		if(m_DBProxy.db_update(officertb,officertb) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE(("CProcChargeMsg::UseHeroResumeCard-->修改英雄状态失败：玩家%d，英雄：%d\n",req_msg->from,req_msg->HeroID));
		}
	}
	rep_msg.serialize(req_msg->agent,req_msg->from,iRet,req_msg->HeroID);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	//记录日志
	WriteLogEx(req_msg->from,pCardinfo,UseCard_Log);
}
//任务系统查询GO点
void CProcChargeMsg::TaskQueryMoney(const char *msg)
{
	LPNotify_QueryMoney_Request req_msg = (LPNotify_QueryMoney_Request)msg;
	if(req_msg->from >MAX_SERVER_NUM || req_msg->from <SERVER_BOUND_MAP_BEGIN)
		return;

	Notify_QueryMoney_Response rep_msg;
	int money = 0;
	int iRet = GetGoDian(req_msg->PlayerID,money);
	if(iRet <=0)
	{
		money = -1;
	}
	rep_msg.serialize(req_msg->agent,req_msg->PlayerID,req_msg->TaskID,money,req_msg->from);
	event_notify(req_msg->from,(char*)&rep_msg,rep_msg.length);
}
//添加、减少GO点
void CProcChargeMsg::AddPlayerGODian(const char *msg)
{
	LPNotify_AddMoney_Request req_msg = (LPNotify_AddMoney_Request)msg;
	if(req_msg->from >MAX_SERVER_NUM || req_msg->from <SERVER_BOUND_MAP_BEGIN)
		return;
	Notify_AddMoney_Response rep_msg;
	int iRet = 0;
	char szbuf[SUBGODIAN_LOG_SIZE] = {0};
	sprintf(szbuf,"服务器%d要求增加或扣除GO点。",req_msg->from);
	//扣除GO点
	iRet = SubGoDian(req_msg->PlayerID,req_msg->Money,szbuf);
	if(iRet <0)
	{//失败
		iRet = 111;
	}
	else if(iRet ==0)
	{//Go点不足
		iRet = 1;
	}
	else
	{
		iRet = 0;
	}
	rep_msg.serialize(req_msg->agent,req_msg->PlayerID,iRet,req_msg->from);
	event_notify(req_msg->from,(char*)&rep_msg,rep_msg.length);
	if (iRet ==0)
	{
		m_pTask->WriteLog(&m_DBProxy,MDM_GP_MAP_CHARGE,req_msg->PlayerID,m_pTask->GetLogFormat(AddGODian_Log),req_msg->from,req_msg->Money);
	}
}
//使用技能卡，大地图返回消息
void CProcChargeMsg::onAddOfficerSkillResponse(const char *msg)
{
	LPNotify_AddOfficerSkill_Response req_msg = (LPNotify_AddOfficerSkill_Response)msg;
	UseSpecialSkillCard_Response rep_msg;
	if (req_msg->Error_Code != 0)
	{
		rep_msg.serialize(req_msg->agent,req_msg->PlayerID,req_msg->Error_Code);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	PlayerItemStorageTable iteminfo;
	iteminfo.setmasterid(req_msg->PackID,req_msg->PlayerID);
	iteminfo.setdetailid(req_msg->SortID);
	int iRet = GetItemInfo(iteminfo,&m_DBProxy);
	iRet = DelCard(iteminfo);
	rep_msg.serialize(req_msg->agent,req_msg->PlayerID,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	//记录日志
	LPCardParam pCardinfo = m_pTask->GetCardParam(iteminfo.itemid_);
	if (pCardinfo)
	{
		WriteLogEx(req_msg->PlayerID,pCardinfo,UseCard_Log);
	}

}
//使用一个卡的通知，服务器之间通知
void CProcChargeMsg::onNotifyUseOneCard(const char *msg)
{
	LPNotify_UseOneCard_Request req_msg = (LPNotify_UseOneCard_Request)msg;
	LPCardParam pCardinfo = m_pTask->GetCardParam(req_msg->cardID);
	if (pCardinfo)
	{
		WriteLogEx(req_msg->playerID,pCardinfo,UseCard_Log,req_msg->from);
	}
}
int CProcChargeMsg::GetItemPos(int playerid,int officerid,int itemid,int &sortid)
{
	int pos = -1;
	PlayerItemStorageTable itemPos;
	PlayerItemStorageTable *pRetSet = 0;
	char *retbuf = 0;
	int count = 0;
	int iRet = 0;
	itemPos.setmasterid(officerid,playerid);
	iRet = m_DBProxy.db_multi_select(itemPos,retbuf,count);
	sortid = -1;
	if(iRet == DB_Proxy::DB_FAILED)
	{
		pos = -2;
	}
	else if(iRet == DB_Proxy::DB_EOF)
	{//背包为空
		pos = 0;
	}
	else
	{
		count = count /sizeof(PlayerItemStorageTable);
		pRetSet = (PlayerItemStorageTable*)retbuf;
		int tmppos[EN_PACK_GRID_NUM] = {0};
		int i = 0;
		for(i = 0;i<count && i<EN_PACK_GRID_NUM;i++)
		{
			if((itemid >=0) && (pRetSet[i].itemid_ == itemid))
			{
				sortid = pRetSet[i].sortid_;
				pos = pRetSet[i].showpos_;
				break;
			}
			else
			{
				tmppos[pRetSet[i].showpos_] = 1;
			}
		}
		if(pos ==-1)
		{
			for(i=0;i<EN_PACK_GRID_NUM;i++)
			{
				if(tmppos[i] ==0)
				{
					pos = i;
					break;
				}
			}
		}
	}
	if(retbuf)
	{
		delete[] retbuf;
		retbuf = 0;
	}
	return pos;
}

int CProcChargeMsg::GetItemPos(int playerid,int officerid,int itemid,PlayerItemStorageTable &itemrec)
{
	int pos = -1;
	PlayerItemStorageTable itemPos;
	PlayerItemStorageTable *pRetSet = 0;
	char *retbuf = 0;
	int count = 0;
	int iRet = 0;
	itemPos.setmasterid(officerid,playerid);
	iRet = m_DBProxy.db_multi_select(itemPos,retbuf,count);
	itemrec.sortid_ = -1;
	if(iRet == DB_Proxy::DB_FAILED)
	{
		pos = -2;
	}
	else if(iRet == DB_Proxy::DB_EOF)
	{//背包为空
		pos = 0;
	}
	else
	{
		count = count /sizeof(PlayerItemStorageTable);
		pRetSet = (PlayerItemStorageTable*)retbuf;

		int i = 0;
		for(i = 0;i<count;i++)
		{
			if(pRetSet[i].itemid_ == itemid)
			{
				//sortid = pRetSet[i].sortid_;
				itemrec = pRetSet[i];
				pos = pRetSet[i].showpos_;
				break;
			}
		}
	}
	if(retbuf)
	{
		delete[] retbuf;
		retbuf = 0;
	}
	return pos;
}

//返回值：0：成功；1：不存在；111：失败
int CProcChargeMsg::GetItemInfo(PlayerItemStorageTable &itemrec,DB_Proxy *pDBProxy)
{
	int iRet = 0;
	iRet = pDBProxy->db_select(itemrec);
	if(iRet == DB_Proxy::DB_FAILED)
	{
		iRet = 111;
	}
	else if(iRet == DB_Proxy::DB_EOF)
	{
		iRet = 1;
	}
	else
	{
		iRet = 0;
	}
	return iRet ;
}
int CProcChargeMsg::GetUnusedPos(PlayerItemStorageTable &packitem,DB_Proxy *pDBProxy)
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
void CProcChargeMsg::WriteLogEx(int playerid,CardParam *pCardParam,LogType logtype,int svrid)
{
	const char *pLogFmt = m_pTask->GetLogFormat(logtype);
	char buf[128];
	sprintf(buf,"%s(ID:%d，Price:%d，Value:%d)",pCardParam->CardName,pCardParam->CardID,pCardParam->Price,pCardParam->Value);
	m_pTask->WriteLog(&m_DBProxy,svrid,playerid,pLogFmt,buf,pCardParam->Price);
}

int CProcChargeMsg::DelCard(PlayerItemStorageTable &iteminfo)
{
	int iRet = 0;
	if(iteminfo.count_ <=1)
	{//删除
		if(m_DBProxy.db_delete(iteminfo) !=DB_Proxy::DB_SUCCEED)
		{
			printf("Delete card fault(playerid:%d,officerid:%d,itemid:%d,count:%d)\n",
				iteminfo.playerid_,iteminfo.officerid_,iteminfo.itemid_,iteminfo.count_);
			iRet = 111;
		}
	}
	else
	{//数量减少1
		iteminfo.count_ -=1;
		if(m_DBProxy.db_update(iteminfo,iteminfo) != DB_Proxy::DB_SUCCEED)
		{
			printf("Update card fault(playerid:%d,officerid:%d,itemid:%d,count:%d)\n",
				iteminfo.playerid_,iteminfo.officerid_,iteminfo.itemid_,iteminfo.count_);
			iRet = 112;
		}
	}

	return iRet;
}
//判断玩家的某城市的建造列表是否为空
bool CProcChargeMsg::BuildListIsEmpty(int playerid,int mapid,int cityid,DB_Proxy *pDBProxy)
{
	ConstructionBuild consbuildtb;
	consbuildtb.setmasterid(mapid,cityid,playerid);
	char *retbuf = 0;
	int count = 0;
	bool bRet = false;
	int iRet = pDBProxy->db_multi_select(consbuildtb,retbuf,count);
	if(iRet == DB_Proxy::DB_EOF)
	{
		bRet = true;
	}
	if(retbuf)
	{
		delete[] retbuf;
		retbuf = 0;
	}
	return bRet;
}
int CProcChargeMsg::UpdatePlayerState(int playerid,int type,int value)
{
	int iRet = 0;
	PlayerProduceState statetb;
	statetb.setmasterid(type);
	statetb.setdetailid(playerid);
	if(m_DBProxy.db_select(statetb) ==DB_Proxy::DB_SUCCEED)
	{
		statetb.value_ = value;
		if(m_DBProxy.db_update(statetb,statetb) !=DB_Proxy::DB_SUCCEED)
		{
			iRet = -1;
		}
	}
	else
	{
		statetb.setmasterid(type);
		statetb.setdetailid(playerid);
		statetb.value_ = value;
		if(m_DBProxy.db_insert(statetb) != DB_Proxy::DB_SUCCEED)
		{
			iRet = -2;
		}
	}
	return iRet;
}

int CProcChargeMsg::DelPlayerState(int playerid,int type)
{
	int iRet = 0;
	PlayerProduceState statetb;
	statetb.setmasterid(type);
	statetb.setdetailid(playerid);
	if(m_DBProxy.db_delete(statetb) != DB_Proxy::DB_SUCCEED)
	{
		iRet = -1;
	}

	return iRet;
}
//买卡，大地图加入背包后的消息处理
void CProcChargeMsg::onNotifyBuyCard(const char *msg)
{
	LPNotify_BuyCard_Response req_msg = (LPNotify_BuyCard_Response)msg;
	if (req_msg->from >MAX_SERVER_NUM)
	{
		return;
	}
	BuyCard_Response rep_msg;
	LPCardParam pCardinfo = m_pTask->GetCardParam(req_msg->cardID);
	if(pCardinfo == NULL)
	{//不存在该类卡
		rep_msg.serialize(req_msg->agent,req_msg->playerID,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}

	char szbuf[SUBGODIAN_LOG_SIZE] = {0};
	sprintf(szbuf,"购买卡%s(%d)",pCardinfo->CardName,pCardinfo->CardID);
	if (req_msg->error_Code !=0)
	{//失败
		rep_msg.serialize(req_msg->agent,req_msg->playerID,req_msg->error_Code);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		sprintf(szbuf,"购买卡%s(%d)失败。",pCardinfo->CardName,pCardinfo->CardID);
		SubGoDian(req_msg->playerID,-pCardinfo->Price,szbuf);
	}
	else
	{
		rep_msg.serialize(req_msg->agent,req_msg->playerID,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		/*记录日志*/
		WriteLogEx(req_msg->playerID,pCardinfo,BuyCard_Log);
	}
}
//军团升级扣除GO点。
void CProcChargeMsg::onNotifyUpdateConsortia(const char *msg)
{
	LPNotify_UpdateConsortia_Request req_msg = (LPNotify_UpdateConsortia_Request)msg;
	if (req_msg->from != MDM_GP_MAP_CONSORTIA)
	{
		return;
	}
	int iRet = 0;
	char szbuf[SUBGODIAN_LOG_SIZE] = {0};
	sprintf(szbuf,"军团【%d】升级，扣除GO点。",req_msg->consortiaID,req_msg->goDian);
	iRet = SubGoDian(req_msg->playerID,req_msg->goDian,szbuf);
	if(iRet <0)
	{//失败
		iRet = -111;
	}
	else if(iRet ==0)
	{//Go点不足
		iRet = -1;
	}
	if(iRet <=0)
	{//直接通知客户端
		UpdateConsortia_Response rep_msg;
		rep_msg.serialize(req_msg->agent,req_msg->playerID,3);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return;
	}
	//通知军团服务器
	Notify_UpdateConsortia_Response notifyMsg;
	notifyMsg.serialize(req_msg->agent,req_msg->playerID,req_msg->consortiaID);
	event_notify(notifyMsg.to,(char*)&notifyMsg,notifyMsg.length);
	m_pTask->WriteLog(&m_DBProxy,req_msg->from,req_msg->playerID,m_pTask->GetLogFormat(UpdateConsortia_Log),req_msg->consortiaID,req_msg->goDian);
}
