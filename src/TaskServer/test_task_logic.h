#ifndef TEST_TASK_LOGIC_H
#define TEST_TASK_LOGIC_H

#include "ProTaskSystem.h"


/*������������*/
extern int RspGetTaskCount(const char * buff,int len);

//��������б�
extern int RspGetTaskList(const char * buff,int len);
//����״̬���Ľ��
extern int RspSetTaskState(REP_SET_TASK_STATE* lpRsp);
//��������
extern int RspIssueUserTask(ISSUE_USER_TASK* lpRsp);
//�������״̬
extern int RspTaskStateComplete(REP_TASKSTATE_COMPLETE* lpRsp);

#endif