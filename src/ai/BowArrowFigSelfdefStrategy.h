// @brief 简要描述: 弓箭手自卫策略
// @author 作者: 黄 文 才

#ifndef _BOW_ARROW_FIGSELFDEFSTRATEGY
#define _BOW_ARROW_FIGSELFDEFSTRATEGY

#include "ibowarrowfigstragegy.h"

class BowArrowFigSelfdefStrategy :
	public IBowArrowFigStrategy
{
public:
	BowArrowFigSelfdefStrategy();
public:
	virtual ~BowArrowFigSelfdefStrategy(void);

	// 函数描述：执行命令
	// @param mapDataPtr 部队地图数据指针
	// @param seekPathArithmParamPtr 寻路对象的指针
	// @param armyObj 当前操作的部队
	// @param strategyMgr 策略管理器
	// @return 返回true / false ； true: 能够走  false：不能走
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjParam, IManager *strategyMgr, bool isEven);
private:
	// 函数描述：确定被攻击的部队是否在逃脱（在射程外，并在视野范围内）
	// @param armyObjParam  攻击部队的对象（当前AI控制的部队）
	// @param LastAttackTarget 被攻击的部队的智能指针
	// @return 是否在逃脱
	bool isAttackArmyIsOutruning(const IArmyPtr armyObjParam, const IArmyPtr LastAttackTarget) const;

	// 函数描述：处理射程内有无敌军和是否反击被射程外的敌军攻击
	// @param seekPathArithmParamPtr 寻路对象的指针
	// @param LastAttackedSelf最后一个攻击自己的敌军
	// @param EnemyInGunshotPtr在射程内的敌军
	// @param Command需要返回给部队的命令结构
	// @return 成功与否
	bool thinkingInOffset(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, const IArmyPtr LastAttackedSelf, const IArmyPtr EnemyInGunshotPtr, IArmyPtr armyObjParam, IManager *strategyMgr, bool isEven) const;
};

#endif
