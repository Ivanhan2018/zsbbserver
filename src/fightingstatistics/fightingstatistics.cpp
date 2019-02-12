// $Id: fightingstatistics.cpp 1 2009-12-08 18:59:28 Zhangwenlong $ 
// @brief 战斗统计类，负责各种战斗的统计，暂时只适配原有的系统
// @author 张文龙

#include "fightingstatistics.h"
#include "globalmacrodef.h"


namespace
{
	static void safeStrCpy(char* dest, size_t destlen, const char* src, size_t srclen)
	{
		if(srclen < destlen)
		{
			strcpy(dest, src);
			dest[destlen - 1] = '\0';
			return;
		}

		memcpy(dest, src, destlen - 1);
		dest[destlen - 1] = '\0';
	}


	// TODO:是玩家部队
	static bool isPlayerArmy(PlayerId playerId)
	{
		return playerId >= 100000;
	}


	// 计算武器的折损率
	static void computeWeaponsAccordingtoPecent(WeaponMap& dest, const WeaponMap& src, float percent)
	{
		WeaponMap::const_iterator it = src.begin();
		for (; it != src.end(); ++it)
		{
			// 控制， 如果加入百分比考虑， 有可能为0， 就不加入缴获
			int num = static_cast<int>((*it).second * percent);
			if(num == 0)
				continue;
			dest.insert(WeaponMap::value_type((*it).first, num));
		}
	}

	// 计算防具的折损率
	static void computeArmorAccordingtoPercent(ArmorMap& dest, const ArmorMap& src, float percent)
	{
		ArmorMap::const_iterator it = src.begin();
		for (; it != src.end(); ++it)
		{
			// 控制， 如果加入百分比考虑， 有可能为0， 就不加入缴获
			int num = static_cast<int>((*it).second * percent);
			if(num == 0)
				continue;
			dest.insert(ArmorMap::value_type((*it).first, num));
		}
	}


} // namespace

STATISTICSSYSTEM_NAMESPACE_BEGIN
FightingStatistics::FightingStatistics(float percent)
:startTime_(0)
,overTime_(0)
,campInfos_()
,winCampId_(0)
,percent_(percent)
{

}

FightingStatistics::~FightingStatistics()
{
	startTime_ = 0;
	overTime_ = 0;
	winCampId_ = 0;
}

void FightingStatistics::onStart()
{
 	time(&startTime_);
}

// 查找玩家信息
FightingStatistics::PlayerInfoPtr FightingStatistics::findElseInsertPlayerInfo(CountyId countyId, PlayerId playerId)
{
	CampInfoMap::iterator it = campInfos_.find(countyId);
	// 找到了camp
	if( it != campInfos_.end())
	{
		CampInfo* campInfo = (*it).second.get();
		PlayerInfoMap::iterator it_player = campInfo->playerInfos.find(playerId);
		// 找到了player
		if(it_player != campInfo->playerInfos.end())
		{
			return (*it_player).second;
		}

		// 未找到player
		PlayerInfoPtr playerInfo(new PlayerInfo());
		campInfo->playerInfos.insert(PlayerInfoMap::value_type(playerId, playerInfo));
		return playerInfo;

	}

	// 没找到camp
	PlayerInfoPtr playerInfo(new PlayerInfo());

	CampInfoPtr campInfo(new CampInfo());
	campInfo->playerInfos.insert(PlayerInfoMap::value_type(playerId, playerInfo));

	campInfos_.insert(CampInfoMap::value_type(countyId, campInfo));
	return playerInfo;
}

FightingStatistics::ArmyInfoPtr FightingStatistics::findElseInsertArmyInfo(CountyId countyId, PlayerId playerId, ArmyId armyId)
{
	PlayerInfoPtr playerInfo = findElseInsertPlayerInfo(countyId, playerId);
	if(playerInfo.get() == 0)
		return ArmyInfoPtr();

	ArmyInfoMap::const_iterator it = playerInfo->armyInfos.find(armyId);
	// 找到了
	if(it != playerInfo->armyInfos.end())
	{
		return (*it).second;
	}

	// 未找到
	ArmyInfoPtr armyInfo(new ArmyInfo());
	playerInfo->armyInfos.insert(ArmyInfoMap::value_type(armyId, armyInfo));
	return armyInfo;
}

