#pragma once
#include "armymanager.h"
struct Notify_Enter_Ectype;
struct EctypeInfoTable;
class EctypeArmyManager :public ArmyManager
{
public:
	EctypeArmyManager(void);
public:
	~EctypeArmyManager(void);
public:
	//ͨ������ID���armyָ��
	virtual IArmyPtr getArmyByID(unsigned int armyID) const;
	//�������Ӳ���
	virtual bool checkAddArmy(IArmyPtr armyPtr){
		return addArmy(armyPtr);
	}
	//�Ƿ���Ч�ĸ����ȼ�
	virtual bool isValidLevel(int playerLevel);
	//��ʼ�����������Ϣ
	bool initPlayerArmy(Notify_Enter_Ectype* lpEvent ,list<IArmyPtr>& armyList , map<int ,int>&countryMap ,map<unsigned int , unsigned int>& tmpArmyMap);
	//��ʼ������NPC��Ϣ
	bool initNpcArmy(int ectypeID , EctypeInfoTable * ectypeInfo);
	//װ�ؽ�����
	bool loadBuildInfo(void);
	//����������ʵ��
	BuildArmy*  createBuildArmy(unsigned int buildID , list<POS_STRUCT>& pos , const POS_STRUCT &centryPos);
	//��Ӳ��ӵ�ϵͳ
	bool insertArmyToSystem(list<IArmyPtr>& armyList);
};
