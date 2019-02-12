#ifndef TESTCOMPUTINGHURT_TESTHURT_H_
#define TESTCOMPUTINGHURT_TESTHURT_H_
#include "testmacro.h"
#include "ihurt.h"
#include "irandomgenerator.h"
#include "hurtfactory.h"
#include "mockarmy.h"
#include "common.h"

using namespace computinghurt;

typedef boost::shared_ptr<IHurt> ComputingHurtPtr;

static boost::shared_ptr<ArmyInfo> createArmyInfo(SKILL_TYPE id, HeroType herotype, SOLDIER_TYPE soldiertype = MAGE)
{
	boost::shared_ptr<SkillInfo> skill(new SkillInfo);
	skill.get()->id = id;
	skill.get()->probability = 10;
	skill.get()->level = 1;

	SkillInfos skillinfos;
	skillinfos.push_back(*skill.get());

	boost::shared_ptr<HeroInfo> heroinfo(new HeroInfo);
	heroinfo.get()->intelligence = 1;
	heroinfo.get()->power = 2;
	heroinfo.get()->agile = 3;
	heroinfo.get()->endurance = 4;
	heroinfo.get()->type = herotype;
	heroinfo.get()->skillsinfo = skillinfos;

	SoldiersInfo soldierinfo;
	soldierinfo.power = 10;
	soldierinfo.physicDefense = 20;
	soldierinfo.magicDefense = 30;
	soldierinfo.numberOfSoldiers = 100;
	soldierinfo.shootingRate = 30;
	soldierinfo.type = soldiertype;
	soldierinfo.hp = 100;
	soldierinfo.weaponId = 2;

	boost::shared_ptr<ArmyInfo> armyinfo(new ArmyInfo);
	armyinfo.get()->heroInfo = *heroinfo.get();
	armyinfo.get()->soldierInfo = soldierinfo;


	return armyinfo;
}


class MockRandomGenerator : public IRandomGenerator
{
public:
	MockRandomGenerator(){}
	virtual int generator(int /*from*/, int /*to*/)
	{
		return 0;
	}
};

class TestArmyHurt : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(TestArmyHurt);
	CPPUNIT_TEST(testAdditionHurt);
	CPPUNIT_TEST(testAttackAll);
	CPPUNIT_TEST(testNothinkingAttack);
	CPPUNIT_TEST(testCatched);
	CPPUNIT_TEST(testNothinkingDefense);
	CPPUNIT_TEST(testAttack);
	CPPUNIT_TEST(testDefense);
	CPPUNIT_TEST(testGlorySkill);
	CPPUNIT_TEST_SUITE_END();

	// 测试攻击技能
	void testAttack()
	{
		// 无技能
		testAttackOne(SKILLID_INVALID, All_Defend, 296, SKILLID_INVALID, All_Defend);
		// 狂暴打击
		testAttackOne(Double_Hit, All_Defend, 593, Double_Hit, All_Defend);
		// 末日审判
		{
			// 法师部队
			testAttackOne(Doomsday_Skill, All_Defend, 890, Doomsday_Skill, All_Defend);

			// 非法师部队
			testAttackOne(Doomsday_Skill, All_Defend, 296, SKILLID_INVALID, All_Defend, SHOOTER);
		}
	}

 	// 无视防御
 	void testNothinkingDefense()
	{
		// 致命打击
		testAttackOne(Kill_Fate, SKILLID_INVALID, 310, Kill_Fate, SKILLID_INVALID);
	}
	// 无视攻击
	void testNothinkingAttack()
	{
		testAttackOne(Salvo_Skill, Attack_ALL, 0, SKILLID_INVALID, Attack_ALL);
	}

	// 攻击所有
	void testAttackAll()
	{
		// 齐射
		{
			// 如果不是弓箭手部队
			testAttackOne(Salvo_Skill, SKILLID_INVALID, 300, SKILLID_INVALID, SKILLID_INVALID);

			// 是弓箭手部队
			testAttackOne(Salvo_Skill, SKILLID_INVALID, 300, Salvo_Skill, SKILLID_INVALID, SHOOTER);
		}
	}

	// 附加伤害
	void testAdditionHurt()
	{
		// 流血不止
		testAttackOne(Blood_Lost, All_Defend, 396, Blood_Lost, All_Defend);
	}

	// 测试防御技能
	void testDefense()
	{
		// 无技能
		testAttackOne(Double_Hit, SKILLID_INVALID, 601, Double_Hit, SKILLID_INVALID);
		// 铁璧防护
		testAttackOne(Double_Hit, All_Defend, 593, Double_Hit, All_Defend);
	}

 	// 光环技能
 	void testGlorySkill()
	{
		// 攻击
		{
			// 奋击
			testAttackOne(Do_All_CAN_Skill, SKILLID_INVALID, 329, SKILLID_INVALID, SKILLID_INVALID, WARRIOR);
			// 瞄准
			testAttackOne(Aim_at_Skill, SKILLID_INVALID, 329, SKILLID_INVALID, SKILLID_INVALID, SHOOTER);
			// 法力专注
			testAttackOne(Magic_Speciality, SKILLID_INVALID, 329, SKILLID_INVALID, SKILLID_INVALID, MAGE);
			// 神圣冥想，被牧师攻击，以前又没有受到伤害，故伤害是0
			testAttackOne(Holiness_Meditation, SKILLID_INVALID, 0, SKILLID_INVALID, SKILLID_INVALID, PRIEST);
		}
		// 防御
		{
			// 方阵
			testAttackOne(SKILLID_INVALID, Phalanx_Skill, 300, SKILLID_INVALID, SKILLID_INVALID, MAGE, WARRIOR);
			// 箭阵
			testAttackOne(SKILLID_INVALID, Gladiate_Skill, 300, SKILLID_INVALID, SKILLID_INVALID, MAGE, SHOOTER);
			// 法盾
			testAttackOne(SKILLID_INVALID, Magic_shield, 300, SKILLID_INVALID, SKILLID_INVALID, MAGE, MAGE);
			// 圣盾
			testAttackOne(SKILLID_INVALID, Holy_Shield, 300, SKILLID_INVALID, SKILLID_INVALID, MAGE, PRIEST);
		}

	}

	// 捕获
	void testCatched()
	{
		// 		attacker.onKilled(beAttackers.front().get());
		// 		CPPUNIT_ASSERT(!attacker.isCatched_);
	}


