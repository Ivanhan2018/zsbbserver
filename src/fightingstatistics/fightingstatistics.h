// $Id: fightingstatistics.h 1 2009-12-08 18:59:42 Zhangwenlong $ 
// @brief 战斗统计类，负责各种战斗的统计，暂时只适配原有的系统
// @author 张文龙

#ifndef FIGHTINGSTATISTICS_FIGHTINGSTATISTICS_H_
#define FIGHTINGSTATISTICS_FIGHTINGSTATISTICS_H_
#include <map>
#include <ctime>
#include <boost/shared_ptr.hpp>

#include "iddefs.h"
#include "namespacedef.h"
#pragma warning(push)
#pragma warning(disable:4100)
#include "ProFightSystem.h"
#include "Server_Protocol.h"
#pragma warning(pop)


STATISTICSSYSTEM_NAMESPACE_BEGIN

// 副本统计
class FightingStatistics
{
public:
	explicit FightingStatistics(float percent);
	~FightingStatistics();

	// @brief 当战斗开始时调用
 	void onStart();

	// @brief 当阵营campid的玩家playerid的部队armyid加入战斗时
	// @param campId 阵营ID
	// @param playerId 玩家ID
	// @param armyId 部队ID
	// @param soldierNum 该部队士兵人数
	// @param type 部队士兵类型
	void onEnterWar(CountyId countyId, 
		PlayerId playerId, 
		ArmyId armyId, 
		unsigned int soldierNum, 
		SOLDIER_TYPE type, 
		const std::string& armyName,
		const std::string& heroName,
		int heroFaceId,
		const std::string& playerName);

	// @brief 当阵营campId的玩家palyerId抓获英雄heroId
	// @param campId 阵营ID
	// @param playerId 玩家ID
	// @param heroId 英雄ID
	void onCatchedHero(CountyId countyId, PlayerId playerId, ArmyId armyId, HeroId heroId);

	// @brief 当阵营campId的玩家palyerId的英雄heroId被抓获
	// @param campId 阵营ID
	// @param playerId 玩家ID
	// @param heroId 英雄ID
	void onBeCatchedHero(CountyId countyId, PlayerId playerId, ArmyId armyId, HeroId heroId);

	// @brief 当阵营campId的玩家palyerId的英雄heroId受重伤
	// @param campId 阵营ID
	// @param playerId 玩家ID
	// @param heroId 英雄ID
	void onHeroWasInjured(CountyId countyId, PlayerId playerId, ArmyId armyId, HeroId heroId);

	// @brief 当阵营campid的玩家playerid的部队armyid死亡了deathNum个士兵时
	// @param campId 阵营ID
	// @param playerId 玩家ID
	// @param armyId 部队ID
	// @param deathNum 该部队死亡数量
	// @param soldiersNum 士兵数
	void onSoliderDie(CountyId countyId, PlayerId playerId, ArmyId armyId, unsigned int deathNum, unsigned int soldierNum);


	// @brief 当使用补兵卡后调用
	void onAddSoldiers(CountyId countyId, PlayerId playerId, ArmyId armyId, unsigned int soldierNum);

	// @brief 当阵营campid的玩家playerid的部队armyid杀死了短文killNum个士兵时
	// @param campId 阵营ID
	// @param playerId 玩家ID
	// @param armyId 部队ID
	// @param killNum 该部队杀敌数量
	void onKilledEnemy(CountyId countyId, PlayerId playerId, ArmyId armyId, unsigned int killNum);

	// @brief 当战争结束时，活着的部队刷新经验
	// @param campId 阵营ID
	// @param playerId 玩家ID
	// @param armyId 部队ID
	// @param exp 部队获得的经验
	void onArmyFlashExp(CountyId countyId, PlayerId playerId, ArmyId armyId, unsigned int exp);

	// @brief 当部队死亡时
	// @param campId 阵营ID
	// @param playerId 玩家ID
	// @param armyId 部队ID
	// @param exp 部队获得的经验
	void onArmyDie(CountyId countyId, PlayerId playerId, ArmyId armyId, unsigned int exp);

	// @brief 根据部队ID查询部队信息
	// @param param 返回部队信息到tagHeroParam结构
	// @return 查询成功返回true, 否则返回false
	bool queryArmyInfo(CountyId countyId, PlayerId playerId, ArmyId armyId, tagHeroParam& param) const;

	// @brief 战斗结束时调用
	// @param winCampId 胜利方阵营ID
	void onOver(CountyId winCampId);

	// @brief 以玩家ID来查询战斗结果，并存放到info里
	bool queryFightingReport(CountyId countyId, PlayerId playerId, statisticTotalFightInfo& info) const;

	//////////////////////////////////////////////////////////////////////////
	/// 新加入的

	// @brief 当playerid缴获了num个weapon
	void onCaptured(CountyId countyId, PlayerId playerId, WeaponId weaponId, ArmorId armorId, int num);


	// @brief 获得的宝箱数
	void onObtainBox(CountyId countyId, PlayerId playerId, int num);

	// @brief 得到玩家playerId缴获的武器
	void getCapturedWeapon(CountyId countyId, PlayerWeapons& weapons) const;

	// @brief 得到所有玩家缴获的武器
	void getWeapons(PlayerWeapons& weapons) const;


	void getCapturedArmor(CountyId countyId, PlayerArmors& armors) const;
	void getArmors(PlayerArmors& armors) const;

protected:
	// 部队状态
	enum ArmyStatus
	{
		// 活着
		ARMYSTATUS_ALIVE,
		// 死亡
		ARMYSTATUS_DIE
	};

