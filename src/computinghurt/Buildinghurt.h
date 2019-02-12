// @brief �������˺���
// @author ������

#ifndef COMPUTINGHURT_BUILDINGHURT_H_
#define COMPUTINGHURT_BUILDINGHURT_H_
#include "ibuildinghurt.h"
#include "common.h"
#include "namespacedef.h"

COMPUTINGHURT_NAMESPACE_BEGIN

class BuildingHurt : public IBuildingHurt
{
	friend class TestBuildingHurt;
public:
	explicit BuildingHurt(IArmy* selfArmy, const BuildingInfo& buildingInfo);
	~BuildingHurt();
	virtual void release();
	virtual void setArmy(IArmy* army);
	virtual AttackInfo attack(Armys& armys);
	AttackInfo attack(Armys &/*armysCenter*/, Armys &/*armysInOneCircle*/);
	virtual HarmInfo beAttacked(const NothingingDefendStru &nothingDefStru,  IArmy* attacker, int power);
	virtual bool canCatchHero(IArmy* army);
	virtual bool canPlasterHero(IArmy* army);
	virtual SOLDIER_TYPE getSoldierType() const;
	virtual int getCurHurtHp() const;
	virtual int getHeroIncreaseExp() const;
	virtual int getCurrentHp() const;
	virtual int getDefendPower(ATTACK_TYPE attackType) const;
	virtual int GetAttackPower() const;
	virtual int getSoldierNum() const;
	virtual void resetHurt();
	// ����
	virtual void upgrade(const BuildingInfo& info);

	const HeroAttribute * getHeroAttribute()
	{
		return NULL;
	}

	HeroType getHeroType()
	{
		return INVALID_TYPE;
	}	

	// �������ò�������   
	unsigned int resetSoldierNum(unsigned int /*soldierNum*/)
	{
		return 0;
	}

	// ���Ӳ�������
	unsigned int addSoldierNum(unsigned int /*soldierNum*/)
	{
		return 0;
	}

	unsigned int getTotalHp() const
	{
		return totalHp_;
	}

protected:
	BuildingHurt();
	NO_COPYABLE_AND_ASSIGN(BuildingHurt);

	int processHurt(IArmy* attacker, int& hurt);
	HarmInfo processHurtStraight( int power, IArmy* attacker );
private:
	IArmy* selfArmy_;
	BuildingInfo buildingInfo_;
	// ��Ѫֵ
	int totalHp_;
};

COMPUTINGHURT_NAMESPACE_END
#endif // COMPUTINGHURT_BUILDINGHURT_H_
