#ifndef PROCMESSAGE_H__
#define PROCMESSAGE_H__
#include <stdlib.h>
#include <stdarg.h>
#include "Distributed_Header.h"
#include "DB_ProxyHeader.h"
#include "DBTableHeader.h"
#include "ConsortiaDataDefine.h"
#include "Server_Protocol.h"
#include "ProChatSystem.h"
#include "socketsvc.hpp"
#include "ChargeProtocol.h"
#include "ConsortiaManage.h"
#include "ISystemLog.h"

class CConsortiaManage;

class CProcMessage
{
public:
	CProcMessage(CConsortiaManage *pConManage);
	~CProcMessage();
	void ProcMsg(const char *msg);
private:
	bool ConnectDBG();//连接DBG
	bool OnDefault(const char *msg);				//默认处理
	bool OnCreateConsortia(const char *msg);		//创建公会
	bool OnQueryConsortiaList(const char *msg);		//查询公会列表
	bool OnDeleteConsortial(const char *msg);		//删除公会
	bool OnQueryUserConsortiaInfo(const char *msg);	//查询用户所属公会信息
	bool OnModifyPlacard(const char *msg);			//修改公告
	bool OnQueryLogo(const char *msg);				//查询LOGO
	bool OnModifyLogo(const char *msg);				//修改LOGO
	bool OnContributeBank(const char *msg);			//捐献资源到银行
	bool OnPayoutBank(const char *msg);				//支配公会银行的资源
	bool OnQueryBank(const char *msg);				//查询公会银行
	bool OnAllyApply(const char *msg);				//结盟申请(结盟是否应该有阵营限制，不是同意阵营的无法结盟？？？？)
	bool OnAllyConsortia_Agree(const char *msg);	//同意结盟
	bool OnAllyConsortia_Refuse(const char *msg);	//拒绝结盟
	bool OnUnallyConsortia(const char *msg);		//解盟
	bool OnFightApply(const char *msg);				//约战申请
	bool OnFightConsortia_Agree(const char *msg);	//同意约战
	bool OnFightConsortia_Refuse(const char *msg);	//同意约战
	bool OnQueryFight(const char *msg);				//约战查询
	bool OnJoinApply(const char *msg);				//申请加入公会
	bool OnJoinConsortia_Agree(const char *msg);	//同意加入公会
	bool OnJoinConsortia_Refuse(const char *msg);	//拒绝加入公会
	bool OnQueryConUserList(const char *msg);		//查询公会成员列表
	bool OnKickUser(const char *msg);				//Kick某成员
	bool OnQuitConsortia(const char *msg);			//主动退出公会
	bool OnInviteJoinConsortia(const char *msg);	//邀请加入公会
	bool OnInviteJoinCon_Agree(const char *msg);	//同意邀请
	bool OnInviteJoinCon_Refuse(const char *msg);	//拒绝邀请
	bool OnPoseOfficial(const char *msg);			//任职
	bool OnDeposeOfficial(const char *msg);			//免职                                                   
	bool OnQueryOfficial(const char *msg);			//职位查询
	bool OnQueryCreateConLimit(const char *msg);	//查询创建公会的条件
	bool OnModifyConTenet(const char *msg);			//修改宗旨
	bool OnClientLogin(const char *msg);			//登陆
	bool OnTranChairman(const char *msg);			//移交会长
	bool OnQueryPlayerRanking(const char *msg);		//获取个人排行
	bool OnQueryConsortiaRanking(const char *msg);	//获取公会排行
	bool OnQueryFriendsRanking(const char *msg);	//获取人气型有种男排行
	bool OnQueryMoneyRanking(const char *msg);		//获取巨商型有种男排行
	bool OnQueryGeneralRanking(const char *msg);	//获取策略性有种男排行

	//add 20100302
	bool onModifyPurview(const char *msg);			//授权
	void onUpdateConsortia(const char *msg);		//升级
	void onNotifyUpdateConsortia(const char *msg);	//收费系统返回
	int updateConsortia(CConsortia *pConsortia,ConsortiaUpdateParam *pUpdateParam);	//升级的具体操作
	void onContributeMoney(const char *msg);		//捐钱到军团
	void onSalaried(const char *msg);				//领取工资
	void onNotifyPlayerExpOrHonors(const char *msg);//玩家获取了经验或荣誉
	void onModifySignID(const char *msg);			//修改军团标识
	//国家系统新增
	bool OnQueryUserInfoOfCountry(const char *msg);	//查询玩家的国家身份
	bool OnQueryCouncilList(const char *msg);		//查询本阵营的议员列表
	bool OnTakePartInElection(const char *msg);	//参加竞选（必须为议员）
	bool OnQueryCandidateList(const char *msg);//查询候选人列表
	bool OnPlayerBallot(const char *msg);				//投票
	bool OnPlayerDelate(const char *msg);				//弹劾
	bool OnQueryDelateInfo(const char *msg);			//查询弹劾信息
	bool OnDelateBallot(const char *msg);				//弹劾投票
	bool OnModifyElectionCreed(const char *msg);		//修改参选纲领
	bool OnCreateTirrier(const char *msg);				//国防部长创建国防军
	bool OnTirrierGoOut(const char *msg);				//国防军出征
	bool OnUpgradeBuild(const char *msg);				//升级公共建筑
	bool OnModifyNotice(const char *msg);				//修改势力公告
	bool OnQueryTirrierCount(const char *msg);			//查询国防军数量
	bool OnElectionNotify(const char *msg);				//接收到参选扣费，扣费服务器返回的消息
	bool OnCreateTirrierNotify(const char *msg);		//通知郡地图创建国防军
	bool OnUpgradeBuildNotify(const char *msg);			//通知郡地图升级公共建筑
	bool OnVoteEndTimer(const char *msg);				//选举结束定时器
	bool OnUnderWriteEndTimer(const char *msg);			//弹劾签名结束
	bool OnDelateVoteEndTimer(const char *msg);			//弹劾投票结束
	bool OnTirrierGoOutTimer(const char *msg);			//国防军出征定时器
	bool OnModifyTirrierState(const char *msg);			//国防军被消灭
	bool OnMarketRate(const char *msg);					//市场收税
private:
	//int GetPlayerInfo(int playerid,char *name,int &country,bool &isonline,int &agent);//获取玩家信息，内存中不存在则查询数据库
	int PoseOfficial(int player1id,int player2id,int jobid,int &consortiaid);//任公会职位
	int DeposeOfficial(int player1id,int player2id,int &consortiaid);//免公会职位
	int SendMsg2Consortia(int consortiaid,CHAT_TYPE type,const char *msg,int nomsg_playerid = 0);//下发公会内部消息,nomsg_playerid为0时，全部下发
	int SendMsg2Map(int country,CHAT_TYPE msg_type,const char *msg,int snd_type);//下发广播消息（按郡地图）
	int SendMsg2Player(int playerid,CHAT_TYPE type,const char *msg);//向某个玩家下发消息
private:
	CConsortiaManage *m_ConsortiaManage;
	DB_Proxy m_DBProxy;
};


#endif
