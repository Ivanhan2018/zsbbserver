#pragma once
#pragma warning(push)
#pragma warning(disable:4100)
#include "ProFightSystem.h"
#pragma warning(pop)
#include "ProNPCSystem.h"
#include "DBTableHeader.h"
#include "DB_ProxyHeader.h"
#include <map>
#include <vector>
#include "AiCommon.h"
using namespace std;
#define  MAX_ECTYPE_NAME_LEN 32


//NPC参数表
struct EctypeNpcTypeTable{
	unsigned int npcTypeID;       //副本部队类型ID
	char npcName[MAX_ECTYPE_NAME_LEN]; //部队名称
	int level;           //级别
	int raceID;          //种族ID
	int soldierType;     //士兵职业
	int attackPower;     //单兵攻击力
	int attackType;      //攻击类型
	int weaponType;      //武器类型
	int weaponID;       //武器ID
	int defendID; //防具ID
	int attackScope;      //攻击视野
	int magicDependPower; //单兵魔法防御
	int phsicDependPower; //单兵物理防御
	int soldierHp;        //单兵HP
	int movePerNeedTime;  //移动一格所需时间
	int viewScope;        //部队视野
	int dropExp;          //单兵掉落经验
	int skill;            //技能
	int skillLevel;       //技能等级
	int npcType; //NPC种类
};
//enum 
//0、防守
//1、进攻
//2、中立
enum NeutralState{
	STATE_DEFEND = 0, //隶属进攻方
	STATE_ATTACK,
	STATE_NEUTRAL
};

//副本NPC配置结构
struct EctypeNpcArmyTable{
	int ectypeID;         //副本ID
	unsigned int armyID;  //部队ID
	int npcTypeID ;       //NPC类型ID
	int isNeutral;        //是否中立
	int isSurely;         //是否必出
	int strategyType;    //策略类型
	int aiLevel;        //AI等级
	int soldierNum;      //士兵数量
	int posX;            //部队位置
	int posY; 
	int dropItem; //掉落物品Item
	int dropOdds; //掉落物品
	int enterTime;				// NPC进入战场时间
	int quitTime;				// NPC退出战场时间
	int  isInfinityReproduce;  // NPC是否无限繁殖
	unsigned int  adscription;			// 归属ID，死亡后其归属NPC不再刷新
	int rebornTime;			// NPC重生时间
	int armyCardsType;    //掉落军牌类型
	int cardDropOdds;     //军牌掉落概率
};

//建筑物守卫NPC的配置结构
struct NpcTemplate{
	int id; //模板ID
	int npcTypeID; // npc种类ID
	int strategyType; //策略
	int aiLevel;//AI种类
	int soldierNum; //数量
	int x; //相对坐标X
	int y; //相对坐标Y
	int dropItem;//掉落物品
	int dropOdds;//掉落概率
	int enterTime; // 进入时间
	int quitTime;//退出时间
	int	isInfinityReproduce; //是否复制
	int rebornTime;//重生间隔
	int armyCardsType;    //掉落军牌类型
	int cardDropOdds;     //军牌掉落概率
};

//郡内防骚扰，NPC的配置结构
struct NpcGuarderParam{
	int coutryID; //阵营ID
	int mapID; // 地图ID
	int npcID;// NPC ID 
	int strategy;//策略
	int aiLevel; //AI等级
	int npcNum;//NPC数量
	int dropItem; //掉落标示
	int dropOdds; //掉落概率
	int armyCard; //军牌标示
	int armyCardDropOdds;//军牌掉落概率
};
struct EctypeInfoTable{
	int ectypeID;  //副本ID
	int dLevel;   //副本下限等级
	int uLevel;     //副本上限级别
	int mapID;     //地图ID
	int npcArmyNum; //队伍数量下限
	int limitBout; //最大回合数
	char ectypeName[MAX_ECTYPE_NAME_LEN]; //副本名称
	int entranceID; //入口ID
	int limitArmyNum; //部队数量限制
	int limitPlayerNum; //玩家数量限制
	int warType;//场景类型
};
struct DerateHurtTable{
	int weaponID;
	int defendID;
	int attackOdds;
};
enum FightOverCmdEnum{
	KILL_TARGET = 1,      //杀死某目标
	PROTECTED_TARGET = 2, //保护某目标
	GET_DROP_ITEM = 3,     //获得某掉落物品
	BOUT_LIMIT = 4, // 回合限制
	GET_RES_LIMIT  =5// 资源收集
};


struct WarEntranceInfo{
	unsigned int buildId;
	int mapId;
	int label;
	int groupId;
};
struct FightOverCondition {
	int ectypeID; //副本ID
	FightOverCmdEnum cmdType;  //结束CMD类型 
	int targetID;   //目标
	int param;     //结束命令参数
	int condition; //条件 1，表示此条件完成副本结束  条件0此条件非关键条件
};

