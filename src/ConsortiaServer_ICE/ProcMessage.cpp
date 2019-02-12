#include "ProcMessage.h"
#include "ConsortiaProtocol.h"
#include "ConsortiaDataDefine.h"
#include "MapServer_Protocol.h"
#include "log.hpp"

#pragma warning(disable:4189)
#pragma warning(disable:4244)

CProcMessage::CProcMessage(CConsortiaManage *pConManage):m_ConsortiaManage(pConManage)
{
	ConnectDBG();
}

CProcMessage::~CProcMessage()
{
	m_DBProxy.fini();
}


/**************************************************************************
*˵��������DBG
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::ConnectDBG()
{
	if(!m_ConsortiaManage)
		return false;

	m_DBProxy.fini();
	return m_DBProxy.init(m_ConsortiaManage->m_AgentDBParam.DBGIP,m_ConsortiaManage->m_AgentDBParam.DBGPort,MDM_GP_MAP_CONSORTIA);
}

/**************************************************************************
*˵����ҵ������
*���������msg:������Ϣ��
*����ֵ��
**************************************************************************/
void CProcMessage::ProcMsg(const char *msg)
{
	if(!msg)
		return;

	DISTRIBUTED_HEADER *head = (DISTRIBUTED_HEADER*)msg;
	switch(head->command)
	{
	case CC_C_CREATE_CONSORTIA:				//���ᴴ��
		{
			OnCreateConsortia(msg);
		}
		break;
	case CC_C_QUERY_CONSORTIA:				//�����ѯ
		{
			OnQueryConsortiaList(msg);
		}
		break;
	case CC_C_DELETE_CONSORTIA:				//�����ɢ
		{
			OnDeleteConsortial(msg);
		}
		break;
	case CC_C_QUERY_USER_CON:				//��ѯ�����������
		{
			OnQueryUserConsortiaInfo(msg);
		}
		break;
	case CC_C_MODIFY_PLACARD:				//�޸Ĺ���
		{
			OnModifyPlacard(msg);
		}
		break;
	case CC_C_QUERY_LOGO:				//��ѯLOGO
		{
			OnQueryLogo(msg);
		}
		break;
	case CC_C_MODIFY_LOGO:				//�޸�LOGO
		{
			OnModifyLogo(msg);
		}
		break;
	case CC_C_CONTRIBUTE_BANK:				//������Դ����������
		{
			OnContributeBank(msg);
		}
		break;
	case CC_C_PAYOUT_BANK:				//֧��������Դ
		{
			OnPayoutBank(msg);
		}
		break;
	case CC_C_QUERY_BANK:				//�������в�ѯ
		{
			OnQueryBank(msg);
		}
		break;
	case CC_C_ALLY_APPLY_CON:				//��������
		{
			OnAllyApply(msg);
		}
		break;
	case CC_C_ALLY_CONSORTIA_AGREE:				//ͬ�����
		{
			OnAllyConsortia_Agree(msg);
		}
		break;
	case CC_C_ALLY_CONSORTIA_REFUSE:			//�ܾ�����
		{
			OnAllyConsortia_Refuse(msg);
		}
		break;
	case CC_C_UNALLY_CONSORTIA:				//����
		{
			OnUnallyConsortia(msg);
		}
		break;
	case CC_C_FIGHT_APPLY_CON:				//Լս����
		{
			OnFightApply(msg);
		}
		break;
	case CC_C_FIGHT_CONSORTIA_AGREE:				//ͬ��Լս
		{
			OnFightConsortia_Agree(msg);
		}
		break;
	case CC_C_FIGHT_CONSORTIA_REFUSE:				//�ܾ�Լս
		{
			OnFightConsortia_Refuse(msg);
		}
		break;
	case CC_C_QUERY_FIGHT:				//Լս��ѯ
		{
			OnQueryFight(msg);
		}
		break;
	case CC_C_JOIN_APPLY_CON:				//������빫��
		{
			OnJoinApply(msg);
		}
		break;
	case CC_C_JOIN_CONSORTIA_AGREE:				//�᳤ͬ����빫��
		{
			OnJoinConsortia_Agree(msg);
		}
		break;
	case CC_C_JOIN_CONSORTIA_REFUSE:				//�᳤�ܾ����빫��
		{
			OnJoinConsortia_Refuse(msg);
		}
		break;
	case CC_C_QUERY_CONUSER:				//�����Ա��ѯ
		{
			OnQueryConUserList(msg);
		}
		break;
	case CC_C_KICK_CONUSER:				//Kick��Ա
		{
			OnKickUser(msg);
		}
		break;
	case CC_C_QUIT_CONSORTIA:				//�˳�����
		{
			OnQuitConsortia(msg);
		}
		break;
	case CC_C_INVITE_JOIN_CON:				//������빫��
		{
			OnInviteJoinConsortia(msg);
		}
		break;
	case CC_C_INVITE_JOIN_AGREE:			//ͬ������
		{
			OnInviteJoinCon_Agree(msg);
		}
		break;
	case CC_C_INVITE_JOIN_REFUSE:			//�ܾ�����
		{
			OnInviteJoinCon_Refuse(msg);
		}
		break;
	case CC_C_POSE_OFFICIAL:				//��ְ
		{
			OnPoseOfficial(msg);
		}
		break;
	case CC_C_DEPOSE_OFFICIAL:				//��ְ
		{
			OnDeposeOfficial(msg);
		}
		break;
	case CC_C_QUERY_OFFICIAL:				//��ѯְλ
		{
			OnQueryOfficial(msg);
		}
		break;
	case CC_C_QUERY_CREATELIMIT:				//��ѯ�����������������
		{
			OnQueryCreateConLimit(msg);
		}
		break;
	case CC_C_MODIFY_CONTENET:				//�޸���ּ
		{
			OnModifyConTenet(msg);
		}
		break;
	case CC_C_CLIENTLOGIN:				//��½
		{
			OnClientLogin(msg);
		}
		break;
	case CC_C_TRANS_CHAIRMAN:				//�ƽ��᳤ְλ
		{
			OnTranChairman(msg);
		}
		break;
	case CC_C_CON_RANKING:
		{
			OnQueryConsortiaRanking(msg);
		}
		break;
	case CC_C_PLAYER_RANKING:
		{
			OnQueryPlayerRanking(msg);
		}
		break;
	case CC_C_CHECKCONSORTIA://��ʱ���
		{
			m_ConsortiaManage->CheckConsortia();
		}
		break;
	case CC_C_TIMER_VOTECOUNCIL://�Զ�ѡ�ٳ���Ա
		{
			m_ConsortiaManage->VoteCouncilTimer(msg,&m_DBProxy);
		}
		break;
	case CC_C_QUERY_USEROFCOUNTRY://��ѯ��ҵĹ������
		{
			OnQueryUserInfoOfCountry(msg);
		}
		break;
	case CC_C_QUERY_COUNCILLIST://��ѯ����Ӫ����Ա�б�
		{
			 OnQueryCouncilList(msg);
		}
		break;
	case CC_C_TAKEPARTIN_ELECTION://�μӾ�ѡ������Ϊ��Ա��
		{
			OnTakePartInElection(msg);
		}
		break;
	case CC_C_QUERY_CANDIDATELIST://��ѯ��ѡ���б�(����������������
		{
			OnQueryCandidateList(msg);
		}
		break;
	case CC_C_PLAYER_BALLOT://��ҽ���ͶƱѡ��
		{
			OnPlayerBallot(msg);				//ͶƱ
		}
		break;
	case CC_C_COUNTRY_DELATE://����
		{
			OnPlayerDelate(msg);				//����
		}
		break;
	case CC_C_TIMER_VOTEEND://ѡ�ٽ���
		{
			OnVoteEndTimer(msg);
		}
		break;
	case CC_C_TIMER_UNDERWRITE://����ǩ��
		{
			OnUnderWriteEndTimer(msg);		//����ǩ������
		}
		break;
	case CC_C_QUERY_DELATEINFO://��ѯ������Ϣ
		{
			OnQueryDelateInfo(msg);
		}
		break;
	case CC_C_DELATE_BALLOT://����ͶƱ
		{
			OnDelateBallot(msg);
		}
		break;
	case CC_C_MODIFY_ELECTIONCREED://�޸Ĳ�ѡ����
		{
			OnModifyElectionCreed(msg);
		}
		break;
	case CC_C_CREATE_TIRRIER://������������������
		{
			OnCreateTirrier(msg);
		}
		break;
	case CC_C_TIRRIER_GOOUT://����������
		{
			OnTirrierGoOut(msg);
		}
		break;
	case CC_C_UPGRADEBUILD://������������
		{
			OnUpgradeBuild(msg);
		}
		break;
	case CC_C_MODIFYNOTICE://�޸�ʵ������
		{
			OnModifyNotice(msg);
		}
		break;
	case CC_C_QUERY_TIRRIERCOUNT://��ѯ����������
		{
			OnQueryTirrierCount(msg);
		}
		break;
	case CC_S_NOTIFY_CREATETIRRIER://֪ͨ����ͼ����������
		{
			OnCreateTirrierNotify(msg);
		}
		break;
	case CC_S_NOTIFY_UPGRADEBUILD://֪ͨ����ͼ������������
		{
			OnUpgradeBuildNotify(msg);
		}
		break;
	case CC_S_NOTIFY_MODIFYNOTICE://֪ͨGM�޸���������
		{
		}
		break;
	case CC_C_NOTIFY_MARKETRATE://�г���˰
		{
			OnMarketRate(msg);
		}
		break;
	case CC_C_TIMER_DELATEEND://��������
		{
			OnDelateVoteEndTimer(msg);		//����ͶƱ����
		}
		break;
	case CC_C_TIMER_TIRRIERGOOUT:		//������������ʱ��
		{
			OnTirrierGoOutTimer(msg);
		}
		break;
	case CGC_S_NOTIFY_ELECTION://��ѡ�۷�
		{
			OnElectionNotify(msg);
		}
		break;
	case EN_C_MODIFYARMYSTATE://������������
		{
			OnModifyTirrierState(msg);
		}
		break;
	case CC_C_TIMER_BROADBEVOTE://ѡ����ǰ֪ͨ
		{
			m_ConsortiaManage->BroadBevote(msg,&m_DBProxy);
		}
		break;
	case CC_C_TIMER_RANKING://����
		{
			m_ConsortiaManage->Ranking(&m_DBProxy);
		}
		break;
	case CC_C_MODIFY_PURVIEW://��Ȩ
		{
			onModifyPurview(msg);
		}
		break;
	case CC_C_UPDATECONSORTIA://����
		{
			onUpdateConsortia(msg);
		}
		break;
	case CC_S_NOTIFY_UPDATECONSORTIA://�շ�ϵͳ����֪ͨ
		{
			onNotifyUpdateConsortia(msg);
		}
		break;
	case CC_C_CONTRIBUTEMONEY://��Ǯ
		{
			onContributeMoney(msg);
		}
		break;
	case CC_C_SALARIED://��ȡ����
		{
			onSalaried(msg);
		}
		break;
	case EN_C_NOTIFY_EXPORCREDIT://��һ�ȡ�˾��������
		{
			onNotifyPlayerExpOrHonors(msg);
		}
		break;
	case CC_C_MODIFY_SIGNID://�޸ľ��ű�ʶ
		{
			onModifySignID(msg);
		}
		break;
	default:
		{
			//OnDefault(msg);
			printf("error command.%d\n",head->command);
		}
		break;
	}
}

/**************************************************************************
*˵����Ĭ�ϴ���
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnDefault(const char *msg)
{
	return true;
}

/**************************************************************************
*˵������������
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnCreateConsortia(const char *msg)
{
	LPCreate_Consortia_Request req_msg = (LPCreate_Consortia_Request)msg;
	Create_Consortia_Response rep_msg;
	int country = 0;
	int ConsortiaID = 0;

	ConsortiaUpdateParam *pConsParam = m_ConsortiaManage->getConsortiaUpdateParam(1);
	if (pConsParam == NULL)
	{
		return false;
	}
	if(m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//�Ѽ��빫��
		rep_msg.serialize(req_msg->agent,req_msg->from,111,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//�����ж��Ƿ�����
	if(m_ConsortiaManage->IsExistConsortiaName(req_msg->ConName))
	{//����
		rep_msg.serialize(req_msg->agent,req_msg->from,1,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//��ȡ�ȼ����жϵȼ��Ƿ����Ҫ��,�漰��PlayerOfficerTable������OfficerID=PlayerIDʱ�������
	PlayerOfficerTable userinfo;
	userinfo.setmasterid(req_msg->from);
	userinfo.setdetailid(req_msg->from);
	if(m_DBProxy.db_select(userinfo) !=DB_Proxy::DB_SUCCEED)
	{//��ѯʧ��
		rep_msg.serialize(req_msg->agent,req_msg->from,111,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}

	if(userinfo.level_ < m_ConsortiaManage->GetLevelLimit())
	{//���𲻷���Ҫ��
		rep_msg.serialize(req_msg->agent,req_msg->from,2,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//��ȡ��Ǯ���Ƿ����
	PlayerChatRoomTable moneyinfo;
	moneyinfo.setid(req_msg->from);
	if(m_DBProxy.db_select(moneyinfo) !=DB_Proxy::DB_SUCCEED)
	{//��ѯʧ��
		rep_msg.serialize(req_msg->agent,req_msg->from,111,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	if(moneyinfo.money_ < m_ConsortiaManage->GetMoneyLimit())
	{//��Դ����
		rep_msg.serialize(req_msg->agent,req_msg->from,3,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	country = moneyinfo.country_;//�����Ӫ
	//��������
	int iConID = m_ConsortiaManage->GetUnusedConID();//��ȡһ������ID
	if(iConID <0)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	__int64 curtime = IceUtil::Time::now().toSeconds();//IceUtil::Time::now().toMilliSeconds();

	Consortia_Ptr new_con_ptr(new CConsortia(iConID,req_msg->ConName,0,0,req_msg->from,curtime,country));
	new_con_ptr->SetUserCount(0);
	new_con_ptr->SetPlacard(req_msg->Placard);
	new_con_ptr->SetTenet(req_msg->Tenet);
	new_con_ptr->SetFaceID(req_msg->FaceID);
	new_con_ptr->SetLevel(1);
	new_con_ptr->SetTeachID(0);
	new_con_ptr->setMaxUserNum(pConsParam->maxUserNum);
	//new_con_ptr->SetCountryID(country);
	if(req_msg->FaceID ==0)
	{//�Զ���LOGO
		new_con_ptr->SetFaceBuff(req_msg->LogoBuf);
	}
	//�������ݿ�
	//�۳���Ǯ
	memset(&moneyinfo,0,sizeof(PlayerChatRoomTable));
	moneyinfo.setid(req_msg->from);
	moneyinfo.money_ = 0 - m_ConsortiaManage->GetMoneyLimit();
	if(m_DBProxy.db_increase(moneyinfo) != DB_Proxy::DB_SUCCEED)
	{//ʧ��
		rep_msg.serialize(req_msg->agent,req_msg->from,4,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		m_ConsortiaManage->AddUnusedConID(iConID);//�Ż�ID
		return false;
	}
	//��������
	ConsortiaInfo con_record;
	con_record.setid(iConID);
	strncpy(con_record.name_,req_msg->ConName,SIZE_32);
	con_record.mapid_ = 0;
	con_record.cityid_ = 0;
	con_record.level_ = 1;
	con_record.chairmanid_ = req_msg->from;
	con_record.techid_ = 0;
	con_record.count_ = 1;
	con_record.faceid_ = req_msg->FaceID;

	strncpy(con_record.placard_,req_msg->Placard,SIZE_256);
	strncpy(con_record.tenet_,req_msg->Tenet,SIZE_128);
	con_record.createtime_ = curtime;
	con_record.maxusernum_ = pConsParam->maxUserNum;
	if(m_DBProxy.db_insert(con_record) != DB_Proxy::DB_SUCCEED)
	{//ʧ��
		rep_msg.serialize(req_msg->agent,req_msg->from,111,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		m_ConsortiaManage->AddUnusedConID(iConID);
		return false;
	}
	//��ӹ���
	IceUtil::DateTime curDay;
	IceUtil::Time::now().toDateTime(curDay);
	char today[9] = {0};
	sprintf(today,"%d%.2d%.2d",curDay.year,curDay.mon,curDay.day);
	m_ConsortiaManage->WriteLock_Consortia(iConID);
	m_ConsortiaManage->AddConsortia(iConID,new_con_ptr);
	ConsortiaUser_Ptr new_user_ptr(new CConsortiaUser(req_msg->from,iConID,CHARIRMAN_JOB,m_ConsortiaManage->GetOfficialPurview(CHARIRMAN_JOB),0));
	new_user_ptr->setLegionTitle(m_ConsortiaManage->getLigionTitleID(0,0));
	new_user_ptr->setSalariedDay(today);
	m_ConsortiaManage->AddConsortiaUser(iConID,new_user_ptr);
	m_ConsortiaManage->Unlock_Consortia(iConID);
	m_ConsortiaManage->AddUsedConID(iConID);//������ù���ID

	//����û���
	ConsortiaPersonnel usertb;
	usertb.consortiaid_ = iConID;
	usertb.playerid_ = req_msg->from;
	usertb.conjob_ =CHARIRMAN_JOB;
	usertb.purview_ = new_user_ptr->GetPurview();//m_ConsortiaManage->GetOfficialPurview(1);
	usertb.type_ = 0;
	usertb.legiontitle_ = new_user_ptr->getLegionTitle();
	strcpy(usertb.salariedday_,today);
	if(m_DBProxy.db_insert(usertb) != DB_Proxy::DB_SUCCEED)
	{
		printf("Insert chairman(ConsortiaID:%d-playerid:%d) fault when creating consortia.\n",iConID,req_msg->from);
	}
	rep_msg.serialize(req_msg->agent,req_msg->from,0,iConID);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));

#ifdef SND_EVENT_NOTIFY
	/*֪ͨ����ϵͳ�������������Ĺ���*/	
	Notify_Add_Consortia notify_msg;
	notify_msg.ulPlayerID = req_msg->from;
	notify_msg.ulConsortiaID = iConID;
	notify_msg.serialize();
	event_notify(MDM_GP_MAP_CHAT,(char*)&notify_msg,sizeof(notify_msg));

	//���͹㲥��Ϣ
	char sndmsg[128];
	//sprintf(sndmsg,"���᡾%s��������",req_msg->ConName);
	sprintf(sndmsg,m_ConsortiaManage->m_NoticeParam[CreateCons_Notice],req_msg->ConName);
	SendMsg2Map(country,tSystem,sndmsg,1);
