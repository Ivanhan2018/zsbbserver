#ifndef TESTCOMPUTINGHURT_MOCKARMY_H_
#define TESTCOMPUTINGHURT_MOCKARMY_H_
#include "iarmy.h"
#include "ihurt.h"


void setHarmParam(const HarmInfo& harmInfo, HarmParm& harmParam)
{
	harmParam = *harmInfo.get();
}

void setFightReport(const AttackInfo& attackInfo, REPORT_FIGHT_INFO_STRUCT& fightReport)
{
	fightReport.ulAttackArmyID = attackInfo->armyId;
	fightReport.AttackPos = attackInfo->attackerPos;
	fightReport.TargetPos = attackInfo->targetPos;

	fightReport.type = attackInfo->armyType;
	fightReport.uWeaponID = attackInfo->weaponId;
	fightReport.AttackType = attackInfo->attackType;
	fightReport.Skill = attackInfo->skillId;
	HarmInfos harminfos = attackInfo->harmInfos;

	size_t count = harminfos.size() < 5 ? harminfos.size() : 5;
	fightReport.count = static_cast<int>(count);

	HarmInfos::const_iterator it = harminfos.begin();
	for (unsigned int i = 0; i < count; ++i)
	{
		HarmInfo info = (*it);
		setHarmParam(info, fightReport.harm[i]);
		++it;
	}
}


typedef boost::shared_ptr<IArmy> IArmySharedPtr;
typedef std::map<unsigned int, IArmySharedPtr> ArmysMap;


class IFightManager
{
public:
	virtual ~IFightManager(){};
	virtual IArmy* getArmy(unsigned int id) const = 0;
};



using namespace computinghurt;

class MockArmy : public IArmy
{
	friend class TestArmyHurt;
public:
	MockArmy(IHurt* computingHurt, IFightManager* manager)
		:computehurt_(computingHurt),skillid_(SKILLID_INVALID), hurt_(0),isCatched_(true),armyTable_(),manager_(manager)
	{
		if(computehurt_)
			computehurt_->setArmy(this);

		POS_STRUCT pos = {1, 1, 1};
		s.push_back(pos);
	}

	virtual void Attack(ArmyArrays& armys)
	{
		if(computehurt_ == 0 || manager_ == 0)
			return ;

		Armys armyPtrs;
		ArmyArrays::const_iterator it = armys.begin();
		for (; it != armys.end(); ++it)
		{
			armyPtrs.push_back(manager_->getArmy(*it));
		}

		AttackInfo attackInfo = computehurt_->attack(armyPtrs);
		hurt_ = 0;

//		REPORT_FIGHT_INFO_STRUCT fightReport;
//		setFightReport(attackInfo, fightReport);
#if 0
		{
			unsigned int armyid = attackInfo.get<0>();
			Positon attackerPos = attackInfo.get<1>();
			Positon targetPos = attackInfo.get<2>();
			ArmyType at = attackInfo.get<3>();
			unsigned int weaponId = attackInfo.get<4>();
			// ��������
			AttackType attackType = attackInfo.get<5>(); 
			// �������ͷŵļ���
			SkillId skillId = attackInfo.get<6>();

			HarmInfos harmInfos = attackInfo.get<7>();
		}
#endif
	}

	virtual HarmInfo beAttacked(IArmy* attacker, int atk, int additionalHurt, bool isNothinkingDefend)
	{
		if(computehurt_ == 0)
			return HarmInfo();

		HarmInfo info = computehurt_->beAttacked(attacker, atk, additionalHurt, isNothinkingDefend);		

		hurt_ = info->LostHp;
		return info;
	}

	// �����˺�
// 	virtual int processHurt(IArmy* attacker, int hurt) 
// 	{
// 		if(computehurt_ == 0)
// 			return 0;
// 
// 		int deaths = computehurt_->processHurt(attacker, hurt);
// 		hurt_ = hurt;
// 		return deaths;
// 	}

	// ֪ͨ�ͷ��˼���
	virtual void notifyRelease(SKILL_TYPE id)
	{
		skillid_ = id;
	}

	void onKilled(IArmy* army) 
	{
		if (computehurt_ == 0)
			return;

		if(computehurt_->canCatchHero(army))
		{
			isCatched_ = true;
		}

		isCatched_ = false;
	}

	virtual SOLDIER_TYPE GetArmyOccupation() const 
	{
		return computehurt_->getSoldierType();
	}

	// �õ����ӵ�ID
	virtual unsigned int GetArmyID() const 
	{
		return 0;
	}

	virtual ARMY_TYPE GetArmyType() const
	{
		return PLAYER_ARMY;
	}

	virtual  const std::vector<POS_STRUCT>& GetArmyPos()const 
	{
		return s;
	}

