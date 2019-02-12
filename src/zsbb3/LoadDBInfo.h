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
	//获得卡片类型
	LPCardParam GetCardConfig(int cardid);

	LPPlayerInitArmyParam_Item GetPlayerInitArmyParam(int countryid,int jobid);
	GMCreateNPCParam_MultiMap_Iterator GMCreateNPCParam_Lowerbound(int countryid,int level);
	GMCreateNPCParam_MultiMap_Iterator GMCreateNPCParam_Upperbound(int countryid,int level);

	//行动点参数,type:0郡地图；1：副本
	LPActionDotParam GetActionDotParam(int mapid,int type);

	bool OfficerRacematchSoldier(RACE_TYPE Race ,SOLDIER_TYPE soldiertype);
	bool DefendMacthSoldier(int defendid,int soldiertype);//甲与职业是否匹配

	PackStoreParam *GetPackStoreParam(int postion);
	LPGMCreateNPCParam_Item GetHiredNPCArmyParam(int countryid,int level);
	HeroEquipAttribute *GetHeroEquipAttribute(int itemid);
	HeroEquipAttribute *GetInlayResultAttribute(int itemid,int postion);//获取精练效果参数，同种宝石镶嵌到不同的装备上产生的效果不同
	/*根据品质、位置和级别获取一件装备
	@param scale：品质
	@param postion：位置
	@param level：级别，获取的装备级别不高于该级别
	*/
	HeroEquipAttribute *getOneEquip(int scale,int postion,int level);
	RefiningProbability *GetRefiningProbability(int refiningtime);
	SkillDetail * getSkillDetail(SKILL_TYPE skillID , int skillLevel); //获取技能描述信息
	/*判断英雄是否学习了满足使用某种级别的武器的精专技能
	@param skills:英雄所学的技能数组
	@param skillnum:技能的数量
	@param type:武器、防具的类型
	@param level:武器、防具的级别
	*/
	bool hasSpecialitySkill(PlayerSkillTable *skills,int skillnum,int type,int level);

	//根据职业ID 获取该职业默认的技能参数
	OfficerInitSkill_MultiMap_iterator getOfficerInitSkill_Lowerbound(int jobid);
	OfficerInitSkill_MultiMap_iterator getOfficerInitSkill_Upperbound(int jobid);
	//获取品质（概率）
	int getEquipScaleByProbability();
	//获取位置（概率）
	int getEquipPostionByProbability();
	ArmyBrand * getArmyBrandParamByID(int brandid);
	ArmyBrand * getArmyBrandParamByLevel(int level);
	//获取成长系数
	int getHeroGroupRate();
	//获取广播消息参数
	BroadMsgParam * getBroadMsgParam(int cmd);
	//获取任务参数
	DayTask_DetailInfo * getDayTaskParam(int taskid);

	ConsortiaUpdateParam* getConsortiaUpdateParam(int level);//根据级别获取军团升级参数
private:
	//读取甲参数表
	void loadSoldierarmorconfig();
	void clearSoldierarmorconfig();
	//读取军衔与城市、英雄数量关系表
	void loadQuantityByRank();
	void clearQuantityByRand();
	//读取邮件参数表
	void loadMailInfoParam();
	void clearMailInfoParam();
	//读取卡参数表
	int loadCardConfig();
	void clearCardConfig();
	//读取玩家初始时的部队信息
	void loadPlayerInitArmyParam();
	void clearPlayerInitArmyParam();
	//读取NPC反击配置表
	void loadGMNPCParam();
	void clearGMNPCParam();
	//读取行动点参数表
	void loadActionDotParam();
	void clearActionDotParam();
	//读取是否可以重叠存放背包的参数
	int loadPackStoreParam();
	void clearPackStoreParam();
	//读取郡守备队参数表
	void loadHiredNPCArmyParam();
	void clearHiredNPCArmyParam();
	//读取装备属性表
	void loadEquipAttribute();
	void insertEquipAttrScalePosLevelMap(HeroEquipAttribute *heroEquipAttri);
	void clearEquipAttribute();
	//读取精练效果表
	void loadInlayResultAttribute();
	void clearInlayResultAttribute();
	//精练成功概率表
	void loadRefiningProbability();
	void clearRefiningProbability();
	//读取技能描述表
	void loadSkillDetail(void);
	//释放技能描述表
	void clearSkillDetail(void);
	//读取武器属性表
	void loadWeaponAttribute();
	void clearWeaponAttribute();
	//读取防具属性表
	void loadDefendAttribute();
	void clearDefendAttribute();
	//读取英雄初始技能参数表
	void loadOfficerInitSkill();
	void clearOfficerInitSkill();
	//读取打开宝箱时，装备的掉落概率参数
	void loadDropEquipProbability();
	//读取军牌兑换参数表
	void loadArmyBrandParam();
	void clearArmyBrandParam();
	//加载公共建筑参数表
	void loadPublicBuildParam();
	//加载英雄成长系数几率表
	void loadHeroGroupRateCapability();

	//读取科研配置表
	void loadResearchConfigure();
	void clearResearchConfigure();
	//读取生产配置表
	void loadProduceConfigure();
	void clearProduceConfigure();
	//读取建筑配置表
	void loadConstructionConfigure();
	void clearConstructionConfigure();
	//读取城市信息表
	void loadGameCityInfo();
	void clearGameCityInfo();
	//加载广播消息参数表
	void loadBroadMsgParam();
	void clearBroadMsgParam();
	//加载日常任务列表
	void loadDayTaskParam();
	void clearDayTaskParam();
	//加载士兵种族表
	void loadSoldierParam();
	//清除士兵种族表
	void clearSoldierParam();
	//读取军团升级参数表
	void loadConsortiaUpdateParam();
	void clearConsortiaUpdateParam();
