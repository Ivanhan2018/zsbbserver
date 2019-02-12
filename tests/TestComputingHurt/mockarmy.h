#ifndef TESTCOMPUTINGHURT_MOCKARMY_H_
#define TESTCOMPUTINGHURT_MOCKARMY_H_
#include "iarmy.h"
#include "ihurt.h"


void setHarmParam(const HarmInfo& harmInfo, HarmParm& harmParam)
{
	harmParam = *harmInfo.get();
}

void setFightReport(const AttackInfo& attackInfo, REPORT_FIGHT_INFO_STRUCT& fightReport)
{
	fightReport.ulAttackArmyID = attackInfo->armyId;
	fightReport.AttackPos = attackInfo->attackerPos;
	fightReport.TargetPos = attackInfo->targetPos;

	fightReport.type = attackInfo->armyType;
	fightReport.uWeaponID = attackInfo->weaponId;
	fightReport.AttackType = attackInfo->attackType;
	fightReport.Skill = attackInfo->skillId;
	HarmInfos harminfos = attackInfo->harmInfos;

	size_t count = harminfos.size() < 5 ? harminfos.size() : 5;
	fightReport.count = static_cast<int>(count);

	HarmInfos::const_iterator it = harminfos.begin();
	for (unsigned int i = 0; i < count; ++i)
	{
		HarmInfo info = (*it);
		setHarmParam(info, fightReport.harm[i]);
		++it;
	}
}


typedef boost::shared_ptr<IArmy> IArmySharedPtr;
typedef std::map<unsigned int, IArmySharedPtr> ArmysMap;


class IFightManager
{
public:
	virtual ~IFightManager(){};
	virtual IArmy* getArmy(unsigned int id) const = 0;
};



using namespace computinghurt;

class MockArmy : public IArmy
{
	friend class TestArmyHurt;
public:
	MockArmy(IHurt* computingHurt, IFightManager* manager)
		:computehurt_(computingHurt),skillid_(SKILLID_INVALID), hurt_(0),isCatched_(true),armyTable_(),manager_(manager)
	{
		if(computehurt_)
			computehurt_->setArmy(this);

		POS_STRUCT pos = {1, 1, 1};
		s.push_back(pos);
	}

	virtual void Attack(ArmyArrays& armys)
	{
		if(computehurt_ == 0 || manager_ == 0)
			return ;

		Armys armyPtrs;
		ArmyArrays::const_iterator it = armys.begin();
		for (; it != armys.end(); ++it)
		{
			armyPtrs.push_back(manager_->getArmy(*it));
		}

		AttackInfo attackInfo = computehurt_->attack(armyPtrs);
		hurt_ = 0;

//		REPORT_FIGHT_INFO_STRUCT fightReport;
//		setFightReport(attackInfo, fightReport);
#if 0
		{
			unsigned int armyid = attackInfo.get<0>();
			Positon attackerPos = attackInfo.get<1>();
			Positon targetPos = attackInfo.get<2>();
			ArmyType at = attackInfo.get<3>();
			unsigned int weaponId = attackInfo.get<4>();
			// 攻击类型
			AttackType attackType = attackInfo.get<5>(); 
			// 攻击方释放的技能
			SkillId skillId = attackInfo.get<6>();

			HarmInfos harmInfos = attackInfo.get<7>();
		}
#endif
	}

	virtual HarmInfo beAttacked(IArmy* attacker, int atk, int additionalHurt, bool isNothinkingDefend)
	{
		if(computehurt_ == 0)
			return HarmInfo();

		HarmInfo info = computehurt_->beAttacked(attacker, atk, additionalHurt, isNothinkingDefend);		

		hurt_ = info->LostHp;
		return info;
	}

	// 处理伤害
// 	virtual int processHurt(IArmy* attacker, int hurt) 
// 	{
// 		if(computehurt_ == 0)
// 			return 0;
// 
// 		int deaths = computehurt_->processHurt(attacker, hurt);
// 		hurt_ = hurt;
// 		return deaths;
// 	}

	// 通知释放了技能
	virtual void notifyRelease(SKILL_TYPE id)
	{
		skillid_ = id;
	}

	void onKilled(IArmy* army) 
	{
		if (computehurt_ == 0)
			return;

		if(computehurt_->canCatchHero(army))
		{
			isCatched_ = true;
		}

		isCatched_ = false;
	}

	virtual SOLDIER_TYPE GetArmyOccupation() const 
	{
		return computehurt_->getSoldierType();
	}

	// 得到部队的ID
	virtual unsigned int GetArmyID() const 
	{
		return 0;
	}

