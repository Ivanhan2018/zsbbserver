#include "gmFixData.h"
#include "gmServerIniFile.h"
#include "DB_ProxyHeader.h"
#include <sys/timeb.h>

#define PRINTOUT

CGmFixData::CGmFixData()
{
}

CGmFixData::~CGmFixData()
{
}

CGmFixData& CGmFixData::Instance()
{
	static CGmFixData aa;
	return aa;
}

Int64 CGmFixData::GetTimeNow()
{
#ifdef _WIN32
#  if defined(_MSC_VER)
    struct _timeb tb;
    _ftime(&tb);
#  elif defined(__BCPLUSPLUS__)
    struct timeb tb;
    ftime(&tb);
#  endif
    return ( static_cast<Int64>(tb.time) * ICE_INT64(1000000) + 
                tb.millitm * 1000 ) / 1000;
#else
    struct timeval tv;
    gettimeofday(&tv, 0);
    return ( tv.tv_sec * ICE_INT64(1000000) + tv.tv_usec ) / 1000;
#endif
}

bool CGmFixData::LoadData(const char * addr, short port,const char * dns,const char * user,const char * pwd,const char* webip,short webport,int timeout)
{
	if (!LoadHeroData(addr,port))
	{
		printf("LoadHeroData(DBG首领英雄) error\n");
		return false;
	}
	if (!ConData(dns,user,pwd))
	{
		printf("ConData(logic) error\n");
		return false;
	}
	if (!LoadLoginDisabled())
	{
		printf("LoadLoginDisabled(禁登入表) error\n");	
		return false;
	}
	if (!LoadSpeakingDisabled())
	{
		printf("LoadSpeakingDisabled(禁言表) error\n");	
		return false;
	}
	if (!LoadGmUserList(webip,webport,timeout))
	{
		printf("LoadGmUserList(conn webserver:%s-%d,timeout:%d) error\n",webip,webport,timeout);	
		return false;
	}
	ReadPackStoreParam();
	return true;
}

void CGmFixData::UnLoadData()
{
	hcHero_.clear();
	dspeaking_.clear();
	dlogin_.clear();
	gmuser_.clear();
	DisConnData();
	ClearPackStoreParam();
}

bool CGmFixData::LoadHeroData(const char * addr, short port)
{
	IceUtil::RWRecMutex::WLock wlk(heromux_);
	//查询DBG	
	DB_Proxy Proxy;
	if (!Proxy.init(addr,port,MDM_GP_GAME_MANAGE_SERVER))
	{
		Proxy.fini();
		return false;
	}
	
	char* buf = 0;
	int count = 0;
	PlayerOfficerTable pot;
	int ret=Proxy.db_select_all(pot,buf,count);
	
	if (ret==DB_Proxy::DB_FAILED)
	{
		Proxy.fini();
		return false;
	}
	else if (ret==DB_Proxy::DB_EOF)
	{
		Proxy.fini();
		return true;
	}
	
	//DB_Proxy::DB_SUCCEED
	const int size=sizeof(PlayerOfficerTable);
	const int reccount=count/size;
	PlayerOfficerTable* ppot;
	for(int i=0;i<reccount;++i)
	{
		ppot = (PlayerOfficerTable*)(buf+i*size);
		if (ppot->playerid_==ppot->officerid_)			//指挥官ID与玩家ID相等，即是首领英雄
			hcHero_[ppot->officername_]=ppot->officerid_;
	}
	
	Proxy.fini();
	return true;
}

bool CGmFixData::InsertHero(const char* heroname,int playerid)
{
	IceUtil::RWRecMutex::WLock wlk(heromux_);
	std::pair<HeroContainer::iterator,bool> itrlt;
	itrlt = hcHero_.insert(HeroContainer::value_type(heroname,playerid));
	return itrlt.second;
}

bool CGmFixData::FindHero(const char * heroname,int& playerid)
{
	IceUtil::RWRecMutex::RLock rlk(heromux_);
	HeroContainer::iterator it;
	it = hcHero_.find(heroname);
	if (it!=hcHero_.end())
	{
		playerid=it->second;
		return true;
	}
	else return false;
}

bool CGmFixData::InsertUC(const int playerid,const int agentid)
{
	IceUtil::RWRecMutex::WLock wlk(ucmux_);
	std::pair<UserContainer::iterator,bool> itrlt;
	itrlt=uc_.insert(UserContainer::value_type(playerid,agentid));
	return itrlt.second;
}

