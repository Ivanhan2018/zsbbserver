#pragma once
#include <iostream>
#include "IArmy.h"
#include "ArmyAction.h"
#include "hurtfactory.h"
#include "ihurt.h"
#include "ServerProtocolHeader.h"
#include "NPCInfo.h"
#include "ZSBB_DataDefine.h"
//部队参数集 
struct PlayerArmyParam{
	WeaponAttribute *weaponAttribute; //武器属性表
	DefendAttribute *defendAttribute; //防具属性表
	SoldierParamTable * soldierParam ;//士兵参数表
	OfficerRaceAttribute *officerRaceAttribute; //英雄种族表
	ArmyTable armyTable; 
	PlayerOfficerTable playerOfficerTable; //部队带队英雄表
	MapDataStru mapData; //地图信息 及 部队沙盘信息
};

//NPC部队参数集 
struct NpcArmyParam{
	EctypeNpcArmyTable* npcArmyTable;
	EctypeNpcTypeTable* npcArmyInfo;
	MapDataStru mapData; //地图信息 及 部队沙盘信息
};
//NPC部队参数集 
struct AttachedNpcArmyParam{
	NpcTemplate* npcArmyTable;
	EctypeNpcTypeTable* npcArmyInfo;
	MapDataStru mapData; //地图信息 及 部队沙盘信息
	unsigned int adscriptionArmyID; //归属者的部队ID
};

//防骚扰NPC参数
struct GuarderArmyParam{
	NpcGuarderParam* npcArmyTable; 
	EctypeNpcTypeTable* npcArmyInfo;
	MapDataStru mapData; //地图信息 及 部队沙盘信息
};

//建筑部队参数集 
struct BuildArmyParam{
	PublicBuildParamTable buildParam;//建筑物的参数 
	DefendAttribute *defendAttribute; //防具属性表
	unsigned int armyID; //部队ID
	vector<POS_STRUCT> armyPos; //部队位置
	MapDataStru mapData; //地图信息 及 部队沙盘信息
};
enum AttackTypeEnu
{
	Unknown_Type = -1,
	Attack_Place = 0,          // 0：地点
	Attack_Follow,             // 1：跟随
	Attack_Instantaneous,      // 2：瞬时
};

#define LOW_LIMIT_SPEED_TIME 500
#define HIGH_LIMIT_SPEED_TIME 4000
class FightManager;
class PropCard;
struct EffectDetail;
class ArmAttack;
class Army : public IArmy
{
public:
	Army(void);
public:
	virtual ~Army(void);
public:
	//攻击
	void Attack(unsigned int armyId, unsigned int numCircle=0);
	void Attack(Armys &armyPtrs);
	//获得部队攻击力
	inline  int GetAttackPower() const{
		return computehurt_->GetAttackPower();
	}
	int GetDefendPower(ATTACK_TYPE attackType) const{
		return computehurt_->getDefendPower(attackType);
	}
	//获得防具ID
	virtual int GetDefendID() const{
		return 0;
	}
	//获得防具Type
	virtual int GetDefendType() const{
		return 0;
	}
	//获得部队的归属
	virtual  unsigned int getPlayerID() const{
		return 0;
	}
	//获得部队英雄ID
	virtual  unsigned int GetOfficerID() const{
		return 0;
	}
	
	//获得掉落经验
	virtual  unsigned int GetLostExp(int num) const{
		return 0*num;
	}

	//获得跟随目标
	IArmyPtr GetFollowTarget() const{
		return NULL;
	}

	//设置跟随目标
	virtual void SetFollowTarget(IArmyPtr /*army*/){
		return;
	}

	//判断是否友军
	virtual bool IsFriendsArmy(IArmyPtr army){
		if(GetCountryID() == army->GetCountryID()){
			return true;
		}
		return false;
	}

	//部队移动
	virtual bool Move(vector<POS_STRUCT>& /*moveTrack*/ , IArmyPtr /*armyPtr*/);

	//获得英雄的头像ID
	virtual int GetFaceID() const {
		return 0;
	}

