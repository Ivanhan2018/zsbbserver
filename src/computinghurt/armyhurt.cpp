// @brief 部队伤害类
// @author 张文龙

#include "Windows.h"
#include "log.hpp"
#include <complex>
#include "armyhurt.h"
#include "utility.h"
#include "common.h"


namespace
{
	static bool isValidSkillId( SKILL_TYPE id )
	{
		return id != SKILLID_INVALID;
	}

	static bool isValidEffectTypeId(EFFECT_TYPE effectId)
	{
		return effectId != INVALID_EFFECT;
	}

	enum 
	{
		MAX_ATTACK_POW = 20000
	};

	const double SQUARE_VAL = 0.67;
} // namespace

COMPUTINGHURT_NAMESPACE_BEGIN

const static double MAX_DERATE_RATE = 0.9;             // 最大减免率
const static double LANDFORM_AFFECT_MODULUS = 0.5;     // 地形影响系数

enum
{
	ONE_HUNDRED = 100,
	FIVE_HUNDRED = 500,	
	TWO_THOUSAND = 2000,
};

ArmyHurt::ArmyHurt(const ArmyInfo& armyInfo, const LandformInfoMap *landformInfoMapPtrParam, IArmy *army, IRandomGenerator* randomGenerator)
:soldiers_(armyInfo.soldierInfo, randomGenerator), hero_(armyInfo.heroInfo, randomGenerator, soldiers_), landformInfoMapPtr_(landformInfoMapPtrParam), selfArmy_(army)
{
	soldiers_.initHP(hero_.getHp());
}

ArmyHurt::~ArmyHurt()
{
	selfArmy_ = 0;
}

void ArmyHurt::release()
{
	delete this;
}

