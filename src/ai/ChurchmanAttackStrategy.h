// @brief 简要描述: 牧师进攻策略
// @author 作者: 黄 文 才
#ifndef _CHURCHMANATTACKSTRATEGY_H_H
#define _CHURCHMANATTACKSTRATEGY_H_H

#include "ichurchmanstrategy.h"

class ChurchmanAttackStrategy :public IChurchmanStrategy
{
public:
	ChurchmanAttackStrategy();
public:
	virtual ~ChurchmanAttackStrategy(void);

	// 函数描述：执行命令
	// @param mapDataPtr 部队地图数据指针
	// @param seekPathArithmParamPtr 寻路对象的指针
	// @param armyObj 当前操作的部队
	// @param strategyMgr 策略管理器
	// @return 返回true / false ； true: 能够走  false：不能走
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr  armyObj, IManager *strategyMgr, bool isEven);

private:
	bool followTarget( IArmyPtr followObjPtr, IArmyPtr armyObj, IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IManager * strategyMgr, bool isEven ) const;

	bool getFollowTarAndFollowOrBack( IArmyPtr &followObjPtr, IArmyPtrvectInvect * mapDataParamPtr, IArmyPtr armyObj, IManager * strategyMgr, SeekPath * seekPathArithmParamPtr, bool isEven);
};

#endif
