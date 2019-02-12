// @brief 英雄类，它负责释放技能，并输出自己的攻击力和防御力
// @author 张文龙

#include <cstdlib>
#include <ctime>
#include "hero.h"
#include "globalmacrodef.h"

namespace
{
	enum
	{
		ONE_HUNDRED_PROBABILITY = 100,
		TEN_NUM = 10,
		FIVE_NUM = 5,
		ZERO_PROBABILITY = 0,
		HERO_MAX_ATTRIBUTE_VAL = 800,
		TWO_THOUSAND = 2000,
		FOUR_THOUSAND = 4000,
		TWO_HUNDRED = 200
	};

	const  int basicBeCapturedRate = 5;	    // 基础被俘率
	const  int basicPlasterRate = 10;		// 基础重创率
	const  int basicForceAttackRate = 8;	// 基础暴击率
}


/*
namespace
{
	static bool isAttackSkill(SKILL_TYPE id)
	{
		return (id == Double_Hit || id == Kill_Fate || 
			id == Blood_Lost || id == Doomsday_Skill || id == Salvo_Skill);
	}

	static bool isDefenseSkill(SKILL_TYPE id)
	{
		return (id == All_Defend || id == Attack_ALL);
	}

	static bool isCatchSkill(SKILL_TYPE id)
	{
		return id == Chatch_Hero;
	}

	static bool isGlorySkill(SKILL_TYPE id)
	{
		return id >= Do_All_CAN_Skill && id <= Holy_Shield;
	}
} // namespace
*/

COMPUTINGHURT_NAMESPACE_BEGIN



Hero::Hero(const HeroInfo& heroinfo, IRandomGenerator* randomGenerator, Soldiers &soldiers)
:heroInfo_(heroinfo)
,attackSkills_()
,defenceSkills_()
,catchSkill_(0)
,plasterSkill_(0)
,glorySkills_()
,skillId2SkillGrade_()
,increaseExp_(0)
,generator_(randomGenerator)
{
	//ljw 20091119
	// 部队HP +＝耐×人数
	//不需要在此初始化士兵HP
	//soldiers.addHpForEndurance(heroinfo.heroAttribute.endurance);

	//ljw 20091119
	//检查英雄属性的合法性
	if(!checkHeroInfo(heroInfo_.heroAttribute)){
		assert(0 && "英雄属性非法");
	}
	SkillInfos::const_iterator itBeg = heroinfo.skillsinfo.begin();
	SkillInfos::const_iterator itEnd = heroinfo.skillsinfo.end();

	SKILL_TYPE skillId = SKILLID_INVALID;

	for (; itBeg!=itEnd; ++itBeg)
	{
		skillId = (itBeg->skillAndGradeStru).id;

		if (SKILLID_INVALID == skillId)
		{
#ifdef _DEBUG			
			assert(0 && "所学的技能类型非法");
#endif
			continue;
		}

		// 攻击技能
		if (FORCE_ATTACK_EFFECT == itBeg->effectType
			|| DEATH_WARRANT_EFFECT == itBeg->effectType
			|| BLOOD_WITHOUT_END_EFFECT == itBeg->effectType
			|| ATTACK_WHOLE_CELL_EFFECT == itBeg->effectType)
		{
			if (attackSkills_.end() != attackSkills_.find(skillId))
			{
#ifdef _DEBUG
				assert(0 && "此技能已学了");
#endif
				continue;
			}

			attackSkills_[skillId] = AttackSkill(*itBeg, randomGenerator);
			continue;
		}

		// 防御技能
		if (JIB_DEFENT_EFFECT == itBeg->effectType
			|| INVINCIBILITY_DEFENT_EFFECT == itBeg->effectType) 
		{
			if (defenceSkills_.end() != defenceSkills_.find(skillId))
			{
#ifdef _DEBUG
				assert(0 && "此技能已学了");
#endif
				continue;
			}

			defenceSkills_[skillId] = DefenceSkill(*itBeg, randomGenerator);
			continue;
		}

		// 光环技能
		if (ONE_HUNDRED_PROBABILITY == itBeg->probability)
		{
			if (glorySkills_.end() != glorySkills_.find(skillId))
			{
#ifdef _DEBUG
				assert(0 && "此技能已学了");
#endif
				continue;
			}

			if (ADD_HP_SKILL == skillId)
			{
				soldiers.addHp(itBeg->effectVal);
			}	

			glorySkills_[skillId] = GlorySkill(*itBeg, randomGenerator);
			continue;
		}

		// 武器专精技能
		if (ZERO_PROBABILITY == itBeg->probability && INVALID_EFFECT == itBeg->effectType)
		{
			const SkillAndGradeStru &skillAndGradeStru = itBeg->skillAndGradeStru;
			skillId2SkillGrade_[skillAndGradeStru.id] = skillAndGradeStru.level;
		}

		// 捕获技能
		if (NULL == catchSkill_ && CAPTURE_EFFECT == itBeg->effectType)
		{
			catchSkill_ = new CatchOrPlasterSkill(*itBeg, randomGenerator);
		}

		// 重创技能	
		if (NULL == plasterSkill_ && PLASTER_EFFECT == itBeg->effectType)
		{
			plasterSkill_ = new CatchOrPlasterSkill(*itBeg, randomGenerator);
		}
	}
}

