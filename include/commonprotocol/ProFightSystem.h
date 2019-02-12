#ifndef PRO_FIGHT_SYSTEM_H
#define PRO_FIGHT_SYSTEM_H
#define NULL   0
#define MAX_MAP_SIZE 82
#define MAX_MAP_ARMY_NUM 200
#define MAX_SKILL_NUM 4
#define MAX_ARMY_NUM   20
#define MAX_NUM_ARMY   20
#define MAX_FIGHT_ROOM_ROOM 1000
#define MAX_FIGHT_BATTLE_ARMY_NUM 150

#include <set>
#include <map>
#include <list>

#include "Global_Macro.h"
#include "Distributed_Header.h"

using namespace std;

#pragma warning(disable:4100)
#pragma pack(1)

//地图类型枚举
typedef enum
{
	COUNTRY_MAP_BEGIN = 1001,  //郡地图起始ID
	COUNTRY_MAP_END   = 2000,  //郡地图结束ID
	ECTYPE_MAP_BEGIN = 2001,   //普通副本地图起始ID
	ECTYPE_MAP_END   = 3000    //普通副本地图结束ID
}FIGHT_MAP_TYPE;

typedef enum
{
	HUMAN			=1,	//人类
	ELVE			=2,	//精灵
	DWARF			=3,	//矮人
	GNOME			=4,	//侏儒
	ORC				=5,	//半兽人
	TAUREN			=6,	//牛头人
	NIGHTELVE		=7,//黑精灵
	UNDEAD			=8,//亡灵
	RACE_BOSS       =9 //BOSS
}RACE_TYPE;

typedef enum
{
	// add by zwl
	SOLDIERTYPE_NONE = 0,
	WARRIOR				=1 ,//战士
	SHOOTER				=2 ,//射手
	HUNTER				=3 ,//猎人
	WARLOCK				=6 ,//术士
	PRIEST				=4 ,//牧师
	MAGE				=5 ,//法师
	ROGUE				=7 ,//潜行者
	SABER				=8 ,//骑兵
	TRANSPROT			=9 ,//运输队伍
	COLONIZE			=10,//殖民马车
	SOLDIER_TYPE_GOV = 47, //行政中心
	SOLDIER_TYPE_CITY = 48, //城市
	SOLDIER_TYPE_SENTRY = 49, //岗哨
	SOLDIER_TYPE_BUILD_TOFT = 50, //地基
	SOLDIER_TYPE_TRAP      =51,    //陷阱
	SOLDIER_TYPE_BARRIER   =52,    //栅栏
}SOLDIER_TYPE;
typedef enum
{
	OFFICER_OCCUPATION_NONE = 0,
	Saber_OFFICER				=1, //圣骑士
	WARRIOR_OFFICER				=2 ,//战士
	SHOOTER_OFFICER				=3 ,//弓箭手
	MAGE_OFFICER				=4 ,//法师
	PRIEST_OFFICER				=5 ,//牧师
	HUNTER_OFFICER				=6 ,//猎人
	WARLOCK_OFFICER				=7 ,//术士
	Rogue_OFFICER				=8 ,//潜行者

}OFFICER_OCCUPATION_TYPE;


typedef enum
{
	EMBRAVE_MORALE			=1,		//鼓舞士气
	HOLD_GROUDN				=2,		//坚守阵地
	CAPTIVE_LEADER			=3,		//俘虏敌首
	SHOOT_EXACTITUDE		=4,		//精确射击
	SHOOT_FAR				=5,		//远程射击
	SHOOT_DEADLINESS		=6,		//致命射击
	FIELD_LIFE				=7,     //野外生存
	WILDNESS				=8,		//狂野爆发
	FAST_FINDPATH			=9,		//快速寻路
	MAGIC_WOOPEN			=10,	//武器附魔
	LAST_CUSS				=11,	//迟缓诅咒
	LESS_MAGIC				=12,	//魔法免疫
	EVADE_MAGIC				=13,	//魔法护盾
	HOLINESS_PROTECT		=14,	//神圣保护
	ADD_LIFE				=15,	//生命祝福
	FLY_PLACE				=16,	//空间跳跃
	BLACK_MAGIC				=17,	//魔法反射
	ADD_MAGIC				=18,	//魔法增强
	ADD_SLIP				=19,	//灵活闪躲
	HIDE_ARMY				=20,	//秘密行动
	ADD_VISUAL				=21,	//极限视野

}SKILL_TABLE;

enum SKILL_TYPE
{
	// add by zwl
	SKILLID_INVALID = 0,

	Double_Hit,          // 狂暴打击 1
	Kill_Fate,           // 致命打击 2
	Blood_Lost,          // 流血不止 3
	All_Defend,          // 铁璧防护 4
	Attack_ALL,          // 无敌防护 5
	Chatch_Hero,         // 捕获 6

	Doomsday_Skill,      // 末日审判 7
	Salvo_Skill,         // 齐射 8

	Do_All_CAN_Skill,    // 奋击 9
	Aim_at_Skill,        // 瞄准 10
	Magic_Speciality ,   // 法力专注 11
	Holiness_Meditation, // 神圣冥想 12

	Phalanx_Skill,       // 方阵 13
	Gladiate_Skill,      // 箭阵 14
	Magic_shield,        // 法盾 15
	Holy_Shield,         // 圣盾 16

	SAILOR,              // 水师 17
	SURPRISE,            // 奇袭 18
	PLASTER,             // 重创 19

	SWEEP_ANYTHING_AWAY, // 横扫 20

	FENCING_EXPERT,      // 剑术专精 21
	LANCE_EXPERT,        // 矛术专精 22
	BOW_EXPERT,          // 弓术专精 23
	RABBI_EXPERT,        // 法术专精 24
	SAINT_EXPERT,        // 圣术专精 25
	BOARDSHELL_EXPORT,   // 板甲专精 26
	LOCKSHELL_EXPORT,    // 锁甲专精 27
	SKINSHELL_EXPORT,    // 皮甲专精 28
	CLOTHSHELL_EXPORT,    // 布甲专精 29
	ATTACK_COUNTRY_TECHNIC, // 攻城术 30
	BREAK_LAW,              // 破法 31
	ANATHEMA,               // 诅咒 32
	ASSAULT,                // 突袭 33
	ADD_HP_SKILL,                 // HP增加 34
	FALL_ENEMY_MAGIC_HARM,  // 降低敌方魔法伤害 35
	FALL_ENEMY_SHOOTER_HARM, // 降低敌方射手伤害 36
	FALL_ENEMY_WARRIOR_HARM,  // 降低敌方战士伤害 37

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// 卡类特效
	SELFBLAST_CARD_SKILL = 500,    // 自爆卡 500
};

