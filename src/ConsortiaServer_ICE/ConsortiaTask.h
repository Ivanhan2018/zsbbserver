#ifndef CONSORTIATASK_H__
#define CONSORTIATASK_H__
#include "IceUtil.h"

#define MAX_MSG_COUNT	65535
class CConsortiaManage;

class CConsortiaTask:public IceUtil::Task<char*,MAX_MSG_COUNT>
{
public:
	CConsortiaTask();
	CConsortiaTask(CConsortiaManage *pConManage);
	~CConsortiaTask();
	void SetManageHande(CConsortiaManage *pConManage);
	virtual bool startTask(std::size_t = 1);
	virtual void stopTask();
	virtual void run();
	virtual void exit(char *msg);

private:
	CConsortiaManage	*m_ConManage;
	bool				m_bRuning;
};

#endif