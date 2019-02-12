// $Id: fightingstatistics.h 1 2009-12-08 18:59:42 Zhangwenlong $ 
// @brief ս��ͳ���࣬�������ս����ͳ�ƣ���ʱֻ����ԭ�е�ϵͳ
// @author ������

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

// ����ͳ��
class FightingStatistics
{
public:
	explicit FightingStatistics(float percent);
	~FightingStatistics();

	// @brief ��ս����ʼʱ����
 	void onStart();

	// @brief ����Ӫcampid�����playerid�Ĳ���armyid����ս��ʱ
	// @param campId ��ӪID
	// @param playerId ���ID
	// @param armyId ����ID
	// @param soldierNum �ò���ʿ������
	// @param type ����ʿ������
	void onEnterWar(CountyId countyId, 
		PlayerId playerId, 
		ArmyId armyId, 
		unsigned int soldierNum, 
		SOLDIER_TYPE type, 
		const std::string& armyName,
		const std::string& heroName,
		int heroFaceId,
		const std::string& playerName);

	// @brief ����ӪcampId�����palyerIdץ��Ӣ��heroId
	// @param campId ��ӪID
	// @param playerId ���ID
	// @param heroId Ӣ��ID
	void onCatchedHero(CountyId countyId, PlayerId playerId, ArmyId armyId, HeroId heroId);

	// @brief ����ӪcampId�����palyerId��Ӣ��heroId��ץ��
	// @param campId ��ӪID
	// @param playerId ���ID
	// @param heroId Ӣ��ID
	void onBeCatchedHero(CountyId countyId, PlayerId playerId, ArmyId armyId, HeroId heroId);

	// @brief ����ӪcampId�����palyerId��Ӣ��heroId������
	// @param campId ��ӪID
	// @param playerId ���ID
	// @param heroId Ӣ��ID
	void onHeroWasInjured(CountyId countyId, PlayerId playerId, ArmyId armyId, HeroId heroId);

	// @brief ����Ӫcampid�����playerid�Ĳ���armyid������deathNum��ʿ��ʱ
	// @param campId ��ӪID
	// @param playerId ���ID
	// @param armyId ����ID
	// @param deathNum �ò�����������
	// @param soldiersNum ʿ����
	void onSoliderDie(CountyId countyId, PlayerId playerId, ArmyId armyId, unsigned int deathNum, unsigned int soldierNum);


	// @brief ��ʹ�ò����������
	void onAddSoldiers(CountyId countyId, PlayerId playerId, ArmyId armyId, unsigned int soldierNum);

	// @brief ����Ӫcampid�����playerid�Ĳ���armyidɱ���˶���killNum��ʿ��ʱ
	// @param campId ��ӪID
	// @param playerId ���ID
	// @param armyId ����ID
	// @param killNum �ò���ɱ������
	void onKilledEnemy(CountyId countyId, PlayerId playerId, ArmyId armyId, unsigned int killNum);

	// @brief ��ս������ʱ�����ŵĲ���ˢ�¾���
	// @param campId ��ӪID
	// @param playerId ���ID
	// @param armyId ����ID
	// @param exp ���ӻ�õľ���
	void onArmyFlashExp(CountyId countyId, PlayerId playerId, ArmyId armyId, unsigned int exp);

	// @brief ����������ʱ
	// @param campId ��ӪID
	// @param playerId ���ID
	// @param armyId ����ID
	// @param exp ���ӻ�õľ���
	void onArmyDie(CountyId countyId, PlayerId playerId, ArmyId armyId, unsigned int exp);

	// @brief ���ݲ���ID��ѯ������Ϣ
	// @param param ���ز�����Ϣ��tagHeroParam�ṹ
	// @return ��ѯ�ɹ�����true, ���򷵻�false
	bool queryArmyInfo(CountyId countyId, PlayerId playerId, ArmyId armyId, tagHeroParam& param) const;

	// @brief ս������ʱ����
	// @param winCampId ʤ������ӪID
	void onOver(CountyId winCampId);

	// @brief �����ID����ѯս�����������ŵ�info��
	bool queryFightingReport(CountyId countyId, PlayerId playerId, statisticTotalFightInfo& info) const;

	//////////////////////////////////////////////////////////////////////////
	/// �¼����

	// @brief ��playerid�ɻ���num��weapon
	void onCaptured(CountyId countyId, PlayerId playerId, WeaponId weaponId, ArmorId armorId, int num);


	// @brief ��õı�����
	void onObtainBox(CountyId countyId, PlayerId playerId, int num);

	// @brief �õ����playerId�ɻ������
	void getCapturedWeapon(CountyId countyId, PlayerWeapons& weapons) const;

	// @brief �õ�������ҽɻ������
	void getWeapons(PlayerWeapons& weapons) const;


	void getCapturedArmor(CountyId countyId, PlayerArmors& armors) const;
	void getArmors(PlayerArmors& armors) const;

protected:
	// ����״̬
	enum ArmyStatus
	{
		// ����
		ARMYSTATUS_ALIVE,
		// ����
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
		// ���ӿ�ʼս��ʱ��
		time_t startTime;
		// ʿ������
		unsigned int soldierNum;
		// ��������
		unsigned int deathNum;
		// ɱ������
		unsigned int killNum;
		// ���ӻ�õľ���ֵ
		unsigned int exp;
		// ����״̬
		ArmyStatus status;
		// ʿ������
		SOLDIER_TYPE type;
		// @note ���±���Ӧ�÷ŵ�playerInfo�
		// ������ǰ�ĽṹҪͳ��������Բ���Ϊ��λ��
		// �ʷŵ������Բ���Ϊ��λ��ͳ��
		// �������Ӣ�ۼ�
		HeroIdSet herosBeCatched;
		// �����Ӣ�ۼ�
		HeroIdSet herosCatched;
		// �����˵�Ӣ�ۼ�
		HeroIdSet heroWasInjured;
		// ��������
		std::string name;
		// Ӣ������
		std::string heroName;
		// Ӣ��ͷ��ID
		int heroFaceId;
		// �������
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
		// ���ս����ʼʱ��
		time_t startTime;
		// ������в��ӵ�ͳ����Ϣ
		ArmyInfoMap armyInfos;
		// �ɻ��������
		WeaponMap weaponMap;

		// �ɻ�ķ���
		ArmorMap armorMap;
		// �õ��ı�����
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

	// �õ��жԷ�����ӪID
	CountyId getEnemyCampId(CountyId campId) const;
	// ����ɱ�������������򣬵õ�����б�
	PlayerNames getTopPlayerNames(CountyId campId) const;

	void modifyPlayerArmorNum( ArmorId armorId, PlayerInfoPtr player, int num );
	void modifyPlayerWeaponNum( PlayerInfoPtr player, WeaponId weaponId, int num );
private:
	friend class TestFightingStatistics;
	// ս����ʼʱ��
	time_t startTime_;
	time_t overTime_;
	// ������Ӫͳ����Ϣ
	CampInfoMap campInfos_;
	// ��ʤ��ӪID
	CountyId winCampId_;

	// ������
	float percent_;
};

STATISTICSSYSTEM_NAMESPACE_END
#endif // FIGHTINGSTATISTICS_FIGHTINGSTATISTICS_H_
