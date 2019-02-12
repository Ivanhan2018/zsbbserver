// @brief 英雄类，它负责释放技能，并输出自己的攻击力和防御力
// @author 张文龙

#ifndef COMPUTINGHURT_HERO_H_
#define COMPUTINGHURT_HERO_H_
#include <map>
#include "skills.h"
#include "namespacedef.h"
#include "soldiers.h"

class IArmy;
class IRandomGenerator;


COMPUTINGHURT_NAMESPACE_BEGIN

class Hero
{
public:
	Hero(const HeroInfo& heroinfo, IRandomGenerator* randomGenerator, Soldiers &soldiers);
	~Hero();

	// 触发技能
	AttackReleaseInfo releaseAttackSkill(const IArmy* selfArmy, const SOLDIER_TYPE& armyType, IArmy *enemyArmy);
	DefendReleaseInfo releaseDefenceSkill(const IArmy* selfArmy, const SOLDIER_TYPE& armyType);

	// 当自己的部队杀死部队army时，捕获英雄
	bool catchHero(IArmy* army, const SOLDIER_TYPE armyType);

	bool canPlasterHero(IArmy* army, const SOLDIER_TYPE armyType);

	// 计算英雄的攻击力
	int computeATK() const;

	// 计算英雄的防御力
	int computeHeroDefence(ATTACK_TYPE attackType) const;

	// 得到光环攻击技能的攻击系数
	double getAttackFactor(SOLDIER_TYPE armyType, SOLDIER_TYPE enemyType, LAND_FORM landForm) const;

	// 得到地形技能的攻击系数
	double getAttLandformFactor(SOLDIER_TYPE armyType, SOLDIER_TYPE enemyType, LAND_FORM landForm) const;

	// 得到地形技能的防御系数
	double getDefLandformFactor(SOLDIER_TYPE armyType, SOLDIER_TYPE enemyType, LAND_FORM landForm) const;

	// 得到光环防御技能的防御系数
	double getDefenseFactor(SOLDIER_TYPE armyType, SOLDIER_TYPE enemyType, LAND_FORM landForm) const;

	int getHp() const;

	// 加经验
	void addExp(int exp);

	int getIncreaseExp() const;

	virtual void upgradeLevel(const HeroAttribute& heroAttribute) ;
	virtual void upgradeOrLearnedSkill(const SkillInfo& skillInfo);

	bool abandonSkill(const SkillAndGradeStru& skillInfo);

	// 是否学习了指定的技能
	bool isLearnedSpecifiGradeSkill(const SkillAndGradeStru & skillAndGradeStru);

	// 是否学习了增加HP的技能
	bool isLearnedAddHpSkill(const SkillAndGradeStru & skillAndGradeStru, double &effectVal);

	//ljw 20091119
	//校验英雄属性信息
	static bool checkHeroInfo(HeroAttribute & heroInfo);

	const HeroAttribute * getHeroAttribute() const
	{
		return &heroInfo_.heroAttribute;
	}

	HeroType getHeroType() const
	{
		return heroInfo_.type;
	}

private:
	Hero();
	Hero(const Hero&);
	Hero& operator=(const Hero&);
private:
	typedef std::map<SKILL_TYPE, AttackSkill> SkillId2AttackSkill;
	typedef std::map<SKILL_TYPE, DefenceSkill> SkillId2DefenceSkill;
	typedef std::map<SKILL_TYPE, GlorySkill> SkillId2GlorySkill;
	typedef std::map<SKILL_TYPE, unsigned int> SkillId2SkillGrade;  // 技能ID--》技能等级

	HeroInfo heroInfo_;

	SkillId2AttackSkill attackSkills_;				// 攻击技能映射容器
	SkillId2DefenceSkill defenceSkills_;			// 防御技能映射容器

	CatchOrPlasterSkill * catchSkill_;				// 捕获技能
	CatchOrPlasterSkill * plasterSkill_;			// 重创技能

	SkillId2GlorySkill glorySkills_;				// 光环技能映射容器
	SkillId2SkillGrade skillId2SkillGrade_;			// 武器专精映射容器
	
	// 增加的经验值， 此值是个增加的值，
	// 上层会将这个增加的值update到数据库中
	int increaseExp_;
	IRandomGenerator* generator_;
};

COMPUTINGHURT_NAMESPACE_END

#endif // COMPUTINGHURT_HERO_H_
