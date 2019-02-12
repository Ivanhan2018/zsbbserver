// @brief 简要描述: 公共接口函数 
// @author 作者: 黄 文 才

#ifndef _COMMON_FUN_H_H_H
#define _COMMON_FUN_H_H_H

//#include "boost/noncopyable.hpp"
#include <vector>
#include "FactoryCreateProfessionMger.h"

using namespace aistrategy;

enum 
{ 
	ONE_CIRCLE = 1,
	FOUR_CIRCLE = 4,
	EIGHT_CIRCLE = 8
};

// 移动，攻击命令公共参数
struct FunParam
{
	IArmyPtrvectInvect * mapDataParamPtr;	// 部队地图数据指针
	SeekPath * seekPathPtr;					// 寻路对象的指针
	ENUM_ARMY_STATUS &armyCurrentStatus;    // 当前的部队状态
	vector<PosXYStru> &movePosVector;		// 移动的目的点坐标
	PosXYStru &moveTagetPos;				// 目标移动的点坐标
	unsigned int &enemyID;					// 敌军部队ID
	IArmyPtr armyPtr;						// 当前操作的部队
	IManager * strategyManagerPtr;			// 策略管理器的指针

	FunParam(IArmyPtrvectInvect * mapDataParamPtrTmp,
		SeekPath * seekPathPtrTmp, ENUM_ARMY_STATUS &armyCurrentStatusTmp, vector<PosXYStru> &movePosVectorTmp, PosXYStru &moveTagetPosTmp, unsigned int &enemyIDTmp,
		IArmyPtr armyPtrTmp, IManager * strategyManagerPtrTmp)
		: mapDataParamPtr(mapDataParamPtrTmp), seekPathPtr(seekPathPtrTmp), armyCurrentStatus(armyCurrentStatusTmp), movePosVector(movePosVectorTmp), 
		  moveTagetPos(moveTagetPosTmp), enemyID(enemyIDTmp), armyPtr(armyPtrTmp), strategyManagerPtr(strategyManagerPtrTmp)
	{

	}

private:
	FunParam();
	FunParam & operator = (const FunParam &attOrMoveFunParam);
};

// 攻击参数
struct attackFunParam
{
	FunParam &funParam; 
	unsigned int attackTargetArmyID;		// 是否执行后退命令

	attackFunParam(FunParam &funParamTmp,
		unsigned int attackTargetArmyIDParam)
		:  funParam(funParamTmp),
		attackTargetArmyID(attackTargetArmyIDParam)
	{

	}

private:
	attackFunParam();
	attackFunParam & operator = (const attackFunParam &attOrMoveFunParam);
};

// 移动参数
struct moveFunParam
{
	FunParam &funParam; 
	PosXYStru &posTarget;					// 移动目标点

	moveFunParam(FunParam &funParamTmp,
		PosXYStru &posTargetParam)
		:  funParam(funParamTmp),
		posTarget(posTargetParam)
	{

	}

private:
	moveFunParam();
	moveFunParam & operator = (const moveFunParam &attOrMoveFunParam);
};

typedef std::set<std::pair<int, int> > pairInSet;
typedef std::vector<IArmyPtr> ArmyPtrVector;
typedef std::list<IArmyPtr> listArmyPtr;
typedef const std::list<IArmyPtr>::const_iterator IArmyConstIter;

class StatFriendArmyAndEnemyStru
{
public:
	StatFriendArmyAndEnemyStru(
	IArmyPtr armyObjParam,
	IArmyPtrvectInvect * mapDataPtr,
	bool isOnlyBuilding,
	bool isOnlyTrusteeship,
	listArmyPtr & lstFriendArmy,
	listArmyPtr & lstEnemy,
	int & countCellsHasEnemy) :
	armyObjParam_(armyObjParam), mapDataPtr_(mapDataPtr), isOnlyBuilding_(isOnlyBuilding), isOnlyTrusteeship_(isOnlyTrusteeship), lstFriendArmy_(lstFriendArmy),
	lstEnemy_(lstEnemy), countCellsHasEnemy_(countCellsHasEnemy)
	{

	}
	