#endif

	ISystemLog::getInstance()->playerMoney_CreateConsortia_Log(req_msg->from,userinfo.officername_,
		moneyinfo.money_ + m_ConsortiaManage->GetMoneyLimit(),moneyinfo.money_,m_ConsortiaManage->GetMoneyLimit(),
		iConID,req_msg->ConName);
	return true;
}

/**************************************************************************
*˵������ѯ�����б�
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnQueryConsortiaList(const char *msg)
{
	LPQuery_Consortial_Request req_msg = (LPQuery_Consortial_Request)msg;
	int count = m_ConsortiaManage->GetConsortCount();
	int pages = (count -1 +MAX_COUNT_PAGE)/MAX_COUNT_PAGE;//��ҳ��
	int size = 0;
	LPQuery_Consortia_Response rep_msg ;
	if(count ==0)
	{//�޹���,���ؿ�
		size = sizeof(Query_Consortia_Response);
		rep_msg = (LPQuery_Consortia_Response)new char[size];
		rep_msg->serialize(size,req_msg->agent,req_msg->from,0,req_msg->Page,0);
		asynch_write(req_msg->agent,(char*)rep_msg,size);
		delete[] (char*)rep_msg;
		return false;
	}
	size = sizeof(Query_Consortia_Response) + MAX_COUNT_PAGE * sizeof(Consortia_Info);
	rep_msg = (LPQuery_Consortia_Response)new char[size];
	LPConsortia_Info conList = (LPConsortia_Info)(rep_msg+1);
	//��ȡ�����������ID
	int ConsortiaID = 0;
	m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID);
	count = MAX_COUNT_PAGE;
	//��ѯ�����б�
	m_ConsortiaManage->GetConsortList(conList,count,req_msg->Page*MAX_COUNT_PAGE,ConsortiaID,&m_DBProxy);
	size = sizeof(Query_Consortia_Response) + sizeof(Consortia_Info) * count;//
	rep_msg->serialize(size,req_msg->agent,req_msg->from,count,req_msg->Page,pages);
	asynch_write(req_msg->agent,(char*)rep_msg,size);
	delete[] (char*)rep_msg;
	return true;
}

/**************************************************************************
*˵����ɾ������
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnDeleteConsortial(const char *msg)
{
	/*
	*/
	LPDelete_Consortia_Request req_msg = (LPDelete_Consortia_Request)msg;
	Delete_Consortia_Response rep_msg;
	int ConsortiaID = 0;
	//������һ�ȡ��������
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//δ���빫��
		rep_msg.serialize(req_msg->agent,req_msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	int error_code = 1;

	Consortia_Ptr con_ptr;
	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);//�Ըù������
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get() ==0)
	{
		error_code = 2;
	}
	else if((con_ptr->GetChairmanID() == req_msg->from))//�᳤
	{
		m_ConsortiaManage->DelConsortia(ConsortiaID);//�б���ɾ��
		error_code = 0;
	}
	else
	{//�ɽ�һ���ж�Ȩ��
		if((con_ptr->GetUserPurview(req_msg->from)& PURVIEW_DISBAND_CON) == PURVIEW_DISBAND_CON)
		{
			m_ConsortiaManage->DelConsortia(ConsortiaID);//�б���ɾ��
			error_code = 0;
		}
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);//�Ըù������

	//���ȷ��ز������
	rep_msg.serialize(req_msg->agent,req_msg->from,error_code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	//printf("agent:%d,from:%d,to:%d\n",rep_msg.agent,rep_msg.from,rep_msg.to);

	if(error_code == 0)
	{
		//���ݿ����
		//ɾ�������Ա��
		{
			ConsortiaUser_Iter useriter;
			ConsortiaPersonnel deluser;
			for(useriter = con_ptr->ConUserList.begin();useriter != con_ptr->ConUserList.end();useriter++)
			{
				deluser.setmasterid(ConsortiaID);
				deluser.setdetailid(useriter->second->GetPlayerID());
				m_ConsortiaManage->DelConsortiaUser(ConsortiaID,useriter->second->GetPlayerID());//�ӹ����Ա�б���ɾ��
				if(m_DBProxy.db_delete(deluser) != DB_Proxy::DB_SUCCEED)
				{
					printf("Delete consortia user fault.\n");
				}
#ifdef SND_EVENT_NOTIFY
				/*������ϵͳ���͸�����ҹ�����֪ͨ*/
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
				delbank.setmasterid(ConsortiaID);
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
			delcon.setid(ConsortiaID);
			if(m_DBProxy.db_delete(delcon) != DB_Proxy::DB_SUCCEED)
			{
					printf("Delete consortia fault.\n");
			}
		}
#ifdef SND_EVENT_NOTIFY
		//����ϵͳ�㲥��Ϣ
		int country = 0;
		bool isonline = true;
		int agent;
		char sndmsg[128];
		m_ConsortiaManage->GetPlayerInfo(con_ptr->GetChairmanID(),0,country,isonline,agent,&m_DBProxy);
		//sprintf(sndmsg,"���᡾%s���ѽ�ɢ��",con_ptr->GetName());
		sprintf(sndmsg,m_ConsortiaManage->m_NoticeParam[DelConsortia_Notice],con_ptr->GetName());
		SendMsg2Map(country,tSystem,sndmsg,1);
#endif
		
		m_ConsortiaManage->DelUsedConID(ConsortiaID);
		m_ConsortiaManage->AddUnusedConID(ConsortiaID);
	}
	else
	{
	}
	return true;
}

/**************************************************************************
*˵������ѯ�û�����������Ϣ
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnQueryUserConsortiaInfo(const char *msg)
{
	LPQuery_User_Con_Request req_msg = (LPQuery_User_Con_Request)msg;
	Query_User_Con_Response rep_msg;
	int ConsortiaID = 0;
	//������һ�ȡ��������
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//δ���빫��
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		return false;
	}
	int code = -1;
	m_ConsortiaManage->ReadLock_Consortia(ConsortiaID);
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get())
	{
		rep_msg.userConInfo.conInfo.ChairmanID = con_ptr->GetChairmanID();
		rep_msg.userConInfo.conInfo.ConsortiaID = ConsortiaID;
		strncpy(rep_msg.userConInfo.conInfo.ConName,con_ptr->GetName(),SIZE_32);
		rep_msg.userConInfo.conInfo.FaceID = con_ptr->GetFaceID();
		strncpy(rep_msg.userConInfo.Placard,con_ptr->GetPlacard(),SIZE_256);
		strncpy(rep_msg.userConInfo.conInfo.Tenet,con_ptr->GetTenet(),SIZE_128);

		//add 20100302
		CConsortiaUser *pUser = con_ptr->getConsortiaUser(req_msg->from);
		rep_msg.userConInfo.ConJob = pUser->GetConsortiaJob();
		rep_msg.userConInfo.Purview = pUser->GetPurview();
		rep_msg.userConInfo.LegionTitle = pUser->getLegionTitle();
		rep_msg.userConInfo.Contribute = pUser->getContribute();
		rep_msg.userConInfo.Credit = pUser->getCredit();
		rep_msg.userConInfo.Exp = pUser->getExp();
		rep_msg.userConInfo.Money = pUser->getMoney();
		rep_msg.userConInfo.conInfo.IsRoyal = con_ptr->isRoyalConsortia();
		rep_msg.userConInfo.conInfo.MaxUserNum = con_ptr->getMaxUserNum();
		rep_msg.userConInfo.conInfo.Money = con_ptr->getMoney();
		rep_msg.userConInfo.conInfo.Exp = con_ptr->getExp();
		rep_msg.userConInfo.conInfo.Credit = con_ptr->getHonors();
		rep_msg.userConInfo.conInfo.SignID = con_ptr->getSignID();
		rep_msg.userConInfo.conInfo.Level = con_ptr->GetLevel();
		rep_msg.userConInfo.conInfo.Count = con_ptr->GetUserCount();
		rep_msg.userConInfo.conInfo.IsFight = 0;
		rep_msg.userConInfo.conInfo.IsUnion = 0;
		IceUtil::DateTime curDay;
		IceUtil::Time::now().toDateTime(curDay);
		char today[9] = {0};
		sprintf(today,"%d%.2d%.2d",curDay.year,curDay.mon,curDay.day);
		rep_msg.userConInfo.isCanSalaried = pUser->isCanSalaried(today);
		code = 0;
	}
	else
	{
		code =1;
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
	rep_msg.serialize(req_msg->agent,req_msg->from,code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	return true;
}

/**************************************************************************
*˵�����޸Ĺ���
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnModifyPlacard(const char *msg)
{
	LPModify_Con_Placard_Request req_msg = (LPModify_Con_Placard_Request)msg;
	Modify_Con_Placard_Response rep_msg;
	int	ConsortiaID = 0;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//δ���빫��
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}	
	int code = 0;
	//Ȩ���ж�
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get() ==0)
	{
		code =111;
	}
	else
	{
		//if(PURVIEW_MODIFY_PLACARD == (con_ptr->GetUserPurview(req_msg->from) & PURVIEW_MODIFY_PLACARD))
		if((con_ptr->GetChairmanID() == req_msg->from) ||(PURVIEW_MODIFY_PLACARD == (con_ptr->GetUserPurview(req_msg->from) & PURVIEW_MODIFY_PLACARD)))
		{//����Ȩ��
			con_ptr->SetPlacard(req_msg->Placard);
			con_ptr->update2DBG(&m_DBProxy);
		}
		else
		{
			code = 1;
		}
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);

	rep_msg.serialize(req_msg->agent,req_msg->from,code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));

	
	return true;
}

/**************************************************************************
*˵������ѯLOGO
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnQueryLogo(const char *msg)
{
	LPQuery_Con_LOGO_Request req_msg = (LPQuery_Con_LOGO_Request)msg;
	Query_Con_LOGO_Response rep_msg;
	if(req_msg->ConsortiaID<0 || req_msg->ConsortiaID >= MAX_CONSORTIA_COUNT)
	{//��Ų��Ϸ�
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	int code = 0;
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->ReadLock_Consortia(req_msg->ConsortiaID);
	m_ConsortiaManage->GetConsortia(req_msg->ConsortiaID,con_ptr);
	if(con_ptr.get() ==0)
	{
		code = 1;
	}
	else
	{//MAX_LOGO_SIZE
		rep_msg.FaceID = con_ptr->GetFaceID();
		memcpy(rep_msg.FaceBuf,con_ptr->GetFaceBuff(),MAX_LOGO_SIZE);
	}
	m_ConsortiaManage->Unlock_Consortia(req_msg->ConsortiaID);

	rep_msg.serialize(req_msg->agent,req_msg->from,code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	return true;
}

/**************************************************************************
*˵�����޸�LOGO
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnModifyLogo(const char *msg)
{
	LPModify_Con_LOGO_Request req_msg = (LPModify_Con_LOGO_Request)msg;
	Modify_Con_LOGO_Response rep_msg;
	int ConsortiaID = 0;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//δ���빫��
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	int code = 0;
	//Ȩ���ж�
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get() ==0)
	{
		code =111;
	}
	else
	{
		if((con_ptr->GetChairmanID() == req_msg->from)||(PURVIEW_MODIFY_LOGO == (con_ptr->GetUserPurview(req_msg->from) & PURVIEW_MODIFY_LOGO)))
		{//����Ȩ��
			con_ptr->SetFaceID(req_msg->FaceID);
			if(req_msg->FaceID ==0)
			{//�Զ���
				con_ptr->SetFaceBuff(req_msg->FaceBuf);
			}
			con_ptr->update2DBG(&m_DBProxy);
		}
		else
		{
			code = 1;
		}
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);

	rep_msg.serialize(req_msg->agent,req_msg->from,code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));

	return true;
}

/**************************************************************************
*˵����������Դ������
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnContributeBank(const char *msg)
{
	return true;
}

/**************************************************************************
*˵����֧�乫�����е���Դ
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnPayoutBank(const char *msg)
{
	return true;
}

/**************************************************************************
*˵������ѯ��������
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnQueryBank(const char *msg)
{
	LPQuery_Con_Bank_Request req_msg = (LPQuery_Con_Bank_Request)msg;
	LPQuery_Con_Bank_Response rep_msg = 0;
	int count = 0;
	int pages = 0;
	int size = 0;
	int ConsortiaID = 0;
	//��ȡ������ڵĹ���ID
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{
		size = sizeof(Query_Con_Bank_Response);
		rep_msg = new Query_Con_Bank_Response;
		rep_msg->serialize(size,req_msg->agent,req_msg->from,-1,req_msg->Page,0);
		asynch_write(rep_msg->agent,(char*)rep_msg,size);
		delete[] (char*)rep_msg;
		return false;
	}

	Consortia_Ptr con_ptr;
	m_ConsortiaManage->ReadLock_Consortia(ConsortiaID);
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get())
	{
		count = con_ptr->GetBankCount();
		pages = (count -1 + MAX_COUNT_PAGE)/MAX_COUNT_PAGE;
		size = sizeof(Query_Con_Bank_Response) + sizeof(Bank_ItemInfo) *MAX_COUNT_PAGE;
		rep_msg = (LPQuery_Con_Bank_Response) new char[size];
		LPBank_ItemInfo bankList = (LPBank_ItemInfo)(rep_msg+1);
		count = MAX_COUNT_PAGE;
		con_ptr->GetBankList(bankList,MAX_COUNT_PAGE*req_msg->Page,count);
		size = sizeof(Query_Con_Bank_Response) + sizeof(Bank_ItemInfo) *count;
	}
	else
	{
		size = sizeof(Query_Con_Bank_Response);
		rep_msg = (LPQuery_Con_Bank_Response) new char[size];
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
	rep_msg->serialize(size,req_msg->agent,req_msg->from,count,req_msg->Page,pages);
	asynch_write(rep_msg->agent,(char*)rep_msg,size);
	delete[] (char*)rep_msg;
	return true;
}

/**************************************************************************
*˵������������
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnAllyApply(const char *msg)
{
	LPAlly_Apply_Con_Request req_msg = (LPAlly_Apply_Con_Request)msg;
	Ally_Apply_Con_Response rep_msg;
	int ConsortiaID = 0;
	char conName[33] = {0};
	int ChairmanID =0;
	char PlayerName[33] = {0};
	//���ȺϷ��Լ��
	if(req_msg->AllyConsortiaID <0 || req_msg->AllyConsortiaID>=MAX_CONSORTIA_COUNT)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//�ж�����Ƿ�Ϊ�᳤
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//ͬһ�����޷�����
	if(ConsortiaID == req_msg->AllyConsortiaID)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,3);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->ReadLock_Consortia(ConsortiaID);
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
	if(!con_ptr.get())
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	strncpy(conName,con_ptr->GetName(),SIZE_32);
	if(con_ptr->GetChairmanID() != req_msg->from)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//��ȡ�Է��᳤
	m_ConsortiaManage->ReadLock_Consortia(req_msg->AllyConsortiaID);
	m_ConsortiaManage->GetConsortia(req_msg->AllyConsortiaID,con_ptr);
	m_ConsortiaManage->Unlock_Consortia(req_msg->AllyConsortiaID);
	if(!con_ptr.get())
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	ChairmanID = con_ptr->GetChairmanID();
	//���ؽ��
	rep_msg.serialize(req_msg->agent,req_msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));

#ifdef SND_EVENT_NOTIFY
	//�����ʼ�
	int country;
	bool isonline = false;
	int agent = req_msg->agent;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,PlayerName,country,isonline,agent,&m_DBProxy))
	{
		strcpy(PlayerName,m_ConsortiaManage->m_NoticeParam[Default_Notice]);
	}

	Notify_Send_Mail email;
	email.type = Consortia_union;
	email.ulRecieverPlayerID = ChairmanID;
	strncpy(email.szSendPlayerName,PlayerName,SIZE_16);
	/*
	strncpy(email.sztitle,"�����������",SIZE_16);
	sprintf(email.szbody,"���᡾%s�����������Ĺ�����н��ˣ��Ƿ�ͬ�⣿",conName);
	*/
	strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[AllyApply_Email].Title,SIZE_16);
	sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[AllyApply_Email].Body,conName);

	LPEmailBody_Union pdata = (LPEmailBody_Union)email.szPertain;
	pdata->ConsortiaID = ConsortiaID;
	pdata->PlayerID = req_msg->from;
	email.serialize();
	event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));