	//获得士兵种族
	virtual RACE_TYPE GetSoldierRace() const {
		return (RACE_TYPE)0;
	}

	//获得部队英雄信息 
	virtual const char * GetOfficerName() const {
		return NULL;
	}

	//获得英雄力量
	virtual int GetOfficerPower() const {
		return 0;
	}

	//获得英雄耐力
	virtual int GetOfficerStamina() const {
		return 0;
	}

	//获得英雄敏捷
	virtual int GetOfficerCelerity() const {
		return 0;
	}

	//获得英雄智力
	virtual int GetOfficerIntellect() const{
		return 0;
	}

	//获得英雄级别
	virtual int GetOfficerLevel() const {
		return 0;
	}

	//获得士兵数量
	virtual int getSoldierNum() const {
		return 1;
	}

	//部队职业
	virtual SOLDIER_TYPE GetArmyOccupation()const {
		return (SOLDIER_TYPE) 0;
	}
	//获得攻击类型
	virtual ATTACK_TYPE GetAttackType()const {
		return (ATTACK_TYPE)0;
	}	

	//获得玩家部队的名称
	virtual const char * GetArmyName() const {
		return NULL;
	}

	//获取部队的位置
	inline const std::vector<POS_STRUCT>& GetArmyPos() const {
		return armyPos_;
	}

	//获得部队类型
	inline ARMY_TYPE GetArmyType()const  {
		return armyType_;
	}

	//获得最近一次自己攻击的目标
	inline IArmyPtr GetLastAttackTarget(){
		return getArmyByID(lastAttackTarget_);
	}

	///获得最近一次攻击自己的目标
	inline IArmyPtr GetLastEnmityTarget(){
		return getArmyByID(lastEnmityTarget_);
	}
	//获得最近一次自己攻击的目标
	virtual  void SetLastAttackTarget(unsigned int lastAttackTarget){  
		lastAttackTarget_ = lastAttackTarget;
	}
	//获得最近一次攻击自己的目标
	virtual  void SetLastEnmityTarget(unsigned int lastEnmityTarget){
		lastEnmityTarget_ = lastEnmityTarget;
	}
	//获得自己进入系统时位置信息
	inline POS_STRUCT GetBirthPos()const {
		return armyBirthPos_;
	}

	//获得国家ID 
	inline int GetCountryID()const {
		return countryID_;
	}

	//获得托管状态
	inline  bool GetTrusteeshipState()const{
		return isTrusteeship_;
	}

	FORWORD changeDirection(POS_STRUCT& curPos,POS_STRUCT& nextPos);
	virtual FORWORD getDirection() const {
		return (FORWORD) 0;
	}
	//获得部队状态
	virtual int GetArmyState() const {
		return 0;
	}
	//导出部队信息
	virtual bool exportArmyInfo(ARMY_POS & armyInfo , int mapID);
	//获得部队移动速度
	virtual int getMoveSpeed() const {
		return moveSpeed_;
	}
	//调用部队处理
	virtual bool processAction(bool processMode);
	///创建AI处理模块
	bool createArmyAction(MapDataStru& mapData , ENUM_USE_SCENE sceneState , AiLevelParam *aiLeveParam);
	virtual bool processMoveMsg(POS_STRUCT & /*movePos*/){
		return false;
	}
	virtual bool processAttackTarget(const IArmyPtr /*attackTargetPtr*/){
		return false;
	}
	//重新设置部队的位置
	virtual void setArmyPos(POS_STRUCT /*armyPos*/) {
		return;
	}
	virtual void getArmyTable(ArmyTable *& armyTable){
		armyTable = NULL;
	}

	// add by zwl
	virtual HarmInfo beAttacked(const computinghurt::NothingingDefendStru &nothingDefStru, IArmy* attacker, int power){
		if(computehurt_ == 0)
			return HarmInfo();

		return computehurt_->beAttacked(nothingDefStru, attacker, power);		
	}
	//获得英雄属性
	virtual const computinghurt::HeroAttribute * getHeroAttribute() const{
		return computehurt_->getHeroAttribute();
	}
	// 通知释放了技能
	virtual void notifyRelease(SKILL_TYPE /*id*/){
	}

