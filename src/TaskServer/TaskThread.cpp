#include "TaskThread.h"
#include "IceUtil.h"
#include <stdio.h>
#include "socketsvc.hpp"
#include "commInterface.h"
#include "ChargeProtocol.h"
#include "MapServer_Protocol.h"
#include "NetTask.h"
#include "ISystemLog.h"

#pragma warning(disable:4127)
#pragma warning(disable:4389)
#pragma warning(disable:4819)
CTaskThread::CTaskThread(CNetTask *netTask)
{
	m_NetTask = netTask;
//	InitTaskThread();
}

CTaskThread::~CTaskThread()
{
//	FInitTaskThread();
}

bool CTaskThread::InitTaskThread(const char * addr, short port)
{
	if (!Proxy.init(addr,port,MDM_GP_MAP_TASK))
   {
	   printf("Connect DBG failed( addr: %s,port: %d) ... \n",addr,port);
		return false;
   }
   
   return true;
}

bool CTaskThread::FInitTaskThread()
{
	return Proxy.fini();
}

/* 处理网络数据部分Function */
int CTaskThread::OnGetTaskCount(const DISTRIBUTED_HEADER * pNetRead)
{
	GET_TASK_COUNT * gettaskcount = (GET_TASK_COUNT * )pNetRead;
	REP_TASK_COUNT rep;
	rep.serialize(gettaskcount->agent,gettaskcount->from,-1);

	char * buf = 0;
	int count=0;
	PlayerTask tab;
	tab.setmasterid(gettaskcount->from);
	int iret = Proxy.db_multi_select(tab,buf,count);
	if (iret == DB_Proxy::DB_SUCCEED)
	{
		int nums_(0);
		const int size_=sizeof(PlayerTask);
		int count_=count / size_;
		PlayerTask * pt;
		for (int i=0; i<count_;i++)
		{
			pt = (PlayerTask*)(buf+i*size_);
			if (pt->state_==tsWc  || pt->state_ ==tsTj)  ++nums_;	
		}
		rep.count = nums_;
		delete [] buf;
		buf = 0;
	}
	else if (iret == DB_Proxy::DB_EOF)
	{
		rep.count = 0;
	}
	asynch_write(rep.agent,(char *)&rep,sizeof(rep));
	
	return 0;
}


//获取任务列表
int CTaskThread::OnGetTaskList(const DISTRIBUTED_HEADER * pNetRead)
{
	GET_TASK_LIST * pGetTaskList=(GET_TASK_LIST *)pNetRead;

	char * buf =0;
	int count = 0;
	PlayerTask pt;
	pt.setmasterid(pGetTaskList->from);
	int iRet = Proxy.db_multi_select(pt,buf,count);

	if (iRet==DB_Proxy::DB_SUCCEED)					//有数据
	{
		int iNum = count / sizeof(PlayerTask);
		if (iNum>0) 
		{
			
			bool bAllTj(true);  //全部提交
			
			const unsigned int tiSize = sizeof(PLAYER_TASK_INFO);
			char * tempbuf = new char[sizeof(REP_GET_TASK_LIST)+iNum*tiSize];
			REP_GET_TASK_LIST * pRep = (REP_GET_TASK_LIST*)tempbuf;
			pRep->serialize(pGetTaskList->agent,pGetTaskList->from,1,iNum);
			PlayerTask * Ptsk;
			PLAYER_TASK_INFO* pTaskInfo;
			for (int i=0;i<iNum;++i)
			{
				Ptsk = (PlayerTask*)(buf+i*sizeof(PlayerTask));
				pTaskInfo = (PLAYER_TASK_INFO*)(tempbuf+sizeof(REP_GET_TASK_LIST)+i*tiSize);
				pTaskInfo->TaskID = Ptsk->taskid_;
				pTaskInfo->OtherInfo.taskfrom  = tfSystem;
				pTaskInfo->OtherInfo.taskstate = Ptsk->state_;
				pTaskInfo->OtherInfo.time		 = Ptsk->tasktime_;
				if (Ptsk->state_<tsTj &&  bAllTj)  bAllTj=false;
			}
			asynch_write(pRep->agent,tempbuf,pRep->length);

			if (bAllTj && CFixTaskTable::Instance().m_BaseTaskMap.size()>(unsigned)iNum)  //策化新增了后续任务的处理
			{
				//找出最大任务
				Ptsk=(PlayerTask*)buf;
				int taskid=Ptsk->taskid_;
				for (int i=1;i<iNum;++i)
				{
					Ptsk=(PlayerTask*)(buf+i*sizeof(PlayerTask));
					if (Ptsk->taskid_>taskid) taskid = Ptsk->taskid_;
				}
				//是否有后序任务
				TaskBaseTable_Map_ConstIterator mit;
				for (mit=CFixTaskTable::Instance().m_BaseTaskMap.begin();mit!=CFixTaskTable::Instance().m_BaseTaskMap.end();mit++)
				{
					if (mit->second.prvtaskid_ == taskid /*|| mit->second.prvtaskid_ < taskid*/)
					{
						OnIssueSystemTask(pGetTaskList->agent,pGetTaskList->from,mit->first,mit->second.tasktype_);	
					}
				}
			}

			delete [] tempbuf;
			tempbuf = 0;
			delete [] buf;
			buf = 0;
			return 0;
		}
		else 
		{
			if (buf) 
			{
				delete [] buf;
				buf = 0;
			}
		}
	}
	//没有任务，则重新检索任务发布（刚注册玩家-首次登入）
	else if (iRet == DB_Proxy::DB_EOF)				//没有数据
	{
		//先回复，后发布
		REP_GET_TASK_LIST repGetTaskList;
		repGetTaskList.serialize(pGetTaskList->agent,pGetTaskList->from,1,0);
		asynch_write(repGetTaskList.agent,(char *)&repGetTaskList,repGetTaskList.length);

		TaskBaseTable_Map_ConstIterator mit;
		for (mit = CFixTaskTable::Instance().m_BaseTaskMap.begin();mit!=CFixTaskTable::Instance().m_BaseTaskMap.end();mit++)
		{
			if (mit->second.prvtaskid_ == -1 && mit->second.taskitem_ == -1 && mit->second.playerlevel_ == -1 && mit->second.playerrank_ == -1)
			{
				//发布任务
				OnIssueSystemTask(pGetTaskList->agent,pGetTaskList->from,mit->first,mit->second.tasktype_);
			}
		}
	}
	else  if (iRet == DB_Proxy::DB_FAILED) //提取数据失败
	{
		//先回复，后发布
		REP_GET_TASK_LIST repGetTaskList;
		repGetTaskList.serialize(pGetTaskList->agent,pGetTaskList->from,0,0);
		asynch_write(repGetTaskList.agent,(char *)&repGetTaskList,repGetTaskList.length);
	}
	return 0;
}

