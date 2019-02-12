#ifndef LOAD_DB_INFO_H
#define LOAD_DB_INFO_H

#include "ProFightSystem.h"
#include "DBTableHeader.h"
#include "CommonFunction.h"
#include "MapServer_Protocol.h"
#include "ConsortiaProtocol.h"
#include "DB_Proxy.h"
#include <map>
#include "ZSBB_DataDefine.h"
#include "ArmyBrand.h"
using namespace std;

class CLoadDBInfo
{
public:
	CLoadDBInfo(void);
	~CLoadDBInfo(void);
	void LoadParam(DB_Proxy * proxy , int mapID = 1256);
	WeaponAttribute* GetWeaponAttribute(int weaponid);
	DefendAttribute* GetDefendAttribute(int iDefendID);
	SoldierParamTable* GetSoldierParam(int soldiertype);
	GameCityInfo* GetCityInfo(int cityid);
	OfficerRaceAttribute* GetOfficerRaceAttr(int race);
	ConstructionConfigure* getConstructionConfigure(int iConID, int iLevel);
	ProduceConfigure* GetProduceConfig(int iProID);
	ResearchConfigure* GetResearchConfig(int iItemID);
	PublicBuildParamTable* GetPublicBuildParam(int buildKind, int level);

	PublicBuildTable* GetPublicBuildInfo(unsigned long buildid);
	void			 AddPublicBuildInfo(PublicBuildTable* build);
	void      UpdatePublicBuildLevel(unsigned long buildid ,int level,int type/*=0*/);
	void Empty();
	void AddUserName(unsigned long UserID,string str);
	string GetHeroName(unsigned long UserID);
	int  GetEquipAntagonismParam(unsigned int WeaponID,unsigned int defendID);
	BatttleField*  GetBattleBaseInfo(unsigned int id);
	LPQuantityByRank_Item GetQuantityByRank(int rank);
	LPSoldierarmorconfig_Item GetSoldierarmorconfig(int defendid);
	LPMailInfoParam_Item GetMailInfoParam(int countryid,int type);
	//��ÿ�Ƭ����
	LPCardParam GetCardConfig(int cardid);

	LPPlayerInitArmyParam_Item GetPlayerInitArmyParam(int countryid,int jobid);
	GMCreateNPCParam_MultiMap_Iterator GMCreateNPCParam_Lowerbound(int countryid,int level);
	GMCreateNPCParam_MultiMap_Iterator GMCreateNPCParam_Upperbound(int countryid,int level);

	//�ж������,type:0����ͼ��1������
	LPActionDotParam GetActionDotParam(int mapid,int type);

	bool OfficerRacematchSoldier(RACE_TYPE Race ,SOLDIER_TYPE soldiertype);
	bool DefendMacthSoldier(int defendid,int soldiertype);//����ְҵ�Ƿ�ƥ��

	PackStoreParam *GetPackStoreParam(int postion);
	LPGMCreateNPCParam_Item GetHiredNPCArmyParam(int countryid,int level);
	HeroEquipAttribute *GetHeroEquipAttribute(int itemid);
	HeroEquipAttribute *GetInlayResultAttribute(int itemid,int postion);//��ȡ����Ч��������ͬ�ֱ�ʯ��Ƕ����ͬ��װ���ϲ�����Ч����ͬ
	/*����Ʒ�ʡ�λ�úͼ����ȡһ��װ��
	@param scale��Ʒ��
	@param postion��λ��
	@param level�����𣬻�ȡ��װ�����𲻸��ڸü���
	*/
	HeroEquipAttribute *getOneEquip(int scale,int postion,int level);
	RefiningProbability *GetRefiningProbability(int refiningtime);
	SkillDetail * getSkillDetail(SKILL_TYPE skillID , int skillLevel); //��ȡ����������Ϣ
	/*�ж�Ӣ���Ƿ�ѧϰ������ʹ��ĳ�ּ���������ľ�ר����
	@param skills:Ӣ����ѧ�ļ�������
	@param skillnum:���ܵ�����
	@param type:���������ߵ�����
	@param level:���������ߵļ���
	*/
	bool hasSpecialitySkill(PlayerSkillTable *skills,int skillnum,int type,int level);

