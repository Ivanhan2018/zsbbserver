// @brief ��Ҫ����: Ѳ�߹��ܹ����ӿں��� 
// @author ����: �� �� ��

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

	// ��ǰ����Ѳ��״̬��
	if (isPatrol)
	{
		// ���Ƿ񵽴�����һ����Ҫ�����Ѳ�ߵ�
 
		// �ﵽ���ϴ���Ҫȥ��Ѳ�ߵ�
		if (0 == getDistanceTwoNode(armyPos.xPos, armyPos.yPos, patrol.xPos, patrol.yPos))
		{
			// ������һ��Ѳ�ߵ�
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
				assert(0 && "Ѳ�ߵ㲻סѲ�ߵ�������");
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
		// �������ǰ���ϴε�Ѳ�ߵ�

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

		AiLevelParam *aiLevelParam = mgrParam.aiLevelParam;   // AI�ȼ�����

		if (NULL == aiLevelParam)
		{
#ifdef _DEBUG
			assert(0 && "AI�ȼ������Ľṹָ��ֵΪ��");
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

		// Ѱ�����Լ������Ѳ�ߵ�
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

		AiLevelParam *aiLevelParam = mgrParam.aiLevelParam;   // AI�ȼ�����

		if (NULL == aiLevelParam)
		{
#ifdef _DEBUG
			assert(0 && "AI�ȼ������Ľṹָ��ֵΪ��");
#endif
			return false;
		}

		AppetentAction AppetentActionObj(mapDataParamPtr, seekPathArithmParamPtr, armyObj, mgrParam.cellHasArmyMax);
		AppetentActionObj.action(isEven, mgrParam.enemyID, mgrParam.movePosVector, mgrParam.armyCurrentStatus, aiLevelParam->tipTopAttackTar, aiLevelParam->isDynamicSeekPath, false, true);

		mgrParam.PatrolStruct.isPatrol = true;
	}

	return true;
}

