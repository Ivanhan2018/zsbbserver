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

//��ͼ����ö��
typedef enum
{
	COUNTRY_MAP_BEGIN = 1001,  //����ͼ��ʼID
	COUNTRY_MAP_END   = 2000,  //����ͼ����ID
	ECTYPE_MAP_BEGIN = 2001,   //��ͨ������ͼ��ʼID
	ECTYPE_MAP_END   = 3000    //��ͨ������ͼ����ID
}FIGHT_MAP_TYPE;

typedef enum
{
	HUMAN			=1,	//����
	ELVE			=2,	//����
	DWARF			=3,	//����
	GNOME			=4,	//٪��
	ORC				=5,	//������
	TAUREN			=6,	//ţͷ��
	NIGHTELVE		=7,//�ھ���
	UNDEAD			=8,//����
	RACE_BOSS       =9 //BOSS
}RACE_TYPE;

typedef enum
{
	// add by zwl
	SOLDIERTYPE_NONE = 0,
	WARRIOR				=1 ,//սʿ
	SHOOTER				=2 ,//����
	HUNTER				=3 ,//����
	WARLOCK				=6 ,//��ʿ
	PRIEST				=4 ,//��ʦ
	MAGE				=5 ,//��ʦ
	ROGUE				=7 ,//Ǳ����
	SABER				=8 ,//���
	TRANSPROT			=9 ,//�������
	COLONIZE			=10,//ֳ����
	SOLDIER_TYPE_GOV = 47, //��������
	SOLDIER_TYPE_CITY = 48, //����
	SOLDIER_TYPE_SENTRY = 49, //����
	SOLDIER_TYPE_BUILD_TOFT = 50, //�ػ�
	SOLDIER_TYPE_TRAP      =51,    //����
	SOLDIER_TYPE_BARRIER   =52,    //դ��
}SOLDIER_TYPE;
typedef enum
{
	OFFICER_OCCUPATION_NONE = 0,
	Saber_OFFICER				=1, //ʥ��ʿ
	WARRIOR_OFFICER				=2 ,//սʿ
	SHOOTER_OFFICER				=3 ,//������
	MAGE_OFFICER				=4 ,//��ʦ
	PRIEST_OFFICER				=5 ,//��ʦ
	HUNTER_OFFICER				=6 ,//����
	WARLOCK_OFFICER				=7 ,//��ʿ
	Rogue_OFFICER				=8 ,//Ǳ����

}OFFICER_OCCUPATION_TYPE;


typedef enum
{
	EMBRAVE_MORALE			=1,		//����ʿ��
	HOLD_GROUDN				=2,		//�������
	CAPTIVE_LEADER			=3,		//��²����
	SHOOT_EXACTITUDE		=4,		//��ȷ���
	SHOOT_FAR				=5,		//Զ�����
	SHOOT_DEADLINESS		=6,		//�������
	FIELD_LIFE				=7,     //Ұ������
	WILDNESS				=8,		//��Ұ����
	FAST_FINDPATH			=9,		//����Ѱ·
	MAGIC_WOOPEN			=10,	//������ħ
	LAST_CUSS				=11,	//�ٻ�����
	LESS_MAGIC				=12,	//ħ������
	EVADE_MAGIC				=13,	//ħ������
	HOLINESS_PROTECT		=14,	//��ʥ����
	ADD_LIFE				=15,	//����ף��
	FLY_PLACE				=16,	//�ռ���Ծ
	BLACK_MAGIC				=17,	//ħ������
	ADD_MAGIC				=18,	//ħ����ǿ
	ADD_SLIP				=19,	//�������
	HIDE_ARMY				=20,	//�����ж�
	ADD_VISUAL				=21,	//������Ұ

}SKILL_TABLE;

enum SKILL_TYPE
{
	// add by zwl
	SKILLID_INVALID = 0,

	Double_Hit,          // �񱩴�� 1
	Kill_Fate,           // ������� 2
	Blood_Lost,          // ��Ѫ��ֹ 3
	All_Defend,          // ��起��� 4
	Attack_ALL,          // �޵з��� 5
	Chatch_Hero,         // ���� 6