#endif
	return true;
}

/**************************************************************************
*˵����ͬ�����
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnAllyConsortia_Agree(const char *msg)
{
	LPAlly_Consortia_Agree_Request req_msg = (LPAlly_Consortia_Agree_Request)msg;
	Ally_Consortia_Agree_Response rep_msg;
	int ConsortiaID = 0;
	int code = 0;
	char conName[33];
	//���ȺϷ��Լ��
	if(req_msg->ConsortiaID <0 || req_msg->ConsortiaID>=MAX_CONSORTIA_COUNT)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//�ж�����Ƿ�Ϊ�᳤
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	if(ConsortiaID == req_msg->ConsortiaID)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,3);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//�ж��Ƿ�
	ConsortiaUnion_Ptr union_ptr;
	CConsortiaUnion *pNewUnion = 0;
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get())
	{
		if(con_ptr->GetChairmanID() != req_msg->from)
		{
			code =1;
		}
		else
		{
			//�ж��Ƿ�����ͬ��
			if(con_ptr->IsUnionConsortia(req_msg->ConsortiaID))
			{
				code = 2;
			}
			else
			{
				strncpy(conName,con_ptr->GetName(),SIZE_32);
				pNewUnion = new CConsortiaUnion(0,ConsortiaID,req_msg->ConsortiaID);
				union_ptr = pNewUnion;
				//���빫��
				code = con_ptr->AddUnion(union_ptr);
			}
		}
	}
	else
	{
		code = 1;
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
	if(code)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,code);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//
	Consortia_Ptr con_ptr2;
	m_ConsortiaManage->WriteLock_Consortia(req_msg->ConsortiaID);
	m_ConsortiaManage->GetConsortia(req_msg->ConsortiaID,con_ptr2);
	if(con_ptr2.get())
	{
		code = con_ptr2->AddUnion(union_ptr);
	}
	else
	{
		code = 111;
	}
	m_ConsortiaManage->Unlock_Consortia(req_msg->ConsortiaID);
	if(code)
	{//
		m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
		m_ConsortiaManage->DelConsortiaUnin(ConsortiaID,req_msg->ConsortiaID);
		m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
		rep_msg.serialize(req_msg->agent,req_msg->from,code);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	rep_msg.serialize(req_msg->agent,req_msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	//���ݿ����
	ConsortiaUnion uniontb;
	uniontb.setid(ConsortiaID,req_msg->ConsortiaID);
	if(m_DBProxy.db_insert(uniontb) != DB_Proxy::DB_SUCCEED)
	{
		printf("OnAllyConsortia_Agree(ConsortiaUnion) fault.\n");
	}
	else
	{
		pNewUnion->SetUnionID(uniontb.unionid_);
	}
	
#ifdef SND_EVENT_NOTIFY
	//�����ʼ�֪ͨ
	int country;
	bool isonline;
	char PlayerName[33] = {0};
	int agent = req_msg->agent;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,PlayerName,country,isonline,agent,&m_DBProxy))
	{
		strcpy(PlayerName,m_ConsortiaManage->m_NoticeParam[Default_Notice]);
	}
	Notify_Send_Mail email;
	email.type = Normal;
	email.ulRecieverPlayerID = req_msg->ApplyPlayerID;
	strncpy(email.szSendPlayerName,PlayerName,SIZE_16);
	/*
	strncpy(email.sztitle,"��������֪ͨ",SIZE_16);
	sprintf(email.szbody,"�������빫�᡾%s�����˵�������ͨ����",conName);
	*/
	strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[AgreeAlly_Email].Title,SIZE_16);
	sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[AgreeAlly_Email].Body,conName);
	email.serialize();
	event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));	

	//���ͽ��˹㲥
#endif
	return true;
}

/**************************************************************************
*˵�����ܾ�����
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnAllyConsortia_Refuse(const char *msg)
{
	LPAlly_Consortia_Refuse_Request req_msg = (LPAlly_Consortia_Refuse_Request)msg;
	Ally_Consortia_Refuse_Response rep_msg;
	int ConsortiaID = 0;
	int code = 0;
	char conName[33] = {0};
	//�ж�����Ƿ�Ϊ�᳤
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	m_ConsortiaManage->ReadLock_Consortia(ConsortiaID);
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get())
	{
		code = 1;
		if(req_msg->from == con_ptr->GetChairmanID())
		{
			code = 0;
			strncpy(conName,con_ptr->GetName(),SIZE_32);
		}
	}
	else
	{
		code = 1;
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
	rep_msg.serialize(req_msg->agent,req_msg->from,code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));

	if(code == 0)
	{
#ifdef SND_EVENT_NOTIFY
		//�ʼ�֪ͨ
		int country;
		bool isonline;
		char PlayerName[33] = {0};
		int agent = req_msg->agent;
		if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,PlayerName,country,isonline,agent,&m_DBProxy))
		{
			strcpy(PlayerName,m_ConsortiaManage->m_NoticeParam[Default_Notice]);
		}

		Notify_Send_Mail email;
		email.type = Normal;
		email.ulRecieverPlayerID = req_msg->ApplyPlayerID;
		strncpy(email.szSendPlayerName,PlayerName,SIZE_16);
		/*
		strncpy(email.sztitle,"��������֪ͨ",SIZE_16);
		sprintf(email.szbody,"�������빫�᡾%s�����˵����󱻾ܾ���",conName);	
		*/
		strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[RefuseAlly_Email].Title,SIZE_16);
		sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[RefuseAlly_Email].Body,conName);	
		email.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));	
#endif
	}
	return true;
}
/**************************************************************************
*˵��������
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnUnallyConsortia(const char *msg)
{
	LPUnally_Consortia_Request req_msg = (LPUnally_Consortia_Request)msg;
	Unally_Consortia_Response rep_msg;
	int ConsortiaID = 0;
	int code = 0;
	char conName[33] = {0};
	int ChairmanID = 0;
	if(req_msg->ConsortiaID <0 || req_msg->ConsortiaID >=MAX_CONSORTIA_COUNT)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//�ж�����Ƿ�Ϊ�᳤
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get())
	{
		if(con_ptr->GetChairmanID() == req_msg->from)
		{
			code = 0;
			if(con_ptr->IsUnionConsortia(req_msg->ConsortiaID))
			{
				con_ptr->DelUnion(req_msg->ConsortiaID);
				strncpy(conName,con_ptr->GetName(),SIZE_32);
			}
			else
			{
				code = 2;
			}
		}
		else
		{
			code = 1;
		}
	}
	else
	{
		code = 1;
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
	if(code ==0)
	{
		m_ConsortiaManage->WriteLock_Consortia(req_msg->ConsortiaID);
		m_ConsortiaManage->GetConsortia(req_msg->ConsortiaID,con_ptr);
		if(con_ptr.get())
		{
			ChairmanID = con_ptr->GetChairmanID();
			con_ptr->DelUnion(ConsortiaID);
		}
		m_ConsortiaManage->Unlock_Consortia(req_msg->ConsortiaID);
	}
	rep_msg.serialize(req_msg->agent,req_msg->from,code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	//�ʼ�֪ͨ
	if(code ==0)
	{
#ifdef SND_EVENT_NOTIFY
		int country;
		bool isonline = true;
		char PlayerName[33] = "ϵͳ";
		int agent = req_msg->agent;
		if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,PlayerName,country,isonline,agent,&m_DBProxy))
		{
			strcpy(PlayerName,m_ConsortiaManage->m_NoticeParam[Default_Notice]);
		}
		Notify_Send_Mail email;
		email.type = Normal;
		email.ulRecieverPlayerID = ChairmanID;
		strncpy(email.szSendPlayerName,PlayerName,SIZE_16);
		/*
		strncpy(email.sztitle,"����֪ͨ",SIZE_16);
		sprintf(email.szbody,"���᡾%s���������Ĺ�����ͬ�˹�ϵ��",conName);
		*/
		strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[Unally_Email].Title,SIZE_16);
		sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[Unally_Email].Body,conName);
		email.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));

		//���ͽ��˹㲥
#endif
	}
	return true;
}

/**************************************************************************
*˵����Լս����
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnFightApply(const char *msg)
{
	return true;
}

/**************************************************************************
*˵����ͬ��Լս
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnFightConsortia_Agree(const char *msg)
{
	return true;
}

/**************************************************************************
*˵�����ܾ�Լս
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnFightConsortia_Refuse(const char *msg)
{
	return true;
}
/**************************************************************************
*˵����Լս��ѯ
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnQueryFight(const char *msg)
{
	return true;
}

/**************************************************************************
*˵����������빫��
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnJoinApply(const char *msg)
{
	LPJoin_Apply_Con_Request req_msg = (LPJoin_Apply_Con_Request)msg;
	Join_Apply_Con_Response rep_msg;

	if(req_msg->ConsortiaID <0 || req_msg->ConsortiaID >MAX_CONSORTIA_COUNT)
		return false;

	int chairman_id = 0;
	int chairman_country = 0;
	bool chairman_Online = false;

	char player_name[32];
	int player_country = 0;
	bool player_Online = false;
	/*�����ж���ҵȼ��Ƿ��������*/
	PlayerOfficerTable playertb;
	playertb.setmasterid(req_msg->from);
	playertb.setdetailid(req_msg->from);
	if(m_DBProxy.db_select(playertb) != DB_Proxy::DB_SUCCEED)
	{//��ѯʧ��
		rep_msg.serialize(req_msg->agent,req_msg->from,112);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	if(playertb.level_ < m_ConsortiaManage->GetJoinConLimit())
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	int code = 0;
	//�ж���Ӫ�Ƿ�һ��
	//���Ȼ�ȡ�᳤����Ϣ
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->ReadLock_Consortia(req_msg->ConsortiaID);
	m_ConsortiaManage->GetConsortia(req_msg->ConsortiaID,con_ptr);
	if(!con_ptr.get())
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,113);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	}
	else
	{
		if(con_ptr->GetUserCount() >= con_ptr->getMaxUserNum())
		{
			rep_msg.serialize(req_msg->agent,req_msg->from,8);
			asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		}
		else
		{
			chairman_id = con_ptr->GetChairmanID();//�᳤ID
			int agent = 0;
			if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,player_name,player_country,player_Online,agent,&m_DBProxy))
			{//δ�ҵ�
				rep_msg.serialize(req_msg->agent,req_msg->from,115);
				asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
			}
			else
			{
				if(con_ptr->GetCountryID() != player_country)
				{//������ͬһ��Ӫ
					rep_msg.serialize(req_msg->agent,req_msg->from,2);
					asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
				}
				else
				{
					rep_msg.serialize(req_msg->agent,req_msg->from,0);
					asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
					//��᳤�����ʼ�֪ͨ��������Ƿ񱣴棿��

#ifdef SND_EVENT_NOTIFY
					Notify_Send_Mail email;
					email.type = Apply_add_consortia;
					email.ulRecieverPlayerID = chairman_id;
					strncpy(email.szSendPlayerName,player_name,SIZE_16);

					strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[JoinApply_Email].Title,SIZE_16);
					sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[JoinApply_Email].Body,player_name);	

					LPEmailBody_JoinCon pdata = (LPEmailBody_JoinCon)email.szPertain;
					pdata->PlayerID = req_msg->from;
					pdata->ConsortiaID = req_msg->ConsortiaID;
					//email.serialize();
					//event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));	
					con_ptr->sendEmail2Offical(req_msg->from,player_name,&email);
#endif
				}
				
			}
		}
			
		
	}
	m_ConsortiaManage->Unlock_Consortia(req_msg->ConsortiaID);

	return true;
}

