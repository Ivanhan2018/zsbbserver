#pragma once
#include "Army.h"
class BuildArmy :public Army
{
public:
	BuildArmy(void);
public:
	~BuildArmy(void);
public:
	bool Initialize(BuildArmyParam & armyParam ,FightManager * fightManager ,  ENUM_USE_SCENE state);
	//��ò�����Ұ��С
	inline int GetViewScope() const{
		if (aiLevelParam_){
			return aiLevelParam_->sightVal;
		}
		return viewScope_;
	}
	//��ò��ӹ�����Χ
	inline int GetAttackScope() const{
		return attackScope_;
	}
	
	//��ע�����ݿ�������ID��Type��λ
	//�������ID
	inline int GetWeaponID() const{
		return weaponID_;
	}
	//�������Type
	inline int GetWeaponType() const{
		return weaponType_;
	}
	//��ò���ID
	inline unsigned int GetArmyID() const{
		return armyID_;
	}
	//����ְҵ
	//��ע�����ﲿ�ӵ�ְҵʹ�ý��������ϸ��Ϣ
	//����ְҵ
	virtual SOLDIER_TYPE GetArmyOccupation()const {
		return soldierType_;
	}
	//��ù�������
	inline ATTACK_TYPE GetAttackType()const {
		return armyAttackType_;
	}	
	ARCH_TYPES getBuildDetailType() const {
		return buildDetailType_;
	}
	virtual void onKilled(IArmy* army);
	bool armyOnDie(IArmyPtr attackArmyPtr , bool isCatched , bool isProcessHurt);
	
	//���ý��������ϸ��Ϣ
	inline void setDetailType(ARCH_TYPES detailType){
		buildDetailType_ = detailType;
	}
	//���ģ��ID
	inline int getTemplateID(void) const{
		return tmplateID_;
	}
	//����ģ��ID
	inline void setTemplateID(int tmplateID) {
		tmplateID_ = tmplateID;
	}
	//���ò��Ӵ���
	virtual bool processAction(bool processMode); 
	//ˢ�½����������
	void upgrade(computinghurt::BuildingInfo& info, PublicBuildParamTable * buildParam);
	//��÷���ID
	virtual int GetDefendID() const{
		return defendID_;
	}
	//��÷���Type
	virtual int GetDefendType() const{
		return defendType_;
	}
	//���轨�������Ӫ����
	void resetCountryID(NeutralState neutralState);
	//��ò��Ӿ��Ƶ���ϵ��
	virtual int getArmyCardDropOdds(void) const {
		int buildLevel = buildLevel_ ? buildLevel_ : 1;
		return buildLevel * 200;
	}
	//���ý������ID
	inline  void setBuildID(unsigned int buildID){
		buildID_ = buildID;
	}
	//��ȡ�������ID
	inline unsigned int getBuildID() const{
		return buildID_;
	}
	//��ý����ﵱǰ�ĵȼ�
	inline int getBuildLevel() const {
		return buildLevel_;
	}
protected:
	SOLDIER_TYPE soldierType_;//ʿ��ְҵ
	unsigned int armyID_;//����ID
	int weaponID_; //����ID
	int weaponType_; //��������
	int defendID_; //����ID
	int defendType_; //��������
	int attackPower_;//������
	int attackScope_;//������Ұ
	int viewScope_;//��Ұ
	int tmplateID_;//ģ��ID
	ATTACK_TYPE armyAttackType_;//���ӹ�������
	ARCH_TYPES buildDetailType_; //��������ϸ���� 
	int buildLevel_; //������ȼ�
	int armyCardsType_; // ��������
	int cardsDropOdds_; //�������
	unsigned int buildID_; //������ID
};
