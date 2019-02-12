// @brief 简要描述: 巡逻功能公共接口函数 
// @author 作者: 黄 文 才

#ifndef _PATROL_FUN_SINGLTON_HH
#define _PATROL_FUN_SINGLTON_HH

#include "AiCommon.h"
#include "SeekPathArithm.h"
#include "IManager.h"

class PatrolFunSingleton
{
public:
	// 函数描述：获取通用函数类单例对象的指针
	// @return 返回
	static PatrolFunSingleton * getInstance();

	// 函数描述：执行巡逻功能
	// @param mapDataPtr 部队地图数据指针
	// @param seekPathArithmParamPtr 寻路对象的指针
	// @param armyObj 当前操作的部队
	// @param strategyMgr 策略管理器
	// @return 返回
	bool executePatrol(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *&strategyMgr, bool isEven);

private:
	PatrolFunSingleton();

	static PatrolFunSingleton * comFunInstance;  // 惟一实例

	class CGarbo // 它的唯一工作就是在析构函数中删除单例
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

	static CGarbo Garbo; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数
};


#endif