	Doomsday_Skill,      // ĩ������ 7
	Salvo_Skill,         // ���� 8

	Do_All_CAN_Skill,    // �ܻ� 9
	Aim_at_Skill,        // ��׼ 10
	Magic_Speciality ,   // ����רע 11
	Holiness_Meditation, // ��ʥڤ�� 12

	Phalanx_Skill,       // ���� 13
	Gladiate_Skill,      // ���� 14
	Magic_shield,        // ���� 15
	Holy_Shield,         // ʥ�� 16

	SAILOR,              // ˮʦ 17
	SURPRISE,            // ��Ϯ 18
	PLASTER,             // �ش� 19

	SWEEP_ANYTHING_AWAY, // ��ɨ 20

	FENCING_EXPERT,      // ����ר�� 21
	LANCE_EXPERT,        // ì��ר�� 22
	BOW_EXPERT,          // ����ר�� 23
	RABBI_EXPERT,        // ����ר�� 24
	SAINT_EXPERT,        // ʥ��ר�� 25
	BOARDSHELL_EXPORT,   // ���ר�� 26
	LOCKSHELL_EXPORT,    // ����ר�� 27
	SKINSHELL_EXPORT,    // Ƥ��ר�� 28
	CLOTHSHELL_EXPORT,    // ����ר�� 29
	ATTACK_COUNTRY_TECHNIC, // ������ 30
	BREAK_LAW,              // �Ʒ� 31
	ANATHEMA,               // ���� 32
	ASSAULT,                // ͻϮ 33
	ADD_HP_SKILL,                 // HP���� 34
	FALL_ENEMY_MAGIC_HARM,  // ���͵з�ħ���˺� 35
	FALL_ENEMY_SHOOTER_HARM, // ���͵з������˺� 36
	FALL_ENEMY_WARRIOR_HARM,  // ���͵з�սʿ�˺� 37

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������Ч
	SELFBLAST_CARD_SKILL = 500,    // �Ա��� 500
};

// Ч������ö��
enum EFFECT_TYPE
{
	INVALID_EFFECT = 0,

	// �񱩴��Ч��
	FORCE_ATTACK_EFFECT,  // 1

	// �������Ч��
	DEATH_WARRANT_EFFECT, // 2

	// ��Ѫ��ֹЧ��
	BLOOD_WITHOUT_END_EFFECT, // 3

	// ��起���Ч��
	JIB_DEFENT_EFFECT, // 4

	// �޵з���Ч��
	INVINCIBILITY_DEFENT_EFFECT, // 5

	// ����Ч��
	CAPTURE_EFFECT, // 6

	// ����ȫ��Ч��(�����䣬��ɨ)
	ATTACK_WHOLE_CELL_EFFECT, // 7

	// 100%���ʹ���Ч��
	PERCENT_ATTACK_EFFECT, // 8

	// 100%���ʷ���Ч��
	PERCENT_DEFEND_EFFECT, // 9

	// 100%���ʹ���Ч��(���μ���)
	PERCENT_ATT_DEF_EFFECT, // 10

	// �ش�Ч��
	PLASTER_EFFECT, // 11

	// HP����
	ADD_HP_EFFECT  // 12

};

enum ENUM_SPECIALLY_EFFECT{
	EFFECT_OCCUPY = 1,//ռ����Ч
	EFFECT_SELF_BLAST = 2, //�Ա�Ч��
	EFFECT_SPEEDUP = 3, //����Ч��
	EFFECT_ADD_SOLDIER = 4, //����Ч��
	EFFECT_MERCENARY = 5, //Ӷ����Ч��
	EFFECT_INVINCIBILITY = 6, //�޵�
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
	PLAYER_ARMY =1,  //��Ҳ���
	NPC_ARMY	=2,  //NPC����
	BUILD		=3,  //������
	NPC_BOSS	=4,  //NPC BOSS
	ATTACHED_ARMY = 5 //��NPC
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
	FRONT  = 1 , //����
	FLANKS = 2 , //����
	REAR   = 3 , //β��
	ATTACK_CTIY=4,
}ATTACK_STRATEGY; //�йܲ���

