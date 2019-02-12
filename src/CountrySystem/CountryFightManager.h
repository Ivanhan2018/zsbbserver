#pragma once
#include <queue>
#pragma warning(push)
#pragma warning(disable:4512)
#include "IceUtil.h"
#pragma warning(pop)
#include "FightManager.h"
#include "Army.h"
#include "PlayerManager.h"
#include "fightingstatistics/fightingstatisticsadapter.h"
#include "battlefieldstatisticsadapter.h"
#include "globalmacrodef.h"
#include "LimitQueue.h"
#include "TaskManager.h"
class IniFile;
class CLoadDBInfo;
class CLoadMapInfo;
class CNPCInfo;
class FightEvent;
class FightOverManager;
class SeekPath;
class ArmyManager;
class RenascenceMgr;
class BroadcastManager;
class TaskManager;
struct REQ_ENTER_NPC_SYSTEM_STRUCT;
struct REQ_ARMY_INFO_STRUCT;
struct Notify_Enter_Ectype;
struct ReqEctypeGetArmyInfo;
struct ReqMoveEctypeArmyInfo;
struct ReqAttackTargetInfo;
struct ReqSetEctypeArmyTrust;
struct REQ_ENTER_FIGHT_SYSTEM_STRUCT;
struct REQ_ARMY_GO_OUT_STRUCT;
struct tagDeleteArmy_Request;
struct tagFIGHT_CreateArmy_Request;
struct EDIT_ARMY;
struct COMEIN_MAP;
struct tagNotify_DelNPCArmy_Request;
struct TIMER_HEADER;
struct LOG_ONLINE_NUM_TIMER;
struct tagModify_PublicBuild_Request;


