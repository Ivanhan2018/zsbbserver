#include "TaskManager.h"
#include <time.h>
#include "FightManager.h"
#include "socketsvc.hpp"
TaskManager::TaskManager(FightManager*fightManager):
fightManager_(fightManager)
{
	lastClearTime_ = time(NULL);
}

TaskManager::~TaskManager(void)
{
}
bool TaskManager::matchingTask(TaskDetail &tmpTask, IArmyPtr dieArmyPtr , int itemType){
	if (tmpTask.itemType == itemType && (tmpTask.itemID == 0 || tmpTask.itemID == dieArmyPtr->GetArmyID())){
		tmpTask.number--;
		if (tmpTask.number <= 0){
			//发送完成消息到大地图
			tagNotify_DayTaskOfPlayer_Response taskResponse;
			taskResponse.serialize(tmpTask.playerID,tmpTask.taskID, fightManager_->getProcessID(),MDM_GP_MAP_TASK);
			event_notify(MDM_GP_MAP_TASK,(char *)&taskResponse,taskResponse.length);
			return true;
		}
	}
	return false;
}
void TaskManager::onArmyDie(IArmyPtr attackArmyPtr , IArmyPtr dieArmyPtr){
	if (attackArmyPtr == NULL || dieArmyPtr == NULL){
		return ;
	}
	if(attackArmyPtr->isPlayerArmy() == false){
		return ;
	}
	int taskType;
	int itemType;
	if(getTaskType(dieArmyPtr , taskType ,itemType) == false){
		return ;
	}
	std::map<std::pair<unsigned int ,int> ,std::map<int ,TaskDetail> >::iterator iter =  
		taskMap_.find(std::pair<unsigned int,int>(attackArmyPtr->getPlayerID(),taskType));
	if (iter == taskMap_.end()){
		return ;
	}
	std::map<int ,TaskDetail>& tmpMap = iter->second;
	std::map<int ,TaskDetail>::iterator subIter;
	for(subIter = tmpMap.begin() ; subIter != tmpMap.end(); ++subIter){
		//
		if (matchingTask(subIter->second, dieArmyPtr , itemType) == true)
		{
			iter->second.erase(subIter);
			return;
		}
		
	}
}
bool TaskManager::receiveTask(unsigned int playerId , tagNotify_DayTaskOfPlayer_Request * newTask){
	if (newTask == NULL){
		return false;
	}
	TaskDetail& tmpDetail = taskMap_[std::pair<unsigned int,int>(playerId,newTask->kindType)][newTask->taskID];
	tmpDetail.endTime = newTask->endTime;
	tmpDetail.itemID = newTask->itemID;
	tmpDetail.itemLevel = newTask->itemLevel;
	tmpDetail.itemType = newTask->itemType;
	tmpDetail.kindType = newTask->kindType;
	tmpDetail.number = newTask->number;
	tmpDetail.playerID = newTask->playerID;
	tmpDetail.taskID = newTask->taskID;
	return true;
}
bool TaskManager::getTaskType(IArmyPtr armyPtr ,int&taskType ,int& itemType){
	if (armyPtr == NULL)
	{
		return false;
	}
	if(armyPtr->getBuildDetailType() != ARCH_INVALID){
		taskType = TASKTARGET_ELIMICONSTRUCTION;
		itemType = armyPtr->getBuildDetailType();
	}
	else
	{
		taskType = TASKTARGET_ELIMIMONSTER;
		itemType = armyPtr->getArmyKind();
	}
	return true;
}
void TaskManager::tidyTask(void){
	tm * tmpTm = localtime(&lastClearTime_);
	int day = tmpTm->tm_mday;
	time_t now = time(NULL);
	tmpTm = localtime(&now);
	if (!(day - tmpTm->tm_mday)){
		return;
	}
	int64 timeNow = IceUtil::Time::now().toSeconds();
	std::map<std::pair<unsigned int ,int> ,std::map<int ,TaskDetail> >::iterator iter;
	std::map<int ,TaskDetail>::iterator subIter;
	for (iter = taskMap_.begin() ; iter != taskMap_.end() ; ){
		std::map<int ,TaskDetail>& tmpMap = iter->second;
		for(subIter = tmpMap.begin(); subIter != tmpMap.end() ; ){
			TaskDetail & tmpTask = subIter->second;
			if (timeNow >= tmpTask.endTime)
			{
				subIter = tmpMap.erase(subIter);
			}
			else
			{
				++ subIter;
			}
		}
		if (tmpMap.size() <= 0)
		{
			iter = taskMap_.erase(iter);
		}
		else
		{
			++ iter;
		}
	}
}
void TaskManager::loadDayTask(void){
	DB_Proxy * curProxy = fightManager_->getCurDBGProxy();
	if (curProxy == NULL){
		return ;
	}
	PlayerDayTask tmpTask;
	PlayerDayTask * tmpTaskPtr = NULL;
	int taskNum = 0;
	if(curProxy->db_select_all(tmpTask ,(char *&)tmpTaskPtr , taskNum) != DB_Proxy::DB_SUCCEED){
		return;
	}
	taskNum /= sizeof(PlayerDayTask);
	int64 timeNow = IceUtil::Time::now().toSeconds();
	DayTask_DetailInfo * tmpTaskDetail;
	for (int i = 0 ; i < taskNum ; i++)
	{
		if(tmpTaskPtr[i].mapid_ != fightManager_->getProcessID() || timeNow >= tmpTaskPtr[i].endtime_){
			continue;
		}
		tmpTaskDetail = fightManager_->getDayTaskParam(tmpTaskPtr[i].taskid_);
		if(tmpTaskDetail == NULL){
			continue;
		}
		TaskDetail& tmpDetail = taskMap_[std::pair<unsigned int,int>(tmpTaskPtr[i].playerid_,tmpTaskDetail->kindType)][tmpTaskDetail->itemType];
		tmpDetail.endTime = tmpTaskPtr[i].endtime_;
		tmpDetail.itemID = tmpTaskDetail->itemID;
		tmpDetail.itemLevel = tmpTaskDetail->itemLevel;
		tmpDetail.itemType = tmpTaskDetail->itemType;
		tmpDetail.kindType = tmpTaskDetail->kindType;
		tmpDetail.number = tmpTaskDetail->number;
		tmpDetail.playerID = tmpTaskPtr[i].playerid_;
		tmpDetail.taskID = tmpTaskPtr[i].taskid_;
	}
	delete [] (char*) tmpTaskPtr;
}