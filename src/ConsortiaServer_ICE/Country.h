#ifndef COUNTRY_H__
#define COUNTRY_H__
#include <map>
#include <set>
#include "DB_ProxyHeader.h"
#include "DBTableHeader.h"
#include "ConsortiaDataDefine.h"
#include "Server_Protocol.h"
#include "socketsvc.hpp"
#include "PlayerRanking.h"
#include "ChargeProtocol.h"
#include "ProLogicSystem.h"
#include "MapServer_Protocol.h"
#include "ISystemLog.h"

class CConsortiaManage;
class CCountry
{
	enum{MAX_UNDERWRITE_COUNT=50};
	class Candidate_Cmp
	{
	public:
		bool operator()(Candidate* pCandidateA,Candidate* pCandidateB)
		{
			if(pCandidateA->vote_ == pCandidateB->vote_)
			{
				if(pCandidateA->expense_ == pCandidateB->expense_ )
				{
					if(pCandidateA->time_ == pCandidateB->time_)
						return (pCandidateA->playerid_ < pCandidateB->playerid_);
					else
						return (pCandidateA->time_  < pCandidateB->time_ );
				}
				else
				{
					return (pCandidateA->expense_ > pCandidateB->expense_);
				}
			}
			return (pCandidateA->vote_ >pCandidateB->vote_);
		}
	};

	typedef std::map<int,Council*> Council_Map;
	typedef std::map<int,Council*>::iterator Council_Map_Iterator;

	typedef std::map<int,Candidate*> Candidate_Map;//候选人列表
	typedef std::map<int,Candidate*>::iterator Candidate_Map_Iterator;

	typedef std::map<int,Ballot*> Ballot_Map;//选票列表
	typedef std::map<int,Ballot*>::iterator Ballot_Map_Iterator;

	typedef std::set<Candidate *,Candidate_Cmp> Candidate_Set;
	typedef std::set<Candidate *,Candidate_Cmp>::iterator Candidate_Set_Iterator;
public:
	CCountry();
	~CCountry();
	int SetCountryID(int id,CConsortiaManage *pManage);
	int GetCountryID();
	unsigned int SetVoteTimes(unsigned int times);
	unsigned int SetHoldTimes(unsigned int times);
	unsigned int SetUnderWriteTimes(unsigned int times);
	unsigned int SetDelateTiems(unsigned int times);
	unsigned int SetUnderWriteNum(unsigned int num);
	void SetVoteFee(int money,int gomoney);
	int InitCountry(DB_Proxy *pDBPoxy);//初始化
	void startTimer();

	int DeleteAllCouncils(DB_Proxy *pDBProxy);//删除所有议员，包括数据库中的数据
	int DeleteAllCandidate(DB_Proxy *pDBProxy);//删除 候选人，包括数据库中的数据
	int DeleteAllBallot(DB_Proxy *pDBProxy);//删除选票，包括数据库中的数据
	int DeleteAllDODDept(DB_Proxy *pDBProxy);//删除国防部成员
	int DeleteAllInteriorDept(DB_Proxy *pDBProxy);//删除内政部成员
	int ResetDelateInfo(DB_Proxy *pDBProxy);//重置弹劾
	int GetTirrierCount(DB_Proxy *pDBProxy);//查询国防军数量
	int GetTirrierCountOfHomeMap(DB_Proxy *pDBProxy);//查询国防军数量
	void addTirrierCountFromWeapon(int weaponid);

	int AddCouncil(CPlayerRanking *pPlayerRanking,Postion_Type postion,DB_Proxy *pDBProxy);
	__int64 ResetNextCouncilTime(DB_Proxy *pDBProxy);//设置下一次启动议员选举的时间
	__int64 ResetNextVoteTime(DB_Proxy *pDBProxy,int times);//设置下一次启动部长选举的时间
	int BroadCastCouncilList(DB_Proxy *pDBProxy,int type,int cmd);

	__int64 GetNextCouncilTime();
	__int64 GetNextVoteTime();
	int GetGoOutTime();
	Council *GetCouncil(int playerid);
	void WriteLock();
	void ReadLock();
	void Unlock();

