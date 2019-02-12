#pragma once
#include "Army.h"
#include "computinghurt/common.h"
class PlayerArmy :public Army
{
public:
	PlayerArmy(void);
public:
	virtual ~PlayerArmy(void);
public:
	
	//初始化部队
	bool Initialize(PlayerArmyParam & armyParam , FightManager * fightManager ,  ENUM_USE_SCENE state);
	
	//获得部队视野大小
	virtual inline int GetViewScope() const{
		if (aiLevelParam_){
			return aiLevelParam_->sightVal;
		}
		return soldierParam_->visualrange_;
	}
	//获得部队攻击范围
	inline int GetAttackScope() const{
		return soldierParam_->attackscop_;
	}
	//获得部队的归属
	unsigned int getPlayerID() const{
		return armyTable_.playerid_;
	}
	//获得部队英雄ID
	unsigned int GetOfficerID() const {
		return playerOfficerTable_.officerid_;
	}
	//备注：数据库中武器ID和Type错位
	//获得武器ID
	int GetWeaponID() const{
		return armyWeapon_->weaponID;
	}
	//获得武器Type
	int GetWeaponType() const {
		return armyWeapon_->weaponType;
	}
	//获得防具ID
	virtual int GetDefendID() const{
		if (armyDefend_){
			return armyDefend_->defendID;
		} 
		return 0;
	}
	//获得防具Type
	inline int GetDefendType() const {
		if (armyDefend_){
			return armyDefend_->defendType;
		}
		return 0;
	}
	//获得掉落经验
	inline unsigned int GetLostExp(int num) const{
		return num * armyWeapon_->exp;
	}
	//获得跟随目标
	inline IArmyPtr GetFollowTarget() const; 
	//设置跟随目标
	inline void SetFollowTarget(IArmyPtr army){
		followTarget_ = army->GetArmyID();
	}
	
	//获得部队ID
	inline  unsigned int GetArmyID() const{ 
		return armyTable_.armyid_;
	}
	//获得英雄的头像ID
	inline int GetFaceID() const {
		return playerOfficerTable_.faceid_;
	}
	//获得士兵种族
	inline RACE_TYPE GetSoldierRace() const {
		return (RACE_TYPE)armyTable_.raceid_;
	}
	//获得部队英雄信息 
	inline const char * GetOfficerName() const {
		return playerOfficerTable_.officername_;
	}
	//获得英雄力量
	inline int GetOfficerPower() const {
		return playerOfficerTable_.power_ + playerOfficerTable_.levelpower_ + playerOfficerTable_.equippower_;
	}
	//获得英雄耐力
	inline int GetOfficerStamina() const {
		return playerOfficerTable_.stamina_ + playerOfficerTable_.levelstamina_ + playerOfficerTable_.equipstamina_;
	}
	//获得英雄敏捷
	inline int GetOfficerCelerity() const {
		return playerOfficerTable_.celerity_ + playerOfficerTable_.levelcelerity_ + playerOfficerTable_.equipcelerity_;
	}
	//获得英雄智力
	inline int GetOfficerIntellect() const{
		return playerOfficerTable_.wit_ + playerOfficerTable_.levelwit_ + playerOfficerTable_.equipwit_;
	}
	//获得英雄级别
	inline int GetOfficerLevel() const {
		return playerOfficerTable_.level_;
	}
	//部队职业
	virtual SOLDIER_TYPE GetArmyOccupation()const {
		return (SOLDIER_TYPE)soldierParam_->soldiertype_;
	}
	//获得士兵数量
	inline int getSoldierNum() const {
		return computehurt_->getSoldierNum();
	}
	//获得攻击类型
	ATTACK_TYPE GetAttackType()const {
		return (ATTACK_TYPE)armyWeapon_->hurt;
	}	
	//部队死亡事件
	virtual bool armyOnDie(IArmyPtr attackArmyPtr , bool isCatched , bool isProcessHurt);
	//获得玩家部队的名称
	const char * GetArmyName() const {
		return armyTable_.armyname_;
	}
	FORWORD getDirection() const {
		return (FORWORD) armyTable_.direction;
	}
	bool processMoveMsg(POS_STRUCT & movePos){
		PosXYStru pos(movePos.PosX , movePos.PosY);
		return action_->setMoveTargetPos(pos);
	}
	bool processAttackTarget(const IArmyPtr attackTargetPtr){
		return action_->setAttackTarget(attackTargetPtr);
	}
	//部队移动
	bool Move(vector<POS_STRUCT>& moveTrack ,IArmyPtr armyPtr) ;
	//重新设置部队的位置
	virtual void setArmyPos(POS_STRUCT armyPos) {
		armyPos_[0] = armyPos;
	}
	virtual void getArmyTable(ArmyTable *& armyTable)  {
		armyTable = &armyTable_;
	}
	//创建计算伤害
	bool createComputeHurt(ARMY_TYPE armyType);
	//获得刷新参数 
	virtual bool getOfficerParam(tagHeroParam & /*officerParam*/) {
		return false;
	}
	//获得FROM地图的ID
	virtual int  getFromMapID() const{
		return armyTable_.mapid_;
	}
	//设置应
	virtual float getAttackFactor() const ;
	//
	virtual void onKilled(IArmy* army);
	virtual int getOfficerExp(void){
		return playerOfficerTable_.exp_;
	}
	virtual bool refreshExp(void);
	//英雄升级
	virtual bool officerGrowUp();
	//@brief 属性升级
	void upgradeLevel(const computinghurt::HeroAttribute& heroAttribute , PlayerOfficerTable & playerInfo);
	//@brief 学习了新的技能或技能升级
	void upgradeOrLearnedSkill(const computinghurt::SkillInfo& skillInfo);
	//废弃技能
	bool abandonSkill(const computinghurt::SkillAndGradeStru& skillAndGradeStru);
	//是否可以重创英雄
	virtual bool canPlasterHero(IArmyPtr armyPtr) const {
		return computehurt_->canPlasterHero(armyPtr);
	}
	//获得部队状态
	virtual int getArmyDBState(void) const {
		return armyTable_.state_;
	}
	//获得部队军牌掉落系数
	virtual int getArmyCardDropOdds(void) const;
	//获得英雄职业
	virtual OFFICER_OCCUPATION_TYPE getHeroOccupation() const {
		return static_cast<OFFICER_OCCUPATION_TYPE>(playerOfficerTable_.jobid_);
	}
	inline void setMercenary(){
		isMercenary_ = true;
	}
	inline bool getIsMercenary() const{
		return isMercenary_;
	}
	inline void setPlayerName(const string&  playerName){
		playerName_ = playerName;
	}
protected:
	
	PlayerOfficerTable playerOfficerTable_; //部队带队英雄表
	WeaponAttribute* armyWeapon_;
	DefendAttribute* armyDefend_;
	SoldierParamTable * soldierParam_ ;//士兵参数表
	OfficerRaceAttribute *officerRace_;//英雄种族属性表
	ArmyTable armyTable_; //部队信息
	unsigned int followTarget_; //跟随目标
	bool isMercenary_; //是否佣兵
};