void FightingStatistics::onCatchedHero(CountyId countyId, PlayerId playerId, ArmyId armyId, HeroId heroId)
{
	ArmyInfoPtr armyInfo = findElseInsertArmyInfo(countyId, playerId, armyId);
	if(armyInfo.get() == 0)
		return;

	armyInfo->herosCatched.insert(heroId);
}

void FightingStatistics::onBeCatchedHero(CountyId countyId, PlayerId playerId, ArmyId armyId, HeroId heroId)
{
	ArmyInfoPtr armyInfo = findElseInsertArmyInfo(countyId, playerId, armyId);
	if(armyInfo.get() == 0)
		return;

	armyInfo->herosBeCatched.insert(heroId);
}

void FightingStatistics::onHeroWasInjured(CountyId countyId, PlayerId playerId, ArmyId armyId, HeroId heroId)
{
	ArmyInfoPtr armyInfo = findElseInsertArmyInfo(countyId, playerId, armyId);
	if(armyInfo.get() == 0)
		return;

	armyInfo->heroWasInjured.insert(heroId);
}

void FightingStatistics::onEnterWar(CountyId countyId, PlayerId playerId, ArmyId armyId, 
									unsigned int soldierNum, SOLDIER_TYPE type, 
									const std::string& armyName,
									const std::string& heroName,
									int heroFaceId,
									const std::string& playerName)
{
	ArmyInfoPtr armyInfo = findElseInsertArmyInfo(countyId, playerId, armyId);
	if(armyInfo.get() == 0)
		return;

	// 20091109 by zwl 当部队在郡里进来再撤退, 再进时出现部队人数累加的情况, 这里将它改为直接赋值
	armyInfo->soldierNum = soldierNum;
	armyInfo->type = type;
	armyInfo->name = armyName;
	armyInfo->heroName = heroName;
	armyInfo->heroFaceId = heroFaceId;
	armyInfo->playerName = playerName;
}

void FightingStatistics::onSoliderDie(CountyId countyId, PlayerId playerId, ArmyId armyId, unsigned int deathNum, unsigned int soldierNum)
{
	ArmyInfoPtr armyInfo = findElseInsertArmyInfo(countyId, playerId, armyId);
	if(armyInfo.get() == 0)
		return;

	armyInfo->soldierNum = soldierNum;
	armyInfo->deathNum += deathNum;
}

void FightingStatistics::onKilledEnemy(CountyId countyId, PlayerId playerId, ArmyId armyId, unsigned int killNum)
{
	ArmyInfoPtr armyInfo = findElseInsertArmyInfo(countyId, playerId, armyId);
	if(armyInfo.get() == 0)
		return;

	armyInfo->killNum += killNum;
}

void FightingStatistics::onArmyFlashExp(CountyId countyId, PlayerId playerId, ArmyId armyId, unsigned int exp)
{
	ArmyInfoPtr armyInfo = findElseInsertArmyInfo(countyId, playerId, armyId);
	if(armyInfo.get() == 0)
		return;

	// 死亡了一次， 不可能再死一次
	//	MY_ASSERT(armyInfo->status != ARMYSTATUS_DIE, "armyInfo->status == ARMYSTATUS_DIE");

	armyInfo->exp = exp;
	armyInfo->status = ARMYSTATUS_ALIVE;
}

void FightingStatistics::onArmyDie(CountyId countyId, PlayerId playerId, ArmyId armyId, unsigned int exp)
{
	ArmyInfoPtr armyInfo = findElseInsertArmyInfo(countyId, playerId, armyId);
	if(armyInfo.get() == 0)
		return;

	// 死亡了一次， 不可能再死一次
//	MY_ASSERT(armyInfo->status != ARMYSTATUS_DIE, "armyInfo->status == ARMYSTATUS_DIE");

	armyInfo->exp = exp;
	armyInfo->status = ARMYSTATUS_DIE;
}

