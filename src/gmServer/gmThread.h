#ifndef __gmThread_H__
#define __gmThread_H__

#include "gmGlobalHead.h"
#include "gmFixData.h"
#include "DB_Proxy.h"


class CGmThread
{
public:
	CGmThread();
	~CGmThread();

	//统一返回值定义:
	//返回值为1成功，其它失败
	bool InitGmThread(const char * addr,short port,const char * dns,const char * user,const char * password);
	bool FInitGmThread();

	bool initdb(const char * addr,short port);
	void finitdb();
	
	bool ConData(const char * dsn,const char * user,const char * password);
	void DisConnData();

	//登录GM
	int OnGmLogin(const DISTRIBUTED_HEADER* pNetRead);
	
	//登出GM
	int OnGmLogout(const DISTRIBUTED_HEADER* pNetRead);
	
	//是否禁登录
	int OnLoginDisabled(const DISTRIBUTED_HEADER* pNetRead);
	
	//是否禁言
	int OnSpeakDisabled(const DISTRIBUTED_HEADER* pNetRead);
	
	//请求势力公告
	int OnGetNoticeForces(const DISTRIBUTED_HEADER* pNetRead);
	
	//GM命令
	int OnGmCommand(const DISTRIBUTED_HEADER* pNetRead);
	
	//增加主英雄名事件
	int OnGmAddMainHero(const DISTRIBUTED_HEADER* pNetRead);
	
	//修改势力公告(公会)
	void ProcModiNoticeForcesEvent(const DISTRIBUTED_HEADER* pNetRead);

	int ProcNotifyCreateNPCArmy(const DISTRIBUTED_HEADER * pNetRead);

	int ProcNotifyDelNPCArmy(const DISTRIBUTED_HEADER * pNetRead);
private:
	//以下是OnGmCommand分解处理函数
	//修改势力公告
	int ProcModiNoticeForces(short gmid,const tagGMCommand* gmc,int fid,const char * fdata);
	
	//GM回显势力公告
	int ProcShowNoticeForces(short gmid,const tagGMCommand* gmc,int fid);

	//GM普通公告（组播)
	int ProcNoticePost(short gmid,const tagGMCommand* gmc,int fid,const char * fdata);

	//GM普通公告（广播）
	int ProcNoticeBroadcast(short gmid,const tagGMCommand* gmc,const char * fdata);

	//禁言
	int ProcSpeakingBanned(short gmid,const tagGMCommand* gmc, int playerid,int ienabled);
	
	//禁登录
	int ProcLoginBanned(short gmid,const tagGMCommand* gmc,int playerid,int ienabled);

	//金钱 科技点
	int ProcAdd2_3(short gmid,const tagGMCommand* gmc,int playerid,int nums);

	//经验值 荣誉点
	int ProcAdd4_5(short gmid,const tagGMCommand* gmc,int playerid,int nums);

	//清除首领英难所有技能
	int ProcClearSkill(short gmid,const tagGMCommand* gmc,int playerid);
	//清楚英雄所学技能
	int clearAllOfficerSkill(int playerid,int officerid);

	//添加英难背包内指定物品
	int ProcAddBackpackGoods(short gmid,const tagGMCommand* gmc,int playerid,int goodsid,int level,int type);

	//添加仓库内指定物品 
	//int ProcAddGoods(int gmid,const tagGMCommand* gmc,int playerid,int mapid,int cityid,int goodsid,int type,int nums);

	//增加指定兵种数量
	int ProcAddArms(short gmid,const tagGMCommand* gmc,int playerid,int mapid,int cityid,int armsid,int nums);

	//指定的科研项立即研究完成
	int ProcAddResearch(short gmid,const tagGMCommand* gmc,int playerid,int rid,int level,int type);

	//添加仓库内指定物品  
	//1粮食，木材，皮料，铁
	//addGoods 账号名称;1;地图id;城市id;物品id;数量
	//（PlayerResource）
	//2.	道具类
	//addGoods 账号名称;2;地图id;城市id;物品id;类型id;数量
	//（ProductionAmount）
	int ProcAddGoods_1(short gmid,const tagGMCommand* gmc,int playerid,int mapid,int cityid,int goodsid,int nums);
	int ProcAddGoods_2(short gmid,const tagGMCommand* gmc,int playerid,int mapid,int cityid,int goodsid,int type,int nums);

	//DBG
	DB_Proxy				Proxy ;
	otl_connect db_;
};


#endif