	void DisplayCouncil();
	void SendEamil();
public:
	int StartVote(DB_Proxy *pDBProxy);
	int QueryUserInfoOfCountry(LPQuery_UserOfCountry_Request msg,DB_Proxy *pDBProxy);//查询玩家国家信息
	int QueryCouncilList(LPQuery_CouncilList_Request msg,DB_Proxy *pDBProxy);		//查询本阵营的议员列表
	int TakePartInElection(LPTakePartIn_Election_Request msg,DB_Proxy *pDBProxy);	//参加竞选（必须为议员）
	int QueryCandidateList(LPQuery_CandidateList_Request msg,DB_Proxy *pDBProxy);//查询候选人列表
	int PlayerBallot(LPPlayer_Ballot_Request msg,DB_Proxy *pDBProxy);				//投票
	int PlayerDelate(LPCountry_Delate_Request msg,DB_Proxy *pDBProxy);				//弹劾
	int QueryDelateInfo(LPQuery_DelateInfo_Request msg,DB_Proxy *pDBProxy);			//查询弹劾信息
	int DelateBallot(LPDelate_Ballot_Request msg,DB_Proxy *pDBProxy);			//弹劾投票
	int ModifyElectionCreed(LPModify_ElectionCreed_Request msg,DB_Proxy *pDBProxy);//修改参选纲领
	int CreateTirrier(LPCreateTirrier_Request msg,DB_Proxy *pDBProxy);//国防部长创建国防军
	int TirrierGoOut(LPTirrierGoOut_Request msg,DB_Proxy *pDBProxy);//国防军出征
	int UpgradeBuild(LPUpgradeBuild_Request msg,DB_Proxy *pDBProxy);//升级公共建筑
	int ModifyNotice(LPModifyNotice_Request msg,DB_Proxy *pDBProxy);//修改势力公告
	int QueryTirrierCount(LPQuery_TirrierCount_Request msg,DB_Proxy *pDBProxy);//查询国防军数量
	int VoteEndTimer(LPConsortia_Timer msg,DB_Proxy *pDBProxy);		//选举结束
	int UnderWriteEndTimer(LPConsortia_Timer msg,DB_Proxy *pDBProxy);		//弹劾签名结束
	int DelateVoteEndTimer(LPConsortia_Timer msg,DB_Proxy *pDBProxy);		//弹劾投票结束
	int ElectionFeeNotify(LPNotify_Election_Response msg,DB_Proxy *pDBProxy);//收费系统返回的通知消息
	int CreateTirrierNotify(LPNotify_CreateTirrier_Response msg,DB_Proxy *pDBProxy);//郡地图返回创建国防军的通知消息
	int UpgradeBuildNotify(LPNotify_UpgradeBuild_Response msg,DB_Proxy *pDBProxy);//郡地图返回升级公共建筑的通知消息
	int TirrierGoOutTimer(LPConsortia_Timer msg,DB_Proxy *pDBProxy);		//国防军出征定时器
	int ModifyTirrierState(LPNotify_ModifyArmyState msg,DB_Proxy *pDBProxy);//国防军被消灭
	int MarketRate(LPNotify_MarketRate_Request msg,DB_Proxy *pDBProxy);		//市场收税
private:
	void ClearAllData();

private:
	CConsortiaManage *m_ConsortiaManage;
	int HomeMapID;
	int				TmpCountryID;
	unsigned int	HoldTimes;		//选举的时间间隔,即任职时长
	unsigned int    VoteTimes;		//选举时长
	int				VoteFee_Money;
	int				VoteFee_GO;

	int				CountryID;		//阵营，1：联盟；2：部落
	unsigned int	Treasury;		//国库资金
	unsigned int	DODMoney;		//军费
	unsigned int	FinanceMoney;	//内政费用
	float			Rate;			//军费的比例
	__int64			Nexttime1;		//下次议员选举时间
	__int64			Nexttime2;		//部长选举结束时间

	int				State;			//状态
	int				ToMapID;		//国防军出征地图
	int				Week;			//国防军出征时间

	Council_Map	m_CouncilMap;//议员列表
	Council_Map	m_DODMap;//国防部
	Council_Map	m_InteriorMap;//内政部
	Candidate_Map		m_CandidateMap;//候选人列表
	Ballot_Map			m_BallotMap;	//选票
	IceUtil::RWRecMutex	m_RWMutex;

	//弹劾相关
	long			m_DelateTimer;		//弹劾定时器
	LPConsortia_Timer m_TimerParam;
	unsigned int	m_UnderWriteTimes;	//签名时长
	unsigned int	m_DelateVoteTimes;	//弹劾投票时长
	unsigned int	m_LimitTimes;		//时间限制，最后多少时间不能启动弹劾
	int				m_DelateType;		//参看Dept_Type
	unsigned int	m_UnderWriteNum;	//签名人数

	//
	long			m_TirrierGoOutTimer;//出征定时器
	LPConsortia_Timer m_TirrierGoOutParam;
	int				m_TirrerFee;		//创建国防军的费用

	//国防军信息
	int				m_TirrierCount[3];	//国防军数量
};

#endif