bool FightingStatistics::queryArmyInfo(CountyId countyId, PlayerId playerId, ArmyId armyId, tagHeroParam& param) const
{
	memset(&param, 0, sizeof(param));
	if(campInfos_.empty())
		return false;

	CampInfoMap::const_iterator it_camp = campInfos_.find(countyId);
	if(it_camp == campInfos_.end())
		return false;

	const PlayerInfoMap& playerInfos = (*it_camp).second.get()->playerInfos;
	PlayerInfoMap::const_iterator it_player = playerInfos.find(playerId);
	if(it_player == playerInfos.end())
		return false;

	const ArmyInfoMap& armyInfos = (*it_player).second.get()->armyInfos;
	ArmyInfoMap::const_iterator it_army = armyInfos.find(armyId);
	if(it_army == armyInfos.end())
		return false;

	const ArmyInfo* armyInfo = (*it_army).second.get();
	if(armyInfo == 0)
		return false;

	param.lStartFight = armyInfo->startTime;
	param.iExp = armyInfo->exp;
	param.iEnemyCount = armyInfo->killNum;
	param.iGeneralCount = static_cast<int>(armyInfo->herosCatched.size());
	param.lPlayerID = playerId;
	param.lMapID = countyId;
	safeStrCpy(param.szArmyName, SIZE_32, armyInfo->name.c_str(), armyInfo->name.length());

	return true;
}

void FightingStatistics::onOver(CountyId winCountyId)
{
	winCampId_ = winCountyId;
	time(&overTime_);
}

const FightingStatistics::PlayerInfoPtr FightingStatistics::getPlayerInfo(CountyId countyId, PlayerId playerId) const
{
	if(campInfos_.empty())
		return PlayerInfoPtr();

	CampInfoMap::const_iterator it = campInfos_.find(countyId);
	// 找到了camp
	if( it != campInfos_.end())
	{
		CampInfo* campInfo = (*it).second.get();
		PlayerInfoMap::const_iterator it_player = campInfo->playerInfos.find(playerId);
		// 找到了player
		if(it_player != campInfo->playerInfos.end())
		{
			return (*it_player).second;
		}

		// 未找到player
		return PlayerInfoPtr();
	}

	return PlayerInfoPtr();
}

FightingStatistics::PlayerStatisticsInfo FightingStatistics::getPlayerStatisticsInfo(const PlayerInfoPtr& playerInfo) const
{
	if(playerInfo.get() == 0)
		return PlayerStatisticsInfo();

	const ArmyInfoMap& armyInfos = playerInfo.get()->armyInfos;

	int catchedNum = 0;
	int beCatchedNum = 0;
	int wasInjuredNum = 0;
	int deathNum = 0;
	int killNum = 0;
	std::string playerName("");
	for (ArmyInfoMap::const_iterator it = armyInfos.begin(); 
		it != armyInfos.end(); ++it)
	{
		catchedNum += static_cast<int>((*it).second.get()->herosCatched.size());
		beCatchedNum += static_cast<int>((*it).second.get()->herosBeCatched.size());
		wasInjuredNum += static_cast<int>((*it).second.get()->heroWasInjured.size());
		deathNum += (*it).second.get()->deathNum;
		killNum += (*it).second.get()->killNum;
		playerName = (*it).second.get()->playerName;
	}

	PlayerStatisticsInfo info(new PlayerStatisticsInfoValue);
	info->catchedNum = catchedNum;
	info->beCatchedNum = beCatchedNum;
	info->wasInjuredNum = wasInjuredNum;
	info->killNum = killNum;
	info->deathNum = deathNum;
	info->playerName = playerName;

	return info;
}