//��ս��������
//������ս���ͺ�ƽ״̬�µ�����
class CountryFightManager : public FightManager 
{
public:
	CountryFightManager(void);
public:
	~CountryFightManager(void);
public:
	//ս����������
	virtual bool processFightOver(int victoryCountryID) ;
	//����
	virtual bool scheduler(void);
	//ɾ���ض�����
	virtual void processDelArmy(IArmyPtr armyPtr , ARMY_TRUST_STATE state);
	//���õ�ͼ
	bool Initialize(CLoadMapInfo * mapInfo , CLoadDBInfo * dataBaseMemData , IniFile * iniFile);
	//�����㲥��Ϣ
	void msgBroadcast(DISTRIBUTED_HEADER * msgData , PROTOCOL_FIGHT fightCmd  , int dataLen);
	//�ͷ��ڴ�	
	void release();
	//֪ͨ���ͼ��Ҳ���״̬�޸�
	int notifyModifyArmyState(unsigned int playerid,unsigned int armyid, int mapid,int armyNum,int state,int weapon);
	//����Ӣ��
	void onCatchedHero(IArmy* army, HeroId heroId);
	// @brief ����ӪcampId�����palyerId��Ӣ��heroId��ץ��
	void onBeCatchedHero(IArmy* army, HeroId heroId);
	// @brief ����ӪcampId�����palyerId��Ӣ��heroId������
	void onHeroWasInjured(IArmy* army, HeroId heroId);
	// @brief ����Ӫcampid�����playerid�Ĳ���armyid������deathNum��ʿ��ʱ
	void onSoliderDie(IArmy * attackArmy ,IArmy* army, unsigned int deathNum);
	// @brief ����Ӫcampid�����playerid�Ĳ���armyidɱ���˶���killNum��ʿ��ʱ
	void onKilledEnemy(IArmy* army, unsigned int killNum);
	//����ͳ��ϵͳ
	void onEnterWar(IArmyPtr armyPtr);
	// @brief ����������ʱ
	void onArmyDie(IArmy* attackPtr , IArmy* army);
	// @brief ��������������һ�ñ���ʱ
	void onObtainBox(const IArmy* army, int num);
	//�ͷ�
	void enterCountry(ArmyPtrVect & armyPtrVect);
	//��ó������ӵ�λ��
	bool getOutArmyPos(int lastMapID , POS_STRUCT & pos);
	//��ô����ŵ�λ��
	bool getGatewayPos(int entraceID , POS_STRUCT & pos);
	//���ñ�ǩ����Ӫ��ӳ��
	virtual void setLabelMap(int attackCountry);
private:
	//����ͻ���moveָ��
	void processMove(REQ_MOVE_INFO_STRUCT* buf);
	//����ͻ��˹���ָ��
	void processAttack(REQ_ATTACK_TARGET_STRUCT* buf);
	//���󲿶���Ϣ
	void processReqArmyInfo(REQ_ARMY_INFO_STRUCT* lpReq);
	//����ս��ϵͳ
	void processEnterSystem(const REQ_ENTER_FIGHT_SYSTEM_STRUCT* lpReq);
	//�����й�
	void processTrusteeship(REQ_TRUST_INFO_STRUCT *reqMsg);
	//�뿪ս��ϵͳ
	void processLeaveSystem(REQ_LEAVE_FIGHT_SYSTEM_STRUCT * msgLeaveSystem);
	//�˳�ս��ϵͳ
	void processExitSystem(REQ_EXIT_FIGHT_SYSTEM_STRUCT * msgExitSystem);
	//�齨����
	void processCreateArmy(tagFIGHT_CreateArmy_Request * msgCreateArmy);
	//ɾ������
	void processDelArmyMsg(tagDeleteArmy_Request* msgDelArmy);
	//�����뿪��
	void processArmyLeave(EventArmyLeave * eventArmyLeave);
	//���ಿ��
	void processEditArmy(EDIT_ARMY * msgEditArmy);
	//���ӴӸ����лع�
	void processEctypeBack(Notify_Back_Army_Ectype * msgArmyBack);
	//���ӳ���
	void processArmyOut(REQ_ARMY_GO_OUT_STRUCT * msgArmyOut);
	//��Ӧ���ӳ���
	void processArmyOutRsp(tagBackArmy_Reach * eventArmyOutRsp);
	//���ӳ�������
	void processArmyReach(tagArmy_Reach * msgArmyReach);
	//ɾ���ض���ҵ����в���
	void processDelPlayerArms(tagNotify_DelNPCArmy_Request * msgPlayerInfo);
	//ѧϰ����
	void processHeroNewSkill(HeroStudySkillEvent* msgNewSkill);
	//�������
	void processHeroClearSkill(HeroStudySkillEvent* msgNewSkill);
	//ˢ��Ӣ�۲���
	void processUpdateHeroParam(heroModifyParamEvent* msgHeroParam);
	//��¼��ǰ������ҵ�����
	void processLogPlayerOnline(LOG_ONLINE_NUM_TIMER * msgTimer);
	//����������
	void processUpdateBuildLevel(tagModify_PublicBuild_Request * msgBuildData);
	//������ҳ���
	void processArmyRetreat(ReqEctypeRetreatArmyInfo* msgArmyRetreat);
	//������뿤��Ϣ
	void processEnterMap(COMEIN_MAP * msgEnterMap);
	//������ҽɻ�
	void processCapture(int winCountryID);
	//������ҽɻ���������
	void captureWeapons(std::map<unsigned int , std::map<unsigned int, int>>& playerWeapons,bool isRefresh);
	//������ҽɻ���Դ
	void captureResource(int attackCountryID);
	//Э�����
	void parse(const char * buf);
	//���ÿ���״̬
	void setMapState(mapstate state);
	//����ս��
	void startFight(bool isNeedSendReport , int attackCountry);
	//�����غ���
	void driveBoutNum(bool isIncreaceBout);
	//������ʱ��
	void startTimerCycle(unsigned int nt = FIGHT_TIMER_CYCLE);
	//�㲥�غ���Ϣ
	void broadcastTurnInfo(void);
	//�������ûغ���Ϣ
	void resetTurnInfo(void);
	//�߲��ӻ����ִ�
	bool kickArmyToNewbie(IArmyPtr armyPtr);
	//���Ͷ�����Ʒ���ʼ�
	void sendDiscardGoodsMail(unsigned int playerID , int countryId , string& playerName);
	//����ɧ�Ų���
	void processGuardArmy(void);
	//ɾ����ɧ�Ų���
	void processDelGuardArmy(void);

private:
	//ÿ���ڴ�������ӵ����� 
	int processActiveNum_;
	//�����ӪID
	int countryID_;
	//�����
	BroadcastManager * broadcastPackManager_;
	//ս��
	statisticssystem::BattlefieldStatisticsAdapter * fightReport_;
	//�ճ�����
	TaskManager* taskManager_;
private:
	NO_COPYABLE_AND_ASSIGN(CountryFightManager);
};