AttackInfo ArmyHurt::attack(Armys& armys)
{
	if (NULL == landformInfoMapPtr_)
	{
#ifdef _DEBUG
		assert(0);
#endif
		return AttackInfo();
	}

	MY_ASSERT(selfArmy_ != 0 && !armys.empty(), "armys.empty() || selfArmy_ == 0");
	
	if(armys.empty() || selfArmy_ == 0)
	{
		return AttackInfo();
	}

	unsigned int landformAffAtt = ONE_HUNDRED;

	LAND_FORM landform = selfArmy_->getLandForm();

	SOLDIER_TYPE soldierType = selfArmy_->GetArmyOccupation();

	LandformInfoMap::const_iterator itFind = landformInfoMapPtr_->find(std::make_pair(landform, soldierType));

	if (landformInfoMapPtr_->end() != itFind)
	{
		aistrategy::LandformAffect* landformAffPtr = itFind->second;

		if (NULL == landformAffPtr)
		{
			assert(0);
			return AttackInfo();
		}

		landformAffAtt = landformAffPtr->attackAffect;
	}

	// 计算基础攻击力
	int power = computeAtk();

	// 释放攻击技能
	AttackReleaseInfo attackInfo = hero_.releaseAttackSkill(selfArmy_, soldiers_.getType(), armys.front());

	if (!attackInfo)
	{
		assert(0);
	}

	BreakOutStru breakoutStru;
	breakoutStru.effectId = INVALID_EFFECT;
	breakoutStru.effectVal = 0.0;

	bool isAttackAllCell = false;      // 是否攻击整个格子

	NothingingDefendStru nothingDefStru;
	nothingDefStru.additionalHurt = 0;				 // 附加伤害值
	nothingDefStru.isNothinkingDefend = false;	     // 是否无视防御

	SKILL_TYPE skillId = attackInfo->skillId;
	EFFECT_TYPE effectId = attackInfo->effectType;

	// 根据效果类型
	switch (effectId)
	{
	case FORCE_ATTACK_EFFECT:
		breakoutStru.effectId = attackInfo->effectType;
		breakoutStru.effectVal = attackInfo->effectVal;
		break;
	case DEATH_WARRANT_EFFECT:
		nothingDefStru.isNothinkingDefend = true;
		break;
	case BLOOD_WITHOUT_END_EFFECT:
		nothingDefStru.isNothinkingDefend = true;
		nothingDefStru.additionalHurt = static_cast<unsigned int>(attackInfo->effectVal);
		break;
	case ATTACK_WHOLE_CELL_EFFECT:
		isAttackAllCell = true;
		break;
	default:
		break;
	}

	if(isValidEffectTypeId(effectId))
	{
		notifyReleaseSkill(skillId);

		if(nothingDefStru.isNothinkingDefend)
		{
			if (armys.size() > 1)
			{
				// for循环
				HarmInfos harminfos = processAttackAll(nothingDefStru, breakoutStru, armys, power, landformAffAtt);
				return newAttackInfo(selfArmy_, armys, skillId, soldiers_.getWeaponId(), harminfos);
			}
			else
			{
				HarmInfos harminfos;
				harminfos.push_back(processAttackOne(nothingDefStru, breakoutStru, armys.front(), power, landformAffAtt));
				return newAttackInfo(selfArmy_, armys, skillId, soldiers_.getWeaponId(), harminfos);
			}
		}

		// 释放攻击全格的技能,获取需要全格的部队
		if (isAttackAllCell)
		{
//#ifdef _DEBUG
//			if (armys.size() > 1)
//			{
//				assert(0 && "攻击全格的法师，又触发了错误的攻击全格的技能");
//			}
//#endif
			IArmyPtr armyPtrTmp = armys.front();

			if (NULL == armyPtrTmp)
			{
#ifdef _DEBUG
				assert(0 && "攻击的目标部队指针为空了");
#endif
			}

			const std::vector<POS_STRUCT> &posVect = armyPtrTmp->GetArmyPos();

#ifdef _DEBUG
			if (posVect.empty())
			{
				assert(0 && "获取部队的坐标失败");
			}
#endif
			const POS_STRUCT &posStru = posVect.front();
			list<IArmyPtr> lstArmy;
			const std::list<IArmyPtr> * lstArmyPtr = armyPtrTmp->getArmysInPos(posStru.PosX, posStru.PosY);

			if (NULL != lstArmyPtr)
			{
				armys.clear();
				armys.insert(armys.end(), (*lstArmyPtr).begin(), (*lstArmyPtr).end());
			}
			else
			{
				assert(0 && "调用获取同格的部队的函数失败");
			}
		}

		// 攻击全格
		if(isAttackAllCell || armys.size() > 1)
		{
			// for循环
			HarmInfos harminfos = processAttackAll(nothingDefStru, breakoutStru, armys, power, landformAffAtt);
			return newAttackInfo(selfArmy_, armys, skillId, soldiers_.getWeaponId(), harminfos);
		}
	}

	// 没有触发技能

	// 默认攻击数组中的第一个
	HarmInfos harminfos;
	harminfos.push_back(processAttackOne(nothingDefStru,  breakoutStru, armys.front(), power, landformAffAtt));
	return newAttackInfo(selfArmy_, armys, skillId, soldiers_.getWeaponId(), harminfos);

}

AttackInfo ArmyHurt::attack(Armys &armysCenter, Armys &armysInOneCircle)
{
	MY_ASSERT(selfArmy_ != 0, "selfArmy_ == 0");

	if (0 == selfArmy_)
	{
		return AttackInfo();
	}

	// 消耗掉该无敌卡
	selfArmy_->clearCard(SELFBLAST_CARD);

	/*
	if(armysCenter.empty() && armysInOneCircle.empty())
	{
		AttackInfo attackInfo(new AttackInfoValue);
		attackInfo->armyId = selfArmy_->GetArmyID();
		attackInfo->attackerPos = selfArmy_->GetArmyPos().front();
		attackInfo->targetPos = selfArmy_->GetArmyPos().front();
		attackInfo->armyType = selfArmy_->GetArmyType();
		attackInfo->weaponId = soldiers_.getWeaponId();
		attackInfo->attackType = PHYSIC_ATTACK;
		attackInfo->skillId = SELFBLAST_CARD_SKILL;
		attackInfo->harmInfos = HarmInfos();

		return attackInfo;
	}
	*/

	if (armysCenter.empty())
	{
		armysCenter.push_back(selfArmy_);
	}

	HarmInfos harminfos = processSelfBlastAttackAll(armysCenter, armysInOneCircle);

	// 自身自杀身亡
	soldiers_.suicide();	
	//
	selfArmy_->armyOnDie(selfArmy_, false, true);

	return newAttackInfo(selfArmy_, armysCenter, SELFBLAST_CARD_SKILL, soldiers_.getWeaponId(), harminfos);
}