int CTaskThread::OnGetTaskListEx(const DISTRIBUTED_HEADER * pNetRead)
{//包括普通任务和日常任务
	GET_TASK_LIST * pGetTaskList=(GET_TASK_LIST *)pNetRead;
	m_NetTask->setOnlinePlayer(pGetTaskList->from,pGetTaskList->agent);

	char *normal_buf = 0;
	int normal_count = 0;
	char *daytask_buf = 0;
	int daytask_count = 0;
	int i = 0;
	bool bAllTJ = true;
	int index = 0;
	int maxTaskID = 0;

	PlayerTask playertasktb;
	PlayerDayTask playerDayTaskTb;

	playertasktb.setmasterid(pGetTaskList->from);
	playerDayTaskTb.setmasterid(pGetTaskList->from);
	if( Proxy.db_multi_select(playertasktb,normal_buf,normal_count) == DB_Proxy::DB_FAILED
		|| Proxy.db_multi_select(playerDayTaskTb,daytask_buf,daytask_count) == DB_Proxy::DB_FAILED)
	{

		REP_GET_TASK_LIST repGetTaskList;
		repGetTaskList.serialize(pGetTaskList->agent,pGetTaskList->from,0,0);
		asynch_write(repGetTaskList.agent,(char *)&repGetTaskList,repGetTaskList.length);
		return 0;
	}
	normal_count = normal_count /sizeof(PlayerTask);
	daytask_count = daytask_count/sizeof(PlayerDayTask);
	PlayerTask *pNormalTaskList = (PlayerTask*)normal_buf;
	PlayerDayTask *pDayTaskList = (PlayerDayTask*)daytask_buf;

	char *retBuf = new char[sizeof(REP_GET_TASK_LIST)+ sizeof(PLAYER_TASK_INFO) * (normal_count + daytask_count)];
	REP_GET_TASK_LIST * pRepMsg = (REP_GET_TASK_LIST*)retBuf;
	pRepMsg->serialize(pGetTaskList->agent,pGetTaskList->from,1,normal_count + daytask_count);
	PLAYER_TASK_INFO *pTaskItemInfo = (PLAYER_TASK_INFO*)(retBuf +  sizeof(REP_GET_TASK_LIST));
	int mapID = 0;
	MapInfoTable maptb;
	int countryId = 0;

	PlayerChatRoomTable playerChatRoomTbl;
	memset(&playerChatRoomTbl, 0, sizeof(playerChatRoomTbl));

	playerChatRoomTbl.setid(pGetTaskList->from);
	
	if(Proxy.db_select(playerChatRoomTbl) == DB_Proxy::DB_FAILED)
	{
		;
	}

	countryId = playerChatRoomTbl.country_;

	for (i =0;i<normal_count;++i,++index)
	{//普通任务
		pTaskItemInfo[index].TaskID = pNormalTaskList[i].taskid_;
		pTaskItemInfo[index].OtherInfo.taskfrom  = tfSystem;
		pTaskItemInfo[index].OtherInfo.taskstate = pNormalTaskList[i].state_;
		pTaskItemInfo[index].OtherInfo.time		 = pNormalTaskList[i].tasktime_;
		if (pNormalTaskList[i].taskid_ > maxTaskID)
		{
			maxTaskID = pNormalTaskList[i].taskid_;
		}
		if (pNormalTaskList[i].state_<tsTj &&  bAllTJ)
			bAllTJ=false;
	}
	for (i =0;i<daytask_count;++i,++index)
	{
		mapID = pDayTaskList[i].mapid_;
		
		memset(&maptb, 0, sizeof(maptb));
		maptb.setid(mapID);

		if(Proxy.db_select(maptb) == DB_Proxy::DB_FAILED)
		{
			;
		}

		// 不是相同阵营的或者是新手村的日常任务出来
		if (countryId != maptb.countryid_ || 100 == maptb.state_)
		{
			// 日常任务
			pTaskItemInfo[index].TaskID = pDayTaskList[i].taskid_;
			pTaskItemInfo[index].OtherInfo.taskfrom  = mapID;
			pTaskItemInfo[index].OtherInfo.taskstate = pDayTaskList[i].state_;
			pTaskItemInfo[index].OtherInfo.time		 = pDayTaskList[i].starttime_;
		}
	}
	asynch_write(pRepMsg->agent,retBuf,pRepMsg->length);
	if (retBuf)
	{
		delete[] retBuf;
		retBuf = 0;
	}
	if (normal_buf)
	{
		delete[] normal_buf;
		normal_buf = 0;
	}
	if (daytask_buf)
	{
		delete[] daytask_buf;
		daytask_buf = 0;
	}

	if (normal_count <=0)
	{//发布新任务
		TaskBaseTable_Map_ConstIterator mit;
		for (mit = CFixTaskTable::Instance().m_BaseTaskMap.begin();mit!=CFixTaskTable::Instance().m_BaseTaskMap.end();mit++)
		{
			if (mit->second.prvtaskid_ == -1 && mit->second.taskitem_ == -1 && mit->second.playerlevel_ == -1 && mit->second.playerrank_ == -1)
			{
				//发布任务
				OnIssueSystemTask(pGetTaskList->agent,pGetTaskList->from,mit->first,mit->second.tasktype_);
			}
		}
	}
	else
	{
		if (bAllTJ && CFixTaskTable::Instance().m_BaseTaskMap.size()>(unsigned)normal_count)  //策化新增了后续任务的处理
		{
			//是否有后序任务
			TaskBaseTable_Map_ConstIterator mit;
			for (mit=CFixTaskTable::Instance().m_BaseTaskMap.begin();mit!=CFixTaskTable::Instance().m_BaseTaskMap.end();mit++)
			{
				if (mit->second.prvtaskid_ == maxTaskID /*|| mit->second.prvtaskid_ < maxTaskID*/)
				{
					OnIssueSystemTask(pGetTaskList->agent,pGetTaskList->from,mit->first,mit->second.tasktype_);	
				}
			}
		}
	}
	
	//发布日常任务
	issueDayTask(pGetTaskList->agent,pGetTaskList->from, countryId);

	return 0;
}
//处理任务状态更改
int CTaskThread::OnModiTaskState(const DISTRIBUTED_HEADER * pNetRead)
{
	SET_TASK_STATE * pState = (SET_TASK_STATE *)pNetRead;
#ifdef _DEBUG
	//printf("receive ModiTaskState PlayerID: %d ,TaskID: %d , TaskTaste: %d \n",pState->from,pState->TaskID,pState->taskstate);
#endif
	int iRet = -1;
	switch (pState->taskstate) 
	{
	case  tsYj:
		{
			iRet = OnAcceptTask(pNetRead);
			break;
		}
	case tsTj:
		{
			iRet = OnReferTask(pNetRead);
			break;
		}
	case  tsSc:
		{
			iRet = OnDeleteTask(pNetRead);
			break;
		}
	default:
		{
			//printf("处理任务状态更改 收到非法任务状态 taskstate: %d  \n",pState->taskstate);
			iRet = -1;
			break;
		}
	}
	if (iRet != 0)
	{
		REP_SET_TASK_STATE rep;
		rep.serialize(pState->agent,pState->from,pState->TaskID,pState->taskstate,0);
		asynch_write(rep.agent,(char *)&rep,sizeof(rep));
	}
	return 0;
}

//玩家等级变更事件
int CTaskThread::OnPlayerLeveEvent(const DISTRIBUTED_HEADER * pNetRead)
{
	USER_PLAYERLEVEL_PLAYERRANK_EVENT * pEvent = (USER_PLAYERLEVEL_PLAYERRANK_EVENT * )pNetRead;
#ifdef _DEBUG
	//printf("receive PLAYERLEVEL_PLAYERRANK_EVENT  PlayerID: %d , Level: %d , Rank: %d \n",pEvent->PlayerID,pEvent->PalyerLevel,pEvent->PlayerRank);
#endif
	TaskBaseTable_Map_ConstIterator mit;
	for ( mit=CFixTaskTable::Instance().m_BaseTaskMap.begin();
		mit!=CFixTaskTable::Instance().m_BaseTaskMap.end();mit++)
	{
		if ( mit->second.prvtaskid_ == -1 && mit->second.taskitem_ == -1 && ((mit->second.playerlevel_!=-1 && mit->second.playerlevel_==pEvent->PalyerLevel) ||
			(mit->second.playerrank_!=-1 && mit->second.playerrank_==pEvent->PlayerRank)) )
		{
			OnIssueSystemTask(pEvent->agent,pEvent->PlayerID,mit->first,mit->second.tasktype_);
		}
	}
	return 0;
}

//任务道具任务事件
int CTaskThread::OnTaskItemEvent(const DISTRIBUTED_HEADER * pNetRead)
{
	USER_TASKITEM_EVENT * pEvent = (USER_TASKITEM_EVENT *)pNetRead;
#ifdef _DEBUG
	//printf("receive TaskItemEvent PlayerID: %d,TaskItem: %d \n",pEvent->PlayerID,pEvent->TaksItem);
#endif
	TaskBaseTable_Map_ConstIterator mit;
	for ( mit=CFixTaskTable::Instance().m_BaseTaskMap.begin();
		mit!=CFixTaskTable::Instance().m_BaseTaskMap.end();mit++)
	{
		if (mit->second.taskitem_==pEvent->TaksItem)
		{
			OnIssueSystemTask(pEvent->agent,pEvent->PlayerID,mit->first,mit->second.tasktype_);
		}
	}
	return 0;
}

//玩家任务完成事件
int CTaskThread::OnTaskCompleteEvent(const DISTRIBUTED_HEADER * pNetRead)
{
	USER_TASK_COMPLETE_EVENT * pEvent = (USER_TASK_COMPLETE_EVENT *)pNetRead;

	OnCompleteTask(pEvent->agent,pEvent->PlayerID,pEvent->TaskID);
	return 0;
}

//副本完成事件
int CTaskThread::OnEctypeEvent(const DISTRIBUTED_HEADER * pNetRead)
{
	ECTYPE_EVENT * pEvent	=	(ECTYPE_EVENT *)pNetRead;

	//该副本是不是任务  20081211 修正
	std::map<int,tagEctypeTask>::const_iterator it=CFixTaskTable::Instance().m_EctypeTaslContainerMap.find(pEvent->EctypeID);
	if (it==CFixTaskTable::Instance().m_EctypeTaslContainerMap.end()) 
		return -1;

	if (it->second.taskType == ttDayTask)
	{
		PlayerDayTask dayTask;
		dayTask.setmasterid(pEvent->PlayerID);
		dayTask.setdetailid(it->second.taskid);
		if (Proxy.db_select(dayTask) != DB_Proxy::DB_SUCCEED)
		{
			return -1;
		}
		if (dayTask.state_ !=  tsYj)
		{
			return -1;
		}
		dayTask.state_ = tsWc;
		if (Proxy.db_update(dayTask,dayTask) != DB_Proxy::DB_SUCCEED)
		{
			return -1;
		}
		//发送状态给玩家
		REP_TASKSTATE_COMPLETE rep;
		rep.serialize(pEvent->agent,dayTask.playerid_,dayTask.taskid_);
		asynch_write(pEvent->agent,(char *)&rep,rep.length);

		PlayerOfficerTable officer;
		officer.setmasterid(pEvent->PlayerID);
		officer.setdetailid(pEvent->PlayerID);
		Proxy.db_select(officer);
		ISystemLog::getInstance()->playerTask_Log(officer.playerid_,officer.officername_,
			dayTask.taskid_,it->second.taskType ,dayTask.state_);
	}
	else
	{

		//该用户是否有该任务且是已接状态
		PlayerTask tb1;
		tb1.setmasterid(pEvent->PlayerID);
		tb1.setdetailid(it->second.taskid);
		if (Proxy.db_select(tb1) != DB_Proxy::DB_SUCCEED)
		{
			return -1;
		}
		if (tb1.state_ != tsYj)
		{
			return -1;
		}

		if (pEvent->Boss >= it->second.boos && pEvent->NpcNums >=it->second.npc && pEvent->BuildNums >= it->second.build)
		{
			OnCompleteTask(pEvent->agent,pEvent->PlayerID,it->second.taskid);
			return 0;
		}
		else 
			return -1;
	}
	return 0 ;
}


