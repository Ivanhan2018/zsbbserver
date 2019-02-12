#include "battlefieldstatisticsadapter.h"
#include "battlefieldstatistics.h"
#include "IArmy.h"

STATISTICSSYSTEM_NAMESPACE_BEGIN

BattlefieldStatisticsAdapter::BattlefieldStatisticsAdapter():
battleFieldStatistics_(new BattlefieldStatistics)
{

}

BattlefieldStatisticsAdapter::~BattlefieldStatisticsAdapter()
{

}

// ս����ʼ
void BattlefieldStatisticsAdapter::onStart(
			 CountyId defenseSideId,		// ���ط���ӪID
			 CountyId attackSideId,			// ��������ӪID
			 MapId mapId,					// ��ͼID
			 FightingStartInfo& info		// ����Ҫ���͵��ͻ��˵���Ϣ
			 )
{
	battleFieldStatistics_->onStart(defenseSideId, attackSideId, mapId, info);
}

void BattlefieldStatisticsAdapter::reset()
{
	battleFieldStatistics_.reset(new BattlefieldStatistics);
}

// @brief ����ӪcampId�����playerId��һ��ʿ������ʱ����
// @param campId ��ӪID
// @param playerId ���ID
void BattlefieldStatisticsAdapter::onSoldierDie(const IArmy* army, unsigned int deathNum)
{
	if(army == 0)
		return;
	battleFieldStatistics_->onSoldierDie(army->GetCountryID(), 
		army->getPlayerID(), deathNum);
}


void BattlefieldStatisticsAdapter::onKilledEnemy(const IArmy* army, unsigned int killedNum)
{
	if(army == 0)
		return;
	battleFieldStatistics_->onKilledEnemy(army->GetCountryID(), 
		army->getPlayerID(), killedNum);
}


// @brief ����ӪcampId�����playerId��һ��Ӣ�۱���ʱ����
// @param campId ��ӪID
// @param playerId ���ID
// @param heroId Ӣ��ID
void BattlefieldStatisticsAdapter::onHeroBeCatched(const IArmy* army)
{
	if(army == 0)
		return;

	battleFieldStatistics_->onHeroBeCatched(army->GetCountryID(),
		army->getPlayerID(),
		army->GetOfficerID());
}

// @brief ����ӪcampId�����playerId��һ��Ӣ��heroId������ʱ����
// @param campId ��ӪID
// @param playerId ���ID
// @param heroId Ӣ��ID
void BattlefieldStatisticsAdapter::onHeroBeHurt(const IArmy* army)
{
	if(army == 0)
		return;

	battleFieldStatistics_->onHeroBeHurt(army->GetCountryID(),
		army->getPlayerID(),
		army->GetOfficerID());
}

// @brief ��ID��ΪcampId����Ӫ�ĳ����������ݻ�ʱ
// @param campId ��ӪID
// @param buildingType ����������
void BattlefieldStatisticsAdapter::onBuildingDestroyed(CountyId defenseSideId, ARCH_TYPES buildingType)
{
	battleFieldStatistics_->onBuildingDestroyed(defenseSideId, buildingType);
}

// @brief ս������ʱ����
// @param winCampId ʤ������ӪID
// @param info ����ս����Ϣ
void BattlefieldStatisticsAdapter::onOver(CountyId winCampId, BattlefieldStatisticsInfo& info)
{
	battleFieldStatistics_->onOver(winCampId, info);
}
STATISTICSSYSTEM_NAMESPACE_END
