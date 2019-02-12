// @brief ս��ͳ��
// @author ������

#ifndef FIGHTINGSTATISTICS_BATTLEFIELDSTATISTICS_H_
#define FIGHTINGSTATISTICS_BATTLEFIELDSTATISTICS_H_
#include <set>
#include <boost/scoped_ptr.hpp>
#include "namespacedef.h"
#include "iddefs.h"
#include "statisticscommon.h"
#include "Global_Macro.h"

STATISTICSSYSTEM_NAMESPACE_BEGIN



struct AttackSide;
struct DefendingSide;
// ս��ͳ��
class BattlefieldStatistics
{
public:
	BattlefieldStatistics();
	~BattlefieldStatistics();

	// ս����ʼ
	void onStart(
		CountyId countId,			// ���ط���ӪID
		CountyId attackSideId,		// ��������ӪID
		MapId mapId,				// ��ͼID
		FightingStartInfo& info		// ����Ҫ���͵��ͻ��˵���Ϣ
		);				

	// @brief ����ӪcampId�����playerId��ʿ������ʱ����
	// @param campId ��ӪID
	// @param playerId ���ID
	// @param deathNum ������
	void onSoldierDie(CountyId campId, PlayerId playerId, unsigned int deathNum);

	// @brief ����ӪcampId�����playerIdɱ����ʿ��ʱ����
	// @param campId ��ӪID
	// @param playerId ���ID
	// @param killedNum ɱ����
	void onKilledEnemy(CountyId campId, PlayerId playerId, unsigned int killedNum);

	// @brief ����ӪcampId�����playerId��һ��Ӣ�۱���ʱ����
	// @param campId ��ӪID
	// @param playerId ���ID
	// @param heroId Ӣ��ID
	void onHeroBeCatched(CountyId campId, PlayerId playerId, HeroId heroId);

	// @brief ����ӪcampId�����playerId��һ��Ӣ��heroId������ʱ����
	// @param campId ��ӪID
	// @param playerId ���ID
	// @param heroId Ӣ��ID
	void onHeroBeHurt(CountyId campId, PlayerId playerId, HeroId heroId);

	// @brief ��ID��ΪcampId����Ӫ�ĳ����������ݻ�ʱ
	// @param campId ��ӪID
	// @param buildingType ����������
	void onBuildingDestroyed(CountyId defenseSideId, ARCH_TYPES buildingType);

	// @brief ս������ʱ����
	// @param winCampId ʤ������ӪID
	// @param info ����ս����Ϣ
	void onOver(CountyId winCampId, BattlefieldStatisticsInfo& info);

protected:
	// �Ƿ��ǹ�����
	bool isAttackSide(CountyId campId) const;

private:
	friend class TestBattlefieldStatistics;
	BattlefieldStatistics(const BattlefieldStatistics&);
	BattlefieldStatistics& operator=(const BattlefieldStatistics&);
	// ս����ʼʱ��
	time_t startTime_;
	// ս������ʱ��
	time_t overTime_;
	// ���ط���ӪID
	CountyId campId_;

	CountyId attackSideId_;
	// ������
	boost::scoped_ptr<AttackSide> attackSide_;
	// ���ط�
	boost::scoped_ptr<DefendingSide> defendingSide_;

	MapId mapid_;
};
STATISTICSSYSTEM_NAMESPACE_END

#endif // FIGHTINGSTATISTICS_BATTLEFIELDSTATISTICS_H_
