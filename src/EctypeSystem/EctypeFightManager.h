#pragma once
#include <queue>
#pragma warning(push)
#pragma warning(disable:4512)
#include "IceUtil.h"
#pragma warning(pop)
#include "FightManager.h"
#include "EctypeManager.h"
#include "fightingstatistics/fightingstatisticsadapter.h"
#include "globalmacrodef.h"
class CLoadDBInfo;
class CLoadMapInfo;
class CNPCInfo;
class FightEvent;
class FightOverManager;
class SeekPath;
class RenascenceMgr;
struct REQ_ENTER_NPC_SYSTEM_STRUCT;
struct REQ_ARMY_INFO_STRUCT;
struct Notify_Enter_Ectype;
struct ReqEctypeGetArmyInfo;
struct ReqMoveEctypeArmyInfo;
struct ReqAttackTargetInfo;
struct ReqSetEctypeArmyTrust;

class EctypeFightManager : public FightManager
{
public:
	EctypeFightManager(void);
public:
	~EctypeFightManager(void);
public:
	//��ø�����roomID
	inline int getRoomID() const {
		return roomID_;
	}
	//���÷����RoomID
	inline void setRoomID(int roomID){
		roomID_ = roomID;
	}
	
	
	//��ø����ȼ�
	virtual int getEctypeDownLevel(void) const {
		return ectypeInfo_->dLevel;
	}
	//��ø����ȼ�
	virtual int getEctypeUpLevel(void) const {
		return ectypeInfo_->uLevel;
	}
	//���õ�ͼ
	bool initialize(CLoadMapInfo * mapInfo,CLoadDBInfo *dataBaseMemData, CNPCInfo * ectypeNpcInfo ,
	Notify_Enter_Ectype* lpEvent,int roomID);
	//�����㲥��Ϣ
	void msgBroadcast(DISTRIBUTED_HEADER * msgData , PROTOCOL_FIGHT fightCmd  , int dataLen);
	//�ƶ�������ɳ���ϵ�λ��
	bool moveArmy(IArmyPtr armyPtr , POS_STRUCT & targetPos);
	//������Ҳ�������
	bool processPlayerArmyDie(IArmy* armyPtr , int playerState , int realState);
	//֪ͨ���ͼ��Ҳ���״̬�޸�
	int notifyModifyArmyState(unsigned int playerid,unsigned int armyid, int mapid,int armyNum,int state,int weapon);
	//���Ӣ�ۼ��ܴ����ĸ���
	int getSkillProbability(int skillType , int skillLevel);
	//�������Ӳ�����Ϣ
	void exportAddArmyInfo(IArmyPtr armyPtr ,ARMY_ADD& addArmyInfo);
	//����Ӣ��
	void onCatchedHero(IArmy* army, HeroId heroId);
	// @brief ����ӪcampId�����palyerId��Ӣ��heroId��ץ��
	// @param army ����
	// @param heroId Ӣ��ID
	void onBeCatchedHero(IArmy* army, HeroId heroId);

	// @brief ����ӪcampId�����palyerId��Ӣ��heroId������
	// @param army ����
	// @param heroId Ӣ��ID
	void onHeroWasInjured(IArmy* army, HeroId heroId);

	// @brief ����Ӫcampid�����playerid�Ĳ���armyid������deathNum��ʿ��ʱ
	// @param army ����
	// @param deathNum �ò�����������
	void onSoliderDie(IArmy * attackArmy , IArmy* army, unsigned int deathNum);

	// @brief ����Ӫcampid�����playerid�Ĳ���armyidɱ���˶���killNum��ʿ��ʱ
	// @param army ����
	// @param killNum �ò���ɱ������
	void onKilledEnemy(IArmy* army, unsigned int killNum);
	//ˢ����Դ��Ϣ
	void updateRes(void);
	//����Ƿ����в���
	bool isHaveArmy(unsigned int playerID) const;
	//�����Ҳ��ӵ�����
	int getArmyNum(unsigned int playerID) const;

	// @brief ����������ʱ
	// @param army ����
	void onArmyDie(IArmy* attackPtr , IArmy* army);
	//������ʱ��
	virtual void startTimerCycle(unsigned int nt = FIGHT_TIMER_CYCLE);
	//����ս���غ���
	virtual void driveBoutNum(bool isIncreaceBout);
	//����ͳ��ϵͳ
	virtual void onEnterWar(IArmyPtr armyPtr);
	//�ͷ��ڴ�
	virtual void release(void);
	//ս����������
	virtual bool processFightOver(int victoryCountryID) ;
	//����
	virtual bool scheduler();
	//�ж�ս���Ƿ����
	virtual bool isFightOver(int& victoryCountryID);
	//��Դ�ռ�
	virtual void collectRes(IArmyPtr armyPtr);

private:
	//����ͻ���moveָ��
	void processMove(ReqMoveEctypeArmyInfo* buf);
	//����ͻ��˵Ĺ���ָ��
	void processAttack(ReqAttackTargetInfo* buf);
	//�������󲿶���Ϣ
	void processReqArmyInfo(ReqEctypeGetArmyInfo* lpReq);
	//����ɾ������
	void processDelArmyMsg(ReqEctypeDeleteArmy* msgDelArmy);
	//�������ϵͳ
	void processEnterSystem(const REQ_ENTER_NPC_SYSTEM_STRUCT* lpReq);
	//�����й�
	void processTrusteeship(ReqSetEctypeArmyTrust *reqMsg);
	//������ҳ���
	void processArmyRetreat(ReqEctypeRetreatArmyInfo* msgArmyRetreat);
	//��������뿪����
	void processLeaveEctype(ReqLeaveEctypeSystem * msgLeaveEctype);
	//��������Ӳ���
	void processAddArmy(ReqAddArmyToRoom * msgAddArmys);
	//���������������Ϣ
	//void OnContributeInfo(CITY_INFO * cityInfo)
	//֪ͨ�������������ӽ��븱���ɹ�
	void notifyZillah(Notify_Enter_Ectype* msgEnterEctype);
	//����
	void parse(const char * buf);
	//�������߻ؿ���
	bool kickArmyOutEctype(vector<unsigned int>& players);
	//������߳�����
	bool kickPlayerOutEctype(vector<unsigned int>& players);
	//�жϸ����Ƿ��������
	bool isValidEctypeData(Notify_Enter_Ectype* msgReqCreate , EctypeInfoTable * ectypeInfo);
	//������ǩ����Ӫ��ӳ��
	void createLabelMap(map<int , int>& countryMap , map<int , list<WarEntranceInfo *>>* warEntrance);
	
private:
	//��ǰ�������
	int ectypeID_;
	//���ID
	int entranceID_;
	//��ǰ����ID
	int roomID_;
	EctypeInfoTable * ectypeInfo_; //����������
	//
	map<int , map<int , int>>  resCollect_; //��Դ
	int resCount_; //��Դ��¼������
private:
	NO_COPYABLE_AND_ASSIGN(EctypeFightManager);
};

