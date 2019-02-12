// @brief ʿ�������࣬���������˺�������������ͷ�����
// @author ������

#ifndef COMPUTINGHURT_SOLDIER_H_
#define COMPUTINGHURT_SOLDIER_H_
#include "common.h"
#include "namespacedef.h"
#include "globalmacrodef.h"
#include "irandomgenerator.h"

COMPUTINGHURT_NAMESPACE_BEGIN

// ʿ��
class Soldiers
{
public:
	Soldiers(const SoldiersInfo& soldierInfo,  IRandomGenerator* randomGenerator);

	~Soldiers();

	void initHP(int heroHp);

	// @brief �����˺����˺��ļ��㹫ʽΪ���ȼ����ɱ��һ��������Ҫ��HP�����
	//        ��(P)��������ʿ����HP + Ӣ�۵�HP��/ ʿ������
	//        Ȼ����������ı�����ʹ���ܵ��˺�ֵ(�ܵ��˺�ֵΪ�ϴ�ʣ����˺�ֵ������ε��˺�ֵ)������
	//        �� D���˺�ֵ��T��/ P
	//        ʣ�µ��˺�ֵΪ��T = T - D*P
	// @param attackerSoldierType ��������ʿ�������ͣ��������ʦ�����Ǽ�Ѫ
	// @parm hurt �˺�
	// @param heroHp Ӣ�۵�HP
	int processHurt(SOLDIER_TYPE attackerSoldierType, int& hurt);

	unsigned int processReduce(double reduceProba, int& hurt);

	SOLDIER_TYPE getType() const;

	unsigned int getWeaponId() const;

	// ���㹥����
	int computeAtk(bool isShowPower) const;

	// ���������
	int computeDefence(ATTACK_TYPE attackType) const;

	int getCurHurtHp() const;

	int getNum() const;

	int getCurrentHp() const;

	void resetHurt();

	//@brief ��Ӣ�۵ȼ�������ʱ
	void onHeroUpgradeLevel(int heroHp);

	// ���ܴ���������HP���ڸ�ѧ���������ʱ���ã���һ��
	void addHp(double effectVal);

	// �ڷ�������HP�ļ���ʱ����
	void reduceHp(double effectVal);

	// ������������HP
	void addHpForEndurance(int endurance);

	unsigned int getPhysicDefense() const
	{
		return static_cast<unsigned int>(soldierInfo_.physicDefense);
	}

	unsigned int getMagicDefense() const
	{
		return static_cast<unsigned int>(soldierInfo_.magicDefense);
	}

	unsigned int resetSoldierNum(unsigned int soldierNum);

	unsigned int addSoldierNum(unsigned int soldierNum);

	unsigned int getTotalHp() const;

	unsigned int suicide();

private:
	Soldiers();
	NO_COPYABLE_AND_ASSIGN(Soldiers);
private:
	SoldiersInfo soldierInfo_;
	// ɱ��һ��ʿ������Ҫ��HP
	int hpOfDeath_; 
	// �����ܵ�HP
	int totaoHp_;
	// �ۻ����˺�
	int totalHurt_;

	IRandomGenerator* randomGenerator_;
};
COMPUTINGHURT_NAMESPACE_END

#endif // COMPUTINGHURT_SOLDIER_H_
