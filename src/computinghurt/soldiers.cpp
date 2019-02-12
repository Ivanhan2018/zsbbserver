// @brief 士兵集合类，它负责处理伤害，输出攻击力和防御力
// @author 张文龙

#include "soldiers.h"

COMPUTINGHURT_NAMESPACE_BEGIN

namespace
{
	enum
	{
		ONE_HUNDRED = 100
	};
}

const double COMPUTE_SOLDIER_BLOOD_MODULUS = 1.5;

Soldiers::Soldiers(const SoldiersInfo& soldierInfo, IRandomGenerator* randomGenerator)
:soldierInfo_(soldierInfo)
,hpOfDeath_(0)
,totaoHp_(0)
,totalHurt_(0)
,randomGenerator_(randomGenerator)
{

}

void Soldiers::initHP(int heroHp)
{

	hpOfDeath_ = static_cast<int>(soldierInfo_.hp + heroHp / COMPUTE_SOLDIER_BLOOD_MODULUS);
	totaoHp_ = hpOfDeath_ * soldierInfo_.numberOfSoldiers;	
	
	MY_ASSERT(hpOfDeath_ > 0, "hpOfDeath_ <= 0");
	MY_ASSERT(totaoHp_ > 0, "totaoHp_ <= 0");
}

Soldiers::~Soldiers()
{
	totaoHp_ = 0;
	totalHurt_ = 0;
}

// @brief 处理伤害，伤害的计算公式为：先计算出杀死一个后所需要的HP算出来
//        它(P)＝（所有士兵的HP + 英雄的HP）/ 士兵人数
//        然后，算出死亡的兵数，使用总的伤害值(总的伤害值为上次剩余的伤害值加上这次的伤害值)整除，
//        即 D＝伤害值（T）/ P
//        剩下的伤害值为：T = T - D*P
// @param attackerSoldierType 攻击部队士兵的类型，如果是牧师，就是加血
// @parm hurt 伤害
// @param heroHp 英雄的HP
int Soldiers::processHurt(SOLDIER_TYPE attackerSoldierType, int& hurt)
{
	//assert(totaoHp_ != 0);
	//MY_ASSERT(soldierInfo_.numberOfSoldiers != 0 && hurt >= 0, 
	//	"totaoHp_ == 0 || soldierInfo_.numberOfSoldiers == 0");
	if (totaoHp_ == 0 || soldierInfo_.numberOfSoldiers == 0)
	{
		hurt = 0;
		totalHurt_ = 0;
		return 0;
	}

	// 杀死一个士兵所需要的HP
	if(hpOfDeath_ == 0)
	{
		hurt = 0;
		totalHurt_ = 0;
		return 0;
	}

	// 是牧师攻击
	if(attackerSoldierType == PRIEST)
	{
		totalHurt_ -= hurt;
		if(totalHurt_ <= 0)
		{
			hurt += totalHurt_;
			// 牧师加血，返回去的是一个负值
			hurt = -hurt;
			totalHurt_ = 0;
			MY_ASSERT(hurt <= 0, "hurt > 0");
			return 0;
		}
		hurt = -hurt;
		MY_ASSERT(hurt <= 0, "hurt > 0");
		return 0;
	}
//	else
//		totalHurt_ += hurt;

	int curHp = getCurrentHp();
	// 如果当前HP<=伤害值，就直接死亡
	if(curHp <= hurt)
	{
		hurt = curHp;
		// 直接处理部队死亡
		int deaths = soldierInfo_.numberOfSoldiers;
		soldierInfo_.numberOfSoldiers = 0;
		totalHurt_ = 0;
		totaoHp_ = 0;
		return deaths;
	}


	totalHurt_ += hurt;
// 	if(totalHurt_ <= 0)
// 	{
// 		hurt += totalHurt_;
// 		totalHurt_ = 0;
// 		return 0;
// 	}
// 
// 	// 如果累积伤害值比当前的HP大，或相等，部队就直接死亡
// 	if(totaoHp_ - totalHurt_ <= 0)
// 	{
// 		// 修正hurt
// 		hurt = totaoHp_;
// 		// 直接处理部队死亡
// 		int deaths = soldierInfo_.numberOfSoldiers;
// 		soldierInfo_.numberOfSoldiers = 0;
// 		totalHurt_ = 0;
// 		totaoHp_ = 0;
// 		return deaths;
// 	}

	// 死亡个数
	const int deaths  = (totalHurt_ / 4 )/ hpOfDeath_;
	// 是否是这样减？
	totalHurt_ -= deaths * hpOfDeath_;

	// 部队HP
	totaoHp_ -= deaths * hpOfDeath_;
	
	soldierInfo_.numberOfSoldiers -= deaths;

	if(totaoHp_ <= 0 || 0 >= soldierInfo_.numberOfSoldiers)
	{
		totaoHp_ = 0;
		totalHurt_ = 0;
		soldierInfo_.numberOfSoldiers = 0;
		// 回调， 通知所属Army部队人数为0, 即死亡
		return deaths;			 
	}

	
	return deaths;	
}

unsigned int Soldiers::suicide()
{
	// 直接处理部队死亡
	int deaths = soldierInfo_.numberOfSoldiers;
	soldierInfo_.numberOfSoldiers = 0;
	totalHurt_ = 0;
	totaoHp_ = 0;

	return deaths;
}