// 效果类型枚举
enum EFFECT_TYPE
{
	INVALID_EFFECT = 0,

	// 狂暴打击效果
	FORCE_ATTACK_EFFECT,  // 1

	// 致命打击效果
	DEATH_WARRANT_EFFECT, // 2

	// 流血不止效果
	BLOOD_WITHOUT_END_EFFECT, // 3

	// 铁璧防护效果
	JIB_DEFENT_EFFECT, // 4

	// 无敌防护效果
	INVINCIBILITY_DEFENT_EFFECT, // 5

	// 捕获效果
	CAPTURE_EFFECT, // 6

	// 攻击全格效果(如齐射，横扫)
	ATTACK_WHOLE_CELL_EFFECT, // 7

	// 100%概率攻击效果
	PERCENT_ATTACK_EFFECT, // 8

	// 100%概率防御效果
	PERCENT_DEFEND_EFFECT, // 9

	// 100%概率攻防效果(地形技能)
	PERCENT_ATT_DEF_EFFECT, // 10

	// 重创效果
	PLASTER_EFFECT, // 11

	// HP增加
	ADD_HP_EFFECT  // 12

};

enum ENUM_SPECIALLY_EFFECT{
	EFFECT_OCCUPY = 1,//占领特效
	EFFECT_SELF_BLAST = 2, //自爆效果
	EFFECT_SPEEDUP = 3, //加速效果
	EFFECT_ADD_SOLDIER = 4, //补兵效果
	EFFECT_MERCENARY = 5, //佣兵卡效果
	EFFECT_INVINCIBILITY = 6, //无敌
	EFFECT_RE
};

typedef enum 
{
	ATTACT      =1,
	EVADE       =2,
	LOOK		=3,
}FIGHT_ROLE_TYPE;

typedef enum
{
	PLAYER_ARMY =1,  //玩家部队
	NPC_ARMY	=2,  //NPC部队
	BUILD		=3,  //建筑物
	NPC_BOSS	=4,  //NPC BOSS
	ATTACHED_ARMY = 5 //绑定NPC
}ARMY_TYPE;

typedef enum
{
	LEFTUP		= 1,
	LEFTDWON	= 2,
	RIGHTDOWN	= 3,
	RIGHTUP		= 4
}FORWORD;

typedef enum
{
	FRONT  = 1 , //正面
	FLANKS = 2 , //侧翼
	REAR   = 3 , //尾部
	ATTACK_CTIY=4,
}ATTACK_STRATEGY; //托管策略

typedef enum
{
	PHYSIC_ATTACK = 0 ,
	MAGIC_ATTACK  = 1,
	ADD_HP		  = 2,
}ATTACK_TYPE; //进攻形式

typedef enum  
{
	FAR_TARGET		= 1,
	NEAR_TARGET		= 2,
	STONG_TARGET	= 3,
	SMALL_TARGET	= 4,
}ATTACK_TARGET; //托管攻击目标

typedef enum
{
	control		= 0, //可控微操	
	trusteeship	= 1, //托管
	patrol		= 2, //巡逻
	Defend		= 3, //防御	
	HOLDING		= 4, //驻守状态
	FOLLOW		= 5,//跟随
}ARMY_STATE; //队伍状态

typedef enum
{
	ECTYPE_NPC = 0, //副本
	UNION   = 1, //联盟
	CLAN	= 2, //部落
	NPC_	= 3, //土著
}ARMY_ROLE;

enum mapstate
{
	peace=0,
	war=1
};

typedef enum
{
	ARMY_MOVE, //移动
	ARMY_ATTACK_TARGET,//攻击队伍
	ARMY_ATTACK_POS,//攻击到具体位置
	ARMY_NO_DOT, //没有目标设定
}ARMY_DOT;

enum ThreadCommand
{
	PRO_FIGHT_ROOM		= 1,
	PRO_ARMY_MOVE		= 2,
	PRO_ARMY_FIGHT_P	= 3,
	PRO_ARMY_FIGHT_T	= 4,
	PRO_ARMY_ADD        = 5,
	PRO_ARMY_DEL = 6 ,
	PRO_ENTER_MAP = 7 , 
	PRO_EXIT_MAP ,
	PRO_LEAVE_MAP,
	PRO_ARMY_FIGHT_DEL_NPC,
	PRO_ARMY_MODIFY,
	PRO_START_FIGHT
};

enum TRUST_STATE_ACT // 托管后判断状态
{
	T_NO_TARGET,
	T_ATTACK,
	T_RETREAT,
	T_CALL,
	T_BACK,
	T_RETREAT_END,
};
//限制类型枚举
enum LIMIT_TYPE{
	TOTAL_LIMIT_TYPE = 1,  //总限制
	CARD_LEVEL_LIMIT_TYPE  = 2, // 军牌等级限制
	TIME_LIMIT_TYPE = 3, //时间段限制
	COUNTRY_LIMIT_TYPE = 4 //郡限制
};

enum ADD_ARMY_FAILD_TYPE{
	PLAYER_NUM_REACH_LIMIT = 0, //玩家数量达到上限
	ARMY_NUM_REACH_LIMIT = 1, //部队数量达到上限
	INVALID_OFFICE_LEAVL_ = 2 , //英雄等级非法
	INVALID_ARMY_INFO = 3, //无效的部队信息
	INVALID_ROOM_REQ = 4 , //请求房间非法（有部队在其他房间，不允许同时进两个不同的房间）
	ECTYPE_SYS_ERR = 5 //副本系统错误
};

//技能描述表
struct SkillDetail {
	SKILL_TYPE skillID; //技能ID
	unsigned int skillType; //技能类型
	bool iscanLearn; //是否可以学习
	std::set<OFFICER_OCCUPATION_TYPE> occupationLimit; //英雄职业限制
	unsigned int levelLimit; //英雄级别限制
	unsigned int level; //技能等级
	unsigned int consumeSkillDot; //消耗技能点
	unsigned int odds; //技能触发概率
	EFFECT_TYPE effectType; //技能效果
	float effectValue; //技能效果值
	std::set<SOLDIER_TYPE> selfOccupationLimit;//本方部队职业限制
	std::set<SOLDIER_TYPE> targetOccupationLimit; //目标方部队职业限制
	LAND_FORM landLimit; //地形限制

