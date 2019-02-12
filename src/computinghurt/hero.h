// @brief Ӣ���࣬�������ͷż��ܣ�������Լ��Ĺ������ͷ�����
// @author ������

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

	// ��������
	AttackReleaseInfo releaseAttackSkill(const IArmy* selfArmy, const SOLDIER_TYPE& armyType, IArmy *enemyArmy);
	DefendReleaseInfo releaseDefenceSkill(const IArmy* selfArmy, const SOLDIER_TYPE& armyType);

	// ���Լ��Ĳ���ɱ������armyʱ������Ӣ��
	bool catchHero(IArmy* army, const SOLDIER_TYPE armyType);

	bool canPlasterHero(IArmy* army, const SOLDIER_TYPE armyType);

	// ����Ӣ�۵Ĺ�����
	int computeATK() const;

	// ����Ӣ�۵ķ�����
	int computeHeroDefence(ATTACK_TYPE attackType) const;

	// �õ��⻷�������ܵĹ���ϵ��
	double getAttackFactor(SOLDIER_TYPE armyType, SOLDIER_TYPE enemyType, LAND_FORM landForm) const;

	// �õ����μ��ܵĹ���ϵ��
	double getAttLandformFactor(SOLDIER_TYPE armyType, SOLDIER_TYPE enemyType, LAND_FORM landForm) const;

	// �õ����μ��ܵķ���ϵ��
	double getDefLandformFactor(SOLDIER_TYPE armyType, SOLDIER_TYPE enemyType, LAND_FORM landForm) const;

	// �õ��⻷�������ܵķ���ϵ��
	double getDefenseFactor(SOLDIER_TYPE armyType, SOLDIER_TYPE enemyType, LAND_FORM landForm) const;

	int getHp() const;

	// �Ӿ���
	void addExp(int exp);

	int getIncreaseExp() const;

	virtual void upgradeLevel(const HeroAttribute& heroAttribute) ;
	virtual void upgradeOrLearnedSkill(const SkillInfo& skillInfo);

	bool abandonSkill(const SkillAndGradeStru& skillInfo);

	// �Ƿ�ѧϰ��ָ���ļ���
	bool isLearnedSpecifiGradeSkill(const SkillAndGradeStru & skillAndGradeStru);

	// �Ƿ�ѧϰ������HP�ļ���
	bool isLearnedAddHpSkill(const SkillAndGradeStru & skillAndGradeStru, double &effectVal);

	//ljw 20091119
	//У��Ӣ��������Ϣ
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
	typedef std::map<SKILL_TYPE, unsigned int> SkillId2SkillGrade;  // ����ID--�����ܵȼ�

	HeroInfo heroInfo_;

	SkillId2AttackSkill attackSkills_;				// ��������ӳ������
	SkillId2DefenceSkill defenceSkills_;			// ��������ӳ������

	CatchOrPlasterSkill * catchSkill_;				// ������
	CatchOrPlasterSkill * plasterSkill_;			// �ش�����

	SkillId2GlorySkill glorySkills_;				// �⻷����ӳ������
	SkillId2SkillGrade skillId2SkillGrade_;			// ����ר��ӳ������
	
	// ���ӵľ���ֵ�� ��ֵ�Ǹ����ӵ�ֵ��
	// �ϲ�Ὣ������ӵ�ֵupdate�����ݿ���
	int increaseExp_;
	IRandomGenerator* generator_;
};

COMPUTINGHURT_NAMESPACE_END

#endif // COMPUTINGHURT_HERO_H_
