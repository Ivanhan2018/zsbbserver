#ifndef CONSORTIAMANAGE_H__
#define CONSORTIAMANAGE_H__

#include <stdlib.h>
#include <stdarg.h>
#include <map>
#include "ConsortiaDataDefine.h"
#include "DB_ProxyHeader.h"
#include "DBTableHeader.h"
#include "MapServer_Protocol.h"
#include "Server_Protocol.h"
#include "socketsvc.hpp"
#include "IceUtil.h"
#include <set>
#include "Country.h"
#include "ZSBB_DataDefine.h"
#define MAX_MSG_COUNT	65535

class CConsortiaManage:public IceUtil::Task<char*,MAX_MSG_COUNT>
{
	class PlayerRanking_Cmp
	{
	public:
		bool operator()(CPlayerRanking* pPlayerA,CPlayerRanking* pPlayerB)
		{
			if(pPlayerA->GetLevel() ==pPlayerB->GetLevel())
			{
				if(pPlayerA->GetMilitaryRank() == pPlayerB->GetMilitaryRank())
				{
					if(pPlayerA->GetMoney() == pPlayerB->GetMoney())
					{
						return (pPlayerA->GetPlayerID() < pPlayerB->GetPlayerID());
					}
					else 
					{
						return (pPlayerA->GetMoney() > pPlayerB->GetMoney());
					}
				}
				else
				{
					return (pPlayerA->GetMilitaryRank() > pPlayerB->GetMilitaryRank());
				}
			}
			return (pPlayerA->GetLevel() >pPlayerB->GetLevel());
		}
	};
	class CollectionRate_Cmp
	{
	public:
		bool operator()(CPlayerRanking* pPlayerA,CPlayerRanking* pPlayerB)
		{
			if(pPlayerA->GetCollectionRate() ==pPlayerB->GetCollectionRate())
			{
				if(pPlayerA->GetLevel() == pPlayerB->GetLevel())
				{
					return (pPlayerA->GetPlayerID() < pPlayerB->GetPlayerID());
				}
				else
				{
					return (pPlayerA->GetLevel() > pPlayerB->GetLevel());
				}
			}
			return (pPlayerA->GetCollectionRate() >pPlayerB->GetCollectionRate());
		}
	};
	class Contribute_Cmp
	{
	public:
		bool operator()(CPlayerRanking* pPlayerA,CPlayerRanking* pPlayerB)
		{
			if(pPlayerA->GetContribute() ==pPlayerB->GetContribute())
			{
				if(pPlayerA->GetLevel() == pPlayerB->GetLevel())
				{
					return (pPlayerA->GetPlayerID() < pPlayerB->GetPlayerID());
				}
				else
				{
					return (pPlayerA->GetLevel() > pPlayerB->GetLevel());
				}
			}
			return (pPlayerA->GetContribute() >pPlayerB->GetContribute());
		}
	};
	class Enemy_Cmp
	{
	public:
		bool operator()(CPlayerRanking* pPlayerA,CPlayerRanking* pPlayerB)
		{
			if(pPlayerA->GetEnemyCount() ==pPlayerB->GetEnemyCount())
			{
				if(pPlayerA->GetLevel() == pPlayerB->GetLevel())
				{
					return (pPlayerA->GetPlayerID() < pPlayerB->GetPlayerID());
				}
				else
				{
					return (pPlayerA->GetLevel() > pPlayerB->GetLevel());
				}
			}
			return (pPlayerA->GetEnemyCount() >pPlayerB->GetEnemyCount());
		}
	};
	class Consortia_Cmp
	{
	public:
		bool operator()(CConsortiaRanking* pConsortiaA,CConsortiaRanking* pConsortiaB)
		{
			if(pConsortiaA->GetUserCount() == pConsortiaB->GetUserCount())
			{
				if(pConsortiaA->GetLevel() == pConsortiaB->GetLevel())
				{
					if(pConsortiaA->GetCreateTime() == pConsortiaB->GetCreateTime())
					{
						return (pConsortiaA->GetConsortiaID() < pConsortiaB->GetConsortiaID());
					}
					else
					{
						return (pConsortiaA->GetCreateTime() > pConsortiaB->GetCreateTime());
					}
				}
				else
				{
					return (pConsortiaA->GetLevel() > pConsortiaB->GetLevel());
				}
			}
			return (pConsortiaA->GetUserCount() > pConsortiaB->GetUserCount());
		}
	};

