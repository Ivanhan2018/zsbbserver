#pragma once
#include <map>
#include "IArmy.h"
#include "Army.h"
using namespace std;
class FightManager;
class CLoadMapInfo;
class CNPCInfo;
class DB_Proxy;
class CLoadDBInfo;
class IniFile;
class BuildArmy;
class ArmyManager
{
public:
	ArmyManager(void);
public:
	~ArmyManager(void);
public:
	//ͨ������ID���armyָ��
	virtual IArmyPtr getArmyByID(unsigned int armyID) const = 0;
	//�������Ӳ���
	virtual bool checkAddArmy(IArmyPtr armyPtr) = 0;
	//ʤ������
	virtual void attackOrguardSwopOnWin(int winCountryID);
	//�������ݵ�DBG
	virtual void saveArmyDataToDBG(void);
	//��ɨ��ȫ��
	virtual void clearSecSection(void);
	//�ͷ��ض���ҵĲ���
	virtual int freeArmyFromQueue(unsigned int playerID);
	//ս���������˺��ָ�
	virtual void cleanupHurt(void);
	// �ն�
	virtual void receiveExcessManInCountry(int excessManNum);
	//�����ҵĲ����б�
	virtual void getPlayerArmys(unsigned int playerID , vector<IArmyPtr> & vecArmys  , int countryID, bool isQuitQueue);
	//��Ӳ��ӵ�������
	virtual bool addArmy(IArmyPtr armyPtr);
	//�����Ӵӹ����������
	virtual void eraseArmy(IArmyPtr armyPtr);
	//�����Ӽ���
	bool insertArmyToList(IArmyPtr armyPtr);
	//�ͷŲ����б�
	void freeArmyList(list<IArmyPtr> & armyList);
	//��ʼ��
	bool Initialize(CLoadMapInfo *mapInfo ,CNPCInfo * npcInfo ,IniFile * iniFile ,
		DB_Proxy * proxy ,CLoadDBInfo *dataBaseMemData, FightManager * fightManager);
	//������Ҳ���ʵ��
	IArmyPtr createPlayerArmy(unsigned int armyID);
	IArmyPtr createPlayerArmy(ArmyTable & armyTable);
	//���������ﲿ��ʵ��
	BuildArmy* createBuildArmy(PublicBuildParamTable & buildParam,vector<POS_STRUCT> & buildPos,unsigned int armyID);
	//װ�ؽ�����
	virtual BuildArmy*  createBuildArmy(unsigned int buildID ,  list<POS_STRUCT>& pos , const POS_STRUCT &centryPos);
	//���첿�Ӳ���
	bool createPlayerArmyParam(PlayerArmyParam & armyParam , ArmyTable & armyTable);
	//�ж��Ƿ���Ч����
	bool isValidArmy(IArmyPtr armyPtr) const;
	//�����ƶ�
	bool moveArmy(IArmyPtr armyPtr , POS_STRUCT & targetPos);
	
	//����ARMY_ADD��Ϣ
	bool exportAddArmyInfo(IArmyPtr armyPtr ,ARMY_ADD& addArmyInfo);
	//�Ƿ������
	bool isExpeditionArmy(IArmyPtr armyPtr);
	//�Ƿ����ִ�
	bool isNewbie(void);
	//
	void getVecPosFromList(vector<POS_STRUCT> & vecPos , list<POS_STRUCT> & listPos);
	//��������
	void onArmyDie(IArmy* attackPtr ,IArmy* army);
	//���������Ĳ���
	virtual void processDieArmy(void);
	//�ͷ��ڴ�
	void release();
	//�Ƿ�Ϊ��Чλ��
	bool IsValidPos(int x ,int y) const;
	//���ɳ���б�
	inline vector<vector<list<IArmyPtr> > > * getArmyList(void){
		return &mapArmyList_;
	}
	//���ϵͳ�еĲ���MAP
	inline const map<unsigned int , IArmyPtr>& getArmyMap(void) const{
		return armyMap_;
	}
	//����ض�����
	std::list<IArmyPtr> * const getArmysInPos(int x , int y){
		if (mapInfo_->IsValidPos(x , y) == false){
			return NULL;
		}
		return &mapArmyList_[x][y];
	}
	//��÷��ط�����
	inline int getDefendArmyNum(void) {
		return defendArmyNum_;
	}
	//��ý���������
	inline int getAttackArmyNum(void) {
		return attackArmyNum_;
	}
	//ǿ�Ƴ���
	virtual int quitQueue(void) {
		return 0;
	}
	//�Ƿ���Ч�ĸ����ȼ�
	virtual bool isValidLevel(int /*playerLevel*/){
		return false;
	}
protected:
	DB_Proxy * dataBaseProxy_;
	//��ͼ����б�
	vector<vector<list<IArmyPtr> > > mapArmyList_;
	//�����б�
	map<unsigned int , IArmyPtr> armyMap_; 
	//�������Ӷ���
	map<unsigned int , IArmyPtr> armyDieMap_;
	//��ǰ��ͼ
	CLoadMapInfo * mapInfo_;
	//NPC��Ϣ��
	CNPCInfo * npcInfo_;
	//DBG ��������
	CLoadDBInfo *dataBaseMemData_;
	//ս��������
	FightManager* fightManager_;
	//�����ļ�
	IniFile * iniFile_;
	//����������
	int attackArmyNum_;
	//���ط�����
	int defendArmyNum_;
	//�����
	computinghurt::IRandomGenerator* randomgenerator_;
};
