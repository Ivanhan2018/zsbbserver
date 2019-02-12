#include "web_server_public.h"
#include <crtdbg.h>
#include "base64.h"

//#define PRINTOUT

web_server_public::web_server_public()
:bconn_(false)
{
}

web_server_public::~web_server_public()
{
//	LOG_NO_RETURN(!bconn_,("web_server_public::~web_server_public \n"));
}

//Int64 web_server_public::GetTimeNow()
//{
//#ifdef _WIN32
//#  if defined(_MSC_VER)
//    struct _timeb tb;
//    _ftime(&tb);
//#  elif defined(__BCPLUSPLUS__)
//    struct timeb tb;
//    ftime(&tb);
//#  endif
//    return (static_cast<Int64>(tb.time) * ICE_INT64(1000000) + 
//                tb.millitm * 1000 ) / 1000;
//#else
//    struct timeval tv;
//    gettimeofday(&tv, 0);
//    return ( tv.tv_sec * ICE_INT64(1000000) + tv.tv_usec ) / 1000;
//#endif
//}
//
//std::string web_server_public::toDatetime(Int64 sec)
//{
//    time_t time = static_cast<long>(sec / 1000000);
//
//    struct tm* t;
//#ifdef _WIN32
//    t = localtime(&time);
//#else
//    struct tm tr;
//    localtime_r(&time, &tr);
//    t = &tr;
//#endif
//
//    char buf[32];
//    strftime(buf, sizeof(buf), "%x %H:%M:%S", t);
//
//    std::ostringstream os;
//    os << buf << ".";
//    os.fill('0');
//    os.width(3);
//    os << static_cast<long>(sec % 1000000 / 1000);
//    return os.str();
//}


bool web_server_public::ConData(const char * dsn,const char * user,const char * password)
{
//	LOG_NO_RETURN(!bconn_,("web_server_public::ConData \n"));
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
		std::cerr <<"********"<<"web_server_public::ConData"<<"********"<<std::endl;
		std::cerr <<"otl_exception::msg : \n";
		std::cerr <<p.msg <<std::endl;
		return false;
	}
	bconn_=true;
	return bconn_;
}

void web_server_public::DisConnData()
{
	if (bconn_) db_.logoff();
	bconn_=false;
}


void web_server_public::OnCheckUser(internal_socket * i_s,const char* buf)
{
	tagCheckUser * checkuser = (tagCheckUser*)buf;

	tagRepResult* reprlt= new tagRepResult;
	reprlt->serialize(checkuser->index,checkuser->command,0);
	try
	{
		std::string str = "select 1 from GameUser with (nolock) where AccountName=\'" ;
		str+=checkuser->user_name;
		str+="\'";
		otl_stream i((int)str.size(), str.c_str(), db_);

		if (i.eof()) reprlt->bsucceed=1;
		i.close();
	}
	catch(otl_exception& p)
	{
		reprlt->serialize(checkuser->index,checkuser->command,0);
		i_s->start_async_write((char *)reprlt,sizeof(*reprlt));
		printf("line(%d) msg:%s\n", __LINE__, p.msg);
		return;
	}
	i_s->start_async_write((char *)reprlt,sizeof(*reprlt));
}

void web_server_public::OnRegUser(internal_socket * i_s,const char * buf)
{

	tagRegUser * reguser = (tagRegUser*)buf;

	tagRepResult* reprlt= new tagRepResult;
	reprlt->serialize(reguser->index,reguser->command,0); 
	
	if (c_name(decrypt)((unsigned char *)reguser->userinfo.password, (unsigned char *)reguser->userinfo.password, \
		&CWebIniFile::Instance().getAes()) != aes_good)
	{
//		LOG_NO_RETURN(false,("OnRegUser函数调用解密时，错误 \n"));
		i_s->start_async_write((char *)reprlt,sizeof(*reprlt));
		return ;
	}

	char encpass[50];
	base64_encode(reguser->userinfo.password, strlen(reguser->userinfo.password), encpass);

	try
	{	
		std::string str = "insert into GameUser(AccountName,[Password],Email,RegDate,RegIP,FromUrl,FromAccountID,qq,memo,AccountType)  \
						  values(:f1<char[16]>,:f2<char[50]>,:f3<char[100]>,:f4<bigint>,:f5<char[20]>,:f6<char[150]>,:f7<int>,:f8<char[100]>,:f9<char[100]>,:f10<int>)";
		otl_stream o((int)str.size(),str.c_str(),db_);
		o.set_commit(0);

		struct in_addr sip;
		memcpy(&sip,&reguser->userinfo.ip,4);
		char * cip = inet_ntoa(sip);

		Int64 regdate;
		regdate=CWebIniFile::GetTimeNow();

		o<<reguser->userinfo.user_name<<encpass<<reguser->userinfo.email<<regdate<<cip<< \
			reguser->userinfo.from_url<<reguser->userinfo.fromAccountID<<reguser->userinfo.qq<<reguser->userinfo.memo << reguser->userinfo.accounttype;
		
		o.flush();
		db_.commit();
		reprlt->bsucceed=1;
		i_s->start_async_write((char *)reprlt,sizeof(*reprlt));
	}
	catch(otl_exception& p)
	{
		db_.rollback();
		i_s->start_async_write((char *)reprlt,sizeof(*reprlt));
		printf("line(%d) msg:%s\n", __LINE__, p.msg);
		return ;
	}
}

void web_server_public::OnUserLoginPlus(internal_socket * i_s,const char * buf)
{
	tagUserLogin * userlogin = (tagUserLogin*)buf;
	
	tagRepResult* reprlt= new tagRepResult;
	reprlt->serialize(userlogin->index,userlogin->command,0);
	if (c_name(decrypt)((unsigned char *)userlogin->password, (unsigned char *)userlogin->password, \
		&CWebIniFile::Instance().getAes()) != aes_good)
	{
//		LOG_NO_RETURN(false,(" OnRegUser函数调用解密时，错误 \n"));
		i_s->start_async_write((char *)reprlt,sizeof(*reprlt));
		return ;
	}

	char encpass[50];
	base64_encode(userlogin->password, strlen(userlogin->password), encpass);

	Int64 regdate;
	regdate=CWebIniFile::GetTimeNow();

	char p[300];
	sprintf(p,"update GameUser set LastLoginTimes=%I64d, LoginTimes=isnull(LoginTimes,0)+1 where AccountName=\'%s\' and [Password]=\'%s\'", \
		   regdate,userlogin->user_name,encpass);
	int irlt = otl_cursor::direct_exec(db_,p,otl_exception::disabled);
	db_.commit();

	if (irlt>0)
	{
		try
		{
			std::string str="select AccountID from GameUser with (nolock) where AccountName=:f1<char[16]> and [Password]=:f2<char[50]>";
			otl_stream o((int)str.size(),str.c_str(),db_);
			o<<userlogin->user_name<<encpass;
			o.flush();

			if (!o.eof()) 
				o>>irlt;
			else
				irlt = 0;
			o.close();
		}
		catch (otl_exception& p)
		{
			irlt = 0;
			printf("line(%d) msg:%s\n", __LINE__, p.msg);
		}
		reprlt->bsucceed = irlt;  
	}
	else 
	{
		reprlt->bsucceed = 0 ; 
	}
	i_s->start_async_write((char *)reprlt,sizeof(*reprlt));
}


