#include "gmThread.h"
#include "gmServerIniFile.h"
#include "commInterface.h"
#include "socketsvc.hpp"
#include "ConsortiaProtocol.h"
#include "MapServer_Protocol.h"
#include "ISystemLog.h"

#pragma warning(disable:4244)
CGmThread::CGmThread()
{

}

CGmThread::~CGmThread()
{

}

bool CGmThread::InitGmThread(const char * addr, short port,const char * dns,const char * user,const char * password)
{
	if (!Proxy.init(addr,port,MDM_GP_GAME_MANAGE_SERVER)) return false;
	if (!ConData(dns,user,password)) return false;
	return true;
}

bool CGmThread::FInitGmThread()
{
	Proxy.fini();
	DisConnData();
	return true;
}

bool CGmThread::initdb(const char * addr,short port)
{
	return ( Proxy.init(addr,port,MDM_GP_GAME_MANAGE_SERVER ));
}

void CGmThread::finitdb()
{
	Proxy.fini();
}

bool CGmThread::ConData(const char * dsn,const char * user,const char * password)
{
	try
	{
		std::string constr;
		constr = "UID=";
		constr += user;
		constr += ";PWD=";
		constr += password;
		constr += ";DSN=";
		constr += dsn;
		db_.rlogon(constr.c_str());
	}
	catch(...)
	{ 
		std::cerr<<"CGmThread::ConData"<<std::endl;
		return false;
	}
	return true;
}

void CGmThread::DisConnData()
{
	db_.logoff();
};

//登录GM
int CGmThread::OnGmLogin(const DISTRIBUTED_HEADER* pNetRead)
{
	tagGmLogin* p = (tagGmLogin*)pNetRead;
	CGmFixData::Instance().InsertUC(p->from,p->agent);

	tagRepGmLogin rep;
	rep.serialize(p->agent,p->from,0,0);

	//禁登录
	if (CGmFixData::Instance().FindLogin(p->from))
	{
		rep.cLogin=1;
	}

	//禁言
	if (CGmFixData::Instance().FindSpeak(p->from))
	{
		rep.cSpeak=1;
	}
	asynch_write(rep.agent,(char*)&rep,rep.length);

	return 1;
}

//登出GM
int CGmThread::OnGmLogout(const DISTRIBUTED_HEADER* pNetRead)
{
	tagGmLogout* p = (tagGmLogout*)pNetRead;
	return CGmFixData::Instance().DeleteUC(p->from)?1:0;
}

//是否禁登录
int CGmThread::OnLoginDisabled(const DISTRIBUTED_HEADER* pNetRead)
{
	tagGmLoginDisabled* p= (tagGmLoginDisabled*)pNetRead;
	tagRepLogin rep;
	if (CGmFixData::Instance().FindLogin(p->from))
	{
		rep.serialize(p->agent,p->from,1);
	}
	else
	{
		rep.serialize(p->agent,p->from,0);
	}
	asynch_write(rep.agent,(char*)&rep,rep.length);
	return 1;
}

//是否禁言
int CGmThread::OnSpeakDisabled(const DISTRIBUTED_HEADER* pNetRead)
{
	tagGmSpeakDisabled * p = (tagGmSpeakDisabled*)pNetRead;
	tagRepSpeak rep;
	if (CGmFixData::Instance().FindSpeak(p->from))
	{
		rep.serialize(p->agent,p->from,1);
	}
	else
	{
		rep.serialize(p->agent,p->from,0);
	}
	asynch_write(rep.agent,(char*)&rep,rep.length);
	return 1;
}

//请求势力公告
int CGmThread::OnGetNoticeForces(const DISTRIBUTED_HEADER * pNetRead)
{
	tagGetNoticeForces * noticeforces = (tagGetNoticeForces*)pNetRead;
	tagRepNoticeForces repnoticeforces;
	
	if (noticeforces->frocesID==ctUnion || noticeforces->frocesID==ctTribal)
	{
		repnoticeforces.serialize(noticeforces->agent,noticeforces->from,\
			CgmServerIniFile::Instance().getNotice(noticeforces->frocesID).c_str());
		asynch_write(repnoticeforces.agent,(char*)&repnoticeforces,repnoticeforces.length);
		return 1;
	}
	repnoticeforces.serialize(noticeforces->agent,noticeforces->from,NULL);
	asynch_write(repnoticeforces.agent,(char*)&repnoticeforces,repnoticeforces.length);
	return 0;
}

//增加主英雄名事件
int CGmThread::OnGmAddMainHero(const DISTRIBUTED_HEADER* pNetRead)
{
	Notify_Add_Main_Hero* pHero = (Notify_Add_Main_Hero*)pNetRead;
	return CGmFixData::Instance().InsertHero(pHero->szOfficerName,pHero->iOfficerID);
}