/**************************************************************************
*˵����ͬ����빫��
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnJoinConsortia_Agree(const char *msg)
{
	LPJoin_Consortia_Agree_Request req_msg = (LPJoin_Consortia_Agree_Request)msg;
	Join_Consortia_Agree_Response rep_msg;

	char conName[33];

	//�����ж�����Ƿ����Ȩ��
	if((m_ConsortiaManage->GetUserPurview(req_msg->from) &PURVIEW_AGRESS_JOIN) != PURVIEW_AGRESS_JOIN)
	{//û����Ȩ��
		rep_msg.serialize(req_msg->agent,req_msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//��ȡ������ڵĹ���ID
	int ConsortiaID = 0;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//�ж�����Ƿ��Ѽ���ĳ����
	int JoinedConID = 0;
	if(m_ConsortiaManage->HasJoinConsortia(req_msg->PlayerID,JoinedConID))
	{//��׼���Ѽ��빫��
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//���ݿ����
	//add by xujincai 20090908�ж�����Ƿ����,��������Ӫ
	int iPlayerCountry = 0;
	bool bOnline = false;
	int iAgent = 0;
	if (m_ConsortiaManage->GetPlayerInfo(req_msg->PlayerID,0,iPlayerCountry,bOnline,iAgent,&m_DBProxy) !=0)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if (con_ptr.get() && con_ptr->GetCountryID() == iPlayerCountry)
	{
		if(con_ptr->GetUserCount() >= con_ptr->getMaxUserNum())
		{
			rep_msg.serialize(req_msg->agent,req_msg->from,8);
			asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
			m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
			return false;
		}
		IceUtil::DateTime curDay;
		IceUtil::Time::now().toDateTime(curDay);
		char today[9] = {0};
		sprintf(today,"%d%.2d%.2d",curDay.year,curDay.mon,curDay.day);
		ConsortiaPersonnel usertb;
		usertb.setmasterid(ConsortiaID);
		usertb.setdetailid(req_msg->PlayerID);
		usertb.conjob_ = LEAGUER_JOB;
		usertb.purview_ = m_ConsortiaManage->GetOfficialPurview(LEAGUER_JOB);
		usertb.type_ = 0;
		usertb.legiontitle_ = m_ConsortiaManage->getLigionTitleID(0,0);
		strcpy(usertb.salariedday_,today);
		if(m_DBProxy.db_insert(usertb) != DB_Proxy::DB_SUCCEED)
		{
			rep_msg.serialize(req_msg->agent,req_msg->from,111);
		}
		else
		{//���빫�����	
			ConsortiaUser_Ptr user_ptr(new CConsortiaUser(req_msg->PlayerID,ConsortiaID,LEAGUER_JOB,usertb.purview_,0));
			user_ptr->setLegionTitle(usertb.legiontitle_);
			user_ptr->setSalariedDay(today);
			m_ConsortiaManage->AddConsortiaUser(ConsortiaID,user_ptr);
			con_ptr->update2DBG(&m_DBProxy);
			rep_msg.serialize(req_msg->agent,req_msg->from,0);
		}
	}
	else
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
	}
	
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	if (rep_msg.Error_Code)
	{
		return false;
	}

#ifdef SND_EVENT_NOTIFY	
	//�����ʼ�֪ͨ���,��֪ͨ����ϵͳ�޸�ĳ��ҵĹ�����
	
	Notify_Add_Consortia notify_msg;
	notify_msg.ulPlayerID = req_msg->PlayerID;
	notify_msg.ulConsortiaID = ConsortiaID;
	notify_msg.serialize();
	event_notify(MDM_GP_MAP_CHAT,(char*)&notify_msg,sizeof(notify_msg));

	int country;
	bool isonline;
	char PlayerName[33] = {0};
	int agent = req_msg->agent;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,PlayerName,country,isonline,agent,&m_DBProxy))
	{
		strcpy(PlayerName,m_ConsortiaManage->m_NoticeParam[Default_Notice]);
	}

	Notify_Send_Mail email;
	email.type = Normal;
	email.ulRecieverPlayerID = req_msg->PlayerID;
	strncpy(email.szSendPlayerName,PlayerName,SIZE_16);
	/*
	strncpy(email.sztitle,"���빫��֪ͨ",SIZE_16);
	sprintf(email.szbody,"���ѱ���%s����׼���빫�᡾%s����",PlayerName,conName);
	*/
	strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[AgreeJoin_Email].Title,SIZE_16);
	sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[AgreeJoin_Email].Body,PlayerName,conName);
	email.serialize();
	event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));	

	//���͹���㲥��Ϣ����ʾ��XX���������ڹ���
	m_ConsortiaManage->GetPlayerInfo(req_msg->PlayerID,PlayerName,country,isonline,agent,&m_DBProxy);
	char sndmsg[128];
	//sprintf(sndmsg,"��%s�����������ڵĹ��ᡣ",PlayerName);
	sprintf(sndmsg,m_ConsortiaManage->m_NoticeParam[JoinConsortia_Notice],PlayerName);
	SendMsg2Consortia(ConsortiaID,tSystem,sndmsg,req_msg->PlayerID);
#endif
	return true;
}

/**************************************************************************
*˵�����ܾ����빫��
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnJoinConsortia_Refuse(const char *msg)
{
	LPJoin_Consortia_Refuse_Request req_msg = (LPJoin_Consortia_Refuse_Request)msg;
	Join_Consortia_Refuse_Response rep_msg;
	char conName[33];
	//������˱��������б������б�
	//��ȡ������ڵĹ���ID
	int ConsortiaID = 0;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	m_ConsortiaManage->ReadLock_Consortia(ConsortiaID);
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get())
	{
		strncpy(conName,con_ptr->GetName(),SIZE_32);
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);

	//���ؽ��
	rep_msg.serialize(req_msg->agent,req_msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));

#ifdef SND_EVENT_NOTIFY
	//�����ʼ�֪ͨ�����뱻�ܾ�
	int country;
	bool isonline;
	char PlayerName[33] = {0};
	int agent = req_msg->agent;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,PlayerName,country,isonline,agent,&m_DBProxy))
	{
		strcpy(PlayerName,m_ConsortiaManage->m_NoticeParam[Default_Notice]);
	}

	Notify_Send_Mail email;
	email.type = Normal;
	email.ulRecieverPlayerID = req_msg->PlayerID;
	strncpy(email.szSendPlayerName,PlayerName,SIZE_16);
	/*
	strncpy(email.sztitle,"���빫��֪ͨ",SIZE_16);
	sprintf(email.szbody,"��%s���ܾ��������빫�᡾%s�������롣",PlayerName,conName);
	*/
	strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[RefuseJoin_Email].Title,SIZE_16);
	sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[RefuseJoin_Email].Body,PlayerName,conName);
	email.serialize();
	event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));	
#endif

	return true;
}
/**************************************************************************
*˵������ѯ�����Ա�б�
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnQueryConUserList(const char *msg)
{
	LPQuery_Con_User_Request req_msg = (LPQuery_Con_User_Request)msg;
	LPQuery_Con_User_Response rep_msg;
	int size = 0;
	int count = 0;
	int pages = 0;
	int ConsortiaID = 0;
	int code = 0;

	//���Ȼ�ȡ������ڵĹ�����
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//δ���빫��
		size = sizeof(Query_Con_User_Response);
		rep_msg = (LPQuery_Con_User_Response)new char[size];
		rep_msg->serialize(size,req_msg->agent,req_msg->from,-1,req_msg->Page,0);
		asynch_write(rep_msg->agent,(char*)rep_msg,size);
		delete[] (char*)rep_msg;
		return false;
	}
	LPCon_UserInfo userlist = 0;
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->ReadLock_Consortia(ConsortiaID);
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(!con_ptr.get())
	{//�����ڹ���
		code = -1;
		size = sizeof(Query_Con_User_Response);
		rep_msg = (LPQuery_Con_User_Response)new char[size];
		rep_msg->serialize(size,req_msg->agent,req_msg->from,-1,req_msg->Page,0);
	}
	else
	{
		count = con_ptr->GetUserCount();
		if(count<=0)
		{
			code = -2;
			size = sizeof(Query_Con_User_Response);
			rep_msg = (LPQuery_Con_User_Response)new char[size];
			rep_msg->serialize(size,req_msg->agent,req_msg->from,0,req_msg->Page,0);
		}
		else
		{
			code = 0;
			pages = (count -1 +MAX_COUNT_PAGE)/MAX_COUNT_PAGE;
			size = sizeof(Query_Con_User_Response) + sizeof(Con_UserInfo) * MAX_COUNT_PAGE;
			rep_msg = (LPQuery_Con_User_Response)new char[size];
			userlist = (LPCon_UserInfo)(rep_msg+1);
			count = MAX_COUNT_PAGE;
			con_ptr->GetUserList(userlist,MAX_COUNT_PAGE*req_msg->Page,count);
		}
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);

	//char *pName = 0;
	if(code ==0)
	{
		for(int i=0;i<count;++i)
		{
			/*��ȡ����ְλ��ְλ�ȼ�
			userlist[i].ConLevle = m_ConsortiaManage->GetOfficialLevel(userlist[i].JobID);
			if((pName = m_ConsortiaManage->GetOfficialName(userlist[i].JobID)))
			{
				strcpy(userlist[i].ConJobName,pName);
			}
			else
				strcpy(userlist[i].ConJobName,"��Ա");
			{
			}
			*/
			//��ȡ���ID�����ơ��ȼ������塢ְϵ������ֵ�����Ρ������㡢���׶�
			PlayerOfficerTable offtb;
			offtb.setmasterid(userlist[i].PlayerID);
			offtb.setdetailid(userlist[i].PlayerID);
			if(m_DBProxy.db_select(offtb) == DB_Proxy::DB_SUCCEED)
			{
				strcpy(userlist[i].Name,offtb.officername_);
				userlist[i].Level = offtb.level_;
				userlist[i].OfficerRace = offtb.officerrace_;
				userlist[i].JobID = offtb.jobid_;
				userlist[i].Exp = offtb.exp_;
				userlist[i].MilitaryRank = offtb.militaryrank_;
				userlist[i].Credit = offtb.credit_;
			}
			else
			{
				printf("OnQueryConUserList(PlayerOfficerTable:%d) fautl.\n",userlist[i].PlayerID);
			}
			/*��ȡ�Ƿ�����*/
			int country;
			bool isonline = false;
			int agent = 0;
			m_ConsortiaManage->GetPlayerInfo(userlist[i].PlayerID,0,country,isonline,agent,&m_DBProxy);
			userlist[i].IsOnline = (isonline)?1:0;
			//��ȡ���׶�
		}
		size = sizeof(Query_Con_User_Response) + sizeof(Con_UserInfo) * count;
		rep_msg->serialize(size,req_msg->agent,req_msg->from,count,req_msg->Page,pages);
	}
	asynch_write(rep_msg->agent,(char*)rep_msg,size);
	delete[] (char*)rep_msg;
	return true;
}

/**************************************************************************
*˵����Kickĳ��Ա
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnKickUser(const char *msg)
{
	LPKick_ConUser_Request req_msg = (LPKick_ConUser_Request)msg;
	Kick_ConUser_Response rep_msg;
	int ConsortiaID = 0;
	int code = 0;
	char conName[33];
	//���Ȼ�ȡ������ڵĹ�����
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//δ���빫��
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//�ж��Ƿ�ͬһ����
	int tmpid;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->KickPlayerID,tmpid))
	{//δ���빫��
		rep_msg.serialize(req_msg->agent,req_msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	if(tmpid != ConsortiaID)
	{//������ͬ�⹫��
		rep_msg.serialize(req_msg->agent,req_msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//Ȩ���ж�

	Consortia_Ptr con_ptr;
	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get() ==0)
	{
		code =111;
	}
	else
	{
		if((con_ptr->GetChairmanID() == req_msg->from) ||(PURVIEW_KICK_USER == (con_ptr->GetUserPurview(req_msg->from) & PURVIEW_KICK_USER)))
		{//����Ȩ��
			if(req_msg->KickPlayerID == con_ptr->GetChairmanID())
			{
				code =3;
			}
			else
			{
				m_ConsortiaManage->DelConsortiaUser(ConsortiaID,req_msg->KickPlayerID);
				code = 0;
				//�������ݿ�
				ConsortiaPersonnel deluser;
				deluser.setmasterid(ConsortiaID);
				deluser.setdetailid(req_msg->KickPlayerID);
				if(m_DBProxy.db_delete(deluser) != DB_Proxy::DB_SUCCEED)
				{
					printf("Delete ConsortiaPersonnel fault when the player(%d) kick %d from consortia(%d)\n",
						req_msg->from,req_msg->KickPlayerID,ConsortiaID);
				}
				con_ptr->update2DBG(&m_DBProxy);
			}
		}
		else
		{
			code = 1;
		}
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);

	rep_msg.serialize(req_msg->agent,req_msg->from,code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));

	if(code == 0)
	{
	
#ifdef SND_EVENT_NOTIFY	
		//�����ʼ�֪ͨ����������ϵͳ���͹����ű䶯֪ͨ
		
		Notify_Add_Consortia notify_msg;
		notify_msg.ulPlayerID = req_msg->KickPlayerID;
		notify_msg.ulConsortiaID = 0;
		notify_msg.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&notify_msg,sizeof(notify_msg));

		int country = 0;
		bool isonline = false;
		char PlayerName[33] = {0};
		int agent = req_msg->agent;
		if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,PlayerName,country,isonline,agent,&m_DBProxy))
		{
			strcpy(PlayerName,m_ConsortiaManage->m_NoticeParam[Default_Notice]);
		}

		Notify_Send_Mail email;
		email.type = Normal;
		email.ulRecieverPlayerID = req_msg->KickPlayerID;
		strncpy(email.szSendPlayerName,PlayerName,SIZE_16);
		/*
		strncpy(email.sztitle,"�˳�����֪ͨ",SIZE_16);
		sprintf(email.szbody,"���ѱ���%s��ǿ���˳����᡾%s����",PlayerName,conName);
		*/
		strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[KickUser_Email].Title,SIZE_16);
		sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[KickUser_Email].Body,PlayerName,conName);
		email.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));	

		//���͹���㲥��Ϣ����ʾ��XX�˳������ڹ���
		m_ConsortiaManage->GetPlayerInfo(req_msg->KickPlayerID,PlayerName,country,isonline,agent,&m_DBProxy);
		char sndmsg[128];
		//sprintf(sndmsg,"��%s������%s���߳��������ڹ��ᡣ",PlayerName,email.szSendPlayerName);
		sprintf(sndmsg,m_ConsortiaManage->m_NoticeParam[KickUser_Notice],PlayerName,email.szSendPlayerName);
		SendMsg2Consortia(ConsortiaID,tSystem,sndmsg,req_msg->KickPlayerID);
#endif
	}
	return true;
}