	virtual ARMY_TYPE GetArmyType() const
	{
		return PLAYER_ARMY;
	}

	virtual  const std::vector<POS_STRUCT>& GetArmyPos()const 
	{
		return s;
	}

	//获得部队视野大小
	virtual inline int GetViewScope() const {return 0;}
	//获得部队攻击范围
	virtual int GetAttackScope() const {return 0;}
	//获得部队防御
	inline  virtual int GetDefendPower(ATTACK_TYPE /*attackType*/) const {return 0;}
	//获得部队攻击力
	inline  virtual int GetAttackPower() const{return 0;}
	//获得部队当前的HP 
	inline  virtual int GetCurrentHp() const {return 0;}
	//获得部队的归属
	virtual  unsigned int getPlayerID() const {return 0;}
	//获得部队英雄ID
	virtual  unsigned int GetOfficerID() const{return 0;}
	//备注：数据库中武器ID和Type错位
	//获得武器ID
	virtual int GetWeaponID() const {return 0;}
	//获得武器Type
	virtual int GetWeaponType() const {return 0;}
	//获得防具ID
	virtual int GetDefendID() const {return 0;}
	//获得防具Type
	virtual int GetDefendType() const{return 0;}
	//获得掉落经验
	virtual  unsigned int GetLostExp(int /*num*/) const {return 1;}
	//获得跟随目标
	virtual IArmyPtr GetFollowTarget() const {return armyptr_;}
	//设置跟随目标
	virtual void SetFollowTarget(IArmyPtr /*army*/) {}
	//判断是否友军
	virtual bool IsFriendsArmy(IArmyPtr /*army*/)  {return false;}
	//部队移动
	virtual bool Move(vector<POS_STRUCT>& /*moveTrack*/ , IArmyPtr /*armyPtr*/){return false;}
	//获得玩家部队的名称
	virtual  const char * GetArmyName()const{return 0;}
	//获得攻击类型
	virtual  ATTACK_TYPE GetAttackType()const {return PHYSIC_ATTACK;}
	//获得最近一次自己攻击的目标
	virtual  IArmyPtr GetLastAttackTarget() {return armyptr_;}
	//获得最近一次攻击自己的目标
	virtual  IArmyPtr GetLastEnmityTarget() {return armyptr_;}
	//获得自己进入系统时位置信息
	virtual  POS_STRUCT GetBirthPos()const{return POS_STRUCT();}
	//获得国家ID 
	virtual int GetCountryID()const {return 0;}
	//获得托管状态
	virtual  bool GetTrusteeshipState()const {return false;}
	//部队死亡事件
	virtual bool armyOnDie(IArmyPtr /*attackArmyPtr*/){return false;}
	//获得士兵数量
	virtual int getSoldierNum() const {return 0;}
	//获得部队的战斗角色
	virtual FIGHT_ROLE_TYPE GetFightRole() const {return ATTACT;}
	//获得英雄的头像ID
	virtual int GetFaceID() const {return 0;}
	//获得士兵种族
	virtual RACE_TYPE GetSoldierRace() const {return HUMAN;}
	//获得部队英雄信息 
	virtual const char * GetOfficerName() const {return 0;}
	//获得英雄力量
	virtual int GetOfficerPower() const {return 0;}
	//获得英雄耐力
	virtual int GetOfficerStamina() const{return 0;}
	//获得英雄敏捷
	virtual int GetOfficerCelerity() const {return 0;}
	//获得英雄智力
	virtual int GetOfficerIntellect() const {return 0;}
	//获得英雄级别
	virtual int GetOfficerLevel() const {return 0;}
	//获得当前部队的伤血
	virtual int GetHurtedHp() const {return 0;}
	//获得部队方向
	virtual FORWORD getDirection() const{return LEFTUP;}
	//获得部队状态
	virtual int GetArmyState() const {return 0;}
	//获得部队移动速度
	virtual int getMoveSpeed() const {return 0;}
	//导出部队信息
	virtual bool exportArmyInfo(ARMY_POS & /*armyInfo*/ , int /*mapID*/) {return false;}
	//调用部队处理
	virtual bool processAction() {return false;}
	//客户端的MOVE消息
	virtual bool processMoveMsg(POS_STRUCT & /*movePos*/) {return false;}
	//获得逆转坐标服务器到客户端
	virtual POS_STRUCT getReversePosS_C(POS_STRUCT & /*sourcePos*/) {return POS_STRUCT();}
	//获得逆转坐标客户端到服务器
	virtual POS_STRUCT getReversePosC_S(POS_STRUCT & /*sourcePos*/) {return POS_STRUCT();}
	//重新设置部队的位置
	virtual void reSetArmyPos(POS_STRUCT /*armyPos*/){return ;}
	//设置攻击目标
	virtual bool processAttackTarget(const IArmyPtr /*attackTargetPtr*/) {return false;}
	//获得玩家部队的ArmyTable信息
	virtual void getArmyTable(ArmyTable *& /*armyTable*/) {}
	//获得建筑物的详细类型
	virtual BuildingType getBuildDetailType() const {return BUILDINGTYPE_BASTILE;}
	virtual bool isPlayerArmy() const {return true;}
	virtual ENUM_ARMY_STATUS getArmyActionStatus() const {return Attack_status;};
	//获得当前攻击的影响系数
	virtual float getAttackFactor() const {return 1;}
	//获得当前防御力的影响系数
	virtual float getDefendFactor() const {return 1;}
	//获得最近一次自己攻击的目标
	virtual  void SetLastAttackTarget(IArmyPtr /*lastAttackTarget*/) {return;}
	//获得最近一次攻击自己的目标
	virtual  void SetLastEnmityTarget(IArmyPtr /*lastEnmityTarget*/) {return;}

