
#include "PlayerManager.h"
#include ".\mutexguard.h"
#include "IniFile.h"
PlayerManager::PlayerManager(IniFile * iniFile):
playerNum_(0),
iniFile_(iniFile)
{
}

PlayerManager::~PlayerManager(void)
{
}
//添加玩家
ActivityPlayerPtr PlayerManager::addActivePlayer(ActivityPlayerPtr activityPlayer){
	MutexGuard<boost::shared_mutex> guard(ectypeManagerMutex_);
	activityPlayers_.insert(map<unsigned int ,ActivityPlayerPtr
	>::value_type(activityPlayer->playerID_ , activityPlayer));
	if (playerNum_ >= iniFile_->m_MaxEnterPlayerNum){
		return ActivityPlayerPtr();
	}
	playerNum_ ++;
	return activityPlayer;
}
//删除活动玩家
void PlayerManager::delActivePlayer(unsigned int playerID){
	MutexGuard<boost::shared_mutex> guard(ectypeManagerMutex_);
	if(activityPlayers_.erase(playerID) > 0){
		playerNum_ --;
	}
}
//获得活动玩家信息
ActivityPlayerPtr PlayerManager::getActivePlayer(unsigned int playerID){
	map<unsigned int ,ActivityPlayerPtr>::const_iterator iter;
	ActivityPlayerPtr actPlayer;
	ShareMutexGuard<boost::shared_mutex> guard(ectypeManagerMutex_);
	iter = activityPlayers_.find(playerID);
	if (iter != activityPlayers_.end()){
		actPlayer = iter->second;
		return actPlayer;
	}
	return ActivityPlayerPtr();
}
//删除特定roomID的玩家
void PlayerManager::delActivePlayersByRoomID(int roomID){
	map<unsigned int , ActivityPlayerPtr>::iterator iter;

	MutexGuard<boost::shared_mutex>  guard(ectypeManagerMutex_);
	for(iter = activityPlayers_.begin() ; iter != activityPlayers_.end();){
		if (iter->second->fightRoomID_ == roomID){
			iter->second->isStayRoom = false;
			iter = activityPlayers_.erase(iter);
		}
		else{
			iter ++;
		}
	}
}