//解析GM命令
int CGmThread::OnGmCommand(const DISTRIBUTED_HEADER * pNetRead)
{
	tagGMCommand * gmcmd = (tagGMCommand*)pNetRead;

	LOG_TRACE_EX((gmcmd->DataLen>0 || gmcmd->DataLen<GM_DATA_SIZE+1),("packet lenght error\n"));
	gmcmd->cData[gmcmd->DataLen]='\0';
	short ckey=0;
	char retcode=0;

	tagGmUserInfo gminfo;
	if (!CGmFixData::Instance().FindGmUserInfo(gmcmd->from,gminfo)) 
	{
		retcode =2;  //非GM账户
		goto failed;
	}

	char * p=gmcmd->cData;
	char * p1 = strchr(p,' ');
	if (!p1) 
	{
		retcode=5;  //参数格式不正确	
		goto failed;
	}

	char tmp[GM_DATA_SIZE+1];
	strncpy(tmp,p,p1-p);
	tmp[p1-p]='\0';
	p=p1+1;


	ckey = CgmServerIniFile::Instance().FindByCmd(tmp);
	if (ckey==0) 
	{
		retcode=3;  //无效命令字
		goto failed; 
	}

	if (!CGmFixData::Instance().QryPermission(gminfo.permission,ckey))
	{	
		retcode=4;   //无此权限;
		goto failed;
	}
	
	switch (ckey)
	{
	//GM私聊(保留)
	case	 1:
		{
			retcode=7; //该功能保留
			break;
		}
	//添加金钱 科技点()  经验值，荣誉点()
	case	2:
	case	3:
	case	4:
	case	5:
		{
			//get账号名称 ...
			p1 = strchr(p,';');
			if (!p1)
			{
				retcode=5;  //参数格式不正确			
				goto failed;
			}
			strncpy(tmp,p,p1-p);
			tmp[p1-p]='\0';
			p=p1+1;

			int playerid;
			if (!CGmFixData::Instance().FindHero(tmp,playerid)) 
			{
				retcode=8;  //无此首领英雄名			
				goto failed;
			}
			//get数量
			int inums=atoi(p);
			if (inums==0)
			{
				retcode=6; //参数值无效
				goto failed;
			}
			if (ckey==2 || ckey==3)
			{
				return ProcAdd2_3(ckey,gmcmd,playerid,inums);
			}
			else 
			{
				return ProcAdd4_5(ckey,gmcmd,playerid,inums);
			}
		}
	//清除首领英难所有技能
	case	6:
		{
			int playerid;
			if (!CGmFixData::Instance().FindHero(p,playerid)) 
			{
				retcode=8;			
				goto failed;
			}
			return ProcClearSkill(ckey,gmcmd,playerid);
		}
	//添加仓库内指定物品
	case	7:
		{
			//get playerid
			p1 = strchr(p,';');
			if (!p1)
			{
				retcode=5;
				goto failed;
			}

			strncpy(tmp,p,p1-p);
			tmp[p1-p]='\0';
			p=p1+1;

			int playerid;
			if (!CGmFixData::Instance().FindHero(tmp,playerid))
			{
				retcode=8;			
				goto failed;
			}

			p1 = strchr(p,';');
			if (!p1)
			{
				retcode=5;
				goto failed;
			}
			strncpy(tmp,p,p1-p);
			tmp[p1-p]='\0';
			p=p1+1;

			int bsid=atoi(tmp);
			if (bsid==1)
			{
				int mapid,cityid,goodsid,nums;
				if (sscanf(p,"%d;%d;%d;%d",&mapid,&cityid,&goodsid,&nums)!=4) 
				{
					retcode=5;
					goto failed;
				}
				if (goodsid<1 || goodsid >4)
				{
					retcode=6;				
					goto failed;
				}
				return this->ProcAddGoods_1(ckey,gmcmd,playerid,mapid,cityid,goodsid,nums);

			}
			else if (bsid==2)
			{
				int mapid,cityid,goodsid,type,nums;
				if (sscanf(p,"%d;%d;%d;%d;%d",&mapid,&cityid,&goodsid,&type,&nums)!=5) 
				{
					retcode=5;				
					goto failed;
				}
				return this->ProcAddGoods_2(ckey,gmcmd,playerid,mapid,cityid,goodsid,type,nums);
			}
			else 
			{	
				retcode=6;
				goto failed;
			}
		}
	//添加英难背包内指定物品
	case	8:
		{
			//get playerid
			p1 = strchr(p,';');
			if (!p1)
			{
				retcode=5;
				goto failed;
			}
			strncpy(tmp,p,p1-p);
			tmp[p1-p]='\0';
			p=p1+1;

			int playerid;
			if (!CGmFixData::Instance().FindHero(tmp,playerid))
			{
				retcode=8;
				goto failed;
			}

			int goodsid,level,type;
			if (sscanf(p,"%d;%d;%d",&goodsid,&level,&type)<3) 
			{
				retcode=5;	
				goto failed;
			}
			return 	ProcAddBackpackGoods(ckey,gmcmd,playerid,goodsid,level,type);

		}
	//增加指定兵种数量
	case	9:
		{
			//get playerid
			p1 = strchr(p,';');
			if (!p1) 
			{
				retcode=5;
				goto failed;
			}
			strncpy(tmp,p,p1-p);
			tmp[p1-p]='\0';
			p=p1+1;

			int playerid;
			if (!CGmFixData::Instance().FindHero(tmp,playerid))
			{
				retcode=8;			
				goto failed;
			}

			int mapid,cityid,armsid,nums;
			if (sscanf(p,"%d;%d;%d;%d",&mapid,&cityid,&armsid,&nums)!=4)
			{
				retcode=5;
				goto failed;
			}
			if (armsid<1 || armsid >8)
			{
				retcode=6;	
				goto failed;
			}
			return ProcAddArms(ckey,gmcmd,playerid,mapid,cityid,armsid,nums);
		}
	//指定的科研项立即研究完成
	case	10:
		{
			//get playerid
			p1 = strchr(p,';');
			if (!p1)
			{
				retcode=5;
				goto failed;
			}
			strncpy(tmp,p,p1-p);
			tmp[p1-p]='\0';
			p=p1+1;

			int playerid;
			if (!CGmFixData::Instance().FindHero(tmp,playerid))
			{
				retcode=8;
				goto failed;
			}

			int rid,level,type;
			if (sscanf(p,"%d;%d;%d",&rid,&level,&type)!=3)
			{
				retcode=5;
				goto failed;
			}
			return ProcAddResearch(ckey,gmcmd,playerid,rid,level,type);
		}
	//禁言 ,//禁登录
	case	11:
	case	12:
		{
			//get accountname...
			p1 = strchr(p,';');
			if (!p1)
			{
				retcode =5;
				goto failed;
			}
			strncpy(tmp,p,p1-p);
			tmp[p1-p]='\0';
			p=p1+1;
			//by name find id
			int playerid;
			if (!CGmFixData::Instance().FindHero(tmp,playerid))
			{
				retcode=8;
				goto failed;
			}
			
			int ieanbled(-1) ;
			if (strcmp(p,"enabled")==0)
				ieanbled=1;
			else if (strcmp(p,"disabled")==0)
				ieanbled=0;
			if (ieanbled==-1)
			{
				retcode=6;
				goto failed;
			}

			if (ckey==11)
			{
				return ProcSpeakingBanned(ckey,gmcmd,playerid,ieanbled);
			}
			else 
			{
				return ProcLoginBanned(ckey,gmcmd,playerid,ieanbled);
			}
		}
	//GM普通公告（组播）
	case	13:
		{
			//get frocesID...
			p1 = strchr(p,';');
			if (!p1)
			{
				retcode=5;
				goto failed;
			}
			strncpy(tmp,p,p1-p);
			tmp[p1-p]='\0';
			int forcesID = atoi(tmp);
			if (forcesID!=ctUnion && forcesID !=ctTribal)
			{
				retcode=8;
				goto failed;
			}
			p=p1+1;
			return ProcNoticePost(ckey,gmcmd,forcesID,p);
		}
	//GM普通公告（广播）
	case	14:
		{
			return ProcNoticeBroadcast(ckey,gmcmd,p);
		}
	//修改势力公告
	case	15:
		{
			//get frocesID...
			p1 = strchr(p,';');
			if (!p1)
			{
				retcode=5;
				goto failed;
			}
			strncpy(tmp,p,p1-p);
			tmp[p1-p]='\0';
			int forcesID = atoi(tmp);
			if (forcesID!=ctUnion && forcesID !=ctTribal)
			{
				retcode=6;
				goto failed;
			}
			p=p1+1;

			//get cData
			//strcpy(tmp,p);
			return ProcModiNoticeForces(ckey,gmcmd,forcesID,p);
		}
	//GM回显势力公告
	case	16:
		{
			int forcesID = atoi(p);
			if (forcesID!=ctUnion && forcesID !=ctTribal)
			{
				retcode=6;
				goto failed;
			}
			return ProcShowNoticeForces(ckey,gmcmd,forcesID);
		}
	//创建NPC
	case 17:
		{//命令格式：npc 阵营;级别;数量;出发郡;目标郡;次数;间隔秒数
			int iCountryID = 0;
			int iArmyCount = 0;
			int iLevel = 0;
			int iFrmMapID = 0;
			int iTargetMapID = 0;
			int iTimes = 0;
			int iInterval = 0;
			int iParamCount = sscanf(p,"%d;%d;%d;%d;%d;%d;%d",&iCountryID,&iLevel,&iArmyCount,&iFrmMapID,&iTargetMapID,&iTimes,&iInterval);
			if ( (iParamCount!= 5 && iParamCount != 7) || iArmyCount >500 || iTimes>1000)
			{
				retcode=6;	
				goto failed;
			}
			Notify_CreateNPCArmy_Request createNPCMsg;
			createNPCMsg.serialize(gmcmd->agent,gmcmd->from,ckey,iCountryID,iLevel,iArmyCount,iFrmMapID,iTargetMapID,iTimes,iInterval,MDM_GP_GAME_MANAGE_SERVER);
			if (event_notify(SERVER_BOUND_MAP_OTHER,(char*)&createNPCMsg,createNPCMsg.length) <=0)
			{
				retcode=6;	
				goto failed;
			}
			return 0;
		}
		break;
		//删除NPC
	case 18:
		{//命令格式：dnpc 郡地图;阵营
			int iMapID =0;
			int iCountryID = 0;
			if (sscanf(p,"%d;%d",&iMapID,&iCountryID)!=2)
			{
				retcode=6;	
				goto failed;
			}
			Notify_DelNPCArmy_Request delNPCMsg;
			delNPCMsg.serialize(gmcmd->agent,gmcmd->from,ckey,iMapID,iCountryID,MDM_GP_GAME_MANAGE_SERVER);
			if (event_notify(iMapID,(char*)&delNPCMsg,delNPCMsg.length) <=0)
			{
				retcode=6;	
				goto failed;
			}
			return 0;
		}
		break;
	}
	
failed:
	tagRepGM repgm;
	repgm.serialize(gmcmd->agent,gmcmd->from,retcode,gmcmd->cData);
	asynch_write(repgm.agent,(char*)&repgm,repgm.length);
	//流水记录...
	CGmFixData::Instance().InsertCmdRec(db_,gmcmd->from,gmcmd->cData,0,ckey,0);
	return 0;
}

