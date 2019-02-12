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
*说明：连接DBG
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::ConnectDBG()
{
	if(!m_ConsortiaManage)
		return false;

	m_DBProxy.fini();
	return m_DBProxy.init(m_ConsortiaManage->m_AgentDBParam.DBGIP,m_ConsortiaManage->m_AgentDBParam.DBGPort,MDM_GP_MAP_CONSORTIA);
}

/**************************************************************************
*说明：业务处理函数
*输入参数：msg:请求消息包
*返回值：
**************************************************************************/
void CProcMessage::ProcMsg(const char *msg)
{
	if(!msg)
		return;

	DISTRIBUTED_HEADER *head = (DISTRIBUTED_HEADER*)msg;
	switch(head->command)
	{
	case CC_C_CREATE_CONSORTIA:				//公会创建
		{
			OnCreateConsortia(msg);
		}
		break;
	case CC_C_QUERY_CONSORTIA:				//公会查询
		{
			OnQueryConsortiaList(msg);
		}
		break;
	case CC_C_DELETE_CONSORTIA:				//公会解散
		{
			OnDeleteConsortial(msg);
		}
		break;
	case CC_C_QUERY_USER_CON:				//查询玩家所属公会
		{
			OnQueryUserConsortiaInfo(msg);
		}
		break;
	case CC_C_MODIFY_PLACARD:				//修改公告
		{
			OnModifyPlacard(msg);
		}
		break;
	case CC_C_QUERY_LOGO:				//查询LOGO
		{
			OnQueryLogo(msg);
		}
		break;
	case CC_C_MODIFY_LOGO:				//修改LOGO
		{
			OnModifyLogo(msg);
		}
		break;
	case CC_C_CONTRIBUTE_BANK:				//捐献资源到公会银行
		{
			OnContributeBank(msg);
		}
		break;
	case CC_C_PAYOUT_BANK:				//支出公会资源
		{
			OnPayoutBank(msg);
		}
		break;
	case CC_C_QUERY_BANK:				//公会银行查询
		{
			OnQueryBank(msg);
		}
		break;
	case CC_C_ALLY_APPLY_CON:				//结盟申请
		{
			OnAllyApply(msg);
		}
		break;
	case CC_C_ALLY_CONSORTIA_AGREE:				//同意结盟
		{
			OnAllyConsortia_Agree(msg);
		}
		break;
	case CC_C_ALLY_CONSORTIA_REFUSE:			//拒绝结盟
		{
			OnAllyConsortia_Refuse(msg);
		}
		break;
	case CC_C_UNALLY_CONSORTIA:				//解盟
		{
			OnUnallyConsortia(msg);
		}
		break;
	case CC_C_FIGHT_APPLY_CON:				//约战申请
		{
			OnFightApply(msg);
		}
		break;
	case CC_C_FIGHT_CONSORTIA_AGREE:				//同意约战
		{
			OnFightConsortia_Agree(msg);
		}
		break;
	case CC_C_FIGHT_CONSORTIA_REFUSE:				//拒绝约战
		{
			OnFightConsortia_Refuse(msg);
		}
		break;
	case CC_C_QUERY_FIGHT:				//约战查询
		{
			OnQueryFight(msg);
		}
		break;
	case CC_C_JOIN_APPLY_CON:				//申请加入公会
		{
			OnJoinApply(msg);
		}
		break;
	case CC_C_JOIN_CONSORTIA_AGREE:				//会长同意加入公会
		{
			OnJoinConsortia_Agree(msg);
		}
		break;
	case CC_C_JOIN_CONSORTIA_REFUSE:				//会长拒绝加入公会
		{
			OnJoinConsortia_Refuse(msg);
		}
		break;
	case CC_C_QUERY_CONUSER:				//公会成员查询
		{
			OnQueryConUserList(msg);
		}
		break;
	case CC_C_KICK_CONUSER:				//Kick成员
		{
			OnKickUser(msg);
		}
		break;
	case CC_C_QUIT_CONSORTIA:				//退出公会
		{
			OnQuitConsortia(msg);
		}
		break;
	case CC_C_INVITE_JOIN_CON:				//邀请加入公会
		{
			OnInviteJoinConsortia(msg);
		}
		break;
	case CC_C_INVITE_JOIN_AGREE:			//同意邀请
		{
			OnInviteJoinCon_Agree(msg);
		}
		break;
	case CC_C_INVITE_JOIN_REFUSE:			//拒绝邀请
		{
			OnInviteJoinCon_Refuse(msg);
		}
		break;
	case CC_C_POSE_OFFICIAL:				//任职
		{
			OnPoseOfficial(msg);
		}
		break;
	case CC_C_DEPOSE_OFFICIAL:				//免职
		{
			OnDeposeOfficial(msg);
		}
		break;
	case CC_C_QUERY_OFFICIAL:				//查询职位
		{
			OnQueryOfficial(msg);
		}
		break;
	case CC_C_QUERY_CREATELIMIT:				//查询创建公会的限制条件
		{
			OnQueryCreateConLimit(msg);
		}
		break;
	case CC_C_MODIFY_CONTENET:				//修改宗旨
		{
			OnModifyConTenet(msg);
		}
		break;
	case CC_C_CLIENTLOGIN:				//登陆
		{
			OnClientLogin(msg);
		}
		break;
	case CC_C_TRANS_CHAIRMAN:				//移交会长职位
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
	case CC_C_CHECKCONSORTIA://定时检测
		{
			m_ConsortiaManage->CheckConsortia();
		}
		break;
	case CC_C_TIMER_VOTECOUNCIL://自动选举出议员
		{
			m_ConsortiaManage->VoteCouncilTimer(msg,&m_DBProxy);
		}
		break;
	case CC_C_QUERY_USEROFCOUNTRY://查询玩家的国家身份
		{
			OnQueryUserInfoOfCountry(msg);
		}
		break;
	case CC_C_QUERY_COUNCILLIST://查询本阵营的议员列表
		{
			 OnQueryCouncilList(msg);
		}
		break;
	case CC_C_TAKEPARTIN_ELECTION://参加竞选（必须为议员）
		{
			OnTakePartInElection(msg);
		}
		break;
	case CC_C_QUERY_CANDIDATELIST://查询候选人列表(国防部、内政部）
		{
			OnQueryCandidateList(msg);
		}
		break;
	case CC_C_PLAYER_BALLOT://玩家进行投票选举
		{
			OnPlayerBallot(msg);				//投票
		}
		break;
	case CC_C_COUNTRY_DELATE://弹劾
		{
			OnPlayerDelate(msg);				//弹劾
		}
		break;
	case CC_C_TIMER_VOTEEND://选举结束
		{
			OnVoteEndTimer(msg);
		}
		break;
	case CC_C_TIMER_UNDERWRITE://弹劾签名
		{
			OnUnderWriteEndTimer(msg);		//弹劾签名结束
		}
		break;
	case CC_C_QUERY_DELATEINFO://查询弹劾信息
		{
			OnQueryDelateInfo(msg);
		}
		break;
	case CC_C_DELATE_BALLOT://弹劾投票
		{
			OnDelateBallot(msg);
		}
		break;
	case CC_C_MODIFY_ELECTIONCREED://修改参选纲领
		{
			OnModifyElectionCreed(msg);
		}
		break;
	case CC_C_CREATE_TIRRIER://国防部长创建国防军
		{
			OnCreateTirrier(msg);
		}
		break;
	case CC_C_TIRRIER_GOOUT://国防军出征
		{
			OnTirrierGoOut(msg);
		}
		break;
	case CC_C_UPGRADEBUILD://升级公共建筑
		{
			OnUpgradeBuild(msg);
		}
		break;
	case CC_C_MODIFYNOTICE://修改实例公告
		{
			OnModifyNotice(msg);
		}
		break;
	case CC_C_QUERY_TIRRIERCOUNT://查询国防军数量
		{
			OnQueryTirrierCount(msg);
		}
		break;
	case CC_S_NOTIFY_CREATETIRRIER://通知郡地图创建国防军
		{
			OnCreateTirrierNotify(msg);
		}
		break;
	case CC_S_NOTIFY_UPGRADEBUILD://通知郡地图升级公共建筑
		{
			OnUpgradeBuildNotify(msg);
		}
		break;
	case CC_S_NOTIFY_MODIFYNOTICE://通知GM修改势力公告
		{
		}
		break;
	case CC_C_NOTIFY_MARKETRATE://市场收税
		{
			OnMarketRate(msg);
		}
		break;
	case CC_C_TIMER_DELATEEND://弹劾结束
		{
			OnDelateVoteEndTimer(msg);		//弹劾投票结束
		}
		break;
	case CC_C_TIMER_TIRRIERGOOUT:		//国防军出征定时器
		{
			OnTirrierGoOutTimer(msg);
		}
		break;
	case CGC_S_NOTIFY_ELECTION://参选扣费
		{
			OnElectionNotify(msg);
		}
		break;
	case EN_C_MODIFYARMYSTATE://国防军被消灭
		{
			OnModifyTirrierState(msg);
		}
		break;
	case CC_C_TIMER_BROADBEVOTE://选举提前通知
		{
			m_ConsortiaManage->BroadBevote(msg,&m_DBProxy);
		}
		break;
	case CC_C_TIMER_RANKING://排行
		{
			m_ConsortiaManage->Ranking(&m_DBProxy);
		}
		break;
	case CC_C_MODIFY_PURVIEW://授权
		{
			onModifyPurview(msg);
		}
		break;
	case CC_C_UPDATECONSORTIA://升级
		{
			onUpdateConsortia(msg);
		}
		break;
	case CC_S_NOTIFY_UPDATECONSORTIA://收费系统返回通知
		{
			onNotifyUpdateConsortia(msg);
		}
		break;
	case CC_C_CONTRIBUTEMONEY://捐钱
		{
			onContributeMoney(msg);
		}
		break;
	case CC_C_SALARIED://领取工资
		{
			onSalaried(msg);
		}
		break;
	case EN_C_NOTIFY_EXPORCREDIT://玩家获取了经验或荣誉
		{
			onNotifyPlayerExpOrHonors(msg);
		}
		break;
	case CC_C_MODIFY_SIGNID://修改军团标识
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
*说明：默认处理
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnDefault(const char *msg)
{
	return true;
}

/**************************************************************************
*说明：创建公会
*输入参数：
*返回值：true:成功;false:失败
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
	{//已加入公会
		rep_msg.serialize(req_msg->agent,req_msg->from,111,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//首先判断是否重名
	if(m_ConsortiaManage->IsExistConsortiaName(req_msg->ConName))
	{//重名
		rep_msg.serialize(req_msg->agent,req_msg->from,1,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//获取等级，判断等级是否符合要求,涉及表PlayerOfficerTable，其中OfficerID=PlayerID时代表玩家
	PlayerOfficerTable userinfo;
	userinfo.setmasterid(req_msg->from);
	userinfo.setdetailid(req_msg->from);
	if(m_DBProxy.db_select(userinfo) !=DB_Proxy::DB_SUCCEED)
	{//查询失败
		rep_msg.serialize(req_msg->agent,req_msg->from,111,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}

	if(userinfo.level_ < m_ConsortiaManage->GetLevelLimit())
	{//级别不符合要求
		rep_msg.serialize(req_msg->agent,req_msg->from,2,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//获取金钱，是否充足
	PlayerChatRoomTable moneyinfo;
	moneyinfo.setid(req_msg->from);
	if(m_DBProxy.db_select(moneyinfo) !=DB_Proxy::DB_SUCCEED)
	{//查询失败
		rep_msg.serialize(req_msg->agent,req_msg->from,111,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	if(moneyinfo.money_ < m_ConsortiaManage->GetMoneyLimit())
	{//资源不足
		rep_msg.serialize(req_msg->agent,req_msg->from,3,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	country = moneyinfo.country_;//玩家阵营
	//创建公会
	int iConID = m_ConsortiaManage->GetUnusedConID();//获取一个公会ID
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
	{//自定义LOGO
		new_con_ptr->SetFaceBuff(req_msg->LogoBuf);
	}
	//操作数据库
	//扣除金钱
	memset(&moneyinfo,0,sizeof(PlayerChatRoomTable));
	moneyinfo.setid(req_msg->from);
	moneyinfo.money_ = 0 - m_ConsortiaManage->GetMoneyLimit();
	if(m_DBProxy.db_increase(moneyinfo) != DB_Proxy::DB_SUCCEED)
	{//失败
		rep_msg.serialize(req_msg->agent,req_msg->from,4,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		m_ConsortiaManage->AddUnusedConID(iConID);//放回ID
		return false;
	}
	//创建公会
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
	{//失败
		rep_msg.serialize(req_msg->agent,req_msg->from,111,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		m_ConsortiaManage->AddUnusedConID(iConID);
		return false;
	}
	//添加公会
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
	m_ConsortiaManage->AddUsedConID(iConID);//添加已用公会ID

	//添加用户表
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
	/*通知聊天系统，创建者所属的公会*/	
	Notify_Add_Consortia notify_msg;
	notify_msg.ulPlayerID = req_msg->from;
	notify_msg.ulConsortiaID = iConID;
	notify_msg.serialize();
	event_notify(MDM_GP_MAP_CHAT,(char*)&notify_msg,sizeof(notify_msg));

	//发送广播消息
	char sndmsg[128];
	//sprintf(sndmsg,"公会【%s】成立。",req_msg->ConName);
	sprintf(sndmsg,m_ConsortiaManage->m_NoticeParam[CreateCons_Notice],req_msg->ConName);
	SendMsg2Map(country,tSystem,sndmsg,1);
#endif

	ISystemLog::getInstance()->playerMoney_CreateConsortia_Log(req_msg->from,userinfo.officername_,
		moneyinfo.money_ + m_ConsortiaManage->GetMoneyLimit(),moneyinfo.money_,m_ConsortiaManage->GetMoneyLimit(),
		iConID,req_msg->ConName);
	return true;
}

/**************************************************************************
*说明：查询公会列表
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnQueryConsortiaList(const char *msg)
{
	LPQuery_Consortial_Request req_msg = (LPQuery_Consortial_Request)msg;
	int count = m_ConsortiaManage->GetConsortCount();
	int pages = (count -1 +MAX_COUNT_PAGE)/MAX_COUNT_PAGE;//总页数
	int size = 0;
	LPQuery_Consortia_Response rep_msg ;
	if(count ==0)
	{//无公会,返回空
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
	//获取玩家所属公会ID
	int ConsortiaID = 0;
	m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID);
	count = MAX_COUNT_PAGE;
	//查询公会列表
	m_ConsortiaManage->GetConsortList(conList,count,req_msg->Page*MAX_COUNT_PAGE,ConsortiaID,&m_DBProxy);
	size = sizeof(Query_Consortia_Response) + sizeof(Consortia_Info) * count;//
	rep_msg->serialize(size,req_msg->agent,req_msg->from,count,req_msg->Page,pages);
	asynch_write(req_msg->agent,(char*)rep_msg,size);
	delete[] (char*)rep_msg;
	return true;
}

/**************************************************************************
*说明：删除公会
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnDeleteConsortial(const char *msg)
{
	/*
	*/
	LPDelete_Consortia_Request req_msg = (LPDelete_Consortia_Request)msg;
	Delete_Consortia_Response rep_msg;
	int ConsortiaID = 0;
	//根据玩家获取所属公会
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//未加入公会
		rep_msg.serialize(req_msg->agent,req_msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	int error_code = 1;

	Consortia_Ptr con_ptr;
	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);//对该公会加锁
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get() ==0)
	{
		error_code = 2;
	}
	else if((con_ptr->GetChairmanID() == req_msg->from))//会长
	{
		m_ConsortiaManage->DelConsortia(ConsortiaID);//列表中删除
		error_code = 0;
	}
	else
	{//可进一步判断权限
		if((con_ptr->GetUserPurview(req_msg->from)& PURVIEW_DISBAND_CON) == PURVIEW_DISBAND_CON)
		{
			m_ConsortiaManage->DelConsortia(ConsortiaID);//列表中删除
			error_code = 0;
		}
	}
	m_ConsortiaManage->Unlock_Consortia(ConsortiaID);//对该公会解锁

	//首先返回操作结果
	rep_msg.serialize(req_msg->agent,req_msg->from,error_code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
	//printf("agent:%d,from:%d,to:%d\n",rep_msg.agent,rep_msg.from,rep_msg.to);

	if(error_code == 0)
	{
		//数据库操作
		//删除公会成员表
		{
			ConsortiaUser_Iter useriter;
			ConsortiaPersonnel deluser;
			for(useriter = con_ptr->ConUserList.begin();useriter != con_ptr->ConUserList.end();useriter++)
			{
				deluser.setmasterid(ConsortiaID);
				deluser.setdetailid(useriter->second->GetPlayerID());
				m_ConsortiaManage->DelConsortiaUser(ConsortiaID,useriter->second->GetPlayerID());//从公会成员列表中删除
				if(m_DBProxy.db_delete(deluser) != DB_Proxy::DB_SUCCEED)
				{
					printf("Delete consortia user fault.\n");
				}
#ifdef SND_EVENT_NOTIFY
				/*向聊天系统发送更改玩家公会编号通知*/
				Notify_Add_Consortia notify_msg;
				notify_msg.ulPlayerID = useriter->second->GetPlayerID();
				notify_msg.ulConsortiaID = 0;
				notify_msg.serialize();
				event_notify(MDM_GP_MAP_CHAT,(char*)&notify_msg,sizeof(notify_msg));
#endif
			}
		}

		//删除公会同盟表
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
		//删除公会约战表
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
		//删除公会银行表
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
		//删除公会表
		{
			ConsortiaInfo delcon;
			delcon.setid(ConsortiaID);
			if(m_DBProxy.db_delete(delcon) != DB_Proxy::DB_SUCCEED)
			{
					printf("Delete consortia fault.\n");
			}
		}
#ifdef SND_EVENT_NOTIFY
		//发送系统广播消息
		int country = 0;
		bool isonline = true;
		int agent;
		char sndmsg[128];
		m_ConsortiaManage->GetPlayerInfo(con_ptr->GetChairmanID(),0,country,isonline,agent,&m_DBProxy);
		//sprintf(sndmsg,"公会【%s】已解散。",con_ptr->GetName());
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
*说明：查询用户所属公会信息
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnQueryUserConsortiaInfo(const char *msg)
{
	LPQuery_User_Con_Request req_msg = (LPQuery_User_Con_Request)msg;
	Query_User_Con_Response rep_msg;
	int ConsortiaID = 0;
	//根据玩家获取所属公会
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//未加入公会
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
*说明：修改公告
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnModifyPlacard(const char *msg)
{
	LPModify_Con_Placard_Request req_msg = (LPModify_Con_Placard_Request)msg;
	Modify_Con_Placard_Response rep_msg;
	int	ConsortiaID = 0;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//未加入公会
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}	
	int code = 0;
	//权限判断
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
		{//具有权限
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
*说明：查询LOGO
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnQueryLogo(const char *msg)
{
	LPQuery_Con_LOGO_Request req_msg = (LPQuery_Con_LOGO_Request)msg;
	Query_Con_LOGO_Response rep_msg;
	if(req_msg->ConsortiaID<0 || req_msg->ConsortiaID >= MAX_CONSORTIA_COUNT)
	{//编号不合法
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
*说明：修改LOGO
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnModifyLogo(const char *msg)
{
	LPModify_Con_LOGO_Request req_msg = (LPModify_Con_LOGO_Request)msg;
	Modify_Con_LOGO_Response rep_msg;
	int ConsortiaID = 0;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//未加入公会
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	int code = 0;
	//权限判断
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
		{//具有权限
			con_ptr->SetFaceID(req_msg->FaceID);
			if(req_msg->FaceID ==0)
			{//自定义
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
*说明：捐献资源到银行
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnContributeBank(const char *msg)
{
	return true;
}

/**************************************************************************
*说明：支配公会银行的资源
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnPayoutBank(const char *msg)
{
	return true;
}

/**************************************************************************
*说明：查询公会银行
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnQueryBank(const char *msg)
{
	LPQuery_Con_Bank_Request req_msg = (LPQuery_Con_Bank_Request)msg;
	LPQuery_Con_Bank_Response rep_msg = 0;
	int count = 0;
	int pages = 0;
	int size = 0;
	int ConsortiaID = 0;
	//获取玩家所在的公会ID
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
*说明：结盟申请
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnAllyApply(const char *msg)
{
	LPAlly_Apply_Con_Request req_msg = (LPAlly_Apply_Con_Request)msg;
	Ally_Apply_Con_Response rep_msg;
	int ConsortiaID = 0;
	char conName[33] = {0};
	int ChairmanID =0;
	char PlayerName[33] = {0};
	//首先合法性检测
	if(req_msg->AllyConsortiaID <0 || req_msg->AllyConsortiaID>=MAX_CONSORTIA_COUNT)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//判断玩家是否为会长
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//同一公会无法结盟
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
	//获取对方会长
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
	//返回结果
	rep_msg.serialize(req_msg->agent,req_msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));

#ifdef SND_EVENT_NOTIFY
	//发送邮件
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
	strncpy(email.sztitle,"公会结盟申请",SIZE_16);
	sprintf(email.szbody,"公会【%s】申请与您的公会进行结盟，是否同意？",conName);
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
*说明：同意结盟
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnAllyConsortia_Agree(const char *msg)
{
	LPAlly_Consortia_Agree_Request req_msg = (LPAlly_Consortia_Agree_Request)msg;
	Ally_Consortia_Agree_Response rep_msg;
	int ConsortiaID = 0;
	int code = 0;
	char conName[33];
	//首先合法性检测
	if(req_msg->ConsortiaID <0 || req_msg->ConsortiaID>=MAX_CONSORTIA_COUNT)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//判断玩家是否为会长
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
	//判断是否
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
			//判断是否已是同盟
			if(con_ptr->IsUnionConsortia(req_msg->ConsortiaID))
			{
				code = 2;
			}
			else
			{
				strncpy(conName,con_ptr->GetName(),SIZE_32);
				pNewUnion = new CConsortiaUnion(0,ConsortiaID,req_msg->ConsortiaID);
				union_ptr = pNewUnion;
				//加入公会
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
	//数据库操作
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
	//发送邮件通知
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
	strncpy(email.sztitle,"结盟申请通知",SIZE_16);
	sprintf(email.szbody,"您申请与公会【%s】结盟的请求已通过。",conName);
	*/
	strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[AgreeAlly_Email].Title,SIZE_16);
	sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[AgreeAlly_Email].Body,conName);
	email.serialize();
	event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));	

	//发送结盟广播
#endif
	return true;
}

/**************************************************************************
*说明：拒绝结盟
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnAllyConsortia_Refuse(const char *msg)
{
	LPAlly_Consortia_Refuse_Request req_msg = (LPAlly_Consortia_Refuse_Request)msg;
	Ally_Consortia_Refuse_Response rep_msg;
	int ConsortiaID = 0;
	int code = 0;
	char conName[33] = {0};
	//判断玩家是否为会长
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
		//邮件通知
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
		strncpy(email.sztitle,"结盟申请通知",SIZE_16);
		sprintf(email.szbody,"您申请与公会【%s】结盟的请求被拒绝。",conName);	
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
*说明：解盟
*输入参数：
*返回值：true:成功;false:失败
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
	//判断玩家是否为会长
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
	//邮件通知
	if(code ==0)
	{
#ifdef SND_EVENT_NOTIFY
		int country;
		bool isonline = true;
		char PlayerName[33] = "系统";
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
		strncpy(email.sztitle,"解盟通知",SIZE_16);
		sprintf(email.szbody,"公会【%s】已与您的公会解除同盟关系。",conName);
		*/
		strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[Unally_Email].Title,SIZE_16);
		sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[Unally_Email].Body,conName);
		email.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));

		//发送结盟广播