Hero::~Hero()
{
	if (NULL != catchSkill_)
	{
		delete catchSkill_;
		catchSkill_ = NULL;
	}

	if (NULL != plasterSkill_)
	{
		delete plasterSkill_;
		plasterSkill_ = NULL;
	}

	increaseExp_ = 0;
	generator_ = 0;
}

AttackReleaseInfo Hero::releaseAttackSkill(const IArmy* selfArmy, const SOLDIER_TYPE& armyType, IArmy *enemyArmy)
{
	std::vector<AttackSkill *> attackSkills;

	AttackSkill attackSkillTmp;

	if (BUILD != enemyArmy->GetArmyType())
	{
		int forceAttackRate = basicForceAttackRate;
		float heroforceAttRate = 0.0;

		const HeroAttribute * enemyHeroAttri = enemyArmy->getHeroAttribute();

		if (NULL == enemyHeroAttri)
		{
			assert(0 && "敌军的英雄属性指针为空了");
		}

		HeroType enemyHeroType = enemyArmy->getHeroType();

		switch(heroInfo_.type)
		{
		case HEROTYPE_POWER:
			heroforceAttRate = static_cast<float>(heroInfo_.heroAttribute.power) / FOUR_THOUSAND;
			break;
		case HEROTYPE_AGILE:
			heroforceAttRate = static_cast<float>(heroInfo_.heroAttribute.agile) / FOUR_THOUSAND;
			break;
		case HEROTYPE_INTELLIGENCE:
			heroforceAttRate = static_cast<float>(heroInfo_.heroAttribute.intelligence) / FOUR_THOUSAND;
			break;
		default:
			break;
		}

		switch(enemyHeroType)
		{
		case HEROTYPE_POWER:
			heroforceAttRate -= static_cast<float>(heroInfo_.heroAttribute.power) / FOUR_THOUSAND;
			break;
		case HEROTYPE_AGILE:
			heroforceAttRate -= static_cast<float>(heroInfo_.heroAttribute.agile) / FOUR_THOUSAND;
			break;
		case HEROTYPE_INTELLIGENCE:
			heroforceAttRate -= static_cast<float>(heroInfo_.heroAttribute.intelligence) / FOUR_THOUSAND;
			break;
		default:
			break;
		}

		heroforceAttRate = forceAttackRate + forceAttackRate * heroforceAttRate;

		forceAttackRate =  static_cast<int>(heroforceAttRate + forceAttackRate * static_cast<float>(heroInfo_.heroAttribute.cruelAttackRate - enemyHeroAttri->cruelAttackRate) / ONE_HUNDRED_PROBABILITY);

		if (forceAttackRate > ONE_HUNDRED_PROBABILITY)
		{
#ifdef _DEBUG
			assert(0 && "固有暴击率大于100了");
#endif
		}

		if (forceAttackRate < 0)
		{
#ifdef _DEBUG
			assert(0 && "固有暴击率小于0了");
#endif
		}

		SkillAndGradeStru skillAndGradeStru;
		skillAndGradeStru.id = Double_Hit;
		skillAndGradeStru.level = 1;

		SkillInfo skillInfo;
		skillInfo.skillAndGradeStru = skillAndGradeStru;
		skillInfo.effectType = FORCE_ATTACK_EFFECT;
		skillInfo.effectVal = 1;
		skillInfo.landForm = LAND_INVALID;
		skillInfo.objectOccupation.empty();
		skillInfo.objectOccupation.insert(SOLDIERTYPE_NONE);
		skillInfo.otherOccupation.clear();
		skillInfo.otherOccupation.insert(SOLDIERTYPE_NONE);
		skillInfo.probability = forceAttackRate;

		attackSkillTmp.upgrade(skillInfo);
		attackSkillTmp.updateRandomGenerator(generator_);

		if (attackSkillTmp.isCanRelease())
		{
			attackSkills.push_back(&attackSkillTmp);
		}
	}

	SkillId2AttackSkill::iterator itAttackSkillBeg = attackSkills_.begin();
	SkillId2AttackSkill::iterator itAttackSkillEnd = attackSkills_.end();
	
	for (; itAttackSkillBeg!=itAttackSkillEnd; ++itAttackSkillBeg)
	{
		if (itAttackSkillBeg->second.isCanRelease())
		{
			attackSkills.push_back(&(itAttackSkillBeg->second));
		}
	}

	unsigned int skillVectSize = static_cast<unsigned int>(attackSkills.size());

	if (0 == skillVectSize)
	{
		std::set<SOLDIER_TYPE>  otherOccupation;
		otherOccupation.insert(SOLDIERTYPE_NONE);
		return AttackSkill::newAttackReleaseInfo(SKILLID_INVALID, INVALID_EFFECT, otherOccupation, 0.0);
	}

	srand(static_cast<unsigned int>(time(0)));	    // 根据系统时间设置随机数种子
	int index = rand() % skillVectSize;				// 取得区间[0,N)的整数

	AttackReleaseInfo info = attackSkills[index]->release(selfArmy, armyType);

	if( info->skillId != SKILLID_INVALID)
	{
		return info;
	}

	std::set<SOLDIER_TYPE>  otherOccupation;
	otherOccupation.insert(SOLDIERTYPE_NONE);

	return AttackSkill::newAttackReleaseInfo(SKILLID_INVALID, INVALID_EFFECT, otherOccupation, 0.0);
}