	typedef std::map<int,PublicBuildParamTable*> PublicBuildParamTable_Map;
	typedef std::map<int,PublicBuildParamTable*>::iterator PublicBuildParamTable_Iterator;

	typedef std::set<CPlayerRanking*,PlayerRanking_Cmp> PlayerRanking_Set;
	typedef std::set<CPlayerRanking*,PlayerRanking_Cmp>::iterator PlayerRanking_Set_Iterator;
	typedef std::set<CPlayerRanking*,CollectionRate_Cmp> CollectionRate_Set;
	typedef std::set<CPlayerRanking*,CollectionRate_Cmp>::iterator CollectionRate_Set_Iterator;
	typedef std::set<CPlayerRanking*,Contribute_Cmp> Contribute_Set;
	typedef std::set<CPlayerRanking*,Contribute_Cmp>::iterator Contribute_Set_Iterator;
	typedef std::set<CPlayerRanking*,Enemy_Cmp> Enemy_Set;
	typedef std::set<CPlayerRanking*,Enemy_Cmp>::iterator Enemy_Set_Iterator;
	typedef std::set<CConsortiaRanking*,Consortia_Cmp> ConsortiaRanking_Set;
	typedef std::set<CConsortiaRanking*,Consortia_Cmp>::iterator ConsortiaRanking_Set_Iterator;

	typedef std::map<int,LPCareerConfig_Item> CareerConfig_Map;
	typedef std::map<int,LPCareerConfig_Item>::iterator CareerConfig_Iterator;

	typedef struct tagVoteParam
	{
		int week;
		int hour;
		int min;
		int holdtimes;//任职时长，(单位：分钟）
		int votetimes;//选举时长,(单位：分钟）
		unsigned int holdseconds;
		int warningtimes;//开始选举提前通知
	}VoteParam,*LPVoteParam;
	
public:
	CConsortiaManage();
	~CConsortiaManage();
	bool InitConManage(LPAgent_DB_Param pParam,const char *filename="Default.data");//先调用本函数，设置连接参数
	bool Start(unsigned int num_threads, 
		DATA_RECEIVER receiver_func, 
		EVENT_NOTIFY notify_func,
		TIMER_NOTIFY timer_func,
		SOCKET_DISCONNECT disconn_func,
		LOGIC_EXCEPTION exception_func);//设置连接参数后，调用该函数，启动服务

	int SetRankingTimes(int times);
	bool Stop();//定制服务

	bool Push(char *msg);

	
	virtual bool startTask(std::size_t = 1);
	virtual void stopTask();
	virtual void run();
	virtual void exit(char *msg);

	//业务函数的声明
	void CheckConsortia();//定时检测公会的人数

	int GetUnusedConID();			//获取一个未使用的公会ID
	int AddUnusedConID(int id);		//增加一个未使用的ID
	int DelUnusedConID(int id);

	int AddUsedConID(int id);
	int DelUsedConID(int id);
	bool IsExistConsortiaName(const char *name);//根据公会名称判断名字是否已存在
	int GetLevelLimit();			//获取创建公会的等级限制
	int	GetMoneyLimit();			//获取创建公会消耗的金钱
	int GetJoinConLimit();			//获取加入公会的限制条件
	int GetConsortCount();			//获取公会数量

	//公会操作
	void WriteLock_Consortia(int consortiaid);
	void Unlock_Consortia(int consortaid);
	void ReadLock_Consortia(int consortiaid);

	int GetConsortList(LPConsortia_Info con_list,int &count,int start,int consortiaid,DB_Proxy *pDBProxy);//获取公会列表
	int GetConsortia(int consortiaid,Consortia_Ptr &con_ptr);//未加锁
	int DelConsortia(int consortiaid);//未加锁
	int AddConsortia(int consortiaid,Consortia_Ptr &con_ptr);//未加锁

	//Add，
	int AddConsortiaUser(int consortiaid,ConsortiaUser_Ptr &user_ptr);//未加锁
	int DelConsortiaUser(int consortiaid,int playerid);//未加锁

