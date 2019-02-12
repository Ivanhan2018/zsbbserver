#include "NetTask.h"
#include "socketsvc.hpp"
#include "gmThread.h"
#include "gmFixData.h"
#include "log.hpp"

/***************************************************************************
// int ������ID
// const char * ���յ�����
// unsigned int �������ݵĳ���
// ����0
***************************************************************************/
int SOCKETSVC_API  EVENT_NOTIFY_CALLBACK(int server_id, const char * buffer, int length)
{
	//DISTRIBUTED_HEADER * header = (DISTRIBUTED_HEADER *)buffer;
	CNetTask::Instance().PutData(buffer,length);
	return 0;
}

/***************************************************************************
// int ���������ID
// int ���ID
// const char * ���յ�����
// unsigned int �������ݵĳ���
// ����0
***************************************************************************/
int SOCKETSVC_API  DATA_RECEIVER_CALLBACK(int agent_id, int player_id, char * buffer, 	int length)
{
	//DISTRIBUTED_HEADER * header = (DISTRIBUTED_HEADER *)buffer;
	CNetTask::Instance().PutData(buffer,length);
	return 0;
}

/***************************************************************************
// const void * ���ö�ʱ��ʱָ���Ļش�����
// ����0
***************************************************************************/
int SOCKETSVC_API  TIMER_NOTIFY_CALLBACK(const void * param,const void * param2, long timer_id)
{
	//cancel_timer(timer_id);
	return 0;
}

/***************************************************************************
// int ���ID
// ����0
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
// const void * ���ö�ʱ��ʱָ���Ļش�����
// ����0
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
			//��¼GM
			case GC_LOGIN:
				{
					gGmThread.OnGmLogin(pNetRead);
					break;
				}
			//�ǳ�GM
			case GC_LOGOUT:
				{
					gGmThread.OnGmLogout(pNetRead);
					break;
				}
			//�����ȡ��������
			case GC_NOTICEFORCES:
				{
					gGmThread.OnGetNoticeForces(pNetRead);
					break;
				}
			//���󳣹�GM����
			case GC_GMCOMMAND:
				{
					gGmThread.OnGmCommand(pNetRead);
					break;
				}
			//������Ӣ�����¼�
			case CMD_ADD_MAIN_HERO:
				{
					gGmThread.OnGmAddMainHero(pNetRead);
					break;
				}
			//����(����)
			case GC_SPEAKDISABLED:
				{
					gGmThread.OnSpeakDisabled(pNetRead);
					break;
				}
			//����¼(����)
			case GC_LOGINDISABLED:
				{
					gGmThread.OnLoginDisabled(pNetRead);
					break;
				}
			//����ת��������������
			case CC_C_NOTIFY_MODIFYNOTICE:
				{
					gGmThread.ProcModiNoticeForcesEvent(pNetRead);
					break;
				}
				//���NPCʱ�����ͼ������Ϣ
			case EN_S_CREATENPCARMY:
				{
					gGmThread.ProcNotifyCreateNPCArmy(pNetRead);
				}
				break;
			case EN_S_DELNPCARMY://ɾ��NPC
				{
					gGmThread.ProcNotifyDelNPCArmy(pNetRead);
				}
				break;
			default:
				{
					printf("��ȡ��������ʱ,�յ��ͻ���%d�ķǷ����� Command: %d \n",pNetRead->from,pNetRead->command);
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