unsigned int Soldiers::processReduce(double reduceProba, int& hurt)
{
	unsigned int deadth = 0;
	deadth = static_cast<unsigned int>(soldierInfo_.numberOfSoldiers * reduceProba);

	hurt = hpOfDeath_ * deadth;

	if (totaoHp_ < hurt)
	{	
		hurt = totaoHp_;
		totaoHp_ = 0;
		totalHurt_ += hurt;
	}
	else
	{
		totaoHp_ -= hurt;
		totalHurt_ += hurt;
	}

	return deadth;
}

SOLDIER_TYPE Soldiers::getType() const
{
	return soldierInfo_.type;
}

// 计算攻击力
int Soldiers::computeAtk(bool isShowPower) const
{
	if (isShowPower)
	{
		return static_cast<int>( (soldierInfo_.power) * soldierInfo_.numberOfSoldiers);
	}
	else
	{
		return static_cast<int>( (soldierInfo_.power) * soldierInfo_.numberOfSoldiers
			* static_cast<float>(randomGenerator_->generator(soldierInfo_.shootingRate, 100)) / 100);
	}
}

// 计算防御力
int Soldiers::computeDefence(ATTACK_TYPE attackType) const
{
	// 部队防御力的计算使用新的公式, 不再考虑部队的人数
	switch (attackType)
	{
	case PHYSIC_ATTACK:
		// 得到防御系数
		return soldierInfo_.physicDefense;
	case MAGIC_ATTACK:
		// 得到防御系数
		return soldierInfo_.magicDefense ;
	case ADD_HP:
		return 0;
	default:
		return 0;	
	}

#if 0
	switch (attackType)
	{
	case PHYSIC_ATTACK:
		// 得到防御系数
		return soldierInfo_.physicDefense * soldierInfo_.numberOfSoldiers ;
	case MAGIC_ATTACK:
		// 得到防御系数
		return soldierInfo_.magicDefense * soldierInfo_.numberOfSoldiers ;
	case ADD_HP:
		return 0;
	default:
		return 0;	
	}
#endif

}

unsigned int Soldiers::getWeaponId() const
{
	return soldierInfo_.weaponId;
}

int Soldiers::getCurHurtHp() const
{
	MY_ASSERT(totalHurt_ >= 0, "totalHurt_ < 0");
	return totalHurt_;
}

int Soldiers::getNum() const
{
	MY_ASSERT(soldierInfo_.numberOfSoldiers >= 0, "soldierInfo_.numberOfSoldiers < 0");
	return soldierInfo_.numberOfSoldiers;
}

int Soldiers::getCurrentHp() const
{
	MY_ASSERT(totaoHp_ - totalHurt_ >= 0, "totaoHp_ - totalHurt_ < 0");

	int currerntHP = totaoHp_ - totalHurt_;

	if (currerntHP < 0)
	{
		return 0;
	}

	return currerntHP;
}

void Soldiers::resetHurt()
{
	totalHurt_ = 0;
}

void Soldiers::onHeroUpgradeLevel(int heroHp)
{
	hpOfDeath_ = static_cast<int>(soldierInfo_.hp + heroHp / COMPUTE_SOLDIER_BLOOD_MODULUS);
	totaoHp_ = hpOfDeath_ * soldierInfo_.numberOfSoldiers;
	MY_ASSERT(totaoHp_ >= totalHurt_, "totalHp_ < totalHurt_");
}

void Soldiers::addHp(double effectVal)
{
	hpOfDeath_ = static_cast<int>(hpOfDeath_ * (1 + effectVal));
	totaoHp_ = hpOfDeath_ * soldierInfo_.numberOfSoldiers;
	MY_ASSERT(totaoHp_ >= totalHurt_, "totalHp_ < totalHurt_");
}

void Soldiers::reduceHp(double effectVal)
{
	hpOfDeath_ = static_cast<int>(hpOfDeath_ * (1 - effectVal));

#ifdef _DEBUG
	if (0 == hpOfDeath_ )
	{
		assert(0 && "杀死一个士兵所需要的HP为0了");
	}
#endif

	totaoHp_ = hpOfDeath_ * soldierInfo_.numberOfSoldiers;
	//MY_ASSERT(totaoHp_ > totalHurt_, "totalHp_ < totalHurt_");
}

void Soldiers::addHpForEndurance(int endurance)
{
	hpOfDeath_ += endurance;
	totaoHp_ = hpOfDeath_ * soldierInfo_.numberOfSoldiers;
	MY_ASSERT(totaoHp_ >= totalHurt_, "totalHp_ < totalHurt_");
}

unsigned int Soldiers::resetSoldierNum(unsigned int soldierNum)
{
	if (soldierNum > ONE_HUNDRED)
	{
		soldierNum = ONE_HUNDRED;
	}

	totalHurt_ = 0;
	soldierInfo_.numberOfSoldiers = soldierNum;

	return soldierNum;
}

unsigned int Soldiers::addSoldierNum(unsigned int soldierNum)
{
	unsigned int addNum = 0;
	unsigned int totalSoldierNum = soldierInfo_.numberOfSoldiers + soldierNum;
	
	if (totalSoldierNum > ONE_HUNDRED)
	{
		totalSoldierNum = ONE_HUNDRED;
		addNum = ONE_HUNDRED - soldierInfo_.numberOfSoldiers;
	}
	else
	{
		addNum = soldierNum;
	}

	soldierInfo_.numberOfSoldiers = totalSoldierNum;

	return addNum;
}

unsigned int Soldiers::getTotalHp() const
{
	return totaoHp_;
}


COMPUTINGHURT_NAMESPACE_END