HarmInfo ArmyHurt::beAttacked(const NothingingDefendStru &nothingDefStru,  IArmy *attacker, int power)
{
	if (NULL == landformInfoMapPtr_)
	{
#ifdef _DEBUG
		assert(0);
#endif
		return HarmInfo();
	}

	MY_ASSERT(selfArmy_!= 0 && attacker!= 0, "selfArmy_ == 0 || attacker == 0");

	if(selfArmy_ == 0 || attacker == 0)
	{
		return HarmInfo();
	}

	IArmy::CardUsedState cardUsedState = selfArmy_->getCardState(INVINCIBILITY_CARD);

	// 使用了无敌卡
	if (IArmy::IS_USING_CARD == cardUsedState)
	{
		return newHarmInfo(selfArmy_, 0, 0, SKILLID_INVALID);
	}

	unsigned int landformDefAtt = ONE_HUNDRED;

	LAND_FORM landForm = selfArmy_->getLandForm();

	SOLDIER_TYPE soldierType = selfArmy_->GetArmyOccupation();

	LandformInfoMap::const_iterator itFind = landformInfoMapPtr_->find(std::make_pair(landForm, soldierType));

	if (landformInfoMapPtr_->end() != itFind)
	{
		aistrategy::LandformAffect* landformAffPtr = itFind->second;

		if (NULL == landformAffPtr)
		{
			assert(0);
			return HarmInfo();
		}

		landformDefAtt = landformAffPtr->defendAffect;
	}

	HarmInfo harmInfoPtr;

	// 无视防御
	if(nothingDefStru.isNothinkingDefend)
	{
		// 所有攻击都为无视防御
		if (0 == nothingDefStru.additionalHurt)
		{
			return processHurtStraight(power, attacker, landformDefAtt);
		}
		// 只有增加的附加伤害才是无视防御的
		else
		{
			harmInfoPtr = processHurtStraight(nothingDefStru.additionalHurt, attacker, landformDefAtt);
		}
	}

	ATTACK_TYPE attackType = attacker->GetAttackType();

	unsigned int preserveVal = 0;

	if (PHYSIC_ATTACK == attackType)
	{
		preserveVal = soldiers_.getPhysicDefense();
	}
	else if (MAGIC_ATTACK == attackType)
	{
		preserveVal = soldiers_.getMagicDefense();
	}


	//#ifdef _DEBUG
	//	if (preserveVal >= 100)
	//	{
	//		assert(0 && "部队装备表中对应的防御减伤值大于等于100了");
	//	}
	//#endif

	if (preserveVal >= 100)
	{
		preserveVal = 98;
	}

	// 部队装备表中对应的防御减伤
	power = static_cast<int>(power * (1 - static_cast<float>(preserveVal) / 100));

	// 相克系数(根据攻击者的武器和被攻击者的防具来得到一个攻击力的减成)
	float attackFactor = attacker->getFactorAccordingToArmor(selfArmy_);
	power = static_cast<int>(power * attackFactor);

	// 根据攻击类型计算的英雄的防御力
	double defense = hero_.computeHeroDefence(attackType);

	defense = std::pow(defense, SQUARE_VAL) / 1000;

	//ljw 20091119
	//属性值过大，引起程序崩溃
	if (defense >= 1){
		//assert(0 && "防御指数不允许超过1");
		defense = 0.98;
	}
	power =  static_cast<int>(power * (1 - defense));

	// 光环防御技能的防御系数
	power = static_cast<int>(power * hero_.getDefenseFactor(selfArmy_->GetArmyOccupation(), attacker->GetArmyOccupation(), landForm));

	// 地形技能的防御系数
	power = static_cast<int>(power * hero_.getDefLandformFactor(selfArmy_->GetArmyOccupation(), attacker->GetArmyOccupation(), landForm));

	// 释放防御技能
	DefendReleaseInfo defendRelease = hero_.releaseDefenceSkill(selfArmy_, soldiers_.getType());

	EFFECT_TYPE effectId = defendRelease->effectType;
	SKILL_TYPE skillId = defendRelease->skillId;
	bool isReduceHurt = false;         // 减少伤害
	double effectVal = 0.0;		   
	bool isNothingAttack = false;	   // 无视攻击
	bool isHurt2AddHp = false;	       // 伤害转为加血

	// 根据效果类型
	switch (effectId)
	{
	case JIB_DEFENT_EFFECT:
		isReduceHurt = true;
		effectVal = defendRelease->effectVal;
		break;
	case INVINCIBILITY_DEFENT_EFFECT:
		isNothingAttack = true;
		break;
	default:
		break;
	}

	if(isValidEffectTypeId(effectId))
	{
		notifyReleaseSkill(skillId);

		// 无视攻击
		if (isNothingAttack)
		{
			if (harmInfoPtr)
			{
				return harmInfoPtr;
			}

			return newHarmInfo(selfArmy_, 0, 0, skillId);
		}

		if (isReduceHurt)
		{
#ifdef _DEBUG
			if (static_cast<int>(1 - effectVal) < 0)
			{
				assert(0 && "减少伤害的百分比值错误");
			}
#endif
			power = static_cast<int>(power * (1 - effectVal));
		}
	}

	int deaths = processHurt(attacker, power, landformDefAtt, isHurt2AddHp);


	if (harmInfoPtr)
	{
		return newHarmInfo(selfArmy_, power + harmInfoPtr->LostHp, deaths + harmInfoPtr->DealNum, skillId);	
	}
	else
	{
		return newHarmInfo(selfArmy_, power, deaths, skillId);
	}
}
#if 0