typedef enum
{
	PHYSIC_ATTACK = 0 ,
	MAGIC_ATTACK  = 1,
	ADD_HP		  = 2,
}ATTACK_TYPE; //������ʽ

typedef enum  
{
	FAR_TARGET		= 1,
	NEAR_TARGET		= 2,
	STONG_TARGET	= 3,
	SMALL_TARGET	= 4,
}ATTACK_TARGET; //�йܹ���Ŀ��

typedef enum
{
	control		= 0, //�ɿ�΢��	
	trusteeship	= 1, //�й�
	patrol		= 2, //Ѳ��
	Defend		= 3, //����	
	HOLDING		= 4, //פ��״̬
	FOLLOW		= 5,//����
}ARMY_STATE; //����״̬

typedef enum
{
	ECTYPE_NPC = 0, //����
	UNION   = 1, //����
	CLAN	= 2, //����
	NPC_	= 3, //����
}ARMY_ROLE;

enum mapstate
{
	peace=0,
	war=1
};

typedef enum
{
	ARMY_MOVE, //�ƶ�
	ARMY_ATTACK_TARGET,//��������
	ARMY_ATTACK_POS,//����������λ��
	ARMY_NO_DOT, //û��Ŀ���趨
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

enum TRUST_STATE_ACT // �йܺ��ж�״̬
{
	T_NO_TARGET,
	T_ATTACK,
	T_RETREAT,
	T_CALL,
	T_BACK,
	T_RETREAT_END,
};
//��������ö��
enum LIMIT_TYPE{
	TOTAL_LIMIT_TYPE = 1,  //������
	CARD_LEVEL_LIMIT_TYPE  = 2, // ���Ƶȼ�����
	TIME_LIMIT_TYPE = 3, //ʱ�������
	COUNTRY_LIMIT_TYPE = 4 //������
};

enum ADD_ARMY_FAILD_TYPE{
	PLAYER_NUM_REACH_LIMIT = 0, //��������ﵽ����
	ARMY_NUM_REACH_LIMIT = 1, //���������ﵽ����
	INVALID_OFFICE_LEAVL_ = 2 , //Ӣ�۵ȼ��Ƿ�
	INVALID_ARMY_INFO = 3, //��Ч�Ĳ�����Ϣ
	INVALID_ROOM_REQ = 4 , //���󷿼�Ƿ����в������������䣬������ͬʱ��������ͬ�ķ��䣩
	ECTYPE_SYS_ERR = 5 //����ϵͳ����
};

//����������
struct SkillDetail {
	SKILL_TYPE skillID; //����ID
	unsigned int skillType; //��������
	bool iscanLearn; //�Ƿ����ѧϰ
	std::set<OFFICER_OCCUPATION_TYPE> occupationLimit; //Ӣ��ְҵ����
	unsigned int levelLimit; //Ӣ�ۼ�������
	unsigned int level; //���ܵȼ�
	unsigned int consumeSkillDot; //���ļ��ܵ�
	unsigned int odds; //���ܴ�������
	EFFECT_TYPE effectType; //����Ч��
	float effectValue; //����Ч��ֵ
	std::set<SOLDIER_TYPE> selfOccupationLimit;//��������ְҵ����
	std::set<SOLDIER_TYPE> targetOccupationLimit; //Ŀ�귽����ְҵ����
	LAND_FORM landLimit; //��������

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
//��������
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
}; //������Ϣ��Ϣ�ṹ
struct ArmyCommandMove:InfoHead
{
	unsigned long ArmyID;
	POS_STRUCT star;
	POS_STRUCT end;
	void inline  serialize(ThreadCommand cmd = PRO_ARMY_MOVE)
	{
		InfoHead::serialize(cmd);
	}
}; //�����ƶ���Ϣ

struct ArmyCommandADD:InfoHead
{
	unsigned long							lPlayerID;					//������ҵ�ID
	unsigned long							lFromMapID;
	unsigned long							lToMapID;					//����Ŀ�꿤ID
	unsigned long							uMidMapID;					//����Ŀ�꿤��ǰһ�����ı��
	unsigned long							lArmyID;
	bool                                    bIsCreateArmy;             //�Ƿ񴴽�����
	void inline  serialize(ThreadCommand cmd = PRO_ARMY_ADD)
	{
		InfoHead::serialize(cmd);
	}
}; //��Ӳ�����Ϣ

struct ArmyCommandDEL:InfoHead
{
	unsigned long							lPlayerID;					//������ҵ�ID
	unsigned long							lArmyID;                    //����ID
	bool                                    bIsDelete;                  //�Ƿ�ɾ��ArmyTable�е���Ϣ
	void inline  serialize(ThreadCommand cmd = PRO_ARMY_DEL)
	{
		InfoHead::serialize(cmd);
	}
}; //ɾ��������Ϣ
//�޸Ĳ�����Ϣ
struct ArmyCommandModify:InfoHead
{
	unsigned long						lPlayerID;					//���ID
	unsigned long						lArmyID;					//����ID
	int									iSoldierNum;				//��������
	unsigned long						lDefendID;					//����ID
	void inline	serialize(ThreadCommand cmd = PRO_ARMY_MODIFY)
	{
		InfoHead::serialize(cmd);
	}
};
//��ʼս��
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
}; //���뿤��Ϣ
struct CommandExitMap:InfoHead
{
	unsigned long  ulPlayerID;
	void inline  serialize(ThreadCommand cmd = PRO_EXIT_MAP)
	{
		InfoHead::serialize(cmd);
	}
}; //�˳�����Ϣ

