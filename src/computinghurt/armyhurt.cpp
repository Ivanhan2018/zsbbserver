// @brief �����˺���
// @author ������

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

const static double MAX_DERATE_RATE = 0.9;             // ��������
const static double LANDFORM_AFFECT_MODULUS = 0.5;     // ����Ӱ��ϵ��

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

	// �������������
	int power = computeAtk();

	// �ͷŹ�������
	AttackReleaseInfo attackInfo = hero_.releaseAttackSkill(selfArmy_, soldiers_.getType(), armys.front());

	if (!attackInfo)
	{
		assert(0);
	}

	BreakOutStru breakoutStru;
	breakoutStru.effectId = INVALID_EFFECT;
	breakoutStru.effectVal = 0.0;

	bool isAttackAllCell = false;      // �Ƿ񹥻���������

	NothingingDefendStru nothingDefStru;
	nothingDefStru.additionalHurt = 0;				 // �����˺�ֵ
	nothingDefStru.isNothinkingDefend = false;	     // �Ƿ����ӷ���

	SKILL_TYPE skillId = attackInfo->skillId;
	EFFECT_TYPE effectId = attackInfo->effectType;

	// ����Ч������
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
				// forѭ��
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

		// �ͷŹ���ȫ��ļ���,��ȡ��Ҫȫ��Ĳ���
		if (isAttackAllCell)
		{
//#ifdef _DEBUG
//			if (armys.size() > 1)
//			{
//				assert(0 && "����ȫ��ķ�ʦ���ִ����˴���Ĺ���ȫ��ļ���");
//			}
//#endif
			IArmyPtr armyPtrTmp = armys.front();

			if (NULL == armyPtrTmp)
			{
#ifdef _DEBUG
				assert(0 && "������Ŀ�겿��ָ��Ϊ����");
#endif
			}

			const std::vector<POS_STRUCT> &posVect = armyPtrTmp->GetArmyPos();

#ifdef _DEBUG
			if (posVect.empty())
			{
				assert(0 && "��ȡ���ӵ�����ʧ��");
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
				assert(0 && "���û�ȡͬ��Ĳ��ӵĺ���ʧ��");
			}
		}

		// ����ȫ��
		if(isAttackAllCell || armys.size() > 1)
		{
			// forѭ��
			HarmInfos harminfos = processAttackAll(nothingDefStru, breakoutStru, armys, power, landformAffAtt);
			return newAttackInfo(selfArmy_, armys, skillId, soldiers_.getWeaponId(), harminfos);
		}
	}

	// û�д�������

	// Ĭ�Ϲ��������еĵ�һ��
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

	// ���ĵ����޵п�
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

	// ������ɱ����
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

	// ʹ�����޵п�
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

	// ���ӷ���
	if(nothingDefStru.isNothinkingDefend)
	{
		// ���й�����Ϊ���ӷ���
		if (0 == nothingDefStru.additionalHurt)
		{
			return processHurtStraight(power, attacker, landformDefAtt);
		}
		// ֻ�����ӵĸ����˺��������ӷ�����
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
	//		assert(0 && "����װ�����ж�Ӧ�ķ�������ֵ���ڵ���100��");
	//	}
	//#endif

	if (preserveVal >= 100)
	{
		preserveVal = 98;
	}

	// ����װ�����ж�Ӧ�ķ�������
	power = static_cast<int>(power * (1 - static_cast<float>(preserveVal) / 100));

	// ���ϵ��(���ݹ����ߵ������ͱ������ߵķ������õ�һ���������ļ���)
	float attackFactor = attacker->getFactorAccordingToArmor(selfArmy_);
	power = static_cast<int>(power * attackFactor);

	// ���ݹ������ͼ����Ӣ�۵ķ�����
	double defense = hero_.computeHeroDefence(attackType);

	defense = std::pow(defense, SQUARE_VAL) / 1000;

	//ljw 20091119
	//����ֵ��������������
	if (defense >= 1){
		//assert(0 && "����ָ����������1");
		defense = 0.98;
	}
	power =  static_cast<int>(power * (1 - defense));

	// �⻷�������ܵķ���ϵ��
	power = static_cast<int>(power * hero_.getDefenseFactor(selfArmy_->GetArmyOccupation(), attacker->GetArmyOccupation(), landForm));

	// ���μ��ܵķ���ϵ��
	power = static_cast<int>(power * hero_.getDefLandformFactor(selfArmy_->GetArmyOccupation(), attacker->GetArmyOccupation(), landForm));

	// �ͷŷ�������
	DefendReleaseInfo defendRelease = hero_.releaseDefenceSkill(selfArmy_, soldiers_.getType());

	EFFECT_TYPE effectId = defendRelease->effectType;
	SKILL_TYPE skillId = defendRelease->skillId;
	bool isReduceHurt = false;         // �����˺�
	double effectVal = 0.0;		   
	bool isNothingAttack = false;	   // ���ӹ���
	bool isHurt2AddHp = false;	       // �˺�תΪ��Ѫ

	// ����Ч������
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

		// ���ӹ���
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
				assert(0 && "�����˺��İٷֱ�ֵ����");
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

// �����ܵĹ�������ʿ������������x����x����������x����ϵ��x������������ߵõ���һ������
int ArmyHurt::computeAtk(IArmy* beAttacker) const
{
	MY_ASSERT(selfArmy_ != 0,"selfArmy_ == 0");
	if(selfArmy_ == 0)
		return 0;

	// �ò����ڲ�ͬ�ĵط��в�ͬ�Ĺ�������������ɭ�֣������������
	float factor = selfArmy_->getAttackFactor();
	double atk = soldiers_.computeAtk() * hero_.getAttackFactor(soldiers_.getType()) + hero_.computeATK();

	// ���ݹ����ߵ������ͱ������ߵķ������õ�һ���������ļ���
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

	// ������Ҳ��ӣ� ���ܲ���Ӣ��
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

	// ������Ҳ��ӣ� ���ܲ���Ӣ��
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

// ���������е�
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

// �����Ա����������е�
HarmInfos ArmyHurt::processSelfBlastAttackAll(const Armys &armysCenter, const Armys &armysInOneCircle)
{
	unsigned int armyHp = selfArmy_->GetCurrentHp();

	unsigned int centerAttPower = armyHp * 30;

	// �Ա����ӣ��Ա�ʱ��HP��300%�������˺�����СΪ2000��,ע���Խ������˺����ٳ�X150%
	if (centerAttPower < TWO_THOUSAND)
	{
		centerAttPower = TWO_THOUSAND;
	}

	// ��Ŀ�겿�����ڵĵз����ӣ�������1���ڣ����ܵ��Ա����ӣ��Ա�ʱ��HP��10������СΪ500���������˺�, ע���Խ������˺����ٳ�X150%
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
	nothingDefStru.additionalHurt = 0;				 // �����˺�ֵ
	nothingDefStru.isNothinkingDefend = false;	     // �Ƿ����ӷ���

	HarmInfo harminfo = beAttacker->beAttacked(nothingDefStru,  selfArmy_, atk);

	// ����ֵ
	harminfo->obtainExp = addExp(beAttacker, harminfo->DealNum, harminfo->LostHp) ;
	//����ֵ/10
	//add by ljw 2009 12 06
	harminfo->obtainExp /= 10;
	
	return harminfo;
}

// �����һ������
HarmInfo ArmyHurt::processAttackOne(const NothingingDefendStru &nothingDefStru, const BreakOutStru &breakoutStru, IArmy * beAttacker, int atk, unsigned int landformAffAtt)
{
	MY_ASSERT(beAttacker != 0, "army == 0");
	
	if(beAttacker == 0)
	{
		return HarmInfo();
	}

	if (selfArmy_->GetCountryID() == beAttacker->GetCountryID() && selfArmy_->GetArmyOccupation() != PRIEST)
	{
		LOG_ERROR(("error:���ܴ���ʱ�������ߵĲ���IDΪ��%d������Ӣ��IDΪ��%d������IDΪ��%d������ְҵ:%d�� ����Ӣ��ְҵ:%d;  �������Ĳ���IDΪ��%d������Ӣ��IDΪ��%d������IDΪ��%d������ְҵ:%d�� ����Ӣ��ְҵ:%d", 
			selfArmy_->GetArmyID(), selfArmy_->GetOfficerID(), selfArmy_->GetCountryID(), selfArmy_->GetArmyOccupation(), selfArmy_->getHeroOccupation(), 
			beAttacker->GetArmyID(), beAttacker->GetOfficerID(), beAttacker->GetCountryID(), beAttacker->GetArmyOccupation(), beAttacker->getHeroOccupation()));
	}

	LAND_FORM landForm = selfArmy_->getLandForm();
	SOLDIER_TYPE soldierType = soldiers_.getType();
	SOLDIER_TYPE enemySoldierType = beAttacker->GetArmyOccupation();

	// Ӣ�۹⻷�ࡢ�����༼��
	atk = static_cast<int>(atk * hero_.getAttackFactor(soldierType, enemySoldierType, landForm) 
		* hero_.getAttLandformFactor(soldierType, enemySoldierType, landForm));

	// �����༼�ܵı����ӳ�
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
		// ����ֵ
		harminfo->obtainExp = addExp(beAttacker, harminfo->DealNum, harminfo->LostHp) ;
		//����ֵ/10
		//add by ljw 2009 12 06
		harminfo->obtainExp /= 10;
	}

	return harminfo;


#if 0
	MY_ASSERT(beAttacker != 0, "army == 0");
	if(beAttacker == 0)
		return HarmInfo();

	HarmInfo harminfo = beAttacker->beAttacked(selfArmy_, atk, additionalHurt, isNothinkingDefend);
	// �Ӿ���
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

	if(attackType == ADD_HP) // ��ʦ��Ѫ
	{
		// ��ʦ���Լ���Ѫ�� ���Ӿ���ֵ
		if(beAttacker == selfArmy_)
			return 0;

		// ��ʦ������ �Ӿ����ǽ��˺�����10
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

// @brief ��õ�ǰ��Ѫ
int ArmyHurt::getCurHurtHp() const
{
	return soldiers_.getCurHurtHp();
}

// @brief ���Ӣ�����ӵľ���ֵ
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

	// ����װ�����ж�Ӧ�ķ�������
	float defendVal = static_cast<float>(preserveVal) / 100;

	// ���ݹ������ͼ����Ӣ�۵ķ�����
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
		assert(0 && "����HP�ļ����Ѿ�ѧϰ��");
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

