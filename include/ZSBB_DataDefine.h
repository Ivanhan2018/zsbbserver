#ifndef ZSBB_DATADEFINE_H__
#define ZSBB_DATADEFINE_H__
#include <map>
#include <vector>
#include "Global_Macro.h"
enum Error_Code
{
	ERR_SUCCESS = 0,			//成功
};

typedef struct tagQuantityByRank_Item
{
	int Rank;					//军衔
	int HeroQuantity;			//英雄数量
	int TownQuantity;			//城市数量
}QuantityByRank_Item,*LPQuantityByRank_Item;
typedef std::map<int,LPQuantityByRank_Item> QuantityByRank_Map;
typedef QuantityByRank_Map::iterator QuantityByRank_Map_Iterator;

typedef struct tagSoldierarmorconfig_Item
{
	int DefendID;//防具ID
	int Soldier;//战士
	int Shooter;//射手
	int Churchman;//牧师
	int Rabbi;//法师
}Soldierarmorconfig_Item,*LPSoldierarmorconfig_Item;
typedef std::map<int,LPSoldierarmorconfig_Item> Soldierarmorconfig_Map;
typedef Soldierarmorconfig_Map::iterator Soldierarmorconfig_Map_Iterator;

typedef struct tagPlayerInitArmyParam_Item
{
	int CountryID;
	int HeroJobID;
	int RaceID;
	int SoldierType;
	int WeaponType;
	int WeaponLevel;
	int DefendType;
	int DefendLevel;
	int Count;
}PlayerInitArmyParam_Item,*LPPlayerInitArmyParam_Item;
typedef std::map<int,LPPlayerInitArmyParam_Item> PlayerInitArmyParam_Map;
typedef PlayerInitArmyParam_Map::iterator PlayerInitArmyParam_Map_Iterator;

typedef struct tagGMCreateNPCParam_Item
{
	int CountryID;
	int Level;
	int PlayerID;
	int OfficerID;
	char AmyName[SIZE_24];
	int RaceID;
	int SoldierType;
	int WeaponType;
	int WeaponLevel;
	int DefendType;
	int DefendLevel;
	int SoldierCount;
	int AymyCount;
	int Price;
}GMCreateNPCParam_Item,*LPGMCreateNPCParam_Item;
typedef std::multimap<int,LPGMCreateNPCParam_Item> GMCreateNPCParam_MultiMap;
typedef std::multimap<int,LPGMCreateNPCParam_Item>::iterator GMCreateNPCParam_MultiMap_Iterator;
typedef std::pair<int, LPGMCreateNPCParam_Item> GMCreateNPCParam_MultiMap_Pair;
typedef std::map<int,LPGMCreateNPCParam_Item> GMCreateNPCParam_Map;
typedef GMCreateNPCParam_Map::iterator GMCreateNPCParam_Map_Iterator;

typedef struct tagCardParam
{
	int			CardID;				//从1000开始,便于与英雄装备区分。
	char		CardName[SIZE_32];	//卡的名称
	CardType	Type;				//类型，参考CardType
	int			Price;				//价格（需消耗的Go点）
	int			Value;				//可购买的数量或装备编号，与CardType有关
	int			Time;				//卡的持续时间，单位：秒
	int			Postion;			//装备位置（1-11为英雄装备）
	bool        OpenEnabled;		//是否可以打开
}CardParam,*LPCardParam;
typedef std::map<int,LPCardParam> CardParam_Map;
typedef std::map<int,LPCardParam>::iterator CardParam_Map_Iterator;

typedef struct tagSpeedParam_Node
{
	int TypeID;
	int Value;
	int Price;
	int Times;
}SpeedParam_Node,*LPSpeedParam_Node;
typedef std::map<int,LPSpeedParam_Node>	SpeedParam_Map;
typedef std::map<int,LPSpeedParam_Node>::iterator SpeedParam_Map_Iterator;

typedef struct tagMailInfoParam_Item
{
	int CounryID;//阵营
	int Type;//邮件类型编号
	char Title[SIZE_16];//邮件标题
	char Content[SIZE_256];//邮件内容
}MailInfoParam_Item,*LPMailInfoParam_Item;
typedef std::map<int,LPMailInfoParam_Item> MailInfoParam_Map;
typedef std::map<int,LPMailInfoParam_Item>::iterator MailInfoParam_Map_Iterator;

struct PackStoreParam
{
	int Postion;//参看USE_PART
	int IsOverlap;//是否可以重叠
};
typedef std::map<int,PackStoreParam*> PackStoreParam_Map;
typedef PackStoreParam_Map::iterator PackStoreParam_Map_Iterator;

