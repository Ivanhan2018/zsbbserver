#ifndef ZSBB_DATADEFINE_H__
#define ZSBB_DATADEFINE_H__
#include <map>
#include <vector>
#include "Global_Macro.h"
enum Error_Code
{
	ERR_SUCCESS = 0,			//�ɹ�
};

typedef struct tagQuantityByRank_Item
{
	int Rank;					//����
	int HeroQuantity;			//Ӣ������
	int TownQuantity;			//��������
}QuantityByRank_Item,*LPQuantityByRank_Item;
typedef std::map<int,LPQuantityByRank_Item> QuantityByRank_Map;
typedef QuantityByRank_Map::iterator QuantityByRank_Map_Iterator;

typedef struct tagSoldierarmorconfig_Item
{
	int DefendID;//����ID
	int Soldier;//սʿ
	int Shooter;//����
	int Churchman;//��ʦ
	int Rabbi;//��ʦ
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
	int			CardID;				//��1000��ʼ,������Ӣ��װ�����֡�
	char		CardName[SIZE_32];	//��������
	CardType	Type;				//���ͣ��ο�CardType
	int			Price;				//�۸������ĵ�Go�㣩
	int			Value;				//�ɹ����������װ����ţ���CardType�й�
	int			Time;				//���ĳ���ʱ�䣬��λ����
	int			Postion;			//װ��λ�ã�1-11ΪӢ��װ����
	bool        OpenEnabled;		//�Ƿ���Դ�
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
	int CounryID;//��Ӫ
	int Type;//�ʼ����ͱ��
	char Title[SIZE_16];//�ʼ�����
	char Content[SIZE_256];//�ʼ�����
}MailInfoParam_Item,*LPMailInfoParam_Item;
typedef std::map<int,LPMailInfoParam_Item> MailInfoParam_Map;
typedef std::map<int,LPMailInfoParam_Item>::iterator MailInfoParam_Map_Iterator;

