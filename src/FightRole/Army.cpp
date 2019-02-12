#include "Army.h"

#include "FightManager.h"
#include "CommInterfaceHeader.h"
#include "log.hpp"
#include "ArmAttack.h"
void setHarmParam(const HarmInfo& harmInfo, HarmParm& harmParam)
{
	harmParam = *harmInfo.get();
	harmParam.LostHp = -harmParam.LostHp;
}

void setFightReport(const AttackInfo& attackInfo, REPORT_FIGHT_INFO_STRUCT*& fightReport)
{
	HarmInfos& harminfos = attackInfo->harmInfos;
	fightReport = (REPORT_FIGHT_INFO_STRUCT*)new char[sizeof(REPORT_FIGHT_INFO_STRUCT) + harminfos.size() * sizeof(HarmParm)];
	if (fightReport == NULL){
		return ;
	}
	fightReport->ulAttackArmyID = attackInfo->armyId;
	fightReport->uWeaponID = attackInfo->weaponId;
	fightReport->AttackType = attackInfo->attackType;
	fightReport->Skill = attackInfo->skillId;
	fightReport->obtainExp = 0;
	fightReport->count = static_cast<unsigned int>(harminfos.size());

	HarmInfos::const_iterator it = harminfos.begin();
	for (unsigned int i = 0; i < fightReport->count; ++i)
	{
		HarmInfo info = (*it);
		setHarmParam(info, (*fightReport)[i]);
		fightReport->obtainExp += info->obtainExp;
		++it;
	}
	fightReport->serialize(0,0,0,RSP_FIGHT_INFO);
}



Army::Army(void):
exp_(0),
lastAttackBoutNum_(-1),
lastMoveBoutNum_(-1),
attackFreq_(1),
isRepeatArmy_(false),
computehurt_(NULL),
action_(NULL),
isVisible_(false),
lordArmyID_(0),
revivalNum_(0),
centryPos_()
{
	
}