//道具卡使用配置
struct PropCardConfig{
	int itermId;
	int effect;
	int cardType;
	int cardValue;
	int lastTime;
	int ectypeLimit;
	int warLimit;
	int countryLimit;
	int fieldLimit;
	int isWarCanUse;
	int isPeaceCanUse;
	int coolingTime;
	int isArmyLimitUse;
};
typedef std::vector<FightOverCondition> FightOverConditionArray;
class CNPCInfo
{
public:
	CNPCInfo();
	~CNPCInfo(void);
	void InitNpc(DB_Proxy * dataBaseProxy);
	EctypeBaseTable* GetEctypInfo(int ectypeid);
	NPC* GetNpcInfo(int npcid);

	void empty();

	bool getEctypeNpc(int ectypeID , map<unsigned int , EctypeNpcArmyTable*> & ectypeNpcArms);
	//获得
	EctypeNpcTypeTable* getEctypeNpcType(int npcArmyTypeID);
	//获得副本信息
	EctypeInfoTable * getEctypeInfoTable(int entranceID);
	//获得巡逻数据
	map<unsigned int , PatrolPosVect*> * getPatrolData(int ectypeID);
	//获得附加NPC的参数ID
	vector<NpcTemplate> * getNpcTemplate(int templateID);
	//获得结束条件数据
	FightOverConditionArray* getFightOverCondition(int ectypeID);
	//获得AI等级参数
	aistrategy::AiLevelParam * getAiLevelParam(int aiLevel);
	//获得武器防具相克
	int getHurtOdds(int weaponType , int defendType);
	//获得副本建筑参数表
	PublicBuildParamTable * getEctypeBuildParam(int buildType , int buildLeve);
	//获得建筑物信息
	PublicBuildTable * getEctypeBuild(unsigned int buildID);
	//获得战场入口信息
	map<int , list<WarEntranceInfo *>>* getWarEntranceInfo(unsigned int mapid);
	//获得道具卡信息
	PropCardConfig * getPropCardInfo(int itemId);
	//获得防骚扰NPC的配置
	const list<NpcGuarderParam*>*getGuarderParam(int mapId , int countryID) const;
	//获得限制信息
	int getLimitValue(LIMIT_TYPE limitType , int limitKey);
	//获得地形影响信息
	aistrategy::LandformAffect * getLandFormAffect(LAND_FORM landForm, SOLDIER_TYPE soldierType);
	//获得地形影响MAP
	inline  LandformInfoMap const* getLandFormMap() const{
		return &landFormMap_; // 地形对部队影响的信息表
	}
protected:
	void loadEctypeNpcType();
	void loadEctypeNpc();
	bool loadEctypeNpc(int ectypeID , map<unsigned int , EctypeNpcArmyTable*> & ectypeNpcMap);
	void loadEctypeInfo();
	void loadPatrolInfo();
	bool loadPatrolInfo(int ectypeID , map<unsigned int , PatrolPosVect*> & patrolMap);
	void loadFightOverCondition();
	void loadAiLevel();
	void loadDerateHurt();
	void loadNpcTemplate();
	void loadEctypeBuildParam();
	void loadEctypeBuild();
	void loadLandFormMap(void); 
	void loadLimitTable(void);
	void loadWarEntranceInfo(void);
	void loadPropCardInfo(void);
	void loadGuarderParam(void);
	void clearEctypeNpcType();
	void clearEctypeNpc();
	void clearEctypeInfo();
	void clearPatrolInfo();
	void clearFightOverCondition();
	void clearAiLevel();
	void clearDerateHurt();
	void clearNpcTemplate();
	void clearNpcBuildParam();
	void clearEctypeBuild();
	void clearLandFormMap(void); 
	void clearLimitTable(void);
	void clearWarEntranceInfo(void);
	void clearPropCardInfo(void);
	void clearGuarderParam(void);
public:
	map<int ,EctypeBaseTable* > m_npcMapinfo;
	map<unsigned ,NPC*> m_NPCList; //npc 信息列表
	map<unsigned int , EctypeNpcTypeTable*>   ectypeNpcTypes_; //副本NPC类型
	map<int , map<unsigned int , EctypeNpcArmyTable*> > ectypeNpcArmys_; //副本所有NPC
	map<int , EctypeInfoTable*> ectypeInfos_; //副本信息
	map<int , map<unsigned int , PatrolPosVect*> > armyPatrolData_; //巡逻数据
	map<int , FightOverConditionArray*> fightOverConditionData_; //战斗结束条件
	map<int , aistrategy::AiLevelParam*> aiLevelParamData_; //AI等级参数数据
	map<int,map<int,int>> derateHurtMap_; //伤害减免数据
	map<int , vector<NpcTemplate> *> npcTemplateData_; //附加 NPC的模板数据
	map<int,map<int , PublicBuildParamTable*> > ectypeBuildParam_; //副本建筑参数表
	map<unsigned int , PublicBuildTable*> ectypeBuild_; // 副本建筑表
	LandformInfoMap landFormMap_; // 地形对部队影响的信息表
	map<std::pair<LIMIT_TYPE , int> , int>  limitValueMap_; //限制信息表
	map<int , map<int , list<WarEntranceInfo *>>> warEntranceMap_; //战场入口信息表
	map<int ,PropCardConfig*> propCardConfig_; //道具卡配置
	map<int , map<int , list<NpcGuarderParam*> > > npcGuardersParam_;//郡防骚扰NPC配置表
	//DBG代理
	DB_Proxy * dataBaseProxy_; 
};

