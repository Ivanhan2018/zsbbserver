#ifndef __um_web_client_H__
#define __um_web_client_H__

#include "um_web.h"

#if defined (WIN32)
#	define UMWEBCLIENT __stdcall
#else
#	define UMWEBCLIENT
#endif

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

enum enSpType
{
	stAddMoney=1,		//sp_pgetCZlist
	stAddMoneyEx=2,		//sp_pgetCZ2list
	stAddMoneyThird=3,  //sp_pgetCZdetaillist
	stPlayerList=4,		//sp_pgetplayerlist
	stUserList=5,        //sp_pgetuserlist
	stUserList3=6,        //sp_pgetuserlist3
	stPlayerFriendList=7  //sp_pgetplayerfriendlist
};

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语

//启动WebClient服务，只做一次
//参数说明：ip指定WebServer的地址，port端口,timeout 超时单位为ms
int UMWEBCLIENT StartWebClient(const char * ip,short port,int timeout,int reconntime); //reconntime<=0不重连 

//停止WebClient服务，与StartWebClient成对使用
int UMWEBCLIENT StopWebClient();

//获取与WebServer网络联接状态:1 连接 0断开
int UMWEBCLIENT NetState();     

//检测账户名称是否可注册
int UMWEBCLIENT CheckUserName(const char * UserName);

//注册用户
int UMWEBCLIENT RegUser(const char* username,const char* pwd,const char * ip,const char* em,
						const int fromAccountID,const char * fromurl, const char * qq, const char * memo, int accounttype);

//用户登录，成功返回用户ID
int UMWEBCLIENT UserLogin(const char * UserName,const char * password,const char * ip);

//更改用户密码
int UMWEBCLIENT UpdatePassword(const char * UserName,const char * OldPassword,const char * NewPassword);

//更改用户密码(不验证)
int UMWEBCLIENT UpdatePasswordNoCheck(const char * UserName,const char * NewPassword);

//更改用户密码--带question及answer
int UMWEBCLIENT UpdatePasswordExt(const char * UserName,const char * OldPassword,const char * NewPassword,const char * ques,\
								  const char * ans);

//充值
//int UMWEBCLIENT AddMoney(const char * UserName,int money,int score);
int UMWEBCLIENT AddMoney(const char * UserName,float money,int godian,int score,int addtype);

//获取用户答案
int UMWEBCLIENT GetAnswer(const char * UserName,char * Answer);

//获取用户邮件
int UMWEBCLIENT GetEmail(const char * UserName,char * Email);

//更改用户Email地址
int UMWEBCLIENT UpdateEmail(const char * UserName,const char * OldEmail,const char * NewEmail);

//扣钱
int UMWEBCLIENT SubGoDian(const int AccountID,const int GoDian, const char * desc);

//查询GO点
int UMWEBCLIENT GetGoDian(const int AccountID,int& GoDian);

//获取积分
int UMWEBCLIENT GetScore(const int AccountID,int& score);

//扣积积分
int UMWEBCLIENT SubScore(const int AccountID,const int score);

//获取个排行 
int UMWEBCLIENT GetPlayerListCs(const int maxrows, int* rows, tagPlayerInfo** ppPlayerInfo);

//获工会排行
int UMWEBCLIENT GetGongHuiListCs(const int maxrows, int* rows, tagGongHuiInfo** ppGongHuiInfo);

//获取GM账户列表
int UMWEBCLIENT GetGmUserList(char*& buf,int& len);

//清除内存
void UMWEBCLIENT DeleteBuf(char *& buf);

//获取用户扩展信息
int UMWEBCLIENT GetUserExtInfo(const int AccountID,char * FactName,char * NameID,char * Born,char * TelCode,char * Sex,\
							   char * Address,char * PostCode,char * FromAccountName);

//更改用户扩展信息
int UMWEBCLIENT UpdateUserExtInfo(const int AccountID,const char * FactName,const char * NameID,const char * Born,\
								  const char * TelCode,const char * Sex,const char * Address,const char * PostCode,const char * FromAccountName);

//充值统计
int UMWEBCLIENT DataTotal(int spType,int CurrPage,int PageSize,const char * StrWhere,const char * AccountType,const char * BeginDate,\
							  const char * EndDate,const char * SearchType, int* rows, char** ppinfo);

//sp_pgetuserbypid
int UMWEBCLIENT GetUserByIDTotal(int FromAccountID,const char * BeginDate,const char * EndDate, tagGetUserByIDTotal* ppinfo);

int UMWEBCLIENT GetRecordCount(const char * sqlstr,int * rcount);

//获取日志中错误信息
int UMWEBCLIENT GetLogInfo(int PageIndex,int PageSize , const char * strWhere, const char * BeginDate, const char * EndDate, int * rows, tagLogInfo ** info);

//获取日志中在线人数
int UMWEBCLIENT GetLogOnline(int PageIndex,int PageSize , const char * strWhere, const char * BeginDate, const char * EndDate, int * rows, tagLogOnline ** online);

int UMWEBCLIENT GetLogOnlineEx(int PageIndex,int PageSize , const char * strWhere, int accounttype, const char * BeginDate, const char * EndDate, int * rows, tagLogOnlineEx ** online);

int UMWEBCLIENT GetGodianLog(int PageIndex,int PageSize , const char * strWhere, int  accounttype, const char * BeginDate, const char * EndDate, int * rows, tagGodianLog ** online);

//获取日志中程序的性能
int UMWEBCLIENT GetLogPerf(int PageIndex,int PageSize , const char * strWhere, const char * BeginDate, const char * EndDate, int * rows, tagLogPerf ** perf);

int UMWEBCLIENT GetResultByProc(const char * procname,const char * p1,const char * p2,const char * p3,const char * p4,const char * p5, char ** output);

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif /*c_plusplus __cplusplus*/

#endif /* __UMWEBCLIENT__ */


