// @brief ʿ�������࣬���������˺�������������ͷ�����
// @author ������

#include "utility.h"
#include <cmath>
#include "globalmacrodef.h"

// ǰ�᣺����ɹ��ͷ��˼���
static ATTACK_TYPE getAttackType(const IArmy* army)
{
	switch (army->GetArmyOccupation())
	{
	case MAGE: // ��ʦ
		return  MAGIC_ATTACK;
	case PRIEST: // ��ʦ
		return ADD_HP;
	default: // ������
		return PHYSIC_ATTACK;
	}
}

COMPUTINGHURT_NAMESPACE_BEGIN


int computeHurt( int power, double defense, int additionalHurt )
{
	const double PERCENT = 1.0 - std::sqrt(defense)/150.0;
	// ���PERCENTС��0, �������˺�Ϊ1
	if(PERCENT < 0 )
		return 1;

	return static_cast<int>(power * PERCENT) + additionalHurt;
}


HarmInfo newHarmInfo(const IArmy* army, int hurt, int deaths, SKILL_TYPE id ) 
{
	MY_ASSERT(army != 0, "army == 0");
	if(army == 0)
		return HarmInfo();

	HarmInfo harmInfo(new HarmInfoValue);
	harmInfo->ArmyID = army->GetArmyID();
	harmInfo->LostHp = hurt;
	harmInfo->DealNum = deaths;
	harmInfo->ESkill = id;
	return harmInfo;
}

AttackInfo newAttackInfo(const IArmy* army, Armys &armys, SKILL_TYPE id, unsigned int weapondId, const HarmInfos& harminfos )
{
	MY_ASSERT(!army->GetArmyPos().empty() &&
		!armys.front()->GetArmyPos().empty(),
		"army->GetArmyPos().empty() ||\
		armys.front()->GetArmyPos().empty()"
		);

	if(army->GetArmyPos().empty() ||
		armys.front()->GetArmyPos().empty()	)
		return AttackInfo();

	AttackInfo attackInfo(new AttackInfoValue);
	attackInfo->armyId = army->GetArmyID();
	attackInfo->attackerPos = army->GetArmyPos().front();
	attackInfo->targetPos = armys.front()->GetArmyPos().front();
	attackInfo->armyType = army->GetArmyType();
	attackInfo->weaponId = weapondId;
	attackInfo->attackType = getAttackType(army);
	attackInfo->skillId = id;
	attackInfo->harmInfos = harminfos;
	
	return attackInfo;
}
COMPUTINGHURT_NAMESPACE_END
