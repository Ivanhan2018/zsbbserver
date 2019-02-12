// @brief 简要描述: 游戏大厅管理类
// @author 作者: 黄 文 才

#ifndef GAME_HALL_MANAGER_H
#define GAME_HALL_MANAGER_H

#include "IceUtil.h"
#include "EctypeHall.h"

class GameHallMgr : public IceUtil::Task<char *> 
{
public:
	GameHallMgr(CLoadDBInfo *dbInfo, DB_Proxy *dbPtr, PlayerID2Info &playId2InfoPtr);

	// run
	void run(void);

	inline void initFieldEctypeRoom()
	{
		return ectypeHall_.initFieldEctypeRoom();
	}

	void UnInitNetTask();

	void parse(char *const buf);

private:
	NO_COPYABLE_AND_ASSIGN(GameHallMgr);

	CLoadDBInfo * dbInfo_;
	DB_Proxy * dbPtr_;	

	EctypeHall ectypeHall_;     // 副本大厅
};

#endif
