#pragma once
#include <map>
#include <boost/shared_ptr.hpp>
#pragma warning(push)
#pragma warning(disable:4244)
#include <boost/thread/shared_mutex.hpp>
#pragma warning(pop)
#include "LoadMapInfo.h"
#include "globalmacrodef.h"
#include "FightSystemHeader.h"
class EctypeManager;
class EctypeManager
{
public:
	EctypeManager(void);
	~EctypeManager(void);

public:
	//加载副本地图
	void loadEctypeMap();
	//获得特定地图信息
	CLoadMapInfo * getMapInfo(int mapID) const;
	//添加活动玩家
	void addActivePlayer(ActivityPlayerPtr activityPlayer);
	//删除活动玩家
	void delActivePlayer(unsigned int playerID);
	//删除活动玩家BY房间ID
	void delActivePlayersByRoomID(int roomID);
	//获得活动玩家信息
	ActivityPlayerPtr getActivePlayer(unsigned int playerID);
protected:
	//副本地图集合
	map<int , CLoadMapInfo*> mapCollection_;
	//当前地图的活动玩家 
	map<unsigned int , ActivityPlayerPtr> activityPlayers_; 
	//副本用户管理锁
	boost::shared_mutex ectypeManagerMutex_;
private:
	NO_COPYABLE_AND_ASSIGN(EctypeManager);
};