	bool fun(int xPos, int yPos);

private:
	IArmyPtr armyObjParam_;
	const IArmyPtrvectInvect * mapDataPtr_;
	bool isOnlyBuilding_;
	bool isOnlyTrusteeship_;
	listArmyPtr & lstFriendArmy_;
	listArmyPtr & lstEnemy_;
	int & countCellsHasEnemy_;

	StatFriendArmyAndEnemyStru();
	StatFriendArmyAndEnemyStru(const StatFriendArmyAndEnemyStru &attOrMoveFunParam);
	StatFriendArmyAndEnemyStru & operator = (const StatFriendArmyAndEnemyStru &attOrMoveFunParam);
};

class SpecifyDistAllPosHasArmy
{
public:
	SpecifyDistAllPosHasArmy(const IArmyPtrvectInvect * mapDataPtr,
	const CLoadMapInfo * staticBlockMapPtr,
	std::list<PosXYStru> & lstAllPosHasArmy) : mapDataPtr_(mapDataPtr), staticBlockMapPtr_(staticBlockMapPtr),
	lstAllPosHasArmy_(lstAllPosHasArmy)
	{

	}

	bool fun(int xPos, int yPos);

private:
	const IArmyPtrvectInvect * mapDataPtr_;
	const CLoadMapInfo * staticBlockMapPtr_;
	std::list<PosXYStru> & lstAllPosHasArmy_;

	SpecifyDistAllPosHasArmy();
	SpecifyDistAllPosHasArmy(const SpecifyDistAllPosHasArmy &attOrMoveFunParam);
	SpecifyDistAllPosHasArmy & operator = (const SpecifyDistAllPosHasArmy &attOrMoveFunParam);
};

class SecurityPosTarget
{
public:
	SecurityPosTarget(IArmyPtr armyObj,
	const IArmyPtrvectInvect * mapDataParamPtr,
	const SeekPath * seekPathArithmParamPtr,
	std::list<PosXYStru> &lstFindPos) :armyObj_(armyObj), mapDataParamPtr_(mapDataParamPtr), seekPathArithmParamPtr_(seekPathArithmParamPtr), lstFindPos_(lstFindPos)
	{

	}

	bool fun(int xPos, int yPos);

private:
	IArmyPtr armyObj_;
	const IArmyPtrvectInvect * mapDataParamPtr_;
	const SeekPath * seekPathArithmParamPtr_;
	std::list<PosXYStru> &lstFindPos_;
	SecurityPosTarget();
	SecurityPosTarget(const SecurityPosTarget &attOrMoveFunParam);
	SecurityPosTarget & operator = (const SecurityPosTarget &attOrMoveFunParam);
};

class CommonFunctionSingleton /*: private boost::noncopyable*/
{
public:	
	enum
	{
		SELF_DEFINE_ARMY_NUM = 5
	};

	enum
	{
		RAMDOM_FROM_VALUE = 1,
		RAMDOM_TO_VALUE = 100
	};

	// 函数描述：获取通用函数类单例对象的指针
	// @return 返回
	static CommonFunctionSingleton * getInstance();

	// 函数描述：返回射程内的某一部队的智能指针，如果射程内无敌军，则返回的是空指针。
	/*
	规则：
	相同距离内的目标选取规则：有敌军单位的，按兵种牧师、法师、射手、战士的顺序进行目标优先选择，
	射程内有1支以上的被优先选中兵种部队时，优先攻击攻击力更高的那只部队；
	*/
	// @param  mapDataPtr 部队地图数据指针
	// @param  armyObjParam 当前操作部队的对象
	// @return 返回射程内的部队指针
	IArmyPtr getGunshotEnemyToAttack(IArmyPtrvectInvect * mapDataPtr, IArmyPtr armyObjParam, const SOLDIER_TYPE tipTopAttackTar=PRIEST);


	// 函数描述：获取指定距离（圈）的部队的list
	// @param  mapDataPtr 部队地图数据指针
	// @param armyObjParam 当前操作部队的对象
	// @param distanceVal 指定的距离
	// @param isOnlyBuilding 是否只处理建筑物
	// @param isOnlyTrusteeship 是否只处理托管部队
	// @param lstFriendArmy 返回友军部队的智能指针列表
	// @param lstEnemy 返回敌军部队的智能指针列表
	// @param countCellsHasEnemy 统计有多少格子有敌军
	// @return 成功与否,指定圈超出地图范围时，返回false
	bool getSpecifyDistanceEnemy(IArmyPtrvectInvect * mapDataPtr,
								 IArmyPtr armyObjParam, 
								 int distanceVal,
								 bool isOnlyBuilding,
								 bool isOnlyTrusteeship,
								 listArmyPtr &lstFriendArmy,
								 listArmyPtr &lstEnemy,
								 int &countCellsHasEnemy);