// 计算总的攻击力：士兵武器攻击力x人数x种族命中率x攻击系数x根据武器与防具得到的一个减成
int ArmyHurt::computeAtk(IArmy* beAttacker) const
{
	MY_ASSERT(selfArmy_ != 0,"selfArmy_ == 0");
	if(selfArmy_ == 0)
		return 0;

	// 该部队在不同的地方有不同的攻击倍数，如在森林，攻击力会减半
	float factor = selfArmy_->getAttackFactor();
	double atk = soldiers_.computeAtk() * hero_.getAttackFactor(soldiers_.getType()) + hero_.computeATK();

	// 根据攻击者的武器和被攻击者的防具来得到一个攻击力的减成
	float attackFactor = selfArmy_->getFactorAccordingArmor(beAttacker);

	return static_cast<int>(atk * factor * attackFactor);
}
#endif

int ArmyHurt::computeAtk(bool isShowPower) const
{
	MY_ASSERT(selfArmy_ != 0,"selfArmy_ == 0");
	if(selfArmy_ == 0)
		return 0;	

	double atk = soldiers_.computeAtk(isShowPower) + hero_.computeATK();

	return static_cast<int>(atk);
}

void ArmyHurt::notifyReleaseSkill(const SKILL_TYPE& skillId) const
{
	MY_ASSERT(selfArmy_ != 0,"selfArmy_ == 0");
	if(selfArmy_ == 0)
		return;

	if(isValidSkillId(skillId))
	{
		selfArmy_->notifyRelease(skillId);
	}
}

bool ArmyHurt::canCatchHero(IArmy* army)
{
	MY_ASSERT(army != 0, "army == 0");
	if(army == 0)
		return false;

	// 不是玩家部队， 不能捕获英雄
	if(!army->isPlayerArmy() || selfArmy_->GetArmyID() == army->GetArmyID())
		return false;

	SOLDIER_TYPE armyType = army->GetArmyOccupation();
	return hero_.catchHero(army, armyType);
}

bool ArmyHurt::canPlasterHero(IArmy* army)
{
	MY_ASSERT(army != 0, "army == 0");
	if(army == 0)
		return false;

	// 不是玩家部队， 不能捕获英雄
	if(!army->isPlayerArmy())
		return false;

	SOLDIER_TYPE armyType = army->GetArmyOccupation();
	return hero_.canPlasterHero(army, armyType);
}