struct CommandLeaveMap:InfoHead
{
	unsigned long  ulPlayerID;
	void inline  serialize(ThreadCommand cmd = PRO_LEAVE_MAP)
	{
		InfoHead::serialize(cmd);
	}
}; //�뿪����Ϣ

//���鹥�������Ϣ
struct ArmyCommandAttackPos:InfoHead
{
	unsigned long AArmyID;
	POS_STRUCT    APos;
	void inline  serialize(ThreadCommand cmd = PRO_ARMY_FIGHT_P)
	{
		InfoHead::serialize(cmd);
	}
};
//���鹥��Ŀ����Ϣ
struct ArmyCommandAttackTarget:InfoHead
{
	unsigned long AArmyID;
	unsigned long RArmyID;
	void inline  serialize(ThreadCommand cmd = PRO_ARMY_FIGHT_T)
	{
		InfoHead::serialize(cmd);
	}
};
//GMɾ������NPC����
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
//ս��ͳ����Ϣ
typedef struct
{
	unsigned long PlayerID;
	int catchHeroNum;			//ץ��Ӣ������
	int catchedHeroNum;			//��ץӢ������
	int hurtHeroNum;			//����Ӣ������
	int iKillNum;				//ɱ����
	int ideadNum;				//�Լ�����������
	int boxNum;
}FIGHT_STATISTIC; //ͳ����Ϣ

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


struct CityInfo   //������Ϣ�б�
{
	unsigned int			uCityID;
	POS_STRUCT              centryPos;
	list<POS_STRUCT>		CityPos;
};

struct GovInfo  //����������Ϣ
{
	unsigned int			uGovID;
	POS_STRUCT              centryPos;
	list<POS_STRUCT>		GovPos;
};

struct EncampmentInfo //������Ϣ
{
	unsigned int			uEncampentID;
	POS_STRUCT				EncapentPos;
};

struct SentryInfo     //������Ϣ
{
	unsigned int			SertryID;
	POS_STRUCT              centryPos;
	POS_STRUCT				SerTryPos;
};
struct ResBuild     //��Դ��
{
	unsigned int			resId;
	POS_STRUCT              centryPos;
	POS_STRUCT				SerTryPos;
};

