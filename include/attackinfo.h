// @brief 对伤害攻击输入的结构，它将与以前的结构适配 
// @author 张文龙

#ifndef FIGHTSYSTEM_ATTACKINFO_H_
#define FIGHTSYSTEM_ATTACKINFO_H_
#include <vector>
#include "boost/shared_ptr.hpp"
#pragma warning(push)
#pragma warning(disable:4100)
#include "ProFightSystem.h"
#pragma warning(pop)

typedef HarmParm HarmInfoValue;
typedef boost::shared_ptr<HarmInfoValue> HarmInfo;
typedef std::vector<HarmInfo> HarmInfos;

// 攻击信息
struct AttackInfoValue
{
	unsigned long armyId;
	POS_STRUCT attackerPos;
	POS_STRUCT targetPos;
	ARMY_TYPE armyType;
	unsigned int weaponId;
	// 攻击类型
	ATTACK_TYPE attackType; 
	// 攻击方释放的技能
	SKILL_TYPE skillId;

	HarmInfos harmInfos;
};

typedef boost::shared_ptr<AttackInfoValue> AttackInfo;

#endif // FIGHTSYSTEM_ATTACKINFO_H_