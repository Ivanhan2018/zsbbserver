#ifndef __NetTask_H__
#define __NetTask_H__
#include "IceUtil.h"
#include "gmGlobalHead.h"
#include "gmServerIniFile.h"

//入口对象
class CNetTask: public IceUtil::Task<char*>
{
private:
	CNetTask();
public:
	~CNetTask(){}
	
	//单一实例化
	static CNetTask& Instance();

	//初始化
	bool InitTask();

	//反初始化
	bool UnInitTask();

	//消息进ThreadPool(Task)
	bool PutData(const char * ptr,unsigned int len);

	bool startTask(size_t n_threads = 1);
	void stopTask();
	void run();
private:
	bool m_bAlive;
};


#endif
