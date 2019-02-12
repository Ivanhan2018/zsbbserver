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
class FightManager;
class PlayerManager;
class IniFile;
class PlayerManager
{
public:
	PlayerManager(IniFile * iniFile);
	~PlayerManager(void);
public:
	//添加活动玩家
	ActivityPlayerPtr addActivePlayer(ActivityPlayerPtr activityPlayer);
	//删除活动玩家
	void delActivePlayer(unsigned int playerID);
	//删除活动玩家BY房间ID
	void delActivePlayersByRoomID(int roomID);
	//获得活动玩家信息
	ActivityPlayerPtr getActivePlayer(unsigned int playerID);
	//获得当前郡的玩家数量
	inline int getPlayerNum(void) {
		return playerNum_;
	}
	//获得玩家列表
	const map<unsigned int , ActivityPlayerPtr> & getPlayerMap(void) const{
		return activityPlayers_;
	}
	//获得玩家的阵营ID

protected:
	//副本地图集合
	map<int , CLoadMapInfo*> mapCollection_;
	//当前地图的活动玩家 
	map<unsigned int , ActivityPlayerPtr> activityPlayers_; 
	//副本用户管理锁
	boost::shared_mutex ectypeManagerMutex_;
	//玩家数量
	int playerNum_; 
	//配置文件
	IniFile * iniFile_;
	//
private:
	NO_COPYABLE_AND_ASSIGN(PlayerManager);
};