struct Propagate	 //������
{
	unsigned int			uPropagateID;
	POS_STRUCT				Pos[2];
};
//���⽨��
struct SpecialBuild{
	unsigned int buildID; //�������ID
	POS_STRUCT pos;
};
typedef struct  //������Ϣ
{
	SOLDIER_TYPE SoldierType; //ְҵ
	int Race;      //����
	int weaponID ; //����ID
	int defendID;  //����ID
	int count; //����
}SOLDIER_INFO_STRUCT;

typedef struct  //������Ϣ
{
	unsigned int	ulPlayerID ;
	unsigned int 	ulOfficerID ;
	unsigned int	ulArmyID ;
	int				iTotalHP;
	int				iCurrentHp;
	char			ArmyName[SIZE_32];
	char			corpsName[SIZE_32];//��������
	char			officerName[SIZE_32]; //��Ӣ������
	unsigned char   heroLevel; //����Ӣ�۵ȼ�
	unsigned int    heroFaceId;//����Ӣ��ͷ��ID
	ARMY_TYPE		ArmyType;
	ARMY_ROLE			Role;
	int				soldierAttack;  // ����ʿ���Ĺ�����
	int				heroAttack;		//Ӣ�۵Ĺ�����
	SOLDIER_INFO_STRUCT Soldier;
	bool				Trustflag;
	unsigned char   ucArmyState;        //����״̬(0--������,1--����,2--����)

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

struct REQ_ENTER_FIGHT_SYSTEM_STRUCT :DISTRIBUTED_HEADER //�������ս��ϵͳ
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
struct RSP_ENTER_FIGHT_SYSTEM_STRUCT:DISTRIBUTED_HEADER //���ز�����Ϣ
{
	unsigned MapID;
	int      count;
	unsigned char flag ;   //1 = ��ʼ 0 �� �������
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
	int type ;//˭�ǽ�����,1 = ����,2= ����
	int VirualScop[MAX_MAP_SIZE/2+1][MAX_MAP_SIZE];
	inline void serialize(int ,int ,int,int);
};
inline void SendVirualScop::serialize(int agent ,int from ,int to ,int cmd)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to,cmd);
}
struct REQ_ARMY_INFO_STRUCT :DISTRIBUTED_HEADER //��ѯһ��������Ϣ
{
	unsigned long ulUserID;
	unsigned long ulMapID;
	unsigned long ulArmyID;
	inline void serialize(unsigned long ,unsigned long,unsigned long );
};

//���ض�����Ϣ
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
	unsigned char	iSoldierOccupation;//ʿ��ְҵ
	unsigned short  iLevel; //�ȼ�
	unsigned short  iPower;//����
	unsigned short  iCelerity;//����
	unsigned short  iIntellect;//����
	unsigned short  iStamina;//����
	inline void serialize( int agent, int from, int to, int command);
};

struct HarmParm 
{
	unsigned int	ArmyID;			// ���������ӱ��
	int				LostHp;			// ����������HPֵ
	int				DealNum ;		// ��������������
	SKILL_TYPE		ESkill;
	unsigned int obtainExp;         // ��þ���
};


struct AttackTarget :DISTRIBUTED_HEADER{
	unsigned int armyId;//������ID
	unsigned int targetId;//Ŀ��ID
	unsigned short actionTime; //����ʱ��
	unsigned short flyTime;//����ʱ��
	inline void serialize( int agent, int from, int to){
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent ,from,to ,EVENT_ACTION_ATTACK_TARGET);
	}
};
struct AttackPoint :DISTRIBUTED_HEADER{
	unsigned int armyId;//������ID
	unsigned short x;//�����ص� X����
	unsigned short y;//�����ص� Y����
	unsigned short actionTime; //����ʱ��
	unsigned short flyTime;//����ʱ��
	inline void serialize( int agent, int from, int to){
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent ,from,to ,EVENT_ACTION_ATTACK_PONIT);
	}
};

