#pragma once

struct DISTRIBUTED_HEADER;
struct FightPack;
enum PROTOCOL_FIGHT;


//组包广播数据
//仅在郡中广播数据时使用
class BroadcastManager
{
public:
	BroadcastManager();
public:
	~BroadcastManager(void);
public:
	//组包
	void insertBroadData(DISTRIBUTED_HEADER * broadcastData ,PROTOCOL_FIGHT fightCmd ,int dataLen);
	//广播数据到客户端
	void msgBroadcastData(void);
	//初始化
	bool initialize(int curServerID);
private:
	//广播包
	FightPack * broadcastPackData_;
	//服务器ID
	int curServerID_;
};
