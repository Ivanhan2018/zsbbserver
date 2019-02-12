#pragma once
#include "IArmy.h"
class FightManager;

//������Ʒ������
class DropGoodsManager
{
public:
	DropGoodsManager(FightManager * fightManager);
public:
	~DropGoodsManager(void);
public:
	//������Ƶ���
	bool processArmyCardDrop(IArmyPtr attackArmy , IArmyPtr army ,int armyCardID , int odds);
	//������Ҿ��Ƶ���
	void processPlayerDrop(IArmyPtr attackArmy , IArmyPtr army);
	//�������þ���ͳ����Ϣ
	void resetArmyCardDropRecord(void);
private:
	bool processTimeLimit(int& index); //����ʱ������
	bool processCountryLimit(int countryID); //��������
	bool processTotalLimit(void); //����������
	bool processCardLevelLimit(int cardLevel); //������Ƶȼ�����
	bool processLimit(LIMIT_TYPE limttType , int limitKey); //��������
	int  getArmyCardLevel(IArmyPtr army) const;	//��þ��Ƶȼ�
	bool increaseLimit(LIMIT_TYPE limttType ,int limitKey); //��������
private:
	FightManager * fightManager_; //ս��������
	time_t lastSaveTime_; //�ϴδ���ʱ��
private:
	NO_COPYABLE_AND_ASSIGN(DropGoodsManager);
};
