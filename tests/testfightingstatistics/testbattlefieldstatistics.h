#ifndef  TESTFIGHTINGSTATISTICS_TESTBATTLEFIELDSTATISTICS_H_
#define TESTFIGHTINGSTATISTICS_TESTBATTLEFIELDSTATISTICS_H_
#include "testmacro.h"
#include "battlefieldstatistics.h"
#include "statisticscommondef.h"

STATISTICSSYSTEM_NAMESPACE_BEGIN
// static bool operator==(const AttackSideInfo& info1, const AttackSideInfo& info2)
// {
// 	return info1.heroBeCatchedNum == info2.heroBeCatchedNum &&
// 		info1.heroBeHurtNum == info2.heroBeHurtNum &&
// 		info1.soldierDeathsNum == info2.soldierDeathsNum;
// }
// 
// static bool operator==(const DefendingSideInfo& info1, const DefendingSideInfo& info2)
// {
// 	return info1.heroBeCatchedNum == info2.heroBeCatchedNum &&
// 		info1.heroBeHurtNum == info2.heroBeHurtNum &&
// 		info1.soldierDeathsNum == info2.soldierDeathsNum &&
// 		info1.archeryTowerDeathsNum == info2.archeryTowerDeathsNum &&
// 		info1.townDeathsNum == info2.townDeathsNum;
// }

class TestBattlefieldStatistics : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(TestBattlefieldStatistics);
// 	CPPUNIT_TEST(testStart);
// 	CPPUNIT_TEST(testOnSoldierDie);
// 	CPPUNIT_TEST(testOnHeroBeCatched);
// 	CPPUNIT_TEST(testOnHeroBeHurt);
// 	CPPUNIT_TEST(testOnOver);
// 	CPPUNIT_TEST(testOnBuildingDestroyed);
	CPPUNIT_TEST_SUITE_END();

protected:
#if 0
	void testStart()
	{
		const CountyId campId = 1;

		BattlefieldStatistics statistics;
		FightingStartInfo info;
		statistics.onStart(countyId, campId, info);
		CPPUNIT_ASSERT(info.countyId == countyId);
		CPPUNIT_ASSERT(statistics.campId_ == campId);
	}

	void testOnSoldierDie()
	{
		const PlayerId playerId = 4;

		BattlefieldStatistics statistics;
		FightingStartInfo info;
		statistics.onStart(countyId, campId_, info);
		// 防守方
		{
			CPPUNIT_ASSERT(statistics.defendingSide_.get()->playerMap.empty());
			statistics.onSoldierDie(campId_, playerId, 1);
			CPPUNIT_ASSERT(statistics.defendingSide_.get()->playerMap.size() == 1);

			PlayerStatisticsMap::const_iterator it = statistics.defendingSide_.get()->playerMap.find(playerId);
			CPPUNIT_ASSERT(it != statistics.defendingSide_.get()->playerMap.end());

			PlayerStatisticsPtr ptr = (*it).second;
			CPPUNIT_ASSERT(ptr.get() != 0);
			CPPUNIT_ASSERT(ptr.get()->soldierDeathsNum == 1);
		}


		// 攻击方
		{
			CPPUNIT_ASSERT(statistics.attackSide_.get()->playerMap.empty());
			statistics.onSoldierDie(campId_ + 1, playerId, 1);
			CPPUNIT_ASSERT(statistics.attackSide_.get()->playerMap.size() == 1);

			PlayerStatisticsMap::const_iterator it = statistics.attackSide_.get()->playerMap.find(playerId);
			CPPUNIT_ASSERT(it != statistics.attackSide_.get()->playerMap.end());

			PlayerStatisticsPtr ptr = (*it).second;
			CPPUNIT_ASSERT(ptr.get() != 0);
			CPPUNIT_ASSERT(ptr.get()->soldierDeathsNum == 1);
		}

	}

	void testOnHeroBeCatched()
	{
		const PlayerId playerId = 4;
		const HeroId heroId = 2;

		BattlefieldStatistics statistics;
		FightingStartInfo info;
		statistics.onStart(countyId, campId_, info);
		// 防守方
		{
			testHeroBeCatched_i(statistics, statistics.defendingSide_.get()->playerMap, campId_, playerId, heroId);
		}
		// 攻击方
		{
			testHeroBeCatched_i(statistics, statistics.attackSide_.get()->playerMap, campId_ + 1, playerId, heroId);
		}

	}

	void testOnHeroBeHurt()
	{
		const PlayerId playerId = 4;
		const HeroId heroId = 2;

		BattlefieldStatistics statistics;
		FightingStartInfo info;
		statistics.onStart(countyId, campId_, info);
		// 防守方
		{
			testOnHeroBeHurt_i(statistics, statistics.defendingSide_.get()->playerMap, campId_, playerId, heroId);
		}
		// 攻击方
		{
			testOnHeroBeHurt_i(statistics, statistics.attackSide_.get()->playerMap, campId_ + 1, playerId, heroId);
		}
	}

	void testOnBuildingDestroyed()
	{
		BattlefieldStatistics statistics;
		FightingStartInfo info;
		statistics.onStart(countyId, campId_, info);
		// 防守方
		{
			BuildingType buildingType = BUILDINGTYPE_ARCHERYTOWER;
			unsigned int pre = statistics.defendingSide_.get()->archeryTowerDeathsNum;
			statistics.onBuildingDestroyed(campId_, buildingType);
			CPPUNIT_ASSERT(pre + 1 == statistics.defendingSide_.get()->archeryTowerDeathsNum);

			buildingType = BUILDINGTYPE_BASTILE;
			pre = statistics.defendingSide_.get()->townDeathsNum;
			statistics.onBuildingDestroyed(campId_, buildingType);
			CPPUNIT_ASSERT(pre + 1 == statistics.defendingSide_.get()->townDeathsNum);

			// 其他无效果
			buildingType = BUILDINGTYPE_INVALID;
			pre = statistics.defendingSide_.get()->townDeathsNum;
			statistics.onBuildingDestroyed(campId_, buildingType);
			CPPUNIT_ASSERT(pre == statistics.defendingSide_.get()->townDeathsNum);
			pre = statistics.defendingSide_.get()->townDeathsNum;
			statistics.onBuildingDestroyed(campId_, buildingType);
			CPPUNIT_ASSERT(pre == statistics.defendingSide_.get()->townDeathsNum);


		}
		// 攻击方
		{
			// 无效果
// 			BuildingType buildingType = BUILDINGTYPE_ARCHERYTOWER;
// 			unsigned int pre = statistics.defendingSide_.get()->archeryTowerDeathsNum;
// 			statistics.onBuildingDestroyed(campId_ + 1, buildingType);
// 			CPPUNIT_ASSERT(pre == statistics.defendingSide_.get()->archeryTowerDeathsNum);
// 
// 			buildingType = BUILDINGTYPE_BASTILE;
// 			pre = statistics.defendingSide_.get()->townDeathsNum;
// 			statistics.onBuildingDestroyed(campId_ + 1, buildingType);
// 			CPPUNIT_ASSERT(pre == statistics.defendingSide_.get()->townDeathsNum);


		}
	}



	void testOnOver()
	{
		// 没有做任何事
		{

			BattlefieldStatistics statistics;
			FightingStartInfo info;
			statistics.onStart(countyId, campId_, info);

			BattlefieldStatisticsInfo bsi;
			statistics.onOver(campId_, bsi);
			CPPUNIT_ASSERT(bsi.winCampId == 0);
			CPPUNIT_ASSERT(bsi.excellentplayerId == 0);
			CPPUNIT_ASSERT(bsi.countyId == 0);
			AttackSideInfo asi;
			CPPUNIT_ASSERT(bsi.attackSideInfo == asi);
			DefendingSideInfo dsi;
			CPPUNIT_ASSERT(bsi.defendingSideInfo == dsi);

		}

		{
			// 正常
			const PlayerId playerId = 4;
			const HeroId heroId = 2;

			BattlefieldStatistics statistics;
			FightingStartInfo info;
			statistics.onStart(countyId, campId_, info);

			statistics.onBuildingDestroyed(campId_, BUILDINGTYPE_ARCHERYTOWER);
			statistics.onHeroBeCatched(campId_ + 1, playerId, heroId);
			statistics.onHeroBeHurt(campId_, playerId, heroId);
			statistics.onSoldierDie(campId_, playerId, 1);
			BattlefieldStatisticsInfo bsi;
			statistics.onOver(campId_, bsi);

			// check
			CPPUNIT_ASSERT(bsi.winCampId == campId_);
			CPPUNIT_ASSERT(bsi.excellentplayerId == playerId);
			CPPUNIT_ASSERT(bsi.countyId == countyId);
			AttackSideInfo asi ;
			asi.soldierDeathsNum = 0;
			asi.heroBeCatchedNum = 1;
			asi.heroBeHurtNum = 0;
			CPPUNIT_ASSERT(bsi.attackSideInfo == asi);
			DefendingSideInfo dsi ;
			dsi.soldierDeathsNum = 1;
			dsi.heroBeCatchedNum = 0;
			dsi.heroBeHurtNum = 1;
			dsi.townDeathsNum = 0;
			dsi.archeryTowerDeathsNum = 1;
			CPPUNIT_ASSERT(bsi.defendingSideInfo == dsi);
		}
	}

