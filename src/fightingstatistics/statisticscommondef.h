// @brief ս��ͳ��Ҫʹ�õ����͵Ķ���
// @author ������

#ifndef FIGHTINGSTATISTICS_STATISTICSCOMMONDEF_H_
#define FIGHTINGSTATISTICS_STATISTICSCOMMONDEF_H_
#include <hash_map>
#include <boost/shared_ptr.hpp>

STATISTICSSYSTEM_NAMESPACE_BEGIN

// ���ͳ����Ϣ
struct PlayerStatistics
{
	PlayerStatistics():killNum(0),soldierDeathsNum(0),herosBeCatched(),herosBeHurt(){}

	unsigned int killNum;
	// ʿ����������
	unsigned int soldierDeathsNum;
	// ����Ӣ����
	HeroIdSet herosBeCatched;
	// ����Ӣ����
	HeroIdSet herosBeHurt;
};

typedef boost::shared_ptr<PlayerStatistics> PlayerStatisticsPtr;
typedef stdext::hash_map<PlayerId, PlayerStatisticsPtr > PlayerStatisticsMap;

// ������
struct AttackSide
{
	// ����������ҵ�ͳ��
	PlayerStatisticsMap playerMap;
};

// ���ط�
struct DefendingSide
{
	DefendingSide():townDeathsNum(0),archeryTowerDeathsNum(0),playerMap()
	{

	}
	// ���򱻴ݻ���
	unsigned int townDeathsNum;
	// �������ݻ���
	unsigned int archeryTowerDeathsNum;
	// ����������ҵ�ͳ��
	PlayerStatisticsMap playerMap;
};


STATISTICSSYSTEM_NAMESPACE_END

#endif // FIGHTINGSTATISTICS_STATISTICSCOMMONDEF_H_
