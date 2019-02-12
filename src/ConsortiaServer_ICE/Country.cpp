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
	return (m_UnderWriteTimes = times);	//签名时长
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
//初始化
int CCountry::InitCountry(DB_Proxy *pDBProxy)
{
	char *buf = 0;
	int count = 0;
	int i =0;
	//查询国家信息
	Country countrytb;
	countrytb.setid(CountryID);
	i = pDBProxy->db_select(countrytb);
	if(i == DB_Proxy::DB_FAILED)
	{
		LOG_TRACE_EX(false,("CCountry::InitCountry->初始化国家信息失败：%d.",CountryID));		
		return 1;
	}
	else if(i == DB_Proxy::DB_EOF)
	{
		countrytb.rate_ = 40;
		if(pDBProxy->db_insert(countrytb) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE_EX(false,("CCountry::InitCountry->初始化国家信息失败：新增国家信息记录%d.",CountryID));	
			return 2;
		}
	}
	Treasury = countrytb.treasury_;		//国库资金
	DODMoney = countrytb.dodmoney_;		//军费
	FinanceMoney = countrytb.financemoney_;	//内政费用
	if (countrytb.rate_ >50)
	{
		countrytb.rate_  = 50;
	}
	Rate = static_cast<float>(countrytb.rate_)/100;//军费的比例
	Nexttime1 = countrytb.nexttime1_;		//下次议员选举时间
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
	//查询所有议员
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
	
	//查询国防部
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
	
	//查询内政部
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
	//查询候选人,国防部、内政部
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
	//内政部候选人
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
	//查询选票m_BallotMap
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
				iFlag++;//弹劾
		}
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
	}
	//内政
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
				iFlag++;//弹劾
		}
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
	}

	GetTirrierCountOfHomeMap(pDBProxy);//查询国防军数量
	//设置定时器
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
		{//选举结束
#ifdef PRINT_HINTINFO
		printf("CountryID:%d 选举结束.\n",CountryID);
#endif
		
			Consortia_Timer endmsg;
			if(iFlag == 0)
			{//选举结束
				endmsg.serialize(CC_C_TIMER_VOTEEND,CountryID);
				VoteEndTimer(&endmsg,pDBProxy);
			}
			else if(iFlag <(int)m_UnderWriteNum)
			{//签名结束
				State = UNDERWRITE_STATE;
				endmsg.serialize(CC_C_TIMER_UNDERWRITE,CountryID);
				UnderWriteEndTimer(&endmsg,pDBProxy);
			}
			else
			{//弹劾投票结束
				State = DELATE_STATE;
				endmsg.serialize(CC_C_TIMER_DELATEEND,CountryID);
				DelateVoteEndTimer(&endmsg,pDBProxy);
			}
		}
		else
		{//正在选举中
			long times = long(Nexttime2 - IceUtil::Time::now().toSeconds());
#ifdef PRINT_HINTINFO
		printf("CountryID:%d 正在选举中(%ld- %d).\n",CountryID,times,iFlag);
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
			{//CC_C_TIMER_UNDERWRITE,						//弹劾签名
				
				State = UNDERWRITE_STATE;
				pTimerParam->serialize(CC_C_TIMER_UNDERWRITE,CountryID);
			}
			else
			{
			//CC_C_TIMER_DELATEEND						//弹劾结束				
				State = DELATE_STATE;
				pTimerParam->serialize(CC_C_TIMER_DELATEEND,CountryID);
			}
			if(set_timer(times,pTimerParam) <0)
			{
				LOG_TRACE_EX(false,("CCountry::InitCountry-->设置定时器失败:状态：%d,定时器：%d",State,pTimerParam->command));
			}
		}
	}
	if(ToMapID)
	{
		long times = GetGoOutTime();//计算出征的时间
		m_TirrierGoOutParam = new Consortia_Timer;
		m_TirrierGoOutParam->serialize(CC_C_TIMER_TIRRIERGOOUT,CountryID);
		m_TirrierGoOutTimer = set_timer(times,m_TirrierGoOutParam);
	}
	return 0;
}

