#ifndef __um_web_H__
#define __um_web_H__

#include "../src/um_web_svc/include/header.h"
#include "../src/um_web_svc/common/um_global_macro.h"

enum {
	username_size=16,
	password_size=16,
	email_size=100,
	question_size=100,
	answer_size=50,
	fromip_size=20,
	fromurl_size=150,
	max_rows=20,
	factname_size=20,
	nameid_size=20,
	born_size=20,
	telcode_size=20,
	sex_size=5,
	address_size=200,
	postcode_size=10,
	strwhere_size=1000,
	accounttype_size=10,
	date_size=20,
	maxpage_size=50,
	dateex_size=24,
	sqlstr_size=500
};

enum emType
{
	emUpdatePassword=4,
	emAddMoney=5,
	emSubMoney=6,
	emUpdateEmail=9,
	emSubScore=14,
	emUpdatePasswordNoCheck=15,
	emUpdatePasswordExt=16
};
struct tagLogRec
{
	int	TypeID;				//类别ID
	char TypeName[32];		//类别名称
	int playerid;
	int godian;
	char usetype[50];
	char Data[100];			//内容
	__int64 LogDate;		//操作日期
	short bSucceed;			//是否成功 --成功为1，其它为错误代码
	inline void serialize(int id,const char * name,const char * data,__int64 logdate,short succ, int playerid=0, int godian=0, const char * usetype=0);
};
inline 
void tagLogRec::serialize(int id,const char * name,const char * data,__int64 logdate,short succ,int playerid, int godian, const char * usetype)
{
	TypeID= id;
	strcpy(TypeName,name);
	this->playerid = playerid;
	this->godian = godian;
	if (!usetype)
		strcpy(this->usetype, "");
	else
		strcpy(this->usetype, usetype);
	LogDate=logdate;
	strcpy(Data,data);
	bSucceed=succ;
}


#pragma pack(push,1)


////////////////////////////////////////////////////////////////////////////////////////////
//接口结构部分声明

struct tagLogInfo
{
	char servername[128];
	char info[300];
	char address[50];
	char datetime[50];
};

struct tagLogOnline
{
	char servername[128];
	int online;
	char datetime[50];
	char address[50];
};

// add by wind
struct tagLogOnlineEx
{
	char address[50];
	char datetime[50];
	int avgnum;
};

struct tagGodianLog
{
	int id;
	int playerid;
	int godian;
	char usetype[50];
	char logdate[50];
};

struct tagLogPerf
{
	char servername[128];
	int bandwidth;
	int broadcast;
	int cpu;
	int mem;
	int vmem;
	char datetime[50];
	char address[50];
};

//个人排行信息
struct tagPlayerInfo
{
	int id;
	int PlayerID;
	char PlayerName[32];	
	int Country;
	int OfficerRace;
	int Level;
	int MilitaryRank;
	int Money;
	int ResourceCount;
	int CityCount;
	int GeneralCount;
	int Credit;
	char Address[50];
};

//工会排行信息
struct tagGongHuiInfo
{
	int ID;
	int ConID;
	char ConName[32];
	int Level;
	int UserCount;
	int PlayerID;
	char ChairmanName[32];
	int Country;
	__int64 CreateTime;
	char Address[50];
};

//英雄表信息
struct tagGetHeroList
{
	int officerid_;
	int playerid_;
	char officername_[24];
	int officerrace_;
	int level_;
	int militaryrank_;
	int jobid_;
	int faceid_;
	int exp_;
	int credit_;
	int enemycount_;
	int generalcount_;
	int mapid_;
	int cityid_;
	int armyid_;
	int state_;
	int stamina_;
	int power_;
	int celerity_;
	int wit_;
	int techdot_;
	int attack_;
	int physicrecovery_;
	int magicrecovery_;
	int actiondot_;
	__int64 updatetime_;
	__int64 gbhtime_;
	int levelstamina_;
	int levelpower_;
	int levelcelerity_;
	int levelwit_;
	int equipstamina_;
	int equippower_;
	int equipcelerity_;
	int equipwit_;
	int magicattack_;
	int herohp_;
	int injurerate_;
	int capturerate_;
	int beinjurerate_;
	int becapturerate_;
	int forcerate_;
	int lsttotalattri_;
	int lstmainattri_;
	int lststamina_;
	int grouprate_;
};