	bool isSatisfOccupationLimit(OFFICER_OCCUPATION_TYPE soldType)
	{
		std::set<OFFICER_OCCUPATION_TYPE>::iterator itFind = occupationLimit.find(soldType);

		bool isSatisf = (itFind != occupationLimit.end() );

		if (isSatisf)
		{
			return true;
		}
		else
		{
			itFind = occupationLimit.find(OFFICER_OCCUPATION_NONE);

			return itFind != occupationLimit.end();
		}
	}

	bool isSatisfselfOccupationLimit(SOLDIER_TYPE soldType)
	{
		std::set<SOLDIER_TYPE>::iterator itFind = selfOccupationLimit.find(soldType);

		bool isSatisf = (itFind != selfOccupationLimit.end() );

		if (isSatisf)
		{
			return true;
		}
		else
		{
			itFind = selfOccupationLimit.find(SOLDIERTYPE_NONE);

			return itFind != selfOccupationLimit.end();
		}
	}

	bool isSatisftargetOccupationLimit(SOLDIER_TYPE soldType)
	{
		std::set<SOLDIER_TYPE>::iterator itFind = targetOccupationLimit.find(soldType);

		bool isSatisf = (itFind != targetOccupationLimit.end() );

		if (isSatisf)
		{
			return true;
		}
		else
		{
			itFind = targetOccupationLimit.find(SOLDIERTYPE_NONE);

			return itFind != targetOccupationLimit.end();
		}
	}


};
typedef std::map<SKILL_TYPE , std::map<unsigned int ,  SkillDetail*> > SkillDetail_Map;
typedef SkillDetail_Map::iterator SkillDetail_Map_Iterator;