	// 函数描述：获取指定距离（圈）的坐标列表
	// @param  mapDataPtr 部队地图数据指针
	// @param armyObjParam 当前操作部队的对象
	// @param distanceVal 指定的距离
	// @param lstPos 返回指定圈上的坐标列表
	// @return 成功与否
	template <typename A>
	bool getSpecifyDistancePos(IArmyPtrvectInvect * mapDataPtr, const IArmyPtr armyObjParam, const int distanceVal, A &a);

	// 函数描述：获取指定范围内的要攻击的敌军，并判断是否采取攻击
	/*
	规则：
	部队A视野内出现敌军B——系统统计以部队A为中心的8格范围内的已托管部队（或NPC部队）的攻击力总和与HP总和（得出数值TFP1和THP1，即总火力、总血值），
	并统计以部队B为中心的8格范围内敌军部队的攻击力总和与HP总和（得出数值TFP2和THP2）；
	如果TFP2+THP2大于 2倍的TFP1+THP1，则判断敌人太强大，部队A选择后退，转入后退状态；
	*/
	// @param  mapDataPtr 部队地图数据指针
	// @param armyObjSelf 当前操作部队的对象
	// @param disVal 指定的部队为中心的几范围内统计HP和攻击力
	// @param TargetArmyPtr 返回攻击的部队,如果没有可以攻击的部队，置为空指针。
	// @param isAttack 是否进行攻击
	// @return 成功与否
	bool getAttackArmy(IArmyPtrvectInvect * mapDataPtr, IArmyPtr armyObjSelf, const int disVal,  IArmyPtr &TargetArmyPtr, bool &isAttack, std::list<IArmyPtr> &lstFriendGroup, const SOLDIER_TYPE tipTopAttackTar);

	// 函数描述：获取后退的坐标点。
	/*
	规则：
	选择最近的未被摧毁的我方的城镇/城堡边沿外一格为移动目标；如果全部城镇、城堡都被摧毁，则所有后退的部队向一个固定点后退（行政中心坐标点3格范围内）
	*/
	// @param  mapDataPtr 部队地图数据指针
	// @param armyObjParam  AI所挂的军队
	// @param Command 需要返回给部队的命令结构
	// @return 成功与否
	bool GetPosOfCountermarch(IArmyPtrvectInvect * mapDataPtr, CLoadMapInfo *ClassImpedientPtr, IArmyPtr armyObjParam, PosXYStru &posXY);

	// 函数描述：获取指定格内是否存在的主战部队作为其跟随对象。优先级:战士->射手
	// @param  mapDataPtr 部队地图数据指针
	// @param armyObj  AI所挂的军队
	// @param iWithinScopeVal  指定格子数
	// @param followArmyObj 返回跟随的目标部队
	// @param 成功与否
	bool getFollowObjPtr(IArmyPtrvectInvect * mapDataPtr, IArmyPtr armyObj,  const int iWithinScopeVal, IArmyPtr &followArmyObj);


	// 函数描述:获得以自己为中心(包含自己)的指定范围内的友军（已托管空闲部队（或NPC部队））
	// @param  mapDataPtr 部队地图数据指针
	// @param armyObjParam 当前操作部队的对象
	// @param distanceVal 指定的距离
	// @param lstArmy  返回集团军(多个部队)
	// @return 成功与否
	bool getGroupArmy(IArmyPtrvectInvect * mapDataPtr, IArmyPtr armyObjSelf, const int disVal, std::list<IArmyPtr> &lstArmy);

	// 函数描述:获得部队中血最少的那只, 专用于牧师
	// @param listArmy 部队容器
	// @return 返回部队中血最少的那只
	IArmyPtr getMinHPArmy(const std::list<IArmyPtr> &listArmy);


	// 函数描述:执行设置移动到目的点命令，获取需要走的点
	// @param  attOrMoveFunParm 设置移动 目标的参数
	// @return 成功与否
	bool setMoveTargetPos(moveFunParam &attOrMoveFunParm);

