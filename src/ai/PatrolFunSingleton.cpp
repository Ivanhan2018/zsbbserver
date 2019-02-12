// @brief 简要描述: 巡逻功能公共接口函数 
// @author 作者: 黄 文 才

#include "PatrolFunSingleton.h"
#include "AppetentAction.h"
#include "CommonFun.h"

#if defined(_WIN32)
#	define _WIN32_WINNT 0x0502
#endif

#include <boost/asio/detail/mutex.hpp>

boost::asio::detail::mutex mutexTmp;

PatrolFunSingleton * PatrolFunSingleton::comFunInstance = NULL;

PatrolFunSingleton::PatrolFunSingleton(void)
{
}

PatrolFunSingleton * PatrolFunSingleton::getInstance()
{
	if (NULL == comFunInstance)
	{
		boost::asio::detail::scoped_lock<boost::asio::detail::mutex> lock(mutexTmp);

		if (NULL == comFunInstance)
		{
			comFunInstance = new PatrolFunSingleton();
		}
	}

	return comFunInstance;
}

bool PatrolFunSingleton::executePatrol(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *&strategyMgr, bool isEven)
{
	const std::vector<POS_STRUCT> &armyPosVect = armyObj->GetArmyPos();

	if (armyPosVect.empty())
	{
		return false;
	}

	managerParam &mgrParam = strategyMgr->getMgrParam();

	bool isPatrol = mgrParam.PatrolStruct.isPatrol;
	PosXYStru &patrol  = mgrParam.PatrolStruct.patrol;
	PosXYStru armyPos, armyTmp;

	int minDistance = 0, distTmp = 0;

	PatrolPosVect::iterator itTmp;

	PatrolPosVect::iterator itBeg = (mgrParam.PatrolPosVectPtr)->begin();
	PatrolPosVect::iterator itEnd= (mgrParam.PatrolPosVectPtr)->end();

	armyPos.xPos = armyPosVect[0].PosX;
	armyPos.yPos = armyPosVect[0].PosY;

	// 当前正在巡逻状态下
	if (isPatrol)
	{
		// 看是否到达了上一次需要到达的巡逻点
 
		// 达到了上次需要去的巡逻点
		if (0 == getDistanceTwoNode(armyPos.xPos, armyPos.yPos, patrol.xPos, patrol.yPos))
		{
			// 查找下一个巡逻点
			for (; itBeg!=itEnd; ++itBeg)
			{
				if ((*itBeg).patrolPos == patrol)
				{
					break;
				}
			}

			if (itBeg == itEnd)
			{
#ifdef _DEBUG
				assert(0 && "巡逻点不住巡逻点向量中");
#endif
			}

			++itBeg;

			if (itBeg == itEnd)
			{
				itBeg = mgrParam.PatrolPosVectPtr->begin();
			}

			mgrParam.PatrolStruct.patrol.xPos = itBeg->patrolPos.xPos;
			mgrParam.PatrolStruct.patrol.yPos = itBeg->patrolPos.yPos;

		}
		// 否则继续前往上次的巡逻点

		FunParam funParam(mapDataParamPtr, 
							seekPathArithmParamPtr, 
							mgrParam.armyCurrentStatus, 
							mgrParam.movePosVector, 
							mgrParam.moveTagetPos,
							mgrParam.enemyID,
							armyObj,
							strategyMgr);

		moveFunParam attOrMoveFunParm(funParam, (mgrParam.PatrolStruct).patrol);

		CommonFunctionSingleton::getInstance()->setMoveTargetPos(attOrMoveFunParm);

		AiLevelParam *aiLevelParam = mgrParam.aiLevelParam;   // AI等级参数

		if (NULL == aiLevelParam)
		{
#ifdef _DEBUG
			assert(0 && "AI等级参数的结构指针值为空");
#endif
			return false;
		}

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath, false, true);

		mgrParam.PatrolStruct.isPatrol = true;

	}
	else
	{
		minDistance = getDistanceTwoNode(armyPos.xPos, armyPos.yPos, itBeg->patrolPos.xPos, itBeg->patrolPos.yPos);
		itTmp = itBeg;

		// 寻找离自己最近的巡逻点
		for (++itBeg; itBeg!=itEnd; ++itBeg)
		{
			distTmp = getDistanceTwoNode(armyPos.xPos, armyPos.yPos, itBeg->patrolPos.xPos, itBeg->patrolPos.yPos);

			if (distTmp < minDistance)
			{
				minDistance = distTmp;
				itTmp = itBeg;
			}
		}

		mgrParam.PatrolStruct.patrol = (*itTmp).patrolPos;

		FunParam funParam(mapDataParamPtr, 
							seekPathArithmParamPtr, 
							mgrParam.armyCurrentStatus, 
							mgrParam.movePosVector, 
							mgrParam.moveTagetPos,
							mgrParam.enemyID,
							armyObj,
							strategyMgr);

		moveFunParam attOrMoveFunParm(funParam, (*itTmp).patrolPos);

		CommonFunctionSingleton::getInstance()->setMoveTargetPos(attOrMoveFunParm);

		AiLevelParam *aiLevelParam = mgrParam.aiLevelParam;   // AI等级参数

		if (NULL == aiLevelParam)
		{
#ifdef _DEBUG
			assert(0 && "AI等级参数的结构指针值为空");
#endif
			return false;
		}

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath, false, true);

		mgrParam.PatrolStruct.isPatrol = true;
	}

	return true;
}

