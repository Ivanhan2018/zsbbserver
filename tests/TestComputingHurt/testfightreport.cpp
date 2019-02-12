#include "testfightreport.h"
//#include "testreportcommon.h"

#if 0
class Army : public IArmy
{
public:
	virtual void attack(Armys& armys) = 0;
	// 当被攻击时
	virtual void beAttacked(int atk, int additionalHurt) = 0;
	// 处理伤害
	virtual void processHurt(int hurt) = 0;
	// 通知释放了技能
	virtual void notifyRelease(SkillId id) = 0;
	// 当杀死了对方部队时
	virtual void onKilled(IArmy* army) = 0;
	// 得到武器攻击力
	virtual int getWeaponAtk() const = 0;
	// 得到士兵防御力
	virtual int getSoldierDefense() const = 0;
	// 得到部队人数
	virtual int getNumberOfSoldiers() const = 0;
	// 得到种族命中率
	virtual int getShootingRate() const = 0;
	// 得到部队类型
	virtual ArmyType getArmyType() const = 0;
	// 得到建筑物的攻击力
	virtual int getAtk() const = 0;
	// 得到建筑物防御力
	virtual int getDefense() const = 0;
};
#endif

void TestFightReport::testReport()
{

}
