#pragma once
#include "NPCInfo.h"
#include "IArmy.h"
#include <map>

struct FightOverRecord{
	IArmyPtr armyPtr;
	int      cmdType;
	unsigned int targetID;
};
typedef vector<FightOverRecord> FightOverRecordArray;
class FightManager;
struct FightOverCondition;
class FightOverManager
{
public:
	FightOverManager(int limitMaxBoutNum ,FightManager * fightManager);
public:
	~FightOverManager(void);
public:
	//ս���Ƿ����
	bool isFightOver(int& victoryCountryID);
	//���ʤ�����
	Armys * getWinner(void);
	//��������
	void onArmyDie(IArmyPtr attacker , IArmyPtr dieArmy);
	//��Ʒ����
	void onGoodsDropped(IArmyPtr attacker , int droppedItem);
	//ˢ����Դ
	void onUpdateRes(int countryId,int resType , int resNum);
	//����ս����������
	void setFightOverCondition(FightOverConditionArray & fightOverConditionArray); 
protected:
	FightManager *fightManager_;
	FightOverConditionArray fightOverConditionArray_; //��������
	bool isFightOver_; //ս���Ƿ����
	int	 limitMaxBoutNum_; //���غ�����
	bool isDefaultMode_; //�Ƿ�Ĭ��ģʽ 
	int winCountryID_; //ʤ��������ӪID
};
