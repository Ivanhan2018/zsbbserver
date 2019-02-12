#ifndef __TaskThread_H__
#define __TaskThread_H__
#include "ProTaskSystem.h"
#include "FixTaskTable.h"
#include "MapServer_Protocol.h"
#include "CommonFunction.h"
#include "Time_Protocol.h"
#include <map>
class CNetTask;

class CTaskThread
{
public:
	CTaskThread(CNetTask *netTask);
	~CTaskThread();

	bool InitTaskThread(const char * addr, short port);
	bool FInitTaskThread();

	/* 处理网络数据部分Function */

	//获取玩家任务完成个数
	int OnGetTaskCount(const DISTRIBUTED_HEADER * pNetRead);
	//获取任务列表
	int OnGetTaskList(const DISTRIBUTED_HEADER * pNetRead);
	int OnGetTaskListEx(const DISTRIBUTED_HEADER * pNetRead);
	//处理状态任务更改
	int OnModiTaskState(const DISTRIBUTED_HEADER * pNetRead);
	//玩家等级变更事件
	int OnPlayerLeveEvent(const DISTRIBUTED_HEADER * pNetRead);
	//任务道具事件
	int OnTaskItemEvent(const DISTRIBUTED_HEADER * pNetRead);
	//玩家任务完成事件
	int OnTaskCompleteEvent(const DISTRIBUTED_HEADER * pNetRead);
	//副本完成事件
	int OnEctypeEvent(const DISTRIBUTED_HEADER * pNetRead);
	//检测生产类用户是否完成	
	int OnCheckTask(const DISTRIBUTED_HEADER * pNetRead);

	int OnNotify_QueryMoney(const DISTRIBUTED_HEADER * pNetRead);

	//定时器
	int onTimerMsg(const char *timermsg);
	//日常任务完成通知
	int onNotifyDayTaskFinished(const char *notifymsg);


private:
	/* other-处理任务状态分解 */
	//发布日常任务
	void issueDayTask(int agent,int playerid, int countryID);
	void checkDayTask(int agent,int playerid);
	bool checkColonialDayTask(int playerid,int mapid,int number,int level);
	//提交任务
	int OnReferTask(const DISTRIBUTED_HEADER * pNetRead);
	//接受任务
	int OnAcceptTask(const DISTRIBUTED_HEADER * pNetRead);
	//删除任务
	int OnDeleteTask(const DISTRIBUTED_HEADER * pNetRead);

	//任务完成
	int OnCompleteTask(int handle, unsigned long playerid, unsigned int taskid);
	//发布任务
	int OnIssueSystemTask(int handle,unsigned long ulPlayerID,unsigned int TaskID,int tasktype);

	/* 输助Function */
	//查询任务状态
	int OnQryTask(unsigned long playerid,unsigned int taskid, int & ts);
	
	//检测任务是否完成
	int CheckTaskIsComplete(int agent,unsigned long playerid, unsigned int taskid,const char * pContest);

    //获取任务奖励
	void GetTaskHortation(unsigned long playerID ,unsigned int TaskID,const int tasktype);
	
	//查询数量(玩家编号, 地图编号, 道具编号, 查询标志)
	int  CheckItem(int agent,unsigned long PlayerID, int taskid,int TypeID, int ItemID, int ItemNum, int iFlag,int iLevel); 
	bool AllTaskComplete(int playerid);//检查玩家是否所有任务已完成

	int GetItemCount(int playerid,int officerid,int itemid,int pos);
	//清除日常任务表
	void clearAllDayTask();
private:
	//bool					bAlive;
	DB_Proxy				Proxy ;
	CNetTask				*m_NetTask;
};

#endif