private:
	void testHeroBeCatched_i( BattlefieldStatistics &statistics, PlayerStatisticsMap& playerMap, CountyId campId, const PlayerId playerId, const HeroId heroId )
	{
		CPPUNIT_ASSERT(playerMap.empty());
		statistics.onHeroBeCatched(campId, playerId, heroId);
		CPPUNIT_ASSERT(playerMap.size() == 1);

		PlayerStatisticsMap::const_iterator it = playerMap.find(playerId);
		CPPUNIT_ASSERT(it != playerMap.end());

		PlayerStatisticsPtr ptr = (*it).second;
		CPPUNIT_ASSERT(ptr.get() != 0);

		HeroIdSet::iterator hero_it = ptr.get()->herosBeCatched.find(heroId);
		CPPUNIT_ASSERT(hero_it != ptr.get()->herosBeCatched.end());
		CPPUNIT_ASSERT(*hero_it == heroId);
	}

	void testOnHeroBeHurt_i( BattlefieldStatistics &statistics, PlayerStatisticsMap& playerMap, CountyId campId, const PlayerId playerId, const HeroId heroId )
	{
		CPPUNIT_ASSERT(playerMap.empty());
		statistics.onHeroBeHurt(campId, playerId, heroId);
		CPPUNIT_ASSERT(playerMap.size() == 1);

		PlayerStatisticsMap::const_iterator it = playerMap.find(playerId);
		CPPUNIT_ASSERT(it != playerMap.end());

		PlayerStatisticsPtr ptr = (*it).second;
		CPPUNIT_ASSERT(ptr.get() != 0);

		HeroIdSet::iterator hero_it = ptr.get()->herosBeHurt.find(heroId);
		CPPUNIT_ASSERT(hero_it != ptr.get()->herosBeHurt.end());
		CPPUNIT_ASSERT(*hero_it == heroId);
	}
#endif

private:
	static const CountyId campId_ = 1;
	static const CountyId countyId = 1;
};
STATISTICSSYSTEM_NAMESPACE_END

#endif // TESTFIGHTINGSTATISTICS_TESTBATTLEFIELDSTATISTICS_H_