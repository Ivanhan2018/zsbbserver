
#include "NetTask.h"
#include "Global_Macro.h"
#include "Distributed_Header.h"
CNetTask g_NetTask ;
ReciverThread g_ThreadPool;
#include "IniFile.h"
int SOCKETSVC_API  event_notify_callback(int server_id, const char * buffer,  int length)
{
	std::cout << "event_notify_callback server id:" << server_id <<std::endl;

	DISTRIBUTED_HEADER * header = (DISTRIBUTED_HEADER *)buffer;
	if (header->length>1024*5 && header->length<0)
	{
		cout<<header->command<<endl;
		return 0;
	}
	cout<<header->command<<endl;
	char * szbuf = new char[header->length];
	memcpy(szbuf,buffer , header->length);
	if (!g_ThreadPool.putQueue(szbuf))
	{
		printf("error");
	}
	return 0;
}

int SOCKETSVC_API  data_receiver_callback(int agent_id, int player_id, char * buffer,  int length)
{
	std::cout << "data_receiver_callback agent_id id:" << agent_id << " player id:" << player_id <<std::endl;
	DISTRIBUTED_HEADER * header = (DISTRIBUTED_HEADER *)buffer;
	char* buf = new char [length];
	memcpy(buf,buffer,length);
	if (g_ThreadPool.putQueue(buf))
	{
		return 0 ;
	}
	//delete []buf;
	return 0;
}

int SOCKETSVC_API  timer_notify_callback(const void * param,const void * param2, long /*timerID*/)
{
	//std::cout << "timer_notify_callback: " << (int)param <<std::endl;
	return 0;
}

void SOCKETSVC_API  socket_disconn_callback(bool player_all, int agent_id, int server_id, int player_id)
{
	if (player_all)
	{
		std::cout << "agent disconnect :" << agent_id << "  myid: " << server_id <<std::endl;
	}
	else
	{
		std::cout << "client disconnect :" << player_id << "  myid: " << server_id <<std::endl;
	}
}

void SOCKETSVC_API  logic_exception_callback(int code, int agent_id, int from_id, int to_id)
{
	std::cout << "logic exception: \nrequest from " << from_id << " to " << to_id 
		<< "\nagent_id " << agent_id << "\nmessage:" << get_error_message(code) << std::endl;
}
int __stdcall DataRecive(int handle, char *buffer, unsigned long Datasize);
int __stdcall timer_notify(const void * param)
{
	printf("param : %d\n", *(int*)param);
	return TRUE;
}

int __stdcall event_notify(int server_id, const char * buffer,  int length)
{
	printf("event_notify server id : %d, buffer length : %d\n", server_id, length);
	char* buf = new char [length];
	memcpy(buf,buffer,length);
	if (g_ThreadPool.putQueue(buf))
	{
		return 0 ;
	}
	return TRUE;
}

int __stdcall data_receiver(int agent_id, int player_id, char * buffer,  int length )
{
	printf("data_receiver agent id:%d, player id:%d, buffer length:%d\n", agent_id, player_id, length);

	// 将数据返发回去
	DISTRIBUTED_HEADER * header = (DISTRIBUTED_HEADER *)buffer;
	char* buf = new char [length];
	memcpy(buf,buffer,length);
	if (g_ThreadPool.putQueue(buf))
	{
		return 0 ;
	}
//	if (asynch_write(agent_id, buffer, length) == FALSE)
	return 0;
}
CNetTask::CNetTask(void)
{
}

CNetTask::~CNetTask(void)
{
}
bool CNetTask::InitTask()
{
	g_IniFile.ReadFile("serverconfig.ini");
	if (!init(g_IniFile.strAgentID.c_str(),g_IniFile.m_AgentPort,g_IniFile.m_LogSvrIP.c_str(), g_IniFile.m_LogSvrPort , MDM_GP_MAP_CHAT,5,data_receiver_callback,event_notify_callback,timer_notify_callback,socket_disconn_callback,logic_exception_callback))
	{
		return false;
	}
	printf("\n启动了聊天服务器%d\n",MDM_GP_MAP_CHAT);
	//CmdPool.InitRecvCmdPool();
	g_ThreadPool.startTask(1);
	if(!proxy.init(g_IniFile.strDBIP.c_str(),g_IniFile.m_DBPort/*"192.168.1.18" ,8899*/, MDM_GP_MAP_CHAT))
		return false;

	return true ;
}

bool CNetTask::UnInitTask()
{
	fini();

	//CmdPool.UnInitRecvCmdPool();
	proxy.fini();
	g_ThreadPool.stopTask();
	return true;
}



