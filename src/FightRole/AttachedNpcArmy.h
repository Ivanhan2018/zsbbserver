#pragma once
#include "Army.h"

class AttachedNpcArmy :public Army
{
public:
	AttachedNpcArmy(void);
public:
	~AttachedNpcArmy(void);
public:
	bool Initialize(AttachedNpcArmyParam & armyParam , FightManager * fightManager, ENUM_USE_SCENE state , bool &isEnterSystem);
	virtual void getArmyTable(ArmyTable *& armyTable){
		armyTable = NULL;
	}
	//���ˢ�²��� 
	virtual bool getOfficerParam(tagHeroParam & /*officerParam*/) {
		return false;
	}
	virtual void onKilled(IArmy* army);
	virtual bool armyOnDie(IArmyPtr attackArmyPtr , bool isCatched , bool isProcessHurt);
	virtual inline int GetViewScope() const {
		if (aiLevelParam_){
			return aiLevelParam_->sightVal;
		}
		return npcArmyInfo_->viewScope;
	}
	virtual int GetAttackScope() const {
		return npcArmyInfo_->attackScope;
	}
	virtual int GetWeaponID() const {
		return npcArmyInfo_->weaponID;
	}
	virtual int GetWeaponType() const{
		return npcArmyInfo_->weaponType;
	}
	virtual int GetDefendID() const {
		return npcArmyInfo_->defendID;
	}
	int GetDefendType() const;
	virtual  unsigned int GetArmyID() const{
		return npcArmyID_;
	}
	bool createComputeHurt();
	//���ʿ������
	virtual RACE_TYPE GetSoldierRace() const {
		return (RACE_TYPE)npcArmyInfo_->raceID;
	}
	//���ʿ������
	inline int getSoldierNum() const {
		return computehurt_->getSoldierNum();
	}
	//����ְҵ
	virtual SOLDIER_TYPE GetArmyOccupation()const {
		return (SOLDIER_TYPE) npcArmyInfo_->soldierType;
	}
	FORWORD getDirection() const {
		return (FORWORD) npcDirection_;
	}
	//�����Ҳ��ӵ�����
	virtual const char * GetArmyName() const {
		return npcArmyInfo_->npcName;
	}
	virtual void setArmyPos(POS_STRUCT armyPos) {
		armyPos_[0] = armyPos;
	}
	bool Move(vector<POS_STRUCT>& moveTrack , IArmyPtr armyPtr);
	unsigned int GetLostExp(int num) const {
		return npcArmyInfo_->dropExp * num;
	}
	bool isNpcArmy() const {
		return true;
	}
	//��ù�������
	virtual ATTACK_TYPE GetAttackType()const {
		return (ATTACK_TYPE)npcArmyInfo_->attackType;
	}
	//��ø���Ŀ��
	inline IArmyPtr GetFollowTarget() const; 
	//���ø���Ŀ��
	inline void SetFollowTarget(IArmyPtr army){
		followTarget_ = army->GetArmyID();
	}
	void resetID(unsigned int npcArmyID){
		npcArmyID_ = npcArmyID;
	}
	//��������
	virtual bool revival(bool& isTerminate);
	//���Ӹ���
	virtual IArmyPtr repeat(ENUM_USE_SCENE state , bool& isTerminate);
	//��ò�������
	virtual int getArmyKind(void) const{
		return npcArmyInfo_->npcType;
	}
protected:
	NpcTemplate* npcArmyTable_;
	EctypeNpcTypeTable* npcArmyInfo_;
	unsigned int followTarget_;
	FORWORD npcDirection_;
	int npcArmyID_; //����ID

};
