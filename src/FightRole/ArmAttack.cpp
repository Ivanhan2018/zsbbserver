#include "ArmAttack.h"
#include "IArmy.h"

namespace MaterialEnemyNameSpace
{
	void getMaterialEnemy(int numCircle, IArmyPtr selfArmyPtr, IArmyPtr enemyPtr, ArmyAction * action, const POS_STRUCT &posStru, bool isAttackPos, 
		Armys &armyPtrsCenter, Armys &armyPtrsNear)
	{
		armyPtrsCenter.clear();
		armyPtrsNear.clear();

		// 出于自爆状态或者强制爆炸状态
		if (0 != numCircle)
		{
			std::vector<unsigned int> attackedTargetInOneCircle;
			std::vector<IArmyPtr> armysCenter, armysInOneCircle;
			action->getSelfBlastAttArmys(PosXYStru(posStru.PosX, posStru.PosY), armysCenter, armysInOneCircle);

			std::vector<IArmyPtr>::iterator itBeg = armysCenter.begin();
			std::vector<IArmyPtr>::iterator itEnd = armysCenter.end();

			for (; itBeg!=itEnd; ++itBeg)
			{
				if (NULL == (*itBeg) || 0 == (*itBeg)->GetCurrentHp())
				{
					//#ifdef _DEBUG
					//				assert(0 && "部队指针为空了");
					//#endif
					continue;
				}

				if (selfArmyPtr->IsFriendsArmy(*itBeg) || ARCH_BUILD_TOFT == (*itBeg)->getBuildDetailType()
					|| (ARCH_TRAP == (*itBeg)->getBuildDetailType() && !selfArmyPtr->isVisible(*itBeg)))
				{
					continue;			
				}

				armyPtrsCenter.push_back(*itBeg);
			}

			itBeg = armysInOneCircle.begin();
			itEnd = armysInOneCircle.end();

			for (; itBeg!=itEnd; ++itBeg)
			{
				if (NULL == (*itBeg) || 0 == (*itBeg)->GetCurrentHp())
				{
					//#ifdef _DEBUG
					//				assert(0 && "部队指针为空了");
					//#endif
					continue;
				}

				if (selfArmyPtr->IsFriendsArmy(*itBeg) || ARCH_BUILD_TOFT == (*itBeg)->getBuildDetailType()
					|| (ARCH_TRAP == (*itBeg)->getBuildDetailType() && !selfArmyPtr->isVisible(*itBeg)))
				{
					continue;			
				}

				armyPtrsNear.push_back(*itBeg);
			}
		}
		else
		{
			// 法师默认攻击全格,攻击某一点的武器也攻击全格
			if (MAGE == selfArmyPtr->GetArmyOccupation() || ARCH_TRAP == selfArmyPtr->getBuildDetailType() || isAttackPos)
			{
				std::vector<IArmyPtr> Armys;
				action->getArmysInPos(PosXYStru(posStru.PosX, posStru.PosY), Armys);
				std::vector<IArmyPtr>::iterator itBeg = Armys.begin();
				std::vector<IArmyPtr>::iterator itEnd = Armys.end();

				for (; itBeg!=itEnd; ++itBeg)
				{
					if (NULL == (*itBeg) || 0 == (*itBeg)->GetCurrentHp())
					{
						//#ifdef _DEBUG
						//				assert(0 && "部队指针为空了");
						//#endif
						continue;
					}

					if (selfArmyPtr->IsFriendsArmy(*itBeg) || ARCH_BUILD_TOFT == (*itBeg)->getBuildDetailType()
						|| (ARCH_TRAP == (*itBeg)->getBuildDetailType() && !selfArmyPtr->isVisible(*itBeg)))
					{
						continue;			
					}

					armyPtrsCenter.push_back(*itBeg);
				}
			}
			else if (!isAttackPos && NULL != enemyPtr)
			{
				armyPtrsCenter.push_back(enemyPtr);
			}
		}
	}
}

TargetAttack::TargetAttack(unsigned int numCircle, unsigned int actionLockBout, unsigned int trajectorySpeed, unsigned int trajectoryBout,
						   ArmyAction * armyAction, FightManager *fightManager,  IArmyPtr selfArmyPtr, unsigned int armyId, bool isFollowAttType)
: dArmyId_(armyId), isFollowAttType_(isFollowAttType)
{
	numCircle_ = numCircle;	
	actionLockBout_ = actionLockBout;
	trajectorySpeed_ = trajectorySpeed;
	trajectoryBout_ = trajectoryBout;
	action_ = armyAction;
	selfArmyPtr_ = selfArmyPtr;
	fightManager_ = fightManager;
}

PointAttack::PointAttack(unsigned int numCircle, unsigned int actionLockBout, unsigned int trajectorySpeed, unsigned int trajectoryBout, 
						 ArmyAction * armyAction, FightManager *fightManager, IArmyPtr selfArmyPtr, const POS_STRUCT &attPos)
: attackPoint_(attPos)
{	
	numCircle_ = numCircle;	
	actionLockBout_ = actionLockBout;
	trajectorySpeed_ = trajectorySpeed;
	trajectoryBout_ = trajectoryBout;
	action_ = armyAction;
	selfArmyPtr_ = selfArmyPtr;
	fightManager_ = fightManager;
}

bool TargetAttack::action()
{
	if (NULL == action_)
	{
		return true;
	}

	// 
	if (0 == actionLockBout_)
	{
		if (0 != trajectoryBout_)
		{
			trajectoryBout_--;

			return false;
		}

		IArmyPtr enemyPtr = selfArmyPtr_->getArmyByID(dArmyId_);

		if (NULL == enemyPtr || NULL == action_)
		{
			return true;
		}

		const std::vector<POS_STRUCT> &posStructVect = enemyPtr->GetArmyPos();

		if (0 == posStructVect.size())
		{
			assert(0 && "部队坐标为空");
			return true;		
		}

		const POS_STRUCT &posStru = posStructVect[0];
		Armys armyPtrsCenter, armyPtrsNear;

		// 1.恢复攻击具体攻击对象
		MaterialEnemyNameSpace::getMaterialEnemy(numCircle_, selfArmyPtr_, enemyPtr, action_, posStru, false, armyPtrsCenter, armyPtrsNear);

		if (0 == numCircle_)
		{
			if (0 == armyPtrsCenter.size())
			{
				return true;
			}

			selfArmyPtr_->Attack(armyPtrsCenter);
		}
		else
		{
			selfArmyPtr_->Attack(armyPtrsCenter, armyPtrsNear);
		}

		return true;
	}
	else
	{
		actionLockBout_--;
	}

	return false;
}

bool PointAttack::action()
{
	if (NULL == action_)
	{
		return true;
	}

	// 
	if (0 == actionLockBout_)
	{
		if (0 != trajectoryBout_)
		{
			trajectoryBout_--;

			return false;
		}

		Armys armyPtrsCenter, armyPtrsNear;

		// 1.恢复攻击具体攻击对象
		MaterialEnemyNameSpace::getMaterialEnemy(numCircle_, selfArmyPtr_, NULL, action_, attackPoint_, true, armyPtrsCenter, armyPtrsNear);

		if (armyPtrsCenter.empty() && armyPtrsNear.empty())
		{
			return true;
		}

		if (0 == numCircle_)
		{
			selfArmyPtr_->Attack(armyPtrsCenter);
		}
		else
		{
			selfArmyPtr_->Attack(armyPtrsCenter, armyPtrsNear);
		}

		return true;
	}
	else
	{
		actionLockBout_--;
	}

	return false;
}
