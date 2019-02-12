#pragma once
#include <map>
#include "MapServerProtocolHeader.h"
#include "IArmy.h"
struct TaskDetail{
	unsigned int playerID;
	int taskID; //����ID
	short kindType;//����
	short itemType;//Ŀ������
	int itemID;//Ŀ��ID
	short number;//����
	short itemLevel;//����
	__int64 endTime;//����ʱ��
};
class FightManager;



//���������
class TaskManager
{
public:
	TaskManager(FightManager*fightManager);
public:
	~TaskManager(void);
public:
	//��������ʱ�ص�
	void onArmyDie(IArmyPtr attackArmyPtr , IArmyPtr dieArmyPtr);
	//��������
	bool receiveTask(unsigned int playerId , tagNotify_DayTaskOfPlayer_Request * newTask);
	//�����ճ�����
	void tidyTask(void);   
	//�����ճ�����
	void loadDayTask(void); 
private:
	//�����������
	bool getTaskType(IArmyPtr armyPtr ,int&taskType ,int& itemType);
	//���ĳ����
	bool matchingTask(TaskDetail &tmpTask , IArmyPtr dieArmyPtr , int itemType);
private:
	 //�����¼
	std::map<std::pair<unsigned int ,int> ,std::map<int ,TaskDetail> > taskMap_;
	//�ϴ�����ʱ��
	time_t lastClearTime_; //
	//ս��������
	FightManager* fightManager_;//
};
