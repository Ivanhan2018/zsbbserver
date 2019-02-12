// @brief ʿ�������࣬���������˺�������������ͷ�����
// @author ������

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

// @brief �����˺����˺��ļ��㹫ʽΪ���ȼ����ɱ��һ��������Ҫ��HP�����
//        ��(P)��������ʿ����HP + Ӣ�۵�HP��/ ʿ������
//        Ȼ����������ı�����ʹ���ܵ��˺�ֵ(�ܵ��˺�ֵΪ�ϴ�ʣ����˺�ֵ������ε��˺�ֵ)������
//        �� D���˺�ֵ��T��/ P
//        ʣ�µ��˺�ֵΪ��T = T - D*P
// @param attackerSoldierType ��������ʿ�������ͣ��������ʦ�����Ǽ�Ѫ
// @parm hurt �˺�
// @param heroHp Ӣ�۵�HP
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

	// ɱ��һ��ʿ������Ҫ��HP
	if(hpOfDeath_ == 0)
	{
		hurt = 0;
		totalHurt_ = 0;
		return 0;
	}

	// ����ʦ����
	if(attackerSoldierType == PRIEST)
	{
		totalHurt_ -= hurt;
		if(totalHurt_ <= 0)
		{
			hurt += totalHurt_;
			// ��ʦ��Ѫ������ȥ����һ����ֵ
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
	// �����ǰHP<=�˺�ֵ����ֱ������
	if(curHp <= hurt)
	{
		hurt = curHp;
		// ֱ�Ӵ���������
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
// 	// ����ۻ��˺�ֵ�ȵ�ǰ��HP�󣬻���ȣ����Ӿ�ֱ������
// 	if(totaoHp_ - totalHurt_ <= 0)
// 	{
// 		// ����hurt
// 		hurt = totaoHp_;
// 		// ֱ�Ӵ���������
// 		int deaths = soldierInfo_.numberOfSoldiers;
// 		soldierInfo_.numberOfSoldiers = 0;
// 		totalHurt_ = 0;
// 		totaoHp_ = 0;
// 		return deaths;
// 	}

	// ��������
	const int deaths  = (totalHurt_ / 4 )/ hpOfDeath_;
	// �Ƿ�����������
	totalHurt_ -= deaths * hpOfDeath_;

	// ����HP
	totaoHp_ -= deaths * hpOfDeath_;
	
	soldierInfo_.numberOfSoldiers -= deaths;

	if(totaoHp_ <= 0 || 0 >= soldierInfo_.numberOfSoldiers)
	{
		totaoHp_ = 0;
		totalHurt_ = 0;
		soldierInfo_.numberOfSoldiers = 0;
		// �ص��� ֪ͨ����Army��������Ϊ0, ������
		return deaths;			 
	}

	
	return deaths;	
}

unsigned int Soldiers::suicide()
{
	// ֱ�Ӵ���������
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

// ���㹥����
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

// ���������
int Soldiers::computeDefence(ATTACK_TYPE attackType) const
{
	// ���ӷ������ļ���ʹ���µĹ�ʽ, ���ٿ��ǲ��ӵ�����
	switch (attackType)
	{
	case PHYSIC_ATTACK:
		// �õ�����ϵ��
		return soldierInfo_.physicDefense;
	case MAGIC_ATTACK:
		// �õ�����ϵ��
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
		// �õ�����ϵ��
		return soldierInfo_.physicDefense * soldierInfo_.numberOfSoldiers ;
	case MAGIC_ATTACK:
		// �õ�����ϵ��
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
		assert(0 && "ɱ��һ��ʿ������Ҫ��HPΪ0��");
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
