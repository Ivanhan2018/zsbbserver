// @brief ��Ҫ����: ������������ʵ��
// @author ����: �� �� ��

#ifndef _LIMIT_MAN_NUM_QUEUE
#define _LIMIT_MAN_NUM_QUEUE

#include <list>
#include <set>
#include <map>
#include <vector>
#include <assert.h>
#include "boost\shared_ptr.hpp"
#include "IArmy.h"
#include "LoadMapInfo.h"
#include "ArmyManager.h"
#include "FightManager.h"

typedef std::list<IArmyPtr> ArmyPtrLst;
typedef unsigned int ArmyID;
typedef std::map<ArmyID, IArmyPtr> ArmyId2armyPtr;
typedef std::set<unsigned int> PlayerIdSet;
typedef std::map<unsigned int /*���ID*/, std::set<unsigned int /*����ID*/> > PlayerID2ArmyIDs;
typedef std::vector<IArmyPtr> ArmyPtrVect;


class FightManager;
class IniFile;
class CLoadMapInfo;
class LimitQueue
{
public:
	LimitQueue(CLoadMapInfo * mapInfo, IniFile * iniFile, FightManager *fightMgrPtr, ArmyManager *armyMgrPtr);
	virtual ~LimitQueue(void);

	// �����������Ƿ����ֱ�ӽ��뿤
	// @param armyPtr ������뿤�Ĳ���ָ��
	// @return true������ֱ�ӽ��뿤��false������ֱ�ӽ��뿤�����Զ��������
	bool isDirectEnterCounty(const IArmyPtr armyPtr, int curArmyTolInCounty);

	// ����������������
	// @return ���س��ӵĲ�����
	unsigned int quitQueue(int curArmyTolInCounty);

	// �����������ռ��ڶ��������еķ����ߵ���Ҳ���
	// @param armylstParam ���ص����з����ߵ���Ҳ����б�
	// @return �ɹ����
	bool collectAllOutOfLineArmy(ArmyPtrLst &armylstParam);
	
	// ����������ǿ�Ƴ���
	// @param playerID ���ID
	// @return ���س��ӵĲ�����
	unsigned int compelQuitQueue(unsigned int playerID);

	// ��������������ָ����ҵ����в���
	// @param playerID ָ�������ID
	// @param armyLstParam ����ָ����ҵĲ���ָ���б�
	// @return ��
	void searchAllArmyOfSpeciPlayer(unsigned int playerID, ArmyPtrLst &armyLstParam, unsigned int countryId, bool isQuitQueue);

	// �������������ָ����armyId�Ĳ���ʵ��
	// @param armyId ָ���Ĳ���ID
	// @return ����ָ��,û�ҵ�������NULL
	inline IArmyPtr getArmyPtrOnArmyId(ArmyID armyId)
	{
		ArmyId2armyPtr::const_iterator it = armyId2Ptr_.find(armyId);

		if (armyId2Ptr_.end() != it)
		{
			return it->second;
		}
		else
		{
			return NULL;
		}
	}

	// �����������ж�ָ������Ƿ��ڸö�����
	// @param playerID ָ�������ID
	// @return ���ڴ���ң�������true�����򣬷���false
	bool speciPlayerInQueue(unsigned int playerID);
	
	// ������������ȡǿ�Ƴ��Ӻ���Ҫ�նӵ�����
	// @return ����Ӧ���նӵ�����
	inline int getCanQuitNum(int countryId)
	{
		if (0 == countryId)
		{
//#ifdef _DEBUG	
//			assert(0);
//#endif	
			return 0;
		}

		if (NULL == fightMgrPtr_)
		{
#ifdef _DEBUG	
			assert(0);
#endif	
			return 0;
		}

		mapstate state = mapInfo_->GetMapState();
		int canQuitQueArmyNum = 0;
		int curArmyTolInCounty = 0;
		int guardCountryId_ = mapInfo_->GetMapOwer(); // ���ط�

		if (countryId == guardCountryId_)
		{
			curArmyTolInCounty = fightMgrPtr_->getDefendArmyNum();
		}
		else
		{
			curArmyTolInCounty = fightMgrPtr_->getAttackArmyNum();
		}

		if (war != state)
		{
			canQuitQueArmyNum = curArmyTolInCounty - totalArmyCountInPeace_;
		}
		else
		{
			canQuitQueArmyNum = curArmyTolInCounty - totalArmyCountInWar_;
		}

		return canQuitQueArmyNum;
	}

	// ���������������
	// @param armyPtr ������뿤�Ĳ���ָ��
	// @return ��
	void enterQueue(const IArmyPtr armyPtr);

private:
	// ������������ʼ��
	// @return ��
	void init();

	CLoadMapInfo * mapInfo_;
	IniFile * iniFile_;
	FightManager *fightMgrPtr_; 
	ArmyManager *armyMgrPtr_;

	unsigned int totalArmyCountInWar_;		  // ս��ʱ�ڣ������ܹ����ڵ���󲿶���
	unsigned int totalArmyCountInPeace_;      // ��ƽʱ�ڣ������ܹ����ڵ���󲿶���

	ArmyPtrLst lstArmy_;					  // �����еĲ����б�
	ArmyId2armyPtr armyId2Ptr_;               // �ڶ����е�armyId--->armyPtr��ӳ������
		
	PlayerIdSet playerIdSet_;				  // �ڶ����е����ID����

	//PlayerID2ArmyIDs playerID2ArmyIDset_;     // �ڿ��е����ID--->����ID����
};

#endif