void web_server_public::OnModifyPasswordPlus(internal_socket * i_s,const char * buf)
{
	tagModifyPassword * modifypwd = (tagModifyPassword*)buf;
	tagRepResult* reprlt= new tagRepResult;
	reprlt->serialize(modifypwd->index,modifypwd->command,0);
	
	if (c_name(decrypt)((unsigned char *)modifypwd->old_password, (unsigned char *)modifypwd->old_password, \
		&CWebIniFile::Instance().getAes()) != aes_good)
	{
//		LOG_NO_RETURN(false,(" OnRegUser函数调用解密时，错误 \n"));
		i_s->start_async_write((char *)reprlt,sizeof(*reprlt));
		return ;
	}

	char old_encpass[50];
	base64_encode(modifypwd->old_password, strlen(modifypwd->old_password), old_encpass);

	if (c_name(decrypt)((unsigned char *)modifypwd->new_password, (unsigned char *)modifypwd->new_password, \
		&CWebIniFile::Instance().getAes()) != aes_good)
	{
//		LOG_NO_RETURN(false,(" OnRegUser函数调用解密时，错误 \n"));
		i_s->start_async_write((char *)reprlt,sizeof(*reprlt));
		return ;
	}

	char new_encpass[50];
	base64_encode(modifypwd->new_password, strlen(modifypwd->new_password), new_encpass);

	char p[300];
	sprintf(p,"update GameUser set [password]=\'%s\' where  AccountName=\'%s\' and [Password]=\'%s\'", \
	    	new_encpass,modifypwd->user_name,old_encpass);
	int irlt = otl_cursor::direct_exec(db_,p,otl_exception::disabled);
	db_.commit();

	if (irlt>0) 
		reprlt->bsucceed =1; 
	else 
		reprlt->bsucceed=0;  
	
	tagLogRec logrec;
	char strdata[100];
	sprintf(strdata,"emUpdatePassword user:%s,old:%s,new:%s",modifypwd->user_name,modifypwd->old_password,modifypwd->new_password);
	logrec.serialize(emUpdatePassword,CWebIniFile::Instance().GetTypeName(emUpdatePassword),strdata,CWebIniFile::Instance().GetTimeNow(),reprlt->bsucceed);

	i_s->start_async_write((char *)reprlt,sizeof(*reprlt));
	CWebIniFile::Instance().InsertLogRec(logrec, db_);
}

void web_server_public::OnModifyPasswordNoCheck(internal_socket * i_s,const char * buf)
{
	tagModifyPasswordNoCheck * modifypwd = (tagModifyPasswordNoCheck*)buf;
	tagRepResult* reprlt= new tagRepResult;
	reprlt->serialize(modifypwd->index,modifypwd->command,0);
	
	if (c_name(decrypt)((unsigned char *)modifypwd->new_password, (unsigned char *)modifypwd->new_password, \
		&CWebIniFile::Instance().getAes()) != aes_good)
	{
//		LOG_NO_RETURN(false,(" OnRegUser函数调用解密时，错误 \n"));
		i_s->start_async_write((char *)reprlt,sizeof(*reprlt));
		return ;
	}

	char new_encpass[50];
	base64_encode(modifypwd->new_password, strlen(modifypwd->new_password), new_encpass);

	char p[300];
	sprintf(p,"update GameUser set [password]=\'%s\' where  AccountName=\'%s\'", \
	    	new_encpass,modifypwd->user_name);
	int irlt = otl_cursor::direct_exec(db_,p,otl_exception::disabled);
	db_.commit();

	if (irlt>0) 
		reprlt->bsucceed =1; 
	else 
		reprlt->bsucceed=0;  
	
	tagLogRec logrec;
	char strdata[100];
	sprintf(strdata,"emUpdatePasswordNoCheck user:%s new:%s",modifypwd->user_name,modifypwd->new_password);
	logrec.serialize(emUpdatePasswordNoCheck,CWebIniFile::Instance().GetTypeName(emUpdatePasswordNoCheck), \
		strdata,CWebIniFile::Instance().GetTimeNow(),reprlt->bsucceed);

	i_s->start_async_write((char *)reprlt,sizeof(*reprlt));
	CWebIniFile::Instance().InsertLogRec(logrec, db_);
}

void web_server_public::OnModifyPasswordExt(internal_socket * i_s,const char * buf)
{
	tagModifyPasswordExt * modifypwd = (tagModifyPasswordExt*)buf;
	tagRepResult* reprlt= new tagRepResult;
	reprlt->serialize(modifypwd->index,modifypwd->command,0);
	
	if (c_name(decrypt)((unsigned char *)modifypwd->old_password, (unsigned char *)modifypwd->old_password, \
		&CWebIniFile::Instance().getAes()) != aes_good)
	{
//		LOG_NO_RETURN(false,(" OnRegUser函数调用解密时，错误 \n"));
		i_s->start_async_write((char *)reprlt,sizeof(*reprlt));
		return ;
	}

	char old_encpass[50];
	base64_encode(modifypwd->old_password, strlen(modifypwd->old_password), old_encpass);

	if (c_name(decrypt)((unsigned char *)modifypwd->new_password, (unsigned char *)modifypwd->new_password, \
		&CWebIniFile::Instance().getAes()) != aes_good)
	{
//		LOG_NO_RETURN(false,(" OnRegUser函数调用解密时，错误 \n"));
		i_s->start_async_write((char *)reprlt,sizeof(*reprlt));
		return ;
	}

	char new_encpass[50];
	base64_encode(modifypwd->new_password, strlen(modifypwd->new_password), new_encpass);

	char p[400];
	sprintf(p,"update GameUser set [password]=\'%s\',Question=\'%s\',Answer=\'%s\'  where  AccountName=\'%s\' and [Password]=\'%s\'", \
		new_encpass,modifypwd->question,modifypwd->answer,modifypwd->user_name,old_encpass);
	int irlt = otl_cursor::direct_exec(db_,p,otl_exception::disabled);
	db_.commit();

	if (irlt>0) 
		reprlt->bsucceed =1; 
	else 
		reprlt->bsucceed=0;  
	
	tagLogRec logrec;
	char strdata[100];
	sprintf(strdata,"emUpdatePasswordExt user:%s,old:%s,new:%s",modifypwd->user_name,modifypwd->old_password,modifypwd->new_password);
	logrec.serialize(emUpdatePasswordExt,CWebIniFile::Instance().GetTypeName(emUpdatePasswordExt),strdata,CWebIniFile::Instance().GetTimeNow(),reprlt->bsucceed);

	i_s->start_async_write((char *)reprlt,sizeof(*reprlt));
	CWebIniFile::Instance().InsertLogRec(logrec, db_);
}


