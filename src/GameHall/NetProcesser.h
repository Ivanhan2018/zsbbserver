#ifndef NETPROCESSER_H_
#define NETPROCESSER_H_

#include "GameHallMgr.h"
#include "globalmacrodef.h"


class NetProcesser
{
public:
	NetProcesser(CLoadDBInfo *dbInfo, DB_Proxy *dbPtr, PlayerID2Info &playId2InfoPtr, int curProcessID);
	~NetProcesser(void);

	// 初始化NetProcesser
	bool initialize();

	inline bool startTask()
	{
		return gameHallMgr_.startTask();
	}

	inline void UnInitNetTask()
	{
		return gameHallMgr_.UnInitNetTask();
	}

	// 解析客户端发送的命令
	void parse(char *const buf);

	//设置服务器ID
	inline void setProcessID(int curProcessID)
	{
		curProcessID_ = curProcessID;
	}

	inline void initFieldEctypeRoom()
	{
		return gameHallMgr_.initFieldEctypeRoom();
	}

private:
	NO_COPYABLE_AND_ASSIGN(NetProcesser);	
	
	CLoadDBInfo* dbInfo_;
	DB_Proxy * dbPtr_;	
	int curProcessID_;           //  当前服务器注册的进程ID

	GameHallMgr gameHallMgr_;
};

extern NetProcesser *netprocesser_;

#endif // NETPROCESSER_H_