//修改势力公告
int CGmThread::ProcModiNoticeForces(short gmid,const tagGMCommand* gmc, int fid,const char * fdata)
{
	//转给聊天服务器
	MODIFY_NOTICEFORCES_EVENT nfevent;
	nfevent.serialize(ntNoticeForces,fid,fdata);
	event_notify(nfevent.to,(char*)&nfevent,nfevent.length);
	CgmServerIniFile::Instance().WriteNotice(fid,fdata);

	//回复
	tagRepGM repgm;
	repgm.serialize(gmc->agent,gmc->from,1,gmc->cData);
	asynch_write(repgm.agent,(char*)&repgm,repgm.length);
	//流水记录
	CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,1);
	return 1;
}

//修改势力公告(公会)
void CGmThread::ProcModiNoticeForcesEvent(const DISTRIBUTED_HEADER* pNetRead)
{
	LPNotify_ModifyNotice_Request  p=(LPNotify_ModifyNotice_Request)pNetRead;

	//转给聊天服务器
	MODIFY_NOTICEFORCES_EVENT nfevent;
	nfevent.serialize(ntNoticeForces,p->CountryID,p->Notice);
	event_notify(nfevent.to,(char*)&nfevent,nfevent.length);
	CgmServerIniFile::Instance().WriteNotice(p->CountryID,p->Notice);

	//流水记录
	CGmFixData::Instance().InsertCmdRec(db_,0,p->Notice,1,15,1);
}