void web_server_public::OnAddMoneyPlus(internal_socket * i_s,const char * buf)
{
	tagAddMoney* addmoney = (tagAddMoney*)buf;
	tagRepResult * reprlt = new tagRepResult;
	
	char p[300];
	sprintf(p,"update GameUser set Money=Money + %f,godian=godian+%d, Score=Score + %d where AccountName=\'%s\'",addmoney->money,addmoney->godian,addmoney->score,addmoney->user_name);
	int irlt = otl_cursor::direct_exec(db_,p,otl_exception::disabled);
	db_.commit();
	
	if (irlt>0)
		reprlt->serialize(addmoney->index,addmoney->command,1);
	else 
		reprlt->serialize(addmoney->index,addmoney->command,0);

	i_s->start_async_write((char *)reprlt,sizeof(*reprlt));

	tagLogRec logrec;
	char strdata[100];
	sprintf(strdata,"emAddMoney user:%s,money:%f,godian:%d,score:%d",addmoney->user_name,addmoney->money,addmoney->godian,addmoney->score);
	logrec.serialize(emAddMoney,CWebIniFile::Instance().GetTypeName(emAddMoney),strdata,CWebIniFile::Instance().GetTimeNow(),irlt);
	CWebIniFile::Instance().InsertLogRec(logrec, db_);
	
	//insert rec to GameAddMoney table
	if (irlt>0)
	{
		sprintf(p,"select AccountID from GameUser with (nolock) where AccountName=\'%s\'",addmoney->user_name);
		otl_stream i((int)strlen(p),p,db_);
		int AccountID=0;
		if (!i.eof()) 
			i>>AccountID;
		i.close();
		if (AccountID==0) return;

		try
		{
			sprintf(p,"insert GameAddMoney(AccountID,Money,godian,Score,addDate,addtype) values(:f1<int>,:f2<float>,:f3<int>,:f4<int>,:f5<bigint>,:f6<int>)");
			otl_stream o((int)strlen(p),p,db_);
			o.set_commit(0);
			o<<AccountID<<addmoney->money<<addmoney->godian<<addmoney->score<<logrec.LogDate<<addmoney->addtype;
			o.flush();
			db_.commit();
		}
		catch (otl_exception& p)
		{
			db_.rollback();
			printf("line(%d) msg:%s\n", __LINE__, p.msg);
		}
	}
}

void web_server_public::OnSubGoDian(internal_socket * i_s,const char * buf)
{
	tagSubMoney* submoney = (tagSubMoney*)buf;
	tagRepResult* reprlt = new tagRepResult;
	char p[300];
	sprintf(p,"update GameUser set GoDian=GoDian-%d where AccountID=%d and GoDian>=%d",\
			submoney->GoDian,submoney->account_id,submoney->GoDian);
	int irlt = otl_cursor::direct_exec(db_,p,otl_exception::disabled);
	db_.commit();
	
	if (irlt>0)
		reprlt->serialize(submoney->index,submoney->command,1);
	else 
		reprlt->serialize(submoney->index,submoney->command,0);

	i_s->start_async_write((char *)reprlt,sizeof(*reprlt));

	tagLogRec logrec;
	char strdata[100];
	sprintf(strdata,"emSubMoney userid:%d,GoDian:%d",submoney->account_id,submoney->GoDian);
	logrec.serialize(emSubMoney,CWebIniFile::Instance().GetTypeName(emSubMoney),strdata,CWebIniFile::Instance().GetTimeNow(),irlt, submoney->account_id, submoney->GoDian, submoney->desc);
	CWebIniFile::Instance().InsertLogRec(logrec, db_);
}

void web_server_public::OnGetGoDian(internal_socket * i_s,const char * buf)
{
	tagGetMoney * getmoney = (tagGetMoney*)buf;
	tagRepGetMoney * repgetmoney = new tagRepGetMoney;
	repgetmoney->serialize(getmoney->index,0,0);
	int GoDian = 0;

	try
	{
		std::string str = "select GoDian from GameUser with (nolock) where AccountID=:f1<int>";
		otl_stream i((int)str.size(),str.c_str(),db_);
		i<<getmoney->account_id;

		i.flush();
		if (!i.eof())
		{	
			i>>GoDian;
			repgetmoney->bsucceed = 1;
			repgetmoney->GoDian = GoDian;
		}
		i.close();
		i_s->start_async_write((char *)repgetmoney,sizeof(*repgetmoney));
	}
	catch (otl_exception& p)
	{
		i_s->start_async_write((char *)repgetmoney,sizeof(*repgetmoney));
		printf("line(%d) msg:%s\n", __LINE__, p.msg);
	}
}
	
void web_server_public::OnGetScore(internal_socket * i_s,const char * buf)
{
	tagGetScore* getscore= (tagGetScore*)buf;
	tagRepGetScore* repgetscore= new tagRepGetScore;
	repgetscore->serialize(getscore->index,0,0);
	int score_(0);

	try
	{
		std::string str = "select Score from GameUser with (nolock) where AccountID=:f1<int>";
		otl_stream i((int)str.size(),str.c_str(),db_);
		i<<getscore->account_id;

		i.flush();
		if (!i.eof())
		{	
			i>>score_;
			repgetscore->bsucceed = 1;
			repgetscore->score= score_;
		}
		i.close();
		i_s->start_async_write((char *)repgetscore,sizeof(*repgetscore));
	}
	catch (otl_exception& p)
	{
		i_s->start_async_write((char *)repgetscore,sizeof(*repgetscore));
		printf("line(%d) msg:%s\n", __LINE__, p.msg);
	}
}

void web_server_public::OnSubScore(internal_socket * i_s,const char * buf)
{
	tagSubScore* subscore= (tagSubScore*)buf;
	tagRepResult* reprlt = new tagRepResult;
	char p[300];
	sprintf(p,"update GameUser set Score=Score-%d where AccountID=%d and Score>=%d",\
		subscore->score,subscore->account_id,subscore->score);
	int irlt = otl_cursor::direct_exec(db_,p,otl_exception::disabled);
	db_.commit();
	
	if (irlt>0)
		reprlt->serialize(subscore->index,subscore->command,1);
	else 
		reprlt->serialize(subscore->index,subscore->command,0);

	i_s->start_async_write((char *)reprlt,sizeof(*reprlt));

	tagLogRec logrec;
	char strdata[100];
	sprintf(strdata,"emSubScore userid:%d,score:%d",subscore->account_id,subscore->score);
	logrec.serialize(emSubScore,CWebIniFile::Instance().GetTypeName(emSubScore),strdata,CWebIniFile::Instance().GetTimeNow(),irlt);
	CWebIniFile::Instance().InsertLogRec(logrec, db_);
}


void web_server_public::OnUpdateEmail(internal_socket * i_s,const char * buf)
{
	tagUpdateEmail * email = (tagUpdateEmail*)buf;
	tagRepResult * reprlt = new tagRepResult;
	char p[300];
	sprintf(p,"update GameUser set Email=\'%s\' where AccountName=\'%s\' and Email=\'%s\'",\
			email->new_email,email->user_name,email->old_email);
	int irlt = otl_cursor::direct_exec(db_,p,otl_exception::disabled);
	db_.commit();

	if (irlt>0)
		reprlt->serialize(email->index,email->command,1);
	else 
		reprlt->serialize(email->index,email->command,0);

	i_s->start_async_write((char *)reprlt,sizeof(*reprlt));

	tagLogRec logrec;
	char strdata[100];
	sprintf(strdata,"emUpdateEmail user:%s,",email->user_name);
	logrec.serialize(emUpdateEmail,CWebIniFile::Instance().GetTypeName(emUpdateEmail),strdata,CWebIniFile::Instance().GetTimeNow(),irlt);
	CWebIniFile::Instance().InsertLogRec(logrec, db_);
}