struct REPORT_FIGHT_INFO_STRUCT :DISTRIBUTED_HEADER  //����ս�����
{
	unsigned long	ulAttackArmyID;     //����������
	ATTACK_TYPE		AttackType ;		//������ʽ����������ħ������
	unsigned int	uWeaponID;			//�������
	SKILL_TYPE     Skill ;				//ʹ�õļ���
	int            obtainExp;           //��þ���
	unsigned int count;
	inline HarmParm& operator [](unsigned int index){
		return ((HarmParm*)(this + 1))[index];
	}
	inline void serialize( int agent, int from, int to, int command);
}; //���潻ս�����������

//struct HarmParm 
//{
//	unsigned int	ArmyID;			// ���������ӱ��
//	int				LostHp;			// ����������HPֵ
//	int				DealNum ;		// ��������������
//	ARMY_TYPE       type ;
//	SKILL_TYPE		ESkill;
//
//	unsigned int obtainExp;         // ��þ���
//};
//struct REPORT_FIGHT_INFO_STRUCT :DISTRIBUTED_HEADER  //����ս�����
//{
//	unsigned long	ulAttackArmyID;     //����������
//	POS_STRUCT		AttackPos;			//������λ��
//	POS_STRUCT		TargetPos;			//������Ŀ��λ��
//	ARMY_TYPE       type ;
//	ATTACK_TYPE		AttackType ;		//������ʽ����������ħ������
//	unsigned int	uWeaponID;			//�������
//	SKILL_TYPE     Skill ;				//ʹ�õļ���
//	int            obtainExp;           //��þ���
//	unsigned int count;
//	inline HarmParm& operator [](unsigned int index){
//		return ((HarmParm*)(this + 1))[index];
//	}
//	inline void serialize( int agent, int from, int to, int command);
//}; //���潻ս�����������

struct REQ_MOVE_INFO_STRUCT :DISTRIBUTED_HEADER //�����ƶ�
{
	unsigned int	ulUserID;
	unsigned int	ulArmyID;
	unsigned		uMapID ;
	POS_STRUCT		pos;
	inline void serialize(unsigned int ,unsigned int ,unsigned ,int ,int  );
};
struct REQ_ARMY_GO_OUT_STRUCT: public DISTRIBUTED_HEADER
{
	unsigned long ArmyID; // ����������
	unsigned long tagMapID; //Ŀ�Ŀ����
	int			sortID;		//����Ϊ���ͣ������Ժ����䡣Ŀǰ��0��ʹ�ü��ٿ�����0��ʹ�ü��ٿ���
	void inline serialize(int from, int to,  unsigned long armyid,unsigned long tagmap,int sortid);
};

struct RSP_ARMY_GO_OUT_STRUCT: public DISTRIBUTED_HEADER
{
	void inline serialize(int length, int agent, int from, int to, int command);
	unsigned long ArmyID;
	bool bIsSucc; // �����Ƿ�ɹ�
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
//���������й���Ϣ
struct ReqSetArmyTrustInfo:DISTRIBUTED_HEADER
{
	unsigned long		ulUserID;//��ұ��
	unsigned long		ulArmyID;//������
	bool				IsTrust; //  true = �й� false = ȡ���й�
	ARMY_TRUST_STATE	truststate;// �й�״��
	void inline serialize(int userid, int mapid, int command)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),0,userid,mapid ,command);
	}
};

//�����й���Ϣ
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