typedef struct _pos
{
	//unsigned long ulmapID ;
	int PosX;
	int PosY;
	int weight ;
	void operator =(const _pos tmp)
	{
		this->PosX = tmp.PosX;
		this->PosY = tmp.PosY;
		this->weight = tmp.weight;
	}
	bool operator <(const _pos tmp)
	{
		if (this->PosY<tmp.PosY)
		{
			return true;
		}
		else if (this->PosY == tmp.PosY && this->PosX <tmp.PosX)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}POS_STRUCT;

typedef struct _point
{
	int dx ;
	int dy ;
	int weight ;

}POINT_STRUCT;
//格子坐标
struct GridCoordinate {
	unsigned short x;
	unsigned short y;
};
/*

struct InfoHead
{
	ThreadCommand commdid;
	void inline  serialize(ThreadCommand cmd)
	{
		this->commdid = cmd;
	}
};
struct FightCommand :InfoHead
{
	int  RoomID;
	void inline  serialize(ThreadCommand cmd = PRO_FIGHT_ROOM)
	{
		InfoHead::serialize(cmd);
	}
}; //处理信息消息结构
struct ArmyCommandMove:InfoHead
{
	unsigned long ArmyID;
	POS_STRUCT star;
	POS_STRUCT end;
	void inline  serialize(ThreadCommand cmd = PRO_ARMY_MOVE)
	{
		InfoHead::serialize(cmd);
	}
}; //队伍移动消息

struct ArmyCommandADD:InfoHead
{
	unsigned long							lPlayerID;					//出征玩家的ID
	unsigned long							lFromMapID;
	unsigned long							lToMapID;					//到达目标郡ID
	unsigned long							uMidMapID;					//到达目标郡的前一个郡的编号
	unsigned long							lArmyID;
	bool                                    bIsCreateArmy;             //是否创建部队
	void inline  serialize(ThreadCommand cmd = PRO_ARMY_ADD)
	{
		InfoHead::serialize(cmd);
	}
}; //添加部队消息

struct ArmyCommandDEL:InfoHead
{
	unsigned long							lPlayerID;					//出征玩家的ID
	unsigned long							lArmyID;                    //部队ID
	bool                                    bIsDelete;                  //是否删除ArmyTable中的信息
	void inline  serialize(ThreadCommand cmd = PRO_ARMY_DEL)
	{
		InfoHead::serialize(cmd);
	}
}; //删除部队消息
//修改部队信息
struct ArmyCommandModify:InfoHead
{
	unsigned long						lPlayerID;					//玩家ID
	unsigned long						lArmyID;					//部队ID
	int									iSoldierNum;				//部队人数
	unsigned long						lDefendID;					//防具ID
	void inline	serialize(ThreadCommand cmd = PRO_ARMY_MODIFY)
	{
		InfoHead::serialize(cmd);
	}
};
//开始战斗
struct ArmyCommandStartFight:InfoHead
{
	unsigned int	uiMapID;
	bool			bFirst;
	void inline serialize(ThreadCommand cmd = PRO_START_FIGHT)
	{
		InfoHead::serialize(cmd);
	}
};
struct ArmyCommandEnterMap:InfoHead
{
	unsigned long  ulPlayerID;
	unsigned long  ulMapID;
	int iAgentID; //
	void inline  serialize(ThreadCommand cmd = PRO_ENTER_MAP)
	{
		InfoHead::serialize(cmd);
	}
}; //进入郡消息
struct CommandExitMap:InfoHead
{
	unsigned long  ulPlayerID;
	void inline  serialize(ThreadCommand cmd = PRO_EXIT_MAP)
	{
		InfoHead::serialize(cmd);
	}
}; //退出郡消息

struct CommandLeaveMap:InfoHead
{
	unsigned long  ulPlayerID;
	void inline  serialize(ThreadCommand cmd = PRO_LEAVE_MAP)
	{
		InfoHead::serialize(cmd);
	}
}; //离开郡消息

//队伍攻击点的消息
struct ArmyCommandAttackPos:InfoHead
{
	unsigned long AArmyID;
	POS_STRUCT    APos;
	void inline  serialize(ThreadCommand cmd = PRO_ARMY_FIGHT_P)
	{
		InfoHead::serialize(cmd);
	}
};
//队伍攻击目标消息
struct ArmyCommandAttackTarget:InfoHead
{
	unsigned long AArmyID;
	unsigned long RArmyID;
	void inline  serialize(ThreadCommand cmd = PRO_ARMY_FIGHT_T)
	{
		InfoHead::serialize(cmd);
	}
};
//GM删除郡中NPC部队
struct ArmyCommandDELNPC:InfoHead
{
	int iAgent;
	int iFrom;
	int iCountryID;
	int iCMD;
	unsigned long ulPlayerID;
	void inline  serialize(ThreadCommand cmd = PRO_ARMY_FIGHT_DEL_NPC)
	{
		InfoHead::serialize(cmd);
	}
};
*/
//战斗统计消息
typedef struct
{
	unsigned long PlayerID;
	int catchHeroNum;			//抓获英雄数量
	int catchedHeroNum;			//被抓英雄数量
	int hurtHeroNum;			//重伤英雄数量
	int iKillNum;				//杀敌数
	int ideadNum;				//自己的伤亡数量
	int boxNum;
}FIGHT_STATISTIC; //统计信息

struct Item
{
	unsigned long ItemID;
	int           type ;
	int			  count;
};
typedef struct
{
	unsigned int ItemID;
	unsigned int pos;
	int			 iLevel;
}OFFICER_ITEM_INFO_STRUCT;



typedef struct  _Index_Weapon
{
	unsigned long ulItemId ;
	int iLevel ;
	bool operator <(const struct _Index_Weapon& weapon)
	{
		if (ulItemId!=weapon.ulItemId)
		{
			return ulItemId < weapon.ulItemId;
		}
		else if(ulItemId = weapon.ulItemId && iLevel != weapon.iLevel)
		{
			return iLevel < weapon.iLevel;
		}
		else
		{
			return false;
		}
	}
	
}INDEX_WEAPON;


struct CityInfo   //城市信息列表
{
	unsigned int			uCityID;
	POS_STRUCT              centryPos;
	list<POS_STRUCT>		CityPos;
};

struct GovInfo  //行政中心信息
{
	unsigned int			uGovID;
	POS_STRUCT              centryPos;
	list<POS_STRUCT>		GovPos;
};

struct EncampmentInfo //大本云信息
{
	unsigned int			uEncampentID;
	POS_STRUCT				EncapentPos;
};

struct SentryInfo     //哨所信息
{
	unsigned int			SertryID;
	POS_STRUCT              centryPos;
	POS_STRUCT				SerTryPos;
};
struct ResBuild     //资源点
{
	unsigned int			resId;
	POS_STRUCT              centryPos;
	POS_STRUCT				SerTryPos;
};

struct Propagate	 //传送门
{
	unsigned int			uPropagateID;
	POS_STRUCT				Pos[2];
};
//特殊建筑
struct SpecialBuild{
	unsigned int buildID; //建筑物的ID
	POS_STRUCT pos;
};
typedef struct  //兵种信息
{
	SOLDIER_TYPE SoldierType; //职业
	int Race;      //种族
	int weaponID ; //武器ID
	int defendID;  //防具ID
	int count; //数量
}SOLDIER_INFO_STRUCT;

typedef struct  //部队信息
{
	unsigned int	ulPlayerID ;
	unsigned int 	ulOfficerID ;
	unsigned int	ulArmyID ;
	int				iTotalHP;
	int				iCurrentHp;
	char			ArmyName[SIZE_32];
	char			corpsName[SIZE_32];//军团名称
	char			officerName[SIZE_32]; //主英雄名称
	unsigned char   heroLevel; //带队英雄等级
	unsigned int    heroFaceId;//带队英雄头像ID
	ARMY_TYPE		ArmyType;
	ARMY_ROLE			Role;
	int				soldierAttack;  // 单个士兵的攻击力
	int				heroAttack;		//英雄的攻击力
	SOLDIER_INFO_STRUCT Soldier;
	bool				Trustflag;
	unsigned char   ucArmyState;        //部队状态(0--队列中,1--正常,2--出征)

}ARMY_INFO_STRUCT;

typedef struct
{
	ARMY_INFO_STRUCT	ArmyInfo;
	unsigned int		MapID;
	int					PosX;
	int					PosY;
	bool				bMove;
	
	FORWORD				Direction;

}ARMY_POS;

struct REQ_ENTER_FIGHT_SYSTEM_STRUCT :DISTRIBUTED_HEADER //请求进入战斗系统
{
	unsigned long  ulPlayerID;
	unsigned long  ulMapID;
	inline void serialize(unsigned long ,unsigned long ,int cmd = REQ_ENTER_FIGHT_SYSTEM);
};
struct BulidParam
{
	unsigned long ulBuildID;
	int			  iBuildType;
	int			  iBuildLevel;
	//void inline serialize(int length, int agent, int from, int to, int command)
};
struct BulidInfoList:DISTRIBUTED_HEADER
{
	int  count;
	int  mapid;
	BulidParam BuildList[MAX_NUM_ARMY];
	void inline serialize(int length, int agent, int from, int to, int command)
	{
		DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
	}
};
struct RSP_ENTER_FIGHT_SYSTEM_STRUCT:DISTRIBUTED_HEADER //返回部队信息
{
	unsigned MapID;
	int      count;
	unsigned char flag ;   //1 = 开始 0 ＝ 后面继续
	ARMY_POS army[MAX_MAP_ARMY_NUM];
	inline void serialize(int lenth ,int agent ,int mapID ,unsigned long PlayerID , int cmd);
};
struct ARMY_ADD :DISTRIBUTED_HEADER
{
	ARMY_POS Army ;
	inline void serialize(int mapid,int playerid,int cmd ,int agent = 0);
};

inline void ARMY_ADD::serialize(int mapid ,int playerid ,int cmd ,int agent)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,mapid,playerid,cmd);
}
struct SendVirualScop : DISTRIBUTED_HEADER 
{
	int type ;//谁是进攻方,1 = 联盟,2= 部落
	int VirualScop[MAX_MAP_SIZE/2+1][MAX_MAP_SIZE];
	inline void serialize(int ,int ,int,int);
};
inline void SendVirualScop::serialize(int agent ,int from ,int to ,int cmd)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to,cmd);
}
struct REQ_ARMY_INFO_STRUCT :DISTRIBUTED_HEADER //查询一个部队信息
{
	unsigned long ulUserID;
	unsigned long ulMapID;
	unsigned long ulArmyID;
	inline void serialize(unsigned long ,unsigned long,unsigned long );
};