DefendReleaseInfo Hero::releaseDefenceSkill(const IArmy* selfArmy, const SOLDIER_TYPE& armyType)
{
	SkillId2DefenceSkill::iterator itDefSkillBeg = defenceSkills_.begin();
	SkillId2DefenceSkill::iterator itDefSkillEnd = defenceSkills_.end();

	std::vector<DefenceSkill *> defendSkills;

	for (; itDefSkillBeg!=itDefSkillEnd; ++itDefSkillBeg)
	{
		if (itDefSkillBeg->second.isCanRelease())
		{
			defendSkills.push_back(&(itDefSkillBeg->second));
		}
	}

	unsigned int skillVectSize = static_cast<unsigned int>(defendSkills.size());

	if (0 == skillVectSize)
	{
		std::set<SOLDIER_TYPE>  otherOccupation;
		otherOccupation.insert(SOLDIERTYPE_NONE);
		return DefenceSkill::newDefendReleaseInfo(SKILLID_INVALID, INVALID_EFFECT, otherOccupation, 0.0);
	}

	srand(static_cast<unsigned int>(time(0)));	    // 根据系统时间设置随机数种子
	unsigned int index = rand() % skillVectSize;				// 取得区间[0,N)的整数

	AttackReleaseInfo info = defendSkills[index]->release(selfArmy, armyType);

	if( info->skillId != SKILLID_INVALID)
	{
		return info;
	}

	std::set<SOLDIER_TYPE>  otherOccupation;
	otherOccupation.insert(SOLDIERTYPE_NONE);

	return DefenceSkill::newDefendReleaseInfo(SKILLID_INVALID, INVALID_EFFECT, otherOccupation, 0.0);
}

