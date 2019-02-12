#pragma once
#include <map>
#include "IArmy.h"
#include "ArmyManager.h"
using namespace std;
class FightManager;
class CLoadMapInfo;
class CNPCInfo;
class DB_Proxy;
class CLoadDBInfo;
class CountyQueueMgr;
class ClearSecuSection;
class IniFile;
class BuildArmy;



//���в��ӹ�����
//��Ҫ���������в��Ӻͽ��������Ϣ
class CountryArmyManager :public ArmyManager
{
public:
	CountryArmyManager(void);
public:
	~CountryArmyManager(void);
public:
	//��Ӳ��ӵ����ӹ�������
	bool addArmy(IArmyPtr armyPtr);
	//������ӹ������е�ĳ����
	void eraseArmy(IArmyPtr armyPtr);
	bool Initialize(CLoadMapInfo *mapInfo ,CNPCInfo * npcInfo ,IniFile * iniFile ,
		DB_Proxy * proxy ,CLoadDBInfo *dataBaseMemData, FightManager * fightManager);
	
	bool moveArmy(IArmyPtr armyPtr , POS_STRUCT & targetPos);
	//�������Ӳ���
	bool checkAddArmy(IArmyPtr armyPtr);
	//����ARMY_ADD��Ϣ
	bool exportAddArmyInfo(IArmyPtr armyPtr ,ARMY_ADD& addArmyInfo);
	//��listת��Ϊvec
	void getVecPosFromList(vector<POS_STRUCT> & vecPos , list<POS_STRUCT> & listPos);
	//��������
	void onArmyDie(IArmy* attackPtr ,IArmy* army);
	//�ͷ��ض���ҵĲ���
	int freeArmyFromQueue(unsigned int playerID);
	//�ͷ�
	void release();
	// �ն�
	void receiveExcessManInCountry(int excessManNum);
	//���������߻����ִ�
	void kickExpeditionArmy(int countryID);
	//ʤ������
	void attackOrguardSwopOnWin(int winCountryID);
	//�������ݵ�DBG
	void saveArmyDataToDBG(void);
	//��ɨ��ȫ��
	void clearSecSection(void);
	//�����ҵĲ����б�
	void getPlayerArmys(unsigned int playerID , vector<IArmyPtr> & vecArmys , int countryID, bool isQuitQueue);

	//ս���������˺��ָ�
	void cleanupHurt(void);
	//ͨ������ID���armyָ��
	virtual IArmyPtr getArmyByID(unsigned int armyID) const;
	//װ��Ѳ�߲���
	bool attachPatrolArmy();
	//ǿ�Ƴ���
	virtual int quitQueue(void) ;
	//������ɧ�Ų���
	void createGuardArmy(int mapId , int countryId);
private:
	//װ�ز�����Ϣ
	bool loadArmyInfo(void);
	//װ�ؽ�������Ϣ
	bool loadBuildInfo(void);
	//�Ƿ���GM�Ľ�������
	bool isHaveGMAttack() const;
private:
	//�����Ӷ��й�����
	CountyQueueMgr * armyQueueManager_;
	//����ȫ��������
	ClearSecuSection * secManager_;
};
