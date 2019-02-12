#pragma once
#include "Global_Macro.h"
#include "IArmy.h"
class FightManager;
struct PropCardConfig;



class PropCard{
public:
	PropCard(FightManager * fightManager ,IArmyPtr armyPtr, PropCardConfig * propCardPtr);
public:
	~PropCard(void);
public:
	void useCard(void);
	void clearCard(void);
	bool isForceOff(void);
private:
	int startBout_; //��ʼ�غ�
	IArmyPtr armyPtr_;  //����ָ��
	PropCardConfig * propCardPtr_; //����������
	unsigned int effectId_;  //Ч��ID
	FightManager * fightManager_;
};

//���߹�����
class PropManager
{
public:
	PropManager(FightManager* fightManager);
public:
	~PropManager(void);
public:
	//�ض������Ƿ����ʹ��ĳ��Ƭ
	bool isCanUserCard(IArmyPtr armyPtr , PropCardConfig * propCardPtr);
	//ʹ�ÿ�Ƭ
	void useCard(IArmyPtr armyPtr , PropCardConfig * propCardPtr);
	//�����Ƭʹ�ü�¼
	void clearCardRecord(void);
	//���һ�ſհ׿�Ƭ
	PropCard * getNewCard(IArmyPtr armyPtr ,  PropCardConfig * propCardPtr);
	//������ҵĿ�Ƭʹ�����
	void exportCardRecord(unsigned int playerId , int agent) const;

private:
	FightManager* fightManager_;
	//���ʹ�õ��߿���¼<���ID , <������ , ʹ�ô���>>
	map<unsigned int , map<CardType , int>> playerUsePropRecord_; 
	//<���ID,<��Ƭ����,<����ID,��ȴ�غ���>>>
	map<unsigned int,map<CardType,map<unsigned int , int>>> cardCoolingRecord_; 
};
