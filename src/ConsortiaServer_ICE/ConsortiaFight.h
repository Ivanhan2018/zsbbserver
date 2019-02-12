#ifndef CONSORTIAFIGHT_H__
#define CONSORTIAFIGHT_H__

#include "IceUtil.h"
class CConsortiaFight:public IceUtil::Shared
{
public:
	CConsortiaFight();
	CConsortiaFight(int id,int con1id,int con2id,int mapid,int cityid,__int64 time);
	~CConsortiaFight();
	int SetFight(int id,int con1id,int con2id,int mapid,int cityid,__int64 time);
	int SetFightID(int id);
	int SetConsortia1ID(int consortiaid);
	int SetConsortia2ID(int consortiaid);
	int SetMapID(int mapid);
	int SetCityID(int cityid);
	__int64 SetTime(__int64 time);

	int GetFightID();
	int GetConsortia1ID();
	int GetConsortia2ID();
	int GetMapID();
	int GetCityID();
	__int64 GetTime();
private:
	int		FightID;			//ԼսID
	int		Consortia1ID;		//����1 ID
	int		Consortia2ID;		//����2 ID
	int		MapID;				//��ս�Ŀ�ID  ��ս�ص�
	int		CityID;				//��ս�ĳ���ID
	__int64	Time;				//��սʱ��
};
#endif