Army::~Army(void)
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
}
void Army::Attack(unsigned int armyId, unsigned int numCircle){

	IArmyPtr enemyPtr = getArmyByID(armyId);

	if (NULL == enemyPtr || NULL == action_ || NULL == fightManager_)
	{
		assert(0);
		return;
	}
	
	if (enemyPtr->getBuildDetailType() >= ARCH_OCCUPY_BUILDING_START && enemyPtr->getBuildDetailType() <= ARCH_OCCUPY_BUILDING_END){
		return ;
	}
	const std::vector<POS_STRUCT> &posStructVect = enemyPtr->GetArmyPos();

	if (0 == posStructVect.size())
	{
		assert(0 && "部队坐标为空");
		return;		
	}

	const POS_STRUCT &posStru = posStructVect[0];

	// 陷阱攻击
	if (getBuildDetailType() == ARCH_TRAP)
	{
		Armys armyPtrsCenter, armyPtrsNear;
		// 1.恢复攻击具体攻击对象
		MaterialEnemyNameSpace::getMaterialEnemy(0, this, enemyPtr, action_, posStru, true, armyPtrsCenter, armyPtrsNear);

		if (!armyPtrsCenter.empty())
		{
			Attack(armyPtrsCenter);
		}

		return;
	}

	const std::vector<POS_STRUCT> &armyPosStruVect = GetArmyPos();

	if (0 == armyPosStruVect.size())
	{
		assert(0 && "部队坐标为空");
		return;		
	}

	const POS_STRUCT &armyPosStru = armyPosStruVect[0];

	unsigned int trajectoryBout = 0;
		
	if (0 == trajectorySpeed_)
	{
		trajectoryBout = 0;
	}
	else
	{
		trajectoryBout = getDistanceTwoNode(armyPosStru.PosX, armyPosStru.PosY, posStru.PosX, posStru.PosY) / trajectorySpeed_ * 500;
	}

	ArmAttack *armyAttPtr = NULL;

	bool isFollowAttack = false;

	if (Attack_Follow == isFollowAttack)
	{
		isFollowAttack = true;
	}

	switch(attackType_)
	{
	case Attack_Place:
		armyAttPtr = new PointAttack(numCircle, attackLockBout_, trajectorySpeed_, trajectoryBout, action_, fightManager_, this, posStru);

		if (NULL == armyAttPtr)
		{
			return;
		}

		// 把攻击动作的播放动作发出去	
		AttackPoint attackPoint;
		attackPoint.armyId = GetArmyID();
		attackPoint.x = posStru.PosX;
		attackPoint.y = posStru.PosY;
		attackPoint.actionTime = attackLockBout_;
		attackPoint.flyTime = trajectoryBout;

		// 发送
		fightManager_->msgBroadcast(&attackPoint, EVENT_ACTION_ATTACK_PONIT, sizeof(AttackPoint));
		break;
	case Attack_Follow:
		armyAttPtr = new TargetAttack(numCircle, attackLockBout_, trajectorySpeed_, 1, action_, fightManager_,  this, enemyPtr->GetArmyID(), isFollowAttack);

		if (NULL == armyAttPtr)
		{
			return;
		}

		// 把攻击动作的播放动作发出去
		AttackTarget attackTarget;
		attackTarget.armyId = GetArmyID();
		attackTarget.targetId = armyId;
		attackTarget.actionTime = attackLockBout_;
		attackTarget.flyTime = 500;

		// 发送
		fightManager_->msgBroadcast(&attackTarget, EVENT_ACTION_ATTACK_TARGET, sizeof(AttackTarget));	

		break;
	case Attack_Instantaneous:
		armyAttPtr = new TargetAttack(numCircle, attackLockBout_, trajectorySpeed_, 0, action_, fightManager_,  this, enemyPtr->GetArmyID(), isFollowAttack);

		if (NULL == armyAttPtr)
		{
			return;
		}

		// 把攻击动作的播放动作发出去
		attackTarget.armyId = GetArmyID();
		attackTarget.targetId = armyId;
		attackTarget.actionTime = attackLockBout_;
		attackTarget.flyTime = 0;

		// 发送
		fightManager_->msgBroadcast(&attackTarget, EVENT_ACTION_ATTACK_TARGET, sizeof(AttackTarget));	

		break;
	default:
		break;
	}

	if (NULL != armyAttPtr)
	{	
		attackActions_.push_back(armyAttPtr);
	}
	lastAttackBoutNum_ = fightManager_->getCurBoutNum();
}

