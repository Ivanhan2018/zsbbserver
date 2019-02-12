#ifndef __NetTask_H__
#define __NetTask_H__
#include "IceUtil.h"
#include "gmGlobalHead.h"
#include "gmServerIniFile.h"

//��ڶ���
class CNetTask: public IceUtil::Task<char*>
{
private:
	CNetTask();
public:
	~CNetTask(){}
	
	//��һʵ����
	static CNetTask& Instance();

	//��ʼ��
	bool InitTask();

	//����ʼ��
	bool UnInitTask();

	//��Ϣ��ThreadPool(Task)
	bool PutData(const char * ptr,unsigned int len);

	bool startTask(size_t n_threads = 1);
	void stopTask();
	void run();
private:
	bool m_bAlive;
};


#endif
