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

	typedef std::map<int,Candidate*> Candidate_Map;//��ѡ���б�
	typedef std::map<int,Candidate*>::iterator Candidate_Map_Iterator;

	typedef std::map<int,Ballot*> Ballot_Map;//ѡƱ�б�
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
	int InitCountry(DB_Proxy *pDBPoxy);//��ʼ��
	void startTimer();

	int DeleteAllCouncils(DB_Proxy *pDBProxy);//ɾ��������Ա���������ݿ��е�����
	int DeleteAllCandidate(DB_Proxy *pDBProxy);//ɾ�� ��ѡ�ˣ��������ݿ��е�����
	int DeleteAllBallot(DB_Proxy *pDBProxy);//ɾ��ѡƱ���������ݿ��е�����
	int DeleteAllDODDept(DB_Proxy *pDBProxy);//ɾ����������Ա
	int DeleteAllInteriorDept(DB_Proxy *pDBProxy);//ɾ����������Ա
	int ResetDelateInfo(DB_Proxy *pDBProxy);//���õ���
	int GetTirrierCount(DB_Proxy *pDBProxy);//��ѯ����������
	int GetTirrierCountOfHomeMap(DB_Proxy *pDBProxy);//��ѯ����������
	void addTirrierCountFromWeapon(int weaponid);

	int AddCouncil(CPlayerRanking *pPlayerRanking,Postion_Type postion,DB_Proxy *pDBProxy);
	__int64 ResetNextCouncilTime(DB_Proxy *pDBProxy);//������һ��������Աѡ�ٵ�ʱ��
	__int64 ResetNextVoteTime(DB_Proxy *pDBProxy,int times);//������һ����������ѡ�ٵ�ʱ��
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
	int QueryUserInfoOfCountry(LPQuery_UserOfCountry_Request msg,DB_Proxy *pDBProxy);//��ѯ��ҹ�����Ϣ
	int QueryCouncilList(LPQuery_CouncilList_Request msg,DB_Proxy *pDBProxy);		//��ѯ����Ӫ����Ա�б�
	int TakePartInElection(LPTakePartIn_Election_Request msg,DB_Proxy *pDBProxy);	//�μӾ�ѡ������Ϊ��Ա��
	int QueryCandidateList(LPQuery_CandidateList_Request msg,DB_Proxy *pDBProxy);//��ѯ��ѡ���б�
	int PlayerBallot(LPPlayer_Ballot_Request msg,DB_Proxy *pDBProxy);				//ͶƱ
	int PlayerDelate(LPCountry_Delate_Request msg,DB_Proxy *pDBProxy);				//����
	int QueryDelateInfo(LPQuery_DelateInfo_Request msg,DB_Proxy *pDBProxy);			//��ѯ������Ϣ
	int DelateBallot(LPDelate_Ballot_Request msg,DB_Proxy *pDBProxy);			//����ͶƱ
	int ModifyElectionCreed(LPModify_ElectionCreed_Request msg,DB_Proxy *pDBProxy);//�޸Ĳ�ѡ����
	int CreateTirrier(LPCreateTirrier_Request msg,DB_Proxy *pDBProxy);//������������������
	int TirrierGoOut(LPTirrierGoOut_Request msg,DB_Proxy *pDBProxy);//����������
	int UpgradeBuild(LPUpgradeBuild_Request msg,DB_Proxy *pDBProxy);//������������
	int ModifyNotice(LPModifyNotice_Request msg,DB_Proxy *pDBProxy);//�޸���������
	int QueryTirrierCount(LPQuery_TirrierCount_Request msg,DB_Proxy *pDBProxy);//��ѯ����������
	int VoteEndTimer(LPConsortia_Timer msg,DB_Proxy *pDBProxy);		//ѡ�ٽ���
	int UnderWriteEndTimer(LPConsortia_Timer msg,DB_Proxy *pDBProxy);		//����ǩ������
	int DelateVoteEndTimer(LPConsortia_Timer msg,DB_Proxy *pDBProxy);		//����ͶƱ����
	int ElectionFeeNotify(LPNotify_Election_Response msg,DB_Proxy *pDBProxy);//�շ�ϵͳ���ص�֪ͨ��Ϣ
	int CreateTirrierNotify(LPNotify_CreateTirrier_Response msg,DB_Proxy *pDBProxy);//����ͼ���ش�����������֪ͨ��Ϣ
	int UpgradeBuildNotify(LPNotify_UpgradeBuild_Response msg,DB_Proxy *pDBProxy);//����ͼ������������������֪ͨ��Ϣ
	int TirrierGoOutTimer(LPConsortia_Timer msg,DB_Proxy *pDBProxy);		//������������ʱ��
	int ModifyTirrierState(LPNotify_ModifyArmyState msg,DB_Proxy *pDBProxy);//������������
	int MarketRate(LPNotify_MarketRate_Request msg,DB_Proxy *pDBProxy);		//�г���˰
private:
	void ClearAllData();

private:
	CConsortiaManage *m_ConsortiaManage;
	int HomeMapID;
	int				TmpCountryID;
	unsigned int	HoldTimes;		//ѡ�ٵ�ʱ����,����ְʱ��
	unsigned int    VoteTimes;		//ѡ��ʱ��
	int				VoteFee_Money;
	int				VoteFee_GO;

	int				CountryID;		//��Ӫ��1�����ˣ�2������
	unsigned int	Treasury;		//�����ʽ�
	unsigned int	DODMoney;		//����
	unsigned int	FinanceMoney;	//��������
	float			Rate;			//���ѵı���
	__int64			Nexttime1;		//�´���Աѡ��ʱ��
	__int64			Nexttime2;		//����ѡ�ٽ���ʱ��

	int				State;			//״̬
	int				ToMapID;		//������������ͼ
	int				Week;			//����������ʱ��

	Council_Map	m_CouncilMap;//��Ա�б�
	Council_Map	m_DODMap;//������
	Council_Map	m_InteriorMap;//������
	Candidate_Map		m_CandidateMap;//��ѡ���б�
	Ballot_Map			m_BallotMap;	//ѡƱ
	IceUtil::RWRecMutex	m_RWMutex;

	//�������
	long			m_DelateTimer;		//������ʱ��
	LPConsortia_Timer m_TimerParam;
	unsigned int	m_UnderWriteTimes;	//ǩ��ʱ��
	unsigned int	m_DelateVoteTimes;	//����ͶƱʱ��
	unsigned int	m_LimitTimes;		//ʱ�����ƣ�������ʱ�䲻����������
	int				m_DelateType;		//�ο�Dept_Type
	unsigned int	m_UnderWriteNum;	//ǩ������

	//
	long			m_TirrierGoOutTimer;//������ʱ��
	LPConsortia_Timer m_TirrierGoOutParam;
	int				m_TirrerFee;		//�����������ķ���

	//��������Ϣ
	int				m_TirrierCount[3];	//����������
};

#endif
