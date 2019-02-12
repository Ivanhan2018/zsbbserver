#include "Country.h"
#include "ConsortiaManage.h"
#include "calendar.h"
#include "log.hpp"
#pragma warning(disable:4100)
#pragma warning(disable:4127)
#pragma warning(disable:4189)
#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4706)
CCountry::CCountry()
{
	VoteFee_Money = 100000;
	VoteFee_GO = 100;
	State = 0;
	m_DelateTimer = 0;
	m_TimerParam = 0;
	ToMapID = 0;
	m_TirrierGoOutTimer = 0;
	m_TirrierGoOutParam = 0;
	m_TirrerFee = 100000;
	memset(m_TirrierCount,0,sizeof(int) *3);
}
CCountry::~CCountry()
{
	ClearAllData();
}

int CCountry::SetCountryID(int id,CConsortiaManage *pManage)
{
	TmpCountryID = id* 10;
	if(id == 1)
	{
		HomeMapID = 1003;
	}
	else
	{
		HomeMapID = 1007;
	}
	m_ConsortiaManage = pManage;
	return (CountryID = id);
}
int CCountry::GetCountryID()
{
	return CountryID;
}
unsigned int CCountry::SetVoteTimes(unsigned int times)
{
	return (VoteTimes = times);
}
unsigned int CCountry::SetHoldTimes(unsigned int times)
{
	return (HoldTimes = times);
}

unsigned int CCountry::SetUnderWriteTimes(unsigned int times)
{
	return (m_UnderWriteTimes = times);	//ǩ��ʱ��
}
unsigned int CCountry::SetDelateTiems(unsigned int times)
{
	return (m_DelateVoteTimes = times);
}
unsigned int CCountry::SetUnderWriteNum(unsigned int num)
{
	return (m_UnderWriteNum = num);
}
void CCountry::SetVoteFee(int money,int gomoney)
{
	VoteFee_Money = money;
	VoteFee_GO = gomoney;
	//printf("VoteFee_Money:%d,VoteFee_GO:%d\n",VoteFee_Money,VoteFee_GO);
}
//��ʼ��
int CCountry::InitCountry(DB_Proxy *pDBProxy)
{
	char *buf = 0;
	int count = 0;
	int i =0;
	//��ѯ������Ϣ
	Country countrytb;
	countrytb.setid(CountryID);
	i = pDBProxy->db_select(countrytb);
	if(i == DB_Proxy::DB_FAILED)
	{
		LOG_TRACE_EX(false,("CCountry::InitCountry->��ʼ��������Ϣʧ�ܣ�%d.",CountryID));		
		return 1;
	}
	else if(i == DB_Proxy::DB_EOF)
	{
		countrytb.rate_ = 40;
		if(pDBProxy->db_insert(countrytb) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE_EX(false,("CCountry::InitCountry->��ʼ��������Ϣʧ�ܣ�����������Ϣ��¼%d.",CountryID));	
			return 2;
		}
	}
	Treasury = countrytb.treasury_;		//�����ʽ�
	DODMoney = countrytb.dodmoney_;		//����
	FinanceMoney = countrytb.financemoney_;	//��������
	if (countrytb.rate_ >50)
	{
		countrytb.rate_  = 50;
	}
	Rate = static_cast<float>(countrytb.rate_)/100;//���ѵı���
	Nexttime1 = countrytb.nexttime1_;		//�´���Աѡ��ʱ��
	Nexttime2 = countrytb.nexttime2_;		//

	if(countrytb.mapid_ == 0)
	{
		ToMapID = countrytb.mapid_;
		Week = 0;
	}
	else
	{
		ToMapID = countrytb.mapid_/10;
		Week = countrytb.mapid_ - ToMapID * 10;
	}

	Council counciltb;
	Council *pCouncilList = 0;
	Council *pNewCouncile = 0;
	//��ѯ������Ա
	counciltb.setmasterid(CountryID,BOULE_DEPT);
	if(pDBProxy->db_multi_select(counciltb,buf,count) == DB_Proxy::DB_SUCCEED)
	{
		count = count/sizeof(Council);
		pCouncilList = (Council*)buf;
		for(i=0;i<count;i++)
		{
			pNewCouncile = new Council;
			memcpy(pNewCouncile,&pCouncilList[i],sizeof(Council));
			m_CouncilMap[pNewCouncile->playerid_] = pNewCouncile;
		}
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
	}
	
	//��ѯ������
	counciltb.setmasterid(CountryID,DOD_DEPT);
	if(pDBProxy->db_multi_select(counciltb,buf,count) == DB_Proxy::DB_SUCCEED)
	{
		count = count/sizeof(Council);
		pCouncilList = (Council*)buf;
		for(i=0;i<count;i++)
		{
			pNewCouncile = new Council;
			memcpy(pNewCouncile,&pCouncilList[i],sizeof(Council));
			m_DODMap[pNewCouncile->playerid_] = pNewCouncile;
		}
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
	}
	
	//��ѯ������
	counciltb.setmasterid(CountryID,INTERIOR_DEPT);
	if(pDBProxy->db_multi_select(counciltb,buf,count) == DB_Proxy::DB_SUCCEED)
	{
		count = count/sizeof(Council);
		pCouncilList = (Council*)buf;
		for(i=0;i<count;i++)
		{
			pNewCouncile = new Council;
			memcpy(pNewCouncile,&pCouncilList[i],sizeof(Council));
			m_InteriorMap[pNewCouncile->playerid_] = pNewCouncile;
		}
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
	}
	//��ѯ��ѡ��,��������������
	Candidate candidatetb;
	Candidate *pCandidateList = 0;
	Candidate *pNewCandidate = 0;
	candidatetb.setmasterid(CountryID,DOD_DEPT);
	if(pDBProxy->db_multi_select(candidatetb,buf,count) == DB_Proxy::DB_SUCCEED)
	{
		count = count/sizeof(Candidate);
		pCandidateList = (Candidate*)buf;
		for(i=0;i<count;i++)
		{
			pNewCandidate = new Candidate;
			memcpy(pNewCandidate,&pCandidateList[i],sizeof(Candidate));
			m_CandidateMap[pNewCandidate->playerid_] = pNewCandidate;
		}
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
	}
	//��������ѡ��
	candidatetb.setmasterid(CountryID,INTERIOR_DEPT);
	if(pDBProxy->db_multi_select(candidatetb,buf,count) == DB_Proxy::DB_SUCCEED)
	{
		count = count/sizeof(Candidate);
		pCandidateList = (Candidate*)buf;
		for(i=0;i<count;i++)
		{
			pNewCandidate = new Candidate;
			memcpy(pNewCandidate,&pCandidateList[i],sizeof(Candidate));
			m_CandidateMap[pNewCandidate->playerid_] = pNewCandidate;
		}
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
	}
	//��ѯѡƱm_BallotMap
	int iFlag = 0;
	Ballot ballottb;
	Ballot *pBallotList = 0;
	Ballot *pNewBallot = 0;
	int key = 0;
	ballottb.setmasterid(CountryID,DOD_DEPT);
	if(pDBProxy->db_multi_select(ballottb,buf,count) == DB_Proxy::DB_SUCCEED)
	{
		count = count/sizeof(Ballot);
		pBallotList = (Ballot*)buf;
		for(i=0;i<count;i++)
		{
			pNewBallot = new Ballot;
			memcpy(pNewBallot,&pBallotList[i],sizeof(Ballot));
			key = pNewBallot->playerid_ * 10 + DOD_DEPT;
			m_BallotMap[key] = pNewBallot;
			if(pNewBallot->value_ == DELATE_VOTE)
				iFlag++;//����
		}
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
	}
	//����
	ballottb.setmasterid(CountryID,INTERIOR_DEPT);
	if(pDBProxy->db_multi_select(ballottb,buf,count) == DB_Proxy::DB_SUCCEED)
	{
		count = count/sizeof(Ballot);
		pBallotList = (Ballot*)buf;
		for(i=0;i<count;i++)
		{
			pNewBallot = new Ballot;
			memcpy(pNewBallot,&pBallotList[i],sizeof(Ballot));
			key = pNewBallot->playerid_ * 10 + INTERIOR_DEPT;
			m_BallotMap[key] = pNewBallot;
			if(pNewBallot->value_ == DELATE_VOTE)
				iFlag++;//����
		}
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
	}

	GetTirrierCountOfHomeMap(pDBProxy);//��ѯ����������
	//���ö�ʱ��
	if(Nexttime2 <=0)
	{
#ifdef PRINT_HINTINFO
		printf("CountryID:%d has ended vote .\n",CountryID);
#endif
		DeleteAllBallot(pDBProxy);
	}
	else
	{
		if(Nexttime2 <IceUtil::Time::now().toSeconds())
		{//ѡ�ٽ���
#ifdef PRINT_HINTINFO
		printf("CountryID:%d ѡ�ٽ���.\n",CountryID);
#endif
		
			Consortia_Timer endmsg;
			if(iFlag == 0)
			{//ѡ�ٽ���
				endmsg.serialize(CC_C_TIMER_VOTEEND,CountryID);
				VoteEndTimer(&endmsg,pDBProxy);
			}
			else if(iFlag <(int)m_UnderWriteNum)
			{//ǩ������
				State = UNDERWRITE_STATE;
				endmsg.serialize(CC_C_TIMER_UNDERWRITE,CountryID);
				UnderWriteEndTimer(&endmsg,pDBProxy);
			}
			else
			{//����ͶƱ����
				State = DELATE_STATE;
				endmsg.serialize(CC_C_TIMER_DELATEEND,CountryID);
				DelateVoteEndTimer(&endmsg,pDBProxy);
			}
		}
		else
		{//����ѡ����
			long times = long(Nexttime2 - IceUtil::Time::now().toSeconds());
#ifdef PRINT_HINTINFO
		printf("CountryID:%d ����ѡ����(%ld- %d).\n",CountryID,times,iFlag);
#endif
			if(times <=0)
				times = 1;
			LPConsortia_Timer pTimerParam = new Consortia_Timer;
			if(iFlag ==0)
			{
				State = VOTE_STATE;
				pTimerParam->serialize(CC_C_TIMER_VOTEEND,CountryID);
			}
			else if(iFlag <5)
			{//CC_C_TIMER_UNDERWRITE,						//����ǩ��
				
				State = UNDERWRITE_STATE;
				pTimerParam->serialize(CC_C_TIMER_UNDERWRITE,CountryID);
			}
			else
			{
			//CC_C_TIMER_DELATEEND						//��������				
				State = DELATE_STATE;
				pTimerParam->serialize(CC_C_TIMER_DELATEEND,CountryID);
			}
			if(set_timer(times,pTimerParam) <0)
			{
				LOG_TRACE_EX(false,("CCountry::InitCountry-->���ö�ʱ��ʧ��:״̬��%d,��ʱ����%d",State,pTimerParam->command));
			}
		}
	}
	if(ToMapID)
	{
		long times = GetGoOutTime();//���������ʱ��
		m_TirrierGoOutParam = new Consortia_Timer;
		m_TirrierGoOutParam->serialize(CC_C_TIMER_TIRRIERGOOUT,CountryID);
		m_TirrierGoOutTimer = set_timer(times,m_TirrierGoOutParam);
	}
	return 0;
}