	//����ְҵID ��ȡ��ְҵĬ�ϵļ��ܲ���
	OfficerInitSkill_MultiMap_iterator getOfficerInitSkill_Lowerbound(int jobid);
	OfficerInitSkill_MultiMap_iterator getOfficerInitSkill_Upperbound(int jobid);
	//��ȡƷ�ʣ����ʣ�
	int getEquipScaleByProbability();
	//��ȡλ�ã����ʣ�
	int getEquipPostionByProbability();
	ArmyBrand * getArmyBrandParamByID(int brandid);
	ArmyBrand * getArmyBrandParamByLevel(int level);
	//��ȡ�ɳ�ϵ��
	int getHeroGroupRate();
	//��ȡ�㲥��Ϣ����
	BroadMsgParam * getBroadMsgParam(int cmd);
	//��ȡ�������
	DayTask_DetailInfo * getDayTaskParam(int taskid);

	ConsortiaUpdateParam* getConsortiaUpdateParam(int level);//���ݼ����ȡ������������
private:
	//��ȡ�ײ�����
	void loadSoldierarmorconfig();
	void clearSoldierarmorconfig();
	//��ȡ��������С�Ӣ��������ϵ��
	void loadQuantityByRank();
	void clearQuantityByRand();
	//��ȡ�ʼ�������
	void loadMailInfoParam();
	void clearMailInfoParam();
	//��ȡ��������
	int loadCardConfig();
	void clearCardConfig();
	//��ȡ��ҳ�ʼʱ�Ĳ�����Ϣ
	void loadPlayerInitArmyParam();
	void clearPlayerInitArmyParam();
	//��ȡNPC�������ñ�
	void loadGMNPCParam();
	void clearGMNPCParam();
	//��ȡ�ж��������
	void loadActionDotParam();
	void clearActionDotParam();
	//��ȡ�Ƿ�����ص���ű����Ĳ���
	int loadPackStoreParam();
	void clearPackStoreParam();
	//��ȡ���ر��Ӳ�����
	void loadHiredNPCArmyParam();
	void clearHiredNPCArmyParam();
	//��ȡװ�����Ա�
	void loadEquipAttribute();
	void insertEquipAttrScalePosLevelMap(HeroEquipAttribute *heroEquipAttri);
	void clearEquipAttribute();
	//��ȡ����Ч����
	void loadInlayResultAttribute();
	void clearInlayResultAttribute();
	//�����ɹ����ʱ�
	void loadRefiningProbability();
	void clearRefiningProbability();
	//��ȡ����������
	void loadSkillDetail(void);
	//�ͷż���������
	void clearSkillDetail(void);
	//��ȡ�������Ա�
	void loadWeaponAttribute();
	void clearWeaponAttribute();
	//��ȡ�������Ա�
	void loadDefendAttribute();
	void clearDefendAttribute();
	//��ȡӢ�۳�ʼ���ܲ�����
	void loadOfficerInitSkill();
	void clearOfficerInitSkill();
	//��ȡ�򿪱���ʱ��װ���ĵ�����ʲ���
	void loadDropEquipProbability();
	//��ȡ���ƶһ�������
	void loadArmyBrandParam();
	void clearArmyBrandParam();
	//���ع�������������
	void loadPublicBuildParam();
	//����Ӣ�۳ɳ�ϵ�����ʱ�
	void loadHeroGroupRateCapability();