//GM账户信息
struct tagGmUserInfo
{
	int account_id;
	char user_name[username_size];
	int permission;
};

//1.充值统计信息结构1
struct tagAddMoneyTotalInfo
{
	int AccountID;
	float CurrMoney;
	float NewMoney;
	int Currgodian;
	int Newgodian;
	char Date[20];
};

//2.充值统计信息结构2
struct tagAddMoneyTotalInfoEx
{
	int AccountID;
	float TotalMoney;
	float CurrMonthMoney;
	float CurrDayMoney;
	int Totalgodian;
	int CurrMonthgodian;
	int CurrDaygodian;
	int FromAccountID;
	char AccountName[32];
};

//3.充值统计信息结构3
struct tagAddMoneyTotalInfoThird
{
	int id;
	int AccountID;
	float Money;
	int Godian;
	int Score;
	char AddDate[dateex_size];
	short AddType;
	char AccountName[32];
};

//4玩家统计
struct tagPlayerListTotal
{
	int AccountID;
	char RegDate[dateex_size];
	int FromAccountID;
	int Levels;
	int TaskNum;
	char LastLoginTime[dateex_size];
	char regip[15];
	char AccountName[32];
	int cpuid;
	int MilitaryRank;

};
//6userlist
struct tagUserListTotal
{
	int FromAccountID;
	int RegNum;
	int TwoLogins;
	int ActiveNum;
	int onelogtimes;
	char Date[20];
};

struct tagUserListTotal3
{
	int FromAccountID;
	int RegNum;
	int TwoLogins;
	int ActiveNum;
	int onelogtimes;
	char Date[20];
	int Level3Num;
	int Level15Num;
	int ValidNum;
};

//6.reference  sp_pgetuserbyid
struct tagGetUserByIDTotal
{
	int i1;
	int i2;
	int i3;
	int i4;
	int i5;
	int i6;
	int i7;
};

struct tagPlayerFriendList
{
	int AccountID;
	char RegDate[dateex_size];
	int FromAccountID;
	int Levels;
	int TaskNum;
	char LastLoginTime[dateex_size];
	char regip[15];
	char AccountName[32];
	int cpuid;
	int GoDian;
};

////////////////////////////////////////////////////////////////////////////////////////////

//普通请求回复包
struct tagRepResult:header
{
	int bsucceed; //1成功，其它失败
	inline void serialize(int index,int command,int succeed);
};

//检验用户名是否注册
struct tagCheckUser:header
{
	char user_name[username_size];
	inline void serialize(int index,const char * username);
};

//注册用户信息
struct tagRegUserInfo
{
	char user_name[username_size];
	char password[password_size];
	unsigned int ip;
	char email[email_size];
	int fromAccountID;
	char from_url[fromurl_size];
	char qq[100];
	char memo[100];
	int accounttype;
	inline void serialize(const char* username,const char* pwd,unsigned int ip,const char* em,const int accountid,const char * fromurl, const char * qq, const char * memo, int accounttype);
};


//用户注册
struct tagRegUser:header
{
	tagRegUserInfo userinfo;
	inline void serialize(int index, const tagRegUserInfo& userinfo_);
	inline void serialize(int index,const char* username,const char* pwd,unsigned int ip,const char* em,const int accountid,const char * fromurl, const char * qq, const char * memo,int accounttype);
};


//用户登陆
struct tagUserLogin:header
{
	char user_name[username_size];
	char password[password_size];
	unsigned int ip;
	inline void serialize(int index,const char* username,const char* pwd,unsigned int ip);
};


//修改用户密码
struct tagModifyPassword:header
{
	char user_name[username_size];
	char old_password[password_size];
	char new_password[password_size];
	inline void serialize(int index,const char* username,const char* oldpassword,const char* newpassword);
};

//修改用户密码
struct tagModifyPasswordNoCheck:header
{
	char user_name[username_size];
	char new_password[password_size];
	inline void serialize(int index,const char* username,const char* newpassword);
};

//修改用户密码--带ques,ans
struct tagModifyPasswordExt:header
{
	char user_name[username_size];
	char old_password[password_size];
	char new_password[password_size];
	char question[question_size];
	char answer[answer_size];
	inline void serialize(int index,const char* username,const char* oldpassword,const char* newpassword,const char * ques,const char * ans);
};


