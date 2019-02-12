// @brief ��Ҫ����: ��ս�������������ط������������й�����
// @author ����: �� �� ��

#ifndef _COUNTY_QUEUE_MGR_
#define _COUNTY_QUEUE_MGR_

#include "LimitQueue.h"
class CountyQueueMgr
{
public:
	CountyQueueMgr(CLoadMapInfo * mapInfo, IniFile * iniFile, FightManager *fightMgrPtr, ArmyManager *armyMgrPtr);
	virtual ~CountyQueueMgr(void);

	// �����������Ƿ����ֱ�ӽ��뿤
	// @param armyPtr ������뿤�Ĳ���ָ��
	// @return true������ֱ�ӽ��뿤��false������ֱ�ӽ��뿤�����Զ��������
	bool isDirectEnterCounty(const IArmyPtr armyPtr);

	// ����������������
	// @return ���س��ӵĲ�����
	unsigned int quitQueue();

	// �����������ռ��ڶ��������еķ����ߵ���Ҳ���
	// @param armylstParam ���ص����з����ߵ���Ҳ����б�
	// @return �ɹ����
	bool collectAllOutOfLineArmy(ArmyPtrLst &armylstParam);

	// ����������ǿ�Ƴ���
	// @param playerID ���ID
	// @return ���س��ӵĲ�����
	unsigned int compelQuitQueue(unsigned int playerID);

	// ����������˳��ʱ����Ҫ��������ʱ���н���
	// @param winCountryID ʤ�����Ĺ���ID
	// @return ��
	void attackOrguardSwopOnWin(unsigned int winCountryID);

	// ��������������ָ����ҵ����в���
	// @param playerID ָ�������ID
	// @param armyLstParam ����ָ����ҵĲ���ָ���б�
	// @param isQuitQueue �Ƿ�ɾ������
	// @return ��
	void searchAllArmyOfSpeciPlayer(unsigned int playerID, ArmyPtrLst &armyLstParam, unsigned int countryId, bool isQuitQueue = false);

	// ��������������ָ����armyId��ArmyPtr
	// @param armyId ָ���Ĳ���ID
	// @return ����ָ��,û�ҵ�������NULL
	inline IArmyPtr getArmyPtrOnArmyId(ArmyID armyId)
	{
		IArmyPtr armyPtrTmp = guardSideQueuePtr_->getArmyPtrOnArmyId(armyId);

		if (NULL == armyPtrTmp)
		{
			armyPtrTmp = attackSideQueuePtr_->getArmyPtrOnArmyId(armyId);
		}

		return armyPtrTmp;
	}


	void enterQueue(const IArmyPtr armyPtr);

private: 
	// ������������ʼ��
	// @return ��
	void init();

	CLoadMapInfo * mapInfo_;
	FightManager *fightMgrPtr_;        // ս��������ָ��
	
	LimitQueue *attackSideQueuePtr_;	  // ����������
	LimitQueue *guardSideQueuePtr_;		  // ���ط�����
	unsigned int guardCountryId_;     // �ط��Ĺ���ID
};

#endif
