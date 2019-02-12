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
	int		PlayerID;				//��ұ��
	char	NickName[NAME_LEN];		//����ǳ�
	int		Country;				//�������ң���Ӫ��
	short	OfficerRace;				//����
	bool	Online;				//����Ƿ�����
	int		Agent;				//������ҵ�Agent
};

#endif
