#ifndef RANKINGTHREAD_H__
#define RANKINGTHREAD_H__
#include "IceUtil.h"

#include "DB_Proxy.h"

class CConsortiaManage;

class CRankingThread:public IceUtil::Thread
{
public:
	CRankingThread(CConsortiaManage *pComManage = 0,int waittime = 900);
	~CRankingThread();
	void run();
	void Stop();
	void WaitFor();
	int StartRanking();
private:
	CConsortiaManage		*m_pConManage;
	bool					m_bRuning;
	//IceUtil::ThreadControl	m_pThreadContr;
	DB_Proxy				m_DBProxy;										//¡¨Ω”DBG
	IceUtil::Semaphore		m_Semaphore;
	int						m_iTime;				//√Î
};
#endif