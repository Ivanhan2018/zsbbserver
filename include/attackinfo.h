// @brief ���˺���������Ľṹ����������ǰ�Ľṹ���� 
// @author ������

#ifndef FIGHTSYSTEM_ATTACKINFO_H_
#define FIGHTSYSTEM_ATTACKINFO_H_
#include <vector>
#include "boost/shared_ptr.hpp"
#pragma warning(push)
#pragma warning(disable:4100)
#include "ProFightSystem.h"
#pragma warning(pop)

typedef HarmParm HarmInfoValue;
typedef boost::shared_ptr<HarmInfoValue> HarmInfo;
typedef std::vector<HarmInfo> HarmInfos;

// ������Ϣ
struct AttackInfoValue
{
	unsigned long armyId;
	POS_STRUCT attackerPos;
	POS_STRUCT targetPos;
	ARMY_TYPE armyType;
	unsigned int weaponId;
	// ��������
	ATTACK_TYPE attackType; 
	// �������ͷŵļ���
	SKILL_TYPE skillId;

	HarmInfos harmInfos;
};

typedef boost::shared_ptr<AttackInfoValue> AttackInfo;

#endif // FIGHTSYSTEM_ATTACKINFO_H_