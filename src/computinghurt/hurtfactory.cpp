// @brief 伤害系统工厂，它负责创建不同类型的伤害系统接口
// @author 张文龙

#include "hurtfactory.h"
#include "armyhurt.h"
#include "Buildinghurt.h"
COMPUTINGHURT_NAMESPACE_BEGIN

IArmyHurt* HurtFactory::createArmyHurt(const ArmyInfo& armyInfo, const LandformInfoMap *landformInfoMapPtrParam, IArmy *army, IRandomGenerator* randomGenerator)
{
	return new ArmyHurt(armyInfo, landformInfoMapPtrParam, army, randomGenerator);
}

IBuildingHurt* HurtFactory::createBuildingHurt(IArmy *army, const BuildingInfo& buildingInfo)
{
	return new BuildingHurt(army, buildingInfo);
}
COMPUTINGHURT_NAMESPACE_END
