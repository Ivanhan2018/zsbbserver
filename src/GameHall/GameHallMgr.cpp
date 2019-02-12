// @brief 简要描述: 游戏大厅管理类
// @author 作者: 黄 文 才

#include "GameHallMgr.h"

GameHallMgr::GameHallMgr(CLoadDBInfo *dbInfo, DB_Proxy *dbPtr, PlayerID2Info &playId2InfoPtr)
: dbInfo_(dbInfo), dbPtr_(dbPtr), ectypeHall_(dbInfo, dbPtr, playId2InfoPtr)
{

}

void GameHallMgr::run(void)
{
	char * buffer = NULL;
	
	while (getQueue(buffer))
	{
		parse(buffer);
		OutputDebugString("GameHallMgr::run while\n");
	}
}

void GameHallMgr::parse(char *const buf)
{
	ectypeHall_.parse(buf);
	delete [] buf;
}

void GameHallMgr::UnInitNetTask()
{
	IceUtil::Task<char*>::stopTask();
	fini();
}
