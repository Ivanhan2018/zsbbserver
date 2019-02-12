#include "battlefieldstatisticsadapter.h"
#include "battlefieldstatistics.h"
#include "IArmy.h"

STATISTICSSYSTEM_NAMESPACE_BEGIN

BattlefieldStatisticsAdapter::BattlefieldStatisticsAdapter():
battleFieldStatistics_(new BattlefieldStatistics)
{

}

BattlefieldStatisticsAdapter::~BattlefieldStatisticsAdapter()
{

}

// 战斗开始
void BattlefieldStatisticsAdapter::onStart(
			 CountyId defenseSideId,		// 防守方阵营ID
			 CountyId attackSideId,			// 进攻方阵营ID
			 MapId mapId,					// 地图ID
			 FightingStartInfo& info		// 返回要发送到客户端的信息
			 )
{
	battleFieldStatistics_->onStart(defenseSideId, attackSideId, mapId, info);
}

void BattlefieldStatisticsAdapter::reset()
{
	battleFieldStatistics_.reset(new BattlefieldStatistics);
}

// @brief 当阵营campId的玩家playerId的一个士兵死亡时调用
// @param campId 阵营ID
// @param playerId 玩家ID
void BattlefieldStatisticsAdapter::onSoldierDie(const IArmy* army, unsigned int deathNum)
{
	if(army == 0)
		return;
	battleFieldStatistics_->onSoldierDie(army->GetCountryID(), 
		army->getPlayerID(), deathNum);
}


void BattlefieldStatisticsAdapter::onKilledEnemy(const IArmy* army, unsigned int killedNum)
{
	if(army == 0)
		return;
	battleFieldStatistics_->onKilledEnemy(army->GetCountryID(), 
		army->getPlayerID(), killedNum);
}


// @brief 当阵营campId的玩家playerId的一个英雄被俘时调用
// @param campId 阵营ID
// @param playerId 玩家ID
// @param heroId 英雄ID
void BattlefieldStatisticsAdapter::onHeroBeCatched(const IArmy* army)
{
	if(army == 0)
		return;

	battleFieldStatistics_->onHeroBeCatched(army->GetCountryID(),
		army->getPlayerID(),
		army->GetOfficerID());
}

// @brief 当阵营campId的玩家playerId的一个英雄heroId受重伤时调用
// @param campId 阵营ID
// @param playerId 玩家ID
// @param heroId 英雄ID
void BattlefieldStatisticsAdapter::onHeroBeHurt(const IArmy* army)
{
	if(army == 0)
		return;

	battleFieldStatistics_->onHeroBeHurt(army->GetCountryID(),
		army->getPlayerID(),
		army->GetOfficerID());
}

// @brief 当ID号为campId的阵营的城镇或箭塔被摧毁时
// @param campId 阵营ID
// @param buildingType 建筑物类型
void BattlefieldStatisticsAdapter::onBuildingDestroyed(CountyId defenseSideId, ARCH_TYPES buildingType)
{
	battleFieldStatistics_->onBuildingDestroyed(defenseSideId, buildingType);
}

// @brief 战斗结束时调用
// @param winCampId 胜利方阵营ID
// @param info 返回战报信息
void BattlefieldStatisticsAdapter::onOver(CountyId winCampId, BattlefieldStatisticsInfo& info)
{
	battleFieldStatistics_->onOver(winCampId, info);
}
STATISTICSSYSTEM_NAMESPACE_END
