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
	//获得刷新参数 
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
	//获得士兵种族
	virtual RACE_TYPE GetSoldierRace() const {
		return (RACE_TYPE)npcArmyInfo_->raceID;
	}
	//获得士兵数量
	inline int getSoldierNum() const {
		return computehurt_->getSoldierNum();
	}
	//部队职业
	virtual SOLDIER_TYPE GetArmyOccupation()const {
		return (SOLDIER_TYPE) npcArmyInfo_->soldierType;
	}
	FORWORD getDirection() const {
		return (FORWORD) npcDirection_;
	}
	//获得玩家部队的名称
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
	//获得攻击类型
	virtual ATTACK_TYPE GetAttackType()const {
		return (ATTACK_TYPE)npcArmyInfo_->attackType;
	}
	//获得跟随目标
	inline IArmyPtr GetFollowTarget() const; 
	//设置跟随目标
	inline void SetFollowTarget(IArmyPtr army){
		followTarget_ = army->GetArmyID();
	}
	void resetID(unsigned int npcArmyID){
		npcArmyID_ = npcArmyID;
	}
	//部队重生
	virtual bool revival(bool& isTerminate);
	//部队复制
	virtual IArmyPtr repeat(ENUM_USE_SCENE state , bool& isTerminate);
	//获得部队类型
	virtual int getArmyKind(void) const{
		return npcArmyInfo_->npcType;
	}
protected:
	NpcTemplate* npcArmyTable_;
	EctypeNpcTypeTable* npcArmyInfo_;
	unsigned int followTarget_;
	FORWORD npcDirection_;
	int npcArmyID_; //部队ID

};