FightingStatistics::StatisticArmyFightInfoPtrs FightingStatistics::getStatisticArmyFightInfoPtrs() const
{
	if (campInfos_.empty())
		return StatisticArmyFightInfoPtrs();

	StatisticArmyFightInfoPtrs infos;

	CampInfoMap::const_iterator it_camp = campInfos_.begin();
	for (; it_camp != campInfos_.end(); ++it_camp)
	{
		PlayerInfoMap::const_iterator it_player = (*it_camp).second.get()->playerInfos.begin();
		for (; it_player != (*it_camp).second.get()->playerInfos.end(); ++it_player)
		{
			// 不是玩家部队
			if(!isPlayerArmy((*it_player).first))
				continue;

			ArmyInfoMap::const_iterator it_army = (*it_player).second.get()->armyInfos.begin();
			for (; it_army != (*it_player).second.get()->armyInfos.end(); ++it_army)
			{
				const ArmyInfoPtr& armyInfo = (*it_army).second;
				StatisticArmyFightInfoPtr ptr(new StatisticArmyFightInfo);
				// ...
				ptr->HeroFaceId = armyInfo->heroFaceId;
				ptr->type = armyInfo->type;
				// TODO:
				ptr->state = 0;
				ptr->SoldierNum = armyInfo->soldierNum;
				ptr->deadNum = armyInfo->deathNum;
				ptr->KillNum = armyInfo->killNum;
				ptr->iExp = armyInfo->exp;
				// 安全问题
 				MY_ASSERT(armyInfo->name.size() < SIZE_24, "armyInfo->name.size() >= SIZE_24");
				safeStrCpy(ptr->ArmyName, SIZE_24, armyInfo->name.c_str(), armyInfo->name.length());
	
				MY_ASSERT(armyInfo->heroName.size() < SIZE_24, "armyInfo->heroName.size() >= SIZE_24");
				safeStrCpy(ptr->HeroName, SIZE_24, armyInfo->heroName.c_str(), armyInfo->heroName.length());

				infos.push_back(ptr);
			}
		}
	}

	return infos;
}

CountyId FightingStatistics::getEnemyCampId(CountyId countyId) const
{
	if(campInfos_.empty())
		return 0;

	CampInfoMap::const_iterator it_camp = campInfos_.begin();
	for(; it_camp != campInfos_.end(); ++it_camp)
	{
		if((*it_camp).first != countyId)
			return (*it_camp).first;
	}

	return 0;
}

FightingStatistics::PlayerStatisticsInfo FightingStatistics::getCampStatisticsInfo(CountyId countyId) const
{
	CampInfoMap::const_iterator it_camp = campInfos_.find(countyId);
	if(it_camp == campInfos_.end())
		return PlayerStatisticsInfo();

	const CampInfo& campInfo = *(*it_camp).second.get();

	int catchedNum = 0;
	int beCatchedNum = 0;
	int wasInjuredNum = 0;
	int killNum = 0;
	int deathNum = 0;

	PlayerInfoMap::const_iterator it_player = campInfo.playerInfos.begin();
	for (; it_player != campInfo.playerInfos.end(); ++it_player)
	{
		PlayerStatisticsInfo temp = getPlayerStatisticsInfo((*it_player).second);
		catchedNum += temp->catchedNum;
		beCatchedNum +=  temp->beCatchedNum;
		wasInjuredNum += temp->wasInjuredNum;
		deathNum += temp->deathNum;
		killNum += temp->killNum;
	}

	PlayerStatisticsInfo info(new PlayerStatisticsInfoValue);
	info->catchedNum = catchedNum;
	info->beCatchedNum = beCatchedNum;
	info->wasInjuredNum = wasInjuredNum;
	info->killNum = killNum;
	info->deathNum = deathNum;
	info->playerName = "";

	return info;
}

void FightingStatistics::getFightStatistic( CountyId countyId, PlayerId playerId, FIGHT_STATISTIC &fightStatistic, statisticTotalFightInfo& info ) const
{
	PlayerInfoPtr playerInfo = getPlayerInfo(countyId, playerId);
	if(playerInfo.get() == 0)
	{
		info.obtainBoxNum = 0;
		fightStatistic.PlayerID = playerId;
		return;
	}

	// 宝箱数
	info.obtainBoxNum = playerInfo->boxNum;
	PlayerStatisticsInfo psi = getPlayerStatisticsInfo(playerInfo);
	fightStatistic.catchHeroNum = psi->catchedNum;
	fightStatistic.catchedHeroNum = psi->beCatchedNum;
	fightStatistic.hurtHeroNum = psi->wasInjuredNum;
	fightStatistic.iKillNum = psi->killNum;
	fightStatistic.ideadNum = psi->deathNum;
	fightStatistic.PlayerID = playerId;
}

void FightingStatistics::setTopPlayers( CountyId countyId, statisticTotalFightInfo &info ) const
{
	// TODO:topPlayername
	PlayerNames playerNames = getTopPlayerNames(countyId);
	if(playerNames.get() == 0)
		return;

	// 排序并设置topplyaername
	std::sort(playerNames.get()->begin(),playerNames.get()->end(), PlayerNameSorting());
	{
		const int PLAYERCOUNTS = 10;
		const int MIN_COUNT = PLAYERCOUNTS < playerNames->size() ? PLAYERCOUNTS : static_cast<int>(playerNames->size());
		PlayerNameStructPtrs::const_iterator it_playername = playerNames->begin();
		for (int i = 0; i < MIN_COUNT; ++i)
		{
			safeStrCpy(info.TopPlayerName[i], SIZE_24, (*it_playername)->second.c_str(), (*it_playername)->second.length());
			++it_playername;
		}
	}
}

