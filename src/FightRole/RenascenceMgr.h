// @brief ��Ҫ����: NPC����������
// @author ����: �� �� ��

#ifndef _SHIREFIGHT_RENASCENCEMGR_H_H_H_H
#define _SHIREFIGHT_RENASCENCEMGR_H_H_H_H

#include <set>
#include <map>
#include "FightManager.h"

typedef unsigned int DeadCycleNum;

typedef std::map<unsigned int /* NPC Army ID */, IArmyPtr> DeadArmyMap;
class BitMap;
class RenascenceMgr
{
public:
	RenascenceMgr(unsigned int baseStartArmyID , unsigned int baseEndArmyID , 
				   FightManager *fightMgrPtrParam);

	virtual ~RenascenceMgr(void);

	// ����������ʵ��NPC��������/��ʱ��ֳ
	// @return ����NPC�����ͷ�ֳ������
	unsigned int getRebornAndReproNpc(ENUM_USE_SCENE state);

	// ������������������NPC��ID������ʱ������������ӳ������
	// @return ��
	bool insertDeadArmy(IArmyPtr deadArmyPtr);
	//�ռ����Ʋ��ӵĻ�����
	void insertRepeatArmy(IArmyPtr deadArmyPtr);
	//����ض�����
	void eraseArmy(unsigned int armyID);
	//��ȡ����ID
	unsigned int getNewArmyID(void);
	//���ղ���ID
	void reclaimArmyID(unsigned int armyID) ;
private:
	//�������NPC
	unsigned int getRebornNpc(void);
	//��ø���NPC
	unsigned int getRepeatNpc(ENUM_USE_SCENE state);
private:
	RenascenceMgr & operator = (const RenascenceMgr & renascenceMgrParam);
	RenascenceMgr(const RenascenceMgr & renascenceMgrParam);
	
	FightManager * fightMgrPtr_;				// ս��ϵͳ������ָ��
	BitMap * bitMap_; //����ID����

	DeadArmyMap deadArmySet_;			// NPC�����Ĳ���IDӳ������(armyid ----> CurCycleNum)
	std::map<unsigned int /* NPC Army ID */, IArmyPtr>  repeatArmySet_; // ���Ʋ��ӹ�����
	//
};

#endif