public:
	WeaponAttribute_Map								m_weaponAttributeMap ;		//武器参数表
	DefendAttribute_Map								m_defendAttributeMap;		//防具属性
	map<int , SoldierParamTable* >					Soldier_Param_list ; //兵种参数表
	GameCityInfo_Map								m_GameCityInfoMap;
	map<int ,OfficerRaceAttribute*>					OfficerRaceAttr_List;
	list<PublicBuildParamTable>						m_listPublicBuildParam;	
	map<int,map<int , PublicBuildParamTable*> >     publicBuildParam_; //副本建筑参数表
	ConstructionConfigure_Multimap					m_ConstructionConfigureMultimap;
	ProduceConfigure_Map							m_ProduceConfigureMap;
	ResearchConfigure_Map							m_ResearchConfigureMap;
	list<EctypeBaseTable>							m_EctypBaseInfoList; //副本编号

	map<unsigned long , PublicBuildTable*>			m_PublicBuildList;			
	map<unsigned long , string>						m_UserNameList; // 玩家名字列表
	map<unsigned int ,map<unsigned int ,int> >		m_EquipAntagonism;//装备相克表信息

	map<unsigned int ,BatttleField*>				m_BattleInfo;

	Soldierarmorconfig_Map							m_SoldierarmorconfigList;
	//军衔与城市数量对应关系表
	QuantityByRank_Map								m_QuantityByRankList;
	//邮件参数表
	MailInfoParam_Map								m_MailInfoParamList;
	//卡参数
	CardParam_Map									m_CardConfigList;

	//创建主英雄时，初始玩家部队的信息
	PlayerInitArmyParam_Map							m_PlayerInitArmyParamList;

	//创建NPC部队的参数
	GMCreateNPCParam_MultiMap						m_GMCreateNPCParamList;

	//是否可以重叠的参数
	PackStoreParam_Map								m_PackStoreParamMap;

	//行动点参数
	ActionDotParam_Map								m_ActionDotParamList;	
	//郡守备队参数
	GMCreateNPCParam_Map							m_HiredNPCArmyList;

	//装备属性表
	HeroEquipAttribute_Map							m_HeroEquipAttributeList;
	HeroEquipAttribute_Multimap_Map_Map				heroEquipScalePosLevelMap;//品质->位置->级别的装备列表
	//精练效果表
	HeroEquipAttribute_Map							m_InlayResultList;
	//精练成功概率表
	RefiningProbability_Map							m_RefiningProbabilityMap;
	//技能效果表
	SkillDetail_Map                                 skillDetailMap_;
	OfficerInitSkill_MultiMap						officerInitSkillMultiMap;
	//打开宝箱是，装备的掉落概率信息

	int												scaleProbability[100];  //品质掉率表
	int												postionProbability[100];//位置掉率表

	//军牌参数表
	ArmyBrand_Map									armyBrandMap;
	ArmyBrand_Map									armyBrandLevelMap;

	//英雄成长系数几率表							
	std::map<int,int>								heroGroupRateMap;
	//广播消息配置表
	BroadMsgParam_Map								broadMsgParamMap;
	//日常任务参数表
	DayTaskParam_Map								dayTaskParamMap;
	//军团升级配置表
	ConsortiaUpdateParam_Map						consortiaUpdateParamMap;
	//数据库DB
	DB_Proxy * dataBaseProxy_;
	//mapID
	int mapID_;
};
extern CLoadDBInfo g_LoadDBinfo;
#endif
