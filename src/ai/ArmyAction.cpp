// @brief 简要描述: 对外接口类，实现AI功能
// @author 作者: 黄 文 才

#include <ctime>
#include "ArmyAction.h"
#include "CommonFun.h"


ArmyAction::ArmyAction(MapDataStru & mapDataAndFraiseParam, IArmyPtr armyPtrObj, ENUM_USE_SCENE enumUseScene, AiLevelParam *aiLevelParamTmp, SeekPath * seekPathArithmPtrParam,
					   const LandformInfoMap *landformInfoMapPtrParam)
: mapDataAndFraise(mapDataAndFraiseParam), armyPtr(armyPtrObj), useAiScene(enumUseScene), seekPathArithmPtr(seekPathArithmPtrParam), landformInfoMapPtr_(landformInfoMapPtrParam)
{
	//seekPathArithmPtr = new SeekPath(mapDataAndFraise.ClassImpedient);
	armyCurrentStatus = Idle_status;
	enemyID = 0;



	/*
	aistrategy::PatrolRecord PatrolRecordTmp;
	PatrolRecordTmp.patrolPos = PosXYStru(20, 22);

	PatrolPosVector.push_back(PatrolRecordTmp);

	
	PatrolRecordTmp.patrolPos = PosXYStru(27, 23);
	PatrolPosVector.push_back(PatrolRecordTmp);

	PatrolRecordTmp.patrolPos = PosXYStru(22, 21);
	PatrolPosVector.push_back(PatrolRecordTmp);
  
  */
	if (NULL == aiLevelParamTmp)
	{
		aiLevelParam.isCanCalled = 1;
		aiLevelParam.countRange = 8;
		aiLevelParam.callRangeVal = 5;
		aiLevelParam.isDynamicSeekPath = 1;
		aiLevelParam.pauseProbability = 0;
		aiLevelParam.tipTopAttackTar = PRIEST;
		aiLevelParam.runAway = 2;
	}
	else
	{
		aiLevelParam = *aiLevelParamTmp;
	}


/*
	if (Counterpart_AI == enumUseScene)
	{
		cellHasArmyMax_ = CELL_HAS_MAX_ONE_ARMY_NUM;
	}
	else
	{
		cellHasArmyMax_ = CELL_HAS_MAX_FIVE_ARMY_NUM;
	}
*/

	cellHasArmyMax_ = CELL_HAS_MAX_THREE_ARMY_NUM;
	
	// 暂时部队都不重叠
	//cellHasArmyMax_ = CELL_HAS_MAX_ONE_ARMY_NUM;

	CreateStrategyManger();
}

void ArmyAction::CreateStrategyManger()
{
	if (NULL == armyPtr || NULL == seekPathArithmPtr)
	{
#ifdef _DEBUG
		assert(0);
#endif
		return;
	}

	SOLDIER_TYPE soldierType = armyPtr->GetArmyOccupation();
	FactoryCreateProfessionMger factoryManger;

	switch (soldierType)
	{
	case WARRIOR:
		strategyManagerPtr = factoryManger.CreateCombatantStrategyMgr(mapDataAndFraise.MapData, mapDataAndFraise.ClassImpedient, seekPathArithmPtr, 
			                                                          armyPtr, &PatrolPosVector, useAiScene, &aiLevelParam, cellHasArmyMax_,
																	  landformInfoMapPtr_);
		break;
	case SHOOTER:		
		strategyManagerPtr = factoryManger.CreateBowArrowFigStrategyMgr(mapDataAndFraise.MapData, mapDataAndFraise.ClassImpedient, seekPathArithmPtr, 
			                                                           armyPtr, &PatrolPosVector, useAiScene, &aiLevelParam, cellHasArmyMax_,
																	   landformInfoMapPtr_);
		break;
	case PRIEST:
		strategyManagerPtr = factoryManger.CreateChurchmanStrategyMgr(mapDataAndFraise.MapData, mapDataAndFraise.ClassImpedient, seekPathArithmPtr, 
																		armyPtr, &PatrolPosVector, useAiScene, &aiLevelParam, cellHasArmyMax_,
																		landformInfoMapPtr_);
		break;
	case MAGE:	
		strategyManagerPtr = factoryManger.CreateRabbiStrategyMgr(mapDataAndFraise.MapData, mapDataAndFraise.ClassImpedient, seekPathArithmPtr, 
																	armyPtr, &PatrolPosVector, useAiScene, &aiLevelParam, cellHasArmyMax_,
																	landformInfoMapPtr_);
		break;
	default:
		strategyManagerPtr = factoryManger.CreateChurchmanStrategyMgr(mapDataAndFraise.MapData, mapDataAndFraise.ClassImpedient, seekPathArithmPtr, 
																	armyPtr, &PatrolPosVector, useAiScene, &aiLevelParam, cellHasArmyMax_,
																	landformInfoMapPtr_);
		break;
	}
}

