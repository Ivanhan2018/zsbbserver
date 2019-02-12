// @brief 技能系统，它初步分为：攻击技能，防御技能，光环技能，捕获技能
// @author 张文龙

#include "skills.h"


namespace
{
	static bool isInRange(computinghurt::IRandomGenerator* randomGenerator, unsigned int probability)
	{
		MY_ASSERT(randomGenerator != 0, "randomGenerator == 0");
		if (randomGenerator == 0 )
			return false;

		int random = randomGenerator->generator(1, 100);
		return static_cast<unsigned int>(random) <= probability;
	}

} // namespace

COMPUTINGHURT_NAMESPACE_BEGIN


AttackSkill::AttackSkill(const SkillInfo& skillInfo, IRandomGenerator* randomGenerator)
:skillInfo_(skillInfo), randomGenerator_(randomGenerator)
{

}

AttackSkill::AttackSkill()
{	
	skillInfo_.skillAndGradeStru.id = SKILLID_INVALID;
	skillInfo_.effectType = INVALID_EFFECT;
}

AttackReleaseInfo AttackSkill::newAttackReleaseInfo(SKILL_TYPE id, EFFECT_TYPE effectType, const std::set<SOLDIER_TYPE> & otherOccupation, double effectVal) 
{
	AttackReleaseInfo attackInfo(new AttackOrDefendReleaseInfoValue);
	attackInfo->skillId = id;
	attackInfo->effectType = effectType;
	attackInfo->otherOccupation = otherOccupation;
	attackInfo->effectVal = effectVal;

	return attackInfo;
}

//, double& factor, bool& isNothinking, bool& isAttackAll, int& additionalHurt
// @param hurt 伤害值
// @param isNothinking 是否无视防御
// @param isAttackAll 是否攻击全格
// @return 技能是否成功, 返回-1, 不成功，即未触发技能
AttackReleaseInfo AttackSkill::release(const IArmy* selfArmy, const SOLDIER_TYPE armyType) const
{
	LAND_FORM landForm = selfArmy->getLandForm();

	std::set<SOLDIER_TYPE>::const_iterator itFindZero = skillInfo_.objectOccupation.find(SOLDIERTYPE_NONE);
	std::set<SOLDIER_TYPE>::const_iterator it = skillInfo_.objectOccupation.find(armyType);

	if((itFindZero != skillInfo_.objectOccupation.end() || it != skillInfo_.objectOccupation.end())
		&& (landForm == skillInfo_.landForm || LAND_INVALID == skillInfo_.landForm))
	{
		return newAttackReleaseInfo(skillInfo_.skillAndGradeStru.id, skillInfo_.effectType, skillInfo_.otherOccupation, skillInfo_.effectVal);
	}

	std::set<SOLDIER_TYPE>  otherOccupation;
	otherOccupation.insert(SOLDIERTYPE_NONE);

	return newAttackReleaseInfo(SKILLID_INVALID, INVALID_EFFECT, otherOccupation, 0.0);
}

bool AttackSkill::isCanRelease() const
{
	return isInRange(randomGenerator_, this->skillInfo_.probability);
}

SKILL_TYPE AttackSkill::getSkillType() const
{
	return skillInfo_.skillAndGradeStru.id;
}

void AttackSkill::upgrade(const SkillInfo& skillInfo)
{
	// 更新
	//MY_ASSERT(skillInfo_.skillAndGradeStru.level < skillInfo.skillAndGradeStru.level, "(*it).level >= skillInfo.level");
	skillInfo_.probability = skillInfo.probability;
	skillInfo_.skillAndGradeStru.level = skillInfo.skillAndGradeStru.level;
}

//////////////////////////////////////////////////////////////////////////

DefenceSkill::DefenceSkill(const SkillInfo& skillinfo, IRandomGenerator* randomGenerator)
:skillInfo_(skillinfo),randomGenerator_(randomGenerator)
{

}

DefenceSkill::DefenceSkill()
{
	skillInfo_.skillAndGradeStru.id = SKILLID_INVALID;
	skillInfo_.effectType = INVALID_EFFECT;
}

// @param defense 防御值
// @param isNothinking 是否无视防御
// @return 技能是否成功, 返回-1, 不成功，即未触发技能
DefendReleaseInfo DefenceSkill::release(const IArmy* selfArmy, const SOLDIER_TYPE& armyType) const
{
	LAND_FORM landForm = selfArmy->getLandForm();

	std::set<SOLDIER_TYPE>::const_iterator itFindZero = skillInfo_.objectOccupation.find(SOLDIERTYPE_NONE);
	std::set<SOLDIER_TYPE>::const_iterator it = skillInfo_.objectOccupation.find(armyType);

	if((itFindZero != skillInfo_.objectOccupation.end() || it != skillInfo_.objectOccupation.end())
		&& (landForm == skillInfo_.landForm || LAND_INVALID == skillInfo_.landForm))
	{
		return newDefendReleaseInfo(skillInfo_.skillAndGradeStru.id, skillInfo_.effectType, skillInfo_.otherOccupation, skillInfo_.effectVal);
	}

	std::set<SOLDIER_TYPE>  otherOccupation;
	otherOccupation.insert(SOLDIERTYPE_NONE);

	return newDefendReleaseInfo(SKILLID_INVALID, INVALID_EFFECT, otherOccupation, 0.0);
}

