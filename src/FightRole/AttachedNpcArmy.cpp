#include "AttachedNpcArmy.h"
#include <boost/serialization/singleton.hpp>
#include "computinghurt/randomgenerator.h"
#include "FightSystemMacro.h"
#include "FightManager.h"
AttachedNpcArmy::AttachedNpcArmy(void)
{
}

AttachedNpcArmy::~AttachedNpcArmy(void)
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
bool AttachedNpcArmy::Initialize(AttachedNpcArmyParam & armyParam , FightManager * fightManager, ENUM_USE_SCENE state , bool &isEnterSystem){
	fightManager_ = fightManager;
	npcArmyTable_ = armyParam.npcArmyTable;
	npcArmyInfo_  = armyParam.npcArmyInfo;
	if (npcArmyTable_ == NULL || npcArmyInfo_ == NULL || fightManager == NULL){
		return false;
	}
	IArmyPtr army = fightManager_->getArmyByID(armyParam.adscriptionArmyID);
	if (fightManager_->isValidArmy(army) == NULL){
		return false;
	}
	POS_STRUCT tmpPos;

	tmpPos = army->GetArmyPos()[0];
	armyPos_.resize(1 , tmpPos);
	armyBirthPos_ = tmpPos;
	armyType_ = NPC_ARMY;
	
	countryID_ = army->GetCountryID();
	
	if(createComputeHurt() == false){
		return false;
	}
	AiLevelParam * aiLevelParam = fightManager_->getAiLevel(armyParam.npcArmyTable->aiLevel);
	if(createArmyAction(armyParam.mapData , state ,aiLevelParam) == false){
		return false;
	}
	action_->setStrategy((enumAIStrategy)npcArmyTable_->strategyType);
	isTrusteeship_ = true;
	followTarget_ = NULL;
	lastAttackTarget_ = 0;
	lastEnmityTarget_ = 0; 
	PatrolPosVect * patrolVect = fightManager_->getPatrolData(GetArmyID());
	if (patrolVect){
		action_->setPatrolPoses(*patrolVect);
	}
	if (fightManager_->getCurCycleNum() >= npcArmyTable_->enterTime)
	{
		isEnterSystem = true;
	}
	else
	{
		isEnterSystem = false;
	}
	if (npcArmyTable_->rebornTime > 0)
	{
		isRepeatArmy_ = true;
	}
	WeaponAttribute * armyWeapon = fightManager_->getWeaponAttribute(GetWeaponID());
	if (armyWeapon == NULL){
		return false;
	}
	attackFreq_ = armyWeapon->attackFreq;
	attackType_ = (AttackTypeEnu)armyWeapon->attackType;
	attackLockBout_ = armyWeapon->actionBout;
	trajectorySpeed_ = armyWeapon->trajectorySpeed;
	return true;
}
void AttachedNpcArmy::onKilled(IArmy* army){
	if (army == NULL){
		return ;
	}
	army->armyOnDie(this , false ,true);
}

bool AttachedNpcArmy::armyOnDie(IArmyPtr attackArmyPtr , bool /*isCatched*/ , bool /*isProcessHurt*/){
	computinghurt::IRandomGenerator* randomgenerator = &boost::serialization::singleton<computinghurt::RandomGenerator>::get_mutable_instance();
	//处理掉落物品
	if(randomgenerator->generator(0 , 100) < npcArmyTable_->dropOdds){
		fightManager_->processBooty(attackArmyPtr , npcArmyTable_->dropItem , TREASUREPACK); 
	}
	fightManager_->processArmyCardDrop(attackArmyPtr , this ,npcArmyTable_->armyCardsType , npcArmyTable_->cardDropOdds);
	//
	fightManager_->onArmyDie(attackArmyPtr , this);
	deadCycle_ = fightManager_->getCurCycleNum();
	return true;
}
bool AttachedNpcArmy::createComputeHurt(){
	computinghurt::IRandomGenerator* randomgenerator = &boost::serialization::singleton<computinghurt::RandomGenerator>::get_mutable_instance();
	computinghurt::ArmyInfo armyInfo = {0};

	computinghurt::SkillInfo officerSkill;
	
	if(fightManager_->getSkillInfo(officerSkill , static_cast<SKILL_TYPE>(npcArmyInfo_->skill) , npcArmyInfo_->skillLevel)){
		armyInfo.heroInfo.skillsinfo.push_back(officerSkill);
	}
	armyInfo.soldierInfo.hp = npcArmyInfo_->soldierHp; 
	armyInfo.soldierInfo.shootingRate = 100;
	armyInfo.soldierInfo.numberOfSoldiers = npcArmyTable_->soldierNum;
	movePerNeedTime_ = npcArmyInfo_->movePerNeedTime;
	calculateSpeed();
	armyInfo.soldierInfo.power = npcArmyInfo_->attackPower;
	armyInfo.soldierInfo.type = (SOLDIER_TYPE) npcArmyInfo_->soldierType;
	armyInfo.soldierInfo.weaponId = npcArmyInfo_->weaponID;
	armyInfo.soldierInfo.magicDefense = npcArmyInfo_->magicDependPower;
	armyInfo.soldierInfo.physicDefense = npcArmyInfo_->phsicDependPower;
	if (computehurt_){
		delete computehurt_;
		computehurt_ = NULL;
	}
	computehurt_ = computinghurt::HurtFactory::createArmyHurt(armyInfo, fightManager_->getLandFormMap() ,this , randomgenerator);
	if (computehurt_ == NULL){
		return false;
	}
	npcDirection_ = (FORWORD)randomgenerator->generator(1 , 4);
	return true;
}

