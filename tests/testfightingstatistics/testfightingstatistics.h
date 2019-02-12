#ifndef TESTFIGHTINGSTATISTICS_TESTFIGHTINGSTATISTICS_H_
#define TESTFIGHTINGSTATISTICS_TESTFIGHTINGSTATISTICS_H_
#include "testmacro.h"
#include "statisticscommondef.h"
#include "fightingstatistics.h"


static void safeStrCpy(char* dest, size_t destlen, const char* src, size_t srclen)
{
	if(srclen < destlen)
	{
		strcpy(dest, src);
		dest[destlen - 1] = '\0';
		return;
	}

	memcpy(dest, src, destlen - 1);
	dest[destlen - 1] = '\0';
}


STATISTICSSYSTEM_NAMESPACE_BEGIN
class TestFightingStatistics : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(TestFightingStatistics);
 	CPPUNIT_TEST(testSafeStrcpy);
	CPPUNIT_TEST(testStart);
	CPPUNIT_TEST(testOnCatchedHero);
	CPPUNIT_TEST(testOnBeCatchedHero);
	CPPUNIT_TEST(testOnHeroWasInjured);
	CPPUNIT_TEST(testOnEnterWar);
	CPPUNIT_TEST(testOnSoldierDie);
	CPPUNIT_TEST(testOnKilledEnemy);
	CPPUNIT_TEST(testOnArmyDie);
	CPPUNIT_TEST(testQueryArmyInfo);
	CPPUNIT_TEST(testQueryFightingReport);
	CPPUNIT_TEST(testOnAddSoldiers);
	CPPUNIT_TEST(testAllKindsOfCamp);
	CPPUNIT_TEST_SUITE_END();