void Army::Attack(Armys &armyPtrsCenter, Armys &armyPtrsNear){
	AttackInfo attackInfo = computehurt_->attack(armyPtrsCenter , armyPtrsNear);
	REPORT_FIGHT_INFO_STRUCT *fightReport = NULL;
	setFightReport(attackInfo, fightReport);
	if (fightReport == NULL){
		return ;
	}
	for(unsigned int i = 0 ; i < fightReport->count; i++){
		fightManager_->onKilledEnemy(this , (*fightReport)[i].DealNum);
		fightManager_->onSoliderDie(this , fightManager_->getArmyByID((*fightReport)[i].ArmyID) , (*fightReport)[i].DealNum);
	}
	for (size_t i = 0 ; i < armyPtrsCenter.size() ; i++){
		if (armyPtrsCenter[i]->GetCurrentHp() == 0){
			onKilled(armyPtrsCenter[i]);
		}
	}
	for (size_t i = 0 ; i < armyPtrsNear.size() ; i++){
		if (armyPtrsNear[i]->GetCurrentHp() == 0){
			onKilled(armyPtrsNear[i]);
		}
	}
	//消息协议需要修改发送
	fightManager_->msgBroadcast(fightReport,RSP_FIGHT_INFO, fightReport->length);
	delete [] (char *)fightReport;
	fightReport = NULL;
	lastAttackBoutNum_ = fightManager_->getCurBoutNum();
	officerGrowUp();
	isActioned_ = true;
	//如果是该攻击是陷阱，则陷阱会损坏
	if (getBuildDetailType() == ARCH_TRAP){
		armyOnDie(armyPtrsCenter[0] , false , true);
	}
}
void Army::getArmysFromArmyArrays(ArmyArrays &armys , Armys& armyPtrs){
	size_t len = armys.size();
	IArmyPtr army;
	for (size_t i = 0; i < len ; i ++){
		army = fightManager_->getArmyByID(armys[i]);
		if (fightManager_->isValidArmy(army)){
			if (GetCountryID() == army->GetCountryID() && GetArmyOccupation() != PRIEST)
			{
				LOG_ERROR(("ARMY：ATTACK攻击者的部队ID为：%d，归属playerID为：%d，国家ID为：%d，部队职业:%d， 带队英雄职业:%d;  被攻击的部队ID为：%d，归属playerID为：，国家ID为：%d，部队职业:%d， 带队英雄职业:%d", 
					GetArmyID(),GetOfficerID(), GetCountryID(), GetArmyOccupation(), getHeroOccupation(), 
					army->GetArmyID(), army->GetOfficerID(), army->GetCountryID(), army->GetArmyOccupation(), army->getHeroOccupation()));
			}
			else
			{
				armyPtrs.push_back(army);
			}

		}

	}
}
void Army::Attack(Armys &armyPtrs)
{
	if(computehurt_ == 0)
		return ;

	if (armyPtrs.size() == 0){
		return ;
	}
	if(armyPtrs.size() == 1 && armyPtrs[0]->getBuildDetailType() == ARCH_RES_BUILDING){
		EffectDetail *tmpEffect = getEffectByType(EFFECT_OCCUPY);
		if (tmpEffect == NULL ){
			useEffect(EFFECT_OCCUPY , armyPtrs[0],armyPtrs[0]->GetCurrentHp());
			return;
		}
		if (tmpEffect->receiveArmyId !=  armyPtrs[0]->GetArmyID())
		{
			clearEffectByType(EFFECT_OCCUPY , true);
			useEffect(EFFECT_OCCUPY , armyPtrs[0],armyPtrs[0]->GetCurrentHp());
			return;
		}
		if (fightManager_->getCurBoutNum() >= tmpEffect->endBout_){
			armyPtrs[0]->setCountryID(GetCountryID());
			MsgChangeCountry tmpMsg;
			tmpMsg.armyId = tmpEffect->receiveArmyId;
			tmpMsg.countryId = GetCountryID();
			tmpMsg.serialize(fightManager_->getProcessID() , 0);
			fightManager_->msgBroadcast(&tmpMsg ,MSG_CHANGE_COUNTRY , tmpMsg.length);
			clearEffectByType(EFFECT_OCCUPY , true);
			armyPtrs[0]->SetLastEnmityTarget(0);
		}
		return ;
	}
	else
	{
		clearEffectByType(EFFECT_OCCUPY , true);
	}
	AttackInfo attackInfo = computehurt_->attack(armyPtrs);
	REPORT_FIGHT_INFO_STRUCT *fightReport = NULL;
	setFightReport(attackInfo, fightReport);
	if (fightReport == NULL){
		return ;
	}
	for(unsigned int i = 0 ; i < fightReport->count; i++){
		fightManager_->onKilledEnemy(this , (*fightReport)[i].DealNum);
		fightManager_->onSoliderDie(this ,armyPtrs[i] ,(*fightReport)[i].DealNum);
	}
	for (size_t i = 0 ; i < armyPtrs.size() ; i++){
		if (armyPtrs[i]->GetCurrentHp() == 0){
			onKilled(armyPtrs[i]);
		}
	}
	//发送
	fightManager_->msgBroadcast(fightReport,RSP_FIGHT_INFO, fightReport->length);
	lastAttackBoutNum_ = fightManager_->getCurBoutNum();
	officerGrowUp();
	isActioned_ = true;
	//如果是该攻击是陷阱，则陷阱会损坏
	if (getBuildDetailType() == ARCH_TRAP){
		armyOnDie(armyPtrs[0] , false , true);
	}
	delete [] (char*)fightReport;
	fightReport = NULL;
}
IArmyPtr Army::getArmyByID(unsigned int armyID){
	return fightManager_->getArmyByID(armyID);
}

