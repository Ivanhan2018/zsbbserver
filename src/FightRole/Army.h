#pragma once
#include <iostream>
#include "IArmy.h"
#include "ArmyAction.h"
#include "hurtfactory.h"
#include "ihurt.h"
#include "ServerProtocolHeader.h"
#include "NPCInfo.h"
#include "ZSBB_DataDefine.h"
//���Ӳ����� 
struct PlayerArmyParam{
	WeaponAttribute *weaponAttribute; //�������Ա�
	DefendAttribute *defendAttribute; //�������Ա�
	SoldierParamTable * soldierParam ;//ʿ��������
	OfficerRaceAttribute *officerRaceAttribute; //Ӣ�������
	ArmyTable armyTable; 
	PlayerOfficerTable playerOfficerTable; //���Ӵ���Ӣ�۱�
	MapDataStru mapData; //��ͼ��Ϣ �� ����ɳ����Ϣ
};

//NPC���Ӳ����� 
struct NpcArmyParam{
	EctypeNpcArmyTable* npcArmyTable;
	EctypeNpcTypeTable* npcArmyInfo;
	MapDataStru mapData; //��ͼ��Ϣ �� ����ɳ����Ϣ
};
//NPC���Ӳ����� 
struct AttachedNpcArmyParam{
	NpcTemplate* npcArmyTable;
	EctypeNpcTypeTable* npcArmyInfo;
	MapDataStru mapData; //��ͼ��Ϣ �� ����ɳ����Ϣ
	unsigned int adscriptionArmyID; //�����ߵĲ���ID
};

//��ɧ��NPC����
struct GuarderArmyParam{
	NpcGuarderParam* npcArmyTable; 
	EctypeNpcTypeTable* npcArmyInfo;
	MapDataStru mapData; //��ͼ��Ϣ �� ����ɳ����Ϣ
};

//�������Ӳ����� 
struct BuildArmyParam{
	PublicBuildParamTable buildParam;//������Ĳ��� 
	DefendAttribute *defendAttribute; //�������Ա�
	unsigned int armyID; //����ID
	vector<POS_STRUCT> armyPos; //����λ��
	MapDataStru mapData; //��ͼ��Ϣ �� ����ɳ����Ϣ
};
enum AttackTypeEnu
{
	Unknown_Type = -1,
	Attack_Place = 0,          // 0���ص�
	Attack_Follow,             // 1������
	Attack_Instantaneous,      // 2��˲ʱ
};

#define LOW_LIMIT_SPEED_TIME 500
#define HIGH_LIMIT_SPEED_TIME 4000
class FightManager;
class PropCard;
struct EffectDetail;
class ArmAttack;
class Army : public IArmy
{
public:
	Army(void);
public:
	virtual ~Army(void);
public:
	//����
	void Attack(unsigned int armyId, unsigned int numCircle=0);
	void Attack(Armys &armyPtrs);
	//��ò��ӹ�����
	inline  int GetAttackPower() const{
		return computehurt_->GetAttackPower();
	}
	int GetDefendPower(ATTACK_TYPE attackType) const{
		return computehurt_->getDefendPower(attackType);
	}
	//��÷���ID
	virtual int GetDefendID() const{
		return 0;
	}
	//��÷���Type
	virtual int GetDefendType() const{
		return 0;
	}
	//��ò��ӵĹ���
	virtual  unsigned int getPlayerID() const{
		return 0;
	}
	//��ò���Ӣ��ID
	virtual  unsigned int GetOfficerID() const{
		return 0;
	}
	
	//��õ��侭��
	virtual  unsigned int GetLostExp(int num) const{
		return 0*num;
	}

	//��ø���Ŀ��
	IArmyPtr GetFollowTarget() const{
		return NULL;
	}

	//���ø���Ŀ��
	virtual void SetFollowTarget(IArmyPtr /*army*/){
		return;
	}

	//�ж��Ƿ��Ѿ�
	virtual bool IsFriendsArmy(IArmyPtr army){
		if(GetCountryID() == army->GetCountryID()){
			return true;
		}
		return false;
	}

	//�����ƶ�
	virtual bool Move(vector<POS_STRUCT>& /*moveTrack*/ , IArmyPtr /*armyPtr*/);

	//���Ӣ�۵�ͷ��ID
	virtual int GetFaceID() const {
		return 0;
	}

