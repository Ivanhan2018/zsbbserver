#include "PlayerRanking.h"
#include <stdio.h>

CPlayerRanking::CPlayerRanking()
{
	RankingID = 0;					//����
	Level = 0;						//�ȼ�
	UpGradeDate = 0;				//��������
	MilitaryRank = 0;				//����
	Money = 0;						//��Ǯ
	FoodCount = 0;					//��ʳ����
	WoodCount = 0;					//ľ������
	IronCount = 0;					//������
	SkinCount = 0;					//Ƥ����
	CityCount = 0;					//�ݵ���
	GeneralCount = 0;				//ɱ�н���������²������
	Credit = 0;						//������

	Contribute = 0;					//����
	EnemyCount = 0;					//ɱ������
	CollectionRate = 0;				//��Դ�ɼ���
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