//充值
struct tagAddMoney:header
{
	char user_name[username_size];
	float money;
	int godian;
	int score;
	int addtype;
	inline void serialize(int index,const char* username, float money, int godian, int score, int addtype);
};

//扣钱
struct tagSubMoney:header
{
	int account_id;
	int GoDian;
	char desc[50];
	inline void serialize(int index,int accountid,int GoDian, const char * desc);
};

//查询GO点
struct tagGetMoney:header
{
	int account_id;
	inline void serialize(int index,int accountid);
};

//回复查询GO点
struct tagRepGetMoney:header
{
	int bsucceed ;
	int GoDian;
	inline void serialize(int index,int succeed,int GoDian);
};

struct tagGetScore:header
{
	int account_id;
	inline void serialize(int index,int accountid);
};

struct tagSubScore:header
{
	int account_id;
	int score;
	inline void serialize(int index,int accountid,int score);
};

struct tagRepGetScore:header
{
	int bsucceed ;
	int score;
	inline void serialize(int index,int succeed,int score);
};

//读取问题答案
struct tagGetAnswer:header
{
	char user_name[username_size];
	inline void serialize(int index,const char * username);
};

//回复读取问题答案
struct tagRepGetAnswer:header
{
	int bsucceed;
	char answer[answer_size];
	inline void serialize(int index,int succeed,const char * ans);
};

//读取邮件
struct tagGetEmail:header
{
	char user_name[username_size];
	inline void serialize(int index,const char * username);
};

//回复读取邮件
struct tagRepGetEmail:header
{
	int bsucceed;
	char email[email_size];
	inline void serialize(int index,int succeed,const char * email_);
};

//更改邮件
struct tagUpdateEmail:header
{
	char user_name[username_size];
	char old_email[email_size];
	char new_email[email_size];
	inline void serialize(int index,const char * username,const char * oldemail,const char * newemail);
};

//玩家任务信息
struct tagPlayerTaskInfo
{
	int PlayerID;
	int TaskID;
	int State;
	__int64 TaskTime;
	int OpType;    //1插入 2更改
};

//获取玩家任务信息
struct tagReqGetPlayerTask:header
{
	inline void serialize(int index);
};

//获取英雄表
struct tagReqGetHeroList:header
{
	inline void serialize(int index);
};

//获取英雄表
struct tagRpsGetHeroList:header
{
	char bSucceed;
	unsigned int BufLen;
	inline void serialize(int index,char bsucceed,int buflen);
};


//返回玩家任务信息
struct tagRpsGetPlayerTask:header
{
	char bSucceed;
	unsigned int BufLen;
	inline void serialize(int index,char bsucceed,int buflen);
};

//回写玩家任务信息
struct tagBackWriteTask:header
{
	inline void serialize(int index);
};

//获取个人排行
struct tagGetPlayerList:header
{
	int maxrows;   //最大行数
	inline void serialize(int index,int maxrow);
};

//返回个人排行
struct tagRepPlayerList:header
{
	char bSucceed;
	int bufflen;
	inline void serialize(int index,char succeed,int blen);
};

//获取公会排行
struct tagGetGongHuiList:header
{
	int maxrows;
	inline void serialize(int index,int maxrow);
};

//返回公会排行
struct tagRepGongHuiList:header
{
	char bSucceed;
	int bufflen;
	inline void serialize(int index,char succeed,int blen);
};


//GM账户列表
struct tagGmUserList:header
{
	char bSucceed;
	int buflen;
	inline void serialize(int index,char succeed,int blen);
};

//账户扩展信息
struct tagUserExtInfo
{
	int AccountID;
	char FactName[factname_size];
	char NameID[nameid_size];
	char Born[born_size];
	char TelCode[telcode_size];
	char Sex[sex_size];
	char Address[address_size];
	char PostCode[postcode_size];
	char FromAccountName[username_size];
	inline void serialize(int accountid,const char * factname,const char * nameid,const char * born,const char * telcode,\
		const char * sex,const char * address,const char * postcode,const char * fromaccountname);
};

//请求更改用户扩展信息
struct tagReqUpdateUserExtInfo:header
{
	tagUserExtInfo UserExtInfo;
	inline void serialize(int index,const tagUserExtInfo* userextinfo);
};

