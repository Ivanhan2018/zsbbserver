#pragma once
#include <map>
#include "MapServerProtocolHeader.h"
#include "IArmy.h"
struct TaskDetail{
	unsigned int playerID;
	int taskID; //任务ID
	short kindType;//种类
	short itemType;//目标类型
	int itemID;//目标ID
	short number;//数量
	short itemLevel;//级别
	__int64 endTime;//结束时间
};
class FightManager;



//任务管理器
class TaskManager
{
public:
	TaskManager(FightManager*fightManager);
public:
	~TaskManager(void);
public:
	//部队死亡时回调
	void onArmyDie(IArmyPtr attackArmyPtr , IArmyPtr dieArmyPtr);
	//接收任务
	bool receiveTask(unsigned int playerId , tagNotify_DayTaskOfPlayer_Request * newTask);
	//整理日常任务
	void tidyTask(void);   
	//加载日常任务
	void loadDayTask(void); 
private:
	//获得任务类型
	bool getTaskType(IArmyPtr armyPtr ,int&taskType ,int& itemType);
	//清除某任务
	bool matchingTask(TaskDetail &tmpTask , IArmyPtr dieArmyPtr , int itemType);
private:
	 //任务记录
	std::map<std::pair<unsigned int ,int> ,std::map<int ,TaskDetail> > taskMap_;
	//上次清理时间
	time_t lastClearTime_; //
	//战斗管理器
	FightManager* fightManager_;//
};
