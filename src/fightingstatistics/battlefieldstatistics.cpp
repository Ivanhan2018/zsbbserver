// @brief 战报统计
// @author 张文龙

#include "battlefieldstatistics.h"
#include <ctime>
#include "statisticscommondef.h"
#include "globalmacrodef.h"

namespace
{
	using namespace statisticssystem;
	// TODO:是玩家部队
	static bool isPlayerArmy(PlayerId playerId)
	{
		return playerId >= 100000;
	}

	static PlayerStatisticsPtr newPlayerStatistics()
	{
		PlayerStatisticsPtr playerStatistics(new PlayerStatistics());
		playerStatistics.get()->soldierDeathsNum = 0;
		return playerStatistics;
	}

	static PlayerStatisticsPtr findPlayerStatistics(const PlayerStatisticsMap& playerMap, PlayerId playerId)
	{
		if(playerMap.empty())
			return PlayerStatisticsPtr();

		PlayerStatisticsMap::const_iterator it = playerMap.find(playerId);
		// 找到了
		if(it != playerMap.end())
		{
			return (*it).second;
		}
		else
		{
			return PlayerStatisticsPtr();
		}
	}

	// 处理士兵死亡
	static void processSoldierDie(PlayerStatisticsMap& playerMap, PlayerId playerId, unsigned int deathNum)
	{
		PlayerStatisticsPtr p = findPlayerStatistics(playerMap, playerId);
		if(p.get() == 0)
		{
			// 找不到
			PlayerStatisticsPtr playerStatistics = newPlayerStatistics();
			playerStatistics->soldierDeathsNum = deathNum;
			playerMap.insert(PlayerStatisticsMap::value_type(playerId, playerStatistics));
			return;
		}
		// 找到了
		p->soldierDeathsNum += deathNum;
	}

	static void processKilledNum(PlayerStatisticsMap& playerMap, PlayerId playerId, unsigned int killedNum)
	{
		PlayerStatisticsPtr p = findPlayerStatistics(playerMap, playerId);
		if(p.get() == 0)
		{
			// 找不到
			PlayerStatisticsPtr playerStatistics = newPlayerStatistics();
			playerStatistics->killNum = killedNum;
			playerMap.insert(PlayerStatisticsMap::value_type(playerId, playerStatistics));
			return;
		}
		// 找到了
		p->killNum += killedNum;
	}

	// 处理被捕获
	static void processBeCatched(PlayerStatisticsMap& playerMap, PlayerId playerId, HeroId heroId)
	{
		PlayerStatisticsPtr p = findPlayerStatistics(playerMap, playerId);
		if(p.get() == 0)
		{
			// 找不到
			PlayerStatisticsPtr playerStatistics = newPlayerStatistics();
			playerStatistics.get()->herosBeCatched.insert(heroId);
			playerMap.insert(PlayerStatisticsMap::value_type(playerId, playerStatistics));
			return;
		}
		// 找到了
		p.get()->herosBeCatched.insert(heroId);
	}

	// 处理被伤害
	static void processBeHurt(PlayerStatisticsMap& playerMap, PlayerId playerId, HeroId heroId)
	{
		PlayerStatisticsPtr p = findPlayerStatistics(playerMap, playerId);
		if(p.get() == 0)
		{
			// 找不到
			PlayerStatisticsPtr playerStatistics = newPlayerStatistics();
			playerStatistics.get()->herosBeHurt.insert(heroId);
			playerMap.insert(PlayerStatisticsMap::value_type(playerId, playerStatistics));
			return;
		}
		// 找到了
		p.get()->herosBeHurt.insert(heroId);
	}



	// 寻找最优异玩家， 如果playerId都是npc, 返回的playerId为0
	static bool findExcellentPlayer(const PlayerStatisticsMap& players, PlayerId& playerId)
	{
		if(players.empty())
			return false;

		unsigned int lastNum = 0;
		playerId = 0;

		PlayerStatisticsMap::const_iterator it = players.begin();
		for (; it != players.end(); ++it)
		{
			if(isPlayerArmy((*it).first) && 
				(*it).second->killNum > lastNum)
			{
				playerId = (*it).first;
				lastNum = (*it).second->killNum;
			}
		}

		return true;
	}

	// 统计死亡总和
	static unsigned int countDeaths(const PlayerStatisticsMap& players)
	{
		if(players.empty())
			return 0;

		unsigned int num = 0;
		PlayerStatisticsMap::const_iterator it = players.begin();
		for (; it != players.end(); ++it)
		{
			num += (*it).second->soldierDeathsNum;
		}

		return num;
	}


	// 得到map中所有玩家捕获的英雄总数
	static unsigned int getHeroBeCatchedNum(const PlayerStatisticsMap& playeStatistics)
	{
		if(playeStatistics.empty())
			return 0;

		unsigned int num = 0;
		PlayerStatisticsMap::const_iterator it = playeStatistics.begin();
		for (; it != playeStatistics.end(); ++it)
		{
			num += static_cast<unsigned int>((*it).second->herosBeCatched.size());
		}

		return num;
	}

	// 得到map中所有玩家受重伤的英雄总数
	static unsigned int getHeroBeHurtNum(const PlayerStatisticsMap& playeStatistics)
	{
		if(playeStatistics.empty())
			return 0;

		unsigned int num = 0;
		PlayerStatisticsMap::const_iterator it = playeStatistics.begin();
		for (; it != playeStatistics.end(); ++it)
		{
			num += static_cast<unsigned int>((*it).second->herosBeHurt.size());
		}

		return num;
	}


