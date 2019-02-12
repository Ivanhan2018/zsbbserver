#include "NetTask.h"
#include "socketsvc.hpp"
#include "Time_Protocol.h"

CNetTask	gNetTask;
/***************************************************************************
// int 服务器ID
// const char * 接收的数据
// unsigned int 接收数据的长度
// 返回0
***************************************************************************/
int SOCKETSVC_API  EVENT_NOTIFY_CALLBACK(int server_id, const char * buffer, int length)
{
	//DISTRIBUTED_HEADER * header = (DISTRIBUTED_HEADER *)buffer;
	gNetTask.PutData(buffer,length);
	return 0;
}

/***************************************************************************
// int 代理服务器ID
// int 玩家ID
// const char * 接收的数据
// unsigned int 接收数据的长度
// 返回0
***************************************************************************/
int SOCKETSVC_API  DATA_RECEIVER_CALLBACK(int agent_id, int player_id, char * buffer, 	int length)
{
	//DISTRIBUTED_HEADER * header = (DISTRIBUTED_HEADER *)buffer;
	gNetTask.PutData(buffer,length);
	return 0;
}

/***************************************************************************
// const void * 设置定时器时指定的回传参数
// 返回0
***************************************************************************/
int SOCKETSVC_API  TIMER_NOTIFY_CALLBACK(const void * param,const void * param1, long timer_id)
{
	//cancel_timer(timer_id);
	gNetTask.putQueue((char*)param);
	return 0;
}

/***************************************************************************
// int 玩家ID
// 返回0
***************************************************************************/
void SOCKETSVC_API  SOCKET_DISCONNECT_CALLBACK(bool player_all,	int agent_id,	int server_id,	int player_id)
{
	gNetTask.setOnlinePlayer(player_id,0);
	return;
}


/***************************************************************************
// const void * 设置定时器时指定的回传参数
// 返回0
***************************************************************************/
void SOCKETSVC_API  LOGIC_EXCEPTION_CALLBACK(int code,int agent_id,	int from_id,	int to_id)
{
	return;
}

bool CNetTask::InitTask()
{
	mTaskIniFile.ReadData("TaskServer.ini");

	if (!init(mTaskIniFile.SockIp().c_str(),mTaskIniFile.SockPort(),mTaskIniFile.LogSvrIP().c_str(),mTaskIniFile.LogSvrPort(),MDM_GP_MAP_TASK,5,DATA_RECEIVER_CALLBACK,EVENT_NOTIFY_CALLBACK,
		                   TIMER_NOTIFY_CALLBACK,SOCKET_DISCONNECT_CALLBACK,LOGIC_EXCEPTION_CALLBACK))
	{
		printf("start socket failed ... \n");	
		return false;
	}
	else printf("start socket succeed ... \n");	
	
	if ( !CFixTaskTable::Instance().LoadData( this,mTaskIniFile.DbgIp().c_str(),mTaskIniFile.DbgPort() ) )
	{
		printf("load fixtasktable failed ... \n");
		return false;
	}
	else printf("load fixtasktable succeed ... \n");

	if (!startTask(10))
	{
		printf("start thread pool failed ...\n");
		return false;
	}
	else printf("start thread pool succeed ...\n");
	//启动日常任务定时器
	return startClearDayTaskTimer(0);
}
int CNetTask::getNextTime(int hour,int min)
{
	IceUtil::DateTime curDataTime;
	IceUtil::Time::now().toDateTime(curDataTime);
	int sec = 0;
	if (sec < curDataTime.sec)
	{
		sec = 60;
		--min;
	}
	if (min <curDataTime.min)
	{
		min += 60;
		--hour;
	}
	if (hour < curDataTime.hour)
	{
		hour += 24;
	}
	return (hour - curDataTime.hour) * 3600 + (min - curDataTime.min) * 60 + sec - curDataTime.sec;
}
bool CNetTask::startClearDayTaskTimer(int times)
{
	Timer_ClearDayTask *pClearDayTaskTimer = new Timer_ClearDayTask;
	pClearDayTaskTimer->serialize();
	if (times <=0)
	{
		times = getNextTime(mTaskIniFile.getClearDayTaskHour(),mTaskIniFile.getClearDayTaskMin());
	}
	if (times <=0)
	{
		times = 100;
	}
	if ( (pClearDayTaskTimer->TimerID = set_timer(times,pClearDayTaskTimer,0)) <=0)
	{
		printf("set timer falut.");
		return false;
	}
	return true;
}