int Hero::computeATK() const
{
	int ATK = 0;

	switch(heroInfo_.type)
	{
	case HEROTYPE_POWER:
		ATK = heroInfo_.heroAttribute.physicAttack + heroInfo_.heroAttribute.power * TEN_NUM;
		break;
	case HEROTYPE_AGILE:
		ATK = heroInfo_.heroAttribute.physicAttack + heroInfo_.heroAttribute.agile * TEN_NUM;
		break;
	case HEROTYPE_INTELLIGENCE:
		ATK = heroInfo_.heroAttribute.magicAttack + heroInfo_.heroAttribute.intelligence * FIVE_NUM;
		break;
	}

	return ATK;
}

int Hero::computeHeroDefence(ATTACK_TYPE attackType) const
{
	int ATK = 0;

	switch(attackType)
	{
	case HEROTYPE_POWER:
	case HEROTYPE_AGILE:
		ATK = heroInfo_.heroAttribute.physicDef + heroInfo_.heroAttribute.power * TEN_NUM;
		break;
	case HEROTYPE_INTELLIGENCE:
		ATK = heroInfo_.heroAttribute.magicDef + heroInfo_.heroAttribute.intelligence * TEN_NUM;
		break;
	}
	return ATK;
}

bool Hero::catchHero(IArmy* army, const SOLDIER_TYPE armyType)
{
	if (BUILD == army->GetArmyType())
	{
		return false;
	}

	const HeroAttribute * heroAttriPtr = army->getHeroAttribute();

	if (NULL == heroAttriPtr)
	{
#ifdef _DEBUG
		assert(0 && "英雄属性的指针为空了");
#endif
		return false;
	}

	int catchHeroRate = static_cast<int>(basicBeCapturedRate + basicBeCapturedRate * static_cast<float>( heroInfo_.heroAttribute.agile -  heroAttriPtr->agile ) / TWO_THOUSAND
		+ basicBeCapturedRate * static_cast<float>( heroInfo_.heroAttribute.ememyCapturedRate - heroAttriPtr->selfCapturedRate ) / ONE_HUNDRED_PROBABILITY);

	if (catchHeroRate >= ONE_HUNDRED_PROBABILITY)
	{
#ifdef _DEBUG
		assert(0 && "捕获概率大于等于100了");
#endif	
		return false;
	}

	if (catchHeroRate < 0)
	{
#ifdef _DEBUG
		assert(0 && "捕获概率小于0了");
#endif	
		return false;
	}


	if (NULL == catchSkill_ 
		|| Chatch_Hero != catchSkill_->getSkillType())
	{
		srand(static_cast<unsigned int>(time(0)));						// 根据系统时间设置随机数种子
		unsigned int randNum = rand() % ONE_HUNDRED_PROBABILITY;		// 取得区间[0,N)的整数

		if (randNum <= static_cast<unsigned int>(catchHeroRate))
		{
			return true;
		}

		return false;
	}

	return catchSkill_->release(army, armyType, catchHeroRate, basicBeCapturedRate);
}

