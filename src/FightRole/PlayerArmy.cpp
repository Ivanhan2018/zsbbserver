#include "PlayerArmy.h"
#pragma warning(push)
#pragma warning(disable:4511)
#pragma warning(disable:4512)
#include "boost/serialization/singleton.hpp"
#pragma warning(pop)
#include "DBTableHeader.h"
#include "FightSystemMacro.h"
#include "FightManager.h"
#include "computinghurt/randomgenerator.h"
#include "CommInterface.h"
#include "log.hpp"
#include "ISystemLog.h"
typedef boost::serialization::singleton<computinghurt::RandomGenerator> RandomSingleton;
PlayerArmy::PlayerArmy(void):
isMercenary_(false)
{
}

PlayerArmy::~PlayerArmy(void)
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
	map<CardType , PropCard *>::iterator cardIter;
	for (cardIter = cardRecord_.begin(); cardIter != cardRecord_.end() ;++ cardIter){
		cardIter->second->clearCard();
		delete cardIter->second;
		cardIter->second = NULL;
	}
}
bool PlayerArmy::Initialize(PlayerArmyParam & armyParam,FightManager * fightManager , ENUM_USE_SCENE state){
	if (fightManager == NULL){
		return false;
	}
	fightManager_ = fightManager;
	memcpy(&armyTable_ , &armyParam.armyTable , sizeof(ArmyTable));
	memcpy(&playerOfficerTable_ , &armyParam.playerOfficerTable, sizeof(PlayerOfficerTable));
	armyWeapon_ = armyParam.weaponAttribute;
	armyDefend_ = armyParam.defendAttribute;
	soldierParam_ = armyParam.soldierParam;
	officerRace_  = armyParam.officerRaceAttribute;
	if (armyWeapon_ == NULL || officerRace_ == NULL || soldierParam_ == NULL){
		return false;
	}
	attackFreq_ = armyWeapon_->attackFreq;
	attackType_ = (AttackTypeEnu)armyWeapon_->attackType;
	attackLockBout_ = armyWeapon_->actionBout;
	trajectorySpeed_ = armyWeapon_->trajectorySpeed;
	POS_STRUCT tmpPos ;
	tmpPos.PosX = armyTable_.x;
	tmpPos.PosY = armyTable_.y;
	tmpPos.weight = 0;
	armyPos_.resize(1 , tmpPos);
	armyBirthPos_ = tmpPos;
	armyType_ = PLAYER_ARMY;
	countryID_ = armyTable_.countryid_; 
	if(createComputeHurt(armyType_) == false){
		return false;
	}
	if(createArmyAction(armyParam.mapData , state , NULL) == false){
		return false;
	}
	
	action_->setStrategy(AttackAI);
	if (isPlayerArmy())
	{
		isTrusteeship_ = false;
	}
	else
	{
		isTrusteeship_ = true;
	}
	followTarget_ = NULL;
	lastAttackTarget_ = 0;
	lastEnmityTarget_ = 0;
	playerName_ = fightManager_->getPlayerName(armyTable_.playerid_ ,armyTable_.playerid_);
	return true;
}

bool PlayerArmy::Move(vector<POS_STRUCT>& moveTrack , IArmyPtr armyPtr){
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
		armyTable_.direction = changeDirection(armyPos , moveTrack[i]);
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
	clearEffectByType(EFFECT_OCCUPY ,true);
	Army::Move(moveTrack , armyPtr);
	return true;
}

