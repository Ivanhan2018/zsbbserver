// @brief 伤害系统公共函数
// @author 张文龙

#ifndef COMPUTINGHURT_UTILITY_H_
#define COMPUTINGHURT_UTILITY_H_
#include "namespacedef.h"
#include "attackinfo.h"
#include "IArmy.h"

COMPUTINGHURT_NAMESPACE_BEGIN

int computeHurt( int power, double defense, int additionalHurt );
HarmInfo newHarmInfo(const IArmy* army, int hurt, int deaths, SKILL_TYPE id );
AttackInfo newAttackInfo(const IArmy* army, Armys &armys, SKILL_TYPE id,
						 unsigned int weapondId, const HarmInfos& harminfos );

COMPUTINGHURT_NAMESPACE_END

#endif // COMPUTINGHURT_UTILITY_H_
