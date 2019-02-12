#ifndef TESTSELFDEFENCEAI_H_
#define TESTSELFDEFENCEAI_H_

#include <vector>
#include <list>

#pragma warning(push)
#pragma warning(disable:4512)
#include <fstream>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/CompilerOutputter.h>
#pragma warning(pop)

class Army
{

};

typedef std::list<Army> Armys;
typedef std::vector<Armys> ArmysVector;
typedef std::vector<ArmysVector > MapData;

// 障碍物
typedef int LoadMapInfo;

struct Ret
{
	virtual ~Ret() {};
	int i ;
};

struct BattleRet : public Ret
{
	Army* army;
};

class IdleRet : public Ret
{

};


class IStatus
{
public:
	virtual ~IStatus(){};
	virtual void attack() = 0;
	virtual Ret* beAttacked(Army* army) = 0;
	virtual void move() = 0;
};


class IdlessStatus : public IStatus
{
	virtual void attack()
	{

	}
	virtual void move()
	{

	}

	virtual Ret* beAttacked(Army* army) 
	{
		return 0;
	}

};

class BattleStatus : public IStatus
{
	virtual void attack()
	{

	}
	virtual void move()
	{

	}

	virtual Ret* beAttacked(Army* army) 
	{
		return 0;
	}
};

enum StatusAI
{
	STATUS_IDEL = 0,
	STATUS_BATTLE,
	STATUS_MAX
};

class StatusManager
{
public:
	explicit StatusManager(Army* army)
		:status_(0),army_(army)
	{
		statusArray_[STATUS_IDEL] = new IdlessStatus;
		statusArray_[STATUS_BATTLE] = new BattleStatus;
		status_ = statusArray_[STATUS_IDEL];
	}

	~StatusManager()
	{
		// todo...
		for(int i = 0; i < STATUS_MAX; ++i)
		{
			delete statusArray_[i];
		}
	}

	Ret* thing(MapData* mapdata, LoadMapInfo* loadMapInfo)
	{
		Army* enemy = findEnemyInShootDistance(*mapdata);
		if(enemy == 0)
			return new IdleRet;
		else
		{
			BattleRet* ret = new BattleRet;
			ret->army = enemy;
			return ret;
		}
	}

	IStatus* getCurStatus() 
	{
		return status_;
	}

	Ret* beAttacked(Army* army) 
	{
		status_ = statusArray_[STATUS_BATTLE];
		return new BattleRet;
	}

protected:
	// 是否在射程内
	static Army* findEnemyInShootDistance(const MapData& mapdata)
	{
		if(mapdata.empty())
			return 0;

		return 0;
	}

private:
	IStatus* status_;
	Army* army_;
	IStatus* statusArray_[STATUS_MAX];
};

class TestSelfDefence : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(TestSelfDefence);
	CPPUNIT_TEST(testBeAttacked);
	CPPUNIT_TEST(testThing);
	CPPUNIT_TEST_SUITE_END();

	// 当被攻击时的返回状态
	void testBeAttacked()
	{
		Army self;
		StatusManager statusManager(&self);
		Army army;
		IStatus* status = statusManager.getCurStatus();
		IdlessStatus* idleStatus =  dynamic_cast<IdlessStatus*>(status);
		// 测试是否默认的状态是空闲
		CPPUNIT_ASSERT(idleStatus != 0);
		struct Ret* ret = statusManager.beAttacked(&army);

		// 得到当前状态
		status = statusManager.getCurStatus();
		BattleStatus* battleStatus = dynamic_cast<BattleStatus*>(status);
		// 测试被攻击后的状态转换为攻击状态
		CPPUNIT_ASSERT(battleStatus != 0);

		BattleRet* battleRet = dynamic_cast<BattleRet*>(ret);
		// 测试是否返回了一个攻击标识
		CPPUNIT_ASSERT(battleRet != 0);
	}

	void testThing()
	{
		Army self;
		StatusManager statusManager(&self);
		Army army;
		IStatus* status = statusManager.getCurStatus();
		IdlessStatus* idleStatus =  dynamic_cast<IdlessStatus*>(status);
		// 测试是否默认的状态是空闲
		CPPUNIT_ASSERT(idleStatus != 0);

		// thing
		MapData mapdata;
		LoadMapInfo loadMapInfo;
		struct Ret* ret = statusManager.thing(&mapdata, &loadMapInfo);

		// 得到当前状态
		status = statusManager.getCurStatus();
		BattleStatus* battleStatus = dynamic_cast<BattleStatus*>(status);
		// 测试被攻击后的状态转换为攻击状态
		CPPUNIT_ASSERT(battleStatus != 0);

		BattleRet* battleRet = dynamic_cast<BattleRet*>(ret);
		// 测试是否返回了一个攻击标识
		CPPUNIT_ASSERT(battleRet != 0);

	}
};

#endif // TESTSELFDEFENCEAI_H_