bool PlayerArmy::createComputeHurt(ARMY_TYPE armyType){
	computinghurt::IRandomGenerator* randomgenerator = &RandomSingleton::get_mutable_instance();
	computinghurt::ArmyInfo armyInfo = {0};
	if(armyType != NPC_ARMY){
		
		armyInfo.heroInfo.heroAttribute.agile =  GetOfficerCelerity();
		armyInfo.heroInfo.heroAttribute.endurance = GetOfficerStamina();
		armyInfo.heroInfo.heroAttribute.intelligence = GetOfficerIntellect();
		armyInfo.heroInfo.heroAttribute.power = GetOfficerPower();
		armyInfo.heroInfo.heroAttribute.physicAttack = playerOfficerTable_.attack_;
		armyInfo.heroInfo.heroAttribute.physicDef = playerOfficerTable_.physicrecovery_;
		armyInfo.heroInfo.heroAttribute.magicDef = playerOfficerTable_.magicrecovery_;
		armyInfo.heroInfo.heroAttribute.magicAttack = playerOfficerTable_.magicattack_;
		armyInfo.heroInfo.heroAttribute.HP = playerOfficerTable_.herohp_;
		armyInfo.heroInfo.heroAttribute.selfBeinjuredRate = playerOfficerTable_.injurerate_;
		armyInfo.heroInfo.heroAttribute.selfCapturedRate = playerOfficerTable_.capturerate_;
		armyInfo.heroInfo.heroAttribute.enemyBeinjuredRate = playerOfficerTable_.beinjurerate_;
		armyInfo.heroInfo.heroAttribute.ememyCapturedRate = playerOfficerTable_.becapturerate_;
		armyInfo.heroInfo.heroAttribute.cruelAttackRate = playerOfficerTable_.forcerate_;

		PlayerSkillTable playerSkill;
		PlayerSkillTable * playerSkillPtr = NULL;
		int skillNum = 0;
		playerSkill.setmasterid(getPlayerID() , GetOfficerID());
		DB_Proxy *proxy = fightManager_->getCurDBGProxy();
		if(proxy->db_multi_select(playerSkill , (char *&)playerSkillPtr , skillNum) == DB_Proxy::DB_FAILED){
			LOG_ERROR(("获得玩家%d英雄%d技能信息失败" , getPlayerID() , GetOfficerID()));
		}
		skillNum /= sizeof(PlayerSkillTable);
		computinghurt::SkillInfo officerSkill;
		for (int i = 0; i < skillNum ; i ++){
			if(!fightManager_->getSkillInfo(officerSkill , static_cast<SKILL_TYPE>(playerSkillPtr[i].skillid_),playerSkillPtr[i].skilllevel_)){
				continue;
			}
			armyInfo.heroInfo.skillsinfo.push_back(officerSkill);
		}
		if(playerSkillPtr){
			delete [] playerSkillPtr;
			playerSkillPtr = NULL;
		}
	}
	switch((OFFICER_OCCUPATION_TYPE)playerOfficerTable_.jobid_){
	case WARRIOR_OFFICER:
		armyInfo.heroInfo.type = computinghurt::HEROTYPE_POWER;
		break;
	case SHOOTER_OFFICER:
		armyInfo.heroInfo.type = computinghurt::HEROTYPE_AGILE;
		break;
	default:
		armyInfo.heroInfo.type = computinghurt::HEROTYPE_INTELLIGENCE;
	}
	
	armyInfo.soldierInfo.hp = (soldierParam_->hp_ + officerRace_->hp_); 
	armyInfo.soldierInfo.shootingRate = soldierParam_->shooting_ ;
	armyInfo.soldierInfo.numberOfSoldiers = armyTable_.firstcount_ ;
	if (armyDefend_){
		armyInfo.soldierInfo.magicDefense = armyDefend_->magicRec + soldierParam_->magicevade_;
		armyInfo.soldierInfo.physicDefense = armyDefend_->physiceRec + soldierParam_->physicevade_;

		//初始化部队速度
		movePerNeedTime_ = soldierParam_->movefrequency_ + armyDefend_->decRate;
	}
	else{
		armyInfo.soldierInfo.magicDefense = soldierParam_->magicevade_;
		armyInfo.soldierInfo.physicDefense = soldierParam_->physicevade_;

		//初始化部队速度
		movePerNeedTime_ = soldierParam_->movefrequency_;
	}
	calculateSpeed();
	map<int , int>::iterator iter = armyWeapon_->race.find(armyTable_.raceid_);
	if (iter == armyWeapon_->race.end())
	{
		armyInfo.soldierInfo.power = armyWeapon_->attack;
	}
	else
	{
		armyInfo.soldierInfo.power = (armyWeapon_->attack * (100+armyWeapon_->addAttack)) / 100;
	}
	armyInfo.soldierInfo.type = (SOLDIER_TYPE) soldierParam_->soldiertype_;
	armyInfo.soldierInfo.weaponId = armyWeapon_->weaponID;
	if (computehurt_){
		delete computehurt_;
		computehurt_ = NULL;
	}
	computehurt_ = computinghurt::HurtFactory::createArmyHurt(armyInfo, fightManager_->getLandFormMap(), this , randomgenerator);
	if (computehurt_ == NULL){
		return false;
	}
	return true;
}