//返回队伍信息
struct RSP_ARMY_INFO_STRUCT :DISTRIBUTED_HEADER
{
	unsigned long   uPlayerID;
	unsigned long   ArmyID;
	int             AttackPoint;
	ATTACK_TYPE		AttackType;
	int				count;
	int				totalHp;
	int				iHeroFace;
	int				Magic_Defend;
	int				Physic_Defend;
	int				iWeaponID;
	int				iDefendID;
	int				iRaceID;
	FIGHT_ROLE_TYPE type ;
	char			szName[SIZE_16];
	char			szMainHeroName[SIZE_16];
	char			szHeroName[SIZE_16];
	int				iTrustState;
	unsigned char	iSoldierOccupation;//士兵职业
	unsigned short  iLevel; //等级
	unsigned short  iPower;//力量
	unsigned short  iCelerity;//敏捷
	unsigned short  iIntellect;//智力
	unsigned short  iStamina;//耐力
	inline void serialize( int agent, int from, int to, int command);
};

struct HarmParm 
{
	unsigned int	ArmyID;			// 攻击方部队编号
	int				LostHp;			// 被攻方掉的HP值
	int				DealNum ;		// 被攻方死亡人数
	SKILL_TYPE		ESkill;
	unsigned int obtainExp;         // 获得经验
};


struct AttackTarget :DISTRIBUTED_HEADER{
	unsigned int armyId;//进攻方ID
	unsigned int targetId;//目标ID
	unsigned short actionTime; //动作时间
	unsigned short flyTime;//飞行时间
	inline void serialize( int agent, int from, int to){
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent ,from,to ,EVENT_ACTION_ATTACK_TARGET);
	}
};
struct AttackPoint :DISTRIBUTED_HEADER{
	unsigned int armyId;//进攻方ID
	unsigned short x;//攻击地点 X坐标
	unsigned short y;//攻击地点 Y坐标
	unsigned short actionTime; //动作时间
	unsigned short flyTime;//飞行时间
	inline void serialize( int agent, int from, int to){
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent ,from,to ,EVENT_ACTION_ATTACK_PONIT);
	}
};

struct REPORT_FIGHT_INFO_STRUCT :DISTRIBUTED_HEADER  //报告战斗情况
{
	unsigned long	ulAttackArmyID;     //攻击队伍编号
	ATTACK_TYPE		AttackType ;		//进攻方式，物理攻击或魔法攻击
	unsigned int	uWeaponID;			//武器编号
	SKILL_TYPE     Skill ;				//使用的技能
	int            obtainExp;           //获得经验
	unsigned int count;
	inline HarmParm& operator [](unsigned int index){
		return ((HarmParm*)(this + 1))[index];
	}
	inline void serialize( int agent, int from, int to, int command);
}; //报告交战部队伤亡情况

//struct HarmParm 
//{
//	unsigned int	ArmyID;			// 攻击方部队编号
//	int				LostHp;			// 被攻方掉的HP值
//	int				DealNum ;		// 被攻方死亡人数
//	ARMY_TYPE       type ;
//	SKILL_TYPE		ESkill;
//
//	unsigned int obtainExp;         // 获得经验
//};
//struct REPORT_FIGHT_INFO_STRUCT :DISTRIBUTED_HEADER  //报告战斗情况
//{
//	unsigned long	ulAttackArmyID;     //攻击队伍编号
//	POS_STRUCT		AttackPos;			//攻击方位置
//	POS_STRUCT		TargetPos;			//被攻击目标位置
//	ARMY_TYPE       type ;
//	ATTACK_TYPE		AttackType ;		//进攻方式，物理攻击或魔法攻击
//	unsigned int	uWeaponID;			//武器编号
//	SKILL_TYPE     Skill ;				//使用的技能
//	int            obtainExp;           //获得经验
//	unsigned int count;
//	inline HarmParm& operator [](unsigned int index){
//		return ((HarmParm*)(this + 1))[index];
//	}
//	inline void serialize( int agent, int from, int to, int command);
//}; //报告交战部队伤亡情况

struct REQ_MOVE_INFO_STRUCT :DISTRIBUTED_HEADER //请求移动
{
	unsigned int	ulUserID;
	unsigned int	ulArmyID;
	unsigned		uMapID ;
	POS_STRUCT		pos;
	inline void serialize(unsigned int ,unsigned int ,unsigned ,int ,int  );
};
struct REQ_ARMY_GO_OUT_STRUCT: public DISTRIBUTED_HEADER
{
	unsigned long ArmyID; // 出征队伍编号
	unsigned long tagMapID; //目的郡编号
	int			sortID;		//定义为整型，用于以后扩充。目前：0不使用加速卡；非0：使用加速卡；
	void inline serialize(int from, int to,  unsigned long armyid,unsigned long tagmap,int sortid);
};

struct RSP_ARMY_GO_OUT_STRUCT: public DISTRIBUTED_HEADER
{
	void inline serialize(int length, int agent, int from, int to, int command);
	unsigned long ArmyID;
	bool bIsSucc; // 出征是否成功
};
struct RSP_MOVE_INFO_STRUCT :DISTRIBUTED_HEADER
{
	unsigned int ulUserID;
	unsigned int ulArmyID;
	mapstate state ;
	POS_STRUCT POS;
	bool flag;
	inline void serialize(int length, int agent, int from, int to, int command);
};

inline void RSP_MOVE_INFO_STRUCT::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent ,from ,to, command);
}
//队伍设置托管消息
struct ReqSetArmyTrustInfo:DISTRIBUTED_HEADER
{
	unsigned long		ulUserID;//玩家编号
	unsigned long		ulArmyID;//队伍编号
	bool				IsTrust; //  true = 托管 false = 取消托管
	ARMY_TRUST_STATE	truststate;// 托管状体
	void inline serialize(int userid, int mapid, int command)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),0,userid,mapid ,command);
	}
};

//返回托管消息
struct RspSetArmyTrustInfo:DISTRIBUTED_HEADER
{
	unsigned long ulUserID;
	unsigned ulArmyID;
	bool IsTrust;
	ARMY_TRUST_STATE state;
	void inline serialize(int length, int agent, int from, int to, int command)
	{
		DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
	}
};

struct REQ_TRUST_INFO_STRUCT : DISTRIBUTED_HEADER // 请求托管
{
	unsigned int	ulUserID ;
	unsigned int	ulArmyID ; //部队编号
	unsigned int	ulMapID ; //郡地图编号
	int				TrustTarget;//托管目标1 = ,  2 = ,3 = 驻守状体
	int				TrustStratey;//托管策略
	inline void serialize(unsigned int  ,unsigned int ,unsigned int , int ,int );
};

