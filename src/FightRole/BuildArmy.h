#pragma once
#include "Army.h"
class BuildArmy :public Army
{
public:
	BuildArmy(void);
public:
	~BuildArmy(void);
public:
	bool Initialize(BuildArmyParam & armyParam ,FightManager * fightManager ,  ENUM_USE_SCENE state);
	//获得部队视野大小
	inline int GetViewScope() const{
		if (aiLevelParam_){
			return aiLevelParam_->sightVal;
		}
		return viewScope_;
	}
	//获得部队攻击范围
	inline int GetAttackScope() const{
		return attackScope_;
	}
	
	//备注：数据库中武器ID和Type错位
	//获得武器ID
	inline int GetWeaponID() const{
		return weaponID_;
	}
	//获得武器Type
	inline int GetWeaponType() const{
		return weaponType_;
	}
	//获得部队ID
	inline unsigned int GetArmyID() const{
		return armyID_;
	}
	//部队职业
	//备注建筑物部队的职业使用建筑物的详细信息
	//部队职业
	virtual SOLDIER_TYPE GetArmyOccupation()const {
		return soldierType_;
	}
	//获得攻击类型
	inline ATTACK_TYPE GetAttackType()const {
		return armyAttackType_;
	}	
	ARCH_TYPES getBuildDetailType() const {
		return buildDetailType_;
	}
	virtual void onKilled(IArmy* army);
	bool armyOnDie(IArmyPtr attackArmyPtr , bool isCatched , bool isProcessHurt);
	
	//设置建筑物的详细信息
	inline void setDetailType(ARCH_TYPES detailType){
		buildDetailType_ = detailType;
	}
	//获得模板ID
	inline int getTemplateID(void) const{
		return tmplateID_;
	}
	//设置模板ID
	inline void setTemplateID(int tmplateID) {
		tmplateID_ = tmplateID;
	}
	//调用部队处理
	virtual bool processAction(bool processMode); 
	//刷新建筑物的设置
	void upgrade(computinghurt::BuildingInfo& info, PublicBuildParamTable * buildParam);
	//获得防具ID
	virtual int GetDefendID() const{
		return defendID_;
	}
	//获得防具Type
	virtual int GetDefendType() const{
		return defendType_;
	}
	//重设建筑物的阵营归属
	void resetCountryID(NeutralState neutralState);
	//获得部队军牌掉落系数
	virtual int getArmyCardDropOdds(void) const {
		int buildLevel = buildLevel_ ? buildLevel_ : 1;
		return buildLevel * 200;
	}
	//设置建筑物的ID
	inline  void setBuildID(unsigned int buildID){
		buildID_ = buildID;
	}
	//获取建筑物的ID
	inline unsigned int getBuildID() const{
		return buildID_;
	}
	//获得建筑物当前的等级
	inline int getBuildLevel() const {
		return buildLevel_;
	}
protected:
	SOLDIER_TYPE soldierType_;//士兵职业
	unsigned int armyID_;//部队ID
	int weaponID_; //武器ID
	int weaponType_; //武器类型
	int defendID_; //防具ID
	int defendType_; //防具类型
	int attackPower_;//攻击力
	int attackScope_;//攻击视野
	int viewScope_;//视野
	int tmplateID_;//模板ID
	ATTACK_TYPE armyAttackType_;//部队攻击类型
	ARCH_TYPES buildDetailType_; //建筑物详细类型 
	int buildLevel_; //建筑物等级
	int armyCardsType_; // 军牌类型
	int cardsDropOdds_; //掉落概率
	unsigned int buildID_; //建筑物ID
};