	//���ʿ������
	virtual RACE_TYPE GetSoldierRace() const {
		return (RACE_TYPE)0;
	}

	//��ò���Ӣ����Ϣ 
	virtual const char * GetOfficerName() const {
		return NULL;
	}

	//���Ӣ������
	virtual int GetOfficerPower() const {
		return 0;
	}

	//���Ӣ������
	virtual int GetOfficerStamina() const {
		return 0;
	}

	//���Ӣ������
	virtual int GetOfficerCelerity() const {
		return 0;
	}

	//���Ӣ������
	virtual int GetOfficerIntellect() const{
		return 0;
	}

	//���Ӣ�ۼ���
	virtual int GetOfficerLevel() const {
		return 0;
	}

	//���ʿ������
	virtual int getSoldierNum() const {
		return 1;
	}

	//����ְҵ
	virtual SOLDIER_TYPE GetArmyOccupation()const {
		return (SOLDIER_TYPE) 0;
	}
	//��ù�������
	virtual ATTACK_TYPE GetAttackType()const {
		return (ATTACK_TYPE)0;
	}	

	//�����Ҳ��ӵ�����
	virtual const char * GetArmyName() const {
		return NULL;
	}

	//��ȡ���ӵ�λ��
	inline const std::vector<POS_STRUCT>& GetArmyPos() const {
		return armyPos_;
	}

	//��ò�������
	inline ARMY_TYPE GetArmyType()const  {
		return armyType_;
	}

	//������һ���Լ�������Ŀ��
	inline IArmyPtr GetLastAttackTarget(){
		return getArmyByID(lastAttackTarget_);
	}

	///������һ�ι����Լ���Ŀ��
	inline IArmyPtr GetLastEnmityTarget(){
		return getArmyByID(lastEnmityTarget_);
	}
	//������һ���Լ�������Ŀ��
	virtual  void SetLastAttackTarget(unsigned int lastAttackTarget){  
		lastAttackTarget_ = lastAttackTarget;
	}
	//������һ�ι����Լ���Ŀ��
	virtual  void SetLastEnmityTarget(unsigned int lastEnmityTarget){
		lastEnmityTarget_ = lastEnmityTarget;
	}
	//����Լ�����ϵͳʱλ����Ϣ
	inline POS_STRUCT GetBirthPos()const {
		return armyBirthPos_;
	}

	//��ù���ID 
	inline int GetCountryID()const {
		return countryID_;
	}

	//����й�״̬
	inline  bool GetTrusteeshipState()const{
		return isTrusteeship_;
	}

	FORWORD changeDirection(POS_STRUCT& curPos,POS_STRUCT& nextPos);
	virtual FORWORD getDirection() const {
		return (FORWORD) 0;
	}
	//��ò���״̬
	virtual int GetArmyState() const {
		return 0;
	}
	//����������Ϣ
	virtual bool exportArmyInfo(ARMY_POS & armyInfo , int mapID);
	//��ò����ƶ��ٶ�
	virtual int getMoveSpeed() const {
		return moveSpeed_;
	}
	//���ò��Ӵ���
	virtual bool processAction(bool processMode);
	///����AI����ģ��
	bool createArmyAction(MapDataStru& mapData , ENUM_USE_SCENE sceneState , AiLevelParam *aiLeveParam);
	virtual bool processMoveMsg(POS_STRUCT & /*movePos*/){
		return false;
	}
	virtual bool processAttackTarget(const IArmyPtr /*attackTargetPtr*/){
		return false;
	}
	//�������ò��ӵ�λ��
	virtual void setArmyPos(POS_STRUCT /*armyPos*/) {
		return;
	}
	virtual void getArmyTable(ArmyTable *& armyTable){
		armyTable = NULL;
	}

	// add by zwl
	virtual HarmInfo beAttacked(const computinghurt::NothingingDefendStru &nothingDefStru, IArmy* attacker, int power){
		if(computehurt_ == 0)
			return HarmInfo();

		return computehurt_->beAttacked(nothingDefStru, attacker, power);		
	}
	//���Ӣ������
	virtual const computinghurt::HeroAttribute * getHeroAttribute() const{
		return computehurt_->getHeroAttribute();
	}
	// ֪ͨ�ͷ��˼���
	virtual void notifyRelease(SKILL_TYPE /*id*/){
	}