#endif
	}
	return true;
}

/**************************************************************************
*说明：约战申请
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnFightApply(const char *msg)
{
	return true;
}

/**************************************************************************
*说明：同意约战
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnFightConsortia_Agree(const char *msg)
{
	return true;
}

/**************************************************************************
*说明：拒绝约战
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnFightConsortia_Refuse(const char *msg)
{
	return true;
}
/**************************************************************************
*说明：约战查询
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnQueryFight(const char *msg)
{
	return true;
}

/**************************************************************************
*说明：申请加入公会
*输入参数：
*返回值：true:成功;false:失败
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
	/*首先判断玩家等级是否符合条件*/
	PlayerOfficerTable playertb;
	playertb.setmasterid(req_msg->from);
	playertb.setdetailid(req_msg->from);
	if(m_DBProxy.db_select(playertb) != DB_Proxy::DB_SUCCEED)
	{//查询失败
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
	//判断阵营是否一致
	//首先获取会长的信息
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
			chairman_id = con_ptr->GetChairmanID();//会长ID
			int agent = 0;
			if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,player_name,player_country,player_Online,agent,&m_DBProxy))
			{//未找到
				rep_msg.serialize(req_msg->agent,req_msg->from,115);
				asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
			}
			else
			{
				if(con_ptr->GetCountryID() != player_country)
				{//不属于同一阵营
					rep_msg.serialize(req_msg->agent,req_msg->from,2);
					asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
				}
				else
				{
					rep_msg.serialize(req_msg->agent,req_msg->from,0);
					asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
					//向会长发送邮件通知（服务端是否保存？）

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
*说明：同意加入公会
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnJoinConsortia_Agree(const char *msg)
{
	LPJoin_Consortia_Agree_Request req_msg = (LPJoin_Consortia_Agree_Request)msg;
	Join_Consortia_Agree_Response rep_msg;

	char conName[33];

	//首先判断玩家是否具有权限
	if((m_ConsortiaManage->GetUserPurview(req_msg->from) &PURVIEW_AGRESS_JOIN) != PURVIEW_AGRESS_JOIN)
	{//没有有权限
		rep_msg.serialize(req_msg->agent,req_msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//获取玩家所在的公会ID
	int ConsortiaID = 0;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//判断玩家是否已加入某公会
	int JoinedConID = 0;
	if(m_ConsortiaManage->HasJoinConsortia(req_msg->PlayerID,JoinedConID))
	{//批准人已加入公会
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//数据库操作
	//add by xujincai 20090908判断玩家是否存在,及级别，阵营
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
		{//加入公会操作	
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
	//发送邮件通知玩家,并通知聊天系统修改某玩家的公会编号
	
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
	strncpy(email.sztitle,"加入公会通知",SIZE_16);
	sprintf(email.szbody,"您已被【%s】批准加入公会【%s】。",PlayerName,conName);
	*/
	strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[AgreeJoin_Email].Title,SIZE_16);
	sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[AgreeJoin_Email].Body,PlayerName,conName);
	email.serialize();
	event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));	

	//发送公会广播消息，提示：XX加入你所在公会
	m_ConsortiaManage->GetPlayerInfo(req_msg->PlayerID,PlayerName,country,isonline,agent,&m_DBProxy);
	char sndmsg[128];
	//sprintf(sndmsg,"【%s】加入你所在的公会。",PlayerName);
	sprintf(sndmsg,m_ConsortiaManage->m_NoticeParam[JoinConsortia_Notice],PlayerName);
	SendMsg2Consortia(ConsortiaID,tSystem,sndmsg,req_msg->PlayerID);
#endif
	return true;
}

/**************************************************************************
*说明：拒绝加入公会
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnJoinConsortia_Refuse(const char *msg)
{
	LPJoin_Consortia_Refuse_Request req_msg = (LPJoin_Consortia_Refuse_Request)msg;
	Join_Consortia_Refuse_Response rep_msg;
	char conName[33];
	//若服务端保存请求列表，则处理列表
	//获取玩家所在的公会ID
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

	//返回结果
	rep_msg.serialize(req_msg->agent,req_msg->from,0);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));

#ifdef SND_EVENT_NOTIFY
	//发送邮件通知，申请被拒绝
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
	strncpy(email.sztitle,"加入公会通知",SIZE_16);
	sprintf(email.szbody,"【%s】拒绝了您加入公会【%s】的申请。",PlayerName,conName);
	*/
	strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[RefuseJoin_Email].Title,SIZE_16);
	sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[RefuseJoin_Email].Body,PlayerName,conName);
	email.serialize();
	event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));	