bool AttachedNpcArmy::Move(vector<POS_STRUCT>& moveTrack , IArmyPtr armyPtr){
	
	size_t i = 0;
	if (moveTrack.size() == 0){
		return false;
	}
	SEND_ARMY_MOVE_INFO_STRUCT *msgArmyMovePtr = (SEND_ARMY_MOVE_INFO_STRUCT*)new char [sizeof(SEND_ARMY_MOVE_INFO_STRUCT) 
		+ moveTrack.size() * sizeof(GridCoordinate)];
	if (msgArmyMovePtr == NULL){
		return false;
	}
	POS_STRUCT armyPos = armyPos_[0];
	for (; i < moveTrack.size() && i < 3 ; i++){
		//修改部队方向
		npcDirection_ = changeDirection(armyPos , moveTrack[i]);
		armyPos = moveTrack[i]; 
		(*msgArmyMovePtr)[i].x = moveTrack[i].PosX;
		(*msgArmyMovePtr)[i].y = moveTrack[i].PosY;
	}
	if(fightManager_->moveArmy(armyPtr, moveTrack[i-1]) == false){
		delete [] (char*)msgArmyMovePtr;
		msgArmyMovePtr = NULL;
		return false;
	}
	msgArmyMovePtr->needTime = FIGHT_TIMER_CYCLE * moveLockBout_;
	msgArmyMovePtr->step = static_cast<int> (i);
	msgArmyMovePtr->Direction = getDirection();
	msgArmyMovePtr->ArmyID = GetArmyID();
	msgArmyMovePtr->uMapId = fightManager_->getMapID();
	msgArmyMovePtr->serialize(0 , fightManager_->getProcessID() ,0,RSP_MOVE_INFO);
	fightManager_->msgBroadcast(msgArmyMovePtr,RSP_MOVE_INFO,msgArmyMovePtr->length);
	delete [] (char*)msgArmyMovePtr;
	msgArmyMovePtr = NULL;
	Army::Move(moveTrack , armyPtr);
	return true;
}

IArmyPtr AttachedNpcArmy::GetFollowTarget() const {
	return fightManager_->getArmyByID(followTarget_);
}

//部队重生
bool AttachedNpcArmy::revival(bool&  isTerminate){

	int curCycle = fightManager_->getCurCycleNum();
	if (isRepeatArmy_ || (npcArmyTable_->quitTime <= curCycle) || (!npcArmyTable_->rebornTime)){
		isTerminate = true;
		return false;
	}
	else
	{
		isTerminate = false;
	}
	IArmyPtr lordArmy = fightManager_->getArmyByID(lordArmyID_);
	if (lordArmyID_ && (lordArmy == NULL || lordArmy->GetCurrentHp() == 0))
	{
		isTerminate = true;
		return false;
	}
	if (npcArmyTable_->enterTime > curCycle)
	{
		return false;
	}

	if (npcArmyTable_->enterTime < curCycle)
	{
		if (curCycle - deadCycle_ < npcArmyTable_->rebornTime)
		{
			return false;
		}
	}
	countryID_ = lordArmy->GetCountryID();
	if (createComputeHurt() == false){
		assert(false);
		return false;
	}
	POS_STRUCT pos = GetBirthPos();
	pos = getValidPos(pos , 1);
	armyPos_[0] = pos;
	return true;
}
//部队复制
IArmyPtr AttachedNpcArmy::repeat(ENUM_USE_SCENE /*state*/ , bool& isTerminate){
	isTerminate = true;
	return NULL;
}
int AttachedNpcArmy::GetDefendType() const{
	DefendAttribute * defendAttribute = fightManager_->getDefendAttribute(GetDefendID());
	if (defendAttribute){
		return defendAttribute->defendType;
	}
	return  0;
}