#include "test_task_logic.h"
#include <iostream>
#include <assert.h>
#include <windows.h>

//#include <IceUtil.h>
//������������
int RspGetTaskCount(const char * buf,int len)
{
	//REP_TASK_COUNT * taskcount = (REP_TASK_COUNT*)buf;
	//printf("taskcount->count = %d \n",taskcount->count);
	return 0;
}

//��������б�
int RspGetTaskList(const char * buff, int len)
{
	//REP_GET_TASK_LIST * pRepGetTaskList = (REP_GET_TASK_LIST*)buff;
	

	//PLAYER_TASK_INFO * pPlayerTaskInfo;
	//printf("receive task list   TaskCount=%d...\n",pRepGetTaskList->TaskCount);

	//for (unsigned int i =0 ;i < pRepGetTaskList->TaskCount;i++)
	//{
	//	printf("TaskID:%d  TaskState: %d , TaskFrom: %d , TaskTime: %d \n",
	//		pPlayerTaskInfo->TaskID,pPlayerTaskInfo->OtherInfo.taskstate,pPlayerTaskInfo->OtherInfo.taskfrom \
	//		,pPlayerTaskInfo->OtherInfo.time);
	//	//IceUtil::Time::microSeconds(1321323232).toDateTime().c_str();
	//}

	return 0;
}

//����״̬���Ľ��
int RspSetTaskState(REP_SET_TASK_STATE* lpRsp)
{
	//printf("receive set task sate result ...\n");
	//if (lpRsp->bSucceed)
	//	printf("taskid:%d, tasksate:%d �ɹ� \n",lpRsp->TaskID,lpRsp->tasksate);
	//else 
	//	printf("taskid:%d, tasksate:%d ʧ�� \n",lpRsp->TaskID,lpRsp->tasksate);
	return 0;
}

//��������
int RspIssueUserTask(ISSUE_USER_TASK* lpRsp)
{
	//PLAYER_TASK_INFO * pPlayerTaskInfo = &lpRsp->TaskInfo;
	//printf("\nTaskID: %d, TaskState: %d , TaskFrom: %d , TaskTime: %d \n",
	//	pPlayerTaskInfo->TaskID,pPlayerTaskInfo->OtherInfo.taskstate,pPlayerTaskInfo->OtherInfo.taskfrom,pPlayerTaskInfo->OtherInfo.time);
	return 0;
}

//�������״̬
int RspTaskStateComplete(REP_TASKSTATE_COMPLETE* lpRsp)
{
	//printf("receive task state complete taskid:%d ...\n",lpRsp->TaskID);
	return 0;
}