int CCountry::StartVote(DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wock(m_RWMutex);
	//������к�ѡ�˱�ͶƱ���������
	DeleteAllCandidate(pDBProxy);//ɾ�� ��ѡ�ˣ��������ݿ��е�����
	DeleteAllBallot(pDBProxy);//ɾ��ѡƱ���������ݿ��е�����
	SendEamil();//20090623�����ʼ�
	//����ѡ�ٽ����Ķ�ʱ����24СʱCC_C_TIMER_VOTEEND
	LPConsortia_Timer pTimerParam = new Consortia_Timer;
	pTimerParam->serialize(CC_C_TIMER_VOTEEND,CountryID);
	set_timer(VoteTimes,pTimerParam);
	ResetNextVoteTime(pDBProxy,VoteTimes);
	State = VOTE_STATE;
	return 0;
}
//��ѯ��ҹ�����Ϣ
int CCountry::QueryUserInfoOfCountry(LPQuery_UserOfCountry_Request msg,DB_Proxy *pDBProxy)
{
	//��ѯ�Ƿ�Ϊ��Ա����������������
	IceUtil::RWRecMutex::RLock rlock(m_RWMutex);
	Query_UserOfCountry_Response rep_msg;
	Council_Map_Iterator iter;
	iter = m_DODMap.find(msg->from);
	if(iter != m_DODMap.end())
	{
		rep_msg.serialize(msg->agent,msg->from,0,iter->second->type_,iter->second->postion_,Treasury,DODMoney,FinanceMoney);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 0;
	}
	iter = m_InteriorMap.find(msg->from);
	if(iter != m_InteriorMap.end())
	{
		rep_msg.serialize(msg->agent,msg->from,0,iter->second->type_,iter->second->postion_,Treasury,DODMoney,FinanceMoney);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 0;
	}
	iter = m_CouncilMap.find(msg->from);
	if(iter != m_CouncilMap.end())
	{//��Ա
		rep_msg.serialize(msg->agent,msg->from,0,iter->second->type_,iter->second->postion_,Treasury,DODMoney,FinanceMoney);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 0;
	}
	rep_msg.serialize(msg->agent,msg->from,111,0,0,Treasury,DODMoney,FinanceMoney);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	return 0;
}
//��ѯ����Ӫ����Ա�б�
int CCountry::QueryCouncilList(LPQuery_CouncilList_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::RLock rlock(m_RWMutex);
	/*
	Council_Map	m_CouncilMap;//��Ա�б�
	Council_Map	m_DODMap;//������
	Council_Map	m_InteriorMap;//������
	BOULE_DEPT,				//����Ժ
	DOD_DEPT,				//������
	INTERIOR_DEPT			//������
	*/
	int size = 0;
	int index = 0;
	Council_Map_Iterator iter;
	LPQuery_CouncilList_Response rep_msg = 0;
	char *pRepBuf = 0;
	if(msg->Type == -1 || msg->Type ==BOULE_DEPT)
	{
		size += m_CouncilMap.size();
	}
	if(msg->Type == -1 || msg->Type ==DOD_DEPT)
	{
		size += m_DODMap.size();
	}
	if(msg->Type == -1 || msg->Type ==INTERIOR_DEPT)
	{
		size += m_InteriorMap.size();
	}
	pRepBuf = new char[sizeof(Query_CouncilList_Response) + size * sizeof(Council_Info)];
	PlayerOfficerTable officertb;
	rep_msg = (LPQuery_CouncilList_Response)pRepBuf;
	if(size == 0)
	{
		
	}
	else
	{
		if(msg->Type == -1 || msg->Type ==BOULE_DEPT)
		{
			for(iter = m_CouncilMap.begin();iter != m_CouncilMap.end();iter++)
			{
				(*rep_msg)[index].PlayerID = iter->second->playerid_;
				strcpy((*rep_msg)[index].Name,iter->second->name_);
				(*rep_msg)[index].Type = iter->second->type_;
				(*rep_msg)[index].Postion = iter->second->postion_;
				officertb.setmasterid(iter->second->playerid_);
				officertb.setdetailid(iter->second->playerid_);
				if(pDBProxy->db_select(officertb) != DB_Proxy::DB_SUCCEED)
				{
					LOG_TRACE_EX(false,("CCountry::QueryCouncilList-->��ѯ���%d��Ϣʧ��",iter->second->playerid_));
				}
				(*rep_msg)[index].Level = officertb.level_;
				(*rep_msg)[index].MilitaryRank = officertb.militaryrank_;
				index++;
			}
		}
		if(msg->Type == -1 || msg->Type ==DOD_DEPT)
		{
			for(iter = m_DODMap.begin();iter != m_DODMap.end();iter++)
			{
				(*rep_msg)[index].PlayerID = iter->second->playerid_;
				strcpy((*rep_msg)[index].Name,iter->second->name_);
				(*rep_msg)[index].Type = iter->second->type_;
				(*rep_msg)[index].Postion = iter->second->postion_;
				officertb.setmasterid(iter->second->playerid_);
				officertb.setdetailid(iter->second->playerid_);
				if(pDBProxy->db_select(officertb) != DB_Proxy::DB_SUCCEED)
				{
					LOG_TRACE_EX(false,("CCountry::QueryCouncilList-->��ѯ���%d��Ϣʧ��",iter->second->playerid_));
				}
				(*rep_msg)[index].Level = officertb.level_;
				(*rep_msg)[index].MilitaryRank = officertb.militaryrank_;
				index++;
			}
		}
		if(msg->Type == -1 || msg->Type ==INTERIOR_DEPT)
		{
			for(iter = m_InteriorMap.begin();iter != m_InteriorMap.end();iter++)
			{
				(*rep_msg)[index].PlayerID = iter->second->playerid_;
				strcpy((*rep_msg)[index].Name,iter->second->name_);
				(*rep_msg)[index].Type = iter->second->type_;
				(*rep_msg)[index].Postion = iter->second->postion_;
				officertb.setmasterid(iter->second->playerid_);
				officertb.setdetailid(iter->second->playerid_);
				if(pDBProxy->db_select(officertb) != DB_Proxy::DB_SUCCEED)
				{
					LOG_TRACE_EX(false,("CCountry::QueryCouncilList-->��ѯ���%d��Ϣʧ��",iter->second->playerid_));
				}
				(*rep_msg)[index].Level = officertb.level_;
				(*rep_msg)[index].MilitaryRank = officertb.militaryrank_;
				index++;
			}
		}
	}
	rep_msg->serialize(msg->agent,msg->from,size,CountryID,CC_S_QUERY_COUNCILLIST);
	asynch_write(rep_msg->agent,(char*)rep_msg,rep_msg->length);
	delete[] pRepBuf;
	pRepBuf = 0;
	return 0;
}
//�μӾ�ѡ������Ϊ��Ա��
int CCountry::TakePartInElection(LPTakePartIn_Election_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	TakePartIn_Election_Response rep_msg;
	
	if(State != VOTE_STATE)
	{
		rep_msg.serialize(msg->agent,msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	//�����ж�����Ƿ�Ϊ��Ա
	Council_Map_Iterator council_iter;
	council_iter = m_CouncilMap.find(msg->from);//��Ա�б�
	if(council_iter == m_CouncilMap.end())
	{//����Ա
		rep_msg.serialize(msg->agent,msg->from,3);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	//�Ƿ��Ѳ�ѡ
	Candidate_Map_Iterator candidata_iter = m_CandidateMap.find(msg->from);//��ѡ���б�
	if(candidata_iter != m_CandidateMap.end())
	{
		rep_msg.serialize(msg->agent,msg->from,4);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	//�жϽ�Ǯ�Ƿ���
	if(msg->Money < VoteFee_Money)
	{//��ѡ���ù���
		rep_msg.serialize(msg->agent,msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	PlayerChatRoomTable playertb;
	playertb.setid(msg->from);
	if(pDBProxy->db_select(playertb) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(msg->agent,msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		LOG_TRACE_EX(false,("CCountry::TakePartInElection-->��ѯ���%d��Ϣʧ��",msg->from));
		return 1;
	}
	if(playertb.money_ < msg->Money)
	{//Money����
		rep_msg.serialize(msg->agent,msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	//�۳�Money
	memset(&playertb,0,sizeof(PlayerChatRoomTable));
	playertb.setid(msg->from);
	playertb.money_ = -msg->Money;
	if(pDBProxy->db_increase(playertb) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(msg->agent,msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		LOG_TRACE_EX(false,("CCountry::TakePartInElection-->�۳����%d��ѡ����ʧ��",msg->from));
		return 1;
	}
	//���շ�ϵͳ���Ϳ۳�GO�����Ϣ
	Notify_Election_Request notify_msg;
	notify_msg.serialize(msg->agent,msg->from,CountryID,msg->Dept,msg->Creed,msg->Money,VoteFee_GO);
	event_notify(MDM_GP_MAP_CHARGE,(char*)&notify_msg,notify_msg.length);
	return 0;
}
//��ѯ��ѡ���б�
int CCountry::QueryCandidateList(LPQuery_CandidateList_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::RLock rlock(m_RWMutex);
	int size = m_CandidateMap.size();
	if(State != VOTE_STATE && size != 0)
	{
		size = 0;
	}
	char *pRetBuf = new char[sizeof(Query_CandidateList_Response) + size * sizeof(Candidate_Info)];
	LPQuery_CandidateList_Response rep_msg = (LPQuery_CandidateList_Response)pRetBuf;
	int index = 0;
	int iTimes  = 0;

	if(State != VOTE_STATE)
	{
		iTimes = 0;
	}
	else
	{
		iTimes = int(Nexttime2 - IceUtil::Time::now().toSeconds());
	}
	if(size)
	{
		Candidate_Map_Iterator iter;
		PlayerOfficerTable officertb;
		for(iter =m_CandidateMap.begin();iter != m_CandidateMap.end();iter++)
		{
			if(msg->Type != -1 && iter->second->type_ != msg->Type)
				continue;
			(*rep_msg)[index].PlayerID = iter->second->playerid_;
			strcpy((*rep_msg)[index].Name,iter->second->name_);
			(*rep_msg)[index].Type = iter->second->type_;
			strcpy((*rep_msg)[index].Creed,iter->second->creed_);
			(*rep_msg)[index].Expense = iter->second->expense_;
			(*rep_msg)[index].Vote = iter->second->vote_;
			officertb.setmasterid(iter->second->playerid_);
			officertb.setdetailid(iter->second->playerid_);
			if(pDBProxy->db_select(officertb) != DB_Proxy::DB_SUCCEED)
			{
				LOG_TRACE_EX(false,("CCountry::QueryCandidateList-->��ѯ��ѡ��%d��Ϣʧ��",iter->second->playerid_));
			}
			(*rep_msg)[index].Level = officertb.level_;
			(*rep_msg)[index].MilitaryRank = officertb.militaryrank_;
			index++;
		}
	}
	rep_msg->serialize(msg->agent,msg->from,index,iTimes);
	asynch_write(rep_msg->agent,(char*)rep_msg,rep_msg->length);
	if(pRetBuf)
	{
		delete[] pRetBuf;
		pRetBuf = 0;
	}
	return 0;
}
//ͶƱ
int CCountry::PlayerBallot(LPPlayer_Ballot_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	if(State != VOTE_STATE)
		return 1;

	//�жϳ���ѡ�˺�ѡ�˵Ĳ�ѡְλ
	Player_Ballot_Response rep_msg;
	int key =0;
	Candidate_Map_Iterator iter = m_CandidateMap.find(msg->Candidate);
	if(iter == m_CandidateMap.end())
	{
		rep_msg.serialize(msg->agent,msg->from,1,msg->Candidate,0,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	Candidate *pCandidate = iter->second;
	//�ж��Ƿ���ͶƱ
	key = msg->from * 10 + pCandidate->type_;
	Ballot_Map_Iterator ballot_iter = m_BallotMap.find(key);
	if(ballot_iter != m_BallotMap.end())
	{
		rep_msg.serialize(msg->agent,msg->from,2,pCandidate->playerid_,pCandidate->type_,pCandidate->vote_);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	pCandidate->vote_++;
	if(pDBProxy->db_update(*pCandidate,*pCandidate) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(msg->agent,msg->from,111,pCandidate->playerid_,pCandidate->type_,pCandidate->vote_);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		LOG_TRACE_EX(false,("CCountry::PlayerBallot-->���%dͶƱѡ��%d,����%d������ѡƱ����ʧ��",msg->from,1,msg->Candidate,pCandidate->type_));
		return 1;
	}
	Ballot *pBallot = new Ballot;
	pBallot->setmasterid(CountryID,pCandidate->type_);
	pBallot->setdetailid(msg->from);
	pBallot->candidate_ = pCandidate->playerid_;
	pBallot->value_ = UPHOLD_VOTE;
	m_BallotMap[key] = pBallot;
	
	if(pDBProxy->db_insert(*pBallot) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(msg->agent,msg->from,111,pCandidate->playerid_,pCandidate->type_,pCandidate->vote_);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		LOG_TRACE_EX(false,("CCountry::PlayerBallot-->���%dͶƱѡ��%d,����%d������ѡƱʧ��",msg->from,1,msg->Candidate,pCandidate->type_));
		return 1;
	}

	rep_msg.serialize(msg->agent,msg->from,0,pCandidate->playerid_,pCandidate->type_,pCandidate->vote_);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	return 0;
}
//����
int CCountry::PlayerDelate(LPCountry_Delate_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	Country_Delate_Response rep_msg;
	if(State !=0 && State != UNDERWRITE_STATE)
	{
		rep_msg.serialize(msg->agent,msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;//״̬����
	}
		
	//Ȩ���ж�
	Council_Map_Iterator council_iter;
	Council *pCouncil = 0;
	int iTmpType = -1;

	council_iter = m_CouncilMap.find(msg->from);
	if(council_iter == m_CouncilMap.end())
	{//����Ա
		rep_msg.serialize(msg->agent,msg->from,3);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	if(msg->Type == DOD_DEPT)
	{
		council_iter = m_DODMap.find(msg->from);
		if(council_iter != m_DODMap.end())
		{//Ϊ�����򸱲���
			pCouncil = council_iter->second;
		}
	}
	else
	{
		council_iter = m_InteriorMap.find(msg->from);
		if(council_iter != m_InteriorMap.end())
		{//Ϊ�����򸱲���
			pCouncil = council_iter->second;
		}
	}
	if(pCouncil && pCouncil->postion_ == MINISTER_POSTION)
	{//Ϊ�������ܵ����Լ�
		rep_msg.serialize(msg->agent,msg->from,5);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	if(State ==0)
	{
		//��48Сʱ���ܵ���
		if(IceUtil::Time::now().toSeconds() + m_UnderWriteTimes + m_DelateVoteTimes + VoteTimes >= Nexttime1)
		{
			rep_msg.serialize(msg->agent,msg->from,4);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			return 1;
		}
		State = UNDERWRITE_STATE;
		DeleteAllBallot(pDBProxy);
		m_DelateType = msg->Type;
		//����ǩ��������ʱ��
		m_TimerParam = new Consortia_Timer;
		m_TimerParam->serialize(CC_C_TIMER_UNDERWRITE,CountryID);
		m_DelateTimer = set_timer(m_UnderWriteTimes,m_TimerParam);
		ResetNextVoteTime(pDBProxy,m_UnderWriteTimes);
		//�㲥��Ϣ,����ǩ������
		Broad_DelateState broadmsg;
		broadmsg.serialize(CountryID,m_DelateType,0);
		broadcast((char*)&broadmsg,broadmsg.length);
	}

	if(m_DelateType != msg->Type)
	{//�뵱ǰ�����еĵ�����һ��
		rep_msg.serialize(msg->agent,msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	int key =msg->from * 10 + m_DelateType;
	Ballot_Map_Iterator ballot_iter = m_BallotMap.find(key);
	if(ballot_iter != m_BallotMap.end())
	{
		rep_msg.serialize(msg->agent,msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	Ballot *pBallot = new Ballot;
	pBallot->setmasterid(CountryID,m_DelateType);
	pBallot->setdetailid(msg->from);
	pBallot->candidate_ = 0;
	pBallot->value_ = DELATE_VOTE;
	m_BallotMap[key] = pBallot;
	if(pDBProxy->db_insert(*pBallot) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(msg->agent,msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		LOG_TRACE_EX(false,(" CCountry::PlayerDelate-->�������%d�ĵ���ǩ��ͶƱ%dʧ�ܡ�",msg->from,DELATE_VOTE));
		return 1;
	}
	rep_msg.serialize(msg->agent,msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	//������5��ǩ��ʱ����ʱ��������ͶƱ
	if(m_BallotMap.size() >=m_UnderWriteNum)
	{
		DeleteAllBallot(pDBProxy);
		LOG_TRACE_EX((m_DelateTimer &&  m_TimerParam),("ָ��Ϊ��"));
		cancel_timer(m_DelateTimer);
		m_TimerParam->serialize(CC_C_TIMER_DELATEEND,CountryID);
		m_DelateTimer = set_timer(m_DelateVoteTimes,m_TimerParam);
		ResetNextVoteTime(pDBProxy,m_DelateVoteTimes);
		
		State = DELATE_STATE;
	}
	return 0;
}
//��ѯ������Ϣ
int CCountry::QueryDelateInfo(LPQuery_DelateInfo_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::RLock rlock(m_RWMutex);
	Query_DelateInfo_Response rep_msg;
	short iState = 0;
	if(m_DODMap.size() >0)
	{
		iState |= DODMINISTER_DEPTSTATE;
	}

	if(m_InteriorMap.size() >0)
	{
		iState |= INTERIOR_DEPTSTATE;
	}
	int Times = int(Nexttime2 - IceUtil::Time::now().toSeconds());
	if(State == UNDERWRITE_STATE)
	{
		rep_msg.serialize(msg->agent,msg->from,m_DelateType,Times,m_BallotMap.size(),0,0,0,iState);
	}
	else if(State == DELATE_STATE)
	{
		//��������Ʊ*3
		rep_msg.serialize(msg->agent,msg->from,m_DelateType,Times,m_UnderWriteNum,0,0,0,iState);
		Ballot_Map_Iterator ballot_iter;//m_BallotMap
		Ballot *pBallot = 0;
		Council_Map_Iterator council_iter;
		Council *pCouncil = 0;
		int count = 1;
		for(ballot_iter = m_BallotMap.begin();ballot_iter != m_BallotMap.end();ballot_iter++)
		{
			pCouncil = 0;
			pBallot = ballot_iter->second;
			if(m_DelateType == DOD_DEPT)
			{
				council_iter = m_DODMap.find(pBallot->playerid_);
				if(council_iter != m_DODMap.end())
				{
					pCouncil = council_iter->second;
				}
			}
			else if(m_DelateType == INTERIOR_DEPT)
			{
				council_iter = m_InteriorMap.find(pBallot->playerid_);
				if(council_iter != m_InteriorMap.end())
				{
					pCouncil = council_iter->second;
				}
			}
			if(pCouncil && pCouncil->postion_ == UNDERSECRETARY_POSTION)
			{
				count = 3;
			}
			else
			{
				count = 1;
			}
			if(pBallot->value_ == NONUSER_VOTE)
			{
				rep_msg.Nonuser +=count;
			}
			else if(pBallot->value_ == UPHOLD_VOTE)
			{
				rep_msg.Uphold +=count;
			}
			else if(pBallot->value_ == OPPOSE_VOTE)
			{
				rep_msg.Oppose +=count;
			}
		}
	}
	else
	{
		rep_msg.serialize(msg->agent,msg->from,-1,0,0,0,0,0,iState);
	}
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	return 0;
}
//����ͶƱ
int CCountry::DelateBallot(LPDelate_Ballot_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	Delate_Ballot_Response rep_msg;
	if(State != DELATE_STATE)
	{
		rep_msg.serialize(msg->agent,msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	int key =msg->from * 10 + m_DelateType;
	Ballot_Map_Iterator ballot_iter = m_BallotMap.find(key);
	if(ballot_iter != m_BallotMap.end())
	{//��ͶƱ
		rep_msg.serialize(msg->agent,msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	Council_Map_Iterator council_iter;
	Council *pCouncil = 0;
	int iTmpType = -1;

	council_iter = m_InteriorMap.find(msg->from);
	if(council_iter != m_InteriorMap.end())
	{
		pCouncil = council_iter->second;
		iTmpType = INTERIOR_DEPT;
	}
	else
	{
		council_iter = m_DODMap.find(msg->from);
		if(council_iter != m_DODMap.end())
		{
			pCouncil = council_iter->second;
			iTmpType = DOD_DEPT;
		}
		else
		{
			council_iter = m_CouncilMap.find(msg->from);
			if(council_iter != m_CouncilMap.end())
			{
				pCouncil = council_iter->second;
				iTmpType = BOULE_DEPT;
			}
		}
	}
	//ûȨ��
	if(pCouncil == NULL)
	{
		rep_msg.serialize(msg->agent,msg->from,3);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}

	//���Ϊ���� Or ������
	if(iTmpType == m_DelateType && pCouncil->postion_ == MINISTER_POSTION)
	{//�������ܲ���ͶƱ
		rep_msg.serialize(msg->agent,msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	Ballot *pBallot = new Ballot;
	pBallot->setmasterid(CountryID,m_DelateType);
	pBallot->setdetailid(msg->from);
	pBallot->candidate_ = 0;
	pBallot->value_ = msg->Value;
	m_BallotMap[key] = pBallot;
	if(pDBProxy->db_insert(*pBallot) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(msg->agent,msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		LOG_TRACE_EX(false,("CCountry::DelateBallot-->�������%d�ĵ���ͶƱ%dʧ�ܡ�",msg->from,msg->Value));
		return 1;
	}
	rep_msg.serialize(msg->agent,msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	return 0;
}
//�޸Ĳ�ѡ����
int CCountry::ModifyElectionCreed(LPModify_ElectionCreed_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	Modify_ElectionCreed_Response rep_msg;
	//�Ƿ��Ѳ�ѡ
	Candidate_Map_Iterator candidata_iter = m_CandidateMap.find(msg->from);//��ѡ���б�
	if(candidata_iter == m_CandidateMap.end())
	{
		rep_msg.serialize(msg->agent,msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	Candidate *pCandidate = candidata_iter->second;
	if(msg->Money >0)
	{//���Ӿ�ѡ����
		PlayerChatRoomTable playertb;
		playertb.setid(msg->from);
		if(pDBProxy->db_select(playertb) != DB_Proxy::DB_SUCCEED)
		{
			rep_msg.serialize(msg->agent,msg->from,111);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			LOG_TRACE(("CCountry::ModifyElectionCreed-->��ѯ���%dʧ��",msg->from));
			return 1;
		}
		if(playertb.money_ <msg->Money)
		{
			rep_msg.serialize(msg->agent,msg->from,2);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			return 1;
		}
		memset(&playertb,0,sizeof(PlayerChatRoomTable));
		playertb.setid(msg->from);
		playertb.money_ = -msg->Money;
		if(pDBProxy->db_increase(playertb) != DB_Proxy::DB_SUCCEED)
		{
			rep_msg.serialize(msg->agent,msg->from,111);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			LOG_TRACE(("CCountry::ModifyElectionCreed-->db_increase���%dʧ��",msg->from));
			return 1;
		}
		pCandidate->expense_ += msg->Money;
		//��ѡ���ü������
		Treasury += msg->Money;
		Country countrytb;
		countrytb.setid(CountryID);
		countrytb.treasury_ = msg->Money;
		if(pDBProxy->db_increase(playertb) != DB_Proxy::DB_SUCCEED)
		{
			rep_msg.serialize(msg->agent,msg->from,111);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			LOG_TRACE(("CCountry::ModifyElectionCreed-->Country���%dʧ��",msg->from));
			return 1;
		}
	}
	if(msg->Creed[0] !=0)
	{//�޸ĸ���
		strcpy(pCandidate->creed_,msg->Creed);
	}
	if(msg->Creed[0] != 0 || msg->Money >0)
	{
		if(pDBProxy->db_update(*pCandidate,*pCandidate) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE(("CCountry::ModifyElectionCreed-->db_update���%dʧ��",msg->from));
		}
		rep_msg.serialize(msg->agent,msg->from,0);
	}
	else
	{
		rep_msg.serialize(msg->agent,msg->from,111);
	}
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	return 0;
}

//������������������
int CCountry::CreateTirrier(LPCreateTirrier_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	CreateTirrier_Response rep_msg;
	//�����ж�Ȩ��
	Council_Map_Iterator iter =	m_DODMap.find(msg->from);
	if(iter == m_DODMap.end())
	{
		rep_msg.serialize(msg->agent,msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	if(iter->second->postion_ != MINISTER_POSTION)
	{
		rep_msg.serialize(msg->agent,msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	//�жϹ���������
	LPTirrierParam pTirrierParam = m_ConsortiaManage->GetTirrierParam(msg->TirrierType);
	if(pTirrierParam == NULL)
	{
		rep_msg.serialize(msg->agent,msg->from,3);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	//�ж�����
	if(m_TirrierCount[0] + m_TirrierCount[1] + m_TirrierCount[2] >=100)
	{
		rep_msg.serialize(msg->agent,msg->from,4);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	if(DODMoney < (unsigned int)pTirrierParam->Money)
	{
		rep_msg.serialize(msg->agent,msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	Country country;
	country.setid(CountryID);
	country.dodmoney_ = - pTirrierParam->Money;
	if(pDBProxy->db_increase(country) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE(("CCountry::CreateTirrier-->���%d����������ʱ�۳��ʽ�ʧ��",msg->from));
	}
	DODMoney -= pTirrierParam->Money;
	Notify_CreateTirrier_Request notify_msg;
	notify_msg.serialize(msg->agent,msg->from,CountryID,pTirrierParam->WeaponType,pTirrierParam->WeaponLevel,pTirrierParam->DefendType,pTirrierParam->DefendLevel,pTirrierParam->SoldierType,msg->TirrierType);
	if(event_notify(HomeMapID,(char*)&notify_msg,notify_msg.length) ==0)
	{
		rep_msg.serialize(msg->agent,msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		LOG_TRACE(("CCountry::CreateTirrier-->���%d����������ʱ֪ͨ����ͼ%dʧ��",msg->from,HomeMapID));
		return 1;
	}
	return 0;
}
//����������
int CCountry::TirrierGoOut(LPTirrierGoOut_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	TirrierGoOut_Response rep_msg;
	//�����ж�Ȩ��
	Council_Map_Iterator iter =	m_DODMap.find(msg->from);
	if(iter == m_DODMap.end())
	{
		rep_msg.serialize(msg->agent,msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	if(iter->second->postion_ != MINISTER_POSTION)
	{
		rep_msg.serialize(msg->agent,msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}

	Country country;
	country.setid(CountryID);
	country.mapid_ = ToMapID *10 + Week;
	ToMapID = msg->MapID;
	Week = msg->Week;
	country.mapid_ =ToMapID * 10 + Week - country.mapid_;
	if(pDBProxy->db_increase(country) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE(("CCountry::TirrierGoOut-->���%d������ʱ֪ͨ����ͼ%dʧ��",msg->from,HomeMapID));
		//return 1;
	}
	//assert(ToMapID == country.mapid_);
	rep_msg.serialize(msg->agent,msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	if(m_TirrierGoOutTimer)
	{
		cancel_timer(m_TirrierGoOutTimer);
		m_TirrierGoOutTimer = 0;
		delete m_TirrierGoOutParam;
		m_TirrierGoOutParam =0;
	}
	long times = GetGoOutTime();//���������ʱ��
	m_TirrierGoOutParam = new Consortia_Timer;
	m_TirrierGoOutParam->serialize(CC_C_TIMER_TIRRIERGOOUT,CountryID);
	m_TirrierGoOutTimer = set_timer(times,m_TirrierGoOutParam);
	//�㲥��Ϣ
	
	Broad_TirrierState broadmsg;
	broadmsg.serialize(CountryID,iter->second->name_,m_TirrerFee,CC_S_BROAD_TIRRIERGOOUT);
	broadcast((char*)&broadmsg,broadmsg.length);
	return 0;
}
//������������
int CCountry::UpgradeBuild(LPUpgradeBuild_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	UpgradeBuild_Response rep_msg;
	//�����ж�Ȩ��
	Council_Map_Iterator iter =	m_InteriorMap.find(msg->from);
	if(iter == m_InteriorMap.end())
	{
		rep_msg.serialize(msg->agent,msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	if(iter->second->postion_ != MINISTER_POSTION)
	{
		rep_msg.serialize(msg->agent,msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	if(FinanceMoney < (unsigned int)msg->Money)
	{
		rep_msg.serialize(msg->agent,msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	Country country;
	country.setid(CountryID);
	country.dodmoney_ = - msg->Money;
	if(pDBProxy->db_increase(country) != DB_Proxy::DB_EOF)
	{
		//assert(false);
	}
	DODMoney -= msg->Money;
	Notify_UpgradeBuild_Request notify_msg;
	notify_msg.serialize(msg->agent,msg->from,CountryID,msg->MapID,msg->BuildID,msg->Money);
	if(event_notify(msg->MapID,(char*)&notify_msg,notify_msg.length) ==0)
	{
		rep_msg.serialize(msg->agent,msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	}
	return 0;
}
//�޸���������
int CCountry::ModifyNotice(LPModifyNotice_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::RLock rlock(m_RWMutex);
	ModifyNotice_Response rep_msg;
	//�����ж�Ȩ��
	Council_Map_Iterator iter =	m_DODMap.find(msg->from);
	if(iter == m_DODMap.end())
	{
		rep_msg.serialize(msg->agent,msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	if(iter->second->postion_ != MINISTER_POSTION)
	{
		rep_msg.serialize(msg->agent,msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	Notify_ModifyNotice_Request notifymsg;
	notifymsg.serialize(msg->agent,msg->from,CountryID,msg->Notice);
	if(event_notify(MDM_GP_GAME_MANAGE_SERVER,(char*)&notifymsg,notifymsg.length))
	{
		rep_msg.serialize(msg->agent,msg->from,0);
	}
	else
	{
		rep_msg.serialize(msg->agent,msg->from,111);
	}
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	return 0;
}
//��ѯ����������
int CCountry::QueryTirrierCount(LPQuery_TirrierCount_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::RLock rlock(m_RWMutex);
	Query_TirrierCount_Response rep_msg;
	rep_msg[0] = m_TirrierCount[0];
	rep_msg[1] = m_TirrierCount[1];
	rep_msg[2] = m_TirrierCount[2];
	rep_msg.serialize(msg->agent,msg->from);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	return 0;
}
//ѡ�ٽ���
int CCountry::VoteEndTimer(LPConsortia_Timer msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	//ѡ�ٽ�����ͳ�Ƴ�ǰ5������һ��Ϊ����������4λΪ������
	DeleteAllBallot(pDBProxy);//ɾ��ѡƱ���������ݿ��е�����
	DeleteAllDODDept(pDBProxy);//ɾ����������Ա
	DeleteAllInteriorDept(pDBProxy);//ɾ����������Ա

	//��������������Ϣ
	
#ifdef PRINT_HINTINFO
	printf("%s����%dѡ�ٽ���\n",IceUtil::Time::now().toDateTime().c_str(),CountryID);
#endif
	/*if(m_CandidateMap.size() <=0)
		return 1;*/

	int iFlag = 0;
	Candidate_Set dod_set,interior_set;
	Candidate_Set_Iterator set_iter;
	Candidate_Map_Iterator iter;
	for(iter = m_CandidateMap.begin();iter != m_CandidateMap.end();iter++)
	{
		if(iter->second->type_ == DOD_DEPT)
			dod_set.insert(iter->second);
		else if(iter->second->type_ == INTERIOR_DEPT)
			interior_set.insert(iter->second);
	}
	int index = 0;
	Council_Map_Iterator council_iter;
	Council *pCouncil = 0;
	Council *pOldCouncil = 0;

	Notify_Send_Mail playeremail;
	Notify_BroadMail broademail;
	char szStartTime[16] = {0};
	char szEndTime[16] = {0};
	int iYear =0,iMon = 0 ,iDay =0 ,iHour =0 ,iMin =0,iSec = 0;
	__int64 iBaseTime = IceUtil::Time::Date(2000, 1, 1).toSeconds();
	__int64 iCurTime = IceUtil::Time::now().toSeconds();
	game_calendar::calendar cal_start(iCurTime -iBaseTime);
	cal_start.get_game_calendar(iYear,iMon,iDay,iHour,iMin,iSec);
	sprintf(szStartTime,"%d��%d��%d��",iYear,iMon,iDay);
	game_calendar::calendar cal_end(iCurTime + HoldTimes -iBaseTime);
	iYear = iMon = iDay = iHour = iMin = iSec = 0;
	cal_end.get_game_calendar(iYear,iMon,iDay,iHour,iMin,iSec);
	sprintf(szEndTime,"%d��%d��%d��",iYear,iMon,iDay);
	LPMailInfoParam_Item pMailInfoParam = 0;
	LPCareerConfig_Item pCareerConfig = 0;
	pMailInfoParam = m_ConsortiaManage->GetMailInfoParam(CountryID,MAIL_COUNTRY_DOD_ELECTION);
	char szContentbuf[SIZE_256] = {0};
	for(set_iter = dod_set.begin();set_iter != dod_set.end() && index <5;set_iter++)
	{
		//m_CouncilMap
		council_iter = m_CouncilMap.find((*set_iter)->playerid_);
		if(council_iter == m_CouncilMap.end())
		{
			//assert(false);
			continue;
		}
		pOldCouncil = council_iter->second;
		if(pOldCouncil == NULL)
		{
			//assert(false);
			continue;
		}

		pCouncil = new Council;
		memcpy(pCouncil,pOldCouncil,sizeof(Council));
		pCouncil->type_ = DOD_DEPT;
		pCouncil->vote_ = (*set_iter)->vote_;
		if(index ==0)
		{
			pCouncil->postion_ = MINISTER_POSTION;//����
		}
		else
		{
			pCouncil->postion_ = UNDERSECRETARY_POSTION;//������
		}
		//����������б�
		m_DODMap[pCouncil->playerid_] = pCouncil;

		if(pDBProxy->db_insert(*pCouncil) != DB_Proxy::DB_SUCCEED)
		{
			//assert(false);
		}
#ifdef PRINT_HINTINFO
		printf("����%d,�����������%d,ְλ%d\n",CountryID,pCouncil->playerid_,pCouncil->postion_);
#endif
		index++;
		iFlag = 1;
		//20090622�����ʼ�
		pCareerConfig = m_ConsortiaManage->GetCareerConfig(DOD_DEPT,pCouncil->postion_);
		if(pMailInfoParam)
		{
			playeremail.type = Normal;
			playeremail.ulRecieverPlayerID = pCouncil->playerid_;
			strcpy(playeremail.szSendPlayerName,"����ϵͳ");
			strcpy(playeremail.sztitle,pMailInfoParam->Title);
			//strcpy(playeremail.szbody,pMailInfoParam->Content);
			StrReplace(pMailInfoParam->Content,playeremail.szbody,"$startime",szStartTime);
			StrReplace(playeremail.szbody,szContentbuf,"$endtime",szEndTime);
			if(pCareerConfig)
			{
				StrReplace(szContentbuf,playeremail.szbody,"$jobname",pCareerConfig->JobName);
			}
			else
			{
				StrReplace(szContentbuf,playeremail.szbody,"$jobname","�ʵ�");
				//assert(false);
			}
			playeremail.serialize();
			event_notify(MDM_GP_MAP_CHAT,(char*)&playeremail,playeremail.length);
		}

		if(pCouncil->postion_ == MINISTER_POSTION)
		{//Ⱥ���ʼ�MAIL_COUNTRY_DOD_VOTEEND
			LPMailInfoParam_Item pBroadMailParam = m_ConsortiaManage->GetMailInfoParam(CountryID,MAIL_COUNTRY_DOD_VOTEEND);
			if(pBroadMailParam)
			{
				broademail.CountryID = CountryID;
				strcpy(broademail.szSendPlayerName,"����ϵͳ");
				strcpy(broademail.sztitle,pBroadMailParam->Title);

				StrReplace(pBroadMailParam->Content,szContentbuf,"$string",pCouncil->name_);
				StrReplace(szContentbuf,broademail.szbody,"$startime",szStartTime);
				StrReplace(broademail.szbody,szContentbuf,"$endtime",szEndTime);
				if(pCareerConfig)
				{
					StrReplace(szContentbuf,broademail.szbody,"$jobname",pCareerConfig->JobName);
				}
				else
				{
					StrReplace(szContentbuf,broademail.szbody,"$jobname","�ʵ�");
				}
				broademail.serialize();
				event_notify(MDM_GP_MAP_CHAT,(char*)&broademail,broademail.length);
			}
		}
	}
	dod_set.clear();
	index = 0;

	pMailInfoParam = m_ConsortiaManage->GetMailInfoParam(CountryID,MAIL_COUNTRY_INTERIOR_ELECTION);
	
	for(set_iter = interior_set.begin();set_iter != interior_set.end() && index <5;set_iter++)
	{
		//m_CouncilMap
		council_iter = m_CouncilMap.find((*set_iter)->playerid_);
		if(council_iter == m_CouncilMap.end())
		{
			//assert(false);
			continue;
		}
		pOldCouncil = council_iter->second;
		if(pOldCouncil == NULL)
		{
			//assert(false);
			continue;
		}

		pCouncil = new Council;
		memcpy(pCouncil,pOldCouncil,sizeof(Council));

		pCouncil->type_ = INTERIOR_DEPT;
		pCouncil->vote_ = (*set_iter)->vote_;
		if(index ==0)
		{
			pCouncil->postion_ = MINISTER_POSTION;//����
		}
		else
		{
			pCouncil->postion_ = UNDERSECRETARY_POSTION;//������
		}
		//�����������б�
		m_InteriorMap[pCouncil->playerid_] = pCouncil;

		if(pDBProxy->db_insert(*pCouncil) != DB_Proxy::DB_SUCCEED)
		{
			//assert(false);
		}
#ifdef PRINT_HINTINFO
		printf("����%d,�����������%d,ְλ%d\n",CountryID,pCouncil->playerid_,pCouncil->postion_);
#endif
		index++;
		iFlag = 1;
		//20090622�����ʼ�
		pCareerConfig = m_ConsortiaManage->GetCareerConfig(INTERIOR_DEPT,pCouncil->postion_);
		if(pMailInfoParam)
		{
			playeremail.type = Normal;
			playeremail.ulRecieverPlayerID = pCouncil->playerid_;
			strcpy(playeremail.szSendPlayerName,"����ϵͳ");
			strcpy(playeremail.sztitle,pMailInfoParam->Title);
			//strcpy(playeremail.szbody,pMailInfoParam->Content);
			StrReplace(pMailInfoParam->Content,playeremail.szbody,"$startime",szStartTime);
			StrReplace(playeremail.szbody,szContentbuf,"$endtime",szEndTime);
			if(pCareerConfig)
			{
				StrReplace(szContentbuf,playeremail.szbody,"$jobname",pCareerConfig->JobName);
			}
			else
			{
				StrReplace(szContentbuf,playeremail.szbody,"$jobname","�ʵ�");
				//assert(false);
			}
			playeremail.serialize();
			event_notify(MDM_GP_MAP_CHAT,(char*)&playeremail,playeremail.length);
		}

		if(pCouncil->postion_ == MINISTER_POSTION)
		{//Ⱥ���ʼ�MAIL_COUNTRY_DOD_VOTEEND
			LPMailInfoParam_Item pBroadMailParam = m_ConsortiaManage->GetMailInfoParam(CountryID,MAIL_COUNTRY_INTERIOR_VOTEEND);
			if(pBroadMailParam)
			{
				broademail.CountryID = CountryID;
				strcpy(broademail.szSendPlayerName,"����ϵͳ");
				strcpy(broademail.sztitle,pBroadMailParam->Title);

				StrReplace(pBroadMailParam->Content,szContentbuf,"$string",pCouncil->name_);
				StrReplace(szContentbuf,broademail.szbody,"$startime",szStartTime);
				StrReplace(broademail.szbody,szContentbuf,"$endtime",szEndTime);
				if(pCareerConfig)
				{
					StrReplace(szContentbuf,broademail.szbody,"$jobname",pCareerConfig->JobName);
				}
				else
				{
					StrReplace(szContentbuf,broademail.szbody,"$jobname","�ʵ�");
				}
				broademail.serialize();
				event_notify(MDM_GP_MAP_CHAT,(char*)&broademail,broademail.length);
			}
		}
	}
	interior_set.clear();

	DeleteAllCandidate(pDBProxy);//ɾ�� ��ѡ�ˣ��������ݿ��е�����
	//�����ʽ�
	unsigned int money = 0;
	Country countrytb;
	countrytb.setid(CountryID);
	countrytb.nexttime2_ = -Nexttime2;
	if(iFlag)
	{
		money = Treasury * Rate;
		countrytb.treasury_ = -(int)money - (int)money;
		countrytb.dodmoney_ += money;
		countrytb.financemoney_ += money;
		Treasury = Treasury - money - money;
		DODMoney += money;		//����
		FinanceMoney += money;	//��������
	}

	if(pDBProxy->db_increase(countrytb) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("CCountry::VoteEndTimer-->��������ʧ��"));
	}
	Nexttime2 = 0;
	State = 0;

	//�㲥��Ϣ
	BroadCastCouncilList(pDBProxy,1,CC_S_BROAD_VOTEND);
	return 0;
}
//����ǩ������
int CCountry::UnderWriteEndTimer(LPConsortia_Timer msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	if(State != UNDERWRITE_STATE)
		return 1;

	Broad_DelateState broadmsg;
	if(m_BallotMap.size() <m_UnderWriteNum)
	{//ǩ��������������������
		m_DelateTimer = 0;
		ResetNextVoteTime(pDBProxy,-1);
		//Nexttime2 = 0;
		State = 0;
		m_TimerParam = 0;

		//ǩ�������㲥������ʧ��
		//�㲥��Ϣ,����ǩ������
		broadmsg.serialize(CountryID,m_DelateType,3);
	}
	else
	{//����ͶƱ����
		DeleteAllBallot(pDBProxy);
		m_TimerParam = new Consortia_Timer;
		m_TimerParam->serialize(CC_C_TIMER_DELATEEND,CountryID);
		m_DelateTimer = set_timer(m_DelateVoteTimes,m_TimerParam);
		ResetNextVoteTime(pDBProxy,m_DelateVoteTimes);
		//����ͶƱ��ʼ�㲥��Ϣ
		broadmsg.serialize(CountryID,m_DelateType,1);
		State = DELATE_STATE;
	}
	broadcast((char*)&broadmsg,broadmsg.length);
	return 0;
}
//����ͶƱ����
int CCountry::DelateVoteEndTimer(LPConsortia_Timer msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	if(State != DELATE_STATE)
		return 1;

	//���͹㲥��Ϣ����������
	Broad_DelateState broadmsg;
	broadmsg.serialize(CountryID,m_DelateType,2);
	broadcast((char*)&broadmsg,broadmsg.length);
	//ͳ��
	int nonuser =0,uphold=0,oppose=0;
	Ballot_Map_Iterator ballot_iter;//m_BallotMap
	Ballot *pBallot = 0;
	Council_Map_Iterator council_iter;
	Council *pCouncil = 0;
	int count = 1;
	for(ballot_iter = m_BallotMap.begin();ballot_iter != m_BallotMap.end();ballot_iter++)
	{
		//pBallot = ballot_iter->second;
		pCouncil = 0;
		pBallot = ballot_iter->second;
		if(m_DelateType == DOD_DEPT)
		{
			council_iter = m_DODMap.find(pBallot->playerid_);
			if(council_iter != m_DODMap.end())
			{
				pCouncil = council_iter->second;
			}
		}
		else if(m_DelateType == INTERIOR_DEPT)
		{
			council_iter = m_InteriorMap.find(pBallot->playerid_);
			if(council_iter != m_InteriorMap.end())
			{
				pCouncil = council_iter->second;
			}
		}
		if(pCouncil && pCouncil->postion_ == UNDERSECRETARY_POSTION)
		{
			count = 3;
		}
		else
		{
			count = 1;
		}
		if(pBallot->value_ == NONUSER_VOTE)
		{
			nonuser +=count;
		}
		else if(pBallot->value_ == UPHOLD_VOTE)
		{
			uphold +=count;
		}
		else if(pBallot->value_ == OPPOSE_VOTE)
		{
			oppose +=count;
		}
	}
	if(uphold > oppose)
	{//�����ɹ�
		Council *pCouncil = 0;
		Council *pNextCouncil = 0;
		int votecount = -1;
		Council_Map_Iterator iter;
		if(m_DelateType == DOD_DEPT)
		{/*
		 Council_Map	m_DODMap;//������
		 Council_Map	m_InteriorMap;//������
		 */
			for(iter = m_DODMap.begin();iter != m_DODMap.end();iter++)
			{
				if(iter->second->postion_ == MINISTER_POSTION)
				{
					pCouncil = iter->second;
				}
				else
				{
					if(iter->second->vote_ >votecount)
					{
						pNextCouncil = iter->second;
						votecount = iter->second->vote_ ;
					}
				}
			}
			if(pCouncil)
				m_DODMap.erase(pCouncil->playerid_);			
		}
		else
		{
			for(iter = m_InteriorMap.begin();iter != m_InteriorMap.end();iter++)
			{
				if(iter->second->postion_ == MINISTER_POSTION)
				{
					pCouncil = iter->second;
				}
				else
				{
					if(iter->second->vote_ >votecount)
					{
						pNextCouncil = iter->second;
						votecount = iter->second->vote_ ;
					}
				}
			}
			if(pCouncil)
				m_InteriorMap.erase(pCouncil->playerid_);	
		}
		if(pNextCouncil)
		{
			pNextCouncil->postion_ = MINISTER_POSTION;
			if(pDBProxy->db_update(*pNextCouncil,*pNextCouncil) != DB_Proxy::DB_SUCCEED)
			{
				LOG_TRACE_EX(false,("CCountry::DelateVoteEndTimer-->��Ӫ%d��%d�²�������%s��",CountryID,m_DelateType,pNextCouncil->playerid_));
			}
		}
		if(pCouncil == NULL)
		{
			//assert(false);
			return 1;
		}

		Council delCouncil;
		delCouncil.setmasterid(CountryID,pCouncil->type_);
		delCouncil.setdetailid(pCouncil->playerid_);
		if(pDBProxy->db_delete(delCouncil) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE_EX(false,("CCountry::DelateVoteEndTimer-->ɾ����Ӫ%d��%d����%s��",CountryID,m_DelateType,pCouncil->playerid_));
		}
		//�����ɹ����㲥��Ϣ
		Broad_ExchangeMinister broad_minister;
		if(pNextCouncil)
		{
			broad_minister.serialize(CountryID,m_DelateType,pCouncil->name_,pNextCouncil->name_);
		}
		else
		{
			broad_minister.serialize(CountryID,m_DelateType,pCouncil->name_,0);
		}
		broadcast((char*)&broad_minister,broad_minister.length);
		delete pCouncil;
		pCouncil = 0;
	}
	else
	{//����ʧ��
	}
	DeleteAllBallot(pDBProxy);
	ResetNextVoteTime(pDBProxy,-1);
	//Nexttime2 = 0;
	State = 0;
	m_TimerParam = 0;
	//�㲥��Ϣ
	return 0;
}
int CCountry::ElectionFeeNotify(LPNotify_Election_Response msg,DB_Proxy *pDBProxy)
{
	TakePartIn_Election_Response rep_msg;
	PlayerChatRoomTable playertb;
	if(msg->Error_Code !=0)
	{
		rep_msg.serialize(msg->agent,msg->from,msg->Error_Code);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		//����Money
		playertb.setid(msg->from);
		playertb.money_ = msg->Money;
		if(pDBProxy->db_increase(playertb) != DB_Proxy::DB_SUCCEED)
		{
			rep_msg.serialize(msg->agent,msg->from,111);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			//assert(false);
		}
		return 1;
	}
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	Candidate *pNewCandidate = new Candidate;
	pNewCandidate->countryid_ = CountryID;
	pNewCandidate->type_ = msg->Dept;
	pNewCandidate->playerid_ = msg->from;
	int country = 0;
	bool bIsOnline = false;
	int agent = 0;
	int playerid = msg->from;
	if(m_ConsortiaManage->GetPlayerInfo(msg->from,pNewCandidate->name_,country,bIsOnline,agent,pDBProxy))
	{
		delete pNewCandidate;
		pNewCandidate  = 0;
		return 1;
	}
	strcpy(pNewCandidate->creed_,msg->Creed);
	pNewCandidate->expense_ = msg->Money;
	pNewCandidate->vote_ = 0;
	pNewCandidate->time_ = IceUtil::Time::now().toSeconds();
	m_CandidateMap[msg->from] = pNewCandidate;//�����б�

	if(pDBProxy->db_insert(*pNewCandidate) != DB_Proxy::DB_SUCCEED)
	{
		//assert(false);
	}
	//��ѡ���ü������
	Treasury += msg->Money;
	Country countrytb;
	countrytb.setid(CountryID);
	countrytb.treasury_ = msg->Money;
	if(pDBProxy->db_increase(countrytb) != DB_Proxy::DB_SUCCEED)
	{
		//assert(false);
	}
	rep_msg.serialize(msg->agent,msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);

	//����XX��ѡXX����ѡ����XX�Ĺ㲥��Ϣ
	Broad_Election_Response broadmsg;
	broadmsg.serialize(CountryID,pNewCandidate->name_,msg->Dept,msg->Money);
	broadcast((char*)&broadmsg,broadmsg.length);

	playertb.setid(msg->from);
	pDBProxy->db_select(playertb);
	ISystemLog::getInstance()->playerMoney_Election_Log(msg->from,pNewCandidate->name_,
		playertb.money_ + msg->Money,playertb.money_,msg->Money,msg->Dept);
	return 0;
}
//����ͼ���ش�����������֪ͨ��Ϣ
int CCountry::CreateTirrierNotify(LPNotify_CreateTirrier_Response msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	CreateTirrier_Response rep_msg;
	if(msg->Error_Code !=0)
	{//����ʧ�ܣ����ؽ�Ǯ
		Country country;
		country.setid(CountryID);
		country.dodmoney_ =  m_TirrerFee;
		if(pDBProxy->db_increase(country) != DB_Proxy::DB_EOF)
		{
			//assert(false);
		}
		DODMoney = m_TirrerFee;
		rep_msg.serialize(msg->agent,msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	m_TirrierCount[msg->TirrierType]++;
	//�����ɹ�
	rep_msg.serialize(msg->agent,msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);

	Query_TirrierCount_Response rep_countmsg;
	rep_countmsg[0] = m_TirrierCount[0];
	rep_countmsg[1] = m_TirrierCount[1];
	rep_countmsg[2] = m_TirrierCount[2];
	rep_countmsg.serialize(msg->agent,msg->from);
	asynch_write(rep_countmsg.agent,(char*)&rep_countmsg,rep_countmsg.length);
	//���͹㲥��Ϣ
	Council_Map_Iterator iter = m_DODMap.find(msg->from);
	if(iter == m_DODMap.end())
	{
		//assert(false);
		return 2;
	}
	Broad_TirrierState broadmsg;
	broadmsg.serialize(CountryID,iter->second->name_,m_TirrerFee,CC_S_BROAD_ADDARMY);
	broadcast((char*)&broadmsg,broadmsg.length);
	return 0;
}
//����ͼ������������������֪ͨ��Ϣ
int CCountry::UpgradeBuildNotify(LPNotify_UpgradeBuild_Response msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	UpgradeBuild_Response rep_msg;
	if(msg->Error_Code !=0)
	{//����ʧ�ܣ����ؽ�Ǯ
		Country country;
		country.setid(CountryID);
		country.dodmoney_ =  msg->Money;
		if(pDBProxy->db_increase(country) != DB_Proxy::DB_EOF)
		{
			//assert(false);
		}
		FinanceMoney = msg->Money;
		rep_msg.serialize(msg->agent,msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	//���׳ɹ�

	rep_msg.serialize(msg->agent,msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	//���͹㲥��Ϣ
	Council_Map_Iterator iter = m_InteriorMap.find(msg->from);
	if(iter == m_InteriorMap.end())
	{
		//assert(false);
		return 2;
	}
	Broad_UpgradeBuild broadmsg;
	broadmsg.serialize(CountryID,iter->second->name_,msg->MapID,msg->BuildID,msg->Money,msg->Level);
	broadcast((char*)&broadmsg,broadmsg.length);
	return 0;
}
//������������ʱ��
int CCountry::TirrierGoOutTimer(LPConsortia_Timer msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	if(ToMapID <1000 || ToMapID >1060)
		return 1;

	//�����ִ��ȡ��������
	ArmyTable army;
	ArmyTable *pArmyList = 0;
	char *buf = 0;
	int count = 0;
	army.setmasterid(HomeMapID);
	if(pDBProxy->db_multi_select(army,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		return 1;
	}
	count = count/sizeof(ArmyTable);
	pArmyList = (ArmyTable*)buf;
	//Notify_TirrierGoOut_Request notifymsg;
	REQ_ARMY_GO_OUT_STRUCT armyoutmsg;
	int playerid = 100;
	if(CountryID == 1)
		playerid = 100;
	else
		playerid = 200;

	for(int i=0;i<count;i++)
	{
		//notifymsg.serialize(HomeMapID,pArmyList[i].armyid_,ToMapID);
		//event_notify(SERVER_BOUND_MAP_OTHER,(char*)&notifymsg,notifymsg.length);
		if(pArmyList[i].playerid_ != playerid ||pArmyList[i].state_ ==2)
			continue;

		armyoutmsg.serialize(playerid,HomeMapID,pArmyList[i].armyid_,ToMapID,0);
		armyoutmsg.agent = 0;
		event_notify(HomeMapID,(char*)&armyoutmsg,armyoutmsg.length);
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	Country country;
	country.setid(CountryID);
	country.mapid_ = ToMapID *10 + Week;
	ToMapID = 0;
	Week = 0;
	country.mapid_ =-country.mapid_;

	if(pDBProxy->db_increase(country) != DB_Proxy::DB_SUCCEED)
	{
		//assert(false);
	}
	m_TirrierGoOutParam = 0;
	return 0;
}
//������������
int CCountry::ModifyTirrierState(LPNotify_ModifyArmyState msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	int iTirrierType = m_ConsortiaManage->GetTirrierType(msg->armyInfo.Fweapon);
	if (iTirrierType >=0)
	{
		m_TirrierCount[iTirrierType]--;
		if (m_TirrierCount[iTirrierType] <0)
		{
			m_TirrierCount[iTirrierType] = 0;
		}
	}
	return 0;
}
//�г���˰
int CCountry::MarketRate(LPNotify_MarketRate_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	Country country;
	country.setid(CountryID);
	country.treasury_ = msg->Money;
	if(pDBProxy->db_increase(country) != DB_Proxy::DB_SUCCEED)
	{
		//assert(false);
		return 1;
	}
	Treasury += msg->Money;
	return 0;
}
int CCountry::DeleteAllCouncils(DB_Proxy *pDBProxy)
{
	Council_Map_Iterator iter;
	Council *pDelCouncil = 0;
	for(iter = m_CouncilMap.begin();iter != m_CouncilMap.end();iter++)
	{
		if(pDelCouncil = iter->second)
		{
			pDBProxy->db_delete(*pDelCouncil);
			delete pDelCouncil;
			pDelCouncil = 0;
		}
	}
	m_CouncilMap.clear();
	return 0;
}
//ɾ�� ��ѡ�ˣ��������ݿ��е�����
int CCountry::DeleteAllCandidate(DB_Proxy *pDBProxy)
{//m_CandidateMap
	Candidate_Map_Iterator iter;
	Candidate *pDelCandidate = 0;
	for(iter = m_CandidateMap.begin();iter != m_CandidateMap.end();iter++)
	{
		if(pDelCandidate = iter->second)
		{
			pDBProxy->db_delete(*pDelCandidate);
			delete pDelCandidate;
			pDelCandidate = 0;
		}
	}
	m_CandidateMap.clear();
	return 0;
}
//ɾ��ѡƱ���������ݿ��е�����
int CCountry::DeleteAllBallot(DB_Proxy *pDBProxy)
{//m_BallotMap
	Ballot_Map_Iterator iter;
	Ballot *pDelBallot = 0;
	for(iter = m_BallotMap.begin();iter != m_BallotMap.end();iter++)
	{
		if(pDelBallot = iter->second)
		{
			pDBProxy->db_delete(*pDelBallot);
			delete pDelBallot;
			pDelBallot = 0;
		}
	}
	m_BallotMap.clear();
	return 0;
}
//ɾ����������Ա
int CCountry::DeleteAllDODDept(DB_Proxy *pDBProxy)
{//m_DODMap
	Council_Map_Iterator iter;
	Council *pDelCouncil = 0;
	for(iter = m_DODMap.begin();iter != m_DODMap.end();iter++)
	{
		if(pDelCouncil = iter->second)
		{
			pDBProxy->db_delete(*pDelCouncil);
			delete pDelCouncil;
			pDelCouncil = 0;
		}
	}
	m_DODMap.clear();
	return 0;
}
//ɾ����������Ա
int CCountry::DeleteAllInteriorDept(DB_Proxy *pDBProxy)
{
	Council_Map_Iterator iter;
	Council *pDelCouncil = 0;
	for(iter = m_InteriorMap.begin();iter != m_InteriorMap.end();iter++)
	{
		if(pDelCouncil = iter->second)
		{
			pDBProxy->db_delete(*pDelCouncil);
			delete pDelCouncil;
			pDelCouncil = 0;
		}
	}
	m_InteriorMap.clear();
	return 0;
}
//���õ���
int CCountry::ResetDelateInfo(DB_Proxy *pDBProxy)
{
	if(m_DelateTimer)
	{
		cancel_timer(m_DelateTimer);
		m_DelateTimer = 0;
	}
	DeleteAllBallot(pDBProxy);
	return 0;
}
//��ѯ����������
int CCountry::GetTirrierCount(DB_Proxy *pDBProxy)
{
	ArmyTable army;
	ArmyTable *pArmyList = 0;
	char *buf = 0;
	int count = 0;
	char *armybuf = 0;
	int armycount = 0;
	int i=0;
	int j =0;
	MapInfoTable maptb;
	MapInfoTable *pMapList = 0;
	if(pDBProxy->db_select_all(maptb,buf,count) == DB_Proxy::DB_SUCCEED)
	{
		count = count/sizeof(MapInfoTable);
		pMapList = (MapInfoTable*)buf;
		for(i=0;i<count;i++)
		{
			/*
			if(pMapList[i].state_ != war && pMapList[i].state_ != 100)//��ս�����ͷ����ִ忤����ƽ����Ӧ���й�������
				continue;
			if(pMapList[i].state_ == 100 && pMapList[i].countryid_ != CountryID)
				continue;
				*/
			if(pMapList[i].state_ == 100 && pMapList[i].mapid_ != HomeMapID)//�Ǳ������ִ�
				continue;
			army.setmasterid(pMapList[i].mapid_);
			if(pDBProxy->db_multi_select(army,armybuf,armycount) == DB_Proxy::DB_SUCCEED)
			{
				armycount = armycount/sizeof(ArmyTable);
				pArmyList = (ArmyTable*)armybuf;
				int playerid = CountryID * 100;
				for(j=0;j<armycount;j++)
				{
					if(pArmyList[j].playerid_ != playerid)
						continue;
					if(pArmyList[j].fweapon_ <= 15)
						m_TirrierCount[0]++;
					else if(pArmyList[j].fweapon_ <= 25)
						m_TirrierCount[1]++;
					else if(pArmyList[j].fweapon_ <= 45)
						m_TirrierCount[2]++;
				}
				if(armybuf)
				{
					delete[] armybuf;
					armybuf = 0;
				}
			}
		}
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
#ifdef PRINT_HINTINFO
	printf("սʿ:%d,������:%d,��ʦ:%d\n",m_TirrierCount[0],m_TirrierCount[1],m_TirrierCount[2]);
#endif
	return 0;
}
//��ѯ����������
int CCountry::GetTirrierCountOfHomeMap(DB_Proxy *pDBProxy)
{
	ArmyTable army;
	ArmyTable *pArmyList = 0;
	char *armybuf = 0;
	int armycount = 0;
	int i=0;
	int j =0;
	army.setmasterid(HomeMapID);
	int playerid = CountryID * 100;
	int type = 0;
	if(pDBProxy->db_multi_select(army,armybuf,armycount) == DB_Proxy::DB_SUCCEED)
	{
		armycount = armycount/sizeof(ArmyTable);
		pArmyList = (ArmyTable*)armybuf;
		for(j=0;j<armycount;j++)
		{
			if(pArmyList[j].playerid_ != playerid)
				continue;
			addTirrierCountFromWeapon(pArmyList[j].fweapon_);
		}
		if(armybuf)
		{
			delete[] armybuf;
			armybuf = 0;
		}
	}
#ifdef PRINT_HINTINFO
	printf("���ִ�սʿ:%d,������:%d,��ʦ:%d\n",m_TirrierCount[0],m_TirrierCount[1],m_TirrierCount[2]);
#endif
	return 0;
}
void CCountry::addTirrierCountFromWeapon(int weaponid)
{
	int type = m_ConsortiaManage->GetTirrierType(weaponid);
	if (type >=0 && type <=2)
	{
		m_TirrierCount[type]++;
	}
}
int CCountry::AddCouncil(CPlayerRanking *pPlayerRanking,Postion_Type postion,DB_Proxy *pDBProxy)
{
	int key = pPlayerRanking->GetPlayerID();// * 100 + TmpCountryID + type;
	Council *pNewCouncil = new Council;
	pNewCouncil->setmasterid(CountryID,BOULE_DEPT);
	pNewCouncil->setdetailid(pPlayerRanking->GetPlayerID());
	strcpy(pNewCouncil->name_,pPlayerRanking->GetName());
	pNewCouncil->battle_ = pPlayerRanking->GetEnemyCount();
	pNewCouncil->economy_ = pPlayerRanking->GetCollectionRate();
	pNewCouncil->contribute_ = pPlayerRanking->GetContribute();
	pNewCouncil->vote_ = 0;
	pNewCouncil->postion_ = postion;

	//Council nCouncil;
	//memcpy(&nCouncil,pNewCouncil,sizeof(Council));
	int iRet = pDBProxy->db_insert(*pNewCouncil);
	if( iRet!= DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("CCountry::AddCouncil->����ίԱʧ��(%d)�����%d����Ӫ��%d,ְλ��%d.",iRet,pPlayerRanking->GetPlayerID(),CountryID,postion));
		delete pNewCouncil;
		pNewCouncil = 0;
		return 1;
	}
	m_CouncilMap[key] = pNewCouncil;
	return 0;
}
__int64 CCountry::ResetNextCouncilTime(DB_Proxy *pDBProxy)
{
	__int64 curTime = IceUtil::Time::now().toSeconds() + HoldTimes;
	Country countrytb;
	countrytb.setid(CountryID);
	countrytb.nexttime1_ = curTime - Nexttime1;
	if(pDBProxy->db_increase(countrytb) == DB_Proxy::DB_SUCCEED)
	{
		Nexttime1 = curTime;
	}
	return Nexttime1;
}
__int64 CCountry::ResetNextVoteTime(DB_Proxy *pDBProxy,int times)
{
	__int64 curTime = 0;
	if(times >=0)
		curTime = IceUtil::Time::now().toSeconds() + times;

	Country countrytb;
	countrytb.setid(CountryID);
	countrytb.nexttime2_ = curTime - Nexttime2;
	if(pDBProxy->db_increase(countrytb) == DB_Proxy::DB_SUCCEED)
	{
		Nexttime2 = curTime;
	}
	return Nexttime2;
}
int CCountry::BroadCastCouncilList(DB_Proxy *pDBProxy,int type,int cmd)
{
	//type==0Ϊ��Ա�б�����δ������+��������Ա�б�
	int count = 0;
	if(type ==0)
		count = m_CouncilMap.size();
	else
		count = m_DODMap.size() + m_InteriorMap.size();

	Council_Map_Iterator iter;
	char *ret_buf = new char[sizeof(Query_CouncilList_Response) + count * sizeof(Council_Info)];
	LPQuery_CouncilList_Response rep_msg = (LPQuery_CouncilList_Response)ret_buf;
	PlayerOfficerTable officertb;
	int index = 0;
	if(type == 0)
	{
		for(iter = m_CouncilMap.begin();iter != m_CouncilMap.end();iter++)
		{
			(*rep_msg)[index].PlayerID = iter->second->playerid_;
			strcpy((*rep_msg)[index].Name,iter->second->name_);
			(*rep_msg)[index].Type = iter->second->type_;
			(*rep_msg)[index].Postion = iter->second->postion_;
			officertb.setmasterid(iter->second->playerid_);
			officertb.setdetailid(iter->second->playerid_);
			if(pDBProxy->db_select(officertb) != DB_Proxy::DB_SUCCEED)
			{
				//assert(false);
			}
			(*rep_msg)[index].Level = officertb.level_;
			(*rep_msg)[index].MilitaryRank = officertb.militaryrank_;
			index++;
		}
	}
	else
	{
		for(iter = m_DODMap.begin();iter != m_DODMap.end();iter++)
		{
			(*rep_msg)[index].PlayerID = iter->second->playerid_;
			strcpy((*rep_msg)[index].Name,iter->second->name_);
			(*rep_msg)[index].Type = iter->second->type_;
			(*rep_msg)[index].Postion = iter->second->postion_;
			officertb.setmasterid(iter->second->playerid_);
			officertb.setdetailid(iter->second->playerid_);
			if(pDBProxy->db_select(officertb) != DB_Proxy::DB_SUCCEED)
			{
				//assert(false);
			}
			(*rep_msg)[index].Level = officertb.level_;
			(*rep_msg)[index].MilitaryRank = officertb.militaryrank_;
			index++;
		}
		for(iter = m_InteriorMap.begin();iter != m_InteriorMap.end();iter++)
		{
			(*rep_msg)[index].PlayerID = iter->second->playerid_;
			strcpy((*rep_msg)[index].Name,iter->second->name_);
			(*rep_msg)[index].Type = iter->second->type_;
			(*rep_msg)[index].Postion = iter->second->postion_;
			officertb.setmasterid(iter->second->playerid_);
			officertb.setdetailid(iter->second->playerid_);
			if(pDBProxy->db_select(officertb) != DB_Proxy::DB_SUCCEED)
			{
				//assert(false);
			}
			(*rep_msg)[index].Level = officertb.level_;
			(*rep_msg)[index].MilitaryRank = officertb.militaryrank_;
			index++;
		}
	}
	rep_msg->serialize(0,0,count,CountryID,cmd);//CC_S_BROAD_VOTESTART
	//asynch_write(rep_msg->agent,(char*)rep_msg,rep_msg->length);
	broadcast((char*)rep_msg,rep_msg->length);
	delete[] ret_buf;
	ret_buf = 0;
	return 0;
}
__int64 CCountry::GetNextCouncilTime()
{
	return Nexttime1;
}
__int64 CCountry::GetNextVoteTime()
{
	return Nexttime2;
}
int CCountry::GetGoOutTime()
{
	int times;
	/*
	IceUtil::DateTime curdate;
	IceUtil::Time::now().toDateTime(curdate);
	//����20��00--����20��00Ϊ�������������Ϊ��������
	if(curdate.week ==4 || (curdate.week ==3 && curdate.hour >=20) || (curdate.week ==5 && curdate.hour <20))
	{//����
		times = m_ConsortiaManage->GetNextTime(curdate.week,5,20);
	}
	else
	{//����
		times = m_ConsortiaManage->GetNextTime(curdate.week,3,20);
	}
	*/
	times = m_ConsortiaManage->GetNextTime(Week,20,0);
#ifdef PRINT_HINTINFO
	printf("��ȡ%d����������ʱ��(week:%d)��%d\n",CountryID,Week,times);
#endif
	times = 10;
	return times;
}
Council *CCountry::GetCouncil(int playerid)
{
	Council_Map_Iterator iter;
	iter = m_CouncilMap.find(playerid);
	if(iter == m_CouncilMap.end())
		return NULL;
	return iter->second;
}
void CCountry::WriteLock()
{
	m_RWMutex.writeLock();
}
void CCountry::ReadLock()
{
	m_RWMutex.readLock();
}
void CCountry::Unlock()
{
	m_RWMutex.unlock();
}
void CCountry::ClearAllData()
{
	
	if(m_TirrierGoOutTimer)
	{
		cancel_timer(m_TirrierGoOutTimer);
		m_TirrierGoOutTimer = 0;
		delete m_TirrierGoOutParam;
		m_TirrierGoOutParam =0;
	}

	Council_Map_Iterator iter;
	Council *pDelCouncil = 0;
	for(iter = m_CouncilMap.begin();iter != m_CouncilMap.end();iter++)
	{
		if(pDelCouncil = iter->second)
		{
			delete pDelCouncil;
			pDelCouncil = 0;
		}
	}
	m_CouncilMap.clear();
	for(iter = m_DODMap.begin();iter != m_DODMap.end();iter++)
	{
		if(pDelCouncil = iter->second)
		{
			delete pDelCouncil;
			pDelCouncil = 0;
		}
	}
	m_DODMap.clear();
	for(iter = m_InteriorMap.begin();iter != m_InteriorMap.end();iter++)
	{
		if(pDelCouncil = iter->second)
		{
			delete pDelCouncil;
			pDelCouncil = 0;
		}
	}
	m_InteriorMap.clear();
	//Candidate_Map		m_CandidateMap;//��ѡ���б�
	Candidate_Map_Iterator candidata_iter;
	Candidate *pDelCandidate = 0;
	for(candidata_iter = m_CandidateMap.begin();candidata_iter != m_CandidateMap.end();candidata_iter++)
	{
		if(pDelCandidate = candidata_iter->second)
		{
			delete pDelCandidate;
			pDelCandidate = 0;
		}
	}
	m_CandidateMap.clear();
	//Ballot_Map			m_BallotMap;
	Ballot_Map_Iterator ballot_iter;
	Ballot *pDelBallot = 0;
	for(ballot_iter = m_BallotMap.begin();ballot_iter != m_BallotMap.end();ballot_iter++)
	{
		if(pDelBallot = ballot_iter->second)
		{
			delete pDelBallot;
			pDelBallot = 0;
		}
	}
	m_BallotMap.clear();
}
void CCountry::DisplayCouncil()
{
	Council_Map_Iterator iter;
	printf("============================country%d(%d)=======================\n",CountryID,m_CouncilMap.size());
	for(iter = m_CouncilMap.begin();iter != m_CouncilMap.end();iter++)
	{
		printf("PlyerID:%d,Name:%15s,Battle:%10d,Economy:%10d,Contribute:%10d,postion:%d\n",
			iter->second->playerid_,iter->second->name_,iter->second->battle_,iter->second->economy_,
			iter->second->contribute_,iter->second->postion_);
	}
}
void CCountry::SendEamil()
{
	Notify_Send_Mail playeremail;
	Notify_BroadMail broademail;
	memset(&playeremail,0,sizeof(playeremail));
	memset(&broademail,0,sizeof(broademail));
	char szStartTime[16] = {0};
	char szEndTime[16] = {0};
	int iYear =0,iMon = 0 ,iDay =0 ,iHour =0 ,iMin =0,iSec = 0;
	__int64 iBaseTime = IceUtil::Time::Date(2000, 1, 1).toSeconds();
	__int64 iCurTime = IceUtil::Time::now().toSeconds();
	game_calendar::calendar cal_start(iCurTime -iBaseTime);
	cal_start.get_game_calendar(iYear,iMon,iDay,iHour,iMin,iSec);
	sprintf(szStartTime,"%d��%d��%d��",iYear,iMon,iDay);
	game_calendar::calendar cal_end(iCurTime + HoldTimes -iBaseTime);
	iYear = iMon = iDay = iHour = iMin = iSec = 0;
	cal_end.get_game_calendar(iYear,iMon,iDay,iHour,iMin,iSec);
	sprintf(szEndTime,"%d��%d��%d��",iYear,iMon,iDay);
	LPMailInfoParam_Item pMailInfoParam = 0;
	LPMailInfoParam_Item pBroadMailParam = 0;
	LPCareerConfig_Item pCareerConfig = 0;
	pMailInfoParam = m_ConsortiaManage->GetMailInfoParam(CountryID,MAIL_COUNTRY_ELECTIONCOUNCIL);
	pBroadMailParam = m_ConsortiaManage->GetMailInfoParam(CountryID,MAIL_COUNTRY_COUNCIL);
	char szContentbuf[2][SIZE_256] = {0};
	char szTmpBuf[SIZE_256] = {0};
	char szContentTmp[SIZE_256] = {0};
	//char szNameList[3][512] = {0};
	int iIndex = 1;
	if(pMailInfoParam)
	{
		StrReplace(pMailInfoParam->Content,szContentTmp,"$startime",szStartTime);
		StrReplace(szContentTmp,szContentbuf[0],"$endtime",szEndTime);
	}
	if(pBroadMailParam)
	{
		GetValue(pBroadMailParam->Content,szTmpBuf,0,'|');
		StrReplace(szTmpBuf,szContentTmp,"$startime",szStartTime);
		StrReplace(szContentTmp,broademail.szbody,"$endtime",szEndTime);
		GetValue(pBroadMailParam->Content,szContentbuf[1],1,'|');
		broademail.CountryID = CountryID;
		strcpy(broademail.szSendPlayerName,"����ϵͳ");
	}
	Council_Map_Iterator iter;
	//printf("============================country%d(%d)=======================\n",CountryID,m_CouncilMap.size());
	Council *pCouncil = 0;
	for(iter = m_CouncilMap.begin();iter != m_CouncilMap.end();iter++)
	{
		pCouncil = iter->second;
		if(pCouncil == NULL)
			continue;
		if(pMailInfoParam)
		{
			playeremail.type = Normal;
			playeremail.ulRecieverPlayerID = pCouncil->playerid_;
			strcpy(playeremail.szSendPlayerName,"����ϵͳ");
			strcpy(playeremail.sztitle,pMailInfoParam->Title);
			pCareerConfig = m_ConsortiaManage->GetCareerConfig(BOULE_DEPT,pCouncil->postion_);
			if(pCareerConfig)
			{
				StrReplace(szContentbuf[0],playeremail.szbody,"$jobname",pCareerConfig->JobName);
			}
			else
			{
				StrReplace(szContentbuf[0],playeremail.szbody,"$jobname","��Ա");
				//assert(false);
			}
			playeremail.serialize();
			event_notify(MDM_GP_MAP_CHAT,(char*)&playeremail,playeremail.length);
		}
		if(pBroadMailParam)
		{
			StrReplace(szContentbuf[1],szTmpBuf,"$toname",pCouncil->name_);
			if(pCareerConfig)
			{
				StrReplace(szTmpBuf,szContentTmp,"$jobname",pCareerConfig->JobName);
			}
			else
			{
				StrReplace(szTmpBuf,szContentTmp,"$jobname","��Ա");
			}
			if(strlen(broademail.szbody) + strlen(szContentTmp) >=SIZE_256-1)
			{//����һ��
				sprintf(broademail.sztitle,"%s--%d",pBroadMailParam->Title,iIndex);
				broademail.serialize();
				event_notify(MDM_GP_MAP_CHAT,(char*)&broademail,broademail.length);
				memset(broademail.szbody,0,SIZE_256);
				iIndex++;
			}
			strcat(broademail.szbody,szContentTmp);
		}
	}

	if(pBroadMailParam && m_CouncilMap.size() >0)
	{
		//broademail.serialize();
		if(iIndex <=1)
		{
			strcpy(broademail.sztitle,pBroadMailParam->Title);
		}
		else
		{
			sprintf(broademail.sztitle,"%s-%d",pBroadMailParam->Title,iIndex);
		}
		broademail.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&broademail,broademail.length);
	}
}