float PlayerArmy::getAttackFactor() const{ 
	if(fightManager_->isInWater(this) == true){
		return 0.25;
	}
	if (fightManager_->isInForest(this) == true){
		return 0.5;
	}
	return 1.0;
}
void PlayerArmy::onKilled(IArmy* army){
	if (computehurt_ == NULL){
		return;
	}
	if(computehurt_->canCatchHero(army)){
		// 捕获英雄
		//副本中暂时不会触发英雄抓捕技能
		//判断是否有逃脱卡
		PlayerItemStorageTable storageTable;
		PlayerItemStorageTable * storageTablePtr = NULL;
		int storageNum = 0;
		storageTable.setmasterid(army->GetOfficerID(),army->getPlayerID());
		DB_Proxy * proxy = fightManager_->getCurDBGProxy();
		if (proxy == NULL || (proxy->db_multi_select(storageTable,(char *&)storageTablePtr
			,storageNum) !=DB_Proxy::DB_SUCCEED)){
			army->armyOnDie(this , true ,false);
			return ;
		}
		storageNum /= sizeof(PlayerItemStorageTable);
		for (int i = 0; i < storageNum; i++){
			if (storageTablePtr[i].itemid_ == 1032)
			{
				UseEscapeCard msgUseCard;
				msgUseCard.serialize(sizeof(UseEscapeCard),0,fightManager_->getMapID(),0,RSP_USE_ECAPE_CARD_INFO);
				string playerName = army->getPlayerName();
				if (playerName.length() > SIZE_24 - 1){
					memcpy(msgUseCard.czMainHeroName,playerName.c_str(), SIZE_24 -1);
				}
				else{
					memcpy(msgUseCard.czMainHeroName,playerName.c_str(), playerName.length());
				}
				msgUseCard.czMainHeroName[SIZE_24-1]='\0';
				msgUseCard.iTime = IceUtil::Time::now().toSeconds();
				msgUseCard.MapID = fightManager_->getMapID();
				memcpy(msgUseCard.czHeroName,army->GetArmyName(),SIZE_24-1);
				msgUseCard.czHeroName[SIZE_24-1]='\0';
				broadcast((char*)&msgUseCard,msgUseCard.length);
				if(proxy->db_delete(storageTablePtr[i]) != DB_Proxy::DB_SUCCEED){
					LOG_ERROR(("删除玩家%d带队英雄%d逃跑卡操作失败！" , army->getPlayerID() , army->GetOfficerID()));
				}
				army->armyOnDie(this , false , false);
				if (storageTablePtr){
					delete [] storageTablePtr;
					storageTablePtr = NULL;
				}
				return ;
			}
		}
		army->armyOnDie(this , true ,false);
		if (storageTablePtr){
			delete [] storageTablePtr;
			storageTablePtr = NULL;
		}
	}
	else{
		army->armyOnDie(this , false , true);
	}
	
}
bool PlayerArmy::armyOnDie(IArmyPtr attackArmyPtr , bool isCatched , bool isProcessHurt){
	if (attackArmyPtr == NULL){
		return false;
	}
	int realState = 0;
	if (isPlayerArmy() == true){
		int officerState = -1; 
		if (isCatched){
			fightManager_->onCatchedHero(attackArmyPtr , GetOfficerID());
			fightManager_->onBeCatchedHero(this , GetOfficerID());
			officerState = 2;
			realState = 3;
			fightManager_->onCaptiveHero(attackArmyPtr , this);
			ISystemLog::getInstance()->heroState_Captured_Log(getPlayerID() , GetOfficerID() ,
				getHeroName().c_str(), attackArmyPtr->getPlayerID() ,attackArmyPtr->GetOfficerID());
		}
		if(!isCatched && isProcessHurt && attackArmyPtr->canPlasterHero(this)){ 
			fightManager_->onHeroWasInjured(attackArmyPtr , GetOfficerID());
			officerState = 1;
			realState = 2;
			tagHeroParam tmpOfficerParam;
			DB_Proxy * proxy = fightManager_->getCurDBGProxy();
			memset(&tmpOfficerParam , 0 , sizeof(tagHeroParam));
			tmpOfficerParam.iState = 2;
			tmpOfficerParam.lPlayerID = getPlayerID();
			tmpOfficerParam.lHeroID = GetOfficerID();
			OnHeroGrowUp(proxy ,tmpOfficerParam,KILLNPC_METHOD);
			ISystemLog::getInstance()->heroState_Injured_Log(getPlayerID() , GetOfficerID() ,
				getHeroName().c_str() ,attackArmyPtr->getPlayerID() ,attackArmyPtr->GetOfficerID());
		}
		if (realState != 0){
			fightManager_->processPlayerDrop(attackArmyPtr , this);
		}
		//处理运输马车的死亡
		if (TRANSPROT == GetArmyOccupation())
		{
			LoadMaterial goodInfo;
			LoadMaterial * goodsPtr = NULL;
			int goodNum = 0;
			goodInfo.setmasterid(getPlayerID() , GetArmyID());
			DB_Proxy * proxy = fightManager_->getCurDBGProxy();
			if(proxy->db_multi_select(goodInfo , (char *&)goodsPtr , goodNum) != DB_Proxy::DB_SUCCEED){
				LOG_ERROR(("查询运输马车装备出错，玩家%d部队%d" , getPlayerID() , GetArmyID()));
			}
			goodNum /= sizeof(LoadMaterial);
			for (int i = 0; i < goodNum ; i++){
				if(proxy->db_delete(goodsPtr[i]) != DB_Proxy::DB_SUCCEED){
					LOG_ERROR(("删除运输马车装备出错，玩家%d部队%d" , getPlayerID() , GetArmyID()));
				}
			}
			if (goodsPtr)
			{
				delete [] goodsPtr;
				goodsPtr = NULL;
			}
		}
		fightManager_->notifyModifyArmyState(getPlayerID() , GetArmyID() , getFromMapID() ,0,ARMY_ANNIHILATE , GetWeaponID());
		fightManager_->processPlayerArmyDie(this , officerState , realState);
		
		fightManager_->onArmyDie(attackArmyPtr,this);
		return true;
	}
	fightManager_->notifyModifyArmyState(getPlayerID() , GetArmyID() , getFromMapID() ,0,ARMY_ANNIHILATE , GetWeaponID());
	fightManager_->processPlayerArmyDie(this , -1 , realState);
	fightManager_->onArmyDie(attackArmyPtr, this);
	return true;
}
//获得跟随目标
IArmyPtr PlayerArmy::GetFollowTarget() const {
	return fightManager_->getArmyByID(followTarget_);
}
bool PlayerArmy::refreshExp(void){
	if (isPlayerArmy() == false){
		return false;
	}
	statisticssystem::FightingStatisticsAdapter * fightRecord = fightManager_->getFightRecord();
	DB_Proxy * dabaseProxy = fightManager_->getCurDBGProxy();
	if (fightRecord == NULL || dabaseProxy == NULL){
		return false;
	}
	fightRecord->onArmyFlashExp(this);
	tagHeroParam tmpOfficerParam;
	fightRecord->queryArmyInfo(this ,tmpOfficerParam);
	int exp = exp_;
	exp_ = tmpOfficerParam.iExp;
	tmpOfficerParam.iExp -= exp;
	OnHeroGrowUp(dabaseProxy ,tmpOfficerParam,KILLNPC_METHOD);
	return true;
}
bool PlayerArmy::officerGrowUp(){
	if (isPlayerArmy() == false){
		return false;
	}
	statisticssystem::FightingStatisticsAdapter * fightRecord = fightManager_->getFightRecord();
	DB_Proxy * dabaseProxy = fightManager_->getCurDBGProxy();
	if (fightRecord == NULL || dabaseProxy == NULL){
		return false;
	}
	fightRecord->onArmyFlashExp(this);
	tagHeroParam tmpOfficerParam;
	fightRecord->queryArmyInfo(this ,tmpOfficerParam);
	int inExp = tmpOfficerParam.iExp + getOfficerExp() - exp_;
	int outExp = 0;
	int level = GetLevel(GetOfficerLevel(),inExp,outExp);
	if(level > GetOfficerLevel()){
		DB_Proxy * proxy = fightManager_->getCurDBGProxy();
		if (proxy == NULL){
			return false;
		}
		int exp = exp_;
		exp_ = tmpOfficerParam.iExp;
		tmpOfficerParam.iExp -= exp;
		OnHeroGrowUp(proxy ,tmpOfficerParam,KILLNPC_METHOD);
		PlayerOfficerTable tmpPlayerInfo;
		tmpPlayerInfo.setmasterid(getPlayerID());
		tmpPlayerInfo.setdetailid(GetOfficerID());
		if(proxy->db_select(tmpPlayerInfo) == DB_Proxy::DB_SUCCEED){
			computinghurt::HeroAttribute heroAttribute;
			heroAttribute.agile = playerOfficerTable_.celerity_ + playerOfficerTable_.levelcelerity_ + playerOfficerTable_.equipcelerity_;
			heroAttribute.endurance = playerOfficerTable_.stamina_ + playerOfficerTable_.levelstamina_ + playerOfficerTable_.equipstamina_;
			heroAttribute.intelligence = playerOfficerTable_.wit_ + playerOfficerTable_.levelwit_ + playerOfficerTable_.equipwit_;
			heroAttribute.power = playerOfficerTable_.power_ + playerOfficerTable_.levelpower_ + playerOfficerTable_.equippower_;
			heroAttribute.physicAttack = playerOfficerTable_.attack_;
			heroAttribute.physicDef = playerOfficerTable_.physicrecovery_;
			heroAttribute.magicDef = playerOfficerTable_.magicrecovery_;
			heroAttribute.magicAttack = playerOfficerTable_.magicattack_;
			heroAttribute.HP = playerOfficerTable_.herohp_;
			heroAttribute.selfBeinjuredRate = playerOfficerTable_.injurerate_;
			heroAttribute.selfCapturedRate = playerOfficerTable_.capturerate_;
			heroAttribute.enemyBeinjuredRate = playerOfficerTable_.beinjurerate_;
			heroAttribute.ememyCapturedRate = playerOfficerTable_.becapturerate_;
			heroAttribute.cruelAttackRate = playerOfficerTable_.forcerate_;
			upgradeLevel(heroAttribute , tmpPlayerInfo);
			return true;
		}
		else{
			LOG_ERROR(("读取部队%d带队英雄%d信息错误！" ,getPlayerID() , GetOfficerID()));
		}
	}
	return false;
}