void web_server_public::OnGetAnswer(internal_socket * i_s,const char * buf)
{
	tagGetAnswer * answer = (tagGetAnswer *)buf;
	tagRepGetAnswer * repanswer = new tagRepGetAnswer;
	
	try
	{
		std::string str = "select Answer from GameUser with (nolock) where AccountName=:f1<char[16]>";
		otl_stream i((int)str.size(),str.c_str(),db_);
		i<<answer->user_name;

		i.flush();
		if (!i.eof())
		{	
			char s[50];
			i>>s;
			repanswer->serialize(answer->index,1,s);
		}
		else 
		{
			repanswer->serialize(answer->index,0,NULL);
		}
		i.close();
		i_s->start_async_write((char *)repanswer,sizeof(*repanswer));
	}
	catch (otl_exception& p)
	{
		repanswer->serialize(answer->index,0,NULL);
		i_s->start_async_write((char *)repanswer,sizeof(*repanswer));
		printf("line(%d) msg:%s\n", __LINE__, p.msg);
	}
}

void web_server_public::OnGetEmail(internal_socket * i_s,const char * buf)
{
	tagGetEmail * email = (tagGetEmail *)buf;
	tagRepGetEmail* repemail = new tagRepGetEmail;
	
	try
	{
		std::string str = "select Email from GameUser with (nolock) where AccountName=:f1<char[16]>";
		otl_stream i((int)str.size(),str.c_str(),db_);
		i<<email->user_name;

		i.flush();
		if (!i.eof())
		{	
			char s[100];
			i>>s;
			repemail->serialize(email->index,1,s);
		}
		else 
		{
			repemail->serialize(email->index,0,NULL);
		}
		i.close();
		i_s->start_async_write((char *)repemail,sizeof(*repemail));
	}
	catch (otl_exception& p)
	{
		repemail->serialize(email->index,0,NULL);
		i_s->start_async_write((char *)repemail,sizeof(*repemail));
		printf("line(%d) msg:%s\n", __LINE__, p.msg);
	}
}

void web_server_public::OnGetPlayerList(internal_socket * i_s,const char * buf)
{
	tagGetPlayerList * prep = (tagGetPlayerList *)buf;

	char * buf1 = 0;
	tagPlayerInfo * pinfo=0; 
	tagRepPlayerList* rlp=0;

	try
	{
		int idx=0;
		int rows=0;
		{
			otl_stream i(100,"select count(1) from vPlayerCompare with (nolock)",db_);
			if (!i.eof()) i >> rows;
		}

		buf1 = new char[sizeof(tagRepPlayerList)+rows*sizeof(tagPlayerInfo)];
		pinfo=(tagPlayerInfo *)(buf1+sizeof(tagRepPlayerList)); 
		rlp=(tagRepPlayerList*)buf1;

		if (rows > 0)
		{
			otl_stream i(100,"select * from vPlayerCompare with (nolock)",db_);

			while (!i.eof() && idx<rows)
			{
				i>>pinfo[idx].id>>pinfo[idx].PlayerID>>pinfo[idx].PlayerName>>pinfo[idx].Country>>pinfo[idx].OfficerRace>>pinfo[idx].Level>> \
				pinfo[idx].MilitaryRank>>pinfo[idx].Money>>pinfo[idx].ResourceCount>>pinfo[idx].CityCount>>pinfo[idx].GeneralCount>>pinfo[idx].Credit>>pinfo[idx].Address;
				++idx;
			}
		}
		rlp->serialize(prep->index,1,idx*sizeof(tagPlayerInfo));
		i_s->start_async_write((char*)rlp,rlp->len);
	}
	catch(otl_exception& p)
	{
		//返回失败包...
		if (!buf1)
		{
			buf1 = new char[sizeof(tagRepPlayerList)];
			rlp=(tagRepPlayerList*)buf1;
		}

		rlp->serialize(prep->index,0,0);
		i_s->start_async_write((char*)rlp,rlp->len);
		printf("line(%d) msg:%s\n", __LINE__, p.msg);
		return;
	}
}

void web_server_public::OnGetGongHuiList(internal_socket * i_s,const char * buf)
{
	tagGetGongHuiList * prep = (tagGetGongHuiList *)buf;
	
	char * buf1 = 0;
	tagGongHuiInfo * pinfo = 0;
	tagRepGongHuiList* rlp = 0;

	try
	{
		int idx=0;
		int rows=0;
		{
			otl_stream i(100,"select count(1) from vConsortiaCompare with (nolock)",db_);
			if (!i.eof()) i >> rows;
		}

		buf1 = new char[sizeof(tagRepGongHuiList)+rows*sizeof(tagGongHuiInfo)];
		pinfo=(tagGongHuiInfo *)(buf1+sizeof(tagRepGongHuiList));
		rlp=(tagRepGongHuiList*)buf1;

		if (rows > 0)
		{
			otl_stream i(100,"select * from vConsortiaCompare with (nolock)",db_);

			while (!i.eof() && idx<rows)
			{
				i>>pinfo[idx].ID>>pinfo[idx].ConID>>pinfo[idx].ConName>>pinfo[idx].Level>>pinfo[idx].UserCount \
					>>pinfo[idx].PlayerID>>pinfo[idx].ChairmanName>>pinfo[idx].Country>>pinfo[idx].CreateTime>>pinfo[idx].Address;
				++idx;
			}
		}

		rlp->serialize(prep->index,1,idx*sizeof(tagGongHuiInfo));
		i_s->start_async_write((char*)rlp,rlp->len);
	}
	catch(otl_exception& p)
	{
		//返回失败包...
		if (!buf1)
		{
			buf1 = new char[sizeof(tagRepGongHuiList)];
			rlp=(tagRepGongHuiList*)buf1;
		}

		rlp->serialize(prep->index,0,0);
		i_s->start_async_write((char*)rlp,rlp->len);
		printf("line(%d) msg:%s\n", __LINE__, p.msg);
	}
}