ArmyAction::~ArmyAction()
{
	//if (NULL != seekPathArithmPtr)
	//{
	//	delete seekPathArithmPtr;
	//	seekPathArithmPtr = NULL;
	//}
}

bool ArmyAction::setMoveTargetPos(PosXYStru &moveTagetPosParam)
{
	if (NULL == armyPtr || !strategyManagerPtr || NULL == seekPathArithmPtr)
	{
		return false;
	}

	const std::vector<POS_STRUCT> &armyPosVect = armyPtr->GetArmyPos();

	if (armyPosVect.empty())
	{
		return false;
	}

	PosXYStru armyPos(armyPosVect[0].PosX, armyPosVect[0].PosY);
	
	bool  isSucess = CommonFunctionSingleton::getInstance()->GetProximateNotStaticBlockPos(mapDataAndFraise.ClassImpedient, armyPos, mapDataAndFraise.MapData, moveTagetPosParam);

	if (!isSucess)
	{
		return false;
	}

	FunParam funParam(mapDataAndFraise.MapData, 
						seekPathArithmPtr, 
						armyCurrentStatus, 
						movePosVector, 
						moveTagetPos,
						enemyID,
						armyPtr,
						strategyManagerPtr.get());

	moveFunParam attOrMoveFunParm(funParam, moveTagetPosParam);

	return CommonFunctionSingleton::getInstance()->setMoveTargetPos(attOrMoveFunParm);
	 
}

bool ArmyAction::setAttackTarget(const IArmyPtr attackTargetPtr)
{
	if (NULL == armyPtr || !strategyManagerPtr || NULL == attackTargetPtr ||  NULL == seekPathArithmPtr)
	{
		return false;
	}

	FunParam funParam(mapDataAndFraise.MapData, 
						seekPathArithmPtr, 
						armyCurrentStatus, 
						movePosVector, 
						moveTagetPos,
						enemyID,
						armyPtr,
						strategyManagerPtr.get());

	attackFunParam attOrMoveFunParm(funParam, attackTargetPtr->GetArmyID());

	return CommonFunctionSingleton::getInstance()->setAttackTarget(attOrMoveFunParm);
}

bool ArmyAction::thinking(bool isEven)
{
	if (NULL == armyPtr)
	{
		return false;
	}

	// 当部队属于空闲状态并且属于托管状态时，执行策略AI
	if (Idle_status == armyCurrentStatus && armyPtr->GetTrusteeshipState())
	{
		if (strategyManagerPtr)
		{
			return strategyManagerPtr->thinking(isEven);
		}
		else
		{
			return false;
		}
	}
	else
	{	
		if (strategyManagerPtr)
		{
			managerParam &mgrParam = strategyManagerPtr->getMgrParam();
			mgrParam.isCountermarch = false;
		}
		else
		{
//#ifdef _DEBUG
//			assert(0);
//#endif
			return true;
		}

		AiLevelParam *aiLevelParamTmp = strategyManagerPtr->getMgrParam().aiLevelParam;

#ifdef _DEBUG
		if (NULL == aiLevelParamTmp)
		{
			assert(0);
		}
#endif

		if(0 == aiLevelParamTmp)
			return false;

		AppetentAction AppetentActionObj(mapDataAndFraise.MapData, seekPathArithmPtr, armyPtr, cellHasArmyMax_);
		AppetentActionObj.action(isEven, enemyID, movePosVector, armyCurrentStatus, aiLevelParamTmp->tipTopAttackTar, aiLevelParamTmp->isDynamicSeekPath);
	}

	return true;
}