	//��ý��������ϸ����
	virtual ARCH_TYPES getBuildDetailType() const {
		return ARCH_INVALID;
	}
	//�ǲ������
	virtual bool isPlayerArmy() const {
		if (getPlayerID() >= 100000){
			return true;
		}
		return false;
	}
	virtual ENUM_ARMY_STATUS getArmyActionStatus() const {
		return action_->getArmyStatusOnTrusteeship();
	}
	//��õ�ǰ������Ӱ��ϵ��
	virtual float getAttackFactor() const{ 
		return 1.0;
	}
	//��õ�ǰ��������Ӱ��ϵ��
	virtual float getDefendFactor() const {
		return 1.0;
	}
	//���Ӣ�����ӵľ���ֵ
	virtual int getHeroIncreaseExp() const{
		return computehurt_->getHeroIncreaseExp();
	}
	//���ˢ�²��� 
	virtual bool getOfficerParam(tagHeroParam & /*officerParam*/) {
		
		return false;
	}
	//���FROM��ͼ��ID
	virtual int  getFromMapID() const{
		return 0;
	}
	//��ò��Թ�����
	virtual  IManager * getStrategyMgr() const {
		return action_->getStrategyMgr();
	}
	//��õ�ǰѪֵ
	inline   int GetCurrentHp() const {
		if(computehurt_ == NULL){
			return 0;
		}
		return computehurt_->getCurrentHp();
	}
	//��õ�ǰ��Ѫ
	virtual int getCurHurtHp() const {
		if(computehurt_ == NULL){
			return 0;
		}
		return computehurt_->getCurHurtHp();
	}
	//��ó�ʼHP
	virtual int getInitHp(void) const {
		return computehurt_->getTotalHp();
	}
	//�����й�״̬
	virtual void setTrusteeshipState(bool trusteeshipState) {
		isTrusteeship_ = trusteeshipState;
	}
	//�ͷ��ڴ�
	inline void release(); 
	//ͨ������ID�������������Ϣ
	virtual IArmyPtr getArmyByID(unsigned int armyID) ;