//回显势力公告
int CGmThread::ProcShowNoticeForces(short gmid,const tagGMCommand* gmc,int fid)
{
	//取公告并回复
	tagRepGM repgm;
	char cData[GM_DATA_SIZE*2+1];
	strcpy(cData,gmc->cData);
	strcat(cData,";");
	strcat(cData,CgmServerIniFile::Instance().getNotice(fid).c_str());
	repgm.serialize(gmc->agent,gmc->from,1,cData);
	asynch_write(repgm.agent,(char*)&repgm,repgm.length);
	
	//流水记录
	CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,1);
	return 1;
}

//GM普通公告（组播）
int CGmThread::ProcNoticePost(short gmid,const tagGMCommand* gmc,int fid,const char * fdata)
{
	//转给聊天服务器
	MODIFY_NOTICEFORCES_EVENT nfevent;
	nfevent.serialize(ntNoticePost,fid,fdata);
	event_notify(nfevent.to,(char*)&nfevent,nfevent.length);

	//回复
	tagRepGM repgm;
	repgm.serialize(gmc->agent,gmc->from,1,gmc->cData);
	asynch_write(repgm.agent,(char*)&repgm,repgm.length);

	//流水记录
	CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,1);
	return 1;

}

//GM普通公告（广播）
int CGmThread::ProcNoticeBroadcast(short gmid,const tagGMCommand* gmc,const char * fdata)
{
	tagNoticeBroadCast nbc;
	nbc.serialize(fdata);
	broadcast((char*)&nbc,nbc.length);

	//流水记录
	CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,1);
	return 1;
}

