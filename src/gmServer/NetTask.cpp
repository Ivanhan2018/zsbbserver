#include "NetTask.h"
#include "socketsvc.hpp"
#include "gmThread.h"
#include "gmFixData.h"
#include "log.hpp"

/***************************************************************************
// int 服务器ID
// const char * 接收的数据
// unsigned int 接收数据的长度
// 返回0
***************************************************************************/
int SOCKETSVC_API  EVENT_NOTIFY_CALLBACK(int server_id, const char * buffer, int length)
{
	//DISTRIBUTED_HEADER * header = (DISTRIBUTED_HEADER *)buffer;
	CNetTask::Instance().PutData(buffer,length);
	return 0;
}

/***************************************************************************
// int 代理服务器ID
// int 玩家ID
// const char * 接收的数据
// unsigned int 接收数据的长度
// 返回0
***************************************************************************/
int SOCKETSVC_API  DATA_RECEIVER_CALLBACK(int agent_id, int player_id, char * buffer, 	int length)
{
	//DISTRIBUTED_HEADER * header = (DISTRIBUTED_HEADER *)buffer;
	CNetTask::Instance().PutData(buffer,length);
	return 0;
}

/***************************************************************************
// const void * 设置定时器时指定的回传参数
// 返回0
***************************************************************************/
int SOCKETSVC_API  TIMER_NOTIFY_CALLBACK(const void * param,const void * param2, long timer_id)
{
	//cancel_timer(timer_id);
	return 0;
}

/***************************************************************************
// int 玩家ID
// 返回0
***************************************************************************/
void SOCKETSVC_API  SOCKET_DISCONNECT_CALLBACK(bool player_all,	int agent_id,	int server_id,	int player_id)
{
	if (player_all)
	{
		CGmFixData::Instance().DeleteUCbyAgent(agent_id);
	}
	else
	{
		CGmFixData::Instance().DeleteUC(player_id);
	}
	return;
}


/***************************************************************************
// const void * 设置定时器时指定的回传参数
// 返回0
***************************************************************************/
void SOCKETSVC_API  LOGIC_EXCEPTION_CALLBACK(int code,int agent_id,	int from_id,int to_id)
{
	return;
}

//CNetTask
CNetTask::CNetTask():m_bAlive(false)
{

}
CNetTask& CNetTask::Instance()
{
	static CNetTask aa;
	return aa;

}

bool CNetTask::InitTask()
{
	CgmServerIniFile::Instance().ReadData("gmServer.ini");
	if (!init(CgmServerIniFile::Instance().SockIp().c_str(),CgmServerIniFile::Instance().SockPort(),CgmServerIniFile::Instance().LogSvrIP().c_str(),
		CgmServerIniFile::Instance().LogSvrPort(),MDM_GP_GAME_MANAGE_SERVER,5,\
		DATA_RECEIVER_CALLBACK,EVENT_NOTIFY_CALLBACK,TIMER_NOTIFY_CALLBACK,SOCKET_DISCONNECT_CALLBACK,LOGIC_EXCEPTION_CALLBACK))
	{
		printf("start socket failed ... \n");	
		return false;
	}
	else printf("start socket succeed ... \n");

	if ( !CGmFixData::Instance().LoadData( CgmServerIniFile::Instance().DbgIp().c_str(),CgmServerIniFile::Instance().DbgPort(),\
		CgmServerIniFile::Instance().Dsn().c_str(),CgmServerIniFile::Instance().User().c_str(),CgmServerIniFile::Instance().Pwd().c_str(),\
		CgmServerIniFile::Instance().WebIP().c_str(),CgmServerIniFile::Instance().WebPort(),CgmServerIniFile::Instance().TimeOut()))
	{
		printf("CGmFixData::LoadData failed ... \n");
		return false;
	}
	else printf("LoadData succeed ... \n");

	if (!startTask(5))
	{
		printf("start thread pool failed ...\n");
		return false;
	}
	else printf("start thread pool succeed ...\n");

	return true;
}

bool CNetTask::UnInitTask()
{
	stopTask();
	fini();
	CGmFixData::Instance().UnLoadData();
	return true;
}


bool CNetTask::PutData(const char * ptr,unsigned int len)
{
	char * msg= new char[len+1];
	memcpy(msg,ptr,len);

	if (!putQueue(msg))
	{
		delete[] msg;
		msg = 0;
		return false;
	}
	return true;
}

bool CNetTask::startTask(size_t n_threads)
{
	if (m_bAlive)  return false;
	m_bAlive = true;
	return IceUtil::Task<char *>::startTask(n_threads);;
}
void CNetTask::stopTask()
{
	m_bAlive=false;
	IceUtil::Task<char *>::stopTask();
}

void CNetTask::run()
{
	DISTRIBUTED_HEADER * pNetRead = 0;
	char *msg = 0;
	CGmThread gGmThread;
	gGmThread.InitGmThread(CgmServerIniFile::Instance().DbgIp().c_str(),CgmServerIniFile::Instance().DbgPort(),\
		CgmServerIniFile::Instance().Dsn().c_str(),CgmServerIniFile::Instance().User().c_str(),CgmServerIniFile::Instance().Pwd().c_str());
	while (m_bAlive)
	{
		if (getQueue(msg))
		{
			pNetRead = (DISTRIBUTED_HEADER*)msg;
			switch (pNetRead->command)
			{
			//登录GM
			case GC_LOGIN:
				{
					gGmThread.OnGmLogin(pNetRead);
					break;
				}
			//登出GM
			case GC_LOGOUT:
				{
					gGmThread.OnGmLogout(pNetRead);
					break;
				}
			//请求获取势力公告
			case GC_NOTICEFORCES:
				{
					gGmThread.OnGetNoticeForces(pNetRead);
					break;
				}
			//请求常规GM命令
			case GC_GMCOMMAND:
				{
					gGmThread.OnGmCommand(pNetRead);
					break;
				}
			//增加主英雄名事件
			case CMD_ADD_MAIN_HERO:
				{
					gGmThread.OnGmAddMainHero(pNetRead);
					break;
				}
			//禁言(保留)
			case GC_SPEAKDISABLED:
				{
					gGmThread.OnSpeakDisabled(pNetRead);
					break;
				}
			//禁登录(保留)
			case GC_LOGINDISABLED:
				{
					gGmThread.OnLoginDisabled(pNetRead);
					break;
				}
			//公会转过来的势力公告
			case CC_C_NOTIFY_MODIFYNOTICE:
				{
					gGmThread.ProcModiNoticeForcesEvent(pNetRead);
					break;
				}
				//添加NPC时，大地图返回信息
			case EN_S_CREATENPCARMY:
				{
					gGmThread.ProcNotifyCreateNPCArmy(pNetRead);
				}
				break;
			case EN_S_DELNPCARMY://删除NPC
				{
					gGmThread.ProcNotifyDelNPCArmy(pNetRead);
				}
				break;
			default:
				{
					printf("在取网络数据时,收到客户端%d的非法请求 Command: %d \n",pNetRead->from,pNetRead->command);
					break;
				}
			}
			if(msg)
			{
				delete[] msg;
				msg = 0;
			}
		}
	} 
	gGmThread.FInitGmThread();
}

