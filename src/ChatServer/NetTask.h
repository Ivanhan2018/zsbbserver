#pragma once
#include "IceUtil.h"
//#include "scktsvr.h"
#include "socketsvc.hpp"
#include "ChatDataServer.h"
#include "DB_Proxy.h"

#include "CommandPool.h"
#include "IniFile.h"

class ReciverThread : public IceUtil::Task<char*>
{
public:
	void run()
	{
		CChatSystemLogic Logic;
		if(!Logic.proxy.init(g_IniFile.strDBIP.c_str(),g_IniFile.m_DBPort, MDM_GP_MAP_CHAT))
		{
			return;
		}
		
		//Logic.testNewsReport();
		char* CenterRead;
		while (getQueue(CenterRead))
		{
			g_GetChatData.GetData(CenterRead ,Logic);
		}
		Logic.proxy.fini();
	}
};
extern ReciverThread g_ThreadPool;

class CNetTask
{
public:
	CNetTask(void);
	~CNetTask(void);
	bool InitTask();
	bool UnInitTask();

	bool GetReciverData(int handle ,void *buffer ,int size);

	bool PutData(int handle, int iCommand, void * ptr, int len);

	DB_Proxy  proxy;
	ReciverThread ThreadPool;

	//CCommandPool CmdPool ;

//	IceUtil::MemoryPool<NTY_CenterRead, IceUtil::Mutex> m_Pool;

};
//extern ReciverThread g_ReciverThread ;
extern CNetTask g_NetTask ;