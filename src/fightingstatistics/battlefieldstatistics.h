// @brief 战报统计
// @author 张文龙

#ifndef FIGHTINGSTATISTICS_BATTLEFIELDSTATISTICS_H_
#define FIGHTINGSTATISTICS_BATTLEFIELDSTATISTICS_H_
#include <set>
#include <boost/scoped_ptr.hpp>
#include "namespacedef.h"
#include "iddefs.h"
#include "statisticscommon.h"
#include "Global_Macro.h"

STATISTICSSYSTEM_NAMESPACE_BEGIN



struct AttackSide;
struct DefendingSide;
// 战场统计
class BattlefieldStatistics
{
public:
	BattlefieldStatistics();
	~BattlefieldStatistics();

	// 战斗开始
	void onStart(
		CountyId countId,			// 防守方阵营ID
		CountyId attackSideId,		// 进攻方阵营ID
		MapId mapId,				// 地图ID
		FightingStartInfo& info		// 返回要发送到客户端的信息
		);				

	// @brief 当阵营campId的玩家playerId的士兵死亡时调用
	// @param campId 阵营ID
	// @param playerId 玩家ID
	// @param deathNum 死亡数
	void onSoldierDie(CountyId campId, PlayerId playerId, unsigned int deathNum);

	// @brief 当阵营campId的玩家playerId杀死了士兵时调用
	// @param campId 阵营ID
	// @param playerId 玩家ID
	// @param killedNum 杀敌数
	void onKilledEnemy(CountyId campId, PlayerId playerId, unsigned int killedNum);

	// @brief 当阵营campId的玩家playerId的一个英雄被俘时调用
	// @param campId 阵营ID
	// @param playerId 玩家ID
	// @param heroId 英雄ID
	void onHeroBeCatched(CountyId campId, PlayerId playerId, HeroId heroId);

	// @brief 当阵营campId的玩家playerId的一个英雄heroId受重伤时调用
	// @param campId 阵营ID
	// @param playerId 玩家ID
	// @param heroId 英雄ID
	void onHeroBeHurt(CountyId campId, PlayerId playerId, HeroId heroId);

	// @brief 当ID号为campId的阵营的城镇或箭塔被摧毁时
	// @param campId 阵营ID
	// @param buildingType 建筑物类型
	void onBuildingDestroyed(CountyId defenseSideId, ARCH_TYPES buildingType);

	// @brief 战斗结束时调用
	// @param winCampId 胜利方阵营ID
	// @param info 返回战报信息
	void onOver(CountyId winCampId, BattlefieldStatisticsInfo& info);

protected:
	// 是否是攻击方
	bool isAttackSide(CountyId campId) const;

private:
	friend class TestBattlefieldStatistics;
	BattlefieldStatistics(const BattlefieldStatistics&);
	BattlefieldStatistics& operator=(const BattlefieldStatistics&);
	// 战斗开始时间
	time_t startTime_;
	// 战斗结束时间
	time_t overTime_;
	// 防守方阵营ID
	CountyId campId_;

	CountyId attackSideId_;
	// 攻击方
	boost::scoped_ptr<AttackSide> attackSide_;
	// 防守方
	boost::scoped_ptr<DefendingSide> defendingSide_;

	MapId mapid_;
};
STATISTICSSYSTEM_NAMESPACE_END

#endif // FIGHTINGSTATISTICS_BATTLEFIELDSTATISTICS_H_
