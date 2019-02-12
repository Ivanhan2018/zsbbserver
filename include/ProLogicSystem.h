#ifndef PRO_LOGIC_H_
#define PRO_LOGIC_H_

#pragma warning(push)
#pragma warning(disable:4100)
#include "ProFightSystem.h"
#pragma warning(pop)
#pragma pack(push,1)

struct tagEnterMap: public DISTRIBUTED_HEADER // �����ͼ
{
	unsigned long	ulUserID;
	unsigned		uMapID;
	void inline serialize( int userid, int mapid, int command);

};

struct REQ_CREATE_ARMY_STRUCT //��������
{
	;
};
struct REQ_MOVE_ARMY_INFO_STRUCT :public DISTRIBUTED_HEADER //�ƶ�����
{
	void inline serialize( int userid, int mapid, int command , unsigned long ArmyID,int x ,int y);
	unsigned long ArmyID;
	int iEndX; //�ƶ����յ�����λ��
	int iEndY;
};
struct RSP_MOVE_ARMY_INFO_STRUCT :public DISTRIBUTED_HEADER // �����ƶ���Ϣ
{
	unsigned long ArmyID;
	int istep;
	inline POS_STRUCT & operator [](unsigned int);
	void inline serialize(int length, int agent, int from, int to, int command);
};
struct REQ_ARMY_OUT_CITY_STRUCT :public DISTRIBUTED_HEADER // ����
{
	unsigned long ArmyID;//������
	unsigned long CityID;//��֮���Ǹ�����
	void inline serialize( int from, int to, unsigned long ArmyId , unsigned long cityID);

};
struct RSP_ARMY_OUT_CITY_STRUCT : public DISTRIBUTED_HEADER //���ض��������Ϣ
{
	unsigned long ArmyID; //���Ƕ�����
	bool flag ;
	POS_STRUCT Pos;       //������λ��
	void inline serialize(int length, int agent, int from, int to, int command);
};



struct REQ_IN_CITY_ARMY_INFO_STRUCT:public DISTRIBUTED_HEADER
{
	unsigned long playerID;
	unsigned long cityID;
	void inline serialize(int from,int to, int command = REQ_IN_CITY_ARMY_INFO);
};
struct ARMY_BASE_INFO_STRUCT
{
	unsigned long	ArmyID;
	char			Name[SIZE_32];
};
struct RSP_IN_CITY_ARMY_INFO_STRUCT: public DISTRIBUTED_HEADER
{
	int count ;
	ARMY_BASE_INFO_STRUCT ArmyInfo[MAX_NUM_ARMY];
	void inline serialize(int length,int agent,int from,int to , int command = RSP_IN_CITY_ARMY_INFO);
};



#pragma pack(pop)
inline POS_STRUCT & RSP_MOVE_ARMY_INFO_STRUCT::operator [](unsigned int index)
{
	return ((POS_STRUCT *)((unsigned char *)this + sizeof(*this)))[index];
}
void inline tagEnterMap::serialize( int userid, int mapid, int command)
{
	this->ulUserID = userid;
	this->uMapID = mapid ;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,userid ,mapid , command);
}

void inline REQ_MOVE_ARMY_INFO_STRUCT::serialize( int userid, int mapid, int command , unsigned long ArmyID,int x ,int y)
{
	this->ArmyID = ArmyID;
	this->iEndX = x;
	this->iEndY = y ;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,userid,mapid,command);
}

void inline RSP_MOVE_ARMY_INFO_STRUCT::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from ,to ,command);
}

void inline REQ_ARMY_OUT_CITY_STRUCT::serialize( int from, int to, unsigned long ArmyId , unsigned long cityid)
{
	this->ArmyID = ArmyId;
	this->CityID = cityid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,REQ_ARMY_OUT_CITY_INFO);
}

void inline RSP_ARMY_OUT_CITY_STRUCT::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to ,command);
}

void inline REQ_IN_CITY_ARMY_INFO_STRUCT::serialize(int from,int to , int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to ,command);
}
void inline RSP_IN_CITY_ARMY_INFO_STRUCT::serialize( int length ,int agent ,int from,int to, int command )
{
	DISTRIBUTED_HEADER::serialize(length,agent ,from,to, command);
}
#endif