	//�����Чλ��
	virtual POS_STRUCT getValidPos(POS_STRUCT & pos ,unsigned int limitNum = 3 , unsigned int  distanceVal = 1){
		POS_STRUCT tmpPos = {0}; 
		PosXYStru tmpXYPos;
		tmpXYPos.xPos = pos.PosX;
		tmpXYPos.yPos = pos.PosY;
		if(action_->getAdaptivePos(tmpXYPos , tmpXYPos , limitNum , distanceVal) == true){
			tmpPos.PosX = tmpXYPos.xPos;
			tmpPos.PosY = tmpXYPos.yPos;
		}
		return tmpPos;
	}
	void setBirthPos(POS_STRUCT & birthPos){
		armyBirthPos_ = birthPos;
	}
	virtual const string & getPlayerName() const{
		return playerName_;
	}
	bool isNpcArmy() const {
		return false;
	}
	//ˢ�²��Ӿ���
	virtual bool refreshExp(void){
		return false;
	}
	//Ӣ������
	virtual bool officerGrowUp(){
		return false;
	}
	//��ô���Ӣ��ԭ�о���
	virtual int getOfficerExp(void){
		return 0;
	}
	// ���ݹ�����(�Լ�)�������뱻�����ߵķ������õ�һ������
	virtual float getFactorAccordingToArmor(IArmy* beAttacker); 
	//���貿������
	virtual unsigned int resetSoldierNum(int soldierNum){
		return computehurt_->resetSoldierNum(soldierNum);
	}
	// ���Ӳ�������
	virtual unsigned int addSoldierNum(unsigned int soldierNum){
		return computehurt_->addSoldierNum(soldierNum);
	}
	// ���Ӳ����ƶ��ٶ�
	virtual void addMoveSpeed(int increaceSpeed) {
		moveSpeed_ += increaceSpeed;

		if (moveSpeed_ <= 0)
		{
			calculateSpeed();
		}
	}
	virtual void addMoveTime(int increaseTime){
		movePerNeedTime_ += increaseTime;
		calculateSpeed();
	}
	//���ò���λ��
	virtual POS_STRUCT setGatewayArmyPos(POS_STRUCT armyPos){
		PosXYStru tmpPos;
		tmpPos.xPos = armyPos.PosX;
		tmpPos.yPos = armyPos.PosY;
		action_->getAdaptivePos(tmpPos , tmpPos);
		armyPos.PosX= tmpPos.xPos;
		armyPos.PosY = tmpPos.yPos;
		armyPos_[0] = armyPos;
		return armyPos;
	}
	//����˺�
	virtual void clearHurt(void){
		computehurt_->resetHurt();
	}
	//�����йܲ���
	virtual void setStrategy(enumAIStrategy aiStrategy){
		action_->setStrategy(aiStrategy);
	}
	//�Ƿ�ɼ�
	virtual bool isVisible(IArmy * army) const {
		if (army->GetCountryID() != GetCountryID() && army->getBuildDetailType() >= 
			ARCH_BUILD_TOFT && army->getBuildDetailType() <= ARCH_TOFT_END){
			return isVisible_;
		}
		return true;
	}
	//��ò��ӵ�ǰ��������
	virtual LAND_FORM getLandForm(void) const ;
	//��������
	virtual bool revival(bool& isTerminate) {
		isTerminate = true;
		return false;
	}
	//���Ӹ���
	virtual IArmyPtr repeat( ENUM_USE_SCENE /*state*/ , bool& isTerminate){
		isTerminate = true;
		return NULL;
	}
	//�Ƿ�BASE����
	virtual bool isRepeatArmy(void) const {
		return isRepeatArmy_;
	}
	//�����Ƿ�BASE����
	inline void forbidRepeat() {
		isRepeatArmy_ = false;
	}
	//����ض�����
	const std::list<IArmyPtr> *  getArmysInPos(int x , int y) const;
	//�󶨸�������
	inline void attachArmy(unsigned int armyID){
		attachArmys.push_back(armyID);
	}
	//������а󶨲���
	inline void clearAttachArmy(void){
		attachArmys.clear();
	}
	//��ò��Ӱ��б�
	inline const vector<unsigned int>& getAttachArmys(void) const{
		return attachArmys;
	}
	void resetID(unsigned int /*npcArmyID*/){
		return ;
	}
	//���ý��������ID
	inline void setCountryID(int countryID) {
		countryID_ = countryID;
	}
	// ����������ָ��ĳ�㣬���������Χ������ʵĵ㣨�ǽ��������û��ռ��5�ˣ��ǵо�վ�ĸ�
	bool getNearPosInOneCircle(const SEEK_PATH_NAME_SPACE::PosXYStru &posInput, SEEK_PATH_NAME_SPACE::PosXYStru &posOutput){
		return action_->getNearPosInOneCircle(posInput , posOutput);
	}
	//�Ƿ�̬�赲
	inline bool isStaticBlook(int x, int y) const {
		return action_->isStaticBlook(x , y);
	}
	//�������λ��
	inline const POS_STRUCT & getCentryPos(void) const{
		return centryPos_;
	}
	//��������λ��
	inline void setCentryPos(const POS_STRUCT & pos){
		centryPos_ = pos;
	}
	//�Ƿ�����ش�Ӣ��
	virtual bool canPlasterHero(IArmyPtr /*armyPtr*/) const {
		return false;
	}
	// ��ȡӢ����������
	virtual computinghurt::HeroType getHeroType() const {
		return computehurt_->getHeroType();
	}
	//��ò���״̬
	virtual int getArmyDBState(void) const {
		return 1;
	}
	//��õ�ͼ״̬
	virtual mapstate getMapState(void) const ;
	//������������ID
	virtual int setLordArmy(unsigned int armyID) {
		return lordArmyID_ = armyID;
	}
	//��ò���Ӣ����Ϣ 
	virtual const string getHeroName() const {
		if (GetOfficerName() == NULL){
			return "";
		}
		return string(GetOfficerName());
	}
	//�Ƿ����ִ�
	virtual bool isNewbie()  const;
	//��ò��Ӿ��Ƶ���ϵ��
	virtual int getArmyCardDropOdds(void) const;
	//��õ�ǰ�غ���
	virtual int getCurBoutNum(void) const ;
	//���Ӣ��ְҵ
	virtual OFFICER_OCCUPATION_TYPE getHeroOccupation() const {
		return OFFICER_OCCUPATION_NONE;
	}
	//���������ص�Ч��
	virtual void clearEffect(void);
	//�Ƿ����ʹ��ĳ���Ϳ�Ƭ
	virtual bool isCanUseCard(CardType cardType){
		map<CardType ,PropCard*>::iterator iter = cardRecord_.find(cardType);
		return iter == cardRecord_.end();
	}
	//ʹ�ÿ�Ƭ
	virtual void useCard(PropCardConfig * propCardPtr);
	//���ĳ�����Ϳ�״̬
	virtual IArmy::CardUsedState getCardState(CardType cardType);
	//���ĳ�����Ϳ�
	virtual void clearCard(CardType cardType);
	//����
	void Attack(Armys &armyPtrsCenter, Armys &armyPtrsNear);
	//ʹ��Ч��
	virtual unsigned int useEffect(ENUM_SPECIALLY_EFFECT type , IArmyPtr armyPtr , int lastTime);
	//��ò�������
	virtual int getArmyKind(void) const{
		return 0;
	}
	//���þ�������
	virtual void setCropsName(char * cropsName){
		if (cropsName){
			cropsName_ = cropsName;
		}
	}
	//��þ�������
	virtual const string & getCropsName(void) const{
		return	cropsName_; 
	}
	//�Ƿ����ʹ������
	virtual bool isCanUseWeapon(void) const;
	//�Ƿ�����ƶ�
	virtual bool isCanMove(void) const;
	//��õ����ٶ�
	virtual int getTrajectorySpeed(void ) const{
		return trajectorySpeed_;
	}
	virtual void clearEffectByType(ENUM_SPECIALLY_EFFECT type , bool isClearEffect);
protected:
	EffectDetail * getEffectByType(ENUM_SPECIALLY_EFFECT type);
	void tidyEffect(void);
	
