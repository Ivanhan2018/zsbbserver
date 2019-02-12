#pragma once
#include <queue>
#pragma warning(push)
#pragma warning(disable:4512)
#include "IceUtil.h"
#pragma warning(pop)
#include "FightSystemMacro.h"
#include "FightSystemHeader.h"
#include "fightingstatistics/fightingstatisticsadapter.h"
#include "NPCInfo.h"
#include "globalmacrodef.h"
#include "AiCommon.h"
#include "Army.h"
#include "ArmyManager.h"
#include "DropGoodsManager.h"
#include "EffectManager.h"
#include "PropManager.h"
class CLoadDBInfo;
class PlayerManager;
class IniFile;
class SeekPath;
class FightOverManager;
class FightEvent;
class RenascenceMgr;

struct TIMER_HEADER;
struct LOG_ONLINE_NUM_TIMER;
struct ReqUsePropCard;



struct DetailEntranceInfo{
	int label;
	int countryId;
	int doorId;
};
class FightManager : public IceUtil::Task<char *> 
{
public:
	FightManager(void);
public:
	~FightManager(void);
public:
	//��õ�ǰ�غ���
	inline unsigned int getCurBoutNum() const{
		return curBoutNum_;
	}
	//��õ�ǰ������
	inline int getCurCycleNum() const{
		return curCycleNum_;
	}
	//���Ѱ·ʵ��
	inline SeekPath * getSeekPather(void){
		return seekPather_;
	}
	//��õ�ǰע��Ľ��̱�ʾ
	inline int getProcessID( ) const{
		return curProcessID_;
	}
	//��÷��ط��Ĺ���ID
	inline int getDefendCountryID(void){
		return getLabelCountryID(0);
	}
	//��ý������Ĺ���ID
	inline int getAttackCountryID(void){
		return getLabelCountryID(1);
	}
	//��������ӪID
	inline int getNeutralCountryID(void){
		return getLabelCountryID(2);
	}
	//��÷��ط�����
	inline int getDefendArmyNum(void) {
		return armyManager_->getDefendArmyNum();
	}
	//��ý���������
	inline int getAttackArmyNum(void) {
		return armyManager_->getAttackArmyNum();
	}
	//��������������Odds
	inline int getHurtOdds(int weaponType , int defendType){
		return npcInfo_->getHurtOdds(weaponType , defendType);
	}
	//��õ�ǰ��ͼID
	inline int getMapID() const{
		return mapInfo_->getMapID();
	}
	//��õ�ǰDBG����
	inline DB_Proxy * getCurDBGProxy(){
		return dataBaseProxy_;
	}
	//���ͳ��ʵ��
	inline statisticssystem::FightingStatisticsAdapter * getFightRecord(void){
		return fightRecord_;
	}
	//��ò���
	inline IArmyPtr getArmyByID(unsigned int armyID) const{
		return armyManager_->getArmyByID(armyID);
	}
	//��Ӳ��ӵ�������
	inline bool addArmy(IArmyPtr armyPtr){
		return armyManager_->addArmy(armyPtr);
	}
	
