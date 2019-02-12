#ifndef __NetTask_H__
#define __NetTask_H__
#include "IceUtil.h"
#include "ProTaskSystem.h"
#include "TaskThread.h"
#include "TaskIniFile.h"
#include "log.hpp"

#pragma warning(disable:4244)
enum {task_data_size=32768};
class ReciverThread;

class CNetTask: public IceUtil::Task<char *,task_data_size>
{
public:
	CNetTask():m_bAlive(false){};
	~CNetTask(){}
	//初始化任务服务
	bool InitTask();
	//反初始化任务服务
	bool UnInitTask();
	//网络数据进队
	bool PutData(const char* ptr,unsigned int len);
	CTaskIniFile mTaskIniFile;

	bool startTask(size_t n_threads = 1)
	{
		if (m_bAlive)  return false;
		m_bAlive = true;
		return IceUtil::Task<char *,task_data_size>::startTask(n_threads);
	}
	void stopTask()
	{
		m_bAlive=false;
		IceUtil::Task<char *,task_data_size>::stopTask();
	}
	bool startClearDayTaskTimer(int times);

	void setOnlinePlayer(int playerid,int agent);
	int getAgentOfPlayer(int playerid);
	void loadTaskEndTime(__int64 curtimes);			//加载日常任务的结束时间（每个任务的持续时间不同）
	void reloadDayTaskTargetMap(__int64 curtimes);//重新加载日常任务的目标郡
	void run()
	{
		DISTRIBUTED_HEADER * pNetRead;
		char *msg = 0;
		CTaskThread gTaskThread(this);
		gTaskThread.InitTaskThread(mTaskIniFile.DbgIp().c_str(),mTaskIniFile.DbgPort());

		while (m_bAlive)
		{
			if (getQueue(msg))
			{
				pNetRead = (DISTRIBUTED_HEADER*)msg;
				switch (pNetRead->command)
				{
				case	TC_GET_TASK_LIST:
					{
						gTaskThread.OnGetTaskListEx(pNetRead);
						break;
					}
				case	TC_GET_TASK_COUNT:
					{
						gTaskThread.OnGetTaskCount(pNetRead);
						break;
					}
				case	TC_SET_TASK_STATE:
					{
						gTaskThread.OnModiTaskState(pNetRead);
						break;
					}
				case	TC_USER_PLAYERLEVEL_PLAYERRANK_EVENT:
					{
						gTaskThread.OnPlayerLeveEvent(pNetRead);
						break;
					}
				case	TC_USER_TASKITEM_EVENT:
					{
						gTaskThread.OnTaskItemEvent(pNetRead);
						break;
					}
				case TC_USER_TASK_COMPLETE_EVENT:
					{			
						gTaskThread.OnTaskCompleteEvent(pNetRead);
						break;
					}
				case TC_ECTYPE_EVENT:
					{
						gTaskThread.OnEctypeEvent(pNetRead);
						break;
					}
				case TC_REQ_CHECK_TASK_OK:
					{
						gTaskThread.OnCheckTask(pNetRead);
						break;
					}
				case CGC_S_NOTIFY_QUERYMONEY://查询GO点
					{
						gTaskThread.OnNotify_QueryMoney(pNetRead);
					}
					break;
				case EN_C_TIMER_NOTIFY://定时器
					{
						gTaskThread.onTimerMsg(msg);
					}
					break;
				case EN_S_NOTIFY_DAYTASKOFPLAYER://日常任务完成通知
					{//日常任务完成通知
						gTaskThread.onNotifyDayTaskFinished(msg);
					}
					break;
				default:
					{
						printf("在取网络数据时,收到客户端的非法请求 Command: %d \n",pNetRead->command);
						break;
					}
				}
				
				if (msg)
				{
					delete[] msg;
					msg = 0;
				}
			}
		}
		gTaskThread.FInitTaskThread();
	}

private:
		int getNextTime(int hour,int min);
private:
	bool m_bAlive;
	std::map<int,int>		m_onlinePlayerMap;

};

extern CNetTask gNetTask;

#endif