int ArmyHurt::processHurt(const IArmy* attacker, int& hurt, unsigned int landformDefAtt, bool isHurt2AddHp)
{
	MY_ASSERT(attacker != 0, "army == 0");
	if(attacker == 0)
		return 0;

	hurt = static_cast<int>(hurt * (static_cast<float>(landformDefAtt) / 100));

	if (isHurt2AddHp)
	{
		return soldiers_.processHurt(PRIEST, hurt);
	}
	else
	{
		return soldiers_.processHurt(attacker->GetArmyOccupation(), hurt);
	}
}

int ArmyHurt::processReduce(double reduceProba, int& hurt) 
{
	return soldiers_.processReduce(reduceProba, hurt);
}	

SOLDIER_TYPE ArmyHurt::getSoldierType() const 
{
	return soldiers_.getType();
}

// 处理攻击所有的
HarmInfos ArmyHurt::processAttackAll(const NothingingDefendStru &nothingDefStru, const BreakOutStru &breakoutStru, Armys& armys, int atk, unsigned int landformAffAtt)
{
	MY_ASSERT(!armys.empty(), "armys.empty()");

	HarmInfos harminfos;
	for(Armys::iterator it = armys.begin(); it != armys.end(); ++it)
	{
		if (ARCH_BUILD_TOFT == (*it)->getBuildDetailType()
			|| ARCH_TRAP == (*it)->getBuildDetailType())
		{
			continue;			
		}

		harminfos.push_back(processAttackOne(nothingDefStru, breakoutStru, (*it), atk, landformAffAtt));
	}
	return harminfos;
}

// 处理自爆卡攻击所有的
HarmInfos ArmyHurt::processSelfBlastAttackAll(const Armys &armysCenter, const Armys &armysInOneCircle)
{
	unsigned int armyHp = selfArmy_->GetCurrentHp();

	unsigned int centerAttPower = armyHp * 30;

	// 自爆部队（自爆时）HP的300%的物理伤害（最小为2000）,注：对建筑物伤害需再乘X150%
	if (centerAttPower < TWO_THOUSAND)
	{
		centerAttPower = TWO_THOUSAND;
	}

	// 与目标部队相邻的敌方部队（即距离1格内），受到自爆部队（自爆时）HP×10％（最小为500）的物理伤害, 注：对建筑物伤害需再乘X150%
	unsigned int nearOneCircleAttPower = armyHp * 10;
	if (nearOneCircleAttPower < FIVE_HUNDRED)
	{
		nearOneCircleAttPower = FIVE_HUNDRED;
	}

	Armys::const_iterator itBeg = armysCenter.begin();
	Armys::const_iterator itEnd = armysCenter.end();
	HarmInfos harminfos;

	for(; itBeg!=itEnd; ++itBeg)
	{
		if ((*itBeg)->GetArmyID() == selfArmy_->GetArmyID())
		{
			continue;
		}

		if (BUILD == (*itBeg)->GetArmyType())
		{
			harminfos.push_back(processSelfBlastAttackOne((*itBeg), centerAttPower * 150 / 100));
		}
		else
		{
			harminfos.push_back(processSelfBlastAttackOne((*itBeg), centerAttPower));
		}
	}

    itBeg = armysInOneCircle.begin();
	itEnd = armysInOneCircle.end();

	for(; itBeg!=itEnd; ++itBeg)
	{
		if (BUILD == (*itBeg)->GetArmyType())
		{
			harminfos.push_back(processSelfBlastAttackOne((*itBeg), nearOneCircleAttPower * 150 / 100));
		}
		else
		{
			harminfos.push_back(processSelfBlastAttackOne((*itBeg), nearOneCircleAttPower));
		}
	}

	return harminfos;
}

HarmInfo ArmyHurt::processSelfBlastAttackOne(IArmy *beAttacker, int atk)
{
	MY_ASSERT(beAttacker != 0, "army == 0");

	if(beAttacker == 0)
	{
		return HarmInfo();
	}

	NothingingDefendStru nothingDefStru;
	nothingDefStru.additionalHurt = 0;				 // 附加伤害值
	nothingDefStru.isNothinkingDefend = false;	     // 是否无视防御

	HarmInfo harminfo = beAttacker->beAttacked(nothingDefStru,  selfArmy_, atk);

	// 经验值
	harminfo->obtainExp = addExp(beAttacker, harminfo->DealNum, harminfo->LostHp) ;
	//经验值/10
	//add by ljw 2009 12 06
	harminfo->obtainExp /= 10;
	
	return harminfo;
}

