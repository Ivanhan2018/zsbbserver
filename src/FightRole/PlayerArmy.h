#pragma once
#include "Army.h"
#include "computinghurt/common.h"
class PlayerArmy :public Army
{
public:
	PlayerArmy(void);
public:
	virtual ~PlayerArmy(void);
public:
	
	//��ʼ������
	bool Initialize(PlayerArmyParam & armyParam , FightManager * fightManager ,  ENUM_USE_SCENE state);
	
	//��ò�����Ұ��С
	virtual inline int GetViewScope() const{
		if (aiLevelParam_){
			return aiLevelParam_->sightVal;
		}
		return soldierParam_->visualrange_;
	}
	//��ò��ӹ�����Χ
	inline int GetAttackScope() const{
		return soldierParam_->attackscop_;
	}
	//��ò��ӵĹ���
	unsigned int getPlayerID() const{
		return armyTable_.playerid_;
	}
	//��ò���Ӣ��ID
	unsigned int GetOfficerID() const {
		return playerOfficerTable_.officerid_;
	}
	//��ע�����ݿ�������ID��Type��λ
	//�������ID
	int GetWeaponID() const{
		return armyWeapon_->weaponID;
	}
	//�������Type
	int GetWeaponType() const {
		return armyWeapon_->weaponType;
	}
	//��÷���ID
	virtual int GetDefendID() const{
		if (armyDefend_){
			return armyDefend_->defendID;
		} 
		return 0;
	}
	//��÷���Type
	inline int GetDefendType() const {
		if (armyDefend_){
			return armyDefend_->defendType;
		}
		return 0;
	}
	//��õ��侭��
	inline unsigned int GetLostExp(int num) const{
		return num * armyWeapon_->exp;
	}
	//��ø���Ŀ��
	inline IArmyPtr GetFollowTarget() const; 
	//���ø���Ŀ��
	inline void SetFollowTarget(IArmyPtr army){
		followTarget_ = army->GetArmyID();
	}
	
	//��ò���ID
	inline  unsigned int GetArmyID() const{ 
		return armyTable_.armyid_;
	}
	//���Ӣ�۵�ͷ��ID
	inline int GetFaceID() const {
		return playerOfficerTable_.faceid_;
	}
	//���ʿ������
	inline RACE_TYPE GetSoldierRace() const {
		return (RACE_TYPE)armyTable_.raceid_;
	}
	//��ò���Ӣ����Ϣ 
	inline const char * GetOfficerName() const {
		return playerOfficerTable_.officername_;
	}
	//���Ӣ������
	inline int GetOfficerPower() const {
		return playerOfficerTable_.power_ + playerOfficerTable_.levelpower_ + playerOfficerTable_.equippower_;
	}
	//���Ӣ������
	inline int GetOfficerStamina() const {
		return playerOfficerTable_.stamina_ + playerOfficerTable_.levelstamina_ + playerOfficerTable_.equipstamina_;
	}
	//���Ӣ������
	inline int GetOfficerCelerity() const {
		return playerOfficerTable_.celerity_ + playerOfficerTable_.levelcelerity_ + playerOfficerTable_.equipcelerity_;
	}
	//���Ӣ������
	inline int GetOfficerIntellect() const{
		return playerOfficerTable_.wit_ + playerOfficerTable_.levelwit_ + playerOfficerTable_.equipwit_;
	}
	//���Ӣ�ۼ���
	inline int GetOfficerLevel() const {
		return playerOfficerTable_.level_;
	}
	//����ְҵ
	virtual SOLDIER_TYPE GetArmyOccupation()const {
		return (SOLDIER_TYPE)soldierParam_->soldiertype_;
	}
	//���ʿ������
	inline int getSoldierNum() const {
		return computehurt_->getSoldierNum();
	}
	//��ù�������
	ATTACK_TYPE GetAttackType()const {
		return (ATTACK_TYPE)armyWeapon_->hurt;
	}	
	//���������¼�
	virtual bool armyOnDie(IArmyPtr attackArmyPtr , bool isCatched , bool isProcessHurt);
	//�����Ҳ��ӵ�����
	const char * GetArmyName() const {
		return armyTable_.armyname_;
	}
	FORWORD getDirection() const {
		return (FORWORD) armyTable_.direction;
	}
	bool processMoveMsg(POS_STRUCT & movePos){
		PosXYStru pos(movePos.PosX , movePos.PosY);
		return action_->setMoveTargetPos(pos);
	}
	bool processAttackTarget(const IArmyPtr attackTargetPtr){
		return action_->setAttackTarget(attackTargetPtr);
	}
	//�����ƶ�
	bool Move(vector<POS_STRUCT>& moveTrack ,IArmyPtr armyPtr) ;
	//�������ò��ӵ�λ��
	virtual void setArmyPos(POS_STRUCT armyPos) {
		armyPos_[0] = armyPos;
	}
	virtual void getArmyTable(ArmyTable *& armyTable)  {
		armyTable = &armyTable_;
	}
	//���������˺�
	bool createComputeHurt(ARMY_TYPE armyType);
	//���ˢ�²��� 
	virtual bool getOfficerParam(tagHeroParam & /*officerParam*/) {
		return false;
	}
	//���FROM��ͼ��ID
	virtual int  getFromMapID() const{
		return armyTable_.mapid_;
	}
	//����Ӧ
	virtual float getAttackFactor() const ;
	//
	virtual void onKilled(IArmy* army);
	virtual int getOfficerExp(void){
		return playerOfficerTable_.exp_;
	}
	virtual bool refreshExp(void);
	//Ӣ������
	virtual bool officerGrowUp();
	//@brief ��������
	void upgradeLevel(const computinghurt::HeroAttribute& heroAttribute , PlayerOfficerTable & playerInfo);
	//@brief ѧϰ���µļ��ܻ�������
	void upgradeOrLearnedSkill(const computinghurt::SkillInfo& skillInfo);
	//��������
	bool abandonSkill(const computinghurt::SkillAndGradeStru& skillAndGradeStru);
	//�Ƿ�����ش�Ӣ��
	virtual bool canPlasterHero(IArmyPtr armyPtr) const {
		return computehurt_->canPlasterHero(armyPtr);
	}
	//��ò���״̬
	virtual int getArmyDBState(void) const {
		return armyTable_.state_;
	}
	//��ò��Ӿ��Ƶ���ϵ��
	virtual int getArmyCardDropOdds(void) const;
	//���Ӣ��ְҵ
	virtual OFFICER_OCCUPATION_TYPE getHeroOccupation() const {
		return static_cast<OFFICER_OCCUPATION_TYPE>(playerOfficerTable_.jobid_);
	}
	inline void setMercenary(){
		isMercenary_ = true;
	}
	inline bool getIsMercenary() const{
		return isMercenary_;
	}
	inline void setPlayerName(const string&  playerName){
		playerName_ = playerName;
	}
protected:
	
	PlayerOfficerTable playerOfficerTable_; //���Ӵ���Ӣ�۱�
	WeaponAttribute* armyWeapon_;
	DefendAttribute* armyDefend_;
	SoldierParamTable * soldierParam_ ;//ʿ��������
	OfficerRaceAttribute *officerRace_;//Ӣ���������Ա�
	ArmyTable armyTable_; //������Ϣ
	unsigned int followTarget_; //����Ŀ��
	bool isMercenary_; //�Ƿ�Ӷ��
};
