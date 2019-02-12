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
		int holdtimes;//��ְʱ����(��λ�����ӣ�
		int votetimes;//ѡ��ʱ��,(��λ�����ӣ�
		unsigned int holdseconds;
		int warningtimes;//��ʼѡ����ǰ֪ͨ
	}VoteParam,*LPVoteParam;
	
public:
	CConsortiaManage();
	~CConsortiaManage();
	bool InitConManage(LPAgent_DB_Param pParam,const char *filename="Default.data");//�ȵ��ñ��������������Ӳ���
	bool Start(unsigned int num_threads, 
		DATA_RECEIVER receiver_func, 
		EVENT_NOTIFY notify_func,
		TIMER_NOTIFY timer_func,
		SOCKET_DISCONNECT disconn_func,
		LOGIC_EXCEPTION exception_func);//�������Ӳ����󣬵��øú�������������

	int SetRankingTimes(int times);
	bool Stop();//���Ʒ���

	bool Push(char *msg);

	
	virtual bool startTask(std::size_t = 1);
	virtual void stopTask();
	virtual void run();
	virtual void exit(char *msg);

	//ҵ����������
	void CheckConsortia();//��ʱ��⹫�������

	int GetUnusedConID();			//��ȡһ��δʹ�õĹ���ID
	int AddUnusedConID(int id);		//����һ��δʹ�õ�ID
	int DelUnusedConID(int id);

	int AddUsedConID(int id);
	int DelUsedConID(int id);
	bool IsExistConsortiaName(const char *name);//���ݹ��������ж������Ƿ��Ѵ���
	int GetLevelLimit();			//��ȡ��������ĵȼ�����
	int	GetMoneyLimit();			//��ȡ�����������ĵĽ�Ǯ
	int GetJoinConLimit();			//��ȡ���빫�����������
	int GetConsortCount();			//��ȡ��������

	//�������
	void WriteLock_Consortia(int consortiaid);
	void Unlock_Consortia(int consortaid);
	void ReadLock_Consortia(int consortiaid);

	int GetConsortList(LPConsortia_Info con_list,int &count,int start,int consortiaid,DB_Proxy *pDBProxy);//��ȡ�����б�
	int GetConsortia(int consortiaid,Consortia_Ptr &con_ptr);//δ����
	int DelConsortia(int consortiaid);//δ����
	int AddConsortia(int consortiaid,Consortia_Ptr &con_ptr);//δ����

	//Add��
	int AddConsortiaUser(int consortiaid,ConsortiaUser_Ptr &user_ptr);//δ����
	int DelConsortiaUser(int consortiaid,int playerid);//δ����

	int AddConsortiaUnion(int consortiaid,ConsortiaUnion_Ptr &union_ptr);//δ����
	int DelConsortiaUnin(int consortiaid,int unin_conid);
	int AddConsoritaBank(int consortiaid,ConsortiaBank_Ptr &bank_ptr);//δ����
	int AddConsortiaBank(int consortiaid,int itemid,int itemcount);//δ����
	int AddConsortiaFight(int consortiaid,ConsortiaFight_Ptr &fight_ptr);//δ����

	//
	bool HasJoinConsortia(int playerid,int &consortiaid);//�ӳ�Աȫ�����ж�,�ж�ĳ����Ƿ��Ѽ��빫��
    int	GetUserPurview(int playerid);//�ӳ�Աȫ���У���ȡ�û���Ȩ��
	//ְλ���
	char* GetOfficialName(int id);
	int GetOfficialPurview(int id);
	int GetOfficialLevel(int id);

	//��ұ�
	int AddPlayer(int playerid,const char *name,int country,bool isonline,int agent);
	int GetPlayerInfo(int playerid,char *name,int &country,bool &isonline,int &agent,DB_Proxy *pDBProxy);
	int GetPlayerID(const char *name);
	int SetPlayerOnline(int playerid,bool isonline,int agent);
	int SetPlayerName(int playerid,const char *name);

	//����
	int SetInsertRankingCount(int count);
	int GetInsertRankingCount();
	int Ranking(DB_Proxy *pDBProxy);
	int GetConsortiaRanking(LPConsortia_RankingInfo conranking_list,int &count,int start,int &pages);
	int GetPlayerRanking(LPPlayer_RankingInfo playerranking_list,int &count,int start,int &pages,DB_Proxy *pDBProxy);
	int GetMoneyRanking(LPPlayer_RankingInfo playerranking_list,int &count,int start,int &pages,DB_Proxy *pDBProxy);//
	//����
	CCountry * GetCountry(int index);
	int VoteCouncilTimer(const char *msg,DB_Proxy *pDBProxy);//ѡ����Ա��ʱ��
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
	int GetTirrierType(int weaponid);//��������ID��ȡ����������

	LPMailInfoParam_Item GetMailInfoParam(int countryid,int type);
	LPCareerConfig_Item GetCareerConfig(int deptid,int jobid);

	LegionTitleParam* getLegionTitleParam(int id);//����ID��ȡ�ƺ���Ϣ
	LegionTitleParam* getLegionTitleParam(int type,int val);//�������ͣ���ֵ��ȡ������������óƺ�
	int getLigionTitleID(int exp,int money);

	ConsortiaUpdateParam* getConsortiaUpdateParam(int level);//���ݼ����ȡ������������

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
	//��ȡ�û���Ϣ
	int GetPlayerAcc(int playerid,DB_Proxy *pDBProxy,char *name,int &country,bool &isonline,int &agent);

	//ͳ������
	void Ranking_Player(DB_Proxy *pDBProxy);
	void Ranking_Consortia(DB_Proxy *pDBProxy);
	void ClearConsortiaRanking(DB_Proxy *pDBProxy);
	void ResetConsortiaRanking(ConsortiaRanking_Ptr *pNewRanking,int size);
	//��ȡ���ĳ�����е�����Ч��
	int GetCityCollectionRate(int playerid,int mapid,int cityid,DB_Proxy *pDBProxy);
	//��ȡ�ʼ�������
	void loadMailInfoParam();
	//����ʼ�����
	void clearMailInfoParam();

	//��ȡְλ����
	void loadCareerConfig();
	void clearCareerConfig();

	//��ȡ���������ƺŲ�����
	void loadLegionTitleParam();
	void clearLegionTitleParam();

	//��ȡ��������������
	void loadConsortiaUpdateParam();
	void clearConsortiaUpdateParam();

	//�����������������ӵ����ź���ۻ������ñ�,ǧ�ֱ�
	void loadConversionRateParam();