/**************************************************************************
*˵���������˳�����
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnQuitConsortia(const char *msg)
{
	LPQuit_Consortia_Request req_msg = (LPQuit_Consortia_Request)msg;
	Quit_Consortia_Response rep_msg;
	int ConsortiaID = 0;
	int code = 0;
	//���Ȼ�ȡ������ڵĹ�����
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//δ���빫��
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}

	Consortia_Ptr con_ptr;
	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get() ==0)
	{
		code =111;
	}
	else
	{
		if(con_ptr->GetChairmanID() == req_msg->from)
		{
			code = 1;
		}
		else
		{
			m_ConsortiaManage->DelConsortiaUser(ConsortiaID,req_msg->from);
			code = 0;
			//�������ݿ�
			ConsortiaPersonnel deluser;
			deluser.setmasterid(ConsortiaID);
			deluser.setdetailid(req_msg->from);
			if(m_DBProxy.db_delete(deluser) != DB_Proxy::DB_SUCCEED)
			{
				printf("Delete ConsortiaPersonnel fault when the player(%d) quit consortia(%d).\n",req_msg->from,ConsortiaID);
			}
			con_ptr->update2DBG(&m_DBProxy);
		}
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
	rep_msg.serialize(req_msg->agent,req_msg->from,code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	
	if(code == 0)
	{
	
#ifdef SND_EVENT_NOTIFY	
		//������ϵͳ���͹����ű䶯֪ͨ
		Notify_Add_Consortia notify_msg;
		notify_msg.ulPlayerID = req_msg->from;
		notify_msg.ulConsortiaID = 0;
		notify_msg.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&notify_msg,sizeof(notify_msg));

		//���͹���㲥��Ϣ����ʾ��XX�˳������ڹ���
		char PlayerName[33] = {0};
		int country = 0;
		bool isonline = true;
		int agent = req_msg->agent;
		m_ConsortiaManage->GetPlayerInfo(req_msg->from,PlayerName,country,isonline,agent,&m_DBProxy);
		char sndmsg[128];
		//sprintf(sndmsg,"��%s���˳������ڹ��ᡣ",PlayerName);
		sprintf(sndmsg,m_ConsortiaManage->m_NoticeParam[QuitConsortia_Notice],PlayerName);
		SendMsg2Consortia(ConsortiaID,tSystem,sndmsg,req_msg->from);
#endif
	}
	return true;
}

/**************************************************************************
*˵����������빫��
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnInviteJoinConsortia(const char *msg)
{
	LPInvite_JoinCon_Request req_msg = (LPInvite_JoinCon_Request)msg;
	Invite_JoinCon_Response rep_msg;
	int ConsortiaID = 0;
	int country = 0;
	int code = 0;
	char PlayerName[SIZE_32];
	//���Ȼ�ȡ������ڵĹ�����
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//δ���빫��
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//�ж��Ƿ���������Ȩ��GetUserPurview
	if((m_ConsortiaManage->GetUserPurview(req_msg->from) &PURVIEW_INVITE_JOIN) !=PURVIEW_INVITE_JOIN)
	{//û��Ȩ��
		rep_msg.serialize(req_msg->agent,req_msg->from,3);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//��ȡ��������Ϣ
	bool IsOnline = false;
	int agent = req_msg->agent;
	m_ConsortiaManage->GetPlayerInfo(req_msg->from,PlayerName,country,IsOnline,agent,&m_DBProxy);
	//��ȡ��������ID�����жϱ��������Ƿ��Ѽ��빫�ᡢ�Ƿ�ͬһ��Ӫ
	int invite_id = 0;
	int invite_country = 0;
	int invite_conid = 0;
	if((invite_id = m_ConsortiaManage->GetPlayerID(req_msg->InvitedName)) <0)
	{//δ�ҵ����
		rep_msg.serialize(req_msg->agent,req_msg->from,112);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}

	//�жϱ��������Ƿ��Ѽ��빫��
	if(m_ConsortiaManage->HasJoinConsortia(invite_id,invite_conid))
	{//�Ѽ��빫�ᣬ�޷�����
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	agent = 0;
	m_ConsortiaManage->GetPlayerInfo(invite_id,0,invite_country,IsOnline,agent,&m_DBProxy);//��ȡ��Ӫ
	if(country != invite_country)
	{//������ͬ����Ӫ
		rep_msg.serialize(req_msg->agent,req_msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	PlayerOfficerTable officer;
	officer.setmasterid(invite_id);
	officer.setdetailid(invite_id);
	if (m_DBProxy.db_select(officer) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	if (officer.level_ < m_ConsortiaManage->GetJoinConLimit())
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,4);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//
	char conName[33];
	m_ConsortiaManage->ReadLock_Consortia(ConsortiaID);
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get())
	{
		if(con_ptr->GetUserCount() >= con_ptr->getMaxUserNum())
		{
			rep_msg.serialize(req_msg->agent,req_msg->from,8);
			asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
			m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
			return false;
		}
		strncpy(conName,con_ptr->GetName(),32);
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
	rep_msg.serialize(req_msg->agent,req_msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));

#ifdef SND_EVENT_NOTIFY
	//���������ʼ�
	Notify_Send_Mail email;
	email.type = Invite_add_consortia;
	email.ulRecieverPlayerID = invite_id;
	strncpy(email.szSendPlayerName,PlayerName,SIZE_16);
	/*
	strncpy(email.sztitle,"������빫��",SIZE_16);
	sprintf(email.szbody,"��%s�����������빫�᡾%s��,�Ƿ�������룿",PlayerName,conName);
	*/
	strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[InviteEmail].Title,SIZE_16);
	sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[InviteEmail].Body,PlayerName,conName);
	LPEmailBody_Invite pdata = (LPEmailBody_Invite)email.szPertain;
	pdata->PlayerID = req_msg->from;
	pdata->ConsortiaID = ConsortiaID;

	email.serialize();
	event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));	
#endif
	return true;
}

/**************************************************************************
*˵����ͬ������
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnInviteJoinCon_Agree(const char *msg)
{
	LPInvite_JoinCon_Agree_Request req_msg = (LPInvite_JoinCon_Agree_Request)msg;
	Invite_JoinCon_Agree_Response rep_msg;
	//�ж��Ƿ�Ϸ�
	if(req_msg->ConsortiaID <0 || req_msg->ConsortiaID >= MAX_CONSORTIA_COUNT)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//�ж�����Ƿ��Ѽ���ĳ����
	int ConsortiaID = 0;
	if(m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//�Ѽ��빫��
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//�ж���ҵȼ��Ƿ��������
	PlayerOfficerTable playertb;
	playertb.setmasterid(req_msg->from);
	playertb.setdetailid(req_msg->from);
	if(m_DBProxy.db_select(playertb) != DB_Proxy::DB_SUCCEED)
	{//��ѯʧ��
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	if(playertb.level_ < m_ConsortiaManage->GetJoinConLimit())
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//add by xujincai 20090908�ж�����Ƿ����,��������Ӫ
	int iPlayerCountry = 0;
	bool bOnline = false;
	int iAgent = 0;
	if (m_ConsortiaManage->GetPlayerInfo(req_msg->from,0,iPlayerCountry,bOnline,iAgent,&m_DBProxy) !=0)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	ConsortiaID = req_msg->ConsortiaID;
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get() && con_ptr->GetCountryID() == iPlayerCountry)
	{
		if(con_ptr->GetUserCount() >= con_ptr->getMaxUserNum())
		{
			rep_msg.serialize(req_msg->agent,req_msg->from,8);
			asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
			m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
			return false;
		}
		//���ݿ����
		IceUtil::DateTime curDay;
		IceUtil::Time::now().toDateTime(curDay);
		char today[9] = {0};
		sprintf(today,"%d%.2d%.2d",curDay.year,curDay.mon,curDay.day);
		ConsortiaPersonnel usertb;
		usertb.setmasterid(ConsortiaID);
		usertb.setdetailid(req_msg->from);
		usertb.conjob_ = LEAGUER_JOB;
		usertb.purview_ = m_ConsortiaManage->GetOfficialPurview(LEAGUER_JOB);
		usertb.type_ = 0;
		usertb.legiontitle_ = m_ConsortiaManage->getLigionTitleID(0,0);
		strcpy(usertb.salariedday_,today);
		if(m_DBProxy.db_insert(usertb) != DB_Proxy::DB_SUCCEED)
		{
			rep_msg.serialize(req_msg->agent,req_msg->from,111);
		}
		else
		{
			//���빫�����
			ConsortiaUser_Ptr user_ptr(new CConsortiaUser(req_msg->from,ConsortiaID,LEAGUER_JOB,usertb.purview_,0));
			user_ptr->setLegionTitle(usertb.legiontitle_);
			user_ptr->setSalariedDay(today);
			m_ConsortiaManage->AddConsortiaUser(ConsortiaID,user_ptr);
			con_ptr->update2DBG(&m_DBProxy);
		}
		rep_msg.serialize(req_msg->agent,req_msg->from,0);
	}
	else
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);

	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	if (rep_msg.Error_Code)
	{
		return false;
	}

#ifdef SND_EVENT_NOTIFY
	//
	//�����ʼ�֪ͨ�����ˣ�����,��֪ͨ����ϵͳ�޸�ĳ��ҵĹ�����
	
	Notify_Send_Mail email;
	email.type = Normal;
	email.ulRecieverPlayerID = req_msg->InviteID;
	strncpy(email.szSendPlayerName,playertb.officername_,SIZE_16);
	/*
	strncpy(email.sztitle,"�������֪ͨ",SIZE_16);
	sprintf(email.szbody,"��%s�����������ļ��빫������롣",playertb.officername_);
	*/
	strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[AgreeInvite_Email].Title,SIZE_16);
	sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[AgreeInvite_Email].Body,playertb.officername_);
	email.serialize();
	event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));

	Notify_Add_Consortia notify_msg;
	notify_msg.ulPlayerID = req_msg->from;
	notify_msg.ulConsortiaID = ConsortiaID;
	notify_msg.serialize();
	event_notify(MDM_GP_MAP_CHAT,(char*)&notify_msg,sizeof(notify_msg));

	//���͹���㲥��Ϣ����ʾ��XX���������ڹ���
	char sndmsg[128];
	//sprintf(sndmsg,"��%s�����������ڵĹ��ᡣ",playertb.officername_);
	sprintf(sndmsg,m_ConsortiaManage->m_NoticeParam[JoinConsortia_Notice],playertb.officername_);
	SendMsg2Consortia(ConsortiaID,tSystem,sndmsg,playertb.playerid_);
#endif
	return true;
}
/**************************************************************************
*˵�����ܾ�����
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnInviteJoinCon_Refuse(const char *msg)
{
	LPInvite_JoinCon_Refuse_Request req_msg = (LPInvite_JoinCon_Refuse_Request)msg;
	Invite_JoinCon_Refuse_Response rep_msg;
	int code = 111;
	char name[33];
	//���������б�
	int chairmancountry = 0;
	bool IsOnline = false;
	int agent = req_msg->agent;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,name,chairmancountry,IsOnline,agent,&m_DBProxy) == 0)
	{//δ�ҵ�
		code = 0;
	}
	rep_msg.serialize(req_msg->agent,req_msg->from,code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));

	//�������˷���֪ͨ������
	if(code ==0)
	{
#ifdef SND_EVENT_NOTIFY
		Notify_Send_Mail email;
		email.type = Normal;
		email.ulRecieverPlayerID = req_msg->InviteID;
		strncpy(email.szSendPlayerName,name,SIZE_16);
		/*
		strncpy(email.sztitle,"�������֪ͨ",SIZE_16);
		sprintf(email.szbody,"��%s���ܾ������ļ��빫������롣",name);
		*/
		strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[RefuseInvite_Email].Title,SIZE_16);
		sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[RefuseInvite_Email].Body,name);

		email.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));
#endif
	}
	return true;
}
/**************************************************************************
*˵������ְ
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnPoseOfficial(const char *msg)
{
	LPPose_ConOfficial_Request req_msg = (LPPose_ConOfficial_Request)msg;
	Pose_ConOfficial_Response rep_msg;
	int ConsortiaID = 0;
	int code = PoseOfficial(req_msg->from,req_msg->PlayerID,req_msg->ConJob,ConsortiaID);
	
	rep_msg.serialize(req_msg->agent,req_msg->from,code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	/*
	*/
	if(code ==0)
	{
#ifdef SND_EVENT_NOTIFY
		int country = 0;
		bool isonline = false;
		char PlayerName[33] = {0};
		int agent = req_msg->agent;
		if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,PlayerName,country,isonline,agent,&m_DBProxy))
		{
			strcpy(PlayerName,m_ConsortiaManage->m_NoticeParam[Default_Notice]);
		}
		Notify_Send_Mail email;
		email.type = Normal;
		email.ulRecieverPlayerID = req_msg->PlayerID;
		strncpy(email.szSendPlayerName,PlayerName,SIZE_16);

		strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[PoseOfficial_Email].Title,SIZE_16);
		sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[PoseOfficial_Email].Body,
			PlayerName,m_ConsortiaManage->GetOfficialName(req_msg->ConJob));

		email.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));	

		//���͹㲥��Ϣ����ʾ��XX������Ϊ�����ڹ����Ա
		m_ConsortiaManage->GetPlayerInfo(req_msg->PlayerID,PlayerName,country,isonline,agent,&m_DBProxy);
		char sndmsg[128];
		sprintf(sndmsg,m_ConsortiaManage->m_NoticeParam[PoseOfficail_Notice],PlayerName,email.szSendPlayerName,
			m_ConsortiaManage->GetOfficialName(req_msg->ConJob));
		SendMsg2Consortia(ConsortiaID,tSystem,sndmsg,0);
#endif
	}
	return true;
}

/**************************************************************************
*˵������ְ
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnDeposeOfficial(const char *msg)
{
	LPDepose_ConOfficial_Request req_msg = (LPDepose_ConOfficial_Request)msg;
	Depose_ConOfficial_Response rep_msg;
	int ConsortiaID = 0;
	int code = DeposeOfficial(req_msg->from,req_msg->PlayerID,ConsortiaID);
	rep_msg.serialize(req_msg->agent,req_msg->from,code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	/*
	*/
	if(code ==0)
	{
#ifdef SND_EVENT_NOTIFY
		int country = 0;
		bool isonline = false;
		char PlayerName[33] = {0};
		int agent = req_msg->agent;
		if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,PlayerName,country,isonline,agent,&m_DBProxy))
		{
			strcpy(PlayerName,m_ConsortiaManage->m_NoticeParam[Default_Notice]);
		}

		Notify_Send_Mail email;
		email.type = Normal;
		email.ulRecieverPlayerID = req_msg->PlayerID;
		strncpy(email.szSendPlayerName,PlayerName,SIZE_16);
		/*
		strncpy(email.sztitle,"��ְ֪ͨ",SIZE_16);
		sprintf(email.szbody,"���ѱ���%s����ȥ�����ڹ����е�ְλ��",PlayerName);
		*/
		strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[DeposeOfficial_Email].Title,SIZE_16);
		sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[DeposeOfficial_Email].Body,PlayerName);

		email.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));

		//���͹㲥��Ϣ����ʾ��XX�Ĺ���ְ�񱻽��
		m_ConsortiaManage->GetPlayerInfo(req_msg->PlayerID,PlayerName,country,isonline,agent,&m_DBProxy);
		char sndmsg[128];
		//sprintf(sndmsg,"��%s���Ĺ���ְ���ѱ���%s�������",PlayerName,email.szSendPlayerName);
		sprintf(sndmsg,m_ConsortiaManage->m_NoticeParam[DeposeOfficail_Notice],PlayerName,email.szSendPlayerName);
		SendMsg2Consortia(ConsortiaID,tSystem,sndmsg,0);
#endif
	}
	return true;
}

