
#include "NetProcesser.h"
#include "socketsvc.hpp"
//#include "log.hpp"
#include "IniFile.h"

#define  ZSBB_ECHO(x) printf x;

int SOCKETSVC_API  event_notify_callback(int server_id, const char * buffer, int length)
{
	char * szbuf = new char[length];
	
	if (NULL == szbuf || NULL == netprocesser_)
	{
		return -1;
	} 

	memcpy(szbuf, buffer , length);

	netprocesser_->parse(szbuf);

	ZSBB_ECHO(("event_notify_callback %d \n" , server_id));
	return 0;
}

int SOCKETSVC_API  data_receiver_callback(int agent_id, int /*player_id*/, char * buffer, int length)
{
	char * szbuf = new char[length];

	if (szbuf == NULL || NULL == netprocesser_)
	{
		return -1;
	}
	memcpy(szbuf,buffer , length);

	netprocesser_->parse(szbuf);

	ZSBB_ECHO(("data_receiver_callback from %d \n" , agent_id));

	return 0;
}

int SOCKETSVC_API  timer_notify_callback(const void * param,const void * /*param_2*/ , long timerID)
{
	if (NULL == netprocesser_)
	{
		return -1;
	}

	netprocesser_->parse((char*const)param);
	ZSBB_ECHO(("timer_notify_callback\n"));
	return 0;
}

void SOCKETSVC_API  socket_disconn_callback(bool /*player_all*/, int /*agent_id*/, int /*server_id*/, int /*player_id*/)
{
	// 需要添加客户端断开消息
}

void SOCKETSVC_API  logic_exception_callback(int /*code*/, int /*agent_id*/, int /*from_id*/, int /*to_id*/)
{
// 	std::cout << "logic exception: \nrequest from " << from_id << " to " << to_id 
// 		<< "\nagent_id " << agent_id << "\nmessage:" << get_error_message(code) << std::endl;

}

NetProcesser *netprocesser_ = NULL;

NetProcesser::NetProcesser(CLoadDBInfo *dbInfo, DB_Proxy *dbPtr, PlayerID2Info &playId2InfoPtr, int curProcessID)
: dbInfo_(dbInfo), dbPtr_(dbPtr), curProcessID_(curProcessID), gameHallMgr_(dbInfo, dbPtr, playId2InfoPtr)
{
	initialize();
}

NetProcesser::~NetProcesser(void)
{
}

void NetProcesser::parse(char *const buf)
{
	if (NULL == buf)
	{
		return;
	}

	gameHallMgr_.putQueue(buf);
}

bool NetProcesser::initialize()
{
	//注册服务器
	if (!init(g_IniFile.strAgentID.c_str(), static_cast<unsigned short>(g_IniFile.m_AgentPort) , g_IniFile.m_LogSvrIP.c_str() , 
		static_cast<unsigned short>(g_IniFile.m_LogSvrPort), curProcessID_, 2, data_receiver_callback, event_notify_callback,
		timer_notify_callback, socket_disconn_callback, logic_exception_callback))
	{
		cout << "connect agent fail" << endl;
		return false;
	}

	return true;
}