bool CGmFixData::FindUC(const int playerid,int& agentid)
{
	IceUtil::RWRecMutex::RLock rlk(ucmux_);
	UserContainer::iterator it;
	it = uc_.find(playerid);
	if (it!=uc_.end())
	{
		agentid=it->second;
		return true;
	}
	else return false;
}

bool CGmFixData::DeleteUC(const int playerid)
{
	IceUtil::RWRecMutex::WLock wlk(ucmux_);
	uc_.erase(playerid);
	return true;
}

bool CGmFixData::DeleteUCbyAgent(const int agentid)
{
	IceUtil::RWRecMutex::WLock wlk(ucmux_);
	UserContainer::iterator it1,it2;
	it1 = uc_.begin();

	while (it1 != uc_.end())
	{
		if (it1->second==agentid)
		{
			it2 = it1;
			++it1;
			uc_.erase(it2);
		}
		else ++it1;
	}
	return true;
}


bool CGmFixData::ConData(const char * dsn,const char * user,const char * password)
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
	catch(otl_exception& p)
	{ 
#ifdef PRINTOUT
		std::cerr <<"\n************************************************\n";
		std::cerr <<"********"<<"CGmFixData::ConData"<<"********"<<std::endl;
		std::cerr <<"otl_exception::code= \n";
		std::cerr <<"****"<<p.code<<"****"<<std::endl;

		std::cerr <<"otl_exception::msg = \n";
		std::cerr <<"****"<<p.msg <<"****"<< std::endl;
		
		if (p.stm_text)
		{
			std::cerr <<"otl_exception::stm_text = \n";
			std::cerr <<"****" <<p.stm_text <<"****"<< std::endl;
		}

		std::cerr <<"otl_exception::sqlstate = \n";
		std::cerr <<"****"<< p.sqlstate <<"****"<< std::endl;

		std::cerr <<"otl_exception::var_info = \n";
		std::cerr <<"****"<< p.var_info <<"****"<< std::endl;
		std::cerr <<"************************************************"<<std::endl;
#endif

		return false;
	}
	return true;
}

void CGmFixData::DisConnData()
{
	db_.logoff();
}

bool CGmFixData::InsertCmdRec(otl_connect& db,int gmid,const char* gmdata,short isvalid,short key,short rlt)
{
	IceUtil::Mutex::Lock rlk(logmux_);
	try
	{	
		std::string str = "insert into GmCommandRec(GmID,CmdData,IsValid,CmdDt,CmdKey,CmdRlt)  \
						  values(:f1<int>,:f2<char[255]>,:f3<short>,:f4<bigint>,:f5<short>,:f6<short>)";
		otl_stream o((int)str.size(),str.c_str(),db);
		o.set_commit(0);

		Int64 regdate;
		regdate=GetTimeNow();

		o<<gmid<<gmdata<<isvalid<<regdate<<key<<rlt;

		o.flush();
		db.commit();
		return true;
	}
	catch(otl_exception& p)
	{
#ifdef PRINTOUT
		std::cerr <<"\n************************************************\n";
		std::cerr <<"********"<<"CGmFixData::InsertCmdRec"<<"********"<<std::endl;
		std::cerr <<"otl_exception::msg = \n";
		std::cerr <<"****"<<p.msg <<"****"<< std::endl;
		std::cerr <<"************************************************"<<std::endl;
#endif
		db.rollback();
		return false;
	}
}

bool CGmFixData::LoadLoginDisabled()
{
	IceUtil::RWRecMutex::WLock wlk(loginmux_);
	try
	{
		string str="select PlayerID from LoginDisabled";
		otl_stream i((int)str.size(),str.c_str(),db_);
		
		i.flush();
		
		int playerid(0);
		while (!i.eof())
		{
			i>>playerid;
			dlogin_.insert(playerid);
		}
		return true;
	}
	catch (otl_exception& p)
	{
#ifdef PRINTOUT
		std::cerr <<"\n************************************************\n";
		std::cerr <<"********"<<"CGmFixData::LoadLoginDisabled"<<"********"<<std::endl;
		std::cerr <<"otl_exception::msg = \n";
		std::cerr <<"****"<<p.msg <<"****"<< std::endl;
		std::cerr <<"************************************************"<<std::endl;
#endif
		return false;
	}
}