	// 函数描述:执行设置攻击对象命令，获取需要走的点
	// @param  attOrMoveFunParm 设置攻击目标的参数
	// @return 成功与否
    bool setAttackTarget(attackFunParam &attOrMoveFunParm);

	bool MoveToSecurityPos(IArmyPtrvectInvect * mapDataParamPtr,  SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, const IArmyPtr attackMeArmy, PosXYStru &posMoveTo);

	// 函数描述：获取以某个点为中心的指定距离（圈）上的所有点
	// @param  mapDataPtr 部队地图数据指针
	// @param posStart 中心点
	// @param distanceVal 指定范围
	// @param lstPos 返回点列表
	// @return  成功与否 
	template <typename A>
	bool GetAllSpecifyPos(IArmyPtrvectInvect * mapDataPtr,
		const PosXYStru &posStart,
		const int distanceVal, 
		A &a);

	bool getSpecifyDistancePos(const IArmyPtr armyObjParam, 
		const int distanceVal, 
		std::vector<PosXYStru> &vectPos,
		IArmyPtrvectInvect * mapDataPtr);
	
	// 函数描述：返回视野外靠近的敌军（规则（优先级）：1.职业   2.最短距离   3.HP最大的）
	// @param mapDataPtr 部队地图数据指针
	// @param armyObjParam 当前操作部队的对象
	// @return  返回找到的部队指针
	IArmyPtr getNearestEnemyOutView(IArmyPtrvectInvect * mapDataPtr, IArmyPtr armyObjParam);

	// 函数描述：获取1~100内的随机数
	// @return  返回获取的随机数
	int GetRandomValFromOneToHundred();

	// 函数描述：返回指定圈上占有部队的坐标点
	// @param mapDataPtr 部队地图数据指针
	// @param armyObjParam 当前操作部队的对象
	// @param lstAllPosHasArmy 输出指定圈上的有部队的所有坐标点
	// @param distanceVal 指定圈数
	// @param isIncludeStatBlock 统计是否含静态障碍点
	// @return  返回找到的部队指针
	bool getSpecifyDistAllPosHasArmy(IArmyPtrvectInvect * mapDataPtr,
																 const CLoadMapInfo * staticBlockMapPtr,
																IArmyPtr armyObjParam,
																std::list<PosXYStru> &lstAllPosHasArmy,
																const int distanceVal);

	// 函数描述：获取目的点附近最近的可到达的离自己最近的点坐标
	// @param ClassImpedient 障碍物地图数据指针
	// @param armyPos 当前操作部队的坐标
	// @param mapDataPtr 部队地图数据指针
	// @param moveTagetPos 需要到达的目的点
	// @return  成功与否
	bool GetProximateNotStaticBlockPos(const CLoadMapInfo *ClassImpedient, const PosXYStru &armyPos, IArmyPtrvectInvect * mapDataPtr, PosXYStru &moveTagetPos);
		
	// 函数描述：根据目标选取规则选择指定范围内的敌军作为攻击目标
	// @param  mapDataPtr 部队地图数据指针
	// @param armyObjParam 所挂AI的部队
	// @param scopeVal 指定范围
	// @param enemyPtr 攻击目标
	//@param tipTopAttackTar       最先攻击的职业类型
	// @return  成功与否 
	bool getApproEnemyToAttackOnAimRule(IArmyPtrvectInvect * mapDataPtr , IArmyPtr armyObjParam, const int scopeVal, IArmyPtr &enemyPtr,  SOLDIER_TYPE tipTopAttackTar = SOLDIERTYPE_NONE);

	bool getArmysInPos(const PosXYStru &pos, const IArmyPtrvectInvect * mapDataPtr, std::vector<IArmyPtr> &Armys);

	bool enemyIsInAttackScope(const std::vector<POS_STRUCT> &armyPosVect, const std::vector<POS_STRUCT> &enemyPosVect, int attackScope);

	bool GetAllSpecifyPos(const PosXYStru &posStart,
		const int distanceVal, 
		std::vector<PosXYStru> &vectPos,
		const IArmyPtrvectInvect * mapDataPtr);

