// @brief 战斗统计适配器，封装了各接口，以使使用者调用更简单
// 它关心具体的类，如IArmy, 而FightingStatistics是与具体类型无关的，它是个通用的统计类
// @author 张文龙

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

// @brief 当战斗开始时调用
void FightingStatisticsAdapter::onStart() const
{
	statistics_->onStart();
}

// @brief 当阵营campid的玩家playerid的部队armyid加入战斗时
// @param army 部队
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

// @brief 当阵营campId的玩家palyerId抓获英雄heroId
// @param army 部队
// @param heroId 英雄ID
void FightingStatisticsAdapter::onCatchedHero(const IArmy* army, HeroId heroId) const
{
	if(army == 0)
		return;

	statistics_->onCatchedHero(army->GetCountryID(),
		army->getPlayerID(), 
		army->GetArmyID(),
		heroId);
}

// @brief 当阵营campId的玩家palyerId的英雄heroId被抓获
// @param army 部队
// @param heroId 英雄ID
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

// @brief 当阵营campId的玩家palyerId的英雄heroId受重伤
// @param army 部队
// @param heroId 英雄ID
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

// @brief 当阵营campid的玩家playerid的部队armyid死亡了deathNum个士兵时
// @param army 部队
// @param deathNum 该部队死亡数量
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

// @brief 当阵营campid的玩家playerid的部队armyid杀死了短文killNum个士兵时
// @param army 部队
// @param killNum 该部队杀敌数量
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

// @brief 当部队死亡时
// @param army 部队
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

// @brief 根据部队ID查询部队信息
// @param army 部队
// @param param 返回部队信息到tagHeroParam结构
// @return 查询成功返回true, 否则返回false
bool FightingStatisticsAdapter::queryArmyInfo(const IArmy* army, tagHeroParam& param) const
{
	if(army == 0)
		return false;

	bool isSuccess = statistics_->queryArmyInfo(
		army->GetCountryID(),
		army->getPlayerID(),
		army->GetArmyID(),
		param);

	// 更新经验值
	param.iExp = army->getHeroIncreaseExp();
	param.lHeroID = army->GetOfficerID();
	// TODO:剩下的字段
	return isSuccess;
}

// @brief 战斗结束时调用
// @param winCampId 胜利方阵营ID
void FightingStatisticsAdapter::onOver(CountyId winCampId) const
{
	statistics_->onOver(winCampId);
}

// @brief 以玩家ID来查询战斗结果，并存放到info里
bool FightingStatisticsAdapter::queryFightingReport(CountyId countryId, PlayerId playerId, statisticTotalFightInfo& info) const
{
	return statistics_->queryFightingReport(countryId, playerId, info);
}

// @brief 当capturer杀死对方部队dieArmy
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
