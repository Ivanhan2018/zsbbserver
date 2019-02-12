// @brief ��Ҫ����: ����AIʵ��
// @author ����: �� �� ��

#ifndef _APPETENTACTION_H_H_H
#define _APPETENTACTION_H_H_H

#include "SeekPathArithm.h"

struct managerParam; 

class AppetentAction
{
public:
	enum
	{
		PATROL_ARMY_MOVE_PACE = 1
	};

	AppetentAction(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathParamPtr,  IArmy *  armyPtrParam, CELL_HAS_ARMY_MAX cellHasArmyMax);
public:
	virtual ~AppetentAction(void);

	// ����������ִ������
	// @param enemyID �о�����ID
	// @param moveTagetPos �ƶ�Ŀ���
	// @param movePosVector ʣ����Ҫ�ߵĵ������
	// @param armyCurrentStatus ��������
	// @param isCountermarch �Ƿ����
	// @param isPatrol �Ƿ�ִ��Ѳ��
	// @param isChurchmanRunaway ��ʦ������Ƿ�����
	// @return ����true / false �� true: �ܹ���  false��������
	bool action(bool isEven,
				unsigned int &enemyID,
				vector<PosXYStru> &movePosVector,
				ENUM_ARMY_STATUS &armyCurrentStatus,
				SOLDIER_TYPE tipTopAttackTar = PRIEST, 
				int isDynamicSeekPath = false,
				bool isCountermarch = false,
				bool isPatrol = false,
				bool isChurchmanRunaway = false);

	void attEnemyInAttScope(IArmyPtr enemyPtr, IArmyPtr selfarmyPtr, bool isUseSelfBlast, bool isForceBlast = false) const;

private:
	struct TrapStruct
	{
		TrapStruct() : isInEnemyTrap(false), trapArmyPtr(NULL)
		{
		}

		bool isInEnemyTrap;
		IArmyPtr trapArmyPtr;
	};
	
	AppetentAction &operator=(const AppetentAction &);
	AppetentAction(const AppetentAction &);

	AppetentAction();
	// ������������һ�������ܷ���
	// @param nextPos ��һ��Ҫ�ߵ�����
	// @param enemyPtr �����Ƿ������о�
	// @return ����true / false �� true: �ܹ���  false��������
	bool isCanGo(const PosXYStru &nextPos, managerParam & mgrParam, IArmyPtr &enemyPtr, TrapStruct &trapStruct, SOLDIER_TYPE soldierType, int moveSpeed, bool isUseInDynSeek = false);

	// �����������Ƿ�׷���о�
	// @param enemyPtr �о�
	// @return ����true / false �� true: ׷��  false����׷��
	bool isNeedSickEnemy(IArmy * enemyPtr);

	// �����������Ƿ�׷���о�
	// @param  armyPtr ��ǰ�����Ĳ���
	// @param enemyPtr �о�
	// @param movePosVector ��Ҫ�ߵĵ������
	// @return ����true / false �� true: ׷��  false����׷��
	//bool GetGoPathPosVector(const IArmyPtr armyPtr, const IArmyPtr enemyPtr, Vect_Pos &movePosVector);

	bool enemyIsInAttackScope(const std::vector<POS_STRUCT> &armyPosVect, const std::vector<POS_STRUCT> &enemyPosVect, int attackScope) const ;

	// ������������̬Ѱ·
	// @param posTarget Ŀ�ĵ�����
	// @param movePosVector ��̬Ѱ·�Ľ��
	// @return ����true / false
	bool GetPathInDynamicSeek(PosXYStru &posTarget, vector<PosXYStru> & movePosVector, unsigned int moveSpeed);

	void isNeedDynSeekAndGetPath(vector<PosXYStru> &movePosVector, 
									managerParam & mgrParam,
									SOLDIER_TYPE soldierType,
									IArmyPtr enemyPtrTmp, 
									int movePosVectorSizeTmp, 
									bool &isNotWay, 
									int &movePosVectorSize, 
									int &iMin, 
									int moveSpeed, 
									ENUM_ARMY_STATUS armyCurrentStatus);

	IArmyPtrvectInvect * mapDataPtr;
	SeekPath * seekPathPtr;
	IArmy *  armyPtr;

	unsigned int increaseMoveTime_;

	CELL_HAS_ARMY_MAX cellHasArmyMax_;
};

#endif




