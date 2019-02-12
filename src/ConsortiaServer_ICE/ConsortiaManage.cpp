#include "ConsortiaManage.h"
#include <stdio.h>
#include "ProcMessage.h"
#include "calendar.h"
#include "log.hpp"

#pragma warning(disable:4100)
#pragma warning(disable:4127)
#pragma warning(disable:4189)
#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4706)
#define CHECK_TIMER		3600		//��ʱ��

CConsortiaManage::CConsortiaManage()
:m_bHasStart(false),m_PlayerRanking(0),m_PlayerRankingSize(0),
m_CollectionRateRanking(0),m_ContributeRanking(0),m_EnemyRanking(0),
m_ConosrtiaRanking(0),m_ConsortiaRankingSize(0),m_InsertRankingCount(20),
m_MilitaryNum(15),m_EconomyNum(10),m_ContributeNum(5)
{								//����ίԱΪ����
	SetVoteParam(4,20,0,30240,1440,1440);
#ifdef PRESS_TEST
	pro_count = 0;
#endif
}

CConsortiaManage::~CConsortiaManage()
{
	Stop();
	ClearAllData();
}


/**************************************************************************
*˵�����������Ӳ�����������DBG��ʼ���������ݱ�
*���������pParam �������ṹAgent_DB_Param��ָ�룬����Agent��DBG��IP���˿�
*����ֵ��true:�ɹ���false:ʧ��
**************************************************************************/
bool CConsortiaManage::InitConManage(LPAgent_DB_Param pParam,const char *filename)
{
	ClearAllData();
	//InitTestData();

	m_AgentDBParam = *pParam;//�������
	ReadLanguageConfig(filename);//��ȡ����

	//��ȡ���ݿ�
	m_DBProxy.fini();
	if(!m_DBProxy.init(m_AgentDBParam.DBGIP,m_AgentDBParam.DBGPort,MDM_GP_MAP_CONSORTIA))
	{//����DBGʧ��
		printf("Connect DBG(%s:%d) server fault.\n",m_AgentDBParam.DBGIP,m_AgentDBParam.DBGPort);
		return false;
	}

	//��ȡ������Ϣ���ñ�
	if(!loadGameCityInfo())
	{
		printf("loadGameCityInfo fault.\n");
		return false;
	}

	if(!loadPublicBuildParamTable())
	{
		printf("GetPublicBuildParamTable fault.\n");
		return false;
	}
	if(!loadTirrierParam())
	{
		printf("GetTirrierParam fault.\n");
		return false;
	}

	loadMailInfoParam();
	loadCareerConfig();
	//��ȡ���������ƺŲ�����
	loadLegionTitleParam();
	//��ȡ��������������
	loadConsortiaUpdateParam();
	//�����������������ӵ����ź���ۻ������ñ�,ǧ�ֱ�
	loadConversionRateParam();
	//��ȡ��ұ�
	if(!GetPlayerAccTable())
	{
		printf("GetPlayerAcc fault.\n");
		return false;
	}
	//��ȡ����ְλ��
	if(!GetConsortiaOfficialTable())
	{
		printf("GetConsortiaOfficial fault.\n");
		return false;

	}

	//��ȡ���������������
	if(!GetCreateConsortiaLimitTable())
	{
		printf("GetCreateConsortia fault.\n");
		return false;
	}

	//��ȡ�����
	if(!GetConsortiaTable())
	{
		printf("GetConsortia fault.\n");
		return false;
	}
	/*��ȡ�����Ա��
	if(!GetConsortiaUserTable())
	{
		printf("GetConsortiaUser fault.\n");
		return false;
	}
	//��ȡ�������б�
	if(!GetConsortiaBankTable())
	{
		printf("GetConsortiaBankTable fault.\n");
		return false;
	}

	*/
	//��ȡ����ͬ�˱�
	if(!GetConsortiaUnionTable())
	{
		printf("GetConsortiaUnion fault.\n");
		return false;
	}
	//��ȡ����Լս��
	if(!GetConsortiaFightTable())
	{
		printf("GetConsortiaFight fault.\n");
		return false;
	}

	//��ʼ������
	//Ranking(&m_DBProxy);
	//��ʼ������
	for(int i=1;i<3;i++)
	{
		m_CountryList[i].SetCountryID(i,this);
	}
	getTirrierCountAllMap();
	return true;
}

/**************************************************************************
*˵������������
*���������	num_threads ��������Ϣ�����еĴ����߳���
receiver_func��������Ϣ�Ļص�����
notify_func������֪ͨ�Ļص�����
timer_func����ʱ��֪ͨ�Ļص�����
disconn_func�����ӶϿ��Ļص�����
exception_func���쳣����Ļص�����
*����ֵ��true:�ɹ���false:ʧ��
**************************************************************************/
bool CConsortiaManage::Start(unsigned int num_threads, 
							 DATA_RECEIVER receiver_func, 
							 EVENT_NOTIFY notify_func,
							 TIMER_NOTIFY timer_func,
							 SOCKET_DISCONNECT disconn_func,
							 LOGIC_EXCEPTION exception_func)
{
	//����������Ϣ����

	if(!startTask(num_threads))
	{
		printf("Star task fault.\n");
		return false;
	}

	//����Agent

	if(!init(m_AgentDBParam.AgentIP,m_AgentDBParam.AgentPort,m_AgentDBParam.LogSvrIP,m_AgentDBParam.LogSvrPort,MDM_GP_MAP_CONSORTIA,5,receiver_func,notify_func,timer_func,disconn_func,exception_func))
	{
		//ֹͣ��Ϣ���з���
		printf("Connect Agent fault,the task will stop.\n");
		stopTask();
		return false;
	}

	LPConsortia_Timer pRankingMsg = new Consortia_Timer;
	pRankingMsg->serialize(CC_C_TIMER_RANKING,0);
	if(set_timer(20,pRankingMsg,0) <=0)
	{
		printf("�������ж�ʱ��ʧ��");
	}

	LPConsortia_Timer pTimerParam = new Consortia_Timer;
	pTimerParam->serialize(CC_C_CHECKCONSORTIA,0);
	m_ConCheckTimer = set_timer(CHECK_TIMER,pTimerParam);
	//m_bHasStart = true;
	//��ʼ������
	for(int i=1;i<3;i++)
	{
		m_CountryList[i].InitCountry(&m_DBProxy);
	}
	__int64 times = 0;
	__int64 curtime = IceUtil::Time::now().toSeconds();
	if(m_CountryList[1].GetNextCouncilTime() <= curtime)
	{
		times = GetNextTime() - curtime;
	}
	else
	{
		times = m_CountryList[1].GetNextCouncilTime() - curtime;
	}
	pTimerParam = new Consortia_Timer;
	pTimerParam->serialize(CC_C_TIMER_VOTECOUNCIL,0);
	set_timer((long)times,pTimerParam);
#ifdef PRINT_HINTINFO
	//set_timer(10,pTimerParam);
	printf("����ϵͳ����Աѡ��ʱ�䣺%ld\n",times);
#endif
	//add by xujincai 20090625
	if(times > m_VoteParam.warningtimes)
	{//��ǰ�ʼ�֪ͨѡ�ٿ�ʼ
		times = times - m_VoteParam.warningtimes;
		pTimerParam = new Consortia_Timer;
		pTimerParam->serialize(CC_C_TIMER_BROADBEVOTE,0);
		set_timer((long)times,pTimerParam);
	}
	return true;
}

/**************************************************************************
*˵����ֹͣ����
*���������	
*����ֵ��true:�ɹ���false:ʧ��
**************************************************************************/
bool CConsortiaManage::Stop()
{
	if(!m_bHasStart)
	{
		return false;
	}
	stopTask();
	cancel_timer(m_ConCheckTimer);
	fini();
	ClearAllData();
	return true;
}

/**************************************************************************
*˵��������Ϣ������Ϣ����
*���������msg:��Ϣ��
*����ֵ��true:�ɹ���false:ʧ��
**************************************************************************/
bool CConsortiaManage::Push(char *msg)
{
	return putQueue(msg);
}

bool CConsortiaManage::startTask(std::size_t nThreads)
{
	m_bHasStart = true;
	return IceUtil::Task<char*,MAX_MSG_COUNT>::startTask(nThreads);
}

void CConsortiaManage::stopTask()
{
	m_bHasStart = false;
	/*
	char *msg;
	while (size())
	{
		getQueue(msg);
		exit(msg);
	}
	*/
	IceUtil::Task<char*,MAX_MSG_COUNT>::stopTask();
}

void CConsortiaManage::run()
{
	char *msg = 0;
	CProcMessage m_ProcMsg(this);

	while(m_bHasStart)
	{
		if(getQueue(msg))
		{
			m_ProcMsg.ProcMsg(msg);
			delete[] msg;
		}
	}
    
}


void CConsortiaManage::exit(char *msg)
{
	delete[] msg;
}
/**************************************************************************
*˵������⹫�������Ƿ�24Сʱδ��10�ˣ��������ɢ����
*���������
*����ֵ��
**************************************************************************/
void CConsortiaManage::CheckConsortia()
{
	int i = 0;
	//������й�������
	for(i=0;i<MAX_CONSORTIA_COUNT;++i)
	{
		//�����жϴ����Ƿ񳬹�24Сʱ
		Consortia_Ptr con_ptr;
		///IceUtil::RWRecMutex::RLock rlock(m_Consortia[i].rwmutex_);
		ReadLock_Consortia(i);
		GetConsortia(i,con_ptr);
		Unlock_Consortia(i);
		if(!con_ptr.get())
		{
			continue;
		}
		if((IceUtil::Time::now().toSeconds() - IceUtil::Time::seconds(con_ptr->GetCreateTime()).toSeconds()) <(3600*m_CreateConsortia.Times))
		{//δ����24Сʱ
			continue;
		}
		if(con_ptr->GetUserCount() >= m_CreateConsortia.Personnel)
		{//�����ﵽҪ��
			con_ptr->SetFrtNoFullTime(0);
			continue;
		}
		else
		{
			if(con_ptr->GetFrtNoFullTime() ==0)
			{//��¼�״�δ��������ʱ��
				con_ptr->SetFrtNoFullTime(IceUtil::Time::now().toSeconds());
			}
		}

		//�Ƿ�����24Сʱ
		if((IceUtil::Time::now().toSeconds() - con_ptr->GetFrtNoFullTime()) <(3600*m_CreateConsortia.Times))
		{
			continue;
		}
		//��ɢ����
		bool bDelete = false;
		WriteLock_Consortia(i);
		if(m_Consortia[i].con_ptr.get())
		{
			if(m_Consortia[i].con_ptr->GetConsortiaID() == con_ptr->GetConsortiaID())
			{
				bDelete = true;
				DelConsortia(i);//ɾ������
			}
		}
		Unlock_Consortia(i);
		DelUsedConID(i);
		//�������ݿ�
		if(bDelete)
		{
			//ɾ�������Ա��
			{
				ConsortiaUser_Iter useriter;
				ConsortiaPersonnel deluser;
				for(useriter = con_ptr->ConUserList.begin();useriter != con_ptr->ConUserList.end();useriter++)
				{
					deluser.setmasterid(i);
					deluser.setdetailid(useriter->second->GetPlayerID());
					DelConsortiaUser(i,useriter->second->GetPlayerID());//�ӹ����Ա�б���ɾ��
					if(m_DBProxy.db_delete(deluser) != DB_Proxy::DB_SUCCEED)
					{
						printf("Delete consortia user fault.\n");
					}
#ifdef SND_EVENT_NOTIFY
					//������ϵͳ���͸�����ҹ�����֪ͨ
					Notify_Add_Consortia notify_msg;
					notify_msg.ulPlayerID = useriter->second->GetPlayerID();
					notify_msg.ulConsortiaID = 0;
					notify_msg.serialize();
					event_notify(MDM_GP_MAP_CHAT,(char*)&notify_msg,sizeof(notify_msg));
#endif
				}
			}

			//ɾ������ͬ�˱�
			{
				ConsortiaUnion_Iter unioniter;
				ConsortiaUnion delunion;
				for(unioniter = con_ptr->ConUnionList.begin();unioniter != con_ptr->ConUnionList.end();unioniter++)
				{
					delunion.setid(unioniter->second->GetConsortia1ID(),unioniter->second->GetConsortia2ID());
					if(m_DBProxy.db_delete(delunion) != DB_Proxy::DB_SUCCEED)
					{
						printf("Delete consortia union fault.\n");
					}
				}
			}
			//ɾ������Լս��
			{
				ConsortiaFight_Iter fight_iter;
				ConsortiaFight delfight;
				for(fight_iter = con_ptr->ConFightList.begin();fight_iter != con_ptr->ConFightList.end();fight_iter++)
				{
					delfight.setmasterid(fight_iter->second->GetMapID());
					delfight.setdetailid(fight_iter->second->GetConsortia1ID(),fight_iter->second->GetConsortia2ID());
					if(m_DBProxy.db_delete(delfight) != DB_Proxy::DB_SUCCEED)
					{
						printf("Delete consortia fight fault.\n");
					}
				}
			}
			//ɾ���������б�
			{
				ConsortiaBank_Iter bank_iter;
				ConsortiaBank delbank;
				for(bank_iter = con_ptr->ConBankInfo.begin();bank_iter != con_ptr->ConBankInfo.end();bank_iter++)
				{
					delbank.setmasterid(i);
					delbank.setdetailid(bank_iter->second->GetItemID());
					if(m_DBProxy.db_delete(delbank) != DB_Proxy::DB_SUCCEED)
					{
						printf("Delete consortia bank fault.\n");
					}
				}
			}
			//ɾ�������
			{
				ConsortiaInfo delcon;
				delcon.setid(i);
				if(m_DBProxy.db_delete(delcon) != DB_Proxy::DB_SUCCEED)
				{
					printf("Delete consortia fault.\n");
				}
			}
			
			AddUnusedConID(i);
		}
	}
	/*����ȥ��Լս
	for(i = 0;i<MAX_CONSORTIA_COUNT;++i)
	{
		bool bDelFight = 0;
		Consortia_Ptr con_ptr;
		WriteLock_Consortia(i);
		GetConsortia(i,con_ptr);
		if(con_ptr.get())
		{
			ConsortiaFight_Iter fight_iter;
			for(fight_iter = con_ptr->ConFightList.begin();fight_iter != con_ptr->ConFightList.end();fight_iter++)
			{
				if(fight_iter->second->GetTime() <IceUtil::Time::now().toMilliSeconds())
				{
					con_ptr->DelFight(fight_iter->first);
				}
			}
		}
		Unlock_Consortia(i);
	}
	*/
	//��������б�

	//
	LPConsortia_Timer pTimerParam = new Consortia_Timer;
	pTimerParam->serialize(CC_C_CHECKCONSORTIA,0);
	m_ConCheckTimer = set_timer(CHECK_TIMER,pTimerParam);
}