	//��ò�����Ұ��С
	virtual inline int GetViewScope() const {return 0;}
	//��ò��ӹ�����Χ
	virtual int GetAttackScope() const {return 0;}
	//��ò��ӷ���
	inline  virtual int GetDefendPower(ATTACK_TYPE /*attackType*/) const {return 0;}
	//��ò��ӹ�����
	inline  virtual int GetAttackPower() const{return 0;}
	//��ò��ӵ�ǰ��HP 
	inline  virtual int GetCurrentHp() const {return 0;}
	//��ò��ӵĹ���
	virtual  unsigned int getPlayerID() const {return 0;}
	//��ò���Ӣ��ID
	virtual  unsigned int GetOfficerID() const{return 0;}
	//��ע�����ݿ�������ID��Type��λ
	//�������ID
	virtual int GetWeaponID() const {return 0;}
	//�������Type
	virtual int GetWeaponType() const {return 0;}
	//��÷���ID
	virtual int GetDefendID() const {return 0;}
	//��÷���Type
	virtual int GetDefendType() const{return 0;}
	//��õ��侭��
	virtual  unsigned int GetLostExp(int /*num*/) const {return 1;}
	//��ø���Ŀ��
	virtual IArmyPtr GetFollowTarget() const {return armyptr_;}
	//���ø���Ŀ��
	virtual void SetFollowTarget(IArmyPtr /*army*/) {}
	//�ж��Ƿ��Ѿ�
	virtual bool IsFriendsArmy(IArmyPtr /*army*/)  {return false;}
	//�����ƶ�
	virtual bool Move(vector<POS_STRUCT>& /*moveTrack*/ , IArmyPtr /*armyPtr*/){return false;}
	//�����Ҳ��ӵ�����
	virtual  const char * GetArmyName()const{return 0;}
	//��ù�������
	virtual  ATTACK_TYPE GetAttackType()const {return PHYSIC_ATTACK;}
	//������һ���Լ�������Ŀ��
	virtual  IArmyPtr GetLastAttackTarget() {return armyptr_;}
	//������һ�ι����Լ���Ŀ��
	virtual  IArmyPtr GetLastEnmityTarget() {return armyptr_;}
	//����Լ�����ϵͳʱλ����Ϣ
	virtual  POS_STRUCT GetBirthPos()const{return POS_STRUCT();}
	//��ù���ID 
	virtual int GetCountryID()const {return 0;}
	//����й�״̬
	virtual  bool GetTrusteeshipState()const {return false;}
	//���������¼�
	virtual bool armyOnDie(IArmyPtr /*attackArmyPtr*/){return false;}
	//���ʿ������
	virtual int getSoldierNum() const {return 0;}
	//��ò��ӵ�ս����ɫ
	virtual FIGHT_ROLE_TYPE GetFightRole() const {return ATTACT;}
	//���Ӣ�۵�ͷ��ID
	virtual int GetFaceID() const {return 0;}
	//���ʿ������
	virtual RACE_TYPE GetSoldierRace() const {return HUMAN;}
	//��ò���Ӣ����Ϣ 
	virtual const char * GetOfficerName() const {return 0;}
	//���Ӣ������
	virtual int GetOfficerPower() const {return 0;}
	//���Ӣ������
	virtual int GetOfficerStamina() const{return 0;}
	//���Ӣ������
	virtual int GetOfficerCelerity() const {return 0;}
	//���Ӣ������
	virtual int GetOfficerIntellect() const {return 0;}
	//���Ӣ�ۼ���
	virtual int GetOfficerLevel() const {return 0;}
	//��õ�ǰ���ӵ���Ѫ
	virtual int GetHurtedHp() const {return 0;}
	//��ò��ӷ���
	virtual FORWORD getDirection() const{return LEFTUP;}
	//��ò���״̬
	virtual int GetArmyState() const {return 0;}
	//��ò����ƶ��ٶ�
	virtual int getMoveSpeed() const {return 0;}
	//����������Ϣ
	virtual bool exportArmyInfo(ARMY_POS & /*armyInfo*/ , int /*mapID*/) {return false;}
	//���ò��Ӵ���
	virtual bool processAction() {return false;}
	//�ͻ��˵�MOVE��Ϣ
	virtual bool processMoveMsg(POS_STRUCT & /*movePos*/) {return false;}
	//�����ת������������ͻ���
	virtual POS_STRUCT getReversePosS_C(POS_STRUCT & /*sourcePos*/) {return POS_STRUCT();}
	//�����ת����ͻ��˵�������
	virtual POS_STRUCT getReversePosC_S(POS_STRUCT & /*sourcePos*/) {return POS_STRUCT();}
	//�������ò��ӵ�λ��
	virtual void reSetArmyPos(POS_STRUCT /*armyPos*/){return ;}
	//���ù���Ŀ��
	virtual bool processAttackTarget(const IArmyPtr /*attackTargetPtr*/) {return false;}
	//�����Ҳ��ӵ�ArmyTable��Ϣ
	virtual void getArmyTable(ArmyTable *& /*armyTable*/) {}
	//��ý��������ϸ����
	virtual BuildingType getBuildDetailType() const {return BUILDINGTYPE_BASTILE;}
	virtual bool isPlayerArmy() const {return true;}
	virtual ENUM_ARMY_STATUS getArmyActionStatus() const {return Attack_status;};
	//��õ�ǰ������Ӱ��ϵ��
	virtual float getAttackFactor() const {return 1;}
	//��õ�ǰ��������Ӱ��ϵ��
	virtual float getDefendFactor() const {return 1;}
	//������һ���Լ�������Ŀ��
	virtual  void SetLastAttackTarget(IArmyPtr /*lastAttackTarget*/) {return;}
	//������һ�ι����Լ���Ŀ��
	virtual  void SetLastEnmityTarget(IArmyPtr /*lastEnmityTarget*/) {return;}

