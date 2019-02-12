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
	//获得当前回合数
	inline unsigned int getCurBoutNum() const{
		return curBoutNum_;
	}
	//获得当前周期数
	inline int getCurCycleNum() const{
		return curCycleNum_;
	}
	//获得寻路实例
	inline SeekPath * getSeekPather(void){
		return seekPather_;
	}
	//获得当前注册的进程标示
	inline int getProcessID( ) const{
		return curProcessID_;
	}
	//获得防守方的国家ID
	inline int getDefendCountryID(void){
		return getLabelCountryID(0);
	}
	//获得进攻方的国家ID
	inline int getAttackCountryID(void){
		return getLabelCountryID(1);
	}
	//中立方阵营ID
	inline int getNeutralCountryID(void){
		return getLabelCountryID(2);
	}
	//获得防守方人数
	inline int getDefendArmyNum(void) {
		return armyManager_->getDefendArmyNum();
	}
	//获得进攻方人数
	inline int getAttackArmyNum(void) {
		return armyManager_->getAttackArmyNum();
	}
	//获得武器防具相克Odds
	inline int getHurtOdds(int weaponType , int defendType){
		return npcInfo_->getHurtOdds(weaponType , defendType);
	}
	//获得当前地图ID
	inline int getMapID() const{
		return mapInfo_->getMapID();
	}
	//获得当前DBG代理
	inline DB_Proxy * getCurDBGProxy(){
		return dataBaseProxy_;
	}
	//获得统计实例
	inline statisticssystem::FightingStatisticsAdapter * getFightRecord(void){
		return fightRecord_;
	}
	//获得部队
	inline IArmyPtr getArmyByID(unsigned int armyID) const{
		return armyManager_->getArmyByID(armyID);
	}
	//添加部队到管理器
	inline bool addArmy(IArmyPtr armyPtr){
		return armyManager_->addArmy(armyPtr);
	}
	
	//将部队从管理器中清除
	inline void eraseArmy(IArmyPtr armyPtr){
		armyManager_->eraseArmy(armyPtr);
	}
	//获得特定位置的地形
	inline LAND_FORM getLandForm(int x , int y) const{
		if(mapInfo_->IsValidPos(x , y) == false){
			return LAND_INVALID;
		}
		return static_cast<LAND_FORM>(mapInfo_->MapInfo[x][y]);
	}
	//获得地图指针
	inline CLoadMapInfo *  getMapInfo(void) const{
		return mapInfo_;
	}
	//获得特定坐标
	std::list<IArmyPtr> * const getArmysInPos(int x , int y) const{
		return armyManager_->getArmysInPos(x , y);
	}
	inline vector<vector<list<IArmyPtr> > > * getArmyList(void) const {
		return armyManager_->getArmyList();
	}
	//是否GM部队
	inline bool isGMArmy(IArmyPtr army) const{
		if (army == NULL){
			return false;
		}
		return army->getPlayerID() == 6;
	}
	//是否GM指定玩家ID
	inline bool isGMPlayer(unsigned int playerId) const{
		return playerId == 6;
	}
	//获得随机数
	inline int generator(int from , int to) {
		return randomgenerator_->generator(from , to);
	}
	//获得地图状态
	mapstate getMapState(void) const {
		return mapInfo_->GetMapState();
	}
	//获得新手村ID
	inline int getNewbieID(int countryID) {
		switch(countryID){
		case UNION:
			return 1003;
		case CLAN:
			return 1007;
		}
		return -1;
	}
	//获得战斗系统场景
	inline ENUM_USE_SCENE getFightScene(void) const{
		return fightScene_;
	}
	//获得限制信息
	int getLimitValue(LIMIT_TYPE limitType , int limitKey){
		return npcInfo_->getLimitValue(limitType , limitKey);
	}
	//获得地形影响MAP
	inline LandformInfoMap const * getLandFormMap() const{
		return npcInfo_->getLandFormMap(); // 地形对部队影响的信息表
	}
	//获得战争类型
	inline GAME_HALL_TYPE getWarType(){
		return warType_;
	}
	//获得副本下限等级
	virtual int getEctypeDownLevel(void) const {
		return 0;
	}
	//获得副本上限等级
	virtual int getEctypeUpLevel(void) const {
		return 0;
	}
	//资源收集
	virtual void collectRes(IArmyPtr /*armyPtr*/){
		return ;
	}
	//设置标签到阵营的映射
	virtual void setLabelMap(int /*attackCountry*/){
		assert(0 && "非法调用");
	}
	//是否新手村
	bool isNewbie(void) const{
		return armyManager_->isNewbie();
	}
	// 获得军牌等级
	int getArmyCardLevel(int armyCardID) const;
	//根据军牌等级获得军牌ID
	int getArmyCardItem(int armyCardLevel) const;
	//获得防具信息
	DefendAttribute * getDefendAttribute(int defendID) const ;
	WeaponAttribute * getWeaponAttribute(int weaponID) const;
	// 函数描述：把死亡的NPC的ID和死亡时的周期数存入映射容器
	bool reclaimDeadArmy(IArmyPtr deadArmyPtr);
	//收集可复制部队
	void reclaimRepeatArmy(IArmyPtr armyPtr);
	//获得玩家名称
	string getPlayerName(unsigned int playerID , unsigned int officerID);
	//获得军团名称
	string getCropsName(unsigned int playerID);
	//获得AI等级参数
	aistrategy::AiLevelParam * getAiLevel(int aiLevel);
	//获得巡逻路径
	PatrolPosVect * getPatrolData(unsigned int armyID);
	//获得新DB实例
	DB_Proxy * getNewDbgProxy(void);
	//移动部队在沙盘上的位置
	bool moveArmy(IArmyPtr armyPtr , POS_STRUCT & targetPos);
	//插入处理队列
	void insertProcessQueue(IArmyPtr armyPtr , bool isDirectInsert = false);
	//从处理队列中弹出
	void eraseProcessQueue(unsigned int armyID);
	//处理玩家部队死亡
	bool processPlayerArmyDie(IArmy* armyPtr , int playerState , int realState);
	//处理缴获物品
	bool processBooty(IArmy * armyPtr , int dropItem , USE_PART cardType);
	//强制战斗系统中添加新部队
	bool forceInsertNewArmy(IArmyPtr armyPtr , bool isBroadcast = true) ;
	
	//通知大地图玩家部队状态修改
	int notifyModifyArmyState(unsigned int playerid,unsigned int armyid, int mapid,int armyNum,int state,int weapon);
	//是否有效部队
	bool isValidArmy(IArmyPtr armyPtr) const; 
	
	//是否在水中
	bool isInWater(const IArmy * armyPtr) const;
	//是否在森林中
	bool isInForest(const IArmy * armyPtr) const;
	//当前地图中插入用户
	void insertPlayerInfo(ActivityPlayerPtr activityPlayer);
	//广播添加部队信息
	void broadcastAddArmy(IArmyPtr armyPtr);
	////创建NPC部队
	//bool createNpcArmy(NpcArmyParam & npcArmyParam , unsigned int npcArmyID);
	//清除绑定NPC
	void clearAttachedNpc(IArmyPtr armyPtr ,bool isKillArmy);
	//绑定NPC
	bool attachNpc(IArmyPtr armyPtr , int templateID ,const POS_STRUCT & centryPos , ENUM_USE_SCENE state);
	//适配技能结构
	bool getSkillInfo(computinghurt::SkillInfo& skillInfo , SKILL_TYPE skillID , int skillLevel);
	//是否单人副本
	bool isSingleEctype(unsigned int ectypId);
	//run
	void run(void);
	//处理定时器消息
	void processTimerEvent(TIMER_HEADER * msgTimer);
	//记录当前地图在线玩家的数量
	void processLogPlayerOnline(LOG_ONLINE_NUM_TIMER * msgTimer);
	//获得绑定部队的实际坐标
	void TranslateCorrd(const POS_STRUCT & centryPos, POS_STRUCT & realPos ,int nOffsetRow,int nOffsetCol);

	//英雄被抓获
	virtual void onCatchedHero(IArmy* army, HeroId heroId) = 0;
	// @brief 当阵营campId的玩家palyerId的英雄heroId被抓获
	virtual void onBeCatchedHero(IArmy* army, HeroId heroId) = 0;
	// @brief 当阵营campId的玩家palyerId的英雄heroId受重伤
	virtual void onHeroWasInjured(IArmy* army, HeroId heroId) = 0;
	// @brief 当阵营campid的玩家playerid的部队armyid死亡了deathNum个士兵时
	virtual void onSoliderDie(IArmy * attackArmy , IArmy* army, unsigned int deathNum) = 0;

	// @brief 当阵营campid的玩家playerid的部队armyid杀死了短文killNum个士兵时
	virtual void onKilledEnemy(IArmy* army, unsigned int killNum) = 0;

	// @brief 当部队死亡时
	virtual void onArmyDie(IArmy* attackPtr , IArmy* army) = 0;
	//进入统计系统
	virtual void onEnterWar(IArmyPtr armyPtr) = 0;

	//广播信息
	virtual void msgBroadcast(DISTRIBUTED_HEADER * msgData , PROTOCOL_FIGHT fightCmd  , int dataLen) = 0;
	//释放内存
	virtual void release(void) = 0;
	//开启定时器
	virtual void startTimerCycle(unsigned int nt = FIGHT_TIMER_CYCLE) = 0;
	//驱动战斗回合数
	virtual void driveBoutNum(bool processMode) = 0;
	//删除特定部队
	virtual void processDelArmy(IArmyPtr armyPtr , ARMY_TRUST_STATE state);
	//释放
	virtual void enterCountry(std::vector<IArmyPtr> & armyPtrVect);
	//解析协议
	virtual void parse(const char * buf) = 0;
	//调度
	virtual bool scheduler(void) = 0;
	//判断战斗是否结束
	virtual bool isFightOver(int& victoryCountryID);
	//获得特定玩家信息
	ActivityPlayerPtr getPlayerInfo(unsigned int playerID) const;
	//响应英雄被捕信息
	void onCaptiveHero(IArmyPtr attackPtr , IArmyPtr armyPtr);
	//获得英雄阵营
	int getCountryID(unsigned int playerID);
	//处理军牌掉落
	bool processArmyCardDrop(IArmyPtr attackArmy , IArmyPtr army ,int armyCardID , int odds);
	//处理玩家军牌掉落
	void processPlayerDrop(IArmyPtr attackArmy , IArmyPtr army);
	//启动在线人数统计
	void startLogTimer(void);
	//获得标签的阵营
	int getLabelCountryID(int label);
	//通过阵营ID获得doorId;
	int getDoorId(int countryId);
	//激发特效
	unsigned int useEffect(EffectDetail& effectDetail);
	//消除特效
	bool  clearEffect(unsigned int effectId);
	//导出特效
	void exportEffect(unsigned int playerId , int agent);
	//获得指定ID的特效信息
	EffectDetail *getEffectByID(unsigned int effectId);
	//是否可以使用某卡片
	bool isCanUserCard(IArmyPtr armyPtr , PropCardConfig * propCardPtr);
	//使用某卡片
	void useCard(IArmyPtr armyPtr , CardType cardTpe);
	//获得一个新卡片实例
	PropCard * getNewCard(IArmyPtr armyPtr ,  PropCardConfig * propCardPtr);
	//获得日常任务的参数配置
	DayTask_DetailInfo * getDayTaskParam(int taskId);
	//获得New Army ID
	unsigned int getNewArmyID(void) ;
	//回收Army ID
	void reclaimArmyID(unsigned int armyID) ;