//请求检测生产类任务是否完成	
int CTaskThread::OnCheckTask(const DISTRIBUTED_HEADER * pNetRead)
{
	REQ_CHECK_TASK_OK * req = (REQ_CHECK_TASK_OK *)pNetRead;

	//对生产类任务
	PlayerTask uTable;
	uTable.setmasterid(req->from);
	char * buf =0;
	int count = 0;
	if (Proxy.db_multi_select(uTable,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		return -1;
	}

	TaskBaseTable_Map_ConstIterator cit;
	const int tSize = sizeof(uTable);
	int iNum = count / tSize;
	PlayerTask * pTable;	
	for (int i=0; i<iNum ; ++i)
	{
		pTable = (PlayerTask *)(buf + i*tSize);
		if (pTable->state_ == tsYj)
		{
			cit =  CFixTaskTable::Instance().m_BaseTaskMap.find(pTable->taskid_);
			if (cit != CFixTaskTable::Instance().m_BaseTaskMap.end())
			{
				if (cit->second.tasktype_ == ttScKy)
				{
					if (CheckTaskIsComplete(req->agent,pTable->playerid_,pTable->taskid_,cit->second.contest_)==0)
					{
						OnCompleteTask(req->agent,pTable->playerid_,pTable->taskid_);
					}
				}
			}
		}
	}
	delete [] buf;
	buf = 0;
	//检查日常任务是否完成
	checkDayTask(req->agent,req->from);
	return 0;
}

void CTaskThread::checkDayTask(int agent,int playerid)
{
	PlayerDayTask playerDayTask;
	char * buf = 0;
	int count = 0;
	playerDayTask.setmasterid(playerid);
	if (Proxy.db_multi_select(playerDayTask,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		return;
	}
	count = count / sizeof(PlayerDayTask);
	PlayerDayTask *pDayTaskList = (PlayerDayTask*)buf;
	std::map<int,DayTask_DetailInfo>::const_iterator iter;
	//发送状态给玩家
	REP_TASKSTATE_COMPLETE rep;
	for (int i=0;i<count;++i)
	{
		if (pDayTaskList[i].state_ != tsYj)
		{
			continue;
		}
		iter = CFixTaskTable::Instance().m_DayTaskMap.find(pDayTaskList[i].taskid_);
		if (iter == CFixTaskTable::Instance().m_DayTaskMap.end())
		{
			Proxy.db_delete(pDayTaskList[i]);
			continue;
		}
		if (iter->second.kindType == TASKTARGET_COLONIALCITY)//殖民
		{
			if (checkColonialDayTask(playerid,pDayTaskList[i].mapid_,iter->second.number,iter->second.itemLevel))
			{
				pDayTaskList[i].state_ = tsWc;
				if (Proxy.db_update(pDayTaskList[i],pDayTaskList[i]) == DB_Proxy::DB_SUCCEED)
				{
					rep.serialize(agent,playerid,pDayTaskList[i].taskid_);
					asynch_write(agent,(char *)&rep,rep.length);
				}
			}
		}
	}
	if (buf)
	{
		delete[] buf;
		buf = 0;
	}
}

bool CTaskThread::checkColonialDayTask(int playerid,int mapid,int number,int level)
{
	PlayerCity cityInfo;
	cityInfo.setmasterid(playerid);
	cityInfo.setdetailid(mapid);
	if (Proxy.db_select(cityInfo) != DB_Proxy::DB_SUCCEED)
	{
		return false;
	}
	int total = 0;
	PlayerConstruction constructions;
	char *buf = 0;
	int count = 0;
	constructions.setmasterid(cityInfo.mapid_,cityInfo.cityid_,cityInfo.playerid_);
	if (Proxy.db_multi_select(constructions,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		return false;
	}
	count = count / sizeof(PlayerConstruction);
	PlayerConstruction *pConsList = (PlayerConstruction*)buf;
	for (int i=0;i<count;++i)
	{
		if (pConsList[i].level_ >= level)
		{
			++total;
		}
	}
	if (buf)
	{
		delete[] buf;
		buf = 0;
	}
	
	return (total >= number)?true:false;
}
/* other-处理任务状态分解 */
//提交任务
int CTaskThread::OnReferTask(const DISTRIBUTED_HEADER * pNetRead)
{
	SET_TASK_STATE * pState = (SET_TASK_STATE *)pNetRead;

	//检测是否满足提交条件
	TaskBaseTable_Map_ConstIterator cit = CFixTaskTable::Instance().m_BaseTaskMap.find(pState->TaskID);
	if (cit == CFixTaskTable::Instance().m_BaseTaskMap.end())
	{
		return -1;
	}
	if (cit->second.tasktype_ != ttDayTask)
	{
		int ts = -1;
		if (OnQryTask(pState->from,pState->TaskID,ts)!=0)
		{
			return -1;
		}

		if (ts != tsYj && ts != tsWc)
		{
			return -1;
		}
		if (cit->second.tasktype_ == ttScKy)
		{
			if (ts == tsYj)  
			{
				if (CheckTaskIsComplete(pState->agent,pState->from,pState->TaskID,cit->second.contest_)==0)
			 {
				 OnCompleteTask(pState->agent,pState->from,pState->TaskID);
			 }
				else 
			 {
				 return -1;
			 }
			}
		}
		else 
		{	
			//非生产科研类
			if (ts !=tsWc)
			{
				return -1;
			}
		}

		//更改DBG
		PlayerTask uTable;
		uTable.setmasterid(pState->from);
		uTable.setdetailid(pState->TaskID);
		uTable.state_ = tsTj;
		uTable.tasktime_  = IceUtil::Time::now().toSeconds();
		if (Proxy.db_update(uTable,uTable)!=DB_Proxy::DB_SUCCEED)
		{
			return -1;
		}

		//回包
		REP_SET_TASK_STATE rep;
		rep.serialize(pState->agent,pState->from,pState->TaskID,pState->taskstate,1);
		asynch_write(rep.agent,(char *)&rep,sizeof(rep));

		PlayerOfficerTable officer;
		officer.setmasterid(pState->from);
		officer.setdetailid(pState->from);
		Proxy.db_select(officer);

		ISystemLog::getInstance()->playerTask_Log(officer.playerid_,officer.officername_,
			uTable.taskid_,cit->second.tasktype_ ,uTable.state_);

		//是否有后序任务
		TaskBaseTable_Map_ConstIterator mit;
		for (mit=CFixTaskTable::Instance().m_BaseTaskMap.begin();mit!=CFixTaskTable::Instance().m_BaseTaskMap.end();mit++)
		{
			if (mit->second.prvtaskid_ == pState->TaskID)
			{
				OnIssueSystemTask(pState->agent,pState->from,mit->first,mit->second.tasktype_);	
			}
		}
	}
	else
	{
		PlayerDayTask playerDayTask;
		playerDayTask.setmasterid(pState->from);
		playerDayTask.setdetailid(pState->TaskID);
		if (Proxy.db_select(playerDayTask) != DB_Proxy::DB_SUCCEED)
		{
			return -1;
		}
		if (playerDayTask.state_ != tsWc)
		{
			return -1;
		}
		playerDayTask.state_ = tsTj;
		if (Proxy.db_update(playerDayTask,playerDayTask) != DB_Proxy::DB_SUCCEED)
		{
			return -1;
		}
		//回包
		REP_SET_TASK_STATE rep;
		rep.serialize(pState->agent,pState->from,pState->TaskID,pState->taskstate,1);
		asynch_write(rep.agent,(char *)&rep,sizeof(rep));
	}
	//任务奖励
	GetTaskHortation(pState->from,pState->TaskID,cit->second.tasktype_);
	/*检查玩家是否完成所有任务
	if(AllTaskComplete(pState->from))
	{
		LPMailInfoParam_Item pMailInfoParam = 0;
		PlayerChatRoomTable playertb;
		playertb.setid(pState->from);
		if(Proxy.db_select(playertb) == DB_Proxy::DB_SUCCEED)
		{
			pMailInfoParam = GetMailInfoParam(playertb.country_,MAIL_TASK_END);
			if(pMailInfoParam)
			{
				Notify_Send_Mail email;
				email.type = Normal;
				email.ulRecieverPlayerID = pState->from;
				strcpy(email.szSendPlayerName,"任务系统");
				strcpy(email.sztitle,pMailInfoParam->Title);
				strcpy(email.szbody,pMailInfoParam->Content);
				email.serialize();
				event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));
			}
		}
	}*/
	return 0;
}

//接受任务
int CTaskThread::OnAcceptTask(const DISTRIBUTED_HEADER * pNetRead)
{
	SET_TASK_STATE * pState = (SET_TASK_STATE *)pNetRead;

	TaskBaseTable_Map_ConstIterator cit;
	cit = CFixTaskTable::Instance().m_BaseTaskMap.find(pState->TaskID);
	if (cit == CFixTaskTable::Instance().m_BaseTaskMap.end())
	{
		return - 1;
	}
	if (cit->second.tasktype_ != ttDayTask)
	{
		int ts = -1;
		if (OnQryTask(pState->from,pState->TaskID,ts)!=0)
		{
			return -1;
		}

		if (ts != tsWj)
		{
			return -1;
		}

		//更改DBG
		PlayerTask uTable;
		uTable.setmasterid(pState->from);
		uTable.setdetailid(pState->TaskID);
		uTable.state_ = tsYj;
		uTable.tasktime_  = IceUtil::Time::now().toSeconds();
		if (Proxy.db_update(uTable,uTable)!=DB_Proxy::DB_SUCCEED)
		{
			return -1;
		}

		//回包
		REP_SET_TASK_STATE rep;
		rep.serialize(pState->agent,pState->from,pState->TaskID,pState->taskstate,1);
		asynch_write(rep.agent,(char *)&rep,sizeof(rep));
		PlayerOfficerTable officer;
		officer.setmasterid(pState->from);
		officer.setdetailid(pState->from);
		Proxy.db_select(officer);

		ISystemLog::getInstance()->playerTask_Log(officer.playerid_,officer.officername_,
			uTable.taskid_,cit->second.tasktype_ ,uTable.state_);
		//检测任务是否完成(仅限生产科研类)
		if ((cit->second.tasktype_==ttScKy) && (CheckTaskIsComplete(pState->agent,pState->from,pState->TaskID,cit->second.contest_)==0))
			OnCompleteTask(pState->agent,pState->from,pState->TaskID);
	}
	else
	{//日常任务
		PlayerDayTask playerDayTask;
		playerDayTask.setmasterid(pState->from);
		playerDayTask.setdetailid(pState->TaskID);
		if (Proxy.db_select(playerDayTask) != DB_Proxy::DB_SUCCEED)
		{
			return -1;
		}
		if (playerDayTask.state_ != tsWj)
		{
			return -1;
		}
		playerDayTask.state_ = tsYj;
		if (Proxy.db_update(playerDayTask,playerDayTask) != DB_Proxy::DB_SUCCEED)
		{
			return -1;
		}

		//回包
		REP_SET_TASK_STATE rep;
		rep.serialize(pState->agent,pState->from,pState->TaskID,pState->taskstate,1);
		asynch_write(rep.agent,(char *)&rep,sizeof(rep));
		//通知对应的服务器
		if (playerDayTask.mapid_ >0)
		{
			std::map<int,DayTask_DetailInfo>::const_iterator dayTaskIter = CFixTaskTable::Instance().m_DayTaskMap.find(pState->TaskID);
			if (dayTaskIter != CFixTaskTable::Instance().m_DayTaskMap.end())
			{
				Notify_DayTaskOfPlayer_Request notifyFight;
				notifyFight.serialize(pState->from,playerDayTask.taskid_,dayTaskIter->second.kindType,dayTaskIter->second.itemType,
					dayTaskIter->second.itemID,dayTaskIter->second.number,dayTaskIter->second.itemLevel,playerDayTask.endtime_,
					MDM_GP_MAP_TASK,playerDayTask.mapid_);
				event_notify(playerDayTask.mapid_,(char*)&notifyFight,notifyFight.length);
			}
		}
		checkDayTask(pState->agent,pState->from);
	}
	
	//判断该任务是否需要发送邮件通知20090611		
	if(cit->second.mailtype_ >0)
	{
		LPMailInfoParam_Item pMailInfoParam = 0;
		PlayerChatRoomTable playertb;
		playertb.setid(pState->from);
		if(Proxy.db_select(playertb) == DB_Proxy::DB_SUCCEED)
		{
			pMailInfoParam = CFixTaskTable::Instance().getMailInfoParam(playertb.country_,cit->second.mailtype_);
			if(pMailInfoParam)
			{
				Notify_Send_Mail email;
				email.type = Normal;
				email.ulRecieverPlayerID = pState->from;
				strcpy(email.szSendPlayerName,"任务系统");
				strcpy(email.sztitle,pMailInfoParam->Title);
				strcpy(email.szbody,pMailInfoParam->Content);
				email.serialize();
				event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));
			}
		}
	}
	return 0;
}

//删除任务
int CTaskThread::OnDeleteTask(const DISTRIBUTED_HEADER * pNetRead)
{
	SET_TASK_STATE * pState = (SET_TASK_STATE *)pNetRead;
	TaskBaseTable_Map_ConstIterator mit;
	mit = CFixTaskTable::Instance().m_BaseTaskMap.find(pState->TaskID);
	if (mit == CFixTaskTable::Instance().m_BaseTaskMap.end())
	{
		return -1;
	}
	if (mit->second.tasktype_ != ttDayTask)
	{
		int ts = -1;
		if (OnQryTask(pState->from,pState->TaskID,ts)!=0)
		{
			return -1;
		}

		if (ts == tsSc || ts == tsTj)
		{
			return -1;
		}

		//更改DBG
		PlayerTask uTable;
		uTable.setmasterid(pState->from);
		uTable.setdetailid(pState->TaskID);
		uTable.state_ = tsSc;
		uTable.tasktime_  = IceUtil::Time::now().toSeconds();
		if (Proxy.db_update(uTable,uTable)!=DB_Proxy::DB_SUCCEED)
		{
			return -1;
		}

		//回包
		REP_SET_TASK_STATE rep;
		rep.serialize(pState->agent,pState->from,pState->TaskID,pState->taskstate,1);
		asynch_write(rep.agent,(char *)&rep,sizeof(rep));
		PlayerOfficerTable officer;
		officer.setmasterid(pState->from);
		officer.setdetailid(pState->from);
		Proxy.db_select(officer);

		ISystemLog::getInstance()->playerTask_Log(officer.playerid_,officer.officername_,
			uTable.taskid_,mit->second.tasktype_ ,uTable.state_);

		//是否有后序任务 20081209（删除后继续发后续任务）
		for (mit=CFixTaskTable::Instance().m_BaseTaskMap.begin();mit!=CFixTaskTable::Instance().m_BaseTaskMap.end();mit++)
		{
			if (mit->second.prvtaskid_ == pState->TaskID)
			{
				OnIssueSystemTask(pState->agent,pState->from,mit->first,mit->second.tasktype_);	
			}
		}

		/*检查玩家是否完成所有任务
		if(AllTaskComplete(pState->from))
		{
		LPMailInfoParam_Item pMailInfoParam = 0;
		PlayerChatRoomTable playertb;
		playertb.setid(pState->from);
		if(Proxy.db_select(playertb) == DB_Proxy::DB_SUCCEED)
		{
		pMailInfoParam = GetMailInfoParam(playertb.country_,MAIL_TASK_END);
		if(pMailInfoParam)
		{
		Notify_Send_Mail email;
		email.type = Normal;
		email.ulRecieverPlayerID = pState->from;
		strcpy(email.szSendPlayerName,"任务系统");
		strcpy(email.sztitle,pMailInfoParam->Title);
		strcpy(email.szbody,pMailInfoParam->Content);
		email.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));
		}
		}
		}*/
	}
	else
	{//日常任务
		PlayerDayTask playerDayTask;
		playerDayTask.setmasterid(pState->from);
		playerDayTask.setdetailid(pState->TaskID);
		if (Proxy.db_select(playerDayTask) != DB_Proxy::DB_SUCCEED)
		{
			return -1;
		}
		if (playerDayTask.state_ == tsTj || playerDayTask.state_ == tsSc)
		{//已提交、删除的任务无法删除
			return -1;
		}
		playerDayTask.state_ = tsSc;
		if (Proxy.db_update(playerDayTask,playerDayTask) != DB_Proxy::DB_SUCCEED)
		{
			return -1;
		}
		//回包
		REP_SET_TASK_STATE rep;
		rep.serialize(pState->agent,pState->from,pState->TaskID,pState->taskstate,1);
		asynch_write(rep.agent,(char *)&rep,sizeof(rep));
	}
	return 0;
}

void CTaskThread::issueDayTask(int agent,int playerid, int countryID)
{
	std::map<int,DayTask_DetailInfo>::const_iterator dayTaskIter;
	char * buf = 0;
	int count = 0;
	int i = 0;
	ISSUE_USER_TASK repNewTask;

	PlayerChatRoomTable playerInfo;
	playerInfo.setid(playerid);
	if (Proxy.db_select(playerInfo) != DB_Proxy::DB_SUCCEED)
	{
		return;
	}
	PlayerDayTask playerDayTask;
	playerDayTask.setmasterid(playerid);
	if (Proxy.db_multi_select(playerDayTask,buf,count) == DB_Proxy::DB_FAILED)
	{
		return;
	}

	PlayerOfficerTable officer;
	officer.setmasterid(playerid);
	officer.setdetailid(playerid);
	Proxy.db_select(officer);
	count = count /sizeof(PlayerDayTask);
	PlayerDayTask *pDayTaskList = (PlayerDayTask*)buf;
	playerDayTask.setmasterid(playerid);

	int mapID = 0;
	MapInfoTable maptb;

	repNewTask.serialize(agent,playerid);
	__int64 curTime = IceUtil::Time::now().toSeconds();
	int iconslevel = 0;
	if (playerInfo.consortiaid_ >0)
	{
		ConsortiaInfo consInfo;
		consInfo.setid(playerInfo.consortiaid_);
		if (Proxy.db_select(consInfo) == DB_Proxy::DB_SUCCEED)
		{
			iconslevel = consInfo.level_;
		}
	}
	for (dayTaskIter = CFixTaskTable::Instance().m_DayTaskMap.begin();
		dayTaskIter != CFixTaskTable::Instance().m_DayTaskMap.end();dayTaskIter++)
	{
		for (i=0;i<count;++i)
		{
			if (pDayTaskList[i].taskid_ == dayTaskIter->first)
			{
				break;
			}
		}
		if (i < count)
		{//玩家有该日常任务
			continue;
		}
		if (dayTaskIter->second.taskType == ttConsortiaTask && iconslevel != dayTaskIter->second.taskLevel)//军团任务
		{
			continue;
		}

		mapID = dayTaskIter->second.maps[playerInfo.country_];

		memset(&maptb, 0, sizeof(maptb));
		maptb.setid(mapID);

		if(Proxy.db_select(maptb) == DB_Proxy::DB_FAILED)
		{
			;
		}

		// 不是相同阵营的或者是新手村的日常任务出来
		if (countryID != maptb.countryid_ || 100 == maptb.state_)
		{
			playerDayTask.setdetailid(dayTaskIter->first);
			playerDayTask.mapid_ = mapID;
			playerDayTask.starttime_ = curTime;
			playerDayTask.endtime_ = dayTaskIter->second.endTime;
			playerDayTask.state_ = tsWj;
			if (Proxy.db_insert(playerDayTask) == DB_Proxy::DB_SUCCEED)
			{
				//通知客户端
				repNewTask.TaskInfo.TaskID = playerDayTask.taskid_;
				repNewTask.TaskInfo.OtherInfo.taskfrom = playerDayTask.mapid_;
				repNewTask.TaskInfo.OtherInfo.taskstate= playerDayTask.state_;
				repNewTask.TaskInfo.OtherInfo.time		=playerDayTask.starttime_; 
				asynch_write(repNewTask.agent,(char *)&repNewTask,repNewTask.length);

				ISystemLog::getInstance()->playerTask_Log(officer.playerid_,officer.officername_,
					playerDayTask.taskid_,ttDayTask,playerDayTask.state_);
			}
		}
	}

	if (buf)
	{
		delete[] buf;
		buf = 0;
	}
}
//发布任务
int CTaskThread::OnIssueSystemTask(int handle,unsigned long ulPlayerID,unsigned int TaskID,int tasktype)
{
	if (tasktype == ttDayTask)
	{
	}
	else
	{
		const unsigned __int64 dtTime=IceUtil::Time::now().toSeconds();
		//1.插入任务至玩家任务表
		PlayerTask uTable;
		uTable.playerid_=ulPlayerID;
		uTable.taskid_=TaskID;
		uTable.state_=tsWj;
		uTable.tasktime_=dtTime;
		if (Proxy.db_insert(uTable)!=DB_Proxy::DB_SUCCEED)
		{
			return -1;
		}

		//2.发布任务至玩家
		ISSUE_USER_TASK rep;
		rep.serialize(handle,ulPlayerID);
		rep.TaskInfo.TaskID = TaskID;
		rep.TaskInfo.OtherInfo.taskfrom = tfSystem;
		rep.TaskInfo.OtherInfo.taskstate= tsWj;
		rep.TaskInfo.OtherInfo.time		=dtTime; 
		asynch_write(rep.agent,(char *)&rep,sizeof(rep));

		PlayerOfficerTable officer;
		officer.setmasterid(ulPlayerID);
		officer.setdetailid(ulPlayerID);
		Proxy.db_select(officer);

		ISystemLog::getInstance()->playerTask_Log(officer.playerid_,officer.officername_,
			uTable.taskid_,tasktype,uTable.state_);
	}
	return 0;
}

//任务完成
int CTaskThread::OnCompleteTask(int handle, unsigned long playerid, unsigned int taskid)
{
 	//更改任务列表
	PlayerTask uTable;
	uTable.setmasterid(playerid);
	uTable.setdetailid(taskid);
	uTable.state_=tsWc;
	uTable.tasktime_ = IceUtil::Time::now().toSeconds();
	if (Proxy.db_update(uTable,uTable)!=DB_Proxy::DB_SUCCEED )
	{
		return -1;
	}

	//发送状态给玩家
	REP_TASKSTATE_COMPLETE rep;
	rep.serialize(handle,playerid,taskid);
	asynch_write(handle,(char *)&rep,sizeof(rep));

	PlayerOfficerTable officer;
	officer.setmasterid(playerid);
	officer.setdetailid(playerid);
	Proxy.db_select(officer);
	TaskBaseTable_Map_ConstIterator mit = CFixTaskTable::Instance().m_BaseTaskMap.find(taskid);
	if (mit != CFixTaskTable::Instance().m_BaseTaskMap.end())
	{
		ISystemLog::getInstance()->playerTask_Log(officer.playerid_,officer.officername_,
			uTable.taskid_,mit->second.tasktype_ ,uTable.state_);
	}

	return 0;
}

/* 输助Function */

//查询任务状态
int CTaskThread::OnQryTask(unsigned long playerid,unsigned int taskid, int & ts)
{
	ts = -1;
	PlayerTask uTable;
	uTable.setmasterid(playerid);
	uTable.setdetailid(taskid);
	if (Proxy.db_select(uTable)!=DB_Proxy::DB_SUCCEED)
	{
#ifdef _DEBUG
		//printf("查询玩家任务状态时失败  PlayerID: %d, TaskID: %d \n",playerid,taskid);
#endif
		return -1;
	}
	ts = uTable.state_;
	return 0;
}

//检测任务是否完成(针对生产科研类任务)
int CTaskThread::CheckTaskIsComplete(int agent,unsigned long playerid, unsigned int taskid, const char * pContest)
{
	int iFlag;
	int TypeID;		
	int ItemID;
	int ItemNum;
	int ItemLevel;
	
	if (sscanf(pContest,"%d:%d:%d:%d:%d",&iFlag,&TypeID,&ItemID,&ItemNum,&ItemLevel) != 5)
    {
#ifdef _DEBUG
		 //printf("检测任务是否完成时，任务 TaskId: %d, 任务内容 Contest: %s 其格式不正确 ...\n",taskid,pContest);   
#endif
		 return -1;
    } 
	 if (CheckItem(agent,playerid,taskid,TypeID,ItemID,ItemNum,iFlag,ItemLevel) != 0)
	 {
		return -1;
    }
	 return 0;
}


//获取任务奖励
void CTaskThread::GetTaskHortation(unsigned long playerID ,unsigned int TaskID,const int tasktype)
{
	std::map<unsigned int,HORTATION_Plus>::const_iterator mit;
	mit = CFixTaskTable::Instance().m_HortationMap.find(TaskID);
	int i = 0;
	PlayerOfficerTable officer;
	officer.setmasterid(playerID);
	officer.setdetailid(playerID);
	Proxy.db_select(officer);
	PlayerItemStorageTable iteminfo;
	int oldcount = 0,curcount = 0;
	if (mit != CFixTaskTable::Instance().m_HortationMap.end())
	{
		int mapID = 0;
		int cityID = 0;
		if (mit->second.b0 || mit->second.SoldierCount >0)
		{
			if (GetHomeCityInfo(&Proxy,playerID,mapID,cityID) !=0)
			{
				return;
			}
		}
		if (mit->second.b0)  //道具
		{
			for (i=0;i<mit->second.h0.count;i++)
			{
				if(0==UpdateProductionAmountEx(&Proxy,playerID,mapID,cityID,mit->second.h0.hlist[i].hid,mit->second.h0.hlist[i].htype,
					mit->second.h0.hlist[i].num,oldcount,curcount))
				{
					ISystemLog::getInstance()->playerProduce_TaskAward_Log(officer.playerid_,officer.officername_,
						mapID,cityID,mit->second.h0.hlist[i].htype,mit->second.h0.hlist[i].hid,oldcount,curcount,
						mit->second.h0.hlist[i].num,TaskID,tasktype);
				}
			}
		}
		//装备
		if (mit->second.b1)  
		{
			bool bOverlap = false;
			for (i=0;i<mit->second.h1.count;i++)
			{
				HeroEquipAttribute *heroEqupAttr = CFixTaskTable::Instance().getHeroEquipAttribute(mit->second.h1.hlist[i].hid);
				if (heroEqupAttr == NULL)
				{
					continue;
				}
				PackStoreParam *pPackStoreParam = CFixTaskTable::Instance().getPackStoreParam(heroEqupAttr->postion);
				if (pPackStoreParam)
				{
					bOverlap = pPackStoreParam->IsOverlap;
				}
				else
				{
					bOverlap = false;
				}
				/*
				if (mit->second.h1.hlist[i].num == 1)
				{
					OnInsertHeroEquip(Proxy,playerID,1,heroEqupAttr->itemID,heroEqupAttr->level,heroEqupAttr->postion,bOverlap,heroEqupAttr->skill,heroEqupAttr->skillLevel,iteminfo);
				}
				else
				{
				*/
				for (int j=0;j<mit->second.h1.hlist[i].num;j++)
				{
					OnInsertHeroEquip(Proxy,playerID,1,heroEqupAttr->itemID,heroEqupAttr->level,heroEqupAttr->postion,bOverlap,heroEqupAttr->skill,heroEqupAttr->skillLevel,iteminfo);
					ISystemLog::getInstance()->playerEquip_TaskAward_Log(officer.playerid_,officer.officername_,
						heroEqupAttr->postion,heroEqupAttr->itemID,1);
				}
				//}
			}
		}
		//荣誉点，经验值
		if (mit->second.b2) 
		{
			tagHeroParam  HeroParam;
			memset(&HeroParam,0,sizeof(HeroParam));
			HeroParam.lPlayerID = playerID;
			HeroParam.lHeroID=playerID;
			HeroParam.iCredit = mit->second.h2.i1;
			HeroParam.iExp = mit->second.h2.i2;
			OnHeroGrowUp(Proxy,HeroParam,TASKAWARD_METHOD);
		}
		//金钱
		if (mit->second.b3) 
		{
			PlayerChatRoomTable tb3;
			tb3.setid(playerID);
			tb3.money_=mit->second.money;
			if (Proxy.db_increase(tb3)!=DB_Proxy::DB_SUCCEED)
			{
			}
			else
			{
				ISystemLog::getInstance()->playerMoney_TaskAward_Log(officer.playerid_,officer.officername_,
					tb3.money_ - mit->second.money,tb3.money_,mit->second.money,TaskID,tasktype);
			}
		}
		//粮食，木材，铁，皮料
		if (mit->second.b4)  
		{
			char * buf1 = 0;
			int c1     = 0;
			PlayerCity tb1;
			tb1.setmasterid(playerID);
			if (Proxy.db_multi_select(tb1,buf1,c1)  != DB_Proxy::DB_SUCCEED)
			{
				return ;
			}
			int ts1 = sizeof(tb1);
			int num1=c1 / ts1;
			int total =mit->second.h4.nums[0]+mit->second.h4.nums[1]+mit->second.h4.nums[2]+mit->second.h4.nums[3];
			
			//放入新手村
			int pos=-1;
			for (int i1=0;i1<num1;++i1)
			{
				//if ( ((PlayerCity*)(buf1+i1*ts1))->mapid_ ==1029 || ((PlayerCity*)(buf1+i1*ts1))->mapid_ ==1030 )
				if ( ((PlayerCity*)(buf1+i1*ts1))->state_ == 100 )
				{
					pos=i1;
					break;
				}
			}
			if (pos==-1) return ;

			int mapid=((PlayerCity*)(buf1+pos*ts1))->mapid_;
			int cityid=((PlayerCity*)(buf1+pos*ts1))->cityid_;
			delete [] buf1;
			buf1 = 0;

			PlayerResource tb2;
			tb2.setmasterid(cityid,mapid);
			tb2.setdetailid(playerID);
			if (Proxy.db_select(tb2)!=DB_Proxy::DB_SUCCEED)
			{
				return;
			}

			if (total>tb2.content_) 
			{
				return;
			}
			
			memset(&tb2,0,sizeof(PlayerResource));
			tb2.setmasterid(cityid,mapid);
			tb2.setdetailid(playerID);
			tb2.foodcount_=mit->second.h4.nums[0];
			tb2.woodcount_=mit->second.h4.nums[1];
			tb2.ironcount_=mit->second.h4.nums[3];
			tb2.skincount_=mit->second.h4.nums[2];
			tb2.content_=-total;
			if (Proxy.db_increase(tb2)!=DB_Proxy::DB_SUCCEED)
			{
#ifdef	_DEBUG
				//printf("任务奖励粮食/木材/铁/皮料时失败,写数据不成功。 playerid:%d	taskid:%d \n",playerID,TaskID);
#endif
			}

		}

		
		//奖励卡
		LPCardParam pCardParam = 0;
		PackStoreParam *pPackStoreParam = 0;
		bool bOverlap = false;
		for (i =0;i<mit->second.CardCount;i++)
		{
			pCardParam = CFixTaskTable::Instance().getCardConfig(mit->second.CardItem[i].CardItemID);
			if (pCardParam == 0)
			{
				continue;
			}
			pPackStoreParam = CFixTaskTable::Instance().getPackStoreParam(pCardParam->Postion);
			if (pPackStoreParam)
			{
				bOverlap = pPackStoreParam->IsOverlap;
			}
			else
			{
				bOverlap = false;
			}
			for (int j=0;j<mit->second.CardItem[i].Count;j++)
			{
				OnInsertHeroEquip(&Proxy,playerID,1,mit->second.CardItem[i].CardItemID,1,pCardParam->Postion,bOverlap,0,0,iteminfo);
				ISystemLog::getInstance()->playerEquip_TaskAward_Log(officer.playerid_,officer.officername_,
					pCardParam->Postion,mit->second.CardItem[i].CardItemID,1);
			}
		}
		//奖励GO点
		if (mit->second.GoDian >0)
		{
			Notify_AddMoney_Request addmoney_msg;
			addmoney_msg.serialize(0,playerID,mit->second.GoDian,MDM_GP_MAP_TASK);
			event_notify(MDM_GP_MAP_CHARGE,(char*)&addmoney_msg,addmoney_msg.length);
		}
		//奖励技能点
		if (mit->second.TechDot >0)
		{
			PlayerOfficerTable mOfficer;
			mOfficer.setmasterid(playerID);
			mOfficer.setdetailid(playerID);
			mOfficer.techdot_ = mit->second.TechDot;
			if (Proxy.db_increase(mOfficer) != DB_Proxy::DB_SUCCEED)
			{
			}
			ISystemLog::getInstance()->heroSkillDot_TaskAward_Log(officer.playerid_,officer.officerid_,officer.officername_,
				officer.techdot_,mOfficer.techdot_,mit->second.TechDot,TaskID,tasktype);
		}
		//奖励士兵,奖励到新手村
		if (mit->second.SoldierCount >0)
		{
			PlayerSoldierRace soldierRace,updateSoldierRace;
			soldierRace.setmasterid(mapID,cityID);
			soldierRace.setdetailid(playerID);
			int iRet = Proxy.db_select(soldierRace) ;
			int iRace = 1;
			
			if (iRet != DB_Proxy::DB_FAILED)
			{
				updateSoldierRace.setmasterid(mapID,cityID);
				updateSoldierRace.setdetailid(playerID);
				if (soldierRace.human_ >0)
				{//人类1
					iRace = 1;
					oldcount = soldierRace.human_;
					updateSoldierRace.human_ = mit->second.SoldierCount;
				}
				else if (soldierRace.fairy_ >0)
				{//精灵2:
					iRace = 2;
					oldcount = soldierRace.fairy_;
					updateSoldierRace.fairy_ = mit->second.SoldierCount;
				}
				else if (soldierRace.mannikin_ >0)
				{//矮人
					iRace = 3;
					oldcount = soldierRace.mannikin_;
					updateSoldierRace.mannikin_ = mit->second.SoldierCount;
				}
				else if (soldierRace.dandiprat_ >0)
				{//侏儒
					iRace = 4;
					oldcount = soldierRace.dandiprat_;
					updateSoldierRace.dandiprat_ = mit->second.SoldierCount;
				}
				else if(soldierRace.halfanimal_ >0)
				{//半兽人
					iRace = 5;
					oldcount = soldierRace.dandiprat_;
					updateSoldierRace.halfanimal_ = mit->second.SoldierCount;
				}
				else if (soldierRace.cattle_ >0)
				{//牛头人
					iRace = 6;
					oldcount = soldierRace.cattle_;
					updateSoldierRace.cattle_ = mit->second.SoldierCount;
				}
				else if (soldierRace.blackfairy_ >0)
				{//黑精灵
					iRace = 7;
					oldcount = soldierRace.blackfairy_;
					updateSoldierRace.blackfairy_ = mit->second.SoldierCount;
				}
				else if (soldierRace.decpesrson_ >0)
				{//亡灵
					iRace = 8;
					oldcount = soldierRace.blackfairy_;
					updateSoldierRace.human_ = mit->second.SoldierCount;
				}
				else
				{
					iRace = 1;
					oldcount = soldierRace.human_;
					updateSoldierRace.human_ = mit->second.SoldierCount;
				}
				if (iRet == DB_Proxy::DB_SUCCEED)
				{
					if (Proxy.db_increase(updateSoldierRace) != DB_Proxy::DB_SUCCEED)
					{
					}
				}
				else
				{
					if (Proxy.db_insert(updateSoldierRace) != DB_Proxy::DB_SUCCEED)
					{
					}
				}
				ISystemLog::getInstance()->playerSoldier_TaskAward_Log(officer.playerid_,officer.officername_,
					updateSoldierRace.mapid_,updateSoldierRace.cityid_,iRace,oldcount,oldcount+mit->second.SoldierCount,mit->second.SoldierCount,
					TaskID,tasktype);
			}
		}
	}  
	return ;
}

//查询数量
int  CTaskThread::CheckItem(int agent,unsigned long PlayerID, int taskid, int TypeID, int ItemID, int ItemNum, int iFlag,int iLevel)
{
	switch (iFlag)
	{
	//建筑
	case TASKTARGET_CONSTRUCTION://1：建筑
		{
			char * buf1 = 0;
			int c1     = 0;
			PlayerCity tb1;
			tb1.setmasterid(PlayerID);
			if (Proxy.db_multi_select(tb1,buf1,c1)  != DB_Proxy::DB_SUCCEED)
			{
				return -1;
			}
			int ts1 = sizeof(tb1);
			int num1=c1 / ts1;
     		int sl = 0;
			for (int i1=0;i1<num1;++i1)
			{
				char * buf2 = 0;
				int c2 = 0;
				PlayerConstruction tb2;
				tb2.setmasterid( ((PlayerCity *)(buf1+i1*ts1))->mapid_,((PlayerCity *)(buf1+i1*ts1))->cityid_,PlayerID);
				if (Proxy.db_multi_select(tb2,buf2,c2) != DB_Proxy::DB_SUCCEED)
				{
					continue;
				}
				int ts2 = sizeof(tb2);
				int num2=c2 / ts2;
				PlayerConstruction * pPlayerConstruction;

				for (int i2=0;i2<num2;++i2)
				{
					pPlayerConstruction = (PlayerConstruction*)(buf2+i2*ts2);

					if ( pPlayerConstruction->constructionid_ == ItemID  && pPlayerConstruction->level_>=iLevel )
					{
						if ( (++sl) >=ItemNum) 
						{
							delete [] buf2;
							buf2= 0;
							delete [] buf1;
							buf1 = 0;
							return 0;
						}
					}
				}
				delete [] buf2;
				buf2 = 0;
			}
			delete [] buf1;
			buf1 = 0;
			break;
		}
	//科研
	case TASKTARGET_RESEARCH://2：科研
		{
			ResearchItem tb1;
			tb1.setmasterid(PlayerID,TypeID);
			tb1.setdetailid(ItemID);
			if (Proxy.db_select(tb1) == DB_Proxy::DB_SUCCEED)
			{
				return 0;
			}
			break;
		}
	//生产道具
	case TASKTARGET_PRODUCTION://3：生产道具
		{
			char * buf1 = 0;
			int c1     = 0;
			PlayerCity tb1;
			tb1.setmasterid(PlayerID);
			if (Proxy.db_multi_select(tb1,buf1,c1)  != DB_Proxy::DB_SUCCEED)
			{
				return -1;
			}
			int ts1 = sizeof(tb1);
			int num1=c1 / ts1;
			int sl = 0;
			for (int i1=0;i1<num1;++i1)
			{
				ProductionAmount tb2;
				tb2.setmasterid( ((PlayerCity *)(buf1+i1*ts1))->mapid_,((PlayerCity *)(buf1+i1*ts1))->cityid_,PlayerID,TypeID);
				tb2.setdetailid(ItemID);
				if (Proxy.db_select(tb2) == DB_Proxy::DB_SUCCEED)
				{
					sl+=tb2.productioncount_;
					if (sl>=ItemNum) 
					{
						delete [] buf1;
						buf1 = 0;
						return 0;
					}
				}
			}
			delete [] buf1;
			buf1 = 0;
			break;
		}
	//招募新兵
	case TASKTARGET_SOLDIER://4：招募新兵
		{
			char * buf1 = 0;
			int c1     = 0;
			PlayerCity tb1;
			tb1.setmasterid(PlayerID);
			if (Proxy.db_multi_select(tb1,buf1,c1)  != DB_Proxy::DB_SUCCEED)
			{
				return -1;
			}
			int ts1 = sizeof(tb1);
			int num1=c1 / ts1;
			int sl = 0;
			for (int i1=0;i1<num1;++i1)
			{
				PlayerSoldierRace tb2;
				tb2.setmasterid( ((PlayerCity*)(buf1+i1*ts1))->mapid_,((PlayerCity*)(buf1+i1*ts1))->cityid_);
				tb2.setdetailid(PlayerID);
				if (Proxy.db_select(tb2) ==DB_Proxy::DB_SUCCEED)
				{
					/*
					switch (ItemID)
					{
					case 1:
						{
							sl+=tb2.human_;
							break;
						}
					case 2:
						{
							sl+=tb2.mannikin_;
							break;
						}
					case 3:
						{
							sl+=tb2.fairy_;
							break;
						}
					case 4:
						{
							sl+=tb2.dandiprat_;
							break;
						}
					case 5:
						{
							sl+=tb2.halfanimal_;
							break;
						}
					case 6:
						{
							sl+=tb2.cattle_;
							break;
						}
					case 7:
						{
							sl+=tb2.blackfairy_;
							break;
						}
					case 8:
						{
							sl+=tb2.decpesrson_;
							break;
						}
					}*/
					sl = sl + tb2.human_ + tb2.mannikin_ + tb2.fairy_ + tb2.dandiprat_+ \
						tb2.halfanimal_ + tb2.cattle_ + tb2.blackfairy_ + tb2.decpesrson_;	
					if (sl>=ItemNum) 
					{
						delete [] buf1;
						buf1 = 0;
						return 0;
					}
				}

			}
			delete [] buf1;
			buf1 = 0;
			break;
		}
	//组建部队
	case TASKTARGET_ARMY://5：组建部队:部队数量
		{
			char * buf1 = 0;
			int c1 = 0;
			PlayerOfficerTable tb1;
			tb1.setmasterid(PlayerID);
			if (Proxy.db_multi_select(tb1,buf1,c1) !=DB_Proxy::DB_SUCCEED)
			{
				return -1;
			}
			int ts1 = sizeof(tb1);
			int num1 =c1 / ts1;
			int sl = 0;
			PlayerOfficerTable * pp;
			pp=(PlayerOfficerTable*)buf1;
			for (int i1=0;i1<num1;++i1)
			{
				if (pp[i1].armyid_>0 && pp[i1].state_==1)
				{
					if ( (++sl) >=ItemNum )
					{
						delete [] buf1;
						buf1 = 0;
						return 0;
					}
				}
			}
			delete [] buf1;
			buf1 = 0;
			break;
		}
	//殖民部队
	case TASKTARGET_SPECIALARMY://6：特殊部队：殖民部队、运输部队
		{
			//PlayerOfficerTable ArmyTable
			char * buf=0;
			int count=0;
			PlayerOfficerTable tb1;
			tb1.setmasterid(PlayerID);
			if (Proxy.db_multi_select(tb1,buf,count)!=DB_Proxy::DB_SUCCEED)
			{
				return -1;
			}
			
			PlayerOfficerTable * p=(PlayerOfficerTable *)buf;
			int sl=0;
			int num=count/sizeof(PlayerOfficerTable);
			for (int i=0;i<num;++i)
			{
				if (p[i].armyid_>0 && p[i].state_==1)
				{
					ArmyTable tb2;
					tb2.setmasterid(p[i].mapid_);
					tb2.setdetailid(p[i].armyid_);
					if (Proxy.db_select(tb2)==DB_Proxy::DB_SUCCEED) 
					{
						//殖民部队 150 151 152
						//运输部队 175 176 177
						if (ItemID==1)
						{
							if (tb2.fweapon_==150 || tb2.fweapon_==151 || tb2.fweapon_==152) 
							{
								if ((++sl)>=ItemNum)
								{
									delete [] buf;
									buf = 0;
									return 0;
								}
							}
						}
						else
						if (ItemID==2)
						{
							if (tb2.fweapon_==175 || tb2.fweapon_==176 || tb2.fweapon_==177) 
							{
								if ((++sl)>=ItemNum)
								{
									delete [] buf;
									buf = 0;
									return 0;
								}
							}
						}
					}
				}
			}
			delete [] buf;
			buf = 0;
			break;
		}
	//殖民城市
	case TASKTARGET_PLAYERCITY://7：殖民城市:部队数量
		{
			PlayerCity tb ;
			tb.setmasterid(PlayerID);
			char * buf=0;
			int count=0;
			if (Proxy.db_multi_select(tb,buf,count)  != DB_Proxy::DB_SUCCEED)
			{
				return -1;
			}
			int num=count/(sizeof(PlayerCity));
			delete [] buf;
			buf = 0;
			return (num>=ItemNum)?0:-1;
		}
	//捐献任务
	case TASKTARGET_CONTRIBUTE ://8：捐献任务
		{
			PlayerChatRoomTable tb;
			tb.setid(PlayerID);
			if (Proxy.db_select(tb) != DB_Proxy::DB_SUCCEED)
			{
				return -1;
			}
			return (tb.contribute_>=ItemNum)?0:-1;
		}
	//招募英雄
	case TASKTARGET_HERO ://9：招募英雄
		{
			PlayerOfficerTable tb;
			tb.setmasterid(PlayerID);
			char * buf=0;
			int count =0;
			if (Proxy.db_multi_select(tb,buf,count) != DB_Proxy::DB_SUCCEED) return -1;
			PlayerOfficerTable * ptb=(PlayerOfficerTable*)buf;
			int num = count / sizeof(PlayerOfficerTable);
			int sl=0;
			bool bHavHero = false;
			for (int i=0;i<num;i++)
			{
				if (ptb[i].level_>=iLevel) 
				{
					sl++;
					if(TypeID >0 && ptb[i].jobid_ == TypeID)
					{
						bHavHero = true;
					}
				}
			}
			if(buf)
			{
				delete [] buf;
				buf = 0;
			}
			if(sl >=ItemNum)
			{
				if(TypeID <=0)
					return 0;
				else
					return bHavHero?0:-1;
			}
			else
			{
				return -1;
			}
			break;
		}
	//加入军团
	case TASKTARGET_CONSORTIA ://10：加入军团
		{
			PlayerChatRoomTable tb;
			tb.setid(PlayerID);
			if (Proxy.db_select(tb) != DB_Proxy::DB_SUCCEED) return -1;
			return (tb.consortiaid_>0)?0:-1;
		}
	//添加好友
	case TASKTARGET_FRIENDS ://11：添加好友
		{
			FirendTable tb;
			tb.setmasterid(PlayerID,1);  //好友为1，黑名单为2
			char * buf = 0;
			int count=0;
			if (Proxy.db_multi_select(tb,buf,count) != DB_Proxy::DB_SUCCEED) return -1;
			int num = count / sizeof(FirendTable) ;
			delete [] buf ;
			buf = 0;
			return (num>=ItemNum)?0:-1;
		}
	//市场挂牌交易
	case TASKTARGET_MARKET ://12：市场挂牌交易
		{
			PlayerChatRoomTable ChatTab;
			ChatTab.setid(PlayerID);
			if (Proxy.db_select(ChatTab) != DB_Proxy::DB_SUCCEED) return -1;

			Market tb;
			int goodstype=1;
			int num=0;
			while (1)
			{
				char * buf;
				int count=0;
				tb.setmasterid(ChatTab.country_,goodstype);
				if (Proxy.db_multi_select(tb,buf,count) !=DB_Proxy::DB_SUCCEED) 
				{
					if (++goodstype>6) break;
					continue;
				}
				int c=count/sizeof(Market);
				Market * ptb=(Market*)buf;
				for (int i=0;i<c;i++)
				{
					if (ptb[i].playerid_==PlayerID) 
					{
						if (++num>=ItemNum) 
						{
							delete[] buf;
							buf = 0;
							return 0;
						}
					}
				}
				delete [] buf;
				buf = 0;
				if (++goodstype>6) break;
			}
			break;
		}
	//收集套装
	case TASKTARGET_EQUIPMENT://13：收集套装
		{
			PlayerOfficerTable OfficerTb;
			OfficerTb.setmasterid(PlayerID);
			char * buf1=0;
			int c1=0;
			if (Proxy.db_multi_select(OfficerTb,buf1,c1) != DB_Proxy::DB_SUCCEED) return -1;
			PlayerOfficerTable* pOfficerTb=(PlayerOfficerTable*)buf1;
			int num1=c1/sizeof(PlayerOfficerTable);
			unsigned char sl[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
			unsigned char rlt[4]={0,0,0,0};
			for (int i=0;i<=num1;i++)
			{
				PlayerItemStorageTable tb;
				if (i<num1)
					tb.setmasterid(pOfficerTb[i].officerid_,PlayerID);
				else
					tb.setmasterid(1,PlayerID);
				char * buf2;
				int c2=0;
				if (Proxy.db_multi_select(tb,buf2,c2) !=DB_Proxy::DB_SUCCEED) continue;
				int num2=c2/sizeof(PlayerItemStorageTable);
				PlayerItemStorageTable * ptb=(PlayerItemStorageTable*)buf2;
				for (int j=0;j<num2;j++)
				{
					if (ptb[j].itemid_>=1 && ptb[j].itemid_<=20 && sl[ptb[j].itemid_-1]==0) 
					{
						sl[ptb[j].itemid_-1]=1;
						++rlt[(ptb[j].itemid_-1)/5];
					}
				}
				delete [] buf2;
				buf2 = 0;
				if (rlt[0]>=5 || rlt[1]>=5 || rlt[2]>=5 || rlt[3]>=5)
				{
					delete[] buf1;
					buf1 = 0;
					return 0;
				}
			}
			delete [] buf1;
			buf1 = 0;
			break;
		}
	//收集基本资源
	case TASKTARGET_BASERESOURCE ://14：收集基本资源
		{
			PlayerCity tb1;
			char * buf1=0;
			int count1=0;
			tb1.setmasterid(PlayerID);
			if (Proxy.db_multi_select(tb1,buf1,count1) !=DB_Proxy::DB_SUCCEED) return -1;
			int num1=count1/sizeof(PlayerCity);
			PlayerCity * ptb1=(PlayerCity *)buf1;
			int sl=0;
			for(int i =0;i<num1;i++)
			{
				PlayerResource tb2;
				tb2.setmasterid(ptb1[i].cityid_,ptb1[i].mapid_);
				tb2.setdetailid(PlayerID);
				if (Proxy.db_select(tb2) ==DB_Proxy::DB_SUCCEED) 
				{
					switch (ItemID)
					{
					case 1:
						{
							sl+=tb2.foodcount_;
							break;
						}
					case 2:
						{
							sl+=tb2.woodcount_;
							break;
						}
					case 3:
						{
							sl+=tb2.skincount_;
							break;
						}
					case 4:
						{
							sl+=tb2.ironcount_;
							break;
						}
					}
					if (sl>=ItemNum)
					{
						delete [] buf1;
						buf1 = 0;
						return 0;
					}
				}
			}
			delete [] buf1;
			buf1 = 0;
			break;
		}
		//军衔
	case TASKTARGET_MILITARYRANK://15：军衔
		{
			PlayerOfficerTable playerofficer;
			playerofficer.setmasterid(PlayerID);
			playerofficer.setdetailid(PlayerID);
			if(Proxy.db_select(playerofficer) != DB_Proxy::DB_SUCCEED)
				return -1;
			if(playerofficer.militaryrank_ >= ItemNum)
				return 0;
		}
		break;
		//GO点查询
	case TASKTARGET_GOCOUNT://16：GO点查询
		{
			Notify_QueryMoney_Request notifymsg;
			notifymsg.serialize(agent,PlayerID,taskid,MDM_GP_MAP_TASK);
			event_notify(MDM_GP_MAP_CHARGE,(char*)&notifymsg,notifymsg.length);
		}
		break;
		//查询卡
	case TASKTARGET_CARD: //17：查询卡
		{
			//首先判断公共背包
			int iItemCount = 0;
			iItemCount = GetItemCount(PlayerID,1,TypeID,ItemID);
			if(iItemCount >= ItemNum)
				return 0;
			//判断英雄背包
			PlayerOfficerTable officertb;
			PlayerOfficerTable *pOfficerList = 0;
			char *buf = 0;
			int count = 0;
			officertb.setmasterid(PlayerID);
			if(Proxy.db_multi_select(officertb,buf,count)!=DB_Proxy::DB_SUCCEED)
				return -1;
			count /=sizeof(PlayerOfficerTable);
			pOfficerList = (PlayerOfficerTable*)buf;
			for(int i=0;i<count;i++)
			{
				iItemCount += GetItemCount(PlayerID,pOfficerList[i].officerid_,TypeID,ItemID);
				if(iItemCount >>= ItemNum)
					break;
			}
			if(buf)
			{
				delete[] buf;
				buf = 0;
			}
			if(iItemCount >= ItemNum)
				return 0;
		}
		break;
	default:
		{
		}
		break;
	}
	return -1;
}

//检查玩家是否所有任务已完成
bool CTaskThread::AllTaskComplete(int playerid)
{
	PlayerTask playertask;
	PlayerTask *pPlayerTaskList = 0;
	char *buf = 0;
	int count = 0;
	bool bAllComplete = true;
	playertask.setmasterid(playerid);
	if(Proxy.db_multi_select(playertask,buf,count) != DB_Proxy::DB_SUCCEED)
		return false;

	count = count/sizeof(PlayerTask);
	pPlayerTaskList = (PlayerTask*)buf;
	for(int i=0;i<count;i++)
	{
		if(pPlayerTaskList[i].state_ != tsTj && pPlayerTaskList[i].state_ != tsSc)
		{
			bAllComplete = false;
			break;
		}
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	return bAllComplete;
}
int CTaskThread::GetItemCount(int playerid,int officerid,int itemid,int pos)
{
	int iItemCount = 0;
	PlayerItemStorageTable itemtb;
	PlayerItemStorageTable *pItemList = 0;
	int count = 0;
	char *buf = 0;
	itemtb.setmasterid(officerid,playerid);
	if(Proxy.db_multi_select(itemtb,buf,count) != DB_Proxy::DB_SUCCEED)
		return 0;
	count /= sizeof(PlayerItemStorageTable);
	pItemList = (PlayerItemStorageTable*)buf;
	for(int i=0;i<count;i++)
	{
		if(pItemList[i].itemid_ == itemid && pItemList[i].pos_ == pos)
			iItemCount++;
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	return iItemCount;
}
int CTaskThread::OnNotify_QueryMoney(const DISTRIBUTED_HEADER * pNetRead)
{
	LPNotify_QueryMoney_Response req_msg = (LPNotify_QueryMoney_Response)pNetRead;
	if(req_msg->from != MDM_GP_MAP_CHARGE)
		return -2;

	if (req_msg->Money < 0)
	{
		return -1;
	}
	//检查任务是否完成
	TaskBaseTable_Map_ConstIterator iter;
	iter = CFixTaskTable::Instance().m_BaseTaskMap.find(req_msg->TaskID);
	if(iter == CFixTaskTable::Instance().m_BaseTaskMap.end())
		return -1;
	int iFlag = 0;
	int TypeID = 0;		
	int ItemID = 0;
	int ItemNum = 0;
	int ItemLevel = 0;
	
	if (sscanf(iter->second.contest_,"%d:%d:%d:%d:%d",&iFlag,&TypeID,&ItemID,&ItemNum,&ItemLevel) != 5)
    {
		 return -1;
    } 
	if(ItemNum > req_msg->Money)
		return -1;

	OnCompleteTask(req_msg->agent,req_msg->PlayerID,req_msg->TaskID);
	return 0;
}
//定时器
int CTaskThread::onTimerMsg(const char *timermsg)
{
	TIMER_HEADER *pTimerHead = (TIMER_HEADER*)timermsg;
	switch(pTimerHead->timer_command )
	{
	case EN_TIMER_CLEARDAYTASK://清除日常任务
		{
			clearAllDayTask();
		}
		break;
	default:
		break;
	}
	return 0;
}
//清除日常任务表
void CTaskThread::clearAllDayTask()
{
	PlayerDayTask dayTaskTable;
	char * buf = 0;
	int count = 0;
	if (Proxy.db_select_all(dayTaskTable,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		return;
	}
	count = count / sizeof(PlayerDayTask);
	PlayerDayTask *pTaskList = (PlayerDayTask*)buf;
	__int64 curTime = IceUtil::Time::now().toSeconds();
	for (int i=0;i<count;++i)
	{
		if (pTaskList[i].endtime_ > curTime)
		{
			continue;
		}
		if (Proxy.db_delete(pTaskList[i]) != DB_Proxy::DB_SUCCEED)
		{
		}
	}
	if (buf)
	{
		delete[] buf;
		buf = 0;
	}
	//重新加载日常任务的目标郡
	m_NetTask->reloadDayTaskTargetMap(curTime);
	m_NetTask->startClearDayTaskTimer(86400);
}
//日常任务完成通知
int CTaskThread::onNotifyDayTaskFinished(const char *notifymsg)
{
	LPNotify_DayTaskOfPlayer_Response pNotifyMsg = (LPNotify_DayTaskOfPlayer_Response)notifymsg;
	int agent = m_NetTask->getAgentOfPlayer(pNotifyMsg->playerID);
	PlayerDayTask dayTask;
	dayTask.setmasterid(pNotifyMsg->playerID);
	dayTask.setdetailid(pNotifyMsg->taskID);
	if (Proxy.db_select(dayTask) != DB_Proxy::DB_SUCCEED)
	{
		return -1;
	}
	if (dayTask.mapid_ != pNotifyMsg->from)
	{
		return -2;
	}
	dayTask.state_ = tsWc;
	if (Proxy.db_update(dayTask,dayTask) != DB_Proxy::DB_SUCCEED)
	{
		return -3;
	}
	//发送状态给玩家
	if (agent >0)
	{
		REP_TASKSTATE_COMPLETE rep;
		rep.serialize(agent,pNotifyMsg->playerID,pNotifyMsg->taskID);
		asynch_write(agent,(char *)&rep,sizeof(rep));
	}
	return 0;
}
