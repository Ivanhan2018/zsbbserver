#ifndef PRO_LOGIC_H_
#define PRO_LOGIC_H_

#pragma warning(push)
#pragma warning(disable:4100)
#include "ProFightSystem.h"
#pragma warning(pop)
#pragma pack(push,1)

struct tagEnterMap: public DISTRIBUTED_HEADER // 进入地图
{
	unsigned long	ulUserID;
	unsigned		uMapID;
	void inline serialize( int userid, int mapid, int command);

};

struct REQ_CREATE_ARMY_STRUCT //创建队伍
{
	;
};
struct REQ_MOVE_ARMY_INFO_STRUCT :public DISTRIBUTED_HEADER //移动队伍
{
	void inline serialize( int userid, int mapid, int command , unsigned long ArmyID,int x ,int y);
	unsigned long ArmyID;
	int iEndX; //移动的终点坐标位置
	int iEndY;
};
struct RSP_MOVE_ARMY_INFO_STRUCT :public DISTRIBUTED_HEADER // 返回移动信息
{
	unsigned long ArmyID;
	int istep;
	inline POS_STRUCT & operator [](unsigned int);
	void inline serialize(int length, int agent, int from, int to, int command);
};
struct REQ_ARMY_OUT_CITY_STRUCT :public DISTRIBUTED_HEADER // 出城
{
	unsigned long ArmyID;//队伍编号
	unsigned long CityID;//来之于那个城市
	void inline serialize( int from, int to, unsigned long ArmyId , unsigned long cityID);

};
struct RSP_ARMY_OUT_CITY_STRUCT : public DISTRIBUTED_HEADER //返回队伍出城信息
{
	unsigned long ArmyID; //出城队伍编号
	bool flag ;
	POS_STRUCT Pos;       //出来的位置
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