protected:

	// @param attackid 攻击者技能ID
	// @param defenseID 防守者技能ID
	// @param hurt 伤害值
	// @param releaseAttack 攻击者释放的技能ID
	// @param releaseDefense 防守者释放的技能ID
	// @param armyType 攻击者部队类型
	void testAttackOne(SKILL_TYPE attackid, SKILL_TYPE defenseId, int hurt, 
		SKILL_TYPE releaseAttack, SKILL_TYPE releaseDefense, 
		const SOLDIER_TYPE& armyType = MAGE, 
		const SOLDIER_TYPE& defenseArmyType = MAGE)
	{
		boost::shared_ptr<ArmyInfo> attackHeroinfo = createArmyInfo(attackid, HEROTYPE_POWER, armyType);
		boost::shared_ptr<ArmyInfo> beAttackHeroinfo = createArmyInfo(defenseId, HEROTYPE_POWER, defenseArmyType);

		boost::shared_ptr<IRandomGenerator> rg(new MockRandomGenerator());
		ComputingHurtPtr attackerHurt(HurtFactory::createArmyHurt(*attackHeroinfo.get(), rg.get()));
		ComputingHurtPtr beAttackerHurt(HurtFactory::createArmyHurt(*beAttackHeroinfo.get(), rg.get()));

		MockFightManager manager;

		MockArmy attacker(attackerHurt.get(), &manager);
		IArmySharedPtr army2(new MockArmy(beAttackerHurt.get(), &manager));
		manager.insert(1, &army2);
		MockArmy* beAttacker = dynamic_cast<MockArmy*>(army2.get());
		ArmyArrays beAttackers;
		beAttackers.push_back(1);

		attacker.Attack(beAttackers);



		// attacker 
		CPPUNIT_ASSERT(attacker.skillid_ == releaseAttack);
		CPPUNIT_ASSERT(attacker.hurt_ == 0);


		// beAttackers 
		CPPUNIT_ASSERT(beAttacker->skillid_ == releaseDefense);
		CPPUNIT_ASSERT(beAttacker->hurt_ == hurt);
	}
};



#endif // TESTCOMPUTINGHURT_TESTHURT_H_
