// @brief ս��ͳ������������װ�˸��ӿڣ���ʹʹ���ߵ��ø���
// �����ľ�����࣬��IArmy, ��FightingStatistics������������޹صģ����Ǹ�ͨ�õ�ͳ����
// @author ������

#include "fightingstatisticsadapter.h"
#include "fightingstatistics.h"

STATISTICSSYSTEM_NAMESPACE_BEGIN

FightingStatisticsAdapter::FightingStatisticsAdapter(float percent)
:statistics_(new FightingStatistics(percent))
,percent_(percent)
{

}

FightingStatisticsAdapter::~FightingStatisticsAdapter()
{

}

// @brief ��ս����ʼʱ����
void FightingStatisticsAdapter::onStart() const
{
	statistics_->onStart();
}

// @brief ����Ӫcampid�����playerid�Ĳ���armyid����ս��ʱ
// @param army ����
void FightingStatisticsAdapter::onEnterWar(const IArmy* army) const
{
	if(army == 0)
		return;

	std::string armyName;
	if(army->GetArmyName() != 0)
		armyName = army->GetArmyName();

	statistics_->onEnterWar(army->GetCountryID(),
		army->getPlayerID(),
		army->GetArmyID(),
		army->getSoldierNum(),
		army->GetArmyOccupation(),
		armyName,
		army->getHeroName(),
		army->GetFaceID(),
		army->getPlayerName());

}

// @brief ����ӪcampId�����palyerIdץ��Ӣ��heroId
// @param army ����
// @param heroId Ӣ��ID
void FightingStatisticsAdapter::onCatchedHero(const IArmy* army, HeroId heroId) const
{
	if(army == 0)
		return;

	statistics_->onCatchedHero(army->GetCountryID(),
		army->getPlayerID(), 
		army->GetArmyID(),
		heroId);
}

// @brief ����ӪcampId�����palyerId��Ӣ��heroId��ץ��
// @param army ����
// @param heroId Ӣ��ID
void FightingStatisticsAdapter::onBeCatchedHero(const IArmy* army, HeroId heroId) const
{
	if(army == 0)
		return;

	statistics_->onBeCatchedHero(
		army->GetCountryID(),
		army->getPlayerID(),
		army->GetArmyID(),
		heroId);
}

// @brief ����ӪcampId�����palyerId��Ӣ��heroId������
// @param army ����
// @param heroId Ӣ��ID
void FightingStatisticsAdapter::onHeroWasInjured(const IArmy* army, HeroId heroId) const
{
	if(army == 0)
		return;

	statistics_->onHeroWasInjured(
		army->GetCountryID(),
		army->getPlayerID(),
		army->GetArmyID(),
		heroId);
}

// @brief ����Ӫcampid�����playerid�Ĳ���armyid������deathNum��ʿ��ʱ
// @param army ����
// @param deathNum �ò�����������
void FightingStatisticsAdapter::onSoliderDie(const IArmy* army, unsigned int deathNum) const
{
	if(army == 0)
		return;

	statistics_->onSoliderDie(
		army->GetCountryID(),
		army->getPlayerID(),
		army->GetArmyID(),
		deathNum,
		army->getSoldierNum());
}

// @brief ����Ӫcampid�����playerid�Ĳ���armyidɱ���˶���killNum��ʿ��ʱ
// @param army ����
// @param killNum �ò���ɱ������
void FightingStatisticsAdapter::onKilledEnemy(const IArmy* army, unsigned int killNum) const
{
	if(army == 0)
		return;

	statistics_->onKilledEnemy(
		army->GetCountryID(),
		army->getPlayerID(),
		army->GetArmyID(),
		killNum);
}

// @brief ����������ʱ
// @param army ����
void FightingStatisticsAdapter::onArmyDie(const IArmy* army) const
{
	if(army == 0)
		return;

	statistics_->onArmyDie(
		army->GetCountryID(),
		army->getPlayerID(),
		army->GetArmyID(),
		army->getHeroIncreaseExp());
}

void FightingStatisticsAdapter::onArmyFlashExp(const IArmy* army) const
{
	if(army == 0)
		return;

	statistics_->onArmyFlashExp(
		army->GetCountryID(),
		army->getPlayerID(),
		army->GetArmyID(),
		army->getHeroIncreaseExp());
}

// @brief ���ݲ���ID��ѯ������Ϣ
// @param army ����
// @param param ���ز�����Ϣ��tagHeroParam�ṹ
// @return ��ѯ�ɹ�����true, ���򷵻�false
bool FightingStatisticsAdapter::queryArmyInfo(const IArmy* army, tagHeroParam& param) const
{
	if(army == 0)
		return false;

	bool isSuccess = statistics_->queryArmyInfo(
		army->GetCountryID(),
		army->getPlayerID(),
		army->GetArmyID(),
		param);

	// ���¾���ֵ
	param.iExp = army->getHeroIncreaseExp();
	param.lHeroID = army->GetOfficerID();
	// TODO:ʣ�µ��ֶ�
	return isSuccess;
}

// @brief ս������ʱ����
// @param winCampId ʤ������ӪID
void FightingStatisticsAdapter::onOver(CountyId winCampId) const
{
	statistics_->onOver(winCampId);
}

// @brief �����ID����ѯս�����������ŵ�info��
bool FightingStatisticsAdapter::queryFightingReport(CountyId countryId, PlayerId playerId, statisticTotalFightInfo& info) const
{
	return statistics_->queryFightingReport(countryId, playerId, info);
}

// @brief ��capturerɱ���Է�����dieArmy
void FightingStatisticsAdapter::onCaptured(const IArmy* capturer, const IArmy* dieArmy, int num) const
{
	if(capturer == 0 || dieArmy == 0 || num == 0)
		return;

	statistics_->onCaptured(
		capturer->GetCountryID(),
		capturer->getPlayerID(),
		dieArmy->GetWeaponID(),
		dieArmy->GetDefendID(),
		num
		);
}

void FightingStatisticsAdapter::getCapturedWeapon(CountyId countyId, PlayerWeapons& weapons) const
{
	statistics_->getCapturedWeapon(countyId, weapons);
}

void FightingStatisticsAdapter::reset()
{
	statistics_.reset(new FightingStatistics(percent_));
}

void FightingStatisticsAdapter::onObtainBox(const IArmy* army, int num)
{
	if(army == 0 || num == 0)
		return;

	statistics_->onObtainBox(
		army->GetCountryID(),
		army->getPlayerID(),
		num
		);
}

void FightingStatisticsAdapter::getWeapons(PlayerWeapons& weapons) const
{
	statistics_->getWeapons(weapons);
}

void FightingStatisticsAdapter::getCapturedArmor(CountyId countyId, PlayerArmors& armors) const
{
	statistics_->getCapturedArmor(countyId, armors);
}

void FightingStatisticsAdapter::getArmors(PlayerArmors& armors) const
{
	statistics_->getArmors(armors);
}

void FightingStatisticsAdapter::onAddSoldiers(const IArmy* army) const
{
	if(army == 0)
		return;

	statistics_->onAddSoldiers(
		army->GetCountryID(),
		army->getPlayerID(),
		army->GetArmyID(),
		army->getSoldierNum());
}


STATISTICSSYSTEM_NAMESPACE_END