void CNetTask::setOnlinePlayer(int playerid,int agent)
{
	m_onlinePlayerMap[playerid] = agent;
}
int CNetTask::getAgentOfPlayer(int playerid)
{
	std::map<int,int>::const_iterator iter;
	iter = m_onlinePlayerMap.find(playerid);
	if (iter == m_onlinePlayerMap.end())
	{
		return 0;
	}
	return iter->second;
}
//加载日常任务的结束时间（每个任务的持续时间不同）
void CNetTask::loadTaskEndTime(__int64 curtimes)
{
	std::map<int,DayTask_DetailInfo>::iterator iter;
	__int64 endTime = 0; 
	__int64 tmpEndTime = getNextTime(mTaskIniFile.getClearDayTaskHour(),mTaskIniFile.getClearDayTaskMin()) + curtimes;
	for (iter = CFixTaskTable::Instance().m_DayTaskMap.begin();iter != CFixTaskTable::Instance().m_DayTaskMap.end();iter++)
	{
		endTime = mTaskIniFile.getTaskEndtime(iter->first);
		if (endTime < curtimes)
		{
			iter->second.endTime = tmpEndTime;
			mTaskIniFile.writeTaskEndtime(iter->first,tmpEndTime);
		}
		else
		{
			iter->second.endTime = endTime;
		}
	}
}
//重新加载日常任务的目标郡
void CNetTask::reloadDayTaskTargetMap(__int64 curtimes)
{
	//重新加载目标郡
	std::map<int,DayTask_DetailInfo>::iterator iter;
	IceUtil::DateTime curDataTime;
	IceUtil::Time::now().toDateTime(curDataTime);
	char daybuf[16] = {0};
	sprintf(daybuf,"%d%.2d",curDataTime.mon,curDataTime.day);

	FILE *fp = 0;
	fp = fopen("configtable/TargetMapOfDayTask.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/TargetMapOfDayTask.csv失败。\n");
		return;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);
	char gameGroup[32] = {0};
	char day[8] = {0};
	int taskid = 0;
	int mapid1 = 0;
	int mapid2 = 0;

	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;

		if(sscanf(buf,"%[^,],%[^,],%d,%d,%d",&gameGroup,&day,&taskid,&mapid1,&mapid2)<5)
		{
			continue;
		}
		if (strcmp(gameGroup,mTaskIniFile.getGameGroupName().c_str()))
		{
			continue;
		}
		if (strcmp(daybuf,day))
		{
			continue;
		}
		iter = CFixTaskTable::Instance().m_DayTaskMap.find(taskid);
		if (iter == CFixTaskTable::Instance().m_DayTaskMap.end())
		{
			continue;
		}
		iter->second.maps[1] = mapid1;
		iter->second.maps[2] = mapid2;
	}
	fclose(fp);

	for (iter = CFixTaskTable::Instance().m_DayTaskMap.begin();iter != CFixTaskTable::Instance().m_DayTaskMap.end();iter++)
	{
		if (iter->second.endTime <= curtimes)
		{
			iter->second.endTime = curtimes + iter->second.days * 86400;
			mTaskIniFile.writeTaskEndtime(iter->first,iter->second.endTime );
		}
	}
}

bool CNetTask::UnInitTask()
{
	stopTask();
    fini();
	CFixTaskTable::Instance().UnLoadData();
	return true;
}


bool CNetTask::PutData( const char* ptr,unsigned int len)
{
	char * pNetRead= new char[len];
	memcpy(pNetRead,ptr,len);
	if (!putQueue(pNetRead))
	{
		delete[] pNetRead;
		pNetRead = 0;
		return false;
	}
	return true;
}

