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
	/*��ȡ��������,���ؼ���ID
	*/
	int getBaseSkill();
	int getBaseSkillLevel();
	/*��ȡ�����������Ӽ���,���ؼ���ID
	*/
	int getInalySkill();
	int getInalySkillLevel();

	/*��������Ƕ��ʯ)
	@param  inlayitem:��ʯID��
	@param  rate:�ɹ���,ʹ�����˷��ĳɹ��ʼӳ�
	���ص�ǰ����Ƕ����
	*/
	int onInlay(int inlayitem,int rate);
	/*Ӣ���Ƿ���Դ���
	@param playerofficer:Ӣ������
	@param error_code:���ܴ���ԭ�򣬼�����falseʱ������
	*/
	bool isCanDress(PlayerOfficerTable *playerofficer,int &error_code);
	/*����װ��
	@param playerofficer:Ӣ�۵�����
	@param newofficerattr:���Ϻ�Ӣ�����Եı仯
	*/
	bool Dress(PlayerOfficerTable *playerofficer,PlayerOfficerTable *newofficerattr,int &error_code);
	bool Undress(PlayerOfficerTable *playerofficer,PlayerOfficerTable *newofficerattr);
private:
	void initInlayInfo();
private:
	Inlay_Union	itemID_;			//װ��ID
	CLoadDBInfo *loadDBInfo_;	
	HeroEquipAttribute equipAttr_;
	int basePrice_;					//�۸񣬵ͼ�+������ļ۸�ӱ�
	int inlaySkill_;				//�����󣬸��ӵļ���
	int inlaySkillLevel_;			//���Ӽ��ܼ���
	int inlayCount_;				//��������
};
#endif
