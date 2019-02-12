// $Id: computingarmyhurt.h 2 2009-08-27 12:12:38 Zhangwenlong $ 
// @brief �����˺���
// �����ӹ���ʱ���˺�ֵ�ļ������£�
// �������ļ��㣺����������Ӣ�۵ĺ�����ʿ���ģ���ʽ���£�
// �ܹ�������T�� = ��ʿ������������������������������+Ӣ�۹��������� ��1+�⻷��Ч������ ���ϵ�� �� ������Ӱ��+ ���μ��ܣ�
// ���У�Ӣ�۹����� �� ���ݻ�����������������Ӣ�۸���Ĳ�ͬ��x 5
// ���в������ڵ�λ��(��ɭ�֣�½�أ����Ĺ���ϵ���ǲ�ͬ��)����ͬȷ���ģ�
//
// Ӣ�۵����������=����Ӣ�۵������������/150
// Ӣ�۵�ħ��������=����Ӣ�۵�ħ����������/150

// �����﹥�����ʣ�װ���������1���⻷��Ч��������ר�����ܣ���������Ӱ�죫���μ��ܣ�
// ����ħ�������ʣ�װ��ħ������1���⻷��Ч��������ר�����ܣ���������Ӱ�죫���μ��ܣ�

// ���ӵļ����ʲ��ó���90%������90%��90%���㡣
// ����˺�=Damage X (1-���Ӽ�����) X��1-Ӣ�ۼ����ʣ�
// 
// ������ʽ��
// ���ӵ���ʿ������=����+ְҵ+Ӣ��HP/1.5
// @author ������
// 2009-09-25

#ifndef COMPUTINGHURT_ARMYHURT_H_
#define COMPUTINGHURT_ARMYHURT_H_
#include "iarmyhurt.h"
#include "soldiers.h"
#include "hero.h"
#include "namespacedef.h"


COMPUTINGHURT_NAMESPACE_BEGIN

class ArmyHurt : public IArmyHurt
{
public:
	ArmyHurt(const ArmyInfo& armyInfo, const LandformInfoMap *landformInfoMapPtrParam, IArmy *army, IRandomGenerator* randomGenerator);
	~ArmyHurt();
	virtual void release();
	virtual AttackInfo attack(Armys& armys);
	virtual AttackInfo attack(Armys &armysCenter, Armys &armysInOneCircle);
	virtual HarmInfo beAttacked(const NothingingDefendStru &nothingDefStru,  IArmy* attacker, int power);
	virtual bool canCatchHero(IArmy* army);
	virtual bool canPlasterHero(IArmy* army);
	virtual SOLDIER_TYPE getSoldierType() const ;
	virtual int getCurHurtHp() const;
	virtual int getHeroIncreaseExp() const;
	virtual int getCurrentHp() const;
	virtual int getDefendPower(ATTACK_TYPE attackType) const;
	virtual int GetAttackPower() const;
	virtual int getSoldierNum() const;
	virtual void resetHurt();
	virtual void upgradeLevel(const HeroAttribute& heroAttribute);
	virtual void upgradeOrLearnedSkill(const SkillInfo& skillInfo);


	// ����������������ѧ�ļ���
	// @param  skillAndGradeStru ���ܻ�����Ϣ���ݽṹ
	// @return true/false
	virtual bool abandonSkill(const SkillAndGradeStru& skillAndGradeStru);

	// �����������Ƿ�ѧϰ��ָ���ȼ��ļ���
	// @param  SkillAndGradeStru ���ܡ��ȼ������ݽṹ
	// @return true/false
	virtual bool isLearnedSpecifiGradeSkill(const SkillAndGradeStru & skillAndGradeStruct);

	// 
	const HeroAttribute * getHeroAttribute()
	{
		return hero_.getHeroAttribute();
	}

	HeroType getHeroType()
	{
		return hero_.getHeroType();
	}

	// �������ò�������   
    unsigned int resetSoldierNum(unsigned int soldierNum);

	// ���Ӳ�������
	unsigned int addSoldierNum(unsigned int soldierNum);

    unsigned int getTotalHp() const;

protected:
	int processHurt(const IArmy* army, int& hurt, unsigned int landformDefAtt, bool isHurt2AddHp = false);

	// �����ܵĹ�������ʿ������������x����x����������
	int computeAtk(bool isShowPower = false) const;

	// ֪ͨ����army�ͷ��˼���
	void notifyReleaseSkill(const SKILL_TYPE& skillId) const;

	// ���������е�
	HarmInfos processAttackAll(const NothingingDefendStru &nothingDefStru, const BreakOutStru &breakoutStru, Armys& armys, int atk, unsigned int landformAffAtt);

	// �����һ������
	HarmInfo processAttackOne(const NothingingDefendStru &nothingDefStru,  const BreakOutStru &breakoutStru, IArmy* beAttacker, int atk, unsigned int landformAffAtt);

	// ���Ӿ���ֵ
	unsigned int addExp(const IArmy* enemy, int deaths, int hurt);

	// ֱ�Ӵ����˺������ӷ���
	HarmInfo processHurtStraight( int power, const IArmy* attacker, unsigned int landformDefAtt);

	// ������ٲ�������
	HarmInfo processReduceMan(double reduceProba);

	int processReduce(double reduceProba, int& hurt);

	HarmInfo processSelfBlastAttackOne(IArmy *beAttacker, int atk);

	// �����Ա����������е�
	HarmInfos processSelfBlastAttackAll(const Armys &armysCenter, const Armys &armysInOneCircle);

private:
	ArmyHurt();
	ArmyHurt(const ArmyHurt &armyHurt);

private:
	Soldiers soldiers_;	
	Hero hero_;	
	const LandformInfoMap *landformInfoMapPtr_;
	IArmy* selfArmy_;
};
COMPUTINGHURT_NAMESPACE_END

#endif // COMPUTINGHURT_ARMYHURT_H_