	//�����Ӵӹ����������
	inline void eraseArmy(IArmyPtr armyPtr){
		armyManager_->eraseArmy(armyPtr);
	}
	//����ض�λ�õĵ���
	inline LAND_FORM getLandForm(int x , int y) const{
		if(mapInfo_->IsValidPos(x , y) == false){
			return LAND_INVALID;
		}
		return static_cast<LAND_FORM>(mapInfo_->MapInfo[x][y]);
	}
	//��õ�ͼָ��
	inline CLoadMapInfo *  getMapInfo(void) const{
		return mapInfo_;
	}
	//����ض�����
	std::list<IArmyPtr> * const getArmysInPos(int x , int y) const{
		return armyManager_->getArmysInPos(x , y);
	}
	inline vector<vector<list<IArmyPtr> > > * getArmyList(void) const {
		return armyManager_->getArmyList();
	}
	//�Ƿ�GM����
	inline bool isGMArmy(IArmyPtr army) const{
		if (army == NULL){
			return false;
		}
		return army->getPlayerID() == 6;
	}
	//�Ƿ�GMָ�����ID
	inline bool isGMPlayer(unsigned int playerId) const{
		return playerId == 6;
	}
	//��������
	inline int generator(int from , int to) {
		return randomgenerator_->generator(from , to);
	}
	//��õ�ͼ״̬
	mapstate getMapState(void) const {
		return mapInfo_->GetMapState();
	}
	//������ִ�ID
	inline int getNewbieID(int countryID) {
		switch(countryID){
		case UNION:
			return 1003;
		case CLAN:
			return 1007;
		}
		return -1;
	}
	//���ս��ϵͳ����
	inline ENUM_USE_SCENE getFightScene(void) const{
		return fightScene_;
	}
	//���������Ϣ
	int getLimitValue(LIMIT_TYPE limitType , int limitKey){
		return npcInfo_->getLimitValue(limitType , limitKey);
	}
	//��õ���Ӱ��MAP
	inline LandformInfoMap const * getLandFormMap() const{
		return npcInfo_->getLandFormMap(); // ���ζԲ���Ӱ�����Ϣ��
	}
	//���ս������
	inline GAME_HALL_TYPE getWarType(){
		return warType_;
	}
	//��ø������޵ȼ�
	virtual int getEctypeDownLevel(void) const {
		return 0;
	}
	//��ø������޵ȼ�
	virtual int getEctypeUpLevel(void) const {
		return 0;
	}
	//��Դ�ռ�
	virtual void collectRes(IArmyPtr /*armyPtr*/){
		return ;
	}
	//���ñ�ǩ����Ӫ��ӳ��
	virtual void setLabelMap(int /*attackCountry*/){
		assert(0 && "�Ƿ�����");
	}
	//�Ƿ����ִ�
	bool isNewbie(void) const{
		return armyManager_->isNewbie();
	}
	// ��þ��Ƶȼ�
	int getArmyCardLevel(int armyCardID) const;
	//���ݾ��Ƶȼ���þ���ID
	int getArmyCardItem(int armyCardLevel) const;
	//��÷�����Ϣ
	DefendAttribute * getDefendAttribute(int defendID) const ;
	WeaponAttribute * getWeaponAttribute(int weaponID) const;
	// ������������������NPC��ID������ʱ������������ӳ������
	bool reclaimDeadArmy(IArmyPtr deadArmyPtr);
	//�ռ��ɸ��Ʋ���
	void reclaimRepeatArmy(IArmyPtr armyPtr);
	//����������
	string getPlayerName(unsigned int playerID , unsigned int officerID);
	//��þ�������
	string getCropsName(unsigned int playerID);
	//���AI�ȼ�����
	aistrategy::AiLevelParam * getAiLevel(int aiLevel);
	//���Ѳ��·��
	PatrolPosVect * getPatrolData(unsigned int armyID);
	//�����DBʵ��
	DB_Proxy * getNewDbgProxy(void);
	//�ƶ�������ɳ���ϵ�λ��
	bool moveArmy(IArmyPtr armyPtr , POS_STRUCT & targetPos);
	//���봦�����
	void insertProcessQueue(IArmyPtr armyPtr , bool isDirectInsert = false);
	//�Ӵ�������е���
	void eraseProcessQueue(unsigned int armyID);
	//������Ҳ�������
	bool processPlayerArmyDie(IArmy* armyPtr , int playerState , int realState);
	//����ɻ���Ʒ
	bool processBooty(IArmy * armyPtr , int dropItem , USE_PART cardType);
	//ǿ��ս��ϵͳ������²���
	bool forceInsertNewArmy(IArmyPtr armyPtr , bool isBroadcast = true) ;
	
	//֪ͨ���ͼ��Ҳ���״̬�޸�
	int notifyModifyArmyState(unsigned int playerid,unsigned int armyid, int mapid,int armyNum,int state,int weapon);
	//�Ƿ���Ч����
	bool isValidArmy(IArmyPtr armyPtr) const; 
	