/**************************************************************************
*˵����ְλ��ѯ
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnQueryOfficial(const char *msg)
{
	return true;
}

/**************************************************************************
*˵������ѯ�������������
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnQueryCreateConLimit(const char *msg)
{
	LPQuery_CreateConLimit_Request req_msg = (LPQuery_CreateConLimit_Request)msg;
	Query_CreateConLimit_Response rep_msg;
	rep_msg.LimitInfo.Level = m_ConsortiaManage->GetLevelLimit();
	rep_msg.LimitInfo.Money = m_ConsortiaManage->GetMoneyLimit();
	rep_msg.LimitInfo.JoinLevel = m_ConsortiaManage->GetJoinConLimit();

	rep_msg.serialize(req_msg->agent,req_msg->from,0);	
	asynch_write(req_msg->agent,(char*)&rep_msg,sizeof(rep_msg));
	return true;
}
/**************************************************************************
*˵�����޸���ּ
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnModifyConTenet(const char *msg)
{
	LPModify_ConTenet_Request req_msg = (LPModify_ConTenet_Request)msg;
	Modify_ConTenet_Response rep_msg;
	int	ConsortiaID = 0;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//δ���빫��
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	int code = 0;
	//Ȩ���ж�
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get() ==0)
	{
		code =111;
	}
	else
	{
		//if(PURVIEW_MODIFY_TENT == (con_ptr->GetUserPurview(req_msg->from) & PURVIEW_MODIFY_TENT))
		if(con_ptr->GetChairmanID() == req_msg->from)
		{//����Ȩ��
			con_ptr->SetTenet(req_msg->Tenet);
			con_ptr->update2DBG(&m_DBProxy);

		}
		else
		{
			code = 1;
		}
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
	rep_msg.serialize(req_msg->agent,req_msg->from,code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));

	
	return true;
}
/**************************************************************************
*˵������½
*���������
*����ֵ��true:�ɹ�;false:ʧ��
**************************************************************************/
bool CProcMessage::OnClientLogin(const char *msg)
{
	LPClientLogin_Request req_msg = (LPClientLogin_Request)msg;
	ClientLogin_Response rep_msg;
	int code = 0;
	if(m_ConsortiaManage->SetPlayerOnline(req_msg->from,true,req_msg->agent))
	{
		int country;
		bool isonline = true;
		int agent = req_msg->agent;
		code =m_ConsortiaManage->GetPlayerInfo(req_msg->from,0,country,isonline,agent,&m_DBProxy);
	}
	rep_msg.serialize(req_msg->agent,req_msg->from,code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	return true;
}
/**************************************************************************
*˵�����ƽ��᳤��ÿ������ֻ����һ���᳤��ԭ�᳤������
*���������
*����ֵ��0:�ɹ�;����:ʧ��
**************************************************************************/
bool CProcMessage::OnTranChairman(const char *msg)
{
	LPTrans_Chairman_Request req_msg = (LPTrans_Chairman_Request)msg;
	Trans_Chairman_Response rep_msg;
	int ConsortiaID = 0;//����ID
	int code = 0;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//δ���빫��
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//�ж��Ƿ�ͬ�⹫��
	int tmpConID = 0;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->PlayerID,tmpConID))
	{//δ���빫��
		rep_msg.serialize(req_msg->agent,req_msg->from,3);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	if(tmpConID != ConsortiaID)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,3);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}

	//�ж��Ƿ�Ϊ�᳤
	Consortia_Ptr con_ptr;
	CConsortiaUser *pChairMan  = 0;
	CConsortiaUser *pNextChairMan = 0;
	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get())
	{
		if(req_msg->from != con_ptr->GetChairmanID())
		{//�ǻ᳤
			code = 2;
		}
		else
		{
			//
			pChairMan = con_ptr->getConsortiaUser(req_msg->from);
			pNextChairMan = con_ptr->getConsortiaUser(req_msg->PlayerID);
			if (pChairMan ==0 || pNextChairMan == 0)
			{
				code = 111;
			}
			else
			{
				code = 0;
				//������һ���ų����ݣ���д��DB
				con_ptr->SetChairmanID(req_msg->PlayerID);
				int purview = m_ConsortiaManage->GetOfficialPurview(CHARIRMAN_JOB);
				if (pNextChairMan->GetConsortiaJob() == OFFICIAL_JOB)
				{//���Ϊ��Ա�����Ա���� -1
					con_ptr->addOfficalCount(-1);
				}
				pNextChairMan->SetConsortiaJob(CHARIRMAN_JOB);
				pNextChairMan->SetPurview(purview);
				pNextChairMan->update2DBG(&m_DBProxy);

				//�޸�ԭ���ų����ݣ���д��DB
				if (con_ptr->getOfficalCount() >=5)
				{//��Ա��������Ϊ��ͨ��Ա
					purview = m_ConsortiaManage->GetOfficialPurview(LEAGUER_JOB);
					pChairMan->SetConsortiaJob(LEAGUER_JOB);
					pChairMan->SetPurview(purview);
				}
				else
				{//��Աδ������Ϊ��Ա��
					purview = m_ConsortiaManage->GetOfficialPurview(OFFICIAL_JOB);
					pChairMan->SetConsortiaJob(OFFICIAL_JOB);
					con_ptr->addOfficalCount(1);
					pChairMan->SetPurview(purview);
				}
				pChairMan->update2DBG(&m_DBProxy);

				//������Ϣд��DB
				con_ptr->update2DBG(&m_DBProxy);
			}
		}
	}
	else
	{
		code = 111;
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);

	rep_msg.serialize(req_msg->agent,req_msg->from,code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	if(code ==0)
	{
	
#ifdef SND_EVENT_NOTIFY	
		//�ʼ�֪ͨ
		int country = 0;
		bool isonline = false;
		char PlayerName[33] = {0};
		int agent = req_msg->agent;
		if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,PlayerName,country,isonline,agent,&m_DBProxy))
		{
			strcpy(PlayerName,m_ConsortiaManage->m_NoticeParam[Default_Notice]);
		}

		Notify_Send_Mail email;
		email.type = Normal;
		email.ulRecieverPlayerID = req_msg->PlayerID;
		strncpy(email.szSendPlayerName,PlayerName,SIZE_16);
		/*
		strncpy(email.sztitle,"��ְ֪ͨ",SIZE_16);
		sprintf(email.szbody,"�᳤��%s���Ѱѹ���Ļ᳤ְλת��������",PlayerName);
		*/
		strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[TranChairman_Email].Title,SIZE_16);
		sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[TranChairman_Email].Body,PlayerName);

		email.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));

		//���͹㲥��Ϣ����ʾ��XX������Ϊ�����ڹ���᳤
		agent = 0;
		m_ConsortiaManage->GetPlayerInfo(req_msg->PlayerID,PlayerName,country,isonline,agent,&m_DBProxy);
		char sndmsg[128];
		//sprintf(sndmsg,"��%s���ѹ���᳤��ְλ�ƽ�����%s����",email.szSendPlayerName,PlayerName);
		sprintf(sndmsg,m_ConsortiaManage->m_NoticeParam[TranChairman_Notice],email.szSendPlayerName,PlayerName);
		SendMsg2Consortia(ConsortiaID,tSystem,sndmsg,0);
#endif
	}
	return true;
}
/**************************************************************************
*˵������ȡ��������
*���������
*����ֵ��0:�ɹ�;����:ʧ��
**************************************************************************/
bool CProcMessage::OnQueryPlayerRanking(const char *msg)
{
	LPQuery_Player_Ranking_Request req_msg = (LPQuery_Player_Ranking_Request)msg;
	LPQuery_Player_Ranking_Response rep_msg ;
	LPPlayer_RankingInfo playerlist;

	int count  = MAX_COUNT_PAGE;
	int pages = 0;//��ҳ��
	int size = 0;
	size = sizeof(Query_Player_Ranking_Response) + MAX_COUNT_PAGE * sizeof(Player_RankingInfo);
	rep_msg = (LPQuery_Player_Ranking_Response)new char[size];
	playerlist = (LPPlayer_RankingInfo)(rep_msg+1);

	m_ConsortiaManage->GetPlayerRanking(playerlist,count,MAX_COUNT_PAGE*req_msg->Page,pages,&m_DBProxy);
	size = sizeof(Query_Player_Ranking_Response) + sizeof(Player_RankingInfo) * count;//
	rep_msg->serialize(size,req_msg->agent,req_msg->from,count,req_msg->Page,pages);
	asynch_write(req_msg->agent,(char*)rep_msg,size);
	delete[] (char*)rep_msg;
	return true;
}
/**************************************************************************
*˵������ȡ��������
*���������
*����ֵ��0:�ɹ�;����:ʧ��
**************************************************************************/
bool CProcMessage::OnQueryConsortiaRanking(const char *msg)
{
	LPQuery_Con_Ranking_Request req_msg = (LPQuery_Con_Ranking_Request)msg;
	LPQuery_Con_Ranking_Response rep_msg ;
	LPConsortia_RankingInfo conlist;

	int count  = MAX_COUNT_PAGE;
	int pages = 0;//��ҳ��
	int size = 0;
	size = sizeof(Query_Con_Ranking_Response) + MAX_COUNT_PAGE * sizeof(Consortia_RankingInfo);
	rep_msg = (LPQuery_Con_Ranking_Response)new char[size];
	conlist = (LPConsortia_RankingInfo)(rep_msg+1);

	m_ConsortiaManage->GetConsortiaRanking(conlist,count,MAX_COUNT_PAGE*req_msg->Page,pages);
	size = sizeof(Query_Con_Ranking_Response) + sizeof(Consortia_RankingInfo) * count;//
	rep_msg->serialize(size,req_msg->agent,req_msg->from,count,req_msg->Page,pages);
	asynch_write(req_msg->agent,(char*)rep_msg,size);
	delete[] (char*)rep_msg;
	return true;
}
/**************************************************************************
*˵������ȡ����������������
*���������
*����ֵ��0:�ɹ�;����:ʧ��
**************************************************************************/
bool CProcMessage::OnQueryFriendsRanking(const char *msg)
{
	LPQuery_Player_Ranking_Request req_msg = (LPQuery_Player_Ranking_Request)msg;
	LPQuery_Player_Ranking_Response rep_msg;
	LPPlayer_RankingInfo playerlist;

	int count = MAX_COUNT_PAGE;
	int pages = 0; // ��ҳ��
	int size = 0;
	size = sizeof(Query_Player_Ranking_Response) + MAX_COUNT_PAGE * sizeof(Player_RankingInfo);
	rep_msg = (LPQuery_Player_Ranking_Response)new char[size];
	playerlist = (LPPlayer_RankingInfo)(rep_msg + 1);

	m_ConsortiaManage->GetPlayerRanking(playerlist,count,MAX_COUNT_PAGE*req_msg->Page,pages,&m_DBProxy);
	size = sizeof(Query_Player_Ranking_Response) + sizeof(Player_RankingInfo) * count;
	rep_msg->serialize(size,req_msg->agent,req_msg->from,count,req_msg->Page,pages);
	asynch_write(req_msg->agent,(char*)rep_msg,size);
	delete[] (char*)rep_msg;
	return true;
}
/**************************************************************************
*˵������ȡ����������������
*���������
*����ֵ��0:�ɹ�;����:ʧ��
**************************************************************************/
bool CProcMessage::OnQueryMoneyRanking(const char *msg)
{
	LPQuery_Player_Ranking_Request req_msg = (LPQuery_Player_Ranking_Request)msg;
	LPQuery_Player_Ranking_Response rep_msg;
	LPPlayer_RankingInfo playerlist;

	int count = MAX_COUNT_PAGE;
	int pages = 0;//��ҳ��
	int size = 0;
	size = sizeof(Query_Player_Ranking_Response) + MAX_COUNT_PAGE * sizeof(Player_RankingInfo);
	rep_msg = (LPQuery_Player_Ranking_Response)new char[size];
	playerlist = (LPPlayer_RankingInfo)(rep_msg + 1);

	m_ConsortiaManage->GetMoneyRanking(playerlist,count,MAX_COUNT_PAGE*req_msg->Page,pages,&m_DBProxy);
	size = sizeof(Query_Player_Ranking_Response) + sizeof(Player_RankingInfo) * count;
	rep_msg->serialize(size,req_msg->agent,req_msg->from,count,req_msg->Page,pages);
	asynch_write(req_msg->agent,(char*)rep_msg,size);
	delete[] (char*)rep_msg;
	return true;
}
/**************************************************************************
*˵������ȡ����������������
*���������
*����ֵ��0:�ɹ�;����:ʧ��
**************************************************************************/
bool CProcMessage::OnQueryGeneralRanking(const char *msg)
{
	return true;
}