protected:
	//使用道具卡
	void processUsePropCard(ReqUsePropCard* buf);
	//重设军牌限制
	void resetArmyCardDropRecord(void);
	//使用补兵卡
	void processAddSodier(ReqUsePropCard* buf , IArmyPtr armyPtr , PropCardConfig* propCardParam);
	//使用加速卡
	void processSpeedCard(IArmyPtr armyPtr , PropCardConfig* propCardParam);
	//使用佣兵卡
	void processMercenaryCard(ReqUsePropCard* buf ,IArmyPtr armyPtr , PropCardConfig* propCardParam);
	//处理道具卡错误
	void processPropCardError(ReqUsePropCard* buf , int errCode);
protected:
	//最大回合数 
	int maxBoutNum_; 
	//当前回合数
	unsigned int curBoutNum_;
	//当前已处理部队的数量
	int processedNum_;
	//本回合需要处理的部队的数量
	int needProcessNum_;
	//每周期处理库活动部队的数量 
	int processActiveNum_;
	//空闲回合数
	int curIdleBoutNum_;
	//当前周期数
	int curCycleNum_; 
	//当前进程ID
	int curProcessID_;
	//是否需要强制退出、
	bool isForceExit_;
	//当前的运行状态
	bool isRun_; 

	
	//是否在处理队列中
	std::map<unsigned int , bool> isInProcessQueue_;
	// 部队，障碍物地图数据
	MapDataStru mapDataAndFraise_;	
	//配置文件指针
	IniFile * iniFile_;
	//寻路管理器
	SeekPath * seekPather_;
	//副本NPC部队信息
	CNPCInfo* npcInfo_;
	//DBG 缓存数据
	CLoadDBInfo *dataBaseMemData_;
	//当前地图
	CLoadMapInfo * mapInfo_;
	//DBG代理
	DB_Proxy * dataBaseProxy_; 
	//部队管理器
	ArmyManager * armyManager_;
	//玩家管理器
	PlayerManager * playerManager_;
	//战斗胜负管理器
	FightOverManager * fightOverManager_;
	//战斗事件
	FightEvent* fightEvent_;
	//繁殖复活管理器
	RenascenceMgr *renascenceMgr_;
	//巡逻数据
	map<unsigned int , PatrolPosVect*> * patrolMap_;
	//随机数
	computinghurt::IRandomGenerator* randomgenerator_;
	//部队处理队列
	std::queue<unsigned int> *curProcessQueue_;
	//运行队列
	std::queue<unsigned int> *processQueue_;
	//战斗统计
	statisticssystem::FightingStatisticsAdapter * fightRecord_;
	//
	static float lossRate_;
	//战斗系统场景
	ENUM_USE_SCENE fightScene_; //
	//掉落物品管理
	DropGoodsManager* dropGoodsManager_;
	GAME_HALL_TYPE warType_;//战斗类型（副本、限制战场、非限制战场、野外）
	map<int , DetailEntranceInfo> labelMap_; //标签到阵营的映射
	map<unsigned int , unsigned int> armysMap_; //部队映射
	//效果管理器
	EffectManager effectManager_;
	//道具使用管理器
	PropManager*   propManager_; 
private:
	NO_COPYABLE_AND_ASSIGN(FightManager);
};
