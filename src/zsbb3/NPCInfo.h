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


//NPC������
struct EctypeNpcTypeTable{
	unsigned int npcTypeID;       //������������ID
	char npcName[MAX_ECTYPE_NAME_LEN]; //��������
	int level;           //����
	int raceID;          //����ID
	int soldierType;     //ʿ��ְҵ
	int attackPower;     //����������
	int attackType;      //��������
	int weaponType;      //��������
	int weaponID;       //����ID
	int defendID; //����ID
	int attackScope;      //������Ұ
	int magicDependPower; //����ħ������
	int phsicDependPower; //�����������
	int soldierHp;        //����HP
	int movePerNeedTime;  //�ƶ�һ������ʱ��
	int viewScope;        //������Ұ
	int dropExp;          //�������侭��
	int skill;            //����
	int skillLevel;       //���ܵȼ�
	int npcType; //NPC����
};
//enum 
//0������
//1������
//2������
enum NeutralState{
	STATE_DEFEND = 0, //����������
	STATE_ATTACK,
	STATE_NEUTRAL
};

//����NPC���ýṹ
struct EctypeNpcArmyTable{
	int ectypeID;         //����ID
	unsigned int armyID;  //����ID
	int npcTypeID ;       //NPC����ID
	int isNeutral;        //�Ƿ�����
	int isSurely;         //�Ƿ�س�
	int strategyType;    //��������
	int aiLevel;        //AI�ȼ�
	int soldierNum;      //ʿ������
	int posX;            //����λ��
	int posY; 
	int dropItem; //������ƷItem
	int dropOdds; //������Ʒ
	int enterTime;				// NPC����ս��ʱ��
	int quitTime;				// NPC�˳�ս��ʱ��
	int  isInfinityReproduce;  // NPC�Ƿ����޷�ֳ
	unsigned int  adscription;			// ����ID�������������NPC����ˢ��
	int rebornTime;			// NPC����ʱ��
	int armyCardsType;    //�����������
	int cardDropOdds;     //���Ƶ������
};

//����������NPC�����ýṹ
struct NpcTemplate{
	int id; //ģ��ID
	int npcTypeID; // npc����ID
	int strategyType; //����
	int aiLevel;//AI����
	int soldierNum; //����
	int x; //�������X
	int y; //�������Y
	int dropItem;//������Ʒ
	int dropOdds;//�������
	int enterTime; // ����ʱ��
	int quitTime;//�˳�ʱ��
	int	isInfinityReproduce; //�Ƿ���
	int rebornTime;//�������
	int armyCardsType;    //�����������
	int cardDropOdds;     //���Ƶ������
};

//���ڷ�ɧ�ţ�NPC�����ýṹ
struct NpcGuarderParam{
	int coutryID; //��ӪID
	int mapID; // ��ͼID
	int npcID;// NPC ID 
	int strategy;//����
	int aiLevel; //AI�ȼ�
	int npcNum;//NPC����
	int dropItem; //�����ʾ
	int dropOdds; //�������
	int armyCard; //���Ʊ�ʾ
	int armyCardDropOdds;//���Ƶ������
};
struct EctypeInfoTable{
	int ectypeID;  //����ID
	int dLevel;   //�������޵ȼ�
	int uLevel;     //�������޼���
	int mapID;     //��ͼID
	int npcArmyNum; //������������
	int limitBout; //���غ���
	char ectypeName[MAX_ECTYPE_NAME_LEN]; //��������
	int entranceID; //���ID
	int limitArmyNum; //������������
	int limitPlayerNum; //�����������
	int warType;//��������
};
struct DerateHurtTable{
	int weaponID;
	int defendID;
	int attackOdds;
};
enum FightOverCmdEnum{
	KILL_TARGET = 1,      //ɱ��ĳĿ��
	PROTECTED_TARGET = 2, //����ĳĿ��
	GET_DROP_ITEM = 3,     //���ĳ������Ʒ
	BOUT_LIMIT = 4, // �غ�����
	GET_RES_LIMIT  =5// ��Դ�ռ�
};