// 处理第一个部队
HarmInfo ArmyHurt::processAttackOne(const NothingingDefendStru &nothingDefStru, const BreakOutStru &breakoutStru, IArmy * beAttacker, int atk, unsigned int landformAffAtt)
{
	MY_ASSERT(beAttacker != 0, "army == 0");
	
	if(beAttacker == 0)
	{
		return HarmInfo();
	}

	if (selfArmy_->GetCountryID() == beAttacker->GetCountryID() && selfArmy_->GetArmyOccupation() != PRIEST)
	{
		LOG_ERROR(("error:技能处理时，攻击者的部队ID为：%d，归属英雄ID为：%d，国家ID为：%d，部队职业:%d， 带队英雄职业:%d;  被攻击的部队ID为：%d，归属英雄ID为：%d，国家ID为：%d，部队职业:%d， 带队英雄职业:%d", 
			selfArmy_->GetArmyID(), selfArmy_->GetOfficerID(), selfArmy_->GetCountryID(), selfArmy_->GetArmyOccupation(), selfArmy_->getHeroOccupation(), 
			beAttacker->GetArmyID(), beAttacker->GetOfficerID(), beAttacker->GetCountryID(), beAttacker->GetArmyOccupation(), beAttacker->getHeroOccupation()));
	}

	LAND_FORM landForm = selfArmy_->getLandForm();
	SOLDIER_TYPE soldierType = soldiers_.getType();
	SOLDIER_TYPE enemySoldierType = beAttacker->GetArmyOccupation();

	// 英雄光环类、地形类技能
	atk = static_cast<int>(atk * hero_.getAttackFactor(soldierType, enemySoldierType, landForm) 
		* hero_.getAttLandformFactor(soldierType, enemySoldierType, landForm));

	// 爆发类技能的倍数加成
	if (FORCE_ATTACK_EFFECT == breakoutStru.effectId)
	{
		atk = static_cast<int>(atk * (1 + breakoutStru.effectVal));
	}

	atk = static_cast<int>(atk * (static_cast<float>(landformAffAtt) / 100));

	if (atk > MAX_ATTACK_POW)
	{
		atk = MAX_ATTACK_POW;
	}

	HarmInfo harminfo = beAttacker->beAttacked(nothingDefStru,  selfArmy_, atk);

	if (harminfo)
	{
		// 经验值
		harminfo->obtainExp = addExp(beAttacker, harminfo->DealNum, harminfo->LostHp) ;
		//经验值/10
		//add by ljw 2009 12 06
		harminfo->obtainExp /= 10;
	}

	return harminfo;


#if 0
	MY_ASSERT(beAttacker != 0, "army == 0");
	if(beAttacker == 0)
		return HarmInfo();

	HarmInfo harminfo = beAttacker->beAttacked(selfArmy_, atk, additionalHurt, isNothinkingDefend);
	// 加经验
	addExp(beAttacker,harminfo->DealNum, harminfo->LostHp);
	return harminfo;
#endif

}

unsigned int ArmyHurt::addExp(const IArmy* beAttacker, int deaths, int hurt)
{
	MY_ASSERT(selfArmy_ != 0 && beAttacker != 0, "army == 0 || army == 0");
	if(selfArmy_ == 0 || beAttacker == 0)
		return 0;

	ATTACK_TYPE attackType = selfArmy_->GetAttackType();
	unsigned int exp = 0;

	if(attackType == ADD_HP) // 牧师加血
	{
		// 牧师给自己加血， 不加经验值
		if(beAttacker == selfArmy_)
			return 0;

		// 牧师攻击后， 加经验是将伤害除以10
		exp = -hurt/10;
		hero_.addExp(exp);
	}
	else
	{
		exp = beAttacker->GetLostExp(deaths);
		hero_.addExp(exp);
	}

	return exp;
}