	int AddConsortiaUnion(int consortiaid,ConsortiaUnion_Ptr &union_ptr);//未加锁
	int DelConsortiaUnin(int consortiaid,int unin_conid);
	int AddConsoritaBank(int consortiaid,ConsortiaBank_Ptr &bank_ptr);//未加锁
	int AddConsortiaBank(int consortiaid,int itemid,int itemcount);//未加锁
	int AddConsortiaFight(int consortiaid,ConsortiaFight_Ptr &fight_ptr);//未加锁

	//
	bool HasJoinConsortia(int playerid,int &consortiaid);//从成员全表中判断,判断某玩家是否已加入公会
    int	GetUserPurview(int playerid);//从成员全表中，获取用户的权限
	//职位相关
	char* GetOfficialName(int id);
	int GetOfficialPurview(int id);
	int GetOfficialLevel(int id);

	//玩家表
	int AddPlayer(int playerid,const char *name,int country,bool isonline,int agent);
	int GetPlayerInfo(int playerid,char *name,int &country,bool &isonline,int &agent,DB_Proxy *pDBProxy);
	int GetPlayerID(const char *name);
	int SetPlayerOnline(int playerid,bool isonline,int agent);
	int SetPlayerName(int playerid,const char *name);

	//排行
	int SetInsertRankingCount(int count);
	int GetInsertRankingCount();
	int Ranking(DB_Proxy *pDBProxy);
	int GetConsortiaRanking(LPConsortia_RankingInfo conranking_list,int &count,int start,int &pages);
	int GetPlayerRanking(LPPlayer_RankingInfo playerranking_list,int &count,int start,int &pages,DB_Proxy *pDBProxy);
	int GetMoneyRanking(LPPlayer_RankingInfo playerranking_list,int &count,int start,int &pages,DB_Proxy *pDBProxy);//
	//国家
	CCountry * GetCountry(int index);
	int VoteCouncilTimer(const char *msg,DB_Proxy *pDBProxy);//选举议员定时器
	int BroadBevote(const char *msg,DB_Proxy *pDBProxy);
	int getTirrierCountAllMap();

	int SetMilitaryCount(int count);
	int SetEconomyCount(int count);
	int SetContributeCount(int count);
	int GetMilitaryCount(int count);
	int GetEconomyCount(int count);
	int GetContributeCount(int count);

	void SetVoteParam(int week,int hour,int min,int holdtimes,int votetimes,int warningtimes);
	void SetDelateParam(int underwritetimes,int delatetimes,int underwritenum);
	void SetVoteFee(int money,int gomoney);
	__int64 GetNextTime();
	int GetNextTime(int week,int hour,int min);

public:
	LPTirrierParam GetTirrierParam(int tirriertype);
	int GetTirrierType(int weaponid);//根据武器ID获取国防军类型

	LPMailInfoParam_Item GetMailInfoParam(int countryid,int type);
	LPCareerConfig_Item GetCareerConfig(int deptid,int jobid);

	LegionTitleParam* getLegionTitleParam(int id);//根据ID获取称号信息
	LegionTitleParam* getLegionTitleParam(int type,int val);//根据类型，数值获取满足条件的最好称号
	int getLigionTitleID(int exp,int money);

	ConsortiaUpdateParam* getConsortiaUpdateParam(int level);//根据级别获取军团升级参数

	ConversionRateParam* getConversionRateParam();

private:
	void ClearAllData();
	void ReadLanguageConfig(const char *filename);
	bool GetPlayerAccTable();
	bool GetConsortiaTable();
	bool GetConsortiaUserTable(Consortia_Ptr &con_ptr);
	bool GetConsortiaUnionTable();
	bool GetConsortiaBankTable(Consortia_Ptr &con_ptr);
	bool GetConsortiaFightTable();
	bool GetConsortiaOfficialTable();
	bool GetCreateConsortiaLimitTable();

	//add by xujincai 20090320
	bool loadGameCityInfo();
	bool loadPublicBuildParamTable();
	bool loadTirrierParam();
	//获取用户信息
	int GetPlayerAcc(int playerid,DB_Proxy *pDBProxy,char *name,int &country,bool &isonline,int &agent);