#endif

	return true;
}
/**************************************************************************
*说明：查询公会成员列表
*输入参数：
*返回值：true:成功;false:失败
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

	//首先获取玩家所在的公会编号
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//未加入公会
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
	{//不存在公会
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
			/*获取公会职位、职位等级
			userlist[i].ConLevle = m_ConsortiaManage->GetOfficialLevel(userlist[i].JobID);
			if((pName = m_ConsortiaManage->GetOfficialName(userlist[i].JobID)))
			{
				strcpy(userlist[i].ConJobName,pName);
			}
			else
				strcpy(userlist[i].ConJobName,"成员");
			{
			}
			*/
			//获取玩家ID、名称、等级、种族、职系、经验值、军衔、荣誉点、贡献度
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
			/*获取是否在线*/
			int country;
			bool isonline = false;
			int agent = 0;
			m_ConsortiaManage->GetPlayerInfo(userlist[i].PlayerID,0,country,isonline,agent,&m_DBProxy);
			userlist[i].IsOnline = (isonline)?1:0;
			//获取贡献度
		}
		size = sizeof(Query_Con_User_Response) + sizeof(Con_UserInfo) * count;
		rep_msg->serialize(size,req_msg->agent,req_msg->from,count,req_msg->Page,pages);
	}
	asynch_write(rep_msg->agent,(char*)rep_msg,size);
	delete[] (char*)rep_msg;
	return true;
}