void web_server_public::OnGetGmUserList(internal_socket * i_s,const char * buf)
{
	header * prep = (header*)buf;
	try
	{
		char pstr[100];
		sprintf(pstr,"select rows=count(*) from GameUser with (nolock) where AccountType=1");
		otl_stream i;
    	i.open((int)strlen(pstr),pstr,db_);

		int rows;
		i>>rows;
		i.close();

		if (rows == 0)
		{
			tagGmUserList * frlp = new tagGmUserList;
			frlp->serialize(prep->index,1,0);
			i_s->start_async_write((char *)frlp,frlp->len);
			return;
		}

		sprintf(pstr,"select AccountID,AccountName,Permission from GameUser with (nolock) where AccountType=1");
		i.open((int)strlen(pstr),pstr,db_);
		char * buf1 = new char[sizeof(tagGmUserList)+rows*sizeof(tagGmUserInfo)];
		tagGmUserList * gmlist= (tagGmUserList*)buf1;
		tagGmUserInfo * gminfo= (tagGmUserInfo*)(buf1+sizeof(tagGmUserList));
		
		int idx=0;
		while (!i.eof() && idx<rows)
		{
			i>>gminfo[idx].account_id>>gminfo[idx].user_name>>gminfo[idx].permission;
			++idx;
		}
		i.close();

		gmlist->serialize(prep->index,1,rows*sizeof(tagGmUserInfo));
		i_s->start_async_write((char*)gmlist,gmlist->len);
	}
	catch(otl_exception& p)
	{
		//返回失败包...
		tagGmUserList * frlp = new tagGmUserList;
		frlp->serialize(prep->index,0,0);
		i_s->start_async_write((char *)frlp,frlp->len);
		printf("line(%d) msg:%s\n", __LINE__, p.msg);
	}
}

void web_server_public::OnGetUserExtInfo(internal_socket * i_s,const char * buf)
{
	tagReqGetUserExtInfo* rep=(tagReqGetUserExtInfo*)buf;
	tagRpsGetUserExtInfo * rps=new tagRpsGetUserExtInfo;
	
	try
	{
		std::string str = "select * from GameUserInfo with (nolock) where AccountID=:f1<int>";
		otl_stream i((int)str.size(),str.c_str(),db_);
		i<<rep->AccountId;

		i.flush();
		if (!i.eof())	//1
		{	
			tagUserExtInfo user;
			i>>user.AccountID>>user.FactName>>user.NameID>>user.Born>>user.TelCode>>user.Sex>>user.Address>>user.PostCode>>user.FromAccountName;
			rps->serialize(rep->index,1,&user);
		}
		else			//9
		{
			rps->serialize(rep->index,9,NULL);
		}
		i.close();
		i_s->start_async_write((char *)rps,rps->len);
	}
	catch (otl_exception& p)
	{
		rps->serialize(rep->index,0,NULL);
		i_s->start_async_write((char *)rps,rps->len);
		printf("line(%d) msg:%s\n", __LINE__, p.msg);
	}
}


void web_server_public::OnUpdateUserExtInfo(internal_socket * i_s,const char * buf)
{
	tagReqUpdateUserExtInfo* req=(tagReqUpdateUserExtInfo*)buf;
	tagRpsUpdateUserExtInfo* rps = new tagRpsUpdateUserExtInfo;
	
	std::string str = "select 1 from GameUserInfo with (nolock) where AccountID=:f1<int>";
	otl_stream i((int)str.size(),str.c_str(),db_);
	i<<req->UserExtInfo.AccountID;
	i.flush();
	int b=i.eof();
	i.close();

	if (b)		//insert
	{
		try
		{
			str.clear();
			str="insert GameUserInfo values(:f1<int>,:f2<char[20]>,:f3<char[20]>,:f4<char[20]>,:f5<char[20]>,:f6<char[5]>,\
				:f7<char[200]>,:f8<char[10]>,:f9<char[16]>)";
			otl_stream o((int)str.size(),str.c_str(),db_);
			o.set_commit(0);
			o<<req->UserExtInfo.AccountID<<req->UserExtInfo.FactName<<req->UserExtInfo.NameID<<req->UserExtInfo.Born<<\
				req->UserExtInfo.TelCode<<req->UserExtInfo.Sex<<req->UserExtInfo.Address<<req->UserExtInfo.PostCode<<req->UserExtInfo.FromAccountName;
		
			o.flush();
			db_.commit();
			rps->serialize(req->index,1);
			i_s->start_async_write((char *)rps,rps->len);
		}
		catch (otl_exception& p)
		{
			db_.rollback();
			rps->serialize(req->index,0);
			i_s->start_async_write((char *)rps,rps->len);
			printf("line(%d) msg:%s\n", __LINE__, p.msg);
		}
	}
	else		//update
	{
		char p[400];
		sprintf(p,"update GameUserInfo set FactName=\'%s\',NameID=\'%s\',TelCode=\'%s\',Born=\'%s\',Sex=\'%s\',Address=\'%s\',PostCode=\'%s\',FromAccountName=\'%s\' \
					where AccountID=%d",req->UserExtInfo.FactName,req->UserExtInfo.NameID,req->UserExtInfo.TelCode,req->UserExtInfo.Born,\
					req->UserExtInfo.Sex,req->UserExtInfo.Address,req->UserExtInfo.PostCode,req->UserExtInfo.FromAccountName,req->UserExtInfo.AccountID);
		int irlt = otl_cursor::direct_exec(db_,p,otl_exception::disabled);
		db_.commit();
		
		rps->serialize(req->index,irlt>0?1:0);
		i_s->start_async_write((char *)rps,rps->len);
	}
}