int CCountry::StartVote(DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wock(m_RWMutex);
	//清空所有候选人表、投票表、弹劾表等
	DeleteAllCandidate(pDBProxy);//删除 候选人，包括数据库中的数据
	DeleteAllBallot(pDBProxy);//删除选票，包括数据库中的数据
	SendEamil();//20090623发送邮件
	//设置选举结束的定时器，24小时CC_C_TIMER_VOTEEND
	LPConsortia_Timer pTimerParam = new Consortia_Timer;
	pTimerParam->serialize(CC_C_TIMER_VOTEEND,CountryID);
	set_timer(VoteTimes,pTimerParam);
	ResetNextVoteTime(pDBProxy,VoteTimes);
	State = VOTE_STATE;
	return 0;
}
//查询玩家国家信息
int CCountry::QueryUserInfoOfCountry(LPQuery_UserOfCountry_Request msg,DB_Proxy *pDBProxy)
{
	//查询是否为议员、国防部、内政部
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
	{//议员
		rep_msg.serialize(msg->agent,msg->from,0,iter->second->type_,iter->second->postion_,Treasury,DODMoney,FinanceMoney);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 0;
	}
	rep_msg.serialize(msg->agent,msg->from,111,0,0,Treasury,DODMoney,FinanceMoney);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	return 0;
}
//查询本阵营的议员列表
int CCountry::QueryCouncilList(LPQuery_CouncilList_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::RLock rlock(m_RWMutex);
	/*
	Council_Map	m_CouncilMap;//议员列表
	Council_Map	m_DODMap;//国防部
	Council_Map	m_InteriorMap;//内政部
	BOULE_DEPT,				//众议院
	DOD_DEPT,				//国防部
	INTERIOR_DEPT			//内政部
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
					LOG_TRACE_EX(false,("CCountry::QueryCouncilList-->查询玩家%d信息失败",iter->second->playerid_));
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
					LOG_TRACE_EX(false,("CCountry::QueryCouncilList-->查询玩家%d信息失败",iter->second->playerid_));
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
					LOG_TRACE_EX(false,("CCountry::QueryCouncilList-->查询玩家%d信息失败",iter->second->playerid_));
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
//参加竞选（必须为议员）
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
	//首先判断玩家是否为议员
	Council_Map_Iterator council_iter;
	council_iter = m_CouncilMap.find(msg->from);//议员列表
	if(council_iter == m_CouncilMap.end())
	{//非议员
		rep_msg.serialize(msg->agent,msg->from,3);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	//是否已参选
	Candidate_Map_Iterator candidata_iter = m_CandidateMap.find(msg->from);//候选人列表
	if(candidata_iter != m_CandidateMap.end())
	{
		rep_msg.serialize(msg->agent,msg->from,4);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	//判断金钱是否够用
	if(msg->Money < VoteFee_Money)
	{//竞选费用过低
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
		LOG_TRACE_EX(false,("CCountry::TakePartInElection-->查询玩家%d信息失败",msg->from));
		return 1;
	}
	if(playertb.money_ < msg->Money)
	{//Money不够
		rep_msg.serialize(msg->agent,msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	//扣除Money
	memset(&playertb,0,sizeof(PlayerChatRoomTable));
	playertb.setid(msg->from);
	playertb.money_ = -msg->Money;
	if(pDBProxy->db_increase(playertb) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(msg->agent,msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		LOG_TRACE_EX(false,("CCountry::TakePartInElection-->扣除玩家%d参选费用失败",msg->from));
		return 1;
	}
	//向收费系统发送扣除GO点的消息
	Notify_Election_Request notify_msg;
	notify_msg.serialize(msg->agent,msg->from,CountryID,msg->Dept,msg->Creed,msg->Money,VoteFee_GO);
	event_notify(MDM_GP_MAP_CHARGE,(char*)&notify_msg,notify_msg.length);
	return 0;
}
//查询候选人列表
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
				LOG_TRACE_EX(false,("CCountry::QueryCandidateList-->查询候选人%d信息失败",iter->second->playerid_));
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
//投票
int CCountry::PlayerBallot(LPPlayer_Ballot_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	if(State != VOTE_STATE)
		return 1;

	//判断出候选人候选人的参选职位
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
	//判断是否已投票
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
		LOG_TRACE_EX(false,("CCountry::PlayerBallot-->玩家%d投票选举%d,类型%d，更新选票数量失败",msg->from,1,msg->Candidate,pCandidate->type_));
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
		LOG_TRACE_EX(false,("CCountry::PlayerBallot-->玩家%d投票选举%d,类型%d，插入选票失败",msg->from,1,msg->Candidate,pCandidate->type_));
		return 1;
	}

	rep_msg.serialize(msg->agent,msg->from,0,pCandidate->playerid_,pCandidate->type_,pCandidate->vote_);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	return 0;
}
//弹劾
int CCountry::PlayerDelate(LPCountry_Delate_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	Country_Delate_Response rep_msg;
	if(State !=0 && State != UNDERWRITE_STATE)
	{
		rep_msg.serialize(msg->agent,msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;//状态不对
	}
		
	//权限判断
	Council_Map_Iterator council_iter;
	Council *pCouncil = 0;
	int iTmpType = -1;

	council_iter = m_CouncilMap.find(msg->from);
	if(council_iter == m_CouncilMap.end())
	{//非议员
		rep_msg.serialize(msg->agent,msg->from,3);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	if(msg->Type == DOD_DEPT)
	{
		council_iter = m_DODMap.find(msg->from);
		if(council_iter != m_DODMap.end())
		{//为部长或副部长
			pCouncil = council_iter->second;
		}
	}
	else
	{
		council_iter = m_InteriorMap.find(msg->from);
		if(council_iter != m_InteriorMap.end())
		{//为部长或副部长
			pCouncil = council_iter->second;
		}
	}
	if(pCouncil && pCouncil->postion_ == MINISTER_POSTION)
	{//为部长不能弹劾自己
		rep_msg.serialize(msg->agent,msg->from,5);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	if(State ==0)
	{
		//后48小时不能弹劾
		if(IceUtil::Time::now().toSeconds() + m_UnderWriteTimes + m_DelateVoteTimes + VoteTimes >= Nexttime1)
		{
			rep_msg.serialize(msg->agent,msg->from,4);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			return 1;
		}
		State = UNDERWRITE_STATE;
		DeleteAllBallot(pDBProxy);
		m_DelateType = msg->Type;
		//设置签名结束定时器
		m_TimerParam = new Consortia_Timer;
		m_TimerParam->serialize(CC_C_TIMER_UNDERWRITE,CountryID);
		m_DelateTimer = set_timer(m_UnderWriteTimes,m_TimerParam);
		ResetNextVoteTime(pDBProxy,m_UnderWriteTimes);
		//广播消息,弹劾签名启动
		Broad_DelateState broadmsg;
		broadmsg.serialize(CountryID,m_DelateType,0);
		broadcast((char*)&broadmsg,broadmsg.length);
	}

	if(m_DelateType != msg->Type)
	{//与当前进行中的弹劾不一致
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
		LOG_TRACE_EX(false,(" CCountry::PlayerDelate-->插入玩家%d的弹劾签名投票%d失败。",msg->from,DELATE_VOTE));
		return 1;
	}
	rep_msg.serialize(msg->agent,msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	//当超过5人签名时，即时启动弹劾投票
	if(m_BallotMap.size() >=m_UnderWriteNum)
	{
		DeleteAllBallot(pDBProxy);
		LOG_TRACE_EX((m_DelateTimer &&  m_TimerParam),("指针为空"));
		cancel_timer(m_DelateTimer);
		m_TimerParam->serialize(CC_C_TIMER_DELATEEND,CountryID);
		m_DelateTimer = set_timer(m_DelateVoteTimes,m_TimerParam);
		ResetNextVoteTime(pDBProxy,m_DelateVoteTimes);
		
		State = DELATE_STATE;
	}
	return 0;
}
//查询弹劾信息
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
		//副部长的票*3
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
//弹劾投票
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
	{//已投票
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
	//没权限
	if(pCouncil == NULL)
	{
		rep_msg.serialize(msg->agent,msg->from,3);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}

	//玩家为部长 Or 副部长
	if(iTmpType == m_DelateType && pCouncil->postion_ == MINISTER_POSTION)
	{//部长不能参与投票
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
		LOG_TRACE_EX(false,("CCountry::DelateBallot-->插入玩家%d的弹劾投票%d失败。",msg->from,msg->Value));
		return 1;
	}
	rep_msg.serialize(msg->agent,msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	return 0;
}
//修改参选纲领
int CCountry::ModifyElectionCreed(LPModify_ElectionCreed_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	Modify_ElectionCreed_Response rep_msg;
	//是否已参选
	Candidate_Map_Iterator candidata_iter = m_CandidateMap.find(msg->from);//候选人列表
	if(candidata_iter == m_CandidateMap.end())
	{
		rep_msg.serialize(msg->agent,msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	Candidate *pCandidate = candidata_iter->second;
	if(msg->Money >0)
	{//增加竞选费用
		PlayerChatRoomTable playertb;
		playertb.setid(msg->from);
		if(pDBProxy->db_select(playertb) != DB_Proxy::DB_SUCCEED)
		{
			rep_msg.serialize(msg->agent,msg->from,111);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			LOG_TRACE(("CCountry::ModifyElectionCreed-->查询玩家%d失败",msg->from));
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
			LOG_TRACE(("CCountry::ModifyElectionCreed-->db_increase玩家%d失败",msg->from));
			return 1;
		}
		pCandidate->expense_ += msg->Money;
		//参选费用计入国库
		Treasury += msg->Money;
		Country countrytb;
		countrytb.setid(CountryID);
		countrytb.treasury_ = msg->Money;
		if(pDBProxy->db_increase(playertb) != DB_Proxy::DB_SUCCEED)
		{
			rep_msg.serialize(msg->agent,msg->from,111);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			LOG_TRACE(("CCountry::ModifyElectionCreed-->Country玩家%d失败",msg->from));
			return 1;
		}
	}
	if(msg->Creed[0] !=0)
	{//修改纲领
		strcpy(pCandidate->creed_,msg->Creed);
	}
	if(msg->Creed[0] != 0 || msg->Money >0)
	{
		if(pDBProxy->db_update(*pCandidate,*pCandidate) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE(("CCountry::ModifyElectionCreed-->db_update玩家%d失败",msg->from));
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

//国防部长创建国防军
int CCountry::CreateTirrier(LPCreateTirrier_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	CreateTirrier_Response rep_msg;
	//首先判断权限
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
	//判断国防军参数
	LPTirrierParam pTirrierParam = m_ConsortiaManage->GetTirrierParam(msg->TirrierType);
	if(pTirrierParam == NULL)
	{
		rep_msg.serialize(msg->agent,msg->from,3);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	//判断数量
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
		LOG_TRACE(("CCountry::CreateTirrier-->玩家%d创建国防军时扣除资金失败",msg->from));
	}
	DODMoney -= pTirrierParam->Money;
	Notify_CreateTirrier_Request notify_msg;
	notify_msg.serialize(msg->agent,msg->from,CountryID,pTirrierParam->WeaponType,pTirrierParam->WeaponLevel,pTirrierParam->DefendType,pTirrierParam->DefendLevel,pTirrierParam->SoldierType,msg->TirrierType);
	if(event_notify(HomeMapID,(char*)&notify_msg,notify_msg.length) ==0)
	{
		rep_msg.serialize(msg->agent,msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		LOG_TRACE(("CCountry::CreateTirrier-->玩家%d创建国防军时通知郡地图%d失败",msg->from,HomeMapID));
		return 1;
	}
	return 0;
}
//国防军出征
int CCountry::TirrierGoOut(LPTirrierGoOut_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	TirrierGoOut_Response rep_msg;
	//首先判断权限
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
		LOG_TRACE(("CCountry::TirrierGoOut-->玩家%d国防军时通知郡地图%d失败",msg->from,HomeMapID));
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
	long times = GetGoOutTime();//计算出征的时间
	m_TirrierGoOutParam = new Consortia_Timer;
	m_TirrierGoOutParam->serialize(CC_C_TIMER_TIRRIERGOOUT,CountryID);
	m_TirrierGoOutTimer = set_timer(times,m_TirrierGoOutParam);
	//广播消息
	
	Broad_TirrierState broadmsg;
	broadmsg.serialize(CountryID,iter->second->name_,m_TirrerFee,CC_S_BROAD_TIRRIERGOOUT);
	broadcast((char*)&broadmsg,broadmsg.length);
	return 0;
}
//升级公共建筑
int CCountry::UpgradeBuild(LPUpgradeBuild_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	UpgradeBuild_Response rep_msg;
	//首先判断权限
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
//修改势力公告
int CCountry::ModifyNotice(LPModifyNotice_Request msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::RLock rlock(m_RWMutex);
	ModifyNotice_Response rep_msg;
	//首先判断权限
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
//查询国防军数量
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
//选举结束
int CCountry::VoteEndTimer(LPConsortia_Timer msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	//选举结束，统计出前5名，第一名为部长，其余4位为副部长
	DeleteAllBallot(pDBProxy);//删除选票，包括数据库中的数据
	DeleteAllDODDept(pDBProxy);//删除国防部成员
	DeleteAllInteriorDept(pDBProxy);//删除内政部成员

	//清除弹劾的相关信息
	
#ifdef PRINT_HINTINFO
	printf("%s国家%d选举结束\n",IceUtil::Time::now().toDateTime().c_str(),CountryID);
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
	sprintf(szStartTime,"%d年%d月%d日",iYear,iMon,iDay);
	game_calendar::calendar cal_end(iCurTime + HoldTimes -iBaseTime);
	iYear = iMon = iDay = iHour = iMin = iSec = 0;
	cal_end.get_game_calendar(iYear,iMon,iDay,iHour,iMin,iSec);
	sprintf(szEndTime,"%d年%d月%d日",iYear,iMon,iDay);
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
			pCouncil->postion_ = MINISTER_POSTION;//部长
		}
		else
		{
			pCouncil->postion_ = UNDERSECRETARY_POSTION;//副部长
		}
		//加入国防部列表
		m_DODMap[pCouncil->playerid_] = pCouncil;

		if(pDBProxy->db_insert(*pCouncil) != DB_Proxy::DB_SUCCEED)
		{
			//assert(false);
		}
#ifdef PRINT_HINTINFO
		printf("国家%d,国防部：玩家%d,职位%d\n",CountryID,pCouncil->playerid_,pCouncil->postion_);
#endif
		index++;
		iFlag = 1;
		//20090622发送邮件
		pCareerConfig = m_ConsortiaManage->GetCareerConfig(DOD_DEPT,pCouncil->postion_);
		if(pMailInfoParam)
		{
			playeremail.type = Normal;
			playeremail.ulRecieverPlayerID = pCouncil->playerid_;
			strcpy(playeremail.szSendPlayerName,"国家系统");
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
				StrReplace(szContentbuf,playeremail.szbody,"$jobname","皇帝");
				//assert(false);
			}
			playeremail.serialize();
			event_notify(MDM_GP_MAP_CHAT,(char*)&playeremail,playeremail.length);
		}

		if(pCouncil->postion_ == MINISTER_POSTION)
		{//群发邮件MAIL_COUNTRY_DOD_VOTEEND
			LPMailInfoParam_Item pBroadMailParam = m_ConsortiaManage->GetMailInfoParam(CountryID,MAIL_COUNTRY_DOD_VOTEEND);
			if(pBroadMailParam)
			{
				broademail.CountryID = CountryID;
				strcpy(broademail.szSendPlayerName,"国家系统");
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
					StrReplace(szContentbuf,broademail.szbody,"$jobname","皇帝");
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
			pCouncil->postion_ = MINISTER_POSTION;//部长
		}
		else
		{
			pCouncil->postion_ = UNDERSECRETARY_POSTION;//副部长
		}
		//加入内政部列表
		m_InteriorMap[pCouncil->playerid_] = pCouncil;

		if(pDBProxy->db_insert(*pCouncil) != DB_Proxy::DB_SUCCEED)
		{
			//assert(false);
		}
#ifdef PRINT_HINTINFO
		printf("国家%d,内政部：玩家%d,职位%d\n",CountryID,pCouncil->playerid_,pCouncil->postion_);
#endif
		index++;
		iFlag = 1;
		//20090622发送邮件
		pCareerConfig = m_ConsortiaManage->GetCareerConfig(INTERIOR_DEPT,pCouncil->postion_);
		if(pMailInfoParam)
		{
			playeremail.type = Normal;
			playeremail.ulRecieverPlayerID = pCouncil->playerid_;
			strcpy(playeremail.szSendPlayerName,"国家系统");
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
				StrReplace(szContentbuf,playeremail.szbody,"$jobname","皇帝");
				//assert(false);
			}
			playeremail.serialize();
			event_notify(MDM_GP_MAP_CHAT,(char*)&playeremail,playeremail.length);
		}

		if(pCouncil->postion_ == MINISTER_POSTION)
		{//群发邮件MAIL_COUNTRY_DOD_VOTEEND
			LPMailInfoParam_Item pBroadMailParam = m_ConsortiaManage->GetMailInfoParam(CountryID,MAIL_COUNTRY_INTERIOR_VOTEEND);
			if(pBroadMailParam)
			{
				broademail.CountryID = CountryID;
				strcpy(broademail.szSendPlayerName,"国家系统");
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
					StrReplace(szContentbuf,broademail.szbody,"$jobname","皇帝");
				}
				broademail.serialize();
				event_notify(MDM_GP_MAP_CHAT,(char*)&broademail,broademail.length);
			}
		}
	}
	interior_set.clear();

	DeleteAllCandidate(pDBProxy);//删除 候选人，包括数据库中的数据
	//调拨资金
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
		DODMoney += money;		//军费
		FinanceMoney += money;	//内政费用
	}

	if(pDBProxy->db_increase(countrytb) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("CCountry::VoteEndTimer-->调拨费用失败"));
	}
	Nexttime2 = 0;
	State = 0;

	//广播消息
	BroadCastCouncilList(pDBProxy,1,CC_S_BROAD_VOTEND);
	return 0;
}
//弹劾签名结束
int CCountry::UnderWriteEndTimer(LPConsortia_Timer msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	if(State != UNDERWRITE_STATE)
		return 1;

	Broad_DelateState broadmsg;
	if(m_BallotMap.size() <m_UnderWriteNum)
	{//签名结束，弹劾不能启动
		m_DelateTimer = 0;
		ResetNextVoteTime(pDBProxy,-1);
		//Nexttime2 = 0;
		State = 0;
		m_TimerParam = 0;

		//签名结束广播，弹劾失败
		//广播消息,弹劾签名启动
		broadmsg.serialize(CountryID,m_DelateType,3);
	}
	else
	{//弹劾投票启动
		DeleteAllBallot(pDBProxy);
		m_TimerParam = new Consortia_Timer;
		m_TimerParam->serialize(CC_C_TIMER_DELATEEND,CountryID);
		m_DelateTimer = set_timer(m_DelateVoteTimes,m_TimerParam);
		ResetNextVoteTime(pDBProxy,m_DelateVoteTimes);
		//弹劾投票开始广播消息
		broadmsg.serialize(CountryID,m_DelateType,1);
		State = DELATE_STATE;
	}
	broadcast((char*)&broadmsg,broadmsg.length);
	return 0;
}
//弹劾投票结束
int CCountry::DelateVoteEndTimer(LPConsortia_Timer msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	if(State != DELATE_STATE)
		return 1;

	//发送广播消息，弹劾结束
	Broad_DelateState broadmsg;
	broadmsg.serialize(CountryID,m_DelateType,2);
	broadcast((char*)&broadmsg,broadmsg.length);
	//统计
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
	{//弹劾成功
		Council *pCouncil = 0;
		Council *pNextCouncil = 0;
		int votecount = -1;
		Council_Map_Iterator iter;
		if(m_DelateType == DOD_DEPT)
		{/*
		 Council_Map	m_DODMap;//国防部
		 Council_Map	m_InteriorMap;//内政部
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
				LOG_TRACE_EX(false,("CCountry::DelateVoteEndTimer-->阵营%d的%d新部长部长%s。",CountryID,m_DelateType,pNextCouncil->playerid_));
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
			LOG_TRACE_EX(false,("CCountry::DelateVoteEndTimer-->删除阵营%d的%d部长%s。",CountryID,m_DelateType,pCouncil->playerid_));
		}
		//弹劾成功，广播消息
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
	{//弹劾失败
	}
	DeleteAllBallot(pDBProxy);
	ResetNextVoteTime(pDBProxy,-1);
	//Nexttime2 = 0;
	State = 0;
	m_TimerParam = 0;
	//广播消息
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
		//返回Money
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
	m_CandidateMap[msg->from] = pNewCandidate;//加入列表

	if(pDBProxy->db_insert(*pNewCandidate) != DB_Proxy::DB_SUCCEED)
	{
		//assert(false);
	}
	//参选费用计入国库
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

	//发送XX参选XX，竞选费用XX的广播消息
	Broad_Election_Response broadmsg;
	broadmsg.serialize(CountryID,pNewCandidate->name_,msg->Dept,msg->Money);
	broadcast((char*)&broadmsg,broadmsg.length);

	playertb.setid(msg->from);
	pDBProxy->db_select(playertb);
	ISystemLog::getInstance()->playerMoney_Election_Log(msg->from,pNewCandidate->name_,
		playertb.money_ + msg->Money,playertb.money_,msg->Money,msg->Dept);
	return 0;
}
//郡地图返回创建国防军的通知消息
int CCountry::CreateTirrierNotify(LPNotify_CreateTirrier_Response msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	CreateTirrier_Response rep_msg;
	if(msg->Error_Code !=0)
	{//创建失败，返回金钱
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
	//创建成功
	rep_msg.serialize(msg->agent,msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);

	Query_TirrierCount_Response rep_countmsg;
	rep_countmsg[0] = m_TirrierCount[0];
	rep_countmsg[1] = m_TirrierCount[1];
	rep_countmsg[2] = m_TirrierCount[2];
	rep_countmsg.serialize(msg->agent,msg->from);
	asynch_write(rep_countmsg.agent,(char*)&rep_countmsg,rep_countmsg.length);
	//发送广播消息
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
//郡地图返回升级公共建筑的通知消息
int CCountry::UpgradeBuildNotify(LPNotify_UpgradeBuild_Response msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	UpgradeBuild_Response rep_msg;
	if(msg->Error_Code !=0)
	{//捐献失败，返回金钱
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
	//捐献成功

	rep_msg.serialize(msg->agent,msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	//发送广播消息
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
//国防军出征定时器
int CCountry::TirrierGoOutTimer(LPConsortia_Timer msg,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	if(ToMapID <1000 || ToMapID >1060)
		return 1;

	//从新手村获取到国防军
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
//国防军被消灭
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
//市场收税
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
//删除 候选人，包括数据库中的数据
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
//删除选票，包括数据库中的数据
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
//删除国防部成员
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
//删除内政部成员
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
//重置弹劾
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
//查询国防军数量
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
			if(pMapList[i].state_ != war && pMapList[i].state_ != 100)//非战斗郡和非新手村郡（和平郡不应该有国防军）
				continue;
			if(pMapList[i].state_ == 100 && pMapList[i].countryid_ != CountryID)
				continue;
				*/
			if(pMapList[i].state_ == 100 && pMapList[i].mapid_ != HomeMapID)//非本方新手村
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
	printf("战士:%d,弓箭手:%d,法师:%d\n",m_TirrierCount[0],m_TirrierCount[1],m_TirrierCount[2]);
#endif
	return 0;
}
//查询国防军数量
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
	printf("新手村战士:%d,弓箭手:%d,法师:%d\n",m_TirrierCount[0],m_TirrierCount[1],m_TirrierCount[2]);
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
		LOG_TRACE_EX(false,("CCountry::AddCouncil->新增委员失败(%d)：玩家%d，阵营：%d,职位：%d.",iRet,pPlayerRanking->GetPlayerID(),CountryID,postion));
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
	//type==0为议员列表，其他未国防部+内政部人员列表
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
	//周三20：00--周五20：00为周五出征，其他为周三出征
	if(curdate.week ==4 || (curdate.week ==3 && curdate.hour >=20) || (curdate.week ==5 && curdate.hour <20))
	{//周五
		times = m_ConsortiaManage->GetNextTime(curdate.week,5,20);
	}
	else
	{//周三
		times = m_ConsortiaManage->GetNextTime(curdate.week,3,20);
	}
	*/
	times = m_ConsortiaManage->GetNextTime(Week,20,0);