struct WarEntranceInfo{
	unsigned int buildId;
	int mapId;
	int label;
	int groupId;
};
struct FightOverCondition {
	int ectypeID; //����ID
	FightOverCmdEnum cmdType;  //����CMD���� 
	int targetID;   //Ŀ��
	int param;     //�����������
	int condition; //���� 1����ʾ��������ɸ�������  ����0�������ǹؼ�����
};

//���߿�ʹ������
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
	//���
	EctypeNpcTypeTable* getEctypeNpcType(int npcArmyTypeID);
	//��ø�����Ϣ
	EctypeInfoTable * getEctypeInfoTable(int entranceID);
	//���Ѳ������
	map<unsigned int , PatrolPosVect*> * getPatrolData(int ectypeID);
	//��ø���NPC�Ĳ���ID
	vector<NpcTemplate> * getNpcTemplate(int templateID);
	//��ý�����������
	FightOverConditionArray* getFightOverCondition(int ectypeID);
	//���AI�ȼ�����
	aistrategy::AiLevelParam * getAiLevelParam(int aiLevel);
	//��������������
	int getHurtOdds(int weaponType , int defendType);
	//��ø�������������
	PublicBuildParamTable * getEctypeBuildParam(int buildType , int buildLeve);
	//��ý�������Ϣ
	PublicBuildTable * getEctypeBuild(unsigned int buildID);
	//���ս�������Ϣ
	map<int , list<WarEntranceInfo *>>* getWarEntranceInfo(unsigned int mapid);
	//��õ��߿���Ϣ
	PropCardConfig * getPropCardInfo(int itemId);
	//��÷�ɧ��NPC������
	const list<NpcGuarderParam*>*getGuarderParam(int mapId , int countryID) const;
	//���������Ϣ
	int getLimitValue(LIMIT_TYPE limitType , int limitKey);
	//��õ���Ӱ����Ϣ
	aistrategy::LandformAffect * getLandFormAffect(LAND_FORM landForm, SOLDIER_TYPE soldierType);
	//��õ���Ӱ��MAP
	inline  LandformInfoMap const* getLandFormMap() const{
		return &landFormMap_; // ���ζԲ���Ӱ�����Ϣ��
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
	map<unsigned ,NPC*> m_NPCList; //npc ��Ϣ�б�
	map<unsigned int , EctypeNpcTypeTable*>   ectypeNpcTypes_; //����NPC����
	map<int , map<unsigned int , EctypeNpcArmyTable*> > ectypeNpcArmys_; //��������NPC
	map<int , EctypeInfoTable*> ectypeInfos_; //������Ϣ
	map<int , map<unsigned int , PatrolPosVect*> > armyPatrolData_; //Ѳ������
	map<int , FightOverConditionArray*> fightOverConditionData_; //ս����������
	map<int , aistrategy::AiLevelParam*> aiLevelParamData_; //AI�ȼ���������
	map<int,map<int,int>> derateHurtMap_; //�˺���������
	map<int , vector<NpcTemplate> *> npcTemplateData_; //���� NPC��ģ������
	map<int,map<int , PublicBuildParamTable*> > ectypeBuildParam_; //��������������
	map<unsigned int , PublicBuildTable*> ectypeBuild_; // ����������
	LandformInfoMap landFormMap_; // ���ζԲ���Ӱ�����Ϣ��
	map<std::pair<LIMIT_TYPE , int> , int>  limitValueMap_; //������Ϣ��
	map<int , map<int , list<WarEntranceInfo *>>> warEntranceMap_; //ս�������Ϣ��
	map<int ,PropCardConfig*> propCardConfig_; //���߿�����
	map<int , map<int , list<NpcGuarderParam*> > > npcGuardersParam_;//����ɧ��NPC���ñ�
	//DBG����
	DB_Proxy * dataBaseProxy_; 
};