bool DefenceSkill::isCanRelease() const
{
	return isInRange(randomGenerator_, this->skillInfo_.probability);
}

SKILL_TYPE DefenceSkill::getSkillType() const
{
	return skillInfo_.skillAndGradeStru.id;
}

void DefenceSkill::upgrade(const SkillInfo& skillInfo)
{
	// 更新
	MY_ASSERT(skillInfo_.skillAndGradeStru.level < skillInfo.skillAndGradeStru.level, "(*it).level >= skillInfo.level");
	skillInfo_.probability = skillInfo.probability;
	skillInfo_.skillAndGradeStru.level = skillInfo.skillAndGradeStru.level;
}

DefendReleaseInfo DefenceSkill::newDefendReleaseInfo(SKILL_TYPE id, EFFECT_TYPE effectType, const std::set<SOLDIER_TYPE> &otherOccupation, double effectVal)
{
	DefendReleaseInfo defendInfo(new AttackOrDefendReleaseInfoValue);
	defendInfo->skillId = id;
	defendInfo->effectType = effectType;
	defendInfo->otherOccupation = otherOccupation;
	defendInfo->effectVal = effectVal;

	return defendInfo;
}


//////////////////////////////////////////////////////////////////////////

CatchOrPlasterSkill::CatchOrPlasterSkill(const SkillInfo& skillinfo, IRandomGenerator* randomGenerator)
:skillInfo_(skillinfo),randomGenerator_(randomGenerator)
{

}

// @return 技能是否成功
bool CatchOrPlasterSkill::release(const IArmy* selfArmy, const SOLDIER_TYPE armyType, int catchHeroRate, int basicBeCapturedRate) const
{
	if(Chatch_Hero == skillInfo_.skillAndGradeStru.id 
		|| PLASTER == skillInfo_.skillAndGradeStru.id)
	{
		LAND_FORM landForm = selfArmy->getLandForm();

		std::set<SOLDIER_TYPE>::const_iterator itFindZero = skillInfo_.objectOccupation.find(SOLDIERTYPE_NONE);

		std::set<SOLDIER_TYPE>::const_iterator it = skillInfo_.objectOccupation.find(armyType);

		if((itFindZero != skillInfo_.objectOccupation.end() || it != skillInfo_.objectOccupation.end())
			&& (landForm == skillInfo_.landForm || LAND_INVALID == skillInfo_.landForm))
		{
			if (skillInfo_.probability / 100  * basicBeCapturedRate + catchHeroRate >= 100)
			{
#ifdef _DEBUG
				assert(0 && "捕获概率大于等于100了");
#endif	
				return true;
			}

			return isInRange(randomGenerator_, skillInfo_.probability / 100  * basicBeCapturedRate + catchHeroRate);
		}
	}

	return false;
}

bool CatchOrPlasterSkill::canRelease() const
{
	return isInRange(randomGenerator_, this->skillInfo_.probability);
}

SKILL_TYPE CatchOrPlasterSkill::getSkillType() const
{
	return skillInfo_.skillAndGradeStru.id;
}

void CatchOrPlasterSkill::upgrade(const SkillInfo& skillInfo)
{
	const SkillAndGradeStru &skillAndGradeStru = skillInfo.skillAndGradeStru;
	SkillAndGradeStru &skillAndGradeStru_ = skillInfo_.skillAndGradeStru;

	MY_ASSERT(skillAndGradeStru.id == skillAndGradeStru_.id, "升级的捕获技能ID不同");

	// 更新
	MY_ASSERT(skillAndGradeStru_.level < skillAndGradeStru.level, "(*it).level >= skillInfo.level");
	
	skillAndGradeStru_.level = skillAndGradeStru.level;
	skillInfo_.probability = skillInfo.probability;

}
//////////////////////////////////////////////////////////////////////////

GlorySkill::GlorySkill(const SkillInfo& skillInfo, IRandomGenerator* /*randomGenerator*/)
:skillInfo_(skillInfo)
{

}

GlorySkill::GlorySkill()
{
	skillInfo_.skillAndGradeStru.id = SKILLID_INVALID;
	skillInfo_.effectType = INVALID_EFFECT;
}

