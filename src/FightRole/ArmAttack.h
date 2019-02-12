#pragma once 

#include "ArmyAction.h"

namespace MaterialEnemyNameSpace
{
	void getMaterialEnemy(int numCircle, IArmyPtr selfArmyPtr, IArmyPtr enemyPtr, ArmyAction * action, const POS_STRUCT &posStru, bool isAttackPos, 
		Armys &armyPtrsCenter, Armys &armyPtrsNear);
};

class FightManager;
class IArmy;

class ArmAttack
{
public:
	virtual ~ArmAttack(void){}

	virtual bool action() = 0;

	unsigned int numCircle_;
	unsigned int actionLockBout_;   // 动作锁定回合数
	unsigned int trajectorySpeed_;  // 弹道速度
	unsigned int trajectoryBout_;   // 弹道时间.

	ArmyAction * action_;
	IArmy *selfArmyPtr_;
	FightManager *fightManager_;
};

// 跟随或瞬时攻击
class TargetAttack : public ArmAttack
{
public:
	virtual ~TargetAttack(){}
	TargetAttack(unsigned int numCircle, unsigned int actionLockBout, unsigned int trajectorySpeed, unsigned int trajectoryBout, ArmyAction * armyAction, 
		FightManager *fightManager, IArmyPtr selfArmyPtr, unsigned int armyId, bool isFollowAttType);

	bool action();
private:

	unsigned int dArmyId_;      // 攻击承受方ID
	bool isFollowAttType_;      // 攻击类型是否是跟随。 TRUE:跟随,  FALSE:瞬时
};

// 地点攻击
class PointAttack : public ArmAttack
{
public:
	virtual ~PointAttack(){}
	PointAttack(unsigned int numCircle, unsigned int actionLockBout, unsigned int trajectorySpeed, unsigned int trajectoryBout,
		ArmyAction * armyAction, FightManager *fightManager, IArmyPtr selfArmyPtr, const POS_STRUCT &attPos);

	bool action();
private:
	POS_STRUCT attackPoint_;   // 攻击承受地点
};