bool ArmyAction::getAdaptiveWhereaboutsPos(const PosXYStru &posInput, PosXYStru &posOutput) const
{
	bool isSucess = false;

	int num = 0;

	int yPos = posInput.yPos;
	
	std::vector<PosXYStru> vectPos;

	std::vector<PosXYStru>::iterator itBeg;
	std::vector<PosXYStru>::iterator itEnd;

	if (yPos <= 4)
	{
		num = 1;
	}
	else
	{
		num = 2;
	}

	CommonFunctionSingleton * commonFunction = CommonFunctionSingleton::getInstance();

	if (NULL == commonFunction)
	{
		return false;
	}

	isSucess = commonFunction->GetAllSpecifyPos(posInput, 1, vectPos, mapDataAndFraise.MapData);

	if (!isSucess || vectPos.empty())
	{
		return false;
	}

	itBeg = vectPos.begin();
	itEnd = vectPos.end();

	if (static_cast<int>(vectPos.size()) > num)
	{
		itBeg += num;
	}

	if (itBeg!=itEnd)
	{
		posOutput.xPos = itBeg->xPos;
		posOutput.yPos = itBeg->yPos;
	}

	return true;
}

bool ArmyAction::getAdaptivePos(const PosXYStru &posInput, PosXYStru &posOutput, unsigned int cellHasArmyNum, unsigned int distanceVal)
{
	// 临时使用
	//cellHasArmyNum = CELL_HAS_MAX_ONE_ARMY_NUM;

	CommonFunctionSingleton * commonFunction = CommonFunctionSingleton::getInstance();

	if (NULL == commonFunction)
	{
		return false;
	}

	bool isSucess = false;
	std::vector<PosXYStru> vectPos;

	std::vector<PosXYStru>::iterator itBeg;
	std::vector<PosXYStru>::iterator itEnd;

	CLoadMapInfo *loadMapInfoPtr = mapDataAndFraise.ClassImpedient;	
	
	if (NULL == loadMapInfoPtr)
	{
#ifdef  _DEBUG
		assert(0 && "部队地图指针为空了");
#endif
		return false;
	}

#ifdef _DEBUG
	if (NULL == mapDataAndFraise.MapData)
	{
		assert(0 && "障碍物地图指针为空了");
		return false;
	}
#endif

	IArmyPtrvectInvect &armyInvect = *(mapDataAndFraise.MapData);

	// 先校验当前点是否符合要求
	if (0 == distanceVal)
	{
		if (isAdativePos(posInput, loadMapInfoPtr, armyInvect, cellHasArmyNum))
		{
			posOutput = posInput;
			return true;
		}
		else
		{
			distanceVal = 1;
		}
	}

	for (; ;)
	{
		isSucess = commonFunction->GetAllSpecifyPos(posInput, distanceVal, vectPos, mapDataAndFraise.MapData);

		if (!isSucess || 0 == vectPos.size())
		{
			return false;
		}

		itBeg = vectPos.begin();
		itEnd = vectPos.end();

		for (; itBeg!=itEnd; ++itBeg)
		{
			if (!isAdativePos(*itBeg, loadMapInfoPtr, armyInvect, cellHasArmyNum))
			{
				continue;
			}

			posOutput.xPos = itBeg->xPos;
			posOutput.yPos = itBeg->yPos;

			return true;

		}

		distanceVal++;
	}
}