double GlorySkill::getAttackFactor(SOLDIER_TYPE armyType, SOLDIER_TYPE enemyType, LAND_FORM landForm) const
{
	if (PERCENT_DEFEND_EFFECT == skillInfo_.effectType
		|| PERCENT_ATT_DEF_EFFECT == skillInfo_.effectType)
	{
		return 0.0;
	}

	std::set<SOLDIER_TYPE>::const_iterator itFindZero = skillInfo_.objectOccupation.find(SOLDIERTYPE_NONE);
	std::set<SOLDIER_TYPE>::const_iterator itFindArmy = skillInfo_.objectOccupation.find(armyType);

	bool isFind = false;

	std::set<SOLDIER_TYPE>::const_iterator itFind = skillInfo_.otherOccupation.find(enemyType);

	if (skillInfo_.otherOccupation.end() != itFind)
	{
		isFind = true;
	}
	else
	{
		itFind = skillInfo_.otherOccupation.find(SOLDIERTYPE_NONE);

		if (skillInfo_.otherOccupation.end() != itFind)
		{
			isFind = true;
		}
	}


	if ((itFindZero != skillInfo_.objectOccupation.end() || itFindArmy!=skillInfo_.objectOccupation.end())
		&& isFind
		&& (landForm == skillInfo_.landForm || LAND_INVALID == skillInfo_.landForm))
	{
		return skillInfo_.effectVal;
	}
	
	return 0.0;
}

double GlorySkill::getAttDefFactor(SOLDIER_TYPE armyType, SOLDIER_TYPE enemyType, LAND_FORM landForm) const
{
	if (PERCENT_ATT_DEF_EFFECT != skillInfo_.effectType)
	{
		return 0.0;
	}

	std::set<SOLDIER_TYPE>::const_iterator itFindZero = skillInfo_.objectOccupation.find(SOLDIERTYPE_NONE);
	std::set<SOLDIER_TYPE>::const_iterator itFindArmy = skillInfo_.objectOccupation.find(armyType);

	bool isFind = false;

	std::set<SOLDIER_TYPE>::const_iterator itFind = skillInfo_.otherOccupation.find(enemyType);

	if (skillInfo_.otherOccupation.end() != itFind)
	{
		isFind = true;
	}
	else
	{
		itFind = skillInfo_.otherOccupation.find(SOLDIERTYPE_NONE);

		if (skillInfo_.otherOccupation.end() != itFind)
		{
			isFind = true;
		}
	}

	if ((itFindZero != skillInfo_.objectOccupation.end() || itFindArmy!=skillInfo_.objectOccupation.end())
		&& isFind
		&& (landForm == skillInfo_.landForm || LAND_INVALID == skillInfo_.landForm))
	{
		return skillInfo_.effectVal;
	}

	return 0.0;
}

double GlorySkill::getDefenseFactor(SOLDIER_TYPE armyType, SOLDIER_TYPE enemyType, LAND_FORM landForm) const
{
	if (PERCENT_ATTACK_EFFECT == skillInfo_.effectType
		|| PERCENT_ATT_DEF_EFFECT == skillInfo_.effectType)
	{
		return 0.0;
	}

	std::set<SOLDIER_TYPE>::const_iterator itFindZero = skillInfo_.objectOccupation.find(SOLDIERTYPE_NONE);
	std::set<SOLDIER_TYPE>::const_iterator itFindArmy = skillInfo_.objectOccupation.find(armyType);

	bool isFind = false;

	std::set<SOLDIER_TYPE>::const_iterator itFind = skillInfo_.otherOccupation.find(enemyType);

	if (skillInfo_.otherOccupation.end() != itFind)
	{
		isFind = true;
	}
	else
	{
		itFind = skillInfo_.otherOccupation.find(SOLDIERTYPE_NONE);

		if (skillInfo_.otherOccupation.end() != itFind)
		{
			isFind = true;
		}
	}

	if ((itFindZero != skillInfo_.objectOccupation.end() || itFindArmy!=skillInfo_.objectOccupation.end())
		&&  isFind
		&& (landForm == skillInfo_.landForm || LAND_INVALID == skillInfo_.landForm))
	{
		if (skillInfo_.effectVal >= 1)
		{
			return 0.98;
		}

		return skillInfo_.effectVal;
	}

	return 0.0;
}


SKILL_TYPE GlorySkill::getSkillType() const
{
	return skillInfo_.skillAndGradeStru.id;
}

double GlorySkill::getEffectVal() const
{
	return skillInfo_.effectVal;
}

void GlorySkill::upgrade(const SkillInfo& skillInfo)
{
	// 更新
	MY_ASSERT(skillInfo_.skillAndGradeStru.level < skillInfo.skillAndGradeStru.level, "(*it).level >= skillInfo.level");
	skillInfo_.probability = skillInfo.probability;
	skillInfo_.skillAndGradeStru.level = skillInfo.skillAndGradeStru.level;
}

COMPUTINGHURT_NAMESPACE_END