protected:
	void testStart()
	{
		FightingStatistics fightStatistics(PERCENT/100.0);
		CountyIds campIds;
		fightStatistics.onStart();
		CPPUNIT_ASSERT(fightStatistics.startTime_ != 0);
	}

	void testOnCatchedHero()
	{
		FightingStatistics fightStatistics(PERCENT/100.0);
		CountyIds campIds;
		fightStatistics.onStart();
		CPPUNIT_ASSERT(fightStatistics.startTime_ != 0);

		const CountyId CAMPID(4);
		const PlayerId PLAYERID(2);
		const ArmyId ARMYID(5);
		const HeroId HEROID(3);
		// δ�м�¼ʱ
		{
			fightStatistics.onCatchedHero(CAMPID, PLAYERID, ARMYID, HEROID);
			// ���ץ���Ӣ��ID����û��һ����heroId
			FightingStatistics::ArmyInfoPtr armyInfo = fightStatistics.findElseInsertArmyInfo(CAMPID, PLAYERID, ARMYID);
			const HeroIdSet& heroCatched = armyInfo->herosCatched;
			HeroIdSet::const_iterator it_hero = heroCatched.find(HEROID);
			CPPUNIT_ASSERT(it_hero != heroCatched.end());

		}
	

		// �м�¼ʱ
		{
			fightStatistics.onCatchedHero(CAMPID, PLAYERID, ARMYID, HEROID + 1);
			FightingStatistics::ArmyInfoPtr armyInfo = fightStatistics.findElseInsertArmyInfo(CAMPID, PLAYERID, ARMYID);
			const HeroIdSet& heroCatched = armyInfo->herosCatched;
			HeroIdSet::const_iterator it_hero = heroCatched.find(HEROID + 1);
			CPPUNIT_ASSERT(it_hero != heroCatched.end());
		}

		{
			// û��playerid
			fightStatistics.onCatchedHero(CAMPID, PLAYERID + 1, ARMYID, HEROID + 1);
			FightingStatistics::ArmyInfoPtr armyInfo = fightStatistics.findElseInsertArmyInfo(CAMPID, PLAYERID, ARMYID);
			const HeroIdSet& heroCatched = armyInfo->herosCatched;
			HeroIdSet::const_iterator it_hero = heroCatched.find(HEROID + 1);
			CPPUNIT_ASSERT(it_hero != heroCatched.end());

		}

	}

	void testOnBeCatchedHero()
	{
		FightingStatistics fightStatistics(PERCENT/100.0);
		CountyIds campIds;
		fightStatistics.onStart();
		CPPUNIT_ASSERT(fightStatistics.startTime_ != 0);

		const CountyId CAMPID(4);
		const PlayerId PLAYERID(2);
		const ArmyId ARMYID(5);
		const HeroId HEROID(3);
		// δ�м�¼ʱ
		{
			fightStatistics.onBeCatchedHero(CAMPID, PLAYERID, ARMYID, HEROID);
			// ���ץ���Ӣ��ID����û��һ����heroId
			FightingStatistics::ArmyInfoPtr armyInfo = fightStatistics.findElseInsertArmyInfo(CAMPID, PLAYERID, ARMYID);
			const HeroIdSet& heroBeCatched = armyInfo->herosBeCatched;
			HeroIdSet::const_iterator it_hero = heroBeCatched.find(HEROID);
			CPPUNIT_ASSERT(it_hero != heroBeCatched.end());

		}


		// �м�¼ʱ
		{
			fightStatistics.onBeCatchedHero(CAMPID, PLAYERID, ARMYID, HEROID + 1);
			FightingStatistics::ArmyInfoPtr armyInfo = fightStatistics.findElseInsertArmyInfo(CAMPID, PLAYERID, ARMYID);
			const HeroIdSet& heroBeCatched = armyInfo->herosBeCatched;
			HeroIdSet::const_iterator it_hero = heroBeCatched.find(HEROID + 1);
			CPPUNIT_ASSERT(it_hero != heroBeCatched.end());
		}

		{
			// û��playerid
			fightStatistics.onBeCatchedHero(CAMPID, PLAYERID + 1, ARMYID, HEROID + 1);
			FightingStatistics::ArmyInfoPtr armyInfo = fightStatistics.findElseInsertArmyInfo(CAMPID, PLAYERID, ARMYID);
			const HeroIdSet& heroBeCatched = armyInfo->herosBeCatched;
			HeroIdSet::const_iterator it_hero = heroBeCatched.find(HEROID + 1);
			CPPUNIT_ASSERT(it_hero != heroBeCatched.end());

		}

	}

	void testOnHeroWasInjured()
	{
		FightingStatistics fightStatistics(PERCENT/100.0);
		CountyIds campIds;
		fightStatistics.onStart();
		CPPUNIT_ASSERT(fightStatistics.startTime_ != 0);

		const CountyId CAMPID(4);
		const PlayerId PLAYERID(2);
		const ArmyId ARMYID(5);
		const HeroId HEROID(3);
		// δ�м�¼ʱ
		{
			fightStatistics.onHeroWasInjured(CAMPID, PLAYERID, ARMYID, HEROID);
			// ���ץ���Ӣ��ID����û��һ����heroId
			FightingStatistics::ArmyInfoPtr armyInfo = fightStatistics.findElseInsertArmyInfo(CAMPID, PLAYERID, ARMYID);
			const HeroIdSet& heroWasInjured = armyInfo->heroWasInjured;
			HeroIdSet::const_iterator it_hero = heroWasInjured.find(HEROID);
			CPPUNIT_ASSERT(it_hero != heroWasInjured.end());

		}


		// �м�¼ʱ
		{
			fightStatistics.onHeroWasInjured(CAMPID, PLAYERID, ARMYID, HEROID + 1);
			FightingStatistics::ArmyInfoPtr armyInfo = fightStatistics.findElseInsertArmyInfo(CAMPID, PLAYERID, ARMYID);
			const HeroIdSet& heroWasInjured = armyInfo->heroWasInjured;
			HeroIdSet::const_iterator it_hero = heroWasInjured.find(HEROID + 1);
			CPPUNIT_ASSERT(it_hero != heroWasInjured.end());
		}

		{
			// û��playerid
			fightStatistics.onHeroWasInjured(CAMPID, PLAYERID + 1, ARMYID, HEROID + 1);
			FightingStatistics::ArmyInfoPtr armyInfo = fightStatistics.findElseInsertArmyInfo(CAMPID, PLAYERID, ARMYID);
			const HeroIdSet& heroWasInjured = armyInfo->heroWasInjured;
			HeroIdSet::const_iterator it_hero = heroWasInjured.find(HEROID + 1);
			CPPUNIT_ASSERT(it_hero != heroWasInjured.end());

		}
	}

	void testOnEnterWar()
	{
		FightingStatistics fightStatistics(PERCENT/100.0);
		CountyIds campIds;
		fightStatistics.onStart();
		CPPUNIT_ASSERT(fightStatistics.startTime_ != 0);

		const CountyId CAMPID(4);
		const PlayerId PLAYERID(2);
		const ArmyId ARMYID(3);
		const unsigned int SOLDIERNUM(100);
		const SOLDIER_TYPE SOLDIERTYPE(WARRIOR);
		const std::string ARMYNAME("armyName");
		const std::string HERONAME("heroName");
		const int HEROFACEID(4);
		const std::string PLAYERNAME("playerName");

		// ���βμ�ս��
		fightStatistics.onEnterWar(CAMPID, PLAYERID, ARMYID, SOLDIERNUM, 
			SOLDIERTYPE, ARMYNAME, HERONAME, HEROFACEID, PLAYERNAME);
		FightingStatistics::ArmyInfoPtr armyInfo = fightStatistics.findElseInsertArmyInfo(CAMPID, PLAYERID, ARMYID);
		CPPUNIT_ASSERT(armyInfo->soldierNum == SOLDIERNUM);
		CPPUNIT_ASSERT(armyInfo->type == SOLDIERTYPE);
		CPPUNIT_ASSERT(armyInfo->startTime != 0);
		CPPUNIT_ASSERT(armyInfo->deathNum == 0);
		CPPUNIT_ASSERT(armyInfo->killNum == 0);
		CPPUNIT_ASSERT(armyInfo->exp == 0);
		CPPUNIT_ASSERT(armyInfo->name == ARMYNAME);
		CPPUNIT_ASSERT(armyInfo->heroName == HERONAME);
		CPPUNIT_ASSERT(armyInfo->heroFaceId == HEROFACEID);
		CPPUNIT_ASSERT(armyInfo->playerName == PLAYERNAME);
	// �ٴβμ�ս��
		fightStatistics.onEnterWar(CAMPID, PLAYERID, ARMYID, SOLDIERNUM, 
			SOLDIERTYPE, ARMYNAME, HERONAME, HEROFACEID, PLAYERNAME);
		CPPUNIT_ASSERT(armyInfo->soldierNum == SOLDIERNUM);
		CPPUNIT_ASSERT(armyInfo->type == SOLDIERTYPE);

		// ��ҿ�ʼս��ʱ��
		FightingStatistics::PlayerInfoPtr playerInfo = fightStatistics.findElseInsertPlayerInfo(CAMPID, PLAYERID);
		CPPUNIT_ASSERT(playerInfo.get() != 0);
		CPPUNIT_ASSERT(playerInfo->startTime != 0);

	}

	void testOnSoldierDie()
	{
		FightingStatistics fightStatistics(PERCENT/100.0);
		CountyIds campIds;
		fightStatistics.onStart();
		CPPUNIT_ASSERT(fightStatistics.startTime_ != 0);

		const CountyId CAMPID(4);
		const PlayerId PLAYERID(2);
		const ArmyId ARMYID(3);
		const unsigned int DEATHNUM(100);
		const unsigned int SOLDIERSNUM(200);
		// ʿ������
		fightStatistics.onSoliderDie(CAMPID, PLAYERID, ARMYID, DEATHNUM, SOLDIERSNUM);
		FightingStatistics::ArmyInfoPtr armyInfo = fightStatistics.findElseInsertArmyInfo(CAMPID, PLAYERID, ARMYID);
		CPPUNIT_ASSERT(armyInfo->deathNum == DEATHNUM);
		CPPUNIT_ASSERT(armyInfo->soldierNum == SOLDIERSNUM);
		// ʿ������
		fightStatistics.onSoliderDie(CAMPID, PLAYERID, ARMYID, DEATHNUM, SOLDIERSNUM);
		CPPUNIT_ASSERT(armyInfo->deathNum == DEATHNUM + DEATHNUM);
		CPPUNIT_ASSERT(armyInfo->soldierNum == SOLDIERSNUM);
	}

	void testOnKilledEnemy()
	{
		FightingStatistics fightStatistics(PERCENT/100.0);
		CountyIds campIds;
		fightStatistics.onStart();
		CPPUNIT_ASSERT(fightStatistics.startTime_ != 0);

		const CountyId CAMPID(4);
		const PlayerId PLAYERID(2);
		const ArmyId ARMYID(3);
		const unsigned int KILLNUM(100);
		// ɱ������
		fightStatistics.onKilledEnemy(CAMPID, PLAYERID, ARMYID, KILLNUM);
		FightingStatistics::ArmyInfoPtr armyInfo = fightStatistics.findElseInsertArmyInfo(CAMPID, PLAYERID, ARMYID);
		CPPUNIT_ASSERT(armyInfo->killNum == KILLNUM);
		// ɱ������
		fightStatistics.onKilledEnemy(CAMPID, PLAYERID, ARMYID, KILLNUM);
		CPPUNIT_ASSERT(armyInfo->killNum == KILLNUM + KILLNUM);
	}

	void testOnArmyDie()
	{
		FightingStatistics fightStatistics(PERCENT/100.0);
		CountyIds campIds;
		fightStatistics.onStart();
		CPPUNIT_ASSERT(fightStatistics.startTime_ != 0);

		const CountyId CAMPID(4);
		const PlayerId PLAYERID(2);
		const ArmyId ARMYID(3);
		const unsigned int EXP(100);
		// ����
		fightStatistics.onArmyDie(CAMPID, PLAYERID, ARMYID, EXP);
		FightingStatistics::ArmyInfoPtr armyInfo = fightStatistics.findElseInsertArmyInfo(CAMPID, PLAYERID, ARMYID);
		CPPUNIT_ASSERT(armyInfo->exp == EXP);
		CPPUNIT_ASSERT(armyInfo->status == FightingStatistics::ARMYSTATUS_DIE);

	}

	void testQueryArmyInfo()
	{

		FightingStatistics fightStatistics(PERCENT/100.0);
		CountyIds campIds;
		fightStatistics.onStart();
		CPPUNIT_ASSERT(fightStatistics.startTime_ != 0);

		const CountyId CAMPID(4);
		const PlayerId PLAYERID(2);
		const ArmyId ARMYID(3);
		const unsigned int SOLDIERNUM(100);
		const SOLDIER_TYPE SOLDIERTYPE(WARRIOR);
		const std::string ARMYNAME("armyName");
		const std::string HERONAME("heroName");
		const int HEROFACEID(4);
		const std::string PLAYERNAME("playerName");

		// ���βμ�ս��
		fightStatistics.onEnterWar(CAMPID, PLAYERID, ARMYID, SOLDIERNUM, SOLDIERTYPE, 
			ARMYNAME, HERONAME, HEROFACEID, PLAYERNAME);
		fightStatistics.onEnterWar(CAMPID, PLAYERID + 1, ARMYID + 1, SOLDIERNUM, 
			SOLDIERTYPE, ARMYNAME, HERONAME, HEROFACEID, PLAYERNAME);

		// �з�
		fightStatistics.onEnterWar(CAMPID + 1, PLAYERID, ARMYID, SOLDIERNUM, 
			SOLDIERTYPE, ARMYNAME, HERONAME, HEROFACEID, PLAYERNAME);
		fightStatistics.onOver(CAMPID);

		tagHeroParam heroParam;
		CPPUNIT_ASSERT(fightStatistics.queryArmyInfo(CAMPID, PLAYERID, ARMYID, heroParam));
		CPPUNIT_ASSERT(heroParam.lPlayerID == PLAYERID);
		CPPUNIT_ASSERT(heroParam.lMapID == CAMPID);
		CPPUNIT_ASSERT(heroParam.iExp == 0);
		CPPUNIT_ASSERT(heroParam.iEnemyCount == 0);
		CPPUNIT_ASSERT(heroParam.iGeneralCount == 0);

		CPPUNIT_ASSERT(!fightStatistics.queryArmyInfo(CAMPID + 1, 
			PLAYERID + 1, ARMYID, heroParam));

	}

	void testSafeStrcpy()
	{
		{
			const int DEST_LEN = 2;
			char abc[DEST_LEN];
			std::string str("abc");
			safeStrCpy(abc, DEST_LEN, str.c_str(), str.length());
			CPPUNIT_ASSERT(std::string("a") == abc);

		}

		{
			const int DEST_LEN = 3;
			char abc[DEST_LEN];
			std::string str("abc");
			safeStrCpy(abc, DEST_LEN, str.c_str(), str.length());
			CPPUNIT_ASSERT(std::string("ab") == abc);
		}
		{
			const int DEST_LEN = 4;
			char abc[DEST_LEN];
			std::string str("abc");
			safeStrCpy(abc, DEST_LEN, str.c_str(), str.length());
			CPPUNIT_ASSERT(std::string("abc") == abc);
		}
		{
			const int DEST_LEN = 5;
			char abc[DEST_LEN] = {2};
			std::string str("abc");
			safeStrCpy(abc, DEST_LEN, str.c_str(), str.length());
			CPPUNIT_ASSERT(std::string("abc") == abc);
		}
	}

	void testQueryFightingReport()
	{
		FightingStatistics fightStatistics(PERCENT/100.0);
		CountyIds campIds;
		fightStatistics.onStart();
		CPPUNIT_ASSERT(fightStatistics.startTime_ != 0);

		const CountyId CAMPID(4);
		const PlayerId PLAYERID(2);
		const ArmyId ARMYID(3);
		const unsigned int SOLDIERNUM(100);
		const SOLDIER_TYPE SOLDIERTYPE(WARRIOR);
		const std::string ARMYNAME("armyName");
		const std::string HERONAME("heroName");
		const int HEROFACEID(4);
		const std::string PLAYERNAME("playerName");
		// ���βμ�ս��
		fightStatistics.onEnterWar(CAMPID, PLAYERID, ARMYID, SOLDIERNUM, 
			SOLDIERTYPE, ARMYNAME, HERONAME, HEROFACEID, PLAYERNAME);
		fightStatistics.onEnterWar(CAMPID, PLAYERID + 1, ARMYID + 1, SOLDIERNUM, 
			SOLDIERTYPE, ARMYNAME, HERONAME, HEROFACEID, PLAYERNAME);

		// �з�
		fightStatistics.onEnterWar(CAMPID + 1, PLAYERID, ARMYID, SOLDIERNUM, 
			SOLDIERTYPE, ARMYNAME, HERONAME, HEROFACEID, PLAYERNAME);
		fightStatistics.onOver(CAMPID);


		statisticTotalFightInfo info;
		fightStatistics.queryFightingReport(CAMPID, PLAYERID, info);


		statisticTotalFightInfo info1;
		fightStatistics.queryFightingReport(CAMPID, PLAYERID + 1, info1);

	}

	// ������
	void testOnAddSoldiers()
	{
		FightingStatistics fightStatistics(PERCENT/100.0);
		CountyIds campIds;
		fightStatistics.onStart();
		CPPUNIT_ASSERT(fightStatistics.startTime_ != 0);

		const CountyId CAMPID(4);
		const PlayerId PLAYERID(2);
		const ArmyId ARMYID(3);
		const unsigned int SOLDIERSNUM(200);
		// ������
		fightStatistics.onAddSoldiers(CAMPID, PLAYERID, ARMYID, SOLDIERSNUM);
		FightingStatistics::ArmyInfoPtr armyInfo = fightStatistics.findElseInsertArmyInfo(CAMPID, PLAYERID, ARMYID);
		CPPUNIT_ASSERT(armyInfo->soldierNum == SOLDIERSNUM);
		// ������
		fightStatistics.onAddSoldiers(CAMPID, PLAYERID, ARMYID, SOLDIERSNUM + 1);
		CPPUNIT_ASSERT(armyInfo->soldierNum == SOLDIERSNUM + 1);
	}

	// ���������Ӫquery
	void testAllKindsOfCamp()
	{
		// ���˸������� ����Ĺۿ�
		const CountyId LM_CAMPID(1);
		const PlayerId LM_PLAYERID(100002);
		const ArmyId LM_ARMYID(3);
		const unsigned int LM_SOLDIERNUM(100);
		const SOLDIER_TYPE LM_SOLDIERTYPE(WARRIOR);
		const std::string LM_ARMYNAME("armyName");
		const std::string LM_HERONAME("heroName");
		const int LM_HEROFACEID(4);
		const std::string LM_PLAYERNAME("playerName");

		const CountyId TZ_CAMPID(3);
		const PlayerId TZ_PLAYERID(100002);
		const ArmyId TZ_ARMYID(3);
		const unsigned int TZ_SOLDIERNUM(100);
		const SOLDIER_TYPE TZ_SOLDIERTYPE(WARRIOR);
		const std::string TZ_ARMYNAME("armyName");
		const std::string TZ_HERONAME("heroName");
		const int TZ_HEROFACEID(4);
		const std::string TZ_PLAYERNAME("playerName");
		const int LM_DEATHNUM(4);


		FightingStatistics fightStatistics(PERCENT/100.0);
		fightStatistics.onStart();
		fightStatistics.onEnterWar(LM_CAMPID, LM_PLAYERID, LM_ARMYID, LM_SOLDIERNUM, LM_SOLDIERTYPE, LM_ARMYNAME, LM_HERONAME, LM_HEROFACEID, LM_PLAYERNAME);
		fightStatistics.onSoliderDie(LM_CAMPID, LM_PLAYERID, LM_ARMYID, LM_DEATHNUM, LM_SOLDIERNUM - LM_DEATHNUM);
		fightStatistics.onEnterWar(TZ_CAMPID, TZ_PLAYERID, TZ_ARMYID, TZ_SOLDIERNUM, TZ_SOLDIERTYPE, TZ_ARMYNAME, TZ_HERONAME, TZ_HEROFACEID, TZ_PLAYERNAME);
		fightStatistics.onOver(TZ_CAMPID);

		statisticTotalFightInfo info;
		const CountyId BL_CAMPID(2);
		const PlayerId BL_PLAYERID(100002);
		fightStatistics.queryFightingReport(BL_CAMPID, BL_PLAYERID, info);
		CPPUNIT_ASSERT(info.playerStatic.PlayerID == BL_PLAYERID);
		CPPUNIT_ASSERT(info.obtainBoxNum == 0);
		CPPUNIT_ASSERT(info.CLossNum == LM_DEATHNUM);
		CPPUNIT_ASSERT(info.CcatchHeroNum == 0);
		CPPUNIT_ASSERT(info.CHurtHeroNum == 0);

		// �ҷ��� ��ʵ���ҷ���Ϣ
		CPPUNIT_ASSERT(info.ULossNum == 0);
		CPPUNIT_ASSERT(info.UcatchHeroNum == 0);
		CPPUNIT_ASSERT(info.UHurtHeroNum == 0);

		CPPUNIT_ASSERT(info.num == 2);

		CPPUNIT_ASSERT(info.winer == TZ_CAMPID);

		CPPUNIT_ASSERT(info.ArmyStatic[0].SoldierNum == LM_SOLDIERNUM - LM_DEATHNUM);
		CPPUNIT_ASSERT(info.ArmyStatic[0].deadNum == LM_DEATHNUM);

	}

	private:
		static const int PERCENT = 100;
};

STATISTICSSYSTEM_NAMESPACE_END
#endif // TESTFIGHTINGSTATISTICS_TESTFIGHTINGSTATISTICS_H_