void web_server_public::OnDataTotal(internal_socket * i_s,const char * buf)
{
	tagReqAddMoneyTotal * rep = (tagReqAddMoneyTotal*)buf;

	char * buf1; 
	char sql[200];
	switch (rep->command)
	{
		case CM_ADDMONEYTOTAL: 
			buf1=new char[sizeof(tagRpsAddMoneyTotal)+rep->PageSize*sizeof(tagAddMoneyTotalInfo)];
			strcpy(sql,"{call sp_pgetCZlist(:f<int>,:f2<int>,:f3<char[1000]>,:f4<char[10]>,:f5<char[20]>,:f6<char[20]>,:f7<char[10]>)}"); 
			break;
		case CM_ADDMONEYTOTALEX:
			buf1=new char[sizeof(tagRpsAddMoneyTotal)+rep->PageSize*sizeof(tagAddMoneyTotalInfoEx)];
			strcpy(sql,"{call sp_pgetCZ2list(:f<int>,:f2<int>,:f3<char[1000]>,:f4<char[10]>,:f5<char[20]>,:f6<char[20]>)}"); 
			break;
		case CM_ADDMONEYTOTALTHIRD:
			buf1=new char[sizeof(tagRpsAddMoneyTotal)+rep->PageSize*sizeof(tagAddMoneyTotalInfoThird)];
			strcpy(sql,"{call sp_pgetCZdetaillist(:f<int>,:f2<int>,:f3<char[1000]>,:f4<char[10]>,:f5<char[20]>,:f6<char[20]>)}"); 
			break;
		case CM_PLAYERLISTTOTAL:
			buf1=new char[sizeof(tagRpsAddMoneyTotal)+rep->PageSize*sizeof(tagPlayerListTotal)];
			strcpy(sql,"{call sp_pgetplayerlist(:f<int>,:f2<int>,:f3<char[1000]>,:f4<char[10]>,:f5<char[20]>,:f6<char[20]>)}"); 
			break;
		case CM_USERLISTTOTAL:
			buf1=new char[sizeof(tagRpsAddMoneyTotal)+rep->PageSize*sizeof(tagUserListTotal)];
			strcpy(sql,"{call sp_pgetuserlist(:f<int>,:f2<int>,:f3<char[1000]>,:f4<char[10]>,:f5<char[20]>,:f6<char[20]>,:f7<char[10]>)}"); 
			break;
		case CM_USERLISTTOTAL3:
			buf1=new char[sizeof(tagRpsAddMoneyTotal)+rep->PageSize*sizeof(tagUserListTotal3)];
			strcpy(sql,"{call sp_pgetuserlist3(:f<int>,:f2<int>,:f3<char[1000]>,:f4<char[10]>,:f5<char[20]>,:f6<char[20]>,:f7<char[10]>)}"); 
			break;
		case CM_PLAYERFRIENDLIST:
			buf1=new char[sizeof(tagRpsAddMoneyTotal)+rep->PageSize*sizeof(tagPlayerFriendList)];
			strcpy(sql,"{call sp_pgetplayerfriendlist(:f<int>,:f2<int>,:f3<char[1000]>,:f4<char[10]>,:f5<char[20]>,:f6<char[20]>)}"); 
			break;
	
	}
	tagRpsAddMoneyTotal* rps=(tagRpsAddMoneyTotal*)buf1;

	try
	{
		otl_stream i(100, sql,db_,otl_implicit_select); 
		i.set_commit(0);

		if ((rep->command == CM_USERLISTTOTAL) ||(rep->command == CM_ADDMONEYTOTAL) || (rep->command == CM_USERLISTTOTAL3))
			i<<rep->CurrPage<<rep->PageSize<<rep->StrWhere<<rep->AccountType<<rep->BeginDate<<rep->EndDate<<rep->SearchType;
		else
			i<<rep->CurrPage<<rep->PageSize<<rep->StrWhere<<rep->AccountType<<rep->BeginDate<<rep->EndDate;

		i.flush();

		switch (rep->command)
		{
		case CM_ADDMONEYTOTAL:
			{
				tagAddMoneyTotalInfo * info=(tagAddMoneyTotalInfo*)(buf1+sizeof(tagRpsAddMoneyTotal));
				int rows=0;
				while (!i.eof())
				{
					i>>info[rows].AccountID>>info[rows].CurrMoney>>info[rows].NewMoney
						>>info[rows].Currgodian>>info[rows].Newgodian>>info[rows].Date;
					++rows;
				}
				i.close();
				db_.commit();
				rps->serialize(rep->index,rep->command,1,rows*sizeof(tagAddMoneyTotalInfo));
				break;
			}
		case CM_ADDMONEYTOTALEX:
			{
				tagAddMoneyTotalInfoEx * info=(tagAddMoneyTotalInfoEx*)(buf1+sizeof(tagRpsAddMoneyTotal));
				int rows=0;
				while (!i.eof())
				{
					i>>info[rows].AccountID>>info[rows].TotalMoney>>info[rows].CurrMonthMoney>>info[rows].CurrDayMoney>>info[rows].Totalgodian
						>>info[rows].CurrMonthgodian>>info[rows].CurrDaygodian>>info[rows].FromAccountID>>info[rows].AccountName;
					++rows;
				}
				i.close();
				db_.commit();
				rps->serialize(rep->index,rep->command,1,rows*sizeof(tagAddMoneyTotalInfoEx));
				break;
			}
		case CM_ADDMONEYTOTALTHIRD:
			{
				tagAddMoneyTotalInfoThird * info=(tagAddMoneyTotalInfoThird*)(buf1+sizeof(tagRpsAddMoneyTotal));
				int rows=0;
				while (!i.eof())
				{
					i>>info[rows].id>>info[rows].AccountID>>info[rows].Money>>info[rows].Godian
						>>info[rows].Score>>info[rows].AddDate>>info[rows].AddType>>info[rows].AccountName;
					++rows;
				}
				i.close();
				db_.commit();
				rps->serialize(rep->index,rep->command,1,rows*sizeof(tagAddMoneyTotalInfoThird));
				break;
			}
		case CM_PLAYERLISTTOTAL:
			{
				tagPlayerListTotal * info=(tagPlayerListTotal*)(buf1+sizeof(tagRpsAddMoneyTotal));
				int rows=0;
				while (!i.eof())
				{
					i>>info[rows].AccountID>>info[rows].RegDate>>info[rows].FromAccountID>>info[rows].Levels>>
						info[rows].TaskNum>>info[rows].LastLoginTime>>info[rows].regip>>info[rows].AccountName>>
						info[rows].cpuid>>info[rows].MilitaryRank;
					++rows;
				}
				i.close();
				db_.commit();
				rps->serialize(rep->index,rep->command,1,rows*sizeof(tagPlayerListTotal));
				break;
			}
		case CM_USERLISTTOTAL:
			{
				tagUserListTotal * info=(tagUserListTotal*)(buf1+sizeof(tagRpsAddMoneyTotal));
				int rows=0;
				while (!i.eof())
				{
					i>>info[rows].FromAccountID>>info[rows].RegNum>>info[rows].TwoLogins>>info[rows].ActiveNum>>info[rows].onelogtimes>>info[rows].Date;
					++rows;
				}
				i.close();
				db_.commit();
				rps->serialize(rep->index,rep->command,1,rows*sizeof(tagUserListTotal));
				break;
			}
		case CM_USERLISTTOTAL3:
			{
				tagUserListTotal3 * info=(tagUserListTotal3*)(buf1+sizeof(tagRpsAddMoneyTotal));
				int rows=0;
				while (!i.eof())
				{
					i>>info[rows].FromAccountID>>info[rows].RegNum>>info[rows].TwoLogins>>info[rows].ActiveNum>>info[rows].onelogtimes>>info[rows].Date>>info[rows].Level3Num>>info[rows].Level15Num>>info[rows].ValidNum;
					++rows;
				}
				i.close();
				db_.commit();
				rps->serialize(rep->index,rep->command,1,rows*sizeof(tagUserListTotal3));
				break;
			}
		case CM_PLAYERFRIENDLIST:
			{
				tagPlayerFriendList * info=(tagPlayerFriendList*)(buf1+sizeof(tagRpsAddMoneyTotal));
				int rows=0;
				while (!i.eof())
				{
					i>>info[rows].AccountID>>info[rows].RegDate>>info[rows].FromAccountID>>info[rows].Levels>>
						info[rows].TaskNum>>info[rows].LastLoginTime>>info[rows].regip>>info[rows].AccountName>>info[rows].cpuid>>info[rows].GoDian;
					++rows;
				}
				i.close();
				db_.commit();
				rps->serialize(rep->index,rep->command,1,rows*sizeof(tagPlayerFriendList));
				break;
			}
		}
		i_s->start_async_write((char *)rps,rps->len);
	}
	catch(otl_exception& p)
	{
		db_.rollback();
		rps->serialize(rep->index,rep->command,0,0);
		i_s->start_async_write((char *)rps,rps->len);
		printf("line(%d) msg:%s\n (command:%d, CurrPage:%d, PageSize:%d, AccountType:%s, BeginDate:%s, EndDate:%s, StrWhere:%s)\n",
			__LINE__, p.msg, rep->command,rep->CurrPage,rep->PageSize,rep->AccountType,rep->BeginDate,rep->EndDate,rep->StrWhere);
	}
}

