// @brief 士兵集合类，它负责处理伤害，输出攻击力和防御力
// @author 张文龙

#ifndef COMPUTINGHURT_SOLDIER_H_
#define COMPUTINGHURT_SOLDIER_H_
#include "common.h"
#include "namespacedef.h"
#include "globalmacrodef.h"
#include "irandomgenerator.h"

COMPUTINGHURT_NAMESPACE_BEGIN

// 士兵
class Soldiers
{
public:
	Soldiers(const SoldiersInfo& soldierInfo,  IRandomGenerator* randomGenerator);

	~Soldiers();

	void initHP(int heroHp);

	// @brief 处理伤害，伤害的计算公式为：先计算出杀死一个后所需要的HP算出来
	//        它(P)＝（所有士兵的HP + 英雄的HP）/ 士兵人数
	//        然后，算出死亡的兵数，使用总的伤害值(总的伤害值为上次剩余的伤害值加上这次的伤害值)整除，
	//        即 D＝伤害值（T）/ P
	//        剩下的伤害值为：T = T - D*P
	// @param attackerSoldierType 攻击部队士兵的类型，如果是牧师，就是加血
	// @parm hurt 伤害
	// @param heroHp 英雄的HP
	int processHurt(SOLDIER_TYPE attackerSoldierType, int& hurt);

	unsigned int processReduce(double reduceProba, int& hurt);

	SOLDIER_TYPE getType() const;

	unsigned int getWeaponId() const;

	// 计算攻击力
	int computeAtk(bool isShowPower) const;

	// 计算防御力
	int computeDefence(ATTACK_TYPE attackType) const;

	int getCurHurtHp() const;

	int getNum() const;

	int getCurrentHp() const;

	void resetHurt();

	//@brief 当英雄等级更新了时
	void onHeroUpgradeLevel(int heroHp);

	// 技能触发的增加HP。在刚学到这个技能时调用，仅一次
	void addHp(double effectVal);

	// 在废弃增加HP的技能时调用
	void reduceHp(double effectVal);

	// 根据耐力增加HP
	void addHpForEndurance(int endurance);

	unsigned int getPhysicDefense() const
	{
		return static_cast<unsigned int>(soldierInfo_.physicDefense);
	}

	unsigned int getMagicDefense() const
	{
		return static_cast<unsigned int>(soldierInfo_.magicDefense);
	}

	unsigned int resetSoldierNum(unsigned int soldierNum);

	unsigned int addSoldierNum(unsigned int soldierNum);

	unsigned int getTotalHp() const;

	unsigned int suicide();

private:
	Soldiers();
	NO_COPYABLE_AND_ASSIGN(Soldiers);
private:
	SoldiersInfo soldierInfo_;
	// 杀死一个士兵所需要的HP
	int hpOfDeath_; 
	// 部队总的HP
	int totaoHp_;
	// 累积的伤害
	int totalHurt_;

	IRandomGenerator* randomGenerator_;
};
COMPUTINGHURT_NAMESPACE_END

#endif // COMPUTINGHURT_SOLDIER_H_