	//��õ�ǰ��Ѫ
	virtual int getCurHurtHp() const {return 0;}
	//���Ӣ�����ӵľ���ֵ
	virtual int getHeroIncreaseExp() const{return 0;}

	virtual bool getOfficerParam(tagHeroParam & /*officerParam*/) {return false;}
	//���FROM��ͼ��ID
	virtual int  getFromMapID() const{return 0;}

	//��ò��Թ�����
	virtual IManager * getStrategyMgr() const{return 0;}
	//�����й�״̬
	virtual void setTrusteeshipState(bool /*trusteeshipState*/) {}
	//�ͷ��ڴ�
	virtual void release() {}

	//ͨ������ID�������������Ϣ
	virtual IArmyPtr getArmyByID(unsigned int /*armyID*/) {return 0;};

	//������һ���Լ�������Ŀ��
	virtual  void SetLastAttackTarget(unsigned int /*lastAttackTarget*/)  {}
	//������һ�ι����Լ���Ŀ��
	virtual  void SetLastEnmityTarget(unsigned int /*lastEnmityTarget*/) {}
	//��õ�����Ʒ
	virtual int getBootyID() const {return 0;}
	//��õ�����Ʒ�ĸ���
	virtual int getBootyProbability() const {return 0;}
	//���������¼�
	virtual bool armyOnDie(IArmyPtr /*attackArmyPtr*/ , bool /*isCatched*/) {return true;}
	//�����Чλ��
	virtual POS_STRUCT getValidPos(POS_STRUCT & pos) {return pos;};

	//����������
	virtual const string & getPlayerName() const { static const std::string s(""); return s;}
	//�Ƿ���NPC����
	virtual bool isNpcArmy() const {return false;}
	//������Ϣ��������
	virtual bool reset(int /*condition*/) {return false;}

	////ˢ�²��Ӿ���
	virtual bool refreshExp(void) {return false;}
	//Ӣ������
	virtual bool officerGrowUp(){return false;}

	// ���ݹ�����(�Լ�)�������뱻�����ߵķ������õ�һ������
	virtual float getFactorAccordingToArmor(IArmy* /*beAttacker*/)
	{
		return 1.0;
	}

	//�������ò��ӵ�λ��
	virtual void setArmyPos(POS_STRUCT /*armyPos*/)
	{
		return;
	}
	//��������ʿ������
	virtual bool resetSoldierNum(int /*soldierNum*/) {return true;}
	//�������ò��ӵ�λ��
	virtual POS_STRUCT setGatewayArmyPos(POS_STRUCT armyPos)
	{
		return armyPos;
	}


private:
	MockArmy();
private:
	IHurt* computehurt_;
	SKILL_TYPE skillid_; // ��ǰ�ͷŵļ���
	int hurt_; // �ܵ����˺�
	bool isCatched_;
	std::vector<POS_STRUCT> s;
	ArmyTable armyTable_;
	IArmyPtr armyptr_;
	IFightManager* manager_;
};


class MockFightManager : public IFightManager
{
public:
	MockFightManager()
		:armys_()
	{
	}

	~MockFightManager()
	{

	}

	void insert(unsigned int id, IArmySharedPtr* army)
	{
		armys_.insert(ArmysMap::value_type(id, *army));
	}


	virtual IArmy* getArmy(unsigned int id) const
	{
		ArmysMap::const_iterator it = armys_.find(id);
		if(it == armys_.end())
			return 0;

		return (*it).second.get();
	}

private:

	ArmysMap armys_;
};

#endif // TESTCOMPUTINGHURT_MOCKARMY_H_