	//�Ƿ���ˮ��
	bool isInWater(const IArmy * armyPtr) const;
	//�Ƿ���ɭ����
	bool isInForest(const IArmy * armyPtr) const;
	//��ǰ��ͼ�в����û�
	void insertPlayerInfo(ActivityPlayerPtr activityPlayer);
	//�㲥��Ӳ�����Ϣ
	void broadcastAddArmy(IArmyPtr armyPtr);
	////����NPC����
	//bool createNpcArmy(NpcArmyParam & npcArmyParam , unsigned int npcArmyID);
	//�����NPC
	void clearAttachedNpc(IArmyPtr armyPtr ,bool isKillArmy);
	//��NPC
	bool attachNpc(IArmyPtr armyPtr , int templateID ,const POS_STRUCT & centryPos , ENUM_USE_SCENE state);
	//���似�ܽṹ
	bool getSkillInfo(computinghurt::SkillInfo& skillInfo , SKILL_TYPE skillID , int skillLevel);
	//�Ƿ��˸���
	bool isSingleEctype(unsigned int ectypId);
	//run
	void run(void);
	//����ʱ����Ϣ
	void processTimerEvent(TIMER_HEADER * msgTimer);
	//��¼��ǰ��ͼ������ҵ�����
	void processLogPlayerOnline(LOG_ONLINE_NUM_TIMER * msgTimer);
	//��ð󶨲��ӵ�ʵ������
	void TranslateCorrd(const POS_STRUCT & centryPos, POS_STRUCT & realPos ,int nOffsetRow,int nOffsetCol);

	//Ӣ�۱�ץ��
	virtual void onCatchedHero(IArmy* army, HeroId heroId) = 0;
	// @brief ����ӪcampId�����palyerId��Ӣ��heroId��ץ��
	virtual void onBeCatchedHero(IArmy* army, HeroId heroId) = 0;
	// @brief ����ӪcampId�����palyerId��Ӣ��heroId������
	virtual void onHeroWasInjured(IArmy* army, HeroId heroId) = 0;
	// @brief ����Ӫcampid�����playerid�Ĳ���armyid������deathNum��ʿ��ʱ
	virtual void onSoliderDie(IArmy * attackArmy , IArmy* army, unsigned int deathNum) = 0;

	// @brief ����Ӫcampid�����playerid�Ĳ���armyidɱ���˶���killNum��ʿ��ʱ
	virtual void onKilledEnemy(IArmy* army, unsigned int killNum) = 0;

	// @brief ����������ʱ
	virtual void onArmyDie(IArmy* attackPtr , IArmy* army) = 0;
	//����ͳ��ϵͳ
	virtual void onEnterWar(IArmyPtr armyPtr) = 0;

