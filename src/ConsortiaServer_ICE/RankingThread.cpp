#include "RankingThread.h"
#include "ConsortiaManage.h"
#include <stdio.h>

CRankingThread::CRankingThread(CConsortiaManage *pComManage,int waittime)
:m_pConManage(pComManage),m_bRuning(false),m_iTime(waittime)
{
	m_bRuning = false;
}
CRankingThread::~CRankingThread()
{
	//stop();
	m_DBProxy.fini();
}

void CRankingThread::run()
{
	IceUtil::Time waittime = IceUtil::Time::seconds(m_iTime);
	while(m_bRuning)
	{
		if(m_Semaphore.timedWait(waittime))
			continue;
		
		if(m_pConManage)
		{
			m_pConManage->Ranking(&m_DBProxy);		
		}
		/*
		printf("CRankingThread::run()\n");
		*/
	}
	m_bRuning = false;
}

void CRankingThread::Stop()
{
	if(!m_bRuning)
		return ;
	m_bRuning = false;
	m_Semaphore.post();
	//WaitFor();
}
void CRankingThread::WaitFor()
{
	//m_pThreadContr.join();
	getThreadControl().join();
}
int CRankingThread::StartRanking()
{
	/*
	*/
	m_DBProxy.fini();
	if(m_pConManage)
	{
		if(!m_DBProxy.init(m_pConManage->m_AgentDBParam.DBGIP,m_pConManage->m_AgentDBParam.DBGPort,MDM_GP_MAP_CONSORTIA))
			return 1;
	}
	m_bRuning = true;
	IceUtil::Thread::start(0);
	return 0;
}