struct RSP_TRUST_INFO_STRUCT : DISTRIBUTED_HEADER
{
	unsigned int	ulUserID;
	unsigned int	ulArmyID;
	unsigned int	ulMapID;
	int				flag ;
	inline void serialize(int length, int agent, int from, int to, int command);
};

struct REQ_CANCEL_TURST_INFO_STRUCT :DISTRIBUTED_HEADER//取消托管
{
	unsigned int	ulUserId;
	unsigned int	ulArmyID;
	unsigned int	ulMapID ;	
	inline void serialize(unsigned int ,unsigned int ,unsigned int);
};

struct RSP_CANCEL_TURST_INFO_STRUCT :DISTRIBUTED_HEADER//取消托管
{
	unsigned int   ulUserID;
	unsigned int   ulArmyID;
	unsigned int   ulmapID;
	bool		   flag;
	inline void serialize( int agent, int from, int to, int command);
};


struct ChangerVirsual :DISTRIBUTED_HEADER
{
	unsigned short block;
	unsigned char  state;
	inline void serialize(int lenth,int agent, int form ,int to ,int cmd);
};

inline void ChangerVirsual::serialize(int lenth,int agent, int form ,int to ,int cmd)
{
	DISTRIBUTED_HEADER::serialize(lenth,agent,from,to ,cmd);
}
//20100205ljw修改部队移动协议包
struct SEND_ARMY_MOVE_INFO_STRUCT :DISTRIBUTED_HEADER //发送队伍移动信息
{
	unsigned int	uMapId;
	unsigned int	ArmyID;
	unsigned short  needTime;//所需时间
	int				step ;
	FORWORD         Direction;
	inline GridCoordinate& operator [](unsigned int index){
		return ((GridCoordinate*)(this + 1))[index];
	}

	inline void serialize( int agent, int from, int to, int command);
};


struct UpDateArmyInfo :DISTRIBUTED_HEADER
{
	unsigned long	ArmyID;
	int				SoldierNum;
	unsigned long   DefendID;
	 //add by ljw 09 11 27
	unsigned int    totalHp; //部队总HP
	unsigned int    curHp;   //部队当前HP
	void inline serialize(int agent, int from, int to, int command = RSP_UPDATE_ARMY_INFO)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to,command);
	}
};

struct REQ_ATTACK_TARGET_STRUCT :DISTRIBUTED_HEADER // 设置进攻目标
{
	unsigned int uUserID;
	unsigned int myArmyID;
	unsigned int TargetArmyID;
	unsigned int uMapID;
	int			 x;
	int			 y;
	inline void serialize(unsigned int userid,unsigned int armyid,unsigned int Target,unsigned int mapid ,int x,int y );

};//选择攻击目标

struct RSP_ATTACK_TARGET_STRUCT : DISTRIBUTED_HEADER
{
	int flag ;
	inline void serialize(int ,int,int,int,int);
};

inline void RSP_ATTACK_TARGET_STRUCT::serialize(int lenth,int agent,int from,int to ,int cmd)
{
	DISTRIBUTED_HEADER::serialize(lenth,agent,from,to,cmd);
}

struct  REQ_SET_PATROL_STATE_STRUCT :DISTRIBUTED_HEADER //请求巡逻
{
	unsigned int uUserID;
	unsigned int uArmyID;
	unsigned int uMapID;
	//POS_STRUCT   StartPos; //巡逻的起点 队伍所在点
	POS_STRUCT   EndPos; //巡逻终点
	inline void serialize(unsigned int  ,unsigned int ,unsigned int,POS_STRUCT & );
}; //设置巡逻状态

struct  RSP_SET_PATROL_STATE_STRUCT :DISTRIBUTED_HEADER
{
	unsigned int uUserID;
	unsigned int uArmyID;
	unsigned int uMapID;
	bool flag ;
	inline void serialize( int agent, int from, int to, int command);
};

struct REQ_CANCEL_PATROL_STATE_STRUCT :DISTRIBUTED_HEADER //请求取消巡逻
{
	unsigned int  uUserID;
	unsigned int  uArmyID;
	unsigned int  uMapID;
	inline void serialize(unsigned int ,unsigned int,unsigned int );
};

struct RSP_CANCEL_PATROL_STATE_STRUCT:DISTRIBUTED_HEADER
{
	unsigned int    uArmyID;
	bool			flag; //是否取消成功
	inline void serialize( int agent, int from, int to, int command);
};

struct REQ_SET_ARMY_DEFEND_STRUCT:DISTRIBUTED_HEADER //设置队伍为防御状态
{
	unsigned int uPlayerID;
	unsigned int uArmyID;
	unsigned int uMapID;
	int State;  //1 = 设置为防御, 0 =  取消防御
	inline void serialize(unsigned int ,int,int ,int,int cmd = REQ_SET_ARMY_DEFEND_INFO);
};


struct RSP_SET_ARMY_DEFAND_STRUCT :DISTRIBUTED_HEADER // 返回设置防御的信息
{
	unsigned int uPlayerID;
	unsigned int uArmyID;
	int flag ;
	inline void serialize(int ,int ,int ,int ,int);
};

struct RSP_DESTROY_ARMY:DISTRIBUTED_HEADER
{
	unsigned int uArmyID;
	ARMY_TYPE type;
	inline void serialize(int from ,int to , int cmd);
};

inline void RSP_DESTROY_ARMY::serialize(int from ,int to , int cmd)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,cmd);
}

struct REQ_LEAVE_FIGHT_SYSTEM_STRUCT : DISTRIBUTED_HEADER
{
	unsigned long  playerID;
	inline void serialize(int plyerid ,int mapid, int command = REQ_LEAVE_FIGHT_SYSTEM_INFO);
};

struct RSP_LEAVE_FIGHT_SYSTEM_STRUCT :DISTRIBUTED_HEADER
{
	unsigned long ulPlayerID;
	inline void serialize(int agent , int form ,int to, int command);
};
struct REQ_EXIT_FIGHT_SYSTEM_STRUCT :DISTRIBUTED_HEADER
{
	unsigned int uPlayerID;
	inline void serialize(int from,int to ,int cmd);
};


