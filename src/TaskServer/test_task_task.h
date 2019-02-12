#pragma once
#include "ProTaskSystem.h"
#include "../ClientLib/ClientSocket.h"

class CTaskTask 
{
private:
	bool bconn;
	ClientSocket Client;
public:
	CTaskTask(void);
	~CTaskTask(void);

	void init(const char * addr, short port, DATA_RECEIVER handler, LOGIC_EXCEPTION exception);
	void finit();
	int SendData(void* buffer,int iDatasize);
		
	int SendGetTaskCount(GET_TASK_COUNT req);
	//��ȡ����б�
	int SendGetTaskList(GET_TASK_LIST Req);
	//��������������״̬
	int SendSetTaskState(SET_TASK_STATE Req);
	//�����������Ƿ����
	int SendChekTaskOK(REQ_CHECK_TASK_OK Req);
};
extern CTaskTask* gTaskTask;