struct tagTime_Hour
{
	int hour;
	int min;
	bool operator<(const tagTime_Hour &node) const
	{
		if (hour >node.hour)
		{
			return false;
		}
		else if(hour == node.hour)
		{
			return (min < node.min);
		}
		return true;
	}
};
typedef struct tagActionDotParam
{
	int MapID;
	tagTime_Hour Day_Start;
	tagTime_Hour Day_End;
	int AttackDot_Day;
	int RecoveryDot_Day;
	int AttackDot_Night;
	int RecoveryDot_Night;
	int Flag;
}ActionDotParam,*LPActionDotParam;
typedef std::map<int,LPActionDotParam> ActionDotParam_Map;
typedef std::map<int,LPActionDotParam>::iterator ActionDotParam_Map_Iterator;

struct HeroEquipAttribute
{
	int itemID;					//装备ID
	char name[SIZE_32];
	int level;
	char jobIDs[6];				//职业
	int postion;				//
	int needStamina;			//耐力需求
	int needPower;				//力量需求
	int needCelerity;			//敏捷需求
	int needWit;				//智力需求
	int stamina;				//耐力
	int power;					//力量
	int celerity;				//敏捷
	int wit;					//智力
	int physicAttack;			//物攻
	int physicRecovery;			//物防
	int magicAttack;			//魔攻
	int magicRecovery;			//魔防
	int attackSpeed;			//攻速
	int heroHP;					//生命
	int injureRate;				//重伤率
	int captureRate;			//捕获率
	int beInjureRate;			//受伤减免
	int beCaptureRate;			//被俘减免
	int forceRate;				//暴击率
	int setID;					//套装ID
	int skill;					//技能
	int skillLevel;				//技能等级
	int quality;				//品质
	int inlayNums;				//镶嵌（精练）次数
	bool isSale;				//是否可以出售
	int price;					//基础价格
	HeroEquipAttribute &operator+=(const HeroEquipAttribute &right_)
	{
		stamina += right_.stamina;						//耐力
		power += right_.power;							//力量
		celerity += right_.celerity;					//敏捷
		wit += right_.wit;								//智力
		physicAttack += right_.physicAttack;			//物攻
		physicRecovery += right_.physicRecovery;		//物防
		magicAttack += right_.magicAttack;				//魔攻
		magicRecovery += right_.magicRecovery;			//魔防
		attackSpeed += right_.attackSpeed;				//攻速
		heroHP += right_.heroHP;						//生命
		injureRate += right_.injureRate;				//重伤率
		captureRate += right_.captureRate;				//捕获率
		beInjureRate += right_.beInjureRate;			//受伤减免
		beCaptureRate += right_.beCaptureRate;			//被俘减免
		forceRate += right_.forceRate;					//暴击率
		//price += right_.price;						//基础价格
		return (*this);
	}
};
typedef std::map<int,HeroEquipAttribute*> HeroEquipAttribute_Map;
typedef HeroEquipAttribute_Map::iterator HeroEquipAttribute_Map_Iterator;

typedef std::multimap<int,HeroEquipAttribute*> HeroEquipAttribute_Multimap;
typedef HeroEquipAttribute_Multimap::iterator HeroEquipAttribute_Multimap_Iterator;
typedef std::pair<int,HeroEquipAttribute*> HeroEquipAttribute_Multimap_Pair;

typedef std::map<int,HeroEquipAttribute_Multimap*> HeroEquipAttribute_Multimap_Map;
typedef HeroEquipAttribute_Multimap_Map::iterator HeroEquipAttribute_Multimap_Map_Iterator;
typedef std::pair<int,HeroEquipAttribute_Multimap*> HeroEquipAttribute_Multimap_Map_Pair;

typedef std::map<int,HeroEquipAttribute_Multimap_Map*> HeroEquipAttribute_Multimap_Map_Map;
typedef HeroEquipAttribute_Multimap_Map_Map::iterator HeroEquipAttribute_Multimap_Map_Map_Iterator;
typedef std::pair<int,HeroEquipAttribute_Multimap_Map*> HeroEquipAttribute_Multimap_Map_Map_Pair;

struct RefiningProbability
{
	int refiningTime;		//精练次数
	int succeedRate;		//成功率
	int priceRate;			//价格倍数
};
typedef std::map<int,RefiningProbability*> RefiningProbability_Map;
typedef RefiningProbability_Map::iterator RefiningProbability_Map_Iterator;