	//获得当前伤血
	virtual int getCurHurtHp() const {return 0;}
	//获得英雄增加的经验值
	virtual int getHeroIncreaseExp() const{return 0;}

	virtual bool getOfficerParam(tagHeroParam & /*officerParam*/) {return false;}
	//获得FROM地图的ID
	virtual int  getFromMapID() const{return 0;}

	//获得策略管理器
	virtual IManager * getStrategyMgr() const{return 0;}
	//设置托管状态
	virtual void setTrusteeshipState(bool /*trusteeshipState*/) {}
	//释放内存
	virtual void release() {}

	//通过部队ID获得其他部队信息
	virtual IArmyPtr getArmyByID(unsigned int /*armyID*/) {return 0;};

	//获得最近一次自己攻击的目标
	virtual  void SetLastAttackTarget(unsigned int /*lastAttackTarget*/)  {}
	//获得最近一次攻击自己的目标
	virtual  void SetLastEnmityTarget(unsigned int /*lastEnmityTarget*/) {}
	//获得掉落物品
	virtual int getBootyID() const {return 0;}
	//获得掉落物品的概率
	virtual int getBootyProbability() const {return 0;}
	//部队死亡事件
	virtual bool armyOnDie(IArmyPtr /*attackArmyPtr*/ , bool /*isCatched*/) {return true;}
	//获得有效位置
	virtual POS_STRUCT getValidPos(POS_STRUCT & pos) {return pos;};

	//获得玩家名称
	virtual const string & getPlayerName() const { static const std::string s(""); return s;}
	//是否是NPC部队
	virtual bool isNpcArmy() const {return false;}
	//部队信息重新设置
	virtual bool reset(int /*condition*/) {return false;}

	////刷新部队经验
	virtual bool refreshExp(void) {return false;}
	//英雄升级
	virtual bool officerGrowUp(){return false;}

	// 根据攻击者(自己)的武器与被攻击者的防具来得到一个减成
	virtual float getFactorAccordingToArmor(IArmy* /*beAttacker*/)
	{
		return 1.0;
	}

	//重新设置部队的位置
	virtual void setArmyPos(POS_STRUCT /*armyPos*/)
	{
		return;
	}
	//重新设置士兵数量
	virtual bool resetSoldierNum(int /*soldierNum*/) {return true;}
	//重新设置部队的位置
	virtual POS_STRUCT setGatewayArmyPos(POS_STRUCT armyPos)
	{
		return armyPos;
	}


private:
	MockArmy();
private:
	IHurt* computehurt_;
	SKILL_TYPE skillid_; // 当前释放的技能
	int hurt_; // 受到的伤害
	bool isCatched_;
	std::vector<POS_STRUCT> s;
	ArmyTable armyTable_;
	IArmyPtr armyptr_;
	IFightManager* manager_;
};


class MockFightManager : public IFightManager
{
public:
	MockFightManager()
		:armys_()
	{
	}

	~MockFightManager()
	{

	}

	void insert(unsigned int id, IArmySharedPtr* army)
	{
		armys_.insert(ArmysMap::value_type(id, *army));
	}


	virtual IArmy* getArmy(unsigned int id) const
	{
		ArmysMap::const_iterator it = armys_.find(id);
		if(it == armys_.end())
			return 0;

		return (*it).second.get();
	}

private:

	ArmysMap armys_;
};

#endif // TESTCOMPUTINGHURT_MOCKARMY_H_
