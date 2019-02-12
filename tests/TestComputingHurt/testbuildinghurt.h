#ifndef TESTCOMPUTINGHURT_TESTBUILDINGHURT_H_
#define TESTCOMPUTINGHURT_TESTBUILDINGHURT_H_

#include "testmacro.h"
#include "ihurt.h"
#include "irandomgenerator.h"
#include "hurtfactory.h"
#include "Buildinghurt.h"

COMPUTINGHURT_NAMESPACE_BEGIN
class TestBuildingHurt : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(TestBuildingHurt);
	CPPUNIT_TEST(testBuilding);
	CPPUNIT_TEST_SUITE_END();

	// 	void testAttack();
	// 	void testBeAttack();

	// 建筑物
	void testBuilding()
	{
		SkillInfo skills[] = {
			{Double_Hit, 10, 1},
			{Kill_Fate, 15, 2},
			{Blood_Lost, 15, 2},
			{All_Defend, 15, 2},
			{Attack_ALL, 15, 2},
			{Chatch_Hero, 15, 2},
			{Doomsday_Skill, 15, 2},
			{Salvo_Skill, 15, 2},
			{Magic_Speciality, 15, 2}, // 光环技能， 法师
			{Magic_shield, 15, 2}, // 光环技能， 法师
		};

		SkillInfos skillinfos;
		for(unsigned int i = 0; i < ARRAY_SIZE(skills); ++i)
			skillinfos.push_back(skills[i]);

		HeroInfo heroinfo = {1, // 智力
			2, // 力量
			3, // 敏捷
			4, // 耐力
			HEROTYPE_POWER, // 类型
			skillinfos
		};

		SoldiersInfo soldiersinfo;
		soldiersinfo.power = 10;
		soldiersinfo.physicDefense = 20;
		soldiersinfo.magicDefense = 30;
		soldiersinfo.numberOfSoldiers = 100;
		soldiersinfo.shootingRate = 30;
		soldiersinfo.hp = 100;
		soldiersinfo.weaponId = 1;

		ArmyInfo armyinfo;
		armyinfo.heroInfo = heroinfo;
		armyinfo.soldierInfo = soldiersinfo;

		BuildingInfo buildingInfo;
		buildingInfo.power = 1;
		buildingInfo.defense = 1;
		buildingInfo.hp = 100;

		boost::shared_ptr<IRandomGenerator> rg(new MockRandomGenerator());

		IBuildingHurt* hurt = HurtFactory::createBuildingHurt(buildingInfo);
		// upgrade
		{
			BuildingInfo bi = {1, 2, 3};
			hurt->upgrade(bi);
			BuildingHurt* bh = dynamic_cast<BuildingHurt*>(hurt);
			CPPUNIT_ASSERT(bh != 0);
			CPPUNIT_ASSERT(bh->buildingInfo_.defense == bi.defense);
			CPPUNIT_ASSERT(bh->buildingInfo_.power == bi.power);
			CPPUNIT_ASSERT(bh->buildingInfo_.hp != bi.hp);
		}



		ComputingHurtPtr attacker(hurt);
		ComputingHurtPtr beAttacker(HurtFactory::createArmyHurt(armyinfo, rg.get()));

		MockFightManager manager;

		MockArmy self(attacker.get(), &manager);
		IArmySharedPtr army2(new MockArmy(beAttacker.get(), &manager));
		manager.insert(1, &army2);
		ArmyArrays destArmys;
		destArmys.push_back(1);

		self.Attack(destArmys);
		self.onKilled(&self);



	}
};

COMPUTINGHURT_NAMESPACE_END
#endif // TESTCOMPUTINGHURT_TESTBUILDINGHURT_H_