
#include "EctypeManager.h"
#include ".\mutexguard.h"
EctypeManager::EctypeManager(void)
{
}

EctypeManager::~EctypeManager(void)
{
	map<int , CLoadMapInfo*>::iterator itBeg = mapCollection_.begin();
	map<int , CLoadMapInfo*>::iterator itEnd = mapCollection_.end();

	for (; itBeg!=itEnd; ++itBeg)
	{
		delete itBeg->second;
		itBeg->second = NULL;
	}
}
void EctypeManager::loadEctypeMap(){
	CLoadMapInfo* tmpMapData = NULL;
	int tmpMapIndex = 0;
	for (int i = 1  ; i <= 200 ; i ++){
		tmpMapIndex = 2000+i;
		tmpMapData = new CLoadMapInfo(tmpMapIndex);
		if (tmpMapData){
			tmpMapData->SetMapOwer(NPC_);
			tmpMapData->SetMapState(war);
			mapCollection_.insert(map<int,CLoadMapInfo*>::value_type(tmpMapIndex ,tmpMapData));
		}
	}
}
CLoadMapInfo * EctypeManager::getMapInfo(int mapID) const{
	map<int , CLoadMapInfo*>::const_iterator iter;
	iter = mapCollection_.find(mapID);
	if (iter != mapCollection_.end()){
		return iter->second ;
	}
	return NULL;
}
void EctypeManager::addActivePlayer(ActivityPlayerPtr activityPlayer){
	MutexGuard<boost::shared_mutex> guard(ectypeManagerMutex_);
	activityPlayers_.insert(map<unsigned int ,ActivityPlayerPtr
	>::value_type(activityPlayer->playerID_ , activityPlayer));
}
//删除活动玩家
void EctypeManager::delActivePlayer(unsigned int playerID){
	MutexGuard<boost::shared_mutex> guard(ectypeManagerMutex_);
	activityPlayers_.erase(playerID);
}
//获得活动玩家信息
ActivityPlayerPtr EctypeManager::getActivePlayer(unsigned int playerID){
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
void EctypeManager::delActivePlayersByRoomID(int roomID){
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