//��ѯ��ҵĹ������
bool CProcMessage::OnQueryUserInfoOfCountry(const char *msg)
{
	LPQuery_UserOfCountry_Request req_msg = (LPQuery_UserOfCountry_Request)msg;
	//�����ж���ҵ���Ӫ
	int country = 0;
	bool isonline = false;
	int agent = 0;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,0,country,isonline,agent,&m_DBProxy))
	{
		return false;
	}
	m_ConsortiaManage->GetCountry(country)->QueryUserInfoOfCountry(req_msg,&m_DBProxy);
	return true;
}
//��ѯ����Ӫ����Ա�б�
bool CProcMessage::OnQueryCouncilList(const char *msg)
{
	LPQuery_CouncilList_Request req_msg = (LPQuery_CouncilList_Request)msg;
	//�����ж���ҵ���Ӫ
	int country = 0;
	bool isonline = false;
	int agent = 0;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,0,country,isonline,agent,&m_DBProxy))
	{
		return false;
	}
	m_ConsortiaManage->GetCountry(country)->QueryCouncilList(req_msg,&m_DBProxy);
	return true;
}
//�μӾ�ѡ������Ϊ��Ա��
bool CProcMessage::OnTakePartInElection(const char *msg)
{
	LPTakePartIn_Election_Request req_msg = (LPTakePartIn_Election_Request)msg;
	//�����ж���ҵ���Ӫ
	int country = 0;
	bool isonline = false;
	int agent = 0;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,0,country,isonline,agent,&m_DBProxy))
	{
		return false;
	}
	m_ConsortiaManage->GetCountry(country)->TakePartInElection(req_msg,&m_DBProxy);
	return true;
}
//��ѯ��ѡ���б�
bool CProcMessage::OnQueryCandidateList(const char *msg)
{
	LPQuery_CandidateList_Request req_msg = (LPQuery_CandidateList_Request)msg;
	//�����ж���ҵ���Ӫ
	int country = 0;
	bool isonline = false;
	int agent = 0;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,0,country,isonline,agent,&m_DBProxy))
	{
		return false;
	}
	m_ConsortiaManage->GetCountry(country)->QueryCandidateList(req_msg,&m_DBProxy);
	return true;
}
//ͶƱ
bool CProcMessage::OnPlayerBallot(const char *msg)
{
	LPPlayer_Ballot_Request req_msg = (LPPlayer_Ballot_Request)msg;
	//�����ж���ҵ���Ӫ
	int country = 0;
	bool isonline = false;
	int agent = 0;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,0,country,isonline,agent,&m_DBProxy))
	{
		return false;
	}
	m_ConsortiaManage->GetCountry(country)->PlayerBallot(req_msg,&m_DBProxy);
	return true;
}
//����
bool CProcMessage::OnPlayerDelate(const char *msg)
{
	LPCountry_Delate_Request req_msg = (LPCountry_Delate_Request)msg;
	//�����ж���ҵ���Ӫ
	int country = 0;
	bool isonline = false;
	int agent = 0;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,0,country,isonline,agent,&m_DBProxy))
	{
		return false;
	}
	m_ConsortiaManage->GetCountry(country)->PlayerDelate(req_msg,&m_DBProxy);
	return true;
}
//��ѯ������Ϣ
bool CProcMessage::OnQueryDelateInfo(const char *msg)
{
	LPQuery_DelateInfo_Request req_msg = (LPQuery_DelateInfo_Request)msg;
	//�����ж���ҵ���Ӫ
	int country = 0;
	bool isonline = false;
	int agent = 0;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,0,country,isonline,agent,&m_DBProxy))
	{
		return false;
	}
	m_ConsortiaManage->GetCountry(country)->QueryDelateInfo(req_msg,&m_DBProxy);
	return true;
}
//����ͶƱ
bool CProcMessage::OnDelateBallot(const char *msg)
{
	LPDelate_Ballot_Request req_msg = (LPDelate_Ballot_Request)msg;
	//�����ж���ҵ���Ӫ
	int country = 0;
	bool isonline = false;
	int agent = 0;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,0,country,isonline,agent,&m_DBProxy))
	{
		return false;
	}
	m_ConsortiaManage->GetCountry(country)->DelateBallot(req_msg,&m_DBProxy);
	return true;
}
//�޸Ĳ�ѡ����
bool CProcMessage::OnModifyElectionCreed(const char *msg)
{
	LPModify_ElectionCreed_Request req_msg = (LPModify_ElectionCreed_Request)msg;
	//�����ж���ҵ���Ӫ
	int country = 0;
	bool isonline = false;
	int agent = 0;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,0,country,isonline,agent,&m_DBProxy))
	{
		return false;
	}
	m_ConsortiaManage->GetCountry(country)->ModifyElectionCreed(req_msg,&m_DBProxy);
	return true;
}
//������������������
bool CProcMessage::OnCreateTirrier(const char *msg)
{
	LPCreateTirrier_Request req_msg = (LPCreateTirrier_Request)msg;
	//�����ж���ҵ���Ӫ
	int country = 0;
	bool isonline = false;
	int agent = 0;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,0,country,isonline,agent,&m_DBProxy))
	{
		return false;
	}
	m_ConsortiaManage->GetCountry(country)->CreateTirrier(req_msg,&m_DBProxy);
	return true;
}
//����������
bool CProcMessage::OnTirrierGoOut(const char *msg)
{
	LPTirrierGoOut_Request req_msg = (LPTirrierGoOut_Request)msg;
	//�����ж���ҵ���Ӫ
	int country = 0;
	bool isonline = false;
	int agent = 0;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,0,country,isonline,agent,&m_DBProxy))
	{
		return false;
	}
	m_ConsortiaManage->GetCountry(country)->TirrierGoOut(req_msg,&m_DBProxy);
	return true;
}
//������������
bool CProcMessage::OnUpgradeBuild(const char *msg)
{
	LPUpgradeBuild_Request req_msg = (LPUpgradeBuild_Request)msg;
	//�����ж���ҵ���Ӫ
	int country = 0;
	bool isonline = false;
	int agent = 0;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,0,country,isonline,agent,&m_DBProxy))
	{
		return false;
	}
	m_ConsortiaManage->GetCountry(country)->UpgradeBuild(req_msg,&m_DBProxy);
	return true;
}
//�޸���������
bool CProcMessage::OnModifyNotice(const char *msg)
{
	LPModifyNotice_Request req_msg = (LPModifyNotice_Request)msg;
	//�����ж���ҵ���Ӫ
	int country = 0;
	bool isonline = false;
	int agent = 0;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,0,country,isonline,agent,&m_DBProxy))
	{
		return false;
	}
	m_ConsortiaManage->GetCountry(country)->ModifyNotice(req_msg,&m_DBProxy);
	return true;
}
//��ѯ����������
bool CProcMessage::OnQueryTirrierCount(const char *msg)
{
	LPQuery_TirrierCount_Request req_msg = (LPQuery_TirrierCount_Request)msg;
	//�����ж���ҵ���Ӫ
	int country = 0;
	bool isonline = false;
	int agent = 0;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,0,country,isonline,agent,&m_DBProxy))
	{
		return false;
	}
	m_ConsortiaManage->GetCountry(country)->QueryTirrierCount(req_msg,&m_DBProxy);
	return true;
}
//�շѷ��������ص���Ϣ
bool CProcMessage::OnElectionNotify(const char *msg)
{
	LPNotify_Election_Response rep_msg = (LPNotify_Election_Response)msg;
	m_ConsortiaManage->GetCountry(rep_msg->CountryID)->ElectionFeeNotify(rep_msg,&m_DBProxy);
	return true;
}
//֪ͨ����ͼ����������
bool CProcMessage::OnCreateTirrierNotify(const char *msg)
{
	LPNotify_CreateTirrier_Response rep_msg = (LPNotify_CreateTirrier_Response)msg;
	m_ConsortiaManage->GetCountry(rep_msg->CountryID)->CreateTirrierNotify(rep_msg,&m_DBProxy);
	return true;
}
//֪ͨ����ͼ������������
bool CProcMessage::OnUpgradeBuildNotify(const char *msg)
{
	LPNotify_UpgradeBuild_Response rep_msg = (LPNotify_UpgradeBuild_Response)msg;
	m_ConsortiaManage->GetCountry(rep_msg->CountryID)->UpgradeBuildNotify(rep_msg,&m_DBProxy);
	return true;
}
//ѡ�ٽ�����ʱ��
bool CProcMessage::OnVoteEndTimer(const char *msg)
{
	LPConsortia_Timer req_msg = (LPConsortia_Timer)msg;
#ifdef PRINT_HINTINFO
	printf("ѡ�ٽ�����ʱ��:%d\n",req_msg->from);
#endif
	m_ConsortiaManage->GetCountry(req_msg->from)->VoteEndTimer(req_msg,&m_DBProxy);
	return true;
}
//����ǩ������
bool CProcMessage::OnUnderWriteEndTimer(const char *msg)
{
	LPConsortia_Timer req_msg = (LPConsortia_Timer)msg;
	m_ConsortiaManage->GetCountry(req_msg->from)->UnderWriteEndTimer(req_msg,&m_DBProxy);
	return true;
}
//����ͶƱ����
bool CProcMessage::OnDelateVoteEndTimer(const char *msg)
{
	LPConsortia_Timer req_msg = (LPConsortia_Timer)msg;
	m_ConsortiaManage->GetCountry(req_msg->from)->DelateVoteEndTimer(req_msg,&m_DBProxy);
	return true;
}
//������������ʱ��
bool CProcMessage::OnTirrierGoOutTimer(const char *msg)
{
	LPConsortia_Timer req_msg = (LPConsortia_Timer)msg;
	m_ConsortiaManage->GetCountry(req_msg->from)->TirrierGoOutTimer(req_msg,&m_DBProxy);
	return true;
}
//������������
bool CProcMessage::OnModifyTirrierState(const char *msg)
{
	LPNotify_ModifyArmyState req_msg = (LPNotify_ModifyArmyState)msg;
	if(req_msg->from ==100 || req_msg->from ==200)
	{
		m_ConsortiaManage->GetCountry(req_msg->from/100)->ModifyTirrierState(req_msg,&m_DBProxy);
	}
	return true;
}
//�г���˰
bool CProcMessage::OnMarketRate(const char *msg)
{
	LPNotify_MarketRate_Request req_msg = (LPNotify_MarketRate_Request)msg;
	//�����ж���ҵ���Ӫ
	int country = 0;
	bool isonline = false;
	int agent = 0;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,0,country,isonline,agent,&m_DBProxy))
	{
		return false;
	}
	m_ConsortiaManage->GetCountry(country)->MarketRate(req_msg,&m_DBProxy);
	return true;
}
/**************************************************************************
*˵�����������ID����ȡ��һ�����Ϣ����Ӫ��������Agent
*���������
*����ֵ��0:�ɹ�;����:ʧ��
**************************************************************************/
/*
int CProcMessage::GetPlayerInfo(int playerid,char *name,int &country,bool &isonline,int &agent)
{
	if(m_ConsortiaManage->GetPlayerInfo(playerid,name,country,isonline,agent) ==0)
	{
		return 0;
	}
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
	m_ConsortiaManage->AddPlayer(playerid,playertb.officername_,country,isonline,agent);
	return 0;
}
*/
/**************************************************************************
*˵����ְλ����
*���������	player1id�����ID��
			player2id�������������ID
			njobid����ְλ���
*����ֵ��
**************************************************************************/
int CProcMessage::PoseOfficial(int player1id,int player2id,int njobid,int &consortiaid)
{
	int code;
	int ConsortiaID;
	int tmpConID;
	if(player1id == player2id)
	{
		return 3;
	}
	//�����ж�Ȩ��
	if(!m_ConsortiaManage->HasJoinConsortia(player1id,ConsortiaID))
	{//δ���빫��
		return 111;
	}
	if(!m_ConsortiaManage->HasJoinConsortia(player2id,tmpConID))
	{//δ���빫��
		return 111;
	}
	if(tmpConID !=ConsortiaID)
	{
		return 111;
	}
	consortiaid = ConsortiaID;

	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
	Consortia_Ptr con_ptr;
	CConsortiaUser *pUser1 = NULL;
	CConsortiaUser *pUser2 = NULL;
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get())
	{
		if (con_ptr->getOfficalCount() >=5)
		{//��Ա�Ѿ�����5��
			code = 3;
		}
		else if(con_ptr->GetChairmanID() == player2id)
		{//���ų�
			code = 2;
		}
		else 
		{
			pUser1 = con_ptr->getConsortiaUser(player1id);
			pUser2 = con_ptr->getConsortiaUser(player2id);
			if (pUser1 == NULL || pUser2 == NULL)
			{
				code = 2;
			}
			else if (pUser2->GetConsortiaJob() == njobid)
			{//���ǹ�Ա
				code = 2;
			}
			else if((pUser1->GetPurview() & PURVIEW_OFFICIAL) == PURVIEW_OFFICIAL)
			{
				code = 0;
				//�ı��Ա����
				int purview = m_ConsortiaManage->GetOfficialPurview(njobid);
				pUser2->SetConsortiaJob(njobid);
				pUser2->SetPurview(purview);
				//���ӹ�Ա����
				con_ptr->addOfficalCount(1);
				//���»�Ա��Ϣ��DB
				pUser2->update2DBG(&m_DBProxy);
			}
			else
			{
				code = 1;
			}
		}
	}
	else
	{
		code = 111;
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
	return code;
}

//�⹫��ְλ
int CProcMessage::DeposeOfficial(int player1id,int player2id,int &consortiaid)
{
	int code;
	int ConsortiaID;
	int tmpConID;
	if(player1id == player2id)
	{
		return 3;
	}
	//�����ж�Ȩ��
	if(!m_ConsortiaManage->HasJoinConsortia(player1id,ConsortiaID))
	{//δ���빫��
		return 111;
	}
	if(!m_ConsortiaManage->HasJoinConsortia(player2id,tmpConID))
	{//δ���빫��
		return 111;
	}
	if(tmpConID !=ConsortiaID)
	{
		return 111;
	}
	consortiaid = ConsortiaID;

	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
	Consortia_Ptr con_ptr;
	CConsortiaUser *pUser1 = NULL;
	CConsortiaUser *pUser2 = NULL;
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get())
	{
		if(con_ptr->GetChairmanID() == player2id)
		{//���ų����ܰ���
			code = 2;
		}
		else 
		{
			pUser1 = con_ptr->getConsortiaUser(player1id);
			pUser2 = con_ptr->getConsortiaUser(player2id);
			if (pUser1 == NULL || pUser2 == NULL)
			{
				code = 2;
			}
			else if (pUser2->GetConsortiaJob() == LEAGUER_JOB)
			{//������ͨ��Ա
				code = 2;
			}
			else if((pUser1->GetPurview() & PURVIEW_REMOVAL) == PURVIEW_REMOVAL)
			{
				code = 0;
				int purview = m_ConsortiaManage->GetOfficialPurview(LEAGUER_JOB);
				pUser2->SetConsortiaJob(LEAGUER_JOB);
				pUser2->SetPurview(purview);
				con_ptr->addOfficalCount(-1);//���ٹ����Ա����
				pUser2->update2DBG(&m_DBProxy);//���µ�DBG
			}
			else
			{
				code = 1;
			}
		}
	}
	else
	{
		code = 111;
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
	return code;
}
/**************************************************************************
*˵�����·������ڲ���Ϣ
*���������	consortiaid������ID��
			type����Ϣ����
			msg����Ϣ����
*����ֵ��
**************************************************************************/
int CProcMessage::SendMsg2Consortia(int consortiaid,CHAT_TYPE type,const char *msg,int nomsg_playerid/* = 0*/)
{
	CHAT_INFO_STRUCT chat_info;
	Consortia_Ptr con_ptr;
	int playerid = 0;
	int countrye = 0;
	bool isonline = false;
	int agent = 0;
	m_ConsortiaManage->ReadLock_Consortia(consortiaid);
	m_ConsortiaManage->GetConsortia(consortiaid,con_ptr);

	if(con_ptr.get())
	{
		ConsortiaUser_Iter iter;
		chat_info.serialize_Ex(MDM_GP_MAP_CONSORTIA,"ϵͳ",type,(char*)msg,0,RSP_SEND_CHAT_INFO,0);
		for(iter = con_ptr->ConUserList.begin();iter != con_ptr->ConUserList.end();iter++)
		{
			if(m_ConsortiaManage->GetPlayerInfo(iter->second->GetPlayerID(),0,countrye,isonline,agent,&m_DBProxy) !=0)
			{
				continue;
			}
			//add by xujincai 20090303���·�����ұ���
			if(iter->second->GetPlayerID() == nomsg_playerid)
			{
				continue;
			}
			if(isonline)
			{
#ifdef PRINT_HINTINFO
				printf("%d:%s\n",iter->second->GetPlayerID(),msg);
				//chat_info.serialize(iter->second->GetPlayerID(),"ϵͳ",type,(char*)msg,RSP_SEND_CHAT_INFO,agent);
#endif
				//chat_info.serialize_Ex(MDM_GP_MAP_CONSORTIA,"ϵͳ",type,(char*)msg,iter->second->GetPlayerID(),RSP_SEND_CHAT_INFO,agent);
				chat_info.to = iter->second->GetPlayerID();
				chat_info.agent = agent;
				asynch_write(agent,(char*)&chat_info,sizeof(CHAT_INFO_STRUCT));
			}
		}
	}
	m_ConsortiaManage->Unlock_Consortia(consortiaid);
	return 0;
}
/**************************************************************************
*˵�����·��㲥��Ϣ��������ͼ��
*���������	country��������Ӫ��
			msg_type����Ϣ����
			msg����Ϣ����
			snd_type:�������ͣ�0��ȫ�����������˺Ͳ��䣩��1������Ӫ��
*����ֵ��
**************************************************************************/
int CProcMessage::SendMsg2Map(int country,CHAT_TYPE msg_type,const char *msg,int snd_type)
{
	//��ѯ����ͼ��Ϣ
	CHAT_INFO_STRUCT chat_info;
	MapInfoTable maptb;
	MapInfoTable *pRetSet = 0;
	char *retbuf = 0;
	int count = 0;
	if(m_DBProxy.db_select_all(maptb,retbuf,count) != DB_Proxy::DB_SUCCEED)
	{
		if(retbuf)
		{
			delete[] retbuf;
		}
		return 1;
	}
	count = count / sizeof(MapInfoTable);//����
	pRetSet = (MapInfoTable*)retbuf;
	chat_info.serialize_Ex(MDM_GP_MAP_CHAT,"ϵͳ",msg_type,(char*)msg,0,RSP_SEND_CHAT_INFO,0);
	//chat_info.serialize(0,"ϵͳ",msg_type,(char*)msg,RSP_SEND_CHAT_INFO,0);MDM_GP_MAP_CONSORTIA
		
	for(int i=0;i<count;i++)
	{
		if(snd_type ==0)
		{
			if(!broadcast_ex(pRetSet[i].mapid_,(char*)&chat_info,sizeof(CHAT_INFO_STRUCT)))
			{
				printf("broadcast_ex(mapid:%d,country:%d) msg(%s) fault.\n",pRetSet[i].mapid_,pRetSet[i].countryid_,msg);
			}
		}
		else if(pRetSet[i].countryid_ == country)
		{
#ifdef PRINT_HINTINFO
			printf("mapid:%d,country:%d,msg:%s\n",pRetSet[i].mapid_,pRetSet[i].countryid_,msg);
#endif
			if(!broadcast_ex(pRetSet[i].mapid_,(char*)&chat_info,sizeof(CHAT_INFO_STRUCT)))
			{
				printf("broadcast_ex(mapid:%d,country:%d) msg(%s) fault.\n",pRetSet[i].mapid_,pRetSet[i].countryid_,msg);
			}
		}
	}

	if(retbuf)
	{
		delete[] retbuf;
	}
	return 0;
}
/**************************************************************************
*˵������ĳ������·���Ϣ
*���������	playerid�����ID��
			type����Ϣ����
			msg����Ϣ����
*����ֵ��
**************************************************************************/
int  CProcMessage::SendMsg2Player(int playerid,CHAT_TYPE type,const char *msg)
{
	return 0;
}
//��Ȩ
bool CProcMessage::onModifyPurview(const char *msg)
{
	LPtagModify_Purview_Request req_msg = (LPtagModify_Purview_Request)msg;
	Modify_Purview_Response rep_msg;
	//��ȡ������ڵĹ���ID
	int ConsortiaID = 0;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}

	short code = 0;
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get() ==0)
	{
		code =111;
	}
	else
	{
		code = 1;
		if (con_ptr->GetChairmanID() == req_msg->playerID)
		{
			code = 2;
		}
		else
		{
			CConsortiaUser *pUser = con_ptr->getConsortiaUser(req_msg->from);//��ȡ��Ȩ����Ϣ	
			if (pUser && ((pUser->GetPurview() & PURVIEW_AUTHORIZE) == PURVIEW_AUTHORIZE))//�ж�Ȩ��
			{
				pUser = con_ptr->getConsortiaUser(req_msg->playerID);//��ȡ����Ȩ����Ϣ
				if (pUser == NULL)
				{
					code = 2;
				}
				else
				{
					ConsortiaPersonnel conuser;
					conuser.setmasterid(ConsortiaID);
					conuser.setdetailid(req_msg->playerID);
					conuser.purview_ = req_msg->purview - pUser->GetPurview();
					if (m_DBProxy.db_increase(conuser) == DB_Proxy::DB_SUCCEED)
					{
						code = 0;
						pUser->SetPurview(req_msg->purview);
					}
					else
					{
						code = 111;
					}
				}
			}
		}
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
	rep_msg.serialize(req_msg->agent,req_msg->from,code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	return true;
}
//����
void CProcMessage::onUpdateConsortia(const char *msg)
{
	LPUpdateConsortia_Request req_msg = (LPUpdateConsortia_Request)msg;
	UpdateConsortia_Response rep_msg;
	//��ȡ������ڵĹ���ID
	int ConsortiaID = 0;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}

	short code = 0;
	ConsortiaUpdateParam *pUpdateParam = 0;
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get() ==0)
	{
		code =111;
	}
	else
	{
		code = 0;
		CConsortiaUser *pUser = con_ptr->getConsortiaUser(req_msg->from);//��ȡ��������Ϣ
		if (pUser == NULL)
		{
			code = 1;
			goto UPDATEEND;
		}
		if ((pUser->GetPurview() & PURVIEW_UPDATECONSORTIA) != PURVIEW_UPDATECONSORTIA)//�ж�Ȩ��
		{
			code = 1;
			goto UPDATEEND;
		}
		pUpdateParam = m_ConsortiaManage->getConsortiaUpdateParam(con_ptr->GetLevel() + 1);
		if (pUpdateParam == NULL)
		{
			code = 111;
			goto UPDATEEND;
		}
		if (pUpdateParam->needMoney > con_ptr->getMoney())
		{
			code = 4;
			goto UPDATEEND;
		}
		if (pUpdateParam->needExp > con_ptr->getExp())
		{
			code = 5;
			goto UPDATEEND;
		}
		if (pUpdateParam->needCredit > con_ptr->getHonors())
		{
			code = 6;
			goto UPDATEEND;
		}
		if (pUpdateParam->isRoyal && con_ptr->getMapID() == 0)
		{
			code = 7;
			goto UPDATEEND;
		}
		if (pUpdateParam->needGODian >0)
		{//֪ͨ�շ�ϵͳ
			Notify_UpdateConsortia_Request notifyChange;
			notifyChange.serialize(req_msg->agent,req_msg->from,ConsortiaID,pUpdateParam->needGODian);
			event_notify(notifyChange.to,(char*)&notifyChange,notifyChange.length);
		}
		else
		{//ֱ������
			code = updateConsortia(con_ptr.get(),pUpdateParam);
		}
	}