/**************************************************************************
*˵�����������
*���������
*����ֵ��
**************************************************************************/
void CConsortiaManage::ClearAllData()
{

	for(int i=1;i<MAX_CONSORTIA_COUNT;++i)
	{
		AddUnusedConID(i);
	}
	//ɾ������
	IceUtil::RWRecMutex::WLock wlock_player(m_PlayerRankingMutex);
	if(m_PlayerRankingSize)
	{
		delete[] m_PlayerRanking;
	}
	m_PlayerRankingSize = 0;
	if(m_CollectionRateRanking)
	{
		delete[] m_CollectionRateRanking;
	}
	m_CollectionRateRanking = 0;
	if(m_ContributeRanking)
	{
		delete[] m_ContributeRanking;
	}
	m_ContributeRanking = 0;
	if(m_EnemyRanking)
	{
		delete[] m_EnemyRanking;
	}
	m_EnemyRanking = 0;

	IceUtil::RWRecMutex::WLock wlock_con(m_ConsortiaRankingMutex);
	if(m_ConsortiaRankingSize)
	{
		delete[] m_ConosrtiaRanking;
	}
	m_ConsortiaRankingSize = 0;
	//ɾ��������Ϣ���ñ�
	GameCityInfo_Map_Iterator cityinfo_iter;
	GameCityInfo *pDelCityInfo = 0;
	for(cityinfo_iter = m_GameCityInfoMap.begin();cityinfo_iter != m_GameCityInfoMap.end();cityinfo_iter++)
	{
		if(pDelCityInfo = cityinfo_iter->second)
		{
			delete pDelCityInfo;
			pDelCityInfo = 0;
		}
	}
	m_GameCityInfoMap.clear();

	PublicBuildParamTable_Iterator publicparam_iter;
	PublicBuildParamTable *pDelParam = 0;
	for(publicparam_iter = m_PublicBuildParam.begin();publicparam_iter != m_PublicBuildParam.end();publicparam_iter++)
	{
		if(pDelParam = publicparam_iter->second)
		{
			delete pDelParam;
			pDelParam = 0;
		}
	}
	m_PublicBuildParam.clear();

	//�����������������
	TirrierParam_Map_Iterator tirrierparam_iter;
	LPTirrierParam pDelTirrierParam = 0;
	for(tirrierparam_iter = m_TirrierParamList.begin();tirrierparam_iter != m_TirrierParamList.end();tirrierparam_iter++)
	{
		if(pDelTirrierParam = tirrierparam_iter->second)
		{
			delete pDelTirrierParam;
			pDelTirrierParam = 0;
		}
	}
	m_TirrierParamList.clear();

	clearMailInfoParam();
	clearCareerConfig();
	clearLegionTitleParam();
	clearConsortiaUpdateParam();
}
/**************************************************************************
*˵������ȡ��ұ�
*���������
*����ֵ��
**************************************************************************/
void CConsortiaManage::ReadLanguageConfig(const char *filename)
{
	if(filename == NULL)
		return;
	//�㲥����
	/*
	Default_Notice,			//Ĭ��
	CreateCons_Notice,		//��������
	DelConsortia_Notice,	//��ɢ����
	JoinConsortia_Notice,	//���빫��
	KickUser_Notice,		//Kick��Ա
	QuitConsortia_Notice,	//�˳�����
	PoseOfficail_Notice,	//��ְ
	DeposeOfficail_Notice,	//��ְ
	TranChairman_Notice,	//�ƽ��᳤
	*/
	ReadConfig(filename,"NoticeInfo","Default",m_NoticeParam[Default_Notice],SIZE_128);
	ReadConfig(filename,"NoticeInfo","CreateConsortia",m_NoticeParam[CreateCons_Notice],SIZE_128);
	ReadConfig(filename,"NoticeInfo","DelConsortia",m_NoticeParam[DelConsortia_Notice],SIZE_128);
	ReadConfig(filename,"NoticeInfo","JoinConsortia",m_NoticeParam[JoinConsortia_Notice],SIZE_128);
	ReadConfig(filename,"NoticeInfo","KickUser",m_NoticeParam[KickUser_Notice],SIZE_128);
	ReadConfig(filename,"NoticeInfo","QuitConsortia",m_NoticeParam[QuitConsortia_Notice],SIZE_128);
	ReadConfig(filename,"NoticeInfo","PoseOfficail",m_NoticeParam[PoseOfficail_Notice],SIZE_128);
	ReadConfig(filename,"NoticeInfo","DeposeOfficail",m_NoticeParam[DeposeOfficail_Notice],SIZE_128);
	ReadConfig(filename,"NoticeInfo","TranChairman",m_NoticeParam[TranChairman_Notice],SIZE_128);

	//Email����
	/*
	*/
	//JoinApply_Email,		���������ʼ�����
	ReadConfig(filename,"JoinApplyEmail","Title",m_EmailParam[JoinApply_Email].Title,SIZE_128);
	ReadConfig(filename,"JoinApplyEmail","Body",m_EmailParam[JoinApply_Email].Body,SIZE_128);
	//AgreeJoin_Email,		ͬ�����
	ReadConfig(filename,"AgreeJoinEmail","Title",m_EmailParam[AgreeJoin_Email].Title,SIZE_128);
	ReadConfig(filename,"AgreeJoinEmail","Body",m_EmailParam[AgreeJoin_Email].Body,SIZE_128);
	//RefuseJoin_Email,		�ܾ�����
	ReadConfig(filename,"RefuseJoinEmail","Title",m_EmailParam[RefuseJoin_Email].Title,SIZE_128);
	ReadConfig(filename,"RefuseJoinEmail","Body",m_EmailParam[RefuseJoin_Email].Body,SIZE_128);
	//KickUser_Email,			Kick��Ա
	ReadConfig(filename,"KickUserEmail","Title",m_EmailParam[KickUser_Email].Title,SIZE_128);
	ReadConfig(filename,"KickUserEmail","Body",m_EmailParam[KickUser_Email].Body,SIZE_128);
	//InviteEmail,			�������
	ReadConfig(filename,"InviteEmail","Title",m_EmailParam[InviteEmail].Title,SIZE_128);
	ReadConfig(filename,"InviteEmail","Body",m_EmailParam[InviteEmail].Body,SIZE_128);
	//AgreeInvite_Email,		ͬ������
	ReadConfig(filename,"AgreeInviteEmail","Title",m_EmailParam[AgreeInvite_Email].Title,SIZE_128);
	ReadConfig(filename,"AgreeInviteEmail","Body",m_EmailParam[AgreeInvite_Email].Body,SIZE_128);
	//RefuseInvite_Email,		�ܾ�����
	ReadConfig(filename,"RefuseInviteEmail","Title",m_EmailParam[RefuseInvite_Email].Title,SIZE_128);
	ReadConfig(filename,"RefuseInviteEmail","Body",m_EmailParam[RefuseInvite_Email].Body,SIZE_128);
	//PoseOfficial_Email,		��ְ֪ͨ
	ReadConfig(filename,"PoseOfficialEmail","Title",m_EmailParam[PoseOfficial_Email].Title,SIZE_128);
	ReadConfig(filename,"PoseOfficialEmail","Body",m_EmailParam[PoseOfficial_Email].Body,SIZE_128);
	//DeposeOfficial_Email,	��ְ֪ͨ
	ReadConfig(filename,"DeposeOfficialEmail","Title",m_EmailParam[DeposeOfficial_Email].Title,SIZE_128);
	ReadConfig(filename,"DeposeOfficialEmail","Body",m_EmailParam[DeposeOfficial_Email].Body,SIZE_128);
	//TranChairman_Email,		�ƽ��᳤
	ReadConfig(filename,"TranChairmanEmail","Title",m_EmailParam[TranChairman_Email].Title,SIZE_128);
	ReadConfig(filename,"TranChairmanEmail","Body",m_EmailParam[TranChairman_Email].Body,SIZE_128);
	//AllyApply_Email,		��������
	ReadConfig(filename,"AllyApplyEmail","Title",m_EmailParam[AllyApply_Email].Title,SIZE_128);
	ReadConfig(filename,"AllyApplyEmail","Body",m_EmailParam[AllyApply_Email].Body,SIZE_128);
	//AgreeAlly_Email,		ͬ�����
	ReadConfig(filename,"AgreeAllyEmail","Title",m_EmailParam[AgreeAlly_Email].Title,SIZE_128);
	ReadConfig(filename,"AgreeAllyEmail","Body",m_EmailParam[AgreeAlly_Email].Body,SIZE_128);
	//RefuseAlly_Email,		�ܾ�����
	ReadConfig(filename,"RefuseAllyEmail","Title",m_EmailParam[RefuseAlly_Email].Title,SIZE_128);
	ReadConfig(filename,"RefuseAllyEmail","Body",m_EmailParam[RefuseAlly_Email].Body,SIZE_128);
	//Unally_Email			����
	ReadConfig(filename,"UnallyEmail","Title",m_EmailParam[Unally_Email].Title,SIZE_128);
	ReadConfig(filename,"UnallyEmail","Body",m_EmailParam[Unally_Email].Body,SIZE_128);
}
/**************************************************************************
*˵������ȡ��ұ�
*���������
*����ֵ��
**************************************************************************/
bool CConsortiaManage::GetPlayerAccTable()
{
	//��ȡ�����Ӫ
	PlayerChatRoomTable player_tb;
	PlayerChatRoomTable *pRetSet = 0;
	char *retbuf = 0;
	int count = 0;
	int iRet = m_DBProxy.db_select_all(player_tb,retbuf,count);
	if(iRet == DB_Proxy::DB_FAILED)
	{
		if(retbuf)
		{
			delete[] retbuf;
		}
		return false;
	}
	else if(iRet == DB_Proxy::DB_EOF)
	{
		if(retbuf)
		{
			delete[] retbuf;
		}
		return true;
	}

	count = count /sizeof(PlayerChatRoomTable);
	if(count <0)
	{
		if(retbuf)
		{
			delete[] retbuf;
		}
		return false;
	}
	pRetSet = (PlayerChatRoomTable*)retbuf;
#ifdef PRINT_HINTINFO
	printf("========================��ȡ��ұ�=================================\n");
#endif
	for(int i=0;i<count;++i)
	{
		//��ȡ�������
		PlayerOfficerTable officertb;
		officertb.setmasterid(pRetSet[i].playerid_);
		officertb.setdetailid(pRetSet[i].playerid_);
		if(m_DBProxy.db_select(officertb) == DB_Proxy::DB_SUCCEED)
		{
 			AddPlayer(pRetSet[i].playerid_,officertb.officername_,pRetSet[i].country_,false,0);
#ifdef PRINT_HINTINFO
			//printf("%d,%s,%d,%d\n",pRetSet[i].playerid_,officertb.officername_,pRetSet[i].country_,pRetSet[i].money_);
#endif
		}
		else
		{
			printf("Get user name fault(playerid:%d)\n",pRetSet[i].playerid_);
		}
	}

	delete[] retbuf;
	retbuf = 0;
	
	return true;
}

/**************************************************************************
*˵������ȡ�����
*���������
*����ֵ��
**************************************************************************/
bool CConsortiaManage::GetConsortiaTable()
{
	ConsortiaInfo con_tb;
	ConsortiaInfo *pRetSet = 0;
	char *retbuf = 0;
	int count = 0;
	int iRet =m_DBProxy.db_select_all(con_tb,retbuf,count);
	if( iRet == DB_Proxy::DB_FAILED)
	{
		if(retbuf)
		{
			delete[] retbuf;
			retbuf = 0;
		}
		return false;
	}
	else if(iRet == DB_Proxy::DB_EOF)
	{
		if(retbuf)
		{
			delete[] retbuf;
			retbuf = 0;
		}
		return true;
	}
	count = count/sizeof(ConsortiaInfo);
	if(count <0)
	{
		if(retbuf)
		{
			delete[] retbuf;
			retbuf = 0;
		}
		return false;
	}

#ifdef PRINT_HINTINFO
	printf("========================��ȡ�����=================================\n");
#endif
	pRetSet = (ConsortiaInfo *)retbuf;
	char szName[32] = {0};
	int iCountryID = 0;
	bool bOnline = false;
	int iAgent = 0;
	bool bModify = false;
	ConsortiaUpdateParam* pUpdateParam  = getConsortiaUpdateParam(1);
	for(int i=0;i<count;++i)
	{
		bModify = false;
		//�᳤��Ϣ
		if (GetPlayerInfo(pRetSet[i].chairmanid_,szName,iCountryID,bOnline,iAgent,&m_DBProxy) !=0)
		{
			m_DBProxy.db_delete(pRetSet[i]);
			continue;
		}
		if (pRetSet[i].level_ <=0)
		{
			pRetSet[i].level_  = 1;
			
			if (pUpdateParam)
			{
				pRetSet[i].maxusernum_ = pUpdateParam->maxUserNum;
			}
			bModify = true;
		}
		else if (pRetSet[i].maxusernum_ <=0)
		{
			if (pUpdateParam)
			{
				pRetSet[i].maxusernum_ = pUpdateParam->maxUserNum;
			}
			bModify = true;
		}
		Consortia_Ptr con_ptr(new CConsortia(pRetSet[i].consortiaid_,pRetSet[i].name_,pRetSet[i].mapid_,pRetSet[i].cityid_,
			pRetSet[i].chairmanid_,pRetSet[i].createtime_,iCountryID));

		con_ptr->SetTeachID(pRetSet[i].techid_);
		con_ptr->SetFaceID(pRetSet[i].faceid_);
		con_ptr->SetPlacard(pRetSet[i].placard_);
		con_ptr->SetFrtNoFullTime(0);
		con_ptr->SetLevel(pRetSet[i].level_);
		con_ptr->SetTenet(pRetSet[i].tenet_);
		con_ptr->setMaxUserNum(pRetSet[i].maxusernum_);
		con_ptr->setMoney(pRetSet[i].money_);
		con_ptr->setExp(pRetSet[i].exp_);
		con_ptr->setHonors(pRetSet[i].honors_);
		con_ptr->setSignID(pRetSet[i].signid_);
		con_ptr->setShortName(pRetSet[i].shortname_);

		AddConsortia(pRetSet[i].consortiaid_,con_ptr);
		DelUnusedConID(pRetSet[i].consortiaid_);
		AddUsedConID(pRetSet[i].consortiaid_);
		GetConsortiaUserTable(con_ptr);
		GetConsortiaBankTable(con_ptr);
		if(con_ptr->GetUserCount() != pRetSet[i].count_)
		{
			pRetSet[i].count_ = con_ptr->GetUserCount();
			bModify = true;
		}
		if (bModify)
		{
			m_DBProxy.db_update(pRetSet[i],pRetSet[i]);
		}
	}
	delete[] retbuf;
	retbuf = 0;
	return true;
}

