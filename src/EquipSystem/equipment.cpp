/*=================================================================*/
/* 文件描述 : 装备类
/* 创建时间 : 2009.11.04
/* 创建者   : xujincai
/* 备注     :
/*
/*-----------------------------------------------------------------*/
/* 修改记录1:
/* 修改时间 :
/* . . .
/*=================================================================*/
#include "equipment.h"
#include "LoadDBInfo.h"
#include <stdlib.h>
#include <assert.h>

IEquipment::IEquipment(CLoadDBInfo *loadDBInfo,__int64 itemID):
loadDBInfo_(loadDBInfo),basePrice_(0),inlaySkill_(0),inlaySkillLevel_(0),inlayCount_(0)
{
	itemID_.NewItemID = itemID;
	initInlayInfo();
}
void IEquipment::resetItem(__int64 itemID)
{
	basePrice_ = 0;
	inlaySkill_ = 0;
	inlayCount_ = 0;
	itemID_.NewItemID = itemID;
	inlaySkillLevel_ = 0;
	inlayCount_ = 0;
	initInlayInfo();
}
void IEquipment::initInlayInfo()
{
	memset(&equipAttr_,0,sizeof(equipAttr_));
	if (loadDBInfo_ == NULL)
	{
		return;
	}
	int itemid = getItemID();
	int inlays[MAX_INLAY_NUMS] = {0};
	if (itemid <=0)
	{
		return ;
	}
	HeroEquipAttribute *heroEquipAttr = loadDBInfo_->GetHeroEquipAttribute(itemid);
	if (heroEquipAttr == NULL)
	{
		return;
	}
	memcpy(&equipAttr_,heroEquipAttr,sizeof(HeroEquipAttribute));
	basePrice_ = equipAttr_.price;
	//equipAttr_.skill = 0;
	getInlayInfo(inlays);
	heroEquipAttr = NULL;

	for (int i=0;i<MAX_INLAY_NUMS;i++)
	{
		if (inlays[i] <=0)
		{
			continue;
		}
		inlayCount_++;
		heroEquipAttr = loadDBInfo_->GetInlayResultAttribute(inlays[i],equipAttr_.postion);
		if (heroEquipAttr)
		{
			equipAttr_ += (*heroEquipAttr);
		}
	}
	//升级到最高时附加一技能
	if (inlayCount_ >=equipAttr_.inlayNums && heroEquipAttr)
	{
		inlaySkill_ = heroEquipAttr->skill;
	}
	if (inlayCount_ >0)
	{
		RefiningProbability *refiningProbability = loadDBInfo_->GetRefiningProbability(inlayCount_);
		if (refiningProbability)
		{
			basePrice_ *= refiningProbability->priceRate;
		}
	}

}
__int64 IEquipment::getInlayItemID() const
{
	return itemID_.NewItemID;
}
int IEquipment::getItemID() const
{
	return itemID_.DetailInfo.GetItemID();
}
int IEquipment::getInlayCount() const
{
	//return itemID_.DetailInfo.GetInlayCount();
	return inlayCount_;
}
int IEquipment::getInlayInfo(int* inlayitems)
{
	itemID_.DetailInfo.GetInlayInfo(inlayitems);
	return 0;
}