bool Hero::canPlasterHero(IArmy* army, const SOLDIER_TYPE armyType)
{
	const HeroAttribute * heroAttriPtr = army->getHeroAttribute();

	if (NULL == heroAttriPtr)
	{
#ifdef _DEBUG
		assert(0 && "英雄属性的指针为空了");
#endif
		return false;
	}

	int plasterHeroRate = static_cast<int>(basicPlasterRate + basicPlasterRate * (static_cast<float>( heroInfo_.heroAttribute.agile - TWO_HUNDRED ) / TWO_THOUSAND - static_cast<float>(heroAttriPtr->agile- TWO_HUNDRED ) / TWO_THOUSAND)
		+ basicPlasterRate * static_cast<float>( heroInfo_.heroAttribute.enemyBeinjuredRate - heroAttriPtr->selfBeinjuredRate ) / ONE_HUNDRED_PROBABILITY);

	if (plasterHeroRate >= ONE_HUNDRED_PROBABILITY)
	{
#ifdef _DEBUG
		assert(0 && "重创概率大于等于100了");
#endif	
		return true;
	}

	if (plasterHeroRate < 0)
	{
#ifdef _DEBUG
		assert(0 && "重创概率小于0了");
#endif	
		return true;
	}

	if (NULL == plasterSkill_
		|| PLASTER != plasterSkill_->getSkillType())
	{
		srand(static_cast<unsigned int>(time(0)));						// 根据系统时间设置随机数种子
		unsigned int randNum = rand() % ONE_HUNDRED_PROBABILITY;		// 取得区间[0,N)的整数

		if (randNum <= static_cast<unsigned int>(plasterHeroRate))
		{
			return true;
		}

		return false;
	}

	return plasterSkill_->release(army, armyType, plasterHeroRate, basicPlasterRate);
}

double Hero::getAttackFactor(SOLDIER_TYPE armyType, SOLDIER_TYPE enemyType, LAND_FORM landForm) const
{
	double factor = 1.0;

	SkillId2GlorySkill::const_iterator itBeg = glorySkills_.begin();
	SkillId2GlorySkill::const_iterator itEnd = glorySkills_.end();

	for (; itBeg!=itEnd; ++itBeg)
	{
		const GlorySkill &glorySkill = itBeg->second;
		if (PERCENT_ATTACK_EFFECT == glorySkill.getEffectType())
		{
			factor *= (1 + glorySkill.getAttackFactor(armyType, enemyType, landForm));
		}
	}

	return factor;
}

double Hero::getDefenseFactor(SOLDIER_TYPE armyType, SOLDIER_TYPE enemyType, LAND_FORM landForm) const
{
	double factor = 1.0;

	SkillId2GlorySkill::const_iterator itBeg = glorySkills_.begin();
	SkillId2GlorySkill::const_iterator itEnd = glorySkills_.end();

	for (; itBeg!=itEnd; ++itBeg)
	{
		const GlorySkill &glorySkill = itBeg->second;

		if (PERCENT_DEFEND_EFFECT == glorySkill.getEffectType())
		{
#ifdef _DEBUG
			if (glorySkill.getDefenseFactor(armyType, enemyType, landForm) > 1)
			{
				assert(0 && "防御效果值大于1了");
			}
#endif
			factor *= (1 - glorySkill.getDefenseFactor(armyType, enemyType, landForm));
		}
	}

	return factor;
}
//ljw 20091119
//英雄hp 分为两部分一部分来自属性 一部分来自装备
int Hero::getHp() const
{
	return heroInfo_.heroAttribute.endurance + heroInfo_.heroAttribute.HP;
}

void Hero::addExp(int exp)
{
	MY_ASSERT(exp >= 0, "exp < 0");
	increaseExp_ += exp;
}

int Hero::getIncreaseExp() const
{
	// 20090925,新的经验值要除以10
	return increaseExp_ / 10;
}

//ljw 20091119
//英雄属性改变需要重新计算部队信息
void Hero::upgradeLevel(const HeroAttribute& heroAttribute)
{
	heroInfo_.heroAttribute = heroAttribute;
	//ljw 20091119
	//检查英雄属性的合法性
	if(!checkHeroInfo(heroInfo_.heroAttribute)){
		assert(0 && "英雄属性非法");
	}
}

/*
template <class T>
static void upgradeOrNewSkill(const SkillInfo& skillInfo, IRandomGenerator* generator, T& t)
{
	T::iterator it = t.begin();
	for (; it != t.end(); ++it)
	{
		if((*it).getSkillType() == skillInfo.id)
		{
			(*it).upgrade(skillInfo);
			return;
		}
	}

	// 插入
	t.push_back(T::value_type(skillInfo, generator));
}
*/