//禁言
int CGmThread::ProcSpeakingBanned(short gmid,const tagGMCommand* gmc,int playerid,int ienabled)
{
	//ienabled=1 禁言 0解除禁言
	int agentid;
	if (ienabled==1)
	{
		CGmFixData::Instance().InsertSpeak(db_,playerid);
		if (CGmFixData::Instance().FindUC(playerid,agentid))
		{
			tagRepSpeak rep;
			rep.serialize(agentid,playerid,1);
			asynch_write(agentid,(char*)&rep,rep.length);
		}
	}
	else
	{
		CGmFixData::Instance().DelSpeak(db_,playerid);
		if (CGmFixData::Instance().FindUC(playerid,agentid))
		{
			tagRepSpeak rep;
			rep.serialize(agentid,playerid,0);
			asynch_write(agentid,(char*)&rep,rep.length);
		}
	}

	//回复
	tagRepGM repgm;
	repgm.serialize(gmc->agent,gmc->from,1,gmc->cData);
	asynch_write(repgm.agent,(char*)&repgm,repgm.length);

	//流水记录
	CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,1);
	return 1;
}

//禁登录
int CGmThread::ProcLoginBanned(short gmid,const tagGMCommand* gmc,int playerid,int ienabled)
{
	//ienabled=1 禁登录 0解除登录
	int agentid;
	if (ienabled==1)
	{
		CGmFixData::Instance().InsertLogin(db_,playerid);
		if (CGmFixData::Instance().FindUC(playerid,agentid))
		{
			tagRepLogin rep;
			rep.serialize(agentid,playerid,1);
			asynch_write(agentid,(char*)&rep,rep.length);
		}
	}
	else
	{
		CGmFixData::Instance().DelLogin(db_,playerid);
		if (CGmFixData::Instance().FindUC(playerid,agentid))
		{
			tagRepLogin rep;
			rep.serialize(agentid,playerid,0);
			asynch_write(agentid,(char*)&rep,rep.length);
		}
	}

	//回复
	tagRepGM repgm;
	repgm.serialize(gmc->agent,gmc->from,1,gmc->cData);
	asynch_write(repgm.agent,(char*)&repgm,repgm.length);

	//流水记录
	CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,1);
	return 1;
}

//金钱 科技点
int CGmThread::ProcAdd2_3(short gmid,const tagGMCommand* gmc,int playerid,int nums)
{
	tagRepGM repgm;
	PlayerChatRoomTable tb,oldinfo;
	tb.setid(playerid);
	oldinfo.setid(playerid);
	Proxy.db_select(oldinfo);
	
	if (gmid==2)
		tb.money_=nums;
	else
		tb.science_=nums;
		
	if (Proxy.db_increase(tb)!=DB_Proxy::DB_SUCCEED)
	{
		repgm.serialize(gmc->agent,gmc->from,0,gmc->cData);
		asynch_write(repgm.agent,(char*)&repgm,repgm.length);
		CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,0);
		return 0;
	}
	repgm.serialize(gmc->agent,gmc->from,1,gmc->cData);
	asynch_write(repgm.agent,(char*)&repgm,repgm.length);
	CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,1);

	PlayerOfficerTable officer;
	officer.setmasterid(playerid);
	officer.setdetailid(playerid);
	Proxy.db_select(officer);
	if (gmid==2)
	{
		ISystemLog::getInstance()->playerMoney_GM_Log(officer.playerid_,officer.officername_,
			oldinfo.money_,tb.money_,nums);
	}
	else
	{
		ISystemLog::getInstance()->playerScience_GM_Log(officer.playerid_,officer.officername_,
			oldinfo.science_,tb.science_,nums);
	}
	return 1;
}

