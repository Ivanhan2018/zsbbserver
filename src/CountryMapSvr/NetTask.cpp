#include "NetTask.h"
#include "Distributed_Header.h"
#include <iostream>
#include "socketsvc.hpp"
#include "log.hpp"
//#include "LogicSystem.h"
#pragma warning(disable:4100)
#pragma warning(disable:4189)
#pragma warning(disable:4127)
#pragma warning(disable:4244)
int SOCKETSVC_API  event_notify_callback(int server_id, const char * buffer, int length)
{
	DISTRIBUTED_HEADER * header = (DISTRIBUTED_HEADER *)buffer;
	char * szbuf = new char[header->length];
	memcpy(szbuf,buffer , header->length);
	if (!g_NetTask.putQueue(szbuf))
	{
		LOG_ERROR(("接收到服务器%d的通知消息，压入队列失败：玩家%d,CMD:%d.\n",server_id,header->from,header->command));
		delete[] szbuf;
	}
	return 0;
}

int SOCKETSVC_API  data_receiver_callback(int agent_id, int player_id, char * buffer, int length)
{
	DISTRIBUTED_HEADER * header = (DISTRIBUTED_HEADER *)buffer;
	char * szbuf = new char[header->length];
	memcpy(szbuf,buffer , header->length);
	if (!g_NetTask.putQueue(szbuf))
	{
		LOG_ERROR(("接收到客户端消息，压入队列失败：agent:%d,玩家%d,CMD:%d.\n",agent_id,header->from,header->command));
		delete[] szbuf;
	}

	return 0;
}

int SOCKETSVC_API  timer_notify_callback(const void * param,const void * param_2, long timer_id)
{
	//cancel_timer(timer_id);
	if (!g_NetTask.putQueue((char*)param))
	{
		TIMER_HEADER * header = (TIMER_HEADER *)param;
		LOG_ERROR(("接收到定时器消息，压入队列失败：agent:%d,玩家%d,CMD:%d.\n",header->agent,header->from,header->command));
		delete[] (char*)param;
	}
	return 0;
}

void SOCKETSVC_API  socket_disconn_callback(bool player_all, int agent_id, int server_id, int player_id)
{
	if (player_all)
	{
		std::cout << "agent disconnect :" << agent_id << "  myid: " << server_id <<std::endl;
		//g_UserManager->EmptyUserInfo();
	}
	else
	{
		std::cout << "client disconnect :" << player_id << "  myid: " << server_id <<std::endl;
		REQ_EXIT_FIGHT_SYSTEM_STRUCT *pReq = new REQ_EXIT_FIGHT_SYSTEM_STRUCT;
		pReq->serialize(player_id,server_id,REQ_EXIT_FIGHT_SYSTEM_INFO);
		if (!g_NetTask.putQueue((char*)pReq))
		{
			delete[] (char*)pReq;
		}
		if(server_id == SERVER_BOUND_MAP_OTHER)
		{//
			EXIT_MAP_SERVER *pExitSvrMsg = new EXIT_MAP_SERVER;
			pExitSvrMsg->serialize(player_id,server_id);
			if (!g_NetTask.putQueue((char*)pExitSvrMsg))
			{
				delete[] (char*)pExitSvrMsg;
			}
		}
	}
}
void SOCKETSVC_API  logic_exception_callback(int code, int agent_id, int from_id, int to_id)
{
	std::cout << "logic exception: \nrequest from " << from_id << " to " << to_id 
		<< "\nagent_id " << agent_id << "\nmessage:" << get_error_message(code) << std::endl;
}

CNetTask g_NetTask;


CNetTask::CNetTask(void)
{

}

CNetTask::~CNetTask(void)
{
}
bool CNetTask::InitNetTask(int mapid)
{
	//载入地图资源
	cout<<"thread ID: %d"<<mapid<<endl;
	if (!init(g_IniFile.strAgentID.c_str(),g_IniFile.m_AgentPort ,g_IniFile.m_LogSvrIP.c_str() , g_IniFile.m_LogSvrPort , mapid,5,data_receiver_callback,event_notify_callback,timer_notify_callback,socket_disconn_callback,logic_exception_callback))
	{
		cout<<"connect agent fail"<<endl;
		return false;
	}
	return true ;
}

void CNetTask::UnInitNetTask()
{
	IceUtil::Task<char*>::stopTask();
	fini();
}
