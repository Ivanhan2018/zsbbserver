#ifndef NETTASK_H__
#define NETTASK_H__
#include "IceUtil.h"
#include "DataServer.h"
#include "IniFile.h"
class CNetTask : public IceUtil::Task<char*>
{
public:
	CNetTask();
	~CNetTask(void);
	bool InitNetTask(int mapid);
	void UnInitNetTask();
public:
	void run()
	{
		char* szbuffer;
		while (getQueue(szbuffer))
		{
			g_DataServer.GetData(szbuffer);

		}
	}
};
extern CNetTask		g_NetTask;
#endif