//回复请求更改用户扩展信息
struct tagRpsUpdateUserExtInfo:header
{
	char bSucceed;  //1成功有信息,其它失败
	inline void serialize(int index,char succeed);
};

//请求获取用户扩展信息
struct tagReqGetUserExtInfo:header
{
	int AccountId;
	inline void serialize(int index,int accountid);
};

//回复请求获取用户扩展信息
struct tagRpsGetUserExtInfo:header
{
	char bSucceed;  //1成功有信息,9成功但无信息,其它失败
	tagUserExtInfo UserExtInfo;
	inline void serialize(int index,char succeed,const tagUserExtInfo* userextinfo);
};

/*以下是统计部分协议定义*/

struct tagReqGetUserByIDTotal:header   //回复协议(tagRpsAddMoneyTotal)
{
	int FromAccountID;
	char BeginDate[date_size];
	char EndDate[date_size];
	inline void serialize(int index,int fromid,const char * d1,const char * d2);
};

struct tagReqGetLogInfo:header
{
	int PageIndex;
	int PageSize;
	char StrWhere[strwhere_size];
	char BeginDate[50];
	char EndDate[50];
	inline void serialize(int index,int PageIndex, int PageSize, const char * StrWhere, const char * BeginDate, const char * EndDate);
};

struct tagRpsGetLogInfo:header
{
	char bSucceed;
	int BufLen;
	inline void serialize(int index,char bsucceed,int BufLen);
};

struct tagReqGetLogOnline:header
{
	int PageIndex;
	int PageSize;
	char StrWhere[strwhere_size];
	char BeginDate[50];
	char EndDate[50];
	inline void serialize(int index,int PageIndex, int PageSize, const char * StrWhere, const char * BeginDate, const char * EndDate);
};

struct tagReqGetLogOnlineEx:header
{
	int PageIndex;
	int PageSize;
	char StrWhere[strwhere_size];
	int accounttype;
	char BeginDate[50];
	char EndDate[50];
	inline void serialize(int index,int PageIndex, int PageSize, const char * StrWhere, 
		int accounttype, const char * BeginDate, const char * EndDate);
};

struct tagReqGetGodianLog:header
{
	int PageIndex;
	int PageSize;
	char StrWhere[strwhere_size];
	int accounttype;
	char BeginDate[50];
	char EndDate[50];

	inline void serialize(int index,int PageIndex, int PageSize, const char * StrWhere, 
		int accounttype, const char * BeginDate, const char * EndDate);
};


struct tagRpsGetGodianLog:header
{
	char bSucceed;
	int BufLen;

	inline void serialize(int index,char bsucceed,int BufLen);
};

struct tagRpsGetLogOnline:header
{
	char bSucceed;
	int BufLen;
	inline void serialize(int index,char bsucceed,int BufLen);
};

struct tagRpsGetLogOnlineEx:header
{
	char bSucceed;
	int BufLen;
	inline void serialize(int index,char bsucceed,int BufLen);
};

struct tagReqGetLogPerf:header
{
	int PageIndex;
	int PageSize;
	char StrWhere[strwhere_size];
	char BeginDate[50];
	char EndDate[50];
	inline void serialize(int index,int PageIndex, int PageSize, const char * StrWhere, const char * BeginDate, const char * EndDate);
};

struct tagRpsGetLogPerf:header
{
	char bSucceed;
	int BufLen;
	inline void serialize(int index,char bsucceed,int BufLen);
};

//账户充值统计
struct tagReqAddMoneyTotal:header   
{
	int CurrPage;
	int PageSize;
	char StrWhere[strwhere_size];
	char AccountType[accounttype_size];
	char BeginDate[date_size];
	char EndDate[date_size];
	char SearchType[10];
	inline void serialize(int index,int cmd,int currpage,int pagesize,const char * strwhere,
		const char * atype,const char * bdate,const char * edate, const char * searchtype);
};


struct tagRpsAddMoneyTotal:header
{
	char bSucceed;
	int BufLen;
	inline void serialize(int index,int cmd,char bsucceed,int bufflen);
};

//GetRecordCount
struct tagReqGetRecordCount:header
{
	char SqlStr[sqlstr_size];
	inline void serialize(int index,const char * strsql);
};