public:
	Agent_DB_Param			m_AgentDBParam;						//����Agent��DBG�Ĳ���
	//Email֪ͨ����ʾ��Ϣ������
	char					m_NoticeParam[10][SIZE_128];
	EmailParam				m_EmailParam[SIZE_16];
	
#ifdef PRESS_TEST
	long pro_count;
	size_t TaskSize();
#endif

private:
	VoteParam				m_VoteParam;
	long					m_ConCheckTimer;								//����������ⶨʱ����
	int						m_InsertRankingCount;							//���м�¼д��DBG������
	int						m_MilitaryNum;									//����ίԱ������
	int						m_EconomyNum;									//����ίԱ������
	int						m_ContributeNum;								//����ίԱΪ����
	DB_Proxy				m_DBProxy;										//����DBG
	//CConsortiaTask			m_ConsortiaTask;							//��Ϣ����

	ConsortiaNode			m_Consortia[MAX_CONSORTIA_COUNT];	//�����б�

	ConsortiaUser_Map		m_AllConUserMap;					//�������й���ĳ�Ա
	RWMutex					m_AllConUserMapMutex;				//�����Ա����


	CreateConsortiaLimit	m_CreateConsortia;			//���������������һ����¼����ֻ��

	ConsortiaOfficial_Map	m_ConOfficialMap;					//���湫��ְλ��Ϣ��ֻ����

	PlayerAcc_Map			m_PlayerAccMap;						//����������ҵ���Ϣ���ñ�ֻ����ӣ�һ�㲻���޸ġ�ɾ��������
	PlayerAcc_Name_Map		m_PlayerAccNameMap;					//ʹ���û��������������ڸ����û�����ȡID
	RWMutex					m_PlayerAccMutex;

	std::map<int,int>		m_UnusedConID;						//δʹ�õĹ���ID����������ʱ�����л�ȡһ��
	IceUtil::Mutex			m_UnusedMutex;

	std::map<int,int>		m_UsedConID_Map;						//��ʹ�õĹ���ID
	RWMutex					m_UsedMutex;
	bool m_bHasStart;

	//������������
	PlayerRanking_Ptr		*m_PlayerRanking;
	PlayerRanking_Ptr		*m_CollectionRateRanking;
	PlayerRanking_Ptr		*m_ContributeRanking;
	PlayerRanking_Ptr		*m_EnemyRanking;
	int						m_PlayerRankingSize;
	RWMutex					m_PlayerRankingMutex;

	//������������
	ConsortiaRanking_Ptr	*m_ConosrtiaRanking;
	int						m_ConsortiaRankingSize;
	RWMutex					m_ConsortiaRankingMutex;

	//������Ϣ���ñ�
	GameCityInfo_Map		m_GameCityInfoMap;
	PublicBuildParamTable_Map	m_PublicBuildParam;

	CCountry					m_CountryList[3];

	TirrierParam_Map		m_TirrierParamList;//��������������

	//�ʼ�������
	MailInfoParam_Map		m_MailInfoParamList;

	//ְλ����
	CareerConfig_Map		m_CareerConfigList;
	int						m_RankingTimes;

	//���������ƺ����ñ�
	LegionTitleParam_Map_Map	m_LegionTitleParamMap;
	//�����������ñ�
	ConsortiaUpdateParam_Map	m_ConsortiaUpdateParamMap;
	//�����������������ӵ����ź���ۻ������ñ�,ǧ�ֱ�
	ConversionRateParam			m_ConversionRateParam;
};

#endif
