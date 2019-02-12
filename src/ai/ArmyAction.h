// @brief 简要描述: 对外接口类，实现AI功能
// @author 作者: 黄 文 才

#ifndef _ARMACTION_SINGLETON_H_H
#define _ARMACTION_SINGLETON_H_H

#include "AiCommon.h"
#include "SeekPathArithm.h"
#include "FactoryCreateProfessionMger.h"
#include "AppetentAction.h"

enum
{
	ONE_CELL_VAL = 1,
};

class ArmyAction
{
public:
	ArmyAction(MapDataStru & mapDataAndFraiseParam, IArmyPtr armyPtrObj, ENUM_USE_SCENE enumUseScene, AiLevelParam *aiLevelParamTmp, SeekPath * seekPathArithmPtrParam,
			   const LandformInfoMap *landformInfoMapPtrParam);
	~ArmyAction();

	// 函数描述：获取部队在非托管下的状态
	// @return 返回Attack_status，Move_status，Idle_status
	inline ENUM_ARMY_STATUS getArmStatus() const
	{
		return armyCurrentStatus;
	}

	// 函数描述：获取部队在托管下的状态
	// @return 返回Attack_status，Move_status，Idle_status
	inline ENUM_ARMY_STATUS getArmyStatusOnTrusteeship() const
	{
		if (strategyManagerPtr)
		{
			return strategyManagerPtr->getMgrParam().armyCurrentStatus;
		}
		else
		{
			return Idle_status;
		}
	}

	// 函数描述：设置部队移动坐标
	// @param moveTagetPosParam 移动的目标点
	// @return 设置成功与否
	bool setMoveTargetPos(PosXYStru &moveTagetPosParam);

	// 函数描述：部队执行命令
	// @param isEven 是否为偶数。false奇数：只处理移动    true偶数：只处理攻击
	// @return 返回true / false ; true:能够移动或攻击。 false:空闲
	bool thinking(bool isEven);

	// 函数描述：设置攻击对象
	// @param attackTargetPtr 部队的攻击对象
	// @return 设置成功与否
	bool setAttackTarget(const IArmyPtr attackTargetPtr);

	// 函数描述：该函数仅用于，指定副本传送门的点坐标，在副本传送门周围找最合适的点（传送门的正前方）
	// @param posInput 输入的起点坐标
	// @param posOutput 输出的最合适的点坐标
	// @return 设置成功与否
	bool getAdaptiveWhereaboutsPos(const PosXYStru &posInput, PosXYStru &posOutput) const;

	// 函数描述：指定某点，在这个点周围找最合适的点（非建筑物，格子没有占满5人，非敌军站的格）
	// @param posInput 输入的起点坐标
	// @param posOutput 输出的最合适的点坐标
	// @return 设置成功与否
	bool getAdaptivePos(const PosXYStru &posInput, PosXYStru &posOutput, unsigned int cellHasArmyNum = CELL_HAS_MAX_THREE_ARMY_NUM, unsigned int distanceVal = ONE_CELL_VAL);

	bool getNearPosInOneCircle(const PosXYStru &posInput, PosXYStru &posOutput);

	// 函数描述：设置策略类型
	// @param AiStrategy 策略类型
	// @return 设置成功与否
	void setStrategy(enumAIStrategy AiStrategy) const;

	// 函数描述：获取部队的策略管理器
	// @return 策略管理器指针
	inline IManager * getStrategyMgr() const
	{
		if (strategyManagerPtr)
		{
			return strategyManagerPtr.get();
		}
		else
		{
			return NULL;
		}

	}

	// 函数描述:设置巡逻点
	// @param PatrolPosVectParam 巡逻点的向量
	// @return 无
	inline void setPatrolPoses(const PatrolPosVect &PatrolPosVectParam)
	{
		PatrolPosVector = PatrolPosVectParam;
	}

	inline bool isStaticBlook(int x, int y) const
	{
#ifdef _DEBUG
		if (NULL == mapDataAndFraise.ClassImpedient)
		{
			return false;
		}
#endif
		return (mapDataAndFraise.ClassImpedient)->IsBlockoff(x, y);
	}

	bool getSelfBlastAttArmys(const PosXYStru &pos, std::vector<IArmyPtr> &armysCenter, std::vector<IArmyPtr> &armysInOneCircle) const;
	bool getArmysInPos(const PosXYStru &pos, std::vector<IArmyPtr> &Armys) const;
private:
	ArmyAction();
	ArmyAction(const ArmyAction &armyAction);

	// 函数描述：创建对应的策略管理器
	// @return 无
	void CreateStrategyManger();

	// 函数描述:校验当前点是否符合要求（非建筑物，格子没有占满5人，非敌军站的格）
	// @param posXY 需要校验的点坐标
	// @param loadMapInfoPtr 部队地图数据指针
	// @param armyInvect 障碍物地图数据指针
	// @return 是否满足
	bool isAdativePos(const PosXYStru &posXY, const CLoadMapInfo *loadMapInfoPtr, const IArmyPtrvectInvect &armyInvect, unsigned int cellHasArmyNum);

	MapDataStru mapDataAndFraise;
	SeekPath * seekPathArithmPtr;
	
	const LandformInfoMap *landformInfoMapPtr_;	 // 地形影响

	ENUM_ARMY_STATUS armyCurrentStatus;   // 部队在非托管下的状态

	vector<PosXYStru> movePosVector;

	PosXYStru moveTagetPos;

	unsigned int enemyID;

	IArmyPtr armyPtr;

	ManagerPtr strategyManagerPtr;

	PatrolPosVect PatrolPosVector;  // 巡逻点结构体向量

	ENUM_USE_SCENE useAiScene; // 策略AI使用场景

	AiLevelParam aiLevelParam; // AI等级参数

	CELL_HAS_ARMY_MAX cellHasArmyMax_;	// 一个格子能站的最多部队数.目前副本控制一个格子站一个部队，而在郡中则为5个部队

};

#endif