	// 获取自爆卡爆炸后的攻击目标
	bool getSelfBlastAttArmys(const PosXYStru &pos, const IArmyPtrvectInvect * mapDataPtr, std::vector<IArmyPtr> &armysCenter, std::vector<IArmyPtr> &armysInOneCircle);
private:
	// 函数描述：找出同种部队血最多的那只
	// @param listArmy 部队容器
	// @return  同种部队血最多的那只
	IArmyPtr getMaxHPArmy(const std::list<IArmyPtr>& listArmy);

	// 函数描述：校验获取当前操作部队
	// @param  mapDataPtr 部队地图数据指针
	// @param armyObjParam 所挂AI的部队
	// @param Xpos 返回部队的x坐标
	// @param Ypos 返回部队的y坐标
	// @param XMaxVal 返回地图上最大的x坐标
	// @param YMaxVal 返回地图上最小的y坐标
	// @return  成功与否 
	bool invalidateXYpos(IArmyPtrvectInvect * mapDataPtr,int Xpos, int Ypos);

	// 函数描述：统计友军和敌军
	// @param lstIArmyPtr 所有部队列表
	// @param armyObjParam 当前挂AI的部队
	// @param isOnlyBuilding 是否只处理建筑物
	// @param isOnlyTrusteeship 是否只处理托管部队
	// @param lstFriendArmy 返回友军列表
	// @param lstEnemy 返回敌军列表
	// @param countCellsHasEnemy 返回有多少格子有敌军
	// @param setArmyId 返回敌我部队的ID集合
	// @return  同种部队血最多的那只
	/*bool StatFriendArmyAndEnemy(const list<IArmyPtr> &lstIArmyPtr,
								IArmyPtr armyObjParam,
								bool isOnlyBuilding,
								bool isOnlyTrusteeship,
								listArmyPtr &lstFriendArmy,
								listArmyPtr &lstEnemy,
								int &countCellsHasEnemy);
								*/

	

	// 函数描述:返回集团军(多个部队)总HP
	// @param listArmy 多个部队列表
	// @return 返回集团军(多个部队)总HP
	int getGroupArmyHP(const std::list<IArmyPtr>& listArmy);

	// 函数描述:返回集团军(多个部队)总FP(攻击力)
	// @param listArmy 多个部队列表
	// @return 返回集团军(多个部队)总FP
	int getGroupArmyFP(const std::list<IArmyPtr>& listArmy);

	// 函数描述 查找合适的后退坐标
	// @param  mapDataPtr 部队地图数据指针
	// @param lstPos 坐标列表
	// @param setXYpos 敌军城堡的坐标集合
	// @return 找到与否
	bool isFindCountermarchPos(IArmyPtrvectInvect * mapDataPtr, const std::vector<PosXYStru> &vectPos, const pairInSet &setXYpos, PosXYStru &posXY);

	// 函数描述 查找视野外靠近的敌军（规则（优先级）：1.职业   2.最短距离   3.HP最大的）
	// @param lstPos 坐标列表
	// @param listArmy 供选择的敌军列表
	// @return 返回找到的敌军指针
	IArmyPtr getMinDistanceArmy(const IArmyPtr armyObjParam, const std::list<IArmyPtr> &listArmy);

	//根据设定的优先规则来制定该返回的部队
	//@param enemyClergyContainer  牧师容器
	//@param enemyRabbiContainer   法师容器
	//@param enemyBowContainer     弓箭手容器
	//@param enemyWarriorContainer 战士容器
	//@param tipTopAttackTar       最先攻击的职业类型
	//@return 目标部队，如果有就返回，否则返回NULL
	IArmyPtr getArmyByPriority(const listArmyPtr &enemyClergyContainer, const listArmyPtr &enemyRabbiContainer, const listArmyPtr &enemyBowContainer, const listArmyPtr &enemyWarriorContainer, const SOLDIER_TYPE tipTopAttackTar);

private:
	CommonFunctionSingleton();

	static CommonFunctionSingleton * comFunInstance;  // 惟一实例
	
	class CGarbo // 它的唯一工作就是在析构函数中删除单例
	{
	public:
		~CGarbo()
		{
			 if (NULL != CommonFunctionSingleton::comFunInstance)
			 {
				 delete CommonFunctionSingleton::comFunInstance;
				 CommonFunctionSingleton::comFunInstance = NULL;
			 }
		}
	};
	
	static CGarbo Garbo; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数
};

#endif
