// @brief 战报统计要使用的类型的定义
// @author 张文龙

#ifndef FIGHTINGSTATISTICS_STATISTICSCOMMONDEF_H_
#define FIGHTINGSTATISTICS_STATISTICSCOMMONDEF_H_
#include <hash_map>
#include <boost/shared_ptr.hpp>

STATISTICSSYSTEM_NAMESPACE_BEGIN

// 玩家统计信息
struct PlayerStatistics
{
	PlayerStatistics():killNum(0),soldierDeathsNum(0),herosBeCatched(),herosBeHurt(){}

	unsigned int killNum;
	// 士兵死亡人数
	unsigned int soldierDeathsNum;
	// 被俘英雄数
	HeroIdSet herosBeCatched;
	// 重伤英雄数
	HeroIdSet herosBeHurt;
};

typedef boost::shared_ptr<PlayerStatistics> PlayerStatisticsPtr;
typedef stdext::hash_map<PlayerId, PlayerStatisticsPtr > PlayerStatisticsMap;

// 进攻方
struct AttackSide
{
	// 所属所有玩家的统计
	PlayerStatisticsMap playerMap;
};

// 防守方
struct DefendingSide
{
	DefendingSide():townDeathsNum(0),archeryTowerDeathsNum(0),playerMap()
	{

	}
	// 城镇被摧毁数
	unsigned int townDeathsNum;
	// 箭塔被摧毁数
	unsigned int archeryTowerDeathsNum;
	// 所属所有玩家的统计
	PlayerStatisticsMap playerMap;
};


STATISTICSSYSTEM_NAMESPACE_END

#endif // FIGHTINGSTATISTICS_STATISTICSCOMMONDEF_H_