// @brief 获得当前伤血
int ArmyHurt::getCurHurtHp() const
{
	return soldiers_.getCurHurtHp();
}

// @brief 获得英雄增加的经验值
int ArmyHurt::getHeroIncreaseExp() const
{
	return hero_.getIncreaseExp();
}

HarmInfo ArmyHurt::processHurtStraight( int power, const IArmy* attacker, unsigned int landformDefAtt)
{
	if(attacker == 0 )
		return HarmInfo();

	int deaths = processHurt(attacker, power, landformDefAtt);

	return newHarmInfo(selfArmy_, power, deaths, SKILLID_INVALID);
}

HarmInfo ArmyHurt::processReduceMan(double reduceProba)
{
	int hurt = 0;
	int deaths = processReduce(reduceProba, hurt);

	return newHarmInfo(selfArmy_, hurt, deaths, SKILLID_INVALID);
}

int ArmyHurt::getCurrentHp() const
{
	return soldiers_.getCurrentHp();
}

int ArmyHurt::getDefendPower(ATTACK_TYPE attackType) const
{
	int preserveVal = soldiers_.computeDefence(attackType);

	if (preserveVal >= 100)
	{
		preserveVal = 98;
	}

	// 部队装备表中对应的防御减伤
	float defendVal = static_cast<float>(preserveVal) / 100;

	// 根据攻击类型计算的英雄的防御力
	double defense = hero_.computeHeroDefence(attackType);

	defense = std::pow(defense, SQUARE_VAL) / 1000;

	if (defense >= 1)
	{
		defense = 0.98;
	}

	defense = defendVal + defense;

	if (defense >= 1)
	{
		defense = 0.98;
	}

	return static_cast<int>(defense * 100);
}

int ArmyHurt::GetAttackPower() const
{
	return computeAtk(true);
}

int ArmyHurt::getSoldierNum() const
{
	return soldiers_.getNum();
}

void ArmyHurt::resetHurt()
{
	soldiers_.resetHurt();
}

void ArmyHurt::upgradeLevel(const HeroAttribute& heroAttribute)
{
	hero_.upgradeLevel(heroAttribute);
	soldiers_.onHeroUpgradeLevel(hero_.getHp());
}

void ArmyHurt::upgradeOrLearnedSkill(const SkillInfo& skillInfo)
{	
	SKILL_TYPE skillID = skillInfo.skillAndGradeStru.id;
	double effectVal = 0.0;

	bool isSucess = hero_.isLearnedAddHpSkill(skillInfo.skillAndGradeStru, effectVal);

	if (isSucess)
	{
#ifdef _DEBUG
		assert(0 && "增加HP的技能已经学习了");
#endif
		return;
	}

	if (ADD_HP_SKILL == skillID)
	{
		soldiers_.addHp(skillInfo.effectVal);
	}

	hero_.upgradeOrLearnedSkill(skillInfo);
}

bool ArmyHurt::abandonSkill(const SkillAndGradeStru& skillAndGradeStru)
{
	double effectVal = 0.0;
	bool isSucess = hero_.isLearnedAddHpSkill(skillAndGradeStru, effectVal);

	if (isSucess)
	{
		soldiers_.reduceHp(effectVal);
	}

	return hero_.abandonSkill(skillAndGradeStru);
}

bool ArmyHurt::isLearnedSpecifiGradeSkill(const SkillAndGradeStru & skillAndGradeStruct)
{
	return hero_.isLearnedSpecifiGradeSkill(skillAndGradeStruct);
}

unsigned int ArmyHurt::resetSoldierNum(unsigned int soldierNum)
{
	unsigned int addNum = soldiers_.resetSoldierNum(soldierNum);

	soldiers_.initHP(hero_.getHp());

	return addNum;
}

unsigned int ArmyHurt::addSoldierNum(unsigned int soldierNum)
{
	unsigned int addNum = soldiers_.addSoldierNum(soldierNum);

	soldiers_.initHP(hero_.getHp());

	return addNum;
}

unsigned int ArmyHurt::getTotalHp() const
{
	return soldiers_.getTotalHp();
}

COMPUTINGHURT_NAMESPACE_END