void Hero::upgradeOrLearnedSkill(const SkillInfo& skillInfo)
{
	SKILL_TYPE skillID = skillInfo.skillAndGradeStru.id;

	if (SKILLID_INVALID == skillID)
	{
#ifdef _DEBUG
		assert(0 && "技能类型非法");
#endif
		return;
	}

	// 攻击技能
	if (FORCE_ATTACK_EFFECT == skillInfo.effectType
		|| DEATH_WARRANT_EFFECT == skillInfo.effectType
		|| BLOOD_WITHOUT_END_EFFECT == skillInfo.effectType
		|| ATTACK_WHOLE_CELL_EFFECT == skillInfo.effectType)
	{
		SkillId2AttackSkill::iterator itAttackSkillFind = attackSkills_.find(skillID);
		
		if (attackSkills_.end() != itAttackSkillFind)
		{
			itAttackSkillFind->second.upgrade(skillInfo);
			return;
		}
		
		attackSkills_[skillID] = AttackSkill(skillInfo, generator_);
		return;
	}

	// 防御技能
	if (JIB_DEFENT_EFFECT == skillInfo.effectType
		|| INVINCIBILITY_DEFENT_EFFECT == skillInfo.effectType) 
	{

		SkillId2DefenceSkill::iterator itDefSkillFind = defenceSkills_.find(skillID);

		if (defenceSkills_.end() != itDefSkillFind)
		{
			itDefSkillFind->second.upgrade(skillInfo);
			return;
		}

		defenceSkills_[skillID] = DefenceSkill(skillInfo, generator_);
		return;
	}

	// 光环技能
	if (ONE_HUNDRED_PROBABILITY == skillInfo.probability)
	{
		SkillId2GlorySkill::iterator itGlorySkillFind = glorySkills_.find(skillID);

		if (glorySkills_.end() != itGlorySkillFind)
		{
			itGlorySkillFind->second.upgrade(skillInfo);
			return;
		}
		
		glorySkills_[skillID] = GlorySkill(skillInfo, generator_);
		return;
	}

	// 武器专精技能
	if (ZERO_PROBABILITY == skillInfo.probability && INVALID_EFFECT == skillInfo.effectType)
	{
		const SkillAndGradeStru &skillAndGradeStru = skillInfo.skillAndGradeStru;

		SkillId2SkillGrade::iterator itFind = skillId2SkillGrade_.find(skillID);

		if (skillId2SkillGrade_.end() != itFind)
		{
			itFind->second = skillAndGradeStru.level;
			return;
		}

		skillId2SkillGrade_[skillID] = skillAndGradeStru.level;

		return;
	}

	// 捕获技能
	if (CAPTURE_EFFECT == skillInfo.effectType)
	{
		if (NULL != catchSkill_)
		{
			if (catchSkill_->getSkillType() == skillInfo.skillAndGradeStru.id)
			{
				catchSkill_->upgrade(skillInfo);
				return;
			}
			else
			{
//#ifdef _DEBUG
//				assert(0 && "捕获或重创技能只能学一个");
//#endif
				return;
			}
		}

		catchSkill_ = new CatchOrPlasterSkill(skillInfo, generator_);

		return;
	}
	
	// 重创技能
	if (PLASTER_EFFECT == skillInfo.effectType)
	{
		if (NULL != plasterSkill_)
		{
			if (plasterSkill_->getSkillType() == skillInfo.skillAndGradeStru.id)
			{
				plasterSkill_->upgrade(skillInfo);
				return;
			}
			else
			{
				//#ifdef _DEBUG
				//				assert(0 && "捕获或重创技能只能学一个");
				//#endif
				return;
			}
		}

		plasterSkill_ = new CatchOrPlasterSkill(skillInfo, generator_);
	}

	return;
}