	//��ȡ�������ñ�
	void loadResearchConfigure();
	void clearResearchConfigure();
	//��ȡ�������ñ�
	void loadProduceConfigure();
	void clearProduceConfigure();
	//��ȡ�������ñ�
	void loadConstructionConfigure();
	void clearConstructionConfigure();
	//��ȡ������Ϣ��
	void loadGameCityInfo();
	void clearGameCityInfo();
	//���ع㲥��Ϣ������
	void loadBroadMsgParam();
	void clearBroadMsgParam();
	//�����ճ������б�
	void loadDayTaskParam();
	void clearDayTaskParam();
	//����ʿ�������
	void loadSoldierParam();
	//���ʿ�������
	void clearSoldierParam();
	//��ȡ��������������
	void loadConsortiaUpdateParam();
	void clearConsortiaUpdateParam();
public:
	WeaponAttribute_Map								m_weaponAttributeMap ;		//����������
	DefendAttribute_Map								m_defendAttributeMap;		//��������
	map<int , SoldierParamTable* >					Soldier_Param_list ; //���ֲ�����
	GameCityInfo_Map								m_GameCityInfoMap;
	map<int ,OfficerRaceAttribute*>					OfficerRaceAttr_List;
	list<PublicBuildParamTable>						m_listPublicBuildParam;	
	map<int,map<int , PublicBuildParamTable*> >     publicBuildParam_; //��������������
	ConstructionConfigure_Multimap					m_ConstructionConfigureMultimap;
	ProduceConfigure_Map							m_ProduceConfigureMap;
	ResearchConfigure_Map							m_ResearchConfigureMap;
	list<EctypeBaseTable>							m_EctypBaseInfoList; //�������

	map<unsigned long , PublicBuildTable*>			m_PublicBuildList;			
	map<unsigned long , string>						m_UserNameList; // ��������б�
	map<unsigned int ,map<unsigned int ,int> >		m_EquipAntagonism;//װ����˱���Ϣ

	map<unsigned int ,BatttleField*>				m_BattleInfo;

	Soldierarmorconfig_Map							m_SoldierarmorconfigList;
	//���������������Ӧ��ϵ��
	QuantityByRank_Map								m_QuantityByRankList;
	//�ʼ�������
	MailInfoParam_Map								m_MailInfoParamList;
	//������
	CardParam_Map									m_CardConfigList;

	//������Ӣ��ʱ����ʼ��Ҳ��ӵ���Ϣ
	PlayerInitArmyParam_Map							m_PlayerInitArmyParamList;

	//����NPC���ӵĲ���
	GMCreateNPCParam_MultiMap						m_GMCreateNPCParamList;

	//�Ƿ�����ص��Ĳ���
	PackStoreParam_Map								m_PackStoreParamMap;

	//�ж������
	ActionDotParam_Map								m_ActionDotParamList;	
	//���ر��Ӳ���
	GMCreateNPCParam_Map							m_HiredNPCArmyList;

	//װ�����Ա�
	HeroEquipAttribute_Map							m_HeroEquipAttributeList;
	HeroEquipAttribute_Multimap_Map_Map				heroEquipScalePosLevelMap;//Ʒ��->λ��->�����װ���б�
	//����Ч����
	HeroEquipAttribute_Map							m_InlayResultList;
	//�����ɹ����ʱ�
	RefiningProbability_Map							m_RefiningProbabilityMap;
	//����Ч����
	SkillDetail_Map                                 skillDetailMap_;
	OfficerInitSkill_MultiMap						officerInitSkillMultiMap;
	//�򿪱����ǣ�װ���ĵ��������Ϣ

	int												scaleProbability[100];  //Ʒ�ʵ��ʱ�
	int												postionProbability[100];//λ�õ��ʱ�

	//���Ʋ�����
	ArmyBrand_Map									armyBrandMap;
	ArmyBrand_Map									armyBrandLevelMap;

	//Ӣ�۳ɳ�ϵ�����ʱ�							
	std::map<int,int>								heroGroupRateMap;
	//�㲥��Ϣ���ñ�
	BroadMsgParam_Map								broadMsgParamMap;
	//�ճ����������
	DayTaskParam_Map								dayTaskParamMap;
	//�����������ñ�
	ConsortiaUpdateParam_Map						consortiaUpdateParamMap;
	//���ݿ�DB
	DB_Proxy * dataBaseProxy_;
	//mapID
	int mapID_;
};
extern CLoadDBInfo g_LoadDBinfo;
#endif