	//统计排行
	void Ranking_Player(DB_Proxy *pDBProxy);
	void Ranking_Consortia(DB_Proxy *pDBProxy);
	void ClearConsortiaRanking(DB_Proxy *pDBProxy);
	void ResetConsortiaRanking(ConsortiaRanking_Ptr *pNewRanking,int size);
	//获取玩家某个城市的生产效率
	int GetCityCollectionRate(int playerid,int mapid,int cityid,DB_Proxy *pDBProxy);
	//读取邮件参数表
	void loadMailInfoParam();
	//清除邮件参数
	void clearMailInfoParam();

	//读取职位参数
	void loadCareerConfig();
	void clearCareerConfig();

	//读取军团荣誉称号参数表
	void loadLegionTitleParam();
	void clearLegionTitleParam();

	//读取军团升级参数表
	void loadConsortiaUpdateParam();
	void clearConsortiaUpdateParam();

	//个人荣誉、经验增加到军团后的折换率配置表,千分比
	void loadConversionRateParam();
public:
	Agent_DB_Param			m_AgentDBParam;						//连接Agent、DBG的参数
	//Email通知、提示信息的配置
	char					m_NoticeParam[10][SIZE_128];
	EmailParam				m_EmailParam[SIZE_16];
	
#ifdef PRESS_TEST
	long pro_count;
	size_t TaskSize();
#endif

private:
	VoteParam				m_VoteParam;
	long					m_ConCheckTimer;								//公会数量检测定时器；
	int						m_InsertRankingCount;							//排行记录写入DBG的数量
	int						m_MilitaryNum;									//军事委员会人数
	int						m_EconomyNum;									//经济委员会人数
	int						m_ContributeNum;								//公益委员为人数
	DB_Proxy				m_DBProxy;										//连接DBG
	//CConsortiaTask			m_ConsortiaTask;							//消息队列

	ConsortiaNode			m_Consortia[MAX_CONSORTIA_COUNT];	//公会列表

	ConsortiaUser_Map		m_AllConUserMap;					//保存所有公会的成员
	RWMutex					m_AllConUserMapMutex;				//公会成员表级锁


	CreateConsortiaLimit	m_CreateConsortia;			//创建公会的条件表（一条记录），只读

	ConsortiaOfficial_Map	m_ConOfficialMap;					//保存公会职位信息（只读）

	PlayerAcc_Map			m_PlayerAccMap;						//保存所有玩家的信息，该表只会添加，一般不做修改、删除操作；
	PlayerAcc_Name_Map		m_PlayerAccNameMap;					//使用用户名做主键，便于根据用户名获取ID
	RWMutex					m_PlayerAccMutex;

	std::map<int,int>		m_UnusedConID;						//未使用的公会ID，新增公会时，从中获取一个
	IceUtil::Mutex			m_UnusedMutex;

	std::map<int,int>		m_UsedConID_Map;						//已使用的公会ID
	RWMutex					m_UsedMutex;
	bool m_bHasStart;

	//个人排行新增
	PlayerRanking_Ptr		*m_PlayerRanking;
	PlayerRanking_Ptr		*m_CollectionRateRanking;
	PlayerRanking_Ptr		*m_ContributeRanking;
	PlayerRanking_Ptr		*m_EnemyRanking;
	int						m_PlayerRankingSize;
	RWMutex					m_PlayerRankingMutex;

	//公会排行新增
	ConsortiaRanking_Ptr	*m_ConosrtiaRanking;
	int						m_ConsortiaRankingSize;
	RWMutex					m_ConsortiaRankingMutex;

	//城市信息配置表
	GameCityInfo_Map		m_GameCityInfoMap;
	PublicBuildParamTable_Map	m_PublicBuildParam;

	CCountry					m_CountryList[3];

	TirrierParam_Map		m_TirrierParamList;//创建国防军参数

	//邮件参数表
	MailInfoParam_Map		m_MailInfoParamList;

	//职位参数
	CareerConfig_Map		m_CareerConfigList;
	int						m_RankingTimes;

	//军团荣誉称号配置表
	LegionTitleParam_Map_Map	m_LegionTitleParamMap;
	//军团升级配置表
	ConsortiaUpdateParam_Map	m_ConsortiaUpdateParamMap;
	//个人荣誉、经验增加到军团后的折换率配置表,千分比
	ConversionRateParam			m_ConversionRateParam;
};

#endif