void  web_server_public::OnGetUserByIDTotal(internal_socket * i_s,const char * buf)
{
	tagReqGetUserByIDTotal * rep = (tagReqGetUserByIDTotal*)buf;

	//printf("OnGetUserByIDTotal参数:\n  FromAccountID:%d\n  BeginDate:%s\n  EndDate:%s\n",
	//	rep->FromAccountID,rep->BeginDate,rep->EndDate);

	char * buf1 = new char[sizeof(tagRpsAddMoneyTotal)+sizeof(tagGetUserByIDTotal)];
	tagGetUserByIDTotal * info=(tagGetUserByIDTotal*)(buf1+sizeof(tagRpsAddMoneyTotal));
	tagRpsAddMoneyTotal* rps=(tagRpsAddMoneyTotal*)buf1;

	try
	{
		otl_stream i(50, "{call sp_pgetuserbypid(:f<int>,:f1<char[20]>,:f3<char[20]>)}",\
			db_,otl_implicit_select); 
		i.set_commit(0);	
		i<<rep->FromAccountID<<rep->BeginDate<<rep->EndDate;
		i.flush();
		char rlt=0;
		if (!i.eof())
		{
			i>>info->i1>>info->i2>>info->i3>>info->i4>>info->i5>>info->i6>>info->i7;
			rlt=1;
		}
		i.close();
		db_.commit();
		
		if(rlt)
			rps->serialize(rep->index,rep->command,1,sizeof(tagGetUserByIDTotal));
		else
			rps->serialize(rep->index,rep->command,1,0);

		i_s->start_async_write((char *)rps,rps->len);
	}
	catch(otl_exception& p)
	{
		db_.rollback();
		rps->serialize(rep->index,rep->command,0,0);
		i_s->start_async_write((char *)rps,rps->len);
		printf("line(%d) msg:%s\n", __LINE__, p.msg);
//		LOG_NO_RETURN(false,("web_server_public::OnGetUserByIDTotal p.msg:%s\n  FromAccountID:%d\n  "
//		"BeginDate:%s\n  EndDate:%s\n",p.msg,rep->FromAccountID,rep->BeginDate,rep->EndDate));
	}
}

void web_server_public::OnGetRecordCount(internal_socket * i_s,const char * buf)
{
	tagReqGetRecordCount * rep = (tagReqGetRecordCount*)buf;
	
	tagRpsGetRecordCount * rps = new tagRpsGetRecordCount;
	try
	{
		otl_stream i(500,rep->SqlStr,db_);
		i.flush();
		if (!i.eof())
		{
			int rcount;
			i>>rcount;
			rps->serialize(rep->index,1,rcount);
		}
		else
		{
			rps->serialize(rep->index,0,0);
		}
		i.close();
		i_s->start_async_write((char *)rps,rps->len);
	}
	catch (otl_exception& p)
	{
		rps->serialize(rep->index,0,0);
		i_s->start_async_write((char *)rps,rps->len);
		printf("line(%d) msg:%s\n", __LINE__, p.msg);
//		LOG_NO_RETURN(false,("web_server_public::OnGetRecordCount p.msg:%s\n  SqlStr:%s\n",p.msg,rep->SqlStr));
	}
}

void web_server_public::OnGetLogInfo(internal_socket * i_s,const char * buf)
{
	tagReqGetLogInfo * prep = (tagReqGetLogInfo *)buf;
	char * buf1 = 0;
	tagRpsGetLogInfo * rsp;
	tagLogInfo * pinfo;

	try
	{
		otl_stream i(100, "{call sp_pgetserverloginfo(:f1<int>,:f2<int>,:f3<char[1000]>,:f4<char[20]>,:f5<char[20]>)}",\
			db_,otl_implicit_select); 
		i.set_commit(0);
		i<<prep->PageIndex << prep->PageSize << prep->StrWhere << prep->BeginDate << prep->EndDate;
		i.flush();

		buf1 = new char[sizeof(tagRpsGetLogInfo) + prep->PageSize * sizeof(tagLogInfo)];
		rsp = (tagRpsGetLogInfo *)buf1;
		pinfo = (tagLogInfo *)(buf1 + sizeof(tagRpsGetLogInfo));

		int rows = 0;
		while (!i.eof())
		{
			i >> pinfo[rows].servername >> pinfo[rows].info 
				>> pinfo[rows].address >> pinfo[rows].datetime;
			++rows;
		}

		i.close();
		db_.commit();

		rsp->serialize(prep->index, 1, rows*sizeof(tagLogInfo));
		i_s->start_async_write((char*)rsp,rsp->len);
	}
	catch(otl_exception& p)
	{
		//返回失败包...
		if (!buf1)
		{
			buf1 = new char[sizeof(tagRpsGetLogInfo)];
			rsp=(tagRpsGetLogInfo*)buf1;
		}

		rsp->serialize(prep->index,0,0);
		i_s->start_async_write((char*)rsp,rsp->len);
		printf("line(%d) msg:%s\n", __LINE__, p.msg);
//		LOG_NO_RETURN(false,("data_server_public::OnGetPlayerList p.msg:%s \n",p.msg));
		return;
	}
}

void web_server_public::OnGetLogOnline(internal_socket * i_s,const char * buf)
{
	tagReqGetLogOnline * prep = (tagReqGetLogOnline *)buf;
	char * buf1 = 0;
	tagRpsGetLogOnline * rsp;
	tagLogOnline * pinfo;

	try
	{
		otl_stream i(100, "{call sp_pgetserverLogOnline(:f1<int>,:f2<int>,:f3<char[1000]>,:f4<char[20]>,:f5<char[20]>)}",\
			db_,otl_implicit_select); 
		i.set_commit(0);
		i<<prep->PageIndex << prep->PageSize << prep->StrWhere << prep->BeginDate << prep->EndDate;
		i.flush();

		buf1 = new char[sizeof(tagRpsGetLogOnline) + prep->PageSize * sizeof(tagLogOnline)];
		rsp = (tagRpsGetLogOnline *)buf1;
		pinfo = (tagLogOnline *)(buf1 + sizeof(tagRpsGetLogOnline));

		int rows = 0;
		while (!i.eof())
		{
			i >> pinfo[rows].servername >> pinfo[rows].online 
				>> pinfo[rows].datetime >> pinfo[rows].address;
			++rows;
		}

		i.close();
		db_.commit();

		rsp->serialize(prep->index, 1, rows*sizeof(tagLogOnline));
		i_s->start_async_write((char*)rsp,rsp->len);
	}
	catch(otl_exception& p)
	{
		//返回失败包...
		if (!buf1)
		{
			buf1 = new char[sizeof(tagRpsGetLogOnline)];
			rsp=(tagRpsGetLogOnline*)buf1;
		}

		rsp->serialize(prep->index,0,0);
		i_s->start_async_write((char*)rsp,rsp->len);
		printf("line(%d) msg:%s\n", __LINE__, p.msg);
		return;
	}
}

