#pragma once
#include "list"
class FightManager;
struct CoolingInfo;
enum CardType;
class MsgCoolingManager
{
public:
	MsgCoolingManager(FightManager *fightManager);
public:
	~MsgCoolingManager(void);
public:
	//�ռ���Ƭ����ȴ��Ϣ
	void insertCoolingData(CardType cardType , unsigned int armyId , unsigned int coolingTime);
	//������ȴ��Ϣ���ͻ���
	void sendCoolingData(unsigned int playerId , int agent);
private:
	std::list<CoolingInfo> msgCoolingData_; //��Ƭ����ȴ����
	FightManager * fightManager_; //ս�����ָ��
private:
	MsgCoolingManager(void);
};
