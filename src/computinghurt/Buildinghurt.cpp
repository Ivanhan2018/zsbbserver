// @brief 建筑物伤害类
// @author 张文龙

#include "Buildinghurt.h"
#include <cmath>
#include "utility.h"

COMPUTINGHURT_NAMESPACE_BEGIN

BuildingHurt::BuildingHurt(IArmy* selfArmy, const BuildingInfo& buildingInfo):
selfArmy_(selfArmy),buildingInfo_(buildingInfo),totalHp_(buildingInfo.hp)
{

}

BuildingHurt::~BuildingHurt()
{
	selfArmy_ = 0;
}

void BuildingHurt::release()
{
	delete this;
}


void BuildingHurt::setArmy(IArmy* army)
{
	selfArmy_ = army;
}

AttackInfo BuildingHurt::attack(Armys& armys)
{
	MY_ASSERT(!armys.empty() && selfArmy_ != 0, "armys.empty() || selfArmy_ == 0");
	if(armys.empty() || selfArmy_ == 0)
		return AttackInfo();

	int atk = buildingInfo_.power;

	NothingingDefendStru nothingDefStru;
	nothingDefStru.additionalHurt = 0;
	nothingDefStru.isNothinkingDefend = false;

	HarmInfos harminfos ;
	harminfos.push_back(armys.front()->beAttacked(nothingDefStru, selfArmy_, atk));

	// TODO:武器ID为0
	return newAttackInfo(selfArmy_, armys, SKILLID_INVALID, buildingInfo_.weaponId, harminfos);
}

AttackInfo BuildingHurt::attack(Armys &/*armysCenter*/, Armys &/*armysInOneCircle*/)
{
	return AttackInfo();
}

bool BuildingHurt::canCatchHero(IArmy* /*army*/)
{
	// 建筑物不能捕获英雄，因为它没有任何技能
	return false;
}

bool BuildingHurt::canPlasterHero(IArmy* /*army*/)
{
	return false;
}


HarmInfo BuildingHurt::beAttacked(const NothingingDefendStru &nothingDefStru, IArmy* attacker, int power)
{
	
	MY_ASSERT(selfArmy_ != 0, "selfArmy_ == 0");
	if(selfArmy_ == 0)
		return HarmInfo();

	HarmInfo harmInfoPtr;

	if(nothingDefStru.isNothinkingDefend)
	{
		// 所有攻击都为无视防御
		if (0 == nothingDefStru.additionalHurt)
		{
			return processHurtStraight(power, attacker);
		}
		// 只有增加的附加伤害才是无视防御的
		else
		{
			harmInfoPtr = processHurtStraight(nothingDefStru.additionalHurt, attacker);
		}
	}

	ATTACK_TYPE attackType = attacker->GetAttackType();

	unsigned int preserveVal = 0;

	if (PHYSIC_ATTACK == attackType)
	{
		preserveVal = buildingInfo_.physicDefense;
	}
	else if (MAGIC_ATTACK == attackType)
	{
		preserveVal = buildingInfo_.magicDefense;
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

	power += nothingDefStru.additionalHurt;

	int deaths = processHurt(attacker, power);

	//// 得到本方的防御力
	//int defense = buildingInfo_.defense;

	//int hurt = computeHurt(power, defense, nothingDefStru.additionalHurt);
	//int deaths = processHurt(attacker, hurt);

	
	if (harmInfoPtr)
	{
		return newHarmInfo(selfArmy_, power + harmInfoPtr->LostHp, deaths, SKILLID_INVALID);
	}
	else
	{
		return newHarmInfo(selfArmy_, power, deaths, SKILLID_INVALID);
	}

}

int BuildingHurt::processHurt(IArmy* attacker, int& hurt)
{
	// 如果是牧师加血
	if(attacker->GetArmyOccupation() == PRIEST)
	{
		if(buildingInfo_.hp + hurt > totalHp_)
		{
			hurt = totalHp_ - buildingInfo_.hp;
			buildingInfo_.hp = totalHp_;

			hurt = -hurt;
			MY_ASSERT(hurt <= 0, "hurt >0");
			return 0;
		}

		buildingInfo_.hp += hurt;
		hurt = -hurt;
		MY_ASSERT(hurt <= 0, "hurt >0");
		return 0;
	}

	// 非牧师攻击建筑
	buildingInfo_.hp -= hurt;
	if(buildingInfo_.hp < 0)
	{
		hurt += buildingInfo_.hp;
		buildingInfo_.hp = 0;
	}
	return 0;
}

SOLDIER_TYPE BuildingHurt::getSoldierType() const
{
	return SOLDIERTYPE_NONE;
}

// @brief 获得当前伤血
int BuildingHurt::getCurHurtHp() const
{
	return totalHp_ - buildingInfo_.hp;
}

// @brief 获得英雄增加的经验值
int BuildingHurt::getHeroIncreaseExp() const
{
	return 0;
}

HarmInfo BuildingHurt::processHurtStraight( int power, IArmy* attacker )
{
	if(attacker == 0 )
		return HarmInfo();

	int deaths = processHurt(attacker, power);
	return newHarmInfo(selfArmy_, power, deaths, SKILLID_INVALID);
}


int BuildingHurt::getCurrentHp() const
{
	return buildingInfo_.hp;
}

int BuildingHurt::getDefendPower(ATTACK_TYPE attackType) const
{
	if (PHYSIC_ATTACK == attackType)
	{
		return buildingInfo_.physicDefense;
	}
	else if (MAGIC_ATTACK == attackType)
	{
		return buildingInfo_.magicDefense;
	}

	return 0;
}

int BuildingHurt::GetAttackPower() const
{
	return buildingInfo_.power;
}

int BuildingHurt::getSoldierNum() const
{
	return 0;
}

void BuildingHurt::resetHurt()
{
	buildingInfo_.hp = totalHp_;
}

void BuildingHurt::upgrade(const BuildingInfo& info)
{
	totalHp_ = info.hp;
	buildingInfo_.power = info.power;
	buildingInfo_.physicDefense = info.physicDefense;
	buildingInfo_.magicDefense = info.magicDefense;
	buildingInfo_.hp = info.hp;
	buildingInfo_.weaponId = info.weaponId;
}


COMPUTINGHURT_NAMESPACE_END
