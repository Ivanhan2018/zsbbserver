#pragma once
//��Ч����
//��ע������ܷ�����ID��Ϊ0��Ч������Բ��ӵģ���֮Ч������Ե����

#include "ProFightSystem.h"
struct EffectDetail{
	unsigned int effectId; //Ч��ID
	ENUM_SPECIALLY_EFFECT effectType ;//Ч������
	unsigned int useArmyId; //ʹ�÷�
	unsigned int receiveArmyId; //���ܷ�
	POS_STRUCT pos; //Ч��λ��
	int startBout_; //ʹ��ʱ�Ļغ���
	int endBout_; //����ʱ�Ļغ���
};

class FightManager;
//Ч��������
class EffectManager
{
public:
	EffectManager(void);
public:
	~EffectManager(void);
public:

	inline void setFightManager(FightManager * fightManager){
		fightManager_ = fightManager;
	}
	//����Ч��
	inline unsigned int insertEffect(EffectDetail &effectDetail){
		unsigned int effectId = getNewEffectID();
		effectDetail.effectId = effectId;
		effectMap_[effectId] = effectDetail;
		return effectId;
	}
	inline map<unsigned int ,EffectDetail>& getEffectMap(void){
		return effectMap_;
	}
	//ͨ��Ч��ID���Ч��ʵ��
	inline EffectDetail* getEffectByID(unsigned int effectId){
		map<unsigned int ,EffectDetail>::iterator iter = effectMap_.find(effectId);
		if (iter != effectMap_.end()){
			return &(iter->second);
		}
		return NULL;
	}
	//����Ч��
	void tidyEffect(int curBoutNum);
	//���Ч��ID
	bool clearEffect(unsigned int effectId);
private:
	//����µ�Ч��ID
	inline unsigned int getNewEffectID(void){
		return effectId_++;
	}
private:
	unsigned int effectId_; //Ч��ID
	map<unsigned int ,EffectDetail> effectMap_;//Ч����
	FightManager * fightManager_;
};
