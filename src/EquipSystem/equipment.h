#ifndef EQUIPMENT_H__
#define EQUIPMENT_H__
#include "MapServer_Protocol.h"
#include "DB_Table.h"
#include "ZSBB_DataDefine.h"

class CLoadDBInfo;

class IEquipment
{
	enum {MAX_INLAY_NUMS=5};
public:
	IEquipment(CLoadDBInfo *loadDBInfo,__int64 itemID);
private:
	IEquipment(IEquipment &);
	IEquipment& operator=(IEquipment &);

public:
	void resetItem(__int64 itemID);
	__int64 getInlayItemID() const;
	int getItemID() const;
	int getInlayCount() const;
	int getInlayInfo(int* inlayitems);
	HeroEquipAttribute *getEquipAttribute();
	int getLevel();
	int getPrice();
	bool isEquipment();
	bool isCanSale();
	int getPostion();
	/*获取武器技能,返回技能ID
	*/
	int getBaseSkill();
	int getBaseSkillLevel();
	/*获取武器精练附加技能,返回技能ID
	*/
	int getInalySkill();
	int getInalySkillLevel();

	/*精练（镶嵌宝石)
	@param  inlayitem:宝石ID；
	@param  rate:成功率,使用幸运符的成功率加成
	返回当前的镶嵌次数
	*/
	int onInlay(int inlayitem,int rate);
	/*英雄是否可以穿戴
	@param playerofficer:英雄属性
	@param error_code:不能穿的原因，即返回false时才有用
	*/
	bool isCanDress(PlayerOfficerTable *playerofficer,int &error_code);
	/*穿上装备
	@param playerofficer:英雄的属性
	@param newofficerattr:穿上后，英雄属性的变化
	*/
	bool Dress(PlayerOfficerTable *playerofficer,PlayerOfficerTable *newofficerattr,int &error_code);
	bool Undress(PlayerOfficerTable *playerofficer,PlayerOfficerTable *newofficerattr);
private:
	void initInlayInfo();
private:
	Inlay_Union	itemID_;			//装备ID
	CLoadDBInfo *loadDBInfo_;	
	HeroEquipAttribute equipAttr_;
	int basePrice_;					//价格，低价+精练后的价格加倍
	int inlaySkill_;				//精炼后，附加的技能
	int inlaySkillLevel_;			//附加技能级别
	int inlayCount_;				//精练次数
};
#endif