bool Hero::abandonSkill(const SkillAndGradeStru& skillAndGradeStru)
{
	SKILL_TYPE skillID = skillAndGradeStru.id;

	if (SKILLID_INVALID == skillID)
	{
#ifdef _DEBUG
		assert(0 && "技能类型非法");
#endif
		return false;
	}

	// 在捕获技能中寻找
	if (NULL != catchSkill_ 
		&& catchSkill_->getSkillType() == skillID
		&& catchSkill_->getSkillGrade() == skillAndGradeStru.level)
	{

		delete catchSkill_;
		catchSkill_ = NULL;
		return true;
	}

	// 在重创技能中寻找
	if (NULL != plasterSkill_ 
		&& plasterSkill_->getSkillType() == skillID
		&& plasterSkill_->getSkillGrade() == skillAndGradeStru.level)
	{

		delete plasterSkill_;
		plasterSkill_ = NULL;
		return true;
	}

	// 在攻击技能中寻找
	SkillId2AttackSkill::iterator itAttackSkillFind = attackSkills_.find(skillID);

	if (attackSkills_.end() != itAttackSkillFind)
	{
		if (itAttackSkillFind->second.getSkillGrade() == skillAndGradeStru.level)
		{
			attackSkills_.erase(itAttackSkillFind);
			return true;
		}
	}

	// 在防御技能中寻找
	SkillId2DefenceSkill::iterator itDefSkillFind = defenceSkills_.find(skillID);

	if (defenceSkills_.end() != itDefSkillFind)
	{
		if (itDefSkillFind->second.getSkillGrade() == skillAndGradeStru.level)
		{
			defenceSkills_.erase(itDefSkillFind);
			return true;
		}
	}

	// 在武器专精中寻找
	SkillId2SkillGrade::iterator itFind = skillId2SkillGrade_.find(skillID);

	if (itFind != skillId2SkillGrade_.end())
	{
		if (itFind->second == skillAndGradeStru.level)
		{		
			skillId2SkillGrade_.erase(itFind);
			return true;
		}
	}

	// 在光环技能中寻找
	SkillId2GlorySkill::iterator itGlorySkillFind = glorySkills_.find(skillID);

	if (glorySkills_.end() != itGlorySkillFind)
	{
		if (itGlorySkillFind->second.getSkillGrade() == skillAndGradeStru.level)
		{
			glorySkills_.erase(itGlorySkillFind);
			return true;
		}
	}

	return false;
}

double Hero::getAttLandformFactor(SOLDIER_TYPE armyType, SOLDIER_TYPE enemyType, LAND_FORM landForm) const
{	
	double factor = 1.0;

	SkillId2GlorySkill::const_iterator itBeg = glorySkills_.begin();
	SkillId2GlorySkill::const_iterator itEnd = glorySkills_.end();

	for (; itBeg!=itEnd; ++itBeg)
	{
		const GlorySkill &glorySkill = itBeg->second;

		if (PERCENT_ATT_DEF_EFFECT != itBeg->second.getEffectType())
		{
			continue;
		}

#ifdef _DEBUG
		if (glorySkill.getAttDefFactor(armyType, enemyType, landForm) > 1)
		{
			assert(0 && "防御效果值大于1了");
		}
#endif
		factor *= (1 + glorySkill.getAttDefFactor(armyType, enemyType, landForm));
	}

	return factor;
}

double Hero::getDefLandformFactor(SOLDIER_TYPE armyType, SOLDIER_TYPE enemyType, LAND_FORM landForm) const
{	
	double factor = 1.0;

	SkillId2GlorySkill::const_iterator itBeg = glorySkills_.begin();
	SkillId2GlorySkill::const_iterator itEnd = glorySkills_.end();

	for (; itBeg!=itEnd; ++itBeg)
	{
		const GlorySkill &glorySkill = itBeg->second;

		if (PERCENT_ATT_DEF_EFFECT != itBeg->second.getEffectType())
		{
			continue;
		}

#ifdef _DEBUG
		if (glorySkill.getAttDefFactor(armyType, enemyType, landForm) > 1)
		{
			assert(0 && "防御效果值大于1了");
		}
#endif
		factor *= (1 - glorySkill.getAttDefFactor(armyType, enemyType, landForm));
	}

	return factor;
}