struct REQ_TRUST_INFO_STRUCT : DISTRIBUTED_HEADER // �����й�
{
	unsigned int	ulUserID ;
	unsigned int	ulArmyID ; //���ӱ��
	unsigned int	ulMapID ; //����ͼ���
	int				TrustTarget;//�й�Ŀ��1 = ,  2 = ,3 = פ��״��
	int				TrustStratey;//�йܲ���
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

struct REQ_CANCEL_TURST_INFO_STRUCT :DISTRIBUTED_HEADER//ȡ���й�
{
	unsigned int	ulUserId;
	unsigned int	ulArmyID;
	unsigned int	ulMapID ;	
	inline void serialize(unsigned int ,unsigned int ,unsigned int);
};

struct RSP_CANCEL_TURST_INFO_STRUCT :DISTRIBUTED_HEADER//ȡ���й�
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
//20100205ljw�޸Ĳ����ƶ�Э���
struct SEND_ARMY_MOVE_INFO_STRUCT :DISTRIBUTED_HEADER //���Ͷ����ƶ���Ϣ
{
	unsigned int	uMapId;
	unsigned int	ArmyID;
	unsigned short  needTime;//����ʱ��
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
	unsigned int    totalHp; //������HP
	unsigned int    curHp;   //���ӵ�ǰHP
	void inline serialize(int agent, int from, int to, int command = RSP_UPDATE_ARMY_INFO)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to,command);
	}
};

struct REQ_ATTACK_TARGET_STRUCT :DISTRIBUTED_HEADER // ���ý���Ŀ��
{
	unsigned int uUserID;
	unsigned int myArmyID;
	unsigned int TargetArmyID;
	unsigned int uMapID;
	int			 x;
	int			 y;
	inline void serialize(unsigned int userid,unsigned int armyid,unsigned int Target,unsigned int mapid ,int x,int y );

};//ѡ�񹥻�Ŀ��

struct RSP_ATTACK_TARGET_STRUCT : DISTRIBUTED_HEADER
{
	int flag ;
	inline void serialize(int ,int,int,int,int);
};

inline void RSP_ATTACK_TARGET_STRUCT::serialize(int lenth,int agent,int from,int to ,int cmd)
{
	DISTRIBUTED_HEADER::serialize(lenth,agent,from,to,cmd);
}

struct  REQ_SET_PATROL_STATE_STRUCT :DISTRIBUTED_HEADER //����Ѳ��
{
	unsigned int uUserID;
	unsigned int uArmyID;
	unsigned int uMapID;
	//POS_STRUCT   StartPos; //Ѳ�ߵ���� �������ڵ�
	POS_STRUCT   EndPos; //Ѳ���յ�
	inline void serialize(unsigned int  ,unsigned int ,unsigned int,POS_STRUCT & );
}; //����Ѳ��״̬

struct  RSP_SET_PATROL_STATE_STRUCT :DISTRIBUTED_HEADER
{
	unsigned int uUserID;
	unsigned int uArmyID;
	unsigned int uMapID;
	bool flag ;
	inline void serialize( int agent, int from, int to, int command);
};

struct REQ_CANCEL_PATROL_STATE_STRUCT :DISTRIBUTED_HEADER //����ȡ��Ѳ��
{
	unsigned int  uUserID;
	unsigned int  uArmyID;
	unsigned int  uMapID;
	inline void serialize(unsigned int ,unsigned int,unsigned int );
};

struct RSP_CANCEL_PATROL_STATE_STRUCT:DISTRIBUTED_HEADER
{
	unsigned int    uArmyID;
	bool			flag; //�Ƿ�ȡ���ɹ�
	inline void serialize( int agent, int from, int to, int command);
};

struct REQ_SET_ARMY_DEFEND_STRUCT:DISTRIBUTED_HEADER //���ö���Ϊ����״̬
{
	unsigned int uPlayerID;
	unsigned int uArmyID;
	unsigned int uMapID;
	int State;  //1 = ����Ϊ����, 0 =  ȡ������
	inline void serialize(unsigned int ,int,int ,int,int cmd = REQ_SET_ARMY_DEFEND_INFO);
};


struct RSP_SET_ARMY_DEFAND_STRUCT :DISTRIBUTED_HEADER // �������÷�������Ϣ
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
	bool isNeedRefresh; // �Ƿ���Ҫˢ�� add by ljw 2009 12 08
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