struct WeaponAttribute
{
	int weaponID;			//武器ID
	int weaponType;			//武器类型
	int cost;				//价格
	int attack;				//攻击	
	int hurt;				//伤害系数
	int singleHands;		//单双手
	int soldier;			//兵种
	int exp;				//掉落经验
	int level;				//级别
	std::map<int , int> race; //种族
	int addAttack; //增加攻击的百分比
	int attackFreq; //攻击频率
	int trajectorySpeed; //弹道速度
	int attackType; //攻击类型（0地点攻击 ， 1，追踪攻击，2，瞬时攻击）
	int actionBout;//动作时间
};
typedef std::map<int,WeaponAttribute*> WeaponAttribute_Map;
typedef WeaponAttribute_Map::iterator WeaponAttribute_Map_Iterator;

struct DefendAttribute
{
	int defendID;			//防具ID
	int defendType;			//防具类型
	int cost;				//造价
	int decRate;			//减移动速
	int physiceRec;			//物理防御
	int magicRec;			//魔防防御
	int level;				//级别
};
typedef std::map<int,DefendAttribute*> DefendAttribute_Map;
typedef DefendAttribute_Map::iterator DefendAttribute_Map_Iterator;

//英雄初始的精专技能参数
struct OfficerInitSkill
{
	int jobID;				//英雄的职业
	int skill;				//初始技能
	int skillLevel;			//初始技能等级
};
typedef std::map<int,OfficerInitSkill*> OfficerInitSkill_Map;
typedef OfficerInitSkill_Map::iterator OfficerInitSkill_Map_Iterator;
typedef std::multimap<int,OfficerInitSkill*> OfficerInitSkill_MultiMap;
typedef OfficerInitSkill_MultiMap::iterator OfficerInitSkill_MultiMap_iterator;
typedef std::pair<int, OfficerInitSkill*> OfficerInitSkill_MultiMap_Pair;

//任务配置表
struct TaskBaseTable
{
	int taskid_; // taskid = 一条
	char taskname_[SIZE_32];
	int tasktype_;
	int tasklevel_;
	int playerlevel_;
	int playerrank_;
	int taskitem_;
	int prvtaskid_;
	char bonus_[SIZE_128];
	char contest_[SIZE_128];
	int mailtype_;
	int days;
};
typedef std::map<unsigned int,TaskBaseTable> TaskBaseTable_Map;
typedef TaskBaseTable_Map::const_iterator TaskBaseTable_Map_ConstIterator;

//奖励项
struct AwardsItem
{
	int type;			//奖励类型，枚举AWARDS_TYPE
	int itemID;			//具体奖励，根据奖励类型而定；如：如果奖励卡，则为卡ID；如果为资源，则为枚举BASE_RESOURCE_TYPE；等等
	int itemType;		//奖励的物品类型，目前针对type为AWARDS_SOLDIEREQUIPMENT有效，表示是武器、防具或者是特殊工具
	int count;			//数量
};
typedef std::vector<AwardsItem*> AwardsItem_Vector;
typedef std::vector<AwardsItem*>::const_iterator AwardsItem_Vector_Iterator;

/*特殊英雄性值*/
struct tagSpecialHero
{
	int HeroID;						//特殊英雄ID--1000+1起始
	char HeroName[24];				//特殊英难名称
	int JodID;						//职业
	int RaceID;						//种族
	int FaceID;						//头像编号
	int minStamina;					//最小耐力
	int maxStamina;					//最小耐力
	int minPower;						//力量
	int maxPower;						//力量
	int minCelerity;					//敏捷
	int maxCelerity;					//敏捷
	int minWit;						//智慧
	int maxWit;						//智慧
	int TechID;						//特殊技能ID
	int TechLevel;					//技能级别
};
//从DB_Table移过来
struct ResearchConfigure
{
	int itemid_;
	int level_; // type_ = ID 多条
	int type_; // type_ + itemid_  + level_ = ID 一条
	int money_;
	int science_;
	int completetimes_;
};
typedef std::map<int,ResearchConfigure*> ResearchConfigure_Map;
typedef ResearchConfigure_Map::iterator ResearchConfigure_Map_Iterator;

//从DB_Table移过来
struct ProduceConfigure
{
	int itemid_; 
	int weanpontypeid_;
	int itemtype_; 
	int level_;
	int foodcount_;
	int woodcount_;
	int ironcount_;
	int skincount_;
	int redstonecount_;
	int yellowstonecount_;
	int greenstonecount_;
	int backironcount_;
	int money_;
	int science_;
	int completetimes_;
};
typedef std::map<int,ProduceConfigure*> ProduceConfigure_Map;
typedef ProduceConfigure_Map::iterator ProduceConfigure_Map_Iterator;

