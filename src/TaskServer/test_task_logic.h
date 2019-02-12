#ifndef TEST_TASK_LOGIC_H
#define TEST_TASK_LOGIC_H

#include "ProTaskSystem.h"


/*玩家任务完成数*/
extern int RspGetTaskCount(const char * buff,int len);

//玩家任务列表
extern int RspGetTaskList(const char * buff,int len);
//任务状态更改结果
extern int RspSetTaskState(REP_SET_TASK_STATE* lpRsp);
//发布任务
extern int RspIssueUserTask(ISSUE_USER_TASK* lpRsp);
//任务完成状态
extern int RspTaskStateComplete(REP_TASKSTATE_COMPLETE* lpRsp);

#endif