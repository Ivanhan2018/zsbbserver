// @brief ս��ͳ��
// @author ������

#include "battlefieldstatistics.h"
#include <ctime>
#include "statisticscommondef.h"
#include "globalmacrodef.h"

namespace
{
	using namespace statisticssystem;
	// TODO:����Ҳ���
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
		// �ҵ���
		if(it != playerMap.end())
		{
			return (*it).second;
		}
		else
		{
			return PlayerStatisticsPtr();
		}
	}

	// ����ʿ������
	static void processSoldierDie(PlayerStatisticsMap& playerMap, PlayerId playerId, unsigned int deathNum)
	{
		PlayerStatisticsPtr p = findPlayerStatistics(playerMap, playerId);
		if(p.get() == 0)
		{
			// �Ҳ���
			PlayerStatisticsPtr playerStatistics = newPlayerStatistics();
			playerStatistics->soldierDeathsNum = deathNum;
			playerMap.insert(PlayerStatisticsMap::value_type(playerId, playerStatistics));
			return;
		}
		// �ҵ���
		p->soldierDeathsNum += deathNum;
	}

	static void processKilledNum(PlayerStatisticsMap& playerMap, PlayerId playerId, unsigned int killedNum)
	{
		PlayerStatisticsPtr p = findPlayerStatistics(playerMap, playerId);
		if(p.get() == 0)
		{
			// �Ҳ���
			PlayerStatisticsPtr playerStatistics = newPlayerStatistics();
			playerStatistics->killNum = killedNum;
			playerMap.insert(PlayerStatisticsMap::value_type(playerId, playerStatistics));
			return;
		}
		// �ҵ���
		p->killNum += killedNum;
	}

	// ��������
	static void processBeCatched(PlayerStatisticsMap& playerMap, PlayerId playerId, HeroId heroId)
	{
		PlayerStatisticsPtr p = findPlayerStatistics(playerMap, playerId);
		if(p.get() == 0)
		{
			// �Ҳ���
			PlayerStatisticsPtr playerStatistics = newPlayerStatistics();
			playerStatistics.get()->herosBeCatched.insert(heroId);
			playerMap.insert(PlayerStatisticsMap::value_type(playerId, playerStatistics));
			return;
		}
		// �ҵ���
		p.get()->herosBeCatched.insert(heroId);
	}

	// �����˺�
	static void processBeHurt(PlayerStatisticsMap& playerMap, PlayerId playerId, HeroId heroId)
	{
		PlayerStatisticsPtr p = findPlayerStatistics(playerMap, playerId);
		if(p.get() == 0)
		{
			// �Ҳ���
			PlayerStatisticsPtr playerStatistics = newPlayerStatistics();
			playerStatistics.get()->herosBeHurt.insert(heroId);
			playerMap.insert(PlayerStatisticsMap::value_type(playerId, playerStatistics));
			return;
		}
		// �ҵ���
		p.get()->herosBeHurt.insert(heroId);
	}



	// Ѱ����������ң� ���playerId����npc, ���ص�playerIdΪ0
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

	// ͳ�������ܺ�
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


	// �õ�map��������Ҳ����Ӣ������
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

	// �õ�map��������������˵�Ӣ������
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


	// ��AttackSide��AttackSideInfo
	static void getAttackSideInfo(const AttackSide& attackSide, AttackSideInfo& info)
	{
		info.soldierDeathsNum = countDeaths(attackSide.playerMap);
		info.heroBeCatchedNum = getHeroBeCatchedNum(attackSide.playerMap);
		info.heroBeHurtNum = getHeroBeHurtNum(attackSide.playerMap);
	}

	// ��DefendingSide��DefendingSideInfo
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
	// ����һ����ӪID�����ǹ�����
	return campId_ != campId;
}


// ս����ʼ
void BattlefieldStatistics::onStart(
									CountyId defenseSideId,							// ���ط���ӪID
									CountyId attackSideId,						// ��������ӪID
									MapId mapId,
									FightingStartInfo& info						// ����Ҫ���͵��ͻ��˵���Ϣ
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

// ��ʿ������ʱ
void BattlefieldStatistics::onSoldierDie(CountyId campId, PlayerId playerId, unsigned int deathNum)
{
	// ������Ӫ�����ж��ǽ��������Ƿ�����
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
	// ������Ӫ�����ж��ǽ��������Ƿ�����
	if(isAttackSide(campId))
	{
		processKilledNum(attackSide_->playerMap, playerId, killedNum);
	}
	else
	{
		processKilledNum(defendingSide_->playerMap, playerId, killedNum);
	}
}


// ��Ӣ�۱���ʱ
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

// ��Ӣ��������ʱ
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

// �����򱻴ݻ�ʱ
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


// ս������
void BattlefieldStatistics::onOver(CountyId winCampId, BattlefieldStatisticsInfo& info)
{
	time(&overTime_);

	const PlayerStatisticsMap& playerStatisticsMap = isAttackSide(winCampId) ? attackSide_.get()->playerMap : defendingSide_.get()->playerMap;

	// ��INFO��ֵ
	info.startTime = startTime_;
	info.overTime = overTime_;
	info.countyId = campId_;
	info.attackSideId = attackSideId_;
	info.winCampId = winCampId;
	info.mapId = mapid_;
	getAttackSideInfo(*attackSide_.get(), info.attackSideInfo);
	getDefendingSideInfo(*defendingSide_.get(), info.defendingSideInfo);

	// �ҵ����������
	info.excellentplayerId = 0;
	if(!findExcellentPlayer(playerStatisticsMap, info.excellentplayerId))
		return ;

	return ;
}

STATISTICSSYSTEM_NAMESPACE_END