//经验值 荣誉点
int CGmThread::ProcAdd4_5(short gmid,const tagGMCommand* gmc,int playerid,int nums)
{
	tagRepGM repgm;

	tagHeroParam  HeroParam;
	memset(&HeroParam,0,sizeof(HeroParam));
	HeroParam.lPlayerID = playerid;
	HeroParam.lHeroID=playerid;
	if (gmid==4)
		HeroParam.iExp  = nums;
	else
		HeroParam.iCredit = nums;

	if (!OnHeroGrowUp(Proxy,HeroParam,GM_METHOD))
	{
		repgm.serialize(gmc->agent,gmc->from,0,gmc->cData);
		asynch_write(repgm.agent,(char*)&repgm,repgm.length);
		CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,0);
		return 0;
	}
	repgm.serialize(gmc->agent,gmc->from,1,gmc->cData);
	asynch_write(repgm.agent,(char*)&repgm,repgm.length);
	CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,1);
	return 1;
}

//清除首领英难所有技能
int CGmThread::ProcClearSkill(short gmid,const tagGMCommand* gmc,int playerid)
{
	tagRepGM repgm;

	PlayerOfficerTable tb;
	tb.setmasterid(playerid);
	tb.setdetailid(playerid);

	bool bsucced(false);
	if (Proxy.db_select(tb)==DB_Proxy::DB_SUCCEED)
	{
		bsucced = (clearAllOfficerSkill(playerid,playerid)>=0)?true:false;
	}
	
	if (!bsucced)
	{
		repgm.serialize(gmc->agent,gmc->from,0,gmc->cData);
		asynch_write(repgm.agent,(char*)&repgm,repgm.length);
		CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,0);
		return 0;
	}

	repgm.serialize(gmc->agent,gmc->from,1,gmc->cData);
	asynch_write(repgm.agent,(char*)&repgm,repgm.length);
	CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,1);
	return 1;
}


//清楚英雄所学技能
int CGmThread::clearAllOfficerSkill(int playerid,int officerid)
{
	PlayerSkillTable officerSkill;
	officerSkill.setmasterid(playerid,officerid);
	char *skillBuf = 0;
	int skillCount = 0;
	if (Proxy.db_multi_select(officerSkill,skillBuf,skillCount) != DB_Proxy::DB_SUCCEED)
	{
		return -1;
	}
	skillCount = skillCount/sizeof(PlayerSkillTable);
	PlayerSkillTable *skillList = (PlayerSkillTable*)skillBuf;
	for (int i=0;i<skillCount;i++)
	{
		if (Proxy.db_delete(skillList[i]) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE_EX(false,("clearAllOfficerSkill-->删除玩家%d的英雄%d已学技能%d失败。\n",playerid,officerid,skillList[i].skillid_));
		}
	}
	if (skillBuf)
	{
		delete[] skillBuf;
		skillBuf = 0;
	}
	return skillCount;
}
//添加英难背包内指定物品
int CGmThread::ProcAddBackpackGoods(short gmid,const tagGMCommand* gmc,int playerid,int goodsid,int level,int type)
{
	tagRepGM repgm;
	PackStoreParam *pPackStoreParam = CGmFixData::Instance().GetPackStoreParam(type);
	bool bOverlap = false;
	if(pPackStoreParam )
	{
		bOverlap = pPackStoreParam->IsOverlap;
	}
	PlayerItemStorageTable iteminfo;
	if (!OnInsertHeroEquip(Proxy,playerid,1,goodsid,level,type,bOverlap,0,0,iteminfo))
	{
		repgm.serialize(gmc->agent,gmc->from,0,gmc->cData);
		asynch_write(repgm.agent,(char*)&repgm,repgm.length);
		CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,0);
		return 0;
	}

	repgm.serialize(gmc->agent,gmc->from,1,gmc->cData);
	asynch_write(repgm.agent,(char*)&repgm,repgm.length);
	CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,1);

	PlayerOfficerTable officer;
	officer.setmasterid(playerid);
	officer.setdetailid(playerid);
	Proxy.db_select(officer);
	ISystemLog::getInstance()->playerEquip_GM_Log(officer.playerid_,officer.officername_,
		type,goodsid,1);
	return 1;
}