	//获得建筑物的详细类型
	virtual ARCH_TYPES getBuildDetailType() const {
		return ARCH_INVALID;
	}
	//是不是玩家
	virtual bool isPlayerArmy() const {
		if (getPlayerID() >= 100000){
			return true;
		}
		return false;
	}
	virtual ENUM_ARMY_STATUS getArmyActionStatus() const {
		return action_->getArmyStatusOnTrusteeship();
	}
	//获得当前攻击的影响系数
	virtual float getAttackFactor() const{ 
		return 1.0;
	}
	//获得当前防御力的影响系数
	virtual float getDefendFactor() const {
		return 1.0;
	}
	//获得英雄增加的经验值
	virtual int getHeroIncreaseExp() const{
		return computehurt_->getHeroIncreaseExp();
	}
	//获得刷新参数 
	virtual bool getOfficerParam(tagHeroParam & /*officerParam*/) {
		
		return false;
	}
	//获得FROM地图的ID
	virtual int  getFromMapID() const{
		return 0;
	}
	//获得策略管理器
	virtual  IManager * getStrategyMgr() const {
		return action_->getStrategyMgr();
	}
	//获得当前血值
	inline   int GetCurrentHp() const {
		if(computehurt_ == NULL){
			return 0;
		}
		return computehurt_->getCurrentHp();
	}
	//获得当前伤血
	virtual int getCurHurtHp() const {
		if(computehurt_ == NULL){
			return 0;
		}
		return computehurt_->getCurHurtHp();
	}
	//获得初始HP
	virtual int getInitHp(void) const {
		return computehurt_->getTotalHp();
	}
	//设置托管状态
	virtual void setTrusteeshipState(bool trusteeshipState) {
		isTrusteeship_ = trusteeshipState;
	}
	//释放内存
	inline void release(); 
	//通过部队ID获得其他部队信息
	virtual IArmyPtr getArmyByID(unsigned int armyID) ;


