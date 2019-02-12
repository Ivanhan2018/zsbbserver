#pragma once
//特效描述
//备注如果承受方部队ID不为0则效果是针对部队的，反之效果是针对地域的

#include "ProFightSystem.h"
struct EffectDetail{
	unsigned int effectId; //效果ID
	ENUM_SPECIALLY_EFFECT effectType ;//效果类型
	unsigned int useArmyId; //使用方
	unsigned int receiveArmyId; //承受方
	POS_STRUCT pos; //效果位置
	int startBout_; //使用时的回合数
	int endBout_; //结束时的回合数
};

class FightManager;
//效果管理器
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
	//插入效果
	inline unsigned int insertEffect(EffectDetail &effectDetail){
		unsigned int effectId = getNewEffectID();
		effectDetail.effectId = effectId;
		effectMap_[effectId] = effectDetail;
		return effectId;
	}
	inline map<unsigned int ,EffectDetail>& getEffectMap(void){
		return effectMap_;
	}
	//通过效果ID获得效果实体
	inline EffectDetail* getEffectByID(unsigned int effectId){
		map<unsigned int ,EffectDetail>::iterator iter = effectMap_.find(effectId);
		if (iter != effectMap_.end()){
			return &(iter->second);
		}
		return NULL;
	}
	//整理效果
	void tidyEffect(int curBoutNum);
	//清除效果ID
	bool clearEffect(unsigned int effectId);
private:
	//获得新的效果ID
	inline unsigned int getNewEffectID(void){
		return effectId_++;
	}
private:
	unsigned int effectId_; //效果ID
	map<unsigned int ,EffectDetail> effectMap_;//效果集
	FightManager * fightManager_;
};