//增加指定兵种数量
int CGmThread::ProcAddArms(short gmid,const tagGMCommand* gmc,int playerid,int mapid,int cityid,int armsid,int nums)
{
	tagRepGM repgm;
		
	PlayerSoldierRace tb,oldSoldier;
	oldSoldier.setmasterid(mapid,cityid);
	oldSoldier.setdetailid(playerid);
	Proxy.db_select(oldSoldier);
	int oldcount = 0;
	switch (armsid) 
	{
		case 1:
			{
				tb.human_=nums;
				oldcount = oldSoldier.human_;
				break;
			}
		case 2:
			{
				tb.mannikin_=nums;
				oldcount = oldSoldier.mannikin_;
				break;
			}
		case 3:
			{
				tb.fairy_=nums;
				oldcount = oldSoldier.fairy_;
				break;
			}
		case 4:
			{
				tb.dandiprat_=nums;
				oldcount = oldSoldier.dandiprat_;
				break;
			}
		case 5:
			{
				tb.halfanimal_=nums;
				oldcount = oldSoldier.halfanimal_;
				break;
			}
		case 6:
			{
				tb.cattle_=nums;
				oldcount = oldSoldier.cattle_;
				break;
			}
		case 7:
			{
				tb.blackfairy_=nums;
				oldcount = oldSoldier.blackfairy_;
				break;
			}
		case 8:
			{
				tb.decpesrson_=nums;
				oldcount = oldSoldier.decpesrson_;
				break;
			}
		default: break;
	}
	tb.setmasterid(mapid,cityid);
	tb.setdetailid(playerid);

	if (Proxy.db_increase(tb)!=DB_Proxy::DB_SUCCEED)
	{
		repgm.serialize(gmc->agent,gmc->from,0,gmc->cData);
		asynch_write(repgm.agent,(char*)&repgm,repgm.length);
		CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,0);
		return 0;
	}

	repgm.serialize(gmc->agent,gmc->from,1,gmc->cData);
	asynch_write(repgm.agent,(char*)&repgm,repgm.length);
	CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,1);
	PlayerOfficerTable officer;
	officer.setmasterid(playerid);
	officer.setdetailid(playerid);
	Proxy.db_select(officer);
	ISystemLog::getInstance()->playerSoldier_GM_Log(officer.playerid_,officer.officername_,
		mapid,cityid,armsid,oldcount,oldcount + nums,nums);
	return 1;
}

//指定的科研项立即研究完成,目前该功能应禁止，GM中不能这样处理科研
int CGmThread::ProcAddResearch(short gmid,const tagGMCommand* gmc,int playerid,int rid,int level,int type)
{
	tagRepGM repgm;

	ResearchItem tb;
	tb.playerid_=playerid;
	tb.type_=type;
	tb.itemid_=rid;
	tb.level_=level;
	tb.count_=1;

	if (Proxy.db_insert(tb)!=DB_Proxy::DB_SUCCEED)
	{
		repgm.serialize(gmc->agent,gmc->from,0,gmc->cData);
		asynch_write(repgm.agent,(char*)&repgm,repgm.length);
		CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,0);
		return 0;
	}
	//删除科研中表ResearchActivity resItem;
	repgm.serialize(gmc->agent,gmc->from,1,gmc->cData);
	asynch_write(repgm.agent,(char*)&repgm,repgm.length);
	CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,1);
	return 1;
}

//添加仓库内指定物品 
int CGmThread::ProcAddGoods_1(short gmid,const tagGMCommand* gmc,int playerid,int mapid,int cityid,int goodsid,int nums)
{
	tagRepGM repgm;

	bool bsucceed(false);
	PlayerResource tb;
	tb.setmasterid(cityid,mapid);
	tb.setdetailid(playerid);
	bsucceed=Proxy.db_select(tb)==DB_Proxy::DB_SUCCEED  && tb.content_>=nums;
		
	if (bsucceed)
	{
		memset(&tb,0,sizeof(PlayerResource));
		switch (goodsid)
		{
		case 1:
			{
				tb.foodcount_=nums;
				break;
			}
		case 2:
			{
				tb.woodcount_=nums;
				break;
			}
		case 3:
			{
				tb.ironcount_=nums;
				break;
			}
		case 4:
			{
				tb.skincount_=nums;
				break;
			}
		default:break;
		}
		tb.content_=-nums;
		tb.setmasterid(cityid,mapid);
		tb.setdetailid(playerid);
		bsucceed=Proxy.db_increase(tb)==DB_Proxy::DB_SUCCEED;
	};
	if (!bsucceed)
	{
		repgm.serialize(gmc->agent,gmc->from,0,gmc->cData);
		asynch_write(repgm.agent,(char*)&repgm,repgm.length);
		CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,0);
		return 0;
	}

	repgm.serialize(gmc->agent,gmc->from,1,gmc->cData);
	asynch_write(repgm.agent,(char*)&repgm,repgm.length);
	CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,1);
	return 1;
}

