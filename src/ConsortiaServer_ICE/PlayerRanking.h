#ifndef PLAYERRANKING_H__
#define PLAYERRANKING_H__
#include "IceUtil.h"
#include "Global_Macro.h"
#include "PlayerAcc.h"

class CPlayerRanking:public CPlayerAcc
{
public:
	CPlayerRanking();
	~CPlayerRanking();
	int SetRankingID(int id);
	int SetLevel(int level);
	__int64 SetUpGradeDate(__int64 upgrade);
	int SetMilitaryRank(int rank);
	int SetMoney(int money);
	int SetFoodCount(int foodcount);
	int AddFoodCount(int foodcount);
	int SetWoodCount(int woodcount);
	int AddWoodCount(int woodcount);
	int SetIronCount(int ironcount);
	int AddIronCount(int ironcount);
	int SetSkinCount(int skincount);
	int AddSkinCount(int skincount);
	int SetCityCount(int citycount);
	int SetGeneralCount(int count);
	int SetCredit(int credit);

	int SetContribute(int contribute);
	int SetEnemyCount(int enemycount);
	int SetCollectionRate(int rate);
	int AddCollectionRate(int rate);

	int GetRankingID();
	int GetLevel();
	__int64 GetUpGradeDate();
	int GetMilitaryRank();
	int GetMoney();
	int GetFoodCount();
	int GetWoodCount();
	int GetIronCount();
	int GetSkinCount();
	int GetCityCount();
	int GetGeneralCount();
	int GetCredit();
	int GetTotalResource();

	int GetContribute();
	int GetEnemyCount();
	int GetCollectionRate();

	bool operator<(CPlayerRanking &playerranking);
private:
	int			RankingID;					//����
	int			Level;						//�ȼ�
	__int64		UpGradeDate;				//��������
	int			MilitaryRank;				//����
	int			Money;						//��Ǯ
	int			FoodCount;					//��ʳ����
	int			WoodCount;					//ľ������
	int			IronCount;					//������
	int			SkinCount;					//Ƥ����
	int			CityCount;					//�ݵ���
	int			GeneralCount;				//ɱ�н���������²������
	int			Credit;						//������

	int			Contribute;					//����
	int			EnemyCount;					//ɱ������
	int			CollectionRate;				//��Դ�ɼ���
};
#endif