#ifdef PRINT_HINTINFO
	printf("获取%d国防军出征时间(week:%d)：%d\n",CountryID,Week,times);
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
	//Candidate_Map		m_CandidateMap;//候选人列表
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
	sprintf(szStartTime,"%d年%d月%d日",iYear,iMon,iDay);
	game_calendar::calendar cal_end(iCurTime + HoldTimes -iBaseTime);
	iYear = iMon = iDay = iHour = iMin = iSec = 0;
	cal_end.get_game_calendar(iYear,iMon,iDay,iHour,iMin,iSec);
	sprintf(szEndTime,"%d年%d月%d日",iYear,iMon,iDay);
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
		strcpy(broademail.szSendPlayerName,"国家系统");
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
			strcpy(playeremail.szSendPlayerName,"国家系统");
			strcpy(playeremail.sztitle,pMailInfoParam->Title);
			pCareerConfig = m_ConsortiaManage->GetCareerConfig(BOULE_DEPT,pCouncil->postion_);
			if(pCareerConfig)
			{
				StrReplace(szContentbuf[0],playeremail.szbody,"$jobname",pCareerConfig->JobName);
			}
			else
			{
				StrReplace(szContentbuf[0],playeremail.szbody,"$jobname","议员");
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
				StrReplace(szTmpBuf,szContentTmp,"$jobname","议员");
			}
			if(strlen(broademail.szbody) + strlen(szContentTmp) >=SIZE_256-1)
			{//发送一次
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