/**************************************************************************
*˵������ȡ�����Ա��
*���������
*����ֵ��
**************************************************************************/
bool CConsortiaManage::GetConsortiaUserTable(Consortia_Ptr &con_ptr)
{
	ConsortiaPersonnel per_tb;
	ConsortiaPersonnel *pRetSet = 0;
	char *retbuf = 0;
	int count = 0;
	per_tb.setmasterid(con_ptr->GetConsortiaID());
	int iRet = m_DBProxy.db_multi_select(per_tb,retbuf,count);
	if(iRet == DB_Proxy::DB_FAILED)
	{
		if(retbuf)
		{
			delete[] retbuf;
		}
		return false;
	}
	else if(iRet == DB_Proxy::DB_EOF)
	{
		if(retbuf)
		{
			delete[] retbuf;
		}
		return true;
	}
	count = count/sizeof(ConsortiaPersonnel);
	if(count <0)
	{
		if(retbuf)
		{
			delete[] retbuf;
		}
		return false;
	}

#ifdef PRINT_HINTINFO
	printf("========================��ȡ�����Ա��=================================\n");
#endif
	pRetSet = (ConsortiaPersonnel*)retbuf;

	int iCountryID = 0;
	bool bOnline = false;
	int iAgent = 0;
	PlayerChatRoomTable rootb;
	bool bModify = false;
	int lTitleID = getLigionTitleID(0,0);
	for(int i=0;i<count;++i)
	{
		bModify = false;
		if (GetPlayerInfo(pRetSet[i].playerid_,0,iCountryID,bOnline,iAgent,&m_DBProxy) !=0)
		{
			m_DBProxy.db_delete(pRetSet[i]);
			continue;
		}
		if (iCountryID != con_ptr->GetCountryID())
		{
			printf("���:%d,��Ӫ:%d,����%d��Ӫ%d\n",pRetSet[i].playerid_,iCountryID,con_ptr->GetConsortiaID(),con_ptr->GetCountryID());
			m_DBProxy.db_delete(pRetSet[i]);
			continue;
		}
		rootb.setid(pRetSet[i].playerid_);
		if (m_DBProxy.db_select(rootb) == DB_Proxy::DB_SUCCEED && rootb.consortiaid_ !=con_ptr->GetConsortiaID())
		{
			rootb.consortiaid_ =con_ptr->GetConsortiaID();
			m_DBProxy.db_update(rootb,rootb);
		}
		if (pRetSet[i].conjob_ ==0)
		{
			pRetSet[i].conjob_ = LEAGUER_JOB;
			bModify = true;
		}
		if (pRetSet[i].legiontitle_ <lTitleID)
		{
			pRetSet[i].legiontitle_ = lTitleID;
			bModify = true;
		}
		if (CHARIRMAN_JOB == pRetSet[i].conjob_ && GetOfficialPurview(pRetSet[i].conjob_) != pRetSet[i].purview_)
		{
			pRetSet[i].purview_ = GetOfficialPurview(pRetSet[i].conjob_);
			bModify = true;
		}
		if (bModify)
		{

			m_DBProxy.db_update(pRetSet[i],pRetSet[i]);
		}
		ConsortiaUser_Ptr user_ptr(new CConsortiaUser(pRetSet[i].playerid_,pRetSet[i].consortiaid_,pRetSet[i].conjob_,pRetSet[i].purview_,pRetSet[i].type_));

		user_ptr->setContribute(pRetSet[i].contribute_);
		user_ptr->setLegionTitle(pRetSet[i].legiontitle_);
		user_ptr->setSalariedDay(pRetSet[i].salariedday_);
		user_ptr->setCredit(pRetSet[i].honors_);
		user_ptr->setExp(pRetSet[i].exp_);
		user_ptr->setMoney(pRetSet[i].money_);
		//��ӵ������Ա����
		if(AddConsortiaUser(pRetSet[i].consortiaid_,user_ptr))
		{
			m_DBProxy.db_delete(pRetSet[i]);
		}
	}
	delete[] retbuf;
	retbuf = 0;
	return true;
}

/**************************************************************************
*˵������ȡ����ͬ�˱�
*���������
*����ֵ��
**************************************************************************/
bool CConsortiaManage::GetConsortiaUnionTable()
{
	ConsortiaUnion union_tb;
	ConsortiaUnion *pRetSet = 0;
	char *retbuf = 0;
	int count = 0;
	int iRet = m_DBProxy.db_select_all(union_tb,retbuf,count);
	if(iRet ==DB_Proxy::DB_FAILED)
	{
		if(retbuf)
		{
			delete[] retbuf;
		}
		return false;
	}
	else if(iRet == DB_Proxy::DB_EOF)
	{
		if(retbuf)
		{
			delete[] retbuf;
		}
		return true;
	}
	count = count/sizeof(ConsortiaUnion);
	if(count <0)
	{
		if(retbuf)
		{
			delete[] retbuf;
		}
		return false;
	}
#ifdef PRINT_HINTINFO
	printf("========================��ȡ����ͬ�˱�=================================\n");
#endif
	pRetSet = (ConsortiaUnion*)retbuf;
	for(int i=0;i<count;++i)
	{
		ConsortiaUnion_Ptr union_ptr(new CConsortiaUnion(pRetSet[i].unionid_,pRetSet[i].consortia1id_,pRetSet[i].consortia2id_));
		//��ӵ������ͬ���б��У�ͬ���漰����������
		AddConsortiaUnion(pRetSet[i].consortia1id_,union_ptr);
		AddConsortiaUnion(pRetSet[i].consortia2id_,union_ptr);
	}
	delete[] retbuf;
	return true;
}

/**************************************************************************
*˵������ȡ�������б�
*���������
*����ֵ��
**************************************************************************/
bool CConsortiaManage::GetConsortiaBankTable(Consortia_Ptr &con_ptr)
{
	ConsortiaBank bank_tb;
	ConsortiaBank *pRetSet = 0;
	char *retbuf = 0;
	int count = 0;
	bank_tb.setmasterid(con_ptr->GetConsortiaID());
	int iRet = m_DBProxy.db_multi_select(bank_tb,retbuf,count);
	if( iRet == DB_Proxy::DB_FAILED)
	{
		if(retbuf)
		{
			delete[] retbuf;
		}
		return false;
	}
	else if(iRet == DB_Proxy::DB_EOF)
	{
		if(retbuf)
		{
			delete[] retbuf;
		}
		if(retbuf)
		{
			delete[] retbuf;
		}
		return true;
	}
	count = count/sizeof(ConsortiaBank);
	if(count <0)
	{
		if(retbuf)
		{
			delete[] retbuf;
		}
		return false;
	}
#ifdef PRINT_HINTINFO
	printf("========================��ȡ�������б�=================================\n");
#endif
	pRetSet = (ConsortiaBank*)retbuf;
	for(int i=0;i<count;++i)
	{
		//ConsortiaBank_Ptr bank_ptr(new CConsortiaBank(pRetSet[i].consortiaid_,pRetSet[i].itemid_,pRetSet[i].itemcount_));
		AddConsortiaBank(pRetSet[i].consortiaid_,pRetSet[i].itemid_,pRetSet[i].itemcount_);
	}
	delete[] retbuf;
	retbuf = 0;
	return true;
}

/**************************************************************************
*˵������ȡ����Լս��
*���������
*����ֵ��
**************************************************************************/
bool CConsortiaManage::GetConsortiaFightTable()
{
	ConsortiaFight fight_tb;
	ConsortiaFight *pRetSet = 0;
	char *retbuf = 0;
	int count = 0;
	int iRet = m_DBProxy.db_select_all(fight_tb,retbuf,count) ;
	if(iRet == DB_Proxy::DB_FAILED)
	{
		if(retbuf)
		{
			delete[] retbuf;
		}
		return false;
	}
	else if(iRet == DB_Proxy::DB_EOF)
	{
		if(retbuf)
		{
			delete[] retbuf;
		}
		return true;
	}
	count = count/sizeof(ConsortiaFight);
	if(count <0)
	{
		if(retbuf)
		{
			delete[] retbuf;
		}
		return false;
	}
#ifdef PRINT_HINTINFO
	printf("========================��ȡ����Լս��=================================\n");
#endif
	pRetSet = (ConsortiaFight*)retbuf;
	for(int i=0;i<count;++i)
	{
		ConsortiaFight_Ptr fight_ptr(new CConsortiaFight(pRetSet[i].fightid_,pRetSet[i].consortia1id_,pRetSet[i].consortia2id_,
			pRetSet[i].mapid_,pRetSet[i].cityid_,pRetSet[i].time_));
		//��ӵ������Լս�б��У�Լս�漰����������
		AddConsortiaFight(pRetSet[i].consortia1id_,fight_ptr);
		AddConsortiaFight(pRetSet[i].consortia2id_,fight_ptr);
	}
	delete[] retbuf;
	return true;
}

/**************************************************************************
*˵������ȡ����ְλ��
*���������
*����ֵ��
**************************************************************************/
bool CConsortiaManage::GetConsortiaOfficialTable()
{
	FILE *fp = 0;
	fp = fopen("configtable/ConsortiaOfficial.csv","r");
	if(fp == NULL)
	{
		printf("��configtable/ConsortiaOfficial.csvʧ�ܡ�\n");
		return false;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);
	//
	int officialID = 0;
	char officialName[SIZE_16]={0};
	int level = 0;
	int purview = 0;
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		if(sscanf(buf,"%d,%[^,],%d,%d",
			&officialID,&officialName,&level,&purview)<4)
		{
			continue;
		}

		ConsortiaOfficial_Ptr off_ptr(new CConsortiaOfficial(officialID,officialName,purview,level));
		m_ConOfficialMap[officialID] = off_ptr;
	}
	fclose(fp);
	return true;
}

/**************************************************************************
*˵������ȡ���������������
*���������
*����ֵ��
**************************************************************************/
bool CConsortiaManage::GetCreateConsortiaLimitTable()
{
	FILE *fp = 0;
	fp = fopen("configtable/CreateConsortia.csv","r");
	if(fp == NULL)
	{
		printf("��configtable/CreateConsortia.csvʧ�ܡ�\n");
		return false;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);
	if(fgets(buf,1024,fp) == NULL)
		return false;

	if(sscanf(buf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
		&m_CreateConsortia.ResID ,&m_CreateConsortia.FoodCount,&m_CreateConsortia.WoodCount,&m_CreateConsortia.IronCount,
		&m_CreateConsortia.SkinCount,&m_CreateConsortia.RedStoneCount,&m_CreateConsortia.YollowStoneCount,&m_CreateConsortia.GreenStoneCount,
		&m_CreateConsortia.BackIronCount,&m_CreateConsortia.Times,&m_CreateConsortia.Personnel,&m_CreateConsortia.Science,
		&m_CreateConsortia.Money,&m_CreateConsortia.Level,&m_CreateConsortia.JoinLevel)<15)
	{
		return false;
	}
	fclose(fp);
	return true;
}

bool CConsortiaManage::loadGameCityInfo()
{
	FILE *fp = 0;
	fp = fopen("configtable/GameCityInfo.csv","r");
	if(fp == NULL)
	{
		printf("��configtable/GameCityInfo.csvʧ�ܡ�\n");
		return false;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	GameCityInfo* pItem = 0;
	//m_ConstructionConfigureMultimap
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new GameCityInfo;
		if(sscanf(buf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&pItem->cityid_,&pItem->mapid_,&pItem->foodcount_,&pItem->woodcount_,
			&pItem->ironcount_,&pItem->skincount_,&pItem->x_,&pItem->y_,
			&pItem->human_,&pItem->mannikin_,&pItem->fairy_,&pItem->dandiprat_,
			&pItem->halfanimal_,&pItem->cattle_,&pItem->blackfairy_,&pItem->decpesrson_)<16)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		m_GameCityInfoMap[pItem->cityid_] = pItem;
	}
	fclose(fp);

	return true;
}