struct tagRpsGetRecordCount:header
{
	char bSucceed;
	int RecCount;
	inline void serialize(int index,char bsucceed,int reccount);
};

struct tagReqGetResultByProc:header   
{
	char procname[50];
	char p1[50];
	char p2[50];
	char p3[50];
	char p4[50];
	char p5[100];
	inline void serialize(int index, const char * procname,const char * p1,const char * p2,const char * p3,const char * p4,const char * p5);
};

struct tagRpsGetResultByProc:header
{
	char bsucceed;
	char result[50];
	inline void serialize(int index, char bsucceed, const char * result);
};

#pragma pack(pop)


inline 
void tagRepResult::serialize(int index,int command,int succeed)
{
	this->bsucceed=succeed;
	header::serialize(sizeof(*this),index,command);
}

inline 
void tagCheckUser::serialize(int index,const char * username)
{
	strcpy(this->user_name,username);
	header::serialize(sizeof(*this),index,CM_CHECK_USER);
}

inline 
void tagRegUserInfo::serialize(const char* username,const char* pwd,unsigned int ip,const char* em,const int accountid,const char * fromurl, const char * qq, const char * memo, int accounttype)
{
	strcpy(this->user_name,username);
	strcpy(this->password,pwd);
	this->ip=ip;
	strcpy(this->email,em);
	this->fromAccountID=accountid;
	strcpy(this->from_url,fromurl);
	strcpy(this->qq, qq);
	strcpy(this->memo, memo);
	this->accounttype = accounttype;
}

inline 
void tagRegUser::serialize(int index,const tagRegUserInfo& userinfo_)
{
	this->userinfo=userinfo;
	header::serialize(sizeof(*this),index,CM_REGUSER);
}

inline 
void tagRegUser::serialize(int index,const char* username,const char* pwd,unsigned int ip,const char* em,const int accountid,const char * fromurl, const char * qq, const char * memo,int accounttype)
{
	strcpy(this->userinfo.user_name,username);
	strcpy(this->userinfo.password,pwd);
	this->userinfo.ip=ip;
	strcpy(this->userinfo.email,em);
	this->userinfo.fromAccountID=accountid;
	strcpy(this->userinfo.from_url,fromurl);
	if (qq) strcpy(this->userinfo.qq,qq);
	if (memo) strcpy(this->userinfo.memo,memo);
	this->userinfo.accounttype = accounttype;
	header::serialize(sizeof(*this),index,CM_REGUSER);
}

inline 
void tagUserLogin::serialize(int index,const char* username,const char* pwd,unsigned int ip)
{
	strcpy(this->user_name,username);
	strcpy(this->password,pwd);
	this->ip=ip;
	header::serialize(sizeof(*this),index,CM_USER_LOING);
}

inline 
void tagModifyPassword::serialize(int index,const char* username,const char* oldpassword,const char* newpassword)
{
	strcpy(this->user_name,username);
	strcpy(this->old_password,oldpassword);
	strcpy(this->new_password,newpassword);
	header::serialize(sizeof(*this),index,CM_MODIFY_PASSWORD);
}

inline 
void tagModifyPasswordExt::serialize(int index,const char* username,const char* oldpassword,const char* newpassword,const char * ques,const char * ans)
{
	strcpy(this->user_name,username);
	strcpy(this->old_password,oldpassword);
	strcpy(this->new_password,newpassword);
	strcpy(this->question,ques);
	strcpy(this->answer,ans);
	header::serialize(sizeof(*this),index,CM_MODIFY_PASSWORD_EXT);
}

inline 
void tagModifyPasswordNoCheck::serialize(int index,const char* username,const char* newpassword)
{
	strcpy(this->user_name,username);
	strcpy(this->new_password,newpassword);
	header::serialize(sizeof(*this),index,CM_MODIFY_PASSWORD_NO);
}

inline
void tagAddMoney::serialize(int index,const char * UserName,float money,int godian,int score,int addtype)
{
	strcpy(this->user_name,UserName);
	this->money=money;
	this->godian=godian;
	this->score=score;
	this->addtype=addtype;
	header::serialize(sizeof(*this),index,CM_ADDMONEY);
}

