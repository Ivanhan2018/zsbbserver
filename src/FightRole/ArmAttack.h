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
	unsigned int actionLockBout_;   // ���������غ���
	unsigned int trajectorySpeed_;  // �����ٶ�
	unsigned int trajectoryBout_;   // ����ʱ��.

	ArmyAction * action_;
	IArmy *selfArmyPtr_;
	FightManager *fightManager_;
};

// �����˲ʱ����
class TargetAttack : public ArmAttack
{
public:
	virtual ~TargetAttack(){}
	TargetAttack(unsigned int numCircle, unsigned int actionLockBout, unsigned int trajectorySpeed, unsigned int trajectoryBout, ArmyAction * armyAction, 
		FightManager *fightManager, IArmyPtr selfArmyPtr, unsigned int armyId, bool isFollowAttType);

	bool action();
private:

	unsigned int dArmyId_;      // �������ܷ�ID
	bool isFollowAttType_;      // ���������Ƿ��Ǹ��档 TRUE:����,  FALSE:˲ʱ
};

// �ص㹥��
class PointAttack : public ArmAttack
{
public:
	virtual ~PointAttack(){}
	PointAttack(unsigned int numCircle, unsigned int actionLockBout, unsigned int trajectorySpeed, unsigned int trajectoryBout,
		ArmyAction * armyAction, FightManager *fightManager, IArmyPtr selfArmyPtr, const POS_STRUCT &attPos);

	bool action();
private:
	POS_STRUCT attackPoint_;   // �������ܵص�
};