/**************************************************************************
*说明：Kick某成员
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnKickUser(const char *msg)
{
	LPKick_ConUser_Request req_msg = (LPKick_ConUser_Request)msg;
	Kick_ConUser_Response rep_msg;
	int ConsortiaID = 0;
	int code = 0;
	char conName[33];
	//首先获取玩家所在的公会编号
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//未加入公会
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//判断是否同一公会
	int tmpid;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->KickPlayerID,tmpid))
	{//未加入公会
		rep_msg.serialize(req_msg->agent,req_msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	if(tmpid != ConsortiaID)
	{//不属于同意公会
		rep_msg.serialize(req_msg->agent,req_msg->from,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//权限判断

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
		{//具有权限
			if(req_msg->KickPlayerID == con_ptr->GetChairmanID())
			{
				code =3;
			}
			else
			{
				m_ConsortiaManage->DelConsortiaUser(ConsortiaID,req_msg->KickPlayerID);
				code = 0;
				//操作数据库
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
		//发送邮件通知，及向聊天系统发送公会编号变动通知
		
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
		strncpy(email.sztitle,"退出公会通知",SIZE_16);
		sprintf(email.szbody,"您已被【%s】强制退出公会【%s】。",PlayerName,conName);
		*/
		strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[KickUser_Email].Title,SIZE_16);
		sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[KickUser_Email].Body,PlayerName,conName);
		email.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));	

		//发送公会广播消息，提示：XX退出你所在公会
		m_ConsortiaManage->GetPlayerInfo(req_msg->KickPlayerID,PlayerName,country,isonline,agent,&m_DBProxy);
		char sndmsg[128];
		//sprintf(sndmsg,"【%s】被【%s】踢出了你所在公会。",PlayerName,email.szSendPlayerName);
		sprintf(sndmsg,m_ConsortiaManage->m_NoticeParam[KickUser_Notice],PlayerName,email.szSendPlayerName);
		SendMsg2Consortia(ConsortiaID,tSystem,sndmsg,req_msg->KickPlayerID);
