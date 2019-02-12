
#include "BuildArmy.h"
#include "computinghurt/common.h"
#include "FightManager.h"
#include "ArmAttack.h"
BuildArmy::BuildArmy(void)
{
}

BuildArmy::~BuildArmy(void)
{
	if(computehurt_)
	{
		computehurt_->release();
		computehurt_ = NULL;
	}
	if(action_)
	{
		delete action_;
		action_ = 0;
	}
	list<ArmAttack*>::iterator iter;
	for (iter = attackActions_.begin() ; iter != attackActions_.end() ; ++iter)
	{
		delete (*iter);
		(*iter) = NULL;
	}
	attackActions_.clear();
}
bool BuildArmy::Initialize(BuildArmyParam & armyParam ,FightManager * fightManager , ENUM_USE_SCENE state){
	moveSpeed_ = 0;
	fightManager_ = fightManager;
	soldierType_ = SHOOTER;
	armyAttackType_ = PHYSIC_ATTACK;
	armyType_ = BUILD;
	weaponType_ = 4;
	weaponID_ = 26;
	defendID_ = armyParam.buildParam.denfenid_;
	buildLevel_ = armyParam.buildParam.level_;
	armyCardsType_ = armyParam.buildParam.armyCardsType_; // 军牌类型
	cardsDropOdds_ = armyParam.buildParam.cardsDropOdds_; //掉落概率
	DefendAttribute * defendAttribute = fightManager_->getDefendAttribute(defendID_);
	if (defendAttribute)
	{
		defendType_ = defendAttribute->defendType;
	}
	else
	{
		defendType_ = 0;
	}
	armyID_ = armyParam.armyID;
	attackPower_ = armyParam.buildParam.attack_;
	attackScope_ =armyParam.buildParam.attackscop_;
	viewScope_ =armyParam.buildParam.visualrange_;
	armyPos_ = armyParam.armyPos;
	armyBirthPos_ = armyPos_[0];
	computinghurt::BuildingInfo buildInfo;
	if (armyParam.defendAttribute){
		buildInfo.physicDefense = armyParam.defendAttribute->physiceRec;
		buildInfo.magicDefense = armyParam.defendAttribute->magicRec;
	}
	else
	{
		buildInfo.physicDefense = 0;
		buildInfo.magicDefense = 0;
	}
	buildInfo.hp = armyParam.buildParam.hp_;
	buildInfo.power = attackPower_;
	buildInfo.weaponId = weaponID_;
	computehurt_ = computinghurt::HurtFactory::createBuildingHurt(this , buildInfo);
	if (computehurt_ == NULL){
		return false;
	}
	isTrusteeship_ = true; 
	lastAttackTarget_ = 0;
	lastEnmityTarget_ = 0;
	countryID_ = 0;
	buildDetailType_ = static_cast<ARCH_TYPES>(armyParam.buildParam.pbuildingkind_);
	switch(buildDetailType_){
	case ARCH_TRAP:
		soldierType_ = SOLDIER_TYPE_TRAP;
		break;
	case ARCH_BARRIER:
		soldierType_ = SOLDIER_TYPE_BARRIER;
		break;
	default:
		soldierType_ = SHOOTER;
	}
	tmplateID_ = armyParam.buildParam.templateid_;
	WeaponAttribute * armyWeapon = fightManager_->getWeaponAttribute(GetWeaponID());
	if (armyWeapon == NULL){
		return false;
	}
	attackFreq_ = armyWeapon->attackFreq;
	attackType_ = (AttackTypeEnu) armyWeapon->attackType;
	attackLockBout_ = armyWeapon->actionBout;
	trajectorySpeed_ = armyWeapon->trajectorySpeed;
	return createArmyAction(armyParam.mapData , state , NULL) ;
}

void BuildArmy::onKilled(IArmy* army){
	if (army == NULL){
		return ;
	}
	army->armyOnDie(this , false ,true);
}
bool BuildArmy::armyOnDie(IArmyPtr attackArmyPtr , bool /*isCatched*/ , bool /*isProcessHurt*/){
	if (attackArmyPtr == NULL){
		return false;
	}
	fightManager_->processArmyCardDrop(attackArmyPtr , this ,armyCardsType_  , cardsDropOdds_);
	fightManager_->onArmyDie(attackArmyPtr,this);
	return true;
}
void BuildArmy::upgrade(computinghurt::BuildingInfo& info ,PublicBuildParamTable * buildParam){
	computinghurt::IBuildingHurt* computeBuildHurt = dynamic_cast
		<computinghurt::IBuildingHurt*>(computehurt_);
	if (computeBuildHurt == NULL){
		return ;
	}
	info.weaponId = GetWeaponID();
	attackScope_ = buildParam->attackscop_;
	computeBuildHurt->upgrade(info);
	tmplateID_ = buildParam->templateid_;
	buildLevel_ = buildParam->level_;
	armyCardsType_ = buildParam->armyCardsType_; // 军牌类型
	cardsDropOdds_ = buildParam->cardsDropOdds_; //掉落概率
}
void BuildArmy::resetCountryID(NeutralState neutralState){
	countryID_ = fightManager_->getLabelCountryID(neutralState);
}
//调用部队处理
bool BuildArmy::processAction(bool processMode) {
	isActioned_ = false;
	if (buildDetailType_ >= ARCH_BUILD_TOFT && buildDetailType_ <= ARCH_TOFT_END){
		return isActioned_;
	}
	if(buildDetailType_ == ARCH_RES_BUILDING){
		isActioned_ = true;
		fightManager_->collectRes(this);
	}
	else
	{
		list<ArmAttack*>::iterator iter;
		for(iter = attackActions_.begin(); iter != attackActions_.end(); ){
			ArmAttack * attacker = *iter;
			if(attacker->action()){
				iter = attackActions_.erase(iter);	
				delete attacker;
				attacker = NULL;
				
			}
			else
			{
				++iter;
			}
		}
		if (!processMode || !(processMode && isCanUseWeapon())){
			return isActioned_;
		}
		action_->thinking(processMode);
	}
	return isActioned_;
}