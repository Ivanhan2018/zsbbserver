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
	int		RankingID;						//�������
	int		ConID;							//����ID
	char	ConName[SIZE_32];				//��������
	int		PlayerID;						//�᳤ID
	char	Chairman[SIZE_24];				//�᳤����
	int		Level;							//����ȼ�
	int		Count;							//��Ա����, �������ֶ�
	int		Country;						//��Ӫ
	__int64 CreateTime;						//����ʱ��
};
#endif