void FightingStatistics::setEnemyInfo( CountyId enemyCampId, statisticTotalFightInfo &info ) const
{
	PlayerStatisticsInfo enemyCampInfo = getCampStatisticsInfo(enemyCampId);
	if(enemyCampInfo.get() == 0)
	{
		info.CcatchHeroNum = 0;
		info.CHurtHeroNum = 0;
		info.CLossNum = 0;
		return;
	}
	info.CcatchHeroNum = enemyCampInfo->catchedNum;
	info.CHurtHeroNum = enemyCampInfo->wasInjuredNum;
	info.CLossNum = enemyCampInfo->deathNum;
}

void FightingStatistics::setOwnerInfo( CountyId countyId, statisticTotalFightInfo &info ) const
{
	PlayerStatisticsInfo myCampInfo = getCampStatisticsInfo(countyId);
	if(myCampInfo.get() == 0)
	{
		info.UcatchHeroNum = 0;
		info.UHurtHeroNum = 0;
		info.ULossNum = 0;
		return;
	}
	info.UcatchHeroNum = myCampInfo->catchedNum;
	info.UHurtHeroNum = myCampInfo->wasInjuredNum;
	info.ULossNum = myCampInfo->deathNum;
}

bool FightingStatistics::queryFightingReport(CountyId countyId, PlayerId playerId, statisticTotalFightInfo& info) const
{
	memset(&info, 0, sizeof(info));
	if(campInfos_.empty())
		return false;

	// 敌方阵营ID
	CountyId enemyCampId = getEnemyCampId(countyId);
	if(enemyCampId == 0)
		return false;

	info.winer = winCampId_;

	// 设置FIGHT_STATISTIC
	FIGHT_STATISTIC fightStatistic = {0};
	getFightStatistic(countyId, playerId, fightStatistic, info);
	// 0923 add by zwl
	info.playerStatic = fightStatistic;

	StatisticArmyFightInfoPtrs armyInfos = getStatisticArmyFightInfoPtrs();
	// 数组...
	info.num = armyInfos.size() < 100 ? static_cast<int>(armyInfos.size()) : 100;

	StatisticArmyFightInfoPtrs::const_iterator it = armyInfos.begin();
	for(int i = 0; i < info.num; ++i)
	{
		info.ArmyStatic[i] = *(*it++).get();
	}

	setEnemyInfo(enemyCampId, info);
	setOwnerInfo(countyId, info);
	setTopPlayers(countyId, info);
	return true;
}

FightingStatistics::PlayerNames FightingStatistics::getTopPlayerNames(CountyId countyId) const
{
	if(campInfos_.empty())
		return PlayerNames();

	CampInfoMap::const_iterator it_camp = campInfos_.find(countyId);
	if(it_camp == campInfos_.end())
		return PlayerNames();

	PlayerNames playerNames(new PlayerNameStructPtrs());

	PlayerInfoMap::const_iterator it_player = (*it_camp).second->playerInfos.begin();
	for(; it_player != (*it_camp).second->playerInfos.end(); ++it_player)
	{
		PlayerNameStruct playerNameStruct;
		PlayerStatisticsInfo psi = getPlayerStatisticsInfo((*it_player).second);
		PlayerNameStructPtr ptr(new PlayerNameStruct(psi->killNum, psi->playerName));
		playerNames->push_back(ptr);
	}

	return playerNames;
}

void FightingStatistics::onCaptured(CountyId countyId, PlayerId playerId, WeaponId weaponId, ArmorId armorId, int num)
{

	PlayerInfoPtr player = findElseInsertPlayerInfo(countyId, playerId);
	modifyPlayerWeaponNum(player, weaponId, num);
	modifyPlayerArmorNum(armorId, player, num);
}

