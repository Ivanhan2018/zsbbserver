#ifndef CONSORTIARANKING_H__
#define CONSORTIARANKING_H__
#include "IceUtil.h"
#include "Global_Macro.h"

class CConsortiaRanking:public IceUtil::Shared
{
public:
	CConsortiaRanking();
	~CConsortiaRanking();
	int SetRankingID(int id);
	void SetConName(int conid,const char *name);
	void SetChairman(int playerid,const char *name);
	int SetLevel(int level);
	int SetUserCount(int count);
	int SetCountry(int country);
	__int64 SetCreateTime(__int64 createtime);

	int GetRankingID();
	int GetConsortiaID();
	char* GetConName();
	int GetChairmanID();
	char* GetChairman();
	int GetLevel();
	int GetUserCount();
	int GetCountry();
	__int64 GetCreateTime();

	bool operator<(CConsortiaRanking &conranking);
private:
	int		RankingID;						//排行序号
	int		ConID;							//公会ID
	char	ConName[SIZE_32];				//公会名称
	int		PlayerID;						//会长ID
	char	Chairman[SIZE_24];				//会长名称
	int		Level;							//公会等级
	int		Count;							//成员数量, 主排名字段
	int		Country;						//阵营
	__int64 CreateTime;						//创建时间
};
#endif
