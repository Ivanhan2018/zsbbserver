// @brief 简要描述: 弓箭手防御策略
// @author 作者: 黄 文 才

#ifndef _BOW_ARROW_FIGRECOVERYSTRATEGY
#define _BOW_ARROW_FIGRECOVERYSTRATEGY

#include "ibowarrowfigstragegy.h"

class BowArrowFigRecoveryStrategy :
	public IBowArrowFigStrategy
{
public:
	BowArrowFigRecoveryStrategy(void);
public:
	virtual ~BowArrowFigRecoveryStrategy(void);

	// 函数描述：执行命令
	// @param mapDataPtr 部队地图数据指针
	// @param seekPathArithmParamPtr 寻路对象的指针
	// @param armyObj 当前操作的部队
	// @param strategyMgr 策略管理器
	// @return 返回true / false ； true: 能够走  false：不能走
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven);


private:
	// 函数描述：判断当前部队是否满足撤退条件
	// @param mapDataAndFraise 地图障碍物结构
	// @param armyObjParam AI所挂的军队
	// @return 满足返回是，否则返回否
	bool isSatisfyCondiOfCountermarch(IArmyPtrvectInvect * mapDataParamPtr, IArmyPtr armyObj) const;


	bool callFriendArmyToReinforce( std::list<IArmyPtr> &lstFriendGroup, bool &isSucess, IArmyPtrvectInvect * mapDataParamPtr, IArmyPtr armyObj, IManager * strategyMgr, IArmyPtr &armyPtrTmp, IArmyPtr EnemyInGunshotPtr ) const;
};

#endif