// @brief 得到玩家playerId缴获的武器
void FightingStatistics::getCapturedWeapon(CountyId countyId, PlayerWeapons& playersWeapons) const
{
	playersWeapons.clear();

	// 找郡
	CampInfoMap::const_iterator it = campInfos_.find(countyId);
	if(it == campInfos_.end())
	{
		// 未找到
		return;
	}

	// 找玩家
	PlayerInfoMap::const_iterator it_player = (*it).second->playerInfos.begin();
	for (; it_player != (*it).second->playerInfos.end(); ++it_player)
	{
		WeaponMap dest;
		computeWeaponsAccordingtoPecent(dest, (*it_player).second->weaponMap, percent_);
		playersWeapons.insert(PlayerWeapons::value_type((*it_player).first, dest));
	}
}

void FightingStatistics::getWeapons(PlayerWeapons& weapons) const
{
	weapons.clear();

	CampInfoMap::const_iterator it_camp = campInfos_.begin();
	for(; it_camp != campInfos_.end(); ++it_camp)
	{
		PlayerInfoMap::const_iterator it_player = (*it_camp).second->playerInfos.begin();
		for (; it_player != (*it_camp).second->playerInfos.end(); ++it_player)
		{
			WeaponMap dest;
			computeWeaponsAccordingtoPecent(dest, (*it_player).second->weaponMap, percent_);
			weapons.insert(PlayerWeapons::value_type((*it_player).first, dest));
		}
	}
}

void FightingStatistics::getCapturedArmor(CountyId countyId, PlayerArmors& armors) const
{
	armors.clear();

	// 找郡
	CampInfoMap::const_iterator it = campInfos_.find(countyId);
	if(it == campInfos_.end())
	{
		// 未找到
		return;
	}

	// 找玩家
	PlayerInfoMap::const_iterator it_player = (*it).second->playerInfos.begin();
	for (; it_player != (*it).second->playerInfos.end(); ++it_player)
	{
		ArmorMap dest;
		computeArmorAccordingtoPercent(dest, (*it_player).second->armorMap, percent_);
		armors.insert(PlayerArmors::value_type((*it_player).first, dest));
	}
}

void FightingStatistics::getArmors(PlayerArmors& armors) const
{
	armors.clear();

	CampInfoMap::const_iterator it_camp = campInfos_.begin();
	for(; it_camp != campInfos_.end(); ++it_camp)
	{
		PlayerInfoMap::const_iterator it_player = (*it_camp).second->playerInfos.begin();
		for (; it_player != (*it_camp).second->playerInfos.end(); ++it_player)
		{
			ArmorMap dest;
			computeArmorAccordingtoPercent(dest, (*it_player).second->armorMap, percent_);
			armors.insert(PlayerArmors::value_type((*it_player).first, dest));
		}
	}
}


void FightingStatistics::onObtainBox(CountyId countyId, PlayerId playerId, int num)
{
	PlayerInfoPtr player = findElseInsertPlayerInfo(countyId, playerId);
	player->boxNum += num;
}

void FightingStatistics::modifyPlayerArmorNum( ArmorId armorId, PlayerInfoPtr player, int num )
{
	// 如果无防具
	if(armorId == 0)
		return;

	// 防具
	ArmorMap::iterator it_armor = player->armorMap.find(armorId);
	if(it_armor == player->armorMap.end())
	{
		player->armorMap.insert(ArmorMap::value_type(armorId, num));
		return;
	}

	(*it_armor).second += num;
}

void FightingStatistics::modifyPlayerWeaponNum( PlayerInfoPtr player, WeaponId weaponId, int num )
{
	// 武器
	WeaponMap::iterator it = player->weaponMap.find(weaponId);
	if(it == player->weaponMap.end())
	{
		// 未找到
		player->weaponMap.insert(WeaponMap::value_type(weaponId, num));
		return;
	}

	(*it).second += num;
}

void FightingStatistics::onAddSoldiers(CountyId countyId, PlayerId playerId, ArmyId armyId, unsigned int soldierNum)
{
	ArmyInfoPtr armyInfo = findElseInsertArmyInfo(countyId, playerId, armyId);
	if(armyInfo.get() == 0)
		return;

	armyInfo->soldierNum = soldierNum;
}


// TODO:封装本类IArmy
// ...

STATISTICSSYSTEM_NAMESPACE_END