inline 
void tagSubMoney::serialize(int index,int accountid,int GoDian, const char * desc)
{
	this->account_id = accountid;
	this->GoDian=GoDian;
	if (!desc) 
		strcpy(this->desc, "");
	else
		strcpy(this->desc, desc);

	header::serialize(sizeof(*this),index,CM_SUBMONEY);
}


inline 
void tagUpdateEmail::serialize(int index,const char * username,const char * oldemail,const char * newemail)
{
	strcpy(this->user_name,username);
	strcpy(this->old_email,oldemail);
	strcpy(this->new_email,newemail);
	header::serialize(sizeof(*this),index,CM_UPDATEEMAIL);
}

inline 
void tagGetAnswer::serialize(int index,const char * username)
{
	strcpy(this->user_name,username);
	header::serialize(sizeof(*this),index,CM_GETANSWER);
}

inline 
void tagRepGetAnswer::serialize(int index,int succeed,const char * ans)
{
	this->bsucceed = succeed;
	if (ans) 
		strcpy(this->answer,ans);
	else
		this->answer[0]='\0';
	header::serialize(sizeof(*this),index,CM_REP_GETANSWER);
}

inline 
void tagGetEmail::serialize(int index,const char * username)
{
	strcpy(this->user_name,username);
	header::serialize(sizeof(*this),index,CM_GETEMAIL);
}

inline 
void tagRepGetEmail::serialize(int index,int succeed,const char * email_)
{
	this->bsucceed=succeed;
	if (email_)
		strcpy(this->email,email_);
	else 
		this->email[0]='\0';
	header::serialize(sizeof(*this),index,CM_GETEMAIL);
}

inline 
void tagGetMoney::serialize(int index,int accountid)
{
	this->account_id=accountid;
	header::serialize(sizeof(*this),index,CM_GETMONEY);
}

inline 
void tagRepGetMoney::serialize(int index,int succeed,int GoDian)
{
	this->bsucceed = succeed;
	this->GoDian = GoDian;
	header::serialize(sizeof(*this),index,CM_REP_GETMONEY);
}

inline 
void tagGetPlayerList::serialize(int index,int maxrow)
{
	this->maxrows = maxrow;
	header::serialize(sizeof(*this),index,CM_PLAYERLIST);
}

inline 
void tagGetGongHuiList::serialize(int index,int maxrow)
{
	this->maxrows = maxrow;
	header::serialize(sizeof(*this),index,CM_GONGHUILIST);
}

inline 
void tagRepPlayerList::serialize(int index,char succeed,int blen)
{
	this->bSucceed = succeed;
	this->bufflen =blen;
	header::serialize(sizeof(*this)+blen,index,CM_PLAYERLIST);
}

inline 
void tagRepGongHuiList::serialize(int index,char succeed,int blen)
{
	this->bSucceed = succeed;
	this->bufflen = blen;
	header::serialize(sizeof(*this)+blen,index,CM_GONGHUILIST);
}

inline
void tagReqGetPlayerTask::serialize(int index)
{
	header::serialize(sizeof(*this),index,CM_GETPLAYERTASK);
}

inline
void tagReqGetHeroList::serialize(int index)
{
	header::serialize(sizeof(*this),index,CM_GETHEROLIST);
}

inline
void tagRpsGetHeroList::serialize(int index,char bsucceed, int buflen)
{
	this->bSucceed=bsucceed;
	this->BufLen=buflen;
	header::serialize(sizeof(*this)+buflen,index,CM_RPSGETHEROLIST);
}

inline
void tagRpsGetPlayerTask::serialize(int index,char bsucceed, int buflen)
{
	this->bSucceed=bsucceed;
	this->BufLen=buflen;
	header::serialize(sizeof(*this)+buflen,index,CM_GETPLAYERTASK);
}

inline 
void tagBackWriteTask::serialize(int index)
{
	header::serialize(sizeof(*this),index,CM_BACKWRITETASK);
}

inline 
void tagGetScore::serialize(int index,int accountid)
{
	this->account_id=accountid;
	header::serialize(sizeof(*this),index,CM_GETSCORE);
}

inline 
void tagSubScore::serialize(int index,int accountid,int score)
{	
	this->account_id = accountid;
	this->score = score;
	header::serialize(sizeof(*this),index,CM_SUBSCORE);
}

inline 
void tagRepGetScore::serialize(int index,int succeed,int socre)
{
	this->bsucceed=succeed;
	this->score=score;
	header::serialize(sizeof(*this),index,CM_REP_GETSCORE);
}

