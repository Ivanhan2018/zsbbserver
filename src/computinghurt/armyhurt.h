// $Id: computingarmyhurt.h 2 2009-08-27 12:12:38 Zhangwenlong $ 
// @brief 部队伤害类
// 当部队攻击时，伤害值的计算如下：
// 攻击力的计算：攻击力包括英雄的和所有士兵的，公式如下：
// 总攻击力（T） = （士兵人数×武器攻击力×种族命中率+英雄攻击力）× （1+光环类效果）× 相克系数 × （地形影响+ 地形技能）
// 其中，英雄攻击力 ＝ 敏捷或力量或智力（根据英雄各类的不同）x 5
// 还有部队所在的位置(如森林，陆地，它的攻击系数是不同的)来共同确定的）
//
// 英雄的物理减免率=（√英雄的物理防御力）/150
// 英雄的魔法减免率=（√英雄的魔法防御力）/150

// 部队物攻减免率＝装备物防×（1＋光环类效果＋兵种专精技能）×（地形影响＋地形技能）
// 部队魔攻减免率＝装备魔防×（1＋光环类效果＋兵种专精技能）×（地形影响＋地形技能）

// 部队的减免率不得超过90%，超过90%按90%计算。
// 最后伤害=Damage X (1-部队减免率) X（1-英雄减免率）
// 
// 生命公式：
// 部队单个士兵生命=种族+职业+英雄HP/1.5
// @author 张文龙
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


	// 函数描述：废弃所学的技能
	// @param  skillAndGradeStru 技能基本信息数据结构
	// @return true/false
	virtual bool abandonSkill(const SkillAndGradeStru& skillAndGradeStru);

	// 函数描述：是否学习了指定等级的技能
	// @param  SkillAndGradeStru 技能、等级的数据结构
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

	// 重新设置部队数量   
    unsigned int resetSoldierNum(unsigned int soldierNum);

	// 增加部队数量
	unsigned int addSoldierNum(unsigned int soldierNum);

    unsigned int getTotalHp() const;

protected:
	int processHurt(const IArmy* army, int& hurt, unsigned int landformDefAtt, bool isHurt2AddHp = false);

	// 计算总的攻击力：士兵武器攻击力x人数x种族命中率
	int computeAtk(bool isShowPower = false) const;

	// 通知部队army释放了技能
	void notifyReleaseSkill(const SKILL_TYPE& skillId) const;

	// 处理攻击所有的
	HarmInfos processAttackAll(const NothingingDefendStru &nothingDefStru, const BreakOutStru &breakoutStru, Armys& armys, int atk, unsigned int landformAffAtt);

	// 处理第一个部队
	HarmInfo processAttackOne(const NothingingDefendStru &nothingDefStru,  const BreakOutStru &breakoutStru, IArmy* beAttacker, int atk, unsigned int landformAffAtt);

	// 增加经验值
	unsigned int addExp(const IArmy* enemy, int deaths, int hurt);

	// 直接处理伤害，无视防御
	HarmInfo processHurtStraight( int power, const IArmy* attacker, unsigned int landformDefAtt);

	// 处理减少部队人数
	HarmInfo processReduceMan(double reduceProba);

	int processReduce(double reduceProba, int& hurt);

	HarmInfo processSelfBlastAttackOne(IArmy *beAttacker, int atk);

	// 处理自爆卡攻击所有的
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
