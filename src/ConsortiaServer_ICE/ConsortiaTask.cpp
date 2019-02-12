#include "ConsortiaTask.h"
#include "ConsortiaManage.h"
#include "ProcMessage.h"

CConsortiaTask::CConsortiaTask()
{
	m_bRuning = false;
}
CConsortiaTask::CConsortiaTask(CConsortiaManage *pConManage):m_ConManage(pConManage)
{
	m_bRuning = false;
}
CConsortiaTask::~CConsortiaTask()
{
}

void CConsortiaTask::SetManageHande(CConsortiaManage *pConManage)
{
	m_ConManage = pConManage;
}
bool CConsortiaTask::startTask(std::size_t nThreads)
{
	if(!m_ConManage)
		return false;

	m_bRuning = true;
	return IceUtil::Task<char*,MAX_MSG_COUNT>::startTask(nThreads);
}

void CConsortiaTask::stopTask()
{
	m_bRuning = false;
	/*
	char *msg;
	while (size())
	{
		getQueue(msg);
		exit(msg);
	}
	*/
	IceUtil::Task<char*,MAX_MSG_COUNT>::stopTask();
}

void CConsortiaTask::run()
{
	char *msg = 0;
	CProcMessage m_ProcMsg(m_ConManage);

#ifdef PRINT_USETIME
	long start,end;
	int count = 0;
#endif
	while(m_bRuning)
	{
		if(getQueue(msg))
		{
#ifdef PRINT_USETIME
			start = GetTickCount();
#endif

#ifdef PRESS_TEST
			InterlockedIncrement(&m_ConManage->pro_count);
#endif

			m_ProcMsg.ProcMsg(msg);
			delete[] msg;

#ifdef PRINT_USETIME
			end = GetTickCount();
			if((end-start) >50)
			{
				DISTRIBUTED_HEADER *h = (DISTRIBUTED_HEADER*)msg;
				printf("Command %d use time:%d\n",h->command,end-start);
			}
#endif
		}
	}
    
}


void CConsortiaTask::exit(char *msg)
{
	delete[] msg;
}