inline 
void tagGmUserList::serialize(int index,char succeed,int blen)
{
	this->bSucceed = succeed;
	this->buflen=blen;
	header::serialize(sizeof(*this)+blen,index,CM_GMUSERLIST);
}

inline 
void tagUserExtInfo::serialize(int accountid,const char * factname,const char * nameid,const char * born,const char * telcode,\
		const char * sex,const char * address,const char * postcode,const char * fromaccountname)
{
	this->AccountID=accountid;
	strcpy(this->FactName,factname);
	strcpy(this->NameID,nameid);
	strcpy(this->Born,born);
	strcpy(this->TelCode,telcode);
	strcpy(this->Sex,sex);
	strcpy(this->Address,address);
	strcpy(this->PostCode,postcode);
	strcpy(this->FromAccountName,fromaccountname);
}

inline 
void tagReqUpdateUserExtInfo::serialize(int index,const tagUserExtInfo* userextinfo)
{
	if (userextinfo)
		this->UserExtInfo=*userextinfo;
	header::serialize(sizeof(*this),index,CM_UPDATEUSEREXTINFO);
}

inline
void tagRpsUpdateUserExtInfo::serialize(int index,char succeed)
{
	this->bSucceed=succeed;
	header::serialize(sizeof(*this),index,CM_UPDATEUSEREXTINFO);
}

inline 
void tagReqGetUserExtInfo::serialize(int index,int accountid)
{
	this->AccountId=accountid;
	header::serialize(sizeof(*this),index,CM_GETUSEREXTINFO);
}

inline 
void tagRpsGetUserExtInfo::serialize(int index,char succeed,const tagUserExtInfo* userextinfo)
{
	this->bSucceed=succeed;
	if (succeed==1)
	{
		this->UserExtInfo=*userextinfo;
		header::serialize(sizeof(*this),index,CM_GETUSEREXTINFO);
	}
	else
	{
		header::serialize(sizeof(*this)-sizeof(tagUserExtInfo),index,CM_GETUSEREXTINFO);
	}
}

inline 
void tagReqAddMoneyTotal::serialize(int index,int cmd,int currpage,int pagesize,const char * strwhere,const char * atype,const char * bdate,const char * edate, const char * searchtype)
{
	this->CurrPage=currpage;
	this->PageSize=pagesize;
	strcpy(this->StrWhere,strwhere);
	strcpy(this->AccountType,atype);
	strcpy(this->BeginDate,bdate);
	strcpy(this->EndDate,edate);
	strcpy(this->SearchType,searchtype);
	
	header::serialize(sizeof(*this),index,cmd);
}

inline 
void tagRpsAddMoneyTotal::serialize(int index,int cmd,char bsucceed,int bufflen)
{
	this->bSucceed=bsucceed;
	this->BufLen=bufflen;
	header::serialize(sizeof(*this)+bufflen,index,cmd);
}

inline 
void tagReqGetUserByIDTotal::serialize(int index,int fromid,const char* d1,const char* d2)
{
	this->FromAccountID=fromid;
	strcpy(this->BeginDate,d1);
	strcpy(this->EndDate,d2);
	header::serialize(sizeof(*this),index,CM_USERBYPIDTOTAL);
}

inline 
void tagReqGetRecordCount::serialize(int index,const char * strsql)
{
	strcpy(this->SqlStr,strsql);
	header::serialize(sizeof(*this),index,CM_GETRECORDCOUNT);
}

inline 
void tagRpsGetRecordCount::serialize(int index,char bsucceed,int reccount)
{
	this->bSucceed=bsucceed;
	this->RecCount=reccount;
	header::serialize(sizeof(*this),index,CM_GETRECORDCOUNT);
}


inline 
void tagReqGetLogInfo::serialize(int index,int PageIndex, int PageSize, const char * StrWhere, const char * BeginDate, const char * EndDate)
{
	this->PageIndex = PageIndex;
	this->PageSize = PageSize;
	strcpy(this->StrWhere, StrWhere);
	strcpy(this->BeginDate, BeginDate);
	strcpy(this->EndDate, EndDate);
	header::serialize(sizeof(*this),index,CM_GETLOGINFO);
}