bool CGmFixData::InsertLogin(otl_connect& db,const int playerid)
{
	IceUtil::RWRecMutex::WLock wlk(loginmux_);
	dlogin_.insert(playerid);

	try
	{	
		std::string str = "insert into LoginDisabled(PlayerID) values(:f1<int>)";
		otl_stream o((int)str.size(),str.c_str(),db);
		o.set_commit(0);
		o<<playerid;
		o.flush();
		db.commit();
		return true;
	}
	catch(otl_exception& p)
	{
#ifdef PRINTOUT
		std::cerr <<"\n************************************************\n";
		std::cerr <<"********"<<"CGmFixData::InsertLogin"<<"********"<<std::endl;
		std::cerr <<"otl_exception::msg = \n";
		std::cerr <<"****"<<p.msg <<"****"<< std::endl;
		std::cerr <<"************************************************"<<std::endl;
#endif
		db.rollback();
		return false;
	}
}

bool CGmFixData::FindLogin(const int playerid)
{
	IceUtil::RWRecMutex::RLock rlk(loginmux_);
	DataCit cit=dlogin_.find(playerid);
	return (cit!=dlogin_.end());
}

bool CGmFixData::DelLogin(otl_connect& db,const int playerid)
{
	IceUtil::RWRecMutex::WLock wlk(loginmux_);
	dlogin_.erase(playerid);
	try
	{	
								  
		std::string str = "delete LoginDisabled where PlayerID=:f1<int>";
		otl_stream o((int)str.size(),str.c_str(),db);
		o.set_commit(0);
		o<<playerid;
		o.flush();
		db.commit();
		return true;
	}
	catch(otl_exception& p)
	{
#ifdef PRINTOUT
		std::cerr <<"\n************************************************\n";
		std::cerr <<"********"<<"CGmFixData::DelLogin"<<"********"<<std::endl;
		std::cerr <<"otl_exception::msg = \n";
		std::cerr <<"****"<<p.msg <<"****"<< std::endl;
		std::cerr <<"************************************************"<<std::endl;
#endif
		db.rollback();
		return false;
	}
}

bool CGmFixData::LoadSpeakingDisabled()
{
	IceUtil::RWRecMutex::WLock wlk(speakmux_);
	try
	{
		string str="select PlayerID from SpeakingDisabled";
		otl_stream i((int)str.size(),str.c_str(),db_);
		
		i.flush();
		
		int playerid(0);
		while (!i.eof())
		{
			i>>playerid;
			dspeaking_.insert(playerid);
		}
		return true;
	}
	catch (otl_exception& p)
	{
#ifdef PRINTOUT
		std::cerr <<"\n************************************************\n";
		std::cerr <<"********"<<"CGmFixData::LoadSpeakingDisabled"<<"********"<<std::endl;
		std::cerr <<"otl_exception::msg = \n";
		std::cerr <<"****"<<p.msg <<"****"<< std::endl;
		std::cerr <<"************************************************"<<std::endl;
#endif
		return false;
	}
}

bool CGmFixData::InsertSpeak(otl_connect& db,const int playerid)
{
	IceUtil::RWRecMutex::WLock wlk(speakmux_);
	dspeaking_.insert(playerid);

	try
	{	
		std::string str = "insert into SpeakingDisabled(PlayerID) values(:f1<int>)";
		otl_stream o((int)str.size(),str.c_str(),db);
		o.set_commit(0);
		o<<playerid;
		o.flush();
		db.commit();
		return true;
	}
	catch(otl_exception& p)
	{
#ifdef PRINTOUT
		std::cerr <<"\n************************************************\n";
		std::cerr <<"********"<<"CGmFixData::InsertSpeak"<<"********"<<std::endl;
		std::cerr <<"otl_exception::msg = \n";
		std::cerr <<"****"<<p.msg <<"****"<< std::endl;
		std::cerr <<"************************************************"<<std::endl;
#endif
		db.rollback();
		return false;
	}
}

bool CGmFixData::FindSpeak(const int playerid)
{
	IceUtil::RWRecMutex::RLock rlk(speakmux_);
	DataCit cit=dspeaking_.find(playerid);
	return (cit!=dspeaking_.end());
}