//@brief 属性升级
void PlayerArmy::upgradeLevel(const computinghurt::HeroAttribute& heroAttribute , PlayerOfficerTable & playerInfo){
	computinghurt::IArmyHurt * computeArmyHurt = dynamic_cast<computinghurt::IArmyHurt*>(computehurt_);
	if (computeArmyHurt == NULL){
		return ;
	}
	computeArmyHurt->upgradeLevel(heroAttribute);
	if (playerInfo.level_ == playerOfficerTable_.level_)
	{
		return ;
	}
	memcpy(&playerOfficerTable_ , &playerInfo, sizeof(PlayerOfficerTable));
	RspUpgradeLevel rspfreshLevel;
	size_t len = getPlayerName().length();
	if (len > SIZE_24-1){
		len = SIZE_24-1;
	}
	rspfreshLevel.iLevel = static_cast<unsigned short>(GetOfficerLevel());
	memcpy(rspfreshLevel.PlayerName , getPlayerName().c_str(),len);
	rspfreshLevel.PlayerName[len]='\0';
	if (GetOfficerName()){
		memcpy(rspfreshLevel.HeroName ,GetOfficerName(),SIZE_24-1);
		rspfreshLevel.HeroName[SIZE_24-1]='\0';
	}
	rspfreshLevel.ulArmyID = GetArmyID();
	rspfreshLevel.serialize(0, fightManager_->getProcessID(),0);
	fightManager_->msgBroadcast(&rspfreshLevel, RSP_HERO_UPGRADE_LEVEL ,rspfreshLevel.length);
}
//@brief 学习了新的技能或技能升级
void PlayerArmy::upgradeOrLearnedSkill(const computinghurt::SkillInfo& skillInfo){
	computinghurt::IArmyHurt * computeArmyHurt = dynamic_cast<computinghurt::IArmyHurt*>(computehurt_);
	if (computeArmyHurt == NULL){
		return ;
	}
	computeArmyHurt->upgradeOrLearnedSkill(skillInfo);
}
bool PlayerArmy::abandonSkill(const computinghurt::SkillAndGradeStru& skillAndGradeStru){
	computinghurt::IArmyHurt * computeArmyHurt = dynamic_cast<computinghurt::IArmyHurt*>(computehurt_);
	if (computeArmyHurt == NULL){
		return false;
	}
	return computeArmyHurt->abandonSkill(skillAndGradeStru);
}
//获得部队军牌掉落系数
int PlayerArmy::getArmyCardDropOdds(void) const{
	int heroLevel = GetOfficerLevel() ? GetOfficerLevel() : 1;
	int defendLevel = armyDefend_ ? armyDefend_->level : 1;
	defendLevel = defendLevel ? defendLevel : 1;
	int weaponLevel = armyWeapon_ ? armyWeapon_->level : 1;
	weaponLevel = weaponLevel ? weaponLevel : 1;
	return heroLevel * defendLevel * weaponLevel;
}