struct FightTime:DISTRIBUTED_HEADER
{
	unsigned long roomid ;
	inline void serialize(int command)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),0,0,0,command);
	}
};

struct BootyInfo
{
	unsigned long itemID;
	unsigned long itemType;
	unsigned long itemNum;
};
struct GetBootyInfo:DISTRIBUTED_HEADER
{
	bool isNeedRefresh; // 是否需要刷新 add by ljw 2009 12 08
	unsigned long MapID;
	int count;
	BootyInfo Booty[100];
	inline void serialize(int length,int agent,int from,int to,int command = RSP_GET_BOOTY_INFO)
	{
		DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
	}
};

struct FightPack:DISTRIBUTED_HEADER
{
	unsigned short  packnum;
	static const int maxDataLen = 10240;
	char Info[maxDataLen];
	
	inline void serialize(int length, int agent, int from, int to, int command = RSP_FIGHT_PACK_INFO)
	{
		DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
	}
};

struct UseEscapeCard :DISTRIBUTED_HEADER
{
	unsigned long MapID;
	int64         iTime;
	char czMainHeroName[SIZE_24];
	char czHeroName[SIZE_24];
	inline void serialize(int length, int agent, int from, int to, int command)
	{
		DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
	}
};
struct TotalDealInfo:DISTRIBUTED_HEADER
{
	unsigned long mapid;
	int			  iTurn;
	int    AttackdealNum;
	int    EvadedealNum;
	inline void serialize(int mapid)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),0,mapid,0,RSP_ONE_TRUN_DEAL_NUM);
	}
};
struct RspTurnInfo:DISTRIBUTED_HEADER
{
	unsigned long	mapID;
	int				iTurnNum;
	int				CurToTalArmyNum;
	int             CurProArmyNum;
	inline void serialize(int agent, int from, int to, int command)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to,command);
	}
};

struct RspUpgradeLevel:DISTRIBUTED_HEADER //升级通知
{
	char PlayerName[SIZE_24];
	char HeroName[SIZE_24];
	unsigned short iLevel;
	unsigned long  ulArmyID;
	inline void serialize(int agent , int from ,int to , int command = RSP_HERO_UPGRADE_LEVEL)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to, command);
	}
};
struct StatisticArmyFightInfo //统计队伍参战信息
{
	char					ArmyName[SIZE_24];		//队伍名字
	char					HeroName[SIZE_24];		//英雄名字
	int						HeroFaceId;
	SOLDIER_TYPE			type;			//士兵类型
	unsigned char			state;
	int						SoldierNum;			//士兵数
	int						deadNum;				//死亡数量
	int						KillNum;				//杀敌数量
	int						iExp;					//获取EXP
};
struct StatisticGoodFightInfo
{
	char PlayerName[10][SIZE_24];
};
struct statisticTotalFightInfo:DISTRIBUTED_HEADER
{
	GAME_HALL_TYPE warType; //场景战斗类型（郡战斗为-1）
	int UcatchHeroNum;	//	我方抓获英雄数
	int UHurtHeroNum;	//	我方受伤英雄数
	int ULossNum;		//  我方士兵损失数量
	int CcatchHeroNum;	//	敌方抓获英雄数
	int CHurtHeroNum;	//	敌方受伤英雄数
	int CLossNum;		//	敌方士兵损失数量
	int winer;			// 1  =联盟，  2 = 部落 , 3 = 土著
	int obtainBoxNum;      //获得宝箱的数量
	char TopPlayerName[10][SIZE_24];
	FIGHT_STATISTIC		playerStatic; // 玩家的统计信息
	int num ;			//队伍数量
	StatisticArmyFightInfo ArmyStatic[100];//队伍信息统计
	
	inline void serialize(int length, int agent, int from, int to, int command)
	{
		DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
	}
};
//addby ljw 2009 12 25

struct MsgObtainBooty:DISTRIBUTED_HEADER
{
	char  winnerName[32];
	char  name[32];
	int   itemID;
	inline void serialize(int agent,int from,int to,int command = BROAD_MSG_OBTAIN_BOOTY)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to,command);
	}
};

struct EventArmyLeave : DISTRIBUTED_HEADER // 在郡中删除部队
{
	ARMY_TRUST_STATE armyState; //需要改成的状态
	unsigned char armyNum; // 部队数量
	inline void  serialize(int from, int to)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this) + sizeof(unsigned long) * armyNum,0,from,to,EVENT_ARMY_LEAVE);
	}

	inline unsigned long& operator[](int iIndex)
	{
		return ((unsigned long*)((unsigned char *)this + sizeof(EventArmyLeave)))[iIndex];
	}
};

struct ResInfo{
	unsigned char countryId; //阵营ID
	unsigned char resType;//资源类型
	unsigned int  resCount; //资源数量
};
struct MsgUpdateRes : DISTRIBUTED_HEADER{
	unsigned char count; //消息数量
	inline void  serialize(int from, int to)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this) + sizeof(MsgUpdateRes) * count,0,from,to,MSG_UPDATE_RES);
	}
	inline ResInfo& operator[](int iIndex)
	{
		return ((ResInfo*)((unsigned char *)this + sizeof(MsgUpdateRes)))[iIndex];
	}
};

struct SpeciallyEffect{
	unsigned int effectId; //效果ID
	ENUM_SPECIALLY_EFFECT effectType ;//效果类型
	unsigned int useArmyId; //使用方
	unsigned int receiveArmyId; //承受方
	int startBout_; //使用时的回合数
	int endBout_; //结束时的回合数
};
//当前特效
struct MsgSpeciallyEffect : DISTRIBUTED_HEADER{
	unsigned int count; //消息数量
	inline void  serialize(int agent , int from, int to)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this) + sizeof(SpeciallyEffect) * count,agent,from,to,MSG_SPECIALLY_EFFECT);
	}
	inline SpeciallyEffect& operator[](int iIndex)
	{
		return ((SpeciallyEffect*)((unsigned char *)this + sizeof(MsgSpeciallyEffect)))[iIndex];
	}
};
//清除效果
struct MsgClearEffect : DISTRIBUTED_HEADER{
	unsigned int count; //消息数量
	inline void  serialize(int agent , int from, int to)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this) + sizeof(unsigned int) * count,agent,from,to,MSG_CLEAR_EFFECT);
	}
	inline unsigned int& operator[](int iIndex)
	{
		return ((unsigned int*)((unsigned char *)this + sizeof(MsgClearEffect)))[iIndex];
	}
};
//改变阵营
struct MsgChangeCountry : DISTRIBUTED_HEADER{
	unsigned int armyId; //消息数量
	int countryId;
	inline void  serialize(int from, int to)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,MSG_CHANGE_COUNTRY);
	}
};
//卡片冷却信息
struct CoolingInfo {
	CardType cardType; //卡片类型
	unsigned int armyId; //部队ID
	unsigned int coolingTime; //需要冷却时间
};
//卡片冷却消息
struct MsgPropCardCooling : DISTRIBUTED_HEADER{
	unsigned int count;//消息数量
	inline void  serialize(int agent , int from, int to)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this) + sizeof(CoolingInfo) * count,agent,from,to,MSG_CARD_COOLING_INFO);
	}
	inline CoolingInfo& operator[](int iIndex)
	{
		return ((CoolingInfo*)((unsigned char *)(this + 1)))[iIndex];
	}
};
#pragma pack()
//////////////////////