bool CConsortiaManage::loadPublicBuildParamTable()
{
	FILE *fp = 0;
	int key = 0;
	fp = fopen("configtable/PublicBuildParamtable.csv","r");
	if(fp == NULL)
	{
		printf("��PublicBuildParamtable.csv�ļ�ʧ�ܣ�\n");
		return false;
	}
	char fileBuf[1024]= {0};
	fgets(fileBuf,1024,fp);
	PublicBuildParamTable * buildParamPtr = 0;

	while(!feof(fp))
	{
		if(fgets(fileBuf,1024,fp) == NULL)
			break;
		buildParamPtr = new PublicBuildParamTable;
		if (buildParamPtr == NULL)
		{
			continue;
		}
		if(sscanf(fileBuf,"%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&buildParamPtr->pbuildingkind_ , &buildParamPtr->name_ ,&buildParamPtr->level_,&buildParamPtr->templateid_ , &buildParamPtr->attack_,
			&buildParamPtr->attackkind_, &buildParamPtr->upgradefee_,&buildParamPtr->foodcount_, &buildParamPtr->woodcount_ , &buildParamPtr->ironcount_,
			&buildParamPtr->skincount_ , &buildParamPtr->attackscop_ ,&buildParamPtr->visualrange_,&buildParamPtr->denfenid_,&buildParamPtr->hp_,
			&buildParamPtr->exp_ , &buildParamPtr->skill_ , &buildParamPtr->skillodds_,&buildParamPtr->output_,&buildParamPtr->armyCardsType_,
			&buildParamPtr->cardsDropOdds_)< 15)
		{
			delete buildParamPtr;
			buildParamPtr = 0;
			continue;
		}
		m_PublicBuildParam[buildParamPtr->pbuildingkind_ * 100 + buildParamPtr->level_] = buildParamPtr;
	}
	fclose(fp);
	return true;
}
bool CConsortiaManage::loadTirrierParam()
{
	FILE *fp = 0;
	char buf[1024] = {0};
	fp = fopen("configtable/TirrierParam.csv","r");
	if(fp == NULL)
	{
		printf("��configtable/TirrierParam.csvʧ�ܡ�\n");
		return false;
	}

	fgets(buf,1024,fp);
	LPTirrierParam pNewNode = 0;
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pNewNode = new TirrierParam;
		if(sscanf(buf,"%d,%d,%d,%d,%d,%d,%d",&pNewNode->TirrierType,&pNewNode->SoldierType,
			&pNewNode->WeaponType,&pNewNode->WeaponLevel,&pNewNode->DefendType,&pNewNode->DefendLevel,
			&pNewNode->Money) <7)
		{
			delete pNewNode;
			pNewNode = 0;
			continue;
		}

		m_TirrierParamList[pNewNode->TirrierType] = pNewNode;
	}
	fclose(fp);
	return true;
}
/**************************************************************************
*˵������ȡһ��δʹ�õĹ���ID
*���������
*����ֵ������ID
**************************************************************************/
int CConsortiaManage::GetPlayerAcc(int playerid,DB_Proxy *pDBProxy,char *name,int &country,bool &isonline,int &agent)
{
	//��ȡ��Ӫ
	PlayerChatRoomTable roomtb;
	roomtb.setid(playerid);
	if(m_DBProxy.db_select(roomtb) != DB_Proxy::DB_SUCCEED)
	{
		return 1;
	}
	country = roomtb.country_;
	//��ȡ����
	PlayerOfficerTable playertb;
	playertb.setmasterid(playerid);
	playertb.setdetailid(playerid);
	if(m_DBProxy.db_select(playertb) != DB_Proxy::DB_SUCCEED)
	{
		return 2;
	}
	if(name)
	{
		strcpy(name,playertb.officername_);
	}
	AddPlayer(playerid,playertb.officername_,country,isonline,agent);
	return 0;
}
/**************************************************************************
*˵������ȡһ��δʹ�õĹ���ID
*���������
*����ֵ������ID
**************************************************************************/
int CConsortiaManage::GetUnusedConID()
{
	int id = -1;
	std::map<int,int>::iterator iter;
	IceUtil::Mutex::Lock lock(m_UnusedMutex);
	iter = m_UnusedConID.begin();
	if(iter != m_UnusedConID.end())
	{
		id = iter->second;
		m_UnusedConID.erase(id);
	}
	return id;
}
/**************************************************************************
*˵��������һ��δʹ�õĹ���ID
*���������
*����ֵ��
**************************************************************************/
int CConsortiaManage::AddUnusedConID(int id)
{
	IceUtil::Mutex::Lock lock(m_UnusedMutex);
	m_UnusedConID[id] = id;
	return 0;
}
int CConsortiaManage::DelUnusedConID(int id)
{
	IceUtil::Mutex::Lock lock(m_UnusedMutex);
	m_UnusedConID.erase(id);
	return 0;
}
/**************************************************************************
*˵��������һ��ʹ�õĹ���ID
*���������
*����ֵ��
**************************************************************************/
int CConsortiaManage::AddUsedConID(int id)
{
	IceUtil::RWRecMutex::WLock wlock(m_UsedMutex);
	m_UsedConID_Map[id] = id;
	return 0;
}
/**************************************************************************
*˵��������һ��ʹ�õĹ���ID
*���������
*����ֵ��
**************************************************************************/
int CConsortiaManage::DelUsedConID(int id)
{
	IceUtil::RWRecMutex::WLock wlock(m_UsedMutex);
	m_UsedConID_Map.erase(id);
	return 0;
}
/**************************************************************************
*˵�������ݹ��������ж������Ƿ��Ѵ���ID
*���������
*����ֵ��
**************************************************************************/
bool CConsortiaManage::IsExistConsortiaName(const char *name)
{
	bool bRet = false;
	
	for(int i=0;i<MAX_CONSORTIA_COUNT;++i)
	{//
		Consortia_Ptr con_ptr;
		m_Consortia[i].rwmutex_.readLock();
		con_ptr = m_Consortia[i].con_ptr;
		m_Consortia[i].rwmutex_.unlock();
		if(con_ptr.get())
		{//�ж��Ƿ����
			if(strcmp(con_ptr->GetName(),name) ==0)
			{//���ҵ�
				bRet = true;
				break;
			}
		}
	}
	return bRet;
}

/**************************************************************************
*˵������ȡ��������ĵȼ�����ID
*���������
*����ֵ��
**************************************************************************/
int CConsortiaManage::GetLevelLimit()
{
	return m_CreateConsortia.Level;
}
/**************************************************************************
*˵������ȡ�����������ĵĽ�ǮID
*���������
*����ֵ��
**************************************************************************/
int	CConsortiaManage::GetMoneyLimit()
{
	return m_CreateConsortia.Money;
}
/**************************************************************************
*˵������ȡ���빫����Ҫ�ĵȼ�
*���������
*����ֵ��
**************************************************************************/
int CConsortiaManage::GetJoinConLimit()
{
	return m_CreateConsortia.JoinLevel;
}
/**************************************************************************
*˵������ȡ��������
*���������
*����ֵ��
**************************************************************************/
int CConsortiaManage::GetConsortCount()
{
	int count = 0;
	IceUtil::RWRecMutex::RLock rlock(m_UsedMutex);
	count =(int) m_UsedConID_Map.size();
	return (count>0)?count:0;
}

/**************************************************************************
*˵������������ز���
*���������
*����ֵ��
**************************************************************************/
void CConsortiaManage::WriteLock_Consortia(int consortiaid)
{
	LOG_TRACE_EX(consortiaid >=0 && consortiaid<MAX_CONSORTIA_COUNT,("WriteLock_Consortia-->����ID�Ƿ�%d",consortiaid));
	if(consortiaid >=0 && consortiaid<MAX_CONSORTIA_COUNT)
		m_Consortia[consortiaid].rwmutex_.writeLock();
}
void CConsortiaManage::Unlock_Consortia(int consortiaid)
{
	LOG_TRACE_EX(consortiaid >=0 && consortiaid<MAX_CONSORTIA_COUNT,("Unlock_Consortia-->����ID�Ƿ�%d",consortiaid));
	if(consortiaid >=0 && consortiaid<MAX_CONSORTIA_COUNT)
		m_Consortia[consortiaid].rwmutex_.unlock();
}
void CConsortiaManage::ReadLock_Consortia(int consortiaid)
{
	LOG_TRACE_EX(consortiaid >=0 && consortiaid<MAX_CONSORTIA_COUNT,("ReadLock_Consortia-->����ID�Ƿ�%d",consortiaid));
	if(consortiaid >=0 && consortiaid<MAX_CONSORTIA_COUNT)
		m_Consortia[consortiaid].rwmutex_.readLock();
}
/**************************************************************************
*˵������ȡ�����б�
*���������
*����ֵ��
**************************************************************************/
int CConsortiaManage::GetConsortList(LPConsortia_Info con_list,int &count,int start,int consortiaid,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::RLock rlock(m_UsedMutex);
	std::map<int,int>::iterator iter;
	int index = 0;
	int id = 0;
	int pass = 0;
	int chairman_id;
	char chairman_name[33] = {0};
	int country;
	bool isonline = false;
	int agent = 0;
	for(iter = m_UsedConID_Map.begin();iter != m_UsedConID_Map.end();iter++)
	//for(id =0;id<MAX_CONSORTIA_COUNT;id++)
	{
		id = iter->second;
		Consortia_Ptr con_ptr;
		ReadLock_Consortia(id);
		con_ptr = m_Consortia[id].con_ptr;
		Unlock_Consortia(id);
		if(con_ptr.get())
		{
			if((pass++) <start)
			{
				continue;
			}
			con_list[index].ConsortiaID = con_ptr->GetConsortiaID();
			strncpy(con_list[index].ConName ,con_ptr->GetName(),SIZE_32);
			con_list[index].Level = con_ptr->GetLevel();
			con_list[index].Count = con_ptr->GetUserCount();
			chairman_id = con_ptr->GetChairmanID();
			con_list[index].FaceID = con_ptr->GetFaceID();
			con_list[index].countryID = con_ptr->GetCountryID();
			if(GetPlayerInfo(chairman_id,chairman_name,country,isonline,agent,pDBProxy))
			{
				GetPlayerAcc(chairman_id,pDBProxy,chairman_name,country,isonline,agent);
			}
			strncpy(con_list[index].ChairmanName,chairman_name,SIZE_32);
			strncpy(con_list[index].Tenet,con_ptr->GetTenet(),SIZE_128);
			if(consortiaid)
			{
				con_list[index].IsUnion = con_ptr->IsUnionConsortia(consortiaid);
				con_list[index].IsFight = con_ptr->IsFightConsortia(consortiaid);
			}
			else
			{
				con_list[index].IsUnion = 0;
				con_list[index].IsFight = 0;
			}
			//add 20100302
			con_list[index].IsRoyal = con_ptr->isRoyalConsortia();
			con_list[index].MaxUserNum = con_ptr->getMaxUserNum();
			con_list[index].Money = con_ptr->getMoney();
			con_list[index].Exp = con_ptr->getExp();
			con_list[index].Credit = con_ptr->getHonors();
			con_list[index].SignID = con_ptr->getSignID();

			if((++index) >= count && count >0)
			{
				break;
			}
			chairman_name[0] = 0;
		}
		else
		{
		}
	}
	count = index;
	return 0;
}
/**************************************************************************
*˵�������ݹ���ID��ȡ����,û�м�����ע���̰߳�ȫ����
*���������
*����ֵ��
**************************************************************************/
int CConsortiaManage::GetConsortia(int consortiaid,Consortia_Ptr &con_ptr)
{
	//IceUtil::RWRecMutex::RLock rlock(m_Consortia[consortiaid].rwmutex_);
	con_ptr = m_Consortia[consortiaid].con_ptr;
	//
	return 0;
}

/**************************************************************************
*˵�������ݹ���IDɾ������,û�м�����ע���̰߳�ȫ����
*���������
*����ֵ��
**************************************************************************/
int CConsortiaManage::DelConsortia(int consortiaid)
{
	m_Consortia[consortiaid].con_ptr = 0;
	return 0;
}
/**************************************************************************
*˵����
*���������
*����ֵ��
**************************************************************************/
int CConsortiaManage::AddConsortia(int consortiaid,Consortia_Ptr &con_ptr)
{
	//IceUtil::RWRecMutex::WLock wlock(m_Consortia[consortiaid].rwmutex_);
	m_Consortia[consortiaid].con_ptr = con_ptr;
	return 0;
}
/**************************************************************************
*˵������ӹ����Ա
*���������
*����ֵ��
**************************************************************************/
int CConsortiaManage::AddConsortiaUser(int consortiaid,ConsortiaUser_Ptr &user_ptr)
{
	int iRet = 1;
	//��ӹ���ĳ�Ա
	//IceUtil::RWRecMutex::WLock wlock(m_Consortia[consortiaid].rwmutex_);
	if(m_Consortia[consortiaid].con_ptr.get())
	{
		m_Consortia[consortiaid].con_ptr->AddUser(user_ptr);

		//������г�Ա��	
		IceUtil::RWRecMutex::WLock wAlllock(m_AllConUserMapMutex);
		m_AllConUserMap[user_ptr->GetPlayerID()] = user_ptr;
		iRet = 0;
	}

	return iRet;
}

/**************************************************************************
*˵����ɾ�������Ա
*���������
*����ֵ��
**************************************************************************/
int CConsortiaManage::DelConsortiaUser(int consortiaid,int playerid)
{
	//ɾ������
	//IceUtil::RWRecMutex::WLock wlock(m_Consortia[consortiaid].rwmutex_);
	if(m_Consortia[consortiaid].con_ptr.get())
	{
		m_Consortia[consortiaid].con_ptr->DelUser(playerid);
	}
	// wlock.release();

	//ɾ��ȫ��
	IceUtil::RWRecMutex::WLock wAlllock(m_AllConUserMapMutex);
	m_AllConUserMap.erase(playerid);
	return 0;
}
/**************************************************************************
*˵������ӹ���ͬ��
*���������
*����ֵ��
**************************************************************************/
int CConsortiaManage::AddConsortiaUnion(int consortiaid,ConsortiaUnion_Ptr &union_ptr)
{
	//IceUtil::RWRecMutex::WLock wlock(m_Consortia[consortiaid].rwmutex_);
	if(m_Consortia[consortiaid].con_ptr.get())
	{
		m_Consortia[consortiaid].con_ptr->AddUnion(union_ptr);
		return 0;
	}
	//
	return 1;
}
int CConsortiaManage::DelConsortiaUnin(int consortiaid,int unin_conid)
{
	if(m_Consortia[consortiaid].con_ptr.get())
	{
		return m_Consortia[consortiaid].con_ptr->DelUnion(unin_conid);
	}
	return 1;
}
/**************************************************************************
*˵������ӹ�������
*���������
*����ֵ��
**************************************************************************/
int CConsortiaManage::AddConsoritaBank(int consortiaid,ConsortiaBank_Ptr &bank_ptr)
{
	IceUtil::RWRecMutex::WLock wlock(m_Consortia[consortiaid].rwmutex_);
	return 0;
}
int CConsortiaManage::AddConsortiaBank(int consortiaid,int itemid,int itemcount)
{
	//IceUtil::RWRecMutex::WLock wlock(m_Consortia[consortiaid].rwmutex_);
	if(m_Consortia[consortiaid].con_ptr.get())
	{
		m_Consortia[consortiaid].con_ptr->AddBankItem(itemid,itemcount);
		return 0;
	}
	//
	return 1;
}

/**************************************************************************
*˵������ӹ���Լս
*���������
*����ֵ��
**************************************************************************/
int CConsortiaManage::AddConsortiaFight(int consortiaid,ConsortiaFight_Ptr &fight_ptr)
{
	//IceUtil::RWRecMutex::WLock wlock(m_Consortia[consortiaid].rwmutex_;
	if(m_Consortia[consortiaid].con_ptr.get())
	{
		m_Consortia[consortiaid].con_ptr->AddFight(fight_ptr);
		return 0;
	}
	return 1;
}