	//获得有效位置
	virtual POS_STRUCT getValidPos(POS_STRUCT & pos ,unsigned int limitNum = 3 , unsigned int  distanceVal = 1){
		POS_STRUCT tmpPos = {0}; 
		PosXYStru tmpXYPos;
		tmpXYPos.xPos = pos.PosX;
		tmpXYPos.yPos = pos.PosY;
		if(action_->getAdaptivePos(tmpXYPos , tmpXYPos , limitNum , distanceVal) == true){
			tmpPos.PosX = tmpXYPos.xPos;
			tmpPos.PosY = tmpXYPos.yPos;
		}
		return tmpPos;
	}
	void setBirthPos(POS_STRUCT & birthPos){
		armyBirthPos_ = birthPos;
	}
	virtual const string & getPlayerName() const{
		return playerName_;
	}
	bool isNpcArmy() const {
		return false;
	}
	//刷新部队经验
	virtual bool refreshExp(void){
		return false;
	}
	//英雄升级
	virtual bool officerGrowUp(){
		return false;
	}
	//获得带队英雄原有经验
	virtual int getOfficerExp(void){
		return 0;
	}
	// 根据攻击者(自己)的武器与被攻击者的防具来得到一个减成
	virtual float getFactorAccordingToArmor(IArmy* beAttacker); 
	//重设部队数量
	virtual unsigned int resetSoldierNum(int soldierNum){
		return computehurt_->resetSoldierNum(soldierNum);
	}
	// 增加部队数量
	virtual unsigned int addSoldierNum(unsigned int soldierNum){
		return computehurt_->addSoldierNum(soldierNum);
	}
	// 增加部队移动速度
	virtual void addMoveSpeed(int increaceSpeed) {
		moveSpeed_ += increaceSpeed;

		if (moveSpeed_ <= 0)
		{
			calculateSpeed();
		}
	}
	virtual void addMoveTime(int increaseTime){
		movePerNeedTime_ += increaseTime;
		calculateSpeed();
	}
	//设置部队位置
	virtual POS_STRUCT setGatewayArmyPos(POS_STRUCT armyPos){
		PosXYStru tmpPos;
		tmpPos.xPos = armyPos.PosX;
		tmpPos.yPos = armyPos.PosY;
		action_->getAdaptivePos(tmpPos , tmpPos);
		armyPos.PosX= tmpPos.xPos;
		armyPos.PosY = tmpPos.yPos;
		armyPos_[0] = armyPos;
		return armyPos;
	}
	//清除伤害
	virtual void clearHurt(void){
		computehurt_->resetHurt();
	}
	//设置托管策略
	virtual void setStrategy(enumAIStrategy aiStrategy){
		action_->setStrategy(aiStrategy);
	}
	//是否可见
	virtual bool isVisible(IArmy * army) const {
		if (army->GetCountryID() != GetCountryID() && army->getBuildDetailType() >= 
			ARCH_BUILD_TOFT && army->getBuildDetailType() <= ARCH_TOFT_END){
			return isVisible_;
		}
		return true;
	}
	//获得部队当前所处地形
	virtual LAND_FORM getLandForm(void) const ;
	//部队重生
	virtual bool revival(bool& isTerminate) {
		isTerminate = true;
		return false;
	}
	//部队复制
	virtual IArmyPtr repeat( ENUM_USE_SCENE /*state*/ , bool& isTerminate){
		isTerminate = true;
		return NULL;
	}
	//是否BASE部队
	virtual bool isRepeatArmy(void) const {
		return isRepeatArmy_;
	}
	//设置是否BASE部队
	inline void forbidRepeat() {
		isRepeatArmy_ = false;
	}
	//获得特定坐标
	const std::list<IArmyPtr> *  getArmysInPos(int x , int y) const;
	//绑定附属部队
	inline void attachArmy(unsigned int armyID){
		attachArmys.push_back(armyID);
	}
	//清除所有绑定部队
	inline void clearAttachArmy(void){
		attachArmys.clear();
	}
	//获得部队绑定列表
	inline const vector<unsigned int>& getAttachArmys(void) const{
		return attachArmys;
	}
	void resetID(unsigned int /*npcArmyID*/){
		return ;
	}
	//设置建筑物国家ID
	inline void setCountryID(int countryID) {
		countryID_ = countryID;
	}
	// 函数描述：指定某点，在这个点周围找最合适的点（非建筑物，格子没有占满5人，非敌军站的格）
	bool getNearPosInOneCircle(const SEEK_PATH_NAME_SPACE::PosXYStru &posInput, SEEK_PATH_NAME_SPACE::PosXYStru &posOutput){
		return action_->getNearPosInOneCircle(posInput , posOutput);
	}
	//是否静态阻挡
	inline bool isStaticBlook(int x, int y) const {
		return action_->isStaticBlook(x , y);
	}
	//获得中心位置
	inline const POS_STRUCT & getCentryPos(void) const{
		return centryPos_;
	}
	//设置中心位置
	inline void setCentryPos(const POS_STRUCT & pos){
		centryPos_ = pos;
	}
	//是否可以重创英雄
	virtual bool canPlasterHero(IArmyPtr /*armyPtr*/) const {
		return false;
	}
	// 获取英雄属性类型
	virtual computinghurt::HeroType getHeroType() const {
		return computehurt_->getHeroType();
	}
	//获得部队状态
	virtual int getArmyDBState(void) const {
		return 1;
	}
	//获得地图状态
	virtual mapstate getMapState(void) const ;
	//设置隶属部队ID
	virtual int setLordArmy(unsigned int armyID) {
		return lordArmyID_ = armyID;
	}
	//获得部队英雄信息 
	virtual const string getHeroName() const {
		if (GetOfficerName() == NULL){
			return "";
		}
		return string(GetOfficerName());
	}
	//是否新手村
	virtual bool isNewbie()  const;
	//获得部队军牌掉落系数
	virtual int getArmyCardDropOdds(void) const;
	//获得当前回合数
	virtual int getCurBoutNum(void) const ;
	//获得英雄职业
	virtual OFFICER_OCCUPATION_TYPE getHeroOccupation() const {
		return OFFICER_OCCUPATION_NONE;
	}
	//清除和其相关的效果
	virtual void clearEffect(void);
	//是否可以使用某类型卡片
	virtual bool isCanUseCard(CardType cardType){
		map<CardType ,PropCard*>::iterator iter = cardRecord_.find(cardType);
		return iter == cardRecord_.end();
	}
	//使用卡片
	virtual void useCard(PropCardConfig * propCardPtr);
	//获得某种类型卡状态
	virtual IArmy::CardUsedState getCardState(CardType cardType);
	//获得某种类型卡
	virtual void clearCard(CardType cardType);
	//攻击
	void Attack(Armys &armyPtrsCenter, Armys &armyPtrsNear);
	//使用效果
	virtual unsigned int useEffect(ENUM_SPECIALLY_EFFECT type , IArmyPtr armyPtr , int lastTime);
	//获得部队类型
	virtual int getArmyKind(void) const{
		return 0;
	}
	//设置军团名称
	virtual void setCropsName(char * cropsName){
		if (cropsName){
			cropsName_ = cropsName;
		}
	}
	//获得军团名称
	virtual const string & getCropsName(void) const{
		return	cropsName_; 
	}
	//是否可以使用武器
	virtual bool isCanUseWeapon(void) const;
	//是否可以移动
	virtual bool isCanMove(void) const;
	//获得弹道速度
	virtual int getTrajectorySpeed(void ) const{
		return trajectorySpeed_;
	}
	virtual void clearEffectByType(ENUM_SPECIALLY_EFFECT type , bool isClearEffect);
protected:
	EffectDetail * getEffectByType(ENUM_SPECIALLY_EFFECT type);
	void tidyEffect(void);
	
