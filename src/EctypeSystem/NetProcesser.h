#ifndef NETPROCESSER_H_
#define NETPROCESSER_H_

#include <vector>
#include <list>
#pragma warning(push)
#pragma warning(disable:4244)
#include <boost/thread/shared_mutex.hpp>
#pragma warning(pop)
#include "EctypeManager.h"
#include "globalmacrodef.h"
class EctypeFightManager;
class FightEvent;
class CNPCInfo;
class DB_Proxy;
class CLoadDBInfo;
struct Notify_Enter_Ectype;
struct Notify_Is_In_Ectype;
struct MsgEctypeDelPlayer;
struct MsgEctypeFightOver;

enum FightManagersState{
	FIGHTMANAGE_NO_CREATE,
	FIGHTMANAGE_IS_CREATING,
	FIGHTMANAGE_IS_RUNING,
	FIGHTMANAGE_HAS_STOPED
};
#define  MAX_FIGHT_MANAGER_NUM 1000
class NetProcesser
{
public:
	static NetProcesser* instance();
public:
	~NetProcesser(void);
public:
	//初始化NetProcesser
	bool initialize();
	//解析客户端发送的命令
	void parse(char *const buf);
	//设置服务器ID
	void setProcessID(int curProcessID){
		curProcessID_ = curProcessID;
	}
	//释放内存
	void release();
	//添加玩家
	void insertPlayer(ActivityPlayerPtr playerPtr);
	//获得玩家信息
	ActivityPlayerPtr getActivePlayer(unsigned int playerId);
protected:
	//获得空闲房间
	int getEmptyManager(void);
	void setManagerState(int roomID , FightManagersState state);
	//验证房间ID是否有效
	bool isValidRoomID(int roomID) const;
	//创建房间 
	void createRoom(Notify_Enter_Ectype* lpEvent);
	//验证玩家是否在副本
	void isInEctype(const Notify_Is_In_Ectype* lpEvent);
	//副本战斗结束
	void processEctypeOver(MsgEctypeFightOver * msgEctypeFightOver);
private:
	NetProcesser(void);
protected:
	bool pushDataToManager(const char * buf,  int roomID);
	bool createActPlayerList(std::list<ActivityPlayerPtr>& actPlayerList, Notify_Enter_Ectype* lpEvent , int roomID);
	void delPlayerInfo(MsgEctypeDelPlayer * msgEctypeDelPlayer);
	void freeArmyFromEctype();
protected:
	typedef std::vector<EctypeFightManager*> EctypeFightManagers;
	//FightManager集合
	EctypeFightManagers fightManagers;
	//服务器状态
	std::vector<FightManagersState>  managerStates;
	//静态实例
	static NetProcesser* instance_;
	//副本管理器
	EctypeManager* ectypeManager_;
	//局域网内部事件通知
	FightEvent*   fightEvent_;
	//DBG中的一些缓存数据
	CLoadDBInfo * dataBaseMemData_;
	//DBG代理
	DB_Proxy * dataBaseProxy_;
	//副本NPC部队TABLE
	CNPCInfo * ectypeNpcInfo_;
	//当前服务器注册的进程ID
	int curProcessID_;
	//服务器读写锁
	boost::shared_mutex ectypeMutex_;
private:
	NO_COPYABLE_AND_ASSIGN(NetProcesser);
	
};

#endif // NETPROCESSER_H_
