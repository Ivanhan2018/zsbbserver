#ifndef PROCMESSAGE_H__
#define PROCMESSAGE_H__
#include <stdarg.h>
#include <stdio.h>
#include "Distributed_Header.h"
#include "DB_ProxyHeader.h"
#include "DBTableHeader.h"
#include "ChargeProtocol.h"
#include "ConsortiaProtocol.h"
#include "ZSBB_DataDefine.h"
class CChargeTask;
class CProcChargeMsg
{
public:
	CProcChargeMsg(CChargeTask *pTask,const char *dbgip,int port);
	~CProcChargeMsg();
	void ProcMsg(const char *msg);
private:
	bool ConnectDBG(const char *ip,int port);
	void QueryMoney(const char *msg);						//查询Go点
	void BuyCard(const char *msg);							//购买卡
	void UseResourceCard(const char *msg);					//使用基本资源卡
	void UseResourceSpeedCard(const char *msg);				//使用基本资源生产加速卡
	void UseResruitHeroCard(const char *msg);				//使用英雄招募卡
	void UseSpecialSkillCard(const char *msg);				//使用特殊技能卡
	void UseOtherSpeedCard(const char *msg);				//使用建造、科研、征兵加速卡
	void UseAutoCard(const char *msg);						//使用托管卡
	void CancelAuto(const char *msg);						//取消托管
	void QueryAutoList(const char *msg);					//查询托管信息（未完成的托管列表)
	void UseHeroEquipCard(const char *msg);					//使用英雄装备卡
	void QueryResourceRate(const char *msg);				//查询玩家某生产速率
	void UseExpandPackCard(const char *msg);				//使用背包扩展卡
	void ProcTimerMsg(const char *msg);						//处理定时器
	void BuildComplete(const char *msg);					//郡服务器下发通知
	void CollectResourceMsg(const char *msg);				//郡服务器的收集资源通知消息
	void ProcBuildNotify(int plyaerid,int mapid,int cityid);//托管业务处理
	void ExchangeMoney(const char *msg);					//兑换
	void ProImmedCompleteNotify(const char *msg);			//服务器返回的通知消息
	void IncreaseRecourceRate(const char *msg);				//不使用加速卡直接加速
	void ElectionNotify(const char *msg);
	void UseHeroResumeCard(const char *msg);				//使用恢复卡
	void TaskQueryMoney(const char *msg);					//任务系统查询GO点
	void AddPlayerGODian(const char *msg);					//添加、减少GO点
	void onAddOfficerSkillResponse(const char *msg);		//使用技能卡，大地图返回消息
	void onNotifyUseOneCard(const char *msg);				//使用一个卡的通知，服务器之间通知
	void onNotifyBuyCard(const char *msg);					//买卡，大地图加入背包后的消息处理
	void onNotifyUpdateConsortia(const char *msg);			//军团升级扣除GO点。
	void WriteLogEx(int playerid,CardParam *pCardParam,LogType logtype,int svrid = MDM_GP_MAP_CHARGE);//写日志
public:
	int GetItemPos(int playerid,int officerid,int itemid,int &sortid);
	int GetItemPos(int playerid,int officerid,int itemid,PlayerItemStorageTable &itemrec);
	static int GetItemInfo(PlayerItemStorageTable &itemrec,DB_Proxy *pDBProxy);	//获取卡在背包中的存储信息
	static int GetUnusedPos(PlayerItemStorageTable &packitem,DB_Proxy *pDBProxy);		//查找一个未使用的背包单元格
	int DelCard(PlayerItemStorageTable &iteminfo);								//从背包中删除某道具（卡）
	static bool BuildListIsEmpty(int playerid,int mapid,int cityid,DB_Proxy *pDBProxy);//判断用户某城市的建造列表是否为空
	int UpdatePlayerState(int playerid,int type,int value);								//修改用户的某种状态
	int DelPlayerState(int playerid,int type);											//删除用户的某种状态信息
private:
	CChargeTask *m_pTask;
	DB_Proxy m_DBProxy;
};
#endif