/**************************************************************************
*˵���������ж�ĳ����Ƿ��Ѽ��빫��
*���������
*����ֵ��
**************************************************************************/
bool CConsortiaManage::HasJoinConsortia(int playerid,int &consortiaid)
{
	bool bExit = false;
	IceUtil::RWRecMutex::RLock rlock(m_AllConUserMapMutex);
	ConsortiaUser_Iter iter = m_AllConUserMap.find(playerid);
	if(iter != m_AllConUserMap.end())
	{
		consortiaid = iter->second->GetConsortiaID();
		bExit = true;
	}
	else
	{
		consortiaid = 0;
	}
	return bExit;
}
/**************************************************************************
*˵�����ӳ�Աȫ���У���ȡ�û���Ȩ��
*���������
*����ֵ��
**************************************************************************/
int	CConsortiaManage::GetUserPurview(int playerid)
{
	int purview = 0;
	IceUtil::RWRecMutex::RLock rlock(m_AllConUserMapMutex);
	ConsortiaUser_Iter iter = m_AllConUserMap.find(playerid);
	if(iter != m_AllConUserMap.end())
	{
		purview = iter->second->GetPurview();
	}
	else
	{
	}
	return purview;
}
//ְλ���
/**************************************************************************
*˵������ȡְλ����
*���������
*����ֵ��
**************************************************************************/
char* CConsortiaManage::GetOfficialName(int id)
{
	ConsortiaOfficial_Iteraotr iter;
	iter = m_ConOfficialMap.find(id);
	if(iter == m_ConOfficialMap.end())
	{
		return 0;
	}
	return iter->second->GetOfficialName();
}
int CConsortiaManage::GetOfficialPurview(int id)
{
	ConsortiaOfficial_Iteraotr iter;
	iter = m_ConOfficialMap.find(id);
	if(iter == m_ConOfficialMap.end())
	{
		return 0;
	}
	return iter->second->GetPurview();
}
int CConsortiaManage::GetOfficialLevel(int id)
{
	ConsortiaOfficial_Iteraotr iter;
	iter = m_ConOfficialMap.find(id);
	if(iter == m_ConOfficialMap.end())
	{
		return -1;
	}
	
	return iter->second->GetLevel();
}
//��ұ�
int CConsortiaManage::AddPlayer(int playerid,const char *name,int country,bool isonline,int agent)
{
	PlayerAcc_Ptr player_ptr(new CPlayerAcc(playerid,name,country,isonline,agent));
	IceUtil::RWRecMutex::WLock wlock(m_PlayerAccMutex);
	m_PlayerAccMap[playerid] = player_ptr;
	m_PlayerAccNameMap[name] = playerid;
	return 0;
}

int CConsortiaManage::GetPlayerInfo(int playerid,char *name,int &country,bool &isonline,int &agent,DB_Proxy *pDBProxy)
{
	PlayerAcc_Iterator iter;
	//IceUtil::RWRecMutex::RLock rlock(m_PlayerAccMutex);
	iter = m_PlayerAccMap.find(playerid);
	if(iter == m_PlayerAccMap.end())
	{
		//��ȡ��Ӫ
		PlayerChatRoomTable roomtb;
		roomtb.setid(playerid);
		if(pDBProxy->db_select(roomtb) != DB_Proxy::DB_SUCCEED)
		{
			return 1;
		}
		country = roomtb.country_;
		//��ȡ����
		PlayerOfficerTable playertb;
		playertb.setmasterid(playerid);
		playertb.setdetailid(playerid);
		if(pDBProxy->db_select(playertb) != DB_Proxy::DB_SUCCEED)
		{
			return 2;
		}
		if(name)
		{
			strcpy(name,playertb.officername_);
		}
		AddPlayer(playerid,playertb.officername_,country,isonline,agent);
		return 0;
	}
	if(name)
	{
		strcpy(name,iter->second->GetName());
	}
	country = iter->second->GetCountry();
	isonline = iter->second->IsOnline();
	agent = iter->second->GetAgent();
	return 0;
}
int CConsortiaManage::GetPlayerID(const char *name)
{
	int PlayerID = -1;
	IceUtil::RWRecMutex::RLock rlock(m_PlayerAccMutex);
	/*
	PlayerAcc_Iterator iter;
	for(iter = m_PlayerAccMap.begin();iter != m_PlayerAccMap.end();iter++)
	{
		if(strncmp(name,iter->second->GetName(),CPlayerAcc::NAME_LEN) ==0)
		{
			PlayerID = iter->second->GetPlayerID();
			break;
		}
	}
	*/
	PlayerAcc_Name_Iterator iter;
	iter = m_PlayerAccNameMap.find(name);
	if(iter != m_PlayerAccNameMap.end())
	{
		PlayerID = iter->second;
	}
	return PlayerID;
}

int CConsortiaManage::SetPlayerOnline(int playerid,bool isonline,int agent)
{
	IceUtil::RWRecMutex::WLock wlock(m_PlayerAccMutex);
	PlayerAcc_Iterator iter;
	iter = m_PlayerAccMap.find(playerid);
	if(iter == m_PlayerAccMap.end())
	{
		return 1;
	}
	iter->second->SetOnline(isonline);
	iter->second->SetAgent(agent);
	return 0;
}

int CConsortiaManage::SetPlayerName(int playerid,const char *name)
{
	IceUtil::RWRecMutex::WLock wlock(m_PlayerAccMutex);
	PlayerAcc_Iterator iter;
	iter = m_PlayerAccMap.find(playerid);
	if(iter == m_PlayerAccMap.end())
	{
		return 1;
	}
	iter->second->SetName(name);
	return 0;
}

void CConsortiaManage::Ranking_Player(DB_Proxy *pDBProxy)
{
	int rootlevel = 30;
	{//��ȡ���ڵ㼶��
		char buf[SIZE_32] = {0};
		if(ReadConfig(CFGFILE,"System","PlayerRankingLevel",buf,SIZE_32))
		{
			rootlevel = atoi(buf);
		}
	}
	int size = m_PlayerAccMap.size();
	if(size <=0)
		return ;

	/*add by xujincai 20090320
	typedef std::set<PlayerRanking_Ptr,CollectionRate_Cmp> CollectionRate_Set;
	typedef std::set<PlayerRanking_Ptr,CollectionRate_Cmp>::iterator CollectionRate_Set_Iterator;
	typedef std::set<PlayerRanking_Ptr,Contribute_Cmp> Contribute_Set;
	typedef std::set<PlayerRanking_Ptr,Contribute_Cmp>::iterator Contribute_Set_Iterator;
	typedef std::set<PlayerRanking_Ptr,Enemy_Cmp> Enemy_Set;
	typedef std::set<PlayerRanking_Ptr,Enemy_Cmp>::iterator Enemy_Set_Iterator;
	*/
	PlayerRanking_Set playerranking_set;
	CollectionRate_Set collectionrate_set;
	Contribute_Set contribute_set;
	Enemy_Set enemy_set;

	PlayerRanking_Ptr *nwPlayerRanking = new PlayerRanking_Ptr[size];
	PlayerRanking_Ptr *nwCollectionrate = new PlayerRanking_Ptr[size];
	PlayerRanking_Ptr *nwContribute = new PlayerRanking_Ptr[size];
	PlayerRanking_Ptr *nwEnemy = new PlayerRanking_Ptr[size];
	PlayerAcc_Iterator iter;
	int index = 0;
	int i = 0;
	//������ڵ�
	CPlayerRanking *pNewPlayerRanking = 0;
	//����б�
	for(iter = m_PlayerAccMap.begin();iter != m_PlayerAccMap.end() && index<size && m_bHasStart;iter++)
	{
		pNewPlayerRanking = new CPlayerRanking();

		pNewPlayerRanking->SetPlayerID(iter->second->GetPlayerID());
		pNewPlayerRanking->SetName(iter->second->GetName());
		pNewPlayerRanking->SetCountry(iter->second->GetCountry());
		//��ȡ�ȼ������Ρ�ɱ�н�������������
		PlayerOfficerTable playertb;
		playertb.setmasterid(pNewPlayerRanking->GetPlayerID());
		playertb.setdetailid(pNewPlayerRanking->GetPlayerID());
		if(pDBProxy->db_select(playertb) == DB_Proxy::DB_SUCCEED)
		{
			//
			pNewPlayerRanking->SetLevel(playertb.level_);
			pNewPlayerRanking->SetMilitaryRank(playertb.militaryrank_);
			pNewPlayerRanking->SetGeneralCount(playertb.generalcount_);
			pNewPlayerRanking->SetCredit(playertb.credit_);
			pNewPlayerRanking->SetOfficerRace((short)playertb.officerrace_);
			//add by xujincai 20090320
			pNewPlayerRanking->SetEnemyCount(playertb.enemycount_);
		}
		//��ȡ�������ʱ��UpGradeDate
		pNewPlayerRanking->SetUpGradeDate(0);
		//��ȡ��Ǯ
		PlayerChatRoomTable moneyinfo;
		moneyinfo.setid(pNewPlayerRanking->GetPlayerID());
		if(pDBProxy->db_select(moneyinfo) == DB_Proxy::DB_SUCCEED)
		{
			//
			pNewPlayerRanking->SetMoney(moneyinfo.money_);
			pNewPlayerRanking->SetCountry(moneyinfo.country_);
			//add by xujincai 20090320
			pNewPlayerRanking->SetContribute(moneyinfo.contribute_);
		}
		/*�ݵ���*/
		PlayerCity cityinfo;
		cityinfo.setmasterid(pNewPlayerRanking->GetPlayerID());
		PlayerCity *pRetSet = 0;
		char *retbuf = 0;
		int count = 0;
		PlayerResource resourcetb;
		int iCollectionRate = 0;
		if(pDBProxy->db_multi_select(cityinfo,retbuf,count) == DB_Proxy::DB_SUCCEED)
		{
			count = count / sizeof(PlayerCity);//����
			pNewPlayerRanking->SetCityCount(count);
			pRetSet = (PlayerCity*)retbuf;
			for(i=0;i<count;i++)
			{//��ȡ���г��е���Դ
				resourcetb.setmasterid(pRetSet[i].cityid_,pRetSet[i].mapid_);
				resourcetb.setdetailid(pNewPlayerRanking->GetPlayerID());
				if(pDBProxy->db_select(resourcetb) == DB_Proxy::DB_SUCCEED)
				{
					pNewPlayerRanking->AddFoodCount(resourcetb.foodcount_);
					pNewPlayerRanking->AddWoodCount(resourcetb.woodcount_);
					pNewPlayerRanking->AddIronCount(resourcetb.ironcount_);
					pNewPlayerRanking->AddSkinCount(resourcetb.skincount_);
				}
				iCollectionRate = GetCityCollectionRate(pNewPlayerRanking->GetPlayerID(),pRetSet[i].mapid_,pRetSet[i].cityid_,pDBProxy);
				pNewPlayerRanking->AddCollectionRate(iCollectionRate);
			}
			if(retbuf)
			{
				delete[] retbuf;
			}
		}
		playerranking_set.insert(pNewPlayerRanking);
		collectionrate_set.insert(pNewPlayerRanking);
		contribute_set.insert(pNewPlayerRanking);
		enemy_set.insert(pNewPlayerRanking);
		index++;
	}
	if (!m_bHasStart)
	{
		return ;
	}
	PlayerRanking_Set_Iterator player_set_iter;
	i = 0;
	for(player_set_iter = playerranking_set.begin();player_set_iter !=playerranking_set.end(); player_set_iter++)
	{
		nwPlayerRanking[i++] = *player_set_iter;
	}
	/*��д����
	for(int i=0;i<size;i++)
	{
	nwPlayerRanking[i]->SetRankingID(i);
	}
	*/
	//дDBG
	PlayerCompare newCompare;
	//������ձ�
	PlayerCompare *pRetSet_Cmp = 0;
	char *buf = 0;
	int count_cmp;
	if(pDBProxy->db_select_all(newCompare,buf,count_cmp) == DB_Proxy::DB_SUCCEED)
	{
		count_cmp = count_cmp/sizeof(PlayerCompare);
		pRetSet_Cmp = (PlayerCompare*)buf;
		for(i=0;i<count_cmp;i++)
		{
			pDBProxy->db_delete(pRetSet_Cmp[i]);
		}
	}
	if(buf)
	{
		delete[] buf;
		buf =0;
	}
	for(i= 0;i<m_InsertRankingCount && i<size;i++)
	{
		newCompare.setid(i);
		newCompare.playerid_ = nwPlayerRanking[i]->GetPlayerID();
		strcpy(newCompare.playername_,nwPlayerRanking[i]->GetName());
		newCompare.country_ = nwPlayerRanking[i]->GetCountry();
		newCompare.officerrace_ = nwPlayerRanking[i]->GetOfficerRace();
		newCompare.level_ = nwPlayerRanking[i]->GetLevel();
		newCompare.militaryrank_ = nwPlayerRanking[i]->GetMilitaryRank();
		newCompare.money_ = nwPlayerRanking[i]->GetMoney();
		newCompare.resourcecount_ = nwPlayerRanking[i]->GetTotalResource();
		newCompare.citycount_ = nwPlayerRanking[i]->GetCityCount();
		newCompare.generalcount_ = nwPlayerRanking[i]->GetGeneralCount();
		newCompare.credit_ = nwPlayerRanking[i]->GetCredit();
		pDBProxy->db_insert(newCompare);
	}
	/*
	
		collectionrate_set.insert(pNewPlayerRanking);
		contribute_set.insert(pNewPlayerRanking);
		enemy_set.insert(pNewPlayerRanking);
	*/
	i = 0;
	CollectionRate_Set_Iterator collectionrate_iter;
	for(collectionrate_iter = collectionrate_set.begin();collectionrate_iter !=collectionrate_set.end(); collectionrate_iter++)
	{
		nwCollectionrate[i++] = *collectionrate_iter;
	}
	i = 0;
	Contribute_Set_Iterator contribute_iter;
	for(contribute_iter = contribute_set.begin();contribute_iter !=contribute_set.end(); contribute_iter++)
	{
		nwContribute[i++] = *contribute_iter;
	}
	i = 0;
	Enemy_Set_Iterator enemy_iter;
	for(enemy_iter = enemy_set.begin();enemy_iter !=enemy_set.end(); enemy_iter++)
	{
		nwEnemy[i++] = *enemy_iter;
	}
	playerranking_set.clear();
	contribute_set.clear();
	collectionrate_set.clear();
	enemy_set.clear();
	/*
#ifdef PRINT_HINTINFO
	for(int j=0;j<10 && j<size;j++)
	{
		printf("%2d(id:%d,name:%-15s,country:%d,Level:%2d,rate:%5d,contribut:%10d,enemy:%10d)\n",j,
			nwEnemy[j]->GetPlayerID(),nwEnemy[j]->GetName(),
			nwEnemy[j]->GetCountry(),nwEnemy[j]->GetLevel(),
			nwEnemy[j]->GetCollectionRate(),nwEnemy[j]->GetContribute(),
			nwEnemy[j]->GetEnemyCount());
	}
#endif
	*/
	//д�뱾���ļ�(��ʱ������)
	//ʹ��������
	/*
	PlayerRanking_Ptr		*m_CollectionRateRanking;
	PlayerRanking_Ptr		*m_ContributeRanking;
	PlayerRanking_Ptr		*m_EnemyRanking;

	PlayerRanking_Ptr *nwCollectionrate = new PlayerRanking_Ptr[size];
	PlayerRanking_Ptr *nwContribute = new PlayerRanking_Ptr[size];
	PlayerRanking_Ptr *nwEnemy = new PlayerRanking_Ptr[size];
	*/
	IceUtil::RWRecMutex::WLock wlock(m_PlayerAccMutex);
	PlayerRanking_Ptr *oldRanking = m_PlayerRanking;
	PlayerRanking_Ptr *poldCollectionRate = m_CollectionRateRanking;
	PlayerRanking_Ptr *poldContributeRanking = m_ContributeRanking;
	PlayerRanking_Ptr *poldEnemyRanking = m_EnemyRanking;
	m_PlayerRanking = nwPlayerRanking;
	m_CollectionRateRanking = nwCollectionrate;
	m_ContributeRanking = nwContribute;
	m_EnemyRanking = nwEnemy;
	m_PlayerRankingSize = size;
	wlock.release();

	if(oldRanking)
	{
		delete[] oldRanking;
		oldRanking = 0;
	}
	if(poldCollectionRate)
	{
		delete[] poldCollectionRate;
		poldCollectionRate = 0;
	}
	if(poldContributeRanking)
	{
		delete[] poldContributeRanking;
		poldContributeRanking = 0;
	}
	if(poldEnemyRanking)
	{
		delete[] poldEnemyRanking;
		poldEnemyRanking = 0;
	}
}