bool CGmFixData::DelSpeak(otl_connect& db,const int playerid)
{
	IceUtil::RWRecMutex::WLock wlk(speakmux_);
	dspeaking_.erase(playerid);

	try
	{	
		std::string str = "delete SpeakingDisabled where PlayerID=:f1<int>";
		otl_stream o((int)str.size(),str.c_str(),db);
		o.set_commit(0);
		o<<playerid;
		o.flush();
		db.commit();
		return true;
	}
	catch(otl_exception& p)
	{
#ifdef PRINTOUT
		std::cerr <<"\n************************************************\n";
		std::cerr <<"********"<<"CGmFixData::DelSpeak"<<"********"<<std::endl;
		std::cerr <<"otl_exception::msg = \n";
		std::cerr <<"****"<<p.msg <<"****"<< std::endl;
		std::cerr <<"************************************************"<<std::endl;
#endif
		db.rollback();
		return false;
	}
}

bool CGmFixData::LoadGmUserList(const char * ip,short port ,int timeout)
{
	if (StartWebClient(ip,port,timeout,0)==1)
	{
		char * buf=0;
		int buflen=0;
		if (GetGmUserList(buf,buflen)==1)
		{
			int rows = buflen / sizeof(tagGmUserInfo);
			tagGmUserInfo * p=(tagGmUserInfo*)buf;
			
			//printf("rows:%d\n",rows);
			for (int i=0;i<rows;i++)
			{
				gmuser_.insert(make_pair(p[i].account_id,p[i]));
				//printf("%d %s %d\n",p[i].account_id,p[i].user_name,p[i].permission);
			}
			DeleteBuf(buf);
			StopWebClient();
			return true;
		}
		else 
		{
			StopWebClient();
			return false;
		}
	}
	else return false;
}

bool CGmFixData::FindGmUserInfo(const int playerid,tagGmUserInfo& gmuserinfo)
{
	GmUserContainer::iterator it;
	it = gmuser_.find(playerid);
	if (it!=gmuser_.end())
	{
		gmuserinfo=it->second;
		return true;
	}
	else return false;
}

//GM权限
bool CGmFixData::QryPermission(int code,int cmdcode)
{
	switch (code)
	{
		//查询类  (禁言，标登陆)
	case 1:
		{
			return (cmdcode==11 || cmdcode==12) ;
		}
		//公告类
	case 2:
		{
			switch (cmdcode)
			{
			case 13:
			case 14:
			case 15:
			case 16:
				{
					return true;
				}
			default:
				{
					return false;
				}
			}
		}
		//NPC反击权限
	case 4:
		{
			switch(cmdcode)
			{
			case 17:
			case 18:
				{
					return true;
				}
			default:
				{
					return false;
				}
			}
		}
		//添加物资类
	case 6:
		{
			switch (cmdcode)
			{
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
				{
					return true;
				}
			default:
				{
					return false;
				}
			}
		}
		//超级用户
	case 9:
		{
			return true;
		}
	default:
		{
			return false;
		}
	}
}

PackStoreParam *CGmFixData::GetPackStoreParam(int postion)
{
	PackStoreParam_Map_Iterator iter = packStoreParamMap_.find(postion);
	if (iter != packStoreParamMap_.end())
	{
		return iter->second;
	}
	return NULL;
}
int CGmFixData::ReadPackStoreParam()
{
	FILE *fp = 0;
	char buf[1024] = {0};
	fp = fopen("configtable/PackStoreParam.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/PackStoreParam.csv失败。\n");
		return 1;
	}

	fgets(buf,1024,fp);
	PackStoreParam* pNewNode = 0;
	int iOverlap = 0;
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp)== NULL)
			break;
		pNewNode = new PackStoreParam;
		if(sscanf(buf,"%d,%d",&pNewNode->Postion,&iOverlap) <2)
		{
			delete pNewNode;
			pNewNode = 0;
			continue;
		}
		pNewNode->IsOverlap = iOverlap;
		packStoreParamMap_[pNewNode->Postion] = pNewNode;
	}
	fclose(fp);
	return 0;
}
void CGmFixData::ClearPackStoreParam()
{
	//背包是否重叠参数
	PackStoreParam* pPackStoreParam = 0;
	PackStoreParam_Map_Iterator packiter;
	for (packiter = packStoreParamMap_.begin();packiter != packStoreParamMap_.end();packiter++)
	{
		pPackStoreParam = packiter->second;
		if (pPackStoreParam)
		{
			delete pPackStoreParam;
			pPackStoreParam =0;
		}
	}
	packStoreParamMap_.clear();
}