bool ArmyAction::getNearPosInOneCircle(const PosXYStru &posInput, PosXYStru &posOutput)
{	
	CLoadMapInfo *loadMapInfoPtr = mapDataAndFraise.ClassImpedient;	

	if (NULL == loadMapInfoPtr)
	{
#ifdef _DEBUG
		assert(0);
#endif
		return false;
	}

	if (!loadMapInfoPtr->IsBlockoff(posInput.xPos, posInput.yPos))
	{
		posOutput = posInput;
		return true;
	}
	
	CommonFunctionSingleton * commonFunction = CommonFunctionSingleton::getInstance();

	if (NULL == commonFunction)
	{
		return false;
	}

	bool isSucess = false;
	std::vector<PosXYStru> vectPos;

	std::vector<PosXYStru>::iterator itBeg;
	std::vector<PosXYStru>::iterator itEnd;

	if (NULL == loadMapInfoPtr)
	{
#ifdef  _DEBUG
		assert(0 && "部队地图指针为空了");
#endif
		return false;
	}

#ifdef _DEBUG
	if (NULL == mapDataAndFraise.MapData)
	{
		assert(0 && "障碍物地图指针为空了");
		return false;
	}
#endif

	isSucess = commonFunction->GetAllSpecifyPos(posInput, ONE_CELL_VAL, vectPos, mapDataAndFraise.MapData);

	if (!isSucess || 0 == vectPos.size())
	{
		return false;
	}

	itBeg = vectPos.begin();
	itEnd = vectPos.end();
	std::vector<PosXYStru> vectResultPos;

	for (; itBeg!=itEnd; ++itBeg)
	{
		if (loadMapInfoPtr->IsBlockoff(itBeg->xPos, itBeg->yPos))
		{
			continue;
		}

		vectResultPos.push_back(*itBeg);
	}

	unsigned int sizeResultPos = static_cast<unsigned int>(vectResultPos.size());
	if (0 == sizeResultPos)
	{
		return false;
	}

	srand(static_cast<unsigned int>(time(0)));	    // 根据系统时间设置随机数种子
	int index = rand() % sizeResultPos;				 // 取得区间[0,N)的整数

	posOutput = vectResultPos[index];

	return true;
}

bool ArmyAction::isAdativePos(const PosXYStru &posXY, const CLoadMapInfo *loadMapInfoPtr, const IArmyPtrvectInvect &armyInvect, unsigned int cellHasArmyNum)
{
	// 先判断该点是否是障碍物
	if (loadMapInfoPtr->IsBlockoff(posXY.xPos, posXY.yPos))
	{
		return false;
	}

	const list<IArmyPtr> &armyLst = armyInvect[posXY.xPos][posXY.yPos];

	if (armyLst.size() >= cellHasArmyNum)
	{
		return false;
	}

	if (armyLst.size() > 0)
	{
		IArmyPtr armyObjPtr = *(armyLst.begin());

		if (!(armyPtr->IsFriendsArmy(armyObjPtr)))
		{
			return false;
		}
	}

	return true;
}

void ArmyAction::setStrategy(enumAIStrategy AiStrategy) const
{
 	if (NULL != strategyManagerPtr.get())
	{	
		strategyManagerPtr->setStrategy(AiStrategy);	
	}
}

bool ArmyAction::getSelfBlastAttArmys(const PosXYStru &pos, std::vector<IArmyPtr> &armysCenter, std::vector<IArmyPtr> &armysInOneCircle) const 
{
	return CommonFunctionSingleton::getInstance()->getSelfBlastAttArmys(pos, mapDataAndFraise.MapData, armysCenter, armysInOneCircle);
}

bool ArmyAction::getArmysInPos(const PosXYStru &pos, std::vector<IArmyPtr> &Armys) const
{
	return CommonFunctionSingleton::getInstance()->getArmysInPos(pos, mapDataAndFraise.MapData, Armys);
}