struct PackStoreParam
{
	int Postion;//�ο�USE_PART
	int IsOverlap;//�Ƿ�����ص�
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
	int itemID;					//װ��ID
	char name[SIZE_32];
	int level;
	char jobIDs[6];				//ְҵ
	int postion;				//
	int needStamina;			//��������
	int needPower;				//��������
	int needCelerity;			//��������
	int needWit;				//��������
	int stamina;				//����
	int power;					//����
	int celerity;				//����
	int wit;					//����
	int physicAttack;			//�﹥
	int physicRecovery;			//���
	int magicAttack;			//ħ��
	int magicRecovery;			//ħ��
	int attackSpeed;			//����
	int heroHP;					//����
	int injureRate;				//������
	int captureRate;			//������
	int beInjureRate;			//���˼���
	int beCaptureRate;			//��������
	int forceRate;				//������
	int setID;					//��װID
	int skill;					//����
	int skillLevel;				//���ܵȼ�
	int quality;				//Ʒ��
	int inlayNums;				//��Ƕ������������
	bool isSale;				//�Ƿ���Գ���
	int price;					//�����۸�
	HeroEquipAttribute &operator+=(const HeroEquipAttribute &right_)
	{
		stamina += right_.stamina;						//����
		power += right_.power;							//����
		celerity += right_.celerity;					//����
		wit += right_.wit;								//����
		physicAttack += right_.physicAttack;			//�﹥
		physicRecovery += right_.physicRecovery;		//���
		magicAttack += right_.magicAttack;				//ħ��
		magicRecovery += right_.magicRecovery;			//ħ��
		attackSpeed += right_.attackSpeed;				//����
		heroHP += right_.heroHP;						//����
		injureRate += right_.injureRate;				//������
		captureRate += right_.captureRate;				//������
		beInjureRate += right_.beInjureRate;			//���˼���
		beCaptureRate += right_.beCaptureRate;			//��������
		forceRate += right_.forceRate;					//������
		//price += right_.price;						//�����۸�
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
	int refiningTime;		//��������
	int succeedRate;		//�ɹ���
	int priceRate;			//�۸���
};
typedef std::map<int,RefiningProbability*> RefiningProbability_Map;
typedef RefiningProbability_Map::iterator RefiningProbability_Map_Iterator;

struct WeaponAttribute
{
	int weaponID;			//����ID
	int weaponType;			//��������
	int cost;				//�۸�
	int attack;				//����	
	int hurt;				//�˺�ϵ��
	int singleHands;		//��˫��
	int soldier;			//����
	int exp;				//���侭��
	int level;				//����
	std::map<int , int> race; //����
	int addAttack; //���ӹ����İٷֱ�
	int attackFreq; //����Ƶ��
	int trajectorySpeed; //�����ٶ�
	int attackType; //�������ͣ�0�ص㹥�� �� 1��׷�ٹ�����2��˲ʱ������
	int actionBout;//����ʱ��
};
typedef std::map<int,WeaponAttribute*> WeaponAttribute_Map;
typedef WeaponAttribute_Map::iterator WeaponAttribute_Map_Iterator;

struct DefendAttribute
{
	int defendID;			//����ID
	int defendType;			//��������
	int cost;				//���
	int decRate;			//���ƶ���
	int physiceRec;			//�������
	int magicRec;			//ħ������
	int level;				//����
};
typedef std::map<int,DefendAttribute*> DefendAttribute_Map;
typedef DefendAttribute_Map::iterator DefendAttribute_Map_Iterator;

//Ӣ�۳�ʼ�ľ�ר���ܲ���
struct OfficerInitSkill
{
	int jobID;				//Ӣ�۵�ְҵ
	int skill;				//��ʼ����
	int skillLevel;			//��ʼ���ܵȼ�
};
typedef std::map<int,OfficerInitSkill*> OfficerInitSkill_Map;
typedef OfficerInitSkill_Map::iterator OfficerInitSkill_Map_Iterator;
typedef std::multimap<int,OfficerInitSkill*> OfficerInitSkill_MultiMap;
typedef OfficerInitSkill_MultiMap::iterator OfficerInitSkill_MultiMap_iterator;
typedef std::pair<int, OfficerInitSkill*> OfficerInitSkill_MultiMap_Pair;

//�������ñ�
struct TaskBaseTable
{
	int taskid_; // taskid = һ��
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

//������
struct AwardsItem
{
	int type;			//�������ͣ�ö��AWARDS_TYPE
	int itemID;			//���影�������ݽ������Ͷ������磺�������������Ϊ��ID�����Ϊ��Դ����Ϊö��BASE_RESOURCE_TYPE���ȵ�
	int itemType;		//��������Ʒ���ͣ�Ŀǰ���typeΪAWARDS_SOLDIEREQUIPMENT��Ч����ʾ�����������߻��������⹤��
	int count;			//����
};
typedef std::vector<AwardsItem*> AwardsItem_Vector;
typedef std::vector<AwardsItem*>::const_iterator AwardsItem_Vector_Iterator;

/*����Ӣ����ֵ*/
struct tagSpecialHero
{
	int HeroID;						//����Ӣ��ID--1000+1��ʼ
	char HeroName[24];				//����Ӣ������
	int JodID;						//ְҵ
	int RaceID;						//����
	int FaceID;						//ͷ����
	int minStamina;					//��С����
	int maxStamina;					//��С����
	int minPower;						//����
	int maxPower;						//����
	int minCelerity;					//����
	int maxCelerity;					//����
	int minWit;						//�ǻ�
	int maxWit;						//�ǻ�
	int TechID;						//���⼼��ID
	int TechLevel;					//���ܼ���
};
//��DB_Table�ƹ���
struct ResearchConfigure
{
	int itemid_;
	int level_; // type_ = ID ����
	int type_; // type_ + itemid_  + level_ = ID һ��
	int money_;
	int science_;
	int completetimes_;
};
typedef std::map<int,ResearchConfigure*> ResearchConfigure_Map;
typedef ResearchConfigure_Map::iterator ResearchConfigure_Map_Iterator;

//��DB_Table�ƹ���
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

//��DB_Table�ƹ���
struct ConstructionConfigure
{
	int constructionid_;
	int level_; // constructionid_ + level_ = id һ��
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

//��DB_Table�ƹ���
struct GameCityInfo
{
	int cityid_; // cityid_ + mapid_ = ID һ��
	int mapid_; // mapid_ = ID ����
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

//��ʼ��ҽ���
struct InitConstruction
{
	int constructionID;	//����ID
	int level;			//����
};
typedef std::map<int,InitConstruction*> InitConstruction_Map;
typedef InitConstruction_Map::iterator InitConstruction_Map_Iterator;

//�㲥��Ϣ����
struct BroadMsgParam
{
	int cmdID;				//��Ϣ
	int minLevel;			//�㲥��������
	int maxLevel;			//�㲥��������
	int msgRange;			//��Χ���ο�MSGRANGE_TYPEö��
};
typedef std::map<int,BroadMsgParam*> BroadMsgParam_Map;
typedef BroadMsgParam_Map::iterator BroadMsgParam_Map_Iterator;

//�ճ��������Ŀ����Ϣ
struct DayTask_DetailInfo
{
	int taskType;			//��������
	int taskLevel;				//���񼶱�
	int kindType;			//����
	int itemType;			//Ŀ������
	int itemID;				//Ŀ��ID
	int number;				//����
	int itemLevel;			//����
	int maps[3];			//��ͼ1:���ˣ�2����
	int days;				//������1��һ�죻>1��һ��
	__int64 endTime;		//����ʱ��
};
typedef std::map<int,DayTask_DetailInfo*> DayTaskParam_Map;
typedef DayTaskParam_Map::iterator DayTaskParam_Map_Iterator;

//���ųƺ�
struct LegionTitleParam
{
	int legTitleID;			//ID
	int legTitleType;		//����
	char name[16];			//����
	int level;				//����
	int condValue;			//���������ֵ
	int wage;				//����
};
typedef std::map<int,LegionTitleParam*> LegionTitleParam_Map;
typedef LegionTitleParam_Map::const_iterator LegionTitleParam_Map_Iterator;

typedef std::map<int,LegionTitleParam_Map*> LegionTitleParam_Map_Map;
typedef LegionTitleParam_Map_Map::iterator LegionTitleParam_Map_Map_Iterator;
typedef std::pair<int, LegionTitleParam*> LegionTitleParam_Map_Pair;
//������Դ����
struct BaseRate
{
	int foodRate;			//��ʳ
	int woodRate;			//ľ��
	int ironRate;			//��
	int skinRate;			//Ƥ
};
//����������������
struct ConsortiaUpdateParam
{
	int level;				//����
	bool isRoyal;			//�Ƿ�Ϊ�ʼҾ���
	int needMoney;			//�����Ǯ
	int needGODian;			//����GO��
	int needExp;			//���辭��
	int needCredit;			//����������
	int maxUserNum;			//��������
	BaseRate baseRate;		//���ӵĲ���
	int extraUserCount;		//ÿ�����˶������ӵĲ���
	BaseRate extraRate;		//�������ӵĲ���
};
typedef std::map<int,ConsortiaUpdateParam*> ConsortiaUpdateParam_Map;
typedef ConsortiaUpdateParam_Map::const_iterator ConsortiaUpdateParam_Map_Iterator;

//�����������������ӵ����ź���ۻ������ñ�,ǧ�ֱ�
struct ConversionRateParam
{
	int expRate;				//����	
	int creditRate;				//����
	//תΪ���׶ȵĲ���
	int money2ContrRate;		//��ǮתΪ���׶ȵĻ���
	int exp2ContrRate;			//����תΪ���׶ȵĻ���
	int credit2ContrRate;		//������תΪ���׶ȵĻ���
};
#endif//ZSBB_DATADEFINE_H__