struct RspUpgradeLevel:DISTRIBUTED_HEADER //����֪ͨ
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
struct StatisticArmyFightInfo //ͳ�ƶ����ս��Ϣ
{
	char					ArmyName[SIZE_24];		//��������
	char					HeroName[SIZE_24];		//Ӣ������
	int						HeroFaceId;
	SOLDIER_TYPE			type;			//ʿ������
	unsigned char			state;
	int						SoldierNum;			//ʿ����
	int						deadNum;				//��������
	int						KillNum;				//ɱ������
	int						iExp;					//��ȡEXP
};
struct StatisticGoodFightInfo
{
	char PlayerName[10][SIZE_24];
};
struct statisticTotalFightInfo:DISTRIBUTED_HEADER
{
	GAME_HALL_TYPE warType; //����ս�����ͣ���ս��Ϊ-1��
	int UcatchHeroNum;	//	�ҷ�ץ��Ӣ����
	int UHurtHeroNum;	//	�ҷ�����Ӣ����
	int ULossNum;		//  �ҷ�ʿ����ʧ����
	int CcatchHeroNum;	//	�з�ץ��Ӣ����
	int CHurtHeroNum;	//	�з�����Ӣ����
	int CLossNum;		//	�з�ʿ����ʧ����
	int winer;			// 1  =���ˣ�  2 = ���� , 3 = ����
	int obtainBoxNum;      //��ñ��������
	char TopPlayerName[10][SIZE_24];
	FIGHT_STATISTIC		playerStatic; // ��ҵ�ͳ����Ϣ
	int num ;			//��������
	StatisticArmyFightInfo ArmyStatic[100];//������Ϣͳ��
	
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

struct EventArmyLeave : DISTRIBUTED_HEADER // �ڿ���ɾ������
{
	ARMY_TRUST_STATE armyState; //��Ҫ�ĳɵ�״̬
	unsigned char armyNum; // ��������
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
	unsigned char countryId; //��ӪID
	unsigned char resType;//��Դ����
	unsigned int  resCount; //��Դ����
};
struct MsgUpdateRes : DISTRIBUTED_HEADER{
	unsigned char count; //��Ϣ����
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
	unsigned int effectId; //Ч��ID
	ENUM_SPECIALLY_EFFECT effectType ;//Ч������
	unsigned int useArmyId; //ʹ�÷�
	unsigned int receiveArmyId; //���ܷ�
	int startBout_; //ʹ��ʱ�Ļغ���
	int endBout_; //����ʱ�Ļغ���
};
//��ǰ��Ч
struct MsgSpeciallyEffect : DISTRIBUTED_HEADER{
	unsigned int count; //��Ϣ����
	inline void  serialize(int agent , int from, int to)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this) + sizeof(SpeciallyEffect) * count,agent,from,to,MSG_SPECIALLY_EFFECT);
	}
	inline SpeciallyEffect& operator[](int iIndex)
	{
		return ((SpeciallyEffect*)((unsigned char *)this + sizeof(MsgSpeciallyEffect)))[iIndex];
	}
};
//���Ч��
struct MsgClearEffect : DISTRIBUTED_HEADER{
	unsigned int count; //��Ϣ����
	inline void  serialize(int agent , int from, int to)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this) + sizeof(unsigned int) * count,agent,from,to,MSG_CLEAR_EFFECT);
	}
	inline unsigned int& operator[](int iIndex)
	{
		return ((unsigned int*)((unsigned char *)this + sizeof(MsgClearEffect)))[iIndex];
	}
};
//�ı���Ӫ
struct MsgChangeCountry : DISTRIBUTED_HEADER{
	unsigned int armyId; //��Ϣ����
	int countryId;
	inline void  serialize(int from, int to)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,MSG_CHANGE_COUNTRY);
	}
};
//��Ƭ��ȴ��Ϣ
struct CoolingInfo {
	CardType cardType; //��Ƭ����
	unsigned int armyId; //����ID
	unsigned int coolingTime; //��Ҫ��ȴʱ��
};
//��Ƭ��ȴ��Ϣ
struct MsgPropCardCooling : DISTRIBUTED_HEADER{
	unsigned int count;//��Ϣ����
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