HeroEquipAttribute *IEquipment::getEquipAttribute()
{
	return &equipAttr_;
}
int IEquipment::getLevel()
{
	return equipAttr_.level;
}
int IEquipment::getPrice()
{
	return basePrice_;
}
bool IEquipment::isEquipment()
{
	if (getItemID() >0)
	{
		return true;
	}
	return false;
}
bool IEquipment::isCanSale()
{
	return equipAttr_.isSale;
}
int IEquipment::getPostion()
{
	return equipAttr_.postion;
}
int IEquipment::getBaseSkill()
{
	return equipAttr_.skill;
}
int IEquipment::getBaseSkillLevel()
{
	return equipAttr_.skillLevel;
}
int IEquipment::getInalySkill()
{
	return inlaySkill_;
}
int IEquipment::getInalySkillLevel()
{
	return inlaySkillLevel_;
}
int IEquipment::onInlay(int inlayitem,int rate)
{
	if (equipAttr_.itemID <=0)
	{
		return -4;
	}

	if ( inlayCount_>=equipAttr_.inlayNums)
	{
		return -1;
	}
	HeroEquipAttribute *heroEquipAttr = loadDBInfo_->GetInlayResultAttribute(inlayitem,equipAttr_.postion);
	if (heroEquipAttr == NULL)
	{
		return -2;
	}
	/*
	if (heroEquipAttr->postion != INLAYRESOURCE)
	{
		return -3;
	}
	*/
	RefiningProbability *refiningProbability = loadDBInfo_->GetRefiningProbability(inlayCount_+1);
	if (refiningProbability)
	{
		rate = refiningProbability->succeedRate + (refiningProbability->succeedRate * rate)/100;
	}
	else
	{
		rate = 0;
	}
	if (rand()%100 >=rate)
	{
		return 0;
	}
	int ret = itemID_.DetailInfo.Inlay(inlayitem);
	if (ret >0)
	{
		equipAttr_ += (*heroEquipAttr);
		inlayCount_++;
		if (refiningProbability)
			basePrice_ = equipAttr_.price * refiningProbability->priceRate;
	}
	if (ret == equipAttr_.inlayNums)
	{//增加技能
		inlaySkill_ = heroEquipAttr->skill;
	}
	return ret;
}
bool IEquipment::isCanDress(PlayerOfficerTable *playerofficer,int &error_code)
{
	assert(playerofficer);
	assert(playerofficer->jobid_>=0 && playerofficer->jobid_ <=5);
	/*
	int level;
	char jobIDs[6];				//职业
	int postion;				//
	int needStamina;			//耐力需求
	int needPower;				//力量需求
	int needCelerity;			//敏捷需求
	int needWit;				//智力需求
	*/
	if (playerofficer->level_ < equipAttr_.level)
	{
		error_code = EN_LEVEL_NOT_ENOUGH;
		return false;
	}
	if (equipAttr_.jobIDs[playerofficer->jobid_] ==0)
	{
		error_code = EN_JOBNOMATCHING;
		return false;
	}
	if ((playerofficer->stamina_ + playerofficer->levelstamina_ + playerofficer->equipstamina_)<equipAttr_.needStamina)
	{
		error_code = EN_STAMINA_NOT_ENOUGH;
		return false;
	}
	if ((playerofficer->power_ + playerofficer->levelpower_ + playerofficer->equippower_) <equipAttr_.needPower)
	{
		error_code = EN_POWER_NOT_ENOUGH;
		return false;
	}
	if((playerofficer->celerity_ + playerofficer->levelcelerity_ + playerofficer->equipcelerity_)<equipAttr_.needCelerity)
	{
		error_code = EN_CELERITY_NOT_ENOUGH;
		return false;
	}
	if ((playerofficer->wit_ + playerofficer->levelwit_ + playerofficer->equipwit_) <equipAttr_.needWit)
	{
		error_code = EN_WIT_NOT_ENOUGH;
		return false;
	}
	error_code = EN_SUCCESSED;
	return true;
}
bool IEquipment::Dress(PlayerOfficerTable *playerofficer,PlayerOfficerTable *newofficerattr,int &error_code)
{
	if (!isCanDress(playerofficer,error_code))
	{
		return false;
	}
	/*
	int physicAttack;			//物攻
	int physicRecovery;			//物防
	int magicAttack;			//魔攻
	int magicRecovery;			//魔防
	int attackSpeed;			//攻速
	int heroHP;					//生命
	int injureRate;				//重伤率
	int captureRate;			//捕获率
	int beInjureRate;			//受伤减免
	int beCaptureRate;			//被俘减免
	int forceRate;				//暴击率
	*/
	newofficerattr->equipstamina_ += equipAttr_.stamina;
	newofficerattr->equippower_ += equipAttr_.power;
	newofficerattr->equipcelerity_ += equipAttr_.celerity;
	newofficerattr->equipwit_ += equipAttr_.wit;
	newofficerattr->attack_ += equipAttr_.physicAttack;
	newofficerattr->physicrecovery_ += equipAttr_.physicRecovery;
	newofficerattr->magicattack_ += equipAttr_.magicAttack;
	newofficerattr->magicrecovery_ += equipAttr_.magicRecovery;
	newofficerattr->herohp_ += equipAttr_.heroHP;
	newofficerattr->injurerate_ += equipAttr_.injureRate;
	newofficerattr->capturerate_ += equipAttr_.captureRate;
	newofficerattr->beinjurerate_ += equipAttr_.beInjureRate;
	newofficerattr->becapturerate_ += equipAttr_.beCaptureRate;
	newofficerattr->forcerate_ += equipAttr_.forceRate;
	return true;
}


bool IEquipment::Undress(PlayerOfficerTable *playerofficer,PlayerOfficerTable *newofficerattr)
{
	newofficerattr->equipstamina_ -= equipAttr_.stamina;
	newofficerattr->equippower_ -= equipAttr_.power;
	newofficerattr->equipcelerity_ -= equipAttr_.celerity;
	newofficerattr->equipwit_ -= equipAttr_.wit;
	newofficerattr->attack_ -= equipAttr_.physicAttack;
	newofficerattr->physicrecovery_ -= equipAttr_.physicRecovery;
	newofficerattr->magicattack_ -= equipAttr_.magicAttack;
	newofficerattr->magicrecovery_ -= equipAttr_.magicRecovery;
	newofficerattr->herohp_ -= equipAttr_.heroHP;
	newofficerattr->injurerate_ -= equipAttr_.injureRate;
	newofficerattr->capturerate_ -= equipAttr_.captureRate;
	newofficerattr->beinjurerate_ -= equipAttr_.beInjureRate;
	newofficerattr->becapturerate_ -= equipAttr_.beCaptureRate;
	newofficerattr->forcerate_ -= equipAttr_.forceRate;

	playerofficer->equipstamina_ -= equipAttr_.stamina;
	playerofficer->equippower_ -= equipAttr_.power;
	playerofficer->equipcelerity_ -= equipAttr_.celerity;
	playerofficer->equipwit_ -= equipAttr_.wit;
	playerofficer->attack_ -= equipAttr_.physicAttack;
	playerofficer->physicrecovery_ -= equipAttr_.physicRecovery;
	playerofficer->magicattack_ -= equipAttr_.magicAttack;
	playerofficer->magicrecovery_ -= equipAttr_.magicRecovery;
	playerofficer->herohp_ -= equipAttr_.heroHP;
	playerofficer->injurerate_ -= equipAttr_.injureRate;
	playerofficer->capturerate_ -= equipAttr_.captureRate;
	playerofficer->beinjurerate_ -= equipAttr_.beInjureRate;
	playerofficer->becapturerate_ -= equipAttr_.beCaptureRate;
	playerofficer->forcerate_ -= equipAttr_.forceRate;
	return true;	
}