FORWORD Army::changeDirection(POS_STRUCT& curPos,POS_STRUCT& nextPos){
	int x = curPos.PosX - nextPos.PosX;
	int y = curPos.PosY - nextPos.PosY;
	if (curPos.PosX & 2){
		if(x > 0 && y == 0){
			return LEFTUP;
		}
		if (x > 0 && y < 0){
			return RIGHTUP;
		}
		if (x<0 && y == 0){
			return LEFTDWON;
		}
	}
	else{
		if (x > 0 && y >0){
			return LEFTUP;
		}
		if (x > 0 && y == 0) {
			return RIGHTUP;
		}
		if (x < 0 && y > 0) {
			return LEFTDWON;
		}
	}
	return RIGHTDOWN;
}
bool Army::createArmyAction(MapDataStru& mapData , ENUM_USE_SCENE sceneState , AiLevelParam *aiLeveParam){
	aiLevelParam_ = aiLeveParam;
	if (action_){
		delete action_;
		action_ = NULL;
	}
	action_ = new ArmyAction(mapData , this ,sceneState , aiLeveParam , fightManager_->getSeekPather() , fightManager_->getLandFormMap());
	if (action_ == NULL){
		return false;
	}
	return true;
}
float Army::getFactorAccordingToArmor(IArmy* beAttacker){
	return fightManager_->getHurtOdds(GetWeaponType(), beAttacker->GetDefendType()) / (float) 100.0;
}
//获得部队当前所处地形
LAND_FORM Army::getLandForm(void) const {
	return fightManager_->getLandForm(armyPos_[0].PosX , armyPos_[0].PosY);
}
//获得特定坐标
const std::list<IArmyPtr> *  Army::getArmysInPos(int x , int y) const{
	return fightManager_->getArmysInPos(x , y );
}
//获得地图状态
mapstate Army::getMapState(void) const {
	return fightManager_->getMapState();
}
//是否新手村
bool Army::isNewbie(void)  const{
	return fightManager_->isNewbie();
}
bool Army::exportArmyInfo(ARMY_POS & armyInfo , int mapID){
	armyInfo.bMove = false;
	armyInfo.MapID = mapID;
	armyInfo.ArmyInfo.ulArmyID = GetArmyID();
	armyInfo.ArmyInfo.ulOfficerID = GetOfficerID();
	armyInfo.ArmyInfo.ulPlayerID = getPlayerID();
	//坐标和客户端交互时需要逆转
	POS_STRUCT tmpPos = armyPos_[0];
	armyInfo.PosX = tmpPos.PosX;
	armyInfo.PosY = tmpPos.PosY;
	armyInfo.Direction = getDirection();
	armyInfo.ArmyInfo.ucArmyState = static_cast<unsigned char> (getArmyDBState());
	armyInfo.ArmyInfo.Trustflag = isTrusteeship_;
	armyInfo.ArmyInfo.soldierAttack = GetAttackPower();
	armyInfo.ArmyInfo.heroAttack = GetAttackPower();
	armyInfo.ArmyInfo.ArmyType = GetArmyType();
	armyInfo.ArmyInfo.Soldier.count = getSoldierNum();
	armyInfo.ArmyInfo.Soldier.defendID = GetDefendID();
	armyInfo.ArmyInfo.Soldier.Race = GetSoldierRace();
	armyInfo.ArmyInfo.Soldier.SoldierType = GetArmyOccupation();
	armyInfo.ArmyInfo.Soldier.weaponID = GetWeaponID();
	armyInfo.ArmyInfo.iTotalHP = getInitHp();
	armyInfo.ArmyInfo.iCurrentHp = GetCurrentHp();
	armyInfo.ArmyInfo.heroFaceId = GetFaceID();
	armyInfo.ArmyInfo.Role = (ARMY_ROLE) GetCountryID();
	memset(armyInfo.ArmyInfo.ArmyName,0,SIZE_32);
	if (GetArmyName()){
		memcpy(armyInfo.ArmyInfo.ArmyName,GetArmyName(),SIZE_32);
	}
	armyInfo.ArmyInfo.heroLevel = GetOfficerLevel();
	const std::string& playerName = getPlayerName();
	int playerNameLen = playerName.length() > SIZE_32 ? playerName.length() : SIZE_32 -1;
	memcpy(armyInfo.ArmyInfo.officerName,playerName.c_str(),playerNameLen);
	const std::string& cropsName = getPlayerName();
	int cropsNameLen = cropsName.length() > SIZE_32 ? cropsName.length() : SIZE_32 -1;
	memcpy(armyInfo.ArmyInfo.corpsName,cropsName.c_str(),cropsNameLen);
	return true;
}
//获得当前回合数
int Army::getCurBoutNum(void) const {
	return fightManager_->getCurBoutNum();
}
//获得部队军牌掉落系数
int Army::getArmyCardDropOdds(void) const{
	int dropOdds = 1;
	switch(fightManager_->getFightScene()){
	case County_AI:
	{
		IArmyPtr armyPtr = fightManager_->getArmyByID(lordArmyID_);
		if (armyPtr == NULL){
			break;
		}
		dropOdds = armyPtr->getArmyCardDropOdds();
	}
	case Counterpart_AI:
		dropOdds = fightManager_->getEctypeDownLevel() * 100;
		break;
	}
	dropOdds = dropOdds ? dropOdds : 1;
	return dropOdds;
}
void Army::tidyEffect(void){
	map<ENUM_SPECIALLY_EFFECT ,unsigned int>::iterator iter;
	for (iter = effectRecord_.begin() ; iter != effectRecord_.end();)
	{
		EffectDetail * tmpEffect = fightManager_->getEffectByID(iter->second);
		if (tmpEffect!=NULL && tmpEffect->endBout_ <= fightManager_->getCurBoutNum()){
			fightManager_->clearEffect(tmpEffect->effectId);
			iter = effectRecord_.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
EffectDetail * Army::getEffectByType(ENUM_SPECIALLY_EFFECT type){
	map<ENUM_SPECIALLY_EFFECT ,unsigned int>::iterator iter;
	iter = effectRecord_.find(type);
	if (iter == effectRecord_.end()){
		return NULL;
	}
	return fightManager_->getEffectByID(iter->second);
}
unsigned int Army::useEffect(ENUM_SPECIALLY_EFFECT type , IArmyPtr armyPtr ,int lastTime){
	if (lastTime <= 0){
		return 0;
	}
	EffectDetail effectDetail;
	effectDetail.effectType = type;
	effectDetail.useArmyId = GetArmyID();
	if (armyPtr)
	{
		effectDetail.receiveArmyId = armyPtr->GetArmyID();
	}
	else
	{
		effectDetail.receiveArmyId  = 0;
	}
	effectDetail.startBout_ = fightManager_->getCurBoutNum();
	effectDetail.endBout_ = effectDetail.startBout_ + lastTime;
	unsigned int effectId = fightManager_->useEffect(effectDetail);
	effectRecord_[type] = effectId;
	return effectId;
}
void Army::clearEffect(void){
	map<ENUM_SPECIALLY_EFFECT , unsigned int>::iterator iter;
	for (iter = effectRecord_.begin() ; iter != effectRecord_.end();++iter){
		fightManager_->clearEffect(iter->second);
	}
	effectRecord_.clear();
}
void Army::clearEffectByType(ENUM_SPECIALLY_EFFECT type , bool isClearEffect){
	map<ENUM_SPECIALLY_EFFECT , unsigned int>::iterator iter = effectRecord_.find(type);
	if (iter == effectRecord_.end()){
		return;
	}
	IArmyPtr armyPtr = GetLastAttackTarget();
	if (armyPtr != NULL && armyPtr->getBuildDetailType() > ARCH_OCCUPY_BUILDING_START && armyPtr->getBuildDetailType() < ARCH_OCCUPY_BUILDING_END){
		armyPtr->SetLastEnmityTarget(0);
	}
	if (isClearEffect)
	{
		fightManager_->clearEffect(iter->second);
	}
	
	effectRecord_.erase(iter);
}
//使用卡片
IArmy::CardUsedState Army::getCardState(CardType cardType){
	map<CardType , PropCard*>::iterator iter = cardRecord_.find(cardType);
	if(iter == cardRecord_.end()){
		return NO_USED_CARD;
	}
	if (iter->second->isForceOff()){
		return FORCE_OFF_CARD;
	}
	return IS_USING_CARD;
}
//使用卡片
void Army::useCard(PropCardConfig * propCardPtr){
	if (propCardPtr == NULL){
		return ;
	}
	if (propCardPtr->lastTime <= 0){
		return ;
	}
	PropCard * tmpPtr = fightManager_->getNewCard(this , propCardPtr);
	if (tmpPtr == NULL){
		return ;
	}
	tmpPtr->useCard();
	map<CardType , PropCard *>::iterator iter = cardRecord_.find(static_cast<CardType>(propCardPtr->cardType));
	if (iter != cardRecord_.end()){
		iter->second->clearCard();
		delete iter->second;
		iter->second = NULL;
	}
	cardRecord_[static_cast<CardType>(propCardPtr->cardType)] = tmpPtr;

}
//清除某种类型卡
void Army::clearCard(CardType cardType){
	map<CardType , PropCard *>::iterator iter = cardRecord_.find(cardType);
	if (iter == cardRecord_.end()){
		return ;
	}
	iter->second->clearCard();
	delete iter->second;
	iter->second = NULL;
	cardRecord_.erase(iter);
}
void Army::tidyCard(void){
	map<CardType , PropCard *>::iterator iter;
	for (iter = cardRecord_.begin(); iter != cardRecord_.end() ;){
		if (iter->second->isForceOff())
		{
			iter->second->clearCard();
			delete iter->second;
			iter->second = NULL;
			iter = cardRecord_.erase(iter);
		}
		else{
			++ iter;
		}
	}
}
//是否可以使用武器
bool Army::isCanUseWeapon(void) const{
	return isNoLockAction() && ((fightManager_->getCurBoutNum() - lastAttackBoutNum_) >=  attackFreq_);
}
bool Army::isCanMove(void) const{
	return isNoLockAction() && ((fightManager_->getCurBoutNum() - lastMoveBoutNum_) >=  moveLockBout_);
}
bool Army::isNoLockAction(void) const{
	return ((fightManager_->getCurBoutNum() - lastAttackBoutNum_) >= attackLockBout_) && 
		((fightManager_->getCurBoutNum() - lastMoveBoutNum_) >= moveLockBout_);
}
//部队移动
bool Army::Move(vector<POS_STRUCT>& /*moveTrack*/ , IArmyPtr /*armyPtr*/){
	lastMoveBoutNum_ = fightManager_->getCurBoutNum();
	isActioned_ = true;
	return true;
}
void Army::calculateSpeed(void){
	if (movePerNeedTime_ == 0){
		moveSpeed_ = 0;
		moveLockBout_ = 10000000;
		return;
	}
	unsigned int tmp = movePerNeedTime_;
	if (tmp > HIGH_LIMIT_SPEED_TIME){
		tmp = HIGH_LIMIT_SPEED_TIME;
	}
	if (tmp < LOW_LIMIT_SPEED_TIME){
		tmp = LOW_LIMIT_SPEED_TIME;
	}
	moveSpeed_ = FIGHT_TIMER_CYCLE / tmp;
	if (moveSpeed_ <= 0){
		moveSpeed_ = 1;
	}
	moveLockBout_ = tmp / FIGHT_TIMER_CYCLE;
}
//调用部队处理
bool Army::processAction(bool processMode){
	isActioned_ = false;
	list<ArmAttack*>::iterator iter;
	for(iter = attackActions_.begin(); iter != attackActions_.end(); ){
		ArmAttack * attacker = *iter;
		if(attacker->action()){	
			delete attacker;
			attacker = NULL;
			iter = attackActions_.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	
	if (processMode)
	{
		if (!isCanUseWeapon()){
			return isActioned_;
		}
	}
	else{
		if (!isCanMove()){
			return isActioned_;
		}
	}
	action_->thinking(processMode);
	tidyCard();
	return isActioned_;
}