#endif
	}
	return true;
}

/**************************************************************************
*说明：主动退出公会
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnQuitConsortia(const char *msg)
{
	LPQuit_Consortia_Request req_msg = (LPQuit_Consortia_Request)msg;
	Quit_Consortia_Response rep_msg;
	int ConsortiaID = 0;
	int code = 0;
	//首先获取玩家所在的公会编号
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//未加入公会
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
			//操作数据库
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
		//向聊天系统发送公会编号变动通知
		Notify_Add_Consortia notify_msg;
		notify_msg.ulPlayerID = req_msg->from;
		notify_msg.ulConsortiaID = 0;
		notify_msg.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&notify_msg,sizeof(notify_msg));

		//发送公会广播消息，提示：XX退出你所在公会
		char PlayerName[33] = {0};
		int country = 0;
		bool isonline = true;
		int agent = req_msg->agent;
		m_ConsortiaManage->GetPlayerInfo(req_msg->from,PlayerName,country,isonline,agent,&m_DBProxy);
		char sndmsg[128];
		//sprintf(sndmsg,"【%s】退出你所在公会。",PlayerName);
		sprintf(sndmsg,m_ConsortiaManage->m_NoticeParam[QuitConsortia_Notice],PlayerName);
		SendMsg2Consortia(ConsortiaID,tSystem,sndmsg,req_msg->from);
#endif
	}
	return true;
}

/**************************************************************************
*说明：邀请加入公会
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnInviteJoinConsortia(const char *msg)
{
	LPInvite_JoinCon_Request req_msg = (LPInvite_JoinCon_Request)msg;
	Invite_JoinCon_Response rep_msg;
	int ConsortiaID = 0;
	int country = 0;
	int code = 0;
	char PlayerName[SIZE_32];
	//首先获取玩家所在的公会编号
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//未加入公会
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//判断是否具有邀请的权限GetUserPurview
	if((m_ConsortiaManage->GetUserPurview(req_msg->from) &PURVIEW_INVITE_JOIN) !=PURVIEW_INVITE_JOIN)
	{//没有权限
		rep_msg.serialize(req_msg->agent,req_msg->from,3);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//获取邀请人信息
	bool IsOnline = false;
	int agent = req_msg->agent;
	m_ConsortiaManage->GetPlayerInfo(req_msg->from,PlayerName,country,IsOnline,agent,&m_DBProxy);
	//获取被邀请人ID，及判断被邀请人是否已加入公会、是否同一阵营
	int invite_id = 0;
	int invite_country = 0;
	int invite_conid = 0;
	if((invite_id = m_ConsortiaManage->GetPlayerID(req_msg->InvitedName)) <0)
	{//未找到玩家
		rep_msg.serialize(req_msg->agent,req_msg->from,112);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}

	//判断被邀请人是否已加入公会
	if(m_ConsortiaManage->HasJoinConsortia(invite_id,invite_conid))
	{//已加入公会，无法邀请
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	agent = 0;
	m_ConsortiaManage->GetPlayerInfo(invite_id,0,invite_country,IsOnline,agent,&m_DBProxy);//获取阵营
	if(country != invite_country)
	{//不属于同意阵营
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
	//发送邀请邮件
	Notify_Send_Mail email;
	email.type = Invite_add_consortia;
	email.ulRecieverPlayerID = invite_id;
	strncpy(email.szSendPlayerName,PlayerName,SIZE_16);
	/*
	strncpy(email.sztitle,"邀请加入公会",SIZE_16);
	sprintf(email.szbody,"【%s】邀请您加入公会【%s】,是否接受邀请？",PlayerName,conName);
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
*说明：同意邀请
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnInviteJoinCon_Agree(const char *msg)
{
	LPInvite_JoinCon_Agree_Request req_msg = (LPInvite_JoinCon_Agree_Request)msg;
	Invite_JoinCon_Agree_Response rep_msg;
	//判断是否合法
	if(req_msg->ConsortiaID <0 || req_msg->ConsortiaID >= MAX_CONSORTIA_COUNT)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//判断玩家是否已加入某公会
	int ConsortiaID = 0;
	if(m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//已加入公会
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//判断玩家等级是否符合条件
	PlayerOfficerTable playertb;
	playertb.setmasterid(req_msg->from);
	playertb.setdetailid(req_msg->from);
	if(m_DBProxy.db_select(playertb) != DB_Proxy::DB_SUCCEED)
	{//查询失败
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
	//add by xujincai 20090908判断玩家是否存在,及级别，阵营
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
		//数据库操作
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
			//加入公会操作
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
	//发送邮件通知邀请人（？）,并通知聊天系统修改某玩家的公会编号
	
	Notify_Send_Mail email;
	email.type = Normal;
	email.ulRecieverPlayerID = req_msg->InviteID;
	strncpy(email.szSendPlayerName,playertb.officername_,SIZE_16);
	/*
	strncpy(email.sztitle,"邀请加入通知",SIZE_16);
	sprintf(email.szbody,"【%s】接受了您的加入公会的邀请。",playertb.officername_);
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

	//发送公会广播消息，提示：XX加入你所在公会
	char sndmsg[128];
	//sprintf(sndmsg,"【%s】加入你所在的公会。",playertb.officername_);
	sprintf(sndmsg,m_ConsortiaManage->m_NoticeParam[JoinConsortia_Notice],playertb.officername_);
	SendMsg2Consortia(ConsortiaID,tSystem,sndmsg,playertb.playerid_);
#endif
	return true;
}
/**************************************************************************
*说明：拒绝邀请
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnInviteJoinCon_Refuse(const char *msg)
{
	LPInvite_JoinCon_Refuse_Request req_msg = (LPInvite_JoinCon_Refuse_Request)msg;
	Invite_JoinCon_Refuse_Response rep_msg;
	int code = 111;
	char name[33];
	//处理邀请列表
	int chairmancountry = 0;
	bool IsOnline = false;
	int agent = req_msg->agent;
	if(m_ConsortiaManage->GetPlayerInfo(req_msg->from,name,chairmancountry,IsOnline,agent,&m_DBProxy) == 0)
	{//未找到
		code = 0;
	}
	rep_msg.serialize(req_msg->agent,req_msg->from,code);
	asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));

	//向邀请人发送通知（？）
	if(code ==0)
	{
#ifdef SND_EVENT_NOTIFY
		Notify_Send_Mail email;
		email.type = Normal;
		email.ulRecieverPlayerID = req_msg->InviteID;
		strncpy(email.szSendPlayerName,name,SIZE_16);
		/*
		strncpy(email.sztitle,"邀请加入通知",SIZE_16);
		sprintf(email.szbody,"【%s】拒绝了您的加入公会的邀请。",name);
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
*说明：任职
*输入参数：
*返回值：true:成功;false:失败
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

		//发送广播消息，提示：XX被任命为你所在公会官员
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
*说明：免职
*输入参数：
*返回值：true:成功;false:失败
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
		strncpy(email.sztitle,"免职通知",SIZE_16);
		sprintf(email.szbody,"您已被【%s】免去了您在公会中的职位。",PlayerName);
		*/
		strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[DeposeOfficial_Email].Title,SIZE_16);
		sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[DeposeOfficial_Email].Body,PlayerName);

		email.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));

		//发送广播消息，提示：XX的公会职务被解除
		m_ConsortiaManage->GetPlayerInfo(req_msg->PlayerID,PlayerName,country,isonline,agent,&m_DBProxy);
		char sndmsg[128];
		//sprintf(sndmsg,"【%s】的公会职务已被【%s】解除。",PlayerName,email.szSendPlayerName);
		sprintf(sndmsg,m_ConsortiaManage->m_NoticeParam[DeposeOfficail_Notice],PlayerName,email.szSendPlayerName);
		SendMsg2Consortia(ConsortiaID,tSystem,sndmsg,0);