void web_server_public::OnGetLogOnlineEx(internal_socket * i_s,const char * buf)
{
	tagReqGetLogOnlineEx * prep = (tagReqGetLogOnlineEx *)buf;
	char * buf1 = 0;
	tagRpsGetLogOnlineEx * rsp;
	tagLogOnlineEx * pinfo;

	try
	{
		otl_stream i(100, "{call sp_pgetonlinelog(:f1<int>,:f2<int>,:f3<char[1000]>,:f4<int>,:f5<char[20]>,:f6<char[20]>)}",\
			db_,otl_implicit_select); 
		i.set_commit(0);
		i<<prep->PageIndex << prep->PageSize << prep->StrWhere << prep->accounttype << prep->BeginDate << prep->EndDate;
		i.flush();

		buf1 = new char[sizeof(tagRpsGetLogOnlineEx) + prep->PageSize * sizeof(tagLogOnlineEx)];
		rsp = (tagRpsGetLogOnlineEx *)buf1;
		pinfo = (tagLogOnlineEx *)(buf1 + sizeof(tagRpsGetLogOnlineEx));

		int rows = 0;
		while (!i.eof())
		{
			i >> pinfo[rows].address >> pinfo[rows].datetime >> pinfo[rows].avgnum;
			++rows;
		}

		i.close();
		db_.commit();

		rsp->serialize(prep->index, 1, rows*sizeof(tagLogOnlineEx));
		i_s->start_async_write((char*)rsp,rsp->len);
	}
	catch(otl_exception& p)
	{
		//返回失败包...
		if (!buf1)
		{
			buf1 = new char[sizeof(tagRpsGetLogOnlineEx)];
			rsp=(tagRpsGetLogOnlineEx*)buf1;
		}

		rsp->serialize(prep->index,0,0);
		i_s->start_async_write((char*)rsp,rsp->len);
		printf("line(%d) msg:%s\n", __LINE__, p.msg);
		return;
	}
}

void web_server_public::OnGetGodianLog(internal_socket * i_s,const char * buf)
{
	tagReqGetGodianLog * prep = (tagReqGetGodianLog *)buf;
	char * buf1 = 0;
	tagRpsGetGodianLog * rsp;
	tagGodianLog * pinfo;

	try
	{
		otl_stream i(100, "{call sp_pgetgodianlog(:f1<int>,:f2<int>,:f3<char[1000]>,:f4<int>,:f5<char[50]>,:f6<char[50]>)}",db_,otl_implicit_select); 
		i.set_commit(0);
		i<<prep->PageIndex << prep->PageSize << prep->StrWhere << prep->accounttype << prep->BeginDate << prep->EndDate;

		i.flush();

		buf1 = new char[sizeof(tagRpsGetGodianLog) + prep->PageSize * sizeof(tagGodianLog)];
		rsp = (tagRpsGetGodianLog *)buf1;
		pinfo = (tagGodianLog *)(buf1 + sizeof(tagRpsGetGodianLog));

		int rows = 0;
		while (!i.eof())
		{
			i >> pinfo[rows].id >> pinfo[rows].playerid >> pinfo[rows].godian >> pinfo[rows].usetype >> pinfo[rows].logdate;
			++rows;
		}

		i.close();
		db_.commit();

		rsp->serialize(prep->index, 1, rows*sizeof(tagGodianLog));
		i_s->start_async_write((char*)rsp,rsp->len);
	}
	catch(otl_exception& p)
	{
		//返回失败包...
		if (!buf1)
		{
			buf1 = new char[sizeof(tagRpsGetGodianLog)];
			rsp=(tagRpsGetGodianLog*)buf1;
		}

		rsp->serialize(prep->index,0,0);
		i_s->start_async_write((char*)rsp,rsp->len);
		printf("line(%d) msg:%s\n", __LINE__, p.msg);
		return;
	}
}

void web_server_public::OnGetLogPerf(internal_socket * i_s,const char * buf)
{
	tagReqGetLogPerf * prep = (tagReqGetLogPerf *)buf;
	char * buf1 = 0;
	tagRpsGetLogPerf * rsp;
	tagLogPerf * pinfo;

	try
	{
		otl_stream i(100, "{call sp_pgetserverLogPerf(:f1<int>,:f2<int>,:f3<char[1000]>,:f4<char[20]>,:f5<char[20]>)}",\
			db_,otl_implicit_select); 
		i.set_commit(0);
		i<<prep->PageIndex << prep->PageSize << prep->StrWhere << prep->BeginDate << prep->EndDate;
		i.flush();

		buf1 = new char[sizeof(tagRpsGetLogPerf) + prep->PageSize * sizeof(tagLogPerf)];
		rsp = (tagRpsGetLogPerf *)buf1;
		pinfo = (tagLogPerf *)(buf1 + sizeof(tagRpsGetLogPerf));

		int rows = 0;
		while (!i.eof())
		{
			i >> pinfo[rows].servername >> pinfo[rows].bandwidth 
				>> pinfo[rows].broadcast >> pinfo[rows].cpu
				>> pinfo[rows].mem >> pinfo[rows].vmem
				>> pinfo[rows].datetime >> pinfo[rows].address;
			++rows;
		}

		i.close();
		db_.commit();

		rsp->serialize(prep->index, 1, rows*sizeof(tagLogPerf));
		i_s->start_async_write((char*)rsp,rsp->len);
	}
	catch(otl_exception& p)
	{
		//返回失败包...
		if (!buf1)
		{
			buf1 = new char[sizeof(tagRpsGetLogPerf)];
			rsp=(tagRpsGetLogPerf*)buf1;
		}


		rsp->serialize(prep->index,0,0);
		i_s->start_async_write((char*)rsp,rsp->len);
		printf("line(%d) msg:%s\n", __LINE__, p.msg);
//		LOG_NO_RETURN(false,("data_server_public::OnGetPlayerList p.msg:%s \n",p.msg));
		return;
	}
}

void web_server_public::OnGetResultByProc(internal_socket * i_s,const char * buf)
{
	tagReqGetResultByProc * prep = (tagReqGetResultByProc *)buf;
	char * buf1 = 0;
	tagRpsGetResultByProc * rsp;

	try
	{
		char str[512];
		sprintf(str, "{call %s(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\')}", 
			prep->procname, prep->p1,prep->p2,prep->p3,prep->p4,prep->p5);

		otl_stream i(512, str, db_,otl_implicit_select); 
		i.set_commit(0);
		i.flush();

		buf1 = new char[sizeof(tagRpsGetResultByProc)];
		rsp = (tagRpsGetResultByProc *)buf1;
		rsp->serialize(prep->index, 1, "");
		i >> rsp->result;
		i.close();
		db_.commit();
		i_s->start_async_write((char*)rsp,rsp->len);
	}
	catch(otl_exception& p)
	{
		//返回失败包...
		if (!buf1)
		{
			buf1 = new char[sizeof(tagRpsGetResultByProc)];
			rsp=(tagRpsGetResultByProc*)buf1;
		}

		rsp->serialize(prep->index, 0, "");
		i_s->start_async_write((char*)rsp,rsp->len);
		printf("line(%d) msg:%s\n", __LINE__, p.msg);
//		LOG_NO_RETURN(false,("data_server_public::OnGetPlayerList p.msg:%s \n",p.msg));
		return;
	}
}



