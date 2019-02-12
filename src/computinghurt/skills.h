// @brief ����ϵͳ����������Ϊ���������ܣ��������ܣ��⻷���ܣ�������
// @author ������

#ifndef COMPUTINGHURT_SKILLS_H_
#define COMPUTINGHURT_SKILLS_H_
#include <boost/shared_ptr.hpp>
#include "common.h"
#include "irandomgenerator.h"
#include "namespacedef.h"
#include "IArmy.h"

COMPUTINGHURT_NAMESPACE_BEGIN

struct AttackOrDefendReleaseInfoValue
{
	SKILL_TYPE skillId;			   // ����ID
	EFFECT_TYPE effectType;		   // Ч������ 
	std::set<SOLDIER_TYPE> otherOccupation;  // �Է�ְҵ����
	double effectVal;			   // Ч��ֵ 
};

typedef boost::shared_ptr<AttackOrDefendReleaseInfoValue> AttackReleaseInfo;
typedef boost::shared_ptr<AttackOrDefendReleaseInfoValue> DefendReleaseInfo;

// ��������
class AttackSkill
{
public:
	AttackSkill(const SkillInfo& skillInfo, IRandomGenerator* randomGenerator);
	AttackSkill();

	AttackReleaseInfo release(const IArmy * selfArmy, const SOLDIER_TYPE armyType) const;

	static AttackReleaseInfo newAttackReleaseInfo(SKILL_TYPE id, EFFECT_TYPE effectType, const std::set<SOLDIER_TYPE> &otherOccupation, double effectVal);

	SKILL_TYPE getSkillType() const;

	inline unsigned int getSkillGrade() const
	{
		return (skillInfo_.skillAndGradeStru).level;
	}

	void upgrade(const SkillInfo& skillInfo);

	void updateRandomGenerator(IRandomGenerator* randomGenerator)
	{
		randomGenerator_ = randomGenerator;
	}

	bool isCanRelease() const;

private:
	SkillInfo skillInfo_;
	IRandomGenerator* randomGenerator_;
};

// ��������
class DefenceSkill
{
public:
	DefenceSkill(const SkillInfo& skillinfo, IRandomGenerator* randomGenerator);
	DefenceSkill();

	DefendReleaseInfo release(const IArmy* selfArmy, const SOLDIER_TYPE& armyType) const;

	static DefendReleaseInfo newDefendReleaseInfo(SKILL_TYPE id, EFFECT_TYPE effectType, const std::set<SOLDIER_TYPE> &otherOccupation, double effectVal);

	SKILL_TYPE getSkillType() const;
	void upgrade(const SkillInfo& skillInfo);

	inline unsigned int getSkillGrade() const
	{
		return (skillInfo_.skillAndGradeStru).level;
	}

	bool isCanRelease() const;

private:
	SkillInfo skillInfo_;
	IRandomGenerator* randomGenerator_;

};

// ������
class CatchOrPlasterSkill
{
public:
	CatchOrPlasterSkill(const SkillInfo& skillinfo, IRandomGenerator* randomGenerator);

	// @return �����Ƿ�ɹ�
	bool release(const IArmy* selfArmy, const SOLDIER_TYPE armyType, int catchHeroRate, int basicBeCapturedRate) const;
	SKILL_TYPE getSkillType() const;
	void upgrade(const SkillInfo& skillInfo);

	inline unsigned int getSkillGrade() const
	{
		return (skillInfo_.skillAndGradeStru).level;
	}

protected:
	bool canRelease() const;

private:
	CatchOrPlasterSkill();

private:
	SkillInfo skillInfo_;
	IRandomGenerator* randomGenerator_;
};


// �⻷����
class GlorySkill
{
public:
	explicit GlorySkill(const SkillInfo& skillInfo, IRandomGenerator* randomGenerator);
	GlorySkill();
	double getAttackFactor(SOLDIER_TYPE armyType, SOLDIER_TYPE enemyType, LAND_FORM landForm) const;
	double getAttDefFactor(SOLDIER_TYPE armyType, SOLDIER_TYPE enemyType, LAND_FORM landForm) const;
	double getDefenseFactor(SOLDIER_TYPE armyType, SOLDIER_TYPE enemyType, LAND_FORM landForm) const;
	SKILL_TYPE getSkillType() const;
	double getEffectVal() const;
	void upgrade(const SkillInfo& skillInfo);

	inline EFFECT_TYPE getEffectType() const
	{
		return skillInfo_.effectType;
	}

	inline unsigned int getSkillGrade() const
	{
		return (skillInfo_.skillAndGradeStru).level;
	}

	inline double getSkillEffectVal() const
	{
		return skillInfo_.effectVal;
	}

private:
	SkillInfo skillInfo_;
};


COMPUTINGHURT_NAMESPACE_END
#endif // COMPUTINGHURT_SKILLS_H_