	// 从AttackSide到AttackSideInfo
	static void getAttackSideInfo(const AttackSide& attackSide, AttackSideInfo& info)
	{
		info.soldierDeathsNum = countDeaths(attackSide.playerMap);
		info.heroBeCatchedNum = getHeroBeCatchedNum(attackSide.playerMap);
		info.heroBeHurtNum = getHeroBeHurtNum(attackSide.playerMap);
	}

	// 从DefendingSide到DefendingSideInfo
	static void getDefendingSideInfo(const DefendingSide& defendingSide, DefendingSideInfo& info)
	{
		info.soldierDeathsNum = countDeaths(defendingSide.playerMap);
		info.heroBeCatchedNum = getHeroBeCatchedNum(defendingSide.playerMap);
		info.heroBeHurtNum = getHeroBeHurtNum(defendingSide.playerMap);

		info.archeryTowerDeathsNum = defendingSide.archeryTowerDeathsNum;
		info.townDeathsNum = defendingSide.townDeathsNum;
	}
} // namespace

STATISTICSSYSTEM_NAMESPACE_BEGIN
BattlefieldStatistics::BattlefieldStatistics():
startTime_(0)
,overTime_(0)
,campId_(0)
,attackSideId_(0)
,attackSide_(new AttackSide)
,defendingSide_(new struct DefendingSide)
,mapid_(0)
{

}

BattlefieldStatistics::~BattlefieldStatistics()
{
	startTime_ = 0;
	overTime_ = 0;
	attackSideId_ = 0;
	campId_ = 0;
	mapid_ = 0;
}

bool BattlefieldStatistics::isAttackSide(CountyId campId) const
{
	// 不是一个阵营ID，将是攻击方
	return campId_ != campId;
}


// 战斗开始
void BattlefieldStatistics::onStart(
									CountyId defenseSideId,							// 防守方阵营ID
									CountyId attackSideId,						// 进攻方阵营ID
									MapId mapId,
									FightingStartInfo& info						// 返回要发送到客户端的信息
									)
{
	time(&startTime_);
	campId_ = defenseSideId;
	attackSideId_ = attackSideId;
	mapid_ = mapId;

	info.defenseSideId = defenseSideId;
	info.startTime = startTime_;
	info.attackSideId = attackSideId;
	info.mapId = mapId;
}

// 当士兵死亡时
void BattlefieldStatistics::onSoldierDie(CountyId campId, PlayerId playerId, unsigned int deathNum)
{
	// 根据阵营，来判断是进攻方还是防御方
	if(isAttackSide(campId))
	{
		processSoldierDie(attackSide_->playerMap, playerId, deathNum);
	}
	else
	{
		processSoldierDie(defendingSide_->playerMap, playerId, deathNum);
	}
}

void BattlefieldStatistics::onKilledEnemy(CountyId campId, PlayerId playerId, unsigned int killedNum)
{
	// 根据阵营，来判断是进攻方还是防御方
	if(isAttackSide(campId))
	{
		processKilledNum(attackSide_->playerMap, playerId, killedNum);
	}
	else
	{
		processKilledNum(defendingSide_->playerMap, playerId, killedNum);
	}
}


// 当英雄被俘时
void BattlefieldStatistics::onHeroBeCatched(CountyId campId, PlayerId playerId, HeroId heroId)
{
	if(isAttackSide(campId))
	{
		processBeCatched(attackSide_->playerMap, playerId, heroId);
	}
	else
	{
		processBeCatched(defendingSide_->playerMap, playerId, heroId);
	}
}

// 当英雄受重伤时
void BattlefieldStatistics::onHeroBeHurt(CountyId campId, PlayerId playerId, HeroId heroId)
{
	if(isAttackSide(campId))
	{
		processBeHurt(attackSide_->playerMap, playerId, heroId);
	}
	else
	{
		processBeHurt(defendingSide_->playerMap, playerId, heroId);
	}
}

// 当城镇被摧毁时
void BattlefieldStatistics::onBuildingDestroyed(CountyId defenseSideId, ARCH_TYPES buildingType)
{
	MY_ASSERT(!isAttackSide(defenseSideId), "campid is attack side");
	if(isAttackSide(defenseSideId))
	{
		return;
	}
	else
	{
		switch (buildingType)
		{
		case ARCH_CITY:
			defendingSide_.get()->townDeathsNum++;
			break;
		case ARCH_SENTRY:
			defendingSide_.get()->archeryTowerDeathsNum++;
			break;
		case ARCH_TOWNHOUSE:
		case ARCH_INVALID:
		default:
			break;
		}
	}
}


// 战斗结束
void BattlefieldStatistics::onOver(CountyId winCampId, BattlefieldStatisticsInfo& info)
{
	time(&overTime_);

	const PlayerStatisticsMap& playerStatisticsMap = isAttackSide(winCampId) ? attackSide_.get()->playerMap : defendingSide_.get()->playerMap;

	// 给INFO赋值
	info.startTime = startTime_;
	info.overTime = overTime_;
	info.countyId = campId_;
	info.attackSideId = attackSideId_;
	info.winCampId = winCampId;
	info.mapId = mapid_;
	getAttackSideInfo(*attackSide_.get(), info.attackSideInfo);
	getDefendingSideInfo(*defendingSide_.get(), info.defendingSideInfo);

	// 找到最优异玩家
	info.excellentplayerId = 0;
	if(!findExcellentPlayer(playerStatisticsMap, info.excellentplayerId))
		return ;

	return ;
}

STATISTICSSYSTEM_NAMESPACE_END
