#include "testfightreport.h"
//#include "testreportcommon.h"

#if 0
class Army : public IArmy
{
public:
	virtual void attack(Armys& armys) = 0;
	// ��������ʱ
	virtual void beAttacked(int atk, int additionalHurt) = 0;
	// �����˺�
	virtual void processHurt(int hurt) = 0;
	// ֪ͨ�ͷ��˼���
	virtual void notifyRelease(SkillId id) = 0;
	// ��ɱ���˶Է�����ʱ
	virtual void onKilled(IArmy* army) = 0;
	// �õ�����������
	virtual int getWeaponAtk() const = 0;
	// �õ�ʿ��������
	virtual int getSoldierDefense() const = 0;
	// �õ���������
	virtual int getNumberOfSoldiers() const = 0;
	// �õ�����������
	virtual int getShootingRate() const = 0;
	// �õ���������
	virtual ArmyType getArmyType() const = 0;
	// �õ�������Ĺ�����
	virtual int getAtk() const = 0;
	// �õ������������
	virtual int getDefense() const = 0;
};
#endif

void TestFightReport::testReport()
{

}
