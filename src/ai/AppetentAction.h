// @brief 简要描述: 本能AI实现
// @author 作者: 黄 文 才

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

	// 函数描述：执行命令
	// @param enemyID 敌军部队ID
	// @param moveTagetPos 移动目标点
	// @param movePosVector 剩下需要走的点的容器
	// @param armyCurrentStatus 策略类型
	// @param isCountermarch 是否后退
	// @param isPatrol 是否执行巡逻
	// @param isChurchmanRunaway 牧师被打后是否逃跑
	// @return 返回true / false ； true: 能够走  false：不能走
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
	// 函数描述：下一点坐标能否走
	// @param nextPos 下一个要走的坐标
	// @param enemyPtr 返回是否遇到敌军
	// @return 返回true / false ； true: 能够走  false：不能走
	bool isCanGo(const PosXYStru &nextPos, managerParam & mgrParam, IArmyPtr &enemyPtr, TrapStruct &trapStruct, SOLDIER_TYPE soldierType, int moveSpeed, bool isUseInDynSeek = false);

	// 函数描述：是否追击敌军
	// @param enemyPtr 敌军
	// @return 返回true / false ； true: 追击  false：不追击
	bool isNeedSickEnemy(IArmy * enemyPtr);

	// 函数描述：是否追击敌军
	// @param  armyPtr 当前操作的部队
	// @param enemyPtr 敌军
	// @param movePosVector 需要走的点的容器
	// @return 返回true / false ； true: 追击  false：不追击
	//bool GetGoPathPosVector(const IArmyPtr armyPtr, const IArmyPtr enemyPtr, Vect_Pos &movePosVector);

	bool enemyIsInAttackScope(const std::vector<POS_STRUCT> &armyPosVect, const std::vector<POS_STRUCT> &enemyPosVect, int attackScope) const ;

	// 函数描述：动态寻路
	// @param posTarget 目的地坐标
	// @param movePosVector 动态寻路的结果
	// @return 返回true / false
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