	//�㲥��Ϣ
	virtual void msgBroadcast(DISTRIBUTED_HEADER * msgData , PROTOCOL_FIGHT fightCmd  , int dataLen) = 0;
	//�ͷ��ڴ�
	virtual void release(void) = 0;
	//������ʱ��
	virtual void startTimerCycle(unsigned int nt = FIGHT_TIMER_CYCLE) = 0;
	//����ս���غ���
	virtual void driveBoutNum(bool processMode) = 0;
	//ɾ���ض�����
	virtual void processDelArmy(IArmyPtr armyPtr , ARMY_TRUST_STATE state);
	//�ͷ�
	virtual void enterCountry(std::vector<IArmyPtr> & armyPtrVect);
	//����Э��
	virtual void parse(const char * buf) = 0;
	//����
	virtual bool scheduler(void) = 0;
	//�ж�ս���Ƿ����
	virtual bool isFightOver(int& victoryCountryID);
	//����ض������Ϣ
	ActivityPlayerPtr getPlayerInfo(unsigned int playerID) const;
	//��ӦӢ�۱�����Ϣ
	void onCaptiveHero(IArmyPtr attackPtr , IArmyPtr armyPtr);
	//���Ӣ����Ӫ
	int getCountryID(unsigned int playerID);
	//������Ƶ���
	bool processArmyCardDrop(IArmyPtr attackArmy , IArmyPtr army ,int armyCardID , int odds);
	//������Ҿ��Ƶ���
	void processPlayerDrop(IArmyPtr attackArmy , IArmyPtr army);
	//������������ͳ��
	void startLogTimer(void);
	//��ñ�ǩ����Ӫ
	int getLabelCountryID(int label);
	//ͨ����ӪID���doorId;
	int getDoorId(int countryId);
	//������Ч
	unsigned int useEffect(EffectDetail& effectDetail);
	//������Ч
	bool  clearEffect(unsigned int effectId);
	//������Ч
	void exportEffect(unsigned int playerId , int agent);
	//���ָ��ID����Ч��Ϣ
	EffectDetail *getEffectByID(unsigned int effectId);
	//�Ƿ����ʹ��ĳ��Ƭ
	bool isCanUserCard(IArmyPtr armyPtr , PropCardConfig * propCardPtr);
	//ʹ��ĳ��Ƭ
	void useCard(IArmyPtr armyPtr , CardType cardTpe);
	//���һ���¿�Ƭʵ��
	PropCard * getNewCard(IArmyPtr armyPtr ,  PropCardConfig * propCardPtr);
	//����ճ�����Ĳ�������
	DayTask_DetailInfo * getDayTaskParam(int taskId);
	//���New Army ID
	unsigned int getNewArmyID(void) ;
	//����Army ID
	void reclaimArmyID(unsigned int armyID) ;
protected:
	//ʹ�õ��߿�
	void processUsePropCard(ReqUsePropCard* buf);
	//�����������
	void resetArmyCardDropRecord(void);
	//ʹ�ò�����
	void processAddSodier(ReqUsePropCard* buf , IArmyPtr armyPtr , PropCardConfig* propCardParam);
	//ʹ�ü��ٿ�
	void processSpeedCard(IArmyPtr armyPtr , PropCardConfig* propCardParam);
	//ʹ��Ӷ����
	void processMercenaryCard(ReqUsePropCard* buf ,IArmyPtr armyPtr , PropCardConfig* propCardParam);
	//������߿�����
	void processPropCardError(ReqUsePropCard* buf , int errCode);
protected:
	//���غ��� 
	int maxBoutNum_; 
	//��ǰ�غ���
	unsigned int curBoutNum_;
	//��ǰ�Ѵ����ӵ�����
	int processedNum_;
	//���غ���Ҫ����Ĳ��ӵ�����
	int needProcessNum_;
	//ÿ���ڴ�������ӵ����� 
	int processActiveNum_;
	//���лغ���
	int curIdleBoutNum_;
	//��ǰ������
	int curCycleNum_; 
	//��ǰ����ID
	int curProcessID_;
	//�Ƿ���Ҫǿ���˳���
	bool isForceExit_;
	//��ǰ������״̬
	bool isRun_; 

	
	//�Ƿ��ڴ��������
	std::map<unsigned int , bool> isInProcessQueue_;
	// ���ӣ��ϰ����ͼ����
	MapDataStru mapDataAndFraise_;	
	//�����ļ�ָ��
	IniFile * iniFile_;
	//Ѱ·������
	SeekPath * seekPather_;
	//����NPC������Ϣ
	CNPCInfo* npcInfo_;
	//DBG ��������
	CLoadDBInfo *dataBaseMemData_;
	//��ǰ��ͼ
	CLoadMapInfo * mapInfo_;
	//DBG����
	DB_Proxy * dataBaseProxy_; 
	//���ӹ�����
	ArmyManager * armyManager_;
	//��ҹ�����
	PlayerManager * playerManager_;
	//ս��ʤ��������
	FightOverManager * fightOverManager_;
	//ս���¼�
	FightEvent* fightEvent_;
	//��ֳ���������
	RenascenceMgr *renascenceMgr_;
	//Ѳ������
	map<unsigned int , PatrolPosVect*> * patrolMap_;
	//�����
	computinghurt::IRandomGenerator* randomgenerator_;
	//���Ӵ������
	std::queue<unsigned int> *curProcessQueue_;
	//���ж���
	std::queue<unsigned int> *processQueue_;
	//ս��ͳ��
	statisticssystem::FightingStatisticsAdapter * fightRecord_;
	//
	static float lossRate_;
	//ս��ϵͳ����
	ENUM_USE_SCENE fightScene_; //
	//������Ʒ����
	DropGoodsManager* dropGoodsManager_;
	GAME_HALL_TYPE warType_;//ս�����ͣ�����������ս����������ս����Ұ�⣩
	map<int , DetailEntranceInfo> labelMap_; //��ǩ����Ӫ��ӳ��
	map<unsigned int , unsigned int> armysMap_; //����ӳ��
	//Ч��������
	EffectManager effectManager_;
	//����ʹ�ù�����
	PropManager*   propManager_; 
private:
	NO_COPYABLE_AND_ASSIGN(FightManager);
};