//从DB_Table移过来
struct ConstructionConfigure
{
	int constructionid_;
	int level_; // constructionid_ + level_ = id 一条
	int completetimes_;
	int money_;
	int foodcount_;
	int woodcount_;
	int ironcount_;
	int skincount_;
	int science_;
	int modify_;
};
typedef std::multimap<int,ConstructionConfigure*> ConstructionConfigure_Multimap;
typedef ConstructionConfigure_Multimap::iterator ConstructionConfigure_Multimap_Iterator;
typedef std::pair<int, ConstructionConfigure*> ConstructionConfigure_MultiMap_Pair;

//从DB_Table移过来
struct GameCityInfo
{
	int cityid_; // cityid_ + mapid_ = ID 一条
	int mapid_; // mapid_ = ID 多条
	int foodcount_;
	int woodcount_;
	int ironcount_;
	int skincount_;
	int x_;
	int y_;
	int human_;			//zeng 06/10
	int mannikin_;
	int fairy_;
	int dandiprat_;
	int halfanimal_;
	int cattle_;
	int blackfairy_;
	int decpesrson_;
};
typedef std::map<int,GameCityInfo*> GameCityInfo_Map;
typedef GameCityInfo_Map::iterator GameCityInfo_Map_Iterator;

//初始玩家建筑
struct InitConstruction
{
	int constructionID;	//建筑ID
	int level;			//级别
};
typedef std::map<int,InitConstruction*> InitConstruction_Map;
typedef InitConstruction_Map::iterator InitConstruction_Map_Iterator;

//广播消息参数
struct BroadMsgParam
{
	int cmdID;				//消息
	int minLevel;			//广播级别下限
	int maxLevel;			//广播级别上限
	int msgRange;			//范围，参考MSGRANGE_TYPE枚举
};
typedef std::map<int,BroadMsgParam*> BroadMsgParam_Map;
typedef BroadMsgParam_Map::iterator BroadMsgParam_Map_Iterator;

//日常任务完成目标信息
struct DayTask_DetailInfo
{
	int taskType;			//任务类型
	int taskLevel;				//任务级别
	int kindType;			//种类
	int itemType;			//目标类型
	int itemID;				//目标ID
	int number;				//数量
	int itemLevel;			//级别
	int maps[3];			//地图1:联盟；2部落
	int days;				//天数，1：一天；>1：一周
	__int64 endTime;		//结束时间
};
typedef std::map<int,DayTask_DetailInfo*> DayTaskParam_Map;
typedef DayTaskParam_Map::iterator DayTaskParam_Map_Iterator;

//军团称号
struct LegionTitleParam
{
	int legTitleID;			//ID
	int legTitleType;		//类型
	char name[16];			//名称
	int level;				//级别
	int condValue;			//满足的条件值
	int wage;				//工资
};
typedef std::map<int,LegionTitleParam*> LegionTitleParam_Map;
typedef LegionTitleParam_Map::const_iterator LegionTitleParam_Map_Iterator;

typedef std::map<int,LegionTitleParam_Map*> LegionTitleParam_Map_Map;
typedef LegionTitleParam_Map_Map::iterator LegionTitleParam_Map_Map_Iterator;
typedef std::pair<int, LegionTitleParam*> LegionTitleParam_Map_Pair;
//基础资源产量
struct BaseRate
{
	int foodRate;			//粮食
	int woodRate;			//木材
	int ironRate;			//铁
	int skinRate;			//皮
};
//军团升级条件参数
struct ConsortiaUpdateParam
{
	int level;				//级别
	bool isRoyal;			//是否为皇家军团
	int needMoney;			//所需金钱
	int needGODian;			//所需GO点
	int needExp;			//所需经验
	int needCredit;			//所需荣誉点
	int maxUserNum;			//人数上限
	BaseRate baseRate;		//增加的产率
	int extraUserCount;		//每多少人额外增加的产率
	BaseRate extraRate;		//而外增加的产率
};
typedef std::map<int,ConsortiaUpdateParam*> ConsortiaUpdateParam_Map;
typedef ConsortiaUpdateParam_Map::const_iterator ConsortiaUpdateParam_Map_Iterator;

//个人荣誉、经验增加到军团后的折换率配置表,千分比
struct ConversionRateParam
{
	int expRate;				//经验	
	int creditRate;				//荣誉
	//转为贡献度的参数
	int money2ContrRate;		//金钱转为贡献度的基数
	int exp2ContrRate;			//经验转为贡献度的基数
	int credit2ContrRate;		//荣誉点转为贡献度的基数
};
#endif//ZSBB_DATADEFINE_H__