int CGmThread::ProcAddGoods_2(short gmid,const tagGMCommand* gmc,int playerid,int mapid,int cityid,int goodsid,int type,int nums)
{
	tagRepGM repgm;

	bool bsucceed(false);
	PlayerResource tb;
	tb.setmasterid(cityid,mapid);
	tb.setdetailid(playerid);
	bsucceed=Proxy.db_select(tb)==DB_Proxy::DB_SUCCEED && tb.content_>=nums;

	ProductionAmount tbpb;
	if (bsucceed)
	{
		memset(&tb,0,sizeof(PlayerResource));
		tb.setmasterid(cityid,mapid);
		tb.setdetailid(playerid);
		tb.content_=-nums;

		tbpb.setmasterid(mapid,cityid,playerid,type);
		tbpb.setdetailid(goodsid);
		int dbrlt=Proxy.db_select(tbpb);
		if (dbrlt==DB_Proxy::DB_SUCCEED)   //找得到，直接改
		{
			tbpb.productioncount_=nums;
			bsucceed = (Proxy.db_increase(tb)==DB_Proxy::DB_SUCCEED) && (Proxy.db_increase(tbpb)==DB_Proxy::DB_SUCCEED);
		}
		else if (dbrlt==DB_Proxy::DB_EOF)		//找不到，即Insert
		{
			tbpb.productioncount_=nums;
			bsucceed=(Proxy.db_increase(tb)==DB_Proxy::DB_SUCCEED) && (Proxy.db_insert(tbpb)==DB_Proxy::DB_SUCCEED);
		}
		else bsucceed=false;
	}

	if (!bsucceed)
	{
		repgm.serialize(gmc->agent,gmc->from,0,gmc->cData);
		asynch_write(repgm.agent,(char*)&repgm,repgm.length);
		CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,0);
		return 0;
	}
	repgm.serialize(gmc->agent,gmc->from,1,gmc->cData);
	asynch_write(repgm.agent,(char*)&repgm,repgm.length);
	CGmFixData::Instance().InsertCmdRec(db_,gmc->from,gmc->cData,1,gmid,1);

	PlayerOfficerTable officer;
	officer.setmasterid(playerid);
	officer.setdetailid(playerid);
	Proxy.db_select(officer);
	ISystemLog::getInstance()->playerProduce_GM_Log(officer.playerid_,officer.officername_,
		mapid,cityid,type,goodsid,tbpb.productioncount_ - nums,tbpb.productioncount_,nums);
	return 1;
}
int CGmThread::ProcNotifyCreateNPCArmy(const DISTRIBUTED_HEADER * pNetRead)
{
	LPNotify_CreateNPCArmy_Response req_msg = (LPNotify_CreateNPCArmy_Response)pNetRead;
	if (req_msg->from <SERVER_BOUND_MAP_BEGIN || req_msg->from >MAX_SERVER_NUM)
	{
		return 1;
	}
	tagRepGM repgm;
	char buf[GM_DATA_SIZE+1] = {0};
	std::string strcmd,strdata;
	CgmServerIniFile::Instance().FindByKey(req_msg->GMCmd,strcmd,strdata);
	if (req_msg->Times >0)
	{
		sprintf(buf,"%s %d;%d;%d;%d;%d;%d;%d",strcmd.c_str(),req_msg->CountryID,req_msg->Level,req_msg->Count,
			req_msg->FromMap,req_msg->TargetMap,req_msg->Times,req_msg->Interval);
	}
	else
	{
		sprintf(buf,"%s %d;%d;%d;%d;%d",strcmd.c_str(),req_msg->CountryID,req_msg->Level,req_msg->Count,
			req_msg->FromMap,req_msg->TargetMap);
	}
	int code = (req_msg->Error_Code ==0)?1:0;
	repgm.serialize(req_msg->agent,req_msg->to,code,buf);
	asynch_write(repgm.agent,(char*)&repgm,repgm.length);
	CGmFixData::Instance().InsertCmdRec(db_,req_msg->to,buf,1,req_msg->GMCmd,code);
	return 0;
}
int CGmThread::ProcNotifyDelNPCArmy(const DISTRIBUTED_HEADER * pNetRead)
{
	LPNotify_DelNPCArmy_Response req_msg = (LPNotify_DelNPCArmy_Response)pNetRead;
	if (req_msg->from <SERVER_BOUND_MAP_BEGIN || req_msg->from >MAX_SERVER_NUM)
	{
		return 1;
	}
	tagRepGM repgm;
	int code = 0;
	char buf[GM_DATA_SIZE+1] = {0};
	std::string strcmd,strdata;
	CgmServerIniFile::Instance().FindByKey(req_msg->GMCmd,strcmd,strdata);
	sprintf(buf,"%s %d;%d",strcmd.c_str(),req_msg->from,req_msg->CountryID);
	code = (req_msg->DelCount >0)?1:0;
	repgm.serialize(req_msg->agent,req_msg->to,code,buf);
	asynch_write(repgm.agent,(char*)&repgm,repgm.length);
	CGmFixData::Instance().InsertCmdRec(db_,req_msg->to,buf,1,req_msg->GMCmd,req_msg->DelCount);
	return 0;
}