inline void REQ_ENTER_FIGHT_SYSTEM_STRUCT::serialize(unsigned long Player,unsigned long mapid ,int cmd)
{
	this->ulPlayerID = Player;
	this->ulMapID = mapid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, mapid, Player, cmd);
}
inline void REQ_ARMY_INFO_STRUCT::serialize(unsigned long userid,unsigned long mapid,unsigned long armyid )
{
	this->ulUserID = userid;
	this->ulMapID = mapid;
	this->ulArmyID = armyid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, userid, mapid, REQ_ARMY_INFO);
}
inline void REQ_MOVE_INFO_STRUCT::serialize(unsigned int userid ,unsigned int armyid ,unsigned mapid,int x ,int y )
{
	this->ulArmyID = userid;
	this->ulArmyID = armyid;
	this->uMapID = mapid;
	this->pos.PosX = x;
	this->pos.PosY = y;
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, userid, mapid, REQ_SELECT_MOVE_POS);
}

inline void REQ_TRUST_INFO_STRUCT::serialize(unsigned int userid,unsigned int armyid,unsigned int mapid, int target,int stratey )
{
	this->ulUserID = userid;
	this->ulArmyID = armyid;
	this->ulMapID = mapid;
	this->TrustStratey = stratey;
	this->TrustTarget = target;
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, userid, mapid, REQ_SELECT_TRUST_STRATEY);
}
inline void REQ_ATTACK_TARGET_STRUCT ::serialize(unsigned int userid,unsigned int armyid,unsigned int Target,unsigned int mapid ,int x,int y )
{
	this->uUserID = userid;
	this->myArmyID = armyid;
	this->TargetArmyID =Target;
	this->uMapID = mapid;
	this->x = x;
	this->y = y ;
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, userid, mapid, REQ_SELECT_ATTACK_TARGET);
}
inline void REQ_SET_PATROL_STATE_STRUCT::serialize(unsigned int userid,unsigned int armyid,unsigned int mapid ,POS_STRUCT & end)
{
	this->uUserID = userid;
	this->uArmyID = armyid ;
	this->uMapID = mapid ;
	this->EndPos.PosX = end.PosX;
	this->EndPos.PosY = end.PosY;
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, userid, mapid, REQ_SET_PATROL_STATE);
}
inline void REQ_CANCEL_PATROL_STATE_STRUCT ::serialize(unsigned int userid ,unsigned int armyid,unsigned int mapid )

{
	this->uUserID = userid;
	this->uArmyID = armyid;
	this->uMapID = mapid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, userid, mapid, REQ_CANCEL_PATROL_INFO);
}
void inline RSP_ARMY_GO_OUT_STRUCT::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from ,to , command);
}

void inline REQ_ARMY_GO_OUT_STRUCT::serialize(int from, int to, unsigned long armyid,unsigned long tagmap,int sortid)
{
	this->ArmyID = armyid;
	this->tagMapID = tagmap;
	this->sortID = sortid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from ,to ,REQ_ARMY_GO_OUT_INFO);
}
inline void REQ_CANCEL_TURST_INFO_STRUCT ::serialize(unsigned int userid,unsigned int armyid ,unsigned int mapid)
{

	this->ulUserId = userid;
	this->ulArmyID = armyid;
	this->ulMapID = mapid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, userid, mapid, this->command);
}
inline void REPORT_FIGHT_INFO_STRUCT ::serialize( int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(HarmParm),agent,from,to,command);
}
inline void RSP_ARMY_INFO_STRUCT ::serialize( int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to,command);
}
inline void SEND_ARMY_MOVE_INFO_STRUCT ::serialize( int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + sizeof(GridCoordinate) * step,agent,from,to,command);
}
inline void RSP_CANCEL_PATROL_STATE_STRUCT ::serialize( int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to,command);
}



inline void RSP_SET_PATROL_STATE_STRUCT::serialize(int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from ,to ,command);
}



inline void RSP_CANCEL_TURST_INFO_STRUCT::serialize(int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to,command);
}

inline void RSP_ENTER_FIGHT_SYSTEM_STRUCT::serialize(int lenth ,int agent ,int mapID ,unsigned long PlayerID , int cmd)
{

	DISTRIBUTED_HEADER::serialize(lenth,agent,mapID,PlayerID,cmd);
}

inline void REQ_SET_ARMY_DEFEND_STRUCT::serialize(unsigned int Armyid,int state,int playerid,int mapid,int cmd)
{
	this->uArmyID = Armyid;
	this->uPlayerID = playerid ;
	this->State = state;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,mapid,cmd);
}

inline void RSP_SET_ARMY_DEFAND_STRUCT ::serialize(int lenth,int agent,int from,int to,int cmd)
{
	DISTRIBUTED_HEADER::serialize(lenth,agent,from,to,cmd);
}

inline void RSP_TRUST_INFO_STRUCT::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from ,to ,command);
}


inline void REQ_EXIT_FIGHT_SYSTEM_STRUCT::serialize(int from,int to ,int cmd)
{
	this->uPlayerID = from ;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,cmd);
}
inline void REQ_LEAVE_FIGHT_SYSTEM_STRUCT::serialize( int plyerid ,int mapid, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,plyerid,mapid,command);
}

inline void RSP_LEAVE_FIGHT_SYSTEM_STRUCT::serialize(int agent , int form ,int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,form,to,command);
}
#endif