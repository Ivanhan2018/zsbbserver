#include "PlayerRanking.h"
#include <stdio.h>

CPlayerRanking::CPlayerRanking()
{
	RankingID = 0;					//排名
	Level = 0;						//等级
	UpGradeDate = 0;				//升级日期
	MilitaryRank = 0;				//军衔
	Money = 0;						//金钱
	FoodCount = 0;					//粮食数量
	WoodCount = 0;					//木材数量
	IronCount = 0;					//铁数量
	SkinCount = 0;					//皮数量
	CityCount = 0;					//据点数
	GeneralCount = 0;				//杀敌将次数（俘虏次数）
	Credit = 0;						//荣誉点

	Contribute = 0;					//捐献
	EnemyCount = 0;					//杀敌数量
	CollectionRate = 0;				//资源采集率
}

CPlayerRanking::~CPlayerRanking()
{
	//printf("playerid:%d   ~CPlayerRanking()\n",GetPlayerID());
}

int CPlayerRanking::SetRankingID(int id)
{
	return (RankingID = id);
}
int CPlayerRanking::SetLevel(int level)
{
	return (Level = level);
}
__int64 CPlayerRanking::SetUpGradeDate(__int64 upgrade)
{
	return (UpGradeDate = upgrade);
}
int CPlayerRanking::SetMilitaryRank(int rank)
{
	return (MilitaryRank = rank);
}
int CPlayerRanking::SetMoney(int money)
{
	return (Money = money);
}
int CPlayerRanking::SetFoodCount(int foodcount)
{
	return (FoodCount = foodcount);
}
int CPlayerRanking::AddFoodCount(int foodcount)
{
	FoodCount += foodcount;
	return FoodCount;
}
int CPlayerRanking::SetWoodCount(int woodcount)
{
	return (WoodCount = woodcount);
}
int CPlayerRanking::AddWoodCount(int woodcount)
{
	WoodCount += woodcount;
	return WoodCount;
}
int CPlayerRanking::SetIronCount(int ironcount)
{
	return (IronCount = ironcount);
}
int CPlayerRanking::AddIronCount(int ironcount)
{
	IronCount += ironcount;
	return IronCount;
}
int CPlayerRanking::SetSkinCount(int skincount)
{
	return (SkinCount = skincount);
}
int CPlayerRanking::AddSkinCount(int skincount)
{
	SkinCount += skincount;
	return SkinCount;
}
int CPlayerRanking::SetCityCount(int citycount)
{
	return (CityCount = citycount);
}
int CPlayerRanking::SetGeneralCount(int count)
{
	return (GeneralCount = count);
}
int CPlayerRanking::SetCredit(int credit)
{
	return (Credit = credit);
}

int CPlayerRanking::SetContribute(int contribute)
{
	return (Contribute = contribute);
}
int CPlayerRanking::SetEnemyCount(int enemycount)
{
	return (EnemyCount = enemycount);
}
int CPlayerRanking::SetCollectionRate(int rate)
{
	return (CollectionRate = rate);
}
int CPlayerRanking::AddCollectionRate(int rate)
{
	CollectionRate += rate;
	return CollectionRate;
}

int CPlayerRanking::GetRankingID()
{
	return RankingID;
}
int CPlayerRanking::GetLevel()
{
	return Level;
}
__int64 CPlayerRanking::GetUpGradeDate()
{
	return UpGradeDate;
}
int CPlayerRanking::GetMilitaryRank()
{
	return MilitaryRank;
}
int CPlayerRanking::GetMoney()
{
	return Money;
}
int CPlayerRanking::GetFoodCount()
{
	return FoodCount;
}
int CPlayerRanking::GetWoodCount()
{
	return WoodCount;
}
int CPlayerRanking::GetIronCount()
{
	return IronCount;
}
int CPlayerRanking::GetSkinCount()
{
	return SkinCount;
}
int CPlayerRanking::GetCityCount()
{
	return CityCount;
}
int CPlayerRanking::GetGeneralCount()
{
	return GeneralCount;
}
int CPlayerRanking::GetCredit()
{
	return Credit;
}
int CPlayerRanking::GetTotalResource()
{
	return (FoodCount + WoodCount + IronCount + SkinCount);
}

bool CPlayerRanking::operator<(CPlayerRanking &playerranking)
{
	if(this->GetLevel() <playerranking.GetLevel())
	{
		return false;
	}
	else if(this->GetLevel() == playerranking.GetLevel())
	{
		/*
		if(this->GetUpGradeDate() >= playerranking.GetUpGradeDate())
		{
			return true;
		}
		*/
		if(this->GetMilitaryRank() < playerranking.GetMilitaryRank())
		{
			return false;
		}
		else if(this->GetMilitaryRank() == playerranking.GetMilitaryRank())
		{
			if(this->GetMoney() < playerranking.GetMoney())
			{
				return false;
			}
		}
	}
	return true;
}

int CPlayerRanking::GetContribute()
{
	return Contribute;
}
int CPlayerRanking::GetEnemyCount()
{
	return EnemyCount;
}
int CPlayerRanking::GetCollectionRate()
{
	return CollectionRate;
}