	struct ArmyInfo
	{
		ArmyInfo()
			:startTime(0)
			,soldierNum(0)
			,deathNum(0)
			,killNum(0)
			,exp(0)
			,status(ARMYSTATUS_ALIVE)
			,type(SOLDIERTYPE_NONE)
			,herosBeCatched()
			,herosCatched()
			,heroWasInjured()
			,name("")
			,heroName("")
			,heroFaceId(0)
			,playerName("")
		{
			time(&startTime);
		}
		// 部队开始战斗时间
		time_t startTime;
		// 士兵人数
		unsigned int soldierNum;
		// 死亡人数
		unsigned int deathNum;
		// 杀敌人数
		unsigned int killNum;
		// 部队获得的经验值
		unsigned int exp;
		// 部队状态
		ArmyStatus status;
		// 士兵类型
		SOLDIER_TYPE type;
		// @note 以下本来应该放到playerInfo里，
		// 由于以前的结构要统计这个是以部队为单位，
		// 故放到这里以部队为单位来统计
		// 被捕获的英雄集
		HeroIdSet herosBeCatched;
		// 捕获的英雄集
		HeroIdSet herosCatched;
		// 受重伤的英雄集
		HeroIdSet heroWasInjured;
		// 部队名称
		std::string name;
		// 英雄名称
		std::string heroName;
		// 英雄头像ID
		int heroFaceId;
		// 玩家名字
		std::string playerName;
	};

	typedef boost::shared_ptr<ArmyInfo> ArmyInfoPtr;
	typedef std::map<ArmyId, ArmyInfoPtr> ArmyInfoMap;


	struct PlayerInfo
	{
		PlayerInfo()
			:startTime(0)
			,armyInfos()
			,weaponMap()
			,boxNum(0)
		{
			time(&startTime);
		}
		// 玩家战斗开始时间
		time_t startTime;
		// 玩家所有部队的统计信息
		ArmyInfoMap armyInfos;
		// 缴获的武器集
		WeaponMap weaponMap;

		// 缴获的防具
		ArmorMap armorMap;
		// 得到的宝箱数
		int boxNum;
	};

	typedef boost::shared_ptr<PlayerInfo> PlayerInfoPtr;
	typedef std::map<PlayerId, PlayerInfoPtr> PlayerInfoMap;

	struct CampInfo
	{
		CampInfo():playerInfos(){}
		PlayerInfoMap playerInfos;
	};

	typedef boost::shared_ptr<CampInfo> CampInfoPtr;
	typedef std::map<CountyId, CampInfoPtr> CampInfoMap;

	typedef boost::shared_ptr<StatisticArmyFightInfo> StatisticArmyFightInfoPtr;
	typedef std::vector<StatisticArmyFightInfoPtr> StatisticArmyFightInfoPtrs;

	struct PlayerStatisticsInfoValue
	{
		int catchedNum;
		int beCatchedNum;
		int wasInjuredNum;
		int killNum;
		int deathNum;
		std::string playerName;
	};
	typedef boost::shared_ptr<PlayerStatisticsInfoValue> PlayerStatisticsInfo;

	typedef std::pair<int, std::string> PlayerNameStruct;
	typedef boost::shared_ptr<PlayerNameStruct> PlayerNameStructPtr;
	typedef std::vector<PlayerNameStructPtr> PlayerNameStructPtrs;
	typedef boost::shared_ptr<PlayerNameStructPtrs> PlayerNames;

	class PlayerNameSorting
	{
	public:
		bool operator()(const PlayerNameStructPtr& p1, const PlayerNameStructPtr& p2) const
		{
			return p1->first > p2->first;
		}
	};

protected:
	PlayerInfoPtr findElseInsertPlayerInfo(CountyId campId, PlayerId playerId);
	ArmyInfoPtr findElseInsertArmyInfo(CountyId campId, PlayerId playerId, ArmyId armyId);
	const PlayerInfoPtr getPlayerInfo(CountyId campId, PlayerId playerId) const;
	PlayerStatisticsInfo getPlayerStatisticsInfo(const PlayerInfoPtr& playerInfo) const;
	PlayerStatisticsInfo getCampStatisticsInfo(CountyId campId) const;
	StatisticArmyFightInfoPtrs getStatisticArmyFightInfoPtrs() const;

	void getFightStatistic( CountyId countyId, PlayerId playerId, 
		FIGHT_STATISTIC &fightStatistic, 
		statisticTotalFightInfo& info  )const;
	void setOwnerInfo( CountyId countyId, statisticTotalFightInfo &info ) const;
	void setEnemyInfo( CountyId enemyCampId, statisticTotalFightInfo &info ) const;
	void setTopPlayers( CountyId countyId, statisticTotalFightInfo &info ) const;

	// 得到敌对方的阵营ID
	CountyId getEnemyCampId(CountyId campId) const;
	// 根据杀死敌人数量排序，得到玩家列表
	PlayerNames getTopPlayerNames(CountyId campId) const;

	void modifyPlayerArmorNum( ArmorId armorId, PlayerInfoPtr player, int num );
	void modifyPlayerWeaponNum( PlayerInfoPtr player, WeaponId weaponId, int num );
private:
	friend class TestFightingStatistics;
	// 战斗开始时间
	time_t startTime_;
	time_t overTime_;
	// 所有阵营统计信息
	CampInfoMap campInfos_;
	// 获胜阵营ID
	CountyId winCampId_;

	// 折损率
	float percent_;
};

STATISTICSSYSTEM_NAMESPACE_END
#endif // FIGHTINGSTATISTICS_FIGHTINGSTATISTICS_H_