inline 
void tagRpsGetLogInfo::serialize(int index,char bsucceed,int bufflen)
{
	this->bSucceed=bsucceed;
	this->BufLen=bufflen;
	header::serialize(sizeof(*this)+bufflen,index,CM_RPSLOGINFO);
}

inline 
void tagReqGetLogOnline::serialize(int index,int PageIndex, int PageSize, const char * StrWhere, const char * BeginDate, const char * EndDate)
{
	this->PageIndex = PageIndex;
	this->PageSize = PageSize;
	strcpy(this->StrWhere, StrWhere);
	strcpy(this->BeginDate, BeginDate);
	strcpy(this->EndDate, EndDate);
	header::serialize(sizeof(*this),index,CM_GETLOGONLINE);
}

inline 
void tagReqGetLogOnlineEx::serialize(int index,int PageIndex, int PageSize, const char * StrWhere, int accounttype, const char * BeginDate, const char * EndDate)
{
	memset((char *)this, 0, sizeof(*this));
	this->PageIndex = PageIndex;
	this->PageSize = PageSize;
	strcpy(this->StrWhere, StrWhere);
	this->accounttype = accounttype;
	strcpy(this->BeginDate, BeginDate);
	strcpy(this->EndDate, EndDate);
	header::serialize(sizeof(*this),index,CM_GETLOGONLINEEX);
}

inline 
void tagRpsGetLogOnline::serialize(int index,char bsucceed,int bufflen)
{
	this->bSucceed=bsucceed;
	this->BufLen=bufflen;
	header::serialize(sizeof(*this)+bufflen,index,CM_RPSLOGONLINE);
}

inline 
void tagRpsGetLogOnlineEx::serialize(int index,char bsucceed,int bufflen)
{
	this->bSucceed=bsucceed;
	this->BufLen=bufflen;
	header::serialize(sizeof(*this)+bufflen,index,CM_RPSLOGONLINEEX);
}

 
inline 
void tagReqGetGodianLog::serialize(int index,int PageIndex, int PageSize, const char * StrWhere, 
								   int  accounttype, const char * BeginDate, const char * EndDate)
{
	memset((char *)this, 0, sizeof(*this));
	this->PageIndex = PageIndex;
	this->PageSize = PageSize;
	strcpy(this->StrWhere, StrWhere);
	this->accounttype = accounttype;
	strcpy(this->BeginDate, BeginDate);
	strcpy(this->EndDate, EndDate);
	header::serialize(sizeof(*this),index,CM_GETGODIANLOG);
}

inline 
void tagRpsGetGodianLog::serialize(int index,char bsucceed,int bufflen)
{
	this->bSucceed=bsucceed;
	this->BufLen=bufflen;
	header::serialize(sizeof(*this)+bufflen,index,CM_RPSGODIANLOG);
}

inline 
void tagReqGetLogPerf::serialize(int index,int PageIndex, int PageSize, const char * StrWhere, const char * BeginDate, const char * EndDate)
{
	this->PageIndex = PageIndex;
	this->PageSize = PageSize;
	strcpy(this->StrWhere, StrWhere);
	strcpy(this->BeginDate, BeginDate);
	strcpy(this->EndDate, EndDate);
	header::serialize(sizeof(*this),index,CM_GETLOGPERF);
}

inline 
void tagRpsGetLogPerf::serialize(int index,char bsucceed,int bufflen)
{
	this->bSucceed=bsucceed;
	this->BufLen=bufflen;
	header::serialize(sizeof(*this)+bufflen,index,CM_RPSLOGPERF);
}

inline
void tagReqGetResultByProc::serialize(int index,const char * procname,const char * p1,const char * p2,const char * p3,const char * p4,const char * p5)
{
	strcpy(this->procname, procname);
	strcpy(this->p1, p1);
	strcpy(this->p2, p2);
	strcpy(this->p3, p3);
	strcpy(this->p4, p4);
	strcpy(this->p5, p5);
	header::serialize(sizeof(*this),index,CM_GETRESULTBYPROC);
}

inline 
void tagRpsGetResultByProc::serialize(int index, char bsucceed, const char * result)
{
	this->bsucceed=bsucceed;
	strcpy(this->result, result);
	header::serialize(sizeof(*this),index,CM_RSPGETRESULTBYPROC);
}

#endif

