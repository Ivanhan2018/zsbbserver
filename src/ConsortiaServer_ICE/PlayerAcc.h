#ifndef PLAYERACC_H__
#define PLAYERACC_H__

#include "IceUtil.h"
class CPlayerAcc:public IceUtil::Shared
{
public:
	enum{NAME_LEN=24};
	CPlayerAcc();
	CPlayerAcc(int id,const char* name,int country,bool isonline,int agent);
	~CPlayerAcc();

	int SetPlayer(int id,const char* name,int country);
	int SetPlayerID(int id);
	char* SetName(const char *name);
	int SetCountry(int country);
	short SetOfficerRace(short offrace);
	bool SetOnline(bool isonline);
	int SetAgent(int agent);

	int GetPlayerID();
	char *GetName();
	int GetCountry();
	short GetOfficerRace();
	bool IsOnline();
	int GetAgent();

private:	
	int		PlayerID;				//玩家编号
	char	NickName[NAME_LEN];		//玩家昵称
	int		Country;				//所属国家（阵营）
	short	OfficerRace;				//种族
	bool	Online;				//玩家是否在线
	int		Agent;				//保存玩家的Agent
};

#endif