UPDATEEND:
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
	if (code != 0 || pUpdateParam->needGODian <=0)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,code);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	}
}
//�շ�ϵͳ����
void CProcMessage::onNotifyUpdateConsortia(const char *msg)
{
	LPNotify_UpdateConsortia_Response req_msg = (LPNotify_UpdateConsortia_Response)msg;
	UpdateConsortia_Response rep_msg;
	if (req_msg->from != MDM_GP_MAP_CHARGE)
	{
		return;
	}
	//��ȡ������ڵĹ���ID
	int ConsortiaID = req_msg->consortiaID;
	short code = 111;
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get())
	{
		ConsortiaUpdateParam *pUpdateParam = m_ConsortiaManage->getConsortiaUpdateParam(con_ptr->GetLevel() + 1);
		
		if (pUpdateParam)
		{
			code = updateConsortia(con_ptr.get(),pUpdateParam);			
		}
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
	rep_msg.serialize(req_msg->agent,req_msg->playerID,code);
}
//�����ľ������
int CProcMessage::updateConsortia(CConsortia *pConsortia,ConsortiaUpdateParam *pUpdateParam)
{
	ConsortiaInfo consInfo;
	consInfo.setid(pConsortia->GetConsortiaID());
	consInfo.money_ = -pUpdateParam->needMoney;
	consInfo.exp_ = -pUpdateParam->needExp;
	consInfo.honors_ = -pUpdateParam->needCredit;
	consInfo.maxusernum_ = pConsortia->getMaxUserNum() - pUpdateParam->maxUserNum;
	consInfo.level_ = 1;
	if (m_DBProxy.db_increase(consInfo) != DB_Proxy::DB_SUCCEED)
	{
		return 111;
	}
	pConsortia->addMoney(-pUpdateParam->needMoney);
	pConsortia->addExp(-pUpdateParam->needExp);
	pConsortia->addHonors(-pUpdateParam->needCredit);
	pConsortia->setMaxUserNum(pUpdateParam->maxUserNum);
	pConsortia->SetLevel(pUpdateParam->level);

	return 0;
}
//��Ǯ������
void CProcMessage::onContributeMoney(const char *msg)
{
	LPContributeMoney_Request req_msg = (LPContributeMoney_Request)msg;
	ContributeMoney_Response rep_msg;
	if (req_msg->money <=0)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	//��ȡ������ڵĹ���ID
	int ConsortiaID = 0;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	//�жϽ�Ǯ
	PlayerChatRoomTable oldmoney,playertb;
	oldmoney.setid(req_msg->from);
	if (m_DBProxy.db_select(oldmoney) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	if (oldmoney.money_ < req_msg->money)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	short code = 111;
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get())
	{
		memset(&playertb,0,sizeof(playertb));
		playertb.setid(req_msg->from);
		playertb.money_ = -req_msg->money;
		if (m_DBProxy.db_increase(playertb) == DB_Proxy::DB_SUCCEED)
		{
			code = 0;
			CConsortiaUser *pUser = con_ptr->getConsortiaUser(req_msg->from);
			if (pUser)
			{
				pUser->addMoney(req_msg->money);
				ConversionRateParam *pConvParam = m_ConsortiaManage->getConversionRateParam();
				ConsortiaPersonnel conuser;
				conuser.setmasterid(ConsortiaID);
				conuser.setdetailid(req_msg->from);
				conuser.money_ = req_msg->money;
				conuser.contribute_ = req_msg->money/pConvParam->money2ContrRate;
				if (conuser.contribute_ >0)
				{
					pUser->addContribute(conuser.contribute_);
				}

				int titleID = m_ConsortiaManage->getLigionTitleID(pUser->getExp(),pUser->getMoney());
				conuser.legiontitle_ = titleID - pUser->getLegionTitle();
				pUser->setLegionTitle(titleID);
				if (m_DBProxy.db_increase(conuser) == DB_Proxy::DB_SUCCEED)
				{
				}
			}
			//���Ӿ��ŵĽ�Ǯ
			con_ptr->addMoney(req_msg->money);
			ConsortiaInfo cons;
			cons.setid(ConsortiaID);
			cons.money_ = req_msg->money;
			m_DBProxy.db_increase(cons);
		}
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
	rep_msg.serialize(req_msg->agent,req_msg->from,code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	if (code ==0)
	{
		char name[SIZE_24] = {0};
		int country = 0;
		bool isonline = false;
		int agent = 0;
		if (m_ConsortiaManage->GetPlayerInfo(req_msg->from,name,country,isonline,agent,&m_DBProxy)==0)
		{
			ISystemLog::getInstance()->playerMoney_Contribute2Cons_Log(req_msg->from,name,
				oldmoney.money_,playertb.money_,req_msg->money,ConsortiaID,con_ptr->GetName());
		}
	}
}
//��ȡ����
void CProcMessage::onSalaried(const char *msg)
{
	LPalaried_Request req_msg = (LPalaried_Request)msg;
	Salaried_Response rep_msg;
	//��ȡ������ڵĹ���ID
	int ConsortiaID = 0;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,-2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	int code = -1;
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	PlayerChatRoomTable playertb;
	playertb.setid(req_msg->from);
	int iLegionTitle = 0;
	if(con_ptr.get())
	{
		IceUtil::DateTime curDay;
		IceUtil::Time::now().toDateTime(curDay);
		char today[9] = {0};
		sprintf(today,"%d%.2d%.2d",curDay.year,curDay.mon,curDay.day);

		CConsortiaUser *pUser = con_ptr->getConsortiaUser(req_msg->from);
		if (pUser && pUser->isCanSalaried(today))
		{
			iLegionTitle = pUser->getLegionTitle();
			LegionTitleParam *pLigionTitleParam = m_ConsortiaManage->getLegionTitleParam(iLegionTitle);
			if (pLigionTitleParam)
			{
				playertb.money_ = pLigionTitleParam->wage;
				if (m_DBProxy.db_increase(playertb)== DB_Proxy::DB_SUCCEED)
				{
					code = pLigionTitleParam->wage;
					pUser->setSalariedDay(today);
					pUser->update2DBG(&m_DBProxy);
				}
				else
				{
					code = -3;
				}
			}
			else
			{
				code = 0;
			}
		}
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
	rep_msg.serialize(req_msg->agent,req_msg->from,code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	if (code >0)
	{
		char name[SIZE_24] = {0};
		int country = 0;
		bool isonline = false;
		int agent = 0;
		if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,name,country,isonline,agent,&m_DBProxy) == 0)
		{
			ISystemLog::getInstance()->playerMoney_ConsSubsidy_Log(req_msg->from,name,
				playertb.money_ - code,playertb.money_,code,ConsortiaID,iLegionTitle);
		}
	}
}
//��һ�ȡ�˾��������
void CProcMessage::onNotifyPlayerExpOrHonors(const char *msg)
{
	LPNotify_ExpOrCredit_Request req_msg = (LPNotify_ExpOrCredit_Request)msg;
	if (req_msg->from >10000)
	{
		return;
	}
	//��ȡ������ڵĹ���ID
	int ConsortiaID = 0;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->playerID,ConsortiaID))
	{
		return;
	}
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get())
	{
		CConsortiaUser *pUser = con_ptr->getConsortiaUser(req_msg->playerID);
		if (pUser)
		{
			ConversionRateParam *pConversionRateParam = m_ConsortiaManage->getConversionRateParam();
			int iconstribute = 0;
			ConsortiaPersonnel conuser;
			conuser.setmasterid(ConsortiaID);
			conuser.setdetailid(req_msg->playerID);
			conuser.exp_ = req_msg->exp;
			conuser.honors_ = req_msg->credit;
			iconstribute = req_msg->exp / pConversionRateParam->exp2ContrRate + req_msg->credit / pConversionRateParam->credit2ContrRate;
			conuser.contribute_ = iconstribute;
			int titleID = m_ConsortiaManage->getLigionTitleID(pUser->getExp() + req_msg->exp,pUser->getMoney());
			conuser.legiontitle_ = titleID - pUser->getLegionTitle();
			if (m_DBProxy.db_increase(conuser) == DB_Proxy::DB_SUCCEED)
			{
				pUser->setLegionTitle(titleID);
				pUser->addContribute(iconstribute);
				pUser->addCredit(req_msg->credit);
				pUser->addExp(req_msg->exp);

				ConsortiaInfo cons;
				cons.setid(ConsortiaID);
				cons.exp_ = req_msg->exp * pConversionRateParam->expRate/1000;
				cons.honors_ = req_msg->credit * pConversionRateParam->creditRate/1000;
				con_ptr->addHonors(cons.honors_);
				con_ptr->addExp(cons.exp_);
				if (m_DBProxy.db_increase(cons) == DB_Proxy::DB_SUCCEED)
				{
				}
			}
		}
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
}
//�޸ľ��ű�ʶ
void CProcMessage::onModifySignID(const char *msg)
{
	LPModify_SignID_Request req_msg = (LPModify_SignID_Request)msg;
	Modify_SignID_Response rep_msg;
	//��ȡ������ڵĹ���ID
	int ConsortiaID = 0;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	short code = 0;
	Consortia_Ptr con_ptr;
	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get())
	{
		CConsortiaUser *pUser = con_ptr->getConsortiaUser(req_msg->from);
		if (pUser == NULL)
		{
			code = 1;
			goto SIGNIDEND;
		}
		if ((pUser->GetPurview() & PURVIEW_MODIFYSIGNID) != PURVIEW_MODIFYSIGNID)
		{
			code = 1;
			goto SIGNIDEND;
		}
		if (!con_ptr->isRoyalConsortia())
		{
			code = 2;
			goto SIGNIDEND;
		}
		ConsortiaInfo consInfo;
		consInfo.setid(ConsortiaID);
		consInfo.signid_ = req_msg->newSignID - con_ptr->getSignID();
		if (m_DBProxy.db_increase(consInfo) != DB_Proxy::DB_SUCCEED)
		{
			code = 111;
			goto SIGNIDEND;
		}
		code = 0;
		if (consInfo.signid_ != req_msg->newSignID)
		{
			consInfo.signid_ = req_msg->newSignID;
			if (m_DBProxy.db_update(consInfo,consInfo) != DB_Proxy::DB_SUCCEED)
			{
				code = 111;
			}
		}
		if (code == 0)
		{
			con_ptr->setSignID(consInfo.signid_);
		}
	}
	else
	{
		code = 111;
	}
SIGNIDEND:
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);
	rep_msg.serialize(req_msg->agent,req_msg->from,code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
}