	void getArmysFromArmyArrays(ArmyArrays &armys , Armys& armyPtrs);
	//是否处于锁定状态
	bool isNoLockAction(void) const;
	//计算移动速度
	void calculateSpeed(void);
	//清理卡片
	void tidyCard(void);
protected:
	bool isTrusteeship_; //是否托管
	vector<POS_STRUCT> armyPos_; //部队位置
	POS_STRUCT armyBirthPos_; //诞生地的位置
	POS_STRUCT centryPos_; //中心位置
	unsigned int lastAttackTarget_;//最近一次的攻击目标
	unsigned int lastEnmityTarget_;//最近一次攻击自己的目标
	unsigned int lordArmyID_; //隶属部队ID
	ARMY_TYPE armyType_; //部队类型
	int countryID_; //国家ID 
	ArmyAction * action_; //部队行动处理对象
	FightManager * fightManager_; //战斗管理器
	//伤害模型
	computinghurt::IHurt *computehurt_; 
	//附属部队NPC
	vector<unsigned int> attachArmys;
	//玩家部队名称
	string playerName_;
	//隶属军团名称
	string cropsName_;
	//AI等级参数
	AiLevelParam * aiLevelParam_;
	//上次累计经验值
	int exp_;
	//是否有动作
	bool isActioned_;
	//是否可见
	bool isVisible_;
	//死亡时的周期数
	int deadCycle_;
	//是否BASE部队
	bool isRepeatArmy_;
	//部队重生次数
	int revivalNum_;
	//效果使用记录
	map<ENUM_SPECIALLY_EFFECT , unsigned int> effectRecord_; 
	//卡片使用记录
	map<CardType , PropCard *> cardRecord_;
	int moveSpeed_; //移动速度
	int attackFreq_; //攻击频率
	int attackLockBout_; //攻击锁定时间
	int moveLockBout_; //移动锁定时间
	//上次攻击回合数
	unsigned int lastAttackBoutNum_;
	//上次移动时的回合数
	unsigned int lastMoveBoutNum_;
	list<ArmAttack*> attackActions_;//攻击动作集
	int movePerNeedTime_; //移动一格所需时间
	AttackTypeEnu attackType_;  // 攻击类型
	int trajectorySpeed_; //弹道速度
};