bool Hero::isLearnedSpecifiGradeSkill(const SkillAndGradeStru & skillAndGradeStru)
{
	SKILL_TYPE skillID = skillAndGradeStru.id;

	if (SKILLID_INVALID == skillID)
	{
#ifdef _DEBUG
		assert(0 && "技能类型非法");
#endif
		return false;
	}


	// 在捕获技能中寻找
	if (NULL != catchSkill_ 
		&& catchSkill_->getSkillType() == skillID
		&& catchSkill_->getSkillGrade() == skillAndGradeStru.level)
	{
		return true;
	}


	// 在重创技能中寻找
	if (NULL != plasterSkill_ 
		&& plasterSkill_->getSkillType() == skillID
		&& plasterSkill_->getSkillGrade() == skillAndGradeStru.level)
	{
		return true;
	}

	// 在攻击技能中寻找
	SkillId2AttackSkill::iterator itAttackSkillFind = attackSkills_.find(skillID);

	if (attackSkills_.end() != itAttackSkillFind)
	{
		if (itAttackSkillFind->second.getSkillGrade() == skillAndGradeStru.level)
		{
			return true;
		}
	}

	// 在防御技能中寻找
	SkillId2DefenceSkill::iterator itDefSkillFind = defenceSkills_.find(skillID);

	if (defenceSkills_.end() != itDefSkillFind)
	{
		if (itDefSkillFind->second.getSkillGrade() == skillAndGradeStru.level)
		{
			return true;
		}
	}

	// 在光环技能中寻找
	SkillId2GlorySkill::iterator itGlorySkillFind = glorySkills_.find(skillID);

	if (glorySkills_.end() != itGlorySkillFind)
	{
		if (itGlorySkillFind->second.getSkillGrade() == skillAndGradeStru.level)
		{
			return true;
		}
	}

	// 武器专精
	SkillId2SkillGrade::iterator itFind = skillId2SkillGrade_.find(skillID);

	if (itFind != skillId2SkillGrade_.end())
	{
		if (itFind->second == skillAndGradeStru.level)
		{
			return true;
		}
	}

	return false;
}

bool Hero::isLearnedAddHpSkill(const SkillAndGradeStru & skillAndGradeStru, double &effectVal)
{
	SKILL_TYPE skillID = skillAndGradeStru.id;

	// 在光环技能中寻找
	SkillId2GlorySkill::iterator itGlorySkillFind = glorySkills_.find(skillID);

	if (glorySkills_.end() != itGlorySkillFind)
	{
		if (itGlorySkillFind->second.getSkillGrade() == skillAndGradeStru.level)
		{
			effectVal = itGlorySkillFind->second.getSkillEffectVal();
			return true;
		}
	}

	return false;
}
//ljw 20091119
//校验英雄属性信息
bool Hero::checkHeroInfo(HeroAttribute & heroInfo){
	bool isValidValue = true;
	if(heroInfo.agile > HERO_MAX_ATTRIBUTE_VAL){
		isValidValue = false;
		heroInfo.agile = HERO_MAX_ATTRIBUTE_VAL;
	}
	if(heroInfo.endurance > HERO_MAX_ATTRIBUTE_VAL){
		isValidValue = false;
		heroInfo.endurance = HERO_MAX_ATTRIBUTE_VAL;
	}
	if(heroInfo.intelligence > HERO_MAX_ATTRIBUTE_VAL){
		isValidValue = false;
		heroInfo.intelligence = HERO_MAX_ATTRIBUTE_VAL;
	}
	if(heroInfo.power > HERO_MAX_ATTRIBUTE_VAL){
		isValidValue = false;
		heroInfo.power = HERO_MAX_ATTRIBUTE_VAL;
	}
	return isValidValue;
}

COMPUTINGHURT_NAMESPACE_END