void CConsortiaManage::Ranking_Consortia(DB_Proxy *pDBProxy)
{
	int rootcount = 30;
	{//��ȡ���ڵ㼶��
		char buf[SIZE_32] = {0};
		if(ReadConfig(CFGFILE,"System","ConRankingCount",buf,SIZE_32))
		{
			rootcount = atoi(buf);
		}
	}
	IceUtil::RWRecMutex::RLock rlock(m_UsedMutex);
	int size = m_UsedConID_Map.size();
	if(size <=0)
	{
		ResetConsortiaRanking(0,0);
		ClearConsortiaRanking(pDBProxy);
		return;
	}

	ConsortiaRanking_Ptr *nwConRankking = new ConsortiaRanking_Ptr[size];
	ConsortiaRanking_Set consortia_set;

	CConsortiaRanking *pConRanking = 0;
	std::map<int,int>::iterator iter;
	int index = 0;
	int id = 0;
	int chairman_id;
	char chairman_name[33] = {0};
	int country;
	bool isonline = false;
	int agent = 0;
	int i = 0;
	for(iter = m_UsedConID_Map.begin();iter != m_UsedConID_Map.end();iter++)
	{
		id = iter->second;
		Consortia_Ptr con_ptr;
		ReadLock_Consortia(id);
		con_ptr = m_Consortia[id].con_ptr;
		Unlock_Consortia(id);
		if(con_ptr.get())
		{
			pConRanking = new CConsortiaRanking();
			pConRanking->SetConName(con_ptr->GetConsortiaID(),con_ptr->GetName());
			pConRanking->SetLevel(con_ptr->GetLevel());
			pConRanking->SetUserCount(con_ptr->GetUserCount());

			chairman_id = con_ptr->GetChairmanID();

			if(GetPlayerInfo(chairman_id,chairman_name,country,isonline,agent,pDBProxy))
			{
				GetPlayerAcc(chairman_id,pDBProxy,chairman_name,country,isonline,agent);
			}
			pConRanking->SetChairman(chairman_id,chairman_name);
			pConRanking->SetCountry(country);
			pConRanking->SetCreateTime(con_ptr->GetCreateTime());

			chairman_name[0] = 0;
			consortia_set.insert(pConRanking);
		}
		else
		{
		}
	}
	ConsortiaRanking_Set_Iterator cons_iter;
	i = 0;
	for (cons_iter = consortia_set.begin();cons_iter != consortia_set.end();cons_iter++)
	{
		nwConRankking[i++] = *cons_iter;
	}
	/*��д����
	for(int i=0;i<size;i++)
	{
		nwConRankking[i]->SetRankingID(i);
	}
	*/
	//дDBG
	ConsortiaCompare newCompare;
	/*������ձ�
	ConsortiaCompare *pRetSet = 0;
	char *buf = 0;
	int count = 0;
	if(pDBProxy->db_select_all(newCompare,buf,count) == DB_Proxy::DB_SUCCEED)
	{
		count = count/sizeof(ConsortiaCompare);
		pRetSet = (ConsortiaCompare*)buf;
		for(i = 0;i<count;i++)
		{
			pDBProxy->db_delete(pRetSet[i]);
		}
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}*/
	ClearConsortiaRanking(pDBProxy);
	for(i= 0;i<m_InsertRankingCount && i <size;i++)
	{
		newCompare.setid(i);
		newCompare.conid_ = nwConRankking[i]->GetConsortiaID();
		strcpy(newCompare.conname_,nwConRankking[i]->GetConName());
		newCompare.level_ = nwConRankking[i]->GetLevel();
		newCompare.usercount_ = nwConRankking[i]->GetUserCount();
		newCompare.playerid_ = nwConRankking[i]->GetChairmanID();
		strcpy(newCompare.chairmanname_, nwConRankking[i]->GetChairman());
		newCompare.country_ = nwConRankking[i]->GetCountry();
		newCompare.createtime_ = nwConRankking[i]->GetCreateTime();
		pDBProxy->db_insert(newCompare);
	}
	/*
#ifdef PRINT_HINTINFO
	for(int j=0;j<size && j<10;j++)
	{
		printf("%2d(id:%-2d,name:%-10s,PlayerID:%-6d,chairman:%-10s,count:%2d,Level:%2d,country:%2d)\n",j,
			nwConRankking[j]->GetConsortiaID(),nwConRankking[j]->GetConName(),
			nwConRankking[j]->GetChairmanID(),nwConRankking[j]->GetChairman(),
			nwConRankking[j]->GetUserCount(),nwConRankking[j]->GetLevel(),
			nwConRankking[j]->GetCountry());
	}
#endif
	*/
	//д�뱾���ļ�(��ʱ������)
	//ʹ��������
	/*
	IceUtil::RWRecMutex::WLock wlock(m_ConsortiaRankingMutex);
	ConsortiaRanking_Ptr *oldRanking = m_ConosrtiaRanking;
	m_ConosrtiaRanking = nwConRankking;
	m_ConsortiaRankingSize = size;
	wlock.release();
	if(oldRanking)
	{
		delete[] oldRanking;
	}
	*/
	ResetConsortiaRanking(nwConRankking,size);
}
void CConsortiaManage::ClearConsortiaRanking(DB_Proxy *pDBProxy)
{
	ConsortiaCompare newCompare;
	//������ձ�
	ConsortiaCompare *pRetSet = 0;
	char *buf = 0;
	int count = 0;
	if(pDBProxy->db_select_all(newCompare,buf,count) == DB_Proxy::DB_SUCCEED)
	{
		count = count/sizeof(ConsortiaCompare);
		pRetSet = (ConsortiaCompare*)buf;
		for(int i = 0;i<count;i++)
		{
			pDBProxy->db_delete(pRetSet[i]);
		}
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
}
void CConsortiaManage::ResetConsortiaRanking(ConsortiaRanking_Ptr *pNewRanking,int size)
{
	IceUtil::RWRecMutex::WLock wlock(m_ConsortiaRankingMutex);
	ConsortiaRanking_Ptr *oldRanking = m_ConosrtiaRanking;
	m_ConosrtiaRanking = pNewRanking;
	m_ConsortiaRankingSize = size;
	wlock.release();
	if(oldRanking)
	{
		delete[] oldRanking;
	}
}

int CConsortiaManage::GetCityCollectionRate(int playerid,int mapid,int cityid,DB_Proxy *pDBProxy)
{
	int rate = 0;
	int iPubliRate[4] = {100,100,100,100};//��ʳ������Ƥ��ľ��
	GameCityInfo_Map_Iterator cityinfo_iter = m_GameCityInfoMap.find(cityid);
	if(cityinfo_iter == m_GameCityInfoMap.end())
	{
		LOG_TRACE_EX(false,("CConsortiaManage::GetCityCollectionRate-->��ȡ����%d��Ϣʧ��",cityid));
		return 0;
	}
	GameCityInfo *pGameCitInfo = cityinfo_iter->second;
	if(pGameCitInfo == NULL)
	{
		LOG_TRACE_EX(false,("CConsortiaManage::GetCityCollectionRate-->��ȡ����%d��Ϣʧ��:Ϊ��",cityid));
		return 0;
	}
	
	PublicBuildTable publicbuild;
	PublicBuildParamTable_Iterator iter;
	PublicBuildParamTable *pPubliParam = 0;
	//��ѯ���������ļ���
	publicbuild.setmasterid(mapid);
	int i =0;
	char id_buf[10] = {0};
	/*
	ARCH_LUMBERYARD=6,     //ľ��
	ARCH_STOPE     =7,     //�ɿ�
	ARCH_GRINDERY  =8,     //ĥ��
	ARCH_SKIN      =9,     //Ƥ�ϼӹ���
	*/
	for(i=ARCH_LUMBERYARD;i<=ARCH_SKIN;i++)
	{
		sprintf(id_buf,"%d%.2d01",mapid,i);
		publicbuild.setdetailid(atoi(id_buf));
		if(pDBProxy->db_select(publicbuild) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE_EX(false,("CConsortiaManage::GetCityCollectionRate-->��ȡ����%d�Ĺ�������%sʧ��",cityid,id_buf));
			continue;
		}
		iter = m_PublicBuildParam.find(i*100 + publicbuild.currentlevel_);
		if(iter == m_PublicBuildParam.end())
		{
			LOG_TRACE_EX(false,("CConsortiaManage::GetCityCollectionRate-->��ȡ����%d��Ϣ��������%s����ʧ��",cityid,id_buf));
			continue;
		}
		iPubliRate[i-ARCH_LUMBERYARD] = iter->second->output_;
	}
	PlayerConstruction playercons;
	PlayerConstruction *pPlayerCons = 0;
	char *buf = 0;
	int count = 0;
	playercons.setmasterid(mapid,cityid,playerid);
	if(pDBProxy->db_multi_select(playercons,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		return 0;
	}
	count =count /sizeof(PlayerConstruction);
	pPlayerCons = (PlayerConstruction*)buf;
	//����
	int iCurRate = 0;
	int iTmpRate[2] = {100,10000};
	int iFlag = 0;
	int index  = 0;
	for(i=0;i<count;i++)
	{
		iFlag = 0;
		iCurRate = 0;
		if(pPlayerCons[i].constructionid_ == 1)
		{//��ʳ
			iCurRate = pPlayerCons[i].output_ * pGameCitInfo->foodcount_;
			index = ARCH_GRINDERY - ARCH_LUMBERYARD;
			if(iPubliRate[index] >100)
			{
				iCurRate = iCurRate * iPubliRate[index];
				iFlag++;
			}
		}
		else if(pPlayerCons[i].constructionid_ == 2)
		{//Wood
			index = ARCH_LUMBERYARD - ARCH_LUMBERYARD;
			iCurRate = pPlayerCons[i].output_ * pGameCitInfo->woodcount_;
			if(iPubliRate[index] >100)
			{
				iCurRate = iCurRate * iPubliRate[index];
				iFlag++;
			}
		}
		else if(pPlayerCons[i].constructionid_ == 3)
		{//Ƥ
			index = ARCH_SKIN - ARCH_LUMBERYARD;
			iCurRate = pPlayerCons[i].output_ * pGameCitInfo->woodcount_;
			if(iPubliRate[index] >100)
			{
				iCurRate = iCurRate * iPubliRate[index];
				iFlag++;
			}
		}
		else if(pPlayerCons[i].constructionid_ == 4)
		{//��
			index = ARCH_STOPE - ARCH_LUMBERYARD;
			iCurRate = pPlayerCons[i].output_ * pGameCitInfo->woodcount_ * 5;
			if(iPubliRate[index] >100)
			{
				iCurRate = iCurRate * iPubliRate[index];
				iFlag++;
			}
		}
		else if(pPlayerCons[i].constructionid_ == 6)
		{
			iFlag = -1;
			iCurRate = pPlayerCons[i].output_;
		}
		if(iFlag >=0 && iCurRate >0)
		{
			iCurRate = iCurRate/iTmpRate[iFlag];
		}
		rate += iCurRate;

	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	return rate;
}
__int64 CConsortiaManage::GetNextTime()
{
	int dayindex[7] ={7,1,2,3,4,5,6};
	IceUtil::DateTime curdate;
	IceUtil::Time::now().toDateTime(curdate);
	__int64 curtime = IceUtil::Time::now().toSeconds();
	int idays = (dayindex[m_VoteParam.week] - curdate.week);
	int ihour = (m_VoteParam.hour - curdate.hour);
	int imin = m_VoteParam.min - curdate.min;
	if(idays <0)
	{
		idays = 7 + idays;
	}
	else if(idays ==0)
	{
		if(ihour <0)
			idays = 7;
		else if(ihour ==0 && imin <0)
			idays = 7;
	}
	curtime += idays * 86400 + ihour * 3600 + imin * 60 - curdate.sec;
	return curtime;
}
int CConsortiaManage::GetNextTime(int week,int hour,int min)
{
	int dayindex[7] ={7,1,2,3,4,5,6};
	IceUtil::DateTime curdate;
	IceUtil::Time::now().toDateTime(curdate);
	//__int64 curtime = IceUtil::Time::now().toSeconds();
	int idays = (dayindex[week] - curdate.week);
	int ihour = (hour - curdate.hour);
	int imin = min - curdate.min;
	if(idays <0)
	{
		idays = 7 + idays;
	}
	else if(idays ==0)
	{
		if(ihour <0)
			idays = 7;
		else if(ihour ==0 && imin <0)
			idays = 7;
	}
	return (idays * 86400 + ihour * 3600 + imin * 60 - curdate.sec);
	//return curtime;
}
LPTirrierParam CConsortiaManage::GetTirrierParam(int tirriertype)
{
	TirrierParam_Map_Iterator iter;
	iter = m_TirrierParamList.find(tirriertype);
	if(iter == m_TirrierParamList.end())
		return NULL;

	return iter->second;
}
//��������ID��ȡ����������
int CConsortiaManage::GetTirrierType(int weaponid)
{
	TirrierParam_Map_Iterator iter;
	int iType = -1;
	for(iter = m_TirrierParamList.begin();iter!= m_TirrierParamList.end();iter++)
	{
		if (iter->second->WeaponType == weaponid)
		{
			iType = iter->second->TirrierType;
			break;
		}
	}
	if (iType <0)
	{
		if(weaponid <=25)
		{
			iType = 0;
		}
		else if(weaponid <=40)
		{
			iType = 1;
		}
		else if(weaponid <=50)
		{
			iType = 2;
		}
	}
	return iType;
}
int CConsortiaManage::SetInsertRankingCount(int count)
{
	if(count <=0)
		m_InsertRankingCount = 100000000;
	else
		m_InsertRankingCount = count;
	return m_InsertRankingCount;
}
int CConsortiaManage::SetRankingTimes(int times)
{
	if (times >0)
	{
		m_RankingTimes = times;
	}
	return m_RankingTimes;
}
int CConsortiaManage::GetInsertRankingCount()
{
	return m_InsertRankingCount;
}
int CConsortiaManage::Ranking(DB_Proxy *pDBProxy)
{
#ifdef PRINT_HINTINFO
	printf("========================��ȡ����=================================\n");
	int start = GetTickCount();
	int end = start;
#endif
	Ranking_Player(pDBProxy);//Ӧ�Լ�д�����㷨�����Ż�
#ifdef PRINT_HINTINFO
	end = GetTickCount();
	printf("Ranking_Player :%d\n",end-start);
	start = GetTickCount();
#endif
	Ranking_Consortia(pDBProxy);//Ӧ�Լ�д�����㷨�����Ż�
#ifdef PRINT_HINTINFO
	end = GetTickCount();
	printf("Ranking_Consortia :%d\n",end-start);
#endif
	LPConsortia_Timer pRankingMsg = new Consortia_Timer;
	pRankingMsg->serialize(CC_C_TIMER_RANKING,0);
	set_timer(m_RankingTimes,pRankingMsg,0);
	return 0;
}

int CConsortiaManage::GetConsortiaRanking(LPConsortia_RankingInfo conranking_list,int &count,int start,int &pages)
{
	IceUtil::RWRecMutex::RLock rlock(m_ConsortiaRankingMutex);

	pages = (m_ConsortiaRankingSize -1 + count) /count;
	int index = 0;
	bool isonline = false;
	__int64 baseTime = IceUtil::Time::Date(2000, 1, 1).toSeconds();
	for(int i = start;i<m_ConsortiaRankingSize;i++)
	{
		conranking_list[index].RankingID = i;
		strncpy(conranking_list[index].ConName,m_ConosrtiaRanking[i]->GetConName(),SIZE_32);
		strncpy(conranking_list[index].Chairman,m_ConosrtiaRanking[i]->GetChairman(),SIZE_24);
		conranking_list[index].Level = m_ConosrtiaRanking[i]->GetLevel();
		conranking_list[index].Count = m_ConosrtiaRanking[i]->GetUserCount();
		conranking_list[index].Country = m_ConosrtiaRanking[i]->GetCountry();
		conranking_list[index].CreateTime = m_ConosrtiaRanking[i]->GetCreateTime() - baseTime;

		if((++index) >= count && count >0)
		{
			break;
		}
	}
	count = index;
	return 0;
}

int CConsortiaManage::GetPlayerRanking(LPPlayer_RankingInfo playerranking_list,int &count,int start,int &pages,DB_Proxy *pDBProxy)
{
	IceUtil::RWRecMutex::RLock rlock(m_PlayerRankingMutex);

	pages = (m_PlayerRankingSize -1 + count) /count;
	int index = 0;
	int country;
	bool isonline = false;
	int agent = 0;
	for(int i = start;i<m_PlayerRankingSize;i++)
	{
		playerranking_list[index].RankingID = i;
		strncpy(playerranking_list[index].Name,m_PlayerRanking[i]->GetName(),SIZE_24);
		playerranking_list[index].Level = m_PlayerRanking[i]->GetLevel();
		playerranking_list[index].MilitaryRank = m_PlayerRanking[i]->GetMilitaryRank();
		playerranking_list[index].Money = m_PlayerRanking[i]->GetMoney();
		playerranking_list[index].ResourceCount = m_PlayerRanking[i]->GetTotalResource();
		playerranking_list[index].CityCount = m_PlayerRanking[i]->GetCityCount();
		playerranking_list[index].GeneralCount = m_PlayerRanking[i]->GetGeneralCount();
		playerranking_list[index].Credit = m_PlayerRanking[i]->GetCredit();
		playerranking_list[index].Country = m_PlayerRanking[i]->GetCountry();
		playerranking_list[index].OfficerRace = m_PlayerRanking[i]->GetOfficerRace();
		GetPlayerInfo(m_PlayerRanking[i]->GetPlayerID(),0,country,isonline,agent,pDBProxy);
		playerranking_list[index].IsOnline = isonline;
/*
#ifdef PRINT_HINTINFO
		printf("%d(%s,%d,%d,%d,%d,%d,%d,%d,%d)\n",playerranking_list[index].RankingID,playerranking_list[index].Name,
			playerranking_list[index].Level,playerranking_list[index].MilitaryRank,playerranking_list[index].Money,
			playerranking_list[index].ResourceCount,playerranking_list[index].CityCount,playerranking_list[index].GeneralCount,
			playerranking_list[index].Credit ,playerranking_list[index].IsOnline);
#endif
			*/
		if((++index) >= count && count >0)
		{
			break;
		}
	}
	count = index;
	return 0;
}
int CConsortiaManage::GetMoneyRanking(LPPlayer_RankingInfo playerranking_list,int &count,int start,int &pages,DB_Proxy *pDBProxy)
{

}
CCountry * CConsortiaManage::GetCountry(int index)
{
	//assert(index >0 && index <3);
	if(index <=0 || index >=3)
		return NULL;
	return &m_CountryList[index];
}

#ifdef PRESS_TEST
size_t CConsortiaManage::TaskSize()
{
	return m_ConsortiaTask.size();
}
#endif


int CConsortiaManage::SetMilitaryCount(int count)
{
	return (m_MilitaryNum = count);
}
int CConsortiaManage::SetEconomyCount(int count)
{
	return (m_EconomyNum = count);
}
int CConsortiaManage::SetContributeCount(int count)
{
	return (m_ContributeNum = count);
}
int CConsortiaManage::GetMilitaryCount(int count)
{
	return m_MilitaryNum;
}
int CConsortiaManage::GetEconomyCount(int count)
{
	return m_EconomyNum;
}
int CConsortiaManage::GetContributeCount(int count)
{
	return m_ContributeNum;
}
void CConsortiaManage::SetVoteParam(int week,int hour,int min,int holdtimes,int votetimes,int warningtimes)
{
	//assert(holdtimes > warningtimes);
	m_VoteParam.week = week;
	m_VoteParam.hour = hour;
	m_VoteParam.min = min;
	m_VoteParam.holdtimes = holdtimes;
	m_VoteParam.votetimes = votetimes;
	m_VoteParam.holdseconds = holdtimes * 60;
	m_VoteParam.warningtimes = warningtimes * 60;
	m_CountryList[1].SetVoteTimes(votetimes * 60);
	m_CountryList[2].SetVoteTimes(votetimes * 60);
	m_CountryList[1].SetHoldTimes(m_VoteParam.holdseconds);
	m_CountryList[2].SetHoldTimes(m_VoteParam.holdseconds);
}
void CConsortiaManage::SetDelateParam(int underwritetimes,int delatetimes,int underwritenum)
{
	int times;
	times = underwritetimes * 60;
	m_CountryList[1].SetUnderWriteTimes(times);
	m_CountryList[2].SetUnderWriteTimes(times);
	times = delatetimes * 60;
	m_CountryList[1].SetDelateTiems(times);
	m_CountryList[2].SetDelateTiems(times);
	m_CountryList[1].SetUnderWriteNum(underwritenum);
	m_CountryList[2].SetUnderWriteNum(underwritenum);
}
void CConsortiaManage::SetVoteFee(int money,int gomoney)
{
	m_CountryList[1].SetVoteFee(money,gomoney);
	m_CountryList[2].SetVoteFee(money,gomoney);
}
int CConsortiaManage::VoteCouncilTimer(const char *msg,DB_Proxy *pDBProxy)
{

	IceUtil::RWRecMutex::RLock rlock(m_PlayerRankingMutex);
	m_CountryList[1].WriteLock();
	m_CountryList[2].WriteLock();
	//�������ԭ����Ա
	m_CountryList[1].DeleteAllCouncils(pDBProxy);
	m_CountryList[2].DeleteAllCouncils(pDBProxy);
	int count1 = 0;
	int count2 = 0;
	int country = 0;
	int i = 0;
	//ս��m_EnemyRanking
 	for(i = 0;i<m_PlayerRankingSize;i++)
	{
		country = m_EnemyRanking[i]->GetCountry();
		if(country == 1 && count1<m_MilitaryNum)
		{
			count1++;
			m_CountryList[1].AddCouncil(m_EnemyRanking[i].get(),MILITARY_POSTION,pDBProxy);
		}
		else if(country == 2 && count2<m_MilitaryNum)
		{
			count2++;
			m_CountryList[2].AddCouncil(m_EnemyRanking[i].get(),MILITARY_POSTION,pDBProxy);
		}
		if(count1 >=m_MilitaryNum && count2 >=m_MilitaryNum)
			break;
	}
	//����m_CollectionRateRanking
	count1 = count2 = 0;
	for(i = 0;i<m_PlayerRankingSize;i++)
	{
		country = m_CollectionRateRanking[i]->GetCountry();
		if(country == 1 && count1<m_EconomyNum)
		{
			if(m_CountryList[1].GetCouncil(m_CollectionRateRanking[i]->GetPlayerID()))
				continue;

			count1++;
			m_CountryList[1].AddCouncil(m_CollectionRateRanking[i].get(),ECONOMY_POSTION,pDBProxy);
		}
		else if(country == 2 && count2<m_EconomyNum)
		{
			if(m_CountryList[2].GetCouncil(m_CollectionRateRanking[i]->GetPlayerID()))
				continue;

			count2++;
			m_CountryList[2].AddCouncil(m_CollectionRateRanking[i].get(),ECONOMY_POSTION,pDBProxy);
		}
		if(count1 >=m_EconomyNum && count2 >=m_EconomyNum)
			break;
	}
	//����m_ContributeRanking
	count1 = count2 = 0;
	for(i = 0;i<m_PlayerRankingSize;i++)
	{
		country = m_ContributeRanking[i]->GetCountry();
		if(country == 1 && count1<m_ContributeNum)
		{
			if(m_CountryList[1].GetCouncil(m_ContributeRanking[i]->GetPlayerID()))
				continue;
			count1++;
			m_CountryList[1].AddCouncil(m_ContributeRanking[i].get(),CONTRIBUTE_POSTION,pDBProxy);
		}
		else if(country == 2 && count2<m_ContributeNum)
		{
			if(m_CountryList[2].GetCouncil(m_ContributeRanking[i]->GetPlayerID()))
				continue;
			count2++;
			m_CountryList[2].AddCouncil(m_ContributeRanking[i].get(),CONTRIBUTE_POSTION,pDBProxy);
		}
		if(count1 >=m_ContributeNum && count2 >=m_ContributeNum)
			break;
	}
	m_CountryList[1].ResetNextCouncilTime(pDBProxy);
	m_CountryList[2].ResetNextCouncilTime(pDBProxy);
	//ȫ�������㲥��Ա����
	m_CountryList[1].BroadCastCouncilList(pDBProxy,0,CC_S_BROAD_VOTESTART);
	m_CountryList[2].BroadCastCouncilList(pDBProxy,0,CC_S_BROAD_VOTESTART);

	m_CountryList[2].Unlock();
	m_CountryList[1].Unlock();
	//������һ����Աѡ��ʱ��
	LPConsortia_Timer pTimerParam = new Consortia_Timer;
	pTimerParam->serialize(CC_C_TIMER_VOTECOUNCIL,0);
	set_timer(m_VoteParam.holdseconds,pTimerParam);
#ifdef PRINT_HINTINFO
	printf("times:%d,seconds:%d\n",m_VoteParam.holdtimes,m_VoteParam.holdseconds);
#endif
	//add by xujincai 20090625
	int times = m_VoteParam.holdseconds - m_VoteParam.warningtimes;
	pTimerParam = new Consortia_Timer;
	pTimerParam->serialize(CC_C_TIMER_BROADBEVOTE,0);
	set_timer((long)times,pTimerParam);
	//ȫ���㲥ѡ�ٿ�ʼ��Ϣ

	//��������ѡ��
	m_CountryList[1].StartVote(pDBProxy);
	m_CountryList[2].StartVote(pDBProxy);
	return 0;
}
int CConsortiaManage::BroadBevote(const char *msg,DB_Proxy *pDBProxy)
{
	Notify_BroadMail broademail;
	char szStartTime[16] = {0};
	char szEndTime[16] = {0};
	int iYear =0,iMon = 0 ,iDay =0 ,iHour =0 ,iMin =0,iSec = 0;
	__int64 iBaseTime = IceUtil::Time::Date(2000, 1, 1).toSeconds();
	__int64 iCurTime = IceUtil::Time::now().toSeconds() + m_VoteParam.warningtimes;
	game_calendar::calendar cal_start(iCurTime  -iBaseTime);
	cal_start.get_game_calendar(iYear,iMon,iDay,iHour,iMin,iSec);
	sprintf(szStartTime,"%d��%d��%d��",iYear,iMon,iDay);
	game_calendar::calendar cal_end(iCurTime + m_VoteParam.votetimes * 60 -iBaseTime);
	iYear = iMon = iDay = iHour = iMin = iSec = 0;
	cal_end.get_game_calendar(iYear,iMon,iDay,iHour,iMin,iSec);
	sprintf(szEndTime,"%d��%d��%d��",iYear,iMon,iDay);
	char szContentbuf[SIZE_256] = {0};
	LPMailInfoParam_Item pMailInfoParam = 0;
	pMailInfoParam = GetMailInfoParam(1,MAIL_COUNTRY_DOD_VOTESTART);
	if(pMailInfoParam)
	{
		StrReplace(pMailInfoParam->Content,szContentbuf,"$startime",szStartTime);
		StrReplace(szContentbuf,broademail.szbody,"$endtime",szEndTime);

		broademail.CountryID = 1;
		strcpy(broademail.szSendPlayerName,"����ϵͳ");
		strcpy(broademail.sztitle,pMailInfoParam->Title);
		broademail.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&broademail,broademail.length);
	}
	pMailInfoParam = GetMailInfoParam(2,MAIL_COUNTRY_DOD_VOTESTART);
	if(pMailInfoParam)
	{
		StrReplace(pMailInfoParam->Content,szContentbuf,"$startime",szStartTime);
		StrReplace(szContentbuf,broademail.szbody,"$endtime",szEndTime);

		broademail.CountryID = 2;
		strcpy(broademail.szSendPlayerName,"����ϵͳ");
		strcpy(broademail.sztitle,pMailInfoParam->Title);
		broademail.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&broademail,broademail.length);
	}
	return 0;
}
LPMailInfoParam_Item CConsortiaManage::GetMailInfoParam(int countryid,int type)
{
	int key = type * 10 + countryid;
	MailInfoParam_Map_Iterator iter;
	iter = m_MailInfoParamList.find(key);
	if(iter == m_MailInfoParamList.end())
		return NULL;
	return iter->second;
}
//��ȡ�ʼ�������
void CConsortiaManage::loadMailInfoParam()
{
	FILE *fp = 0;
	fp = fopen("configtable/MailInfoParam.csv","r");
	if(fp == NULL)
	{
		printf("��configtable/MailInfoParam.csvʧ�ܡ�\n");
		return;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);
	char content[256] = {0};

	LPMailInfoParam_Item pItem = 0;
	int iLen = 0;
	//m_QuantityByRankList
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new MailInfoParam_Item;
		if(sscanf(buf,"%d,%d,%[^,],%[^,]",&pItem->CounryID,&pItem->Type,&pItem->Title,&content)<4)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		iLen = strlen(content) -1;
		if(content[iLen] == '\n')
			content[iLen] = 0;
		StrReplace(content,pItem->Content,"\\n","\r\n");
		m_MailInfoParamList[pItem->Type * 10 + pItem->CounryID] = pItem;
		//printf("countryid:%d,type:%d,title:%s,content:%s\n",pItem->CounryID,pItem->Type,pItem->Title,pItem->Content);
	}
	fclose(fp);
}
void CConsortiaManage::clearMailInfoParam()
{
	MailInfoParam_Map_Iterator iter;
	LPMailInfoParam_Item pDelItem = 0;
	for(iter = m_MailInfoParamList.begin();iter != m_MailInfoParamList.end();iter++)
	{
		if(pDelItem = iter->second)
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_MailInfoParamList.clear();
}

LPCareerConfig_Item CConsortiaManage::GetCareerConfig(int deptid,int jobid)
{
	int key = deptid  + jobid * 100;
	CareerConfig_Iterator iter = m_CareerConfigList.find(key);
	if(iter == m_CareerConfigList.end())
		return NULL;
	return iter->second;
}
//��ȡְλ����
void CConsortiaManage::loadCareerConfig()
{
	
	FILE *fp = 0;
	fp = fopen("configtable/CareerConfig.csv","r");
	if(fp == NULL)
	{
		printf("��configtable/CareerConfig.csvʧ�ܡ�\n");
		return;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	LPCareerConfig_Item pItem = 0;
	int iLen = 0;
	//m_CareerConfigList
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new CareerConfig_Item;
		if(sscanf(buf,"%d,%d,%[^,]",&pItem->DeptID,&pItem->JobID,&pItem->JobName)<3)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		iLen = strlen(pItem->JobName) - 1;
		if(pItem->JobName[iLen] == '\n')
			pItem->JobName[iLen] = 0;
		m_CareerConfigList[pItem->DeptID  + pItem->JobID * 100] = pItem;
		//printf("DeptID:%d,JobID:%d,JobName:%s\n",pItem->DeptID,pItem->JobID,pItem->JobName);
	}
	fclose(fp);
}
void CConsortiaManage::clearCareerConfig()
{
	CareerConfig_Iterator iter;
	LPCareerConfig_Item pDelItem = 0;
	for(iter = m_CareerConfigList.begin();iter != m_CareerConfigList.end();iter++)
	{
		if((pDelItem = iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_CareerConfigList.clear();
}
int CConsortiaManage::getTirrierCountAllMap()
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
	if(m_DBProxy.db_select_all(maptb,buf,count) == DB_Proxy::DB_SUCCEED)
	{
		count = count/sizeof(MapInfoTable);
		pMapList = (MapInfoTable*)buf;
		for(i=0;i<count;i++)
		{
			if(pMapList[i].state_ == 100)//���ִ�
				continue;
			army.setmasterid(pMapList[i].mapid_);
			if(m_DBProxy.db_multi_select(army,armybuf,armycount) == DB_Proxy::DB_SUCCEED)
			{
				armycount = armycount/sizeof(ArmyTable);
				pArmyList = (ArmyTable*)armybuf;
				for(j=0;j<armycount;j++)
				{
					if(pArmyList[j].playerid_ != 100 && pArmyList[j].playerid_ !=200)
						continue;
					m_CountryList[pArmyList[j].countryid_].addTirrierCountFromWeapon(pArmyList[j].fweapon_);
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
	return 0;
}
//��ȡ���������ƺŲ�����
void CConsortiaManage::loadLegionTitleParam()
{
	FILE *fp = 0;
	fp = fopen("configtable/LegionTitleParam.csv","r");
	if(fp == NULL)
	{
		printf("��configtable/LegionTitleParam.csvʧ�ܡ�\n");
		return;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	LegionTitleParam* pItem = 0;
	LegionTitleParam_Map_Map_Iterator mapIter;
	LegionTitleParam_Map *pLegionTitleParamMap = 0;

	//m_LegionTitleParamMap
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new LegionTitleParam;
		if(sscanf(buf,"%d,%d,%[^,],%d,%d,%d",&pItem->legTitleID,&pItem->legTitleType,&pItem->name,&pItem->level,&pItem->condValue,&pItem->wage)<6)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		mapIter = m_LegionTitleParamMap.find(pItem->legTitleType);
		if (mapIter == m_LegionTitleParamMap.end())
		{
			pLegionTitleParamMap = new LegionTitleParam_Map;
			m_LegionTitleParamMap[pItem->legTitleType] = pLegionTitleParamMap;
		}
		else
		{
			pLegionTitleParamMap = mapIter->second;
		}
		pLegionTitleParamMap->insert(LegionTitleParam_Map_Pair(pItem->legTitleID,pItem));
	}
	fclose(fp);
}
void CConsortiaManage::clearLegionTitleParam()
{
	LegionTitleParam* pItem = 0;
	LegionTitleParam_Map_Map_Iterator mapIter;
	LegionTitleParam_Map *pLegionTitleParamMap = 0;
	LegionTitleParam_Map_Iterator itemIter;
	for (mapIter = m_LegionTitleParamMap.begin();mapIter != m_LegionTitleParamMap.end();mapIter++)
	{
		pLegionTitleParamMap = mapIter->second;
		if (pLegionTitleParamMap == NULL)
		{
			continue;
		}
		for (itemIter = pLegionTitleParamMap->begin();itemIter != pLegionTitleParamMap->end();itemIter++)
		{
			if ((pItem = itemIter->second))
			{
				delete pItem;
				pItem = 0;
			}
		}
		pLegionTitleParamMap->clear();
		delete pLegionTitleParamMap;
		pLegionTitleParamMap = 0;
	}
	m_LegionTitleParamMap.clear();
}

//��ȡ��������������
void CConsortiaManage::loadConsortiaUpdateParam()
{
	FILE *fp = 0;
	fp = fopen("configtable/ConsortiaUpdateParam.csv","r");
	if(fp == NULL)
	{
		printf("��configtable/ConsortiaUpdateParam.csvʧ�ܡ�\n");
		return;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	ConsortiaUpdateParam* pItem = 0;
	int iRoyal = 0;
	//m_ConsortiaUpdateParamMap
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new ConsortiaUpdateParam;
		if(sscanf(buf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&pItem->level,&iRoyal,&pItem->needMoney,&pItem->needGODian,
			&pItem->needExp,&pItem->needCredit,&pItem->maxUserNum,&pItem->baseRate.foodRate,
			&pItem->baseRate.woodRate,&pItem->baseRate.ironRate,&pItem->baseRate.skinRate,&pItem->extraUserCount,
			&pItem->extraRate.foodRate,&pItem->extraRate.woodRate,&pItem->extraRate.ironRate,&pItem->extraRate.skinRate)<16)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		pItem->isRoyal = (iRoyal >0)?true:false;
		m_ConsortiaUpdateParamMap[pItem->level] = pItem;
	}
	fclose(fp);
}
void CConsortiaManage::clearConsortiaUpdateParam()
{
	ConsortiaUpdateParam_Map_Iterator iter;
	ConsortiaUpdateParam *pDelItem = 0;
	for (iter = m_ConsortiaUpdateParamMap.begin();iter != m_ConsortiaUpdateParamMap.end();iter++)
	{
		if ((pDelItem = iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_ConsortiaUpdateParamMap.clear();
}

//����ID��ȡ�ƺ���Ϣ
LegionTitleParam* CConsortiaManage::getLegionTitleParam(int id)
{
	LegionTitleParam* pItem = 0;
	LegionTitleParam_Map_Map_Iterator mapIter;
	LegionTitleParam_Map *pLegionTitleParamMap = 0;
	LegionTitleParam_Map_Iterator itemIter;
	for (mapIter = m_LegionTitleParamMap.begin();mapIter != m_LegionTitleParamMap.end();mapIter++)
	{
		pLegionTitleParamMap = mapIter->second;
		if (pLegionTitleParamMap == NULL)
		{
			continue;
		}
		itemIter = pLegionTitleParamMap->find(id);
		if (itemIter != pLegionTitleParamMap->end())
		{
			pItem = itemIter->second;
			break;
		}
	}
	return pItem;
}
//�������ͣ���ֵ��ȡ������������óƺ�
LegionTitleParam* CConsortiaManage::getLegionTitleParam(int type,int val)
{
	LegionTitleParam* pItem = 0;
	LegionTitleParam_Map_Map_Iterator mapIter;
	LegionTitleParam_Map *pLegionTitleParamMap = 0;
	LegionTitleParam_Map_Iterator itemIter;
	mapIter = m_LegionTitleParamMap.find(type);
	if (mapIter == m_LegionTitleParamMap.end())
	{
		return NULL;
	}
	pLegionTitleParamMap = mapIter->second;
	if (pLegionTitleParamMap == NULL)
	{
		return NULL;
	}
	for (itemIter = pLegionTitleParamMap->begin();itemIter != pLegionTitleParamMap->end();itemIter++)
	{
		if (itemIter->second->condValue <= val)
		{
			pItem = itemIter->second;
		}
	}
	return pItem;
}
int CConsortiaManage::getLigionTitleID(int exp,int money)
{
	LegionTitleParam *pMoney = getLegionTitleParam(LOVE_LEGIONTITLE,money);	
	LegionTitleParam *pHonors = getLegionTitleParam(EXP_LEGIONTITLE,exp);
	LegionTitleParam *pCurParam = pMoney;
	if (pMoney == NULL)
	{
		pCurParam = pHonors;
	}
	else if(pHonors == NULL)
	{
		pCurParam = pMoney;
	}
	else
	{
		pCurParam = (pMoney->level > pHonors->level)?pMoney:pHonors;
	}
	if (pCurParam == NULL)
	{
		return 0;
	}
	return pCurParam->legTitleID;
}
//���ݼ����ȡ������������
ConsortiaUpdateParam* CConsortiaManage::getConsortiaUpdateParam(int level)
{
	ConsortiaUpdateParam_Map_Iterator iter;
	iter = m_ConsortiaUpdateParamMap.find(level);
	if (iter == m_ConsortiaUpdateParamMap.end())
	{
		return NULL;
	}
	return iter->second;
}
//�����������������ӵ����ź���ۻ������ñ�,ǧ�ֱ�
void CConsortiaManage::loadConversionRateParam()
{
	FILE *fp = 0;
	fp = fopen("configtable/ConversionRate.csv","r");
	if(fp == NULL)
	{
		printf("��configtable/ConversionRate.csvʧ�ܡ�\n");
		return;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);


	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;

		if(sscanf(buf,"%d,%d,%d,%d,%d",&m_ConversionRateParam.expRate,&m_ConversionRateParam.creditRate,
			&m_ConversionRateParam.money2ContrRate,&m_ConversionRateParam.exp2ContrRate,&m_ConversionRateParam.credit2ContrRate)<5)
		{
			continue;
		}
		if (m_ConversionRateParam.money2ContrRate <=0)
		{
			m_ConversionRateParam.money2ContrRate = 1;
		}
		if (m_ConversionRateParam.exp2ContrRate <=0)
		{
			m_ConversionRateParam.exp2ContrRate  = 1;
		}
		if (m_ConversionRateParam.credit2ContrRate <=0)
		{
			m_ConversionRateParam.credit2ContrRate = 1;
		}
	}
	fclose(fp);
}
ConversionRateParam* CConsortiaManage::getConversionRateParam()
{
	return &m_ConversionRateParam;
}
