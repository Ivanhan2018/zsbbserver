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
	int			RankingID;					//排名
	int			Level;						//等级
	__int64		UpGradeDate;				//升级日期
	int			MilitaryRank;				//军衔
	int			Money;						//金钱
	int			FoodCount;					//粮食数量
	int			WoodCount;					//木材数量
	int			IronCount;					//铁数量
	int			SkinCount;					//皮数量
	int			CityCount;					//据点数
	int			GeneralCount;				//杀敌将次数（俘虏次数）
	int			Credit;						//荣誉点

	int			Contribute;					//捐献
	int			EnemyCount;					//杀敌数量
	int			CollectionRate;				//资源采集率
};
#endif