#endif
	}
	return true;
}

/**************************************************************************
*说明：职位查询
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnQueryOfficial(const char *msg)
{
	return true;
}

/**************************************************************************
*说明：查询创建公会的条件
*输入参数：
*返回值：true:成功;false:失败
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
*说明：修改宗旨
*输入参数：
*返回值：true:成功;false:失败
**************************************************************************/
bool CProcMessage::OnModifyConTenet(const char *msg)
{
	LPModify_ConTenet_Request req_msg = (LPModify_ConTenet_Request)msg;
	Modify_ConTenet_Response rep_msg;
	int	ConsortiaID = 0;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//未加入公会
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	int code = 0;
	//权限判断
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
		{//具有权限
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
*说明：登陆
*输入参数：
*返回值：true:成功;false:失败
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
*说明：移交会长，每个公会只能有一个会长；原会长降级。
*输入参数：
*返回值：0:成功;其他:失败
**************************************************************************/
bool CProcMessage::OnTranChairman(const char *msg)
{
	LPTrans_Chairman_Request req_msg = (LPTrans_Chairman_Request)msg;
	Trans_Chairman_Response rep_msg;
	int ConsortiaID = 0;//公会ID
	int code = 0;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{//未加入公会
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return false;
	}
	//判断是否同意公会
	int tmpConID = 0;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->PlayerID,tmpConID))
	{//未加入公会
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

	//判断是否为会长
	Consortia_Ptr con_ptr;
	CConsortiaUser *pChairMan  = 0;
	CConsortiaUser *pNextChairMan = 0;
	m_ConsortiaManage->WriteLock_Consortia(ConsortiaID);
	m_ConsortiaManage->GetConsortia(ConsortiaID,con_ptr);
	if(con_ptr.get())
	{
		if(req_msg->from != con_ptr->GetChairmanID())
		{//非会长
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
				//更新下一军团长数据，并写入DB
				con_ptr->SetChairmanID(req_msg->PlayerID);
				int purview = m_ConsortiaManage->GetOfficialPurview(CHARIRMAN_JOB);
				if (pNextChairMan->GetConsortiaJob() == OFFICIAL_JOB)
				{//如果为官员，则官员数量 -1
					con_ptr->addOfficalCount(-1);
				}
				pNextChairMan->SetConsortiaJob(CHARIRMAN_JOB);
				pNextChairMan->SetPurview(purview);
				pNextChairMan->update2DBG(&m_DBProxy);

				//修改原军团长数据，并写入DB
				if (con_ptr->getOfficalCount() >=5)
				{//官员已满，降为普通会员
					purview = m_ConsortiaManage->GetOfficialPurview(LEAGUER_JOB);
					pChairMan->SetConsortiaJob(LEAGUER_JOB);
					pChairMan->SetPurview(purview);
				}
				else
				{//官员未慢，降为官员。
					purview = m_ConsortiaManage->GetOfficialPurview(OFFICIAL_JOB);
					pChairMan->SetConsortiaJob(OFFICIAL_JOB);
					con_ptr->addOfficalCount(1);
					pChairMan->SetPurview(purview);
				}
				pChairMan->update2DBG(&m_DBProxy);

				//军团信息写入DB
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
		//邮件通知
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
		strncpy(email.sztitle,"任职通知",SIZE_16);
		sprintf(email.szbody,"会长【%s】已把公会的会长职位转交给您。",PlayerName);
		*/
		strncpy(email.sztitle,m_ConsortiaManage->m_EmailParam[TranChairman_Email].Title,SIZE_16);
		sprintf(email.szbody,m_ConsortiaManage->m_EmailParam[TranChairman_Email].Body,PlayerName);

		email.serialize();
		event_notify(MDM_GP_MAP_CHAT,(char*)&email,sizeof(email));

		//发送广播消息，提示：XX被任命为你所在公会会长
		agent = 0;
		m_ConsortiaManage->GetPlayerInfo(req_msg->PlayerID,PlayerName,country,isonline,agent,&m_DBProxy);
		char sndmsg[128];
		//sprintf(sndmsg,"【%s】把公会会长的职位移交给【%s】。",email.szSendPlayerName,PlayerName);
		sprintf(sndmsg,m_ConsortiaManage->m_NoticeParam[TranChairman_Notice],email.szSendPlayerName,PlayerName);
		SendMsg2Consortia(ConsortiaID,tSystem,sndmsg,0);
#endif
	}
	return true;
}
/**************************************************************************
*说明：获取个人排行
*输入参数：
*返回值：0:成功;其他:失败
**************************************************************************/
bool CProcMessage::OnQueryPlayerRanking(const char *msg)
{
	LPQuery_Player_Ranking_Request req_msg = (LPQuery_Player_Ranking_Request)msg;
	LPQuery_Player_Ranking_Response rep_msg ;
	LPPlayer_RankingInfo playerlist;

	int count  = MAX_COUNT_PAGE;
	int pages = 0;//总页数
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
*说明：获取公会排行
*输入参数：
*返回值：0:成功;其他:失败
**************************************************************************/
bool CProcMessage::OnQueryConsortiaRanking(const char *msg)
{
	LPQuery_Con_Ranking_Request req_msg = (LPQuery_Con_Ranking_Request)msg;
	LPQuery_Con_Ranking_Response rep_msg ;
	LPConsortia_RankingInfo conlist;

	int count  = MAX_COUNT_PAGE;
	int pages = 0;//总页数
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
*说明：获取人气型有种男排行
*输入参数：
*返回值：0:成功;其他:失败
**************************************************************************/
bool CProcMessage::OnQueryFriendsRanking(const char *msg)
{
	LPQuery_Player_Ranking_Request req_msg = (LPQuery_Player_Ranking_Request)msg;
	LPQuery_Player_Ranking_Response rep_msg;
	LPPlayer_RankingInfo playerlist;

	int count = MAX_COUNT_PAGE;
	int pages = 0; // 总页数
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
*说明：获取巨商型有种男排行
*输入参数：
*返回值：0:成功;其他:失败
**************************************************************************/
bool CProcMessage::OnQueryMoneyRanking(const char *msg)
{
	LPQuery_Player_Ranking_Request req_msg = (LPQuery_Player_Ranking_Request)msg;
	LPQuery_Player_Ranking_Response rep_msg;
	LPPlayer_RankingInfo playerlist;

	int count = MAX_COUNT_PAGE;
	int pages = 0;//总页数
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
*说明：获取策略性有种男排行
*输入参数：
*返回值：0:成功;其他:失败
**************************************************************************/
bool CProcMessage::OnQueryGeneralRanking(const char *msg)
{
	return true;
}


//查询玩家的国家身份
bool CProcMessage::OnQueryUserInfoOfCountry(const char *msg)
{
	LPQuery_UserOfCountry_Request req_msg = (LPQuery_UserOfCountry_Request)msg;
	//首先判断玩家的阵营
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
//查询本阵营的议员列表
bool CProcMessage::OnQueryCouncilList(const char *msg)
{
	LPQuery_CouncilList_Request req_msg = (LPQuery_CouncilList_Request)msg;
	//首先判断玩家的阵营
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
//参加竞选（必须为议员）
bool CProcMessage::OnTakePartInElection(const char *msg)
{
	LPTakePartIn_Election_Request req_msg = (LPTakePartIn_Election_Request)msg;
	//首先判断玩家的阵营
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
//查询候选人列表
bool CProcMessage::OnQueryCandidateList(const char *msg)
{
	LPQuery_CandidateList_Request req_msg = (LPQuery_CandidateList_Request)msg;
	//首先判断玩家的阵营
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
//投票
bool CProcMessage::OnPlayerBallot(const char *msg)
{
	LPPlayer_Ballot_Request req_msg = (LPPlayer_Ballot_Request)msg;
	//首先判断玩家的阵营
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
//弹劾
bool CProcMessage::OnPlayerDelate(const char *msg)
{
	LPCountry_Delate_Request req_msg = (LPCountry_Delate_Request)msg;
	//首先判断玩家的阵营
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
//查询弹劾信息
bool CProcMessage::OnQueryDelateInfo(const char *msg)
{
	LPQuery_DelateInfo_Request req_msg = (LPQuery_DelateInfo_Request)msg;
	//首先判断玩家的阵营
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
//弹劾投票
bool CProcMessage::OnDelateBallot(const char *msg)
{
	LPDelate_Ballot_Request req_msg = (LPDelate_Ballot_Request)msg;
	//首先判断玩家的阵营
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
//修改参选纲领
bool CProcMessage::OnModifyElectionCreed(const char *msg)
{
	LPModify_ElectionCreed_Request req_msg = (LPModify_ElectionCreed_Request)msg;
	//首先判断玩家的阵营
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
//国防部长创建国防军
bool CProcMessage::OnCreateTirrier(const char *msg)
{
	LPCreateTirrier_Request req_msg = (LPCreateTirrier_Request)msg;
	//首先判断玩家的阵营
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
//国防军出征
bool CProcMessage::OnTirrierGoOut(const char *msg)
{
	LPTirrierGoOut_Request req_msg = (LPTirrierGoOut_Request)msg;
	//首先判断玩家的阵营
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
//升级公共建筑
bool CProcMessage::OnUpgradeBuild(const char *msg)
{
	LPUpgradeBuild_Request req_msg = (LPUpgradeBuild_Request)msg;
	//首先判断玩家的阵营
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
//修改势力公告
bool CProcMessage::OnModifyNotice(const char *msg)
{
	LPModifyNotice_Request req_msg = (LPModifyNotice_Request)msg;
	//首先判断玩家的阵营
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
//查询国防军数量
bool CProcMessage::OnQueryTirrierCount(const char *msg)
{
	LPQuery_TirrierCount_Request req_msg = (LPQuery_TirrierCount_Request)msg;
	//首先判断玩家的阵营
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
//收费服务器返回的消息
bool CProcMessage::OnElectionNotify(const char *msg)
{
	LPNotify_Election_Response rep_msg = (LPNotify_Election_Response)msg;
	m_ConsortiaManage->GetCountry(rep_msg->CountryID)->ElectionFeeNotify(rep_msg,&m_DBProxy);
	return true;
}
//通知郡地图创建国防军
bool CProcMessage::OnCreateTirrierNotify(const char *msg)
{
	LPNotify_CreateTirrier_Response rep_msg = (LPNotify_CreateTirrier_Response)msg;
	m_ConsortiaManage->GetCountry(rep_msg->CountryID)->CreateTirrierNotify(rep_msg,&m_DBProxy);
	return true;
}
//通知郡地图升级公共建筑
bool CProcMessage::OnUpgradeBuildNotify(const char *msg)
{
	LPNotify_UpgradeBuild_Response rep_msg = (LPNotify_UpgradeBuild_Response)msg;
	m_ConsortiaManage->GetCountry(rep_msg->CountryID)->UpgradeBuildNotify(rep_msg,&m_DBProxy);
	return true;
}
//选举结束定时器
bool CProcMessage::OnVoteEndTimer(const char *msg)
{
	LPConsortia_Timer req_msg = (LPConsortia_Timer)msg;
#ifdef PRINT_HINTINFO
	printf("选举结束定时器:%d\n",req_msg->from);
#endif
	m_ConsortiaManage->GetCountry(req_msg->from)->VoteEndTimer(req_msg,&m_DBProxy);
	return true;
}
//弹劾签名结束
bool CProcMessage::OnUnderWriteEndTimer(const char *msg)
{
	LPConsortia_Timer req_msg = (LPConsortia_Timer)msg;
	m_ConsortiaManage->GetCountry(req_msg->from)->UnderWriteEndTimer(req_msg,&m_DBProxy);
	return true;
}
//弹劾投票结束
bool CProcMessage::OnDelateVoteEndTimer(const char *msg)
{
	LPConsortia_Timer req_msg = (LPConsortia_Timer)msg;
	m_ConsortiaManage->GetCountry(req_msg->from)->DelateVoteEndTimer(req_msg,&m_DBProxy);
	return true;
}
//国防军出征定时器
bool CProcMessage::OnTirrierGoOutTimer(const char *msg)
{
	LPConsortia_Timer req_msg = (LPConsortia_Timer)msg;
	m_ConsortiaManage->GetCountry(req_msg->from)->TirrierGoOutTimer(req_msg,&m_DBProxy);
	return true;
}
//国防军被消灭
bool CProcMessage::OnModifyTirrierState(const char *msg)
{
	LPNotify_ModifyArmyState req_msg = (LPNotify_ModifyArmyState)msg;
	if(req_msg->from ==100 || req_msg->from ==200)
	{
		m_ConsortiaManage->GetCountry(req_msg->from/100)->ModifyTirrierState(req_msg,&m_DBProxy);
	}
	return true;
}
//市场收税
bool CProcMessage::OnMarketRate(const char *msg)
{
	LPNotify_MarketRate_Request req_msg = (LPNotify_MarketRate_Request)msg;
	//首先判断玩家的阵营
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
*说明：根据玩家ID，获取玩家基本信息，阵营、姓名、Agent
*输入参数：
*返回值：0:成功;其他:失败
**************************************************************************/
/*
int CProcMessage::GetPlayerInfo(int playerid,char *name,int &country,bool &isonline,int &agent)
{
	if(m_ConsortiaManage->GetPlayerInfo(playerid,name,country,isonline,agent) ==0)
	{
		return 0;
	}
	//获取阵营
	PlayerChatRoomTable roomtb;
	roomtb.setid(playerid);
	if(m_DBProxy.db_select(roomtb) != DB_Proxy::DB_SUCCEED)
	{
		return 1;
	}
	country = roomtb.country_;
	//获取姓名
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
*说明：职位任免
*输入参数：	player1id：玩家ID；
			player2id：被任命的玩家ID
			njobid：新职位编号
*返回值：
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
	//首先判断权限
	if(!m_ConsortiaManage->HasJoinConsortia(player1id,ConsortiaID))
	{//未加入公会
		return 111;
	}
	if(!m_ConsortiaManage->HasJoinConsortia(player2id,tmpConID))
	{//未加入公会
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
		{//官员已经超过5人
			code = 3;
		}
		else if(con_ptr->GetChairmanID() == player2id)
		{//军团长
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
			{//已是官员
				code = 2;
			}
			else if((pUser1->GetPurview() & PURVIEW_OFFICIAL) == PURVIEW_OFFICIAL)
			{
				code = 0;
				//改变会员数据
				int purview = m_ConsortiaManage->GetOfficialPurview(njobid);
				pUser2->SetConsortiaJob(njobid);
				pUser2->SetPurview(purview);
				//增加官员数量
				con_ptr->addOfficalCount(1);
				//更新会员信息到DB
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

//免公会职位
int CProcMessage::DeposeOfficial(int player1id,int player2id,int &consortiaid)
{
	int code;
	int ConsortiaID;
	int tmpConID;
	if(player1id == player2id)
	{
		return 3;
	}
	//首先判断权限
	if(!m_ConsortiaManage->HasJoinConsortia(player1id,ConsortiaID))
	{//未加入公会
		return 111;
	}
	if(!m_ConsortiaManage->HasJoinConsortia(player2id,tmpConID))
	{//未加入公会
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
		{//军团长不能罢免
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
			{//已是普通会员
				code = 2;
			}
			else if((pUser1->GetPurview() & PURVIEW_REMOVAL) == PURVIEW_REMOVAL)
			{
				code = 0;
				int purview = m_ConsortiaManage->GetOfficialPurview(LEAGUER_JOB);
				pUser2->SetConsortiaJob(LEAGUER_JOB);
				pUser2->SetPurview(purview);
				con_ptr->addOfficalCount(-1);//减少公会官员数量
				pUser2->update2DBG(&m_DBProxy);//更新到DBG
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
*说明：下发公会内部消息
*输入参数：	consortiaid：公会ID；
			type：消息类型
			msg：消息内容
*返回值：
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
		chat_info.serialize_Ex(MDM_GP_MAP_CONSORTIA,"系统",type,(char*)msg,0,RSP_SEND_CHAT_INFO,0);
		for(iter = con_ptr->ConUserList.begin();iter != con_ptr->ConUserList.end();iter++)
		{
			if(m_ConsortiaManage->GetPlayerInfo(iter->second->GetPlayerID(),0,countrye,isonline,agent,&m_DBProxy) !=0)
			{
				continue;
			}
			//add by xujincai 20090303不下发给玩家本人
			if(iter->second->GetPlayerID() == nomsg_playerid)
			{
				continue;
			}
			if(isonline)
			{
#ifdef PRINT_HINTINFO
				printf("%d:%s\n",iter->second->GetPlayerID(),msg);
				//chat_info.serialize(iter->second->GetPlayerID(),"系统",type,(char*)msg,RSP_SEND_CHAT_INFO,agent);
#endif
				//chat_info.serialize_Ex(MDM_GP_MAP_CONSORTIA,"系统",type,(char*)msg,iter->second->GetPlayerID(),RSP_SEND_CHAT_INFO,agent);
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
*说明：下发广播消息（按郡地图）
*输入参数：	country：公会阵营；
			msg_type：消息类型
			msg：消息内容
			snd_type:发送类型，0：全服务器（联盟和部落）；1：本阵营；
*返回值：
**************************************************************************/
int CProcMessage::SendMsg2Map(int country,CHAT_TYPE msg_type,const char *msg,int snd_type)
{
	//查询郡地图信息
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
	count = count / sizeof(MapInfoTable);//行数
	pRetSet = (MapInfoTable*)retbuf;
	chat_info.serialize_Ex(MDM_GP_MAP_CHAT,"系统",msg_type,(char*)msg,0,RSP_SEND_CHAT_INFO,0);
	//chat_info.serialize(0,"系统",msg_type,(char*)msg,RSP_SEND_CHAT_INFO,0);MDM_GP_MAP_CONSORTIA
		
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
*说明：向某个玩家下发消息
*输入参数：	playerid：玩家ID；
			type：消息类型
			msg：消息内容
*返回值：
**************************************************************************/
int  CProcMessage::SendMsg2Player(int playerid,CHAT_TYPE type,const char *msg)
{
	return 0;
}
//授权
bool CProcMessage::onModifyPurview(const char *msg)
{
	LPtagModify_Purview_Request req_msg = (LPtagModify_Purview_Request)msg;
	Modify_Purview_Response rep_msg;
	//获取玩家所在的公会ID
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
			CConsortiaUser *pUser = con_ptr->getConsortiaUser(req_msg->from);//获取授权者信息	
			if (pUser && ((pUser->GetPurview() & PURVIEW_AUTHORIZE) == PURVIEW_AUTHORIZE))//判断权限
			{
				pUser = con_ptr->getConsortiaUser(req_msg->playerID);//获取被授权者信息
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
//升级
void CProcMessage::onUpdateConsortia(const char *msg)
{
	LPUpdateConsortia_Request req_msg = (LPUpdateConsortia_Request)msg;
	UpdateConsortia_Response rep_msg;
	//获取玩家所在的公会ID
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
		CConsortiaUser *pUser = con_ptr->getConsortiaUser(req_msg->from);//获取升级者信息
		if (pUser == NULL)
		{
			code = 1;
			goto UPDATEEND;
		}
		if ((pUser->GetPurview() & PURVIEW_UPDATECONSORTIA) != PURVIEW_UPDATECONSORTIA)//判断权限
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
		{//通知收费系统
			Notify_UpdateConsortia_Request notifyChange;
			notifyChange.serialize(req_msg->agent,req_msg->from,ConsortiaID,pUpdateParam->needGODian);
			event_notify(notifyChange.to,(char*)&notifyChange,notifyChange.length);
		}
		else
		{//直接升级
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
//收费系统返回
void CProcMessage::onNotifyUpdateConsortia(const char *msg)
{
	LPNotify_UpdateConsortia_Response req_msg = (LPNotify_UpdateConsortia_Response)msg;
	UpdateConsortia_Response rep_msg;
	if (req_msg->from != MDM_GP_MAP_CHARGE)
	{
		return;
	}
	//获取玩家所在的公会ID
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
//升级的具体操作
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
//捐钱到军团
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
	//获取玩家所在的公会ID
	int ConsortiaID = 0;
	if(!m_ConsortiaManage->HasJoinConsortia(req_msg->from,ConsortiaID))
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,1);
		asynch_write(rep_msg.agent,(char*)&rep_msg,sizeof(rep_msg));
		return;
	}
	//判断金钱
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
			//增加军团的金钱
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
//领取工资
void CProcMessage::onSalaried(const char *msg)
{
	LPalaried_Request req_msg = (LPalaried_Request)msg;
	Salaried_Response rep_msg;
	//获取玩家所在的公会ID
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
//玩家获取了经验或荣誉
void CProcMessage::onNotifyPlayerExpOrHonors(const char *msg)
{
	LPNotify_ExpOrCredit_Request req_msg = (LPNotify_ExpOrCredit_Request)msg;
	if (req_msg->from >10000)
	{
		return;
	}
	//获取玩家所在的公会ID
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
//修改军团标识
void CProcMessage::onModifySignID(const char *msg)
{
	LPModify_SignID_Request req_msg = (LPModify_SignID_Request)msg;
	Modify_SignID_Response rep_msg;
	//获取玩家所在的公会ID
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
