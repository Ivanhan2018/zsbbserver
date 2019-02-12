#include "test_task_task.h"
#include <crtdbg.h>

CTaskTask* gTaskTask = 0 ;

CTaskTask::CTaskTask(void)
:bconn(false)
{
}

CTaskTask::~CTaskTask(void)
{
}

void CTaskTask::init(const char * addr, short port, DATA_RECEIVER handler, LOGIC_EXCEPTION exception)
{
	bconn = Client.Connect(addr,port,handler,exception);
}


void CTaskTask::finit()
{
	Client.Disconnect();
	bconn=false;
}

int CTaskTask::SendData(void* buffer,int iDatasize)
{
	return Client.Send((char*)buffer,iDatasize);
}

int CTaskTask::SendGetTaskCount(GET_TASK_COUNT req)
{
	req.command = TC_GET_TASK_COUNT;
	return(SendData(&req,sizeof(GET_TASK_COUNT)));
}


//��ȡ����б�
int CTaskTask::SendGetTaskList(GET_TASK_LIST Req)
{
	Req.command = TC_GET_TASK_LIST;
	return(SendData( &Req ,sizeof(GET_TASK_LIST)));
}

//��������������״̬
int CTaskTask::SendSetTaskState(SET_TASK_STATE Req)
{
	Req.command = TC_SET_TASK_STATE;
	return(SendData( &Req ,sizeof(SET_TASK_STATE)));
}

//�����������Ƿ����
int CTaskTask::SendChekTaskOK(REQ_CHECK_TASK_OK Req)
{
	Req.command = TC_REQ_CHECK_TASK_OK;
	return(SendData( &Req ,sizeof(REQ_CHECK_TASK_OK)));
}

