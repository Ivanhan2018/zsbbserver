// @brief ��Ҫ����: Ѳ�߹��ܹ����ӿں��� 
// @author ����: �� �� ��

#ifndef _PATROL_FUN_SINGLTON_HH
#define _PATROL_FUN_SINGLTON_HH

#include "AiCommon.h"
#include "SeekPathArithm.h"
#include "IManager.h"

class PatrolFunSingleton
{
public:
	// ������������ȡͨ�ú����൥�������ָ��
	// @return ����
	static PatrolFunSingleton * getInstance();

	// ����������ִ��Ѳ�߹���
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @param strategyMgr ���Թ�����
	// @return ����
	bool executePatrol(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *&strategyMgr, bool isEven);

private:
	PatrolFunSingleton();

	static PatrolFunSingleton * comFunInstance;  // Ωһʵ��

	class CGarbo // ����Ψһ��������������������ɾ������
	{
	public:
		~CGarbo()
		{
			if (NULL != PatrolFunSingleton::comFunInstance)
			{
				delete PatrolFunSingleton::comFunInstance;
				PatrolFunSingleton::comFunInstance = NULL;
			}
		}
	};

	static CGarbo Garbo; // ����һ����̬��Ա���ڳ������ʱ��ϵͳ�����������������
};


#endif