	void getArmysFromArmyArrays(ArmyArrays &armys , Armys& armyPtrs);
	//�Ƿ�������״̬
	bool isNoLockAction(void) const;
	//�����ƶ��ٶ�
	void calculateSpeed(void);
	//����Ƭ
	void tidyCard(void);
protected:
	bool isTrusteeship_; //�Ƿ��й�
	vector<POS_STRUCT> armyPos_; //����λ��
	POS_STRUCT armyBirthPos_; //�����ص�λ��
	POS_STRUCT centryPos_; //����λ��
	unsigned int lastAttackTarget_;//���һ�εĹ���Ŀ��
	unsigned int lastEnmityTarget_;//���һ�ι����Լ���Ŀ��
	unsigned int lordArmyID_; //��������ID
	ARMY_TYPE armyType_; //��������
	int countryID_; //����ID 
	ArmyAction * action_; //�����ж��������
	FightManager * fightManager_; //ս��������
	//�˺�ģ��
	computinghurt::IHurt *computehurt_; 
	//��������NPC
	vector<unsigned int> attachArmys;
	//��Ҳ�������
	string playerName_;
	//������������
	string cropsName_;
	//AI�ȼ�����
	AiLevelParam * aiLevelParam_;
	//�ϴ��ۼƾ���ֵ
	int exp_;
	//�Ƿ��ж���
	bool isActioned_;
	//�Ƿ�ɼ�
	bool isVisible_;
	//����ʱ��������
	int deadCycle_;
	//�Ƿ�BASE����
	bool isRepeatArmy_;
	//������������
	int revivalNum_;
	//Ч��ʹ�ü�¼
	map<ENUM_SPECIALLY_EFFECT , unsigned int> effectRecord_; 
	//��Ƭʹ�ü�¼
	map<CardType , PropCard *> cardRecord_;
	int moveSpeed_; //�ƶ��ٶ�
	int attackFreq_; //����Ƶ��
	int attackLockBout_; //��������ʱ��
	int moveLockBout_; //�ƶ�����ʱ��
	//�ϴι����غ���
	unsigned int lastAttackBoutNum_;
	//�ϴ��ƶ�ʱ�Ļغ���
	unsigned int lastMoveBoutNum_;
	list<ArmAttack*> attackActions_;//����������
	int movePerNeedTime_; //�ƶ�һ������ʱ��
	AttackTypeEnu attackType_;  // ��������
	int trajectorySpeed_; //�����ٶ�
};
